flagsedit.version = "1.0.0"

flagsedit.CTRLO = 15

flagsedit.mistake_made = false

--
-- Flag hints.  Tells the editor what sort of value a particular flag
-- should have, so it can make more intelligent choices
--

flagsedit.HINT_INT      = 1 -- Integer, not intelligence
flagsedit.HINT_DUAL_INT = 2
flagsedit.HINT_BOOL     = 3
flagsedit.HINT_STR      = 4 -- String, not strength
flagsedit.HINT_FLAGSET  = 5
flagsedit.HINT_ARRAY    = 6 -- Flagset being used like an array
flagsedit.HINT_FUNC     = 7
flagsedit.HINT_OBJ      = 8
flagsedit.HINT_MONST    = 9

flagsedit.hints = {
	[FLAG_INVIS] = flagsedit.HINT_BOOL,
	[FLAG_REFLECT] = flagsedit.HINT_BOOL,
	[FLAG_FREE_ACT] = flagsedit.HINT_BOOL,
	[FLAG_HOLD_LIFE] = flagsedit.HINT_BOOL,
	[FLAG_WRAITH] = flagsedit.HINT_BOOL,
	[FLAG_EASY_KNOW] = flagsedit.HINT_BOOL,
	[FLAG_HIDE_TYPE] = flagsedit.HINT_BOOL,
	[FLAG_SHOW_MODS] = flagsedit.HINT_BOOL,
	[FLAG_SHOW_AC_MODS] = flagsedit.HINT_BOOL,
	[FLAG_SHOW_COMBAT_MODS] = flagsedit.HINT_BOOL,
	[FLAG_NORM_ART] = flagsedit.HINT_BOOL,
	[FLAG_INSTA_ART] = flagsedit.HINT_BOOL,
	[FLAG_BLESSED] = flagsedit.HINT_BOOL,
	[FLAG_CANNOT_REMOVE] = flagsedit.HINT_BOOL,
	[FLAG_AUTO_CURSE] = flagsedit.HINT_BOOL,
	[FLAG_NO_TELE] = flagsedit.HINT_BOOL,
	[FLAG_NO_MAGIC] = flagsedit.HINT_BOOL,
	[FLAG_FOUL_CURSE] = flagsedit.HINT_BOOL,
	[FLAG_DRAIN_EXP] = flagsedit.HINT_BOOL,
	[FLAG_TELEPORT] = flagsedit.HINT_BOOL,
	[FLAG_AGGRAVATE] = flagsedit.HINT_BOOL,
	[FLAG_CURSED] = flagsedit.HINT_BOOL,
	[FLAG_CURSE_NO_DROP] = flagsedit.HINT_BOOL,
	[FLAG_HEAVY_CURSE] = flagsedit.HINT_BOOL,
	[FLAG_PERMA_CURSE] = flagsedit.HINT_BOOL,
	[FLAG_BLACK_BREATH] = flagsedit.HINT_BOOL,
	[FLAG_NO_RECHARGE] = flagsedit.HINT_BOOL,
	[FLAG_COULD2H] = flagsedit.HINT_BOOL,
	[FLAG_MUST2H] = flagsedit.HINT_BOOL,
	[FLAG_SPECIAL_GENE] = flagsedit.HINT_BOOL,
	[FLAG_CLIMB] = flagsedit.HINT_BOOL,
	[FLAG_EASY_KNOW] = flagsedit.HINT_BOOL,
	[FLAG_LITE_SUN] = flagsedit.HINT_BOOL,
	[FLAG_TRAP_AWARE] = flagsedit.HINT_BOOL,
	[FLAG_WEAPON] = flagsedit.HINT_BOOL,
	[FLAG_FLOOR] = flagsedit.HINT_BOOL,
	[FLAG_WALL] = flagsedit.HINT_BOOL,
	[FLAG_PERMANENT] = flagsedit.HINT_BOOL,
	[FLAG_REMEMBER] = flagsedit.HINT_BOOL,
	[FLAG_NOTICE] = flagsedit.HINT_BOOL,
	[FLAG_DONT_NOTICE_RUNNING] = flagsedit.HINT_BOOL,
	[FLAG_CAN_RUN] = flagsedit.HINT_BOOL,
	[FLAG_DOOR] = flagsedit.HINT_BOOL,
	[FLAG_SUPPORT_LIGHT] = flagsedit.HINT_BOOL,
	[FLAG_DOOR] = flagsedit.HINT_BOOL,
	[FLAG_EMPTY] = flagsedit.HINT_BOOL,
	[FLAG_FLAT] = flagsedit.HINT_BOOL,
	[FLAG_TOWER] = flagsedit.HINT_BOOL,
	[FLAG_DOUBLE] = flagsedit.HINT_BOOL,
	[FLAG_NO_GENO] = flagsedit.HINT_BOOL,
	[FLAG_UNIQUE] = flagsedit.HINT_BOOL,
	[FLAG_QUESTOR] = flagsedit.HINT_BOOL,
	[FLAG_MALE] = flagsedit.HINT_BOOL,
	[FLAG_FEMALE] = flagsedit.HINT_BOOL,
	[FLAG_FORCE_DEPTH] = flagsedit.HINT_BOOL,
	[FLAG_FORCE_MAXHP] = flagsedit.HINT_BOOL,
	[FLAG_FORCE_SLEEP] = flagsedit.HINT_BOOL,
	[FLAG_NEVER_BLOW] = flagsedit.HINT_BOOL,
	[FLAG_NEVER_MOVE] = flagsedit.HINT_BOOL,
	[FLAG_ATTR_ANY] = flagsedit.HINT_BOOL,
	[FLAG_OPEN_DOOR] = flagsedit.HINT_BOOL,
	[FLAG_BASH_DOOR] = flagsedit.HINT_BOOL,
	[FLAG_MOVE_BODY] = flagsedit.HINT_BOOL,
	[FLAG_KILL_BODY] = flagsedit.HINT_BOOL,
	[FLAG_TAKE_ITEM] = flagsedit.HINT_BOOL,
	[FLAG_KILL_ITEM] = flagsedit.HINT_BOOL,
	[FLAG_NONLIVING] = flagsedit.HINT_BOOL,
	[FLAG_NO_DEATH] = flagsedit.HINT_BOOL,
	[FLAG_NO_TARGET] = flagsedit.HINT_BOOL,
	[FLAG_DROP_CORPSE] = flagsedit.HINT_BOOL,
	[FLAG_DROP_SKELETON] = flagsedit.HINT_BOOL,
	[FLAG_NEVER_GENE] = flagsedit.HINT_BOOL,

	[FLAG_TRAP_IDX]      = flagsedit.HINT_INT,
	[FLAG_MANA_MAX]      = flagsedit.HINT_INT,
	[FLAG_MANA_CURR]     = flagsedit.HINT_INT,
	[FLAG_SPELL_FAILURE] = flagsedit.HINT_INT,
	[FLAG_LUCK]          = flagsedit.HINT_INT,
	[FLAG_SPEED]         = flagsedit.HINT_INT,
	[FLAG_FLY]           = flagsedit.HINT_INT,
	[FLAG_AMOUNT]        = flagsedit.HINT_INT,
	[FLAG_ESP_HOSTILE]   = flagsedit.HINT_INT,
	[FLAG_ESP_NEUTRAL]   = flagsedit.HINT_INT,
	[FLAG_ESP_FRIENDLY]  = flagsedit.HINT_INT,
	[FLAG_ESP_SLEEPING]  = flagsedit.HINT_INT,
	[FLAG_ESP_AWAKE]     = flagsedit.HINT_INT,
	[FLAG_SOUND_KILL]    = flagsedit.HINT_INT,

	[FLAG_NOTE_DIES]   = flagsedit.HINT_STR,
	[FLAG_KILLED_VERB] = flagsedit.HINT_STR,

	[FLAG_DAM]          = flagsedit.HINT_ARRAY,
	[FLAG_RESIST]       = flagsedit.HINT_ARRAY,
	[FLAG_ABSORB]       = flagsedit.HINT_ARRAY,
	[FLAG_PROTECT_FROM] = flagsedit.HINT_ARRAY,
	[FLAG_IGNORE]       = flagsedit.HINT_ARRAY,
	[FLAG_HARMED_BY]    = flagsedit.HINT_ARRAY,
	[FLAG_STATS]        = flagsedit.HINT_ARRAY,
	[FLAG_SUSTAIN]      = flagsedit.HINT_ARRAY,
	[FLAG_DEVICE_USE]   = flagsedit.HINT_ARRAY,
	[FLAG_SPEEDS]       = flagsedit.HINT_ARRAY,
	[FLAG_SKILL_BONUS]  = flagsedit.HINT_ARRAY,

	[FLAG_PASS_WALL]   = flagsedit.HINT_FLAGSET,
	[FLAG_CAN_PASS]    = flagsedit.HINT_FLAGSET,
	[FLAG_OBJ_THEME]   = flagsedit.HINT_FLAGSET,
	[FLAG_LEVEL_BONUS] = flagsedit.HINT_FLAGSET,

	[FLAG_LEVELS]      = flagsedit.HINT_FUNC,
	[FLAG_CREATE_DROP] = flagsedit.HINT_FUNC,
}

