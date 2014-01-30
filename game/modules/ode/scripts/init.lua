--
-- This file is loaded at the initialisation of ToME
--

-- Add the slots
tome_dofile("slots.lua")

-- Add the game intro
tome_dofile("intro.lua")

-- Load the damage types
tome_dofile("dam_type.lua")

-- Load the objects
tome_dofile("objects.lua")

-- Load the monster memory
tome_dofile("memory.lua")

-- Load spells
tome_dofile_anywhere(TENGINE_DIR_DATA, "magic/spells.lua", FALSE)

-- Load the options
tome_dofile("options.lua")

-- Add the combat systems
tome_dofile("combat.lua")

-- Add the level generator
tome_dofile("generator.lua")

-- Subsystems
load_subsystem("no_town", { dungeon="Dungeon of Ode" level=1})
load_subsystem("player_life")
load_subsystem("quaff")
load_subsystem("resting", {
	stop_check = function()
		return player.chp() == player.mhp()
	end
})
