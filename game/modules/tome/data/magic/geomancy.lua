-- Geomancy school

declare_global_constants {
	"CALL_THE_ELEMENTS",
	"CHANNEL_ELEMENTS",
	"DRIPPING_TREAD",
	"ELEMENTAL_MINION",
	"ELEMENTAL_WAVE",
	"GEOLYSIS",
	"GROW_BARRIER",
	"VAPORIZE",
	"channel_the_elements",
	"geomancy_dig",
	"geomancy_random_floor",
	"geomancy_random_wall",
}
declare_globals {
	"geomancy_can_tunnel",
}

function geomancy_random_wall(y, x)
	local c_ptr = cave(y, x)

	-- Do not destroy permanent things
	if cave_feat_is(c_ptr, FLAG_PERMANENT) then return end

	local feat = nil
	local table =
	{
		[1] = { SKILL_FIRE, FEAT_SANDWALL, 1},

		[2] = { SKILL_WATER, FEAT_TREES, 1},
		[3] = { SKILL_WATER, FEAT_ICE_WALL, 12},

		[4] = { SKILL_EARTH, FEAT_WALL_EXTRA, 1},
	}

	while feat == nil do
		local t = table[rng(getn(table))]

		-- Do we meet the requirements ?
		-- And then select the features based on skill proportions
		if get_skill_scale(t[1], 50) >= t[3] and rng.percent(get_skill_scale(t[1], 100)) then
			feat = t[2]
		end
	end

	cave_set_feat(y, x, feat)
end


dam.ELEMENTAL_WALL = dam.add
{
	["color"]       = { color.GREEN, 0 },
	["angry"]       = function() return true, FALSE end,
	["grid"]     	= function(state)
			if player.py ~= state.y or player.px ~= state.x then
				geomancy_random_wall(state.y, state.x)
			end
	end,
}

function geomancy_random_floor(y, x, kill_wall)
	local c_ptr = cave(y, x)

	-- Do not destroy permanent things
	if cave_feat_is(c_ptr, FLAG_PERMANENT) then return end
	if not kill_wall then
		if cave_feat_is(c_ptr, FLAG_FLOOR) ~= true then return end
	end

	local feat = nil
	local table =
	{
		[1] = { SKILL_FIRE, FEAT_SAND, 1},
		[2] = { SKILL_FIRE, FEAT_SHAL_LAVA, 8},
		[3] = { SKILL_FIRE, FEAT_DEEP_LAVA, 18},

		[4] = { SKILL_WATER, FEAT_SHAL_WATER, 1},
		[5] = { SKILL_WATER, FEAT_DEEP_WATER, 8},
		[6] = { SKILL_WATER, FEAT_ICE, 18},

		[7] = { SKILL_EARTH, FEAT_GRASS, 1},
		[8] = { SKILL_EARTH, FEAT_FLOWER, 8},
		[9] = { SKILL_EARTH, FEAT_DARK_PIT, 18},
	}

	while feat == nil do
		local t = table[rng(getn(table))]

		-- Do we meet the requirements ?
		-- And then select the features based on skill proportions
		if get_skill_scale(t[1], 50) >= t[3] and rng.percent(get_skill_scale(t[1], 100)) then
			feat = t[2]
		end
	end

	cave_set_feat(y, x, feat)
end


dam.ELEMENTAL_GROWTH = dam.add
{
	["color"]       = { color.GREEN, 0 },
	["angry"]       = function() return true, FALSE end,
	["grid"]     	= function(state)
			geomancy_random_floor(state.y, state.x)
	end,
}