function flagsedit.register_flag_hints(t)
	for k, e in t do
		flagsedit.hints[k] = e
	end
end

flagsedit.dam_list   = {}
flagsedit.speed_list = {}
flagsedit.skill_list = {}

hook(hook.INIT_GAME,
function(begin_or_end)
	if begin_or_end == "begin" then
		return
	end

	for i = 1, getn(dam.raw) do
		flagsedit.dam_list[i] = get_dam_type_info(i, "desc")
	end

	for i = 1, __s_num - 1 do
		flagsedit.skill_list[i] = s_info(i).name
	end

	for i, v in __shared_flag do
		flagsedit.hints[i] = flagsedit.HINT_ARRAY
	end

	local i = 1
	while true do
		local desc = get_speed_desc(i)

		if desc == "???" then
			break
		end

		flagsedit.speed_list[i] = desc

		i = i + 1
	end
end) -- hook(hook.INIT_GAME)

flagsedit.stat_list = {
	"STR",
	"INT",
	"WIS",
	"DEX",
	"CON",
	"CHR"
}

flagsedit.array_index_name_list = {
	[FLAG_STATS]   = flagsedit.stat_list,
	[FLAG_SUSTAIN] = flagsedit.stat_list,

	[FLAG_DAM]          = flagsedit.dam_list,
	[FLAG_RESIST]       = flagsedit.dam_list,
	[FLAG_ABSORB]       = flagsedit.dam_list,
	[FLAG_PROTECT_FROM] = flagsedit.dam_list,
	[FLAG_IGNORE]       = flagsedit.dam_list,
	[FLAG_HARMED_BY]    = flagsedit.dam_list,

	[FLAG_SPEEDS] = flagsedit.speed_list,

	[FLAG_SKILL_BONUS]  = flagsedit.skill_list,
}

