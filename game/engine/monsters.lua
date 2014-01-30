-- SYSTEM FILE
--
-- Monster stuff, do not touch
--


declare_global_constants {
	"flag_get_blow",
	"flag_get_monster",
	"add_monster_attack_method",
	"add_monster_attack_type",
	"find_monster_attack_method",
	"find_monster_attack_type",
	"monster_msg",
	"monster_player_msg",
	"player_cast_monster_spell",
	"set_fake_monster",
	"summon_monster",
	"control_monster_force_casting",
	"race_info",
	"monster_random_say",
	"for_each_monster",
	"parse_named_monster",
	"place_named_monster",
	"place_named_monster_near",
	"monster_take_hit",
}
declare_globals {
	"monster_explode",
	"monster_attack",
	"monster_regen",
	"default_monster_drop",
	"__monster_attack_methods",
	"__monster_attack_methods_max",
	"__monster_attack_types",
	"__monster_attack_types_max",
}

function summon_monster(y, x, lev, faction, typ)
	-- Setup placed_monster_idx, to distinguish monsters we
	-- summoned versus monsters summoned by other sources in
	-- response to various hooks
	placed_monster_id    = placed_monster_id + 1
	local our_placed_id  = placed_monster_id
	local cur_placed_num = placed_monsters_num

	if not typ then
		typ = {}
	end

	if tag(typ.flags) == TAG_TABLE then
		typ.flags = getter.flags(typ.flags)
	end

	if tag(typ.flags_forbid) == TAG_TABLE then
		typ.flags_forbid = getter.flags(typ.flags_forbid)
	end

	-- Don't allow for uniques, unless the caller either allows for
	-- them or specifically requires them.
	if not typ.allow_uniques and not (typ.flags and
									 typ.flags[FLAG_UNIQUE]) then
		if not typ.flags_forbid then
			typ.flags_forbid = flag_new()
		end

		typ.flags_forbid[FLAG_UNIQUE] = true
	end

	-- By default, don't care about faction friendliness
	if not typ.min_friend and not typ.max_friend then
		-- HACK
		typ.min_friend = 1
		typ.max_friend = -1
	end

	typ.min_friend = typ.min_friend or -infinity
	typ.max_friend = typ.max_friend or  infinity

	local ret
	ret = summon_specific(y, x, lev, faction,
						   typ.min_friend, typ.max_friend, typ.group_ok,
						   typ.flags, typ.flags_forbid,
						   typ.chars, typ.chars_forbid,
						   typ.name, typ.name_forbid)

	local summoned = {}
	for i = cur_placed_num, placed_monsters_num - 1 do
		if placed_monster_ids[i + 1] == our_placed_id then
			tinsert(summoned, placed_monsters[i + 1])
		end
	end

	placed_monster_id = our_placed_id - 1

	return ret, summoned
end

function race_info(monst)
	return race_info_idx(monst.r_idx, monst.ego)
end

function lookup_race(race, sval)
	if type(race) == "number" then
		return %lookup_race(race, sval)
	else
		return %lookup_race(strbyte(race), sval)
	end
end

--- @fn
-- @brief A monster takes damage from either the player or an other monster
function monster_take_hit(who, m_idx, dam, note)
	if not who or (who > 0 and not monster(who)) then who = WHO_PLAYER end
	if who == WHO_PLAYER then
		return mon_take_hit(m_idx, dam, 0, note)
	else
		return mon_take_hit_mon(who, m_idx, dam, 0, note)
	end
end

-- Monster combat
__monster_attack_methods_max = 0
__monster_attack_methods = {}
__monster_attack_types_max = 0
__monster_attack_types = {}
function add_monster_attack_method(t)
	assert(t.name, "Monster attack method has no name")
	assert(t.desc, "Attack method " .. t.name .. " has no desc")

	if not t.fct and not t.player and not t.monster then
		t.fct = function() end
	end

	if not t.monster and t.player then t.monster = function() end end
	if not t.player and t.monster then t.player = function() end end

	t.index = __monster_attack_methods_max

	__monster_attack_methods[t.index] = t
	__monster_attack_methods[t.name] = t

	__monster_attack_methods_max = __monster_attack_methods_max + 1
end