CALL_THE_ELEMENTS = add_spell
{
	["name"] = 	"Call the Elements",
	["school"] = 	{SCHOOL_GEOMANCY},
	["level"] = 	1,
	["mana"] = 	2,
	["mana_max"] = 	20,
	["fail"] = 	10,
	-- Unnafected by blindness
	["blind"] =     FALSE,
	["random"] =    0,
	["spell"] = 	function()
			local ret, dir = 0, 0

			if get_skill(SKILL_AIR) >= 34 then
				ret, dir = get_aim_dir()
				if not ret then return SPELL_NOTHING end
			end

			fire_ball(dam.ELEMENTAL_GROWTH, dir, 1, 1 + get_cast_level(5, 0))
			return true
	end,
	["info"] = 	function()
			return "rad "..(1 + get_cast_level(5, 0))
	end,
	["desc"] =	{
			"Randomly creates various elements around you",
			"Each type of element chance is controlled by your level",
			"in the corresponding skill",
			"At air level 17 it can be targeted",
	}
}

-- Seperate function because an other spell needs it
function channel_the_elements(y, x, level, silent)
	local t =
	{
		-- Earth stuff
		[FEAT_GRASS] = function()
			hp_player(player.mhp() * (5 + get_skill_scale(SKILL_EARTH, 20)) / 100)
		end,
		[FEAT_FLOWER] = function()
			hp_player(player.mhp() * (5 + get_skill_scale(SKILL_EARTH, 30)) / 100)
		end,
		[FEAT_DARK_PIT] = function()
			local ret, dir = get_aim_dir()
			if not ret then return end

			local type = dam.DARK
			if get_skill(SKILL_EARTH) >= 36 then type = dam.NETHER end
			fire_bolt(type, dir, rng.roll(10, get_skill_scale(SKILL_EARTH, 50)))
		end,

		-- Water stuff
		[FEAT_SHAL_WATER] = function()
			local ret, dir = get_aim_dir()
			if not ret then return end

			local type = dam.WATER
			if get_skill(SKILL_WATER) >= 36 then type = dam.WAVE end

			if get_skill(SKILL_WATER) >= 16 then
				fire_beam(type, dir, rng.roll(3, get_skill_scale(SKILL_WATER, 50)))
			else
				fire_bolt(type, dir, rng.roll(3, get_skill_scale(SKILL_WATER, 50)))
			end
		end,
		[FEAT_DEEP_WATER] = function()
			local ret, dir = get_aim_dir()
			if not ret then return end

			local type = dam.WATER
			if get_skill(SKILL_WATER) >= 36 then type = dam.WAVE end

			if get_skill(SKILL_WATER) >= 16 then
				fire_beam(type, dir, rng.roll(5, get_skill_scale(SKILL_WATER, 50)))
			else
				fire_bolt(type, dir, rng.roll(5, get_skill_scale(SKILL_WATER, 50)))
			end
		end,
		[FEAT_ICE] = function()
			local ret, dir = get_aim_dir()
			if not ret then return end

			if get_skill(SKILL_WATER) >= 24 then
				fire_ball(dam.ICE, dir, get_skill_scale(SKILL_WATER, 340), 3)
			else
				fire_bolt(dam.ICE, dir, rng.roll(3, get_skill_scale(SKILL_WATER, 50)))
			end
		end,

		-- Fire stuff
		[FEAT_SAND] = function()
			local type
			if get_skill(SKILL_FIRE) >= 16 then
				type = AURA_DARK_FIRE
			else
				type = AURA_FIRE
			end
			local dur = rng(20) + %level + get_skill_scale(SKILL_AIR, 50)
			timed_effect(timed_effect.AURA, dur, type, 5 + get_skill_scale(SKILL_FIRE, 20), 5 + get_skill_scale(SKILL_FIRE, 14))
			timed_effect(timed_effect.BLIND, dur)
		end,
		[FEAT_SHAL_LAVA] = function()
			local ret, dir = get_aim_dir()
			if not ret then return end

			if get_skill(SKILL_FIRE) >= 30 then
				fire_bolt(dam.HELL_FIRE, dir, rng.roll(get_skill_scale(SKILL_FIRE, 30), 15))
			else
				fire_bolt(dam.FIRE, dir, rng.roll(get_skill_scale(SKILL_FIRE, 30), 15))
			end
		end,
		[FEAT_DEEP_LAVA] = function()
			local ret, dir = get_aim_dir()
			if not ret then return end

			if get_skill(SKILL_FIRE) >= 30 then
				fire_ball(dam.HELL_FIRE, dir, rng.roll(get_skill_scale(SKILL_FIRE, 30), 15), 3)
			else
				fire_ball(dam.FIRE, dir, rng.roll(get_skill_scale(SKILL_FIRE, 30), 15), 3)
			end
		end,
	}

	if t[cave(y, x).feat] then
		t[cave(y, x).feat]()

		if rng.percent(100 - level) then
			if cave(y, x).feat == FEAT_FLOWER then
				cave_set_feat(y, x, FEAT_GRASS)
			else
				cave_set_feat(y, x, FEAT_FLOOR)
			end
			message("The area is drained.")
		end
	elseif not silent then
		message("You cannot channel this area.")
	end