----------------------------------------------------------------------

function flagsedit.add_flag(flagset, is_array, array_index_names)
	local new_flag = input_box("New flag? ", 50, "")

	if new_flag == "" then return end

	if not is_array and not globals()["FLAG_"..new_flag] then
		msg_box("Unknown flag '"..new_flag.."'")
		return
	end

	local flag

	if is_array then
		local reverse = nil

		if array_index_names then
			reverse = {}
			for k, v in array_index_names do
				reverse[v] = reverse[v] or k
			end
		end

		if reverse and reverse[new_flag] then
			flag = reverse[new_flag]
		else
			flag = tonumber(new_flag)
		end

		if not flag then
			if array_index_names then
				msg_box("No such named index")
			else
				msg_box("You must enter a number for the array's index")
			end
			return
		end
	else
		flag = globals()["FLAG_"..new_flag]
	end

	-- HACK HACK HACK: If the flag was defined in Lua rather than
	-- C, we get a Lua table with the key "value" and a value which
	-- is the flag's index.  This is a workaround until we figure out
	-- why in the world this is happening.
	if type(flag) == "table" then
		flag = flag.value
	end

	if flagset[flag] then
		msg_box("Flag already exists")
		return
	end

	local hint = flagsedit.hints[flag]

	if is_array then
		hint = nil
	end

	while not hint do
		local ret = msg_box("Add an (i)nteger, (s)tring or (f)lagset?")

		if not ret then return end

		ret = strlower(strchar(ret))

		if ret == "i" then
			hint = flagsedit.HINT_INT
			break
		elseif ret == "s" then
			hint = flagsedit.HINT_STR
			break
		elseif ret == "f" then
			hint = flagsedit.HINT_FLAGSET
			break
		end

		msg_box("Invalid selection (ESC to abort)")
	end

	if hint == flagsedit.HINT_INT then
		local new_val = tonumber(input_box("Value? ", 5, "1"))

		if not new_val then new_val = 1 end
		flagset[flag] = new_val
	elseif hint == flagsedit.HINT_BOOL then
		flagset[flag] = true
	elseif hint == flagsedit.HINT_STR then
		local new_val = input_box("String? ", 70, "")

		if not new_val then new_val = "" end
		flagset[flag] = new_val
	elseif hint == flagsedit.HINT_FLAGSET then
		flagset[flag] = flag_new()
	elseif hint == flagsedit.HINT_ARRAY then
		flagset[flag] = flag_new()
	elseif hint == flagsedit.HINT_FUNC then
		msg_box("You can't add functions using the flags editor")
	elseif hint == flagsedit.HINT_OBJ then
		msg_box("You can't add objects using the flags editor")
	elseif hint == flagsedit.HINT_MON then
		msg_box("You can't add monsters using the flags editor")
	else
		msg_box("ERROR: Invalid hint")
	end
