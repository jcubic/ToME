-- The default barehand combat subsystem
--
-- Load it with: load_subsystem("combat_brehand_default")
--
-- Unlike other combat subsystems this does not define an actualy combat type but
-- gives the possibility to craete ones, like monk's atatcks, beorning attacks, ents, ...
--
-- Parameters:
-- * base_skill_name: The name of the skill which defines this combat method
-- * dual_wield: function which allows or forbids dual wielding(defaults to forbid)
-- * on_hit: function which will be called when a blow successfully landed (default to nothing)
-- * max_blows: function which must return a number of blows to add to the default ones(4 usualy) (defaults to 0)
-- * energy_use: function called after the attack to determine the amount of energy expended

-- Load the combat subsystem which defines some basic stuff for all
-- combat systems
load_subsystem("combat")

-- Make the namespace
combat.default_barehand = {}

combat.default_barehand.BTH_PLUS_ADJ = 3

-- Return the blow table to use for attacks
combat.default_barehand.get_blows = get_subsystem_param("combat_barehand_default", "blow_table") or function() return {} end

-- Did we hit the monster?
combat.default_barehand.test_hit = get_subsystem_param("combat_barehand_default", "test_hit") or function() return false end

-- By how much do we increase damage because of a critical hit?
-- Return 1, 1, 0 for no change to damage.
combat.default_barehand.critical_hit = get_subsystem_param("combat_barehand_default", "critical_hit") or function() return 1, 1, 0 end

-- Extra stuff to do when a hands hits
combat.default_barehand.on_hit = get_subsystem_param("combat_barehand_default", "on_hit") or function() return false end

combat.default_barehand.STAT_STR = get_subsystem_param("combat_barehand_default", "stat_str")

combat.default_barehand.INVEN_MAINHAND = get_subsystem_param("combat_barehand_default", "mainhand_slot") or 0
combat.default_barehand.INVEN_OFFHAND = get_subsystem_param("combat_barehand_default", "offhand_slot") or 0

combat.default_barehand.available = get_subsystem_param("combat_barehand_default", "available") or function() return true end

-- Resolve skill name afterwards
hook(hook.INFO_DATA_LOADED,
	function()
		combat.default_barehand.SKILL_BAREHAND = find_skill(get_subsystem_param("combat_barehand_default", "base_skill_name"))
	end
)

function combat.default_barehand.do_hit(m_ptr, y, x)
	local c_ptr = cave(y, x)

	local add_damage       = player.to_d
	local crit_mult, crit_div, crit_bonus

	-- Attempt 'times'
	local blow_table = combat.default_barehand.get_blows()
	local ma_ptr = blow_table[1]
	local old_ptr = blow_table[1]
	local plev = get_skill(combat.default_barehand.SKILL_BAREHAND)
	local max_tries = plev / 7
	local max = getn(blow_table)
	if plev < 7 then max_tries = 1 end

	for times = 1, max_tries do
		repeat
			ma_ptr = blow_table[rng(max)]
		until not ((ma_ptr.level > plev) or (rng(plev) < ma_ptr.chance))

		-- keep the highest level attack available we found
		if ma_ptr.level > old_ptr.level and not player.has_intrinsic(FLAG_CONFUSED) then
			old_ptr = ma_ptr
			if (wizard) then message("Attack re-selected.") end
		else
			ma_ptr = old_ptr
		end
	end

	-- Basic damage
	local k = rng.roll(ma_ptr.dice[1], ma_ptr.dice[2]) + add_damage
	local max_k = (ma_ptr.dice[1] * ma_ptr.dice[2]) + add_damage

	-- Any effects upon blow ?
	combat.default_barehand.on_hit(m_ptr)

	-- Did the hook kill the monster or make it disappear?
	if c_ptr.m_idx == 0 then
		return
	end

	crit_mult, crit_div, crit_bonus = combat.default_barehand.critical_hit(m_ptr, combat.default_barehand.SKILL_BAREHAND)

	local dam_type = ma_ptr.type

	k = k * crit_mult / crit_div
	max_k = max_k * crit_mult / crit_div

	-- No negative damage
	if k < 0 then k = 0 end

	if (wizard) then message("You do "..k.." damage ("..get_dam_type_info(dam_type, "desc") .. ")")	end

	message(format(ma_ptr.desc, monster_desc(m_ptr, 0)))

	-- Damage, check for fear and death
	-- All damage doing uses project
	local proj_flags = (PROJECT_JUMP | PROJECT_HIDE | PROJECT_HIDE_BLAST | PROJECT_STOP | PROJECT_KILL | PROJECT_NO_REFLECT)
	project(0, 0, y, x, k, dam_type, proj_flags)

	-- Blow responses (passive attacks)
	if c_ptr.m_idx ~= 0 and has_flag(m_ptr, FLAG_BLOW_RESPONSE) then
		local responses = m_ptr.flags[FLAG_BLOW_RESPONSE]

		for i = 1, flag_max_key(responses) do
			if responses[i] then
				local blow_response = get_blow_response(i)
				blow_response.weapon_blow(WHO_PLAYER, c_ptr.m_idx,
									  nil, nil, nil,
										  nil,
										  crit_mult, crit_div,
										  max_k,
										  responses[i],
										  flag_get2(responses, i))
			end
		end
	end -- Blow responses (passive attacks)
