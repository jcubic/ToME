----------------------------------------------------------------------
-- Display current knowledge menu
----------------------------------------------------------------------

constant("curr_know", {})

curr_know.menu_list = {}
curr_know.menu_chars = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'}
curr_know.char_to_index = {}

do
	for i = 1, getn(curr_know.menu_chars) do
		local char = strlower(curr_know.menu_chars[i])
		curr_know.char_to_index[char] = i
	end
end

function curr_know.add_menu_items(items)
	if type(items[1]) == "table" then
		for i = 1, getn(items) do
			tinsert(curr_know.menu_list, items[i])
		end
	else
		tinsert(curr_know.menu_list, items)
	end
end

function curr_know.show_menu()
	if getn(curr_know.menu_list) > getn(curr_know.menu_chars) then
		error("Too many types of knowledge to display")
	end

	term.save()

	while true do
		term.clear()

		--Ask for a choice
		term.prt("Display current knowledge", 2, 0)

		for i = 1, getn(curr_know.menu_list) do
			local char = curr_know.menu_chars[i]
			local desc = curr_know.menu_list[i].desc

			term.prt("(" .. char .. ") " .. desc, 3 + i, 5)
		end

		term.prt("Command (ESC to quit): ", 17, 0)

		local key  = term.inkey()
		local char = strlower(strchar(key))

		if key == ESCAPE then
			break
		end

		local choice = curr_know.char_to_index[char]

		if choice and curr_know.menu_list[choice] then
			local title = curr_know.menu_list[choice].title

			local to_show

			if curr_know.menu_list[choice].pass_as_func then
				to_show = curr_know.menu_list[choice].func
			else
				to_show = curr_know.menu_list[choice].func()
			end


			if to_show then
				show_arr_as_list(to_show, title, true)
			else
				message(color.VIOLET,
						"ERROR: Nothing to show")
			end
		end
	end

	term.load()
end -- curr_know.show_menu()

----------------------------------------------------------------------------
-- Artifacts
----------------------------------------------------------------------------
function curr_know.show_artifacts()
	local lines = {}

	local arts = {}

	for i = 1, max_k_idx - 1 do
		local obj = k_info(i)

		if has_flag(obj, FLAG_NORM_ART) and not
			has_flag(obj, FLAG_INSTA_ART) and obj.aware then
			local name = obj.name
			name = "the " .. clean_whitespace(gsub(name, "[&~]", ""))

			local char = strchar(obj.x_char)

			if char == "#" then
				char = "##"
			end

			tinsert(arts,
					{
						tval = obj.tval,
						name = name,
						char = char,
						color = format("#0x%02x", obj.x_attr)
					})
		end
	end

	for i = 1, max_a_idx - 1 do
		local art = a_info(i)

		if art.known then
			local k_idx = lookup_kind(art.tval, art.sval)
			local obj   = k_info(k_idx)

			local name = art.name
			name = "the " .. clean_whitespace(gsub(name, "[&~]", ""))

			local char = strchar(obj.x_char)

			if char == "#" then
				char = "##"
			end

			tinsert(arts,
					{
						tval = art.tval,
						name = name,
						char = char,
						color = format("#0x%02x", art.x_attr)
					})
		end
	end

	sort(arts,
		 function(a, b)
			 if a.tval == b.tval then
				 return a.name < b.name
			 else
				 return a.tval < b.tval
			 end
		 end)

	for i = 1, getn(arts) do
		local name = arts[i].name

		if game_options.items_capitalize == game_options.CAPITALIZE_ALL or
			game_options.items_capitalize == game_options.CAPITALIZE_ARTS
		then
			name = book_capitals(name)
		end

		tinsert(lines, "    " .. arts[i].color .. arts[i].char .. "#w " ..
				name)
	end

	return lines
end --curr_know.show_artifacts()

----------------------------------------------------------------------------
-- Uniques.  Modules can supply subsystem parameter "unique_ignore" (a
-- function) to make certain uniques not be ignore, or parameters
-- "sort_uniques_by_level", "sort_uniques_by_name" or
-- "sort_uniques_by_symbol" (also functions) if they want to tweak
-- how uniques are sorted.
----------------------------------------------------------------------------
curr_know.unique_ignore =
	get_subsystem_param("curr_know", "unique_ignore") or
	function(monst, r_idx)
		return false
	end

