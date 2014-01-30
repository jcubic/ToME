-- Care for the combat systems

declare_global_constants {
	"add_combat_system",
	"add_blow_response",
	"call_attack",
	"check_combat_style",
	"choose_combat",
	"show_combat_choices",
	"get_blow_response",
	"get_combat_dam_info",
	"get_combat_num",
	"get_combat_style",
	"get_combat_skill_name",
	"get_combat_skills",
	"select_default_combat",
	"switch_combat_style",
	"get_combat_availability",
}
declare_globals {
	"__blow_responses",
	"__combats",
	"__combats_callbacks_max",
}

__combats = {}
__combats_callbacks_max = 0
function add_combat_system(t)
	assert(t.name, "no combat name")
	assert(t.desc, "no combat desc")
	assert(t.info, "no combat info")
	assert(t.available, "no combat availability")
	assert(t.skill, "no combat skill")
	assert(t.energy, "no combat energy")
	assert(t.attack, "no combat attack")

	for index, h in t.hooks do
		local callbackname = "__lua__combat_callback"..__combats_callbacks_max
		declare_global_constants {callbackname}
		hook.add_script(index, callbackname, callbackname)
		setglobal(callbackname, h)
		__combats_callbacks_max = __combats_callbacks_max + 1
	end

	-- Resolve skill name afterwards
	hook(hook.INFO_DATA_LOADED,
		function()
			local t = %t
			t.skill = globals()[t.skill] or find_skill(t.skill)
			__combats[t.skill] = t
		end
	)
end

function call_attack(y, x, max, dir)
	if player.combat_style == 0 then
		message("You have no available combat style with which to attack.")
		return false
	end
	assert(__combats[player.combat_style], "Unknown combat combat for skill "..player.combat_style);
	local ret, args = __combats[player.combat_style].attack(y, x, max, iif(dir ~= -1, dir, nil))
	energy_use = __combats[player.combat_style].energy(ret, args or {}) or energy_use
	return ret
end

function get_combat_skills()
	local c = {}

	for k, e in __combats do
		if e.available() == true then
			tinsert(c, e)
		end
	end

	return c
end

function switch_combat_style(combat, silent, is_autoswitch)
	if not combat then
		combat = {skill = 0}
	end

	local change = true

--[[ DGDGDGDG -- hummm
	local max = player.inventory_limits(INVEN_PACK)

	for i = 1, max do
		local obj = player.inventory[INVEN_MAINHAND][i]
		if obj then
			if (obj.ident & IDENT_CURSED) ~= 0 then
				message("Hmmm, your "..object_desc(obj, 0, 0).." seems to be cursed.")
				change = nil
				break
			else
				inven_takeoff(compute_slot(INVEN_MAINHAND, i), 255, false)
			end
		end
	end
]]
	if change then
		if player.combat_style == combat.skill then
			-- Same styles, so no need to switch
			return true, nil
		end

		local old_system
		local new_system = combat

		if player.combat_style ~= 0 then
			old_system = __combats[player.combat_style]
		else
			old_system = {skill = 0}
		end

		if old_system.pre_switch_from then
			old_system.pre_switch_from(new_system, silent, is_autoswitch)
		end
		if new_system.pre_switch_to then
			new_system.pre_switch_to(old_system, silent, is_autoswitch)
		end

		if old_system.can_switch_from then
			if not old_system.can_switch_from(new_system, silent,
											  is_autoswitch)
			then
				return false
			end
		end
		if new_system.can_switch_to then
			if not new_system.can_switch_to(old_system, silent,
											is_autoswitch)
			then
				return false
			end
		end

		player.combat_style = combat.skill

		local msgs = {}
		if old_system.post_switch_from then
			old_system.post_switch_from(new_system, msgs, is_autoswitch)
		end
		if new_system.post_switch_to then
			new_system.post_switch_to(old_system, msgs, is_autoswitch)
		end

		return true, msgs
	end
end -- switch_combat_style()

