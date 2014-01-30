-- The default non-weapon "body" (possession/polymorphed) combat system.

-- Load it with: load_subsystem("combat_body_default")

-- Parameters:
-- * base_skill_name: The name of the skill which defines this combat method
-- * on_hit: function which will be called when a blow successfully landed 
--   (default to nothing)

-- Load the combat subsystem which defines some basic stuff for all
-- combat systems
load_subsystem("combat")

-- Make the namespace
combat.body_default = {}

combat.body_default.BTH_PLUS_ADJ = 3

-- Can we use a particular monster blow?
function combat.body_default.default_can_use_blow(body, blow, target)
	-- We aren't suicidal
	if blow.method.explode then
		return false
	end

	-- Definition forbids use by player
	if blow.method.no_body or blow.effect.no_body then
		return false
	end

	local special = {}

	if blow.method.fct then
		blow.method.fct(special, blow.effect, target)
	elseif method.body then
		blow.method.body(special, blow.effect, target)
	elseif method.monster and target > 0 then
		blow.method.monster(special, blow.effect, target)
	end

	-- Still not suicidal
	if special.explode then
		return false
	end

	if special.no_body then
		return false
	end

	return true
end -- default_can_use_blow()

combat.body_default.can_use_blow =
	get_subsystem_param("combat_body_default", "can_use_blow") or
	combat.body_default.default_can_use_blow

-- Return the blow table to use for attacks
combat.body_default.get_blows =
	get_subsystem_param("combat_body_default", "blow_table") or
	function(body, target)
		body   = body   or player.body_monster
		target = target or WHO_PLAYER

		if not flag_exists(body.blow, 0) then
			return {}
		end

		local blows = {}
		for ap_cnt = 0, flag_max_key(body.blow) do
			local blow_struct = flag_get_blow(body.blow, ap_cnt)

            local effect_idx = blow_struct.effect
            local method_idx = blow_struct.method

			if (method_idx == 0) then
				-- No more attacks
				break
			end

            local effect = __monster_attack_types[effect_idx]
            local method = __monster_attack_methods[method_idx]

			local blow = {
				effect = effect,
				method = method,
				d_dice = blow_struct.d_dice,
				d_side = blow_struct.d_side,
			}

			if combat.body_default.can_use_blow(body, blow, target)	then
				tinsert(blows, blow)
			end
		end -- for ap_cnt = 0, flag_max_key(monst.blow) do
		return blows
	end -- get_blows()

combat.body_default.INVEN_MAINHAND =
	get_subsystem_param("combat_body_default", "mainhand_slot") or 0
combat.body_default.INVEN_OFFHAND =
	get_subsystem_param("combat_body_default", "offhand_slot") or 0

combat.body_default.available =
	get_subsystem_param("combat_body_default", "available") or
	function()
		local blows = combat.body_default.get_blows()

		return (getn(blows) > 0)
	end

-- Did we hit the monster?
combat.body_default.test_hit =
	get_subsystem_param("combat_body_default", "test_hit") or
	function(power, level, ac)
		local i, k

		-- Percentile dice
		k = rng.number(100)

		-- Hack -- Always miss or hit
		if (k < 10) then
			if k < 5 then
				return true
			else
				return false
			end
		end

		-- Calculate the "attack quality"
		i = (power + (level * 3))

		-- Power and Level compete against Armor
		if ((i > 0) and (rng(i + luck(-10, 10)) > ((ac * 3) / 4))) then
			return true
		end

		-- Assume miss
		return false
	end -- test_hit()

-- Critical blow.  All hits that do 95% of total possible damage,
-- and which also do at least 20 damage, or, sometimes, N damage.
-- This is used only to determine "cuts" and "stuns" and such.
combat.body_default.critical_hit =
	get_subsystem_param("combat_body_default", "critical_hit") or
	function(dice, sides, dam)
		local max = 0
		local total = dice * sides

		-- Must do at least 95% of perfect
		if (dam < (total * 19) / 20) then return (0) end

		-- Weak blows rarely work
		if ((dam < 20) and (rng.number(100) >= dam)) then return (0) end

		-- Perfect damage
		if (dam == total) then max = max + 1 end

		-- Super-charge
		if (dam >= 20) then
			while (rng.number(100) < 2) do
				max = max + 1
			end
		end

		-- Critical damage
		if (dam > 45) then return (6 + max) end
		if (dam > 33) then return (5 + max) end
		if (dam > 25) then return (4 + max) end
		if (dam > 18) then return (3 + max) end
		if (dam > 11) then return (2 + max) end
		return (1 + max)
	end -- critical_hit()

