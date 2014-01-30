-- Functions to help with spells, do not touch

declare_globals {
	"SPELL_NOTHING",
	"HAVE_ARTIFACT",
	"HAVE_EGO",
	"HAVE_OBJECT",
	"SCHOOL_MONSTER_ABILITY",
	"SCHOOL_MONSTER_SPELL",
	"spell",
	"school",
	"load_spells",
	"activate_activation",
	"activate_stick",
	"add_monster_spell",
	"add_damage_type",
	"adjust_power",
	"grab_object_spells",
	"add_object_spell",
	"remove_object_spell",
	"setup_object_spells",
	"make_book",
	"book_spells_num",
	"spell_is_cooldown",
	"spell_copy_to",
	"spell_copy_to_tester",
	"can_spell_random",
	"get_monster_spell_msg",
	"cast_monster_spell",
	"cast_school_spell",
	"check_school_spell_failure",
	"check_affect",
	"exec_lasting_spell",
	"find_dam_type",
	"find_spell",
	"get_activation_desc",
	"get_activation_timeout",
	"get_caster_damage",
	"get_dam_type_info",
	"get_dam_color_desc",
	"get_god_level",
	"lua_get_level",
	"get_level_device",
	"get_level_school",
	"get_mana",
	"get_power",
	"get_power_name",
	"get_random_spell",
	"get_random_stick",
	"get_spell_caster_info",
	"get_spell_caster_level",
	"get_spell_stat",
	"get_stick_base_level",
	"get_stick_charges",
	"get_stick_max_level",
	"have_object",
	"is_obvious",
	"is_ok_spell",
	"print_book",
	"print_device_desc",
	"print_spell_desc",
	"print_spell_desc_spell_x",
	"set_auto_cast",
	"set_get_aim_dir",
	"set_get_level",
	"set_get_rep_dir",
	"set_dam_type",
	"set_spell_caster",
	"set_current_spell_info",
	"set_tgt_pt",
	"set_monst_helper",
	"spell_chance",
	"spell_in_book",
	"spell_from_book_by_name",
	"spell_x",
	"unset_current_spell_info",
	"unset_auto_cast",
	"unset_get_aim_dir",
	"unset_get_level",
	"unset_get_rep_dir",
	"unset_spell_caster",
	"unset_tgt_pt",
	"unset_monst_helper",
	"execute_school_spell",
	"dam",
	"do_monster_saving_throw"
}

declare_globals {
	"__schools",
	"__schools_num",
	"__spell_desc",
	"__spell_info",
	"__spell_school",
	"__spell_spell",
	"__spells_by_name",
	"__tmp_spells",
	"__tmp_spells_num",
	"add_school",
	"add_spell",
	"get_level_stick_skill",
	"get_level_max_stick",
	"get_level_use_stick",
	"old_get_aim_dir",
	"old_get_level",
	"old_get_rep_dir",
	"old_spell_caster",
	"old_current_spell_info",
	"old_tgt_pt",
	"old_tgt_pt_prompt",
	"old_target_who",
	"old_target_col",
	"old_target_row",
	"school_book",
	"get_level",
	"get_cast_level",
	"get_cast_range",
	"current_spell_info"
}

-- Wrapper around tome_dofile for easy spells loading
function load_spells(name)
	tome_dofile_anywhere(TENGINE_DIR_DATA, "magic/"..name)
end

-- Returned by spells when they didnt activate at all
SPELL_NOTHING = -1

current_spell_info = {}

__schools = {}
__schools_num = 0

__tmp_spells = {}
__tmp_spells_num = 0

__spells_by_name = {}

function add_school(s)
	assert(s.name, "No school name!")
	assert(s.skill, "No school skill!")

	-- Need hooks?
	if s.hooks then
		hook(s.hooks)
	end

	__schools[__schools_num] = s

	__schools_num = __schools_num + 1
	return (__schools_num - 1)
end

-- Finish up school(resolve the skill names)
hook(hook.INFO_DATA_LOADED,
	function()
		for index, sch in __schools do
			sch.skill = getglobal(sch.skill)
			if sch.gods then
				for god, data in sch.gods do
					data.skill = getglobal(data.skill)
				end
			end
		end
	end
)

function add_spell(s)
	assert(s.name, "No spell name!")
	assert(s.school, "No spell school!")
	assert(s.level, "No spell level!")
	assert(s.mana or s.custom_mana, "No spell mana!")
	if not s.mana then s.mana = 0 end
	if not s.mana_max then s.mana_max = s.mana end
	assert(s.fail, "No spell failure rate!")
	assert(s.spell, "No spell function!")
	if not s.info then s.info = function() return "" end end
	assert(s.desc, "No spell desc!")
	if not s.antimagic then s.antimagic = true end
	if not s.anti_magic then s.anti_magic = true end
	if s.lasting then
		assert(type(s.lasting) == "function", "Spell lasting is not function")
	end
	s.random = s.random or "magic"

	if s.stick then
		local k, e
		for k, e in s.stick do
			if type(k) == "table" then
				assert(e.base_level, "Arg no stick base level")
				assert(e.max_level, "Arg no stick max level")
			end
		end
	end

	if type(s.school) == "number" then s.school = {s.school} end
	s.skill_level = s.level
	s.index = __tmp_spells_num

	__spell_school[s.index] = s.school
	__spell_spell[s.index] = s.spell
	__spell_info[s.index] = s.info
	__spell_desc[s.index] = s.desc

	__tmp_spells[s.index] = s
	__spells_by_name[strlower(s.name)] = s.index

	__tmp_spells_num = __tmp_spells_num + 1
	return s.index
end

function spell(i)
	return __tmp_spells[i]
end

function school(i)
	return __schools[i]
end

-- Creates the school books array
__spell_spell = {}
__spell_info = {}
__spell_desc = {}
__spell_school = {}
school_book = {}

-- Find a spell by name
function find_spell(name)
	local i

	-- undersocres are like spaces
	name = gsub(name, "(_)", " ")
	name = strlower(name)

	if __spells_by_name[name] then return __spells_by_name[name] end
	return -1
end

-- Find if the school is under the influence of a god, returns nil or the level
function get_god_level(sch)
	if __schools[sch].gods[player.pgod] then
		return (get_skill_raw(__schools[sch].gods[player.pgod].skill) * __schools[sch].gods[player.pgod].mul) / __schools[sch].gods[player.pgod].div
	else
		return nil
	end
end

