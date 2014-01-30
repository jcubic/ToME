--
-- Damage handling subsystem
--

dam.res = {}
dam.aux = {}

function dam.aux.state_copy(state)
	local nstate = {}
	for k, e in state do nstate[k] = e end
	return nstate
end

function dam.res.stack(list)
	local tmp = 100
	for i=1,getn(list) do tmp = tmp*(100-list[i])/100 end
	return 100 - tmp
end

function dam.res.average(list)
	assert(getn(list) > 0, "Resistance list is empty!")
	local tmp = 0
	for i=1,getn(list) do tmp = tmp + list[i] end
	return tmp/getn(list)
end

function dam.res.get(state, damtype, oldres, target)
	if not oldres then oldres = 0 end
	local r = dam.get_resist(damtype, target, state)
	-- must check for obvious and update memory
	if state.m_ptr then
		-- monster memory
		if r ~= 0 and state.seen then memory.add(RT_RESIST, damtype, state.m_ptr, r) end
	end
	return dam.res.stack{r,oldres}
end

function dam.checkflag(state, flag)
	if state.m_ptr and has_flag(state.m_ptr, flag) then
		if state.seen then memory.add(RT_MISC,flag,state.m_ptr) end
		return true
	else return nil end
end

function dam.combret_m(a,b)
	local res = {}
	for i,v in { "stun", "conf", "fear", "dist", "pois", "cut", "poly", "msg", "msg_die", "msg_heal" } do
		res[v] = a[v] or b[v]
	end
	for i,v in { "anger" } do
		res[v] = a[v] + b[v]
	end
	res.dam = a.dam + b.dam
	res.resist = dam.res.average{a.resist, b.resist}
	return res
end

function dam.combret_g(a,b)
	local res = {}
	for i,v in { "blind" } do
		res[v] = a[v] or b[v]
	end

	res.dam = a.dam + b.dam
	res.resist = dam.res.average{a.resist, b.resist}
	return res
end

function dam.combret_o(a,b)
	local res = {}
	for i,v in { "kill", "msg_die", "msg_heal" } do
		res[v] = a[v] or b[v]
	end
	res.dam = a.dam + b.dam
	res.resist = dam.res.average{a.resist, b.resist}
	res.note_kill = a.note_kill
	return res
end

function dam.combret_p(a,b)
	local res = {}
	for i,v in { "move", "fuzzy" } do
		res[v] = a[v] or b[v]
	end
	res.dam = a.dam + b.dam
	res.resist = dam.res.average{a.resist, b.resist}
	return res
end

function dam.combine_helper(t, stack, div, combfunc, callfunc)
	return function(state)
		local t = %t
		local div = %div
		local combfunc = %combfunc
		local callfunc = %callfunc
		local stack = %stack
		-- backwards iteration: the first damage type should override others
		local tstate = dam.aux.state_copy(state)
		tstate.dam = state.dam*t[getn(t)][2]/div
		local r = dam.call(t[getn(t)][1], callfunc, tstate, stack)
		for i = getn(t) - 1, 1, -1 do
			tstate.dam = state.dam*t[i][2]/div
			r = combfunc(dam.call(t[i][1], callfunc, tstate, stack),r)
		end
		return r
	end
end

-- produces a new damage type as a weighed sum of others
-- ie: { {dam.FIRE, 100}, {dam.COLD,50} }
-- the weights do not have to add up to 100, in that case
-- the total damage will be more/less than expected
function dam.combine(types, stack, normalize, desc, force_index,
					 color, text_color, color_desc)
	local t = {}
	local div = 0
	local colors = {}
	for i,v in types do
		if type(v) == "table" then
			div = div + v[2]
			tinsert(t,v)
			tinsert(colors,dam.raw[v[1]].color)
			text_color = text_color or dam.raw[v[1]].text_color
			color_desc = color_desc or dam.raw[v[1]].color_desc
		else
			tinsert(t,{ v, 100 })
			div = div + 100
			tinsert(colors,dam.raw[v].color)
			text_color = text_color or dam.raw[v].text_color
			color_desc = color_desc or dam.raw[v].color_desc
		end
	end
	if not normalize or normalize == FALSE then div = 100 end

	return add_damage_type
	{
		index      = force_index
		color      = color or
			function() local c = %colors return c[rng(getn(c))]() end,
		text_color = text_color or dam.raw[dam.DEFAULT].text_color,
		color_desc = color_desc
		gfx	       = dam.raw[t[1][1]].gfx,
		desc       = iif(desc, desc, dam.raw[t[1][1]].desc),
		monster	   = dam.combine_helper(t, stack, div,
										dam.combret_m, "monster"),
		grid       = dam.combine_helper(t, stack, div, dam.combret_g, "grid"),
		object     = dam.combine_helper(t, stack, div,
										dam.combret_o, "object"),
		player     = dam.combine_helper(t, stack, div,
										dam.combret_p, "player"),
	}