combat.body_default.get_blink_msg =
	get_subsystem_param("combat_body_default", "get_blink_msg") or
	function(body, blow, monster, params, special)
		return "You blink away."
	end

combat.body_default.modify_blow =
	get_subsystem_param("combat_body_default", "modify_blow") or
	function(body, blow, monster)
		return false
	end

combat.body_default.get_attack_level =
	get_subsystem_param("combat_body_default", "get_attack_level") or
	function(body, blow, monster)
		return body.level
	end

combat.body_default.get_blow_power =
	get_subsystem_param("combat_body_default", "get_blow_power") or
	function(body, blow, monster)
		return blow.effect.power
	end

combat.body_default.get_blow_dam =
	get_subsystem_param("combat_body_default", "get_blow_dam") or
	function(body, blow, monster)
		return rng.roll(blow.d_dice, blow.d_side)
	end

-- Extra stuff to do when a hands hits
combat.body_default.on_hit =
	get_subsystem_param("combat_body_default", "on_hit") or
	function()
		return false 
	end

combat.body_default.STAT_STR = get_subsystem_param("combat_body_default",
												   "stat_str")


combat.body_default.energy = get_subsystem_param("combat_body_default",
												 "energy") or
	function() return get_player_energy(SPEED_GLOBAL) end

-- Resolve skill name afterwards
hook(hook.INFO_DATA_LOADED,
	function()
		combat.body_default.SKILL_BODY =
			find_skill(get_subsystem_param("combat_body_default",
										   "base_skill_name"))
	end
)

function combat.body_default.weapon_check(blow, prev_slots)
	if not blow.method.weapon then
		return false
	end

	if not combat.default_weapon then
		-- If we're not using a weapon system, we can't use weapon
		-- objects, so just use the monster's "innate" weapon
		blow.found = true
		return true
	end

	local part  = blow.method.slot
	local inven = player.inventory[part]
	local max   = flag_max_key(inven)
	local prev  = prev_slots[part]
	local slot  = nil
	local weap  = nil

	if not prev then
		prev = 1
	end

	for i = prev + 1, max do
		local obj = inven[i]

		if obj and obj.flags[FLAG_WEAPON] and
			not obj.flags[FLAG_NEVER_BLOW]
		then
			slot = i
			weap = obj
			break
		end
	end

	if not slot then
		for i = 1, prev do
			local obj = inven[i]

			if obj and obj.flags[FLAG_WEAPON] and
				not obj.flags[FLAG_NEVER_BLOW]
			then
				slot = i
				weap = obj
				break
			end
		end -- for i = 1, prev do
	end -- if not found then

	if slot then
		prev_slots[part] = slot
	end

	blow.found  = slot
	blow.weapon = true
	blow.part   = part
	blow.slot   = slot
	blow.obj    = weap

	if part == combat.body_default.INVEN_MAINHAND then
		blow.mainhand = true
	elseif part == combat.body_default.INVEN_OFFHAND then
		blow.offhand = true
	end

	return blow.found
end -- combat.body_default.weapon_check()

function combat.body_default.barehand_check(blow)
	if not blow.method.barehand then
		return 0
	end

	local slot1  = blow.method.hand1_slot
	local avail1 = player.inventory_limits(slot1) -
		flag_used(player.inventory[slot1]) + 1

	blow.avail  = avail1
	blow.avail1 = avail1

	if slot1 == combat.body_default.INVEN_MAINHAND then
		blow.mainhand = true
	elseif slot1 == combat.body_default.INVEN_OFFHAND then
		blow.offhand = true
	end

	if blow.method.hand2_slot then
		local slot2  = blow.method.hand2_slot
		local avail2 = player.inventory_limits(slot2) -
			flag_used(player.inventory[slot2]) + 1

		blow.avail  = min(avail1, avail2)
		blow.avail2 = avail2

		if slot2 == combat.body_default.INVEN_MAINHAND then
			blow.mainhand = true
		elseif slot2 == combat.body_default.INVEN_OFFHAND then
			blow.offhand = true
		end
	end

	blow.barehand = true

	return blow.avail
end -- combat.body_default.barehand_check()