function lua_get_level(s, lvl, max, min, bonus)
	local tmp

	tmp = lvl - ((spell(s).skill_level - 1) * (skills.STEP_VALUE / 10))

	if (tmp >= (skills.STEP_VALUE / 10)) then
		-- We require at least one spell level
		tmp = tmp + bonus
	end

	tmp = (tmp * (max * (skills.STEP_VALUE / 10)) / (skills.MAX_VALUE / 10))

	if (tmp < 0) then
		-- Shift all negative values, so they map to appropriate integer
		tmp = tmp - (skills.STEP_VALUE / 10 - 1)
	end

	-- Now, we can safely divide
	lvl = tmp / (skills.STEP_VALUE / 10)

	if (lvl < min) then
		lvl = min
	end

	return lvl
end


-- Change this fct if I want to switch to learnable spells
function get_level_school(s, max, min)
	local lvl, sch, index, num, bonus
	local allow_spell_power = true

	lvl = 0
	num = 0
	bonus = 0

	-- No max specified ? assume 50
	if not max then
		max = (skills.MAX_VALUE / skills.STEP_VALUE)
	end
	if not min then
		min = 1
	end

	-- Do we pass tests?
	if __tmp_spells[s].depend then
		if __tmp_spells[s].depend() ~= true then
			return min, "n/a"
		end
	end

	for index, sch in __spell_school[s] do
		local r, p, m, ok = 0, 0, 0

		-- Does it require we worship a specific god?
		if __schools[sch].god then
			if __schools[sch].god ~= player.pgod then
				if min then return min, "n/a"
				else return 1, "n/a" end
			end
		end

		-- Take the basic skill value
		r = get_skill_raw(school(sch).skill)

		-- Do we pass tests?
		if __schools[sch].depend then
			if __schools[sch].depend() ~= true then
				return min, "n/a"
			end
		end

		-- Are we affected by spell power ?
		-- All teh schools must allow it for it to work
		if not __schools[sch].spell_power then
			allow_spell_power = nil
		end

		-- Are we under a god effect ?
		if __schools[sch].gods then
			p = get_god_level(sch)
			if not p then p = 0 end
		end

		-- Are we under any other effect?
		if player.spell_get_extra_level then
                	local ret, erase = player.spell_get_extra_level(s, sch)
                        m = ret or 0
                        if erase then r, p = 0, 0 end
		end

		-- Find the higher
		ok = r
		if ok < p then ok = p end
		if ok < m then ok = m end

		-- Do we need to add a special bonus ?
		if __schools[sch].bonus_level then
			bonus = bonus + (__schools[sch].bonus_level() * (skills.STEP_VALUE / 10))
		end

		-- All schools must be non zero to be able to use it
		if ok == 0 then return min, "n/a" end

		-- Apply it
		lvl = lvl + ok
		num = num + 1
	end

	-- Add the Spellpower skill as a bonus
	if allow_spell_power and player.spell_get_power_bonus then
		bonus = bonus + (player.spell_get_power_bonus(s, sch) * (skills.STEP_VALUE / 10))
	end

	-- Add bonus from objects
	bonus = bonus + (player.to_s * (skills.STEP_VALUE / 10))

	-- / 10 because otherwise we can overflow a s32b and we can use a u32b because the value can be negative
	-- The loss of information should be negligible since 1 skill = 1000 internally
	lvl = (lvl / num) / 10
	lvl = lua_get_level(s, lvl, max, min, bonus)

	return lvl, nil
end

-- This is the function to use when casting through a stick
function get_level_device(s, max, min)
	local lvl

	-- No max specified ? assume 50
	if not max then
		max = skills.MAX_VALUE / skills.STEP_VALUE
	end

	lvl = get_skill(get_level_stick_skill) * skills.STEP_VALUE
	lvl = lvl + (get_level_use_stick * skills.STEP_VALUE)

	-- Sticks are limited
	if lvl - ((spell(s).skill_level + 1) * skills.STEP_VALUE) > get_level_max_stick * skills.STEP_VALUE then
		lvl = (get_level_max_stick + spell(s).skill_level - 1) * skills.STEP_VALUE
	end

	-- / 10 because otherwise we can overflow a s32b and we can use a u32b because the value can be negative
	-- The loss of information should be negligible since 1 skill = 1000 internally
	lvl = lvl / 10
	if not min then
		lvl = lua_get_level(s, lvl, max, 1, 0)
	else
		lvl = lua_get_level(s, lvl, max, min, 0)
	end

	return lvl
end

-- The real get_level, works for schooled magic and for innate powers
get_level_use_stick = -1
get_level_max_stick = -1
get_level_stick_skill = -1
function get_level(s, max, min)
	if type(s) == "number" then
		-- Ahah shall we use Magic device instead ?
		if get_level_use_stick > -1 then
			return get_level_device(s, max, min)
		else
			local lvl, na = get_level_school(s, max, min)
			return lvl
		end
	else
		return get_level_power(s, max, min)
	end
end

-- A wrapper arround get_level that uses the current spell
function get_cast_level(max, min)
	return get_level(current_spell_info.index, max, min)
end

-- A wrapper arround get_level that uses the current spell
function get_cast_range(s)
	if __tmp_spells[s or current_spell_info.index].range then
		return __tmp_spells[s or current_spell_info.index].range()
	else
		return 16
	end
end

-- Can we cast the spell ?
function is_ok_spell(s, obj)
	if get_level(s, (skills.MAX_VALUE / skills.STEP_VALUE), 0) == 0 then return nil end
	if __tmp_spells[s].flag and get_flag(obj, __tmp_spells[s].flag[1]) < __tmp_spells[s].flag[2] then return nil end
	return 1
end

-- Get the amount of mana(or power) needed
function get_mana(s)
	if __tmp_spells[s].custom_mana then return __tmp_spells[s].custom_mana() end
	return spell(s).mana + get_level(s, spell(s).mana_max - spell(s).mana, 0)
end

-- Return the amount of power(mana, piety, whatever) for the spell
function get_power(s)
	if __tmp_spells[s].custom_power then
		return __tmp_spells[s].custom_power("get")
	elseif check_affect(s, "piety", FALSE) then
		return player.grace()
	else
		return player.csp()
	end
end

-- Return the amount of power(mana, piety, whatever) for the spell
function get_power_name(s)
	if __tmp_spells[s].custom_power then
		return __tmp_spells[s].custom_power("name")
	elseif check_affect(s, "piety", FALSE) then
		return "piety"
	else
		return "mana"
	end
end

