--
-- Handle calculations of vital stats
--

--
-- Calculate the players (maximal) hit points
-- Adjust current hitpoints if necessary
--
function player.calc_hitpoints()
	local bonus, bonus_skill = 0, 0
	local mhp

	-- Un-inflate "half-hitpoint bonus per level" value
	bonus = (player.stat(A_CON) - 10)

	-- Combat skill bonus
	bonus_skill = get_skill_scale(SKILL_COMBAT, 1000)

	-- Calculate hitpoints
	mhp = player_hp[player.lev] + (bonus * (1 + player.lev / 10)) + bonus_skill

	-- Always have at least one hitpoint per level
	if mhp < (player.lev / 2) + 1 then mhp = (player.lev / 2) + 1 end

	-- Factor in the pernament hp modifications
	mhp = mhp + player.hp_mod
	if mhp < 1 then mhp = 1 end

	-- Sorcery impose a hp penality
	if get_skill(SKILL_SORCERY) > 0 then
		mhp = mhp - ((mhp * get_skill_scale(SKILL_SORCERY, 25)) / 100)
		if mhp < 1 then mhp = 1 end
	end

	-- Augment Hitpoint
	mhp = mhp + ((mhp * player.to_l) / 100)
	if mhp < 1 then mhp = 1 end

	if player.body_monster.r_idx ~= 0 then
		local max_hp = player.body_monster.maxhp

		-- Adjust the hp with the possession skill
		max_hp = (max_hp * (20 + get_skill_scale(SKILL_POSSESSION, 80))) / 100

		mhp = (max_hp + sroot(max_hp) + mhp) / 3
	end

	-- Undead form
	if player.undead_form.active then
		mhp = (mhp * 4) / (player.lev / 2)
	end

	-- Disembodied? XXXX
	if player.body_monster.r_idx == 1 then mhp = 1 end

	-- New maximum hitpoints
	if player.mhp() ~= mhp then
		-- Enforce maximum
		if player.chp() >= mhp then
			player.chp(mhp)
		end

		-- Save the new max-hitpoints
		player.mhp(mhp)

		-- Display hitpoints (later)
		player.redraw[FLAG_PR_HP] = true

		-- Window stuff
		player.window[FLAG_PW_PLAYER] = true
	end
end

--
-- Calculate maximum mana.  You do not need to know any spells.
-- Note that mana is lowered by heavy (or inappropriate) armor.
--
function player.calc_mana()
	local msp, levels, cur_wgt, max_wgt, old

	levels = player.lev / 2

	-- Extract total mana
	local stat_value = max(player.stat(A_INT), player.stat(A_WIS)) - 10
	msp = get_skill_scale(SKILL_MAGIC, 200) + (stat_value * levels / (4 * 5))

	-- Usually add one mana
	if msp > 0 then msp = msp + 1 end

	if player.body_monster.r_idx ~= 0 then
		local max_sp = player.body_monster.flags[FLAG_MANA_MAX]

		if max_sp then
			max_sp = (max_sp *
					  (20 + get_skill_scale(SKILL_POSSESSION, 80))) / 100

			msp = (max_sp + sroot(max_sp) + msp) / 3
		end
	end

	-- Apply descriptor mana
	msp = msp + ((msp * player.descriptor.mana) / 100)

	-- Augment mana
	if player.to_m > 0 then msp = msp + ((msp * player.to_m) / 100) end

	-- Assume player not encumbered by armor
	player.cumber_armor = FALSE

	-- Weigh the armor
	cur_wgt = 0

	-- Determine the weight allowance
	max_wgt = 200 + get_skill_scale(SKILL_COMBAT, 500)

	-- Heavy armor penalizes mana
	if ((cur_wgt - max_wgt) / 10) > 0 then
		-- Encumbered
		player.cumber_armor = true

		-- Reduce mana
		msp = msp - ((cur_wgt - max_wgt) / 10)
	end

	-- Mana can never be negative
	if msp < 0 then msp = 0 end

	old = player.msp()
	player.msp(msp)

	-- Maximum mana has changed
	if player.msp() ~= old then
		-- Enforce new limit
		if player.csp() >= msp then
			player.csp(msp)
		end
	end

	-- Hack -- handle "xtra" mode
	if character_xtra then return end

	-- Take note when "glove state" changes
	if player.old_cumber_glove ~= player.cumber_glove then
		-- Message
		if player.cumber_glove then
			message("Your covered hands feel unsuitable for spellcasting.")
		else
			message("Your hands feel more suitable for spellcasting.")
		end

		-- Save it
		player.old_cumber_glove = player.cumber_glove
	end

	-- Take note when "armor state" changes
	if player.old_cumber_armor ~= player.cumber_armor then
		-- Message
		if player.cumber_armor then
			message("The weight of your armor encumbers your movement.")
		else
			message("You feel able to move more freely.")
		end

		-- Save it
		player.old_cumber_armor = player.cumber_armor
	end