end

CHANNEL_ELEMENTS = add_spell
{
	["name"] = 	"Channel Elements",
	["school"] = 	{SCHOOL_GEOMANCY},
	["level"] = 	5,
	["mana"] = 	3,
	["mana_max"] = 	30,
	["fail"] = 	20,
	-- Unnafected by blindness
	["blind"] =     FALSE,
	["random"] =    0,
	["spell"] = 	function()
			channel_the_elements(player.py, player.px, get_cast_level(50), nil)
			return true
	end,
	["info"] = 	function()
			return ""
	end,
	["desc"] =	{
			"Draws on the caster's immediate environs to form an attack or other effect.",
			"Grass/Flower heals",
			"Water creates water attacks",
			"Ice creates ice attacks",
			"Sand creates a wall of burning sand around you, but the wall is thick and blinds you",
			"Lava creates fire attacks",
			"Deep lava creates a ball attack",
			"Abyss creates darkness ray attacks",
			"At Earth level 8, the healing may be projected to heal a companion or pet.",
			"At Earth level 18, darkness becomes nether.",
			"At Water level 8, water attacks become beams with a striking effect.",
			"At Water level 12, ice attacks become balls of ice shards",
			"At Fire level 20, fire become darkfire.",
	}
}

ELEMENTAL_WAVE = add_spell
{
	["name"] = 	"Elemental Wave",
	["school"] = 	{SCHOOL_GEOMANCY},
	["level"] = 	29,
	["mana"] = 	15,
	["mana_max"] = 	50,
	["fail"] = 	20,
	-- Unnafected by blindness
	["blind"] =     FALSE,
	["random"] =    0,
	["spell"] = 	function()
			local ret, dir = get_rep_dir()
			if not ret then return SPELL_NOTHING end

			local y, x = explode_dir(dir)
			y = y + player.py
			x = x + player.px

			local t =
			{
				-- Earth stuff
				[FEAT_GRASS] 		= { dam.POIS, dam.POIS, 10 + get_skill_scale(SKILL_EARTH, 200) },
				[FEAT_FLOWER] 		= { dam.POIS, dam.POIS, 10 + get_skill_scale(SKILL_EARTH, 300) },
				-- cannot turn chasm into a wave

				-- Water stuff
				[FEAT_SHAL_WATER] 	= { dam.WATER, dam.WATER, 10 + get_skill_scale(SKILL_WATER, 200) },
				[FEAT_DEEP_WATER] 	= { dam.WATER, dam.WATER, 10 + get_skill_scale(SKILL_WATER, 300) },
				[FEAT_ICE] 		= { dam.ICE, dam.ICE, 10 + get_skill_scale(SKILL_WATER, 200) },

				-- Fire stuff
				[FEAT_SAND] 		= { dam.LITE, dam.LITE, 10 + get_skill_scale(SKILL_FIRE, 400) },
				[FEAT_SHAL_LAVA] 	= { dam.FIRE, dam.VALINOREAN_FIRE, 10 + get_skill_scale(SKILL_FIRE, 200) },
				[FEAT_DEEP_LAVA] 	= { dam.FIRE, dam.VALINOREAN_FIRE, 10 + get_skill_scale(SKILL_FIRE, 300) },
			}


			local effect = t[cave(y, x).feat]
			if not effect then
				message("You cannot channel this area.")
			else
				local typ = effect[1]
				if get_cast_level() >= 40 then typ = effect[2] end

				cave_set_feat(y, x, FEAT_FLOOR)

				fire_wave(typ, 0, effect[3], 0, 6 + get_cast_level(20), player.speed(), EFF_WAVE + EFF_LAST + getglobal("EFF_DIR"..dir))
			end

			return true
	end,
	["info"] = 	function()
			return ""
	end,
	["desc"] =	{
			"Draws on an adjacent special square to project a slow-moving",
			"wave of that element in that direction",
			"Abyss squares cannot be channeled into a wave.",
	}
}

