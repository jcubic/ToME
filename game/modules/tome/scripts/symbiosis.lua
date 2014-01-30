--
-- Symbiosis system
--

declare_global_constants
{
	"symbiosis",
}
symbiosis = {}

new_speed("SYMBIOSIS", "symbiosis")

new_flag("NO_SYMBIOSIS")
new_flag("NO_HYPNOTIZE")
new_flag("SYMB_NO_DEFAULT")
new_flag("SYMB_BONUS")
new_flag("SYMB_BONUS_SCALED")
new_flag("SYMB_BONUS_BY_LEVEL")

new_flag("SYMB_REBEL")
new_flag("SYMB_ATTACK")

new_flag("HYPNOTIZE_PRE")
new_flag("HYPNOTIZE_POST")

--------------------------------------------------------------------

function symbiosis.spell_is_disrupted(s, silent)
	local spl = spell(s)

	if player.antimagic > 0 and __tmp_spells[s].antimagic == true then
		if not silent then
			message("Your anti-magic field disrupts any magic attempts.")
		end
		return true
	end

	if player.anti_magic and __tmp_spells[s].anti_magic == true then
		if not silent then
			message("You anti-magic shell disrupts any magic attempts.")
		end
		return true
	end

	return false
end

function symbiosis.insuff_power_effect(spl, cost, power)
	message("You faint from the effort!")

	timed_effect.inc(timed_effect.PARALYZED,
					 rng(1, 5 * (power - cost)))

	player.redraw[FLAG_PR_STATE] = true

	if rng.percent(50) then
		message("You have damaged your body!")

		-- 25% chance of non-temporary
		local non_temp = rng.percent(25)

		dec_stat(A_CON, 15 + rng(1, 10),
				 iif(non_temp, STAT_DEC_NORMAL, STAT_DEC_TEMPORARY))
	end
	return power
end

symbiosis.SCHOOL = add_school {
	name  = "Symbiosis",
	skill = "SKILL_SYMBIOTIC",

	-- FLAG_SPELL_FAILURE doesn't affect symbiosis spells.
	change_spell_chance =
		function(s, chance, failure)
			return chance, 0
		end

	can_insuf_power =
		function(spl)
			return true
		end

	insuf_power_prompt =
		function(spl)
			return "You do not have enough mana to use this power, " ..
				"attempt anyways?"
		end

	insuf_power_effect = symbiosis.insuff_power_effect
} -- symbiosis.SCHOOL

-- Use symbiote power "spell"
symbiosis.SPELL_INATE_POWER = add_spell
{
	["name"]       = "Use symbiote power",
	["school"]     = symbiosis.SCHOOL,
	["level"]      = 0,
	["mana"]       = 0,
	["mana_max"]   = 0,
	["fail"]       = 0,
	["random"]     = -1,
	["antimagic"]  = FALSE,
	["anti_magic"] = FALSE,
	["spell"]      = function()
		symbiosis.use_symbiote_power()
	end,
	["desc"] =	{
		"Use one of your symbiote's powers."
	}
	["info"] = 	function()
		if not player.inventory[INVEN_SYMBIOTE][1] then
			return "Need a symbiote."
		else
			return ""
		end
	end,
	["needs_symb"] = true,

	-- Never fails, since success or failure happens with the individual
	-- symbiote power
	["change_spell_chance"] =
		function(s, chance, failure)
			return 0, 0
		end

} -- Use symbiote power "spell"

-- Hypnotize spell
symbiosis.SPELL_INATE_HYPNO = add_spell
{
	["name"]       = "Hypnotize",
	["school"]     = symbiosis.SCHOOL,
	["level"]      = 1,
	["mana"]       = 1,
	["mana_max"]   = 1,
	["fail"]       = 10,
	["random"]     = -1,
	["antimagic"]  = FALSE,
	["anti_magic"] = FALSE,
	["spell"]      = function()
		message("Hypnotize which pet or companion?")

		local ret, dir = get_rep_dir()
		if not ret then return SPELL_NOTHING end

		local y, x = explode_dir(dir)
		y, x = y + player.py, x + player.px
		local m_idx = cave(y, x).m_idx
		local monst = monster(m_idx)

		if m_idx == 0 or monst.r_idx == 0 then
			message("There does not appear to be any monster there.")
			return
		end

		if or_msg("The monster resists hypnosis.",
				  monst.flags[FLAG_NO_HYPNOTIZE],
				  monst.flags[FLAG_NO_SYMBIOSIS])
		then
			return
		end

		if not monst.flags[FLAG_SYMBIOTE] then
			message("You can only hypnotize monsters which cannot move.")
			return
		end

		if monst.faction ~= player.faction then
			message("You can only hypnotize pets and companions.")
			return
		end

		if monst.flags[FLAG_HYPNOTIZE_PRE] then
			local func
			func = get_func_from_flag_or_registry(monst.flags,
												  FLAG_HYPNOTIZE_PRE)

			if func(monst) then
				return
			end
		end

		-- Make monster visible, for the times when monster_desc()
		-- is called.
		local symb = create_object(TV_SYMBIOTE, SV_SYMBIOTE)
		pets.remove_pet_ai(monst)
		symb.flags[FLAG_MONSTER_OBJ] = monster_type_dup(monst)
		message("You hypnotize " .. monster_desc(monst, 0) .. ".")
		delete_monster_idx(m_idx)

		monst = symb.flags[FLAG_MONSTER_OBJ]
		symbiosis.calc_obj_bonus(symb)

		if monst.flags[FLAG_HYPNOTIZE_POST] then
			local func
			func = get_func_from_flag_or_registry(monst.flags,
												  FLAG_HYPNOTIZE_POST)

			func(monst, symb, y, x)
		end

		if flag_used(monst.spells) > 0 and
			not monst.flags[FLAG_MANA_CURR]
		then
			message(color.LIGHT_RED, "WARNING: " ..
					symbiosis.get_symbiote_name(symb) .. "has spells, " ..
						"but no mana.")
			monst.flags[FLAG_MANA_CURR] = 0
			monst.flags[FLAG_MANA_MAX]  = 0
		end
		monst.ml = true

		drop_near(symb, -1, y, x)
	end, -- spell()
	["info"] = 	function()
			return ""
	end,
	["desc"] =	{
		"Hypnotize a companion or pet suitable for symbiosis, after which you can",
		"wear it.  The monster must be right next to you.",
	}
} -- Hypnotize spell

-- Release spell
symbiosis.SPELL_INATE_RELEASE = add_spell
{
	["name"]       = "Release",
	["school"]     = symbiosis.SCHOOL,
	["level"]      = 1,
	["mana"]       = 1,
	["mana_max"]   = 1,
	["fail"]       = 10,
	["random"]     = -1,
	["antimagic"]  = FALSE,
	["anti_magic"] = FALSE,
	["spell"]     = function()
		local ret, item
		ret, item = get_item("Awaken which monster?",
							 "You have no monsters to awaken.",
							 USE_INVEN | USE_FLOOR | USE_EQUIP,
			function (obj)
				return (obj.tval == TV_SYMBIOTE)
			end)

		if not ret then
			return SPELL_NOTHING
		end

		local obj = get_object(item)

		if not obj.flags[FLAG_MONSTER_OBJ] then
			message(color.LIGHT_RED,
					"ERROR: Symbiote has no FLAG_MONSTER_OBJ!")
			return false
		end
		local monst = monster_type_dup(obj.flags[FLAG_MONSTER_OBJ])

		local d     = 1
		local tries = 0
		local y, x
		while d < 4 do
			local y, x = scatter(player.py, player.px, d, 0)

			if cave_empty_bold(y, x) then
				local m_idx = place_monster_type(y, x, monst)
				if m_idx ~= 0 and monster(m_idx).r_idx ~= 0 then
					monst = monster(m_idx)
					pets.set_pet_ai(monst)
					break
				end
			end

			tries = tries + 1
			if imod(tries, 8) then
				d = d + 1
			end
		end

		if d >= 4 then
			message("There is nowhere for your symbiote to go; not " ..
					"releasing it.")
			delete_monster_type(monst, true)
			return
		end

		if monst.ml then
			message("You awakened " .. monster_desc(monst, 0) .. ".")
		else
			message("You awakened " .. object_desc(obj, 0) ..
					", but you can't see it.")
		end

		item_increase(item, -obj.number)
		item_describe(item)
		item_optimize(item)
	end,
	["info"] = 	function()
			return ""
	end,
	["desc"] =	{
		"Release a symbiotic monster from hypnosis.  You must be holding",
		"it, wearing it, or standing over it to do this."
	}
} -- Release spell