end


--
-- Calculate the player's sanity
--
function player.calc_sanity()
	local bonus, msane

	-- Hack -- use the con/hp table for sanity/wis
	bonus = player.stat(A_WIS) - 10

	-- Hack -- assume 5 sanity points per level.
	msane = 5 * ((player.lev / 2) + 1) + (bonus * player.lev / (4 * 4))

	if msane < (player.lev / 2) + 1 then msane = (player.lev / 2) + 1 end

	if player.msane() ~= msane then
		-- Sanity carries over between levels.
		local new_csane = player.csane() + msane - player.msane()

		player.msane(msane)
		player.csane(new_csane)

		if player.csane() >= msane then
			player.csane(msane)
		end
	end
end

--
-- Computes current weight limit.
--
function player.weight_limit()
	local str = player.stat(A_STR)
	local i

	-- Weight limit based only on strength
	--i = 250 + player.stat(A_STR) * 180
	if str <= 8 then
		i = 500 + str * 63
	elseif str > 8 and str <= 32 then
		i = 1000 + (str - 8) * 89
	else
		i = 3126 + (str - 32) * 16
	end

	-- Sanity check
	if i < 10 then i = 10 end

	-- Return the result
	return i
end

--
-- Return a price factor(in percents) for buying/selling stuff
--
function player.calc_price_factor()
	return 120 - player.stat(A_CHR)
end