function add_monster_attack_type(t)
	assert(t.name, "Monster attack type has no name")
	assert(t.power, "Monster attack type " .. t.name ..
		   " has no power")
	assert(t.type, "Monster attack type " .. t.name ..
		   " has no type")
	assert(t.desc, "Monster attack type " .. t.name ..
		   " has no desc")
	if not t.fct and not t.player and not t.monster then
		t.fct = function() end
	end

	if not t.monster and t.player then t.monster = function() end end
	if not t.player and t.monster then t.player = function() end end

	t.index = __monster_attack_types_max

	__monster_attack_types[t.index] = t
	__monster_attack_types[t.name] = t

	__monster_attack_types_max = __monster_attack_types_max + 1
end

function find_monster_attack_method(s)
	assert(__monster_attack_methods[s], "unknown method " .. s)
	return __monster_attack_methods[s].index
end

function find_monster_attack_type(s)
	assert(__monster_attack_types[s], "unknown type " .. s)
	return __monster_attack_types[s].index
end

-- All the monster attack methods
add_monster_attack_method
{
	["name"]	= "*",
	["action"]	= "",
	["desc"]    = "*",
}
add_monster_attack_type
{
	["name"]	= "*",
	["power"]	= 60,
	["type"]	= 0,
	["desc"]    = "*",
}

function flag_get_blow(flag, idx)
	return __wrap_ptr.to_monster_blow(flag_get_ptr(flag, idx, FLAG_TYPE_MONSTER_BLOW))
end
function flag_get_monster(flag, idx)
	return __wrap_ptr.to_monster_type(flag_get_ptr(flag, idx, FLAG_TYPE_MONSTER))
end

-- Displays a monster message
function monster_msg(msg, see)
	if see ~= nil and see ~= FALSE then
	       -- Display
		if game_options.disturb_other then
			message(msg)
		else
			message.add(1, msg, color.WHITE)
			player.window[FLAG_PW_MESSAGE] = true
		end
	end
end

-- If the target is the player always print, otehrwith uses monster_msg
function monster_player_msg(msg, target, cond)
	if target then message(msg)
	else monster_msg(msg, cond)
	end
end

-- Fakes a monster record from a race. Uses m_idx = 0 which is normaly impossible
function set_fake_monster(r_idx, hp)
	local monst = monster(0)

	monst.r_idx = r_idx
	monst.ego = 0

	local race = race_info(monst)

	monst.ml = true
	monst.fy = player.py
	monst.fx = player.px
	monst.maxhp = rng.maxroll(race.hdice, race.hside)
	monst.hp = hp

	flag_copy(monst.blow, race.blow)
end

function monster_attack(m_idx, y, x, force_name)
	message(color.VIOLET, "Module must implement monster_attack()")
end

monster_explode = function() end

-- Player casting monster spells
function player_cast_monster_spell(m_spells, source_level, level, get_mana, calc_level, success_fail)
	local spells = {}

	-- List the monster spells
	for i = 1, m_spells.size do
		if (m_spells.node[i].flags & FLAG_FLAG_USED) ~= 0 then
			local spl = m_spells.node[i].key

			-- Verify level
			if calc_level(flag_get(m_spells, spl)) <= level then
				tinsert
				(
					spells,
					{
						index = spl,
						fail = bound(((100 - flag_get2(m_spells, spl)) * level) / source_level, 5, 95),
						level = calc_level(flag_get(m_spells, spl))
					}
				)
			end
		end
	end
	-- Sort by level and name
	sort(spells, function(s1, s2) return (s1.level < s2.level) or ((s1.level == s2.level) and (spell(s1.index).name < spell(s2.index).name)) end)

	-- Ask what spell to cast
	local ret = choose_from_list(
		spells, "Spells", "Cast which spell?",
		function (y, x, thing)
			if not thing then
				term.blank_print(format("%-20s%-16s Level Cost Fail Info", "Name", "School"), y, x)
			else
				local color = color.LIGHT_GREEN

				set_get_level(thing.level)
				if %get_mana(thing.index) > player.csp() then
					color = color.ORANGE
				end
				term.blank_print(color, format("%-20s%-16s   %3d %4s %3d%s %s", spell(thing.index).name, "Monster", thing.level, %get_mana(thing.index), thing.fail, "%", __spell_info[thing.index]()), y, x)
				unset_get_level()
			end
		end,
		function (y, thing)
			print_spell_desc(thing.index, y)
		end
	)

	-- If nothing is selected, quit
	if not ret then return end

	-- try to cast
	set_get_level(ret.level)
	if rng.percent(ret.fail) then
		success_fail("fail", ret)
	else
		__spell_spell[ret.index]()
		success_fail("success", ret)
	end
	unset_get_level()
	energy_use = get_player_energy(SPEED_GLOBAL)
