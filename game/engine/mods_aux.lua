-- Ok some functions that we dont need are dangerous
execute = nil
getenv = nil
setlocale = nil
--exit = nil
openfile = nil
writeto = nil
readfrom = nil
appendto = nil
remove = nil
rename = nil
tmpname = nil

modules = {}
__modules_next_pack = -1
__modules_by_name = {}
__modules_by_sname = {}

current_module = nil

__module_files = {}

game_engine = nil

-- Setups the T-Engine parameters
function set_engine(t)
	assert(t.version, "No engine version!")
	game_engine = t
end

-- Return the version of the T-Engine
function get_engine_version()
	return game_engine.version[1], game_engine.version[2], game_engine.version[3]
end

-- Tests for modules or engine version
require_version = {}
function require_version.test_version(v1, v2)
	if v1[1] > v2[1] then return true
	elseif (v1[1] == v2[1]) and (v1[2] > v2[2]) then return true
	elseif (v1[1] == v2[1]) and (v1[2] == v2[2]) and (v1[3] > v2[3]) then return true
	elseif (v1[1] == v2[1]) and (v1[2] == v2[2]) and (v1[3] == v2[3]) then return 2
	end
	return nil
end
function require_version.engine(maj, min, pat)
	local major, minor, patch = get_engine_version()

	if major > maj then return true
	elseif (major == maj) and (minor > min) then return true
	elseif (major == maj) and (minor == min) and (patch >= pat) then return true
	end

	return nil
end
function require_version.module(module, maj, min, pat)
	-- A list of modules/versions to check, valid if at least one is
	if type(module) == "table" then
		for mod, version in module do
			if require_version.module(mod, version[1], version[2], version[3]) then return true end
		end
		return nil

	-- Check a single module/version
	else
		local major, minor, patch = current_module.version[1], current_module.version[2], current_module.version[3]

		if module ~= current_module.name then return nil end
		if major > maj then return true
		elseif (major == maj) and (minor > min) then return true
		elseif (major == maj) and (minor == min) and (patch >= pat) then return true
		end

		return nil
	end
end

-- Redirect a specific file
function redirect_file(from_dir, from_file, to_dir, to_file)
	local olddir
	local newdir

	if type(from_dir) == "string" then
		olddir = path_build(from_dir, from_file)
	elseif type(from_dir) == "table" and getn(from_dir) == 1 then
		olddir = path_build(from_dir[1], from_file)
	elseif type(from_dir) == "table" then
		olddir = path_build(from_dir[1], from_dir[2])
		for i = 3, getn(from_dir) do
			olddir = path_build(olddir, from_dir[i])
		end
		olddir = path_build(olddir, from_file)
	end

	if type(to_dir) == "string" then
		newdir = path_build(to_dir, to_file)
	elseif type(to_dir) == "table" and getn(to_dir) == 1 then
		newdir = path_build(to_dir[1], to_file)
	elseif type(to_dir) == "table" then
		newdir = path_build(to_dir[1], to_dir[2])
		for i = 3, getn(to_dir) do
			newdir = path_build(newdir, to_dir[i])
		end
		newdir = path_build(newdir, to_file)
	end

	__module_files[olddir] = newdir
end

function setup_module(mod)
	-- Redirect specific files?
	if mod.files then
		for k, e in mod.files do
			if type(k) == "table" and type(e) == "table" then
				-- Add the full path
				local to = tremove(e)
				local from = tremove(k)
				redirect_file(k, from, e, to)
			end
		end
	end
end

function init_module(i)
        setup_module(get_module(i))
end

function max_modules()
        return getn(modules)
end

function get_module_name(j)
	return modules[j].name
end

function get_module_desc(j)
	return modules[j].desc
end

function get_module(j)
	return modules[j]
end

function find_module(name)
	-- If we got a file, we must first load it
	if strfind(name, ".team", 1, 1) then
		local mod = load_module(name)
		name = mod.name
	end
	if __modules_by_sname[name] then
		return __modules_by_sname[name]
	end
	return __modules_by_name[name]
end

function get_real_file_path(file)
	if __module_files[file] then
		return __module_files[file]
	else
		return file
	end
end