dam.CHARM_SYMBIOTE = dam.add
{
    gfx        = 0,
    desc       = "charm symbiote",
    color      = {color.SLATE},
    text_color = "#w",
    monster    = function(state)
		local monst = state.m_ptr
		local name  = monster_desc(monst, 0)

		if monst.flags[FLAG_UNIQUE] or not monst.flags[FLAG_SYMBIOTE] then
			message(strcap(name) .. " is unaffected!")
			return {dam = 0, angry = 1}
		end

		if monst.faction == player.faction then
			message(strcap(name) .. " is already your pet.")
			return {dam = 0, angry = 0}
		end

		if player.has_intrinsic(FLAG_AGGRAVATE) then
			message(strcap(name) .. " hates you too much!")
			return {dam = 0, angry = 1}
		end

		if monst.flags[FLAG_NO_SYMBIOSIS] or
			monst.flags[FLAG_NO_HYPNOTIZE]
		then
			message(strcap(name) .. " resists!")
			return {dam = 0, angry = 1}
		end

		if monst.level > rng(1, max(1, state.dam - 10) + 10) then
			message(strcap(name) .. " resists!")
			return {dam = 0, angry = 1}
		end

		message(strcap(name) .. " is in your thrall!")

		factions.change_faction(monst, player.faction)

        return{dam = 0, angry = 0}
    end
} -- dam.CHARM_SYMBIOTE

-- Charm symbiote spel
symbiosis.SPELL_INATE_CHARM = add_spell
{
	["name"]       = "Charm symbiote",
	["school"]     = symbiosis.SCHOOL,
	["level"]      = 5,
	["mana"]       = 2,
	["mana_max"]   = 2,
	["fail"]       = 20,
	["random"]     = -1,
	["antimagic"]  = FALSE,
	["anti_magic"] = FALSE,
	["spell"]     = function()
		local ret, dir

		ret, dir = get_aim_dir()
		if not ret then return SPELL_NOTHING end

		return fire_bolt(dam.CHARM_SYMBIOTE, dir,
						 get_skill(SKILL_SYMBIOTIC) * 3 / 2)
	end,
	["info"] = 	function()
		return "power " .. (get_skill(SKILL_SYMBIOTIC) * 3 / 2)
	end,
	["desc"] =	{
		"Tries to charm a symbiotic monster into being your pet,",
		"after which you can hypnotize and wear it.  Does not work",
		"on uniques."
	}
} -- Charm spell

-- Share life spell
symbiosis.SPELL_INATE_SHARE = add_spell
{
	["name"]       = "Share life",
	["school"]     = symbiosis.SCHOOL,
	["level"]      = 9,
	["mana"]       = 5,
	["mana_max"]   = 5,
	["fail"]       = 30,
	["random"]     = -1,
	["antimagic"]  = FALSE,
	["anti_magic"] = FALSE,
	["spell"]     = function()
		local symb = player.inventory[INVEN_SYMBIOTE][1]
		if not symb then
			message(color.LIGHT_RED, "ERROR: You have no symbiote!")
			return SPELL_NOTHING
		end

		local monst = symb.flags[FLAG_MONSTER_OBJ]
		if not monst then
			message(color.LIGHT_RED, "ERROR: The symbiote object has " ..
					"no MONSTER_OBJ!")
			return SPELL_NOTHING
		end

		local name, pronoun, possessive =
			symbiosis.get_symbiote_name(symb)

		local player_pct = player.chp() * 100 / player.mhp()
		local monst_pct  = monst.hp     * 100 / monst.maxhp
		local avg_pct    = (player_pct + monst_pct) / 2

		if monst_pct == player_pct then
			message("You and " .. name .. " are equally healthy, " ..
					"sharing life would do nothing.")
			return SPELL_NOTHING
		end

		message("You and " .. name .. " share life.")

		player.chp(player.mhp() * avg_pct / 100)
		monst.hp = monst.maxhp * avg_pct / 100

		if monst.hp <= 0 then
			message(strcap(name) .. " dies sharing " .. possessive ..
					" health with you.")
			symbiosis.kill_symbiote()
		end

		player.redraw[FLAG_PR_MH]     = true
		player.redraw[FLAG_PR_HP]     = true
		player.redraw[FLAG_PR_HEALTH] = true
	end,
	["info"] = 	function()
		if not player.inventory[INVEN_SYMBIOTE][1] then
			return "Need a symbiote."
		else
			return ""
		end
	end,
	["needs_symb"] = true,
	["desc"] =	{
		"Shares health between you and your symbiote.  The one who is",
		"more damaged will become healthier, and the other one will",
		"become less healthy."
	}
} -- Share life spell

-- Share life spell
symbiosis.SPELL_INATE_HEAL = add_spell
{
	["name"]       = "Heal symbiote",
	["school"]     = symbiosis.SCHOOL,
	["level"]      = 29,
	["mana"]       = 14,
	["mana_max"]   = 14,
	["fail"]       = 40,
	["random"]     = -1,
	["antimagic"]  = FALSE,
	["anti_magic"] = FALSE,
	["spell"]     = function()
		local symb = player.inventory[INVEN_SYMBIOTE][1]
		if not symb then
			message(color.LIGHT_RED, "ERROR: You have no symbiote!")
			return SPELL_NOTHING
		end

		local monst = symb.flags[FLAG_MONSTER_OBJ]
		if not monst then
			message(color.LIGHT_RED, "ERROR: The symbiote object has " ..
					"no MONSTER_OBJ!")
			return SPELL_NOTHING
		end

		local name = symbiosis.get_symbiote_name(symb)

		if monst.hp == monst.maxhp then
			message(strcap(name) .. " is aleady completely healthy.")
			return SPELL_NOTHING
		end

		local pct = 15 + get_skill_scale(SKILL_SYMBIOTIC, 35)
		local heal = monst.maxhp * pct / 100

		monst.hp = monst.hp + heal

		if monst.hp >= monst.maxhp then
			monst.hp = monst.maxhp
			message(strcap(name) .. " is completely healed.")
		else
			message(stcap(name) .. " is partially healed.")
		end

		player.redraw[FLAG_PR_MH] = true
	end,
	["info"] = 	function()
		if not player.inventory[INVEN_SYMBIOTE][1] then
			return "Need a symbiote."
		else
			return "Heals " .. (15 + get_skill_scale(SKILL_SYMBIOTIC, 35)) ..
				"%"
		end
	end,
	["needs_symb"] = true,
	["desc"] =	{
		"Induce your symbiote to heal its wounds."
	}
} -- Heal spell

-- Summon spell
symbiosis.SPELL_SUMMON_SYMBIOTE = add_spell
{
	["name"]       = "Summon symbiote",
	["school"]     = symbiosis.SCHOOL,
	["level"]      = 59,
	["mana"]       = 35,
	["mana_max"]   = 35,
	["fail"]       = 60,
	["random"]     = -1,
	["antimagic"]  = FALSE,
	["anti_magic"] = FALSE,
	["spell"]      = function()
		local ret, summoned = 
			summon_monster(player.py, player.px, get_cast_level(50),
						   player.faction, {flags = {SYMBIOTE = true}})

			if getn(summoned) == 0 then
				message("You failed to summon anything.")
				return
			end

			local seen, unseen = 0, 0
			for i = 1, getn(summoned) do
				local monst = summoned[1]
				monst.faction = player.faction

				if monst.ml then
					seen = seen + 1
				else
					unseen = unseen + 1
				end
			end

			if seen > 1 then
				message("You summoned more than one pet!")
			elseif seen == 1 then
			elseif unseen > 0 then
				message("You summoned a pet, but you can't seem to see it.")
			else
				error("IMPOSSIBLE!")
			end
		end,
	["info"] = 	function()
			return ""
	end,
	["desc"] =	{
		"Releases pheromones into the air to summon a symbiotic pet."
	}
} -- Summon spell

