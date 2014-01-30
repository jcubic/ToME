-- SYSTEM FILE
--
-- Lua wizard functions

constant("wizcmd", {})

wizcmd.CTRL_F = 6

hook(hook.DEBUG_COMMAND,
     function(cmd)
	     if strchar(cmd) == 'F' then
		     wizcmd.change_feature()
		     return true
	     elseif strchar(cmd) == 'R' then
		     wizcmd.create_trap()
		     return true
		 elseif strchar(cmd) == 'n' then
			 wizcmd.summon_named_monst(false)
			 return true
		 elseif strchar(cmd) == 'N' then
			 wizcmd.summon_named_monst(true)
			 return true
		 elseif strchar(cmd) == 'b' then
			 wizcmd.teleport_to()
			 return true
		 elseif strchar(cmd) == 'T' then
			 wizcmd.town_teleport()
			 return true
		 elseif strchar(cmd) == 'D' then
			 wizcmd.dungeon_teleport()
			 return true
		 elseif strchar(cmd) == 'D' then
			 wizcmd.dungeon_teleport()
			 return true
		 elseif strchar(cmd) == 'Z' then
			 wizcmd.zap_all()
			 return true
		 elseif strchar(cmd) == 'B' then
			 wizcmd.wiz_body()
			 return true
		 elseif strchar(cmd) == ';' then
			 wizcmd.find_dead_monsters()
			 return true
		 elseif cmd == wizcmd.CTRL_F then
			 wizcmd.forget_object()
			 return true
	     end

	     return false
     end
     )

function wizcmd.find_str_in_arr(array, max_idx, field, str)
	local out = {}

	for i = 1, max_idx do
		if strfind(strlower(array[i][field]), str) then
			tinsert(out, i)
		else
		end
	end

	return out
end -- wizcmd.find_str_in_arr()

-- Change the cave feature where the player is currently standing
function wizcmd.change_feature()
	local feature = get_string("Which feature? ")

	if not feature or feature == "" then
		return
	end

	feature = strlower(feature)
	feature = clean_whitespace(feature)

	if feature == "" then
		return
	end

	local feat_idx = nil
	if tonumber(feature) and tonumber(feature) > 0 then
		feat_idx = tonumber(feature)
	else
		for i = 1, max_f_idx do
			if strlower(f_info[i].name) == feature then
				feat_idx = i - 1
				break
			end
		end -- for i = 1, max_f_idx do
		if not feat_idx then
			-- Look for features which contain the request string as
			-- a substring.
			local possible = wizcmd.find_str_in_arr(f_info, max_f_idx,
													"name", feature)

			if getn(possible) == 1 then
				feat_idx = possible[1] - 1
			elseif getn(possible) > 1 then
				local names = {}
				for i = 1, getn(possible) do
					tinsert(names, f_info[possible[i]].name)
				end
				local choice
				choice = choose_from_list_simple(names,
												 "Choose feature")
				if not choice then
					return
				end
				feat_idx = possible[choice] - 1
			end
		end -- if not feat_idx then
	end

	if not feat_idx then
		message("No such feature as '" .. feature .. "'")
		return
	end

	cave_set_feat(player.py, player.px, feat_idx)
end -- wizcmd.change_feature()

-- Create a named trap where the player is currently standing
function wizcmd.create_trap()
	local i
	local wanted = get_string("Which trap? ")

	if not wanted then
		return
	end

	wanted = strlower(wanted)
	wanted = clean_whitespace(wanted)

	local trap_idx = traps.traps_by_name[wanted]

	if not trap_idx then
		trap_idx = traps.traps_by_name[wanted .. " trap"]
	end

	if not trap_idx then
		trap_idx = traps.traps_by_name["trap of " .. wanted]
	end

	if not trap_idx then
		message("No such trap as '" .. wanted .. "'")
		return
	end

	local level_str = get_string("What level (defaults to dun_level)? ")
	local level

	if not level_str or strlen(level_str) == 0 then
		level = dun_level
	else
		level_str = clean_whitespace(level_str)
		level = tonumber(level_str)

		-- Is it a number?
		if not level then
			message("Invalid level '" .. level_str .. "': not a number")
			return
		end

		if level <= 0 then
			message("WARNING: Creating trap with non-positive level")
		end
	end

	local faction_str = get_string("Which faction (defaults to dungeon)? ")
	local faction

	if not faction_str or strlen(faction_str) == 0 then
		faction = FACTION_DUNGEON
	else
		faction_str = clean_whitespace(faction_str)
		faction_str = strlower(faction_str)

		if strfind(faction_str, "dungeon") or faction_str == "d" then
			faction = FACTION_DUNGEON
		elseif strfind(faction_str, "town") or faction_str == "t" then
			faction = FACTION_TOWN
		elseif strfind(faction_str, "player")
			or faction_str == "p" then
			faction = FACTION_PLAYER
		else
			message("Unknown faction '" .. faction_str .. "'")
			return
		end
	end

	local trap_info = traps.trap_list[trap_idx]

	local trap = traps.make_trap(trap_info, faction)
	flag_set(trap, FLAG_LEVEL, level)
	traps.place_location_trap(trap, player.py, player.px)
