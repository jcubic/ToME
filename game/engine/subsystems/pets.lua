--
-- Pets AI and commands subsytem
--

constant("pets", {})
settag(pets, TAG_NAMESPACE)

new_flag("PET_TYPE")
new_flag("PET_ORDERS")
new_flag("PET_ESCORT_DIST")
new_flag("PET_TARGET")
new_flag("PET_USING_DEFAULTS")
safe_new_flag("ORIGINAL_FLAGS")

hook.new_hook_type("PET_COMMAND_PRE")
new_flag("PET_COMMAND_PRE")

hook.new_hook_type("PET_COMMAND_POST")
new_flag("PET_COMMAND_POST")

---------------------------------------------------------------------------

pets.is_pet = get_subsystem_param("pets", "is_pet") or
function(monst)
	return (monst.r_idx ~= 0) and (monst.faction == player.faction)
end

-- Change this if you want the player to, for example, not be
-- aware of pets that aren't visible.
pets.is_aware = get_subsystem_param("pets", "is_aware") or
function(monst)
	return true
end

-- Change this if, for example, you want the player to be unable
-- to command pets which are out of line-of-sight.
pets.can_command = get_subsystem_param("pets", "can_command") or
function(monst)
	return true
end

pets.pet_types = get_subsystem_param("pets", "pet_types") or
{
	companion = true,
	ally      = true,
}

pets.get_pet_type = get_subsystem_param("pets", "get_pet_type") or
function(monst)
	if monst.flags[FLAG_PET_TYPE] then
		return monst.flags[FLAG_PET_TYPE]
	end

	if monst.flags[FLAG_PERMANENT] then
		return "companion"
	else
		return "ally"
	end
end

function pets.add_pet_type(type_name)
	assert(not pets.pet_types[type_name], "Pet type '" .. type_name ..
		   "' already exists.")
	pets.pet_types[type_name] = true
end

function pets.get_all_of_type(list, typ)
	local out_list = {}

	if not typ then
		out_list = list
	else
		for i = 1, getn(list) do
			local monst = list[i]

			if pets.get_pet_type(monst) == typ then
				tinsert(out_list, monst)
			end
		end
	end

	return out_list
end

pets.max_escort_dist =
	get_subsystem_param("pets", "max_escort_dist") or 40

-------------------------------------------------------------------------
-- Targetting
-------------------------------------------------------------------------

add_loadsave("pets.target_list", {})

pets.acquire_target =
	get_subsystem_param("pets", "acquire_target") or
	function(pet, target)
		local who_targets = pets.target_list[target]

		if not who_targets then
			who_targets              = {}
			pets.target_list[target] = who_targets
		end
		who_targets[pet] = true
	end

pets.lose_target =
	get_subsystem_param("pets", "lose_target") or
	function(pet, target)
		local who_targets = pets.target_list[target]

		if not who_targets then
			return
		end
		who_targets[pet] = nil
	end

pets.on_target_gone     = function() end
pets.inform_target_gone = function() end

hook(hook.MONSTER_REMOVED,
function(m_idx, monst)
	local who_targets = pets.target_list[m_idx]

	if who_targets then
		pets.on_target_gone(m_idx, who_targets)
		pets.target_list[m_idx] = nil
	end
end)

pets.can_target = get_subsystem_param("pets", "can_target") or
	function(monst)
		if factions.get_friendliness(player.faction, monst.faction) >= 0 then
			message("You can only target hostile monsters.")
			return false
		end

		local no_target = monst.flags[FLAG_NO_TARGET] 

		if no_target then
			if tag(no_target) == TAG_STRING then
				message(no_target)
			else
				message("You cannot target that monster.")
			end
			return false
		end

		return true
	end

-------------------------------------------------------------------------
-- AI
-------------------------------------------------------------------------

ai.new
{
	name  = "PET"
	state =
	{
		PET_NEXT_AI = ai.NONE
	}
	init  = function(monst, state)
	end, -- init

	exec  = function(m_idx, monst, state)
		local target = state[FLAG_PET_TARGET]
		if target then
			local target_mon = monster(target)
			if target_mon.r_idx == 0 then
				-- Target is dead
				pets.on_target_gone(target, pets.target_list[target])
			else
				-- Only use PET_TARGET if there isn't a hostile 
				-- monster adjacent to us.
				if not ai.adjacent(monst, target_mon.fy, target_mon.fx) then
					monst.target = target
				end
			end
		end

		-- First, see what the monster prefers to do, then modify
		-- it before the engine acts on it.
		ai.exec(flag_get(state, FLAG_PET_NEXT_AI), m_idx, monst)

		local dist = state[FLAG_PET_ESCORT_DIST]
		if dist > 0 then
			if dist < monst.cdis and monst.ai_action ~= ai.action.OTHER then
				-- Too far away, move closer
				monst.ai_action = ai.action.MOVE
				monst.ai_move_y = player.py
				monst.ai_move_x = player.px
			elseif dist == monst.cdis and
				(monst.ai_action == ai.action.MOVE or
				 monst.ai_action == ai.action.MOVE_DIR)
			then
				-- Exactly at the edge, can we move?  We can "move" outside
				-- of the max escort distance if it's to attack an
				-- adjacent monster.
				local y, x

				if monst.ai_action == ai.action.MOVE then
					y = monst.ai_move_y
					x = monst.ai_move_x
				else
					y = monst.ai_move_y - monst.fy
					x = monst.ai_move_x - monst.fx
				end

				if player.py == y and player.px == x then
					monst.ai_action = ai.action.REST
				elseif distance(y, x, player.py, player.px > dist) and
					not ai.adjacent(monst, y, x)
				then
					monst.ai_action = ai.action.REST
				end
			end -- elseif dist == monst.cdis and ...
		end -- if dist > 0 then
	end -- exec
}
pets.default_ai = get_subsystem_param("pets", "default_ai") or ai.PET

-------------------------------------------------------------------------
-- Autoamtically update monster AIs
-------------------------------------------------------------------------
function pets.default_set_pet_ai(monst)
	assert(not monst.flags[FLAG_ORIGINAL_FLAGS],
		   monster_desc(monst, 256 | 128) .. " already has ORIGINAL_FLAGS")

	monst.flags[FLAG_PET_ORDERS] = flag_new(2)

	local original = flag_new(2)
	monst.flags[FLAG_ORIGINAL_FLAGS] = original

	original[FLAG_AI]        = monst.ai
	original[FLAG_OPEN_DOOR] = monst.flags[FLAG_OPEN_DOOR]
	original[FLAG_BASH_DOOR] = monst.flags[FLAG_BASH_DOOR]
	original[FLAG_TAKE_ITEM] = monst.flags[FLAG_TAKE_ITEM]

	local state = monst.ai_states
	if not state[FLAG_MOVE_AI] then
		state[FLAG_MOVE_AI] = ai.NONE
	end
	if not state[FLAG_CASTING_AI] then
		state[FLAG_CASTING_AI] = ai.NONE
	end
	original[FLAG_AI_STATES] = flag_dup(state)

	-- Start out not escorting
	state[FLAG_PET_ESCORT_DIST] = 0

	state[FLAG_PET_NEXT_AI] = monst.ai
	monst.ai                = pets.default_ai

	-- Issue default commands if possible.
	if pets.can_command(monst) then
		pets.reset_to_defaults({monst})
	end