end

-- Symbiosis spell casting
declare_global_constants"symbiosis_cast_monster_spell"

function symbiosis_cast_monster_spell(r_idx, level)
	-- No magic
	if (player.antimagic > 0) then
		message("Your anti-magic field disrupts any magic attempts.")
		return
	end

	-- No magic
	if (player.anti_magic > 0) then
		message("Your anti-magic shell disrupts any magic attempts.")
		return
	end

	player_cast_monster_spell
	(
		race_info_idx(r_idx, 0).spells,
		race_info_idx(r_idx, 0).level,
		level,
		function() return 0 end,
		function(base_lvl)
			return get_skill_scale(SKILL_SYMBIOTIC, base_lvl)
		end,
		function(type, thing)
			if type == "fail" then
				message("You fail to get the spell off.")
			end
		end
	)
end

-- Controlled monster forced casting
function control_monster_force_casting()
	if player.control == 0 then return FALSE end

	local m_ptr = monster(player.control)
	local r_ptr = race_info(m_ptr)

	-- Copy spells
	local flag_spells = flag_dup(r_ptr.spells)

	-- Add end control spell
	flag_set_full(flag_spells, MONST_SPELL_STOP_CONTROL, FLAG_FLAG_BOTH, 0, 100)

	local spells = {}

	-- List the monster spells
	for i = 1, flag_spells.size do
		if (flag_spells.node[i].flags & FLAG_FLAG_USED) ~= 0 then
			local spl = flag_spells.node[i].key

			-- Verify level
			tinsert
			(
				spells,
				{
					index = spl,
					fail = 100 - flag_get2(flag_spells, spl),
					level = flag_get(flag_spells, spl)
				}
			)
		end
	end
	-- Sort by level and name
	sort(spells, function(s1, s2) return (s1.level < s2.level) or ((s1.level == s2.level) and (spell(s1.index).name < spell(s2.index).name)) end)

	-- Ask what spell to cast
	local ret = choose_from_list(
		spells, "Spells", "Cast which spell?",
		function (y, x, thing)
			if not thing then
				term.blank_print(format("%-20s%-16s Level Cost Fail Info", "Name", "School"), y, x)
			else
				local color = color.LIGHT_GREEN
				term.blank_print(color, format("%-20s%-16s   %3d %4s %3d%s %s", spell(thing.index).name, "Monster", thing.level, 0, thing.fail, "%", __spell_info[thing.index]()), y, x)
			end
		end,
		function (y, thing)
			print_spell_desc(thing.index, y)
		end
	)

	-- Something to do ?
	if ret then
		energy_use = get_player_energy(SPEED_GLOBAL)

		if ret.index == MONST_SPELL_STOP_CONTROL then
			set_spell_caster(player.control)
			__spell_spell[ret.index]()
			unset_spell_caster()
		else
			cast_monster_spell_force_spell = {}
			cast_monster_spell_force_spell.m_idx = player.control
			cast_monster_spell_force_spell.spell = ret.index
		end
	end

	-- Free memory
	flag_free(flag_spells, true)

	return true
end

--
-- Inform monster that it has died
--
new_flag("ON_DEATH")
hook(hook.MONSTER_DEATH,
function(m_idx)
	local monster = monster(m_idx)
	local func

	if monster.flags[FLAG_ON_DEATH] then
		func = get_function_registry_from_flag(monster.flags, FLAG_ON_DEATH)
		func(monster)
	end

	if monster.ego then
		local ego = re_info[monster.ego + 1]

		if ego[FLAG_ON_DEATH] then
			func = get_function_registry_from_flag(ego.flags,
							       FLAG_ON_DEATH)
			func(monster)
			end
	end
end)