function load_module_dir(mod)
	-- Right hand tree walk
	if mod.require_modules then
		for dep, _ in mod.require_modules do
			load_module_dir(modules[find_module(dep)])
		end
	end

	if (mod.real_dir ~= "???") and (not fs.add_to_search_path(mod.real_dir, FALSE)) then
		quit(format("Module initialisation failed, could not set the search path to %s: %s.\n", mod.real_dir, fs.get_last_error()));
	end
end

function assign_current_module(name)
        current_module = get_module(find_module(name))

	-- Setup the search path, may need to load multiple dirs
	load_module_dir(current_module)
end

function get_module_info(type, subtype, ...)
	if subtype then
		if tag(current_module[type]) == TAG_FUNCTION then
			return call(current_module[type], merge_tables(merge_tables({}, {subtype}), arg))
		elseif current_module[type] and tag(current_module[type][subtype]) == TAG_FUNCTION then
			return call(current_module[type][subtype], arg)
		elseif current_module[type] then
			return current_module[type][subtype]
		end
	else
		if tag(current_module[type]) == TAG_FUNCTION then
			return call(current_module[type], {})
		else
			return current_module[type]
		end
	end
end

function module_savefile_loadable(savefile_mod, savefile_death)
        for _, e in current_module.mod_savefiles do
                if e[1] == savefile_mod then
                        if e[2] == "all" then
	                	return true
                        elseif e[2] == "alive" and savefile_death == FALSE then
                                return true
                        elseif e[2] == "dead" and savefile_death == true then
                                return true
                        end
                end
        end
        return FALSE
end

-- Loads a module from a team file
function load_module(real_dir)
	-- Add the dir to the search path
	fs.add_to_search_path(real_dir, FALSE)

	-- Only if the module realy contains the module.lua file
	if fs.get_real_dir("/module.lua") == real_dir then
		add_module_current_real_dir = real_dir
		tome_dofile_anywhere(TENGINE_DIR, "module.lua")
	else
		quit("Module team file: "..real_dir.." does not contain a module.lua file("..fs.get_real_dir("/module.lua").." ~= "..real_dir..")")
	end
	-- Remove it
	fs.remove_from_search_path(real_dir)

	-- Return the new module
	return modules[getn(modules)]
end

-- Note on the use of / instead of calling path_build
-- path_build is now useless and deprecated because of the physfs library
-- that provides FS abstraction, as such all file/dir names are written in an
-- unix syntax and converted latter by the abstraction layer
function scan_extra_modules(search_dir)
        local dirs = fs.enumerate_files(search_dir, FALSE)
	sort(dirs, function(a, b)
		if a == "tome" or a == "tome.team" then return 1
		elseif b == "tome" or b == "tome.team" then return nil
		else return a < b
		end
	end)
        for i = 1, getn(dirs) do
		local dir = search_dir.."/"..dirs[i]
		-- Make up the physical location
		local real_dir = fs.get_dependent_path(dir)

		-- If it is a directory, it is easy
		if fs.is_directory(dir) then
			local file = dir.."/module.lua"
        	       	if fs.file_exist(file) == true then
				add_module_current_real_dir = real_dir
       	        	        tome_dofile_anywhere(dir, "module.lua")
	                end
		-- If it is a team file(T-Engine Archived Module) it gets ugly
		elseif strfind(dir, ".team", 1, 1) then
			load_module(real_dir)
		end
		-- FIXME: Delete the string behind real_dir properly.
        end

	-- Now all modules are loaded
end

