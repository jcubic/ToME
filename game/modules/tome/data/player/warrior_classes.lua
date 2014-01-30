new_player_descriptor
{
	type = "class"
	name = "Warrior"
	desc = {
		"Simple fighters, they hack away with their trusty weapon.",
	}
	blows =
	{
		mul = 5
		num = 4
		weight = 30
	}
	descriptor_choices =
	{
		subclass =
		{
			__ALL__ = "never"
			Warrior = "allow"
			Swordmaster = "allow"
			Axemaster = "allow"
			Haftedmaster = "allow"
			Polemaster = "allow"
			Unbeliever = "allow"
		}
	}
	abilities =
	{
		["Extra Max Blow(1)"] = 1
		["Extra Max Blow(2)"] = 1
	}
	starting_objects =
	{
		{ obj = lookup_kind(TV_HEAVY_ARMOUR, SV_SPLINT_MAIL) },
	}
}

new_player_descriptor
{
	type = "subclass"
	name = "Warrior"
	desc = {
		"Simple fighters, they hack away with their trusty weapon.",
	}
	stats = { [A_STR]=5, [A_INT]=-2, [A_WIS]=-2, [A_DEX]=2, [A_CON]=2, [A_CHR]=-1, }
	experience = 0
	skills =
	{
		["Antimagic"]       = { mods.add(0)   , mods.add(550)  }
		["Archery"]         = { mods.add(1000), mods.add(600)  }
		["Axe-mastery"]     = { mods.add(0)   , mods.add(400)  }
		["Combat"]          = { mods.add(2000), mods.add(800)  }
		["Disarming"]       = { mods.add(1000), mods.add(900)  }
		["Hafted-mastery"]  = { mods.add(0)   , mods.add(400)  }
		["Magic"]           = { mods.add(1000), mods.add(300)  }
		["Magic-device"]    = { mods.add(0)   , mods.add(150)  }
		["Polearm-mastery"] = { mods.add(0)   , mods.add(400)  }
		["Sneakiness"]      = { mods.add(1000), mods.add(900)  }
		["Spirituality"]    = { mods.add(1000), mods.add(400)  }
		["Sword-mastery"]   = { mods.add(0)   , mods.add(400)  }
		["Knife-mastery"]   = { mods.add(0)   , mods.add(400)  }
		["Weaponmastery"]   = { mods.add(1000), mods.add(850)  }
	}
	abilities =
	{
		["Berserk"] = 5
	}
	starting_objects =
	{
		{ obj = lookup_kind(TV_SWORD, SV_RAPIER) },
		{ obj = lookup_kind(TV_SHIELD, SV_WOODEN_SHIELD) },
	}
}

new_player_descriptor
{
	type = "subclass"
	name = "Swordmaster"
	desc = {
				"Fighters specialised in the use of swords",
	}
	stats = { [A_STR]=4, [A_INT]=-2, [A_WIS]=-2, [A_DEX]=3, [A_CON]=2, [A_CHR]=-1, }
	experience = 0
	skills =
	{
		["Antimagic"]       = { mods.add(0)   , mods.add(450)  }

		["Disarming"]       = { mods.add(1000), mods.add(900)  }
		["Magic"]           = { mods.add(1000), mods.add(300)  }
		["Magic-device"]    = { mods.add(0)   , mods.add(150)  }
		["Sneakiness"]      = { mods.add(1000), mods.add(900)  }
		["Spirituality"]    = { mods.add(1000), mods.add(400)  }

		["Combat"]          = { mods.add(2000), mods.add(800)  }
		["Weaponmastery"]   = { mods.add(1000), mods.add(850)  }
		["Sword-mastery"]   = { mods.add(0)   , mods.add(700)  }
		["Polearm-mastery"] = { mods.add(0)   , mods.add(200)  }
		["Hafted-mastery"]  = { mods.add(0)   , mods.add(200)  }
		["Axe-mastery"]     = { mods.add(0)   , mods.add(200)  }
		["Knife-mastery"]   = { mods.add(0)   , mods.add(200)  }
		["Archery"]         = { mods.add(1000), mods.add(300)  }
	}
	starting_objects =
	{
		{ obj = lookup_kind(TV_SWORD, SV_RAPIER) },
		{ obj = lookup_kind(TV_SHIELD, SV_WOODEN_SHIELD) },
	}
}

new_player_descriptor
{
	type = "subclass"
	name = "Axemaster"
	desc = {
		"Fighters specialised in the use of axes",
	}
	stats = { [A_STR]=4, [A_INT]=-2, [A_WIS]=-2, [A_DEX]=3, [A_CON]=2, [A_CHR]=-1, }
	experience = 0
	skills =
	{
		["Antimagic"]       = { mods.add(0)   , mods.add(450)  }

		["Disarming"]       = { mods.add(1000), mods.add(900)  }
		["Magic"]           = { mods.add(1000), mods.add(300)  }
		["Magic-device"]    = { mods.add(0)   , mods.add(150)  }
		["Sneakiness"]      = { mods.add(1000), mods.add(900)  }
		["Spirituality"]    = { mods.add(1000), mods.add(400)  }

		["Combat"]          = { mods.add(2000), mods.add(800)  }
		["Weaponmastery"]   = { mods.add(1000), mods.add(850)  }
		["Sword-mastery"]   = { mods.add(0)   , mods.add(200)  }
		["Polearm-mastery"] = { mods.add(0)   , mods.add(200)  }
		["Hafted-mastery"]  = { mods.add(0)   , mods.add(200)  }
		["Axe-mastery"]     = { mods.add(0)   , mods.add(700)  }
		["Knife-mastery"]   = { mods.add(0)   , mods.add(200)  }
		["Archery"]         = { mods.add(1000), mods.add(300)  }
	}
	starting_objects =
	{
		{ obj = lookup_kind(TV_AXE, SV_FLINT_AXE) },
		{ obj = lookup_kind(TV_SHIELD, SV_WOODEN_SHIELD) },
	}
}