function curr_know.show_uniques_aux(sort_func)
	local lines = {}

	local alive = {}
	local dead  = {}

	for r_idx = 1, max_r_idx - 1 do
		local monst = r_info(r_idx)

		if has_flag(monst, FLAG_UNIQUE) and not
			curr_know.unique_ignore(monst, r_idx)
		then
			local is_dead  = (monst.max_num == 0)
			local known = (is_dead or monst.r_sights > 0 or
						   game_options.cheat_know)

			if known then
				local char = strchar(monst.d_char)

				if char == "#" then
					char = "##"
				end

				local entry = {
					r_idx = r_idx,
					monst = monst,
					char  = char,
					color = format("#0x%02x", monst.d_attr)
				}

				if is_dead then
					tinsert(dead, entry)
				else
					tinsert(alive, entry)
				end
			end
		end -- if has_flag(monst, FLAG_UNIQUE)
	end -- for r_idx = 1, max_r_idx - 1 do

	alive = sort_func(alive)
	dead  = sort_func(dead)

	tinsert(lines, "*** #BAlive#w ***")

	if getn(alive) == 0 then
		tinsert(lines, "    No (known) uniques are left alive.")
	else
		for i = 1, getn(alive) do
			local entry = alive[i]

			local kills     = entry.monst.r_pkills
			local kills_str = ""

			if kills > 0 then
				local times

				if kills == 1 then
					times = "once"
				else
					times = kills .. " times"
				end

				kills_str = " (killed " .. times .. ", but still alive)"
			end

			tinsert(lines, "    " .. entry.color .. entry.char .. "#w " ..
					book_capitals(entry.monst.name) .. kills_str)
		end
	end

	tinsert(lines, "")

	tinsert(lines, "*** #RDead#w ***")

	if getn(dead) == 0 then
		tinsert(lines, "    No uniques are dead yet.")
	else
		for i = 1, getn(dead) do
			local entry = dead[i]

			local kills     = entry.monst.r_pkills
			local kills_str = ""

			if kills > 1 then
				local times

				kills_str = " (killed " .. kills .. " times)"
			end

			tinsert(lines, "    " .. entry.color .. entry.char .. "#w " ..
					book_capitals(entry.monst.name) .. kills_str)
		end
	end

	return lines
end

function curr_know.show_uniques(state, char, key, begin, arr)
	if not state then
		state = {sort = "level"}
		char  = "l"
	end

	char = strlower(char)

	local info = {}
	local func

	if char == "l" then
		func               = curr_know.sort_uniques_by_level
		state.sort         = "level"
		info.footer        = "'n' to sort by name, 's' to sort by symbol"
		info.to_func_chars = {s = true, S = true, n = true, N = true}
	elseif char == "s" then
		func               = curr_know.sort_uniques_by_symbol
		state.sort         = "symbol"
		info.footer        = "'l' to sort by level, 'n' to sort by name"
		info.to_func_chars = {l = true, L = true, n = true, N = true}
	elseif char == "n" then
		func               = curr_know.sort_uniques_by_name
		state.sort         = "name"
		info.footer        = "'l' to sort by level, 's' to sort by symbol"
		info.to_func_chars = {l = true, L = true, s = true, S = true}
	end

	-- Sorting has changed, srart over at being of list
	info.begin = 1

	return curr_know.show_uniques_aux(func), info, state
end -- curr_know.show_uniques()

curr_know.sort_uniques_by_level =
	get_subsystem_param("curr_know", "sort_uniques_by_level") or
	function(list)
		sort(list,
			 function(a, b)
				 if a.monst.level == b.monst.level then
					 return strlower(a.monst.name) <
						 strlower(b.monst.name)
				 else
					 return a.monst.level < a.monst.level
				 end
			 end)

		return list
	end -- curr_know.sort_uniques_by_level()

curr_know.sort_uniques_by_name =
	get_subsystem_param("curr_know", "sort_uniques_by_name") or
	function(list)
		sort(list,
			 function(a, b)
				 return strlower(a.monst.name) <
					 strlower(b.monst.name)
			 end)

		return list
	end -- curr_know.sort_uniques_by_name()