end -- pets.default_set_pet_ai()

function pets.default_remove_pet_ai(monst)
	assert(monst.flags[FLAG_ORIGINAL_FLAGS],
		   monster_desc(monst, 256 | 128) .. " does not have ORIGINAL_FLAGS")

	pets.remove_all_orders(monst)

	local original = monst.flags[FLAG_ORIGINAL_FLAGS]

	flag_copy(monst.ai_states, original[FLAG_AI_STATES])
	original[FLAG_AI_STATES] = nil

	monst.ai = original[FLAG_AI]

	flag_add(monst.flags, monst.flags[FLAG_ORIGINAL_FLAGS])

	monst.flags[FLAG_ORIGINAL_FLAGS] = nil
	monst.flags[FLAG_PET_TYPE]       = nil
	monst.flags[FLAG_PET_ORDERS]     = nil
end -- pets.default_set_pet_ai()

pets.set_pet_ai = get_subsystem_param("pets", "set_pet_ai") or
	pets.default_set_pet_ai

pets.remove_pet_ai = get_subsystem_param("pets", "remove_pet_ai") or
	pets.default_remove_pet_ai

pets.on_faction_change = get_subsystem_param("pets", "on_faction_change") or
function(monst, old_faction, new_faction)
	if new_faction == player.faction then
		pets.set_pet_ai(monst)

		-- Don't target it now that it's friendly.
		local m_idx = cave(monst.fy, monst.fx).m_idx
		if pets.target_list[m_idx] then
			pets.on_target_gone(m_idx, pets.target_list[m_idx])
		end
	elseif old_faction == player.faction then
		pets.remove_pet_ai(monst)
	end
end

hook(hook.NEW_MONSTER_POST,
function(y, x)
	local monst = monster(cave(y, x).m_idx)

	if pets.is_pet(monst) then
		pets.set_pet_ai(monst)
	end
end)

hook(hook.MON_CHANGE_FACTION_POST, pets.on_faction_change)

---------------------------------------------------------------------------
---------------------------------------------------------------------------

pets.orders            = {}
pets.order_name_to_idx = {}
pets.std_orders        = {}
pets.std_order_list    = {}

function pets.std_order(t)
	assert(t.name, "Standard order has no name")

	pets.std_orders[t.name] = t
	tinsert(pets.std_order_list, t.name)
end

function pets.new_order(t)
	assert(t.name, "Order has no name")
	assert(t.do_add, "Order '" .. t.name .. "' has no 'do_add' field")
	assert(t.do_remove, "Order '" .. t.name .. "' has no 'do_remove' field")

	t.name   = strlower(t.name)
	t.define = "PET_ORDER_" .. gsub(strupper(t.name), " ", "_")

	tinsert(pets.orders, t)
	t.__index__ = getn(pets.orders)

	pets.order_name_to_idx[t.name] = t.__index__

	globals()[t.define] = t.__index__

	if t.on_new then
		t.on_new(t)
	end

	return t.__index__
end

function pets.add_order(monst, order, cmd, ...)
	local order_name, order_idx, order_obj

	if type(order) == "string" then
		order_name = order
		order_idx  = pets.order_name_to_idx[order]
		order_obj  = pets.orders[order_idx]
	elseif type(order) == "number" then
		order_idx  = order
		order_obj  = pets.orders[order_idx]
		order_name = order_obj.name
	else
		error("Invalid order type")
	end

	if monst.flags[FLAG_PET_ORDERS][order_idx] then
		pets.remove_order(monst, order_idx, cmd)
	end

	local data = order_obj.do_add(monst, cmd, unpack(arg))

	monst.flags[FLAG_PET_ORDERS][order_idx] = data or true
end

function pets.remove_order(monst, order, cmd)
	local order_name, order_idx, order_obj

	if type(order) == "string" then
		order_name = order
		order_idx  = pets.order_name_to_idx[order]
		order_obj  = pets.orders[order_idx]
	elseif type(order) == "number" then
		order_idx  = order
		order_obj  = pets.orders[order_idx]
		order_name = order_obj.name
	else
		error("Invalid order type")
	end

	if not monst.flags[FLAG_PET_ORDERS][order_idx] then
		if wizard then
			message(color.LIGHT_RED, "WARNING: monster " ..
					monster_desc(256 | 128) .. " not already following " ..
						"order " .. order_obj.name)
		end
		return
	end

	order_obj.do_remove(monst, cmd, monst.flags[FLAG_PET_ORDERS][order_idx])

	monst.flags[FLAG_PET_ORDERS][order_idx] = nil
end

function pets.remove_all_orders(monst, cmd)
	local orders = {}

	foreach_flags(monst.flags[FLAG_PET_ORDERS],
		function(flags, key)
			tinsert(%orders, key)
		end)

	for i = 1, getn(orders) do
		pets.remove_order(monst, orders[i], cmd)
	end
end

-----

pets.std_order {
	name = "target",
	do_add =
		function(monst, cmd, m_idx)
			monst.ai_states[FLAG_PET_TARGET] = m_idx
			pets.acquire_target(cave(monst.fy, monst.fx).m_idx, m_idx)
		end,

	do_remove =
		function(monst, cmd)
			local target = monst.ai_states[FLAG_PET_TARGET]
			pets.lose_target(cave(monst.fy, monst.fx).m_idx, target or 0)
			monst.ai_states[FLAG_PET_TARGET] = nil
		end

	on_new =
		function()
			pets.on_target_gone	=
				function(m_idx, who_targets)
					for pet_idx, _ in who_targets do
						local pet = monster(pet_idx)

						if pet.r_idx ~= 0 then
							pets.inform_target_gone(pet, m_idx)
						end
					end
				end

			pets.inform_target_gone =
				function(pet, m_idx)
					local target = pet.ai_states[FLAG_PET_TARGET]

					if target and target == m_idx then
						pets.remove_order(pet, "target")
					end
				end
		end, -- on_new()
} -- target