end

function flagsedit.save_obj(obj)
	local name = object_desc(obj)

	local ret = msg_box("About to lose object '" .. name ..
						"', place it in your pack? (y/n)")

	if strlower(strchar(ret)) == "y" then
		local copy = new_object()
		object_copy(copy, obj)

		inven_carry(copy, false)
	else
		-- The object might itself have embedded objects, so recurse
		-- on it
		flagsedit.save_obj_recurse(obj.flags)
	end
end -- flagsedit.save_obj()

function flagsedit.save_obj_recurse(flagset)
	local max = flag_max_key(flagset)

	for i = 1, max do
		if tag(flagset[i]) == TAG_OBJECT then
			flagsedit.save_obj(flagset[i])
		elseif tag(flagset[i]) == TAG_FLAG then
			flagsedit.save_obj_recurse(flagset[i])
		end
	end
end -- flagsedit.save_obj_recurse()

function flagsedit.del_flag(flags, key)
	local ret = msg_box("Really delete '"..(find_flag(key).name).."' (y/n)?")

	if strlower(strchar(ret)) ~= "y" then return end

	-- Ask user if any objects deleted should be placed in the player's pack
	if tag(flags[key]) == TAG_OBJECT then
		flagsedit.save_obj(flags[key])
	elseif tag(flags[key]) == TAG_FLAG then
		flagsedit.save_obj_recurse(flags[key])
	end

	flag_remove(flags, key)
end

function flagsedit.display_flag(flags, key, name, sel_val, is_array)
	local ret, wid, hgt = term.get_size()

	draw_box(0, (wid / 2) + 1, hgt - 1, (wid / 2) - 2)
	term.print(color.LIGHT_BLUE, name, 0, (wid / 2) + 2)

	local x, y = (wid / 2) + 2, 1

	local nb = 1

	local color_mode = color.WHITE

	local node = flag_get_node(flags, key)
	if not node then
		term.print(color.VIOLET, "ERROR: Flag does not exist!!",
					y + 1, x)
		return
	end	

	local fflags = node.flags
	if fflags & FLAG_FLAG_KNOWN ~= 0 then
		term.print(color.LIGHT_GREEN, "Flag is known", y, x)
	else
		term.print(color.LIGHT_RED, "Flag is not known", y, x)
	end

	y = y + 1

	local random_node = "Node is a randomization node (shouldn't see this)"
	if fflags & FLAG_FLAG_FLAGS ~= 0 then
		if flagsedit.hints[key] == flagsedit.HINT_ARRAY then
			term.print(color_mode, "Val is an array", y, x)
		else
			term.print(color_mode, "Val is a flagset", y, x)
		end
	elseif fflags & FLAG_FLAG_RAND ~= 0 then
		term.print(color_mode, random_node, y, x)
	elseif fflags & FLAG_FLAG_MBONUS ~= 0 then
		term.print(color_mode, random_node, y, x)
	elseif fflags & FLAG_FLAG_RAND_MBONUS ~= 0 then
		term.print(col_mode, random_node, y, x)
	elseif fflags & FLAG_FLAG_PTR ~= 0 then
		local typ = tag(flags[key])

		if typ == TAG_MONSTER then
			term.print(color_mode, "Val is a monster", y, x)
		elseif typ == TAG_OBJECT then
			term.print(color_mode, "Val is an object", y, x)
		elseif typ == TAG_STRING then
			term.print(color_mode, "Val: '" .. flags[key] .. "'",
					   y, x)
		else
			term.print(color_mode, "Val is an unknown type of pointer", y, x)
			term.print(color_mode, "lua type is " .. type(flags[key]), y +1, x)
		end
	else
		if flagsedit.hints[key] == flagsedit.HINT_BOOL and
			not is_array then
			term.print(color_mode, "Val: True", y, x)
		elseif flagsedit.hints[key] == flagsedit.HINT_FUNC and
			not is_array then
			term.print(color_mode, "Flag is a callable Lua function", y, x)
		else
			term.print(color_mode, "Val:", y, x)
			term.print(flag_get(flags, key), y, x + 6)

			if fflags & FLAG_FLAG_BOTH ~= 0 then
				term.print(color_mode, "Val2:", y + 1, x)
				term.print(flag_get2(flags, key), y + 1, x + 6)
				nb = 2
			end
		end
	end
	return nb