VAPORIZE = add_spell
{
	["name"] = 	"Vaporize",
	["school"] = 	{SCHOOL_GEOMANCY},
	["level"] = 	7,
	["mana"] = 	3,
	["mana_max"] = 	30,
	["fail"] = 	15,
	-- Unnafected by blindness
	["blind"] =     FALSE,
	-- Must have at least 4 Air
	["random"] =    0,
	["depend"] =    function()
			if get_skill(SKILL_AIR) >= 8 then return true end
	end,
	["spell"] = 	function()
			local t =
			{
				-- Earth stuff
				[FEAT_GRASS] 		= { dam.POIS, dam.POIS, 5 + get_skill_scale(SKILL_EARTH, 100) },
				[FEAT_FLOWER] 		= { dam.POIS, dam.POIS, 5 + get_skill_scale(SKILL_EARTH, 150) },
				[FEAT_DARK_PIT] 	= { dam.DARK, dam.DARK, 5 + get_skill_scale(SKILL_EARTH, 200) },

				-- Water stuff
				[FEAT_SHAL_WATER] 	= { dam.WATER, dam.WATER, 5 + get_skill_scale(SKILL_WATER, 100) },
				[FEAT_DEEP_WATER] 	= { dam.WATER, dam.WATER, 5 + get_skill_scale(SKILL_WATER, 150) },
				[FEAT_ICE] 		= { dam.ICE, dam.ICE, 5 + get_skill_scale(SKILL_WATER, 100) },

				-- Fire stuff
				[FEAT_SAND] 		= { dam.LITE, dam.LITE, 5 + get_skill_scale(SKILL_FIRE, 200) },
				[FEAT_SHAL_LAVA] 	= { dam.FIRE, dam.VALINOREAN_FIRE, 5 + get_skill_scale(SKILL_FIRE, 100) },
				[FEAT_DEEP_LAVA] 	= { dam.FIRE, dam.VALINOREAN_FIRE, 5 + get_skill_scale(SKILL_FIRE, 150) },
			}

			local effect = t[cave(player.py, player.px).feat]
			if not effect then
				message("You cannot channel this area.")
			else
				local typ = effect[1]
				if get_cast_level() >= 40 then typ = effect[2] end

				cave_set_feat(player.py, player.px, FEAT_FLOOR)

				fire_cloud(typ, 0, effect[3], 1 + get_skill_scale(SKILL_AIR, 5), 10 + get_cast_level(20), player.speed())
			end

       			return true
	end,
	["info"] = 	function()
			return "rad "..(1 + get_cast_level(4)).." dur "..(10 + get_cast_level(20))
	end,
	["desc"] =	{
			"Draws upon your immediate environs to form a cloud of damaging vapors",
	}
}

