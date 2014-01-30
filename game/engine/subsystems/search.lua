-----------------------------------------------------------------------
-- Searching subsystem
-----------------------------------------------------------------------
constant("search", {})
settag(search, TAG_NAMESPACE)

new_flag("SEARCH_POWER")
new_flag("SEARCH_FREQ")
new_flag("SEARCH_HARDNESS")
new_flag("FIND_MSG_FEAT")
new_flag("FIND_MSG_OBJ")

hook.new_hook_type("ON_SEARCH")
new_flag("ON_SEARCH", true)

hook.new_hook_type("FIND_PRE")
new_flag("FIND_PRE", true)

hook.new_hook_type("FIND_POST")
new_flag("FIND_POST", true)

search.default_power = get_subsystem_param("search", "default_power") or
	function(search_type, passive)
		local power = player.intrinsic(FLAG_SEARCH_POWER)

		if player.has_intrinsic(FLAG_CONFUSED) or
			player.has_intrinsic(FLAG_HALLUCINATE)
		then
			power = power / 10
		end
		return power
	end

search.default_find_test = get_subsystem_param("search",
											   "default_find_test") or
	function(findable, power, hardness, search_type)
		return rng(hardness) < power
	end

search.default_cant_see_penalty =
	get_subsystem_param("search", "default_cant_see_penalty") or

	function(power, passive, search_type)
		return power / 10
	end

search.do_passive = get_subsystem_param("search", "do_passive") or
	function()
		local power    = flag_get (player.intrinsic_flags, FLAG_SEARCH_FREQ)
		local hardness = flag_get2(player.intrinsic_flags, FLAG_SEARCH_FREQ)

		if hardness == 0 then
			hardness = 100
		end

		return rng(hardness) < power
	end

-------------------------------------------------------------------------
-------------------------------------------------------------------------

search.default_types = {}

search.default_types["terrain"] = {
	name = "terrain",

	add_flags = {"SEARCH_POWER_FEAT"},

	can_passive = 
		function()
			return true
		end,
			
	get_power =
		function(passive)
			return search.default_power("terrain", passive) +
				player.intrinsic(FLAG_SEARCH_POWER_FEAT)
		end,

	cant_see_penalty =
		function(power, passive)
			return search.default_cant_see_penalty(power, passive, "terrain")
		end

	find_test = 
		function(findable, power, hardness)
			return search.default_find_test(findable, power, hardness,
											"terrain")
		end

	get_findables =
		function(y, x, passive, power)
			local c_ptr = cave(y, x)
			local f_ptr = f_info(c_ptr.feat)

			if f_ptr.flags[FLAG_ON_SEARCH] then
				local func
				func = get_function_registry_from_flag(f_ptr.flags,
													   FLAG_ON_SEARCH)
				func(y, x, passive, "terrain", f_ptr)
			end
			if c_ptr.flags[FLAG_ON_SEARCH] then
				local func
				func = get_function_registry_from_flag(c_ptr.flags,
													   FLAG_ON_SEARCH)
				func(y, x, passive, "terrain", f_ptr)
			end

			local thing
			if f_ptr.flags[FLAG_SECRET] then
				thing = f_ptr
			elseif tag(c_ptr.flags[FLAG_SECRET]) == TAG_NUMBER then
				thing = c_ptr
			else
				return nil
			end

			local hardness = 100 + get_flag(thing, FLAG_SEARCH_HARDNESS)
			return { {
					thing    = thing,
					source   = c_ptr,
					hardness = hardness,
					flags    = thing.flags,
			} }
		end,

	found =
		function(y, x, findable)
			local f_ptr
			local mimic

			if cave(y, x).mimic ~= 0 then
				f_ptr = f_info(cave(y, x).feat)
				mimic = f_info(cave(y, x).mimic)
			else
				f_ptr = f_info(findable.thing.flags[FLAG_SECRET])
				mimic = f_info(cave(y, x).feat)
			end

			local msg
			if findable.thing.flags[FLAG_FIND_MSG_FEAT] then
				msg = findable.thing.flags[FLAG_FIND_MSG_FEAT]
			else
				msg = "You find a @feat@."
			end

			if msg and msg ~=  "" then
				msg = gsub(msg, "@mimic@", mimic.name)
				msg = gsub(msg, "@Mimic@", strcap(mimic.name))
				msg = gsub(msg, "@feat@", f_ptr.name)
				msg = gsub(msg, "@Feat@", strcap(f_ptr.name))
				message(msg)
			end

			local into = findable.thing.flags[FLAG_SECRET]
			if tag(findable.thing) == TAG_GRID then
				findable.thing.flags[FLAG_SECRET] = nil
			end
			cave(y, x).mimic = 0
			cave_set_feat(y, x, into)
			lite_spot(y, x)

		end,
} -- features

