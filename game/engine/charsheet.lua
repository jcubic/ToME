-- Character sheets subsystem

declare_globals
{
	"character_sheet",
	"set_character_sheet",
	"__character_sheets",
}

__character_sheets = {}
character_sheet = {}

function set_character_sheet(t)
	assert(t.name, "no sheet name")
	assert(t.page, "no sheet page")
	assert(t.display, "no sheet display")

	__character_sheets[t.page] = t
end

function character_sheet.print_kill_count(file)
	__print_kill_count(file)
end

function character_sheet.display(sheet_nb, enter_quit)
	local cur = __character_sheets[sheet_nb]

	term.clear()

	local name = game_module..": "..cur.name
	term.color_text(color.WHITE, name, 0, 40 - (strlen(name) / 2))

	for location, data in cur.display do
		if (not data.condition) or data.condition() then
			if data.data then
				local text, text_color
				local title_recolor = nil

				-- String ? This is a simple variable name, grab it
				if type(data.data) == "string" then text, title_recolor = dostring("return "..data.data)
				-- Ok, must be a function
				else text, title_recolor = data.data() end

				text_color = color.WHITE
				if data.color then
					if data.color == "auto" then
						if text < 0 then text_color = color.LIGHT_RED
						elseif text == 0 then text_color = color.LIGHT_BLUE
						else text_color = color.LIGHT_GREEN
						end
					else
						text_color = data.color
					end
				end

				-- Display the title
				if not title_recolor then title_recolor = color.WHITE end
				term.color_text(title_recolor, data.title, location[1], location[2])

				-- And now the data
				local dist = location[4]

				-- Means text is actually right aligned
				if location[4] < 0 then
					text = format("%"..-(location[4] + strlen(data.title))..location[3], text)
					dist = 0
				end
				term.color_text(text_color, text, location[1], location[2] + strlen(data.title) + dist)
			else
				-- Display the title
				term.color_text(color.WHITE, data.title, location[1], location[2])
			end
		end
	end

	local extra = ""
	if cur.keys then
		for key, action in cur.keys do
			if not action.hidden then
				extra = extra.."'"..key.."' to "..action.name..", "
			end
		end
	end
	if enter_quit then
		extra = "[Press ENTER]"
	else
		extra = "["..extra.."'f' to file, 'p' for previous, 'n' for next, or ESC]"
	end
	term.color_text(color.WHITE, extra, 23, 40 - (strlen(extra) / 2))
	return cur
end

--------------------------------------------------------------------------
-- Writing the character sheet to a file
--------------------------------------------------------------------------
hook.new_hook_type("DUMP_CHARSHEET")

character_sheet.dump_order = "cdsaeih"