function geomancy_can_tunnel()
	return
	{
		[FEAT_WALL_EXTRA] = true,
		[FEAT_WALL_OUTER] = true,
		[FEAT_WALL_INNER] = true,
		[FEAT_WALL_SOLID] = true,

		[FEAT_MAGMA] = true,
		[FEAT_QUARTZ] = true,
		[FEAT_MAGMA_H] = true,
		[FEAT_QUARTZ_H] = true,
		[FEAT_MAGMA_K] = true,
		[FEAT_QUARTZ_K] = true,

		[FEAT_TREES] = true,
		[FEAT_DEAD_TREE] = true,

		[FEAT_SANDWALL] = true,
		[FEAT_SANDWALL_H] = true,
		[FEAT_SANDWALL_K] = true,

		[FEAT_ICE_WALL] = true,
	}
end

-- Dig & sprew
function geomancy_dig(oy, ox, dir, length)
	local dy, dx = explode_dir(dir)
	local y = dy + oy
	local x = dx + ox
	local geomancy_can_tunnel = geomancy_can_tunnel()

	for i = 1, length do
		local c_ptr = cave(y, x)
		local ox = x - dx
		local oy = y - dy

		-- stop at the end of tunnelable things
		if not geomancy_can_tunnel[c_ptr.feat] then break end

		if geomancy_can_tunnel[cave(y - 1, x - 1).feat] then geomancy_random_wall(y - 1, x - 1) end
		if geomancy_can_tunnel[cave(y - 1, x).feat] then geomancy_random_wall(y - 1, x) end
		if geomancy_can_tunnel[cave(y - 1, x + 1).feat] then geomancy_random_wall(y - 1, x + 1) end

		if geomancy_can_tunnel[cave(y, x - 1).feat] then geomancy_random_wall(y, x - 1) end
		if geomancy_can_tunnel[cave(y, x + 1).feat] then geomancy_random_wall(y, x + 1) end

		if geomancy_can_tunnel[cave(y + 1, x - 1).feat] then geomancy_random_wall(y + 1, x - 1) end
		if geomancy_can_tunnel[cave(y + 1, x).feat] then geomancy_random_wall(y + 1, x) end
		if geomancy_can_tunnel[cave(y + 1, x + 1).feat] then geomancy_random_wall(y + 1, x + 1) end

		y = y + dy
		x = x + dx
	end

	y = y - dy
	x = x - dx
	while (y ~= oy) or (x ~= ox) do
		geomancy_random_floor(y, x, true)

		-- Should we branch ?
		if rng.percent(20) then
			local rot = 1
			if rng.percent(50) then rot = -1 end
			geomancy_dig(y, x, rotate_dir(dir, rot), length / 3)
		end

		y = y - dy
		x = x - dx
	end
end

GEOLYSIS = add_spell
{
	["name"] = 	"Geolysis",
	["school"] = 	{SCHOOL_GEOMANCY},
	["level"] = 	13,
	["mana"] = 	15,
	["mana_max"] = 	40,
	["fail"] = 	15,
	-- Unnafected by blindness
	["blind"] =     FALSE,
	["random"] =    0,
	-- Must have at least 7 Earth
	["depend"] =    function()
			if get_skill(SKILL_EARTH) >= 14 then return true end
	end,
	["spell"] = 	function()
			local ret, dir = get_rep_dir()
			if not ret then return SPELL_NOTHING end

			message("Elements recombine before you, laying down an open path.")
			geomancy_dig(player.py, player.px, dir, 5 + get_cast_level(12))

       			return true
	end,
	["info"] = 	function()
			return "length "..(5 + get_cast_level(12))
	end,
	["desc"] =	{
			"Burrows deeply and slightly at random into a wall,",
			"leaving behind tailings of various different sorts of walls in the passage.",
	}
}

player.dripping_tread = 0
add_loadsave("player.dripping_tread", 0)
hook(hook.BIRTH, function() player.dripping_tread = 0 end)
hook(hook.MOVE_POST,
	function(oy, ox)
		if player.dripping_tread > 0 then
			geomancy_random_floor(oy, ox)
			player.dripping_tread = player.dripping_tread - 1
			if player.dripping_tread == 0 then
				message("You stop dripping raw elemental energies.")
			end
		end
	end
)

