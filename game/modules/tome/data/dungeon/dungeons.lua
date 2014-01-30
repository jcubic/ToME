-- See also scripts/dungeon_aux.lua.

--------------------------------------------------------
---- Special Wilderness definition ---------------------
--------------------------------------------------------

new_dungeon_type
{
	__index__ = 0
	define_as = "DUNGEON_WILDERNESS"
	name = "Wilderness" short_name = "Wdn"
	desc = "*A BUG*YOU should see this message!*"
	mindepth = 0 maxdepth = 0
	min_player_level = 1
	size_y = 3 size_x = 3
	min_monsters = 14
	alloc_chance = 500
	flags =
	{
		AMBIENT_MUSIC = "wilderness-travel"
		SURFACE_LITE=true
		FLAT=true NO_SHAFT=true NO_RECALL=true
		OBJ_THEME = getter.flags {
			THEME_TREASURE=25
			THEME_COMBAT=25
			THEME_MAGIC=25
			THEME_TOOLS=25
		}

		SAVE_LEVEL=true
		SAVED_LEVEL_DECAY_MONSTERS={100,400}
		SAVED_LEVEL_DECAY_OBJECTS={50,200}
		SAVED_LEVEL_DECAY_EFFECTS=true
		SAVED_LEVEL_DECAY_EXTRA=dungeons.maintain_decaying_level
		LEVEL_GENERATE_POST = function()
			-- Bypass maps once relevant quests have been completed:
			if quest(QUEST_WATCHER).status == QUEST_STATUS_COMPLETED then
				c_ptr = cave(32, 44)
				flag_empty(c_ptr.flags)
				cave_set_feat(32, 44, FEAT_MORE)
				c_ptr.flags[FLAG_DUNGEON] = DUNGEON_KHAZAD_DUM
				c_ptr.flags[FLAG_LEVEL_CHANGER] = 1
				-- Need a way to place the player on the west side when he arrives!
			end
			if quest(QUEST_SIEGE_OF_KHAZAD_DUM).status == QUEST_STATUS_COMPLETED then
				c_ptr = cave(32, 47)
				flag_empty(c_ptr.flags)
				cave_set_feat(32, 47, FEAT_MORE)
				c_ptr.flags[FLAG_DUNGEON] = DUNGEON_KHAZAD_DUM
				c_ptr.flags[FLAG_LEVEL_CHANGER] = 1
				-- Need a way to place the player on the east side when he arrives!
			end
			if quest(QUEST_GREAT_GOBLIN).status == QUEST_STATUS_COMPLETED then
			end
		end
	}
	floors =
	{
		[FEAT_GRASS] = 95
		[FEAT_FLOWER] = 5
	}
	walls =
	{
		inner = FEAT_TREES,
		outer = FEAT_SMALL_TREES,
		[FEAT_TREES] = 100
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				ALLOW_IN_EVERYWHERE=true
			}
		}
	}
}