function character_sheet.write_to_file(name)
	local path = path_build(TENGINE_DIR_USER, name)
	if fs.file_exist(path) then
		local ret = msg_box("Overwrite file? (y/N)")
		if ret ~= strbyte('y') and ret ~= strbyte('Y') then
			return
		end
	end

	local file = fs.open(path, "w")
	if not file then
		local str = "Failed to write file: " .. fs.get_last_error()
		msg_box(str)
		message(str)
		return
	end

	name = strlower(name)

	local is_html = ends_with(name, ".htm") or ends_with(name, ".html")

	if character_sheet.dump_to_file and
		character_sheet.dump_to_file(file, is_html)
	then
		-- Has defined and used its own "dump charsheet to file", so
		-- there's nothing more to do
		fs.close(file)
		return
	end

	local old_text_out = desc_obj.text_out
	desc_obj.text_out = character_sheet.text_to_array

	if is_html then
		fs.write(file, '<table bgcolor="Black"><tr><td><PRE><font color="White">')
	end

	local str = current_module.name..' '..current_module.version[1]..
		'.'..current_module.version[2]..'.'..
		current_module.version[3]..' Character Sheet\n'
	fs.write(file, str)

	hook.process(hook.DUMP_CHARSHEET, "start", file, is_html)

	for i = 1, strlen(character_sheet.dump_order) do
		local curr = strsub(character_sheet.dump_order, i, i)

		if curr == "c" then
			hook.process(hook.DUMP_CHARSHEET, "before sheets", file, is_html,
						 __character_sheets)

			for i = 1, getn(__character_sheets) do
				hook.process(hook.DUMP_CHARSHEET, "before single sheet",
							 file, is_html, __character_sheets[i])
				character_sheet.write_sheet(file, __character_sheets[i],
											is_html)
				hook.process(hook.DUMP_CHARSHEET, "after single sheet",
							 file, is_html, __character_sheets[i])
			end
			hook.process(hook.DUMP_CHARSHEET, "after sheets", file, is_html,
						 __character_sheets)
		elseif curr == "e" then
			hook.process(hook.DUMP_CHARSHEET, "before equipment",
						 file, is_html)
			character_sheet.write_equipment(file, is_html)
			hook.process(hook.DUMP_CHARSHEET, "after equipment",
						 file, is_html)
		elseif curr == "i" then
			hook.process(hook.DUMP_CHARSHEET, "before inventory",
						 file, is_html)
			character_sheet.write_inventory(file, is_html)
			hook.process(hook.DUMP_CHARSHEET, "after inventory",
						 file, is_html)
		elseif curr == "h" then
			if character_sheet.get_home_list then
				local home_list = character_sheet.get_home_list()


				hook.process(hook.DUMP_CHARSHEET, "before homes",
							 file, is_html, home_list)

				for i = 1, getn(home_list) do
					hook.process(hook.DUMP_CHARSHEET, "before single home",
								 file, is_html, home_list[i])
					character_sheet.write_home(file, home_list[i], is_html)
					hook.process(hook.DUMP_CHARSHEET, "after single home",
								 file, is_html, home_list[i])
				end

				hook.process(hook.DUMP_CHARSHEET, "after homes",
							 file, is_html, home_list)
			end
		elseif curr == "s" then
			hook.process(hook.DUMP_CHARSHEET, "before skills", file, is_html)
			character_sheet.write_skills(file, is_html)
			hook.process(hook.DUMP_CHARSHEET, "after skills", file, is_html)
		elseif curr == "a" then
			hook.process(hook.DUMP_CHARSHEET, "before abilities",
						 file, is_html)
			character_sheet.write_abilities(file, is_html)
			hook.process(hook.DUMP_CHARSHEET, "after abilities",
						 file, is_html)
		elseif curr == "d" then
			hook.process(hook.DUMP_CHARSHEET, "before dungeon depths",
						 file, is_html)
			character_sheet.write_dungeon_depths(file, is_html)
			hook.process(hook.DUMP_CHARSHEET, "after dungeon depths",
						 file, is_html)
		else
			hook.process(hook.DUMP_CHARSHEET, "before module dump",
						 file, is_html, curr)
			hook.process(hook.DUMP_CHARSHEET, "module dump",
						 file, is_html, curr)
			hook.process(hook.DUMP_CHARSHEET, "after module dump",
						 file, is_html, curr)
		end
	end -- for i = 1, strlen(character_sheet.dump_order) do

	hook.process(hook.DUMP_CHARSHEET, "end", file, is_html)

	if is_html then
		fs.write(file, "</font></PRE></td></tr></table>")
	end

	desc_obj.text_out = old_text_out

	fs.close(file)
end -- character_sheet.write_to_file()

