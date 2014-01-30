-- Converted from the Theme 2.3.x wf_info.txt for T3

-- Todo:
-- * Standardize the symbols for easy reading. For example, all lower case letters
-- towns/dungeons, etc. At present, any character can be used to be anything.
-- * Remove duplicates. There are still a few.
-- * Confirm what's a town and what's a dungeon. Sindarin names aren't exactly
-- intuitive. Some things may be miscategorized.
-- * Confirm that everything on here really exists. Several things are defined in
-- here that don't appear to have any entry as a dungeon or town, and I've found
-- at least one thing defined here that doesn't appear on the map.
-- * Confirm that nowhere in the module is anything in here reffered to by number
-- instead of flag. For instance, Cirith Ungol right now is using 'FEAT = 7' That's
-- dangerous, because any time a feature is added or moved, the values will change.

new_wilderness_terrain
{
	name = "Ekkaia"
	long_name = "the Encircling Sea"
	char = 'X'
	feat = FEAT_WORLD_BORDER
	type = TERRAIN_EDGE
	level = 1
	generator= {
		FEAT_WORLD_BORDER,
		FEAT_WORLD_BORDER,
		FEAT_WORLD_BORDER,
		FEAT_WORLD_BORDER,
		FEAT_WORLD_BORDER,
		FEAT_WORLD_BORDER,
		FEAT_WORLD_BORDER,
		FEAT_WORLD_BORDER,
		FEAT_WORLD_BORDER,
		FEAT_WORLD_BORDER,
		FEAT_WORLD_BORDER,
		FEAT_WORLD_BORDER,
		FEAT_WORLD_BORDER,
		FEAT_WORLD_BORDER,
		FEAT_WORLD_BORDER,
		FEAT_WORLD_BORDER,
		FEAT_WORLD_BORDER,
		FEAT_WORLD_BORDER,
	}
}

--------------------------------------------
-- Towns
--------------------------------------------

new_wilderness_terrain
{
	name = "Bree"

	long_name = "a small village"
	char = '1'
	feat = FEAT_TOWN
	type = TERRAIN_TOWN
	level = 1
	entrance = 1
	generator= {
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_ELANOR,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
	}

	flags = {
		DUNGEON_FORCE_MONSTER_EGO =
			function(r_idx)
				return iif(rng.percent(55),
						   RACE_EGO_HUMAN_TOWN,
						   RACE_EGO_HOBBIT_TOWN)
			end

		ALLOW_IN_TOWN_BREE = true
		AMBIENT_MUSIC = "bree-town"
	}

	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				ALLOW_IN_TOWN_BREE=true
			}
		}
	}
}

new_wilderness_terrain
{
	name = "Hobbiton"

	long_name = "A village of Hobbits"
	char = '2'
	feat = FEAT_TOWN
	type = TERRAIN_TOWN
	level = 1
	entrance = 2
	generator= {
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_NIPHREDIL,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
	}
	flags = {
		DUNGEON_FORCE_MONSTER_EGO =
			function(r_idx)
				return iif(rng.percent(55), RACE_EGO_HOBBIT_TOWN)
			end
		ALLOW_IN_TOWN_BREE = true
		AMBIENT_MUSIC = "bree-town"
	}

	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				-- Should probably make flags generic by race
				ALLOW_IN_TOWN_BREE=true
			}
		}
	}
}

new_wilderness_terrain
{
	name = "Minas Anor"

	long_name = "The great town of Gondor"
	char = '3'
	feat = FEAT_TOWN
	type = TERRAIN_TOWN
	level = 1
	entrance = 3
	generator= {
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_IRIS,
		FEAT_TREES,
		FEAT_TREES,
	}
}

new_wilderness_terrain
{
	name = "Caras Galadhon"

	long_name = "Lothlorien's chief city"
	char = '4'
	feat = FEAT_TOWN
	type = TERRAIN_TOWN
	level = 1
	entrance = 4
	generator= {
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_KINGSFOIL,
		FEAT_TREES,
		FEAT_FUMELLA,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
	}
	flags =
	{
		AMBIENT_MUSIC = "elven-town"
	}
}

--------------------------------------------
-- End towns
--------------------------------------------

--------------------------------------------
-- 'Dungeons' (Note that 'dungeon' is an engine term.
-- Some 'dungeons' might not be 'dungeons'.)
--------------------------------------------

new_wilderness_terrain
{
	name = "West Gate"
	long_name = "The city of Khazad-dum"
	char = ')'
	feat = FEAT_MORE
	type = TERRAIN_MOUNTAIN
	level = 30
	entrance = 1022
	generator= {
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_TREES,
		FEAT_TREES,
	}
}
new_wilderness_terrain
{
	name = "East Gate"
	long_name = "The city of Khazad-dum"
	char = '('
	feat = FEAT_MORE
	type = TERRAIN_MOUNTAIN
	level = 30
	entrance = 5
	generator= {
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_DIRT,
	}
	flags =
	{
		AMBIENT_MUSIC = "dwarven-town"
	}
}
new_wilderness_terrain
{
	name = "Beorn's Halls"
	long_name = "the dwelling of Beorn the Shape-changer"
	char = '6'
	feat = FEAT_GRASS
	type = TERRAIN_TOWN
	level = 1
	entrance = 6
	generator= {
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_ELANOR,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
	}
}