curr_know.sort_uniques_by_symbol =
	get_subsystem_param("curr_know", "sort_uniques_by_symbol") or
	function(list)
		-- Sort list by monster character, or by name if the characters
		-- are the same.
		sort(list,
			 function(a, b)
				 if a.monst.d_char == b.monst.d_char then
					 return strlower(a.monst.name) <
						 strlower(b.monst.name)
				 else
					 return a.monst.d_char < b.monst.d_char
				 end
			 end)

		return list
	end -- curr_know.sort_uniques_by_symbol()

-------------------------------------------------------------------------
-- Objects (other than artifacts)
-------------------------------------------------------------------------
function curr_know.show_objects()
	local lines      = {}
	local objs       = {}
	local tval_chars = {}

	for i = 1, max_k_idx - 1 do
		local obj = new_object()
		object_prep(obj, i)
		
		tval_chars[obj.tval] = strchar(k_info(i).x_char)

		if is_aware(obj) and flavour.get_flavour(i) and not
			(has_flag(obj, FLAG_NORM_ART) or has_flag(obj, FLAG_INSTA_ART) or
			 has_flag(obj, FLAG_EASY_KNOW) or is_multiflav_tval(obj.tval))
		then
			local name

			name = object_desc(obj)

			local char = strchar(k_info(i).x_char)

			if char == "#" then
				char = "##"
			end

			tinsert(objs,
					{
						tval = obj.tval,
						name = name,
						char = char,
						color = format("#0x%02x", obj.x_attr)
					})
		end
		delete_object(obj)
	end -- for i = 1, max_k_idx - 1 do

	local multiflav_tvals = get_multiflav_tvals()
	for i = 1, getn(multiflav_tvals) do
		local tval     = multiflav_tvals[i]
		local flav_ids = get_aware_multiflav_ids(tval)

		for j = 1, getn(flav_ids) do
			local id   = flav_ids[j]
			local name = multiflav_id_to_name(tval, id)
			local char = tval_chars[tval]

			local flavour, color = flavour.get(tval, id)

			if char == "#" then
				char = "##"
			end

			tinsert(objs,
					{
						tval = tval,
						name = name,
						char = char,
						color = format("#0x%02x", color)
					})
		end -- for j = 1, getn(flav_ids) do
	end -- for i = 1, getn(multiflav_tvals) do

	sort(objs,
		 function(a, b)
			 if a.tval == b.tval then
				 return a.name < b.name
			 else
				 return a.tval < b.tval
			 end
		 end)

	for i = 1, getn(objs) do
		tinsert(lines, "    " .. objs[i].color .. objs[i].char .. "#w " ..
				objs[i].name)
	end

	return lines
end --curr_know.show_objects()

----------------------------------------------------------------------------
-- Kill count
----------------------------------------------------------------------------
curr_know.symbol_to_kill_type =
	get_subsystem_param("curr_know", "symbol_to_kill_type") or
	function(symbol)
		return get_symbol_info(symbol)
	end

function curr_know.show_kills_aux()
	local kills = {}

	local total = 0

	for r_idx = 1, max_r_idx - 1 do
		local monst = r_info(r_idx)

		if monst.r_pkills > 0 then
			local unique = has_flag(monst, FLAG_UNIQUE)
			local char   = strchar(monst.d_char)
			local esc_char

			-- Do we need to escape this character?
			if char == "#" then
				esc_char = "##"
			else
				esc_char = char
			end

			local entry = {
				r_idx    = r_idx,
				monst    = monst,
				unique   = unique,
				count    = monst.r_pkills,
				char     = char,
				esc_char = esc_char
				color    = format("#0x%02x", monst.d_attr)
			}

			local name = monst.name

			if monst.r_pkills > 1 and not unique then
				name = pluralize(name)
			end

			if unique then
				name = book_capitals(name)
			else
				name = strlower(name)
			end

			entry.name = name

			if not unique or monst.max_num == 0 then
				tinsert(kills, entry)

				if unique then
					total = total + 1
				else
					total = total + monst.r_pkills
				end
			end
		end -- if monst.r_pkills > 0 then
	end -- for r_idx = 1, max_r_idx - 1 do

	return kills, total
end -- curr_know.show_kils_aux()