--------------------------------------------------------
---- Base / start dungeons -----------------------------
--------------------------------------------------------
new_dungeon_type
{
	define_as = "DUNGEON_BARROW_DOWNS"
	name = "Barrow-Downs" short_name = "BDw"
	desc = "a way to the Barrow-Downs."
	mindepth = 1 maxdepth = 10
	min_player_level = 1
	size_y = 2 size_x = 2
	min_monsters = 14
	alloc_chance = 160
	fill_method = 3
	floors =
	{
		[FEAT_DIRT] = { 78, 0 }
		[FEAT_GRASS] = { 18, 95 }
		[FEAT_FLOWER] = { 4, 5 }
	}
	walls =
	{
		inner = FEAT_MOUNTAIN,
		outer = FEAT_TREES,
		[FEAT_TREES] = { 34, 100 }
		[FEAT_MOUNTAIN] = { 66, 0 }
	}
	entries = {
		top = {y = 21, x = 35}
	}
	flags =	{
		-- Entries.top is set so that if you reset recall to the
		-- Barrow-Downs and leave, you'll end up in Bree, but
		-- we don't want the player to enter it from the wilderness
		-- mode map.
		NO_SURFACE_ENTRY=true

--		AMBIENT_MUSIC = "barrow-downs"
		SURFACE_LITE=true
		FLAT=true NO_SHAFT=true
		OBJ_THEME = getter.flags {
			THEME_TREASURE=25
			THEME_COMBAT=25
			THEME_MAGIC=25
			THEME_TOOLS=25
		}
		DUNGEON_GUARDIAN = MONSTER_BARROWS_KING
		-- Can't be reached from overland-map (since we're embedded in
		-- Bree), but want to have fates anyways
		FATES_OK=true

		-- Make sure we're in Bree and right on top of the stairs when
		-- we leave
		STAIR_POST =
			function(updown)
				if updown == "up" and dun_level == 1 then
					player.wild_force_side_y_ok = false
					player.wild_force_side_x_ok = false
					player.wild_mode     = false
					player.old_wild_mode = true
					player.oldpy = 27
					player.oldpx = 13
					player.py    = 27
					player.px    = 13
					player.wilderness_y = 21
					player.wilderness_x = 35
				end
			end


		ROOM_DEFINITION = function(y, x)
			local k = rng(100)
			-- A bit  different room selection than the standard
			if rng(190) <= dun_level then
				if rng(190) <= dun_level then
					if k <= 10 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_GREATER_VAULT) then return true end
					if k <= 25 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_LESSER_VAULT) then return true end
					if k <= 40 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_NEST) then return true end
					if k <= 55 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_PIT) then return true end
					if k <= 60 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_RANDOM_VAULT) then return true end
				end
				if k <= 25 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_LARGE) then return true end
				if k <= 45 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_CROSS) then return true end
				if k <= 65 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_OVERLAPPING) then return true end
			end
			if k <= 6 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_NORMAL) then return true end
			if k <= 16 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_CIRCULAR) then return true end
			return dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_FRACTAL)
		end
		-- Smoothify the trees
                LEVEL_GENERATE_POST = function()
			for i = 1, cur_wid - 2 do for j = 1, cur_hgt - 2 do
				-- trees next to floor may become small trees
				if cave(j, i).feat == FEAT_TREES and (feat_adjacent(j, i, FEAT_DIRT) or feat_adjacent(j, i, FEAT_GRASS) or feat_adjacent(j, i, FEAT_MUD)) then
					if rng(5) <= 2 then
						cave(j, i).feat = FEAT_SMALL_TREES
					end
				end
				-- no doors
				if cave_feat_is(cave(j, i), FLAG_DOOR) then cave(j, i).feat = FEAT_SMALL_TREES end
			end end
		end
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				ALLOW_IN_BARROW_DOWNS=true
				ALLOW_IN_FOREST=true
				ALLOW_IN_EVERYWHERE=true
			}
		}
	}
}
new_dungeon_type
{
	define_as = "DUNGEON_ORC_MINES"
	name = "Orc Mines" short_name = "OMi"
	desc = "an entrance to the Orc Mines."
	mindepth = 1 maxdepth = 10
	min_player_level = 1
	size_y = 3 size_x = 3
	min_monsters = 14
	alloc_chance = 160
	fill_method = 0
	floors =
	{
		[FEAT_DIRT] = { 90, 60 }
		[FEAT_ASH] = { 10, 40 }
		[FEAT_DIRT] = { 0, 0 }
	}
	walls =
	{
		inner = FEAT_MOUNTAIN,
		outer = FEAT_WALL_INNER,
		[FEAT_MOUNTAIN] = { 100, 100 }
		[FEAT_WALL_EXTRA] = { 0, 0 }
		[FEAT_WALL_EXTRA] = { 0, 0 }
	}
	flags =
	{
		CAVE=true
		OBJ_THEME = getter.flags {
			THEME_TREASURE=5
			THEME_COMBAT=60
			THEME_MAGIC=10
			THEME_TOOLS=25
		}
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				--ALLOW_IN_ORC_MINES=true
				ALLOW_IN_EVERYWHERE=true
			}
		}
	}
}
new_dungeon_type
{
	define_as = "DUNGEON_FANGORN"
	name = "Fangorn forest" short_name = "FFo"
	desc = "a way into the corrupted part of Fangorn forest."
	mindepth = 1 maxdepth = 10
	min_player_level = 1
	size_y = 3 size_x = 3
	min_monsters = 14
	alloc_chance = 160
	fill_method = 3
	floors =
	{
		[FEAT_DIRT] = { 40, 70 }
		[FEAT_GRASS] = { 50, 30 }
		[FEAT_ANEMONES] = { 10, 0 }
	}
	walls =
	{
		inner = FEAT_DEAD_TREE,
		outer = FEAT_TREES,
		[FEAT_TREES] = { 25, 70 }
		[FEAT_MUD] = { 50, 25 }
		[FEAT_DEAD_TREE] = { 25, 5 }
	}
	flags =
	{
		SURFACE_LITE=true
		FLAT=true NO_SHAFT=true NO_STREAMERS=true
		CIRCULAR_ROOMS=true NO_DOORS=true
		OBJ_THEME = getter.flags {
			THEME_TREASURE=5
			THEME_COMBAT=25
			THEME_MAGIC=35
			THEME_TOOLS=35
		}
		-- Make some trees small, put some decayed trees in Fangorn, put some grass/dirt
		-- not a level generator, since we (sort of) want rooms.
			LEVEL_GENERATE_POST = function()
			for i = 1, cur_wid - 2 do for j = 1, cur_hgt - 2 do
				-- trees next to floor may become small trees
				if cave(j, i).feat == FEAT_TREES and (feat_adjacent(j, i, FEAT_DIRT) or feat_adjacent(j, i, FEAT_GRASS) or feat_adjacent(j, i, FEAT_MUD)) then
					if rng(5) <= 2 then
						cave(j, i).feat = FEAT_SMALL_TREES
					end
				end
				if cave(j, i).feat == FEAT_TREES and feat_adjacent(j, i, FEAT_DEAD_TREE) then
					if rng(5) <= 4 then
						cave(j, i).feat = FEAT_DECAYING_TREES
					end
				-- small trees next to dead trees will decay
				elseif cave(j, i).feat == FEAT_SMALL_TREES and feat_adjacent(j, i, FEAT_DEAD_TREE) then
					cave(j, i).feat = FEAT_DECAYING_SMALL_TREES
				-- change some mud to dirt / grass
				elseif cave(j, i).feat == FEAT_MUD then
					if feat_adjacent(j, i, FEAT_DIRT) then
						if rng(5) <= 2 then
							cave(j, i).feat = FEAT_DIRT
						end
					elseif feat_adjacent(j, i, FEAT_GRASS) then
						if rng(5) <= 2 then
							cave(j, i).feat = FEAT_SHORT_GRASS
						end
					end
				-- get rid of rubble
				elseif cave(j, i).feat == FEAT_RUBBLE then
					cave(j, i).feat = FEAT_GRASS
				end
			end end
		end

	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				--ALLOW_IN_FANGORN=true
				ALLOW_IN_FOREST=true
				ALLOW_IN_EVERYWHERE=true
			}
		}
	}
}
new_dungeon_type
{
	define_as = "DUNGEON_LAND_OF_RHUN"
	name = "The Land of Rhun" short_name = "LoR"
	desc = "a way to the Land of Rhun."
	mindepth = 1 maxdepth = 10
	min_player_level = 1
	size_y = 3 size_x = 3
	min_monsters = 14
	alloc_chance = 160
	fill_method = 3
	floors =
	{
		-- the floor tiles aren't actually used.
		[FEAT_GRASS] = { 75, 0 }
		[FEAT_LONG_GRASS] = { 20, 90 }
		[FEAT_ELANOR] = { 5, 10 }
	}
	walls =
	{
		inner = FEAT_DIRT,
		outer = FEAT_DIRT,
		[FEAT_DIRT] = { 75, 0 }
		[FEAT_GRASS] = { 15, 70 }
		[FEAT_TALL_GRASS] = { 10, 30 }
	}
	flags =
	{
		SURFACE_LITE=true
		FLAT=true NO_SHAFT=true
		NO_DOORS=true
		OBJ_THEME = getter.flags {
			THEME_TREASURE=25
			THEME_COMBAT=25
			THEME_MAGIC=25
			THEME_TOOLS=25
		}
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				--ALLOW_IN_LAND_OF_RHUN=true
				ALLOW_IN_PLAIN=true
				ALLOW_IN_EVERYWHERE=true
			}
		}
	}
	-- so we don't get rooms/corridors.
	-- (+ adds smoothing)
	generator = "gen_land_of_rhun"
}

-- Catacombs (start dungeon for evil races) to be added here later...

--------------------------------------------------------
---- Main / plot dungeons ------------------------------
--------------------------------------------------------

-- The swamp, only one gigantic level
new_dungeon_type
{
	define_as = "DUNGEON_ANDUIN_SWAMP"
	name = "Swamp around the Anduin river" short_name = "Swp"
	desc = "a way to the swamp."
	mindepth = 25 maxdepth = 25
	min_player_level = 10
	size_y = 2 size_x = 2
	min_monsters = 50
	alloc_chance = 160
	fill_method = 3
	floors =
	{
		[FEAT_SHAL_WATER] = { 70, 70 }
		[FEAT_DIRT] = { 18, 18 }
		[FEAT_TALL_GRASS] = { 12, 12 }
	}
	walls =
	{
		inner = FEAT_WALL_INNER,
		outer = FEAT_WALL_OUTER,
		[FEAT_SWAMP_POOL] = { 50, 50 }
		[FEAT_DIRT] = { 28, 28 }
		[FEAT_TALL_GRASS] = { 22, 22 }
	}
	entries = {
		top = {y = 48, x = 60}
		bottom = {y = 50, x = 62}
	}
	flags =
	{
		ENTRANCE_MIMIC = FEAT_SWAMP_POOL
		SURFACE_LITE=true
		OBJ_THEME = getter.flags {
			THEME_TREASURE=25
			THEME_COMBAT=25
			THEME_MAGIC=20
			THEME_TOOLS=20
		}
		FLAT = true
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				AQUATIC=true
				CAN_SWIM=true
				ALLOW_IN_EVERYWHERE=true
			}
		}
	}
	-- make ponds of water and such, ohh sooooooo neat
	generator = "gen_swamp_anduin"
}

