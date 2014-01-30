--
-- ToME/T-Engine "Baby" interface, for pure noobs :)
--

tbaby = {}

tbaby.EXIT = 1
tbaby.CONTINUE = 2

tbaby.display = function(c, msg, y ,x)
	if not x then
		c, msg, y, x = color.WHITE, c, msg, y
	end
	message.display(x, y, strlen(msg), c, msg)
end

tbaby.background =
{
[[#G ____    __     _  _     ____     ]]
[[#G(_  _)  /  \   ( \/ )   (  __)    ]]
[[#G  )(  _(  O )_ / \/ \ _  ) _)  _  ]]
[[#G (__)(_)\__/(_)\_)(_/(_)(____)(_) ]]
[[                                    ]]
[[and the #rT#w-#REngine                  ]]
}

-- Displays the main menu
function tbaby.menu(menu, cur, hgt)
	for i = 1, getn(menu) do
		local m = menu[i]
		local op, cl = '[', ']'
		local bcol, col = color.SLATE, color.SLATE
		local bcolc, colc = '#s', '#s'
		if cur == i then
			op, cl = '(', ')'
			col = color.YELLOW
			bcol = color.UMBER
			colc = '#y'
			bcolc = '#u'
		end
		tbaby.display(bcol, [[   ,--------------------------, ]], ((hgt - 1 - getn(menu) * 4) / 2) + 0 + i * 4, 22)
		tbaby.display(bcol, format([[  <%s %s%s%s %-20s   %s>]], colc, op, m[1], cl, m[2], bcolc), ((hgt - 1 - getn(menu) * 4) / 2) + 1 + i * 4, 22)
		tbaby.display(bcol, [[   `--------------------------' ]], ((hgt - 1 - getn(menu) * 4) / 2) + 2 + i * 4, 22)
	end
end

-- Displays a readonly modules list
function tbaby.list_mods(hgt, wid)
	local max = getn(modules)
	local more = false
	if max > hgt - 3 then max = hgt - 3; more = true end

	local y = hgt - 3 - max
	term.c_put_str(color.BLUE, " ---------------", y, wid - 17)
	y = y + 1
	tbaby.display(format("#b'#B%-15s", "Installed games"), y, wid - 17)
	y = y + 1
	tbaby.display(format("#b|#b%-15s", "--"), y, wid - 17)
	y = y + 1

	for i = 1, max do
		tbaby.display(format("#b|#B%-15s", modules[i].name), y, wid - 17)
		y = y + 1
	end
end

-- Main display function
function tbaby.start()
	local savefiles = tbaby.get_savefiles()
	local ret, wid, hgt = term.get_size()
	local sel = 1
	local menu =
	{
		{ 'a', "Play a new game", tbaby.new_savefile }
		{ 'b', "Load a saved game", tbaby.load_savefile }
		{ 'c', "Install a module", tbaby.install_module }
		{ 'd', "Informations", function() end }
		{ 'e', "Exit", function() esettings.save() return tbaby.EXIT end }
	}
	local exec_menu = {}
	for i = 1, getn(menu) do exec_menu[menu[i][1]] = menu[i][3] end

	-- If we have savefiles, default to loading one
	if getn(savefiles) >= 2 then
		sel = 2
	end

	while not nil do
		term.clear()
		for i = 1, getn(tbaby.background) do tbaby.display(tbaby.background[i], i - 1, 0) end
		tbaby.menu(menu, sel, hgt)
		tbaby.list_mods(hgt, wid)

		local raw = term.inkey()
		if raw < 0 then
			raw = 0
		end
		local key = strchar(raw)

		local ret, aux = nil
		if key == '2' then
			sel = sel + 1
			if sel > getn(menu) then sel = 1 end
		elseif key == '8' then
			sel = sel - 1
			if sel < 1 then sel = getn(menu) end
		elseif key == '\r' then
			ret, aux = menu[sel][3]()
		elseif exec_menu[key] then
			ret, aux = exec_menu[key]()
		end
		if ret then
			if ret == tbaby.EXIT then return nil
			elseif ret == tbaby.CONTINUE then return aux
			end
		end
	end
end

-- Download a new module and installs it
function tbaby.install_module()
	local ret = (esettings.get("engine.network.always_connect", 0) ~= 0) and strbyte('y') or msg_box("This requires to connect to modules.t-o-m-e.net, do you agree? (y/N)")
	if ret ~= strbyte('Y') and ret ~= strbyte('y') then return end

	local modules_data = net.http_download("T-Engine modules list download screen", net.HTTP_USER_AGENT, "modules.t-o-m-e.net", "/get_list.php?emajor="..game_engine.version[1].."&eminor="..game_engine.version[2], modules_list_http_callback)
	local remote_modules = bytecode.load(modules_data)()
	local installable_modules = {}
	sort(remote_modules, function(a, b)
		if a.name == "ToME" then return 1
		elseif b == "ToME" then return nil
		else return a.name < b.name
		end
	end)
	for i = 1, getn(remote_modules) do
		local old_mod = find_module(remote_modules[i].name)
		local x, y, M, m, p = strfind(remote_modules[i].version, "^(%d+).(%d+).(%d+)")
		local version
		if x then
			version = { tonumber(M),tonumber(m),tonumber(p), }
		else
			version = { -1, -1, -1 }
		end

		local new_mod = {}
		new_mod.is_remote = remote_modules[i].file
		new_mod.name = remote_modules[i].name
		new_mod.desc = remote_modules[i].desc
		new_mod.author = { remote_modules[i].author }
		new_mod.version = version

		-- Only display remote modules that are newer than installed ones
		if not old_mod or (old_mod and require_version.test_version(version, modules[old_mod].version) == true) then
			tinsert(installable_modules, new_mod)
		end
	end

	if getn(installable_modules) == 0 then
		msg_box("There is no new modules to install.")
		return
	end

	local old_term = term.save_to()
	local ret, wid, hgt = term.get_size()
	local sel = 1
	local scroll = 1
	while not nil do
		term.load_from(old_term, false)
		term.screen_color(color.SLATE)
		display_list(3, 0, hgt - 8, 20, "Modules", installable_modules, scroll, sel, color.LIGHT_GREEN, function(e) return e.name end)

		draw_box(3, 21, hgt - 8, wid - 22)
		term.c_put_str(color.LIGHT_BLUE, " "..installable_modules[sel].name.." ", 3, 23)

		local y = 4
		term.put_str("Author:  "..installable_modules[sel].author[1], y, 22)
		term.put_str("Version: "..installable_modules[sel].version[1].."."..installable_modules[sel].version[2].."."..installable_modules[sel].version[3], y + 1, 22)
		y = y + 2
		y = y + 1
		y = display_module_desc(installable_modules[sel].desc, y, 22, hgt - y - 8, wid - 22)

		local raw = term.inkey()
		if raw < 0 then
			raw = 0
		end
		local key = strchar(raw)

		if raw == ESCAPE then
			break
		elseif key == '2' then
			repeat
				sel = sel + 1
				if sel > getn(installable_modules) then sel = getn(installable_modules) end
				if sel >= scroll + hgt then scroll = scroll + 1 end
			until sel ~= getn(installable_modules) + 1
		elseif key == '8' then
			repeat
				sel = sel - 1
				if sel < 1 then sel = 1 end
				if sel < scroll then scroll = scroll - 1 end
			until sel ~= getn(installable_modules) + 1
		elseif key == '\r' then
			local mod_file = install_new_module(installable_modules[sel])
			if mod_file then
				load_module(mod_file)
				msg_box("Module "..installable_modules[sel].name.." installed")
				tremove(installable_modules, sel)
				sel = 1
				scroll = 1
				if getn(installable_modules) == 0 then break end
			end
		end
	end

	term.load_from(old_term, true)
end

-- List savefiles
function tbaby.load_savefile()
	local savefiles = tbaby.get_savefiles()

	if getn(savefiles) < 2 then
		msg_box("You have no saved games, make a new one.")
		return
	end

	local dec_sel = function(savefiles, sel)
		repeat
			sel = sel - 1
		until not savefiles[sel] or not savefiles[sel].not_real
		return sel
	end
	local inc_sel = function(savefiles, sel)
		repeat
			sel = sel + 1
		until not savefiles[sel] or not savefiles[sel].not_real
		return sel
	end

	local old_term = term.save_to()
	local ret, wid, hgt = term.get_size()
	local sel = 2
	local scroll = 1
	local size = hgt - 8 - 3

	-- Find last used
	for i = 1, getn(savefiles) do
		if not savefiles[i].not_real then
			if savefiles[i].last_modified > savefiles[sel].last_modified then
				sel = i
			end
		end
	end

	while not nil do
		term.load_from(old_term, false)
		term.screen_color(color.SLATE)
		display_list(3, 0, hgt - 8, 20, "Characters", savefiles, scroll, sel, color.LIGHT_GREEN, function(e) return e.save end)
		draw_box(3, 21, hgt - 8, wid - 22)

		local y = 4
		if savefiles[sel].is_dead then
			term.c_put_str(color.LIGHT_RED, "This character is dead. Sorry.", y, 22)
			term.c_put_str(color.LIGHT_RED, "If you select it you will start a new character with", y + 1, 22)
			term.c_put_str(color.LIGHT_RED, "the monster memory of the dead one. (Recommended)", y + 2, 22)
			y = y + 3
		end
		for i = 1, getn(savefiles[sel].data) do
			tbaby.display(savefiles[sel].data[i], y + i - 1, 22)
		end

		local raw = term.inkey()
		if raw < 0 then
			raw = 0
		end
		local key = strchar(raw)

		if raw == ESCAPE then
			break
		elseif key == '2' then
			repeat
				sel = inc_sel(savefiles, sel)
				if sel > getn(savefiles) then
					sel = inc_sel(savefiles, 0)
					scroll = 1
				else
					if sel > scroll + size - 1 then scroll = scroll + 1 end
				end
			until not savefiles[sel].not_real
		elseif key == '8' then
			repeat
				sel = dec_sel(savefiles, sel)
				if sel < 1 then
					sel = dec_sel(savefiles, getn(savefiles) + 1)
					scroll = sel - size - 1
					if scroll < 1 then scroll = 1 end
				else
					if sel < scroll then scroll = scroll - 1 end
				end
				if scroll > 1 and savefiles[scroll - 1].not_real then scroll = scroll - 1 end
			until not savefiles[sel].not_real
		elseif raw == 8 or raw == 127 then -- backspace or delete
			local confirm = input_box('Are you *SURE* you want to delete this character? Enter "DELETE" to confirm.', 6)
			if confirm and confirm == "DELETE" then
				local save = savefiles[sel].full_path

				-- Delete all files and then the directory itself
				local files = fs.enumerate_files(save, false)
				for file in files do
					fs.delete(save.."/"..file)
				end
				fs.delete(save)

				-- Now reload savefiles
				savefiles = tbaby.get_savefiles()
				scroll, sel = 1, 2
			end
		elseif key == '\r' then
			birth.process_player_name(savefiles[sel].real_save)
			birth.no_begin_screen = true
			birth.load_from_dead = savefiles[sel].is_dead
			return tbaby.CONTINUE, modules[__modules_by_sname[savefiles[sel].module]].name
		end
	end

	term.load_from(old_term, true)
end

-- List modules and start a new game
function tbaby.new_savefile()
	if getn(modules) == 0 then
		msg_box('You have no module installed.\nPlease use the "Install a module" menu item to get some.')
		return
	end

	-- Does the user realy wants a completly new save ?
	local savefiles = tbaby.get_savefiles()

	local old_term = term.save_to()
	local ret, wid, hgt = term.get_size()
	local sel = 1
	local scroll = 1
	local size = hgt - 8 - 3
	while not nil do
		term.load_from(old_term, false)
		term.screen_color(color.SLATE)
		display_list(3, 0, hgt - 8, 20, "Modules", modules, scroll, sel, color.LIGHT_GREEN, function(e) return e.name end)

		draw_box(3, 21, hgt - 8, wid - 22)
		term.c_put_str(color.LIGHT_BLUE, " "..modules[sel].name.." ", 3, 23)

		local y = 4
		term.put_str("Author:  "..modules[sel].author[1], y, 22)
		term.put_str("Version: "..modules[sel].version[1].."."..modules[sel].version[2].."."..modules[sel].version[3], y + 1, 22)
		y = y + 2
		y = y + 1
		y = display_module_desc(modules[sel].desc, y, 22, hgt - y - 8, wid - 22)

		local raw = term.inkey()
		if raw < 0 then
			raw = 0
		end
		local key = strchar(raw)

		if raw == ESCAPE then
			break
		elseif key == '2' then
			repeat
				sel = sel + 1
				if sel > getn(modules) then
					sel = 1
					scroll = 1
				else
					if sel > scroll + size - 1 then scroll = scroll + 1 end
				end
			until not modules[sel].not_real
		elseif key == '8' then
			repeat
				sel = sel - 1
				if sel < 1 then
					sel = getn(modules)
					scroll = sel - size - 1
					if scroll < 1 then scroll = 1 end
				else
					if sel < scroll then scroll = scroll - 1 end
				end
			until not modules[sel].not_real
		elseif key == '\r' then
			local has_dead = false
			for i = 1, getn(savefiles) do
				if savefiles[i].module == modules[sel].shortname and savefiles[i].is_dead then has_dead = true break end
			end
			if has_dead then
				local ret = msg_box("It seems you have a dead character for this module already.\nIt is advised to re-use to keep monster memory.\nTo do that just load the dead character and make a new one from its ashes.\nDo you still want to make a new game? (y/N)")
				if ret ~= strbyte('y') and ret ~= strbyte('Y') then return end
			end
			birth.no_begin_screen = true
			birth.force_new_game = true
			return tbaby.CONTINUE, modules[sel].name
		end
	end

	term.load_from(old_term, true)
end

-- Gets the list of savefiles from all modules
function tbaby.get_savefiles()
	local savefiles = {}
	local tmp_saves = {}
	local dirs = fs.enumerate_files("/", true)
	for i = 1, getn(dirs) do
		if __modules_by_sname[dirs[i]] then
			local mod = modules[__modules_by_sname[dirs[i]]]
			local saves = fs.enumerate_files("/"..dirs[i].."/"..mod.version[1].."."..mod.version[2].."/save/", true)
			if getn(saves) >= 1 then
				local new_saves = {}
				for j = 1, getn(saves) do
					local fff = fs.open("/"..dirs[i].."/"..mod.version[1].."."..mod.version[2].."/save/"..saves[j].."/descriptor", "r")
					local data = {}
					if fff then
						local line = nil
						repeat
							line = fs.read(fff)
							if line then tinsert(data, line) end
						until line == nil
						fs.close(fff)
					end
					local last_modified = fs.last_modified("/"..dirs[i].."/"..mod.version[1].."."..mod.version[2].."/save/"..saves[j].."/descriptor")
					local player_name = data[1] or saves[j]
					tremove(data, 1)
					local is_dead = false
					if (data[1] or "alive") == "dead" then is_dead = true end
					tremove(data, 1)
					local prefix = " "

					tinsert(new_saves, { module=dirs[i] name=player_name save=prefix..player_name real_save=saves[j] data=data last_modified=last_modified is_dead=is_dead full_path="/"..dirs[i].."/"..mod.version[1].."."..mod.version[2].."/save/"..saves[j] })
				end
				sort(new_saves, function(a, b) return a.name < b.name end)
				tinsert(tmp_saves, new_saves)
			end
		end
	end
	sort(tmp_saves, function(a, b)
		if a[1].module == "tome" then return 1
		elseif b[1].module == "tome" then return nil end
		return strlower(a[1].module) < strlower(b[1].module)
	end)
	for i = 1, getn(tmp_saves) do
		local d = tmp_saves[i]
		tinsert(savefiles, { module=d[1].module save="#R"..modules[__modules_by_sname[d[1].module]].name not_real=true })
		for j = 1, getn(d) do
		      	tinsert(savefiles, d[j])
		end
	end
	return savefiles
end