function symbiosis.use_symbiosis_spells()
	-- create a symbiosis book
	local symb_book = make_book
	(
	 symbiosis.SPELL_INATE_POWER,
	 symbiosis.SPELL_INATE_HYPNO,
	 symbiosis.SPELL_INATE_RELEASE,
	 symbiosis.SPELL_INATE_CHARM,
	 symbiosis.SPELL_INATE_SHARE,
	 symbiosis.SPELL_INATE_HEAL,
	 symbiosis.SPELL_SUMMON_SYMBIOTE
    )

	local spl = nil
	local ret, rep = repeat_keys.pull()
	if ret then
		if is_ok_spell(rep, symb_book) then
			spl = rep
		end
	end

	-- select a spell from that book
	spl = spl or get_school_spell("cast", "is_ok_spell", symb_book)

	-- and delete it.
	delete_object(symb_book)

	if spl == -1 then
		return
	end

	if spell(spl).needs_symb and not player.inventory[INVEN_SYMBIOTE][1] then
		message("You need to be in symbiosis to do that.")
		return
	end

	cast_school_spell(spl, spell(spl), false,
		-- Custom "okay to cast this spell?" function
		function(s, s_ptr)
			if player.has_intrinsic(FLAG_CONFUSED) then
				message("You are too confused!")
				return false
			end
			return true
		end) -- cast_school_spell()
end -- symbiosis.use_symbiosis_spells()

function symbiosis.use_symbiote_power()
	local symb = player.inventory[INVEN_SYMBIOTE][1]
	if not symb then
		message(color.LIGHT_RED, "ERROR: no smybiote whose powers can " ..
				"be used!")
		return false
	end

	local monst = symb.flags[FLAG_MONSTER_OBJ]
	if not monst then
		message(color.LIGHT_RED, "ERROR: symbiote object has no monster!")
		return false
	end

	-- Copy spells
	local flag_spells = monst.spells

	local spells = {}
	local skill  = max(get_skill(SKILL_SYMBIOTIC), 0)

	-- List the monster spells
	for i = 1, flag_spells.size do
		if (flag_spells.node[i].flags & FLAG_FLAG_USED) ~= 0 then
			local spl   = flag_spells.node[i].key
			local level = flag_get(flag_spells, spl)

			local success        = flag_get2(flag_spells, spl)
			local player_success = spell(spl).player_success

			if player_success then
				success = player_success(spl, level, success)
			end

			if level > skill and skill < 100 then
				-- Decrease level and increase failure rate if the
				-- casting level is greater than the symbiosis skill
				local fail = 100 - success
				local pct = 50 - (50 * skill / max(level, 1))
				level = level * pct / 100

				pct = 200 - (100 * skill / max(level, 1))
				fail = fail * pct / 100
				success = 100 - fail
			elseif skill > level then
				-- Increase success rate if symbiosis skill is
				-- greater than the casting level
				success = success * skill / max(level, 1)
			end

			if success < 0 then
				success = 0
			elseif success > 100 then
				success = 100
			end

			set_auto_cast(0, level, player.py, player.px)
			set_monst_helper(monst)
			local mana = get_mana(spl)
			unset_auto_cast()
			unset_monst_helper()

			local free_use = spell(spl).free_use
			if free_use and tag(free_use) ~= TAG_FUNCTION then
				mana = 0
			end

			local use_msg, warn_msg
			local can_use = true
			local clr     = color.LIGHT_GREEN

			if spell(spl).level > skill and skill < 100 then
				can_use = false
				clr     = color.LIGHT_DARK
				use_msg = "Your symbiosis skill is not high enough to " ..
					"use this spell."
				level   = skill - spell(spl).level
			elseif success <= 0 then
				can_use = false
				clr     = color.LIGHT_DARK
				use_msg = "No chance of success."
			elseif mana > player.csp() then
				success = success - (5 * (mana - player.csp()))

				if success <= 0 then
					can_use = false
					clr     = color.LIGHT_DARK
					use_msg = "No chance of success."
				else
					clr      = color.ORANGE
					warn_msg = "You don't have enough mana to cast this " ..
						"spell, attempt anyways?"
				end
			end

			-- Verify level
			if not (spell(spl).no_player or spell(spl).no_symb) then
				tinsert(
						spells,
						{
							index     = spl,
							school    = spell(spl).school
							success   = success,
							level     = level,
							raw_level = flag_get(flag_spells, spl)
							mana      = mana,
							can_use   = can_use,
							use_msg   = use_msg,
							warn_msg  = warn_msg or spell(spl).warn_msg,
							color     = clr,
						}
					)
			end -- if not spell(spl).no_user then
		end
	end

	if getn(spells) == 0 then
		message("Your symbiote has no spells which you can use.")
		return false
	end

	-- Sort by level and name
	sort(spells,
		 function(s1, s2)
			 return
				 (s1.raw_level < s2.raw_level) or
				 ((s1.raw_level == s2.raw_level) and
				  (spell(s1.index).name < spell(s2.index).name))
		 end)

	-- Ask what spell to cast
	local ret = choose_from_list(
		spells, "Spells", "Cast which spell?",
		function (y, x, thing)
			if not thing then
				term.blank_print(format("%-26s Level Cost Fail Info",
										"Name"), y, x)
			else
				local str
				local color = thing.color
				set_get_level(thing.level)
				str = format("%-26s   %3d %4s %3d%s %s",
							 spell(thing.index).name,
							 thing.level, thing.mana,
							 100 - thing.success, "%",
							 __spell_info[thing.index]())
				term.blank_print(color, str, y, x)
				unset_get_level()
			end
		end,
		function (y, thing)
			set_get_level(thing.level)
			print_spell_desc(thing.index, y)
			unset_get_level()
		end
	) -- choose_from_list()

	-- Something to do ?
	if ret then
		if ret.can_use and ret.warn_msg then
			message(ret.warn_msg)
			local press, key = get_com("Cast anyways? (y/n)")

			if not press then
				ret.can_use = false
			end

			local key = strlower(strchar(key))

			if key ~= "y" then
				ret.can_use = false
			end
		end

		energy_use = get_player_energy(SPEED_SYMBIOSIS)

		if not ret.can_use then
			if ret.use_msg then
				message(ret.use_msg)
			end
			energy_use = 0
			ret.mana = 0
		elseif symbiosis.spell_is_disrupted(ret.index) then
			-- Spell disrupted, do nothing.
			ret.mana = 0
		elseif not rng.percent(ret.success) then
			message("You fail to synchronize with your symbiote!")
		else
			set_get_level(ret.level)
			set_monst_helper(monst)
			local spell_ret = __spell_spell[ret.index]()

			if spell_ret == SPELL_NOTHING then
				energy_use = 0
			else
				local free_use = spell(ret.index).free_use
				if free_use and tag(free_use) == TAG_FUNCTION and
					free_use(ret)
				then
					ret.mana = 0
				end

				if ret.mana > player.csp() then
				end

			end -- if ret ~= SPELL_NOTHING then
			unset_get_level()
			unset_monst_helper()
		end -- can use and fail check passed

		if ret.mana > player.csp() then
			ret.mana =
				symbiosis.insuff_power_effect(ret.index, ret.mana,
											  player.csp())
		end
		increase_mana(-ret.mana)
	end -- if ret then

	return true
end -- symbiosis.use_symbiote_power()

--------------------------------------------------------------------------

--
-- Some utility functions
--

