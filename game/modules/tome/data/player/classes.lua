tome_dofile_anywhere("/data/player/", "warrior_classes.lua")
tome_dofile_anywhere("/data/player/", "mage_classes.lua")
tome_dofile_anywhere("/data/player/", "archer_classes.lua")
tome_dofile_anywhere("/data/player/", "rogue_classes.lua")
tome_dofile_anywhere("/data/player/", "lore_classes.lua")

--[[
new_player_descriptor
{
	type = "class"
	name = "Priest"
	titles = { "Believer", "Acolyte", "Adept", "Curate", "Canon", "Priest", "High Priest", "Cardinal", "Inquisitor", "Pope", }
	desc = {
		"A priest serves a god (Vala, Maia or Eru himself) to bring down",
		"the empire of fear and shadows of Morgoth.",
	}
	stats = { [A_STR]=-1, [A_INT]=-3, [A_WIS]=3, [A_DEX]=-1, [A_CHR]=2, }
	life_rating = 2
	experience = 20
	blows =
	{
		mul = 3
		num = 4
		weight = 35
	}
--DGDGDGDG	powers = { "detect curses", }
	flags = { GOD_FRIEND=1 }
	skills =
	{
		["Combat"]          = { mods.add(2000), mods.add(700)  }
		["Magic"]           = { mods.add(1000), mods.add(900)  }
		["Magic-device"]    = { mods.add(0)   , mods.add(50)   }
		["Mindcraft"]       = { mods.add(0)   , mods.add(600)  }
		["Necromancy"]      = { mods.add(0)   , mods.add(600)  }
		["Prayer"]          = { mods.add(1000), mods.add(700)  }
		["Sneakiness"]      = { mods.add(1000), mods.add(900)  }
		["Spell-power"]     = { mods.add(0)   , mods.add(600)  }
		["Spirituality"]    = { mods.add(1000), mods.add(1000) }
		["Weaponmastery"]   = { mods.add(1000), mods.add(700)  }
	}
	abilities =
	{
		["Perfect casting"] = 1
	}
}
]]
