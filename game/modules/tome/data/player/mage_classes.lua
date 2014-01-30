new_player_descriptor
{
	type = "class"
	name = "Mage"
	titles = { "Apprentice", "Trickster", "Illusionist", "Spellbinder", "Evoker", "Conjurer", "Warlock", "Sorcerer", "Ipsissimus", "Archimage", }
	desc = {
		"The basic spellcaster with lots of different skills",
	}
	blows =
	{
		mul = 2
		num = 4
		weight = 40
	}
	descriptor_choices =
	{
		subclass =
		{
			__ALL__ = "never"
			Mage = "allow"
			Geomancer = "allow"
			Warper = "allow"
			Archmage = "allow"
			Necromancer = "allow"
			Runecrafter = "allow"
			Thaumaturgist = "allow"
			Demonologist = "allow"
--			Alchemist = "allow"
		}
	}
	abilities =
	{
		["Perfect casting"] = 1
	}
}

new_player_descriptor
{
	type = "subclass"
	name = "Mage"
	desc = {
		"The basic spellcaster with lots of different skills",
	}
	stats = { [A_STR]=-3, [A_INT]=3, [A_DEX]=1, [A_CON]=-2, [A_CHR]=1, }
	mana = 50
	skills =
	{
		["Air"]             = { mods.add(0)   , mods.add(1000) }
		["Combat"]          = { mods.add(1000), mods.add(500)  }
		["Conveyance"]      = { mods.add(1000), mods.add(1000) }
		["Divination"]      = { mods.add(0)   , mods.add(1000) }
		["Earth"]           = { mods.add(0)   , mods.add(1000) }
		["Fire"]            = { mods.add(0)   , mods.add(1000) }
		["Magic"]           = { mods.add(1000), mods.add(900)  }
		["Magic-device"]    = { mods.add(0)   , mods.add(200)  }
		["Mana"]            = { mods.add(1000), mods.add(1000)  }
		["Meta"]            = { mods.add(0)   , mods.add(1000) }
		["Mind"]            = { mods.add(0)   , mods.add(1000) }
		["Nature"]          = { mods.add(0)   , mods.add(1000) }
		["Necromancy"]      = { mods.add(0)   , mods.add(700)  }
		["Runecraft"]       = { mods.add(0)   , mods.add(700)  }
		["Sneakiness"]      = { mods.add(1000), mods.add(900)  }
		["Sorcery"]         = { mods.add(0)   , mods.add(300)  }
		["Spell-power"]     = { mods.add(0)   , mods.add(600)  }
		["Spirituality"]    = { mods.add(1000), mods.add(550)  }
		["Temporal"]        = { mods.add(1000), mods.add(1000) }
		["Thaumaturgy"]     = { mods.add(0)   , mods.add(700)  }
		["Water"]           = { mods.add(0)   , mods.add(1000) }
		["Weaponmastery"]   = { mods.add(700) , mods.add(500)  }
	}
	starting_objects =
	{
		{ obj = lookup_kind(TV_LIGHT_ARMOUR, SV_ROBE) },
		{ obj = lookup_kind(TV_MSTAFF, SV_MAGE_STAFF) },
		{ obj = lookup_kind(TV_BOOK, SV_BOOK_CANTRIPS) },
	}
}

new_player_descriptor
{
	type = "subclass"
	name = "Geomancer"
	desc = {
		"The master of the four elements",
	}
	stats = { [A_STR]=-3, [A_INT]=3, [A_DEX]=1, [A_CON]=-2, [A_CHR]=1, }
	mana = 50
	skills =
	{
		["Air"]             = { mods.add(1000), mods.add(1100) }
		["Combat"]          = { mods.add(1000), mods.add(500)  }
		["Conveyance"]      = { mods.add(0)   , mods.add(800)  }
		["Divination"]      = { mods.add(0)   , mods.add(800)  }
		["Earth"]           = { mods.add(1000), mods.add(1100) }
		["Fire"]            = { mods.add(1000), mods.add(1100) }
		["Geomancy"]        = { mods.add(1000), mods.add(700)  }
		["Magic"]           = { mods.add(1000), mods.add(900)  }
		["Magic-device"]    = { mods.add(0)   , mods.add(50)   }
		["Meta"]            = { mods.add(0)   , mods.add(800)  }
		["Mind"]            = { mods.add(0)   , mods.add(700)  }
		["Nature"]          = { mods.add(0)   , mods.add(800)  }
		["Necromancy"]      = { mods.add(0)   , mods.add(700)  }
		["Runecraft"]       = { mods.add(0)   , mods.add(700)  }
		["Sneakiness"]      = { mods.add(1000), mods.add(900)  }
		["Spell-power"]     = { mods.add(1000), mods.add(700)  }
		["Spirituality"]    = { mods.add(1000), mods.add(550)  }
		["Temporal"]        = { mods.add(0)   , mods.add(800)  }
		["Thaumaturgy"]     = { mods.add(0)   , mods.add(700)  }
		["Water"]           = { mods.add(1000), mods.add(1100) }
		["Weaponmastery"]   = { mods.add(700) , mods.add(400)  }
	}
	starting_objects =
	{
		{ obj = lookup_kind(TV_LIGHT_ARMOUR, SV_ROBE) },
		-- A water orb with 1 skill bonus
		{ obj=function(obj) object_prep(obj, TV_MSTAFF, SV_FLOWING_STAFF) obj.flags[FLAG_SKILL_BONUS][SKILL_WATER] = 1000 end },
		{ obj=lookup_kind(TV_BOOK, SV_BOOK_BASIC_ELEMENTS) },
	}
}

