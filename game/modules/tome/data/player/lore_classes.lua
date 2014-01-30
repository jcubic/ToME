new_player_descriptor
{
	type = "class"
	name = "Loremaster"
	titles = { "Apprentice", "Apprentice", "Initiate", "Initiate", "Sage", "Sage", "Lorekeeper", "Lorekeeper", "Loremaster", "Loremaster", }
	desc = {
		"Loremasters are skilled in most combat and monster skills",
	}
	blows =
	{
		mul = 3
		num = 4
		weight = 30
	}
	descriptor_choices =
	{
		subclass =		{
			__ALL__ = "never"
			Loremaster = "allow"
			Possessor  = "allow"
			--Mimic      = "allow"
			Symbiant   = "allow"
			Summoner   = "allow"
			--Monk       = "allow"
			--Bard       = "allow"
		}
	}
}

new_player_descriptor
{
	type = "subclass"
	name = "Loremaster"

	desc = {
		"Loremasters are skilled in most combat and monster skills",
	}
	stats = { [A_STR]=1, [A_INT]=-2, [A_WIS]=1, [A_DEX]=1, [A_CHR]=1, }
	life_rating = 8
	experience = 40
	allowed_gods = "All Gods"
	skills =
	{
		["Archery"]         = { mods.add(2000), mods.add(700)  }
		["Barehand-combat"] = { mods.add(2000), mods.add(700)  }
		["Combat"]          = { mods.add(2000), mods.add(700)  }
		["Corpse-preservation"] = { mods.add(2000), mods.add(700)  }
		["Disarming"]       = { mods.add(2000), mods.add(700)  }
		["Magic"]           = { mods.add(0)   , mods.add(600)  }
		["Magic-device"]    = { mods.add(2000), mods.add(1000) }
		["Mimicry"]         = { mods.add(0)   , mods.add(500)  }
		["Monster-lore"]    = { mods.add(2000), mods.add(600)  }
		["Music"]           = { mods.add(0)   , mods.add(300)  }
		["Possession"]      = { mods.add(0)   , mods.add(500)  }
		["Sneakiness"]      = { mods.add(2000), mods.add(700)  }
		["Spirituality"]    = { mods.add(2000), mods.add(700)  }
		["Stealth"]         = { mods.add(2000), mods.add(700)  }
		["Summoning"]       = { mods.add(0)   , mods.add(500)  }
		["Symbiosis"]       = { mods.add(0)   , mods.add(500)  }
		["Weaponmastery"]   = { mods.add(2000), mods.add(700)  }
	}

	starting_objects =
	{
		{ obj = lookup_kind(TV_HAFTED, SV_CLUB) },
		{ obj = lookup_kind(TV_LIGHT_ARMOUR, SV_HARD_LEATHER_JACKET) },
		{ obj = lookup_kind(TV_SLING, SV_SHORT_SLING) },
		{ obj = lookup_kind(TV_SHOT, SV_SLING_STONE) min=25 max=25},
	}
}

new_player_descriptor
{
	type = "subclass"
	name = "Possessor"

	desc = {
		"Only the soul matters; a possesor can abandon his/her current body",
		"to incarnate in the body of a dead monster, thus gaining its powers",
		"and weaknesses."
	}
	stats = { [A_STR]=1, [A_INT]=-2, [A_WIS]=1, [A_DEX]=1, [A_CHR]=1, }
	life_rating = 8
	experience = 40
	allowed_gods = "All Gods"
	skills =
	{
		["Archery"]         = { mods.add(2000), mods.add(400)  }
		["Combat"]          = { mods.add(2000), mods.add(600)  }
		["Corpse-preservation"] = { mods.add(2000), mods.add(900)  }
		["Disarming"]       = { mods.add(2000), mods.add(500)  }
		["Magic"]           = { mods.add(0)   , mods.add(600)  }
		["Magic-device"]    = { mods.add(2000), mods.add(1000) }
		["Mimicry"]         = { mods.add(0)   , mods.add(500)  }
		["Monster-lore"]    = { mods.add(2000), mods.add(600)  }
		["Music"]           = { mods.add(0)   , mods.add(300)  }
		["Possession"]      = { mods.add(2000), mods.add(800)  }
		["Sneakiness"]      = { mods.add(2000), mods.add(700)  }
		["Spirituality"]    = { mods.add(2000), mods.add(500)  }
		["Stealth"]         = { mods.add(2000), mods.add(700)  }
		["Weaponmastery"]   = { mods.add(2000), mods.add(600)  }
	}

	starting_objects =
	{
		{ obj = lookup_kind(TV_SWORD, SV_RAPIER) },
		{ obj = lookup_kind(TV_LIGHT_ARMOUR, SV_HARD_LEATHER_JACKET) },
		{ obj = lookup_kind(TV_POTION, SV_POTION_HEALING) },
	}
}