function character_sheet.write_sheet(file, sheet, is_html)
	local parts = {}

	for location, data in sheet.display do
		if (not data.condition) or data.condition() then
			if data.data then
				local text, text_color
				local title_recolor = nil

				-- String ? This is a simple variable name, grab it
				if type(data.data) == "string" then
					text, title_recolor = dostring("return "..data.data)
				-- Ok, must be a function
				else
					text, title_recolor = data.data()
				end


				--len = len + strlen(data.title) - 1

				-- Means text is actually right aligned
				if location[4] < 0 then
					local len = character_sheet.strlen_minus_colors(data.title)
					text = format("%"..
								  -(location[4] + len + 1) .. location[3],
								  text)
				end

				if is_html then
					text = character_sheet.html_escape(text)
					text = character_sheet.colors_to_html(text)

					data.title = character_sheet.html_escape(data.title)
					data.title = character_sheet.colors_to_html(data.title)
				end

				tinsert(parts, {location[1], location[2],
							data.title .. ' ' .. text, title_recolor})
			else
				local text = data.title

				if is_html then
					text = character_sheet.html_escape(text)
					text = character_sheet.colors_to_html(text)
				end

				tinsert(parts, {location[1], location[2], text})
			end
		end
	end

	sort(parts, function(part1, part2)
		if part1[1] ~= part2[1] then
			return part1[1] < part2[1]
		else
			return part1[2] < part2[2]
		end
	end)

	local lines = {}
	local line_num = 1
	local i = 1

	-- Glue together parts which are to be placed on the same line
	-- number
	while i <= getn(parts) and line_num do
		local curr_line = {}

		while i <= getn(parts) and parts[i][1] == line_num do
			tinsert(curr_line, parts[i])
			i = i + 1
		end

		if getn(curr_line) > 0 then
			local text = ""

			for j = 1, getn(curr_line) do
				local text_len

				if is_html then
					text_len =  character_sheet.strlen_minus_markup(text)
				else
					text_len =  character_sheet.strlen_minus_colors(text)
				end

				if curr_line[j][2] > text_len then
					local blanks = curr_line[j][2] - text_len

					text = text .. strrep(" ", blanks)
				end

				text = text .. curr_line[j][3]
			end

			lines[line_num] = text
		end

		if i <= getn(parts) then
			line_num = parts[i][1]
		end
	end -- while i <= getn(parts) and line_num do

	fs.write(file, '')

	tinsert(lines, 1, " ")
	tinsert(lines, 1, "  *** " .. sheet.name .. " ***")

	if is_html then
		for i = 1, getn(lines) do
			lines[i] = lines[i] or ""
		end
	else -- if is_html then
		for i = 1, getn(lines) do
			lines[i] = character_sheet.strip_colors(lines[i] or "")
		end
	end -- if not is_html then

	for i = 1, getn(lines) do
		if lines[i] and lines[i] ~= "" then
			fs.write(file, lines[i])
		end
	end

	fs.write(file, '')
end -- character_sheet.write()

function character_sheet.write_dungeon_depths(file, is_html)
	fs.write(file, "   *** Dungeon Depth ***")
	fs.write(file, '')

	local max_name_len = -1

	for i = 1, max_d_idx - 1 do
		if max_dlv(i) ~= 0 then
			if strlen(d_info(i).name) > max_name_len then
				max_name_len = strlen(d_info(i).name)
			end
		end
	end

	local lines = {}

	for i = 1, max_d_idx - 1 do
		if max_dlv(i) ~= 0 then
			local str = format("%-" .. max_name_len .. "s: Level %3d (%d')",
							   d_info(i).name, max_dlv(i), max_dlv(i) * 50)

			if player.recall_dungeon == i then
				str = "#B* " .. str
			else
				str = "- " .. str
			end
			tinsert(lines, str)
		end
	end -- for i = 1, max_d_idx do

	if is_html then
		for i = 1, getn(lines) do
			lines[i] = character_sheet.html_escape(lines[i] or "")
			lines[i] = character_sheet.colors_to_html(lines[i])
		end
	else -- if is_html then
		for i = 1, getn(lines) do
			lines[i] = character_sheet.strip_colors(lines[i] or "")
		end
	end -- if not is_html then

	for i = 1, getn(lines) do
		fs.write(file, lines[i])
	end

	fs.write(file, '')
end -- character_sheet.write_dungeon_depths()

