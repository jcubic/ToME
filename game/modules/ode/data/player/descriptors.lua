--
-- Load all the player descriptors
--

-- Make the base one, which contains the basic skills and such things
new_player_descriptor
{
	type = "base"
	name = "Base Descriptor"
	body = { INVEN=15 MAINHAND=1 BODY=1 LITE=1 }
	starting_objects =
	{
		{ obj=function(obj) object_prep(obj, TV_FOOD, SV_FOOD_RATION) end },
		{ obj=function(obj) object_prep(obj, TV_ARMOR, SV_ROBE) end },
		{ obj=function(obj) object_prep(obj, TV_WEAPON, SV_DAGGER) end },
	}
	skills =
	{
		["Combat"] 	= { mods.add(0), 	mods.add(1000) }
		["Defense"] 	= { mods.add(0), 	mods.add(1000) }
	}
	life_rating = 10
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

-- Warrior
new_player_descriptor
{
	type = "class"
	name = "Fighter"
	desc = "Fighters can take lot of punishment."
	stats = { [A_CON] = 2 }
}

-- Berserker
new_player_descriptor
{
	type = "class"
	name = "Berserker"
	desc = "Berserkers hit hard."
	stats = { [A_STR] = 2 }
}
