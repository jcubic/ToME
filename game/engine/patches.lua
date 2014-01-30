-- Handle patch checking & loading
--[[
declare_globals{
	"patch_init",
	"patch_preload",
	"patch_dofile",
	"load_patches",
}
]]

-- Patch modules
__patch_modules = {}

function patch_version(name, version)
	assert(not __patch_modules[name], "Addon " .. name .. " already loaded!!!")
	__patch_modules[name] = version
end

function patchs_list()
	local k, e, first
	first = FALSE
	for k, e in __patch_modules do
		if first == FALSE then hook.print("\n\n  [Addons]\n") first = true end
		hook.print("\n "..k.." version "..e)
	end
	if first == true then hook.print("\n") end
end

function patchs_display()
	local k, e
	for k, e in __patch_modules do
		message("Addon: "..k.." version "..e)
	end
end


-- The dofile functions for each patch
patch_dofile = {}

-- No warning if no patch
patch_init = function() end

-- Now load patches
function load_patches(preload)
	-- Mount patches if needed
	-- .teap = T-Engine Archived Patches
	-- This means a .teap may contain more than one patch
	-- Each patch must be in it's own directory
        local dirs = fs.enumerate_files(TENGINE_DIR_PATCH, FALSE)
	for i = 1, getn(dirs) do
		local dir = path_build(TENGINE_DIR_PATCH, dirs[i])
		local real_dir = fs.get_dependent_path(dir)

		-- Is it a teap file ?
		if strfind(dir, ".teap", 1, 1) then
			-- We cannot use the 3-args add_to_search_path yet because we run before engine/utils.lua
			-- so the function redefinition has not been done yet
			fs.PHYSFS_specific_mount = "addons/"
			if not fs.add_to_search_path(real_dir, FALSE) then
				quit("Could not mount addon "..dir.."("..real_dir..")")
			end
			fs.PHYSFS_specific_mount = nil
		end
		-- FIXME: Delete the string behind real_dir properly.
	end

	-- Now load the patches
	local dirs = fs.enumerate_files(TENGINE_DIR_PATCH, FALSE)
	for i = 1, getn(dirs) do
		local dir = path_build(TENGINE_DIR_PATCH, dirs[i])
		local file = path_build(dir, "patch.lua")
		if fs.file_exist(file) then
			if fs.file_exist(path_build(dir, "preload")) == preload then
				patch_init = 0
				tome_dofile_anywhere(dir, "patch.lua", true)
				if unset_safe_globals then unset_safe_globals() end
				if patch_init == 0 then
					if set_safe_globals then set_safe_globals() end
					quit("Addon in "..file.." did not include a patch_init() function")
				else
					if unset_safe_globals then set_safe_globals() end

					-- create the dofile function
					patch_dofile[dirs[i]] = function(f)
						tome_dofile_anywhere(%dir, f, true)
					end

					local name, version, error = patch_init()
					if name == nil or version == nil or error then
						quit("Addon in "..file.." did not return valid name or version.\nIt must return name, version.\nError returned: "..error)
					end
					patch_version(name, version)
				end
			end
		end -- "patch.lua" exists
	end -- Loop over list of patch directories
end -- function load_patches()


------------------------------------------------------------
---------------------- Subsystems section ------------------
------------------------------------------------------------
__loaded_subsystems = {}
__subsystems_params = {}

--- @fn
-- @brief Loads a subsystem from either /scripts/subsystems/ /engine/subsystems/
function load_subsystem(name, args)
	-- If this subsystem was already loaded, do nothing
	if __loaded_subsystems[name] then return end

	__subsystems_params[name] = args

	if fs.file_exist(path_build(TENGINE_DIR_SCPT, "subsystems/"..name..".lua")) then
		tome_dofile_anywhere(TENGINE_DIR_SCPT, "subsystems/"..name..".lua")
	elseif fs.file_exist(path_build(TENGINE_DIR_ENGINE, "subsystems/"..name..".lua")) then
		tome_dofile_anywhere(TENGINE_DIR_ENGINE, "subsystems/"..name..".lua")
	else
		quit("Subsystem "..name.." not found!")
	end
	__loaded_subsystems[name] = true
end

--- @fn
-- @brief Gets a parameter from a subsystem
function get_subsystem_param(sub, param)
	if not __subsystems_params[sub] then return nil end
	return __subsystems_params[sub][param]
end

--- @fn
-- @brief Is a subsystem loaded
-- @param name String the subsystem to check
-- @return true if it is
function has_subsystem(name)
	return __loaded_subsystems[name]
end
