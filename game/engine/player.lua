-- SYSTEM FILE
--
-- Lua player funtions
--

declare_global_constants {
	"__mkey_fct_activate",
	"__mkey_fct_hide",
	"__power_fct_activate",
	"add_mkey",
	"add_power",
	"ability",
	"skill",
	"kill_player",
	"subrace",
	"subrace_add_power",
	"apply_flags",
	"gather_flags",
--	"self_knowledge",
}
declare_globals {
	"__mkey_fct",
	"__power_fct",
	"take_hit",
	"hp_player",
}

-------- skill stuff ---------

-- Easy ability access
function ability(i)
	return ab_info[i + 1]
end

-- Easy skill access
function skill(i)
	return s_info[i + 1]
end

-- Sart a lasting spell
function player.start_lasting_spell(spl)
	player.music_extra = -spl
end

-- stat mods
function player.modify_stat(stat, inc)
	player.stat_add[stat] = player.stat_add[stat] + inc
end

-- stat
function player.stat(stat)
	return player.stat_ind[stat]
end

-- powers mods
function player.add_power(pow)
	player.powers[1 + pow] = true
end

-- easier inventory access
--function player.inventory(i)
--	return player.inventory_real[i + 1]
--end


-- Return the coordinates of the player whether in wild or not
function player.get_wild_coord()
	if player.wild_mode then
		return player.py, player.px
	else
		return player.wilderness_y, player.wilderness_x
	end
end

-- Create a new power
__power_fct = {}
function add_power(p)
	local i

	assert(p.name, "No power name!")
	assert(p.desc, "No power desc!")
	assert(p.desc_get, "No power desc get!")
	assert(p.desc_lose, "No power desc lose!")
	assert(p.stat, "No power stat!")
	assert(p.level, "No power level!")
	assert(p.cost, "No power cost!")
	assert(p.fail, "No power fail!")
	assert(p.power, "No power power!")

	i = add_new_power(p.name, p.desc, p.desc_get, p.desc_lose, p.level, p.cost, p.stat, p.fail)
	__power_fct[i] = p.power
	return i
end

function __power_fct_activate(power)
	if __power_fct[power] then
		__power_fct[power]()
		return true
	else
		return FALSE
	end
end

-- Register in the hook list
hook.add_script(hook.ACTIVATE_POWER, "__power_fct_activate", "__power_fct_activate")


--- Mkeys

-- Create a new power
__mkey_fct = {
	__next      = 50
	__skills    = {}
	__abilities = {}
}
function add_mkey(p)
	local i

	assert(p.fct, "No mkey fct!")

	if not p.mkey then
		p.mkey = __mkey_fct.__next
		__mkey_fct.__next = __mkey_fct.__next + 1
	end

	if type(p.mkey) == "string" then
		assert(p.type, "mkey can't be a string without specifying a type")
		local mkey_str = p.mkey

		p.mkey = __mkey_fct.__next
		__mkey_fct.__next = __mkey_fct.__next + 1

		if p.type == "skill" then
			__mkey_fct.__skills[mkey_str] = p.mkey
			__mkey_fct.__skills[p.mkey]   = mkey_str
		elseif p.type == "ability" then
			__mkey_fct.__abilities[mkey_str] = p.mkey
			__mkey_fct.__abilities[p.mkey] = mkey_str
		else
			error("Invalid mkey type '" .. mkey_str .. "'")
		end
	end

	__mkey_fct[p.mkey] = {}
	__mkey_fct[p.mkey].activate = p.fct
	if p.browse then __mkey_fct[p.mkey].browse = p.browse end
	if p.hide then __mkey_fct[p.mkey].hide = p.hide
	elseif p.allow then __mkey_fct[p.mkey].hide = function(type) return not %p.allow(type) end end
	return p.mkey
end

function __mkey_fct_hide(power, type)
	if __mkey_fct[power] and __mkey_fct[power].hide then
		return __mkey_fct[power].hide(type)
	end
end

function __mkey_fct_activate(power, type)
	if type == ACTIVATE_SKILL and __mkey_fct[power] then
		__mkey_fct[power].activate()
		return true
	elseif type == BROWSE_SKILL and __mkey_fct[power] and __mkey_fct[power].browse then
		__mkey_fct[power].browse()
		return true
	else
		return FALSE
	end
end

-- Register in the hook list
hook.add_script(hook.MKEY, "__mkey_fct_activate", "__mkey_fct_activate")
hook.add_script(hook.MKEY_HIDE, "__mkey_fct_hide", "__mkey_fct_hide")