-- Cerin Amroth

new_wilderness_terrain
{
	name = "Cerin Amroth"
	long_name = "a place of peace"
	char = '7'
	feat = FEAT_GRASS
	type = TERRAIN_TOWN
	level = 1
	entrance = 7
	generator= {
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_ELANOR,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
	}
}

-- Dale
new_wilderness_terrain
{
	name = "Dale"

	long_name = "a city of Men, being rebuilt"
	char = '8'
	feat = FEAT_SAND
	type = TERRAIN_TOWN
	level = 1
	entrance = 8
	generator= {
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
	}
}

-- Edoras
new_wilderness_terrain
{
	name = "Edoras"

	long_name = "the capital of Rohan"
	char = '9'
	feat = FEAT_TOWN
	type = TERRAIN_TOWN
	level = 1
	entrance = 9
	generator= {
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_ELANOR,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
	}
}

-- Esgaroth
new_wilderness_terrain
{
	name = "Esgaroth"

	long_name = "the city of Lake-Men"
	char = 'a'
	feat = FEAT_TOWN
	type = TERRAIN_TOWN
	level = 1
	entrance = 10
	generator= {
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_ELANOR,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
	}
}

-- Helm's Deep
new_wilderness_terrain
{
	name = "Helm's Deep"

	long_name = "the great fortress of the Rohirrim"
	char = 'b'
	feat = FEAT_GRASS
	type = TERRAIN_TOWN
	level = 1
	entrance = 11
	generator= {
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_ELANOR,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
	}
}

-- Henneth Annun
new_wilderness_terrain
{
	name = "Henneth Annun"

	long_name = "a Ranger hideout"
	char = 'c'
	feat = FEAT_GRASS
	type = TERRAIN_TOWN
	level = 1
	entrance = 12
	generator= {
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_ELANOR,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
	}
}

-- Osgiliath
new_wilderness_terrain
{
	name = "Osgiliath"

	long_name = "a stronghold of Men"
	char = 'e'
	feat = FEAT_TOWN
	type = TERRAIN_TOWN
	level = 1
	entrance = 15
	generator= {
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_ELANOR,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
	}
}

-- Pelargir
new_wilderness_terrain
{
	name = "Pelargir"

	long_name = "the great city at the mouth of Anduin"
	char = 'p'
	feat = FEAT_TOWN
	type = TERRAIN_TOWN
	level = 1
	entrance = 16
	generator= {
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_ELANOR,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
	}
}

-- Thranduil's Halls
new_wilderness_terrain
{
	name = "Thranduil's Halls"

	long_name = "the hidden realm of the Wood-elves"
	char = 'o'
	feat = FEAT_HILL
	type = TERRAIN_TOWN
	level = 1
	entrance = 17
	generator= {
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_ELANOR,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
	}
}

-- Cirith Ungol a 'defined' feature to allow it to have two entrances
new_wilderness_terrain
{
	name = "Cirith Ungol"
	long_name = "the dreaded Spider Pass"
	char = 'u'
	feat = 7
	type = TERRAIN_GRASS
	level = 25
	entrance = 1009
	generator= {
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SHAL_LAVA,
		FEAT_MUD,
		FEAT_MUD,
		FEAT_MUD,
		FEAT_SHAL_LAVA,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_MUD,
		FEAT_MUD,
		FEAT_SHAL_LAVA,
		FEAT_MUD,
		FEAT_MUD,
		FEAT_SHAL_LAVA,
		FEAT_SAND,
		FEAT_MUD,
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				ALLOW_IN_LAVA=true
			}
		}
	}
}
--------------------------------------------
-- End 'Dungeons'
--------------------------------------------


new_wilderness_terrain
{
	name = "grass"
	long_name = "a plain of grass"
	char = '.'
	feat = FEAT_GRASS
	type = TERRAIN_GRASS
	level = 10
	generator= {
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_KINGSFOIL,
		FEAT_GRASS,
		FEAT_ANEMONES,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_TREES,
		FEAT_TREES,
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				ALLOW_IN_PLAIN=true
			}
		}
	}
}

new_wilderness_terrain
{
	name = "forest"
	long_name = "a forest"
	char = 'f'
	feat = FEAT_TREES
	type = TERRAIN_TREES
	level = 25
	generator= {
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_KINGSFOIL,
		FEAT_TREES,
		FEAT_NIPHREDIL,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				ALLOW_IN_FOREST=true
			}
		}
	}
}

new_wilderness_terrain
{
	name = "Mirkwood"
	long_name = "part of Mirkwood forest"
	char = 'T'
	feat = FEAT_TREES
	type = TERRAIN_TREES
	level = 40
	generator= {
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_KINGSFOIL,
		FEAT_TREES,
		FEAT_NIPHREDIL,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				ALLOW_IN_FOREST=true
			}
		}
	}
}