new_player_descriptor
{
	type = "subclass"
	name = "Warper"
	desc = {
		"The master of space and time",
	}
	stats = { [A_STR]=-3, [A_INT]=3, [A_DEX]=1, [A_CON]=-2, [A_CHR]=1, }
	mana = 50
	skills =
	{
		["Air"]             = { mods.add(0)   , mods.add(800)  }
		["Combat"]          = { mods.add(1000), mods.add(500)  }
		["Conveyance"]      = { mods.add(0)   , mods.add(1200) }
		["Divination"]      = { mods.add(0)   , mods.add(1200) }
		["Earth"]           = { mods.add(0)   , mods.add(800)  }
		["Fire"]            = { mods.add(0)   , mods.add(800)  }
		["Magic"]           = { mods.add(1000), mods.add(900)  }
		["Magic-device"]    = { mods.add(0)   , mods.add(50)   }
		["Mana"]            = { mods.add(1000), mods.add(800)  }
		["Meta"]            = { mods.add(0)   , mods.add(900)  }
		["Mind"]            = { mods.add(0)   , mods.add(800)  }
		["Nature"]          = { mods.add(0)   , mods.add(800)  }
		["Necromancy"]      = { mods.add(0)   , mods.add(700)  }
		["Runecraft"]       = { mods.add(0)   , mods.add(700)  }
		["Sneakiness"]      = { mods.add(1000), mods.add(900)  }
		["Spell-power"]     = { mods.add(1000), mods.add(700)  }
		["Spirituality"]    = { mods.add(1000), mods.add(550)  }
		["Temporal"]        = { mods.add(0)   , mods.add(1200) }
		["Thaumaturgy"]     = { mods.add(0)   , mods.add(700)  }
		["Water"]           = { mods.add(0)   , mods.add(700)  }
		["Weaponmastery"]   = { mods.add(700) , mods.add(400)  }
	}
	starting_objects =
	{
		{ obj = lookup_kind(TV_LIGHT_ARMOUR, SV_ROBE) },
		{ obj = lookup_kind(TV_MSTAFF, SV_MAGE_STAFF) },
		{ obj = lookup_kind(TV_BOOK, SV_BOOK_CANTRIPS) },
	}
}

new_player_descriptor
{
	type = "subclass"
	name = "Archmage"
	desc = {
		"The master of all magic schools",
	}
	stats = { [A_STR]=-3, [A_INT]=3, [A_DEX]=1, [A_CON]=-4, [A_CHR]=1, }
	mana = 50
	skills =
	{
		["Air"]             = { mods.add(0)   , mods.add(700)  }
		["Combat"]          = { mods.eq (0)   , mods.eq (0)    }
		["Conveyance"]      = { mods.add(0)   , mods.add(700)  }
		["Divination"]      = { mods.add(0)   , mods.add(700)  }
		["Earth"]           = { mods.add(0)   , mods.add(700)  }
		["Fire"]            = { mods.add(0)   , mods.add(700)  }
		["Magic"]           = { mods.add(1000), mods.add(1000) }
		["Mana"]            = { mods.add(0)   , mods.add(700)  }
		["Meta"]            = { mods.add(0)   , mods.add(700)  }
		["Mind"]            = { mods.add(0)   , mods.add(700)  }
		["Nature"]          = { mods.add(0)   , mods.add(700)  }
		["Necromancy"]      = { mods.add(0)   , mods.add(900)  }
		["Runecraft"]       = { mods.add(0)   , mods.add(900)  }
		["Sneakiness"]      = { mods.add(1000), mods.add(900)  }
		["Spell-power"]     = { mods.add(0)   , mods.add(600)  }
		["Spirituality"]    = { mods.add(1000), mods.add(550)  }
		["Sorcery"]         = { mods.add(1000), mods.add(700)  }
		["Temporal"]        = { mods.add(0)   , mods.add(700)  }
		["Thaumaturgy"]     = { mods.add(0)   , mods.add(900)  }
		["Water"]           = { mods.add(0)   , mods.add(700)  }
		["Weaponmastery"]   = { mods.eq (0)   , mods.eq (0)    }
	}
	starting_objects =
	{
		{ obj = lookup_kind(TV_LIGHT_ARMOUR, SV_ROBE) },
		{ obj = lookup_kind(TV_MSTAFF, SV_MAGE_STAFF) },
		{ obj = lookup_kind(TV_BOOK, SV_BOOK_CANTRIPS) },
	}
}