-- Subraces
function subrace(racem)
	return race_mod_info[racem + 1]
end

function subrace_add_power(subrace, power)
	for i = 1, 4 do
		if subrace.powers[i] == -1 then
			subrace.powers[i] = power
			return not nil
		end
	end
	return nil
end

-- Body parts
function player.add_body_part(part, nb)
	player.inventory[part][INVEN_LIMIT_KEY] = player.inventory[part][INVEN_LIMIT_KEY] + nb
	if player.inventory[part][INVEN_LIMIT_KEY] < 0 then player.inventory[part][INVEN_LIMIT_KEY] = 0 end
end

-- Intrinsincs
function player.has_intrinsic(i)
	return flag_get(player.intrinsic_flags, i) ~= 0
end
function player.intrinsic(i)
	return flag_get(player.intrinsic_flags, i)
end
function player.set_intrinsic(i, v)
	if not v then v = true end
	return flag_set(player.intrinsic_flags, i, v)
end
function player.inc_intrinsic(i, v)
	if not v then v = 1 end
	return flag_set(player.intrinsic_flags, i, flag_get(player.intrinsic_flags, i) + v)
end

function player.add_intrinsic_higher(i, v)
	if tag(v) == TAG_NUMBER then
		local curr_val = player.intrinsic_flags[i] or 0
		if curr_val < v then
			player.intrinsic_flags[i] = v
		end
		return v
	elseif tag(v) == TAG_FLAG then
		if not player.intrinsic_flags[i] then
			player.intrinsic_flags[i] = flag_new()
		end
		return flag_add_higher(player.intrinsic_flags[i], v)
	else
		error("Can't handle value of type '" .. type(v) .. "'")
	end
end

-- Applying flags in a consistent way
function apply_flags(flags_table, src)
	if tag(flags_table) == TAG_FLAG then
		player.apply_flags(flags_table, src or {})
	else
		local flags = grab_flags(flags_table, flag_new(), nil)
		player.apply_flags(flags, src or {})
		flag_free(flags, true)
	end
end

-- player.apply_flags() helpers
function player.set_intrin_by_list(flags, list)
	for i = 1, getn(list) do
		if flag_exists(flags, list[i]) then
			player.set_intrinsic(list[i], true)
		end
	end
end

function player.inc_intrin_by_list(flags, list)
	for i = 1, getn(list) do
		local flag = list[i]
		local mult = 1

		if tag(flag) == TAG_TABLE then
			mult = flag[2]
			flag = flag[1]
		end

		if flag_exists(flags, flag) then
			player.inc_intrinsic(flag, flag_get(flags, flag) * mult)
		end
	end
end

function player.add_intrin_higher_by_list(flags, list)
	for i = 1, getn(list) do
		if flag_exists(flags, list[i]) then
			player.add_intrinsic_higher(list[i], flags[list[i]])
		end
	end
end

function player.set_field_by_list(flags, list)
	for i = 1, getn(list) do
		local flag  = list[i][1]
		local field = list[i][2]

		if flag_exists(flags, flag) then
			player[field] = true
		end
	end
end

function player.inc_field_by_list(flags, list)
	for i = 1, getn(list) do
		local flag  = list[i][1]
		local field = list[i][2]
		local mult  = list[i][3] or 1

		if flag_exists(flags, flag) then
			player[field] = player[field] + (flag_get(flags, flag) * mult)
		end
	end
end

function player.inc_flagset_by_list(flags, list)
	for i = 1, getn(list) do
		local flag  = list[i][1]
		local field = list[i][2]

		if flag_exists(flags, flag) then
			flag_add_increase(player[field], flag_get_flags(flags, flag))
		end
	end
end

function player.add_intrin_blow_resp(resp, param1, param2)
	if tag(resp) == TAG_FLAG then
		for i = 1, flag_max_key(resp) do
			if resp[i] then
				player.add_intrin_blow_resp(i, resp[i], flag_get2(resp, i))
			end
		end
		return
	end

	local resp_flags = player.intrinsic_flags[FLAG_BLOW_RESPONSE]

	if not resp_flags then
		resp_flags = flag_new(2)

		player.intrinsic_flags[FLAG_BLOW_RESPONSE] = resp_flags
	end

	if tag(param1) == TAG_NUMBER then
		if param2 and param2 ~= 0 then
			flag_set_full(resp_flags, resp, FLAG_FLAG_BOTH,
						  (resp_flags[resp] or 0) + param1,
						  flag_get2(resp_flags, resp) + param2)
		else
			if param1 > (resp_flags[resp] or 0) then
				resp_flags[resp] = param1
			end
		end
	else
		resp_flags[resp] = param1
	end