new_wilderness_terrain
{
	name = "Old Forest"
	long_name = "part of the Old Forest"
	char = 'O'
	feat = FEAT_TREES
	type = TERRAIN_TREES
	level = 30
	generator= {
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_KINGSFOIL,
		FEAT_TREES,
		FEAT_NIPHREDIL,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				ALLOW_IN_FOREST=true
				ALLOW_IN_OLD_FOREST=true
			}
		}
	}
}

new_wilderness_terrain
{
	name = "Fangorn"
	long_name = "part of Fangorn Forest"
	char = 't'
	feat = FEAT_TREES
	type = TERRAIN_TREES
	level = 40
	generator= {
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_KINGSFOIL,
		FEAT_TREES,
		FEAT_NIPHREDIL,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				ALLOW_IN_FOREST=true
			}
		}
	}
}

new_wilderness_terrain
{
	name = "road"
	long_name = "a west-east road"
	char = '-'
	feat = FEAT_COBBLESTONE_ROAD
	type = TERRAIN_GRASS
	level = 5
	road = 12
	generator= {
		FEAT_COBBLESTONE_ROAD,
		FEAT_COBBLESTONE_ROAD,
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_KINGSFOIL,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_ELANOR,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_TREES,
		FEAT_TREES,
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				ALLOW_IN_PLAIN=true
			}
		}
	}
}

new_wilderness_terrain
{
	name = "road"
	long_name = "a north-south road"
	char = '|'
	feat = FEAT_COBBLESTONE_ROAD
	type = TERRAIN_GRASS
	level = 5
	road = 3
	generator= {
		FEAT_COBBLESTONE_ROAD,
		FEAT_COBBLESTONE_ROAD,
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_KINGSFOIL,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_ELANOR,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_TREES,
		FEAT_TREES,
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				ALLOW_IN_PLAIN=true
			}
		}
	}
}

new_wilderness_terrain
{
	name = "mountain"
	long_name = "a mountain chain"
	char = '&'
	feat = FEAT_MOUNTAIN
	type = TERRAIN_MOUNTAIN
	level = 60
	generator= {
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_DIRT,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_MOUNTAIN,
		FEAT_MOUNTAIN,
		FEAT_MOUNTAIN,
		FEAT_MOUNTAIN,
		FEAT_MOUNTAIN,
		FEAT_MOUNTAIN,
		FEAT_MOUNTAIN,
		FEAT_MOUNTAIN,
		FEAT_MOUNTAIN,
		FEAT_MOUNTAIN,
		FEAT_MOUNTAIN,
	}
}

new_wilderness_terrain
{
	name = "Misty Mountains"
	long_name = "part of the Misty Mountains"
	char = 'M'
	feat = FEAT_MOUNTAIN
	type = TERRAIN_MOUNTAIN
	level = 65
	generator= {
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_DIRT,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_MOUNTAIN,
		FEAT_MOUNTAIN,
		FEAT_MOUNTAIN,
		FEAT_MOUNTAIN,
		FEAT_MOUNTAIN,
		FEAT_MOUNTAIN,
		FEAT_MOUNTAIN,
		FEAT_MOUNTAIN,
		FEAT_MOUNTAIN,
		FEAT_MOUNTAIN,
		FEAT_MOUNTAIN,
	}
}

new_wilderness_terrain
{
	name = "road"
	long_name = "a west-east-south road"
	char = '+'
	feat = FEAT_COBBLESTONE_ROAD
	type = TERRAIN_GRASS
	level = 5
	road = 14
	generator= {
		FEAT_COBBLESTONE_ROAD,
		FEAT_COBBLESTONE_ROAD,
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_GRASS,
		FEAT_KINGSFOIL,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_ELANOR,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_TREES,
		FEAT_TREES,
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				ALLOW_IN_PLAIN=true
			}
		}
	}
}

new_wilderness_terrain
{
	name = "road"
	long_name = "a west-east-north road"
	char = 'z'
	feat = FEAT_COBBLESTONE_ROAD
	type = TERRAIN_GRASS
	level = 5
	road = 13
	generator= {
		FEAT_COBBLESTONE_ROAD,
		FEAT_COBBLESTONE_ROAD,
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_GRASS,
		FEAT_KINGSFOIL,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_ELANOR,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_TREES,
		FEAT_TREES,
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				ALLOW_IN_PLAIN=true
			}
		}
	}
}