pets.std_order {
	name = "guard",
	do_add =
		function(monst, cmd)
			monst.ai_states[FLAG_PET_ESCORT_DIST] = 1
		end,

	do_remove =
		function(monst, cmd)
			monst.ai_states[FLAG_PET_ESCORT_DIST] = 0
		end
} -- guard

pets.std_order {
	name = "escort",

	describe_arg =
		function(monst, arg)
			return "Stray no more than a distance of " .. arg ..
				" from you."
		end

	do_add =
		function(monst, cmd, dist)
			assert(dist, "Escort command must have a distance argument.")
			assert(dist > 0, "Escort distance must be a positve number.")
			monst.ai_states[FLAG_PET_ESCORT_DIST] = dist
			return dist
		end,

	do_remove =
		function(monst, cmd)
			monst.ai_states[FLAG_PET_ESCORT_DIST] = 0
		end
} -- escot

pets.std_order {
	name = "stay",

	do_add =
		function(monst, cmd)
			local state = monst.ai_states

			if state[FLAG_MOVE_AI] == ai.NONE and
				state[FLAG_CASTING_AI] == ai.NONE
			then
				state[FLAG_AI] = ai.NEVER_MOVE
			else
				state[FLAG_MOVE_AI] = ai.NEVER_MOVE
			end
		end,

	do_remove =
		function(monst, cmd)
			local state = monst.ai_states
			local orig  = monst.flags[FLAG_ORIGINAL_FLAGS]

			if state[FLAG_MOVE_AI] == ai.NONE and
				state[FLAG_CASTING_AI] == ai.NONE
			then
				state[FLAG_AI] = orig[FLAG_AI]
			else
				state[FLAG_MOVE_AI] = orig[FLAG_AI_STATES][FLAG_MOVE_AI]
			end
		end
} -- stay

pets.std_order {
	name = "leave doors alone",

	do_add =
		function(monst, cmd)
			local state = monst.ai_states

			state[FLAG_OPEN_DOOR] = nil
			state[FLAG_BASH_DOOR] = nil
		end

	do_remove =
		function(monst, cmd)
			local state = monst.ai_states
			local orig  = monst.flags[FLAG_ORIGINAL_FLAGS]

			state[FLAG_OPEN_DOOR] = orig[FLAG_OPEN_DOOR]
			state[FLAG_BASH_DOOR] = orig[FLAG_BASH_DOOR]
		end
} -- leave doors alone

pets.std_order {
	name = "dont pickup items",

	do_add =
		function(monst, cmd)
			local state = monst.ai_states

			state[FLAG_TAKE_ITEM] = nil
		end

	do_remove =
		function(monst, cmd)
			local state = monst.ai_states
			local orig  = monst.flags[FLAG_ORIGINAL_FLAGS]

			state[FLAG_TAKE_ITEM] = orig[FLAG_TAKE_ITEM]
		end
} -- don't pickup items

--

pets.std_orders_to_use =
	get_subsystem_param("pets", "std_orders_to_use") or
	pets.std_order_list

for i = 1, getn(pets.std_orders_to_use) do
	local name = pets.std_orders_to_use[i]
	assert(pets.std_orders[name], "'" .. name .. "' is not a standard order")

	pets.new_order(pets.std_orders[name])
end

---------------------------------------------------------------------------

pets.commands            = {}
pets.command_name_to_idx = {}
pets.std_commands        = {}
pets.std_command_list    = {}

function pets.std_command(t)
	assert(t.name, "Standard command has no name")

	pets.std_commands[t.name] = t

	tinsert(pets.std_command_list, t.name)
end

function pets.new_command(t)
	assert(t.name, "Command has no name")
	assert(t.execute, "Command '" .. t.name .. "' has no 'execute' field")

	t.name = strlower(t.name)

	assert(not pets.command_name_to_idx[t.name],
		   "Command '" .. t.name .. "' already added.")

	if t.add_flags then
		for i = 1, getn(t.add_flags) do
			safe_new_flag(t.add_flags[i])
		end
	end

	t.need_flags    = getter.flags(t.need_flags    or {})
	t.forbid_flags  = getter.flags(t.forbid_flags  or {})
	t.need_orders   = getter.array(t.need_orders   or {})
	t.forbid_orders = getter.array(t.forbid_orders or {})

	default_value(t, "remove_orders", {})

	tinsert(pets.commands, t)
	t.__index__ = getn(pets.commands)

	pets.command_name_to_idx[t.name] = t.__index__

	if t.on_new then
		t.on_new(t)
	end

	return t.__index__
end

function pets.choose_cmd(pets_list, cmds_list, colors)
	cmds_list = cmds_list or pets.commands

	local strs = {}
	for i = 1, getn(cmds_list) do
		strs[i] = cmds_list[i].name
	end

	local our_clrs = {}
	colors = colors or function() return color.WHITE end
	for i = 1, getn(cmds_list) do
		our_clrs[i] = colors(cmds_list[i], pets_list) or color.WHITE
	end

	local choice = 
		choose_multi_column("Select command (ESC to quit): ", strs,
							true, 3, our_clrs)

	if choice then
		return cmds_list[choice].name
	end

	return nil
end -- pets.choose_cmd()

function pets.prompt(monst, prompt_str)
	term.prt("", 0, 0)

	local info = format("(lev %d, %d/%d HP)", monst.level,
						monst.hp, monst.maxhp)
	prompt_str = monster_strsub(prompt_str, monst, 512 | 256 | 128)
	prompt_str = gsub(prompt_str, "@info@", info)

	-- Hilite monster with cursor if the player can see it.
	if monst.ml then
		term.move_cursor_relative(monst.fy, monst.fx)
	end
	return get_check(strcap(prompt_str) .. "? ", true)
end

function pets.get_need_msg(monst_flags, need_flags, default)
	local out = {str = default}
	foreach_flags(need_flags,
		function(flags, key)
			if not %monst_flags[key] then
				local str = flags[key]

				if tag(str) == TAG_STRING then
					%out.str = str
				end
			end
		end)
	return out.str
end

function pets.get_forbid_msg(monst_flags, forbid_flags, default)
	local out = {str = default}
	foreach_flags(forbid_flags,
		function(flags, key)
			if %monst_flags[key] and not %out.found then
				local str = %monst_flags[key]

				if tag(str) == TAG_STRING then
					%out.found = true
				else
					str = flags[key]
				end

				if tag(str) == TAG_STRING then
					%out.str = str
				end
			end
		end)
	return out.str
end

