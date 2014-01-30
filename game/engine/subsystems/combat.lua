-- The combat subsystem
--
-- Load it with: load_subsystem("combat")
--
-- Most module authors wont load this module as it it automatically included
-- by other combat subsystems. One wants to include it only when they are creating
-- a completly new combat subsystem.
-- People who want to change the default factions handling will want to load it separately(and before other combat systems)
-- to define the handler function
--
-- Parameters:
-- * check_faction: Function that checks for the attack validity against a monster. Returns either combat.DO_MOVE, combat.DONT_MOVE or combat.ATTACK
--                  Default to pushing past neutrals and allies and chatting with them if possible.

constant("combat", {})

-- Combat system
safe_new_flag("SKILL")
safe_new_flag("BASE_RANGE")
safe_new_flag("MULTIPLIER")
safe_new_flag("RETURN")
safe_new_flag("AMMO")
safe_new_flag("BREAKAGE_CHANCE")
safe_new_flag("ON_HIT")
safe_new_flag("ON_HIT_LEVEL")
safe_new_flag("DAM_COMPUTE")
safe_new_flag("BRAND")
safe_new_flag("SLAY")
safe_new_flag("SPEED_FURY")

hook.new_hook_type("TEST_HIT")
new_flag("TEST_HIT", true)

hook.new_hook_type("CRIT_HIT_CHANCE")
new_flag("CRIT_HIT_CHANCE", true)

hook.new_hook_type("CRIT_HIT_TYPE")
new_flag("CRIT_HIT_TYPE", true)

hook.new_hook_type("WEAP_DAM_DEALT")
new_flag("WEAP_DAM_DEALT", true)

hook.new_hook_type("WEAP_KILLED_MON")
new_flag("WEAP_KILLED_MON", true)

hook.new_hook_type("PLAYER_HIT_MON_POST")

-- How are damage rolls done
combat.COMPUTE_FLAT = 0
combat.COMPUTE_ROLL = 1
combat.compute_roll =
{
	[combat.COMPUTE_FLAT] = function(min, max, range)
		if not range then
			return rng(min, max)
		else
			return min, max
		end
	end
	[combat.COMPUTE_ROLL] = function(min, max, range)
		if not range then
			return rng.roll(min, max / min)
		else
			return min, max * min
		end
	end
}

hook.new_hook_type("INIT_COMBAT_PRE")
hook.new_hook_type("INIT_COMBAT_POST")

-- Continue moving instead of attacking(probably because of pushing past monster)
combat.DO_MOVE = 1
-- Dont move but dont attack
combat.DONT_MOVE = 2
-- ATTACK FIGHT KILL !
combat.ATTACK = 3

-- Check how to handle faction standings
combat.check_faction = get_subsystem_param("combat", "check_faction") or
function(m_idx, monst, y, x)
	local m_name = monster_desc(monst, 0)

	-- Can we chat ?
	if (is_friend(monst) >= 0) and has_flag(monst, FLAG_CHATABLE) and get_check("Talk to "..m_name.."?") then
		talk_to_monster(m_idx)
		return combat.DONT_MOVE
	-- Attack -- only if we can see it OR it is not in a wall
	elseif (is_friend(monst) >= 0) and not (player.has_intrinsic(FLAG_CONFUSED) or player.has_intrinsic(FLAG_HALLUCINATE) or not monst.ml) and player_can_enter(cave(y, x).feat) then
		if player_monster_swap(monst) then
			return combat.DO_MOVE
		else
			return combat.DONT_MOVE
		end
	end

	-- Stop if friendly
	if is_friend(monst) >= 0 and not player.has_intrinsic(FLAG_CONFUSED) and not player.has_intrinsic(FLAG_HALLUCINATE) and monst.ml then
		message("You stop to avoid hitting "..m_name..".")
		return combat.DONT_MOVE
	end
	return combat.ATTACK
end

-- Do various initilizatrion stuff for combat system
function combat.init_combat_turn(m_idx, m_name)
	local monst = monster(m_idx)

	-- Disturb the monster
	monst.csleep = 0

	-- Disturb the player
	term.disturb(0, 0)

	if player.has_intrinsic(FLAG_NEVER_BLOW) then
		local msg = player.intrinsic(FLAG_NEVER_BLOW)

		if type(msg) == "string" and msg ~= "" then
			message(msg)
		else
			message("You are unable to attack for some reason.")
		end
	end

	-- Auto-Recall if possible and visible
	if monst.ml then monster_race_track(monst.r_idx, monst.ego) end

	-- Track a new monster
	if monst.ml then health_track(m_idx) end

	-- Handle player fear
	if player.has_intrinsic(FLAG_FEAR) then
		-- Message
		if monst.ml then
			message("You are too afraid to attack "..m_name.."!")
		else
			message("There is something scary in your way!")
		end
		return nil
	end

	if hook.process(hook.INIT_COMBAT_PRE, monst, m_name) then
		return false
	end

	hook.process(hook.INIT_COMBAT_POST, monst, m_name)

	return true
