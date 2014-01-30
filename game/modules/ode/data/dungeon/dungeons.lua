-- Need a Wilderness entry to be first
new_dungeon_type
{
	__index__ = 0
	name = "Wilderness" short_name = "Wdn"
	desc = "oups BUG!"
	mindepth = 0 maxdepth = 0
	min_player_level = 1
	size_y = 3 size_x = 3
	min_monsters = 140
	alloc_chance = 500
	flags = { FLAT=true NO_RECALL=true }
	floors =
	{
		[FEAT_FLOOR] = 100
	}
	walls =
	{
		inner = FEAT_PERM_SOLID,
		outer = FEAT_PERM_SOLID,
		[FEAT_PERM_SOLID] = 100
	}
}

new_dungeon_type
{
	define_as = "DUNGEON_ODE"
	name = "Dungeon of Ode" short_name = "Ode"
	desc = "an stairway into the dungeon of ode"
	mindepth = 1 maxdepth = 8
	min_player_level = 0
	size_y = 1 size_x = 1
	min_monsters = 14
	alloc_chance = 160
	fill_method = 0
	generator = "dungeon_doom"
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
		NO_RECALL=true SMALLEST=true NO_NEW_MONSTER=true
		OBJ_THEME = getter.flags{THEME_ALL=100}
		DUNGEON_GUARDIAN = MONSTER_DRAGON
	}
	rules =
	{
		[{100, "all"}] = {}
	}
}