new_player_descriptor
{
	type = "subclass"
	name = "Haftedmaster"
	desc = {
		"Fighters specialised in the use of hafted weapons",
	}
	stats = { [A_STR]=4, [A_INT]=-2, [A_WIS]=-2, [A_DEX]=3, [A_CON]=2, [A_CHR]=-1, }
	experience = 0
	skills =
	{
		["Antimagic"]       = { mods.add(0)   , mods.add(450)  }

		["Disarming"]       = { mods.add(1000), mods.add(900)  }
		["Magic"]           = { mods.add(1000), mods.add(300)  }
		["Magic-device"]    = { mods.add(0)   , mods.add(150)  }
		["Sneakiness"]      = { mods.add(1000), mods.add(900)  }
		["Spirituality"]    = { mods.add(1000), mods.add(400)  }

		["Combat"]          = { mods.add(2000), mods.add(800)  }
		["Weaponmastery"]   = { mods.add(1000), mods.add(850)  }
		["Sword-mastery"]   = { mods.add(0)   , mods.add(200)  }
		["Polearm-mastery"] = { mods.add(0)   , mods.add(200)  }
		["Hafted-mastery"]  = { mods.add(0)   , mods.add(700)  }
		["Axe-mastery"]     = { mods.add(0)   , mods.add(200)  }
		["Knife-mastery"]   = { mods.add(0)   , mods.add(200)  }
		["Archery"]         = { mods.add(1000), mods.add(300)  }
	}
	starting_objects =
	{
		{ obj = lookup_kind(TV_HAFTED, SV_CLUB) },
		{ obj = lookup_kind(TV_SHIELD, SV_WOODEN_SHIELD) },
	}
}

new_player_descriptor
{
	type = "subclass"
	name = "Pikeman"
	desc = {
		"Fighters specialised in the use of polearms",
	}
	stats = { [A_STR]=4, [A_INT]=-2, [A_WIS]=-2, [A_DEX]=3, [A_CON]=2, [A_CHR]=-1, }
	experience = 0
	skills =
	{
		["Antimagic"]       = { mods.add(0)   , mods.add(450)  }

		["Disarming"]       = { mods.add(1000), mods.add(900)  }
		["Magic"]           = { mods.add(1000), mods.add(300)  }
		["Magic-device"]    = { mods.add(0)   , mods.add(150)  }
		["Sneakiness"]      = { mods.add(1000), mods.add(900)  }
		["Spirituality"]    = { mods.add(1000), mods.add(400)  }

		["Combat"]          = { mods.add(2000), mods.add(800)  }
		["Weaponmastery"]   = { mods.add(1000), mods.add(850)  }
		["Sword-mastery"]   = { mods.add(0)   , mods.add(200)  }
		["Polearm-mastery"] = { mods.add(0)   , mods.add(700)  }
		["Hafted-mastery"]  = { mods.add(0)   , mods.add(200)  }
		["Axe-mastery"]     = { mods.add(0)   , mods.add(200)  }
		["Knife-mastery"]   = { mods.add(0)   , mods.add(200)  }
		["Archery"]         = { mods.add(1000), mods.add(300)  }
	}
	starting_objects =
	{
		{ obj = lookup_kind(TV_POLEARM, SV_PIKE) },
	}
}

new_player_descriptor
{
	type = "subclass"
	name = "Unbeliever"
	desc = {
		"Fighters who renounced both gods and magic.",
	}
	stats = { [A_STR]=5, [A_INT]=-2, [A_WIS]=-2, [A_DEX]=2, [A_CON]=2, [A_CHR]=-1, }
	experience = 0
	skills =
	{
		["Antimagic"]       = { mods.add(1000), mods.add(900)  }
		["Archery"]         = { mods.add(1000), mods.add(600)  }
		["Axe-mastery"]     = { mods.add(0)   , mods.add(300)  }
		["Combat"]          = { mods.add(2000), mods.add(800)  }
		["Disarming"]       = { mods.add(1000), mods.add(900)  }
		["Hafted-mastery"]  = { mods.add(0)   , mods.add(300)  }
		["Magic"]           = { mods.eq(0)    , mods.eq(0)     }
		["Magic-device"]    = { mods.eq(0)    , mods.eq(0)     }
		["Polearm-mastery"] = { mods.add(0)   , mods.add(300)  }
		["Prayer"]          = { mods.eq(0)    , mods.eq(0)     }
		["Sneakiness"]      = { mods.add(1000), mods.add(900)  }
		["Spirituality"]    = { mods.add(2000), mods.add(500)  }
		["Sword-mastery"]   = { mods.add(0)   , mods.add(300)  }
		["Knife-mastery"]   = { mods.add(0)   , mods.add(300)  }
		["Weaponmastery"]   = { mods.add(1000), mods.add(850)  }
	}
	starting_objects =
	{
		{ obj = function(obj)
			object_prep(obj, TV_KNIFE, SV_KNIFE)
			obj.ego_id[5] = EGO_WEAPON_DARKSTONE
			apply_magic(obj, 0, false, false, false)
			-- Make sure the bonus are set to 0
			obj.to_d = 0
			obj.to_h = 0
			obj.flags[FLAG_ANTIMAGIC] = 50
		  end},
	}
}