end -- combat.default_barehand.do_hit()

combat.default_barehand.COMBAT_BAREHAND = add_combat_system
{
	name	= "Barehand fighting"
	desc	= "Uses your bare hands(or other appendices) to inflict pain and suffering.",
	skill	= get_subsystem_param("combat_barehand_default", "base_skill_name")
	energy	= get_subsystem_param("combat_barehand_default", "energy_use") or
		function() 
			return get_player_energy(SPEED_BAREHAND) 
		end

	info    = function()
		local dambonus = player.dis_to_d

		local blow_table = combat.default_barehand.get_blows()
		local max_blow = getn(blow_table)
		local plev = get_skill(combat.default_barehand.SKILL_BAREHAND)
		local min_attack = blow_table[1]
		local tries = 400

		local i = max_blow
		while (blow_table[i].level > plev) and (i ~= 1) do
			i = i - 1
			tries = tries - 1
			if tries < 0 then
				error("barehand combat subsystem could not select an attack, probably the module has not defined enough atatcks for the level")
				break
			end
		end
		local max_attack = blow_table[i]

		local desc = format("%d-%d", min_attack.dice[1], rng.maxroll(max_attack.dice[1], max_attack.dice[2]))
		return desc
	end,

	available = function()
		return combat.default_barehand.available()
	end,

	attack	  = function(y, x, max)
		local fear = false
		local c_ptr = cave(y, x)
		local m_ptr = monster(c_ptr.m_idx)
		local r_ptr = race_info_idx(m_ptr.r_idx, m_ptr.ego)

		-- is the monster dead yet?
		local mdeath = false
		-- number of blows dealt
		local num_blows = 0

		-- Extract monster name (or "it")
		local m_name = monster_desc(m_ptr, 0)

		-- Allow modules to check faction settings
		local ret = combat.check_faction(c_ptr.m_idx, m_ptr, y, x)
		if ret == combat.DONT_MOVE then return false
		elseif ret == combat.DO_MOVE then return true
		end

		if not combat.init_combat_turn(c_ptr.m_idx, m_name) then return end

		-- For all weapons
		for weap = 1, player.inventory_limits(combat.default_barehand.INVEN_MAINHAND) do
			local obj = player.inventory[combat.default_barehand.INVEN_MAINHAND][weap]

			-- Monster is already dead ? oh pitty :(
			if mdeath then break end

			if not obj then
				-- Calculate the "attack quality"
				local bonus = player.to_h
				local chance = player.skill_thn + (bonus * combat.default_barehand.BTH_PLUS_ADJ)

				local num = iif(weap == 1, player.num_blow, 1)
				if max > -1 and num > max then num = max end

				-- For use in the MONSTER_DEATH hook
				combat.curr          = {}
				combat.curr.weap     = nil
				combat.curr.launcher = nil
				combat.curr.m_idx    = c_ptr.m_idx

				-- For all blows
				for blow_nb = 1, num do
					num_blows = num_blows + 1

					local special = {}

					-- Test for hit
					if combat.default_barehand.test_hit(m_ptr, chance) then
						local prev_hp = m_ptr.hp

						-- Save this for WEAP_DAM_DEALT, in case the
						-- monster dies and m_ptr is wiped
						local r_idx  = m_ptr.r_idx
						local re_idx = m_ptr.ego

						combat.default_barehand.do_hit(m_ptr, y, x, combat.default_barehand.INVEN_MAINHAND, weap)
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

		return false, {mdeath = mdeath, num_blows = num_blows}
	end,

	hooks	=
	{
		-- Calc how many blows to do
		[hook.CALC_BONUS_END] = function()
			if player.combat_style == combat.default_barehand.SKILL_BAREHAND then
				local plev = get_skill(SKILL_HAND)

				player.num_blow = 0
				if (plev > 9) then player.num_blow = player.num_blow + 1 end
				if (plev > 19) then player.num_blow = player.num_blow + 1 end
				if (plev > 29) then player.num_blow = player.num_blow + 1 end
				if (plev > 34) then player.num_blow = player.num_blow + 1 end
				if (plev > 39) then player.num_blow = player.num_blow + 1 end
				if (plev > 44) then player.num_blow = player.num_blow + 1 end
				if (plev > 49) then player.num_blow = player.num_blow + 1 end

				player.num_blow = player.num_blow + 1 + player.extra_blows

				player.to_h = player.to_h + (plev / 3)
				player.to_d = player.to_d + (plev / 3)
				player.dis_to_h = player.dis_to_h + (plev / 3)
				player.dis_to_d = player.dis_to_d + (plev / 3)
			end
		end
		[hook.TAKEOFF_POST] = function(obj, item)
			if item_slot_to_inven(item) == combat.default_barehand.INVEN_MAINHAND and not get_object(item) and get_combat_availability(combat.default_barehand.SKILL_BAREHAND) and player.combat_style ~= combat.default_barehand.SKILL_BAREHAND then
				if player.combat_style ~= combat.default_barehand.SKILL_BAREHAND then
					player.combat_style = combat.default_barehand.SKILL_BAREHAND
					message("Autoswitch to barehand combat mode.")
				end
			end
		end
	},
}