end -- flagsedit.display_flag()

-- If the flagset is being used as an array, then the key is the index
-- that key's value has in the array
function flagsedit.get_list(flagset, is_array, array_index_names)
	local flags   = {}
	local reverse = {}
	local j       = 1

	for i = 1, flagset.size do
		if flagset.node[i].flags & FLAG_FLAG_USED ~= 0 then
			local key = flagset.node[i].key
			if is_array then
				if array_index_names and array_index_names[key] then
					flags[j] = array_index_names[key]
				else
					flags[j] = format("%d", key)
				end
			else
				flags[j] = get_flag_name(key)
			end

			reverse[flags[j]] = reverse[flags[j]] or key
			j = j + 1
		end
	end

	if is_array and array_index_names then
		-- Leave them as is
	elseif is_array then
		local func = function(a, b)
						 return (tonumber(a) < tonumber(b))
					 end
		sort(flags, func)
	else
		sort(flags)
	end

	return flags, reverse
end -- flagsedit.get_list()

function flagsedit.edit_flag(flagset, key, currname, flagset_is_array)
	local fflags = flag_get_node(flagset, key).flags

	-- Save "known" status of flag, and restore it before leaving,
	-- since changing the value of a flag unsets the known status
	local known = flag_is_known(flagset, key)

	if fflags & FLAG_FLAG_FLAGS ~= 0 then
		-- Recursively edit this flagset
		local flagname = get_flag_name(key)
		local is_array = ((flagsedit.hints[key] == flagsedit.HINT_ARRAY) and
						  not flagset_is_array)

		local names

		if flagset_is_array then
			flagname = "[" .. key .. "]"
		else
			names = flagsedit.array_index_name_list[key]
		end

		flagsedit.edit_flagset(flagset[key], currname .. ":" .. flagname,
							   is_array, names)
	elseif fflags & FLAG_FLAG_PTR ~= 0 then
		local typ = tag(flagset[key])

		if typ == TAG_MONSTER then
			local mon = flagset[key]
			flagsedit.edit_flagset(mon.flags, monster_desc(mon, 128))
		elseif typ == TAG_OBJECT then
			local obj     = flagset[key]
			local objname = object_desc(obj)
			flagsedit.edit_flagset(obj.flags, currname .. ":" .. objname)
		elseif typ == TAG_STRING then
			local old_val = flagset[key]
			local new_val = input_box("New string? ", 70, old_val)

			if new_val then
				flagset[key] = new_val
			end
		else
			msg_box("Can't edit unknown pointer type")
		end
	else
		if not flagset_is_array then
			-- If the flagset is being used by array, the keys are indexes
			-- rathe into the array rather than flag indexes, so using
			-- hints would be nonsensical.
			local hint = flagsedit.hints[key]
			if hint == flagsedit.HINT_FUNC then
				msg_box("You can't edit function flags, only delete them")
				return
			elseif hint == flagsedit.HINT_BOOL then
				local ret = msg_box("Set flag to False by removing it? (y/n)")

				ret = strchar(ret)

				if ret == "y" or ret == "Y" then
					flag_remove(flagset, key)
				end
				return
			end
		end -- if not flagset_is_array then

		local old_val = tostring(flag_get(flagset, key))
		local new_val = input_box("New value? ", 6, old_val)

		new_val = tonumber(new_val)

		if new_val then
			if fflags & FLAG_FLAG_BOTH ~= 0 then
				local new_val2

				old_val  = flag_get2(flagset, key)
				new_val2 = input_box("New value2? ", 6, old_val)

				if new_val2 then
					flag_set_full(flagset, key, fflags, new_val, new_val2,
								  nil)
				end
			else
				flag_set(flagset, key, new_val)
			end
		end
	end

	flag_learn(flagset, key, known)
end -- flagsedit.edit_flag()