new_wilderness_terrain
{
	name = "sea"
	long_name = "the sea"
	char = '='
	feat = FEAT_DEEP_WATER
	type = TERRAIN_DEEP_WATER
	level = 70
	generator= {
		FEAT_DEEP_WATER,
		FEAT_DEEP_WATER,
		FEAT_DEEP_WATER,
		FEAT_DEEP_WATER,
		FEAT_DEEP_WATER,
		FEAT_DEEP_WATER,
		FEAT_DEEP_WATER,
		FEAT_DEEP_WATER,
		FEAT_DEEP_WATER,
		FEAT_DEEP_WATER,
		FEAT_DEEP_WATER,
		FEAT_DEEP_WATER,
		FEAT_SHAL_WATER,
		FEAT_SHAL_WATER,
		FEAT_SHAL_WATER,
		FEAT_SHAL_WATER,
		FEAT_SHAL_WATER,
		FEAT_SHAL_WATER,
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				ALLOW_IN_OCEAN=true
			}
		}
	}
}

new_wilderness_terrain
{
	name = "Sea of Rhun"
	long_name = "part of the Sea of Rhun"
	char = ' '
	feat = FEAT_DEEP_WATER
	type = TERRAIN_DEEP_WATER
	level = 50
	generator= {
		FEAT_DEEP_WATER,
		FEAT_DEEP_WATER,
		FEAT_DEEP_WATER,
		FEAT_DEEP_WATER,
		FEAT_DEEP_WATER,
		FEAT_DEEP_WATER,
		FEAT_DEEP_WATER,
		FEAT_DEEP_WATER,
		FEAT_DEEP_WATER,
		FEAT_DEEP_WATER,
		FEAT_DEEP_WATER,
		FEAT_DEEP_WATER,
		FEAT_SHAL_WATER,
		FEAT_SHAL_WATER,
		FEAT_SHAL_WATER,
		FEAT_SHAL_WATER,
		FEAT_SHAL_WATER,
		FEAT_SHAL_WATER,
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				ALLOW_IN_OCEAN=true
			}
		}
	}
}

new_wilderness_terrain
{
	name = "shallow water"
	long_name = "a shallow water area"
	char = '_'
	feat = FEAT_RIVER
	type = TERRAIN_SHALLOW_WATER
	level = 35
	generator= {
		FEAT_DEEP_WATER,
		FEAT_DEEP_WATER,
		FEAT_DEEP_WATER,
		FEAT_SHAL_WATER,
		FEAT_SHAL_WATER,
		FEAT_SHAL_WATER,
		FEAT_SHAL_WATER,
		FEAT_SHAL_WATER,
		FEAT_LILY,
		FEAT_LILY,
		FEAT_SHAL_WATER,
		FEAT_SHAL_WATER,
		FEAT_SHAL_WATER,
		FEAT_SHAL_WATER,
		FEAT_SHAL_WATER,
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_GRASS,
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				ALLOW_IN_RIVER=true
			}
		}
	}
}

new_wilderness_terrain
{
	name = "mountain"
	long_name = "a mountain"
	char = '^'
	feat = FEAT_MOUNTAIN
	type = TERRAIN_MOUNTAIN
	level = 55
	generator= {
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_MUD,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_MOUNTAIN,
		FEAT_MOUNTAIN,
		FEAT_MOUNTAIN,
		FEAT_MOUNTAIN,
		FEAT_MOUNTAIN,
		FEAT_MOUNTAIN,
		FEAT_MOUNTAIN,
		FEAT_MOUNTAIN,
		FEAT_MOUNTAIN,
		FEAT_MOUNTAIN,
		FEAT_MOUNTAIN,
	}
}

new_wilderness_terrain
{
	name = "Haradwaith"
	long_name = "part of the Southern desert"
	char = 'd'
	feat = FEAT_SAND
	type = TERRAIN_DIRT
	level = 44
	generator= {
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_DEAD_TREE,
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				ALLOW_IN_DESERT=true
			}
		}
	}
}

new_wilderness_terrain
{
	name = "desert"
	long_name = "a desert"
	char = 'l'
	feat = FEAT_ASH
	type = TERRAIN_DIRT
	level = 40
	generator= {
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_DEAD_TREE,
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				ALLOW_IN_DESERT=true
			}
		}
	}}


new_wilderness_terrain
{
	name = "jungle"
	long_name = "a jungle"
	char = 'j'
	feat = FEAT_SMALL_TREES
	type = TERRAIN_TREES
	level = 40
	generator= {
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_SMALL_TREES,
		FEAT_TREES,
		FEAT_SMALL_TREES,
		FEAT_TREES,
		FEAT_SMALL_TREES,
		FEAT_TREES,
		FEAT_ANEMONES,
		FEAT_TREES,
		FEAT_SMALL_TREES,
		FEAT_TREES,
		FEAT_SMALL_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
	}
}

