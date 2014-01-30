new_player_descriptor
{
	type = "class"
	name = "Rogue"
	titles = { "Cutpurse", "Robber", "Burglar", "Filcher", "Sharper", "Low Thief", "High Thief", "Master Thief", "Assassin", "Guildmaster", }
	desc = {
		"Rogues are masters of tricks, they can steal from shops and monsters",
		"and lure monsters into deadly traps.",
	}
	stats = { [A_STR]=2, [A_INT]=1, [A_WIS]=-2, [A_DEX]=3, [A_CON]=1, [A_CHR]=-1, }
	blows =
	{
		mul = 3
		num = 4
		weight = 30
	}
	descriptor_choices =
	{
		subclass =
		{
			__ALL__ = "never"
			Rogue = "allow"
			Assassin = "allow"
		}
	}
	starting_objects =
	{
		{ obj = lookup_kind(TV_KNIFE, SV_MISERICORDE) },
		{ obj = lookup_kind(TV_KNIFE, SV_KNIFE) },
	}
}

new_player_descriptor
{
	type = "subclass"
	name = "Rogue"
	desc = {
		"Rogues are masters of tricks, they can steal from shops and monsters",
		"and lure monsters into deadly traps.",
	}
	skills =
	{
		["Backstab"]        = { mods.add(1000), mods.add(1000) }
		["Combat"]          = { mods.add(1000), mods.add(700)  }
		["Conveyance"]      = { mods.add(0)   , mods.add(500)  }
		["Critical-hits"]   = { mods.add(1000), mods.add(500)  }
		["Disarming"]       = { mods.add(1000), mods.add(2000) }
		["Divination"]      = { mods.add(0)   , mods.add(500)  }
		["Dodging"]         = { mods.add(1000), mods.add(2000) }
		["Magic"]           = { mods.add(1000), mods.add(700)  }
		["Magic-device"]    = { mods.add(0)   , mods.add(550)  }
		["Sneakiness"]      = { mods.add(1000), mods.add(2000) }
		["Spirituality"]    = { mods.add(1000), mods.add(700)  }
		["Stealing"]        = { mods.add(1000), mods.add(2000) }
		["Stealth"]         = { mods.add(1000), mods.add(1500) }
		["Knife-mastery"]   = { mods.add(1000), mods.add(700)  }
		["Temporal"]        = { mods.add(0)   , mods.add(500)  }
		["Weaponmastery"]   = { mods.add(1000), mods.add(700)  }
	}
	abilities =
	{
		["Extra Max Blow(1)"] = 10
	}
}

new_player_descriptor
{
	type = "subclass"
	name = "Assassin"
	desc = {
		"Assassins are stealthy killers.",
	}
}