function pets.validate_command(monst, cmd, args, silent)
	if monst.flags[FLAG_PET_COMMAND_PRE] then
		local func
		func = get_func_from_flag_or_registry(monst.flags,
											  FLAG_PET_COMMAND_PRE)

		if func(monst, cmd, args, silent) then
			return false
		end
	end

	if cmd.validate_command then
		local msg = cmd.validate_command(monst, cmd, args)

		if msg then
			if not silent then
				msg = monster_strsub(msg, monst, 256 | 128)
				message(msg)
			end
			return false
		end
	end

	if not flag_contains(monst.flags, cmd.need_flags) then
		if not silent then
			local def = "@Name@ refuses the command."
			local msg = pets.get_need_msg(monst.flags, cmd.need_flags,
										  def)
			msg = monster_strsub(msg, monst, 256 | 128)
			term.prt("", 0, 0)
			message(msg)
		end
		return false
	end

	if not flag_contains(monst.flags[FLAG_PET_ORDERS], cmd.need_orders) then
		if not silent then
			local def = "@Name@ needs to be following other orders." ..
				"as a prerequisite for this command."
			local msg = pets.get_need_msg(monst.flags[FLAG_PET_ORDERS],
										  cmd.need_orders, def)
			msg = monster_strsub(msg, monst, 256 | 128)
			term.prt("", 0, 0)
			message(msg)
		end
		return false
	end

	if flag_intersects(monst.flags, cmd.forbid_flags) then
		if not silent then
			local def = "@Name@ refuses the command."
			local msg = pets.get_forbid_msg(monst.flags, cmd.forbid_flags,
											def)
			msg = monster_strsub(msg, monst, 256 | 128)
			term.prt("", 0, 0)
			message(msg)
		end
		return false
	end

	if flag_intersects(monst.flags[FLAG_PET_ORDERS], cmd.forbid_orders) then
		if not silent then
			local def = "@Name@ is following orders conflicting with this " ..
				" command."
			local msg = pets.get_forbid_msg(monst.flags[FLAG_PET_ORDERS],
											cmd.forbid_orders, def)
			msg = monster_strsub(msg, monst, 256 | 128)
			term.prt("", 0, 0)
			message(msg)
		end
		return false
	end

	return true
end -- pets.validate_command()

function pets.execute_command(cmd, list, args)
	local cmd_name, cmd_idx, cmd_obj

	if type(cmd) == "string" then
		cmd_name = cmd
		cmd_idx  = pets.command_name_to_idx[cmd]
		cmd_obj  = pets.commands[cmd_idx]
	elseif type(cmd) == "number" then
		cmd_idx  = cmd
		cmd_obj  = pets.commands[cmd_idx]
		cmd_name = cmd_obj.name
	else
		error("Invalid command type")
	end

	local num = getn(list)

	if num == 0 then
		message("No monsters to which to give command '" .. cmd_name .. "'")
		return
	end

	if not args and cmd_obj.get_args then
		args = cmd_obj.get_args(cmd_obj, list)

		if not args or args.cancel then
			return
		end
	else
		args = args or {}
	end

	local past_tense = cmd_obj.past_tense or "accepted command"
	local prompt_str = cmd_obj.prompt_str or "issue command to"
	if not strfind(prompt_str, "@[Nn]ame@") then
		prompt_str = prompt_str .. " @name@"
	end

	local do_prompt
	if args.dont_prompt then
		do_prompt = false
	elseif cmd_obj.prompt_always then
		do_prompt = true
	elseif cmd_obj.prompt_check and
		cmd_obj.prompt_check(cmd_obj, list, args)
	then
		do_prompt = true
	elseif getn(list) > 1 then
		if cmd_obj.prompt_for_multiple then
			do_prompt = true
		elseif cmd_obj.ask_prompt_for_multiple then
			local ret, key =
				get_com("Prompt for each of " .. num .. " monsters? (y/n)")

			if ret then
				local char = strlower(strchar(key))

				if char == "y" then
					do_prompt = true
				end
			end
		end
	end

	local num_issues, num_refusals = 0, 0
	local silent = (args.silent and not do_prompt)

	if hook.process(hook.PET_COMMAND_PRE, cmd, list, args, do_prompt) then
		return
	end

	for i = 1, getn(list) do
		local monst = list[i]

		if not do_prompt or pets.prompt(monst, prompt_str) then
			if pets.validate_command(monst, cmd_obj, args, silent) then
				cmd_obj.execute(cmd_obj, monst, args)
				num_issues = num_issues + 1

				if monst.flags[FLAG_PET_COMMAND_POST] then
					local func
					func =
						get_func_from_flag_or_registry(monst.flags,
													   FLAG_PET_COMMAND_POST)
					func(monst, cmd, args, do_prompt)
				end
			else -- if pets.validate_command(monst, cmd_obj, args, silent)
				num_refusals = num_refusals + 1
			end
		end -- if not do_prompt or pets.prompt(monst, verb) then
	end -- for i = 1, getn(list) do
	if getn(list) > 1 and not silent then
		if num_issues == 0 then
			message("No monster accepted your command.")
		elseif num_refusals == 0 then
			message(num_issues .. " monster(s) " .. past_tense .. ".")
		else
			message(num_issues .. " monster(s) " .. past_tense ..
					", " .. num_refusals .. "refused.")
		end
	end

	hook.process(hook.PET_COMMAND_POST, cmd, list, args, do_prompt)

	-- No longer using the default commands
	if not cmd_obj.doesnt_effect_defaults then
		for i = 1, getn(list) do
			list[i].flags[FLAG_PET_USING_DEFAULTS] = nil
		end
	end
end -- pets.execute_command()

---------------------------------------------------------------------------

add_loadsave("pets.default_commands", {})

function pets.reset_to_defaults(pet_list)
	for i = 1, getn(pet_list) do
		pets.remove_all_orders(pet_list[i])
	end

	for cmd, args in pets.default_commands do
		pets.execute_command(cmd, pet_list, args)
	end

	for i = 1, getn(pet_list) do
		pet_list[i].flags[FLAG_PET_USING_DEFAULTS] = true
	end
end

function pets.reset_default_followers()
	local pet_list = {}
	foreach_monster(function(monst, m_idx)
		if pets.is_pet(monst) and pets.can_command(monst) and
			monst.flags[FLAG_PET_USING_DEFAULTS]
		then
			tinsert(%pet_list, monst)
		end
	end)

	pets.reset_to_defaults(pet_list)
end

function pets.add_default_command(cmd_obj, args)
	if not cmd_obj.can_be_default then
		message(color.LIGHT_RED, "ERROR: pet command " .. cmd_obj.name ..
				" cannot be a default command.")
		return false
	end

	args             = args or {}
	args.silent      = true
	args.dont_prompt = true

	pets.default_commands[cmd_obj.__index__] = args

	pets.reset_default_followers()

	return true