-- The orc caves which Bilbo used to pass the misty mountains
new_dungeon_type
{
	define_as = "DUNGEON_ORC_CAVES"
	name = "Orc Caves" short_name = "Orc"
	desc = "a dark tunnel leading to the orc caves beneath the Misty Mountains."
	mindepth = 12 maxdepth = 25
	min_player_level = 12
	size_y = 3 size_x = 3
	min_monsters = 35
	alloc_chance = 150
	fill_method = 3
	floors =
	{
		[FEAT_DIRT] = { 90, 60 }
		[FEAT_ASH] = { 10, 40 }
	}
	entries = {
		top = {y = 21, x = 49}
		bottom = {y = 21, x = 52}
	}
	walls =
	{
		inner = FEAT_MOUNTAIN,
		outer = FEAT_MOUNTAIN,
		[FEAT_MOUNTAIN] = { 70, 70 }
		[FEAT_DARK_PIT] = { 10, 10 }
		[FEAT_WALL_OUTER] = { 20, 20 }
	}
	flags =
	{
		OBJ_THEME = getter.flags {
			THEME_TREASURE=5
			THEME_COMBAT=60
			THEME_MAGIC=10
			THEME_TOOLS=25
		}
		ROOM_DEFINITION = function(y, x)
			local k = rng(100)
			-- A bit  different room selection than the standard
			if rng(190) <= dun_level then
				if rng(190) <= dun_level then
					if k <= 10 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_GREATER_VAULT) then return true end
					if k <= 25 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_LESSER_VAULT) then return true end
					if k <= 40 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_NEST) then return true end
					if k <= 55 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_PIT) then return true end
					if k <= 60 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_RANDOM_VAULT) then return true end
				end
				if k <= 25 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_LARGE) then return true end
				if k <= 45 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_CROSS) then return true end
				if k <= 65 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_OVERLAPPING) then return true end
			end
			if k <= 30 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_NORMAL) then return true end
			if k <= 60 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_CIRCULAR) then return true end
			return dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_FRACTAL)
		end
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				--ALLOW_IN_ORC_CAVES=true
				ALLOW_IN_EVERYWHERE=true
			}
		}
	}
}

new_dungeon_type
{
	define_as = "DUNGEON_MIRKWOOD"
	name = "Mirkwood forest" short_name = "Mkw"
	desc = "a way into Mirkwood forest."
	mindepth = 30 maxdepth = 45
	min_player_level = 9
	size_y = 3 size_x = 3
	min_monsters = 14
	alloc_chance = 160
	fill_method = 0
	floors =
	{
		[FEAT_DIRT] = { 85, 95 }
		[FEAT_FLOWER] = { 5, 0 }
		[FEAT_GRASS] = { 10, 5 }
	}
	walls =
	{
		inner = FEAT_SMALL_TREES,
		outer = FEAT_TREES,
		[FEAT_TREES] = { 100, 100 }
		[FEAT_TREES] = { 0, 0 }
		[FEAT_TREES] = { 0, 0 }
	}
	entries = {
		top = {y = 23, x = 57}
	}
	flags =
	{
		SURFACE_LITE=true
		FLAT=true NO_SHAFT=true NO_STREAMERS=true
		CIRCULAR_ROOMS=true NO_DOORS=true
		OBJ_THEME = getter.flags {
			THEME_TREASURE=20
			THEME_COMBAT=20
			THEME_MAGIC=20
			THEME_TOOLS=20
		}
		ROOM_DEFINITION = function(y, x)
			local k = rng(100)
			-- A bit  different room selection than the standard
			if rng(190) <= dun_level then
				if rng(190) <= dun_level then
					if k <= 10 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_GREATER_VAULT) then return true end
					if k <= 25 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_LESSER_VAULT) then return true end
					if k <= 40 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_NEST) then return true end
					if k <= 55 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_PIT) then return true end
					if k <= 60 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_RANDOM_VAULT) then return true end
				end
				if k <= 25 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_LARGE) then return true end
				if k <= 45 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_CROSS) then return true end
				if k <= 65 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_OVERLAPPING) then return true end
			end
			if k <= 26 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_CIRCULAR) then return true end
			return dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_FRACTAL)
		end
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				ALLOW_IN_MIRKWOOD=true
				ALLOW_IN_EVERYWHERE=true
			}
		}
	}
}
new_dungeon_type
{
	define_as = "DUNGEON_ISENGARD"
	name = "Isengard" short_name = "Igd"
	desc = "a passage to Isengard."
	mindepth = 45 maxdepth = 55
	min_player_level = 67
	size_y = 2 size_x = 2
	min_monsters = 24
	alloc_chance = 160
	fill_method = 0
	floors =
	{
		[FEAT_FLOOR] = { 80, 100 }
		[FEAT_TAINTED_WATER] = { 20, 0 }
		[FEAT_FLOOR] = { 0, 0 }
	}
	walls =
	{
		inner = FEAT_WALL_INNER,
		outer = FEAT_WALL_INNER,
		[FEAT_WALL_INNER] = { 100, 0 }
		[FEAT_WALL_EXTRA] = { 0, 0 }
		[FEAT_WALL_EXTRA] = { 0, 0 }
	}
	entries = {
		top = {y = 42, x = 43}
	}
	flags =
	{
		TOWER=true
		OBJ_THEME = getter.flags {
			THEME_TREASURE=20
			THEME_COMBAT=1
			THEME_MAGIC=70
			THEME_TOOLS=9
		}
		--DUNGEON_GUARDIAN = MONSTER_SARUMAN
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				--ALLOW_IN_ISENGUARD=true
				ALLOW_IN_EVERYWHERE=true
			}
		}
	}
}

new_dungeon_type
{
	name = "Dol Guldur" short_name = "TDG"
	desc = "a gate leading to the tower of Dol Guldur."
	mindepth = 60 maxdepth = 70
	min_player_level = 69
	min_monsters = 24
	alloc_chance = 160
	fill_method = 3
	floors =
	{
		[FEAT_FLOOR] = 80
		[FEAT_TAINTED_WATER] = 20
	}
	walls =
	{
		inner = FEAT_WALL_OUTER,
		outer = FEAT_WALL_INNER,
		[FEAT_WALL_EXTRA] = 100
	}
	flags =
	{
		DUNGEON_GUARDIAN=RACE_NECROMANCER_DOL_GULDUR
		OBJ_THEME = getter.flags {
			THEME_TREASURE=25
			THEME_COMBAT=25
			THEME_MAGIC=35
			THEME_TOOLS=15
		}
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				--ALLOW_IN_DOL_GULDUR=true
				ALLOW_IN_EVERYWHERE=true
			}
		}
	}
}