function symbiosis.get_symbiote_name(symb)
	symb = symb or player.inventory[INVEN_SYMBIOTE][1]

	if not symb then
		message(color.LIGHT_RED, "ERROR: no symbiote to get name from!")
		return "NO SYMBIOTE", "NO SYMBIOTE", "NO SYMBIOTE"
	end

	local monst = symb.flags[FLAG_MONSTER_OBJ]

	local pronoun, possessive

	if monst.flags[FLAG_MALE] then
		pronoun    = "he"
		possessive = "his"
	elseif monst.flags[FLAG_FEMALE] then
		pronoun    = "she"
		possessive = "her"
	else
		pronoun    = "it"
		possessive = "its"
	end

	local name

	-- Did player try to give speical name to symbiote?
	if symb.note > 0 and strfind(quark_str(symb.note), "#named ", 1, 1) then
		local str = quark_str(symb.note)

		local start, finish, note_name =
			strfind(str, "#named ([^%$%^]*)")

		-- Paranoia
		note_name = note_name or ""

		note_name = clean_whitespace(note_name)

		if note_name ~= "" then
			name = note_name
		end
	end

	local special = false
	if name then
		-- Player gave symbiote a (valid) special name
		special = true
	elseif monst.flags[FLAG_UNIQUE] then
		name = monster_desc(monst, 512 | 256 | 128)
	else
		name = "your " .. monster_desc(monst, 512 | 256 | 128)
	end

	return name, pronoun, possessive, special
end -- symbiosis.get_symbiote_name()

function symbiosis.kill_symbiote()
	local symb = player.inventory[INVEN_SYMBIOTE][1]

	if not symb then
		message(color.LIGHT_RED, "ERROR: No symbiote to kill!")
		return
	end

	local item = compute_slot(INVEN_SYMBIOTE, 1)

    item_increase(item, -symb.number)
    item_optimize(item)

	player.redraw[FLAG_PR_MH] = true
	term.disturb(1, 0)
end

function symbiosis.get_rebellion_chance(symb)
	symb = symb or player.inventory[INVEN_SYMBIOTE][1]

	if not symb then
		return 0
	end

	local monst = symb.flags[FLAG_MONSTER_OBJ]

	return monst.level - (player.lev + get_skill_scale(SKILL_SYMBIOTIC, 50))
end

function symbiosis.silent_or(msg, verb, ...)
	for i = 1, getn(arg) do
		if arg[i] then
			return true
		end
	end
	return false
end -- symbiosis.silent_or()

function symbiosis.verb_or(msg, verb, ...)
	local ret = false

	for i = 1, getn(arg) do
		if arg[i] then
			ret = true
			if tag(arg[i]) == TAG_STRING then
				msg = arg[i]
				break
			end
		end
	end

	if ret and msg then
		msg = gsub(msg, "(@verb@)", verb)
		message(msg)
	end

	return ret
end -- symbiosis.verb_or()

function symbiosis.can_symbiotize(verb)
	local do_or

	if verb then
		do_or = symbiosis.verb_or
	else
		do_or = symbiosis.silent_or
	end

	verb = verb or ""

	if get_skill(SKILL_SYMBIOTIC) < 1 then
		if verb ~= "" then
			message("You cannot " .. verb .. " symbiosis without " ..
					"any symbiosis skill.")
		end
		return false
	end

	-- Check monster body
	if do_or("Your current body cannot @verb@ symbiosis.", verb,
			 player.body_monster.flags[FLAG_NO_SYMBIOSIS])
	then
		return false
	end

	-- Check descriptors
	local descriptor_type = {
		race = "You cannot @verb@ symbiosis because of your race.",
		subrace = "You cannot @verb@ symbiosis because of your subrace.",
		race_background = "You cannot @verb@ symbiosis  because of your background.",
		class = "You cannot @verb@ symbiosis because of your class.",
		subclass = "You cannot @verb@ symbiosis because of your subclass."
		undead = "You cannot @verb@ symbiosis because of your undead form."
	}

	for type_name, msg in descriptor_type do
		local desc_num = find_player_descriptor(type_name)

		if desc_num ~= -1 then
			local desc = descriptor_info(desc_num)

			if do_or(msg, verb, desc.flags[FLAG_NO_SYMBIOSIS]) then
				return false
			end
		end
	end -- for type_name, msg in descriptor_type do

	-- Check equipment
	local equip = item_hooks.collect_equip(FLAG_NO_SYMBIOSIS)
	if getn(equip) > 0 then
		local msg
		local known
		for i = 1, getn(equip) do
			local obj = equip[i]

			if is_known(obj) and known_flag(obj, FLAG_NO_SYMBIOSIS) then
				known = true
			end

			if tag(equip.flags[FLAG_NO_SYMBIOSIS]) == TAG_STRING then
				msg = equip.flags[FLAG_NO_SYMBIOSIS]
				break
			end
		end

		if verb == "" then
			return
		end

		if not msg then
			if known then
				msg = "Your equipment is preventing you from @verb@ " ..
					"symbiosis."
			else
				msg = "You cannot @verb@ symbiosis for some reason."
			end
		end

		msg = gsub(msg, "(@verb@)", verb)
		message(verb)
		return false
	end

	-- If NO_SYMBIOSIS is an intrinisc, it's been set by a timed effect,
	-- since player.apply_flags() doesn't copy it from equipment.
	if player.has_intrinsic(FLAG_NO_SYMBIOSIS) then
		do_or("You cannot @verb@ symbiosis for some reason.", verb,
			  player.intrinsic(FLAG_NO_SYMBIOSIS))
		return false
	end

	if do_or("You cannot @verb@ symbiosis for some reason.", verb,
			 level_flags.flags[FLAG_NO_SYMBIOSIS])
	then
		return false
	end
		
	return true
end -- symbiosis.can_symbiotize()

function symbiosis.force_drop()
	local symb = player.inventory[INVEN_SYMBIOTE][1]

	if not symb then
		message(color.LIGHT_RED,
				"ERROR: no symbiote for symbiosis.force_drop() to " ..
					"work on.")
		return
	end

	local monst = symb.flags[FLAG_MONSTER_OBJ]
	if not monst then
		message(color.LIGHT_RED,
				"ERROR: symbiote has no MONSTER_OBJ!")
	else
		-- Takeoff pre isn't going to stop it, but call it anyways,
		-- in case it wants to do something besides prevent a
		-- takeoff.
		if monst.flags[FLAG_TAKEOFF_PRE] then
			local func
			func = get_func_from_flag_or_registry(monst.flags,
												  FLAG_TAKEOFF_PRE)

			func(symb, monst)
		end

		if monst.flags[FLAG_TAKEOFF_POST] then
			local func
			func = get_func_from_flag_or_registry(monst.flags,
												  FLAG_TAKEOFF_POST)

			func(symb, monst)
		end
	end

	local item = compute_slot(INVEN_SYMBIOTE, 1)
	inven_drop(item, symb.number, player.py, player.px, false)

	player.redraw[FLAG_PR_MH] = true
	term.disturb(1, 0)
end -- symbiosis.force_drop()

function symbiosis.is_smart_caster(monst)
	if monst.flags[FLAG_AI] and monst.flags[FLAG_AI] == ai.SMART_CAST then
		return true
	elseif (monst.flags[FLAG_AI_STATES] and
			monst.flags[FLAG_AI_STATES][FLAG_CASTING_AI] and
				monst.flags[FLAG_AI_STATES][FLAG_CASTING_AI] == ai.SMART_CAST)
	then
		return true
	end

	return false
end -- symbiosis.is_smart_caster()