end -- pets.add_default_command()

function pets.remove_default_command(cmd_obj)
	if not pets.default_commands[cmd_obj.__index__] then
		message(color.LIGHT_RED, "ERROR: pet command " .. cmd_obj.name ..
				" is not a default command.")
		return false
	end

	pets.default_commands[cmd_obj.__index__] = nil

	pets.reset_default_followers()

	return true
end

function pets.show_current_defaults(cmds, row, letter)
	for i = 1, getn(cmds) do 
		local cmd      = cmds[i]
		local cmd_obj  = pets.commands[cmd]
		local cmd_name = cmd_obj.name
		local args     = pets.default_commands[cmd]
		local prefix

		if letter then
			prefix = strchar(i + strbyte('a') - 1) .. ") "
		else
			prefix = ""
		end

		term.prt(prefix .. cmd_name, row, 2)
		row = row + 1

		if cmd_obj.describe_args then
			local desc = cmd_obj.describe_args(args)
			term.prt(desc, row, 4 + iif (letter, 3, 0))
			row = row + 1
		end
	end -- for i = 1, getn(cmds) do 
end

function pets.add_default_screen()
	local possible = {}
	for i = 1, getn(pets.commands) do
		if pets.commands[i].can_be_default then
			tinsert(possible, pets.commands[i])
		end
	end

	if getn(possible) == 0 then
		message(color.LIGHT_RED, "ERROR: no commands capable of being " ..
				"used as a default command.")
		return
	end
	sort(possible,
		 function(a, b)
			 return a.__index__ < b.__index__
		 end)

	local cmd_name = pets.choose_cmd(nil, possible)

	if not cmd_name then
		return
	end

	local cmd_idx = pets.command_name_to_idx[cmd_name]
	local cmd_obj = pets.commands[cmd_idx]

	local args
	if cmd_obj.get_args then
		args = cmd_obj.get_args(cmd_obj, nil)

		if not args or args.cancel then
			return
		end
	else
		args = {}
	end

	pets.add_default_command(cmd_obj, args)
end -- pets.add_default_screen()

function pets.remove_default_screen(cmds)
	while true do
		term.clear()

		pets.show_current_defaults(cmds, 2, true)

		term.prt("Choose default command to remove (ESC to quit): ", 0, 0)

		local key  = term.inkey()
		local char = strlower(strchar(key))
		local choice = strbyte(char) - strbyte('a') + 1

		if key == ESCAPE then
			break
		end

		if choice >=1 and choice <= getn(cmds) then
			local cmd_idx = cmds[choice]
			local cmd_obj = pets.commands[cmd_idx]

			pets.remove_default_command(cmd_obj)
			break
		end
	end
end -- pets.remove_default_screen()

function pets.edit_defaults(menu_item, pets_list)
	while true do
		-- List of default commands might change on each iteration
		local cmds = {}
		for cmd, args in pets.default_commands do
			tinsert(cmds, cmd)
		end
		sort(cmds)

		term.clear()

		term.prt("(a) Add/replace command", 2, 5)

		local clr
		if getn(cmds) == 0 then
			clr = color.LIGHT_DARK
		else
			clr = color.WHITE
		end
		term.c_put_str(clr, "(b) Remove command", 3, 5)

		term.prt("--------------------------------------------", 5, 0)
		term.c_put_str(color.LIGHT_GREEN, "Current default commands:", 7, 0)

		pets.show_current_defaults(cmds, 9)

		term.prt("Command (ESC to quit): ", 0, 0)

		local key  = term.inkey()
		local char = strlower(strchar(key))

		if key == ESCAPE then
			break
		end

		if char == 'a' then
			pets.add_default_screen()
		elseif char == 'b' and getn(cmds) > 0 then
			pets.remove_default_screen(cmds)
		end
	end -- while true do
end -- pets.edit_defaults()

----

pets.std_command {
	name       = "reset to defaults",
	prompt_str = "reset @name@ to defaults",
	past_tense = "reset",

	ask_prompt_for_multiple = true,

	execute = function(cmd_obj, monst)
		pets.reset_to_defaults({monst})
	end
} -- reset

pets.std_command {
	name       = "forget all orders",
	prompt_str = "forget all orders for",
	past_tense = "forgot all orders",

	ask_prompt_for_multiple = true,

	execute = function(cmd_obj, monst)
		pets.remove_all_orders(monst)
	end
} -- forget

pets.std_command {
	name       = "dismiss",
	prompt_str = "dismiss @name@ @info@",
	past_tense = "dismissed",

	doesnt_effect_defaults = true,

	add_flags = {"PET_NO_DISMISS", "PET_NO_DROP"},

	forbid_flags = {PET_NO_DISMISS = "@Name@ refuses to leave your service."},

	ask_prompt_for_multiple = true,

	execute = function(cmd_obj, monst, args)
		local y, x = monst.fy, monst.fx
		if not in_bounds(y, x) then
			return
		end

		local m_idx = cave(y, x).m_idx
		if m_idx == 0 then
			if wizard then
				message(color.LIGHT_RED, "ERROR: monster " ..
						monster_desc(monst, 512 | 256 | 128) ..
							" not where it's supposed to be.")
			end
			return
		end

		if not monst.flags[FLAG_PET_NO_DROP] then
			pets.monster_drop_all(monst)
		end
		
		delete_monster_idx(m_idx)
	end
} -- dismiss

pets.std_command {
	name       = "give target",
	prompt_str = "give target to",
	past_tense = "accepted target",

	doesnt_effect_defaults = true,

	add_flags = {"PET_NO_TARGET"},

	forbid_flags = {
		PET_NO_TARGET = "@Name@ refuses to let you decide @possessive@ " ..
			"target."
	},

	forbid_orders = {
		[PET_ORDER_GUARD] = "@Name@ can't accept a target because @pronoun@"
		.. " is guaring you."
	},

	ask_prompt_for_multiple = true,

	get_args = function(cmd_obj, list)
		local ret, x, y = tgt_pt_prompt("Select a monster and press space.")

		if not ret then
			return nil
		end

		local m_idx = cave(y, x).m_idx
		local monst = monster(m_idx)

		if m_idx == 0 or monst.r_idx == 0 or not monst.ml then
			message("There is no monster there.")
			return nil
		end

		if not pets.can_target(monst) then
			return nil
		end

		return {
			m_idx = m_idx,
			monst = monst,
			y     = y,
			x     = x,
		}
	end -- get_args()

	execute = function(cmd_obj, monst, args)
		pets.add_order(monst, "target", cmd_obj, args.m_idx)
	end
} -- target