new_dungeon_type
{
	define_as = "DUNGEON_CIRITH_UNGOL"
	name = "Cirith Ungol" short_name = "CUg"
	desc = "a passage into Cirith Ungol."
	mindepth = 70 maxdepth = 75
	min_player_level = 19
	size_y = 3 size_x = 3
	min_monsters = 14
	alloc_chance = 160
	fill_method = 2
	floors =
	{
		[FEAT_DARK_PIT] = { 5, 10 }
		[FEAT_DIRT] = { 90, 65 }
		[FEAT_WEB] = { 5, 25 }
	}
	walls =
	{
		inner = FEAT_WALL_INNER,
		outer = FEAT_WEB,
		[FEAT_MOUNTAIN] = { 90, 95 }
		[FEAT_WEB] = { 10, 5 }
		[FEAT_WALL_EXTRA] = { 0, 0 }
	}
	entries = {
		top = {y = 56, x = 65}
	}
	flags =
	{
		AMBIENT_MUSIC = "cirith-ungol"
		AMBIENT_SOUNDS_FREQUENCY = 40
		AMBIENT_SOUNDS = getter.array{ "creaking-door" "creaking-door" "thunder" }

		NO_SHAFT=true
		OBJ_THEME = getter.flags {
			THEME_TREASURE=30
			THEME_COMBAT=30
			THEME_MAGIC=30
			THEME_TOOLS=10
		}
		TERRAIN_IGNORE = getter.array {
			[FEAT_WEB] = true
		}

		--DUNGEON_GUARDIAN = MONSTER_SHELOB
		ROOM_DEFINITION = function(y, x)
			local k = rng(100)
			-- A bit  different room selection than the standard
			if rng(190) <= dun_level then
				if rng(190) <= dun_level then
					if k <= 10 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_GREATER_VAULT) then return true end
					if k <= 25 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_LESSER_VAULT) then return true end
					if k <= 40 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_NEST) then return true end
					if k <= 55 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_PIT) then return true end
					if k <= 60 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_RANDOM_VAULT) then return true end
				end
				if k <= 25 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_LARGE) then return true end
				if k <= 45 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_CROSS) then return true end
				if k <= 65 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_OVERLAPPING) then return true end
			end
			if k <= 6 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_NORMAL) then return true end
			if k <= 16 and dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_CIRCULAR) then return true end
			return dungeon_generator.room_build(y, x, dungeon_generator.ROOM_TYPE_FRACTAL)
		end
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				--ALLOW_IN_CIRITH_UNGOL=true
				ALLOW_IN_EVERYWHERE=true
			}
		}
	}
}
new_dungeon_type
{
	define_as = "DUNGEON_MORDOR"
	name = "Mordor" short_name = "Mdr"
	desc = "a way to the land of Mordor."
	mindepth = 754 maxdepth = 85
	min_player_level = 29
	size_y = 3 size_x = 3
	min_monsters = 14
	alloc_chance = 160
	fill_method = 2
	floors =
	{
		[FEAT_DIRT] = { 67, 0 }
		[FEAT_ASH] = { 33, 100 }
		[FEAT_FLOOR] = { 0, 0 }
	}
	walls =
	{
		inner = FEAT_WALL_INNER,
		outer = FEAT_MOUNTAIN,
		[FEAT_MOUNTAIN] = { 50, 0 }
		[FEAT_WALL_INNER] = { 50, 100 }
		[FEAT_WALL_EXTRA] = { 0, 0 }
	}
	entries = {
		top = {y = 54, x = 69}
	}
	flags =
	{
		AMBIENT_MUSIC = "death"
		NO_STREAMERS=true LAVA_RIVER=true CAVERN=true
		OBJ_THEME = getter.flags {
			THEME_TREASURE=5
			THEME_COMBAT=25
			THEME_MAGIC=35
			THEME_TOOLS=35
		}
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				--ALLOW_IN_MORDOR=true
				ALLOW_IN_EVERYWHERE=true
			}
		}
	}
}
new_dungeon_type
{
	define_as = "DUNGEON_BARAD_DUR"
	name = "Barad-dur" short_name = "Bdr"
	desc = "a path to the dark tower."
	mindepth = 85 maxdepth = 100
	min_player_level = 59
	size_y = 3 size_x = 3
	min_monsters = 14
	alloc_chance = 160
	fill_method = 0
	floors =
	{
		[FEAT_FLOOR] = { 100, 100 }
	}
	walls =
	{
		inner = FEAT_WALL_INNER,
		outer = FEAT_WALL_OUTER,
		[FEAT_WALL_INNER] = { 100, 100 }
	}
	entries = {
		top = {y = 49, x = 74}
	}
	flags =
	{
		DUNGEON_GUARDIAN = RACE_SAURON
		NO_EASY_MOVE=true
		--ADJUST_LEVEL_1_2=true ADJUST_LEVEL_1=true
		OBJ_THEME = getter.flags {
			THEME_TREASURE=20
			THEME_COMBAT=20
			THEME_MAGIC=20
			THEME_TOOLS=20
		}
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				--ALLOW_IN_BARAD_DUR=true
				ALLOW_IN_EVERYWHERE=true
			}
		}
	}
}
new_dungeon_type
{
	define_as = "DUNGEON_ORODRUIN"
	name = "Orodruin" short_name = "Oro"
	desc = "a path to the heart of Orodruin."
	mindepth = 85 maxdepth = 99
	min_player_level = 59
	size_y = 2 size_x = 2
	min_monsters = 14
	alloc_chance = 160
	fill_method = 0
	floors =
	{
		[FEAT_SHAL_LAVA] = { 80, 80 }
		[FEAT_FIRE] = { 10, 20 }
		[FEAT_ASH] = { 10, 0 }
	}
	walls =
	{
		inner = FEAT_DEEP_LAVA,
		outer = FEAT_DARK_PIT,
		[FEAT_LAVA_WALL] = { 100, 100 }
		[FEAT_WALL_EXTRA] = { 0, 0 }
		[FEAT_WALL_EXTRA] = { 0, 0 }
	}
	entries = {
		top = {y = 50, x = 70}
	}
	flags =
	{
		CAVE=true LAVA_RIVER=true NO_STREAMERS=true
		NO_EASY_MOVE=true NO_DOORS=true NO_RECALL=true
		OBJ_THEME = getter.flags {
			THEME_TREASURE=10
			THEME_COMBAT=10
			THEME_MAGIC=30
			THEME_TOOLS=30
		}
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				--ALLOW_IN_MOUNT_DOOM=true
				--IM_FIRE=true
				ALLOW_IN_EVERYWHERE=true
			}
		}
	}
	levels =
	{
		[99] =
		{
			name = "Mt Doom"
			level_map = "maps/mount_doom.map"
			desc = "You finally reach the top of Mount Doom, here must lie the Great Fire."
			flags = {
				SAVE_LEVEL=true
				NO_GENO=true NO_NEW_MONSTER=true NO_TELEPORT=true
				-- Fates here might prevent player from destroying
				-- the One Ring.  Probably won't happen, but let's
				-- be paranoid
				NO_EXEC_FATES=true
			}
		}
	}
}
--------------------------------------------------------
---- Side Dungeons -------------------------------------
--------------------------------------------------------
new_dungeon_type
{
	define_as = "DUNGEON_NUMENOR"
	name = "Submerged ruins of Numenor" short_name = "Num"
	desc = "a way to the submerged ruins."
	mindepth = 35 maxdepth = 50
	min_player_level = 49
	size_y = 3 size_x = 3
	min_monsters = 14
	alloc_chance = 160
	fill_method = 3
	floors =
	{
		[FEAT_DEEP_WATER] = { 20, 95 }
		[FEAT_SHAL_WATER] = { 80, 5 }
		[FEAT_FLOOR] = { 0, 0 }
	}
	walls =
	{
		inner = FEAT_DEEP_WATER,
		outer = FEAT_WALL_INNER,
		[FEAT_DEEP_WATER] = { 78, 81 }
		[FEAT_SHAL_WATER] = { 7, 4 }
		[FEAT_WALL_INNER] = { 15, 15 }
	}
	entries = {
		top = {y = 25, x = 1}
	}
	flags =
	{
		NO_SHAFT=true NO_STREAMERS=true WATER_BREATH=true
		OBJ_THEME = getter.flags {
			THEME_TREASURE=30
			THEME_COMBAT=30
			THEME_MAGIC=30
			THEME_TOOLS=10
		}
		--DUNGEON_GUARDIAN = MONSTER_AR_PHARAZON
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				--ALLOW_IN_NUMENOR=true
				AQUATIC=true
				CAN_SWIM=true
				ALLOW_IN_OCEAN=true
				ALLOW_IN_EVERYWHERE=true
			}
		}
	}
}
new_dungeon_type
{
	define_as = "DUNGEON_EREBOR"
	name = "Erebor" short_name = "Ere"
	desc = "a passage into the Lonely Mountain."
	mindepth = 60 maxdepth = 72
	min_player_level = 69
	size_y = 3 size_x = 3
	min_monsters = 20
	alloc_chance = 160
	fill_method = 2
	floors =
	{
		[FEAT_DIRT] = { 100, 100 }
		[FEAT_FLOOR] = { 0, 0 }
		[FEAT_FLOOR] = { 0, 0 }
	}
	walls =
	{
		inner = FEAT_MOUNTAIN,
		outer = FEAT_WALL_INNER,
		[FEAT_MOUNTAIN] = { 90, 90 }
		[FEAT_DARK_PIT] = { 10, 10 }
		[FEAT_WALL_EXTRA] = { 0, 0 }
	}
	entries = {
		top = {y = 15, x = 66}
	}
	flags =
	{
		LAVA_RIVER=true CAVERN=true NO_STREAMERS=true
		DOUBLE=true NO_RECALL=true
		OBJ_THEME = getter.flags {
			THEME_TREASURE=25
			THEME_COMBAT=25
			THEME_MAGIC=25
			THEME_TOOLS=25
		}
		--DUNGEON_GUARDIAN = MONSTER_SMAUG?
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				--ALLOW_IN_EREBOR=true
				DRAGON=true
				ALLOW_IN_EVERYWHERE=true
			}
		}
	}
}