DRIPPING_TREAD = add_spell
{
	["name"] = 	"Dripping Tread",
	["school"] = 	{SCHOOL_GEOMANCY},
	["level"] = 	19,
	["mana"] = 	15,
	["mana_max"] = 	25,
	["fail"] = 	15,
	-- Unnafected by blindness
	["blind"] =     FALSE,
	["random"] =    0,
	-- Must have at least 10 Water
	["depend"] =    function()
			if get_skill(SKILL_WATER) >= 20 then return true end
	end,
	["spell"] =     function()
			if player.dripping_tread == 0 then
				player.dripping_tread = rng(15) + 10 + get_cast_level(50)
				message("You start dripping raw elemental energies.")
			else
				player.dripping_tread = 0
				message("You stop dripping raw elemental energies.")
			end
			return true
	end,
	["info"] = 	function()
			return "dur "..(10 + get_cast_level(50)).."+d15 movs"
	end,
	["desc"] =	{
			"Causes you to leave random elemental forms behind as you walk",
	}
}

GROW_BARRIER = add_spell
{
	["name"] = 	"Grow Barrier",
	["school"] = 	{SCHOOL_GEOMANCY},
	["level"] = 	23,
	["mana"] = 	30,
	["mana_max"] = 	40,
	["fail"] = 	15,
	-- Unnafected by blindness
	["blind"] =     FALSE,
	["random"] =    0,
	-- Must have at least 12 Earth
	["depend"] =    function()
			if get_skill(SKILL_EARTH) >= 24 then return true end
	end,
	["spell"] = 	function()
			local ret, dir = 0, 0

			if get_skill(SKILL_AIR) >= 40 then
				ret, dir = get_aim_dir()
				if not ret then return SPELL_NOTHING end
			end

			fire_ball(dam.ELEMENTAL_WALL, dir, 1, 1)
       			return true
	end,
	["info"] = 	function()
			return ""
	end,
	["desc"] =	{
			"Creates impassable terrain (walls, trees, etc.) around you.",
			"At air level 20 it can be projected around another area.",
	}
}