combat.body_default.analyze_blows = get_subsystem_param("combat_body_default",
														"analyze_blows") or
	function(blow_table)
		local usable_blows   = {}
		local unusable_blows = {}
		local num_weapon     = 0
		local num_barehand   = 0
		local prev_slots     = {}

		for i = 1, getn(blow_table) do
			local blow = blow_table[i]

			if blow.method.weapon then
				if combat.body_default.weapon_check(blow, prev_slots) then
					tinsert(usable_blows, blow)
					num_weapon = num_weapon + 1
				else
					tinsert(unusable_blows, blow)
				end
			elseif blow.method.barehand then
				if combat.body_default.barehand_check(blow) >= 1 then
					tinsert(usable_blows, blow)
					num_barehand = num_barehand + 1
				else
					tinsert(unusable_blows, blow)
				end
			else
				-- Not barehand or weapon
				tinsert(usable_blows, blow)
			end
		end -- for i = 1, getn(blow_table) do

		-- Can't do barehanded and weapon styles at same time.
		local conflicting_blows = {}
		if num_barehand > 1 and num_weapon > 1 then
			blow_table   = usable_blows
			usable_blows = {}

			for i = 1, getn(blow_table) do
				local blow = blow_table[i]

				if blow.method.weapon then
					tinsert(usable_blows, blow)
				elseif blow.method.barehand then
					tinsert(conflicting_blows, blow)
				else
					tinsert(usable_blows, blow)
				end
			end
		end -- if num_barehand > 1 and num_weapon > 1 then

		return usable_blows, unusable_blows, conflicting_blows
	end -- analyze_blows()

combat.body_default.use_weapon_objs =
	get_subsystem_param("combat_body_default", "use_weapon_objs")

combat.body_default.weapon_test_hit =
	get_subsystem_param("combat_body_default",
						"weapon_test_hit") or combat.default_weapon.test_hit

combat.body_default.weapon_do_hit =
	get_subsystem_param("combat_body_default",
						"weapon_do_hit") or combat.default_weapon.do_hit

combat.body_default.do_weapon =
	get_subsystem_param("combat_body_default",
						"do_weapon") or
	function(blow, m_ptr, m_idx, y, x)
		assert(combat.body_default.use_weapon_objs,
			   "combat.body_default.use_weapon_objs not set")
		assert(blow.obj, "No weapon object to use.")

		local obj = blow.obj

		local m_name  = monster_desc(m_ptr, 0)

		combat.curr.weap = blow.obj

		local bonus = player.to_h + obj.to_h
		local chance = player.skill_thn +
			(bonus * combat.body_default.BTH_PLUS_ADJ)

		-- Save this for WEAP_DAM_DEALT, in case the
		-- monster dies and m_ptr is wiped
		local prev_hp = m_ptr.hp
		local r_idx   = m_ptr.r_idx
		local re_idx  = m_ptr.ego

		if combat.default_weapon.test_hit(obj, m_ptr, chance) then
			combat.default_weapon.do_hit(blow.obj, m_ptr, y, x, blow.part,
										 blow.slot, {})
			local dam_dealt
			if m_ptr.hp < 0 or m_ptr.r_idx == 0 then
				dam_dealt = prev_hp
			else
				dam_dealt = prev_hp - m_ptr.hp
			end

			hook.process(hook.WEAP_DAM_DEALT, obj, nil, m_ptr, r_idx, re_idx,
						 dam_dealt)
			item_hooks.process_all(obj, FLAG_WEAP_DAM_DEALT, nil, m_ptr, r_idx,
								   re_idx, dam_dealt)

		else
			message("You miss " .. m_name .. ".")
		end
		combat.curr.weap = nil
	end -- combat.body_default.do_weapon()