-- Temporarily removed, pending worlmap redesign
--[[
new_dungeon_type
{
	define_as = "DUNGEON_OLD_FOREST"
	name = "Old forest" short_name = "OFr"
	desc = "a path into the Old Forest."
	mindepth = 13 maxdepth = 25
	min_player_level = 9
	size_y = 3 size_x = 3
	min_monsters = 15
	alloc_chance = 100
	fill_method = 3
	floors =
	{
		[FEAT_DIRT] = { 76, 68 }
		[FEAT_SHAL_WATER] = { 16, 16 }
		[FEAT_FLOWER] = { 8, 16 }
	}
	entries = {
		top = {y = 21, x = 31}
	}
	walls =
	{
		inner = FEAT_TREES,
		outer = FEAT_SMALL_TREES,
		[FEAT_TREES] = { 100, 100 }
		[FEAT_WALL_EXTRA] = { 0, 0 }
		[FEAT_WALL_EXTRA] = { 0, 0 }
	}
	flags =
	{
		SURFACE_LITE=true
		LAVA_RIVER=true CAVERN=true NO_STREAMERS=true
		DOUBLE=true
		OBJ_THEME = getter.flags {
			THEME_TREASURE=20
			THEME_COMBAT=5
			THEME_MAGIC=15
			THEME_TOOLS=30
		}
		DUNGEON_GUARDIAN = MONSTER_OLD_MAN_WILLOW
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				ALLOW_IN_EVERYWHERE=true
				ALLOW_IN_FOREST=true
				ALLOW_IN_OLD_FOREST=true
			}
		}
	}
}
]]
new_dungeon_type
{
	define_as = "DUNGEON_WITHERED_HEATH"
	name = "Withered Heath" short_name = "Wth"
	desc = "a path to the Withered Heath."
	mindepth = 70 maxdepth = 90
	min_player_level = 9
	size_y = 3 size_x = 3
	min_monsters = 18
	alloc_chance = 100
	fill_method = 4
	floors =
	{
		[FEAT_DIRT] = { 40, 30 }
		[FEAT_ICE] = { 50, 20 }
		[FEAT_ASH] = { 10, 50 }
	}
	walls =
	{
		inner = FEAT_DEAD_SMALL_TREE,
		outer = FEAT_DEAD_TREE,
		[FEAT_ASH] = { 10, 50 }
		[FEAT_DIRT] = { 40, 30 }
		[FEAT_ICE] = { 50, 20 }
	}
	entries = {
		top = {y = 7, x = 57}
	}
	flags =
	{
		NO_DOORS=true CAVERN=true NO_DESTROY=true EMPTY=true
		FLAT=true NO_STREAMERS=true --COLD=true LIFE_LEVEL=true
		OBJ_THEME = getter.flags {
			THEME_TREASURE=20
			THEME_COMBAT=20
			THEME_MAGIC=20
			THEME_TOOLS=20
		}
		TERRAIN_IGNORE = getter.array {
			[FEAT_ICE] = true
			[FEAT_DEAD_TREE] = true
		}
		--DUNGEON_GUARDIAN = MONSTER_THE_HUNTER
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				--ALLOW_IN_WITHERED_HEATH=true
				DRAGON=true
				ALLOW_IN_EVERYWHERE=true
			}
		}
	}
}
new_dungeon_type
{
	define_as = "DUNGEON_HELCARAXE"
	name = "Helcaraxe" short_name = "Ice"
	desc = "a path to the Grinding Ice."
	mindepth = 20 maxdepth = 40
	min_player_level = 19
	size_y = 3 size_x = 3
	min_monsters = 14
	alloc_chance = 160
	fill_method = 2
	floors =
	{
		[FEAT_ICE] = { 0, 90 }
		[FEAT_DIRT] = { 70, 0 }
		[FEAT_SHAL_WATER] = { 30, 10 }
	}
	walls =
	{
		inner = FEAT_WALL_INNER,
		outer = FEAT_WALL_INNER,
		[FEAT_ICE_WALL] = { 0, 100 }
		[FEAT_WALL_INNER] = { 100, 0 }
		[FEAT_WALL_EXTRA] = { 0, 0 }
	}
	entries = {
		top = {y = 1, x = 9}
	}
	flags =
	{
		SURFACE_LITE=true
		DOUBLE=true WATER_RIVER=true CAVERN=true NO_STREAMERS=true
		FLAT=true
		OBJ_THEME = getter.flags {
			THEME_TREASURE=20
			THEME_COMBAT=20
			THEME_MAGIC=20
			THEME_TOOLS=20
		}
		TERRAIN_IGNORE = getter.array {
			[FEAT_ICE] = true
		}
		--DUNGEON_GUARDIAN = MONSTER_LUNGORTHIN
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				--ALLOW_IN_HELCARAXE=true
				--IM_COLD=true
				ALLOW_IN_EVERYWHERE=true
			}
		}
	}
}
new_dungeon_type
{
	define_as = "DUNGEON_ERED_LUIN"
	name = "Ered Luin" short_name = "ELu"
	desc = "a path into the Blue Mountains."
	mindepth = 60 maxdepth = 70
	min_player_level = 59
	size_y = 3 size_x = 3
	min_monsters = 14
	alloc_chance = 160
	fill_method = 3
	floors =
	{
		[FEAT_GRASS] = { 45, 30 }
		[FEAT_NIPHREDIL] = { 30, 15 }
		[FEAT_DIRT] = { 25, 55 }
	}
	walls =
	{
		inner = FEAT_HAIL,
		outer = FEAT_WALL_INNER,
		[FEAT_BLUE_MOUNTAIN] = { 100, 100 }
		[FEAT_WALL_EXTRA] = { 0, 0 }
		[FEAT_WALL_EXTRA] = { 0, 0 }
	}
	entries = {
		top = {y = 29, x = 18}
	}
	flags =
	{
		CAVERN=true CIRCULAR_ROOMS=true RANDOM_TOWNS=true NO_STREAMERS=true
		NO_DESTROY=true
		OBJ_THEME = getter.flags {
			THEME_TREASURE=50
			THEME_COMBAT=10
			THEME_MAGIC=10
			THEME_TOOLS=30
		}
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				--ALLOW_IN_ERED_LUIN=true
				ALLOW_IN_EVERYWHERE=true
			}
		}
	}
}
new_dungeon_type
{
	define_as = "DUNGEON_NEAR_HARAD"
	name = "Near Harad" short_name = "NHa"
	desc = "a desert path to Near Harad."
	mindepth = 20 maxdepth = 25
	min_player_level = 19
	size_y = 3 size_x = 3
	min_monsters = 14
	alloc_chance = 160
	fill_method = 0
	floors =
	{
		[FEAT_SAND] = { 100, 100 }
		[FEAT_FLOOR] = { 0, 0 }
		[FEAT_FLOOR] = { 0, 0 }
	}
	walls =
	{
		inner = FEAT_SAND,
		outer = FEAT_SANDWALL,
		[FEAT_SAND] = { 100, 100 }
		[FEAT_WALL_EXTRA] = { 0, 0 }
		[FEAT_WALL_EXTRA] = { 0, 0 }
	}
	entries = {
		top = {y = 64, x = 97}
	}
	flags =
	{
		SURFACE_LITE=true
		NO_DOORS=true CAVERN=true HOT=true NO_DESTROY=true EMPTY=true FLAT=true
		RANDOM_TOWNS=true NO_STREAMERS=true NO_RECALL=true
		OBJ_THEME = getter.flags {
			THEME_TREASURE=20
			THEME_COMBAT=20
			THEME_MAGIC=20
			THEME_TOOLS=20
		}
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				--ALLOW_IN_NEAR_HARAD=true
				ALLOW_IN_EVERYWHERE=true
			}
		}
	}
}
new_dungeon_type
{
	define_as = "DUNGEON_SANDWORM_LAIR"
	name = "Sandworm Lair" short_name = "SwL"
	desc = "a sandhole."
	mindepth = 22 maxdepth = 30
	min_player_level = 23
	size_y = 3 size_x = 3
	min_monsters = 5
	alloc_chance = 200
	fill_method = 0
	floors =
	{
		[FEAT_SAND] = { 85, 85 }
		[FEAT_MUD] = { 10, 10 }
		[FEAT_ASH] = { 5, 5 }
	}
	walls =
	{
		inner = FEAT_MUD,
		outer = FEAT_MUD,
		[FEAT_SANDWALL] = { 100, 100 }
		[FEAT_WALL_EXTRA] = { 0, 0 }
		[FEAT_WALL_EXTRA] = { 0, 0 }
	}
	entries = {
		top = {y = 64, x = 96}
	}
	flags =
	{
		NO_DOORS=true SAND_VEIN=true
		OBJ_THEME = getter.flags {
			THEME_TREASURE=15
			THEME_COMBAT=5
			THEME_MAGIC=60
			THEME_TOOLS=20
		}
		TERRAIN_IGNORE = getter.array {
			[FEAT_SANDWALL] = true
		}
	}
	rules =
	{
		[{100, "and"}] =
		{
			flags = {
				ALLOW_IN_EVERYWHERE=true
			}
			races = {"w"}
		}
	}
}
new_dungeon_type
{
	define_as = "DUNGEON_MORIA"
	name = "Moria" short_name = "MoM"
	desc = "a stone door leading to the Mines of Moria."
	mindepth = 30 maxdepth = 50
	min_player_level = 30
	size_y = 3 size_x = 3
	min_monsters = 40
	alloc_chance = 40
	fill_method = 0
	floors =
	{
		[FEAT_DIRT] = { 100, 100 }
		[FEAT_FLOOR] = { 0, 0 }
		[FEAT_FLOOR] = { 0, 0 }
	}
	walls =
	{
		inner = FEAT_MOUNTAIN,
		outer = FEAT_WALL_INNER,
		[FEAT_MOUNTAIN] = { 100, 100 }
		[FEAT_MOUNTAIN] = { 0, 0 }
		[FEAT_MOUNTAIN] = { 0, 0 }
	}
--	entries = {
--		top    = {y = 30, x = 45}
--		bottom = {y = 38, x = 44}
--	}
	flags =
	{
		DUNGEON_GUARDIAN = RACE_DURINS_BANE
		WATER_RIVER=true NO_STREAMERS=true FORCE_DOWN=true
		OBJ_THEME = getter.flags {
			THEME_TREASURE=30
			THEME_COMBAT=50
			THEME_MAGIC=10
			THEME_TOOLS=5
		}
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				--ALLOW_IN_MORIA=true
				ALLOW_IN_EVERYWHERE=true
				-- races = {"o", "T", "u", "U", "s"}

			}
		}
	}
}
new_dungeon_type
{
	define_as = "DUNGEON_PATHS_OF_THE_DEAD"
	name = "Paths of the Dead" short_name = "PoD"
	desc = "the entrance to the Paths of the Dead."
	mindepth = 40 maxdepth = 70
	min_player_level = 45
	size_y = 3 size_x = 3
	min_monsters = 24
	alloc_chance = 100
	fill_method = 3
	floors =
	{
		[FEAT_DIRT] = { 85, 85 }
		[FEAT_SHAL_WATER] = { 15, 15 }
		[FEAT_FLOOR] = { 0, 0 }
	}
	walls =
	{
		inner = FEAT_WALL_INNER,
		outer = FEAT_WALL_INNER,
		[FEAT_WALL_INNER] = { 75, 75 }
		[FEAT_DARK_PIT] = { 25, 25 }
		[FEAT_MOUNTAIN] = { 0, 0 }
	}
	entries = {
		top = {y = 49, x = 83}
	}
	flags =
	{
		OBJ_THEME = getter.flags {
			THEME_TREASURE=30
			THEME_COMBAT=50
			THEME_MAGIC=10
			THEME_TOOLS=5
		}
	}
	rules =
	{
		[{100, "and"}] =
		{
			flags = {
				--ALLOW_IN_PATHS_OF_THE_DEAD=true
				ALLOW_IN_EVERYWHERE=true
				UNDEAD=true
			}
		}
	}
}