end

function dam.derive_helper(base, modifier, target)
	return function(state)
		-- Modify the state if needed
		%modifier(%target, state)
		-- And then call the base damage with the modified state
		return dam.raw[%base][%target](state)
	end
end

function dam.derive(t)
	local base, desc, modifier = t.source, t.desc, t.modifier
	return add_damage_type
	{
		color      = dam.raw[base].color,
		text_color = dam.raw[base].text_color,
		color_desc = dam.raw[base].color_desc
		gfx        = dam.raw[base].gfx,
		desc       = iif(desc, desc, dam.raw[base].desc),
		monster    = dam.derive_helper(base, modifier, "monster"),
		grid       = dam.derive_helper(base, modifier, "grid"),
		object     = dam.derive_helper(base, modifier, "object"),
		player     = dam.derive_helper(base, modifier, "player"),
	}
end

-- we need to pass on a resist to the function we derive from
-- we therefore stack the resists manually and call using raw
-- instead of call
function dam.implicit_flag_helper(base, types, div, category)
	return function(state)
		local r = state.resist
		for i, v in %types do
			if dam.checkflag(state, i) then r = dam.stack{r, v} end
		end
		r = dam.res.get(state, %base, r, %category)
		local s = state
		s.dam, s.resist = dam.apply_resistance(s.dam, r)
		return dam.raw[%base][%category](s)
	end
end

function dam.implicit_flag(base, types, desc)
	local div = 100

	return add_damage_type
	{
		color      = dam.raw[base].color,
		text_color = dam.raw[base].text_color,
		color_desc = dam.raw[base].color_desc
		gfx        = dam.raw[base].gfx,
		desc       = iif(desc, desc, dam.raw[base].desc),
		monster    = dam.implicit_flag_helper(base, types, div, "monster"),
		grid       = dam.implicit_flag_helper(base, types, div, "grid"),
		object     = dam.implicit_flag_helper(base, types, div, "object"),
		player     = dam.implicit_flag_helper(base, types, div, "player"),
	}
end

function dam.implicit_resist_helper(base, types, category)
	return function(state)
		local types = %types
		local r = state.resist
		local s = state
		for i = 1, getn(types) do
			r = dam.res.get(s, types[i], r, %category)
		end
		r = dam.res.get(s, %base, r, %category)
		s.dam, s.resist = dam.apply_resistance(s.dam, r)
		return dam.raw[%base][%category](s)
	end
end

function dam.implicit_resist(base, types, desc)
	return add_damage_type
	{
		color      = dam.raw[base].color,
		text_color = dam.raw[base].text_color,
		color_desc = dam.raw[base].color_desc
		gfx        = dam.raw[base].gfx,
		desc       = iif(desc, desc, dam.raw[base].desc),
		monster    = dam.implicit_resist_helper(base, types, "monster"),
		grid       = dam.implicit_resist_helper(base, types, "grid"),
		object     = dam.implicit_resist_helper(base, types, "object"),
		player     = dam.implicit_resist_helper(base, types, "player"),
	}
end

dam.msg = {}
dam.msg.div = { -1, 0, 60, 85, 99, 100 }

-- makes a function that returns battle messages. strings is a list
-- of the form { hurt, normal, lres, res, hres, immune }
function dam.msg.make(strings)
	return function(r)
		local i = 1
		while r > dam.msg.div[i] do i = i+1 end
		return %strings[i]
	end
end