--
-- Apply those flags to the player
--
function player.apply_flags(flags, src)
	src = src or {}

	player.set_intrin_by_list(flags,
		{FLAG_IMPACT, FLAG_AGGRAVATE, FLAG_TELEPORT,
		FLAG_DRAIN_EXP, FLAG_SEE_INVIS, FLAG_FREE_ACT,
		FLAG_HOLD_LIFE, FLAG_WRAITH, FLAG_REFLECT,
		FLAG_NO_MAGIC, FLAG_NO_TELE, FLAG_AUTO_ID, FLAG_CANNOT_MOVE,
		FLAG_CANNOT_MOVE, FLAG_WATER_BREATH, FLAG_CLIMB})

	player.inc_intrin_by_list(flags,
		{FLAG_SPELL_FAILURE, FLAG_CRIT, FLAG_NUTRI_MOD, FLAG_REGEN_LIFE,
			FLAG_REGEN_MANA, FLAG_DRAIN_MANA, FLAG_DRAIN_LIFE, FLAG_XTRA_MIGHT,
			{FLAG_INVIS, 10},
			{FLAG_SEARCH_POWER, 5}, {FLAG_SEARCH_POWER_FEAT, 5},
			{FLAG_SEARCH_POWER_TRAP, 5}, {FLAG_SEARCH_FREQ, 5}})

	player.add_intrin_higher_by_list(flags,
									 {FLAG_ESP, FLAG_FLY, FLAG_PASS_WALL})

	player.set_field_by_list(flags, {
		{FLAG_PRECOGNITION, "precognition"},
		{FLAG_BLACK_BREATH, "black_breath"}
	})

	player.inc_field_by_list(flags, {
		{FLAG_LUCK,       "luck_cur"},
		{FLAG_SPELL,      "to_s"}, -- Spell power
		{FLAG_MANA,       "to_m"}, -- Mana capacity
		{FLAG_LIFE,       "to_l"}, -- Life capacity
		{FLAG_STEALTH,    "skill_stl"},
		{FLAG_INFRA,      "see_infra"},
		{FLAG_SPEED,      "pspeed"}, -- General speed
		{FLAG_BLOWS,      "extra_blows"},
		{FLAG_XTRA_SHOTS, "extra_shots"},
	})

	player.inc_flagset_by_list(flags, {
		{FLAG_STATS,       "stat_add"},
		{FLAG_SKILL_BONUS, "skill_bonuses"},
		{FLAG_SPEEDS,      "speeds"}
	})

	--------------------------------------------

	-- Sustain stats
	if flag_exists(flags, FLAG_SUST_STATS) then
		flag_add_increase(player.intrinsic_flags[FLAG_SUST_STATS],
						  flags[FLAG_SUST_STATS])
	end

	-- Protects from elements
	if flag_exists(flags, FLAG_PROTECT_FROM) then
		flag_add_increase(player.intrinsic_flags[FLAG_PROTECT_FROM],
						  flags[FLAG_PROTECT_FROM])
	end

	-- Additional digging power
	if flag_exists(flags, FLAG_ADD_DIG_POWER) then
		flag_add_increase(player.intrinsic_flags[FLAG_ADD_DIG_POWER],
						  flags[FLAG_ADD_DIG_POWER])
	end

	-- Copy FLAG_FLY into FLAG_CAN_FLY
	if flag_exists(flags, FLAG_FLY) then
		player.add_intrinsic_higher(FLAG_CAN_FLY, flag_get(flags, FLAG_FLY))
	end

	-- Antimagic field
	if flag_exists(flags, FLAG_ANTIMAGIC) then
		local tmp

		tmp = get_skill_scale(SKILL_ANTIMAGIC, 50) - (src.to_h or 0) - (src.to_d or 0) - (src.to_a or 0)
		tmp = (tmp * (100 - rescale(flag_get(flags, FLAG_ABSORBED_LEVELS), player.max_antimagic_levels(), 40))) / 100
		if tmp > 0 then player.antimagic = player.antimagic + tmp end

		tmp = get_skill_scale(SKILL_ANTIMAGIC, 10) - ((src.to_h or 0) + (src.to_d or 0) + (src.to_a or 0)) / 15
		tmp = tmp - rescale(flag_get(flags, FLAG_ABSORBED_LEVELS), player.max_antimagic_levels(), 5)
		if tmp > 0 then player.antimagic_dis = player.antimagic_dis + tmp end
	end

	-- Breaths
	if flag_exists(flags, FLAG_MAGIC_BREATH) then
		player.set_intrinsic(FLAG_MAGIC_BREATH, true)
		player.set_intrinsic(FLAG_WATER_BREATH, true)
	end

	-- Blow responses (passive attacks)
	if flag_exists(flags, FLAG_BLOW_RESPONSE) then
		player.add_intrin_blow_resp(flags[FLAG_BLOW_RESPONSE])
	end

	-- Prevent player from resting?
	if flag_exists(flags, FLAG_NO_RESTING) then
		local curr = player.intrinsic_flags[FLAG_NO_RESTING]

		-- Don't overwrite a previous string value, but otherwise
		-- use whatever the current flagset has.
		if not curr or tag(curr) ~= TAG_STRING then
			player.intrinsic_flags[FLAG_NO_RESTING] =
				flags[FLAG_NO_RESTING]
		end
	end
end