-- Illusory Castle is going away, yes? - BucketMan
--[[
new_dungeon_type
{
	define_as = "DUNGEON_ILLUSORY_CASTLE"
	name = "Illusory Castle" short_name = "Ill"
	desc = "an entrance to the Illusory Castle."
	mindepth = 35 maxdepth = 52
	min_player_level = 29
	size_y = 2 size_x = 2
	min_monsters = 24
	alloc_chance = 100
	fill_method = 1
	floors =
	{
		[FEAT_FLOOR] = { 98, 95 }
		[FEAT_ETHEREAL] = { 2, 5 }
		[FEAT_FLOOR] = { 0, 0 }
	}
	walls =
	{
		inner = FEAT_WALL_INNER,
		outer = FEAT_WALL_INNER,
		[FEAT_WALL_INNER] = { 50, 50 }
		[FEAT_ILLUS_WALL] = { 50, 50 }
		[FEAT_WALL_EXTRA] = { 0, 0 }
	}
	entries = {
		top = {y = 34, x = 43}
	}
	flags =
	{
		RANDOM_TOWNS=true NO_STREAMERS=true
		OBJ_THEME = getter.flags {
			THEME_TREASURE=30
			THEME_COMBAT=50
			THEME_MAGIC=10
			THEME_TOOLS=5
		}
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				--ALLOW_IN_ILLUSORY_CASTLE=true
				ALLOW_IN_EVERYWHERE=true
			}
		}
	}
}
]]