end  -- combat.init_combat_turn()

-----------------------------------------------------------------------
-- Slays and brands
-----------------------------------------------------------------------

combat.slays    = {}
combat.slay_max = 1

function combat.new_slay(name, desc, target, tester_func)
	declare_global_constants{"SLAY_"..name}
	setglobal("SLAY_"..name, combat.slay_max)

	local slay = {
		func   = tester_func,
		name   = name,
		desc   = desc,
		target = target,
		idx    = combat.slay_max
	}

	combat.slays[combat.slay_max] = slay

	combat.slay_max = combat.slay_max + 1
end -- combat.new_slay()

function combat.compute_damage(obj, mon, args)
	local dams        = obj.flags[FLAG_DAM]
	local base_dams   = {}
	local base_types  = {}
	local final_dams  = {}
	local final_types = {}
	local resists     = mon.flags[FLAG_RESIST] or flag_new()
	local num_types   = 1

	-- Compute base damages
	for i = 1, dams.size do
		if (dams.node[i].flags & FLAG_FLAG_USED) ~= 0 then
			local dam_type = dams.node[i].key
			local func = combat.compute_roll[get_flag(obj, FLAG_DAM_COMPUTE)]
			local dam  = func(flag_get(dams, dam_type),
							  flag_get2(dams, dam_type))

			base_dams[num_types]  = dam
			final_dams[num_types] = dam
			base_types[num_types] = dam_type
			final_types[num_types] = dam_type

			num_types = num_types + 1
		end
	end

	-- Apply brands
	local brands  = obj.flags[FLAG_BRAND] or flag_new()
	for i = 1, brands.size do
		if (brands.node[i].flags & FLAG_FLAG_USED) ~= 0 then
			local dam_type = brands.node[i].key
			local mult     = brands[dam_type]
			local resist   = resists[dam_type] or 0
			local new_dam

			for j = 1, getn(base_dams) do
				-- If a brand is applied, then the damage type delt
				-- is changed from the base type to the brand type,
				-- so we have to apply the resistence here, and
				-- project() will handle the resistence to the
				-- brand damage type.
				local base_resist = resists[base_types[j]] or 0
				-- Here we compute the resist to the base type into the new damage because we will change the damage type latter
				new_dam = base_dams[j] * ((100 - base_resist) / 100) * mult
				if (new_dam * (100 - resist) / 100) > final_dams[j] then
					final_dams[j]  = new_dam
					final_types[j] = dam_type
				end
			end
		end
	end

	-- Apply slays
	local slays = obj.flags[FLAG_SLAY] or flag_new()
	for i = 1, slays.size do
		if (slays.node[i].flags & FLAG_FLAG_USED) ~= 0 then
			local slay    = slays.node[i].key
			local mult    = slays[slay]
			local new_dam

			-- Does this slay apply to this monster?
			if combat.slays[slay].func(mon) then
				for j = 1, getn(base_dams) do
					-- If a brand is applied, it multiplies the damage
					-- while leaving the base damage type alone.  If
					-- a brand has already been applied, and this slay
					-- does more damage than the brand, then we change
					-- the damage type back from the brand damage type
					-- to the base damage type.
					local base_resist = resists[base_types[j]] or 0
					-- Here we do NOT compute the resist to the base type into the new damage because we will NOT change the damage type latter
					-- However we still compute it in the test to be sure to test the real damage to be done
					-- Ans thus correctly override brands if needed
					new_dam = base_dams[j] * mult
					if new_dam * ((100 - base_resist) / 100) > final_dams[j] then
						final_dams[j]  = new_dam
						final_types[j] = base_types[j]
					end
				end
			end
		end
	end

	-- Apply speed furies
	local furies = obj.flags[FLAG_SPEED_FURY] or flag_new()
	for i = 1, furies.size do
		if (furies.node[i].flags & FLAG_FLAG_USED) ~= 0 then
			local fury    = furies.node[i].key
			local mult    = flag_get(furies, fury)
			local div     = flag_get2(furies, fury)

			-- Does this apply to this monster?
			if combat.slays[fury].func(mon) then
				args.speed_mult = mult
				args.speed_div = div
			end
		end
	end

	local max_damage = 0
	local sum_dams   = {}

	-- A brand might have folded multiple base damage types into
	-- a single damage type, so we have to add them up here
	for i = 1, getn(final_dams) do
		local dam_type = final_types[i]
		local dam      = final_dams[i]

		max_damage = max_damage + dam

		sum_dams[dam_type] = (sum_dams[dam_type] or 0) + dam
	end

	local out_dams = {}

	for i = 1, getn(sum_dams) do
		if sum_dams[i] then
			tinsert(out_dams, {amnt = sum_dams[i], type = i})
		end
	end

	return out_dams, max_damage
