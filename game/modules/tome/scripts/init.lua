--
-- This file is loaded at the initialisation of ToME
--

-----------------------------------------------------------------
-- Load the various subsystems we want
-----------------------------------------------------------------
load_subsystem("player_life")
load_subsystem("player_mana")
load_subsystem("player_sanity")
load_subsystem("hunger")
load_subsystem("quaff")
load_subsystem("read")
load_subsystem("movement_mode")
load_subsystem("no_wilderness_continuity")
load_subsystem("gods")
load_subsystem("battle_mode")
load_subsystem("temporary_terrain")
load_subsystem("advdialog")

-----------------------------------------------------------------
-- Load the ToME scripts
-----------------------------------------------------------------

-- Add inventory slots
tome_dofile("slots.lua")

-- Add the colors
tome_dofile("colors.lua")

-- Add the factions
tome_dofile("factions.lua")

-- Add the game intro
tome_dofile("intro.lua")

-- Load the damage types
tome_dofile("dam_type.lua")

-- Load the flags
tome_dofile("flags.lua")

-- Load the options
tome_dofile("options.lua")

-- Module specific stuff monster memory should be aware of
tome_dofile("memory.lua")

-- Add object scripts
tome_dofile("object.lua")

-- Load the player helpers
tome_dofile("player.lua")

-- Add the riding system
tome_dofile("riding.lua")

-- Add the combat systems
tome_dofile("combat.lua")

-- Add the possession system
tome_dofile("possession.lua")

-- Add the symbiosis system
tome_dofile("symbiosis.lua")

-- Add the summoning system
tome_dofile("summoning.lua")

-- Add the ingame help system
tome_dofile("help.lua")

-- Add sticks
tome_dofile("sticks.lua")

-- Add dungeon healpers
tome_dofile("dungeons.lua")

-- Add feature helpers
tome_dofile("features.lua")

-- Add sentient objects system
tome_dofile("sentient.lua")

-- Add wilderness encounters
tome_dofile("wilderness/travel.lua")
tome_dofile("wilderness/encounters.lua")

-- Targeting
tome_dofile("target.lua")

-- Load custom AIs
tome_dofile("ai/init.lua")

-- Load fates
tome_dofile("fates.lua")

-- Load display helpers
tome_dofile("display.lua")

-----------------------------------------------------------------
-- Load the various non raw-ified lua files in data/
-----------------------------------------------------------------

-- Add the mimic shapes
load_subsystem("mimicry")

-- Add the corruptions
load_subsystem("corruption")

-- Add the schools of gods(optional)
tome_dofile_anywhere(TENGINE_DIR_DATA, "gods/gods.lua", false)

-- Add the schools of magic(optional)
tome_dofile_anywhere(TENGINE_DIR_DATA, "magic/spells.lua", false)

-- Add traps(optional)
tome_dofile_anywhere(TENGINE_DIR_DATA, "dungeon/traps.lua",    false)
tome_dofile_anywhere(TENGINE_DIR_DATA, "dungeon/trapkits.lua", false)

-- Powers
tome_dofile("powers.lua")

-----------------------------------------------------------------
-- Load quests
-----------------------------------------------------------------
tome_dofile("quests/init.lua")

-----------------------------------------------------------------
-- Load monster dialogs
-----------------------------------------------------------------
tome_dofile("dialogs.lua")

hook(hook.GAME_START, function()
	local tbl = {}
	local sorted = {}
	for i = 1, max_r_idx do
		local race = r_info[i]
		if not tbl[race.level] then
			tbl[race.level] = { level=race.level, num=0 }
			tinsert(sorted, tbl[race.level])
		end
		tbl[race.level].num = tbl[race.level].num + 1
	end
	sort(sorted, function(a, b) return a.level < b.level end)
	local total = 0
	for i = 1, getn(sorted) do
		total = total + sorted[i].num
		print("Number of monsters for level "..sorted[i].level.." : "..sorted[i].num)
	end
	print("Total monsters :" .. total)
end)