function curr_know.show_kills(state, char, key, begin, arr)
	local list, total

	if not state then
		state = {sort = "type"}
		char  = "t"

		list, total = curr_know.show_kills_aux()

		state.list  = list
		state.total = total
	else
		list  = state.list
		total = state.total
	end

	char = strlower(char)

	local info = {}
	local func

	if char == "t" then
		func               = curr_know.sort_kills_by_type
		state.sort         = "type"
		info.footer        = "'n' to sort by name, 'l' to sort by level"
		info.to_func_chars = {n = true, N = true, l = true, L = true}
	elseif char == "l" then
		func               = curr_know.sort_kills_by_level
		state.sort         = "level"
		info.footer        = "'n' to sort by name, 't' to sort by type"
		info.to_func_chars = {n = true, N = true, t = true, T = true}
	elseif char == "n" then
		func               = curr_know.sort_kills_by_name
		state.sort         = "name"
		info.footer        = "'l' to sort by level, 't' to sort by type"
		info.to_func_chars = {l = true, L = true, t = true, T = true}
	end

	sort(list, func)

	local arr = {}

	if total == 0 then
		tinsert(arr, "You have defeated no enemies yet.")
	elseif total == 1 then
		tinsert(arr, "You have defeated one enemy.")
	else
		tinsert(arr, "You have defeated " .. total .. " enemies.")
	end

	tinsert(arr, "")

	if state.sort == "type" then
		local prev_type = ""

		for i = 1, getn(list) do
			local entry = list[i]

			if entry.char ~= prev_type then
				if prev_type ~= "" then
					tinsert(arr, "")
				end

				local type_str = curr_know.symbol_to_kill_type(entry.char)
				type_str = curr_know.pluralize_kill_type(type_str)
				type_str = type_str .. ' ("#y' .. entry.esc_char .. '#w")'
				tinsert(arr, type_str)

				prev_type = entry.char
			end

			tinsert(arr, curr_know.kill_entry_to_str(list[i]))
		end
	else
		for i = 1, getn(list) do
			tinsert(arr, curr_know.kill_entry_to_str(list[i]) ..
					' ("' .. list[i].color .. list[i].esc_char .. '#w")')
		end
	end

	-- Sorting has changed, srart over at being of list
	info.begin = 1

	return arr, info, state
end -- curr_know.show_uniques()

function curr_know.pluralize_kill_type(str)
	if strfind(str, "/") then
		local parts = strsplit(str, "/")

		for i = 1, getn(parts) do
			parts[i] = curr_know.pluralize_kill_type(parts[i])
		end

		return strjoin(parts, "/")
	end

	if str == "etc" or str == "etc." then
		return str
	end

	if strfind(str, " ?%((.*)%)$") then
		local paren_pos = strfind(str, " ?%((.*)%)$")

		local pre_paren = strsub(str, 1, paren_pos - 1)
		local paren_str = strsub(str, paren_pos, -1)

		return curr_know.pluralize_kill_type(pre_paren) .. paren_str
	end

	return pluralize(str)
end

curr_know.kill_type_order = {}

hook(hook.GAME_START,
function()
	local list = {}

	for i = 26, 127 do
		local char = strchar(i)

		tinsert(list, {curr_know.symbol_to_kill_type(char), char})
	end

	sort(list,
		 function(a, b)
			 return a[1] < b[1]
		 end)

	for i = 1, getn(list) do
		curr_know.kill_type_order[list[i][2]] = i
	end
end)

function curr_know.sort_kills_by_type(a, b)
	-- If types are same, sort by name
	if a.char == b.char then
		return curr_know.sort_kills_by_name(a, b)
	else
		return curr_know.kill_type_order[a.char] <
			curr_know.kill_type_order[b.char]
	end
end

function curr_know.sort_kills_by_level(a, b)
	-- If levels are same, sort bv name
	if a.monst.level == b.monst.level then
		return curr_know.sort_kills_by_name(a, b)
	else
		-- Sort in descending order
		return a.monst.level > b.monst.level
	end
end

function curr_know.sort_kills_by_name(a, b)
	-- Don't sort by "a.name < b.name", since the plurilization might
	-- have done weird stuff to the string
	return strlower(a.monst.name) < strlower(b.monst.name)
end