function show_combat_choices()
	local combats = get_combat_skills()
	local max = getn(combats)

	term.blank_print("Choose a combat style:", 0, 0)

	for k, e in combats do
		if type(k) == "number" then
			local msg = format("%c) %s", k - 1 + strbyte('a'), e.name)

			if player.combat_style == e.skill then
				msg = msg .. " [current style]"
			end

			term.blank_print(msg, k, 0)
		end
	end
end

function choose_combat(allow_escape)
	term.save()
	term.clear()

	local combats = get_combat_skills()
	local max = getn(combats)

	local msgs = nil
	local ret
	while not nil do
		term.clear()
		show_combat_choices()

		local c = term.inkey()

		if (c == ESCAPE) and allow_escape then break end

		if (c - strbyte('a') >= 0) and (c - strbyte('a') < max) then
			ret, msgs = switch_combat_style(combats[1 + c - strbyte('a')])
			if ret then
				energy_use = get_player_energy(SPEED_GLOBAL)
				break
			end
		end
		term.load()
		term.save()
	end -- while not nil do

	-- Recalculate bonuses
	player.update = player.update | PU_BONUS

	-- Recalculate hitpoint
	player.update = player.update | PU_HP

	-- Redraw monster hitpoint
	flag_set(player.redraw, FLAG_PR_MH, 1)

	term.load()

	if not ret or not msgs then
		message("Style unchanged.")
	else
		message(color.LIGHT_GREEN, "Switch succsesful.")

		if msgs and getn(msgs) > 0 then
			message("#yNOTE#w: " .. strjoin(msgs, "  "))
		end
	end
end

function select_default_combat()
	if get_combat_num() == 0 then
		if player.combat_style ~= 0 then
			message("You cannot use any combat styles.")
		end
		switch_combat_style(nil, false, true)
		return
	end

	local combats = get_combat_skills()

	player.combat_style = combats[1].skill
end

function get_combat_skill_name()
	if player.combat_style == 0 then
		return "none"
	end

	return __combats[player.combat_style].name
end

function get_combat_num()
	return getn(get_combat_skills())
end

function get_combat_style()
	if player.combat_style == 0 then
		return nil
	end
	return __combats[player.combat_style]
end

function get_combat_dam_info(...)
	if player.combat_style == 0 then
		return "N/A"
	end
	return __combats[player.combat_style].info(unpack(arg))
end

function get_combat_availability(c)
	return __combats[c].available()
end

-- Check if the current combat is still ok, otherwise warn and change
function check_combat_style()
	if player.combat_style == 0 or
		__combats[player.combat_style].available() ~= true
	then
		if get_combat_num() > 1 then
			if player.combat_style == 0 then
				message("You can use combat styles again, please choose one.")
			else
				message("You cannot use " ..
						__combats[player.combat_style].name ..
							" anymore, please select a new one.")
			end
			choose_combat(nil)
		elseif get_combat_num() == 1 then
			if player.combat_style == 0 then
				message("You can only use " .. get_combat_skills()[1].name ..
						", autoswitching to it.")
			else
				message("You cannot use " ..
						__combats[player.combat_style].name ..
							" anymore, autoswitching to " ..
							get_combat_skills()[1].name)
			end
			switch_combat_style(get_combat_skills()[1])
		else
			if player.combat_style ~= 0 then
				message("You cannot use any combat styles.")
			end
			switch_combat_style(nil, false, true)
		end
	end
end

---------------------------------------------------------------

-- "Blow response" is a generalized way of doing passive attacks,
-- like dealing the attacker of a fire elemental fire damage, but
-- can be used for things other than just passive attacks.

new_flag("BLOW_RESPONSE")

__blow_responses = {}

function add_blow_response(t)
	assert(t.name, "No blow response name")
	assert(t.monster_blow or t.weapon_blow, "No response function defined " ..
	   "for blow response " .. t.name)

	t.monster_blow = t.monster_blow or function() end
	t.weapon_blow  = t.weapon_blow  or function() end

	tinsert(__blow_responses, t)

	t.index = getn(__blow_responses)

	declare_global_constants(t.name)
	setglobal(t.name, t.index)

	return t.index
end

function get_blow_response(index)
	return __blow_responses[index]
end