--
-- Ask race if new member can be created
--
new_flag("NEW_MONSTER_PRE")
hook(hook.NEW_MONSTER_PRE,
function(y, x, r_idx, ego, slp, status)
	local r_ptr = r_info[r_idx + 1]
	local func
	local ret = nil

	if r_ptr.flags[FLAG_NEW_MONSTER_PRE] then
		func = get_function_registry_from_flag(r_ptr.flags,
											   FLAG_NEW_MONSTER_PRE)

		ret = func(y, x, r_idx, r_ptr, ego, slp, status)
	end

	local e_ptr
	if ego then
		e_ptr  = re_info[ego + 1]
	else
		e_ptr = { flags = flag_new() }
	end

	if e_ptr.flags[FLAG_NEW_MONSTER_PRE] then
		func = get_function_registry_from_flag(e_ptr.flags,
											   FLAG_NEW_MONSTER_PRE)

		ret = func(y, x, r_idx, r_ptr, ego, slp, status) or ret
	end

	return ret
end)

--
-- Inform race that new member has been created
--
new_flag("NEW_MONSTER_POST")
hook(hook.NEW_MONSTER_POST,
function(y, x, r_idx, ego, slp, status)
	local r_ptr = r_info[r_idx + 1]
	local ret = nil

	if r_ptr.flags[FLAG_NEW_MONSTER_POST] then
		local func = get_function_registry_from_flag(r_ptr.flags,
													 FLAG_NEW_MONSTER_POST);

		ret =  func(y, x, r_idx, r_ptr, ego, slp, status)
	end

	local e_ptr

	if ego then
		e_ptr  = re_info[ego + 1]
	else
		e_ptr= { flags = flag_new() }
	end

	if e_ptr.flags[FLAG_NEW_MONSTER_POST] then
		func = get_function_registry_from_flag(e_ptrflags,
											   FLAG_NEW_MONSTER_POST)

		ret = func(y, x, r_idx, r_ptr, ego, slp, status) or ret
	end

	return ret
end)

--
-- Let monster create its drops
--
function default_monster_drop(m_idx, m_ptr, r_ptr)
	error("Module must define its own default_monster_drop()")
end

new_flag("CREATE_DROP")
new_flag("DROP_THEME")
hook(hook.CREATE_DROP,
function(m_idx, m_ptr)
	local r_ptr = r_info[m_ptr.r_idx + 1]
	local ego   = { flags = flag_new() }

	if m_ptr.ego then
		ego = re_info[m_ptr.ego + 1]
	end


	if not (r_ptr.flags[FLAG_CREATE_DROP] or ego.flags[FLAG_CREATE_DROP]) and
		not (r_ptr.flags[FLAG_DROP_THEME] or ego.flags[FLAG_DROP_THEME]) and
		not dungeon(current_dungeon_idx).flags[FLAG_OBJ_THEME] then
		return
	end

	if r_ptr.flags[FLAG_CREATE_DROP] then
		local func = get_function_registry_from_flag(r_ptr.flags, FLAG_CREATE_DROP)
		func(m_idx, m_ptr, r_ptr)
	else
		default_monster_drop(m_idx, m_ptr, r_ptr)
	end

	-- Ego DROP_THEME just modifies the monster's base theme.  However,
	-- an ego might have it 's own CREATE_DROP function
	if ego.flags[FLAG_CREATE_DROP] then
		local func = get_function_registry_from_flag(ego.flags, FLAG_CREATE_DROP)
		func(m_idx, m_ptr, ego)
	end
end)

--
-- Say something
--
hook(hook.MON_SPEAK,
function(m_idx, m_name)
	local monst = monster(m_idx)
	local r_ptr = r_info[monst.r_idx + 1]

	if r_ptr.flags[FLAG_CAN_SPEAK] then
		local func = get_function_registry_from_flag(r_ptr.flags, FLAG_CAN_SPEAK)
		-- Flag might not be a function, in which case its a boolean telling
		-- the C-code portion of the engine to use a random quote from
		-- file 'monspeak.txt'
		if func then
			func(m_name, m_idx, monst)
			return true
		end
	end
end)

function monster_random_say(list)
	message(list[rng(getn(list))])