-------------------------------------------------------------------------

function search.grab_traps(list, out, source)
	for i = 1, list.size do
		local node = list.node[i]
		local key  = node.key

		-- Traps which the player hasn't found yet
		if (node.flags & FLAG_FLAG_USED) ~= 0 and
			(node.flags & FLAG_FLAG_KNOWN) == 0 and
			list[key][FLAG_TRAP_IDX]
		then
			local trap = list[key]
			tinsert(out,
					{
						thing    = trap,
						flags    = trap,
						key      = key,
						source   = source,
						hardness = 100,
					})
		end
	end

	return out
end -- search.grab_traps()

search.default_types["traps"] = {
	name = "traps",

	add_flags = {"SEARCH_POWER_TRAP"},

	can_passive = 
		function()
			return true
		end,
			
	get_power =
		function(passive)
			return search.default_power("traps", passive) +
				player.intrinsic(FLAG_SEARCH_POWER_TRAP)
		end,

	find_test = 
		function(findable, power, hardness)
			return search.default_find_test(findable, power, hardness,
											"traps")
		end

	cant_see_penalty =
		function(power, passive)
			return search.default_cant_see_penalty(power, passive, "traps")
		end

	get_findables =
		function(y, x, passive, power)
			local list  = {}
			local c_ptr = cave(y, x)
			local f_ptr = f_info(c_ptr.feat)

			if c_ptr.flags[FLAG_ON_SEARCH] then
				local func
				func = get_function_registry_from_flag(c_ptr.flags,
													   FLAG_ON_SEARCH)
				func(y, x, passive, "traps", c_ptr)
			end
			if f_ptr.flags[FLAG_ON_SEARCH] then
				local func
				func = get_function_registry_from_flag(f_ptr.flags,
													   FLAG_ON_SEARCH)
				func(y, x, passive, "traps", c_ptr)
			end

			search.grab_traps(c_ptr.activations, list, c_ptr)

			local obj_start = getn(list) + 1
			for_inventory_inven(c_ptr.inventory,
				function(obj)
					item_hooks.process_all(obj, FLAG_ON_SEARCH, %y, %x,
										   %passive)
					local traps = obj.flags[FLAG_TRAP]
					if traps then
						search.grab_traps(traps, %list, obj)
					end
				end)

			for i = obj_start, getn(list) do
				local item = list[i]

				-- Items can make searching for traps on them harder
				item.hardness = item.hardness +
					flag_get(item.source.flags, FLAG_SEARCH_HARDNESS)
			end

			for i = 1, getn(list) do
				local item = list[i]

				-- From the trap flagset itself
				item.hardness = item.hardness +
					flag_get(item.thing, FLAG_SEARCH_HARDNESS)
			end

			randomize_list(list)

			return list
		end, -- get_findables

	found =
		function(y, x, findable)
			local traps
			local msg
			if tag(findable.source) == TAG_OBJECT then
				local obj = findable.source
				obj.flags[FLAG_TRAP_AWARE] = true
				traps = obj.flags[FLAG_TRAP]

				if obj.flags[FLAG_FIND_MSG_OBJ] then
					msg = "You found a trap on @object@!"
				else
					msg = gsub(msg, "@object@", object_desc(obj))
				end
			else
				local c_ptr = cave(y, x)
				local f_ptr = f_info(iif(c_ptr.mimic > 0,
										 c_ptr.mimic, c_ptr.feat))
				traps = c_ptr.activations

				if findable.thing[FLAG_FIND_MSG_FEAT] then
					msg = findable.thing[FLAG_FIND_MSG_FEAT]
				elseif f_ptr.flags[FLAG_FLOOR] then
					if y == player.py and x == player.px then
						msg = "You found a trap underfoot!"
					else
						msg = "You found a trap on the @feature@!"
					end
				else
					if y == player.py and x == player.px then
						msg = "You're right on top of a trap!"
					else
						msg = "You found a trap in the @feature@!"
					end
				end
				msg = gsub(msg, "@feature@", f_ptr.name)

				c_ptr.info = c_ptr.info | CAVE_TRDT
				player.redraw[FLAG_PR_MAP] = true
			end

			flag_learn(traps, findable.key, true)
			message(strcap(msg))
		end -- found
} -- traps

