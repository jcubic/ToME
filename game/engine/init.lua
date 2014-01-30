--
-- This file is loaded at the initialisation of T-Engine
-- Load the system functions
--
-- global protection
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "globals.lua")

-- Colors
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "colors.lua")

-- Bytecode manipulation utils
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "bytecode.lua")

-- Name of globals to save
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "load.lua")

-- Various utils
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "util.lua")

-- Counters
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "counters.lua")

-- Stats definitions must be laoded early, for they are vital to most things
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "stats.lua")
tome_dofile_anywhere(TENGINE_DIR_DATA, "player/stats.lua")

-- Very thin xml parser(49 lines ;)
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "xml.lua")

-- Very thin lua table serializer
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "pickle.lua")

-- Permanent file code
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "perm.lua")

-- New flags
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "flags.lua")

-- Flavours subsystem
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "flavours.lua")

-- Info files helpers
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "info.lua")

-- Map files loader
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "mapload.lua")

-- Wizard commands
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "wizard.lua")

-- various vital helper code
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "windows.lua")
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "dialog.lua")
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "factions.lua")
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "options.lua")
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "charsheet.lua")
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "display.lua")
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "calendar.lua")
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "player.lua")
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "objects.lua")
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "random_objects.lua")
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "wish.lua")
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "hooks.lua")
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "monsters.lua")
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "powers.lua")
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "building.lua")
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "dungeon.lua")
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "s_aux.lua")
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "traps.lua")
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "combat.lua")
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "quests.lua")
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "memory.lua")
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "birth.lua")
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "timed_effects.lua")
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "obj_mana.lua")
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "devices.lua")
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "ai.lua")
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "sound.lua")
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "target.lua")
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "learning_keys.lua")
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "death.lua")
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "changelog.lua")
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "realtime.lua")
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "kill_wall.lua")

-- Load the ingame contextual help
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "help.lua")

-- let the store specific stuff happen!
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "stores.lua")

-- Object automizer
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "auto.lua")

--------------------------------------------------------------
--------------------------------------------------------------
--------------------------------------------------------------
-------------Here we load the non vital scripts---------------
-----------------------from lib/scpt--------------------------
--------------------------------------------------------------
--------------------------------------------------------------
tome_dofile("init.lua")

--
-- Load data non-optional (edit) files.  Even though these are module
-- files, they will have the same location in every module.
--

-- Load the symbol info
tome_dofile_anywhere(TENGINE_DIR_DATA, "symbols.lua")

-- Load the calendar
tome_dofile_anywhere(TENGINE_DIR_DATA, "calendar.lua")

-- Load the falvours info
tome_dofile_anywhere(TENGINE_DIR_DATA, "flavours.lua")

-- Load the character sheet dislpay
tome_dofile_anywhere(TENGINE_DIR_DATA, "display/charsheet.lua")

-- Load the screen display
tome_dofile_anywhere(TENGINE_DIR_DATA, "display/display.lua")

-- Add the birth sequence
tome_dofile_anywhere(TENGINE_DIR_DATA, "player/birth.lua")

-- Add player calculations
tome_dofile_anywhere(TENGINE_DIR_DATA, "player/calc.lua")

-- Add timed effects, if any
tome_dofile_anywhere(TENGINE_DIR_DATA, "player/timed_effects.lua", FALSE)

-- Add monster timed effects, if any
tome_dofile_anywhere(TENGINE_DIR_DATA, "monsters/monster_effects.lua", FALSE)

-- Add objects descriptions
tome_dofile_anywhere(TENGINE_DIR_DATA, "items/descs.lua")

-- Add musics&sounds if any
tome_dofile_anywhere("/media", "music/music.lua", FALSE)
tome_dofile_anywhere("/media", "sound/sound.lua", FALSE)


--
-- Load patches; this must be done after files in data/ are loaded
--
load_patches(nil)

--------------------------------------------------------------
--------------------------------------------------------------
--------------------------------------------------------------
--
-- Do not thouch after this line
--
tome_dofile_anywhere(TENGINE_DIR_ENGINE, "load2.lua")