function character_sheet.write_skills(file, is_html)
	fs.write(file, "   *** Character skills ***")
	fs.write(file, '')

	local skill_tree = skills.get_tree()
	local data       = {}

	-- Get data
	character_sheet.skill_tree_data(skill_tree, data, " ")

	-- Now format it
	local max_name_width = 0
	local max_val_width  = 0

	for i = 1, getn(data) do
		local len = character_sheet.strlen_minus_colors(data[i][1])

		if len > max_name_width then
			max_name_width = len
		end

		len = character_sheet.strlen_minus_colors(data[i][2])

		if len > max_val_width then
			max_val_width = len
		end
	end

	local lines = {}
	local width = max_name_width + max_val_width + 1

	for i = 1, getn(data) do
		local name = data[i][1]
		local val  = data[i][2]

		local padding = width - character_sheet.strlen_minus_colors(name) -
			character_sheet.strlen_minus_colors(val)

		local str = name .. strrep(" ", padding) .. val

		tinsert(lines, str)
	end

	-- Formatted, now print it
	if is_html then
		for i = 1, getn(lines) do
			lines[i] = character_sheet.html_escape(lines[i] or "")
			lines[i] = character_sheet.colors_to_html(lines[i])
		end
	else -- if is_html then
		for i = 1, getn(lines) do
			lines[i] = character_sheet.strip_colors(lines[i] or "")
		end
	end -- if not is_html then

	for i = 1, getn(lines) do
		fs.write(file, lines[i])
	end

	fs.write(file, '')
end -- character_sheet.write_skills()

function character_sheet.skill_tree_data(skill_tree, data, indent)
	local max_skill = skills.MAX_VALUE / skills.STEP_VALUE

	for i = 1, getn(skill_tree) do
		local info  = skill_tree[i]
		local s_idx = info.skill
		local color

		if get_skill(s_idx) >= max_skill then
			color = "#B"
		elseif get_skill(s_idx) ~= 0 then
			color = "#w"
		elseif s_info(s_idx).mod > 0 then
			color = "#o"
		else
			-- Inacessible skill visible only due to wizard mode
			color = "#s"
		end

		local name = color .. indent

		if info.children then
			name = name .. "-"
		else
			name = name .. "."
		end

		name = name .. " " .. s_info(s_idx).name

		local value

		if get_skill_raw(s_idx) < 0 then
			value = "-"
		else
			value = " "
		end

		local abs_raw = abs(get_skill_raw(s_idx))

		value = value .. format("%01d.%03d",
								abs_raw / skills.STEP_VALUE,
								imod(abs_raw, skills.STEP_VALUE) )

		value = value .. format(" [%01d.%03d]",
								s_info(s_idx).mod / 1000,
								imod(s_info(s_idx).mod, 1000) )

		tinsert(data, {name, value})

		if info.children then
			character_sheet.skill_tree_data(info.children, data,
											indent .. "    ")
		end
	end -- for i = 1, getn(skill_tree) do
end -- character_sheet.skill_tree_data()

function character_sheet.write_abilities(file, is_html)
	fs.write(file, "   *** Character abilities ***")
	fs.write(file, '')

	for i = 0, max_ab_idx - 1 do
		if has_ability(i) then
			fs.write(file, " * " .. ability(i).name)
		end
	end

	fs.write(file, '')
end -- character_sheet.write_abilities()

function character_sheet.write_equipment(file, is_html)
	fs.write(file, "   *** Character equipment ***")
	fs.write(file, '')

    for_inventory(player, INVEN_PACK, INVEN_TOTAL,
		function(obj, pos, slot, item)
			local c = strchar(imod(pos + 1, 27) + strbyte('a') - 1)

			character_sheet.write_obj(%file, obj, c .. ") ", %is_html)
		end)

	fs.write(file, '')
end -- character_sheet.write_equipment()

function character_sheet.write_inventory(file, is_html)
	fs.write(file, "   *** Character inventory ***")
	fs.write(file, '')

    for_inventory(player, INVEN_INVEN, INVEN_PACK,
		function(obj, pos, slot, item)
			local c = strchar(imod(slot - 1, 26) + strbyte('a'))

			character_sheet.write_obj(%file, obj, c .. ") ", %is_html)
		end)

	fs.write(file, '')
end -- character_sheet.write_inventory()

hook.new_hook_type("DUMP_OBJ_ANNOTATE")
hook.new_hook_type("DUMP_OBJ_SKIP_DETAILS")