-- Changes the amount of power(mana, piety, whatever) for the spell
function adjust_power(s, x)
	if (get_mana(s) > get_power(s)) then
		local s_ptr = spell(s)
		local sch   = school(s_ptr.school[1])

		if s_ptr.did_insuff_power then
			s_ptr.did_insuff_power(s, x)
		end
		if sch.did_insuff_power then
			sch.did_insuff_power(s, x)
		end
	end

	if __tmp_spells[s].custom_power then
		return __tmp_spells[s].custom_power("set", x)
	elseif check_affect(s, "piety", FALSE) then
		god.inc_piety(god.ALL, x)
	else
		increase_mana(x)
	end
end

-- Give an object, return spells contained
function grab_object_spells(obj)
	local spells = {}

	if has_flag(obj, FLAG_SPELL_IDX) then
		local flags = flag_get_flags(obj.flags, FLAG_SPELL_IDX)

		for i = 1, flags.size do
			if (flags.node[i].flags & FLAG_FLAG_USED) ~= 0 then
				local idx = flags.node[i].key

				spells[flag_get(flags, idx)] = idx
			end
		end
	end

	return spells
end

-- Add a spell to an object
function add_object_spell(obj, spell)
	local flags
	if has_flag(obj, FLAG_SPELL_IDX) then
		flags = flag_get_flags(obj.flags, FLAG_SPELL_IDX)
	else
		flags = flag_new(2)
		flag_set_full(obj.flags, FLAG_SPELL_IDX, FLAG_FLAG_FLAGS, 0, 0, flags)
	end

	flag_set(flags, spell, flag_used(flags) + 1)
end

-- Remove a spell to from object
function remove_object_spell(obj, spell)
	if has_flag(obj, FLAG_SPELL_IDX) then
		local flags = flag_get_flags(obj.flags, FLAG_SPELL_IDX)

		if flag_exists(flags, spell) then
			-- get it's index
			local idx = flag_get(flags, spell)

			foreach_flags(flags, function(flags, spl)
				if flag_get(flags, spl) > %idx then
					-- if this spell was after the removed spell we take it down one place
					flag_set(flags, spl, flag_get(flags, spl) - 1)
				end
			end)

			-- Now delete it
			flag_remove(flags, spell)
		end
	end
end

-- Setup a spell
function setup_object_spells(obj, book)
	for _, s in school_book[book] do
		add_object_spell(obj, s)
	end
end

--- @fn
-- Makes a temporary "book object" to select a list of spells
-- this object must be deleted afterwards and should never be given
-- to a monster or player
function make_book(...)
	local book = create_object(0, 0)

	for i = 1, getn(arg) do
		add_object_spell(book, arg[i])
	end
	return book
end

-- Print the book and the spells
function print_book(obj)
	local x, y, index, sch, size, s

	x = 0
	y = 2
	size = 0

	-- grab spells
	local spells = grab_object_spells(obj)

	-- Parse all spells
	for i = 1, getn(spells) do
		local s = spells[i]
		local c = color.LIGHT_DARK
		local lvl, na = get_level_school(s, (skills.MAX_VALUE / skills.STEP_VALUE), -(skills.MAX_VALUE / skills.STEP_VALUE))
		local xx, sch_str

		if is_ok_spell(s, obj) then
			if get_mana(s) > get_power(s) then c = color.ORANGE
			else c = color.LIGHT_GREEN end
		end

		xx = nil
		sch_str = ""
		for index, sch in __spell_school[s] do
			if xx then
				sch_str = sch_str.."/"..school(sch).name
			else
				xx = 1
				sch_str = sch_str..school(sch).name
			end
		end

		-- Setup the spell infos
		set_current_spell_info{index = s, faction = FACTION_PLAYER}

		if na then
			term.blank_print(c, format("%c) %-20s%-16s   %3s %4s %3d%s %s", size + strbyte("a"), spell(s).name, sch_str, na, get_mana(s), spell_chance(s), "%", __spell_info[s]()), y, x)
		else
			term.blank_print(c, format("%c) %-20s%-16s   %3d %4s %3d%s %s", size + strbyte("a"), spell(s).name, sch_str, lvl, get_mana(s), spell_chance(s), "%", __spell_info[s]()), y, x)
		end

		-- Unsetup
		unset_current_spell_info()

		y = y + 1
		size = size + 1
	end
	term.blank_print(format("   %-20s%-16s Level Cost Fail Info", "Name", "School"), 1, x)
	return y
end

-- Output the describtion when it is used as a spell
function print_spell_desc(s, y)
	local index, desc, x

	x = 0

	if type(__spell_desc[s]) == "string" then term.blank_print(color.LIGHT_BLUE, __spell_desc[s], y, x)
	else
		for index, desc in __spell_desc[s] do
			term.blank_print(color.LIGHT_BLUE, desc, y, x)
			y = y + 1
		end
	end
	if check_affect(s, "piety", FALSE) then
		term.blank_print(color.LIGHT_WHITE, "It uses piety to cast.", y, x)
		y = y + 1
	end
	if not check_affect(s, "blind") then
		term.blank_print(color.ORANGE, "It is castable even while blinded.", y, x)
		y = y + 1
	end
	if not check_affect(s, "confusion") then
		term.blank_print(color.ORANGE, "It is castable even while confused.", y, x)
		y = y + 1
	end
	if __tmp_spells[s].cooldown then
		term.blank_print(color.VIOLET, "It may only be casted every "..__tmp_spells[s].cooldown.." turns.", y, x)
		y = y + 1
		if spell_is_cooldown(s) then
			term.blank_print(color.VIOLET, "It is currently cooling down and will be ready in "..((player.spells_cooldown[s] - turn) / 10).." turns.", y, x)
			y = y + 1
		end
	end
end

-- Ouput spell desc of the n'th spell in obj at line y
function print_spell_desc_spell_x(obj, n, y)
	print_spell_desc(spell_x(obj, n), y)
end

-- Output the desc when sued as a device
function print_device_desc(s)
	local index, desc

	if type(__spell_desc[s]) == "string" then term.text_out(__spell_desc[s])
	else
		for index, desc in __spell_desc[s] do
			term.text_out("\n" .. desc)
		end
	end
end

-- Number of spells in an object
function book_spells_num(obj)
	-- Parse all spells
	return getn(grab_object_spells(obj))
end

-- Return the x'th spell in the object
function spell_x(obj, s)
	local spells = grab_object_spells(obj)

	if getn(spells) >= 1 then
		return spells[s + 1]
	else
		return -1
	end
end

function spell_in_book(obj, spell)
	local spells = grab_object_spells(obj)

	for i = 1, getn(spells) do
		if spells[i] == spell then
			return true
		end
	end

	return false
end

