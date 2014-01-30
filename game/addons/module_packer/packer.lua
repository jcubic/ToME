module_packer.team_signature =
[[#!/bin/sh
#T-Engine Archived Module

# Note that ToME must be in the path(obviously) to work
tome3 -M"$0" $*
exit $?]]

function module_packer.get_all_files(files, path)
	for i in fs.enumerate_files(path, false) do
		-- Try to ignore silly stuff like auto saves, cvs and svn stuff, ..
		if strlower(i) ~= ".svn" and
		   strlower(i) ~= "cvs" and
		   strlower(i) ~= "{arch}" and
		   not starts_with(i, ".") and
		   not ends_with(i, "~") and
		   not ends_with(i, "#") then
			local fpath = path_build(path, i)

			if i == "changes.log" then
				local oldfpath = fpath
				i = "changes.txt"
				fpath = path_build(path, i)
				changelog.run(oldfpath, fpath)
			end

			if fs.is_directory(fpath) then
				module_packer.get_all_files(files, fpath)
			else
				files[flag_max_key(files) + 1] = strsub(fpath, 2)
			end
		end
	end
end

function module_packer.create(mod)
	-- Clean search path
	local old_search_path = fs.get_search_path()
	for i = 1, getn(old_search_path) do
		fs.remove_from_search_path(old_search_path[i])
	end
	fs.add_to_search_path(mod.real_dir, true)

	local files = flag_new()
	module_packer.get_all_files(files, "/")

	for i = 1, getn(old_search_path) do
		local dir = old_search_path[i]
		fs.add_to_search_path(dir, true)
	end

	local fff = fs.open("/"..mod.shortname..".team.sig", "wb")
	if fff then
		fs.write(fff, module_packer.team_signature)
		fs.close(fff)
		if fs.create_zip("/"..mod.shortname..".team", files, "/"..mod.shortname..".team.sig") then
			msg_box("Module packaged as "..fs.get_dependent_path("/"..mod.shortname..".team"))
		else
			msg_box("Could not pack module, sorry")
		end
	else
		msg_box("Could not create module archive, sorry")
	end
	flag_free(files, true)
end

function module_packer.run()
	module_packer.create(current_module)
end

hook(hook.DEBUG_COMMAND, function(d)
	if d == KTRL('P') then
		module_packer.run()
		return true
	end
end)
