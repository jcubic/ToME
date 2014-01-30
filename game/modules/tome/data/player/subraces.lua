---------------------------------------------------------
--                       Humans                        --
---------------------------------------------------------
new_player_descriptor
{
	type = "subrace"
	define_as = "SUBRACE_HUMAN"
	name = "Human"
	desc = {
		"Humans are are everywhere.",
		"They are the basic race to which all others are compared.",
		"Average in ability, they can be any class.",
	}
}
new_player_descriptor
{
	type = "subrace"
	define_as = "SUBRACE_DUNADAN"
	name = "Dunadan"
	desc = {
		"The greatest of the Edain, humans in all respects but",
		"stronger, smarter and wiser.",
	}
	stats = { [A_STR]=1, [A_INT]=2, [A_WIS]=2, [A_DEX]=2, [A_CON]=3, [A_CHR]=2, }
	luck = 2
	experience = 80
	levels =
	{
		[ 1] = { SUST_STATS=getter.stats{[A_CON]=1} REGEN=true }
	}
}

new_player_descriptor
{
	type = "subrace"
	define_as = "SUBRACE_ROHIRRIM"
	name = "Rohirrim"
	desc = {
		"Humans from the land of Rohan, riding the great Mearas.",
	}
	stats = { [A_STR]=1, [A_INT]=1, [A_WIS]=0, [A_DEX]=3, [A_CON]=1, [A_CHR]=2, }
	experience = 70
	levels =
	{
		[ 1] = { SPEED=3 }
	}
	skills =
	{
		["Weaponmastery"]   = { mods.add(0)   , mods.add(200)  }
		["Riding"]          = { mods.add(5000), mods.add(600)  }
	}
}

new_player_descriptor
{
	type = "subrace"
	define_as = "SUBRACE_BEORNING"
	name = "Beorning"
	desc = {
		"A race of men shapeshifters.",
		"They have the unique power of being able to polymorph to bear forms.",
	}
	stats = { [A_STR]=4, [A_INT]=-2, [A_WIS]=-2, [A_DEX]=-1, [A_CON]=3, [A_CHR]=-5, }
	luck = 1
	life_rating = 3
	experience = 70
	abilities =
	{
		["Bear-form"] = 1
	}
	levels =
	{
		[ 1] = { SUST_STATS = getter.stats{[A_STR]=true} }
	}
	skills =
	{
		["Barehand-combat"] = { mods.add(3000), mods.add(200) }
	}
}

---------------------------------------------------------
--                       Elves                         --
---------------------------------------------------------
new_player_descriptor
{
	type = "subrace"
	define_as = "SUBRACE_NOLDOR"
	name = "Noldor"
	desc = {
		"The Noldor are the second clan of Elves who came to Valinor, and ",
		"are accounted as the greatest of all peoples in Middle-earth. ",
		"They are masters of all skills, and are strong and intelligent. ",
		"They can play all classes except rogues, and very well at that. ",
		"High-elves begin their lives able to see the unseen, and resist ",
		"light effects just like regular elves.  However, there are few ",
		"things that they have not seen already, and experience is very ",
		"hard for them to gain."
	}
	stats = { [A_STR]=1, [A_INT]=3, [A_WIS]=2, [A_DEX]=3, [A_CON]=1, [A_CHR]=5, }
	experience = 200
	levels =
	{
		[1] = { SEE_INVIS=true RESIST=getter.resists{LITE=60} }
	}

	skills =	{
		["Weaponmastery"]   = { mods.add( 2000), mods.add(0) },
		["Archery"]         = { mods.add( 5000), mods.add(0) },
		["Sneakiness"]      = { mods.add(  600), mods.add(0) },
		["Stealth"]         = { mods.add( 8000), mods.add(0) },
		["Disarming"]       = { mods.add(  800), mods.add(0) },
		["Magic-device"]    = { mods.add( 4000), mods.add(0) },
		["Spirituality"]    = { mods.add(20000), mods.add(0) }
	}
}
new_player_descriptor
{
	type = "subrace"
	define_as = "SUBRACE_AVARI"
	name = "Avari"
	desc = {
		"The Avari are those elves who refused the summons of Orome to come",
		"to Valinor, and stayed behind in Middle-earth instead.  While ",
		"somewhat less hardy than the Noldor, they are better at magic, ",
		"gain experience faster, and have an intrinsic magic missile ",
		"attack.  Unlike the Noldor, they are resistant to darkness attacks ",
		"rather than light attacks, and gain the ability to see invisible ",
		"things at a higher level, rather than starting out with it."
	}
	stats = { [A_STR]=-1, [A_INT]=3, [A_WIS]=2, [A_DEX]=2, [A_CON]=-2, [A_CHR]=1, }
	luck = -2
	life_rating = -2
	experience = 150
	infravision = 5
--DGDGDGDG	powers = { "magic missile", }
	levels =
	{
		[ 1] = { RESIST=getter.resists{DARK=50} }
		[20] = { SEE_INVIS=true }
	}
	skills =	{
		["Weaponmastery"]   = { mods.add(-1000), mods.add(0) },
		["Archery"]         = { mods.add( 2000), mods.add(0) },
		["Sneakiness"]      = { mods.add( 1600), mods.add(0) },
		["Stealth"]         = { mods.add( 6000), mods.add(0) },
		["Disarming"]       = { mods.add( 1000), mods.add(0) },
		["Magic"]           = { mods.add( 0)   , mods.add(200) },
		["Magic-device"]    = { mods.add( 3000), mods.add(0) },
		["Spirituality"]    = { mods.add(20000), mods.add(0) }
	}
}

---------------------------------------------------------
--                       Hobbits                       --
---------------------------------------------------------
new_player_descriptor
{
	type = "subrace"
	define_as = "SUBRACE_HARFOOT"
	name = "Harfoot"
	desc = {
		"An old but quiet race related to humans.",
		"They are small and quite weak but good at many things.",
	}
}
new_player_descriptor
{
	type = "subrace"
	define_as = "SUBRACE_FALLOHIDE"
	name = "Fallohide"
	desc = {
		"An old but quiet race related to humans.",
		"They are small and quite weak but good at many things.",
	}
}
new_player_descriptor
{
	type = "subrace"
	define_as = "SUBRACE_STOOR"
	name = "Stoor"
	desc = {
		"An old but quiet race related to humans.",
		"They are small and quite weak but good at many things.",
	}
}

---------------------------------------------------------
--                       Dwarves                       --
---------------------------------------------------------
new_player_descriptor
{
	type = "subrace"
	define_as = "SUBRACE_DWARF"
	name = "Dwarf"
	desc = {
		"The children of Aule, a strong but small race.",
		"Miners and fighters of legend.",
	}
}

---------------------------------------------------------
--                        Ents                         --
---------------------------------------------------------
new_player_descriptor
{
	type = "subrace"
	define_as = "SUBRACE_ENT"
	name = "Ent"
	desc = {
		"Guardian of the forests of Middle-earth, summoned by Yavanna before",
		"even the elves awoke. It is said 'Trolls are strong, Ents are STRONGER'.",
		"Ent-wives went away a long time ago and as such may not be played."
	}
}