--
-- Gather flags, for when we need to see the flags for all the different
-- pieces of equipment at once, rather than one at a time.
--
function player.gather_flags(array, flags, src)
	if flag_exists(flags, FLAG_RESIST) then
		if not array[FLAG_RESIST] then
			array[FLAG_RESIST] = {}
		end

		local resists = flags[FLAG_RESIST]

		for i = 1, flag_max_key(resists) do
			if resists[i] then
				if not array[FLAG_RESIST][i] then
					array[FLAG_RESIST][i] = {}
				end
				tinsert(array[FLAG_RESIST][i], resists[i])
			end
		end
	end
	if flag_exists(flags, FLAG_ABSORB) then
		if not array[FLAG_ABSORB] then
			array[FLAG_ABSORB] = {}
		end

		local absorbs = flags[FLAG_ABSORB]

		for i = 1, flag_max_key(absorbs) do
			if absorbs[i] then
				if not array[FLAG_ABSORB][i] then
					array[FLAG_ABSORB][i] = {}
				end
				tinsert(array[FLAG_ABSORB][i], absorbs[i])
			end
		end
	end

	if flag_exists(flags, FLAG_METAB_PCT) then
		if not array[FLAG_METAB_PCT] then
			array[FLAG_METAB_PCT] = {}
		end

		tinsert(array[FLAG_METAB_PCT], flag_get(flags, FLAG_METAB_PCT))
	end
end -- player.gather_flags()

--
-- Apply the flags which have been gathered together
--
function player.apply_gathered_flags(array)
	if array[FLAG_RESIST] then
		local resists = array[FLAG_RESIST]
		for i = 1, getn(resists) do
			if resists[i] then
				-- Customized in ToME to allow for melee resists to stack additively
				-- but have other resists stack multiplicatively
				local val = 0
				if i == dam.CRUSH or i == dam.SLASH or i == dam.PIERCE then
					for j = 1, getn(resists[i]) do val = val + resists[i][j] end
				else
					val = dam.res.sum(resists[i])
				end

				player.resists[i] = val
			end
		end
	end
	if array[FLAG_ABSORB] then
		local absorbs = array[FLAG_ABSORB]
		for i = 1, getn(absorbs) do
			if absorbs[i] then
				local val = 0
				for j = 1, getn(absorbs[i]) do val = val + absorbs[i][j] end

				player.absorbs[i] = val
			end
		end
	end

	if array[FLAG_METAB_PCT] then
		local pcts     = array[FLAG_METAB_PCT]
		local high_pct = 100
		local low_pct  = 100

		for i = 1, getn(pcts) do
			if pcts[i] > high_pct then
				high_pct = pcts[i]
			elseif pcts[i] < low_pct then
				low_pct  = pcts[i]
			end
		end

		local final_pct = high_pct * low_pct / 100

		if final_pct ~= 100 then
			player.set_intrinsic(FLAG_METAB_PCT, final_pct)
		end
	end
end

