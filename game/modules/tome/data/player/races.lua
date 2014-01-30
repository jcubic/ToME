new_player_descriptor
{
	type = "race"
	define_as = "RACE_HUMAN"
	name = "Human"
	desc = {
		"Humans are are everywhere.",
		"They are the basic race to which all others are compared.",
		"Average in ability, they can be any class.",
	}
	descriptor_choices =
	{
		subrace =
		{
			Human = "allow"
			Dunadan = "allow"
			Rohirrim = "allow"
			Beorning = "allow"
			__ALL__ = "never"
		}
	}
	life_rating = 15
	experience = 100
	age = { 14, 6 }
	weight =
	{
		female = { 150, 20 }
		male = { 180, 25 }
	}
	height =
	{
		female = { 66, 4 }
		male = { 72, 6 }
	}
}

new_player_descriptor
{
	type = "race"
	define_as = "RACE_ELF"
	name = "Elf"
	desc = {
		"Elves are the first born of Arda.",
	}
	descriptor_choices =
	{
		subrace =
		{
			Noldor = "allow"
			Avari = "allow"
			__ALL__ = "never"
		}
	}
	stats = { [A_STR]=1, [A_INT]=3, [A_WIS]=2, [A_DEX]=3, [A_CON]=1, [A_CHR]=5, }
	life_rating = 15
	experience = 100
	age = { 100, 30 }
	weight =
	{
		female = { 180, 15 }
		male = { 190, 20 }
	}
	height =
	{
		female = { 82, 10 }
		male = { 90, 10 }
	}
	infravision = 4
	flags = { ELF=1 }
}


new_player_descriptor
{
	type = "race"
	define_as = "RACE_HOBBIT"
	name = "Hobbit"
	desc = {
		"Hobbits, or halflings, are very good at ranged combat ",
		"(especially with slings), throwing, and have good saving ",
		"throws. They also are very good at searching, disarming, ",
		"perception and stealth; so they make excellent rogues, but ",
		"prefer to be called burglars.  They are much weaker than ",
		"humans, thus not as good at melee fighting, and also not ",
		"capable of carrying as many objects.  Halflings have fair ",
		"infra-vision, so they can detect warm creatures at a ",
		"distance. Hobbits have their dexterity sustained and in time ",
		"they learn to cook a delicious meal from available ",
		"ingredients.  Their sturdy constitutions also allow them to ",
		"resist the insidious poison of the ring-wraiths.  Their feet ",
		"are cover from the ankle down in brown hairy fur, preventing ",
		"them from wearing boots and shoes. ",
	}
	descriptor_choices =
	{
		subrace =
		{
			__ALL__ = "never"
			Harfoot = "allow"
			Stoor = "allow"
			Fallohide = "allow"
		}
	}
	stats = { [A_STR]=-2, [A_INT]=2, [A_WIS]=1, [A_DEX]=3, [A_CON]=2, [A_CHR]=1, }
	luck = 5
	life_rating = 16
	experience = 110
	age = { 21, 12 }
	weight =
	{
		female = { 50, 3 }
		male = { 60, 3 }
	}
	height =
	{
		female = { 33, 3 }
		male = { 36, 3 }
	}
	infravision = 4
	body = { INVEN=-7 FEET=-1}
	flags = { RESIST_BLACK_BREATH=1 XTRA_MIGHT_SLING=1 }
	levels =
	{
		[ 1] = { SUST_STATS=getter.stats{[A_DEX]=1} }
	}

	skills =	{
		["Weaponmastery"]   = { mods.add(-2000), mods.add(0) },
		["Archery"]         = { mods.add( 4000), mods.add(0) },
		["Sling-mastery"]   = { mods.add(    0), mods.add(300) },
		["Sneakiness"]      = { mods.add( 2400), mods.add(0) },
		["Stealth"]         = { mods.add(10000), mods.add(0) },
		["Disarming"]       = { mods.add( 3000), mods.add(0) },
		["Magic-device"]    = { mods.add( 3600), mods.add(0) },
		["Spirituality"]    = { mods.add(18000), mods.add(0) }
	}
}

new_player_descriptor
{
	type = "race"
	define_as = "RACE_DWARF"
	name = "Dwarf"
	desc = {
		"The children of Aule, a strong but small race.",
		"Miners and fighters of legend.",
		"Female dwarves remain a mystery and as such may not be played."
	}
	descriptor_choices =
	{
		subrace =
		{
			__ALL__ = "never"
			Dwarf = "allow"
		}
		sex =
		{
			__ALL__ = "never"
			Male = "allow"
		}
	}
	stats = { [A_STR]=2, [A_INT]=1, [A_WIS]=-1, [A_DEX]=-2, [A_CON]=2, [A_CHR]=-3, }
	luck = 5
	life_rating = 18
	experience = 125
	age = { 35, 15 }
	weight =
	{
		female = { 120, 10 }
		male = { 150, 10 }
	}
	height =
	{
		female = { 46, 3 }
		male = { 48, 3 }
	}
	infravision = 5
	levels =
	{
		[ 1] = {
				RESIST = getter.resists{BLIND=100}
		}
	}
	skills =
	{
		["Weaponmastery"]   = { mods.add( 3000), mods.add(0)  },
		["Axe-mastery"]     = { mods.add(    0), mods.add(200)  },
		["Archery"]         = { mods.add( 1000), mods.add(0)  },
		["Sneakiness"]      = { mods.add( 1400), mods.add(0)},
		["Stealth"]         = { mods.add(-2000), mods.add(0)},
		["Disarming"]       = { mods.add( 1400), mods.add(0)},
		["Magic-device"]    = { mods.add( 1800), mods.add(0)},
		["Spirituality"]    = { mods.add(10000), mods.add(0)},
	}
}

new_player_descriptor
{
	type = "race"
	define_as = "RACE_ENT"
	name = "Ent"
	desc = {
		"Guardian of the forests of Middle-earth, summoned by Yavanna before",
		"even the elves awoke. It is said 'Trolls are strong, Ents are STRONGER'.",
		"Ent-wives went away a long time ago and as such may not be played."
	}
	descriptor_choices =
	{
		subrace =
		{
			__ALL__ = "never"
			Ent = "allow"
		}
		sex =
		{
			__ALL__ = "never"
			Male = "allow"
		}
	}
	stats = { [A_STR]=6, [A_INT]=-3, [A_WIS]=2, [A_DEX]=-5, [A_CON]=6, [A_CHR]=-3, }
	luck = -2
	life_rating = 21
	experience = 190
	age = { 255, 70 }
	weight =
	{
		female = { 100, 20 }
		male = { 100, 25 }
	}
	height =
	{
		female = { 66, 4 }
		male = { 72, 6 }
	}
	infravision = 5
	flags = { NO_STUN=true NO_FOOD=true }
	levels =
	{
		[ 1] = { SPEEDS=getter.speeds{WALK=-7} RESIST=getter.resists{FIRE=-50} }
		[ 5] = { SEE_INVIS=true }
		[20] = { ESP=getter.flags{ORC=1 TROLL=1 EVIL=1 } }
	}
	skills =
	{
		["Barehand-combat"] =  { mods.add(0)   , mods.add(200)  }
		["Boulder-throwing"] = { mods.add(0)   , mods.add(600)  }
	}
	abilities =
	{
		["Tree walking"] = 1
	}
}