new_player_descriptor
{
	type = "subclass"
	name = "Necromancer"
	desc = {
		"The master of the death.",
	}
	stats = { [A_STR]=-3, [A_INT]=3, [A_DEX]=1, [A_CON]=-2, [A_CHR]=1, }
	mana = 50
	skills =
	{
		["Air"]             = { mods.add(0)   , mods.add(700)  }
		["Combat"]          = { mods.add(1000), mods.add(600)  }
		["Conveyance"]      = { mods.add(0)   , mods.add(700)  }
		["Corpse-preservation"] = { mods.add(5000), mods.add(900)  }
		["Divination"]      = { mods.add(0)   , mods.add(700)  }
		["Earth"]           = { mods.add(0)   , mods.add(800)  }
		["Fire"]            = { mods.add(0)   , mods.add(800)  }
		["Magic"]           = { mods.add(1000), mods.add(900)  }
		["Magic-device"]    = { mods.add(0)   , mods.add(200)  }
		["Mana"]            = { mods.add(0)   , mods.add(600)  }
		["Meta"]            = { mods.add(0)   , mods.add(700)  }
		["Mind"]            = { mods.add(0)   , mods.add(900)  }
		["Monster-lore"]    = { mods.add(1000), mods.add(600)  }
		["Nature"]          = { mods.add(0)   , mods.add(500)  }
		["Necromancy"]      = { mods.add(1000), mods.add(1000) }
		["Runecraft"]       = { mods.add(0)   , mods.add(700)  }
		["Sneakiness"]      = { mods.add(1000), mods.add(900)  }
		["Spell-power"]     = { mods.add(0)   , mods.add(600)  }
		["Spirituality"]    = { mods.add(1000), mods.add(550)  }
		["Temporal"]        = { mods.add(0)   , mods.add(700)  }
		["Thaumaturgy"]     = { mods.add(0)   , mods.add(800)  }
		["Water"]           = { mods.add(0)   , mods.add(700)  }
		["Weaponmastery"]   = { mods.add(700) , mods.add(600)  }
	}
	starting_objects =
	{
		{ obj = lookup_kind(TV_LIGHT_ARMOUR, SV_ROBE) },
		{ obj = lookup_kind(TV_SWORD, SV_RAPIER) },
	}
}

new_player_descriptor
{
	type = "subclass"
	name = "Runecrafter"
	desc = {
		"Runecrafters use the runes found in Middle-earth to create",
		"finely tuned spells for each specific situation.",
	}
	stats = { [A_STR]=-3, [A_INT]=3, [A_DEX]=1, [A_CON]=-2, [A_CHR]=1, }
	mana = 50
	skills =
	{
		["Air"]             = { mods.add(0)   , mods.add(700)  }
		["Combat"]          = { mods.add(1000), mods.add(500)  }
		["Conveyance"]      = { mods.add(0)   , mods.add(700)  }
		["Divination"]      = { mods.add(0)   , mods.add(700)  }
		["Earth"]           = { mods.add(0)   , mods.add(700)  }
		["Fire"]            = { mods.add(0)   , mods.add(700)  }
		["Magic"]           = { mods.add(2000), mods.add(950)  }
		["Magic-device"]    = { mods.add(0)   , mods.add(200)  }
		["Mana"]            = { mods.add(1000), mods.add(600)  }
		["Meta"]            = { mods.add(0)   , mods.add(700)  }
		["Mind"]            = { mods.add(0)   , mods.add(700)  }
		["Nature"]          = { mods.add(0)   , mods.add(700)  }
		["Necromancy"]      = { mods.add(0)   , mods.add(700)  }
		["Runecraft"]       = { mods.add(1000), mods.add(1000) }
		["Sneakiness"]      = { mods.add(1000), mods.add(900)  }
		["Spell-power"]     = { mods.add(0)   , mods.add(600)  }
		["Spirituality"]    = { mods.add(1000), mods.add(550)  }
		["Temporal"]        = { mods.add(0)   , mods.add(700)  }
		["Thaumaturgy"]     = { mods.add(0)   , mods.add(700)  }
		["Water"]           = { mods.add(0)   , mods.add(700)  }
		["Weaponmastery"]   = { mods.add(700) , mods.add(500)  }
	}
	starting_objects =
	{
		{ obj = lookup_kind(TV_RUNE1, SV_RUNE_FIRE) },
		{ obj = lookup_kind(TV_RUNE2, SV_RUNE_ARROW) },
		{ obj = lookup_kind(TV_LIGHT_ARMOUR, SV_ROBE) },
		{ obj = lookup_kind(TV_KNIFE, SV_KNIFE) },
	}
}