--The Dead Marshes replace 'swamp'
-- Ugly: I used a non-wilderness terrain_idx to force meaner monsters[?] -furiosity
new_wilderness_terrain
{
	name = "Dead Marshes"
	long_name = "a part of the Dead Marshes"
	char = 'S'
	feat = FEAT_DEAD_MARSHES
	type = TERRAIN_SWAMP
	level = 127
	generator= {
		FEAT_DEAD_MARSHES,
		FEAT_DEAD_MARSHES,
		FEAT_DEAD_MARSHES,
		FEAT_DEAD_MARSHES,
		FEAT_DEAD_MARSHES,
		FEAT_DEAD_MARSHES,
		FEAT_DEAD_MARSHES,
		FEAT_DEAD_MARSHES,
		FEAT_DEAD_MARSHES,
		FEAT_DEAD_MARSHES,
		FEAT_SHAL_WATER,
		FEAT_DEAD_MARSHES,
		FEAT_DENSE_MIST,
		FEAT_DEAD_MARSHES,
		FEAT_CLOUD,
		FEAT_CLOUD,
		FEAT_CLOUD,
		FEAT_DEAD_MARSHES,
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				ALLOW_IN_SWAMP=true
			}
		}
	}
}

new_wilderness_terrain
{
	name = "glacier"
	long_name = "a glacier"
	char = 'g'
	feat = FEAT_ICE
	type = TERRAIN_DIRT
	level = 45
	generator= {
		FEAT_MUD,
		FEAT_MUD,
		FEAT_ICE,
		FEAT_ICE,
		FEAT_ICE,
		FEAT_ICE,
		FEAT_ICE,
		FEAT_ICE,
		FEAT_ICE,
		FEAT_ICE,
		FEAT_ICE,
		FEAT_ICE,
		FEAT_ICE,
		FEAT_ICE,
		FEAT_ICE,
		FEAT_ICE,
		FEAT_DEAD_TREE,
		FEAT_DEAD_TREE,
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				ALLOW_IN_COLD=true
			}
		}
	}
}

new_wilderness_terrain
{
	name = "high mountain"
	long_name = "a high mountain chain"
	char = 'H'
	feat = FEAT_HIGH_MOUNTAIN
	type = TERRAIN_MOUNTAIN
	level = 70
	generator= {
		FEAT_HIGH_MOUNTAIN,
		FEAT_HIGH_MOUNTAIN,
		FEAT_HIGH_MOUNTAIN,
		FEAT_HIGH_MOUNTAIN,
		FEAT_HIGH_MOUNTAIN,
		FEAT_HIGH_MOUNTAIN,
		FEAT_HIGH_MOUNTAIN,
		FEAT_HIGH_MOUNTAIN,
		FEAT_HIGH_MOUNTAIN,
		FEAT_HIGH_MOUNTAIN,
		FEAT_HIGH_MOUNTAIN,
		FEAT_HIGH_MOUNTAIN,
		FEAT_HIGH_MOUNTAIN,
		FEAT_HIGH_MOUNTAIN,
		FEAT_HIGH_MOUNTAIN,
		FEAT_HIGH_MOUNTAIN,
		FEAT_HIGH_MOUNTAIN,
		FEAT_HIGH_MOUNTAIN,
	}
}

new_wilderness_terrain
{
	name = "dark pit"
	long_name = "a dark pit"
	char = 'C'
	feat = FEAT_DARK_PIT
	type = TERRAIN_DIRT
	level = 60
	generator= {
		FEAT_DARK_PIT,
		FEAT_DARK_PIT,
		FEAT_DARK_PIT,
		FEAT_DARK_PIT,
		FEAT_DARK_PIT,
		FEAT_DARK_PIT,
		FEAT_DARK_PIT,
		FEAT_DARK_PIT,
		FEAT_DARK_PIT,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
	}
}

new_wilderness_terrain
{
	name = "low hill"
	long_name = "a low hill"
	char = 'h'
	feat = FEAT_HILL
	type = TERRAIN_GRASS
	level = 15
	generator= {
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_DIRT,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_HILL,
		FEAT_HILL,
		FEAT_HILL,
		FEAT_HILL,
		FEAT_HILL,
		FEAT_HILL,
		FEAT_HILL,
		FEAT_HILL,
		FEAT_HILL,
		FEAT_HILL,
		FEAT_HILL,
	}
}

new_wilderness_terrain
{
	name = "dark mountain"
	long_name = "a dark mountain"
	char = 'A'
	feat = FEAT_DARK_MOUNTAIN
	type = TERRAIN_MOUNTAIN
	level = 75
	generator= {
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
	}
}

new_wilderness_terrain
{
	name = "Ephel Duath"
	long_name = "part of the Mountains of Shadow"
	char = 'D'
	feat = FEAT_DARK_MOUNTAIN
	type = TERRAIN_MOUNTAIN
	level = 80
	generator= {
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
	}
}

new_wilderness_terrain
{
	name = "Ered Luin"
	long_name = "part of the Blue Mountains"
	char = 'B'
	feat = FEAT_BLUE_MOUNTAIN
	type = TERRAIN_MOUNTAIN
	level = 50
	generator= {
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_DIRT,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_BLUE_MOUNTAIN,
		FEAT_BLUE_MOUNTAIN,
		FEAT_IRIS,
		FEAT_BLUE_MOUNTAIN,
		FEAT_BLUE_MOUNTAIN,
		FEAT_BLUE_MOUNTAIN,
		FEAT_BLUE_MOUNTAIN,
		FEAT_BLUE_MOUNTAIN,
		FEAT_BLUE_MOUNTAIN,
		FEAT_BLUE_MOUNTAIN,
		FEAT_BLUE_MOUNTAIN,
	}
}