--
-- Compute the player "state" from various data
--
function player.calc_bonuses(silent)
	local i, j
	local old_invis
	local old_speed
	local old_esp
	local old_see_inv
	local old_dis_ac
	local old_dis_to_a
	local o_ptr

	-- Save the old speed
	old_speed = player.pspeed

	-- Save the old vision stuff
	old_see_inv = player.intrinsic(FLAG_SEE_INVIS)
	if player.has_intrinsic(FLAG_ESP) then
		old_esp = flag_dup(player.intrinsic_flags[FLAG_ESP])
	else
		old_esp = flag_new()
	end

	-- Save the old armor class
	old_dis_ac = player.dis_ac
	old_dis_to_a = player.dis_to_a

	-- Save the old invisibility
	old_invis = player.intrinsic(FLAG_INVIS)

	-- Basic stuff
	player.basic_calc_bonuses(silent)

	-- Antimagic skill
	player.antimagic = player.antimagic + get_skill_scale(SKILL_ANTIMAGIC, 50)
	player.antimagic_dis = player.antimagic_dis +
		get_skill_scale(SKILL_ANTIMAGIC, 10) + 1

	-- Quick mind => faster spell casting
	if has_ability(AB_QUICK_MIND) then
		player.inc_speed(SPEED_CAST, 7)
	end

	-- Various PASS_WALL flag stuff
	if has_ability(AB_TREE_WALK) then
		local walking = flag_new()
		walking[FLAG_PASS_TREES] = 1
		player.add_intrinsic_higher(FLAG_PASS_WALL, walking)
		flag_free(walking,true)
	end

	if player.mimic_form == mimic.SPIDER then
		local walking = flag_new()
		walking[FLAG_PASS_WEB] = 1
		player.add_intrinsic_higher(FLAG_PASS_WALL, walking)
		flag_free(walking,true)
	end

	local new_esp
	if player.has_intrinsic(FLAG_ESP) then
		new_esp = flag_dup(player.intrinsic_flags[FLAG_ESP])
	else
		new_esp = flag_new()
	end

	if not flag_equal(old_esp, new_esp) then
		player.update = player.update | PU_MONSTERS
	end

	-- We dont want memory leaks do we :)
	flag_free(old_esp, true)
	flag_free(new_esp, true)

	-- Hack -- See Invis Change
	if player.intrinsic(FLAG_SEE_INVIS) ~= old_see_inv then
		player.update = player.update | PU_MONSTERS
	end

	-- Extract the current weight (in tenth pounds)
	j = calc_total_weight()

	-- Extract the "weight limit" (in tenth pounds)
	i = player.weight_limit()

	local encumberance = 0

	if j > i / 2 then
		encumberance = (j - (i / 2)) / (i / 10)
	end

	-- Set the food usage.  Do this before slowing the player down
	-- from carrying too much weight, so that carrying a ton of stuff
	-- won't slow down digestion.
	local food_usage = player.speed() + 10

	if food_usage < 1 then
		food_usage = 1
	end

	if encumberance > 10 then
		food_usage = food_usage + 10
	else
		food_usage = food_usage + encumberance
	end

	if player.intrinsic_flags[FLAG_METAB_PCT] then
		food_usage = food_usage * player.intrinsic(FLAG_METAB_PCT) / 100
	end

	if player.has_intrinsic(FLAG_NUTRI_MOD) then
		food_usage = food_usage - player.intrinsic(FLAG_NUTRI_MOD)
	end

	player.inc_intrinsic(FLAG_FOOD_VALUE, food_usage)

	-- Apply "encumbrance" from weight
	player.pspeed = player.pspeed - encumberance

	-- Display the speed (if needed)
	if player.pspeed ~= old_speed then player.redraw[FLAG_PR_SPEED] = true end

	-- Actual Modifier Bonuses (Un-inflate stat bonuses)
	player.to_a = player.to_a + (player.stat(A_DEX) - 10) / 4
	player.to_d = player.to_d + (player.stat(A_STR) - 10) / 4
	player.to_h = player.to_h + (player.stat(A_DEX) - 10) / 4

	-- Displayed Modifier Bonuses (Un-inflate stat bonuses)
	player.dis_to_a = player.dis_to_a + (player.stat(A_DEX) - 10) / 4
	player.dis_to_d = player.dis_to_d + (player.stat(A_STR) - 10) / 4
	player.dis_to_h = player.dis_to_h + (player.stat(A_DEX) - 10) / 4

	-- Redraw armor (if needed)
	if (player.dis_ac ~= old_dis_ac) or (player.dis_to_a ~= old_dis_to_a) then
		-- Redraw
		player.redraw[FLAG_PR_ARMOR] = true

		-- Window stuff
		player.window[FLAG_PW_PLAYER] = true
	end


	-- Examine the "current tool"
	o_ptr = player.inventory[INVEN_TOOL][1]

	-- Check for usabilkity of curent combat style
	check_combat_style()

	-- Examine the main weapon
	o_ptr = player.inventory[INVEN_MAINHAND][1]

	-- Assume not heavy
	player.heavy_wield = FALSE

	-- Let the scripts do what they need
	hook.process(hook.CALC_BONUS_END, silent)

	-- Assume okay
	player.icky_wield = FALSE

	if get_skill_scale(SKILL_DODGE, 50) > 0 then
		-- Get the armor weight
		local cur_wgt = 0

		for _, inven_nb in { INVEN_BODY, INVEN_HEAD, INVEN_OFFHAND, INVEN_OUTER, INVEN_HANDS, INVEN_FEET } do
			local inven = player.inventory[inven_nb]
			for j = 1, flag_max_key(inven) do
				local obj = inven[j]
				if obj then cur_wgt = cur_wgt + obj.weight end
			end
		end

		-- Base dodge chance
		player.dodge_chance = get_skill_scale(SKILL_DODGE, 150) + get_skill_scale(SKILL_HAND, 50)

		-- Armor weight bonus/penalty
		player.dodge_chance = player.dodge_chance - cur_wgt * 2

		-- Encumberance bonus/penalty
		player.dodge_chance = player.dodge_chance - (calc_total_weight() / 100)

		-- Never below 0
		if player.dodge_chance < 0 then player.dodge_chance = 0 end
	else
		player.dodge_chance = 0
	end

	-- Parse all the weapons
	for i = 1, flag_max_key(player.inventory[INVEN_MAINHAND]) do
		local o_ptr = player.inventory[INVEN_MAINHAND][i]

		if o_ptr then
			-- 2handed weapon and shield = less damage
			if has_flag(o_ptr, FLAG_COULD2H) and player.inventory[INVEN_OFFHAND][i] then
				local tmp

				-- Reduce the bonuses
				tmp = o_ptr.to_h / 2
				if tmp < 0 then tmp = -tmp end
				player.to_h = player.to_h - tmp

				tmp = o_ptr.to_d / 2
				if tmp < 0 then tmp = -tmp end
				tmp = tmp + (o_ptr.dd * o_ptr.ds) / 2
				player.to_d = player.to_d - tmp
			end
			-- Sorcerer can't wield a weapon unless it's a mage orb
			if get_skill(SKILL_SORCERY) > 0 then
				local malus = get_skill_scale(SKILL_SORCERY, 100)

				if o_ptr.tval ~= TV_MSTAFF then
					-- Reduce the real bonuses
					player.to_h = player.to_h - malus
					player.to_d = player.to_d - malus

					-- Reduce the mental bonuses
					player.dis_to_h = player.dis_to_h - malus
					player.dis_to_d = player.dis_to_d - malus

					-- Icky weapon
					player.icky_wield = true
				else
					-- Reduce the real bonuses
					player.to_h = player.to_h - malus / 10
					player.to_d = player.to_d - malus / 10

					-- Reduce the mental bonuses
					player.dis_to_h = player.dis_to_h - malus / 10
					player.dis_to_d = player.dis_to_d - malus / 10
				end
			end
		end
	end

	-- Affect Skill -- stealth (bonus one)
	player.skill_stl = player.skill_stl + 1

	-- Affect Skill -- disarming (DEX and INT)
	player.skill_dis = player.skill_dis + (player.stat(A_DEX) - 10) / 10
	player.skill_dis = player.skill_dis + (player.stat(A_INT) - 10) / 5

	-- Affect Skill -- magic devices (INT)
	player.skill_dev = player.skill_dev + get_skill_scale(SKILL_DEVICE, 20)

	-- Affect Skill -- saving throw (WIS)
	player.skill_sav = player.skill_sav + get_skill_scale(SKILL_SPIRITUALITY, 20)

	-- Affect Skill -- disarming (skill)
	player.skill_dis = player.skill_dis + (get_skill_scale(SKILL_DISARMING, 75))

	-- Affect Skill -- magic devices (skill)
	player.skill_dev = player.skill_dev + (get_skill_scale(SKILL_DEVICE, 150))

	-- Affect Skill -- saving throw (skill and level)
	player.skill_sav = player.skill_sav + (get_skill_scale(SKILL_SPIRITUALITY, 75))

	-- Affect Skill -- stealth (skill)
	player.skill_stl = player.skill_stl + (get_skill_scale(SKILL_STEALTH, 25))

	-- Affect Skill -- search ability (Sneakiness skill)
	player.inc_intrinsic(FLAG_SEARCH_POWER, get_skill_scale(SKILL_SNEAK, 35))

	-- Affect Skill -- search frequency (Sneakiness skill)
	player.inc_intrinsic(FLAG_SEARCH_FREQ, get_skill_scale(SKILL_SNEAK, 25))

	-- Affect Skill -- combat (Combat skill + mastery)
	player.skill_thn = player.skill_thn + (50 * (((7 * get_skill_scale(player.combat_style, 50)) + (3 * get_skill_scale(SKILL_COMBAT, 50))) / 10) / 10)

	-- Affect Skill -- combat (shooting) (Level, by Class)
	player.skill_thb = player.skill_thb + (50 * (((7 * get_skill_scale(SKILL_ARCHERY, 50)) + (3 * get_skill_scale(SKILL_COMBAT, 50))) / 10) / 10)

	-- Affect Skill -- combat (throwing) (Level)