-- Return named spell from object, or -1 if object doesn't contain the
-- named spell.
function spell_from_book_by_name(obj, name)
	local spells = grab_object_spells(obj)
	local idx    = __spells_by_name[strlower(clean_whitespace(name))]

	for i = 1, getn(spells) do
		if spells[i] == idx then
			return idx
		end
	end

	return -1
end

-- Returns spell chance of failure for spell
function spell_chance(s)
	local chance, s_ptr

	s_ptr   = spell(s)

	-- Extract the base spell failure rate
	if get_level_use_stick > -1 then
		chance = lua_spell_device_chance(get_level_stick_skill, s_ptr.fail, get_level(s, (skills.MAX_VALUE / skills.STEP_VALUE)), s_ptr.skill_level)
	else
		local fail_adj, fail_min = player.spell_chance_get_fails(get_spell_stat(s))
		chance = lua_spell_chance(s_ptr.fail, get_level(s, (skills.MAX_VALUE / skills.STEP_VALUE)), s_ptr.skill_level, get_mana(s), get_power(s), fail_adj, fail_min)

		-- Let spell and schools change spell chance and failure
		local failure = player.intrinsic(FLAG_SPELL_FAILURE)
		if s_ptr.change_spell_chance then
			chance, failure = s_ptr.change_spell_chance(s, chance, failure)
		end
		for i = 1, getn(s_ptr.school) do
			local sch_ptr = school(s_ptr.school[i])
			if sch_ptr.change_spell_chance then
				chance, failure =
					sch_ptr.change_spell_chance(s, chance, failure)
			end
		end -- for i = 1, getn(s_ptr.school) do

		chance = chance + failure
		if chance > 95 then chance = 5
		elseif chance < 0 then chance = 0 end
	end

	-- Return the chance
	return chance
end

-- Some spells can have a cooldown
add_loadsave("player.spells_cooldown", {})
hook(hook.BIRTH, function() player.spells_cooldown = {} end)
player.spells_cooldown = {}
function spell_is_cooldown(s)
	if not player.spells_cooldown[s] then return false end
	if turn > player.spells_cooldown[s] then return false end
	return true
end

function check_affect(s, name, default)
	local s_ptr = __tmp_spells[s]
	local a

	if type(s_ptr[name]) == "number" then
		a = s_ptr[name]
	else
		a = default
	end
	if a == FALSE then
		return nil
	else
		return true
	end
end

-- Returns the stat to use for the spell, INT by default
function get_spell_stat(s)
	if not __tmp_spells[s].stat then return A_INT
	elseif type(__tmp_spells[s].stat) == "function" then return __tmp_spells[s].stat()
	else return __tmp_spells[s].stat end
end

function check_school_spell_failure(spell, rate)
	if rng.percent(rate) then
		local index, sch

		-- added because this is *extremely* important --pelpel
		if (flush_failure) then message.flush() end

		message("You failed to get the spell off!")
		for index, sch in __spell_school[spell] do
			if __schools[sch].fail then
				__schools[sch].fail(rate)
			end
		end

		return true
	end
	return false
end

hook.new_hook_type("PLAYER_SCHOOL_SPELL_CAST_PRE")
hook.new_hook_type("PLAYER_SCHOOL_SPELL_CAST_POST")
function cast_school_spell(s, s_ptr, no_cost, check_fct)
	local spend_sp = FALSE

	local hook_ret, cancel, force_no_cost = hook.process(hook.PLAYER_SCHOOL_SPELL_CAST_PRE, s, no_cost)
	if hook_ret then
		if cancel then return end
		no_cost = force_no_cost
	end

	-- No magic
	if __tmp_spells[s].antimagic == true and (player.antimagic > 0) then
		message("Your anti-magic field disrupts any magic attempts.")
		return
	end

	-- No magic
	if __tmp_spells[s].anti_magic == true and player.anti_magic then
		message("Your anti-magic shell disrupts any magic attempts.")
		return
	end

	-- Is the spell cooling down ?
	if spell_is_cooldown(s) then
		message("This spell is not yet ready.")
		return
	end

	-- if it costs something then some condition must be met
	if not no_cost then
		if not check_fct then
		 	-- Require lite
			if (check_affect(s, "blind")) and (player.has_intrinsic(FLAG_BLIND) or (no_lite())) then
				message("You cannot see!")
				return
			end

			-- Not when confused
			if (check_affect(s, "confusion")) and player.has_intrinsic(FLAG_CONFUSED) then
				message("You are too confused!")
				return
			end
		elseif not check_fct(s, s_ptr) then
			return
		end

		-- Enough mana
		if (get_mana(s) > get_power(s)) then
			local sch = school(spell(s).school[1])

			if (sch.can_insuf_power and sch.can_insuf_power(s)) or
				(s_ptr.can_insuf_power and can_insuf_power(s))
			then
				local msg = "You do not have enough " .. get_power_name(s) ..
					", do you want to try anyway?"
				if s_ptr.insuf_power_prompt then
					msg = s_ptr.insuf_power_prompt(s)
				elseif sch.insuf_power_prompt then
					msg = sch.insuf_power_prompt(s)
				end

				if (not get_check(msg)) then return end
			else
				local msg = "You do not have enough "
					.. get_power_name(s) .. "."
				if s_ptr.insuf_power_msg then
					msg = s_ptr.insuf_power_msg(s)
				elseif sch.insuf_power_msg then
					msg = sch.insuf_power_msg(s)
				end

				message(msg)
				return
			end
		end

		-- Invoke the spell effect
		if not check_school_spell_failure(s, spell_chance(s)) then
			if (execute_school_spell(s, "player school cast", FACTION_PLAYER) ~= SPELL_NOTHING) then
				spend_sp = true
			end
		else
			spend_sp = true
		end
	else
		execute_school_spell(s, "player school cast no_cost", FACTION_PLAYER)
	end

	local hook_ret, force_spend_sp = hook.process(hook.PLAYER_SCHOOL_SPELL_CAST_POST, s, spend_sp)
	if hook_ret then
		spend_sp = force_spend_sp
	end

	if spend_sp == true then
		local cost  = get_mana(s)
		local power = get_power(s)
		if (cost > power) then
			local sch = school(spell(s).school[1])


			if s_ptr.insuf_power_effect then
				cost = s_ptr.insuf_power_effect(s, cost, power) or cost
			elseif sch.insuf_power_effect then
				cost = sch.insuf_power_effect(s, cost, power) or cost
			end
		end

		-- Reduce mana
		adjust_power(s, -cost)

		-- Take a turn
		energy_use = get_player_energy(SPEED_CAST)

		-- Set cooldown
		if __tmp_spells[s].cooldown then
			player.spells_cooldown[s] = turn + (__tmp_spells[s].cooldown * 10)
		end
	end

	flag_set(player.redraw, FLAG_PR_MANA,1)
	player.window[FLAG_PW_PLAYER] = true