new_wilderness_terrain
{
	name = "grey mountain"
	long_name = "a grey mountain"
	char = 'G'
	feat = FEAT_GREY_MOUNTAIN
	type = TERRAIN_MOUNTAIN
	level = 45
	generator= {
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_DIRT,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_GREY_MOUNTAIN,
		FEAT_GREY_MOUNTAIN,
		FEAT_GREY_MOUNTAIN,
		FEAT_FUMELLA,
		FEAT_GREY_MOUNTAIN,
		FEAT_GREY_MOUNTAIN,
		FEAT_GREY_MOUNTAIN,
		FEAT_GREY_MOUNTAIN,
		FEAT_GREY_MOUNTAIN,
		FEAT_GREY_MOUNTAIN,
		FEAT_GREY_MOUNTAIN,
	}
}

new_wilderness_terrain
{
	name = "Iron Hills"
	long_name = "part of the Iron Hills"
	char = 'I'
	feat = FEAT_GREY_MOUNTAIN
	type = TERRAIN_MOUNTAIN
	level = 56
	generator= {
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_DIRT,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_GREY_MOUNTAIN,
		FEAT_GREY_MOUNTAIN,
		FEAT_GREY_MOUNTAIN,
		FEAT_FUMELLA,
		FEAT_GREY_MOUNTAIN,
		FEAT_GREY_MOUNTAIN,
		FEAT_GREY_MOUNTAIN,
		FEAT_GREY_MOUNTAIN,
		FEAT_GREY_MOUNTAIN,
		FEAT_GREY_MOUNTAIN,
		FEAT_GREY_MOUNTAIN,
	}
}

new_wilderness_terrain
{
	name = "Mount Doom"
	long_name = "a part of Mount Doom"
	char = 'U'
	feat = FEAT_MOUNT_DOOM
	type = TERRAIN_MOUNTAIN
	level = 127
	generator= {
		FEAT_MOUNT_DOOM,
		FEAT_MOUNT_DOOM,
		FEAT_MOUNT_DOOM,
		FEAT_MOUNT_DOOM,
		FEAT_MOUNT_DOOM,
		FEAT_MOUNT_DOOM,
		FEAT_MOUNT_DOOM,
		FEAT_MOUNT_DOOM,
		FEAT_MOUNT_DOOM,
		FEAT_MOUNT_DOOM,
		FEAT_MOUNT_DOOM,
		FEAT_MOUNT_DOOM,
		FEAT_MOUNT_DOOM,
		FEAT_MOUNT_DOOM,
		FEAT_MOUNT_DOOM,
		FEAT_MOUNT_DOOM,
		FEAT_MOUNT_DOOM,
		FEAT_MOUNT_DOOM,
	}
}

new_wilderness_terrain
{
	name = "Lorien"
	long_name = "a Mallorn forest"
	char = '!'
	feat = FEAT_TREE_MALLORN
	type = TERRAIN_TREES
	level = 5
	generator= {
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_TREE_MALLORN,
		FEAT_TREE_MALLORN,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_TREE_MALLORN,
		FEAT_KINGSFOIL,
		FEAT_NIPHREDIL,
		FEAT_TREE_MALLORN,
		FEAT_TREE_MALLORN,
		FEAT_TREES,
		FEAT_TREE_MALLORN,
		FEAT_TREES,
		FEAT_TREE_MALLORN,
		FEAT_TREE_MALLORN,
		FEAT_TREE_MALLORN,
		FEAT_TREE_MALLORN,
	}
}

new_wilderness_terrain
{
	name = "Redhorn Pass"
	long_name = "the peak of Caradhras"
	char = 'R'
	feat = FEAT_SNOWY_PEAK
	type = TERRAIN_GRASS
	level = 30

	generator= {
		FEAT_MUD,
		FEAT_MUD,
		FEAT_ICE,
		FEAT_ICE,
		FEAT_ICE,
		FEAT_ICE,
		FEAT_ICE,
		FEAT_MUD,
		FEAT_ICE,
		FEAT_ICE,
		FEAT_ICE,
		FEAT_MUD,
		FEAT_ICE,
		FEAT_ICE,
		FEAT_ICE,
		FEAT_ICE,
		FEAT_ICE_WALL,
		FEAT_ICE_WALL,
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				ALLOW_IN_COLD=true
			}
		}
	}
}

new_wilderness_terrain
{
	name = "Morannon"
	long_name = "the Black Gate of Mordor"
	char = 'k'
	feat = FEAT_BLACK_GATE
	type = TERRAIN_MOUNTAIN
	level = 54
	generator= {
		FEAT_DARK_MOUNTAIN,
		FEAT_MUD,
		FEAT_DARK_MOUNTAIN,
		FEAT_MUD,
		FEAT_DARK_MOUNTAIN,
		FEAT_MUD,
		FEAT_DARK_MOUNTAIN,
		FEAT_MUD,
		FEAT_MUD,
		FEAT_MUD,
		FEAT_MUD,
		FEAT_MUD,
		FEAT_MUD,
		FEAT_MUD,
		FEAT_MUD,
		FEAT_MUD,
		FEAT_DARK_MOUNTAIN,
		FEAT_DARK_MOUNTAIN,
	}
}