add_module_current_real_dir = "???"
function add_module(t)
	-- If an engine version is required, abide to it
	-- Tests for required modules are done afterwards because all modules need to be
	-- loaded first
	if t.require_engine then
		if not require_version.engine(t.require_engine[1], t.require_engine[2], t.require_engine[3]) then return end
	end

	if t.require_module then
		t.require_modules = {}
		t.require_modules[t.require_module[1]] = { t.require_module[2], t.require_module[3], t.require_module[4] }
	end

        assert(t.name, "No module name")
        assert(t.shortname, "No module short name(used for directories and such)")
        assert(type(t.version) == "table", "No module version")
        assert(t.desc, "No module desc")
        assert(t.author, "No module author")
        if not t.pack then
		t.mod_savefiles = t.mod_savefiles or { t.name }
	end

	assert(not t.layout, "Module "..t.name..": The layout section of modules is now useless. The virtual file system dymanicaly figures it out.")

	t.real_dir = add_module_current_real_dir

	local force_idx_mod = nil
        for _, e in modules do
                if type(e) == "table" and e.name == t.name then
			-- This is newer, forget about the last one
			if require_version.test_version(t.version, e.version) == true then
				force_idx_mod = _
	                        _ALERT("Module name already defined, using new one: "..t.name.."\n")
				break
			-- The other is newer, abord this one
			else
	                        _ALERT("Module name already defined, using previous one: "..t.name.."\n")
				return
			end
                end
        end

        if type(t.author) == "string" then
                t.author = { t.author, "unknown@unknown.net" }
        end

	if not t.pack then
        	for k, e in t.mod_savefiles do
                	if type(e) == "string" then t.mod_savefiles[k] = { e, "all" } end
	        end
	end
--[[
        if type(t.desc) == "table" then
                local d = ""
                for k, e in t.desc do
                        d = d .. e
                        if k < getn(t.desc) then
                                d = d .. "\n"
                        end
                end
                t.desc = d
        end
]]
        if not t.rand_quest then t.rand_quest = FALSE end
        if not t.C_quest then t.C_quest = FALSE end

        if not t.base_dungeon then t.base_dungeon = 0 end

        if not t.max_plev then t.max_plev = 50 end
        if not t.max_exp then t.max_exp = 99999999 end
        if not t.max_money then t.max_money = 999999999 end
        if not t.max_skill_overage then t.max_skill_overage = 9999999 end
        if not t.max_skill_level then t.max_skill_level = 50 end
        if not t.skill_per_level then t.skill_per_level = function() return 5 end end

        if not t.allow_birth then t.allow_birth = true end

	if t.pack then
		local old = __modules_next_pack
		if force_idx_mod then
			__modules_next_pack = force_idx_mod
		end
		modules[__modules_next_pack] = t
		__modules_by_name[t.name] = __modules_next_pack
		__modules_by_sname[t.shortname] = __modules_next_pack
		__modules_next_pack = __modules_next_pack - 1
		__modules_next_pack = old
	else
		if force_idx_mod then t.__index__ = force_idx_mod end
		if t.__index__ then
			modules[t.__index__] = t
			__modules_by_name[t.name] = t.__index__
			__modules_by_sname[t.shortname] = t.__index__
		else
		        tinsert(modules, t)
			t.__index__ = getn(modules)
			__modules_by_name[t.name] = getn(modules)
			__modules_by_sname[t.shortname] = getn(modules)
		end
	end
end
function add_pack(t)
	t.pack = true
	return add_module(t)
end

-- Loading modules done
-- Now must check for modules requirements
function check_module_dependencies(i)
	local mod = modules[i]

	if not mod.require_modules then return true end

	for module, version in mod.require_modules do
		local dep = find_module(module)

		-- Not present a all ? bah cant be good
		if not dep then return nil end
		local dep_mod = modules[dep]

		-- Wrong version ?
		if not require_version.test_version(dep_mod.version, version) then return nil end

		-- Ok check if for parents
		if not check_module_dependencies(dep) then return nil end
	end
	return true
end

function done_loading_modules()
	local new_modules = {}

	for i, m in modules do
		if type(m) == "table" then
			if check_module_dependencies(i) then
				if i < 0 then
					new_modules[i] = m
				else
					tinsert(new_modules, m)
					__modules_by_name[m.name] = getn(new_modules)
					__modules_by_sname[m.shortname] = getn(new_modules)
				end
			end
		end
	end
	-- Use only the trimmed down list
	modules = new_modules
end

-- Force calling a file from a specific module/pack(even if it
-- is shadowed by a file of the same name in an higher module)
function module_dofile(mod, dir, file)
	-- Sanity check
	assert(__modules_by_name[mod], "Unknown module or pack named "..mod)

	-- Save the old search path
	local old_paths = fs.get_search_path()

	-- Now remove them all
	for i = 1, getn(old_paths) do
		fs.remove_from_search_path(old_paths[i])
	end

	-- Just add the module directory to thesearch path
	mod = modules[__modules_by_name[mod]]
	fs.add_to_search_path(mod.real_dir, FALSE)

	-- Now call the file
	tome_dofile_anywhere(dir, file)

	-- Remove it
	fs.remove_from_search_path(mod.real_dir)

	-- Restore them
	for i = 1, getn(old_paths) do
		fs.add_to_search_path(old_paths[i], true)
	end
