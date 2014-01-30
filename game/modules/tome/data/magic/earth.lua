 -- The earth school

declare_global_constants {
	"DIG",
	"SHAKE",
	"STONEPRISON",
	"STONESKIN",
	"STRIKE",
	"wall_stone",
}

STONESKIN = add_spell
{
	name = 		"Stone Skin",
	school = 	SCHOOL_EARTH,
	level = 	1,
	mana = 		1,
	mana_max = 	50,
	fail = 		10,
	inertia = 	{ 2, 50 },
	tactical =
	{
		[ai.tactics.ENHANCEMENT] = 10
	}
	spell = 	function()
			local dur = rng(10) + 10 + get_cast_level(100)
			if get_cast_level(50) >= 25 then
				timed_effect(timed_effect.AURA, dur, AURA_SHARDS, 2 + get_cast_level(5), 3 + get_cast_level(5))
			end
			timed_effect(timed_effect.ARMOR, dur, 10 + get_cast_level(50))
			timed_effect.set_silent(timed_effect.ARMOR_DA, dur, 1 + get_cast_level(5))
			timed_effect.set_silent(timed_effect.ARMOR_DR, dur, 2 + get_cast_level(15))
			return true
	end,
	info = 	function()
			if get_cast_level(50) >= 25 then
				return "dam "..rng.desc_roll(2 + get_cast_level(5),3 + get_cast_level(5)).." dur "..(10 + get_cast_level(100)).."+d10 AC "..(10 + get_cast_level(50)).." DR "..(2 + get_cast_level(15)).."% DA "..(1 + get_cast_level(5))
			else
				return "dur "..(10 + get_cast_level(100)).."+d10 AC "..(10 + get_cast_level(50)).." DR "..(2 + get_cast_level(15)).."% DA "..(1 + get_cast_level(5))
			end
	end,
	desc =	{
			"Creates a shield of earth around you to protect you",
			"At level 50 it starts dealing damage to attackers"
		}
}

DIG = add_spell
{
	name = 		"Dig",
	school = 	SCHOOL_EARTH,
	level = 	23,
	mana = 		14,
	mana_max = 	14,
	fail = 		20,
	stick =
	{
			charge =    { 15, 5 },
			[TV_ORB] =
			{
				rarity = 		25,
				base_level =	{ 1, 1 },
				max_level =		{ 1, 1 },
			},
	},
	spell = function()
			local ret, dir
			ret, dir = get_aim_dir()
			if not ret then return SPELL_NOTHING end

			local proj_flags = (PROJECT_STOP| PROJECT_GRID | PROJECT_ITEM |
								PROJECT_KILL)

			local tx, ty
			if (dir == 5) then
				tx = target_col
				ty = target_row

			else
				-- Use the given direction
				local dy, dx = explode_dir(dir)
				ty = player.py + (dy * 99)
				tx = player.px + (dx * 99)

				-- Mega-Hack -- Both beam and bolt spells should
				-- continue through this fake target. --dsb

				proj_flags = proj_flags | PROJECT_THRU
			end

			return project(0, 0, ty, tx, 1, dam.KILL_WALL, proj_flags)

	end,
	info = 	function()
			return ""
	end,
	desc =	{
			"Digs a hole in a wall much faster than any shovels",
		}
}

STONEPRISON = add_spell
{
	name = 		"Stone Prison",
	school = 	SCHOOL_EARTH,
	level = 	49,
	mana = 		30,
	mana_max = 	50,
	fail = 		65,
	stick =
	{
			charge =    { 5, 3 },
			[TV_ORB] =
			{
				rarity = 		57,
				base_level =	{ 1, 3 },
				max_level =		{ 5, 20 },
			},
	},
	tactical =
	{
		[ai.tactics.ANNOY] = 7
		[ai.tactics.TACTIC] = 7
	}
	spell = 	function()
			local ret, x, y
			if get_cast_level(50) >= 10 then
				ret, x, y = tgt_pt()
			else
				y, x = get_spell_caster_info()
			end
			wall_stone(y, x)
			return true
	end,
	info = 	function()
			return ""
	end,
	desc =	{
			"Creates a prison of walls around you",
			"At level 20 it allows you to target a monster"
		}
}

function wall_stone(y, x)
	for yy = y - 1, y + 1 do
		for xx = x - 1, x + 1 do
			if in_bounds(yy, xx) then
				local c_ptr = cave(yy, xx)
				local feat = f_info[c_ptr.feat + 1]

				if not (player.py == yy and player.px == xx) and
					not (y == yy and x == xx) and
					c_ptr.m_idx == 0 and not feat.flags[FLAG_WALL] and
					not feat.flags[FLAG_PERMANENT] then
					cave_set_feat(yy, xx, FEAT_WALL_SOLID)
				end
			end
		end
	end
end

STRIKE = add_spell
{
	name = 		"Strike",
	school = 	{SCHOOL_EARTH},
	level = 	59,
	mana = 		30,
	mana_max = 	50,
	fail = 		60,
	stick =
	{
			charge =    { 2, 6 },
			[TV_ORB] =
			{
				rarity = 		635,
				base_level =	{ 1, 5 },
				max_level =		{ 10, 50 },
			},
	},
	tactical =
	{
		[ai.tactics.ATTACK] = 4
		[ai.tactics.ANNOY] = 7
		[ai.tactics.TACTIC] = 5
	}
	spell = 	function()
			local ret, dir, rad
			ret, dir = get_aim_dir()
			if not ret then return SPELL_NOTHING end
			if get_cast_level(50) >= 12 then
				return fire_ball(dam.FORCE, dir, 50 + get_cast_level(50), 1)
			else
				return fire_ball(dam.FORCE, dir, 50 + get_cast_level(50), 0)
			end
	end,
	info = 	function()
			if get_cast_level(50) >= 12 then
	       			return "dam "..(50 + get_cast_level(50)).." rad 1"
			else
				return "dam "..(50 + get_cast_level(50))
			end
	end,
	desc =	{
			"Creates a micro-ball of force that will push monsters backwards",
			"If the monster is caught near a wall, it'll be crushed against it",
			"At level 24 it turns into a ball of radius 1"
	}
}

SHAKE = add_spell
{
	name = 		"Shake",
	school = 	{SCHOOL_EARTH},
	level = 	53,
	mana = 		25,
	mana_max = 	30,
	fail = 		60,
	stick =
	{
			charge =    { 5, 10 },
			[TV_ORB] =
			{
				rarity = 		75,
				base_level =	{ 1, 3 },
				max_level =		{ 9, 20 },
			},
	},
	inertia = 	{ 2, 50 },
	tactical =
	{
		[ai.tactics.TACTIC] = 5
		[ai.tactics.ANNOY] = 5
	}
	spell = 	function()
			local ret, x, y
			if get_cast_level(50) >= 10 then
			       	ret, x, y = tgt_pt()
				if not ret then return SPELL_NOTHING end
			else
				y, x = get_spell_caster_info()
			end
			earthquake(y, x, 4 + get_cast_level(10));
			return true
	end,
	info = 	function()
			return "rad "..(4 + get_cast_level(10))
	end,
	desc =	{
			"Creates a localised earthquake",
			"At level 20 it can be targeted at any location"
	}
}