function symbiosis.choose_attack_spell(monst, target, smart_cast)
	local use_field, dont_use_field

	if target then
		-- Attacking on player's behalf
		use_field      = "symb_attack"
		dont_use_field = "symb_no_attack"
	else
		-- Rebelling
		use_field      = "symb_rebel"
		dont_use_field = "symb_no_rebel"
	end

	local spell_list = {}
	for i = 1, monst.spells.size do
		if (monst.spells.node[i].flags & FLAG_FLAG_USED) ~= 0 then
			local idx     = monst.spells.node[i].key
			local level   = flag_get(monst.spells,  idx)
			local success = flag_get2(monst.spells, idx)
			local spl     = spell(idx)
			local tactics = spl.tactical or {}

			-- Use attack, tactical and annoyance spells, as long as they
			-- don't summon annything or try to get us to escape (but
			-- escaping is okay if we're attacking the player, to
			-- cause an annoyance; also don't use TACTIC while rebelling).
			if spl[use_field] or
				(not spl[dont_use_field] and
				 (tactics[ai.tactics.ATTACK] or
				  tactics[ai.tactics.ANNOY] or
				  (target and tactics[ai.tactics.TACTIC])) and
				 not (tactics[ai.tactics.SUMMON] or
					  (target and tactics[ai.tactics.ESCAPE])))
			then
				if not symbiosis.spell_is_disrupted(idx, true) then
					tinsert(spell_list, {
								idx     = idx,
								spl     = spl,
								level   = level,
								success = success,
								tactics = tactics,
								mana    = spl.mana,
							})
				end
			end
		end -- if (monst.spells.node[i].flags & FLAG_FLAG_USED) ~= 0 then
	end -- for i = 1, monst.spells.size do

	-- Spells cast while rebelling are free
	if target then
		for i = getn(spell_list), 1, -1 do
			if spell_list[i].mana > monst.flags[FLAG_MANA_CURR] then
				tremove(spell_list, i)
			end
		end
	end

	if getn(spell_list) == 0 then
		return nil
	end

	local spell_info
	if smart_cast then
		sort(spell_list,
			 function(s1, s2)
				 -- First attack spells, then tactical spells,
				 -- then annoyance spells.
				 if s1.tactics[ai.tactics.ATTACK] then
					 return s1.tactics[ai.tactics.ATTACK] >
						 (s1.tactics[ai.tactics.ATTACK] or 0)
				 end
				 if s1.tactics[ai.tactics.TACTIC] then
					 return s1.tactics[ai.tactics.TACTIC] >
						 (s1.tactics[ai.tactics.TACTIC] or 0)
				 end
				 if s1.tactics[ai.tactics.ANNOY] then
					 return s1.tactics[ai.tactics.ANNOY] >
						 (s1.tactics[ai.tactics.ANNOY] or 0)
				 end
			 end)
		spell_info = spell_list[1]
	else
		-- Non-smart monsters pick a random spell
		spell_info = spell_list[rng(getn(spell_list))]
	end

	return spell_info
end -- symbiosis.choose_attack_spell()

function symbiosis.choose_heal_spell(monst, target, smart_cast, want_mana,
									 want_life)
	local heal_list, mana_list = {}, {}
	for i = 1, monst.spells.size do
		if (monst.spells.node[i].flags & FLAG_FLAG_USED) ~= 0 then
			local idx     = monst.spells.node[i].key
			local level   = flag_get(monst.spells,  idx)
			local success = flag_get2(monst.spells, idx)
			local spl     = spell(idx)
			local tactics = spl.tactical or {}

			local tmp = {
				idx     = idx,
				spl     = spl,
				level   = level,
				success = success,
				tactics = tactics,
				mana    = spl.mana,
			}

			-- Use appropriate type of spell, as long as it also
			-- attacks or annoys, and doesn't summon annything or
			-- try to get us to escape.
			if not (symbiosis.spell_is_disrupted(idx, true) or
					spl.symb_no_rebel)
			then
				if want_life and (spl.symb_heal or
					(not spl.symb_no_heal and tactics[ai.tactics.HEAL] and
					 (tactics[ai.tactics.ATTACK] or
					  tactics[ai.tactics.ANNOY]) and
						 not (tactics[ai.tactics.SUMMON] or
							  tactics[ai.tactics.ESCAPE])))
				then
					tinsert(heal_list, tmp)
				end
				if want_mana and (spl.symb_mana or
					(not spl.symb_no_mana and tactics[ai.tactics.MANA] and
					 (tactics[ai.tactics.ATTACK] or
					  tactics[ai.tactics.ANNOY]) and
						 not (tactics[ai.tactics.SUMMON] or
							  tactics[ai.tactics.ESCAPE])))
				then
					tinsert(mana_list, tmp)
				end
			end
		end -- if (monst.spells.node[i].flags & FLAG_FLAG_USED) ~= 0 then
	end -- for i = 1, monst.spells.size do

	-- Spells cast while rebelling are free
	if target then
		for i = getn(heal_list), 1, -1 do
			if heal_list[i].mana > monst.flags[FLAG_MANA_CURR] then
				tremove(heal_list, i)
			end
		end

		for i = getn(mana_list), 1, -1 do
			if mana_list[i].mana > monst.flags[FLAG_MANA_CURR] then
				tremove(mana_list, i)
			end
		end
	end

	local spell_list = {}
	local tactic
	if want_life and getn(heal_list) >= 1 then
		spell_list = heal_list
		tactic     = ai.tactics.HEAL
	elseif want_mana and getn(mana_list) >= 1 then
		spell_list = mana_list
		tactic     = ai.tactics.MANA
	end

	if getn(spell_list) == 0 then
		return nil
	end

	sort(spell_list,
		 function(s1, s2)
			 if s1.tactics[%tactic] then
				 return s1.tactics[%tactic] > (s1.tactics[%tactic] or 0)
			 end
			 return s1.success > s2.success
		 end)

	return spell_list[1]
end -- symbiosis.choose_heal_spell()

function symbiosis.do_spell(target, heal)
	local symb  = player.inventory[INVEN_SYMBIOTE][1]
	local monst = symb.flags[FLAG_MONSTER_OBJ]

	local smart_cast = symbiosis.is_smart_caster(monst)

	-- Can only specifically choose a healing/mana spell if the
	-- caster is smart.
	if (heal and not smart_cast) then
		return false
	end

	local spell_info
	if heal then
		local want_mana, want_life
        local cur_mana, max_mana =
			get_flag(monst, FLAG_MANA_CURR), get_flag(monst, FLAG_MANA_MAX)

		if monst.hp < (monst.maxhp / 3) then
			want_life = true
		end
		if cur_mana < (max_mana / 3) then
			want_mana = true
		end
		if not (want_mana or want_life) then
			-- Don't need healing or mana, do something else
			return false
		end

		spell_info = symbiosis.choose_heal_spell(monst, target, smart_cast,
												 want_mana, want_life)
	else
		spell_info = symbiosis.choose_attack_spell(monst, target, smart_cast)
	end

	if not spell_info then
		return false
	end

	local failed = rng.percent(100 - spell_info.success)

	local name = symbiosis.get_symbiote_name()
	monst.flags[FLAG_FORCE_MONST_NAME] = name

	local no_msg_field, msg_field, msg_failed_field
	if target then
		no_msg_field     = "symb_attack_no_msg"
		msg_field        = "symb_attack_msg"
		msg_failed_field = "symb_attack_msg_failed"
	else
		no_msg_field     = "symb_rebel_no_msg"
		msg_field        = "symb_rebel_msg"
		msg_failed_field = "symb_rebel_msg_failed"
	end

	if failed then
		msg_field = msg_field_failed
	end

	if not spell_info.spl[no_msg_field] then
		local target_name, blind, unseen_target
		if target then
			blind = player.has_intrinsic(FLAG_BLIND) and
				not spell_info.spl.ignore_blind
			unseen_target = not target.ml and
				not spell_info.spl.ignore_unseen
			target_name = monster_desc(target, 0)
		else
			blind         = false
			unseen_target = false
			target_name   = "you"
		end

		local monst_message =
			get_monster_spell_msg(spell_info.spl, failed, monst, target,
								  blind, false, unseen_target)

		if spell_info.spl[msg_field] then
			local msg = spell_info.spl[msg_field]
			msg = gsub(msg, "(@Source@)", strcap(name))
			msg = gsub(msg, "(@source@)", name)
			monst_message = msg
		end			

		if monst_message then
			if monst_message ~= "" then
				message(monst_message)
			end
		else
			if failed then
				message(strcap(name) .. " failed to cast " ..
						spell_info.spl.name .. " at " .. target_name .. ".")
			else
				message(strcap(name) .. " casts " .. spell_info.spl.name ..
						" at " .. target_name .. ".")
			end
		end
	end -- if not spell_info.spl[no_msg_field] then

	-- Spells cast while rebelling are free
	if target then
		monst.flags[FLAG_MANA_CURR] =
			monst.flags[FLAG_MANA_CURR] - spell_info.mana
	end

	if failed then
		monst.flags[FLAG_FORCE_MONST_NAME] = nil
		return true
	end

	if target then
		set_auto_cast(0, spell_info.level, target.fy, target.fx)
	else
		set_auto_cast(0, spell_info.level, player.py, player.px)
	end

	local spell_ret = spell_info.spl.spell()
	unset_auto_cast()
	monst.flags[FLAG_FORCE_MONST_NAME] = nil

	return true