-------------------------------------------------------------------------

search.types = {}
search.type_list = {}

function search.add_type(t)
	assert(t.name, "Search type does not have name.")
	assert(not search.types[t.name],
		   "Search type '" .. t.name .. "' already registered.")
	assert(tag(t.can_passive) == TAG_FUNCTION,
		   "Search type '" .. t.name .. "' does no have function " ..
			   "can_passive()")
	assert(tag(t.get_power) == TAG_FUNCTION,
		   "Search type '" .. t.name .. "' does no have function " ..
			   "get_power()")
	assert(tag(t.find_test) == TAG_FUNCTION,
		   "Search type '" .. t.name .. "' does no have function " ..
			   "find_test()")
	assert(tag(t.cant_see_penalty) == TAG_FUNCTION,
		   "Search type '" .. t.name .. "' does no have function " ..
			   "cant_see_penaltya()")
	assert(tag(t.get_findables) == TAG_FUNCTION,
		   "Search type '" .. t.name .. "' does no have function " ..
			   "get_findables()")
	assert(tag(t.found) == TAG_FUNCTION,
		   "Search type '" .. t.name .. "' does no have function " ..
			   "found()")

	if t.add_flags then
		for i = 1, getn(t.add_flags) do
			safe_new_flag(t.add_flags[i])
		end
	end

	search.types[t.name] = t
	tinsert(search.type_list, t.name)
end

function search.add_default_type(name, overrides)
	local t = search.default_types[name]
	assert(t, "No such default type as '" .. name .. "'")

	table_copy(t, overrides or {})

	search.add_type(t)
end

-------------------------------------------------------------------------
-------------------------------------------------------------------------

function search.do_hook(all, flag, what, findable, y, x, passive, typ, args)
	local kind = tag(what)

	if kind == TAG_OBJECT then
		if all then 
			if item_hooks.process_all(what, flag, findable, y, x, passive,
									  typ.name, args)
			then
				return true
			end
		else
			if item_hooks.process_one(what, flag, findable, y, x, passive,
									  typ.name, args)
			then
				return true
			end
		end
	elseif (kind == TAG_MONSTER or kind == TAG_FEATURE or
			kind == TAG_GRID)
	then
		if what.flags[flag] then
			local func
			func = get_func_from_flag_or_registry(what.flags, flag)
			if func(what, findable, y, x, passive, typ.name, args) then
				return true
			end
		end
	elseif kind == TAG_FLAG then
		if what[flag] then
			local func
			func = get_func_from_flag_or_registry(what, flag)
			if func(what, findable, y, x, passive, typ.name, args) then
				return true
			end
		end
	elseif kind == TAG_TABLE then
		if tag(what[flag]) == TAG_FUNCTION then
			local func = what[flag]
			if func(what, findable, y, x, passive, typ.name, args) then
				return true
			end
		end
	elseif kind == TAG_FUNCTION then
		if what(what, findable, y, x, passive, typ.name, args) then
			return true
		end
	end

	return false