end

--
-- Chat
--
hook(hook.CHAT,
function(m_idx)
	local monst = monster(m_idx)
	local r_ptr = r_info[monst.r_idx + 1]
	local m_name = monster_desc(monst, 0)

	if r_ptr.flags[FLAG_CHATABLE] then
		local func = get_function_registry_from_flag(r_ptr.flags, FLAG_CHATABLE)
		func(m_name, m_idx, monst)
		return true
	end
end)

--- @fn
-- @brief Parses all monsters of the level and call a function on them
function for_each_monster(callback)
	for i = 1, monst_list.size do if (monst_list.node[i].flags & FLAG_FLAG_USED) ~= 0 then
		local idx = monst_list.node[i].key
		local monst = monster(idx)
		if monst and monst.r_idx ~= 0 then
			callback(idx, monst)
		end
	end end
end

--
-- Functions concerning monsters specified via a string
--
function parse_named_monster(str, quiet, no_faction)
	local orig_str = str
	local str_to_faction = {
		["pet"] = FACTION_PLAYER,
		["companion"] = FACTION_PLAYER,
		["friendly"] = FACTION_PLAYER,
		["town"] = FACTION_TOWN,
		["neutral"] = FACTION_TOWN,
		["hostile"] = FACTION_DUNGEON,
		["enemy"]   = FACTION_DUNGEON,
		["dungeon"] = FACTION_DUNGEON,
	}

	local i = 0

	for i = 0, factions.__next do
		if factions.faction_names[i] then
			str_to_faction[factions.faction_names[i]] = i
		end
	end

	str = clean_whitespace(strlower(str))

	if str == "" then
		return nil
	end

	local words = strsplit(str, ' ')

	local parts = {}

	if str_to_faction[words[1]] and not no_faction then
		parts.faction_name = words[1]
		parts.faction      = str_to_faction[words[1]]

		str = strsub(str, strlen(words[1]) + 2, -1)
	end

	local longest_name = -1
	local longest_idx  = -1
	local i
	for i = 1, max_r_idx do
		local name = strlower(r_info[i].name)

		if strfind(str, name, 1, 1) and strlen(name) > longest_name then
			longest_name = strlen(name)
			longest_idx = i
		end
	end

	if longest_name < 1 then
		-- Try parsing while ignoring faction strings, in case
		-- a monster's name starts with a faction name.
		if not no_faction then
			local parts = parse_named_monster(orig_str, true, true)
			if parts then
				return parts
			end
		end

		if not quiet then
			message(color.VIOLET, "No such monster as '" .. str .. "'")
		end
		return nil
	end

	local race_info = r_info[longest_idx]
	parts.race_name = race_info.name
	parts.race_idx  = longest_idx - 1

	if strlen(str) ~= strlen(parts.race_name) then
		-- Look for monster ego
		if starts_with(str, parts.race_name) then
			-- Suffix ego
			parts.ego_name = strsub(str, strlen(parts.race_name) + 1, -1)
		elseif ends_with(str, parts.race_name) then
			-- Prefix ego
			parts.ego_name = strsub(str, 1,
									strlen(str) - strlen(parts.race_name) - 1)
		else
			if not quiet then
				message(color.VIOLET, "No double-ego monsters exist.")
			end
			return nil
		end

		parts.ego_idx = -1
		local ego_info
		for i = 1, max_re_idx do
			if strlower(re_info[i].name or "") == parts.ego_name then
				parts.ego_idx = i - 1
				ego_info = re_info[i]
				break
			end
		end

		if parts.ego_idx == -1 then
			if not quiet then
				message(color.VIOLET, "No such monster ego as '" ..
						parts.ego_name .. "'")
			end
			return nil
		end

		-- Needed flags
		local max_key = flag_max_key(ego_info.flags)
		for i = 0, max_key do
			if ego_info.flags[i] and not race_info.flags[i] then
				if not quiet then
					message(color.VIOLET,
							"Ego not compatible with monster race.")
				end
				return nil
			end
		end

		-- Forbidden flags
		max_key = flag_max_key(ego_info.hflags)
		for i = 0, max_key do
			if ego_info.hflags[i] and race_info.flags[i] then
				message(color.VIOLET,
						"Ego not compatible with monster race.")
				return nil
			end
		end
	end -- has ego

	if not parts.faction then
		parts.faction      = r_info[parts.race_idx + 1].flags[FLAG_FACTION]

		if not parts.faction then
			parts.faction = FACTION_DUNGEON
		end

		parts.faction_name = factions.faction_names[parts.faction]
	end

	return parts