end

function modules_list_http_callback(cur_size, max_size)
	if max_size then
		local ret, wid, hgt = term.get_size()
		local size = (cur_size * 50) / max_size
		local str = strrep("*", size)..strrep("-", 50 - size)
		term.c_put_str(color.YELLOW, "["..str.."]", hgt / 2, (wid / 2) - 25)
		term.redraw_section((wid / 2) - 25, hgt / 2, (wid / 2) + 25, hgt / 2)

		max_size = ''..max_size
		while strlen(max_size) < 7 do
			max_size = max_size..' '
		end
		term.c_put_str(color.WHITE, "Cur Size: "..cur_size, 3, 0)
		term.c_put_str(color.WHITE, "Max Size: "..max_size, 4, 0)
		term.redraw_section(0, 3, wid - 1, 4)
	end

	term.inkey_scan = true
	if (term.inkey() ~= 0) then
		return true
	end
end

-- Displays a scrolling list
function display_list(y, x, h, w, title, list, begin, sel, sel_color,
					  getter, show_scrollbar)
	local l = create_list(getn(list))

	for i = 1, getn(list) do
		if getter then
			add_to_list(l, i - 1, getter(list[i], i))
		else
			add_to_list(l, i - 1, list[i])
		end
	end

	%display_list(y, x, h, w, title, l, getn(list),
				  begin - 1, sel - 1, sel_color, show_scrollbar)

	delete_list(l, getn(list))
end

function install_new_module(imod)
	local ret = nil
	local old_write = fs.get_write_dir()
	fs.set_write_dir(TOME_WRITE_ROOT..fs.TOME_USER_PATH)
	fs.mkdir("/"..game_engine.version[1].."."..game_engine.version[2].."/modules")

	local i, j, file_name = strfind(imod.is_remote, "__(.*)")
	assert(i, "invalid file module name")
	local data = net.http_download("T-Engine module download screen", net.HTTP_USER_AGENT, "modules.t-o-m-e.net", imod.is_remote, modules_list_http_callback)
	if data then
		local fff = fs.open("/"..game_engine.version[1].."."..game_engine.version[2].."/modules/"..file_name, "w")
		if fff then
			fs.write_data(fff, data, strlen(data))
			fs.close(fff)
			ret = fs.get_dependent_path("/"..game_engine.version[1].."."..game_engine.version[2].."/modules/"..file_name)
		end
	end
	fs.set_write_dir(old_write)

	return ret
end

function strlen_nocolor(s)
	s = gsub(s, "(#[wWrRbBgGsuUvodD])", "")
	return strlen(s)
end

function display_module_desc(desc, start_y, start_x, h, w)
	local y_inc = 0
	for i = 1, getn(desc) do
		local str = desc[i]

		local x = 0
		local last_space = 0
		local words = {}

		-- Break down into words
		while not nil do
		local space = strfind(str, " ", last_space + 1)
		if not space then
			tinsert(words, strsub(str, last_space + 1))
				break
			end
			tinsert(words, strsub(str, last_space + 1, space - 1))
			last_space = space
		end

		-- Output words and make sure no words get over the limit
		for j = 1, getn(words) do
			if x + strlen_nocolor(words[j]) + 1 < w then
				tbaby.display(words[j], y_inc + start_y, start_x + x)
				x = x + strlen_nocolor(words[j]) + 1
			else
				y_inc = y_inc + 1
				tbaby.display(words[j], y_inc + start_y, start_x)
				x = strlen_nocolor(words[j]) + 1
			end
		end

		y_inc = y_inc + 1
		if y_inc >= h then break end
	end
	return start_y + y_inc
end

-- Load babyface interface
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "baby.lua")

-- Get module information from the net
function update_modules_data()
	modules_data = net.http_download("T-Engine modules list download screen", net.HTTP_USER_AGENT, "modules.t-o-m-e.net", "/get_list.php?emajor="..game_engine.version[1].."&eminor="..game_engine.version[2], modules_list_http_callback)
	return modules_data
end