function curr_know.kill_entry_to_str(entry)
	local str

	if entry.unique then
		str = "       " .. entry.name

		if entry.count > 1 then
			str = str .. " (killed " .. entry.count .. " times)"
		end
	else
		str = format("%6d", entry.count) .. " " .. entry.name
	end

	return str
end -- curr_know.kill_entry_to_str()

-------------------------------------------------------------------------
-- Traps
-------------------------------------------------------------------------
function curr_know.show_traps_aux(know_all)
	local known_traps = traps.get_known(know_all)
	local list        = {}

	for i = 1, getn(known_traps) do
		local idx  = known_traps[i]
		local trap = traps.trap_list[idx]

		local entry = {
			name  = traps.ident_name(idx),
			char  = strchar(trap.char),
			color = format("#0x%02x", trap.attr),
			trap  = trap
		}

		tinsert(list, entry)
	end

	return list
end -- curr_know.show_traps_aux()

function curr_know.show_traps(state, char, key, begin, arr)
	if not state then
		state = {sort = "color"}
		char  = "c"
	end

	char = strlower(char)

	local info = {}
	local func

	if char == "c" then
		func               = curr_know.sort_traps_by_color
		state.sort         = "color"
		info.footer        = "'n' to sort by name"
		info.to_func_chars = {n = true, N = true}
	elseif char == "n" then
		func               = curr_know.sort_traps_by_name
		state.sort         = "name"
		info.footer        = "'c' to sort by color"
		info.to_func_chars = {c = true, C = true}
	end

	local traps = curr_know.show_traps_aux(wizard)
	sort(traps, func)

	local lines = {}

	for i = 1, getn(traps) do
		local trap = traps[i]

		tinsert(lines, "    " .. trap.color .. trap.char .. "#w " ..
				trap.name)
	end

	-- Sorting has changed, srart over at being of list
	info.begin = 1

	return lines, info, state
end -- curr_know.show_traps()

function curr_know.sort_traps_by_color(a, b)
	if a.color == b.color then
		return curr_know.sort_traps_by_name(a, b)
	else
		return a.color < b. color
	end
end

-- Ignore "Trap of " at beginning of trap name when sorting
function curr_know.sort_traps_by_name(a, b)
	local a_name = a.name
	local b_name = b.name

	if starts_with(a_name, "Trap of ") then
		a_name = strsub(a_name, 9, -1)
	end

	if starts_with(b_name, "Trap of ") then
		b_name = strsub(b_name, 9, -1)
	end

	return a_name < b_name
end

-----------------------------------------------------------------
-- Recall depths
-----------------------------------------------------------------
function curr_know.display_recall_depths()
	local max_name_len = 0

    for i = 1, max_d_idx - 1 do
        if max_dlv(i) ~= 0 and
			not has_flag(d_info(i), FLAG_NO_RECALL_OUT)
		then
            if strlen(d_info(i).name) > max_name_len then
                max_name_len = strlen(d_info(i).name)
            end
        end
    end

    local lines = {}

    for i = 1, max_d_idx - 1 do
        if max_dlv(i) ~= 0 and
			not has_flag(d_info(i), FLAG_NO_RECALL_OUT)
		then
            local str = format("%-" .. max_name_len .. "s: Level %3d (%d')",
                               d_info(i).name, max_dlv(i), max_dlv(i) * 50)

            if player.recall_dungeon == i then
                str = "* " .. str
            else
                str = "- " .. str
            end
            tinsert(lines, str)
        end
    end -- for i = 1, max_d_idx do

	return lines
end -- curr_know.display_recall_depths()

----------------------------------------------------------------------------
-- Dungeon towns
----------------------------------------------------------------------------
function curr_know.show_dungeon_towns()
	-- Scan all dungeons
	local max_name_len = 0
	local dun_towns    = {}

	for i = 1, max_d_idx - 1 do
		local dun = d_info(i)

		-- Scan all dungeon town slots
		for j = 1, TOWN_DUNGEON do
            local town_idx = dun.t_idx[j]
			local town     = town_info[town_idx + 1]

			-- Only list towns which are real and known
			if (town.flags & TOWN_REAL ~= 0) and
				(town.flags & TOWN_KNOWN ~= 0)
			then

				if strlen(dun.name) > max_name_level then
					max_name_len = strlen(dun.name)
				end

				tinsert(dun_towns, {i, dun.t_level[town_idx + 1]})
			end
		end
	end

	local lines = {}
	for i = 1, getn(dun_towns) do
		local dun = d_info(dun_towns[i][1])
		local lev = dun_towns[i][2]

		tinsert(lines, format("%-" .. max_name_len .. "s: Level %3d (%d')",
							  dun.name, lev, 50 * lev))
	end

	return lines