function combat.body_default.do_non_weapon(blow, m_ptr, m_idx, y, x)
	local c_ptr   = cave(y, x)
	local t_name  = monster_desc(m_ptr, 0)
	local special = {}
	local body    = player.body_monster

	local params = {
		m_ptr  = player,
		m_name = "you",
		effect = blow.effect,
		method = blow.method,
		t_ptr  = m_ptr,
		t_name = t_name,
		t_luck = 0
	}
	local power = combat.body_default.get_blow_power(body, blow, m_ptr)
	local level = combat.body_default.get_attack_level(body, blow, m_ptr)
	level = max(level, 1)

	local done = hook.process(hook.HANDLE_MONSTER_BLOW, params)

	if done then
		-- Done
	elseif blow.effect.name == "*" or
		combat.body_default.test_hit(power, level, m_ptr.ac)
	then
		-- Describe the attack method
		if blow.method.fct then
			blow.method.fct(special, blow.effect, m_idx)
		elseif blow.method.body then
			blow.method.body(special, blow.effect, m_idx)
		elseif blow.method.monster then
			blow.method.monster(special, blow.effect, m_idx)
		end

		-- Get the action
		local act = special.body_action or blow.method.body_action
		if not act then
			local parts = strsplit(blow.method.action, " ")

			if ends_with(parts[1], "es") then
				parts[1] = strsub(parts[1], 1, strlen(parts[1]) - 2)
				act = strjoin(parts, " ")
			elseif ends_with(parts[1], "s") then
				parts[1] = strsub(parts[1], 1, strlen(parts[1]) - 1)
				act = strjoin(parts, " ")
			else
				act = blow.method.desc .. " @target@"
			end
		end
		act = gsub(act, "(@target@)", t_name)
		act = gsub(act, "(@target_possessive@)", t_name .. "'s")

		message("You " .. act .. ".")

		params.dam = combat.body_default.get_blow_dam(body, blow, m_ptr,
													  special)

		if blow.effect.fct then
			blow.effect.fct(params, special, blow.method, m_idx)
		elseif blow.effect.body then
			blow.effect.body(params, special, blow.method, m_idx)
		elseif blow.effect.monster then
			blow.effect.monster(params, special, blow.method, m_idx)
		end

		if blow.effect.type > 0 then
			project(WHO_PLAYER, 0, y, x, params.dam, blow.effect.type,
					PROJECT_KILL | PROJECT_STOP | PROJECT_HIDE |
					PROJECT_HIDE_BLAST | PROJECT_NO_REFLECT)
		end

		if c_ptr.m_idx == 0 or m_ptr.r_idx == 0 then
			-- Monster died or vanished
			return true
		end

		-- Blow responses (passive attacks)
		if flag_exists(m_ptr.flags, FLAG_BLOW_RESPONSE) then
			local responses = m_ptr.flags[FLAG_BLOW_RESPONSE]

			for i = 1, flag_max_key(responses) do
				if responses[i] then
					local blow_response = get_blow_response(i)

					local max_dam = special.max_dam or
						rng.maxroll(blow.d_dice, blow.d_side)

					blow_response.weapon_blow(WHO_PLAYER, m_idx,
											  nil, nil, nil, nil,
											  1, 1,
											  max_dam,
											  responses[i],
											  flag_get2(responses, i))
				end -- if responses[i] then
			end -- for i = 1, flag_max_key(responses) do
		end -- Blow responses (passive attacks)

		if special.blinked then
			if player.has_intrinsic(FLAG_NO_TELE) or
				player.has_intrinsic(FLAG_RES_CONTINUUM) or
				has_flag(level_flags, FLAG_NO_TELEPORT)
			then
				message("You feel a brief wrenching sensation.")
			else
				local msg = 
					combat.body_default.get_blink_msg(body, blow, m_ptr,
													  params, special)
				message(msg)
				teleport_player(20 * 2 + 5)
				return true
			end
		end -- if special.blinked then

		if combat.body_default.on_hit(y, x, c_ptr, m_idx, m_ptr,
									  blow, params, special)
		then
			return true
		end

		if c_ptr.m_idx ~= 0 and m_ptr.r_idx > 0 then
			hook.process(hook.PLAYER_HIT_MON_POST, m_idx, m_ptr,
						 combat.body_default.SKILL_BODY, blow, params,
						 special)
		end
	else -- if not done and hit then
		message("You miss " .. t_name .. ".")
	end
end -- combat.body_default.do_non_weapon()

combat.body_default.change_weapons_mult =
	get_subsystem_param("combat_body_default",
						"change_weapons_mult") or
	function(blow_table, body, m_idx)
		for i = 1, getn(blow_table) do
			local blow = blow_table[i]

			blow.mult = 1 + get_flag(blow.obj, FLAG_BLOWS)
		end
	end -- change_weapons_mult()

combat.body_default.change_non_weapons_mult =
	get_subsystem_param("combat_body_default",
						"change_non_weapons_mult") or
	function(blow_table, body, m_idx)
		for i = 1, getn(blow_table) do
			local blow = blow_table[i]
			blow.mult = 1
		end
	end -- change_non_weapons_mult()