end -- parse_named_monster()

function place_named_monster(y, x, monst, sleep)
	if type(monst) == "string" then
		monst = parse_named_monster(monst)
	end

	if not monst then
		return nil
	end

	local m_idx= place_monster_one(y, x, monst.race_idx, monst.ego_idx or 0,
								   sleep or 0 , monst.faction)

	if m_idx > 0 then
		factions.change_faction(monster(m_idx), monst.faction)
	end

	return m_idx
end -- place_named_monster()

function place_named_monster_near(y, x, monst, sleep, group)
	local i, yp, xp
	local found = false

	yp = nil
	xp = nil

	for i = 0, 20 do
		local dist = (i / 15) + 1

		yp, xp = scatter(y, x, dist, 0)

		if yp and xp and cave_empty_bold(yp, xp) then
			found = true
			break
		end
	end

	if not found then
		return -1
	end

	if type(monst) == "string" then
		monst = parse_named_monster(monst)
	end

	if not monst then
		return -1
	end

	local m_idx = place_monster_aux_ego(yp, xp, monst.race_idx,
										monst.ego_idx or 0, sleep or 0,
										group or 0, monst.faction)

	if m_idx > 0 then
		monster(m_idx).faction = monst.faction
	end

	return m_idx
end -- place_named_monster()

--- @fn
-- @brief Regenerate a monster every 10 game turns
function monster_regen(m_idx, monst)
	-- Monster do not regen by default

	-- Get rid of whatever effecst it might have
	-- If your module does not have any monster_effects this line can be removed
	monster_effect.timeout(m_idx, monst)
end

-- Regen all monsters
hook(hook.PROCESS_WORLD_NON_WILD, function()
	for_each_monster(monster_regen)
end)


-- Freeze all monsters upon new level to prevent instadeath
hook(hook.LEVEL_END_GEN, function()
	for_each_monster(function(m_idx, monst)
		monst.mflag = monst.mflag | MFLAG_BORN
		monst.energy = 0
	end)
end)



----------------------------------------------------------
------------------- Monster timed effects ----------------
----------------------------------------------------------
constant("monster_effect", {})
settag(monster_effect, TAG_NAMESPACE) -- Tag as a namespace

-- The next effect to make
monster_effect.__next = 1

new_flag("EFFECTS")
new_flag("EFFECTS_AUX")

--- @fn
-- @brief Makes a new timed effect
function monster_effect.create(t)
	assert(t.name, "No monster effect name")
	monster_effect[monster_effect.__next] = t
	monster_effect[t.name] = monster_effect.__next
	monster_effect.__next = monster_effect.__next + 1
end

--- @fn
-- @brief Call when the effects time must decrease
-- @param monst Monster Monster to affect
function monster_effect.timeout(m_idx, monst)
	local effects = monst.flags[FLAG_EFFECTS]
	if not effects then return end
	for i = 1, effects.size do
		if (effects.node[i].flags & FLAG_FLAG_USED) ~= 0 then
			local e = effects.node[i].key
			if monster_effect[e].on_timeout then monster_effect[e].on_timeout(m_idx, monst, monster_effect.get(monst, e)) end
			monster_effect.inc(nil, monst, e, -1)
		end
	end
end