end

new_flag("PLAYER_SCHOOL_SPELL_CAST_PRE", true)
hook(hook.PLAYER_SCHOOL_SPELL_CAST_PRE,
function(s, no_cost)
	return item_hooks.process_one_pack(FLAG_PLAYER_SCHOOL_SPELL_CAST_PRE, s, no_cost)
end)

new_flag("PLAYER_SCHOOL_SPELL_CAST_POST", true)
hook(hook.PLAYER_SCHOOL_SPELL_CAST_POST,
function(s, spend_sp)
	return item_hooks.process_all_pack(FLAG_PLAYER_SCHOOL_SPELL_CAST_POST, s, spend_sp)
end)

-- Helper functions
HAVE_ARTIFACT = 0
HAVE_OBJECT = 1
HAVE_EGO = 2
function have_object(mode, type, find, find2)
	local o, i, min, max

	max = INVEN_INVEN
	min = INVEN_INVEN
	if (mode & USE_EQUIP) == USE_EQUIP then
		min = INVEN_PACK
		max = INVEN_TOTAL
	end
	if (mode & USE_INVEN) == USE_INVEN then
		min = INVEN_INVEN
		if max == 0 then max = INVEN_PACK end
	end

	local ret = {}

	for_inventory(player, min, max, function(o, i, j, slot)
		if %type == HAVE_ARTIFACT then
			if %find == o.artifact_id then %ret.inven=i %ret.item=j return end
		end
		if %type == HAVE_OBJECT then
			if %find2 == nil then
				if %find == o.k_idx then %ret.inven=i %ret.item=j return end
			else
				if (%find == o.tval) and (%find2 == o.sval) then %ret.inven=i %ret.item=j return end
			end
		end
		if %type == HAVE_EGO then
			for z = 1, MAX_EGO_PER_OBJ do
				if %find == o.ego_id[z] then %ret.inven=i %ret.item=j return end
			end
		end
	end)

	return ret.inven, ret.item
end

-- Can the spell be randomly found(in random books)
function can_spell_random(i)
	return __tmp_spells[i].random or "magic"
end

-- Find a random spell
function get_random_spell(typ, level)
	local spl, tries

	tries = 1000
	while tries > 0 do
		tries = tries - 1
		spl = rng.number(__tmp_spells_num)
		if (can_spell_random(spl) == typ) and (rng.number(spell(spl).skill_level * 3) < level) then
			break
		end
	end
	if tries > 0 then
		return spl
	else
		return -1
	end
end

spell_copy_to_tester = function(obj) return obj.flags[FLAG_SPELL_CONTAIN] and flag_is_known(obj.flags, FLAG_SPELL_CONTAIN) and not obj.flags[FLAG_SPELL_IDX] end

--- @fn
-- @brief Copy a spell from an object to an other
function spell_copy_to(item)
	if item and not spell_copy_to_tester(get_object(item), item) then return use_object.CANNOT_USE end

	local spell = get_school_spell("copy", "is_ok_spell", nil)
	if spell == -1 then return false end

	-- Spells that cannot be randomly created cannot be copied
	if tag(can_spell_random(spell)) ~= TAG_STRING then
		message("This spell cannot be copied.")
		return false
	end

	-- Get an item
	if not item then
		local ret
		ret, item = get_item("Copy spell into what? ", "You have nothing to copy a spell to.", USE_EQUIP | USE_INVEN | USE_FLOOR,
		   spell_copy_to_tester
		)
		if not ret then return end
	end

	-- Get the item
	local obj = get_object(item)

	message("You copy the spell!")
	add_object_spell(obj, spell)
	item_describe(item)
	return item, spell
end

-- Execute a lasting spell
function exec_lasting_spell(spl)
	assert(__tmp_spells[spl].lasting, "No lasting effect for spell "..__tmp_spells[spl].name.." but called as such")
	return __tmp_spells[spl].lasting()
end

-- Helper function for spell effect to know if they are or not obvious
function is_obvious(effect, old)
	-- True takes precendence over any other values
	if effect == true then return true end
	if old == true then return true end

	-- if both are the same it's easy too
	if effect == old then return effect end

	-- Now the less ovious cases
	if effect == nil and old == SPELL_NOTHING then return nil end
	if effect == SPELL_NOTHING and old == nil then return nil end

	-- Shouldnt happen
	error("is_obvious() should not have terminated: " .. tostring(effect) .. tostring(old))
end

-------------------------Sticks-------------------------

-- Fire off the spell
function activate_stick(spl)
	local ret = execute_school_spell(spl, "player use stick", FACTION_PLAYER)
	local charge, obvious
	if ret == SPELL_NOTHING then
		charge = false
		obvious = false
	else
		charge = true
		obvious = ret
	end
	return charge, obvious
end

----------------------------------- Wand, Staves, Rods specific functions ----------------------------

-- Get a spell for a given stick(wand, orb, rod)
function get_random_stick(stick, level)
	local spl, tries

	if level < 1 then level = 1 end
	tries = 1000
	while tries > 0 do
		tries = tries - 1
		spl = rng.number(__tmp_spells_num)
		if __tmp_spells[spl].stick and (type(__tmp_spells[spl].stick[stick]) == "table") then
			if (rng.number(spell(spl).skill_level * 3) < level) and (rng.percent(100 - __tmp_spells[spl].stick[stick].rarity)) then
				break
			end
		end
	end
	if tries > 0 then
		return spl
	else
		return -1
	end
end

-- Get a random base level
function get_stick_base_level(stick, level, spl)
	-- Paranoia
	if spl < 0 or spl >= __tmp_spells_num or not __tmp_spells[spl].stick[stick] then return 0 end

	local min, max = __tmp_spells[spl].stick[stick].base_level[1], __tmp_spells[spl].stick[stick].base_level[2]
	local range = max - min

	-- Ok the basic idea is to have a max possible level of half the dungeon level
	if range * 2 > level then range = level / 2 end

	-- Randomize a bit
	range = m_bonus(range, dun_level)

	-- And get the result
	return min + range
end