end -- search.do_hook()

function search.find_findable(findable, typ, y, x, passive, power)
	local args = {power = power, hardness = findable.hardness}

	-- Allow hooks to forbid finding it, or to change the search
	-- power or search hardness.
	if hook.process(hook.FIND_PRE, findable, y, x, passive, typ.name,
					args)
	then
		return false
	end
	if item_hooks.process_one_standard(FLAG_FIND_PRE, findable, y, x,
									   passive, typ.name, args)
	then
		return false
	end
	if search.do_hook(false, FLAG_FIND_PRE, findable.thing, findable, y, x,
					  passive, typ, args)
	then
		return false
	end
	if search.do_hook(false, FLAG_FIND_PRE, findable.source, findable, y, x,
					  passive, typ, args)
	then
		return false
	end

	-- Can we find it?
	if not typ.find_test(findable, args.power, args.hardness) then
		return false
	end

	-- Found it!
	typ.found(y, x, findable)
	hook.process(hook.FIND_POST, findable, y, x, passive, typ.name)
	item_hooks.process_one_standard(FLAG_FIND_POST, findable, y, x,
									   passive, typ.name)
	search.do_hook(true, FLAG_FIND_POST, findable.thing, findable, y, x,
					  passive, typ)
	search.do_hook(true, FLAG_FIND_POST, findable.source, findable, y, x,
					  passive, typ)

	term.disturb(0, 0)
	return true
end -- search.find_findable()

search.one_find_per_search =
	get_subsystem_param("search", "one_find_per_search")

search.one_find_per_square =
	get_subsystem_param("search", "one_find_per_square")

search.one_find_per_type =
	get_subsystem_param("search", "one_find_per_type")

function search.search_square(y, x, passive, search_types)
	local found = 0
	for i = 1, getn(search_types) do
		local typ   = search.types[search_types[i]]
		local power = typ.get_power(passive)

		if player.has_intrinsic(FLAG_BLIND) or not
			player_can_see_bold(y, x)
		then
			power = typ.cant_see_penalty(power)
		end

		local findables = typ.get_findables(y, x, passive, power)
		findables = findables or {}

		for j = 1, getn(findables) do
			if search.find_findable(findables[j], typ, y, x,
									passive, power)
			then
				found = found + 1

				if search.one_find_per_search or search.one_find_per_square
				then
					return found
				elseif search.one_find_per_type then
					break
				end
			end -- if found > 0 then
		end -- for j = 1, getn(findables) do
	end -- for i = 1, getn(search_types) do
	return found
end

function search.exec(passive, search_types, energy)
	search_types = search_types or search.type_list

	if not energy then
		if passive then
			energy = 0
		else
			energy = get_player_energy(SPEED_SEARCH)
		end
	end
	energy_use = energy_use + energy

	local found = 0
	for y = player.py - 1, player.py + 1 do
		for x = player.px - 1, player.px + 1 do
			found = found + search.search_square(y, x, passive, search_types)

			if found > 0 and search.one_find_per_search then
				--handle_stuff()
				return found
			end
		end
	end

	return found
end -- search.do_search()
				
hook(hook.MOVE_POST,
function(oy, ox, dir, do_pickup, run, disarm)
	if search.do_passive() then
		search.exec(true)
	end
end)

hook(hook.STAY_POST,
function(oy, ox, dir, do_pickup, run, disarm)
	if search.do_passive() then
		search.exec(true)
	end
end)

if not get_subsystem_param("search", "no_key_bind") then
	hook(hook.KEYPRESS, function (key)
		if key == strbyte('s') then search.exec() return true end
	end)
end