new_player_descriptor
{
	type = "subclass"
	name = "Thaumaturgist"
	desc = {
		"Thaumaturgy spells come from within and are different for each character.",
		"Since attack is the best defence, all their spells are offensive.",
	}
	stats = { [A_STR]=-3, [A_INT]=3, [A_DEX]=1, [A_CON]=-2, [A_CHR]=1, }
	mana = 50
	skills =
	{
		["Air"]             = { mods.add(0)   , mods.add(700)  }
		["Combat"]          = { mods.add(1000), mods.add(500)  }
		["Conveyance"]      = { mods.add(0)   , mods.add(700)  }
		["Divination"]      = { mods.add(0)   , mods.add(700)  }
		["Earth"]           = { mods.add(0)   , mods.add(700)  }
		["Fire"]            = { mods.add(0)   , mods.add(700)  }
		["Magic"]           = { mods.add(1000), mods.add(900)  }
		["Magic-device"]    = { mods.add(0)   , mods.add(200)  }
		["Mana"]            = { mods.add(0),    mods.add(600)  }
		["Meta"]            = { mods.add(0)   , mods.add(700)  }
		["Mind"]            = { mods.add(0)   , mods.add(700)  }
		["Nature"]          = { mods.add(0)   , mods.add(700)  }
		["Necromancy"]      = { mods.add(0)   , mods.add(700)  }
		["Runecraft"]       = { mods.add(0)   , mods.add(700)  }
		["Sneakiness"]      = { mods.add(1000), mods.add(900)  }
		["Spell-power"]     = { mods.add(0)   , mods.add(600)  }
		["Spirituality"]    = { mods.add(1000), mods.add(550)  }
		["Temporal"]        = { mods.add(0)   , mods.add(700)  }
		["Thaumaturgy"]     = { mods.add(1000), mods.add(800)  }
		["Water"]           = { mods.add(0)   , mods.add(700)  }
		["Weaponmastery"]   = { mods.add(700) , mods.add(500)  }
	}
	starting_objects =
	{
		{ obj = lookup_kind(TV_LIGHT_ARMOUR, SV_ROBE) },
		{ obj = lookup_kind(TV_KNIFE, SV_DAGGER) },
	}
}

new_player_descriptor
{
	type = "subclass"
	name = "Demonologist"
	desc = {
		"Masters of the school of demonology, they both trained in melee",
		"fighting than using demon spells to enhance their combat potential.",
	}
	stats = { [A_STR]=-1, [A_INT]=1, [A_DEX]=2, [A_CON]=-1, [A_CHR]=0, }
	mana = 50
	skills =
	{
		["Archery"]         = { mods.add(1000), mods.add(400)  }
		["Combat"]          = { mods.add(2000), mods.add(700)  }
		["Demonology"]      = { mods.add(1000), mods.add(1000) }
		["Disarming"]       = { mods.add(1000), mods.add(900)  }
		["Divination"]      = { mods.add(0)   , mods.add(450)  }
		["Earth"]           = { mods.add(0)   , mods.add(450)  }
		["Fire"]            = { mods.add(0)   , mods.add(450)  }
		["Magic"]           = { mods.add(1000), mods.add(700)  }
		["Magic-device"]    = { mods.add(0)   , mods.add(100)  }
		["Sneakiness"]      = { mods.add(1000), mods.add(900)  }
		["Spirituality"]    = { mods.add(1000), mods.add(600)  }
		["Weaponmastery"]   = { mods.add(1000), mods.add(700)  }
	}
	starting_objects =
	{
		{ obj = lookup_kind(TV_LIGHT_ARMOUR, SV_SOFT_LEATHER_JACKET) },
		{ obj = lookup_kind(TV_DEMON_ITEM, SV_DEMON_WEAPON) },
	}
}
