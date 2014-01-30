-- Handle Yavanna kementari magic school

declare_global_constants {
	"YAVANNA_CHARM_ANIMAL",
	"YAVANNA_GROW_GRASS",
	"YAVANNA_TREE_ROOTS",
	"YAVANNA_UPROOT",
	"YAVANNA_NATURE_WRATH",
}

YAVANNA_CHARM_ANIMAL = add_spell
{
	["name"] =      "Charm Animal",
	["school"] =    {SCHOOL_YAVANNA},
	["level"] =     1,
	["mana"] =      10,
	["mana_max"] =  100,
	["fail"] =      30,
	-- Uses piety to cast
	["piety"] =     true,
	["stat"] =      A_WIS,
	["random"] =    "prayer",
	["spell"] =     function()
			local ret, dir = get_aim_dir()
			if not ret then return SPELL_NOTHING end

			return fire_ball(dam.CONTROL_ANIMAL, dir, 10 + get_cast_level(170), get_cast_level(2))
	end,
	["info"] =      function()
			return "power "..(10 + get_cast_level(170)).." rad "..(get_cast_level(2))
	end,
	["desc"] =      {
			"It tries to tame an animal",
	}
}

YAVANNA_GROW_GRASS = add_spell
{
	["name"] =      "Grow Grass",
	["school"] =    {SCHOOL_YAVANNA},
	["level"] =     19,
	["mana"] =      70,
	["mana_max"] =  150,
	["fail"] =      65,
	-- Uses piety to cast
	["piety"] =     true,
	["stat"] =      A_WIS,
	["random"] =    "prayer",
	["spell"] =     function()
			grow_grass(get_cast_level(4))
			return true
	end,
	["info"] =      function()
			return "rad "..(get_cast_level(4))
	end,
	["desc"] =      {
			"Create a floor of grass around you. While on grass and praying",
			"a worshipper of Yavanna will know a greater regeneration rate"
	}
}

YAVANNA_TREE_ROOTS = add_spell
{
	["name"] =      "Tree Roots",
	["school"] =    {SCHOOL_YAVANNA},
	["level"] =     29,
	["mana"] =      50,
	["mana_max"] =  1000,
	["fail"] =      70,
	-- Uses piety to cast
	["piety"] =     true,
	["stat"] =      A_WIS,
	["random"] =    "prayer",
	["spell"] =     function()
			return set_roots(10 + get_cast_level(30), 10 + get_cast_level(60), 10 + get_cast_level(20))
	end,
	["info"] =      function()
			return "dur "..(10 + get_cast_level(30)).." AC "..(10 + get_cast_level(60)).." dam "..(10 + get_cast_level(20))
	end,
	["desc"] =      {
			"Creates roots deep in the floor from your feet making you more stable and able to",
			"do better attacks but prevents any movement(even teleportation)",
			"It also makes you recover from stunning almost immediately"
	}
}

YAVANNA_NATURE_WRATH = add_spell
{
	["name"] =      "Nature's Wrath",
	["school"] =    {SCHOOL_YAVANNA},
	["level"] =     39,
	["mana"] =      150,
	["mana_max"] =  300,
	["fail"] =      90,
	-- Uses piety to cast
	["piety"] =     true,
	["stat"] =      A_WIS,
	["random"] =    "prayer",
	["spell"] =     function()
			local rad

			rad = 0
			if get_cast_level() >= 50 then rad = 1 end

			return timed_effect(
				timed_effect.MELEE_PROJECT,
				rng(30) + 30 + get_cast_level(150),
				dam.NATURE,
				10 + get_cast_level(50),
				rad,
				PROJECT_STOP | PROJECT_KILL | PROJECT_GRID
			)
	end,
	["info"] =      function()
			return "dur "..(30 + get_cast_level(150)).."+d30 dam "..(10 + get_cast_level(10)).."/blow"
	end,
	["desc"] =      {
			"Imbues your melee weapon with the force of nature itself",
			"At level 50, it spreads over a 1 radius zone around your target"
	}
}

YAVANNA_UPROOT = add_spell
{
	["name"] =      "Uproot",
	["school"] =    {SCHOOL_YAVANNA},
	["level"] =     69,
	["mana"] =      250,
	["mana_max"] =  350,
	["fail"] =      95,
	-- Uses piety to cast
	["piety"] =     true,
	["stat"] =      A_WIS,
	["random"] =    "prayer",
	["spell"] =     function()
			local m_idx, x, y, c_ptr, ret, dir

			ret, dir = get_rep_dir()
			if not ret then return SPELL_NOTHING end
			y, x = explode_dir(dir)
			y, x = y + player.py, x + player.px
			c_ptr = cave(y, x)

			if c_ptr.feat == FEAT_TREES then
				cave_set_feat(y, x, FEAT_GRASS);

				-- Summon it
				y, x = find_position(y, x)
				m_idx = place_monster_one(y, x, test_monster_name("Ent"), 0, FALSE, FACTION_PLAYER)

				-- level it
				if m_idx ~= 0 then
					monster_set_level(m_idx, 30 + get_cast_level(70))
				end

				message("The tree awakes!");
			else
				message("There is no tree there.")
			end
			return true
	end,
	["info"] =      function()
			return ""
	end,
	["desc"] =      {
			"Awakes a tree to help you battle the forces of Morgoth",
	}
}