function character_sheet.write_obj(file, obj, slot_label, is_html)
	hook.process(hook.DUMP_CHARSHEET, "before single object",
				 file, is_html, obj)

	character_sheet.desc_obj_indent = strrep(" ", strlen(slot_label) + 1)

	local lines = {character_sheet.desc_obj_indent}
	character_sheet.desc_obj_lines = lines

	local notes = {before = "", after = ""}

	hook.process(hook.DUMP_OBJ_ANNOTATE, obj, notes)

	if notes.before ~= "" then
		notes.before = notes.before .. " "
	end

	if notes.after ~= "" then
		notes.after = " " .. notes.after
	end

	local text = slot_label .. notes.before .. object_desc(obj, true, 2) ..
		notes.after

	if is_html then
		text = character_sheet.html_escape(text)
	end

	fs.write(file, text)

	if not hook.process(hook.DUMP_OBJ_SKIP_DETAILS, obj, notes) then
		hook.process(hook.OBJECT_DESC, obj, true, false)
	end

	local i = 1

	if is_html then
		for i = 1, getn(lines) do
			lines[i] = character_sheet.html_escape(lines[i])
			lines[i] = character_sheet.colors_to_html(lines[i])
			lines[i] = character_sheet.bold_numbers(lines[i])
		end
	else
		for i = 1, getn(lines) do
			lines[i] = character_sheet.strip_colors(lines[i])
		end
	end

	for i = 1, getn(lines) do
		if clean_whitespace(lines[i]) ~= "" then
			fs.write(file, lines[i])
		end
	end

	hook.process(hook.DUMP_CHARSHEET, "before single object",
				 file, is_html, obj)
end -- character_sheet.write_obj()

function character_sheet.text_to_array(text)
	local num_lines = getn(character_sheet.desc_obj_lines)
	local line      = character_sheet.desc_obj_lines[num_lines]

	line = line .. text

	local len = character_sheet.strlen_minus_colors(line)

	if len >= 80 then
		-- Remove indent
		line = strsub(line, strlen(character_sheet.desc_obj_indent))

		local len = strlen(line)
		local trailing_space = (strsub(line, len, len) == " ")

		-- Split into words.
		local parts = strsplit(line, " ")

		-- Removing leading blanks
		while true do
			if not parts[1] or parts[1] == "" or parts[1] == " " then
				tremove(parts, 1)
			else
				break
			end
		end

		line = character_sheet.desc_obj_indent .. tremove(parts, 1)

		while getn(parts) > 0 do
			len = character_sheet.strlen_minus_colors(line)
			len = len + character_sheet.strlen_minus_colors(parts[1]) + 1

			if len >= 80 then
				character_sheet.desc_obj_lines[num_lines] = line
				line = character_sheet.desc_obj_indent .. strjoin(parts, " ")
				num_lines = num_lines + 1

				break
			end

			line = line .. " " .. tremove(parts, 1)
		end -- while getn(parts) do

		if trailing_space then
			line = line .. " "
		end
	end -- if len >= 80 then

	character_sheet.desc_obj_lines[num_lines] = line
end -- character_sheet.text_to_array()

function character_sheet.html_escape(text)
	local ret = gsub(text, "&", "&amp;")
	ret = gsub(ret, "<", "&lt;")

	return ret
end -- character_sheet.html_escape()

function character_sheet.bold_numbers(text)
	local ret = gsub(text, "([+-]?%d+%%?)", "<b>%1</b>")
	return ret
end

function character_sheet.colors_to_html(text)
	-- See http://www.w3schools.com/html/html_colornames.asp
	local table = {
		w = "White",
		d = "Black",

		s = "Grey",
		o = "Orange",
		r = "Red",
		g = "Green",
		b = "Blue",
		u = "DarkGoldenRod",

		D = "LightSlateGrey",
		W = "White",
		v = "Violet",
		y = "Yellow",
		R = "Pink",
		G = "LightGreen",
		B = "SkyBlue",
		U = "GoldenRod"
	}

	local fonts = 0
	local count
	for char, name in table do
		text, count = gsub(text, "#" .. char,
						   '<font color="' .. name .. '">')

		fonts = fonts + count
	end

	text = text .. strrep("</font>", fonts)

	return text