combat.body_default.get_global_extra_blows =
	get_subsystem_param("combat_body_default",
						"get_global_extra_blows") or
	function(blow_table, body, m_idx)
		return player.extra_blows
	end

combat.body_default.assign_global_extra_blows =
	get_subsystem_param("combat_body_default",
						"assign_global_extra_blows") or
	function(blow_table, body, m_idx)
		local global_blows =
			combat.body_default.get_global_extra_blows(blow_table, body, m_idx)

		-- Spread extra blows among attacks as evenly as possible
		local all    = global_blows / getn(blow_table)
		local remain = imod(global_blows, getn(blow_table))

		if all == 0 and remain == 0 then
			return
		end

		for i = 1, getn(blow_table) do
			local blow = blow_table[i]

			blow.mult = (blow.mult or 1) + all
		end

		if remain == 0 then
			return
		end

		-- Spread remainder at random among the attacks
		local local_table = {}
		for j = 1, getn(blow_table) do
			local_table[j] = blow_table[j]
		end

		for i = 1, remain do
			local idx  = rng(1, getn(local_table))
			local blow = tremove(local_table, idx)

			blow.mult = (blow.mult or 1) + 1
		end -- for i = 1, global_blows.num do
	end -- assign_global_extra_blows()

function combat.body_default.setup_blow_tables(body, blow_table, m_idx)
	local weapon_by_part = {}
	local weapon_table   = {}
	local non_weap_table = {}
	local max_blows_per_part = {}

	for i = 1, getn(blow_table) do
		local blow = blow_table[i]

		combat.body_default.modify_blow(blow, monster(m_idx))

		if blow.weapon then
			if combat.body_default.use_weapon_objs then
				weapon_by_part[blow.part] = weapon_by_part[blow.part] or {}
				tinsert(weapon_by_part[blow.part], blow)
				max_blows_per_part[blow.part] =
					(max_blows_per_part[blow.part] or 0) + 1
			else
				tinsert(non_weap_table, blow)
			end
		else
			tinsert(non_weap_table, blow)
		end
	end

	local blows_per_part = {}
	for part, max_blows in max_blows_per_part do
		local num_weaps = 0
		local num_parts = player.inventory_limits(part)

		local blows_per_part = max_blows / num_parts

		if imod(max_blows, num_parts) ~= 0 and wizard then
			message(color.LIGHT_RED,
					"WARNING: Mismatch between number of blows and " ..
						"number of " ..
						inventory_slot_names[part + 1].desc .. "s " ..
						"for " .. monster_desc(m_ptr, 256 | 128))
		end

		local inven = player.inventory[part]
		for i = 1, flag_max_key(inven) do
			local obj = inven[i]

			if obj.flags[FLAG_WEAPON] and not obj.flags[FLAG_NEVER_BLOW] then
				num_weaps = num_weaps + 1
			end
		end

		local num_blows = blows_per_part * num_weaps
		local weaps     = weapon_by_part[part]

		for i = 1, num_blows do
			tinsert(weapon_table, weaps[i])
		end
	end -- for part, max_blows in max_blows_per_part do

	blow_table = {}
	for i = 1, getn(weapon_table) do
		tinsert(blow_table, weapon_table[i])
	end
	for i = 1, getn(non_weap_table) do
		tinsert(blow_table, non_weap_table[i])
	end

	combat.body_default.change_weapons_mult(weapon_table, body, m_idx)
	combat.body_default.change_non_weapons_mult(non_weap_table, body, m_idx)
	combat.body_default.assign_global_extra_blows(blow_table, body, m_idx)

	-- Remove any blows whose multiplier has been reduced to nothing.
	for i = getn(blow_table), 1, -1 do
		local blow = blow_table[i]

		if blow.mult and blow.mult < 1 then
			tremove(blow_table, i)
		end
	end
	for i = getn(weapon_table), 1, -1 do
		local blow = weapon_table[i]

		if blow.mult and blow.mult < 1 then
			tremove(weapon_table, i)
		end
	end
	for i = getn(non_weap_table), 1, -1 do
		local blow = non_weap_table[i]

		if blow.mult and blow.mult < 1 then
			tremove(non_weap_table, i)
		end
	end
	return blow_table, weapon_table, non_weap_table
end -- combat.body_default.setup_blow_tables()