end -- combat.compute_damage()

function combat.anger_friend(c_ptr, m_ptr)
	if not m_ptr then
		return
	end

	if m_ptr.hp < 0 or m_ptr.r_idx < 0 then
		return
	end

	local m_name = monster_desc(m_ptr, 0)


	if is_friend(m_ptr) == 1 or is_friend(m_ptr) == 0 then
		local new_faction = factions.get_hostile_faction(m_ptr)
		if factions.change_faction(m_ptr, new_faction) and
			player_can_see_bold(m_ptr.fy, m_ptr.fx)
		then
			message(format("%s gets angry!", strcap(m_name)))
		end
	end
end

function combat.handle_on_hit_spells(obj, inven_idx, item_idx, y, x)
	if obj.flags[FLAG_ON_HIT] then
		local spl = get_flag(obj, FLAG_ON_HIT)
		local chance = get_flag2(obj, FLAG_ON_HIT)
		local lvl = get_flag(obj, FLAG_ON_HIT_LEVEL)

		if rng.percent(chance) then
			set_auto_cast(WHO_PLAYER, lvl, y, x)
			execute_school_spell(spl, "player on hit weapon", FACTION_PLAYER, obj, inven_idx, item_idx)
			unset_auto_cast()
		end
	end
end

new_flag("OBJ_EXP_MON_KILL_ACTIVE")
new_flag("OBJ_EXP_MON_KILL_PASSIVE")

function combat.obj_exp_from_death(obj, mon, flag)
	local div = player.max_plv
	local exp = r_info[mon.r_idx + 1].mexp * mon.level / div

	if flag_get2(obj.flags, flag) ~= 0 then
		local func
		func = get_function_registry_from_flag(obj.flags, flag)
		exp = func(exp) or 0
	else
		exp = exp * obj.flags[flag] / 2
	end

	obj.exp = obj.exp + exp
	check_experience_obj(obj)
end

--
-- Use HOOK_MONSTER_DEATH to do WEAP_KILLED_MON, since if we wait until
-- after project() is done, m_ptr will be wiped clean
--
hook(hook.MONSTER_DEATH,
 function(m_idx)
	 combat.killing_weap = nil

	 -- Only invoke WEAP_KILLED_MON if the monster dying is the one we're
	 -- attacking with the weapon.  If hitting the monster lead to any
	 -- side effects which killed other monsters, ignore those.
	 if combat.curr and combat.curr.m_idx == m_idx and combat.curr.weap then
		 combat.killing_weap = combat.curr.weap

		 local mon = monster(m_idx)
		 hook.process(hook.WEAP_KILLED_MON, combat.curr.weap, combat.curr.launcher, mon)
		 item_hooks.process_all(combat.curr.weap, FLAG_WEAP_KILLED_MON, combat.curr.launcher, mon)
		 if combat.curr.launcher then
			 item_hooks.process_all(combat.curr.launcher, FLAG_WEAP_KILLED_MON, combat.currweap, mon)
		 end

		 -- Does weapon gain exp from killing monsters?
		 local obj = combat.curr.weap
		 if obj and obj.flags[FLAG_OBJ_EXP_MON_KILL_ACTIVE] then
			 if is_friend(mon) <= 0 then
				 combat.obj_exp_from_death(obj, mon, FLAG_OBJ_EXP_MON_KILL_ACTIVE)
			 end
		 end

		 -- Does the launcher gain exp?
		 obj = combat.curr.weap.launcer
		 if obj and obj.flags[FLAG_OBJ_EXP_MON_KILL_ACTIVE] then
			 if is_friend(mon) <= 0 then
				 combat.obj_exp_from_death(obj, mon, FLAG_OBJ_EXP_MON_KILL_ACTIVE)
			 end
		 end

		 -- Any equipment that gains exp from the player killing a monster
		 -- with a weapon, even if the equipment wasn't what did the
		 -- killing?
		 local state = { num = 1, list = {} }

		 for_inventory(player, INVEN_PACK, INVEN_TOTAL, function(obj)
			local state = %state
			if obj.flags[FLAG_OBJ_EXP_MON_KILL_PASSIVE] then
				state.num = state.num + 1
				tinsert(state.list, obj)
 			end
		 end)

		 for i = 1, getn(state.list) do
			 local obj = state.list[i]

			 obj.exp = obj.exp + (exp / state.num)
			 check_experience_obj(obj)
		 end
	 end -- if combat.curr and combat.curr.m_idx == m_idx then
end)
