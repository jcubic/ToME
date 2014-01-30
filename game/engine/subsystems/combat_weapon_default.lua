-- The default weapon combat subsystem
--
-- Load it with: load_subsystem("combat_weapon_default")
--
-- This is the "default" combat system as it exists for ToME(a derivate of angband one)
--
-- Parameters:
-- * stat_str: The stat that represents character strength
-- * stat_dev: The stat that represents character dexterity
-- * base_skill_name: The name of the skill which defines this combat method
-- * dual_wield: function which allows or forbids dual wielding(defaults to forbid)
-- * on_hit: function which will be called when a blow successfully landed (default to nothing)
-- * max_blows: function which must return a number of blows to add to the default ones(4 usualy) (defaults to 0)
-- * skill_bonus: function that is called when the correct weapon is wielded, add bonuses as needed
-- * energy_use: function called after the attack to determine the amount of energy expended

-- Load the combat subsystem which defines some basic stuff for all
-- combat systems
load_subsystem("combat")

new_flag("DUAL_WIELD")

-- Make the namespace
combat.default_weapon = {}

combat.default_weapon.BTH_PLUS_ADJ = 3

-- Can dual wield ? if so return the slot(as given by get_slot()), otherwise return null
combat.default_weapon.dual_wield = get_subsystem_param("combat_weapon_default", "dual_wield") or function() return nil end

-- Function that handles weapon skill bonuses
combat.default_weapon.skill_bonus = get_subsystem_param("combat_weapon_default", "skill_bonus") or function()
	local lev = get_skill(combat.default_weapon.get_weaponmastery_skill())

	player.to_h = player.to_h + lev
	player.to_d = player.to_d + (lev / 2)
end

-- Did we hit the monster?
combat.default_weapon.test_hit = get_subsystem_param("combat_weapon_default",
													 "test_hit") or
	function() return false end

-- By how much do we increase damage because of a critical hit?
-- Return 1, 1, 0 for no change to damage.
combat.default_weapon.critical_hit =
	get_subsystem_param("combat_weapon_default",
						"critical_hit") or function() return 1, 1, 0 end

-- Extra stuff to do when a weapon hits
combat.default_weapon.on_hit = get_subsystem_param("combat_weapon_default", "on_hit") or
	function() return false end

-- Maximum number of blows with this weapon
combat.default_weapon.max_blows = get_subsystem_param("combat_weapon_default", "max_blows") or
	function() return 0 end


combat.default_weapon.STAT_STR = get_subsystem_param("combat_weapon_default", "stat_str")
combat.default_weapon.STAT_DEX = get_subsystem_param("combat_weapon_default", "stat_dex")

combat.default_weapon.strength_blow_modifier = get_subsystem_param("combat_weapon_default", "strength_blow_modifier") or
function(mul, div)
	return (player.adj_str_blow[1 + player.stat(combat.default_weapon.STAT_STR)] * mul) / div
end
combat.default_weapon.dexterity_blow_modifier = get_subsystem_param("combat_weapon_default", "dexterity_blow_modifier") or
function()
	return player.adj_dex_blow[1 + player.stat(combat.default_weapon.STAT_STR)]
end

combat.default_weapon.INVEN_MAINHAND = get_subsystem_param("combat_weapon_default", "mainhand_slot") or 0
combat.default_weapon.INVEN_OFFHAND = get_subsystem_param("combat_weapon_default", "offhand_slot") or 0