-- MHH !
	player.skill_tht = player.skill_tht + (50 * player.lev / 20)

	-- Limit Skill -- stealth from 0 to 30
	if player.skill_stl > 30 then player.skill_stl = 30 end
	if player.skill_stl < 0 then player.skill_stl = 0 end

	if player.has_intrinsic(FLAG_NO_MAGIC) and (player.skill_sav < 95) then
		player.skill_sav = 95
	end


	-- Set HP regen
	player.inc_intrinsic(FLAG_REGEN_LIFE, player.mhp() * 5)

	-- Set mana regen
	if player.body_monster.r_idx ~= 0 and
		player.body_monster.flags[FLAG_MANA_REGENERATE]
	then
		local regen = player.body_monster.flags[FLAG_MANA_REGENERATE]

		regen = (regen *
					  (20 + get_skill_scale(SKILL_POSSESSION, 80))) / 100

		player.inc_intrinsic(FLAG_REGEN_MANA, regen)
	else
		player.inc_intrinsic(FLAG_REGEN_MANA, player.msp() * 5)
	end

	-- Regeneration rates double when player is resting or searching
	if mov_mode.get_info(player.movement_mode, "name") == "Searching" or
		timed_effect.get(timed_effect.RESTING)
	then
		if player.intrinsic(FLAG_REGEN_LIFE) > 0 then
			player.set_intrinsic(FLAG_REGEN_LIFE,
								 player.intrinsic(FLAG_REGEN_LIFE) * 2)
		end

		if player.intrinsic(FLAG_REGEN_MANA) > 0 then
			player.set_intrinsic(FLAG_REGEN_MANA,
								 player.intrinsic(FLAG_REGEN_MANA) * 2)
		end
	end

	-- Upkeep for partial summons
	player.set_intrinsic(FLAG_REGEN_MANA,
						 (player.intrinsic(FLAG_REGEN_MANA) *
						  (100 - summoning.regen_factor) / 100))
	player.inc_intrinsic(FLAG_DRAIN_MANA, summoning.maint_cost)

	-- Undead form = negative regen, the more the player normaly regens, the faster he rots away!
	if player.undead_form.active then
		player.set_intrinsic(FLAG_REGEN_LIFE, - abs(player.intrinsic(FLAG_REGEN_LIFE)))
	end

	-- Hack -- handle "xtra" mode
	if character_xtra then return end

	-- Give perfect searching and perception with high enough sneakiness
	if get_skill(SKILL_SNEAK) >= 100 then
		player.set_intrinsic(FLAG_SEARCH_POWER, 100)
		player.set_intrinsic(FLAG_SEARCH_FREQ, 100)
	end

	-- Give perfect disarming with high enough disarming
	if get_skill(SKILL_DISARMING) >= 100 then
		player.set_intrinsic(FLAG_PERFECT_DISARMING, 1)
	end

	-- Take note when "heavy weapon" changes
	if player.old_heavy_wield ~= player.heavy_wield then
		if silent then
			-- do nothing
		-- Message
		elseif player.heavy_wield then
			message("You have trouble wielding such a heavy weapon.")
		elseif player.inventory[INVEN_MAINHAND][1] then
			message("You have no trouble wielding your weapon.")
		else
			message("You feel relieved to put down your heavy weapon.")
		end

		-- Save it
		player.old_heavy_wield = player.heavy_wield
	end


	-- Take note when "illegal weapon" changes
	if player.old_icky_wield ~= player.icky_wield then
		if silent then
			-- do nothing
		-- Message
		elseif player.icky_wield then
			message("You do not feel comfortable with your weapon.")
		elseif player.inventory[INVEN_MAINHAND][1] then
			message("You feel comfortable with your weapon.")
		else
			message("You feel more comfortable after removing your weapon.")
		end

		-- Save it
		player.old_icky_wield = player.icky_wield
	end