dam.msg.normal = dam.msg.make{
	" is hit hard.",
	nil,
	" resists a bit.",
	" resists.",
	" resists a lot.",
	" is immune!"
}
dam.msg.normal_die = function(r) return " dies." end
dam.msg.normal_heal = function(r) return " looks healthier." end

-- @fn
-- @brief Reserves a damage type index for latter
-- @return index Number the damage type index
function dam.reserve()
	dam.max_type = dam.max_type + 1
	return dam.max_type - 1
end


-- @fn
-- @brief makes a new damage type
-- @param list Table a table containing all the parameters(FILL ME)
-- @return index Number the new damage type index
function dam.add(list)
	local base = dam.DEFAULT
	local res = table_copy(list)

	res.index = list.index

	if not list.color then res.color = dam.raw[base].color end
	if not list.text_color then res.color = dam.raw[base].text_color end
	if not list.gfx then res.gfx = dam.raw[base].gfx end

	-- set defaults, so that we don't have to
	res.color, res.gfx = list.color, list.gfx
	if not list.monster then res.monster = dam.raw[base].monster
	else res.monster = function(state)
		local b = {
			dam=state.dam, stun=0,
			fear=0, conf=0, dist=0,
			pois=0, cut=0, poly=0,
			anger=-200,
			resist=state.resist, msg=dam.msg.normal, msg_die=dam.msg.normal_die, msg_heal=dam.msg.normal_heal,
		}
		local a = %list.monster(state)
		for i,v in a or {} do b[i] = v end
		return b
	end end

	if not list.grid then res.grid = dam.raw[base].grid
	else res.grid = function(state)
		local b = {
			dam = state.dam,
			resist = state.resist,
		}
		local a = %list.grid(state)
		for i,v in a or {} do b[i] = v end
		return b
	end end

	if not list.desc then res.desc = dam.raw[base].desc
	else res.desc = list.desc end

	if not list.object then res.object = dam.raw[base].object
	else res.object = function(state)
		local b = {
			kill=FALSE, note_kill=" @isare@ destroyed!",
		}
		local a = %list.object(state)
		for i,v in a or {} do b[i] = v end
		return b
	end end

	if not list.player then res.player = dam.raw[base].player
	else res.player = function(state)
		local b = {
			dam = state.dam,
			resist = state.resist,
		}
		local a = %list.player(state)
		for i,v in a or {} do b[i] = v end
		return b
	end end

	return add_damage_type(res)
end

-- @fn
-- @brief Applies the given absorb to a damage
-- @param dam Number the damage
-- @param absorb Number the absorb
-- @return dam Number the recomputed damage
function dam.apply_absorb(dam, absorb)
	-- Apply the percent, note that in case of negative resistance
	-- it will increase the damage done
	dam = dam - absorb
	if dam < 0 then dam = 0 end
	return dam
end

-- @fn
-- @brief Gets the damage absorb for this target and this state
-- @param type Number the damage type
-- @param target String the target to use(monster,object,grid,player)
-- @param state Table the current state
-- @return absorb Number the absorb of the target
function dam.get_absorb(type, target, state)
	if target == "player" then
		return player.absorbs[type] or 0
	elseif target == "monster" then
		return state.m_ptr.flags[FLAG_ABSORB][type] or 0
	else
		-- Nothing else possess absorb
		return 0
	end
end

-- @fn
-- @brief Applies the given resist percent to a damage
-- @param dam Number the damage
-- @param resist Number the resistance
-- @return dam Number the recomputed damage
-- @return resist Number the recomputed resistance(in case it was out of bounds)
function dam.apply_resistance(dam, resist, bypass_max)
	if not bypass_max then
		if resist > 100 then resist = 100
		elseif resist < -100 then resist = -100 end
	end

	-- Apply the percent, note that in case of negative resistance
	-- it will increase the damage done
	dam = dam - ((dam * resist) / 100)
	return dam, resist
end