end -- player.add_intrin_blow_resp()

-- Gathering flags in a consistent way
function gather_flags(array, flags_table, src)
	if tag(flags_table) == TAG_FLAG then
		player.gather_flags(array, flags_table, src or {})
	else
		local flags = grab_flags(flags_table, flag_new(), nil)
		player.gather_flags(array, flags, src or {})
		flag_free(flags, true)
	end
end

-- Inventory stuff
player.inventory_limits = function(inven)
	return player.inventory[inven][INVEN_LIMIT_KEY]
end

-- Get current speed in a decent format
player.speed = function(speedid)
	if not speedid then
		return player.pspeed - 110
	else
		return (player.pspeed - 110) + flag_get(player.speeds, speedid)
	end
end

-- Set current speed in a decent format
player.inc_speed = function(speedid, inc)
	if not inc then
		player.pspeed = player.pspeed + speedid
	else
		player.speeds[speedid] = flag_get(player.speeds, speedid) + inc
	end
end

-- Descriptor stuff
player.get_descriptor = function(type)
	for i = 1, flag_max_key(player.descriptors) do
		local desc = descriptor_info(player.descriptors[i])
		if desc.type == type then return desc, i end
	end
end

-- Resists
function player.resist(dam)
	return player.resists[dam] or 0
end

-- Get list of resists
function player.resists_list()
	local list = {}

	for i = 1, dam.max_type do
		if dam.raw[i] then
			if player.resists[i] then
				tinsert(list, i)
			end
		end
	end

	return list
end

-- Print out information about player
function self_knowledge(ffff)
	if not player.self_knowledge or
		type(player.self_knowledge) ~= "function" then
		error("player must define function 'self_knowledge'")
		return
	end

	local lines = player.self_knowledge()

	-- Print out to terminal
	term.save()

	-- Erase screen
	for i = 1, 24 do
		term.prt("", i, 13)
	end

	-- Print out lines in chunks of 20
	local line_num
	for i = 1, getn(lines) do
		line_num = imod(i - 1, 20) + 2
		term.prt(lines[i], line_num, 13)

		-- Pause, erase screen every 20 lines
		if imod(i - 1, 20) == 0 and i ~= getn(lines) and
			i ~= 1 then
			term.prt("-- more --", 22, 13)
			term.inkey()

			for i = 1, 24 do
				term.prt("", i, 13)
			end
		end
	end

	term.prt("[Press any key to continue]", line_num + 1, 13)
	term.inkey()

	-- Done, restore screen
	term.load()
end

hook.new_hook_type("CALC_BONUS_BEGIN")


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
end -- player.gather_flags()

--
-- Apply the flags which have been gathered together
--
function player.apply_gathered_flags(array)
	if array[FLAG_RESIST] then
		local resists = array[FLAG_RESIST]
		for i = 1, getn(resists) do
			if resists[i] then
				local val = 0
				for j = 1, getn(resists[i]) do val = val + resists[i][j] end

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
end

--
-- Compute equipment sizes
--
hook
{
	[hook.BODY_PARTS] = function()
		for_inventory(player, INVEN_PACK, INVEN_TOTAL, function(obj)
			if obj.flags[FLAG_EQUIPMENT_SIZE] then
				foreach_flags(obj.flags[FLAG_EQUIPMENT_SIZE], function(flags, slot)
					player.add_body_part(slot, flags[slot])
				end)
			end
		end)
	end

	[hook.TAKEOFF_PRE] = function(obj)
		if obj.flags[FLAG_EQUIPMENT_SIZE] then
			local ret = {}
			foreach_flags(obj.flags[FLAG_EQUIPMENT_SIZE], function(flags, slot)
				-- Make sure there will be enough room left to store the objects
				-- Otherwise they will be dropped by the engine, even if cursed or such
				if player.inventory_limits(slot) - flags[slot] < flag_max_key(player.inventory[slot]) then
					message(color.LIGHT_RED, "You are not able to remove your "..object_desc(%obj).." because it grants")
					message(color.LIGHT_RED, "you additional slots(currently used) in your "..inventory_slot_names[slot + 1].desc..".")
					%ret.stop = true
				end
			end)
			if ret.stop then return true end
		end
	end
}



