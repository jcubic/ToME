-- Need a Player entry to be first
new_monster_race
{
	name = "Player"
	race = '@'
	sval = 0
	color = color.WHITE
	speed = 0 life = { 1, 1 } ac = 0
	aaf = 0 sleep = 0
	level = 0 exp = 0
	rarity = 0
	weight = 100
	body = { INVEN=23 MAINHAND=1 BODY=1 LITE=1 }
	flags = {
		UNIQUE=true
		DROP_CORPSE=true DROP_SKELETON=true
		OPEN_DOOR=true
		BASH_DOOR=true
		NEVER_GENE=true
	}
}

new_monster_race
{
	race = 'H'
	sval = 1
	name = "Hobgoblin"
	level = 1 rarity = 1
	desc = "Oh my! It looks like a hobgoblin!"
	color = color.LIGHT_UMBER
	speed = 0 life = {1,8} ac = 5
	aaf = 20 sleep = 5
	exp = 3
	weight = 1
	blows = { {"HIT","HURT",{1,8}}, }
	flags = {
		OPEN_DOOR=true
		AI = ai.ZOMBIE
	}
}

new_monster_race
{
	race = 'A'
	sval = 1
	name = "Giant Ant"
	level = 2 rarity = 1
	desc = "Some kind of ant.. but it is quite big, beware, it could stomp you!"
	color = color.BLUE
	speed = 0 life = {2,8} ac = 7
	aaf = 20 sleep = 5
	exp = 10
	weight = 1
	blows = { {"STING","STR_POIS",{1,6}}, }
	flags = {
		AI = ai.ZOMBIE
	}
}

new_monster_race
{
	race = 'B'
	sval = 1
	name = "Bat"
	level = 1 rarity = 1
	desc = "Flap flap eeeek, it's the bat!"
	color = color.LIGHT_DARK
	speed = 0 life = {1,8} ac = 7
	aaf = 20 sleep = 20
	exp = 1
	weight = 1
	blows = { {"HIT","HURT",{1,2}}, }
	flags = {
		AI = ai.SNAKE
	}
}

--	{ "gnome",	 10,	0,	{ _x,  8,   1,   5, ___, "1d6" } },
new_monster_race
{
	race = 'g'
	sval = 1
	name = "Gnome"
	level = 1 rarity = 1
	desc = "Its small and nervous ! Yuck !"
	color = color.YELLOW
	speed = 0 life = {1,8} ac = 3
	aaf = 20 sleep = 20
	exp = 8
	weight = 1
	blows = { {"HIT","HURT",{1,6}}, }
	flags = {
		OPEN_DOOR=true
		AI = ai.SNAKE
	}
}

--	{ "jackal",	 0,	ISMEAN,	{ _x,  2,   1,   3, ___, "1d2" } },
new_monster_race
{
	race = 'j'
	sval = 1
	name = "Jackal"
	level = 1 rarity = 1
	desc = "Its small, nervous and dog like!"
	color = color.RED
	speed = 0 life = {1,8} ac = 3
	aaf = 20 sleep = 20
	exp = 2
	weight = 1
	blows = { {"HIT","HURT",{1,2}}, }
	flags = {
		AI = ai.ZOMBIE
	}
}
--	{ "kobold",	 0,	ISMEAN,	{ _x,  1,   1,   3, ___, "1d4" } },
new_monster_race
{
	race = 'k'
	sval = 1
	name = "Kobold"
	level = 1 rarity = 1
	desc = "Brrr."
	color = color.GREEN
	speed = 0 life = {1,8} ac = 0
	aaf = 20 sleep = 20
	exp = 1
	weight = 1
	blows = { {"HIT","HURT",{1,4}}, }
	flags = {
		OPEN_DOOR=true
		AI = ai.ZOMBIE
	}
}
--	{ "orc",	 15,	ISBLOCK,{ _x,  5,   1,   4, ___, "1d8" } },
new_monster_race
{
	race = 'o'
	sval = 1
	name = "Orc"
	level = 1 rarity = 1
	desc = "Sauron is coming ! Sauron is .. erm .. sorry."
	color = color.LIGHT_UMBER
	speed = 0 life = {1,8} ac = 4
	aaf = 20 sleep = 20
	exp = 5
	weight = 1
	blows = { {"HIT","HURT",{1,8}}, }
	flags = {
		OPEN_DOOR=true
		AI = ai.ZOMBIE
	}
}

--	{ "zombie",	 0,	ISMEAN,	{ _x,  7,   2,   2, ___, "1d8" } }
new_monster_race
{
	race = 'z'
	sval = 1
	name = "Zombie"
	level = 2 rarity = 1
	desc = "The walking dead wants you!"
	color = color.LIGHT_DARK
	speed = -10 life = {2,8} ac = 2
	aaf = 20 sleep = 20
	exp = 7
	weight = 1
	blows = { {"HIT","HURT",{1,8}}, }
	flags = {
		OPEN_DOOR=true
		AI = ai.ZOMBIE
	}
}

--	{ "leprechaun",	 0,	0,	{ _x, 10,   3,   2, ___, "1d1" } },
new_monster_race
{
	race = 'l'
	sval = 1
	name = "Leprechaun"
	level = 4 rarity = 1
	desc = "Yet an another small thing .."
	color = color.YELLOW
	speed = 0 life = {3,8} ac = 4
	aaf = 20 sleep = 20
	exp = 10
	weight = 1
	blows = {
		{"HIT","HURT",{1,1}},
		{"HIT","STEAL_GOLD",{0,0}},
	}
	flags = {
		OPEN_DOOR=true
		AI = ai.ZOMBIE
	}
}

