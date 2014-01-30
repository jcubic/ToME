new_player_descriptor
{
	type = "class"
	name = "Archer"
	titles = { "Rock Thrower", "Slinger", "Great Slinger", "Tosser", "Bowman", "Great Bowman", "Great Bowman", "Archer", "Archer", "Great Archer", }
	desc = {
		"'Kill them before they see you' could be the motto of the archer class.",
		"As deadly with a bow as a warrior is with a sword.",
	}
	experience = 30
	blows =
	{
		mul = 4
		num = 4
		weight = 35
	}
	descriptor_choices =
	{
		subclass =
		{
			__ALL__ = "never"
			Archer = "allow"
			Ranger = "allow"
		}
	}
	abilities =
	{
		["Ammo creation"] = 2
	}
}

new_player_descriptor
{
	type = "subclass"
	name = "Archer"
	desc = {
		"'Kill them before they see you' could be the motto of the archer class.",
		"As deadly with a bow as a warrior is with a sword.",
	}
	stats = { [A_STR]=2, [A_INT]=1, [A_DEX]=2, [A_CON]=1, [A_CHR]=1, }
	skills =
	{
		["Archery"]         = { mods.add(1000), mods.add(850)  }
		["Bow-mastery"]     = { mods.add(0)   , mods.add(500)  }
		["Combat"]          = { mods.add(1000), mods.add(800)  }
		["Disarming"]       = { mods.add(1000), mods.add(900)  }
		["Magic"]           = { mods.add(0)   ,mods.add(200)   }
		["Magic-device"]    = { mods.add(0)   , mods.add(100)  }
		["Sling-mastery"]   = { mods.add(0)   , mods.add(500)  }
		["Sneakiness"]      = { mods.add(1000), mods.add(900)  }
		["Weaponmastery"]   = { mods.add(1000), mods.add(500)  }
	}
	starting_objects =
	{
		{ obj = lookup_kind(TV_LIGHT_ARMOUR, SV_SOFT_LEATHER_JACKET) },
		{ obj = lookup_kind(TV_BOW, SV_SHORT_BOW) },
		{ obj = lookup_kind(TV_ARROW, SV_BODKIN_ARROW) min=25 max=25 },
	}
}

new_player_descriptor
{
	type = "subclass"
	name = "Ranger"
	desc = {
		"Rangers are capable archers but are also trained in hand to hand combat",
		"and nature/conveyance/divination magic schools",
	}
	stats = { [A_STR]=2, [A_INT]=1, [A_DEX]=2, [A_CON]=1, [A_CHR]=1, }
	skills =
	{
		["Archery"]         = { mods.add(1000), mods.add(750)  }
		["Bow-mastery"]     = { mods.add(0)   , mods.add(300)  }
		["Combat"]          = { mods.add(1000), mods.add(800)  }
		["Conveyance"]      = { mods.add(0)   , mods.add(500)  }
		["Disarming"]       = { mods.add(1000), mods.add(1000) }
		["Divination"]      = { mods.add(0)   , mods.add(500)  }
		["Magic"]           = { mods.add(1000), mods.add(700)  }
		["Monster-lore"]    = { mods.add(0)   , mods.add(200)  }
		["Magic-device"]    = { mods.add(0)   , mods.add(100)  }
		["Nature"]          = { mods.add(0)   , mods.add(500)  }
		["Sling-mastery"]   = { mods.add(0)   , mods.add(300)  }
		["Sneakiness"]      = { mods.add(1000), mods.add(950)  }
		["Weaponmastery"]   = { mods.add(1000), mods.add(500)  }
	}
	starting_objects =
	{
		{ obj = lookup_kind(TV_LIGHT_ARMOUR, SV_SOFT_LEATHER_JACKET) },
		{ obj = lookup_kind(TV_SWORD, SV_RAPIER) },
		{ obj = lookup_kind(TV_SLING, SV_SHORT_SLING) },
		{ obj = lookup_kind(TV_SHOT, SV_SLING_STONE) min=20 max=20 },
	}
}