function combat.body_default.do_hit(m_ptr, y, x)
	local c_ptr  = cave(y, x)
	local m_idx  = c_ptr.m_idx
	local t_name = monster_desc(m_ptr, 0)

	local body       = player.body_monster
	local blow_table = combat.body_default.get_blows(body, m_idx)

	blow_table = combat.body_default.analyze_blows(blow_table, body, m_idx)

 	if getn(blow_table) == 0 then
		message("You have no usable ways of attacking the monster.")
		if m_ptr.ml then
			-- Don't need to take up energy for bumping into an
			-- invisible/unseen monster.
			player.energy = player.energy + combat.body_default.energy()
		end
		return combat.DONT_MOVE
	end

	local weapon_table, non_weap_table
	blow_table, weapon_table, non_weap_table = 
		combat.body_default.setup_blow_tables(body, blow_table, m_idx)

 	if getn(blow_table) == 0 then
		message("Strange, you can't seem to use any of your attacks.")
		return combat.DONT_MOVE
	end

	if combat.body_default.use_weapon_objs then
		randomize_list(weapon_table)

		for i = 1, getn(weapon_table) do
			local blow = weapon_table[i]

			for j = 1, (blow.mult or 1) do
				local ret = combat.body_default.do_weapon(blow, m_ptr,
														  m_idx, y, x)

				combat.anger_friend(c_ptr, m_ptr, t_name)

				if c_ptr.m_idx == 0 or m_ptr.r_idx == 0 then
					-- Monster died or vanished
					return combat.DONT_MOVE
				end

				if m_ptr.monfear > 0 and m_ptr.ml then
					message(strcap(t_name).." flees in terror!")
				end

				if ret then
					-- Blow code wants us to stop for some reason.
					return combat.DONT_MOVE
				end
			end -- for j = 1, (blow.mult or 1) do
		end -- for i = 1, getn(weapon_table) do
	end -- if combat.body_default.use_weapon_objs then

	randomize_list(non_weap_table)
	for i = 1, getn(non_weap_table) do
		local blow = non_weap_table[i]

		for j = 1, (blow.mult or 1) do
			local ret

			ret = combat.body_default.do_non_weapon(blow, m_ptr, m_idx, y, x)

			combat.anger_friend(c_ptr, m_ptr, t_name)

			if c_ptr.m_idx == 0 or m_ptr.r_idx == 0 then
				-- Monster died or vanished
				return combat.DONT_MOVE
			end

			if m_ptr.monfear > 0 and m_ptr.ml then
				message(strcap(t_name).." flees in terror!")
			end

			if ret then
				-- Blow code wants us to stop for some reason.
				return combat.DONT_MOVE
			end
		end -- for j = 1, (blow.mult or 1) do
	end -- for i = 1, getn(blow_table) do
end -- combat.body_default.do_hit()

combat.body_default.can_switch_to =
	get_subsystem_param("combat_body_default", "can_switch_to") or
	function(old_system, silent)
		local body       = player.body_monster
		local blow_table = combat.body_default.get_blows(body, WHO_PLAYER)

		local do_msg

		if silent then
			do_msg = function() end
		else
			do_msg = message
		end

		if getn(blow_table) == 0 then
			do_msg(color.LIGHT_RED,
					"ERROR: This body has no attacks!")
			return false
		end

		local usable_blows, unusable_blows, conflicting_blows =
			combat.body_default.analyze_blows(blow_table, body, WHO_PLAYER)

		if getn(usable_blows) > 0 then
			return true
		end

		local num_weapon   = 0
		local num_barehand = 0

		for i = 1, getn(unusable_blows) do
			local blow = unusable_blows[i]

			if blow.weapon then
				num_weapon = num_weapon + 1
			elseif blow.barehand then
				num_barehand = num_barehand + 1
			end
		end

		if num_weapon >= 1 then
			-- Player can weild a weapon after switching
			return true
		end

		if num_barehand >= 1 then
			do_msg("This body can only do barehanded attacks, yet " ..
				   "you have equipped something that interferes with " ..
					   "barehanded attacks.  Remove those items if you"..
					   "want to use this combat style.")
			return false
		end

		do_msg("You cannot use any of this body's innate attacks.")
		return false
	end -- can_switch_to()

