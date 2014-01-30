-------------------------------------------------------
-------------------- Spell copy powers ----------------
-------------------------------------------------------
add_mkey
{
	mkey	= "Magic",
	type	= "skill",
	fct	=  spell_copy_to
}

-------------------------------------------------------
-------------------- Beorning powers ------------------
-------------------------------------------------------
add_mkey
{
	mkey	= "Bear-form",
	type	= "ability",
	fct	= function()
		if mimic.get_shape() == mimic.GREAT_BEAR then
			mimic.set_shape(mimic.NONE, 0)
			energy_use = get_player_energy(SPEED_MIMIC)
		elseif mimic.get_shape() == mimic.NONE then
			mimic.set_shape(mimic.GREAT_BEAR, player.lev / 2)
			energy_use = get_player_energy(SPEED_MIMIC)
		end
	end,
}

-- Hide bearform ability for non beornings
hook(hook.INIT_DONE, function()
	ability(AB_BEORNING).hidden = player.get_race().id ~= SUBRACE_BEORNING
end)


-------------------------------------------------------
-------------------- Mimicry skill --------------------
-------------------------------------------------------
mimic.actions =
{
	{
		name = "Shapeshift back to your normal form"
		exec = function()
			if mimic.get_shape() == mimic.ABOMINATION and turn - 10 * (5 + get_skill_scale(SKILL_MIMICRY, 50)) < mimic.timeout then
				message("You still cannot manage to gain back control of your body.")
				return
			end
			return true, mimic.NONE
		end
	}
	{
		name = "Shapeshift using your cloak"
		exec = function()
			local mobj = {}
			for_inventory_inven(player.inventory[INVEN_OUTER], function(obj)
				if obj.flags[FLAG_MIMIC_IDX] then %mobj.obj = obj end
			end)
			if not mobj.obj then
				message("You must wear a mimicry cloak to use this power.")
			else
				return true, mobj.obj.flags[FLAG_MIMIC_IDX]
			end
		end
	}
	{
		name = "Shapeshift into a Fire Elemental"
		allow = function() return has_ability(AB_FIRE_SHAPE) end
		exec = function()
			return true, mimic.FIRE_FORM
		end
	}
	{
		name = "Shapeshift into a Wind Elemental"
		allow = function() return has_ability(AB_AIR_SHAPE) end
		exec = function()
			return true, mimic.WIND_FORM
		end
	}
	{
		name = "Shapeshift into a Water Elemental"
		allow = function() return has_ability(AB_WATER_SHAPE) end
		exec = function()
			return true, mimic.WATER_FORM
		end
	}
	{
		name = "Shapeshift into a Stone Elemental"
		allow = function() return has_ability(AB_EARTH_SHAPE) end
		exec = function()
			return true, mimic.STONE_FORM
		end
	}
}
function mimic.action_shortinfo (y, x, a) if a then term.blank_print(a.name, y, x) end end

add_loadsave("mimic.timeout", 0)
hook(hook.BIRTH, function() mimic.timeout = 0 end)


add_mkey
{
	mkey	= "Mimicry",
	type	= "skill"
	fct	= function()
		local acts = {}
		for i = 1, getn(mimic.actions) do
			if not mimic.actions[i].allow or mimic.actions[i].allow() then
				mimic.actions[i].__index__ = i
				tinsert(acts, mimic.actions[i])
			end
		end
		local act = nil

		local ret, rep = repeat_keys.pull()
		if ret then
			if mimic.actions[rep] and (not mimic.actions[rep].allow or mimic.actions[rep].allow()) then
				act = mimic.actions[rep]
			end
		end

		act = act or choose_from_list(acts, "Mimicry actions", "Perform which mimicry action?", mimic.action_shortinfo)
		if act and act.exec then
			repeat_keys.push(act.__index__)

			local ret, shape = act.exec()
			if ret then
				if shape ~= mimic.NONE then
					if turn - 10 * (5 + get_skill_scale(SKILL_MIMICRY, 50)) < mimic.timeout then
						message("You cannot shapeshift again for "..1 + ((10 * (5 + get_skill_scale(SKILL_MIMICRY, 50)) - turn + mimic.timeout) / 10).." turns.")
						return
					end

					local chance = mimic.get_info(shape, "level") * 3
					chance = chance - get_skill_scale(SKILL_MIMICRY, 150) - (((player.stat(A_DEX) - 10) * 2) / 3)
					if chance < 5 then chance = 5
					elseif chance > 95 then chance = 95 end

					if chance > 75 then
						if not get_check("You feel uneasy with this shape-change. Try anyway?") then return end
					end
					if rng.percent(chance) then
						shape = mimic.ABOMINATION
						message("Your shape-change goes horribly wrong!")
						if rng.percent(player.skill_sav) then
							message("You manage to wrest your body back under control.")
						shape = mimic.NONE
						end
					end
					message("You shapeshift!")
				else
					message("You change back to your natural form.")
				end

				mimic.set_shape(shape, get_skill_scale(SKILL_MIMICRY, 50))
			      	energy_use = get_player_energy(SPEED_MIMIC)
				mimic.timeout = turn
			end
		end
	end,
}