-- This table is used to help calculate the number of blows the player can
-- make in a single round of attacks (one player turn) with a normal weapon.
--
-- This number ranges from a single blow/round for weak players to up to six
-- blows/round for powerful warriors.
--
-- Note that certain artifacts and ego-items give "bonus" blows/round.
--
-- First, from the player class, we extract some values:
--
--    Warrior --> num = 6; mul = 5; div = MAX(30, weapon_weight);
--    Mage    --> num = 4; mul = 2; div = MAX(40, weapon_weight);
--    Priest  --> num = 5; mul = 3; div = MAX(35, weapon_weight);
--    Rogue   --> num = 5; mul = 3; div = MAX(30, weapon_weight);
--    Ranger  --> num = 5; mul = 4; div = MAX(35, weapon_weight);
--    Paladin --> num = 5; mul = 4; div = MAX(30, weapon_weight);
--
-- To get "P", we look up the relevant "player.adj_str_blow[]" (see above),
-- multiply it by "mul", and then divide it by "div", rounding down.
--
-- To get "D", we look up the relevant "player.adj_dex_blow[]" (see above),
-- note especially column 6 (DEX 18/101) and 11 (DEX 18/150).
--
-- The player gets "blows_table[P][D]" blows/round, as shown below,
-- up to a maximum of "num" blows/round, plus any "bonus" blows/round.
combat.default_weapon.blows_table =
{
	-- P/D
	-- 0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11+

	-- 0
	{ 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3 },

	-- 1
	{ 1, 1, 1, 1, 2, 2, 3, 3, 3, 4, 4, 4 },

	-- 2
	{ 1, 1, 2, 2, 3, 3, 4, 4, 4, 5, 5, 5 },

	-- 3
	{ 1, 2, 2, 3, 3, 4, 4, 4, 5, 5, 5, 5 },

	-- 4
	{ 1, 2, 2, 3, 3, 4, 4, 5, 5, 5, 5, 5 },

	-- 5
	{ 2, 2, 3, 3, 4, 4, 5, 5, 5, 5, 5, 6 },

	-- 6
	{ 2, 2, 3, 3, 4, 4, 5, 5, 5, 5, 5, 6 },

	-- 7
	{ 2, 3, 3, 4, 4, 4, 5, 5, 5, 5, 5, 6 },

	-- 8
	{ 3, 3, 3, 4, 4, 4, 5, 5, 5, 5, 6, 6 },

	-- 9
	{ 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6 },

	-- 10
	{ 3, 3, 4, 4, 4, 4, 5, 5, 5, 6, 6, 6 },

	-- 11+
	{ 3, 3, 4, 4, 4, 4, 5, 5, 6, 6, 6, 6 },
}


-- Returns the blow information
function combat.default_weapon.analyze_blow()
	local num = player.descriptor.blow_num;
	local wgt = player.descriptor.blow_wgt;
	local mul = player.descriptor.blow_mul;
	return num + combat.default_weapon.max_blows(), wgt, mul
end

-- Are all the weapons wielded of the right type ?
function combat.default_weapon.get_weaponmastery_skill()
	local s

	s = 0

	-- All weapons must be of the same type
	for i = 1, player.inventory_limits(combat.default_weapon.INVEN_MAINHAND) do
		local obj = player.inventory[combat.default_weapon.INVEN_MAINHAND][i]

		if obj then
			if s == 0 or s == get_flag(obj, FLAG_SKILL) then
				s = get_flag(obj, FLAG_SKILL)
			else
				s = -1
			end
		end
	end

	-- Everything is ok
	return s
end

-- Resolve skill name afterwards
hook(hook.INFO_DATA_LOADED,
	function()
		combat.default_weapon.SKILL_MASTERY = find_skill(get_subsystem_param("combat_weapon_default", "base_skill_name"))
	end
)