-- @fn
-- @brief Gets the damage resistance for this target and this state
-- @param type Number the damage type
-- @param target String the target to use(monster,object,grid,player)
-- @param state Table the current state
-- @return resist Number the resistance of the target
function dam.get_resist(type, target, state)
	if target == "player" then
		return player.resists[type] or 0
	elseif target == "monster" then
		local r = state.m_ptr.flags[FLAG_RESIST][type]
		local seen = state.m_ptr.ml and 
			((state.who ~= WHO_FLOOR) and (state.who ~= WHO_DUNGEON))

		if seen and r and r ~= 0 then
			local res_mem = deep_get{memory.get_entry(race_info(state.m_ptr),state.m_ptr.ego),RT_RESIST}
			res_mem[type] = r
		end

		return r or 0
	else
		-- Nothing else posses resistances
		return 0
	end
end

-- @fn
-- @brief Call the given damage at a target with an execution state
-- this function should usualy not be called directly but instead use
-- the project() function, unles it is used to define new damage types.
-- @param type Number the damage type to apply
-- @param target String the target to use(grid,monster,object,player)
-- @param state Table the state contains various info about the damage, like
-- the x, y damage power, resistance, ...
-- @param stack Boolean should the resistance for this damage type stack
-- with the reistance of the caller's state?
-- @return state Table the result state as given by the damage execution
--
function dam.call(type, target, state, stack)
	state.dam_type = type
	
	-- Grab the resist if the caller hasn't already gone to the
	-- trouble of decided a resistance
	if stack then
		state.resist = dam.res.stack{state.resist, dam.get_resist(type, target, state)}
	else
		state.resist = dam.get_resist(type, target, state)
	end

	state.absorb = dam.get_absorb(type, target, state)

	-- Apply absorb before resists
	state.dam = dam.apply_absorb(state.dam, state.absorb)

	-- Apply the percent, note that in case of negative resistance
	-- it will increase the damage done
	state.dam, state.resist = dam.apply_resistance(state.dam, state.resist)

	local ret = dam.raw[type][target](state)

	if ret then
		for k, v in state do
			ret[k] = ret[k] or state[k]
		end
	end

	return ret
end

function dam.add_msg(state, msg)
	if not state.msg then
		state.msg = msg
	else
		state.msg = state.msg .. " " .. msg
	end
end

function dam.aux.boring_grid(y,x)
	return cave_feat_is(cave(y,x),FLAG_FLOOR) and not cave_feat_is(cave(y,x),FLAG_REMEMBER)
end

function dam.destroy_items(typ, chance)
	if player.intrinsic_flags[FLAG_PROTECT_FROM][typ] then return end

	for inven_idx = INVEN_INVEN, INVEN_TOTAL - 1 do
		local inven = player.inventory[inven_idx]
		local max = flag_max_key(inven)
		for item_idx = max, 1, -1 do
			local obj = inven[item_idx]

			if obj and obj.flags[FLAG_HARMED_BY] and obj.flags[FLAG_HARMED_BY][typ] and
			   (not obj.flags[FLAG_IGNORE] or not obj.flags[FLAG_IGNORE][typ]) then
				local amt = 0
				
				-- Count casualties
				for i = 1, obj.number do if rng.percent(chance) then amt = amt + 1 end end
				
				if amt > 0 then
					local item = compute_slot(inven_idx, item_idx)
				
					message(format("%sour %s %s destroyed!",
					           iif(obj.number > 1,
						   iif(amt == obj.number, "All of y",
					             iif(amt > 1, "Some of y", "One of y")), "Y"),
							           object_desc(obj, 0, 3),
							           iif(amt > 1, "were", "was")))

					item_increase(item, -amt)
					item_optimize(item)
				end
			end
		end
	end
end

dam.DEFAULT = add_damage_type
{
	color      = function() return color.WHITE end,
	text_color = "#w",
	gfx        = 0,
	desc       = "something unspecified",
	monster	   = function(state)
		return {
			dam=state.dam, stun=0,
			fear=0, conf=0, dist=0,
			pois=0, cut=0, poly=0,
			anger=-200,
			resist=state.resist, msg=dam.msg.normal, msg_die=dam.msg.normal_die, msg_heal=dam.msg.normal_heal,
			}
	end,
	grid	= function(state)
		return { dam=state.dam }
	end,
	object	= function(state)
		if rng.percent(-state.resist) then
			return { kill = true, note_kill=" @isare@ destroyed!" }
		else return { kill = FALSE } end
	end,
	player	= function(state)
		return { dam=state.dam }
	end,
}