--- @fn
-- @brief Sets a monster effect timeout and its parameter
-- @param monst Monster Monster to affect
-- @param effect Effect Effect to set
-- @param counter Number The time to have the effect last
-- @param power Number The power of the effect, if relevant
function monster_effect.set(who, monst, effect, counter, power)
	assert(monster_effect[effect], "Unknown effect")
	local effects = monst.flags[FLAG_EFFECTS]
	if not effects then
		effects = flag_new()
		monst.flags[FLAG_EFFECTS] = effects
		monst.flags[FLAG_EFFECTS_AUX] = flag_new()
	end
	if counter > 0 then
		if monst.ml and not flag_exists(effects, effect) then
			if tag(monster_effect[effect].on_gain) == TAG_STRING then
				message(format(monster_effect[effect].on_gain, strcap(monster_desc(monst, 0))))
			else
				monster_effect[effect].on_gain(monst, strcap(monster_desc(monst, 0)))
			end
		end
		flag_set_full(effects, effect, FLAG_FLAG_BOTH, counter, power or 1)
		if who then
			if not monst.flags[FLAG_EFFECTS_AUX][effect] then monst.flags[FLAG_EFFECTS_AUX][effect] = flag_new() end
			monst.flags[FLAG_EFFECTS_AUX][effect][0] = who
		end
	else
		if monst.ml and flag_exists(effects, effect) then
			if tag(monster_effect[effect].on_lose) == TAG_STRING then
				message(format(monster_effect[effect].on_lose, strcap(monster_desc(monst, 0))))
			else
				monster_effect[effect].on_lose(monst, strcap(monster_desc(monst, 0)))
			end
		end
		flag_remove(effects, effect)
	end
end

-- Calling monster_effect sets one
monster_effect.__exec_module = monster_effect.set

--- @fn
-- @brief Get info from the currently executing effect, or any effect
-- @param monst Monster The mosnetr in whcih the effect takes place
-- @param effect Effect The effect to check
function monster_effect.get(monst, effect)
	local effects = monst.flags[FLAG_EFFECTS]
	if not effects then
		return nil, nil
	else
		if flag_get(effects, effect) > 0 then
			return flag_get(effects, effect), flag_get2(effects, effect)
		else
			return nil, nil
		end
	end
end

--- @fn
-- @brief Set info from the currently executing effect, or any effect
-- @param monst Monster The mosnetr in whcih the effect takes place
-- @param effect Effect The effect to check
-- @param value Number The new power value
function monster_effect.set_param(monst, effect, value)
	local effects = monst.flags[FLAG_EFFECTS]
	if not effects then
		return nil, nil
	else
		if flag_exists(effects, effect) then
			flag_set_full(effects, effect, FLAG_FLAG_BOTH, flag_get(effects, effect), value)
		end
	end
end

--- @fn
-- @brief Get who caused the given effect
-- @param monst Monster The mosnetr in whcih the effect takes place
-- @param effect Effect The effect to check
function monster_effect.who(monst, effect)
	local effects = monst.flags[FLAG_EFFECTS_AUX]
	if not effects or not effects[effect] then
		return nil
	else
		return effects[effect][0]
	end
end

--- @fn
-- @brief Get info from the currently executing effect, or any effect
-- @param monst Monster The mosnetr in whcih the effect takes place
-- @param effect Effect The effect to check
-- @param inc Number The value to increase the counter by
function monster_effect.inc(who, monst, effect, inc)
	local cnt, pow = monster_effect.get(monst, effect)
	monster_effect.set(who, monst, effect, (cnt or 0) + inc, pow or 1)
end


--------------------------------------------------------------------------
------------------------------ Detection ---------------------------------
--------------------------------------------------------------------------

constant("monsters", {})

monsters.detect = function(radius, tester, msg)
	msg = msg or "You detect the presence of mnosters!"
	tester = tester or function() return true end
	local detected = false
	for y = player.py - radius, player.py + radius do
		for x = player.px - radius, player.px + radius do
			if in_bounds(y, x) and panel_contains(y, x) and distance(player.py, player.px, y, x) <= radius and cave(y, x).m_idx > 0 then
				local monst = monster(cave(y, x).m_idx)
				local ok = false

				if tag(tester) == TAG_NUMBER then
					ok = monster.flags[tester]
				elseif tag(tester) == TAG_TABLE then
					ok = true
					for i = 1, getn(tester) do
						if not monster.flags[tester[i]] then ok = false break end
					end
				else
					ok = tester(cave(y, x).m_idx, monst)
				end

				if ok then
					detected = true
					repair_monsters = true

					monst.mflag = monst.mflag | MFLAG_MARK | MFLAG_SHOW
					monst.ml = true
					lite_spot(y, x)
				end
			end
		end
	end
	if detected then
		message(msg)
		return true
	end
end