combat.body_default.post_switch_to =
	get_subsystem_param("combat_body_default", "post_switch_to") or
	function(old_system, msgs, is_autoswitch)
		local body       = player.body_monster
		local blow_table = combat.body_default.get_blows(body, WHO_PLAYER)

		msgs = msgs or {}

		if getn(blow_table) == 0 then
			tinsert(msgs,
					"#RERROR#w: Switched to monster body combat style even " ..
						"though this body has no innate attacks at all!")
			return msgs
		end

		local usable_blows, unusable_blows, conflicting_blows =
			combat.body_default.analyze_blows(blow_table, body, WHO_PLAYER)

		local num_weapon   = 0
		local num_barehand = 0

		for i = 1, getn(unusable_blows) do
			local blow = unusable_blows[i]

			if blow.weapon then
				num_weapon = num_weapon + 1
			elseif blow.barehand then
				num_barehand = num_barehand + 1
			end
		end

		msgs = msgs or {}
		if getn(usable_blows) == 0 then
			if num_weapon > 0 then
				tinsert(msgs,
						"This body can wield a weapon; you should equip one.")
			elseif not is_autoswitch then
				message(color.LIGHT_RED,
						"ERROR: Switched to monster body combat style even " ..
							"though there are no usable attacks.")
				return
			end
		elseif getn(unusable_blows) > 0 then
			if num_weapon > 0 then
				tinsert(msgs,
						"This body can use more weapons than you have " ..
						"equipped.")
			end
			if num_barehand > 0 then
				tinsert(msgs,
						"This body could perform some (more) barehanded " ..
							"attacks, but you have things equipped " ..
							"preventing you from using them.")
			end
		end

		if getn(conflicting_blows) > 0 then
			tinsert(msgs,
					"You could perform some (more) barehanded attacks " ..
					"with this body, but you can't do barehanded combat " ..
						"while weilding weapons or holding shields.")
		end
		return msgs
	end -- post_switch_to()

combat.body_default.weapon_info =
	get_subsystem_param("combat_body_default", "weapon_info") or
	function(blow)
		local obj = blow.obj
		local local_min, local_max = 0, 0

		if is_known(obj) then
			local_min = local_min + obj.to_d
			local_max = local_max + obj.to_d
		end

		local comp_type = get_flag(obj, FLAG_DAM_COMPUTE)
				
		-- Compute the max damage
		local dams = obj.flags[FLAG_DAM]
		for i = 1, dams.size do
			if (dams.node[i].flags & FLAG_FLAG_USED) ~= 0
			then
				local dam_type  = dams.node[i].key
				local dam1      = flag_get(dams, dam_type)
				local dam2      = flag_get2(dams, dam_type)
				local min_weap, max_weap =
					combat.compute_roll[comp_type](dam1, dam2, true)
				local_min = local_min + min_weap
				local_max = local_max + max_weap
			end
		end
		return local_min * blow.mult, local_max * blow.mult
	end -- weapon_info()

combat.body_default.non_weapon_info =
	get_subsystem_param("combat_body_default", "non_weapon_info") or
	function(blow)
		return blow.d_dice * blow.mult, (blow.d_dice * blow.d_side *
										 blow.mult)
	end -- non_weapon_info()

combat.body_default.info =
	get_subsystem_param("combat_body_default", "info") or
	function(offhand, mult)
		if offhand then
			return "N/A"
		end

		mult = mult or 1

		local m_idx      = WHO_PLAYER
		local body       = player.body_monster
		local blow_table = combat.body_default.get_blows(body, m_idx)

		blow_table = combat.body_default.analyze_blows(blow_table, body,
													   m_idx)

		if getn(blow_table) == 0 then
			return "nil!"
		end

		local weapon_table, non_weap_table
		blow_table, weapon_table, non_weap_table = 
			combat.body_default.setup_blow_tables(body, blow_table, m_idx)

		if getn(blow_table) == 0 then
			return "nil!"
		end

		local num_blows = 0
		local total_min, total_max = 0, 0
		for i = 1, getn(blow_table) do
			local blow = blow_table[i]
			local tmp_min, tmp_max

			if blow.weapon and combat.body_default.use_weapon_objs then
				tmp_min, tmp_max = combat.body_default.weapon_info(blow)
			else
				tmp_min, tmp_max = combat.body_default.non_weapon_info(blow)
			end

			total_min = total_min + tmp_min
			total_max = total_max + tmp_max

			num_blows = num_blows + (blow.mult or 1)
		end

		if num_blows < 1 or (total_min == 0 and total_max == 0) then
			return "nil!"
		end

		local min = total_min * mult / num_blows
		local max = total_max * mult / num_blows

		return format("%d-%d", signed_number_tostring(min), abs(max))
	end -- info()