new_wilderness_terrain
{
	name = "evergreen wood"
	long_name = "an evergreen wood"
	char = 'E'
	feat = FEAT_SMALL_TREES
	type = TERRAIN_TREES
	level = 40
	generator= {
		FEAT_MUD,
		FEAT_MUD,
		FEAT_FIR_TREE,
		FEAT_FIR_TREE,
		FEAT_FIR_TREE,
		FEAT_FIR_TREE,
		FEAT_FIR_TREE,
		FEAT_FIR_TREE,
		FEAT_NIPHREDIL,
		FEAT_FIR_TREE,
		FEAT_FIR_TREE,
		FEAT_FIR_TREE,
		FEAT_FIR_TREE,
		FEAT_FIR_TREE,
		FEAT_FIR_TREE,
		FEAT_GRASS,
		FEAT_MUD,
		FEAT_MUD,
	}
}

new_wilderness_terrain
{
	name = "Rivendell"
	long_name = "the valley of Rivendell"
	char = 'L'
	feat = FEAT_TOWN
	type = TERRAIN_TOWN
	level = 5
	entrance = 5
	generator= {
		FEAT_DIRT,
		FEAT_MOUNTAIN,
		FEAT_SMALL_TREES,
		FEAT_TREES,
		FEAT_SMALL_TREES,
		FEAT_MOUNTAIN,
		FEAT_SMALL_TREES,
		FEAT_TREES,
		FEAT_ANEMONES,
		FEAT_MOUNTAIN,
		FEAT_SMALL_TREES,
		FEAT_TREES,
		FEAT_SMALL_TREES,
		FEAT_TREES,
		FEAT_TREES,
		FEAT_GRASS,
		FEAT_MOUNTAIN,
		FEAT_GRASS,
	}
	flags =
	{
		AMBIENT_MUSIC = "elven-town"
	}
}

new_wilderness_terrain
{
	name = "Gorgoroth"
	long_name = "the valley of terror"
	char = 'V'
	feat = FEAT_SAND
	type = TERRAIN_DIRT
	level = 79
	generator= {
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SHAL_LAVA,
		FEAT_MUD,
		FEAT_MUD,
		FEAT_MUD,
		FEAT_SHAL_LAVA,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_MUD,
		FEAT_MUD,
		FEAT_SHAL_LAVA,
		FEAT_MUD,
		FEAT_MUD,
		FEAT_SHAL_LAVA,
		FEAT_SAND,
		FEAT_MUD,
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				ALLOW_IN_LAVA=true
			}
		}
	}
}

new_wilderness_terrain
{
	name = "Northern Waste"
	long_name = "part of the Northern Wastelands"
	char = 'w'
	feat = FEAT_ASH
	type = TERRAIN_DIRT
	level = 30
	generator= {
		FEAT_SAND,
		FEAT_ICE,
		FEAT_ICE,
		FEAT_SAND,
		FEAT_ICE,
		FEAT_SAND,
		FEAT_ICE,
		FEAT_SAND,
		FEAT_ICE,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_ICE,
		FEAT_SAND,
		FEAT_ICE,
		FEAT_SAND,
		FEAT_SAND,
		FEAT_SAND,
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				ALLOW_IN_COLD=true
			}
		}
	}
}

new_wilderness_terrain
{
	name = "crossroads"
	long_name = "a crossroads"
	char = 'x'
	feat = FEAT_COBBLESTONE_ROAD
	type = TERRAIN_GRASS
	level = 5
	road = 15
	generator= {
		FEAT_COBBLESTONE_ROAD,
		FEAT_COBBLESTONE_ROAD,
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_GRASS,
		FEAT_KINGSFOIL,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_ELANOR,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_TREES,
		FEAT_TREES,
	}
}

new_wilderness_terrain
{
	name = "north-south-east road"
	long_name = "a north-south-east road"
	char = '['
	feat = FEAT_COBBLESTONE_ROAD
	type = TERRAIN_GRASS
	level = 5
	road = 7
	generator= {
		FEAT_COBBLESTONE_ROAD,
		FEAT_COBBLESTONE_ROAD,
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_GRASS,
		FEAT_KINGSFOIL,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_ELANOR,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_TREES,
		FEAT_TREES,
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				ALLOW_IN_PLAIN=true
			}
		}
	}
}