-- Get a random max level
function get_stick_max_level(stick, level, spl)
	-- Paranoia
	if spl < 0 or spl >= __tmp_spells_num or not __tmp_spells[spl].stick[stick] then return 0 end

	local min, max = __tmp_spells[spl].stick[stick].max_level[1], __tmp_spells[spl].stick[stick].max_level[2]
	local range = max - min

	-- Ok the basic idea is to have a max possible level of half the dungeon level
	if range * 2 > level then range = level / 2 end

	-- Randomize a bit
	range = m_bonus(range, dun_level)

	-- And get the result
	return min + range
end

-- Get the number of desired charges
function get_stick_charges(spl)
	return __tmp_spells[spl].stick.charge[1] + rng(__tmp_spells[spl].stick.charge[2])
end

-- Get activation desc
function get_activation_desc(spl)
	local turns
	if type(__tmp_spells[spl].activate) == 'number' then
		turns = __tmp_spells[spl].activate
	else
		turns = __tmp_spells[spl].activate[1] .. '+d' .. __tmp_spells[spl].activate[2]
	end
	return __tmp_spells[spl].desc[1] .. ' every ' .. turns .. ' turns'
end

-- Compute the timeout of an activation
function get_activation_timeout(spl)
	if type(__tmp_spells[spl].activate) == 'number' then
		return __tmp_spells[spl].activate
	else
		return __tmp_spells[spl].activate[1] + rng(__tmp_spells[spl].activate[2])
	end
end

-- Fire off the spell
function activate_activation(spl, item)
	execute_school_spell(spl, "player use object", FACTION_PLAYER, item)
end


------- Add new damage type ----------
dam = {}
dam.max_type = 1
dam.raw = {}
function add_damage_type(t)
	if not t.index then
		t.index = dam.max_type
		dam.max_type = dam.max_type + 1
	end
	assert(t.color, "No damage type color")
	if not t.monster then t.monster = function() end end
	if not t.angry then t.angry = function() end end
	if not t.object then t.object = function() end end
	if not t.player then t.player = function() end end
	if not t.grid then t.grid = function() end end
	if not t.text_color then t.text_color = "#w" end

	hook
	{
		[hook.DAM_TYPE_COLOR] = function (type, new_gfx)
			local t = %t
			if type == t.index then
				if tag(t.color) == TAG_FUNCTION then
					return true, t.color(new_gfx)
				else
					return true, t.color[new_gfx + 1]
				end
			end
		end,
		[hook.DAM_TYPE_EXEC] = function (action, who, type, dam, rad, y, x, extra)
			local t = %t
			if t.index == type then
				return t[action](who, dam, rad, y, x, extra)
			end
		end,
	}

	if not t.name then t.name = "???" end
	if not t.desc then t.desc = "???" end
	dam.raw[t.index] = t

	-- Should we do something else(like creating resistances)
	if dam.on_damage_creation then dam.on_damage_creation(t) end

	return t.index
end

function find_dam_type(name)
	for _, e in dam.raw do
		if e.name == name then return e.index end
	end
	return -1
end

function get_dam_type_info(index, type)
	return dam.raw[index][type]
end

function get_dam_color_desc(index)
	local info = dam.raw[index]

	if info.color_desc then
		return info.color_desc
	end

	return (info.text_color .. info.desc .. "#w")
end

-- Assign name/descs to basic damage tpe
function set_dam_type(type, name, desc)
	dam.raw[type] = { index=type, name=name, desc=desc }
end
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "dam_types.lua")


-- Utility spell functions

-- Temporarily changes get_aim_dir to not be interactive
old_get_aim_dir = {}
old_target_row  = {}
old_target_col  = {}
function set_get_aim_dir(y, x)
	stack_push(old_get_aim_dir, get_aim_dir)
	stack_push(old_target_col,  target_col)
	stack_push(old_target_row,  target_row)
	function get_aim_dir()
		target_col = %x
		target_row = %y
		return true, 5
	end
end
function unset_get_aim_dir()
	get_aim_dir = stack_pop(old_get_aim_dir)
	target_col  = stack_pop(old_target_col)
	target_row  = stack_pop(old_target_row)
end

-- Temporarily changes get_rep_dir to not be interactive
old_get_rep_dir = {}
function set_get_rep_dir()
	stack_push(old_get_rep_dir, get_rep_dir)
	function get_rep_dir()
		local t = { 1, 2, 3, 6, 9, 8, 7, 4 }
		return true, t[rng(getn(t))]
	end
end
function unset_get_rep_dir()
	get_rep_dir = stack_pop(old_get_rep_dir)
end

-- Temporarily changes tgt_pt to not be interactive
old_tgt_pt        = {}
old_tgt_pt_prompt = {}
old_target_who    = {}
function set_tgt_pt(y, x)
	stack_push(old_tgt_pt,        tgt_pt)
	stack_push(old_tgt_pt_prompt, tgt_pt_prompt)
	stack_push(old_target_who,    target_who)

	if y == player.py and x == player.px then
		target_who = 0
	elseif cave(y, x).m_idx > 0 then
		target_who = cave(y, x).m_idx
	else
		target_who = -1
	end

	function tgt_pt()
		return true, %x, %y
	end
	function tgt_pt_prompt()
		return true, %x, %y
	end
end
function unset_tgt_pt()
	tgt_pt        = stack_pop(old_tgt_pt)
	tgt_pt_prompt = stack_pop(old_tgt_pt_prompt)
	target_who    = stack_pop(old_target_who)
end

-- Temporarily changes the spellcaster
old_spell_caster = {}
function set_spell_caster(sc)
	stack_push(old_spell_caster, spell_caster)
	spell_caster = sc
end
function unset_spell_caster()
	spell_caster = stack_pop(old_spell_caster)
end

-- Temporarily changes the get level function to a fixed level
old_get_level = {}
function set_get_level(level)
	stack_push(old_get_level, get_level)
	get_level = function(s, max, min)
		if not min then min = 1 end
		if not max then max = (skills.MAX_VALUE / skills.STEP_VALUE) end

		if %level < min then return min end

		return (%level * max) / (skills.MAX_VALUE / skills.STEP_VALUE)
	end
end
function unset_get_level()
	get_level = stack_pop(old_get_level)
end

-- Temporarily changes the spell info table to a fixed one
old_current_spell_info = {}
function set_current_spell_info(tbl)
	stack_push(old_current_spell_info, current_spell_info)
	current_spell_info = tbl
end
function unset_current_spell_info()
	current_spell_info = stack_pop(old_current_spell_info)
end