new_player_descriptor
{
	type = "subclass"
	name = "Symbiant"

	desc = {
		"A symbiant can merge his/her body with one of a monster unable to",
		"move by itself.  They also have a few spells to help the symbiosis."
	}
	stats = { [A_STR]=1, [A_INT]=-2, [A_WIS]=1, [A_DEX]=1, [A_CHR]=1, }
	life_rating = 8
	experience = 40
	allowed_gods = "All Gods"
	skills =
	{
		["Archery"]         = { mods.add(2000), mods.add(700)  }
		["Barehand-combat"] = { mods.add(2000), mods.add(600)  }
		["Combat"]          = { mods.add(2000), mods.add(800)  }
		["Corpse-preservation"] = { mods.add(2000), mods.add(900)  }
		["Disarming"]       = { mods.add(2000), mods.add(700)  }
		["Magic"]           = { mods.add(0)   , mods.add(700)  }
		["Magic-device"]    = { mods.add(2000), mods.add(1000) }
		["Mimicry"]         = { mods.add(0)   , mods.add(500)  }
		["Monster-lore"]    = { mods.add(2000), mods.add(110)  }
		["Music"]           = { mods.add(0)   , mods.add(300)  }
		["Possession"]      = { mods.add(0)   , mods.add(100)  }
		["Sneakiness"]      = { mods.add(2000), mods.add(800)  }
		["Spirituality"]    = { mods.add(2000), mods.add(500)  }
		["Stealth"]         = { mods.add(2000), mods.add(700)  }
		["Symbiosis"]       = { mods.add(2000), mods.add(800)  }
		["Weaponmastery"]   = { mods.add(2000), mods.add(800)  }
	}

	starting_objects =
	{
		{ obj = lookup_kind(TV_KNIFE, SV_DAGGER) },
		{ obj = lookup_kind(TV_SCROLL, SV_SCROLL_SUMMON_SYMBIOTE) },
	}
}


new_player_descriptor
{
	type = "subclass"
	name = "Summoner"

	desc = {
		"The summoner can conjure monsters from totems made from defeated",
		"foes."
	}
	stats = { [A_STR]=1, [A_INT]=-2, [A_WIS]=1, [A_DEX]=1, [A_CHR]=1, }
	life_rating = 8
	experience = 40
	allowed_gods = "All Gods"
	skills =
	{
		["Archery"]         = { mods.add(2000) , mods.add(400)  }
		["Combat"]          = { mods.add(2000) , mods.add(600)  }
		["Corpse-preservation"] = { mods.add(2000), mods.add(1000)  }
		["Disarming"]       = { mods.add(2000) , mods.add(500)  }
		["Magic"]           = { mods.add(0)    , mods.add(800)  }
		["Magic-device"]    = { mods.add(2000) , mods.add(1000) }
		["Mimicry"]         = { mods.add(0)    , mods.add(500)  }
		["Monster-lore"]    = { mods.add(32000), mods.add(1200)  }
		["Music"]           = { mods.add(0)    , mods.add(300)  }
		["Possession"]      = { mods.add(0)    , mods.add(100)  }
		["Sneakiness"]      = { mods.add(2000) , mods.add(700)  }
		["Spirituality"]    = { mods.add(2000) , mods.add(500)  }
		["Stealth"]         = { mods.add(2000) , mods.add(700)  }
		["Summoning"]       = { mods.add(1000) , mods.add(700)  }
		["Weaponmastery"]   = { mods.add(2000) , mods.add(600)  }
	}

	starting_objects =
	{
		{ obj = lookup_kind(TV_SWORD, SV_RAPIER) },
		{ obj = lookup_kind(TV_LIGHT_ARMOUR, SV_HARD_LEATHER_JACKET) },
		{ obj = lookup_kind(TV_POTION, SV_POTION_HEALING) },
	}
}