--	{ "quasit",	 30,	ISMEAN,	{ _x, 35,   3,   8, ___, "1d2/1d2/1d4" } },
new_monster_race
{
	race = 'q'
	sval = 1
	name = "Quasit"
	level = 3 rarity = 1
	desc = "Some kind of minor nasty demon."
	color = color.LIGHT_RED
	speed = 10 life = {3,8} ac = 4
	aaf = 20 sleep = 20
	exp = 35
	weight = 1
	blows = {
		{"HIT","HURT",{1,2}},
		{"HIT","HURT",{1,2}},
		{"HIT","HURT",{1,4}},
	}
	flags = {
		OPEN_DOOR=true
		AI = ai.ZOMBIE
	}
}

--	{ "centaur",	 15,	0,	{ _x, 15,   4,   4, ___, "1d6/1d6" } },
new_monster_race
{
	race = 'C'
	sval = 1
	name = "Centaur"
	level = 4 rarity = 1
	desc = "Half horse, half human, I present Mr. Centaur!"
	color = color.YELLOW
	speed = 0 life = {4,8} ac = 6
	aaf = 20 sleep = 20
	exp = 15
	weight = 1
	blows = {
		{"HIT","HURT",{1,6}},
		{"HIT","HURT",{1,6}},
	}
	flags = {
		OPEN_DOOR=true
		AI = ai.ZOMBIE
	}
}

--	{ "yeti",	 30,	0,	{ _x, 50,   4,   4, ___, "1d6/1d6" } },
new_monster_race
{
	race = 'Y'
	sval = 1
	name = "yeti"
	level = 4 rarity = 1
	desc = "Woh! Quick take a picture."
	color = color.WHITE
	speed = 0 life = {4,8} ac = 4
	aaf = 20 sleep = 20
	exp = 50
	weight = 1
	blows = {
		{"HIT","HURT",{1,6}},
		{"HIT","HURT",{1,6}},
	}
	flags = {
		OPEN_DOOR=true
		AI = ai.ZOMBIE
	}
}

--	{ "troll",	 50,	ISREGEN,{ _x, 55,   6,   6, ___, "1d8/1d8/2d6" } },
new_monster_race
{
	race = 'T'
	sval = 1
	name = "Troll"
	level = 6 rarity = 1
	desc = "Bad temper, bad breath, big and ugly. Your basic troll."
	color = color.WHITE
	speed = 0 life = {6,8} ac = 5
	aaf = 20 sleep = 20
	exp = 55
	weight = 1
	blows = {
		{"HIT","HURT",{1,8}},
		{"HIT","HURT",{2,6}},
	}
	flags = {
		OPEN_DOOR=true
		AI = ai.ZOMBIE
	}
}

--	{ "xorn",	 0,	ISMEAN,	{ _x,120,   7,  12, ___, "1d3/1d3/1d3/4d6" } },
new_monster_race
{
	race = 'X'
	sval = 1
	name = "Xorn"
	level = 7 rarity = 1
	desc = "Simply impossible to describe, you must see it for yourself."
	color = color.UMBER
	speed = 0 life = {7,8} ac = 6
	aaf = 20 sleep = 20
	exp = 120
	weight = 1
	blows = {
		{"HIT","HURT",{1,3}},
		{"HIT","HURT",{1,3}},
		{"HIT","HURT",{3,6}},
	}
	flags = {
		OPEN_DOOR=true
		AI = ai.ZOMBIE
	}
}

--[[
	/* Name		 CARRY	FLAG    str, exp, lvl, amr, hpt, dmg */
	{ "nymph",	 100,	0,	{ _x, 40,   3,   1, ___, "0d0" } },
	{ "rust monster",0,	ISMEAN,	{ _x, 25,   5,   8, ___, "0d0/0d0" } },
	{ "wraith",	 0,	0,	{ _x, 55,   5,   6, ___, "1d6" } },
	{ "mimic",	 30,	0,	{ _x,140,   7,   3, ___, "3d4" } },
	{ "violet fungi",0,	ISMEAN,	{ _x, 85,   8,   7, ___, "000d0" } },
	{ "invisible stalker",0,ISINVIS,{ _x,120,   8,   7, ___, "4d4" } },
	{ "umber hulk",	 40,	ISMEAN,	{ _x,130,   8,   8, ___, "3d4/3d4/2d5" } },
	{ "vampire",	 20,	ISREGEN|ISMEAN,{ _x,380,   8,   9, ___, "1d10" } },
	{ "purple worm", 70,	0,	{ _x,7000, 15,   4, ___, "2d12/2d4" } },
]]

--	{ "dragon",	 100,	ISGREED,{ _x,9000, 10,  11, ___, "1d8/1d8/3d10" } },
new_monster_race
{
	-- This will make a variable containing the monster index
	define_as = "MONSTER_DRAGON"
	race = 'D'
	sval = 1
	name = "Dragon"
	level = 10 rarity = 1
	desc = "Do not meddle in the affairs of Dragons, for you are tasty with ketchup..."
	color = color.VIOLET
	speed = 10 life = {10,8} ac = 8
	aaf = 20 sleep = 5
	exp = 300
	weight = 1
	blows = {
		{"HIT","COLD",{1,6}},
		{"HIT","FIRE",{1,6}},
		{"HIT","HURT",{2,8}},
	}
	flags = {
		OPEN_DOOR=true
		INVIS=true
		FORCE_MAXHP=true
		RESIST=getter.resists{FIRE=50 COLD=50}
		AI = ai.SPELL_AND_MOVE
		AI_STATES = getter.flags{ MOVE_AI=ai.ZOMBIE CASTING_AI=ai.RANDOM_CAST }
	}
	spells =
	{
		frequency = 6
		["Breathe Fire"] = { level=10 chance=100 }
		["Breathe Cold"] = { level=10 chance=100 }
	}
}