-- Basic computes for calc_bonuses
player.old_sets_active = {}
player.basic_calc_bonuses = function(silent, extra_fct)
	local i, j, hold
	local o_ptr

	-- Clear extra blows/shots
	player.extra_blows = 0
	player.extra_shots = 0

	-- Clear the stat modifiers
	for i = 0, stats.MAX do player.stat_add[i] = 0 end

	-- Clear the skill modifiers
	flag_empty(player.skill_bonuses)

	-- Mana multiplier
	player.to_m = 0

	-- Life multiplier
	player.to_l = 0

	-- Spell power
	player.to_s = 0

	-- Clear the Displayed/Real armor class
	player.ac = 0
	player.dis_ac = 0

	-- Clear the Displayed/Real Bonuses
	player.dis_to_h = 0
	player.to_h = 0
	player.dis_to_d = 0
	player.to_d = 0
	player.to_a = 0
	player.dis_to_a = 0

	-- Start with "normal" speed
	player.pspeed = 110

	-- Start with a single blow per turn
	player.num_blow = 1

	-- Start with a single shot per turn
	player.num_fire = 1

	-- Starts with single throwing damage
	player.throw_mult = 1

	-- Reset the "xtra" tval
	player.tval_xtra = 0

	-- Clear all the flags
	flag_empty(player.intrinsic_flags)
	flag_empty(player.resists)
	flag_empty(player.absorbs)
	flag_empty(player.speeds)
	player.intrinsic_flags[FLAG_SUST_STATS] = flag_new(2)
	player.intrinsic_flags[FLAG_PROTECT_FROM] = flag_new(2)
	player.precognition = FALSE

	-- The anti magic field surrounding the player
	player.antimagic = 0
	player.antimagic_dis = 0

	-- Base infravision (purely descriptor based)
	player.see_infra = player.descriptor.infra

	-- Base skill -- disarming
	player.skill_dis = 0

	-- Base skill -- magic devices
	player.skill_dev = 0

	-- Base skill -- saving throw
	player.skill_sav = 0

	-- Base skill -- stealth
	player.skill_stl = 0

	-- Base skill -- combat (normal)
	player.skill_thn = 0

	-- Base skill -- combat (shooting)
	player.skill_thb = 0

	-- Base skill -- combat (throwing)
	player.skill_tht = 0

	-- Base Luck
	player.luck_cur = 0

	-- Apply the racial modifiers
	for i = 0, stats.MAX do
		-- Modify the stats for "race"
		player.stat_add[i] = player.stat_add[i] + player.descriptor.adj[i]
	end

	hook.process(hook.CALC_BONUS_BEGIN, silent)

	-- Scan the usable inventory
	player.gathered = {}
	local sets_found = {}
	for_inventory(player, INVEN_PACK, INVEN_TOTAL, function(o_ptr)
		apply_flags(o_ptr.flags, o_ptr)
		gather_flags(player.gathered, o_ptr.flags, o_ptr)

		-- Modify the base armor class
		player.ac = player.ac + o_ptr.ac

		-- The base armor class is always known
		player.dis_ac = player.dis_ac + o_ptr.ac

		-- Apply the bonuses to armor class
		player.to_a = player.to_a + o_ptr.to_a

		-- Apply the bonuses to hit/damage, only for non-weapons
		-- Weapnos are individualy applied in the combat systems
		if not o_ptr.flags[FLAG_WEAPON] then
			player.to_h = player.to_h + o_ptr.to_h
			player.to_d = player.to_d +o_ptr.to_d
		end

		-- Apply the mental bonuses to armor class, if known
		-- Apply the mental bonuses tp hit/damage, if known
		if is_known(o_ptr) then
			player.dis_to_a = player.dis_to_a + o_ptr.to_a
			if not o_ptr.flags[FLAG_WEAPON] then
				player.dis_to_h = player.dis_to_h + o_ptr.to_h
				player.dis_to_d = player.dis_to_d + o_ptr.to_d
			end
		end

		-- Gather and activate sets
		if o_ptr.artifact_id > 0 and a_info(o_ptr.artifact_id).set > 0 then
			local idx = a_info(o_ptr.artifact_id).set
			if not %sets_found[idx] then %sets_found[idx] = { count = 0 active = {} } end
			%sets_found[idx].count = %sets_found[idx].count + 1
			%sets_found[idx].active[o_ptr.artifact_id] = a_info(o_ptr.artifact_id).set_pos
		end
	end)

	-- Apply flags from descriptor
	apply_flags(player.descriptor.flags)
	gather_flags(player.gathered, player.descriptor.flags)

	-- Apply flags that are gained as the player increases in levels
	for i = 0, player.lev do
		local flags = player.descriptor.oflags[i + 1]
		if flag_used(flags) > 0 then
			apply_flags(flags)
			gather_flags(player.gathered, flags)
		end
	end

	-- Now apply sets
	local new_sets_active = {}
	for idx, data in sets_found do
		local set = set_info(idx)

		for art_id, pos in data.active do
			-- Any flags to apply now?
			if flag_used(set.arts(pos).flags[data.count]) > 0 then
				local flags = set.arts(pos).flags[data.count]
				apply_flags(flags, o_ptr)
				gather_flags(player.gathered, flags, o_ptr)
			end
		end

		if data.count == set.num then
			new_sets_active[idx] = true
			if not silent and not player.old_sets_active[idx] then
				message(color.LIGHT_GREEN, 'Set "'..set.name..'" completed!')
			end
		end
	end

	-- Warn sets taken off
	if not silent then
		for idx, _ in player.old_sets_active do
			if not sets_found[idx] or sets_found[idx].count < set_info(idx).num then
				message(color.LIGHT_RED, 'Set "'..set_info(idx).name..'" not complete anymore!')
			end
		end
	end
	player.old_sets_active = new_sets_active

	-- Bonuses granted by the dungeon level
	if has_flag(level_flags, FLAG_LEVEL_BONUS) then
		apply_flags(level_flags.flags[FLAG_LEVEL_BONUS])
		gather_flags(player.gathered, level_flags.flags[FLAG_LEVEL_BONUS])
	end

	-- Bonuses gained from monster body
	if has_flag(player.body_monster, FLAG_BODY_BONUS) then
		apply_flags(player.body_monster[FLAG_BODY_BONUS])
		gather_flags(player.gathered, player.body_monster[FLAG_BODY_BONUS])
	end

	-- Let the scripts do what they need
	if extra_fct then extra_fct(silent) end
	hook.process(hook.CALC_BONUS, silent)

	-- Apply the gathered flags before executing the timed effects,
	-- as the timed effects assume that the equipment resistances have
	-- already been calculated (at least in ToME).
	player.apply_gathered_flags(player.gathered)

	-- Compute the timed effects bonuses
	timed_effect.execute()

	-- Calculate stats
	for i = 0, stats.MAX do
		local top, ind

		-- Extract the new "stat_use" value for the stat
		top = stats.modify_value(player.stat_max[i], player.stat_add[i])

		-- Notice changes
		if player.stat_top[i] ~= top then
			-- Save the new value
			player.stat_top[i] = top

			-- Redisplay the stats later
			player.redraw[FLAG_PR_STATS] = true

			-- Window stuff
			player.window[FLAG_PW_PLAYER] = true
		end

		-- Extract the new "stat_use" value for the stat
		ind = stats.modify_value(player.stat_cur[i], player.stat_add[i])

		-- Notice changes
		if player.stat_ind[i] ~= ind then
			-- Save the new index
			player.stat_ind[i] = ind

			-- Window stuff
			player.window[FLAG_PW_PLAYER] = true
		end
	end