end -- wizcmd.change_feature()

function wizcmd.choose_monster(monster)
	monster = strlower(clean_whitespace(monster))

	local orig_str = monster

	if monster == "" then
		return nil
	end

	-- Suppress error messages first time around, in case
	-- we can search by substring to find the monster the
	-- user was looking for.
	monster = parse_named_monster(monster, true)

	if not monster then
		-- Look for races which contain the request string as
		-- a substring.
		local possible = wizcmd.find_str_in_arr(r_info, max_r_idx,
												"name", orig_str)

		if getn(possible) == 0 then
			-- Give error message
			parse_named_monster(orig_str)
			return
		elseif getn(possible) == 1 then
			monster = r_info[possible[1]].name
		elseif getn(possible) > 1 then
			local names = {}
			for i = 1, getn(possible) do
				tinsert(names, r_info[possible[i]].name)
			end
			local choice
			choice = choose_from_list_simple(names,
											 "Choose monster")
			if not choice then
				return
			end
			monster = r_info[possible[choice]].name
		end

		monster = parse_named_monster(monster)

		if not monster then
			return nil
		end
	end -- if not monster then
	return monster
end

function wizcmd.summon_named_monst(friendly)
	local monst

	if command_arg > 0 then
		monst =
		{
			race_idx = command_arg
			ego_idx = 0
			faction = iif(friendly, FACTION_PLAYER, FACTION_DUNGEON)
		}
	else
		monst = get_string("Which monster? ")

		if not monst then
			return
		end

		monst = wizcmd.choose_monster(monst)

		if not monst then
			return
		end
		monst.ego_idx = monst.ego_idx or 0
	end

	local old_val = m_allow_special[monst.race_idx + 1]
	m_allow_special[monst.race_idx + 1] = true

	local group = false
	if r_info(monst.race_idx).flags[FLAG_ESCORT] or
		(monst.ego_idx ~= 0 and re_info(monst.ego_idx).flags[FLAG_ESCORT])
	then
		local ret, key =
			get_com("Place monster's escort as well? (y/n)")

		if ret then
			key = strlower(strchar(key))
			if key == "y" then
				group = true
			end
		end
	end

	if friendly then
		monst.faction = FACTION_PLAYER
	end

	local m_idx = place_named_monster_near(player.py, player.px,
										   monst, false, group)

	m_allow_special[monst.race_idx + 1] = old_val

end -- wizcmd.summon_named_monst()

function wizcmd.town_teleport()
	local ret, wid, hgt = term.get_size()
	local begin, sel = 1, 1
	local sel_val = 1

	term.save()
	term.clear()

	local key, c

	local towns = {}

	for name, loc in map.town_coord do
		tinsert(towns, name)
	end

	sort(towns)

	local long_towns = {}

	for i = 1, getn(towns) do
		long_towns[i] = towns[i] .. " (" .. map.town_long_name[towns[i]] .. ")"
	end

	while true do
		-- Display the flags
		display_list(0, 0, hgt - 1, wid - 1, "Teleport to which town?",
					 long_towns, begin, sel, color.LIGHT_BLUE)

		key = term.inkey()
		c   = strchar(key)

		if key == ESCAPE then
			break
		elseif c == "\r" then
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
			if sel > getn(towns) then sel = getn(towns) end
			if sel >= begin + hgt - 2 then
				begin = begin + 1
			end
			sel_val = 1
		end
	end

	if key == ESCAPE then
		term.load()
		return
	end

	local town = towns[sel]

	term.load()

	dun_level = 0

	player.wilderness_y = map.town_coord[town].y
	player.wilderness_x = map.town_coord[town].x

	player.inside_arena = 0
	leaving_quest       = player.inside_quest
	player.inside_quest = 0
	player.leaving      = true