end -- symbiosis.do_spell()

----------------------------------------------------------------------------

--
-- Bonus calculations
--

function symbiosis.scale_flags(flags_in, percent, flags_out)
	local flags_out = flags_out or flag_dup(flags_in)

	local mask = (FLAG_FLAG_BOTH | FLAG_FLAG_FLAGS | FLAG_FLAG_PTR)

	for i = 1, flags_out.size do
		local node = flags_out.node[i]
		if (node.flags & FLAG_FLAG_USED) ~= 0 then
			if (node.flags & mask) == 0 then
				local val = flag_get(flags_out, node.key)
				flag_set(flags_out, node.key, val * percent / 100)
			elseif (node.flags & FLAG_FLAG_FLAGS) ~= 0 then
				local sub_flags = flags_out[node.key]
				symbiosis.scale_flags(sub_flags, percent, sub_flags)
			end
		end
	end

	return flags_out
end -- symbiosis.scale_flags()

function symbiosis.calc_obj_bonus(symb)
	local monst = symb.flags[FLAG_MONSTER_OBJ]

	-- Wipe the slate clean; use "zap" to not free any memory.
	local weight = symb.weight
	local elevel = symb.elevel
	local exp    = symb.exp
	local note   = symb.note

	flag_empty_zap(symb.flags)
	object_prep(symb, symb.k_idx)
	symb.weight = weight
	symb.exp    = exp
	symb.elevel = elevel
	symb.note   = note
	symb.marked = true

	symb.flags[FLAG_MONSTER_OBJ] = monst
	if not monst.flags[FLAG_SYMB_NO_DEFAULT] then
		symb.flags[FLAG_RESIST] = flag_dup(monst.flags[FLAG_RESIST])
		symb.flags[FLAG_ABSORB] = flag_dup(monst.flags[FLAG_ABSORB])

		-- If symbiote has no mind, then it's resistance to
		-- confusion and fear can't be shared with the player.
		if monst.flags[FLAG_EMPTY_MIND] then
			symb.flags[FLAG_RESIST][dam.CONFUSION] = 0
			symb.flags[FLAG_RESIST][dam.FEAR]      = 0
		end

		local percent = get_skill_scale(SKILL_SYMBIOTIC, 50)
		symbiosis.scale_flags(symb.flags, percent, symb.flags)

		if monst.flags[FLAG_CAN_FLY] then
			symb.flags[FLAG_CAN_FLY] = 1
		end
		if monst.flags[FLAG_INVISIBLE] then
			symb.flags[FLAG_SEE_INVIS] = 15
		end

		symb.ac = monst.ac * get_skill_scale(SKILL_SYMBIOTIC, 50) / 100
	else
		symb.flags[FLAG_RESIST] = flag_new(2)
		symb.flags[FLAG_ABSORB] = flag_new(2)
	end

	-- This most certainly shouldn't be scaled!
	symb.flags[FLAG_WIELD_SLOT]  = INVEN_SYMBIOTE

	if monst.flags[FLAG_SYMB_BONUS] then
		flag_add_increase(symb.flags, monst.flags[FLAG_SYMB_BONUS])
	end

	if monst.flags[FLAG_SYMB_BONUS_BY_LEVEL] then
		local levels = monst.flags[FLAG_SYMB_BONUS_BY_LEVEL]

		for i = 0, get_skill(SKILL_SYMBIOTIC) do
			local lev = levels[i]

			if lev then
				flag_add_increase(symb.flags, lev)
			end
		end
	end -- if monst.flags[FLAG_SYMB_BONUS_BY_LEVEL] then

	if monst.flags[FLAG_SYMB_BONUS_SCALED] then
		local flags = monst.flags[FLAG_SYMB_BONUS_SCALED]
		local scaled = symbiosis.scale_flags(flags,
											 get_skill(SKILL_SYMBIOTIC))

		flag_add_increase(symb.flags, scaled)
		flag_free(scaled, true)
	end

	-- Learn everything about symbiote
	set_aware(symb)
	set_known(symb)
	flag_learn_all(symb.flags)
	symb.ident = IDENT_KNOWN | IDENT_MENTAL
end -- symbiosis.calc_obj_bonus()

----------------------------------------------------------------------------
-- Symbiote taking damage, or damaging player
----------------------------------------------------------------------------

--
-- Take damage for player
--
hook(hook.PROJECT_P_POST_LUA,
function(who, r, y, x, dmg, typ, a_rad, flags, dir)
	local symb = player.inventory[INVEN_SYMBIOTE][1]

	if not symb or dmg <= 0 or symbiosis.no_take_hit then
		return
	end

	if get_dam_type_info(typ, "bypass_symb") then
		return
	end

	local chance = 5 + get_skill_scale(SKILL_SYMBIOTIC, 50)

	if symbiosis.force_take_hit or not rng.percent(chance) then
		return
	end

	local monst = symb.flags[FLAG_MONSTER_OBJ]
	local name  = strcap(symbiosis.get_symbiote_name(symb))

	if monst.flags[FLAG_EMPTY_MIND] then
		-- Mindless monster's can't defened player from confusion
		if typ == dam.CONFUSION then
			return
		end
	end

	-- Unapply the resist and absorb the symbiote is providing to
	-- the player, then apply the monster's instrinsic resist and
	-- abosrb.
	local orig_dam = dmg

	local unresist = symb.flags[FLAG_RESIST][typ]  or 0
	local unabsorb = symb.flags[FLAG_ABSORB][typ]  or 0
	local resist   = monst.flags[FLAG_RESIST][typ] or 0
	local absorb   = monst.flags[FLAG_ABSORB][typ] or 0

	if unresist == 100 then
		unresist = 99
	end

	-- Unapply
	local symb_dam = dmg * 100 / (100 - unresist) + unabsorb
	-- Apply
	symb_dam = symb_dam * (100 - resist) / 100 - absorb

	if symb_dam <= 0 then
		message(name .. " shields you from the damage.")
		dmg = 0
	elseif monst.hp <= symb_dam then
		dmg = (symb_dam - monst.hp) * dmg / symb_dam

		if dmg >= orig_dam then
			dmg = orig_dam
			message(color.LIGHT_RED, name .. " dies attempting to " ..
					"protect you, you feel very sad...")
		else
			message(color.LIGHT_RED, name .. " dies protecting you, you " ..
					"feel very sad...")
		end

		symbiosis.kill_symbiote()
	else
		message(name .. " takes the damage instead of you.")
		monst.hp = monst.hp - symb_dam
		dmg = 0
	end

	player.redraw[FLAG_PR_MH] = true

	return true, dmg
end) -- hook(hook.PROJECT_P_POST_LUA)