function combat.default_weapon.do_hit(obj, m_ptr, y, x, inven_idx, item_idx, args)
	local c_ptr = cave(y, x)

	-- Basic damage
	local k = 1

	local add_damage       = player.to_d
	local remaining_damage = add_damage

	local dams, max_damage, computed_damages
	local crit_mult, crit_div, crit_bonus

	-- Is the weapon real
	if obj.k_idx ~= 0 then
		add_damage = add_damage + obj.to_d
		remaining_damage = min(add_damage, m_ptr.hp)

		if has_flag(obj, FLAG_DAM) then
			-- Any effects upon blow ?
			combat.default_weapon.on_hit(obj, m_ptr, args)

			-- Did the hook kill the monster or make it disappear?
			if c_ptr.m_idx == 0 then
				return
			end

			-- On hit effects
			combat.handle_on_hit_spells(obj, inven_idx, item_idx, y, x)

			-- Did the hook kill the monster or make it disappear?
			if c_ptr.m_idx == 0 then
				return
			end

			dams = obj.flags[FLAG_DAM]

			-- Compute the max damage
			computed_damages, max_damage =
				combat.compute_damage(obj, m_ptr, args)

			crit_mult, crit_div, crit_bonus =
				combat.default_weapon.critical_hit(obj, m_ptr,
												   combat.default_weapon.SKILL_MASTERY)

			add_damage = add_damage + crit_bonus

			max_damage = max_damage + add_damage

			if max_damage == 0 then
				return
			end

			-- Now apply the damage
			for i = 1, getn(computed_damages) do
				local dam_type = computed_damages[i].type

				k = computed_damages[i].amnt * crit_mult / crit_div

				-- Add a percent of the additional damage
				local add_k = (add_damage * k) / max_damage
				k = k + add_k
				remaining_damage = remaining_damage - add_k

				-- No negative damage
				if k < 0 then k = 0 end

				if (wizard) then
					message("You do "..k.." damage (" ..
							get_dam_type_info(dam_type, "desc") .. ")")
				end

				-- Damage, check for fear and death
				-- All damage doing uses project
				local proj_flags = (PROJECT_JUMP | PROJECT_HIDE |
							PROJECT_HIDE_BLAST | PROJECT_STOP |
							PROJECT_KILL | PROJECT_NO_REFLECT)

				-- Add silent flag forall but the last
				if i < getn(computed_damages) then
					proj_flags = proj_flags | PROJECT_SILENT
				end

				project(0, 0, y, x, k, dam_type, proj_flags)

				if c_ptr.m_idx == 0 then
					break
				end
			end
		end

		if c_ptr.m_idx ~= 0 and m_ptr.r_idx > 0 and remaining_damage > 0 then
			if (wizard) then
				message("You do ".. remaining_damage.." damage (" ..
						"bonuses)")
			end

			-- If the damage from the bonuses still haven't been applied,
			-- then apply them as pure damage
			project(0, 0, y, x, remaining_damage, dam.DEFAULT,
					PROJECT_JUMP | PROJECT_HIDE | PROJECT_HIDE_BLAST |
					PROJECT_STOP | PROJECT_KILL | PROJECT_NO_REFLECT)
		end

		-- Blow responses (passive attacks)
		if c_ptr.m_idx ~= 0 and has_flag(m_ptr, FLAG_BLOW_RESPONSE) then
			local responses = m_ptr.flags[FLAG_BLOW_RESPONSE]

			for i = 1, flag_max_key(responses) do
				if responses[i] then
					local blow_response = get_blow_response(i)

					blow_response.weapon_blow(WHO_PLAYER, c_ptr.m_idx,
											  obj, nil, dams,
											  computed_damages,
											  crit_mult, crit_div,
											  max_damage,
											  responses[i],
											  flag_get2(responses, i))
				end
			end
		end -- Blow responses (passive attacks)
		if c_ptr.m_idx ~= 0 and m_ptr.r_idx > 0 then
			hook.process(hook.PLAYER_HIT_MON_POST, c_ptr.m_idx, m_ptr,
						 combat.default_weapon.SKILL_MASTERY, obj)
		end
	end
end -- combat.default_weapon.do_hit()

-- Are we in dual wield mode ?
function combat.default_weapon.is_dual_wield()
	if player.combat_style == combat.default_weapon.SKILL_MASTERY then
		for weap = 1, player.inventory_limits(combat.default_weapon.INVEN_OFFHAND) do
			local obj = player.inventory[combat.default_weapon.INVEN_OFFHAND][weap]
			if obj and not has_flag(obj, FLAG_NEVER_BLOW) and has_flag(obj, FLAG_WEAPON) and has_flag(obj, FLAG_DUAL_WIELD) then
				return true
			end
		end
	end
end