ELEMENTAL_MINION = add_spell
{
	["name"] = 	"Elemental Minion",
	["school"] = 	{SCHOOL_GEOMANCY},
	["level"] = 	39,
	["mana"] = 	40,
	["mana_max"] = 	80,
	["fail"] = 	25,
	-- Unnafected by blindness
	["random"] =    0,
	-- Must have at least 12 Earth
	["spell"] = 	function()
			local ret, dir = 0, 0

			ret, dir = get_rep_dir()
			if not ret then return SPELL_NOTHING end

			local t =
			{
				[FEAT_WALL_EXTRA] = { SKILL_EARTH, { "Earth elemental", "Xorn", "Xaren" } },
				[FEAT_WALL_OUTER] = { SKILL_EARTH, { "Earth elemental", "Xorn", "Xaren" } },
				[FEAT_WALL_INNER] = { SKILL_EARTH, { "Earth elemental", "Xorn", "Xaren" } },
				[FEAT_WALL_SOLID] = { SKILL_EARTH, { "Earth elemental", "Xorn", "Xaren" } },
				[FEAT_MAGMA] = { SKILL_EARTH, { "Earth elemental", "Xorn", "Xaren" } },
				[FEAT_QUARTZ] = { SKILL_EARTH, { "Earth elemental", "Xorn", "Xaren" } },
				[FEAT_MAGMA_H] = { SKILL_EARTH, { "Earth elemental", "Xorn", "Xaren" } },
				[FEAT_QUARTZ_H] = { SKILL_EARTH, { "Earth elemental", "Xorn", "Xaren" } },
				[FEAT_MAGMA_K] = { SKILL_EARTH, { "Earth elemental", "Xorn", "Xaren" } },
				[FEAT_QUARTZ_K] = { SKILL_EARTH, { "Earth elemental", "Xorn", "Xaren" } },

				[FEAT_DARK_PIT] = { SKILL_AIR, { "Air elemental", "Ancient blue dragon", "Great Storm Wyrm", "Sky Drake" } },

				[FEAT_SANDWALL] = { SKILL_FIRE, { "Fire elemental", "Ancient red dragon" } },
				[FEAT_SANDWALL_H] = { SKILL_FIRE, { "Fire elemental", "Ancient red dragon" } },
				[FEAT_SANDWALL_K] = { SKILL_FIRE, { "Fire elemental", "Ancient red dragon" } },
				[FEAT_SHAL_LAVA] = { SKILL_FIRE, { "Fire elemental", "Ancient red dragon" } },
				[FEAT_DEEP_LAVA] = { SKILL_FIRE, { "Fire elemental", "Ancient red dragon" } },

				[FEAT_ICE_WALL] = { SKILL_WATER, { "Water elemental", "Water troll", "Water demon" } },
				[FEAT_SHAL_WATER] = { SKILL_WATER, { "Water elemental", "Water troll", "Water demon" } },
				[FEAT_DEEP_WATER] = { SKILL_WATER, { "Water elemental", "Water troll", "Water demon" } },
			}

			local y, x = explode_dir(dir)
			y = y + player.py
			x = x + player.px

			local effect = t[cave(y, x).feat]
			if not effect then
				message("You cannot summon from this area.")
			else
				local skill = effect[1]
				local types = effect[2]

				local max = get_skill_scale(skill, getn(types))
				if max == 0 then max = 1 end

				local r_idx = test_monster_name(types[rng(1, max)])

				-- Summon it
				local my, mx = find_position(y, x)
				local m_idx = place_monster_one(my, mx, r_idx, 0, FALSE, FACTION_PLAYER)

				-- level it
				if m_idx ~= 0 then
					monster_set_level(m_idx, 10 + get_cast_level(120))
				end

				cave_set_feat(y, x, FEAT_FLOOR)
			end

       			return true
	end,
	["info"] = 	function()
			return "min level "..(10 + get_cast_level(120))
	end,
	["desc"] =	{
			"Summons a minion from a nearby element.",
			"Walls can summon Earth elmentals, Xorns and Xarens",
			"Dark Pits can summon Air elementals, Ancient blue dragons, Great Storm Wyrms",
			"and Sky Drakes",
			"Sandwalls and lava can summon Fire elementals and Ancient red dragons",
			"Icewall, and water can summon Water elementals, Water trolls and Water demons",
	}
}

-- Now we've added the spells, the user needs a user interface to cast them
-- First we add an MKEY for them (note that MKEYS have to be enabled in
-- s_info.txt or somewhere similar to show up for the user)
add_mkey
{
	mkey 	= "Geomancy",
	type 	= "skill",
	fct 	= function()
		local book = make_book
		(
			CALL_THE_ELEMENTS,
			CHANNEL_ELEMENTS,
			ELEMENTAL_WAVE,
			VAPORIZE,
			GEOLYSIS,
			DRIPPING_TREAD,
			GROW_BARRIER,
			ELEMENTAL_MINION
		)

		local obj = player.inventory[INVEN_MAINHAND][1]
		if not obj or obj.k_idx <= 0 or obj.tval ~= TV_MSTAFF then
			message('You must wield a magestaff to use Geomancy.')
			return
		end

		local spl = nil
		local ret, rep = repeat_keys.pull()
		if ret then
			if is_ok_spell(rep, book) then
				spl = rep
			end
		end

		spl = spl or get_school_spell("cast", "is_ok_spell", book)
		if spl ~= -1 then
			cast_school_spell(spl, spell(spl))
		end

		delete_object(book)
	end,
}