-- ]]
end

----------------------------------------------------------------------------
-- Monster body stuff
----------------------------------------------------------------------------

--
-- Copy race related flags to intrinsics, so they can be accessed from
-- one place
--
function player.apply_race_flags(flags)
	-- Race flags
	player.set_intrin_by_list(flags,
		{
			FLAG_ORC,
			FLAG_TROLL,
			FLAG_GIANT,
			FLAG_DRAGON,
			FLAG_DEMON,
			FLAG_ANIMAL,
			FLAG_HUMAN,
			FLAG_THUNDERLORD,
			FLAG_MAMMAL,
			FLAG_INSECT
		})

	-- Misc flags
	player.set_intrin_by_list(flags,
		{
			FLAG_UNDEAD,
			FLAG_EVIL,
			FLAG_GOOD,
			FLAG_CORRUPTED,
			FLAG_NONLIVING,
			FLAG_COLD_BLOOD,
			FLAG_NEVER_BLOW,
			FLAG_NEVER_MOVE,
		})
end -- player.apply_race_flags()

function player.apply_body(body)
	if body.r_idx == 0 then
		return
	end

	-- Duplicate flags, since we're going to modify them before applying
	-- them
	local flags = flag_dup(body.flags)

	flags[FLAG_SPEED]  = body.speed - 110

	-- Misc flags
	if flags[FLAG_NEVER_MOVE] then
		player.set_intrinsic(FLAG_CANNOT_MOVE, true)
	end

	if flags[FLAG_NEVER_BLOW] and tag(FLAG_NEVER_BLOW) ~= TAG_STRING then
		flags[FLAG_NEVER_BLOW] = "You cannot attack in this form!"
	end

	if flags[FLAG_CAN_FLY] then
		player.add_intrinsic_higher(FLAG_FLY, flags[FLAG_CAN_FLY])
		player.add_intrinsic_higher(FLAG_CAN_FLY, flags[FLAG_CAN_FLY])
	end

	-- Immunities and resists
	local non_inate = flags[FLAG_NON_INNATE_ARMOUR]
	if not non_inate then
		-- Amror is inate, apply body's ac
		player.ac = player.ac + body.ac
	else
		-- non_inate is true, remove non-inate resistances and such
		if tag(non_inate) == TAG_FLAG then
			-- Remove specific damage resistances and absorbs
			flag_del(flags[FLAG_RESIST], non_inate)
			flag_del(flags[FLAG_ABSORB], non_inate)
		else
			-- Remove beneficial resistances standard resistance and
			local types = {dam.SLASH, dam.CRUSH, dam.PIERCE}

			for i = 1, getn(types) do
				local typ = types[i]

				flags[FLAG_RESIST][typ] = min(flags[FLAG_RESIST][typ] or 0,
											  0)
				flags[FLAG_ABSORB][typ] = nil
			end
		end
	end -- if non_inate then

	player.set_intrin_by_list(flags,
		{
			FLAG_NO_CUT,
			FLAG_NO_FEAR,
			FLAG_NO_STUN,
			FLAG_NO_SLEEP,
			FLAG_RES_TELE,
		})

	local resists = flags[FLAG_RESIST]
	if flags[FLAG_NO_CUT] then
		resists[dam.CUT] = 100
	end
	if flags[FLAG_NO_FEAR] then
		resists[dam.FEAR] = 100
	end
	if flags[FLAG_NO_STUN] then
		resists[dam.STUN] = 100
	end

	player.apply_flags(flags)
	player.gather_flags(player.gathered, flags)
	player.apply_race_flags(flags)

	flag_free(flags, true)
end -- player.apply_body()

hook(hook.CALC_BONUS,
function(silent)
	player.apply_race_flags(player.descriptor.flags)
	player.apply_body(player.body_monster)
end)