combat.default_weapon.COMBAT_WEAPON = add_combat_system
{
	name	= "Weapon fighting"
	desc	= "Uses various weapons to inflict pain and suffering."
	skill	= get_subsystem_param("combat_weapon_default", "base_skill_name")
	energy	= get_subsystem_param("combat_weapon_default", "energy_use") or
		function(moved, args)
			args.speed_mult = args.speed_mult or 1
			args.speed_div = args.speed_div or 1
			return (get_player_energy(SPEED_WEAPON) * args.speed_mult) / args.speed_div
		end

	info    = function(offhand, mult)
		mult = mult or 1
		local dambonus = player.dis_to_d
		-- Start with the base (known) damage
		local min, max = dambonus, dambonus

		local inven = iif(offhand, combat.default_weapon.INVEN_OFFHAND, combat.default_weapon.INVEN_MAINHAND)

		-- Access the weapons
		for w = 1, player.inventory_limits(inven) do
			local obj = player.inventory[inven][w]
			local local_min, local_max = dambonus, dambonus

			if offhand then
				if not obj then return "nil!"
				elseif has_flag(obj, FLAG_NEVER_BLOW) or not has_flag(obj, FLAG_WEAPON) or not has_flag(obj, FLAG_DUAL_WIELD) then
					return "nil!"
				end
			else
				if not obj then return "nil!"
				elseif has_flag(obj, FLAG_NEVER_BLOW) or not has_flag(obj, FLAG_WEAPON) then
					return "nil!"
				end
			end

			if is_known(obj) then
				local_min = local_min + obj.to_d
				local_max = local_max + obj.to_d
			end

			-- Compute the max damage
			local dams = obj.flags[FLAG_DAM]
			for i = 1, dams.size do	if (dams.node[i].flags & FLAG_FLAG_USED) ~= 0 then
				local dam_type = dams.node[i].key
				local min_weap, max_weap = combat.compute_roll[get_flag(obj, FLAG_DAM_COMPUTE)](flag_get(dams, dam_type), flag_get2(dams, dam_type), true)
				local_min, local_max = local_min + min_weap, local_max + max_weap
			end end

			min = local_min
			max = local_max
		end

		local desc
		if min == 0 and max == 0 then
			desc = "nil!"
		else
			desc = format("%d-%d", signed_number_tostring(min * mult), abs(max * mult));
		end
		return desc
	end,

	available = function()
		if player.inventory_limits(combat.default_weapon.INVEN_MAINHAND) == 0 then return false end
		return true
	end,

	attack	  = function(y, x, max)
		local fear = false
		local c_ptr = cave(y, x)
		local m_ptr = monster(c_ptr.m_idx)
		local r_ptr = race_info_idx(m_ptr.r_idx, m_ptr.ego)

		-- is the monster dead yet?
		local mdeath = false
		-- number of blows done by the player
		local num_blows = 0

		-- Extract monster name (or "it")
		local m_name = monster_desc(m_ptr, 0)

		-- Allow modules to check faction settings
		local ret = combat.check_faction(c_ptr.m_idx, m_ptr, y, x)
		if ret == combat.DONT_MOVE then return false
		elseif ret == combat.DO_MOVE then return true
		end

		if not combat.init_combat_turn(c_ptr.m_idx, m_name) then return false end

		local args = {}

		-- For all weapons
		for weap = 1, player.inventory_limits(combat.default_weapon.INVEN_MAINHAND) do
			local obj = player.inventory[combat.default_weapon.INVEN_MAINHAND][weap]

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
				if max > -1 and num > max then num = max end

				-- For use in the MONSTER_DEATH hook
				combat.curr          = {}
				combat.curr.weap     = obj
				combat.curr.launcher = nil
				combat.curr.m_idx    = c_ptr.m_idx

				-- For all blows
				for blow_nb = 1, num do
					num_blows = num_blows + 1

					local special = {}

					-- Test for hit
					if combat.default_weapon.test_hit(obj, m_ptr,
													  chance) then
						local prev_hp = m_ptr.hp

						-- Save this for WEAP_DAM_DEALT, in case the
						-- monster dies and m_ptr is wiped
						local r_idx  = m_ptr.r_idx
						local re_idx = m_ptr.ego

						combat.default_weapon.do_hit(obj, m_ptr, y, x, combat.default_weapon.INVEN_MAINHAND, weap, args)
						if c_ptr.m_idx == 0 then
							mdeath = true
						else
							fear = m_ptr.monfear
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

		-- For all off hand weapons
		for weap = 1, player.inventory_limits(combat.default_weapon.INVEN_OFFHAND) do
			local obj = player.inventory[combat.default_weapon.INVEN_OFFHAND][weap]

			-- Monster is already dead ? oh pitty :(
			if mdeath then break end

			if not obj then
				-- nothing
			elseif has_flag(obj, FLAG_NEVER_BLOW) or not has_flag(obj, FLAG_WEAPON) or not has_flag(obj, FLAG_DUAL_WIELD) then
			else
				-- Calculate the "attack quality"
				local bonus = player.to_h + obj.to_h
				local chance = player.skill_thn +
					(bonus * combat.default_weapon.BTH_PLUS_ADJ)

				local num = iif(weap == 1, player.num_blow, 1)
				if max > -1 and num > max then num = max end
				if num >= 2 then num = num / 2 end

				-- For use in the MONSTER_DEATH hook
				combat.curr          = {}
				combat.curr.weap     = obj
				combat.curr.launcher = nil
				combat.curr.m_idx    = c_ptr.m_idx

				-- For all blows
				for blow_nb = 1, num do
					num_blows = num_blows + 1

					local special = {}

					-- Test for hit
					if combat.default_weapon.test_hit(obj, m_ptr,
													  chance) then
						local prev_hp = m_ptr.hp

						-- Save this for WEAP_DAM_DEALT, in case the
						-- monster dies and m_ptr is wiped
						local r_idx  = m_ptr.r_idx
						local re_idx = m_ptr.ego

						combat.default_weapon.do_hit(obj, m_ptr, y, x, combat.default_weapon.INVEN_OFFHAND, weap, args)
						if c_ptr.m_idx ~= 0 then
							fear = m_ptr.monfear
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

		combat.curr = nil

		-- delay fear messages
		if fear and m_ptr.ml then
			message(strcap(m_name).." flees in terror!")
		end

		-- Save num_blows and mdeath for the energy function
		args.mdeath = mdeath
		args.num_blows = num_blows

		return false, args
	end,

	hooks	=
	{
		-- Calc how many blows to do
		[hook.CALC_BONUS_END] = function()
			if player.combat_style == combat.default_weapon.SKILL_MASTERY then
				local obj = player.inventory[combat.default_weapon.INVEN_MAINHAND][1]

				if obj and not player.heavy_wield then
					local str_index, dex_index
					local num, wgt, mul = combat.default_weapon.analyze_blow()
					local div = 0

					-- Enforce a minimum "weight" (tenth pounds)
					if obj.weight < wgt then
						div = wgt
					else
						div = obj.weight
					end

					-- Access the strength vs weight
					str_index = combat.default_weapon.strength_blow_modifier(mul, div)

					-- Maximal value
					if (str_index > 11) then str_index = 11 end

					-- Index by dexterity
					dex_index = combat.default_weapon.dexterity_blow_modifier()

					-- Maximal value
					if (dex_index > 11) then dex_index = 11 end

					-- Use the blows table
					player.num_blow = combat.default_weapon.blows_table[1 + str_index][1 + dex_index]

					-- Maximal value
					if (player.num_blow > num) then player.num_blow = num end

					-- Add in the "bonus blows"
					player.num_blow = player.num_blow + player.extra_blows

					if (combat.default_weapon.get_weaponmastery_skill() ~= -1) then
						player.num_blow = player.num_blow + get_skill_scale(combat.default_weapon.get_weaponmastery_skill(), 2)
					end

					-- Require at least one blow
					if (player.num_blow < 1) then player.num_blow = 1 end
				end

				if (combat.default_weapon.get_weaponmastery_skill() ~= -1) then
					combat.default_weapon.skill_bonus()
				end
			end
		end
		[hook.WIELD_PRE] = function(obj, item, slot)
			local inven = item_slot_to_inven(slot)
			local idx = item_slot_to_item(slot)

			-- Wearing a two handed weapon excludes shields
			if inven == combat.default_weapon.INVEN_OFFHAND then
				if player.inventory[combat.default_weapon.INVEN_MAINHAND][idx] and player.inventory[combat.default_weapon.INVEN_MAINHAND][idx].flags[FLAG_MUST2H] then
					message("You cannot wield your "..object_desc(obj).." with an two-handed weapon.")
					return true
				end
			end
			if inven == combat.default_weapon.INVEN_MAINHAND then
				-- Wearing a shield excludes two handed weapons
				if obj.flags[FLAG_MUST2H] and player.inventory[combat.default_weapon.INVEN_OFFHAND][idx] then
					message("You cannot wield your "..object_desc(obj).." with an offhand item.")
					return true
				end

				-- Wearing an off hand weapon prevents wielding a non dual wieldable main hand weapon
				if not obj.flags[FLAG_DUAL_WIELD] and player.inventory[combat.default_weapon.INVEN_OFFHAND][idx] and player.inventory[combat.default_weapon.INVEN_OFFHAND][idx].flags[FLAG_DUAL_WIELD] then
					message("You can not wield your "..object_desc(obj).." with an other weapon in off hand.")
					return true
				end
			end
		end
		[hook.WIELD_POST] = function(obj, item, slot)
			if item_slot_to_inven(slot) == combat.default_weapon.INVEN_MAINHAND and obj and has_flag(obj, FLAG_WEAPON) and get_combat_availability(combat.default_weapon.SKILL_MASTERY) and player.combat_style ~= combat.default_weapon.SKILL_MASTERY then
				if player.combat_style ~= combat.default_weapon.SKILL_MASTERY then
					player.combat_style = combat.default_weapon.SKILL_MASTERY
					message("Autoswitch to weapon combat mode.")
				end
			end
		end
		[hook.WIELD_INFO] = function(obj, item, slot)
			if obj.flags[FLAG_DUAL_WIELD] then
				if not combat.default_weapon.dual_wield(obj) then return end

				local offslot = get_slot(combat.default_weapon.INVEN_OFFHAND)
				local idx = item_slot_to_item(offslot)

				if player.inventory[combat.default_weapon.INVEN_MAINHAND][idx] and player.inventory[combat.default_weapon.INVEN_MAINHAND][idx].flags[FLAG_DUAL_WIELD] then
					local allow_dual = false
					if combat.default_weapon.bypass_dualwield_check then allow_dual = true
					else allow_dual = get_check("Wield in off hand?") end

					if allow_dual then return true, 1, get_slot(combat.default_weapon.INVEN_OFFHAND) end
				end
			end
		end
	},
}