combat.body_default.num_blow =
	get_subsystem_param("combat_body_default", "num_blow") or
	function()
		local m_idx      = WHO_PLAYER
		local body       = player.body_monster
		local blow_table = combat.body_default.get_blows(body, m_idx)

		blow_table = combat.body_default.analyze_blows(blow_table, body,
													   m_idx)

		if getn(blow_table) == 0 then
			return 0
		end

		local weapon_table, non_weap_table
		blow_table, weapon_table, non_weap_table = 
			combat.body_default.setup_blow_tables(body, blow_table, m_idx)

		if getn(blow_table) == 0 then
			return 0
		end

		local num_blows = 0
		for i = 1, getn(blow_table) do
			local blow = blow_table[i]
			num_blows = num_blows + (blow.mult or 1)
		end

		return max(num_blows, 0)
	end -- combat.body_default.num_blow()

combat.body_default.COMBAT_BODY = add_combat_system
{
	name	= "Monster body fighting"
	desc	= "Uses the inate combat abilites of a monster's body.",
	skill	= get_subsystem_param("combat_body_default", "base_skill_name")

	energy	= function(...)
		return combat.body_default.energy(unpack(arg))
	end

	info = function(...)
		return combat.body_default.info(unpack(arg))
	end,

	available = function()
		return combat.body_default.available()
	end,

	can_switch_to = function(...)
		return combat.body_default.can_switch_to(unpack(arg))
	end

	post_switch_to = function(...)
		return combat.body_default.post_switch_to(unpack(arg))
	end

	attack = function(y, x, max)
		local fear = false
		local c_ptr = cave(y, x)
		local m_ptr = monster(c_ptr.m_idx)
		local r_ptr = race_info_idx(m_ptr.r_idx, m_ptr.ego)

		-- is the monster dead yet?
		local mdeath = false

		-- Extract monster name (or "it")
		local m_name = monster_desc(m_ptr, 0)

		-- Allow modules to check faction settings
		local ret = combat.check_faction(c_ptr.m_idx, m_ptr, y, x)
		if ret == combat.DONT_MOVE then return false
		elseif ret == combat.DO_MOVE then return true
		end

		if not combat.init_combat_turn(c_ptr.m_idx, m_name) then
			return false
		end

		-- For use in the MONSTER_DEATH hook
		combat.curr          = {}
		combat.curr.weap     = nil
		combat.curr.launcher = nil
		combat.curr.m_idx    = c_ptr.m_idx

		ret = combat.body_default.do_hit(m_ptr, y, x)

		combat.curr = nil

		if ret and ret == combat.DO_MOVE then
			return true
		end
		return false
	end,

	hooks	=
	{
		-- Calc how many blows to do
		[hook.CALC_BONUS_END] = function()
			if player.combat_style == combat.body_default.SKILL_BODY then
				player.num_blow = combat.body_default.num_blow()
			end
		end,

		[hook.TAKEOFF_POST] = function(obj, item)
			if item_slot_to_inven(item) == combat.body_default.INVEN_MAINHAND
				and not get_object(item) and
				get_combat_availability(combat.body_default.SKILL_BODY)
				and player.combat_style ~= combat.body_default.SKILL_BODY
			then
				if player.combat_style ~= combat.body_default.SKILL_BODY
				then
					player.combat_style = combat.body_default.SKILL_BODY
					message("Autoswitching to monster body combat mode.")
				end
			end
		end, -- [hook.TAKEOFF_POST]

		[hook.BODY_CHANGE_POST] = function(old_body, new_body)
			local style = combat.body_default.SKILL_BODY

			-- Style isn't available, or is already being used
			if not get_combat_availability(style) or
				player.combat_style == style
			then
				return
			end

			-- Don't use if we're holding a weapon
			local mainhand = combat.body_default.INVEN_MAINHAND
			for weap = 1, player.inventory_limits(mainhand) do
				local obj = player.inventory[mainhand][weap]
			
				if obj and has_flag(obj, FLAG_WEAPON) then
					return
				end
			end

			player.combat_style = style
			message("Autoswitching to monster body combat mode.")
		end,
	},
}