-- Maze is also going away, yes? - BucketMan
--[[
new_dungeon_type
{
	define_as = "DUNGEON_MAZE"
	name = "Maze" short_name = "Maz"
	desc = "a small tunnel leading to a maze of twisty little passages, all alike."
	mindepth = 25 maxdepth = 37
	min_player_level = 29
	size_y = 1 size_x = 1
	min_monsters = 20
	alloc_chance = 160
	fill_method = 1
	floors =
	{
		[FEAT_FLOOR] = { 100, 100 }
		[FEAT_FLOOR] = { 0, 0 }
		[FEAT_FLOOR] = { 0, 0 }
	}
	walls =
	{
		inner = FEAT_WALL_INNER,
		outer = FEAT_WALL_INNER,
		[FEAT_WALL_INNER] = { 98, 98 }
		[FEAT_DOOR] = { 2, 2 }
		[FEAT_WALL_EXTRA] = { 0, 0 }
	}
	entries = {
		top = {y = 57, x = 27}
	}
	flags =
	{
		FORGET=true
		RANDOM_TOWNS=true NO_STREAMERS=true
		OBJ_THEME = getter.flags {
			THEME_TREASURE=2
			THEME_COMBAT=40
			THEME_MAGIC=10
			THEME_TOOLS=40
		}
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				--ALLOW_IN_MAZE=true
				ALLOW_IN_EVERYWHERE=true
			}
		}
	}
	generator = "maze"
}
]]

--------------------------------------------------------
---- Special Purpose Dungeons --------------------------
--------------------------------------------------------

new_dungeon_type
{
	define_as = "DUNGEON_MANDOS"
	name = "Halls of Mandos" short_name = "HMa"
	desc = "*A BUG* you shouldn't see this message!*"
	mindepth = 1 maxdepth = 98
	min_player_level = 1
	size_y = 3 size_x = 3
	min_monsters = 14
	alloc_chance = 160
	fill_method = 0
	floors =
	{
		[FEAT_FLOOR] = 100
	}
	walls =
	{
		inner = FEAT_WALL_OUTER,
		outer = FEAT_WALL_INNER,
		[FEAT_WALL_EXTRA] = 100
	}
	entries = {
		top = {y = 19, x = 49}
	}
	flags =
	{
		RANDOM_TOWNS=true NO_RECALL=true NO_SHAFT=true
		OBJ_THEME = getter.flags {
			THEME_TREASURE=25
			THEME_COMBAT=25
			THEME_MAGIC=25
			THEME_TOOLS=25
		}

		-- Player can see invislbe monsters and pass through walls while
		-- in the Halls
		LEVEL_BONUS = getter.flags {
			SEE_INVIS = 1
			PASS_WALL = getter.flags {
				PASS_INCORP = 10000
			}
		}

		-- The Halls are always lit, with all items known
		LEVEL_END_GEN = function()
				wiz_lite_extra()
			end

		-- Once you leave, you can't get back in
		NO_SURFACE_ENTRY = true
		NO_RECALL_OUT="As an astral being you can't recall."

		LEFT_DUNGEON =
			function()
				message(color.LIGHT_GREEN,
						"Well done!  You have reached the surface!")
			end

		-- No fates while astral
		NO_NEW_FATES=true NO_EXEC_FATES=true

		-- No quests while astral
		NO_NEW_QUESTS=true
	}
	rules =
	{
		[{100, "not or"}] =
		{
			flags = { UNIQUE=true AQUATIC=true}
		}
	}
}

--------------------------------------------------------
---- Quest Dungeons ------------------------------------
--------------------------------------------------------

new_dungeon_type
{
	define_as = "DUNGEON_THIEVES_LAIR"
	name = "Thieves Lair" short_name = "TLa"
	desc = "a door to a seemingly abandoned house."
	mindepth = 1 maxdepth = 3
	min_player_level = 1
	size_y = 1 size_x = 1
	min_monsters = 0
	alloc_chance = 0
	fill_method = 3
	floors =
	{
		[FEAT_FLOOR] = 100
	}
	walls =
	{
		inner = FEAT_WALL_OUTER,
		outer = FEAT_WALL_INNER,
		[FEAT_WALL_EXTRA] = 100
	}
	flags =
	{
		NO_RECALL=true NO_RECALL_OUT=true SAVE_LEVEL=true NO_EASY_MOVE=true
		NO_GENO=true NO_NEW_MONSTER=true NO_EXEC_FATES=true
		OBJ_THEME = getter.flags { THEME_TREASURE=25 THEME_COMBAT=25 THEME_MAGIC=25 THEME_TOOLS=25 }
	}
	rules =
	{
		[{100, "all"}] = {}
	}
	levels =
	{
		[1] =
		{
			name = "House"
			level_map = "maps/quest_thieves1.map"
			desc = "As you enter the entrance collapses behind you, there is no way back here."
			flags = {
				SPECIAL=true
				LEVEL_GENERATE_POST = function()
					-- Collapse the stair to the second floor when the quets is done
					if quest(QUEST_THIEVES).status == QUEST_STATUS_COMPLETED then
						while quest(QUEST_THIEVES).status == QUEST_STATUS_COMPLETED do
							dialogs.BREE_MAYOR_END_QUEST_THIEVES()
						end
						-- Delete the stair to the second floor
						cave_set_feat(1, 9, FEAT_FLOOR)
						-- Add the door out
						cave_set_feat(4, 1, FEAT_LESS)
					end
				end
			}
		}
		[2] =
		{
			name = "TLa 1"
			level_map = "maps/quest_thieves2.map"
			flags = {
				SPECIAL=true NO_TELEPORT=true
				LEVEL_GENERATE_POST = function()
					message(color.YELLOW, "You feel a vicious blow on your head.")
					timed_effect(timed_effect.STUN, 25, 10)

					local items = {}

					-- Get list of objects...
					for_inventory(player, INVEN_INVEN, INVEN_TOTAL,
						function(obj, i, j, item)
							tinsert(%items, item)
						end)

					-- and drop them (in reverse order, so we don't
					-- shift the item array and change the item indexes)
					for i = getn(items), 1, -1 do
						inven_drop(items[i], 100, 2, 22, true)
					end
				end
			}
		}
		[3] =
		{
			name = "TLa 2"
			level_map = "maps/quest_thieves3.map"
			flags = { SPECIAL=true NO_TELEPORT=true FORGET=true }
			desc = "This level is darker than the previous one, you feel uneasy."
		}
	}
}