end -- curr_know.show_dungeon_towns()

-------------------------------------------------------------------
-- Quests
-------------------------------------------------------------------
function curr_know.show_quests()
	local quests = {}
	local ret, wid, hgt = term.get_size()

	for i = 1, max_q_idx - 1 do
		local q = quest(i)

		if q.dynamic_desc then
			tinsert(quests, {q, i})
		elseif not q.silent then
			-- Known fixed quest
			if q.status == QUEST_STATUS_TAKEN or
				q.status == QUEST_STATUS_COMPLETED or
				q.status == QUEST_STATUS_FAILED
			then
				tinsert(quests, {q, i})
			end
		end
	end

	sort(quests,
		 function(a, b)
			 return a[1].level < b[1].level
		 end)

	local lines = {}

	for i = 1, getn(quests) do
		local q   = quests[i][1]
		local idx = quests[i][2]

		if q.dynamic_desc then
			local desc = __quest_dynamic_desc[idx]()

			if desc then
				if type(desc) == "table" then
					for j = 1, getn(desc) do
						local split = line_breaks(desc[j], wid - 2)
						for str in split do
							tinsert(lines, str)
						end
					end
				else
					local split = line_breaks(desc, wid - 2)
					for str in split do
						tinsert(lines, str)
					end
				end
				tinsert(lines, "")
			end
		else
			local str

			if q.status == QUEST_STATUS_TAKEN then
				str = "#y" .. q.name .. " (Danger level: " .. q.level .. ")"
			elseif q.status == QUEST_STATUS_COMPLETED then
				str = "#G" .. q.name .. " Completed -- Unrewarded"
			elseif q.status == QUEST_STATUS_FAILED then
				str = "#R" .. qname .. " Failed"
			end

			tinsert(lines, str)

			for j = 1, 10 do
				str = quest_get_desc(idx, j)
				if not str or str == "" then
					break
				end

				tinsert(lines, str)
			end
			tinsert(lines, "")
		end
	end -- for i = 1, getn(quests) do

	return lines
end --- curr_know.show_quests()

-------------------------------------------------------------------
-------------------------------------------------------------------

curr_know.default_menu_items = {
	artifacts = {
		desc  = "Display known artfiacts",
		title = "Artifacts Seen",
		func  = curr_know.show_artifacts,
	},
	uniques = {
		desc  = "Display known uniques",
		title = "Known Uniques",
		func  = curr_know.show_uniques,

		pass_as_func = true,
	},
	objects = {
		desc  = "Display known objects",
		title = "Known Objects",
		func  = curr_know.show_objects,
	},
	kill_count = {
		desc  = "Display kill count",
		title = "Kill Count",
		func  = curr_know.show_kills,

		pass_as_func = true,
	},
	recall_depths = {
		desc  = "Display recall depths",
		title = "Recall Depths",
		func  = curr_know.display_recall_depths,
	},
	traps = {
		desc  = "Display known traps",
		title = "Known Traps",
		func  = curr_know.show_traps,

		pass_as_func = true,
	},
	dungeon_towns = {
		desc  = "Display known dungeon towns",
		title = "Dungeon Towns",
		func  = curr_know.show_dungeon_towns,
	},
	quests = {
		desc  = "Display current quests",
		title = "Quest status",
		func  = curr_know.show_quests,
	}
}

----------------------------------------------------------------------------
----------------------------------------------------------------------------

-- Init
if not get_subsystem_param("curr_know", "no_key_bind") then
	hook(hook.KEYPRESS, function (key)
		if key == strbyte('~') then curr_know.show_menu()
			return true
		elseif key == KTRL('Q') then
			show_arr_as_list(curr_know.show_quests(), "Quest status")
			return true
		end

		return false
	end)
end
