-- Load the engine settings
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "engine.lua")

-- Load core patches
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "patches.lua")
load_patches(true)

-- Add the downloaded modules location to the search path
fs.add_to_search_path(TOME_WRITE_ROOT..fs.TOME_USER_PATH.."/"..game_engine.version[1].."."..game_engine.version[2].."/", FALSE)

-- Look for packs
scan_extra_modules("/packs")

-- Look for more modules
scan_extra_modules(TENGINE_DIR_MODULES)

-- Remove the path used for downloads
fs.remove_from_search_path(TOME_WRITE_ROOT..fs.TOME_USER_PATH.."/"..game_engine.version[1].."."..game_engine.version[2].."/")