end

function character_sheet.strip_colors(text)
	local ret, count = gsub(text, "#[^#]", "")
	return ret
end

function character_sheet.strlen_minus_colors(text)
	local ret, count = gsub(text, "#[^#]", "")
	return strlen(ret)
end

function character_sheet.strlen_minus_markup(text)
	local ret, count = gsub(text, "<[^>]*>", "")
	return strlen(ret)
end

-------------------------------------------------------------------------

function character_sheet.show(no_term_save, enter_quit)
        if not no_term_save then term.save() end

	local sheet_nb = 1

	while not nil do
	 	local cur = character_sheet.display(sheet_nb, enter_quit)

		local key = strchar(term.inkey())

		if key == strchar(ESCAPE) or (enter_quit and key == '\r') then break
		elseif key == 'n' or key == '6' then
			sheet_nb = sheet_nb + 1
			if not __character_sheets[sheet_nb] then sheet_nb = 1 end
		elseif key == 'p' or key == '4' then
			sheet_nb = sheet_nb - 1
			if not __character_sheets[sheet_nb] then sheet_nb = getn(__character_sheets) end
		elseif key == 'f' then
			local name = input_box("Save character sheet?", 40, player_name())
			if strlen(name) > 0 then
				character_sheet.write_to_file(name)
			end

		elseif cur.keys then
			for act_key, action in cur.keys do
				if key == act_key then
					action.action()
					break
				end
			end
		end
	end

	if not no_term_save then term.load() end
end

function character_sheet.display_stat(stat)
	local text = cnv_stat(player.stat_ind(stat))
	local stat_color = "#G"
	local maxed = ":"

	-- Display "injured" stat
	if (player.stat_ind[stat] < player.stat_top[stat]) then
		if (player.stat_cnt(stat) ~= 0) then
			stat_color = "#o"
		else
			stat_color = "#y"
		end
	end

	-- Display "boosted" stat
	if (player.stat_ind[stat] > player.stat_top[stat]) then
		stat_color = "#v"
	end

	-- Indicate natural maximum
	if (player.stat_top[stat] == 18 + 100) then
		maxed = "!"
	end

	return format("%s%s%20s", maxed, stat_color, text)
end

function character_sheet.section_title(title, len)
	len = len - strlen(title) - 4
	local len1, len2 = len / 2, len / 2
	if (len / 2) * 2 ~= len then len2 = len2 + 1 end
	return format("#B[=%"..len1.."s%s%"..len2.."s=]", strrep("=", len1), title, strrep("=", len2))
end

function character_sheet.misc_skill(x, y)
	local likert_color, txt

	-- Paranoia
	if (y <= 0) then y = 1 end

	-- Negative value
	if (x < 0) then
		return "#DVery Bad"
	end

	local xy = x / y

	-- Analyze the value
	if xy == 0 or xy == 1 then
		likert_color = "#r"
		txt = "Bad"
	elseif xy == 2 then
		likert_color = "#R"
		txt = "Poor"
	elseif xy == 3 or xy == 4 then
		likert_color = "#o"
		txt = "Fair"
	elseif xy == 5 then
		likert_color = "#y"
		txt = "Good"
	elseif xy == 6 then
		likert_color = "#y"
		txt = "Very Good"
	elseif xy == 7 or xy == 8 then
		likert_color = "#G"
		txt = "Excellent"
	elseif xy >= 9 and xy <= 13 then
		likert_color = "#g"
		txt = "Superb"
	elseif xy >= 14 and xy <= 17 then
		likert_color = "#G"
		txt = "Heroic"
	else
		likert_color = "#G"
		txt = "Legendary["..(((xy - 17) * 5) / 2).."]"
	end
	return likert_color..txt
end

windows.type_define
{
	flag = FLAG_PW_PLAYER
	name = "Character sheet"
	display = function()
		character_sheet.display(1)
	end
}