function flagsedit.edit_flagset(flagset, name, is_array, array_index_names)
	term.clear()

	-- Get the flags
	local flags, reverse = flagsedit.get_list(flagset, is_array,
											  array_index_names)
	-- Now the display
	local ret, wid, hgt = term.get_size()
	local begin, sel = 1, 1
	local sel_val = 1

	-- Compound name is too long to display, show just the last part
	if strlen(name) > ((wid / 2) - 2) then
		local parts = strsplit(name, ":")
		name = parts[getn(parts)]
	end

	while not nil do
		local flag_key = nil

		-- Display the flags
		display_list(0, 0, hgt - 1, wid / 2, name, flags, begin, sel,
					 color.LIGHT_BLUE)

		-- Flagset might be empty
		if getn(flags) > 0 then
			-- Display selected flag
			flag_key = reverse[flags[sel]]
			flagsedit.display_flag(flagset, flag_key, flags[sel],
								   sel_val, is_array)
		end

		local key = term.inkey()
		local c   = strchar(key)

		if key == ESCAPE then
			break
		elseif c == "8" then
			-- Up key
			sel = sel - 1
			if sel < 1 then sel = 1 end
			if sel < begin then
				begin = begin - 1
			end
			sel_val = 1
		elseif c == "2" then
			-- Down key
			sel = sel + 1
			if sel > getn(flags) then sel = getn(flags) end
			if sel >= begin + hgt - 2 then
				begin = begin + 1
			end
			sel_val = 1
		elseif c == "a" then
			-- Add new flag
				flagsedit.add_flag(flagset, is_array, array_index_names)
		elseif c == "d" or key == 8 then
			-- Delete flag
			flagsedit.del_flag(flagset, reverse[flags[sel]])
		elseif c == "k" then
			-- Toggle known status of flag
			if flag_is_known(flagset, reverse[flags[sel]]) then
				flag_learn(flagset, reverse[flags[sel]], false)
			else
				flag_learn(flagset, reverse[flags[sel]], true)
			end
		elseif c == "\r" then
			flagsedit.edit_flag(flagset, reverse[flags[sel]], name,
								is_array)
		elseif c == "?" then
			show_file("flag-help.txt", 0, 0, 0)
		else
			if not flagsedit.mistake_made then
				flagsedit.mistake_made = true
				msg_box("Press '?' for help")
			end
		end
		flags, reverse = flagsedit.get_list(flagset, is_array,
											array_index_names)
		if sel > getn(flags) then sel = sel - 1 end
		if sel < 1 then sel = 1 end
	end  -- while not nil do
end -- flagsedit.edit_flagset()

function flagsedit.run_object()
	local ret, item = get_item("Play with what?", "You have nothing to play with", USE_INVEN | USE_EQUIP | USE_FLOOR, function() return true end)

	if ret == FALSE or not item then
		return
	end

	term.save()
	term.clear()


	local obj = get_object(item)
	local o_name = object_desc(obj, 0, 0)

	flagsedit.edit_flagset(obj.flags, o_name)

	-- Recalculate the player's bonuses after having changed the
	-- object's flags
	player.calc_bonuses()

	term.load()
end -- flagsedit.run_object()

function flagsedit.run_monster()
	local ret, x, y = tgt_pt()

	if cave(y, x).m_idx > 0 then
		local m_ptr = monster(cave(y, x).m_idx)

		term.save()
		term.clear()

		flagsedit.edit_flagset(m_ptr.flags, monster_desc(m_ptr, 128))

	term.load()
	end
end -- flagsedit.run_monster()

function flagsedit.run_grid()
	local ret, x, y = tgt_pt()

	term.save()
	term.clear()

	flagsedit.edit_flagset(cave(y, x).flags, "Grid")

	term.load()
end -- flagsedit.run_grid()

function flagsedit.run_level()
	term.save()
	term.clear()

	flagsedit.edit_flagset(level_flags.flags, "Level")

	term.load()
end -- flagsedit.run_level()

function flagsedit.run_dungeon()
	term.save()
	term.clear()

	flagsedit.edit_flagset(d_info[current_dungeon_idx + 1].flags, "Dungeon")

	term.load()
end -- flagsedit.run_level()

hook {
	[hook.DEBUG_COMMAND] = function (d)
		if d == KTRL('O') then
			flagsedit.run_object()
			return true
		elseif d == KTRL('N') then
			local ret, key =
				get_com("Edit flags of [m]onster, [g]rid, [l]evel, [d]ungeon?")

			if not ret then
				return
			end

			key = strlower(strchar(key))

			if key == 'm' then
				flagsedit.run_monster()
			elseif key == 'g' then
				flagsedit.run_grid()
			elseif key == 'l' then
				flagsedit.run_level()
			elseif key == 'd' then
				flagsedit.run_dungeon()
			end

 			return true
		end
		return FALSE
	end
}