hook.new_hook_type("SPELL_CAST_PRE")
hook.new_hook_type("SPELL_CAST_POST")
hook.new_hook_type("ANTIMAGIC_ABSORB")
function execute_school_spell(s, source, faction, ...)
	set_current_spell_info
	{
		index = s
		source = source
		faction = faction
	}

	local hook_ret = hook.process(hook.SPELL_CAST_PRE, arg)
	if not hook_ret then
		local ret = pack(__spell_spell[s](unpack(arg)))
		hook.process(hook.SPELL_CAST_POST, arg)
		unset_current_spell_info()
		return unpack(ret)
	else
		return { SPELL_NOTHING }
	end
end

new_flag("SPELL_CAST_PRE", true)
hook(hook.SPELL_CAST_PRE,
function(args)
	return item_hooks.process_one_pack(FLAG_SPELL_CAST_PRE, args)
end)

new_flag("SPELL_CAST_POST", true)
hook(hook.SPELL_CAST_POST,
function(args)
	return item_hooks.process_all_pack(FLAG_SPELL_CAST_POST, args)
end)

-- For when the player is casting a monster spell which needs a "helper"
-- monster, like the symbiont/possessed-corpse/riden-monster/etc
constant("__monst_helper", {})
__monst_helper.stack = {}
__monst_helper.rebelling_stack = {}

function set_monst_helper(monst, rebelling)
	stack_push(__monst_helper.stack, __monst_helper.helper)
	stack_push(__monst_helper.rebelling_stack, __monst_helper.rebelling)
	__monst_helper.helper    = monst
	__monst_helper.rebelling = rebelling
end

function unset_monst_helper()
	__monst_helper.helper    = stack_pop(__monst_helper.stack)
	__monst_helper.rebelling = stack_pop(__monst_helper.rebelling_stack)
end

-- Redefine the casting environement
function set_auto_cast(sc, level, y, x)
	set_spell_caster(sc)
	set_get_level(level)
	set_get_aim_dir(y, x)
	set_get_rep_dir(y, x)
	set_tgt_pt(y, x)
end
function unset_auto_cast()
	unset_spell_caster()
	unset_get_level()
	unset_get_aim_dir()
	unset_get_rep_dir()
	unset_tgt_pt()
end

function get_monster_spell_msg(spl, failed, monst, target_monst, blind,
							   unseen_caster, unseen_target)
	local target            = "you"
	local target_possessive = "your"
	local target_reflexive  = "yourself"
	local isare             = "are"
	local source
	local source_possessive
	local source_reflexive

	-- 4 = use indefinite for hidden monsters
	source = monster_desc(monst, 4)

	-- 6 = possessive + use indefinite for hidden monsters
	source_possessive = monster_desc(monst, 6)

	-- 35 = Reflexive, genderized if visible ("himself") or "itself"
	source_reflexive = monster_desc(monst, 35)

	if spl.no_target then
		target            = "NO TARGET (BUG)"
		target_possessive = "NO TARGET (BUG)"
		target_reflexive  = "NO TARGET (BUG)"
	end

	-- Get the monsetr name if we target a monster
	if target_monst then
		isare = "is"

		-- 4 = use indefinite for hidden monsters
		target = monster_desc(target_monst, 4)

		-- 6 = possessiveuse indefinite for hidden monsters
		target_possessive = monster_desc(target_monst, 6)

		-- 35 = Reflexive, genderized if visible ("himself") or "itself"
		target_reflexive = monster_desc(target_monst, 35)
	end

	local prefix = iif(failed, "fail_", "")
	local action = spl[prefix .. "action"]

	local str

	-- If "action" is a table, then try to find a key which matches
	-- the given circumstance.
	if unseen_caster and unseen_target then
		-- If the spell has a message to give to the player even when
		-- s/he can't see either the caster or the target, then
		-- the spell should have ignore_unseen set
	elseif type(action) == "table"  then
		if action.blind_like_unseen then
			blind = false
		end

		local MONST = 1
		local VIS   = 2

		local order = {
			{blind = true, target = MONST, hand = true, head = true},
			{blind = true, target = MONST, head = true},
			{blind = true, target = MONST, hand = true},
			{blind = true, target = MONST},

			{blind = true, hand = true,  head = true},
			{blind = true, hand = false, head = true},
			{blind = true, hand = true,  head = false},
			{blind = true},

	 		{caster_vis = true, target = MONST, hand = true, head = true},
			{caster_vis = true, target = MONST, head = true},
			{caster_vis = true, target = MONST, hand = true},
			{caster_vis = true, target = MONST},

			{caster_vis = true, target = VIS, hand = true,  head = true},
			{caster_vis = true, target = VIS, head = true},
			{caster_vis = true, target = VIS, hand = true},
			{caster_vis = true, target = VIS},

	 		{caster_vis = true, hand = true,  head = true},
			{caster_vis = true, head = true},
			{caster_vis = true, hand = true},
			{caster_vis = true},

			{target = VIS, hand = true,  head = true},
			{target = VIS, head = true},
			{target = VIS, hand = true},
			{target = VIS},

			{target = MONST, hand = true,  head = true},
			{target = MONST, head = true},
			{target = MONST, hand = true},
			{target = MONST},

			{hand = true,  head = true},
			{head = true},
			{hand = true},
			{default = true},
		}

		-- Search through table for message, going from most specific
		-- ("unseen_nohand_nohead") to least specific ("default"),
		-- using the first match found.
		for i = 1, getn(order) do
			local has_hands, has_head = false, false

			local parts = {}
			local skip  = false

			if order[i].blind then
				if blind then
					tinsert(parts, "blind")
				else
					skip = true
				end
			end

			if order[i].caster_vis then
				if unseen_caster then
					tinsert(parts, "casterunseen")
				else
					tinsert(parts, "casterseen")
				end
			end

			if order[i].target == MONST then
				if target_monst then
					tinsert(parts, "target")
				else
					tinsert(parts, "player")
				end
			elseif order[i].target == VIS then
				if unseen_target then
					tinsert(parts, "targetunseen")
				else
					tinsert(parts, "targetseen")
				end
			end

			if order[i].hand then
				if has_hands then
					tinsert(parts, "hand")
				else
					tinsert(parts, "nohand")
				end
			end

			if order[i].head then
				if has_head then
					tinsert(parts, "head")
				else
					tinsert(parts, "nohead")
				end
			end

			local use_msg
			if getn(parts) == 0 and order[i].default then
				use_msg = "default"
			else
				use_msg = strjoin(parts, "_")
			end

			if action[use_msg] and not skip then
				if action[use_msg] ~= "" then
					str = action[use_msg]
				else
					return ""
				end
			end
		end
	else -- if type(action) == "table" then
		str = action
	end

	local monst_message = nil

	-- Is a custom message, use it
	if str then
		str = gsub(str, "(@Source@)", strcap(source))
		str = gsub(str, "(@source@)", source)
		str = gsub(str, "(@Target@)", strcap(target))
		str = gsub(str, "(@target@)", target)
		str = gsub(str, "(@target_possessive@)", target_possessive)
		str = gsub(str, "(@source_possessive@)", source_possessive)
		str = gsub(str, "(@target_reflexive@)", target_reflexive)
		str = gsub(str, "(@source_reflexive@)", source_reflexive)
		str = gsub(str, "(@spell@)", spl.name)
		str = gsub(str, "(@isare@)", isare)

		monst_message = str
	end

	return monst_message