pets.std_command {
	name       = "forget target",
	prompt_str = "forget target for",
	past_tense = "forgot target",

	doesnt_effect_defaults = true,

	need_orders = {
		[PET_ORDER_TARGET] = "@Name@ has no target to forget."
	},

	execute = function(cmd_obj, monst, args)
		pets.remove_order(monst, "target", cmd_obj)
	end
} -- forget target

function pets.monster_drop_all(monst)
	local y, x = monst.fy, monst.fx
	if not in_bounds(y, x) then
		return
	end

	for_inventory(monst, INVEN_INVEN, INVEN_TOTAL,
		function(obj, inven, slot, item)
			drop_near(obj, -1, %y, %x)
		end)

	flag_empty_zap(monst.inventory)
end -- dismiss

pets.std_command {
	name       = "stay put",
	prompt_str = "have @name@ stay put",
	past_tense = "staying put",

	can_be_default = true,

	forbid_flags = {
		NEVER_MOVE = "@Name@ never moves anyways."
	},

	execute = function(cmd_obj, monst, args)
		pets.add_order(monst, "stay", cmd_obj)
	end
} -- stay put

pets.std_command {
	name       = "move again",
	prompt_str = "let @name@ move again",
	past_tense = "moving again",

	need_orders = {
		[PET_ORDER_STAY] = "@Name@ was never commanded to stay put."
	},

	execute = function(cmd_obj, monst, args)
		pets.remove_order(monst, "stay", cmd_obj)
	end
} -- move again

pets.std_command {
	name       = "guard you",
	prompt_str = "have @name@ guard you",
	past_tense = "guarding you",

	add_flags = {"PET_NO_GUARD"},

	forbid_flags = {
		NEVER_MOVE = "@Name@ can't guard you since @pronoun@ can't move.",
		PET_NO_GUARD = "@Name@ refuses to guard you.",
	},

	execute = function(cmd_obj, monst, args)
		pets.add_order(monst, "guard", cmd_obj)
	end
} -- guard

pets.std_command {
	name       = "stop guarding",
	prompt_str = "have @name@ stop guarding you",
	past_tense = "stopped guarding you",

	need_orders = {
		[PET_ORDER_GUARD] = "@Name@ wasn't guarding you."
	},

	execute = function(cmd_obj, monst, args)
		pets.remove_order(monst, "guard", cmd_obj)
	end
} -- stop guarding

pets.std_command {
	name       = "escort",
	prompt_str = "have @name@ escort you",
	past_tense = "escorting you",

	can_be_default = true,

	add_flags = {"PET_NO_ESCORT"},

	forbid_flags = {
		NEVER_MOVE = "@Name@ can't escort you since @pronoun@ can't move.",
		PET_NO_ESCORT = "@Name@ refuses to escort you.",
	},

	forbid_orders = {
		[PET_ORDER_GUARD] = "@Name@ can't escort you because @pronoun@" ..
			" is guaring you."
	},

	get_args = function(cmd_obj, list)
		local dist = get_quantity("Maximum distance escort should stray?",
								  pets.max_escort_dist)

		if dist <= 0 then
			return nil
		end
		return {dist = dist}
	end

	describe_args =
		function(args)
			return "Stray a maximum distance of " .. args.dist ..
				" from you."
		end

	execute = function(cmd_obj, monst, args)
		pets.add_order(monst, "escort", cmd_obj, args.dist)
	end
} -- escort

pets.std_command {
	name       = "stop escorting",
	prompt_str = "have @name@ stop escorting you",
	past_tense = "stopped escorting you",

	need_orders = {
		[PET_ORDER_ESCORT] = "@Name@ isn't escorting you."
	},

	execute = function(cmd_obj, monst, args)
		pets.remove_order(monst, "escort", cmd_obj)
	end
} -- stop escorting

pets.std_command {
	name       = "drop objects",
	prompt_str = "have @name@ drop its inventory",
	past_tense = "dropped their inventory",

	doesnt_effect_defaults = true,

	add_flags = {"PET_NO_DROP"},

	forbid_flags = {PET_NO_DROP = "@Name@ refuses to relinquish " ..
		"@pronoun@ loot."},

	execute = function(cmd_obj, monst, args)
		pets.monster_drop_all(monst)
	end
} -- drop objects

pets.std_command {
	name       = "don't pickup items",
	prompt_str = "forbid @name@ from picking up items",
	past_tense = "won't pick up items anymore",

	can_be_default = true,

	add_flags = {"PET_NO_DONT_PICKUP"},

	forbid_flags = {PET_NO_DONT_PICKUP = "@Name@ refuses to stop " ..
		"picking up items."},

	execute = function(cmd_obj, monst, args)
		pets.add_order(monst, "dont pickup items", cmd_obj)
	end
} -- don't pickup items

pets.std_command {
	name       = "pickup items again",
	prompt_str = "have @name@ pickup items again",
	past_tense = "will pickup items again",

	need_orders = {
		[PET_ORDER_DONT_PICKUP_ITEMS] = "@Name@ was never forbidden from " ..
			"picking up items."
	},

	execute = function(cmd_obj, monst, args)
		pets.remove_order(monst, "dont pickup items", cmd_obj)
	end
} -- pickup items again

pets.std_command {
	name       = "leave doors alone",
	prompt_str = "have @name@ leave doors alone",
	past_tense = "won't bash/open doors anymore",

	can_be_default = true,

	add_flags = {"PET_NO_DOORS"},

	forbid_flags = {PET_NO_DOORS = "@Name@ refuses to stop " ..
		"opening/bashing doors."},

	validate_command = function (monst, cmd_obj, args)
		if monst.flags[FLAG_PET_ORDERS][PET_ORDER_LEAVE_DOORS_ALONE] then
			-- Already following that order
			return nil
		end

		-- Has to have at least one of OPEN_DOOR or BASH_DOOR
		if not (monst.flags[FLAG_OPEN_DOOR] or
				monst.flags[FLAG_BASH_DOOR])
		then
			return "@Name@ leaves doors alone in the first place."
		end
	end -- validate_command

	execute = function(cmd_obj, monst, args)
		pets.add_order(monst, "leave doors alone", cmd_obj)
	end
} -- leave doors alone

pets.std_command {
	name       = "open/bash doors again",
	prompt_str = "have @name@ open/bash doors again",
	past_tense = "will open/bash doors again",

	need_orders = {
		[PET_ORDER_LEAVE_DOORS_ALONE] = "@Name@ was never forbidden from " ..
			"opening/bashing doors."
	},

	execute = function(cmd_obj, monst, args)
		pets.remove_order(monst, "leave doors alone", cmd_obj)
	end
} -- open/bash doors again

