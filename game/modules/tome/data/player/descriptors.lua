--
-- Load all the player descriptors
--

-- Make the base one, which contains the basic skills and such things
new_player_descriptor
{
	type = "base"
	name = "Base Descriptor"
	body = { INVEN=23 MAINHAND=1 OFFHAND=1 BODY=1 OUTER=1 LITE=1 HANDS=1 TOOL=1 RING=2 HEAD=1 NECK=1 FEET=1 MOUNT=1 BACKPACK=1 }
	skills =
	{
		["Monster-lore"] 	= { mods.add(0), 	mods.add(500) }
		["Prayer"] 		= { mods.add(0), 	mods.add(500) }
		["Udun"] 		= { mods.add(0), 	mods.add(400) }
		["Magic-device"] 	= { mods.add(1000), 	mods.add(1000) }
		["Riding"]              = { mods.add(0),        mods.add(500)  }
	}
	starting_objects =
	{
		{ obj=function(obj) object_prep(obj, TV_PARCHMENT, SV_BEGINNER_PARCHMENT) end },
		{ obj=function(obj) object_prep(obj, TV_FOOD, SV_FOOD_RATION) end min=3 max=7 },
		{ obj=function(obj) object_prep(obj, TV_LITE, SV_LITE_TORCH) end min=3 max=7 },
		{ obj=function(obj) object_prep(obj, TV_TOOL, SV_SHOVEL) end },
	}
}

-- Now load all the races, classes, whatever
-- BEWARE, the automatic .lua => .raw converter will not be aware that those
-- files have been cahnged, thus if you change any you must make sure to
-- delete ~/.tome/version/tome/raw/player/descriptors.raw
tome_dofile_anywhere("/data/player/", "sexes.lua")

tome_dofile_anywhere("/data/player/", "races.lua")
tome_dofile_anywhere("/data/player/", "subraces.lua")
tome_dofile_anywhere("/data/player/", "races_bg.lua")

tome_dofile_anywhere("/data/player/", "classes.lua")

tome_dofile_anywhere("/data/player/", "undead.lua")