end -- get_monster_spell_msg()

-- Sets everything up to cast a GUSS spell as a monster
declare_globals"cast_monster_spell_force_spell"
cast_monster_spell_force_spell = {}
function cast_monster_spell(m_idx, s, level, fail_perc, y, x)
	local monst = monster(m_idx)
	local c_ptr = cave(y, x)

	-- Are we forced into casting a spell ?
	if cast_monster_spell_force_spell and cast_monster_spell_force_spell.m_idx == m_idx then
		s = cast_monster_spell_force_spell.spell
		cast_monster_spell_force_spell = nil
	end

	-- If the spell is affected by antimagic, they say so
	if (__tmp_spells[s].antimagic == true) and (player.antimagic_dis >= monst.cdis) and (rng.percent(player.antimagic)) then
		message("Your antimagic field disrupts "..monster_desc(monst, 6).." spell.")
		hook.process(hook.ANTIMAGIC_ABSORB, m_idx, s, level)
		return false
	end

	-- Get the monsetr name if we target a monster
	local target_monst
	if c_ptr.m_idx > 0 then
		target_monst = monster(c_ptr.m_idx)
	end

	-- Set target
	set_auto_cast(m_idx, level, y, x)

	-- Temporarily clear any helper monster
	set_monst_helper(nil)

	local failed = rng.percent(100 - fail_perc)

	-- Use custom message
	local str
	local blind  = (player.has_intrinsic(FLAG_BLIND) and
					not __tmp_spells[s].ignore_blind)
	local unseen_caster = (((not monst.ml) or blind) and
						   not __tmp_spells[s].ignore_unseen)
	local unseen_target = (((target_monst and (not target_monst.ml or blind))
						   or __tmp_spells[s].no_target)
							   and not __tmp_spells[s].ignore_unseen)
	local monst_message =
		get_monster_spell_msg(__tmp_spells[s], failed, monst, target_monst,
							  blind, unseen_caster, unseen_target)

	-- If there's a custom message, use it
	if not monst_message and not unseen_caster then
		local target = "you"
		if target_monst then
			target = monster_desc(target_monst, 4)
		end

		if failed then
			monst_message = strcap(monster_desc(monst, 0)) ..
				" fails to cast "..spell(s).name.." at "..target.."."
		else
			monst_message = strcap(monster_desc(monst, 0)) ..
				" casts "..spell(s).name.." at "..target.."."
		end
	end

	-- Disturb if asked for, or if the player is targeted
	if (game_options.disturb_other and monst_message) or
		((player.py == y) and (player.px == x) and not spell(s).no_target)
	then
		term.disturb(1, 0)

		if monst_message then
			message(monst_message)
		end
	else
		-- Do not bother the player
		if monst_message then
			message.add(1, monst_message, color.WHITE)
		end
	end

	-- And finnaly cast the frelling spell!
	if not failed then
		local ret = execute_school_spell(s, "monster cast spell", monst.faction)
		if ret == SPELL_NOTHING then
			failed = true
		end
	end

	-- Unset target
	unset_auto_cast()

	-- Restore previous helper monster
	unset_monst_helper()

	-- Update monster memory if player can see monster, plus if the
	-- spell succeeded or the spell is obvious even when failed.
	if monst.ml and (not failed or __tmp_spells[s].memory_if_fail) then
		-- Do we need to see the target to know what the spell is?
		if not target_monst or target_monst.ml or
			__tmp_spells[s].memory_if_target_unseen
		then
			memory.add(RT_SPELL, s, monst)
		end
	end

	return not failed
end -- cast_monster_spell()

-- Specialized add_spell function to quickly create monster spells
new_skill
{
	define_as = "SKILL_MONSTER"
	name = "Monster-magic"
	desc = "Monster spells"
	flags = { HIDDEN=1 AUTO_HIDE=1 }
}

SCHOOL_MONSTER_ABILITY = add_school
{
	["name"]  = "Monster abilities",
	["memory"] = "",
	["skill"] = "SKILL_MONSTER",
}

SCHOOL_MONSTER_SPELL = add_school
{
	["name"]   = "Monster spells",
	["memory"] = "cast",
	["skill"]  = "SKILL_MONSTER",
}

function add_monster_spell(s)
	if not s.school then s.school = SCHOOL_MONSTER_SPELL end
	if not s.level then s.level = 1 end
	if not s.mana then s.mana = 0 end
	if not s.fail then s.fail = 0 end
	if not s.desc then s.desc = { "Monster spell" } end
	if not s.random then s.random = "monster" end

        local index = add_spell(s)

	-- Automagically sets the variable name, lua just rules ! :)
	declare_global_constants{"MONST_SPELL_"..strupper(gsub(s.name, "( )", "_"))}
	setglobal("MONST_SPELL_"..strupper(gsub(s.name, "( )", "_")), index)
end

-- Returns caster hp / div capped at max
function get_caster_damage(max, div)
	local hp

	if spell_caster == 0 then
		hp = player.chp()
	else
		hp = monster(spell_caster).hp
	end

	hp = hp / div
	if hp > max then hp = max end
	return hp
end

-- Returns various usefull info on the current caster
function get_spell_caster_info()
	if spell_caster == 0 then
		return player.py, player.px
	else
		local monst = monster(spell_caster)
		return monst.fy, monst.fx
	end
end

function get_spell_caster_level()
	if spell_caster == 0 then
		return player.lev
	else
		local monst = monster(spell_caster)
		return monst.level
	end
end

-- The only required spell(used by monsetr forced casting to uncontrol)
add_monster_spell
{
	["name"]        = "Stop control",
	["action"] 	= "You stop controlling @source@.",
	["spell"]       = function()
			if (get_check("Abandon it permanently?")) then
				delete_monster_idx(spell_caster)
			end
			player.control = 0
			return true
	end,
}

function do_monster_saving_throw(who, mon)
	message("IMPLEMENT do_monster_saving_throw()!!")
	return false
end