--

pets.std_commands_to_use =
	get_subsystem_param("pets", "std_commands_to_use") or
	pets.std_command_list

for i = 1, getn(pets.std_commands_to_use) do
	local name = pets.std_commands_to_use[i]
	assert(pets.std_commands[name], "'" .. name ..
		   "' is not a standard command")

	pets.new_command(pets.std_commands[name])
end

---------------------------------------------------------------------------
---------------------------------------------------------------------------

function pets.add_menu_items(items)
	if type(items[1]) == "table" then
		for i = 1, getn(items) do
			tinsert(pets.menu_list, items[i])
		end
	else
		tinsert(pet.menu_list, items)
	end
end

function pets.show_pet_orders(pet, row)
	local orders = {}

	foreach_flags(pet.flags[FLAG_PET_ORDERS],
		function(flags, key)
			tinsert(%orders, key)
		end)

	sort(orders)

	for i = 1, getn(orders) do 
		local order      = orders[i]
		local order_obj  = pets.orders[order]
		local order_name = order_obj.name
		local arg        = pet.flags[FLAG_PET_ORDERS][order]

		term.prt(order_name, row, 2)
		row = row + 1

		if order_obj.describe_arg then
			local desc = order_obj.describe_arg(pet, arg)
			term.prt(desc, row, 4)
			row = row + 1
		end
	end -- for i = 1, getn(orders) do 

	return row
end

pets.show_details = get_subsystem_param("pets", "show_details") or
	function(pet)
		while true do
			term.clear()
			term.prt("Pet details", 0, 35)

			term.prt("Name: ", 2, 0)
			term.c_put_str(color.LIGHT_BLUE,
						   monster_desc(pet, 512 | 256 | 128),
						   2, 6)

			term.prt("Type: ", 3, 0)
			term.c_put_str(color.LIGHT_BLUE, pets.get_pet_type(pet),
						   3, 6)

			term.prt("Level: ", 5, 0)
			term.c_put_str(color.LIGHT_GREEN, "" .. pet.level,
						   5, 10)

			local clr
			if pet.hp < pet.maxhp then
				clr = color.YELLOW
			else
				clr = color.LIGHT_GREEN
			end
			term.prt("HP:", 6, 0)
			term.c_put_str(clr, pet.hp .. "/" .. pet.maxhp, 6, 10)

			if (pet.flags[FLAG_MANA_CURR] and pet.flags[FLAG_MANA_MAX])	then
				local sp  = pet.flags[FLAG_MANA_CURR]
				local msp = pet.flags[FLAG_MANA_MAX]

				if sp < msp then
					clr = color.YELLOW
				else
					clr = color.LIGHT_GREEN
				end
				term.prt("Mana: ", 7, 0)
				term.c_put_str(clr, sp .. "/" .. msp, 7, 10)
			else
				term.prt("Mana:     0/0", 7, 0)
			end

			if pet.ml or wizard then
				local dy = pet.fy - player.py
				local dx = pet.fx - player.px
				
				local y_str, x_str
				if dy ~= 0 then
					y_str = abs(dy) .. iif(dy > 0, "S", "N")
				end
				if dx ~= 0 then
					x_str = abs(dx) .. iif(dx > 0, "E", "W")
				end

				local pos
				if y_str and x_str then
					pos = y_str .. "," .. x_str
				elseif not (y_str or x_str) then
					pos = "Right underneath you?"
				else
					pos = y_str or x_str
				end

				term.prt("Location: " .. pos, 8, 0)
			else
				term.prt("Location: unknown", 8, 0)
			end

			term.prt("Current orders:", 10, 0)
			local row = pets.show_pet_orders(pet, 11)

			term.prt("------------------------------------------------",
					 row, 0)

			term.prt("ESC to exit", row + 1, 0)

			if pets.can_command(pet) then
				term.prt("'c' to issue command to this monster",
						 row + 2, 0)
			end

			local key  = term.inkey()
			local char = strlower(strchar(key))

			if key == ESCAPE then
				break
			end

			if char == 'c' and pets.can_command(pet) then
				local cmd, args = pets.choose_cmd({pet})

				if cmd then
					pets.execute_command(cmd, {pet}, args)
					if cmd == "dismiss" then
						return true
					end
				end
			end
		end -- while true do
		return false
	end -- pets.show_details()

function pets.list_pets(menu_item, pets_list)
	local array = {}

	for i = 1, getn(pets_list) do
		local monst = pets_list[i]
		local name  = monster_desc(monst, 512 | 256 | 128)
		local typ   = pets.get_pet_type(monst)
		local level = monst.level
		local maxhp = monst.maxhp
		local hp    = monst.hp

		local display = format("%-30s %10s %5d %9s", name, typ,
							   level, format("%d/%d", hp, maxhp))

		tinsert(array, {
					monst   = monst,
					name    = name,
					typ     = typ,
					level   = level,
					maxhp   = maxhp,
					hp      = hp,
					display = display,
				})
	end

	sort(array,
		 function(a, b)
			 if a.typ ~= b.typ then
				 return a.typ < b.typ
			 elseif a.name ~= b.name then
				 return a.name < b.name
			 elseif a.level ~= b.level then
				 return a.level < b.level
			 elseif a.maxhhp ~= b.maxhhp then
				 return a.maxhp < b.maxhp
			 else
				 return a.hp < b.hp
			 end
		 end)

	local strs = {}
	for i = 1, getn(array) do
		strs[i] = array[i].display
	end

	local footer = "[ENTER for more details, ESC to quit]"
	local title  = "List of commandable monsters (" .. getn(array) ..
		" total)"

	local header1 =
		format("%-30s %s %-5s %s",
			   "NAME", "   TYPE   ", "LEVEL", "    HP   ")
	local header2 =
		format("%-30s %s %-5s %s",
			   "----", "----------", "-----", "---------")

	tinsert(strs, 1, header2)
	tinsert(strs, 1, header1)

	local sel = 3
	while getn(pets_list) > 0 do
		sel = choose_from_list_simple(strs, title, true, sel, footer)

		if not sel then
			break
		end

		if sel >= 3 then
			local choice = sel - 2
			local pet    = pets_list[choice]

			if pets.show_details(pet) then
				-- Pet was removed
				tremove(strs,      choice + 2)
				tremove(pets_list, choice)
				if sel > (getn(pets_list) + 2) then
					sel = getn(pets_list) + 2
				end
			end
		end

		if sel < 3 then
			sel = 3
		end
	end
end -- pets.list_pets()