function symbiosis.do_rebellion_blows()
	local symb  = player.inventory[INVEN_SYMBIOTE][1]
	local monst = symb.flags[FLAG_MONSTER_OBJ]

	if monst.flags[FLAG_NEVER_BLOW] then
		return false
	end

    if not flag_exists(monst.blow,0) then
		return false
	end

	-- Anaylzie blows
	local num_blows = 0
	for ap_cnt = 0, flag_max_key(monst.blow) do
		local blow = flag_get_blow(monst.blow, ap_cnt)

		-- Extract the attack infomation
		local effect_idx = blow.effect
		local method_idx = blow.method

		-- no more attacks
		if (method_idx == 0) then break end

		local effect = __monster_attack_types[effect_idx]
		local method = __monster_attack_methods[method_idx]

		local special = {}
		if method.symb_attack then
			blow.method.symb_attack(special, blow.effect, target)
		elseif blow.method.fct then
			blow.method.fct(special, blow.effect, target)
		elseif method.monster and target > 0 then
			blow.method.monster(special, blow.effect, target)
		end

		if not (effect.symb_no_rebel or method.symb_no_rebel or
				special.symb_no_rebel)
		then
			num_blows = num_blows + 1
		end
    end -- for ap_cnt = 0, flag_max_key(monst.blow) do

	-- No usable blows?
	if num_blows == 0 then
		return false
	end

	local name    = symbiosis.get_symbiote_name()
	local y, x    = player.py, player.px
    local c_ptr   = cave(y, x)
	local m_idx   = 0
    local m_ptr   = monst
    local r_ptr   = race_info(m_ptr)
    local t_idx   = WHO_PLAYER
    local t_ptr   = player
    local t_flags = t_ptr.intrinsic_flags

	-- Force monster name to be symbiote name
	monst.flags[FLAG_FORCE_MONST_NAME] = strcap(name)

	-- None of player's blow responses will affect the symbiote
	combat_monster.__no_response = true

	-- The symbiote won't blink away
	combat_monster.__no_blink = true

	-- The caster is WHO_PLAYER, so we need this flag
	combat_monster.__project_flags = PROJECT_CASTER

	-- We're in symbiosis, so the monster can't miss
	combat_monster.__force_hit = true

    local combat_type = m_ptr.flags[FLAG_MONSTER_COMBAT_SYSTEM] or
		combat_monster.__default
	combat_monster.__list[combat_type].attack(y, x, c_ptr, m_idx,
											  m_ptr, t_idx, t_ptr,
											  t_flags)

	-- Reset everything
	combat_monster.__no_response   = false
	combat_monster.__no_blink      = false
	combat_monster.__project_flags = 0
	combat_monster.__force_hit     = false

	monst.flags[FLAG_FORCE_MONST_NAME] = nil

	if combat_monster.__exploded then
		monst.hp = 0
	end

	return true
end -- symbiosis.do_rebellion_blows()

-- Monster couldn't use any blows on player, so lets try magic
function symbiosis.do_rebellion_magic()
	return symbiosis.do_spell()
end -- symbiosis.do_rebellion_magic()

function symbiosis.do_rebellion_heal()
	return symbiosis.do_spell(nil, true)
end -- symbiosis.do_rebellion_magic()

hook(hook.PROCESS_WORLD,
function()
	local symb  = player.inventory[INVEN_SYMBIOTE][1]
	if not symb then
		return
	end

	local monst = symb.flags[FLAG_MONSTER_OBJ]

	if not monst then
		message(color.LIGHT_RED, "ERROR: symbiote object lost its " ..
				"MONSTER_OBJ!")
		symbiosis.kill_symbiote()
		return
	end

	local name, pronoun = symbiosis.get_symbiote_name()

	if monst.hp <= 0 then
		message(strcap(name) .. " died for some strange reason.")
		symbiosis.kill_symbiote()
		return
	end

	-- Let symbiote regenerate
	monster_regen(0, monst)
	player.redraw[FLAG_PR_MH] = true

	if monst.hp <= 0 then
		message(strcap(name) .. " died for HP loss.")
		symbiosis.kill_symbiote()
		return
	end

	local chance = symbiosis.get_rebellion_chance()
	if rng(1000) >= chance then
		return
	end

	-- Symbiote is rebelling!
	term.disturb(1, 0)
	message(strcap(name) .. " thinks you are not enough in symbiosis.")

	-- Set things up
	set_monst_helper(monst, true)
	force_die_from_msg("unruly symbiote")
	monst.ml              = true
	monst.fy              = player.py
	monst.fx              = player.px
	monst.faction         = factions.get_hostile_faction(monst)
	symbiosis.rebelling   = true
	symbiosis.no_take_hit = true	

	local func
	if monst.flags[FLAG_SYMB_REBEL] then
		func = get_func_from_flag_or_registry(monst.flags, FLAG_SYMB_REBEL)
	else
		func = function() return false end
	end

	local dropped = false
	if func(symb, monst) then
		-- Symbiote did custom rebelling.
	elseif symbiosis.do_rebellion_heal() then
		-- Symbiote refreshed itself off of the player
	elseif symbiosis.do_rebellion_blows() then
		-- Symbiote used blows on player.
	elseif symbiosis.do_rebellion_magic() then
		-- No blows available, so a spell was used.
	else
		-- No blows *or* spells avaiable, so the only option left
		-- is to break symbiosis.
		monst.faction = player.faction
		dropped = true

		message(strcap(pronoun) .. " falls off in protest!")
		symbiosis.force_drop()
	end

	-- Undo setup
	unset_monst_helper()
	clear_die_from_msg()
	symbiosis.no_take_hit = false
	symbiosis.rebelling   = false

	if not dropped then
		monst.faction = player.faction

		if monst.hp <= 0 then
			message(strcap(name) .. " died attacking you.")
			symbiosis.kill_symbiote()
		end
	end
	player.redraw[FLAG_PR_MH] = true
end) -- hook(hook.PROCESS_WORLD)

---------------------------------------------------------------------------

--
-- Attack monster when player hits it.
--