end -- wizcmd.town_teleport()

function wizcmd.dungeon_teleport()
	local ret, wid, hgt = term.get_size()
	local begin, sel = 1, 1
	local sel_val = 1

	term.save()
	term.clear()

	local key, c

	local dun_names = {}
	local dun_idxs  = {}

	for i = 1, __d_num do
		if d_info[i].name and d_info[i].name ~= "" then
			tinsert(dun_names, d_info[i].name)
			dun_idxs[d_info[i].name] = i - 1
		end
	end

	if getn(dun_names) == 0 then
		message("WARNING: No dungeons to teleport to!!")
		return
	end

	sort(dun_names)

	while true do
		-- Display the flags
		display_list(0, 0, hgt - 1, wid - 1, "Teleport to which dungeon?",
					 dun_names, begin, sel, color.LIGHT_BLUE)

		key = term.inkey()
		c   = strchar(key)

		if key == ESCAPE then
			break
		elseif c == "\r" then
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
			if sel > getn(dun_names) then sel = getn(dun_names) end
			if sel >= begin + hgt - 2 then
				begin = begin + 1
			end
			sel_val = 1
		end
	end

	if key == ESCAPE then
		term.load()
		return
	end

	local dun_idx = dun_idxs[dun_names[sel]]

	term.load()

	dun_level = d_info[dun_idx + 1].mindepth

	player.inside_arena = 0
	leaving_quest       = player.inside_quest
	player.inside_quest = 0
	player.leaving      = true

	current_dungeon_idx = dun_idx
end -- wizcmd.dungeon_teleport()

function wizcmd.forget_object()
	-- Get an item
	local ret, item = get_item("Forget which object?",
							   "You have objects to forget.",
							   USE_INVEN | USE_FLOOR | USE_EQUIP,
							   function(o)
								   return true
							   end)
	if not ret or not item then return nil end

	local obj = get_object(item)

	obj.ident                   = 0
	k_info[obj.k_idx + 1].aware = 0

	for i = 1, flag_max_key(obj.flags) do
		if obj.flags[i] then
			flag_learn(obj.flags, i, false)
		end
	end
end -- wizcmd.forget_object()

function wizcmd.teleport_to()
	local ret, x, y = tgt_pt()

	if not ret then
		return
	end

	-- Move
	local oy, ox = player.py, player.px

    player.py = y
    player.px = x

    -- Redraw old spot
    lite_spot(oy, ox)

    -- Redraw new spot
    lite_spot(y, x)

    -- Check for new panel
    verify_panel()

	if player.wild_mode then
		reveal_wilderness_around_player(player.py, player.px, 0, 3)
	end

    -- Update stuff
    player.update = (player.update | PU_VIEW | PU_FLOW | PU_MON_LITE |
                     PU_DISTANCE)
    player.window[FLAG_PW_OVERHEAD] = true
    handle_stuff()
end -- wizcmd.teleport_to()

-- Zap all monsters on level.
function wizcmd.zap_all()
	foreach_flags(monst_list,
		function(list, m_idx)
			local monst = list[m_idx]

			if monst.r_idx ~= 0 then
				delete_monster_idx(m_idx)
			end
		end)
end -- wizcmd.zap_all()

function wizcmd.wiz_body()
	local monster
	if command_arg > 0 then
		monster =
		{
			race_idx = command_arg
			ego_idx = 0
			faction = iif(friendly, FACTION_PLAYER, FACTION_DUNGEON)
		}
	else
		monster = get_string("Change body to which monster? ")

		if not monster then
			return
		end

		monster = wizcmd.choose_monster(monster)

		if not monster then
			return
		end
	end

	local body = monster_type_new()
	monster_prep(body, monster.race_idx, monster.ego_idx or 0)
	change_player_body(body)
	delete_monster_type(body, true)
end

function wizcmd.find_dead_monsters()
	for y = 1, cur_hgt - 2 do
		for x = 1, cur_wid - 2 do
			local c = cave(y, x)
			if c.m_idx > 0 then
				local monst = monster(c.m_idx)
				if monst.r_idx == 0 then
					print("At "..y.."x"..x.." monster "..c.m_idx.." is dead.")
				end
			end
		end
	end
end
