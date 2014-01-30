-- handle the water school

declare_global_constants {
	"ENTDRAUGHT",
	"GEYSER",
	"ICESTORM",
	"TIDALWAVE",
	"VAPOR",
	"get_geyser_damage",
}

TIDALWAVE = add_spell
{
	name = 	"Tidal Wave",
	school = 	{SCHOOL_WATER},
	level = 	31,
	mana = 	16,
	mana_max = 	40,
	fail = 	65,
	stick =
	{
			charge =    { 6, 5 },
			[TV_ORB] =
			{
				rarity = 		54,
				base_level =	{ 1, 10 },
				max_level =		{ 20, 50 },
			},
	},
	inertia = 	{ 4, 100 },
	tactical =
	{
		[ai.tactics.ATTACK] = 7
		[ai.tactics.ANNOY] = 7
	}
	spell = 	function()
			fire_wave(dam.WATER, 0, 40 + get_cast_level(200), 0, 6 + get_cast_level(10), player.speed(), EFF_WAVE)
			return true
	end,
	info = 	function()
			return "dam "..(40 + get_cast_level(200)).." rad "..(6 + get_cast_level(10))
	end,
	desc =	{
			"Summons a monstrous tidal wave that will expand and crush the",
			"monsters under it's mighty waves"
	}
}

ICESTORM = add_spell
{
	name = 	"Ice Storm",
	school = 	{SCHOOL_WATER},
	level = 	43,
	mana = 	30,
	mana_max = 	60,
	fail = 	80,
	stick =
	{
			charge =    { 3, 7 },
			[TV_ORB] =
			{
				rarity = 		65,
				base_level =	{ 1, 5 },
				max_level =		{ 25, 45 },
			},
	},
	inertia = 	{ 3, 40 },
	tactical =
	{
		[ai.tactics.ATTACK] = 7
		[ai.tactics.ANNOY] = 7
	}
	spell = 	function()
			local dam_type

			if get_cast_level(50) >= 10 then dam_type = dam.ICE
			else dam_type = dam.COLD end
			fire_wave(dam_type, 0, 80 + get_cast_level(200),
					  1 + get_cast_level(3, 0), 20 + get_cast_level(70),
					  player.speed(), EFF_STORM)
			return true
	end,
	info = 	function()
			return "dam "..(80 + get_cast_level(200)).." rad "..(1 + get_cast_level(3, 0)).." dur "..(20 + get_cast_level(70))
	end,
	desc =	{
			"Engulfs you in a storm of roaring cold that strikes your foes",
			"At level 20 it turns into shards of ice"
	}
}

ENTDRAUGHT = add_spell
{
	name = 	"Ent-draught",
	school = 	{SCHOOL_WATER},
	level = 	11,
	mana = 	7,
	mana_max = 	15,
	fail = 	35,
	inertia = 	{ 1, 30 },
	spell = 	function()
			timed_effect.set(timed_effect.FOOD, food.FOOD_MAX - 1)
			--set_food(PY_FOOD_MAX - 1)
			message("The Ent-draught fills your stomach.")
			if get_cast_level(50) >= 5 then
				timed_effect.on_lose(timed_effect.AFRAID)
			end
			if get_cast_level(50) >= 12 then
				timed_effect.inc(timed_effect.HEROISM,
								 rng(25) + 25 + get_cast_level(40))
			end
			if rng.number(1000) == 1 and player.ht < 300 then
				player.ht = player.ht + 1
			end
			return true
	end,
	info = 	function()
			if get_cast_level(50) >= 12 then
				return "dur "..(25 + get_cast_level(40)).."+d25"
			else
				return ""
			end
	end,
	desc =	{
			"Fills up your stomach",
			"At level 10 it boldens your heart",
			"At level 24 it make you heroic"
	}
}

VAPOR = add_spell
{
	name = 	"Vapor",
	school = 	{SCHOOL_WATER},
	level = 	3,
	mana = 	2,
	mana_max = 	12,
	fail = 	20,
	inertia = 	{ 1, 30 },
	tactical =
	{
		[ai.tactics.ATTACK] = 5
	}
	spell = 	function()
			fire_cloud(dam.WATER, 0, 3 + get_cast_level(20), 3 + get_cast_level(9, 0), 5, player.speed())
			return true
	end,
	info = 	function()
       			return "dam "..(3 + get_cast_level(20)).." rad "..(3 + get_cast_level(9, 0)).." dur 5"
	end,
	desc =	{
			"Fills the air with toxic moisture to eradicate annoying critters"
	}
}

function get_geyser_damage()
	return get_level(GEYSER, 10), 3 + get_level(GEYSER, 35)
end

GEYSER = add_spell
{
	name = "Geyser",
	school = SCHOOL_WATER,
	level = 1,
	mana = 1,
	mana_max = 35,
	fail = 5,
	tactical =
	{
		[ai.tactics.ATTACK] = 5
	}
	spell = function()
		local ret, dir
		ret, dir = get_aim_dir{max_range = get_cast_range()}
		if not ret then return SPELL_NOTHING end
		return fire_bolt_or_beam(2 * get_cast_level(85), dam.WATER, dir, rng.roll(get_geyser_damage()), get_cast_range())
	end,
	range = function()
		return 10
	end
	info = function()
		return "dam "..rng.desc_roll(get_geyser_damage())
	end,
	desc =
	{
		"Shoots a geyser of water from your fingertips.",
		"Sometimes it can blast through its first target."
	},
}