new_dungeon_type
{
	define_as = "DUNGEON_FIRES_OF_ISENGARD"
	name = "Orc ambush" short_name = "Amb"
	desc = "BUGGGG!!!!!!!"
	mindepth = 30 maxdepth = 30
	min_player_level = 15
	size_y = 1 size_x = 1
	min_monsters = 0
	alloc_chance = 0
	fill_method = 3
	floors =
	{
		[FEAT_FLOOR] = 100
	}
	walls =
	{
		inner = FEAT_WALL_OUTER,
		outer = FEAT_WALL_INNER,
		[FEAT_WALL_EXTRA] = 100
	}
	flags =
	{
		SURFACE_LITE=true NO_EXEC_FATES=true
		NO_RECALL=true NO_RECALL_OUT=true NO_EASY_MOVE=true SAVE_LEVEL=true
		NO_GENO=true NO_NEW_MONSTER=true
		OBJ_THEME = getter.flags { THEME_TREASURE=20 THEME_COMBAT=50 THEME_MAGIC=10 THEME_TOOLS=10 }
		OBJ_FOUND = "during the Orcish ambush"
	}
	rules =
	{
		[{100, "all"}] = {}
	}
	levels =
	{
		[30] =
		{
			name = "Orc Ambush!"
			level_map = "maps/quest_fires_of_isengard.map"
			desc = "A patrol of orcs ambushes you!"
			flags = {
				SPECIAL=true
			}
		}
	}
}

-- Khazad-dum, West Gate
new_dungeon_type
{
	define_as = "DUNGEON_KZD_WEST"
	name = "West Gate" short_name = "WGt"
	desc = "a path to the West Gate of Khazad-dum"
	mindepth = 30 maxdepth = 30
	min_player_level = 30
	size_y = 3 size_x = 3
	min_monsters = 40
	alloc_chance = 40
	fill_method = 0
	floors =
	{
		[FEAT_DIRT] = { 100, 100 }
		[FEAT_FLOOR] = { 0, 0 }
		[FEAT_FLOOR] = { 0, 0 }
	}
	walls =
	{
		inner = FEAT_MOUNTAIN,
		outer = FEAT_WALL_INNER,
		[FEAT_MOUNTAIN] = { 100, 100 }
		[FEAT_MOUNTAIN] = { 0, 0 }
		[FEAT_MOUNTAIN] = { 0, 0 }
	}
	flags =
	{
		NO_STREAMERS=true
		OBJ_THEME = getter.flags {
			THEME_TREASURE=30
			THEME_COMBAT=50
			THEME_MAGIC=10
			THEME_TOOLS=5
		}
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				--ALLOW_IN_MORIA=true
				ALLOW_IN_EVERYWHERE=true
			}
		}
	}
	levels =
	{
		[30] =
		{
			name = "West Gate"
			level_map = "maps/kzd_west.map"
			desc = "West Gate"
			flags = {
				SAVE_LEVEL=true
				NO_GENO=true NO_NEW_MONSTER=true
				NO_EXEC_FATES=true
				LEVEL_GENERATE_POST = function()
				end
			}
		}
	}
}
-- Khazad-dum, East Gate
new_dungeon_type
{
	define_as = "DUNGEON_KZD_EAST"
	name = "East Gate" short_name = "EGt"
	desc = "a path to the East Gate of Khazad-dum"
	mindepth = 30 maxdepth = 30
	min_player_level = 30
	size_y = 3 size_x = 3
	min_monsters = 40
	alloc_chance = 40
	fill_method = 0
	floors =
	{
		[FEAT_DIRT] = { 100, 100 }
		[FEAT_FLOOR] = { 0, 0 }
		[FEAT_FLOOR] = { 0, 0 }
	}
	walls =
	{
		inner = FEAT_MOUNTAIN,
		outer = FEAT_WALL_INNER,
		[FEAT_MOUNTAIN] = { 100, 100 }
		[FEAT_MOUNTAIN] = { 0, 0 }
		[FEAT_MOUNTAIN] = { 0, 0 }
	}
	flags =
	{
		NO_STREAMERS=true FORCE_DOWN=true
		OBJ_THEME = getter.flags {
			THEME_TREASURE=30
			THEME_COMBAT=50
			THEME_MAGIC=10
			THEME_TOOLS=5
		}
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				--ALLOW_IN_MORIA=true
				ALLOW_IN_EVERYWHERE=true
			}
		}
	}
	levels =
	{
		[30] =
		{
			name = "East Gate"
			level_map = "maps/kzd_east.map"
			desc = "Siege!"
			flags = {
				SAVE_LEVEL=true
				NO_GENO=true NO_NEW_MONSTER=true
				NO_EXEC_FATES=true
				LEVEL_GENERATE_POST = function()
					-- We don't want the player to abuse the nature of SAVE levels
					-- by killing a few orcs, then leaving and returning to the
					-- map. But we also don't want to unconditionally place orcs
					-- every time the player appears, thus risk filling the map.
					-- So we completely wipe and replace every monster  every time
					-- the map loads. This results in the siege monsters being
					-- reset every time the player returns to the map.

					-- First, clear the map of all non-pet monsters:
					for_each_monster(function(m_idx, monst)
						if not has_flag(monst, FLAG_PERMANENT) then
							delete_monster_idx(m_idx)
						end
					end)

					-- Now, restore the siege:
					for i = 19, 22 do
						for j = 6, 7 do
							place_named_monster(i, j, "orcish raider")
						end
					end
					place_named_monster(20, 8, "orcish commander")
					for i = 1, 8 do
						place_named_monster(i * 2 + 10, 10, "orcish crossbowman")
					end
					for i = 1, 4 do
						place_named_monster(i * 2 + 18, 12, "mountain troll")
					end

				end
			}
		}
	}
}
new_dungeon_type
{
	define_as = "DUNGEON_KHAZAD_DUM"
	name = "Khazad-dum" short_name = "Kzd"
	desc = "a stone door leading to the Dwarven city of Khazad-dum"
	mindepth = 30 maxdepth = 30
	min_player_level = 30
	size_y = 3 size_x = 3
	min_monsters = 0
	alloc_chance = 0
	fill_method = 0
	floors =
	{
		[FEAT_FLOOR] = { 100, 100 }
		[FEAT_FLOOR] = { 0, 0 }
		[FEAT_FLOOR] = { 0, 0 }
	}
	walls =
	{
		inner = FEAT_MOUNTAIN,
		outer = FEAT_WALL_INNER,
		[FEAT_MOUNTAIN] = { 100, 100 }
		[FEAT_MOUNTAIN] = { 0, 0 }
		[FEAT_MOUNTAIN] = { 0, 0 }
	}
	flags =
	{
		NO_STREAMERS=true FORCE_DOWN=true
		OBJ_THEME = getter.flags {
			THEME_TREASURE=30
			THEME_COMBAT=50
			THEME_MAGIC=10
			THEME_TOOLS=5
		}
	}
	rules =
	{
		[{100, "or"}] =
		{
			flags = {
				--ALLOW_IN_MORIA=true
				ALLOW_IN_EVERYWHERE=true
			}
		}
	}
	levels =
	{
		[30] =
		{
			name = "Khazad-dum"
			level_map = "maps/khazad.map"
			desc = "Khazad-dum"
			flags = {
				SAVE_LEVEL=true
				NO_GENO=true NO_NEW_MONSTER=true
				NO_EXEC_FATES=true
				LEVEL_GENERATE_POST = function()
				end
			}
		}
	}
}