function handle_remote_modules(max, data)
	local remote_modules = bytecode.load(data)()

	tinsert(modules, { name = "#R--Remote Modules---" })
	for i = 1, getn(remote_modules) do
		local old_mod = find_module(remote_modules[i].name)
		local idx = max + i + 1
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
			tinsert(modules, new_mod)
		end
	end
	return max + getn(remote_modules) + 1
end

function show_modules(wid, hgt, sel, scroll)
	term.clear()
	term.c_put_str(color.YELLOW, "T-Engine Modules List", 0, (wid / 2) - 15)
	term.c_put_str(color.WHITE, "You must select which game you want to play.", 1, (wid / 2) - 27)
	display_list(3, 0, hgt, 20, "Modules", modules, scroll, sel, color.LIGHT_GREEN, function(e) return e.name end)

	draw_box(3, 21, hgt, wid - 22)
	term.c_put_str(color.LIGHT_BLUE, " "..modules[sel].name.." ", 3, 23)

	local y = 4
	term.put_str("Author:  "..modules[sel].author[1], y, 22)
	term.put_str("Version: "..modules[sel].version[1].."."..modules[sel].version[2].."."..modules[sel].version[3], y + 1, 22)
	y = y + 2
	if modules[sel].is_remote then
		term.c_put_str(color.LIGHT_RED, "This is a remote module.", y, 22)
		term.c_put_str(color.LIGHT_RED, "This means it is NOT installed on your computer.", y + 1, 22)
		term.c_put_str(color.LIGHT_RED, "If you select it it will automatically be installed.", y + 2, 22)
		y = y + 4
	end
	y = y + 1
	y = display_module_desc(modules[sel].desc, y, 22, hgt - y, wid - 22)
end

function select_module_screen(force)
	-- Get babyface config
	TOME_BABY_INTERFACE = esettings.get("engine.babyface.enable", 1) ~= 0

	-- Force a module ?
	if force then
		local mod = find_module(force)
		if mod then
			-- We are not in baby mode
			TOME_BABY_INTERFACE = nil
			return modules[mod].name
		end
	end

	done_loading_modules()

	-- The net subsystem will soon be required bythe module selection screen, so load it
	load_subsystem("net")

	if TOME_BABY_INTERFACE then
		return tbaby.start()
	end

	local ret, wid, hgt = term.get_size()
	term.clear()
	local names = {}
	local sel, scroll = 1, 1

	local max = max_modules()
	local max_real = max

	hgt = hgt - 4

	local modules_data = nil

	-- We should connect automatically only if it's in the settings
	-- No sense bothering the player with a question every time
	if (esettings.get("engine.network.always_connect", 0) ~= 0) then
		modules_data = update_modules_data()
	end

	local remote_modules
	local remote_modules_unhandled = nil

	if modules_data then
		remote_modules_unhandled = true
	end

	while not nil do
		if remote_modules_unhandled then
			max = handle_remote_modules(max_real, modules_data)
			remote_modules_unhandled = nil
		end

		show_modules(wid, hgt, sel, scroll)

		if not modules_data then
			term.c_put_str(color.LIGHT_WHITE, "Press m for remote modules.", 2, wid/2 - 18)
		end

		local key = strchar(term.inkey())

		if key == '2' then
			repeat
				sel = sel + 1
				if sel > getn(modules) then sel = getn(modules) end
				if sel >= scroll + hgt then scroll = scroll + 1 end
			until sel ~= max_real + 1
		elseif key == '8' then
			repeat
				sel = sel - 1
				if sel < 1 then sel = 1 end
				if sel < scroll then scroll = scroll - 1 end
			until sel ~= max_real + 1
		elseif key == 'm' and not modules_data then
			modules_data = update_modules_data()
			remote_modules_unhandled = true
		elseif key == '\r' then
			if modules[sel].is_remote then
				local mod_file = install_new_module(modules[sel])
				if mod_file then
					for i = max_real + 1, getn(modules) do modules[i] = nil end
					modules.n = max_real
					load_module(mod_file)
					max_real = getn(modules)
					max = handle_remote_modules(max_real, modules_data)
					sel = 1
				end
				-- FIXME: Delete the string behind mod_file properly.
			else
				return modules[sel].name
			end
		end
	end
end