end

--- @fn
-- @brief This is a placeholder function to damage the player. The life subsystem redefines it
function take_hit(dam, die_from)
end

--- @fn
-- @brief This is a placeholder function to heal the player. The life subsystem redefines it
function hp_player(heal)
end

--- @fn
-- @brief Kills hte player
-- @param reason String Why did the kill happen
function kill_player(reason)
	winner_state = 0
	death = true
	player.leaving = true
	set_died_from(reason or "(unknown)")
end

--- @fn
-- @brief Regenerates the current town(only use when the player is in the wilderness)
function player.regen_town()
	player.leaving = true
	player.oldpx = player.px
	player.oldpy = player.py
end

--- @fn
-- @brief Gains exp from a monster death. Modules should redefine it to whatever they see fit
function player.gain_exp_from_monster(monst)
	if is_friend(monst) <= 0 then
		return race_info(monst).mexp
	end

	return 0
end

function player.make_savefile_descriptor()
	return
	{
		"#GName: #w"..player_name(),
		"#GLevel: #w"..player.lev,
	}
end

-- Prints a descriptor file
hook(hook.SAVE_GAME_END, function()
	local file = fs.open(path_build(savefile(), "descriptor"), "w")
	if file == nil then message("Failed to write file descriptor: "..fs.get_last_error()) return end

	local data = player.make_savefile_descriptor()
	fs.write(file, player_name())
	fs.write(file, iif(death, "dead", "alive"))
	for i = 1, getn(data) do
		fs.write(file, data[i])
	end

	fs.close(file)
end)