new_wilderness_terrain
{
	name = "swamp"
	long_name = "part of a swamp"
	char = 's'
	feat = FEAT_SWAMP_POOL
	type = TERRAIN_SHALLOW_WATER
	level = 25
	generator= {
		FEAT_SWAMP_POOL,
		FEAT_SWAMP_POOL,
		FEAT_TAINTED_WATER,
		FEAT_SHAL_WATER,
		FEAT_SHAL_WATER,
		FEAT_SWAMP_POOL,
		FEAT_TAINTED_WATER,
		FEAT_SWAMP_POOL,
		FEAT_SHAL_WATER,
		FEAT_SHAL_WATER,
		FEAT_TAINTED_WATER,
		FEAT_SWAMP_POOL,
		FEAT_SWAMP_POOL,
		FEAT_TAINTED_WATER,
		FEAT_SWAMP_POOL,
		FEAT_TAINTED_WATER,
		FEAT_SHAL_WATER,
		FEAT_TAINTED_WATER,
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				ALLOW_IN_SWAMP=true
			}
		}
	}
}

new_wilderness_terrain
{
	name = "Nurn"
	long_name = "the valley of Nurn"
	char = '"'
	feat = FEAT_MUD
	type = TERRAIN_GRASS
	level = 50
	generator= {
		FEAT_MUD,
		FEAT_ASH,
		FEAT_MUD,
		FEAT_GRASS,
		FEAT_ASH,
		FEAT_GRASS,
		FEAT_MUD,
		FEAT_GRASS,
		FEAT_ASH,
		FEAT_MUD,
		FEAT_MUD,
		FEAT_ASH,
		FEAT_GRASS,
		FEAT_ASH,
		FEAT_GRASS,
		FEAT_ASH,
		FEAT_MUD,
		FEAT_MUD,
	}
}

new_wilderness_terrain
{
	name = "The Brown Lands"
	long_name = "the Brown Lands"
	char = ';'
	feat = FEAT_TAINTED_WATER
	type = TERRAIN_DIRT
	level = 30
	generator= {
		FEAT_TAINTED_WATER,
		FEAT_TAINTED_WATER,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_TAINTED_WATER,
		FEAT_TAINTED_WATER,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_TAINTED_WATER,
		FEAT_MUD,
		FEAT_ASH,
		FEAT_TAINTED_WATER,
		FEAT_TAINTED_WATER,
		FEAT_MUD,
		FEAT_TAINTED_WATER,
		FEAT_GRASS,
		FEAT_GRASS,
		FEAT_MUD,
	}
}

new_wilderness_terrain
{
	name = "Udun"
	long_name = "the valley of Udun"
	char = 'v'
	feat = FEAT_SHAL_LAVA
	type = TERRAIN_GRASS
	level = 50
	generator= {
		FEAT_MUD,
		FEAT_ASH,
		FEAT_SHAL_LAVA,
		FEAT_SHAL_LAVA,
		FEAT_MUD,
		FEAT_ASH,
		FEAT_SHAL_LAVA,
		FEAT_SHAL_LAVA,
		FEAT_MUD,
		FEAT_ASH,
		FEAT_SHAL_LAVA,
		FEAT_SHAL_LAVA,
		FEAT_MUD,
		FEAT_ASH,
		FEAT_SHAL_LAVA,
		FEAT_SHAL_LAVA,
		FEAT_MUD,
		FEAT_ASH,
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				ALLOW_IN_LAVA=true
			}
		}
	}
}

new_wilderness_terrain
{
	name = "Anduin"
	long_name = "Anduin, the great river"
	char = '~'
	feat = FEAT_ANDUIN
	type = TERRAIN_SHALLOW_WATER
	level = 35
	generator= {
		FEAT_DEEP_WATER,
		FEAT_ANDUIN,
		FEAT_DEEP_WATER,
		FEAT_SHAL_WATER,
		FEAT_SHAL_WATER,
		FEAT_ANDUIN,
		FEAT_SHAL_WATER,
		FEAT_SHAL_WATER,
		FEAT_LILY,
		FEAT_ANDUIN,
		FEAT_SHAL_WATER,
		FEAT_SHAL_WATER,
		FEAT_ANDUIN,
		FEAT_SHAL_WATER,
		FEAT_SHAL_WATER,
		FEAT_ANDUIN,
		FEAT_DIRT,
		FEAT_MUD,
	}
}


-- Bree Swamp
new_wilderness_terrain
{
	name = "Bree Swamp"
	long_name = "The Bree Swamp"
	char = 'i'
	feat = FEAT_RIVER
	type = TERRAIN_GRASS
	level = 1
	generator= {
		FEAT_DEEP_WATER,
		FEAT_DEEP_WATER,
		FEAT_DEEP_WATER,
		FEAT_SHAL_WATER,
		FEAT_SHAL_WATER,
		FEAT_SHAL_WATER,
		FEAT_SHAL_WATER,
		FEAT_SHAL_WATER,
		FEAT_LILY,
		FEAT_LILY,
		FEAT_SHAL_WATER,
		FEAT_SHAL_WATER,
		FEAT_SHAL_WATER,
		FEAT_SHAL_WATER,
		FEAT_SHAL_WATER,
		FEAT_DIRT,
		FEAT_DIRT,
		FEAT_GRASS,
	}
}