function symbiosis.do_attack_blows(target_monst)
	local symb       = player.inventory[INVEN_SYMBIOTE][1]
	local symb_monst = symb.flags[FLAG_MONSTER_OBJ]

	if not flag_exists(symb_monst.blow, 0) then
		return false
	end

	if symb_monst.flags[FLAG_NEVER_BLOW] then
		return false
	end

	local y, x  = target_monst.fy, target_monst.fx
	local c_ptr = cave(y, x)
	local m_idx = c_ptr.m_idx

	if m_idx == 0 then
		message(color.LIGHT_RED, "ERROR: no monster for symbiote to attack.")
		return false
	end

	if target_monst ~= monster(m_idx) then
		message(color.LIGHT_RED, "ERROR: wrong monster for symbiote " ..
				"to attack.")
		return false
	end

	local blows = {}
	for ap_cnt = 0, flag_max_key(symb_monst.blow) do
		local blow_struct = flag_get_blow(symb_monst.blow, ap_cnt)

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

		local special = {}
		if blow.method.symb_attack then
			blow.method.symb_attack(special, blow.effect, WHO_PLAYER)
		elseif blow.method.fct then
			blow.method.fct(special, blow.effect, WHO_PLAYER)
		elseif blow.method.monster then
			blow.method.monster(special, blow.effect, WHO_PLAYER)
		end

		if not (blow.method.symb_no_attack or
				blow.effect.symb_no_attack or
				special.symb_no_attack)
		then
			tinsert(blows, blow)
		end
	end -- for ap_cnt = 0, flag_max_key(monst.blow) do

	if getn(blows) == 0 then
		return false
	end

	local target_name       = monster_desc(target_monst, 0)
	local target_possessive = monster_desc(target_monst, 34)
	local symb_name         = symbiosis.get_symbiote_name(symb)

	for i = 1, getn(blows) do
		local blow = blows[i]

		local params = {}
		params.m_ptr   = symb_monst
		params.m_name  = symb_name
		params.effect  = blow.effect
		params.method  = blow.method
		params.t_ptr   = target_monst
		params.t_name  = target_name
		params.t_flags = target_monst.flags
		params.t_luck  = 0

		local done = hook.process(hook.HANDLE_MONSTER_BLOW, params)

		if not done and (blow.effect.name == "*" or
			combat_monster.default.check_hit(blow.effect.power,
											 symb_monst.level,
											 target_monst.ac, 0))
		then
			local special = {}

			if blow.method.symb_attack then
				blow.method.symb_attack(special, blow.effect, WHO_PLAYER)
			elseif blow.method.fct then
				blow.method.fct(special, blow.effect, WHO_PLAYER)
			elseif blow.method.monster then
				blow.method.monster(special, blow.effect, WHO_PLAYER)
			end

			local act = special.action or blow.method.action

			if act and act ~= "" then
				act = gsub(act, "(@target@)", target_name)
				act = gsub(act, "(@target_possessive@)", target_possessive)
				message(strcap(symb_name) .. " " .. act .. ".")
			end

			params.dam = rng.roll(blow.d_dice, blow.d_side)

			if blow.effect.symb_attack then
				blow.effect.symb_attack(params, special, blow.method,
										WHO_PLAYER)
			elseif blow.effect.fct then
				blow.effect.fct(params, special, blow.method,
								WHO_PLAYER)
			elseif blow.effect.monster then
				blow.effect.monster(params, special, blow.method,
									WHO_PLAYER)
			end

			if (blow.effect.type > 0) and not special.explode then
				project(WHO_PLAYER, 0, y, x, params.dam, blow.effect.type,
						PROJECT_KILL | PROJECT_STOP | PROJECT_HIDE |
						PROJECT_HIDE_BLAST | PROJECT_NO_REFLECT)
			elseif special.explode then
				local hp = symb_monst.hp
				project(WHO_PLAYER, 0, player.py, player.px, hp + 1,
						blow.effect.type,
						PROJECT_KILL | PROJECT_STOP | PROJECT_HIDE |
						PROJECT_HIDE_BLAST | PROJECT_CASTER |
						PROJECT_NO_REFLECT)
				return true
			end

			-- Monster is gone?
			if target_monst.r_idx == 0 or c_ptr.m_idx == 0 then
				return true
			end

			-- Blow responses (passive attacks).  Force symbiote to take
			-- hit for this, since it was the one that touched the
			-- monster.
			symbiosis.force_take_hit = true
			if flag_exists(target_monst.flags, FLAG_BLOW_RESPONSE) then
				local responses = target_monst.flags[FLAG_BLOW_RESPONSE]

				for i = 1, flag_max_key(responses) do
					if responses[i] then
						local blow_response = get_blow_response(i)

						blow_response.monster_blow(WHO_PLAYER, m_idx, effect,
												   method, params, special,
												   responses[i],
												   flag_get2(responses, i))
					end

					-- Blow response killed monster?
					if m_ptr.r_idx == 0 or c_ptr.m_idx == 0 then
						return true
					end
				end
			end -- if flag_exists(t_flags, FLAG_BLOW_RESPONSE) then
			symbiosis.force_take_hit = false

			hook.process(hook.PLAYER_HIT_MON_POST,
						 m_idx, target_monst, SKILL_SYMBIOTIC, blow,
						 params, special)
		else
			message(strcap(symb_name) .. " misses " .. target_name .. ".")
		end
	end -- for i = 1, getn(blows) do

	return true
end -- symbiosis.do_attack_blows()

function symbiosis.do_attack_magic(target_monst)
	return symbiosis.do_spell(target_monst)
end

function symbiosis.do_attack_heal(target_monst)
	return symbiosis.do_spell(target_monst, true)
end

function symbiosis.attack_pos(y, x)
	local m_idx = cave(y, x).m_idx

	if m_idx == 0 then
		return false
	end

	return symbiosis.attack_monst(monster(m_idx))
end

function symbiosis.attack_monst(target_monst)
	if target_monst.r_idx == 0 or target_monst.hp <= 0 then
		return false
	end

	if symbiosis.rebelling then
		message("Symbiote not cooperating?  Strange...")
		return
	end

	local prev_target = symbiosis.attacking
	if prev_target then
		message(color.VIOLET, "symbiosis.attacking not cleared out?")

		if prev_target.r_idx == 0 or prev_target.hp <= 0 then
			message(color.VIOLET, "Previous target is dead.")
			prev_target = nil
		end

		if prev_target ~= target_monst then
			if wizard then
				message(color.VIOLET, "Previous target was " ..
						monster_desc(prev_target, 512 | 256 | 128))
			else
				message(color.VIOLET, "Previous target was different " ..
						"than current target.")
			end
		end
	end

	local symb       = player.inventory[INVEN_SYMBIOTE][1]
	local symb_monst = symb.flags[FLAG_MONSTER_OBJ]
	local symb_name  = symbiosis.get_symbiote_name(symb)

	set_monst_helper(symb_monst)

	symb_monst.ml = true
	symb_monst.fy = player.py
	symb_monst.fx = player.px

	symbiosis.attacking = target_monst

	local func
	if symb_monst.flags[FLAG_SYMB_ATTACK] then
		func =
			get_func_from_flag_or_registry(symb_monst.flags,
										   FLAG_SYMB_ATTACK)
	else
		func = function() return false end
	end

	local attacked = false
	if func(symb, target_monst) then
		-- Symbiote did custom attacking against monster.
		attacked = true
	elseif symbiosis.do_attack_heal(target_monst) then
		-- Symbiote refreshed itself off the monster.
		attacked = true
	elseif symbiosis.do_attack_blows(target_monst) then
		-- Symbiote used blows on monster.
		attacked = true
	elseif symbiosis.do_attack_magic(target_monst) then
		-- No blows available, so a spell was used.
		attacked = true
	end

	symbiosis.force_take_hit = false
	symbiosis.attacking      = prev_target

	unset_monst_helper()

	if symb_monst.r_idx == 0 or symb_monst.hp <= 0 then
		message(strcap(symb_name) .. " died attacking " ..
				target_name .. ".")
		symbiosis.kill_symbiote()
	end

	return attacked
end -- symbiosis.attack_monst()

hook(hook.PLAYER_HIT_MON_POST,
function(m_idx, m_ptr, style, how1, how2, how3)
	if not player.inventory[INVEN_SYMBIOTE][1] then
		return
	end

	-- Don't go into endless recursion
	if style == SKILL_SYMBIOTIC then
		return
	end

	-- Don't do anything if we didn't touch the monster
	if style == SKILL_ARCHERY or (style == SKILL_POSSESSION and
								  not how3.touched)
	then
		return
	end

	symbiosis.attack_monst(m_ptr)
	player.redraw[FLAG_PR_MH] = true

	-- Stop processing hook if monster died.
	if m_ptr.r_idx == 0 then
		return true
	end
end) -- hook(hook.PLAYER_HIT_MON_POST)

---------------------------------------------------------------------------

--
-- Misceullaneous hooks
--

hook(hook.BODY_PARTS,
function()
	if get_skill(SKILL_SYMBIOTIC) >= 1 and
		not player.body_monster.flags[FLAG_NO_SYMBIOSIS]
	then
		player.add_body_part(INVEN_SYMBIOTE, 1)
	end
end) -- hook(hook.BODY_PARTS)

hook(hook.CALC_BONUS_BEGIN,
function()
	local symb = player.inventory[INVEN_SYMBIOTE][1]

	if symb then
		symbiosis.calc_obj_bonus(symb)
	end
end)

hook(hook.CALC_BONUS_END,
function()
	local symb = player.inventory[INVEN_SYMBIOTE][1]

	if symb and not symbiosis.can_symbiotize("maintain") then
		player.redraw[FLAG_PR_MH] = true
		symbiosis.force_drop()
	end
end) -- hook(hook.CALC_BONUS_END)

--
-- Let player take damage done by rebelling symbiote
--
hook(hook.PROJECT_P_PRE_LUA,
function(who, r, y, x, dmg, typ, a_rad, flags, dir)
	if who == 0 and symbiosis.rebelling then
		return true, dmg, typ, (flags | PROJECT_CASTER)
	end
end)

add_mkey
{
	mkey = "Symbiosis",
	type = "skill",
	hide = function()
		return not symbiosis.can_symbiotize()
	end
	fct	= symbiosis.use_symbiosis_spells
}