-------------------------------------------------------
--------------------- Entish powers -------------------
-------------------------------------------------------
add_mkey
{
	mkey	= "Boulder-throwing",
	type	= "skill",
	fct	= function()
	end,
}


-------------------------------------------------------
------------------ Death Touch powers -----------------
-------------------------------------------------------
add_mkey
{
	mkey 	= "Touch of death"
        type	= "ability"
	fct 	= function()
			if player.csp() > 40 then
				increase_mana(-40)
				timed_effect(timed_effect.MELEE_PROJECT, rng(30) + 10, dam.INSTA_DEATH, 1, 0, PROJECT_STOP + PROJECT_KILL)
                                message("You are engulfed in a cloud of darkness.")
				energy_use = 100
			else
				message("You need at least 40 mana.")
			end
	end,
}

-------------------------------------------------------
------------------- Far Reach Attack ------------------
-------------------------------------------------------

add_mkey
{
	mkey 	= "Far reaching attack"
	type	= "ability"
	fct = function()
		-- Prevent macros from accessing it without the skill
		if not ability(AB_FAR_ATTACK).acquired then
			message(color.RED, "You haven't purchased this ability.")
			return false
		end

		-- Must have at least 1 polearm equipped
		local okay = false
		for w = 1, player.inventory_limits(INVEN_MAINHAND) do
			local obj = player.inventory[INVEN_MAINHAND][w]
				if obj and obj.tval == TV_POLEARM then
				okay = true
				break
			end
		end

		if not okay then
			message("You must have a polearm equipped to do a far reaching attack.")
			return false
		end

		-- Select the target
		local ret, dir

		ret, dir = get_aim_dir{max_range = 2}
		if not ret then return false end
		
		local tx, ty

		-- Hack -- Use an actual "target"
		if (dir == 5) then
			tx = target_col
			ty = target_row
		else
			-- Use the given direction
			local dy, dx = explode_dir(dir)
			ty = player.py + (dy * 99)
			tx = player.px + (dx * 99)
		end

		if distance(player.py, player.px, ty, tx) > 2 then
			message("Your polearm cannot reach that far.")
			return false
		elseif not projectable(player.py, player.px, ty, tx) then
			message("You must see your target.")
			return false
		end

		
		-- Finally attack with every equipped polearms
		local c_ptr = cave(ty, tx)

		if c_ptr.m_idx == 0 then
			message("There aren't any monster there")
			return false
		end

		local m_ptr = monster(c_ptr.m_idx)
		local r_ptr = race_info_idx(m_ptr.r_idx, m_ptr.ego)

		-- is the monster dead yet?
		local mdeath = false

		-- Extract monster name (or "it")
		local m_name = monster_desc(m_ptr, 0)

		local args = {}

		-- For all weapons
		for weap = 1, player.inventory_limits(combat.default_weapon.INVEN_MAINHAND) do
			local obj = player.inventory[combat.default_weapon.INVEN_MAINHAND][weap]

			-- Not a polearm
			if not obj or obj.tval ~= TV_POLEARM then break end

			-- Monster is already dead ? oh pitty :(
			if mdeath then break end

			if not obj then
				-- nothing
			elseif has_flag(obj, FLAG_NEVER_BLOW) or not has_flag(obj, FLAG_WEAPON) then
			else
				-- Calculate the "attack quality"
				local bonus = player.to_h + obj.to_h
				local chance = player.skill_thn +
					(bonus * combat.default_weapon.BTH_PLUS_ADJ)

				local num = iif(weap == 1, player.num_blow, 1)

				-- For use in the MONSTER_DEATH hook
				combat.curr          = {}
				combat.curr.weap     = obj
				combat.curr.launcher = nil
				combat.curr.m_idx    = c_ptr.m_idx

				-- For all blows
				for blow_nb = 1, num do
					local special = {}

					-- Test for hit
					if combat.default_weapon.test_hit(obj, m_ptr,
													  chance) then
						local prev_hp = m_ptr.hp

						-- Save this for WEAP_DAM_DEALT, in case the
						-- monster dies and m_ptr is wiped
						local r_idx  = m_ptr.r_idx
						local re_idx = m_ptr.ego

						combat.default_weapon.do_hit(obj, m_ptr, ty, tx, combat.default_weapon.INVEN_MAINHAND, weap, args)
						if c_ptr.m_idx == 0 then
							mdeath = true
						end

						local dam_dealt
						if m_ptr.hp < 0 or m_ptr.r_idx == 0 then
							dam_dealt = prev_hp
						else
							dam_dealt = prev_hp - m_ptr.hp
						end

						hook.process(hook.WEAP_DAM_DEALT, obj, nil, m_ptr,
									 r_idx, re_idx, dam_dealt)
						item_hooks.process_all(obj, FLAG_WEAP_DAM_DEALT,
												nil, m_ptr, r_idx, re_idx,
											   dam_dealt)

						if m_ptr.hp < 0 or m_ptr.r_idx == 0 then
							mdeath = true
							break
 						end

						if mdeath then
							break
						end
					else
						-- Player misses
						message("You miss "..m_name..".")
					end
					combat.anger_friend(c_ptr, m_ptr, m_name)
				end
			end
		end

		-- Use up energy
		energy_use = get_player_energy(SPEED_WEAPON)

	end -- fct
}


-------------------------------------------------------
--------------------- Ammo Creation -------------------
-------------------------------------------------------

add_mkey
{
	mkey 	= "Ammo creation"
	type	= "ability"
	fct 	= function()
		local old_level = object_level

		local ret, dir = get_rep_dir()
		if not ret then return end

		local y, x = explode_dir(dir)
		y, x = y + player.py, x + player.px

		local c_ptr = cave(y,x)

		-- Check for rubble
		if c_ptr.feat == FEAT_RUBBLE then
			cave_set_feat(y, x, FEAT_FLOOR)

			object_level = (dun_level + get_skill(SKILL_SLING)) / 2
			object_level = max(1, object_level)

			-- Never generate an artifact (a non-random one at least)
			-- And makes sure it is always "good" (enchanted)
			local obj = rand_obj.get_obj(object_level, 
				{function(k_idx) return k_info(k_idx).tval == TV_SHOT end},
				nil, 0, true)
			apply_magic(obj, object_level, false, true, false)
	
			if obj then
				message("You create "..object_desc(obj, 1, 0).." from the rubble.")
				drop_near(obj, 0, y, x)
			end
		else
			-- Check for a skeleton on top of the square
			if flag_max_key(c_ptr.inventory) == 0 then 
				message("There is nothing there to make ammo of.")
				return 
			end
			
			local index = flag_max_key(c_ptr.inventory)
			local source = c_ptr.inventory[index]

			if source.tval ~= TV_CORPSE or source.sval ~= SV_CORPSE_SKELETON then
				message("You can only forge arrows out of skeletal remains.")
				return
			end

			-- Use the monster level instead of the dungeon level
			object_level = (source.flags[FLAG_MONSTER_OBJ].level + 
			                get_skill(SKILL_BOW)) / 2
			object_level = max(1, object_level)
			
			-- Never generate an artifact (a non-random one at least)
			-- And makes sure it is always "good" (enchanted)
			local obj = rand_obj.get_obj(object_level, 
				{function(k_idx) return k_info(k_idx).tval == TV_ARROW end},
				nil, 0, true)
			apply_magic(obj, object_level, false, true, false)

			if obj then
				-- To ensure the singular
				local amt = source.number
				source.number = 1
				message("You create "..object_desc(obj, 1, 0).." from the "..
				        object_desc(source, 1, 0)..".")
				source.number = amt
			end

			-- First delete the source skeleton, before dropping the ammo
			source.number = source.number - 1
			if source.number == 0 then
				flag_remove(c_ptr.inventory, index)
			end
			
			if obj then
				drop_near(obj, 0, y, x)
			end
		end

		object_level = old_level

		-- Takes 5 turns to discourage use in combat situations
		energy_use = get_player_energy(SPEED_GLOBAL) * 5

		note_spot(y, x)
		player.redraw[FLAG_PR_MAP] = true
	end,
}


-------------------------------------------------------
---------------- Nicer casting interface --------------
-------------------------------------------------------
load_subsystem("easy_cast")