function pets.give_cmd_to_all_type(pets_list, typ)
	local out_list = pets.get_all_of_type(pets_list, typ)

	if getn(out_list) == 0 then
		if not typ then
			message("You have no commandable monsters.")
		else
			message("You have no " .. pluralize(typ) .. ".")
		end
		term.inkey()
		return nil
	end

	local cmd, args = pets.choose_cmd(out_list)

	if not cmd then
		return nil
	end

	return true, pets.execute_command, cmd, out_list, args
end

function pets.give_cmd_to_all(menu_item, pets_list)
	return pets.give_cmd_to_all_type(pets_list, nil)
end

function pets.give_cmd_to_name(menu_item, pets_list)
	local out_list = {}

	local match = get_string("Name to match? ")

	if not match then
		return nil
	end

	match = clean_whitespace(strlower(match))

	if match == "" then
		return
	end

	for i = 1, getn(pets_list) do
		local monst = pets_list[i]
		local name  = monster_desc(monst, 512 | 256 | 128)

		if strfind(name, match) then
			tinsert(out_list, monst)
		end
	end

	if getn(out_list) == 0 then
		message("No monsters matching '" .. match .. "'")
		return nil
	end

	local cmd, args = pets.choose_cmd(out_list)

	if not cmd then
		return nil
	end

	return true, pets.execute_command, cmd, out_list, args
end -- pets.give_cmd_to_name()

function pets.give_cmd_to_not_name(menu_item, pets_list)
	local out_list = {}

	local match = get_string("Name to exclude? ")

	if not match then
		return nil
	end

	match = clean_whitespace(strlower(match))

	if match == "" then
		return
	end

	for i = 1, getn(pets_list) do
		local monst = pets_list[i]
		local name  = monster_desc(monst, 512 | 256 | 128)

		if not strfind(name, match) then
			tinsert(out_list, monst)
		end
	end

	if getn(out_list) == 0 then
		message("No monsters left after excluding '" .. match .. "'")
		return nil
	end

	local cmd, args = pets.choose_cmd(out_list)

	if not cmd then
		return nil
	end

	return true, pets.execute_command, cmd, out_list, args
end -- pets.give_cmd_to_not_name()

pets.default_menu_items = {
	show_pets = {
		do_menu_item       = pets.list_pets,
		desc               = "List all",
		can_while_confused = true,
	},

	edit_defaults = {
		do_menu_item = pets.edit_defaults,
		desc         = "Edit default commands",
	},

	command_all = {
		do_menu_item = pets.give_cmd_to_all,
		desc         = "Give command to all",
	},

	command_name = {
		do_menu_item = pets.give_cmd_to_name,
		desc         = "Give command to monsters matching name",
	},

	command_not_name = {
		do_menu_item = pets.give_cmd_to_not_name,
		desc         = "Give command to monsters not matching name",
	},
}

pets.menu_list = get_subsystem_param("pets", "menu_list") or
{
	pets.default_menu_items.show_pets,
	pets.default_menu_items.edit_defaults,
	pets.default_menu_items.command_all,
}

pets.no_pets_msg = get_subsystem_param("pets", "no_pets_msg") or
	"You have no monsters to command."
pets.confused_msg = get_subsystem_param("pets", "confused_msg") or
	"You are too confused to command your monsters."
pets.hallu_msg = get_subsystem_param("pets", "hallu_msg") or
	"You are cannot command your monsters while hallucinating."

function pets.show_menu()
	assert(getn(pets.menu_list) <= 26,
		   "Too many pet command menu items to display.")

	local pet_list = {}

	if player.has_intrinsic(FLAG_HALLUCINATE) then
		message(pets.hallu_msg)
		return
	end

	foreach_monster(function(monst, m_idx)
		if pets.is_pet(monst) and (pets.is_aware(monst) or wizard) then
			tinsert(%pet_list, monst)
		end
	end)

	if getn(pet_list) == 0 then
		message(pets.no_pets_msg)
		return
	end

	term.save()

	local confused = player.has_intrinsic(FLAG_CONFUSED) 

	-- Are there any commands we can use while confused?
	if confused then
		local can_while_confused = false
		for i = 1, getn(pets.menu_list) do
			if pets.menu_list[i].can_while_confused then
				can_while_confused = true
				break
			end
		end

		if not can_while_confused then
			term.load()
			message(pets.confused_msg)
			return
		end
	end -- if confused then

	local done, done_func, done_arg1, done_arg2, done_arg3
	while not done do
		term.clear()

		--Ask for a choice
		term.prt("Issue pet commands", 0, 0)

		if confused then
			term.c_put_str(color.YELLOW, pets.confused_msg1, 0)
		end

		for i = 1, getn(pets.menu_list) do
			local char = strchar(strbyte('a') + i - 1)
			local item = pets.menu_list[i]
			local desc = item.desc

			local clr
			if confused and not item.can_while_confused then
				clr = color.LIGHT_DARK
			else
				clr = color.WHITE
			end
			term.c_put_str(clr, "(" .. char .. ") " .. desc, 2 + i, 5)
		end

		term.prt("Command (ESC to quit): ", 17, 0)

		local key    = term.inkey()
		local char   = strlower(strchar(key))
		local choice = strbyte(char) - strbyte('a') + 1

		if key == ESCAPE then
			break
		end

		if choice >= 1 and choice <= getn(pets.menu_list) then
			local item  = pets.menu_list[choice]

			if not confused or item.can_while_confused then
				done, done_func, done_arg1, done_arg2, done_arg3 =
					item.do_menu_item(item, pet_list)
			else
				message(pets.confused_msg)
				term.inkey()
			end
		end

		-- Pet list might have changed, re-construct it.
		pet_list = {}

		foreach_monster(function(monst, m_idx)
			if pets.is_pet(monst) and (pets.is_aware(monst) or wizard) then
				tinsert(%pet_list, monst)
			end
		end)

		if getn(pet_list) == 0 then
			term.prt("", 0, 0)
			message("All pets are gone, exiting.")
			break
		end
	end -- while not done do

	term.load()

	-- Term might have changed because of dismissed monster, so
	-- update
	player.update = player.update | PU_MONSTERS
	player.redraw[FLAG_PR_MAP] = true
	handle_stuff()

	if done_func and getn(pet_list) > 0 then
		done_func(done_arg1, done_arg2, done_arg3)
	end
end -- pets.show_menu()

function pets.do_command()
	pets.show_menu(pet_list)
end

if not get_subsystem_param("pets", "no_key_bind") then
	hook(hook.KEYPRESS, function (key)
        if key == strbyte('P') then
			pets.show_menu()
			return true
		end
    end)
end
