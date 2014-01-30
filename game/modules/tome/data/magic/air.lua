-- handle the air school

declare_global_constants {
	"AIRWINGS",
	"INVISIBILITY",
	"NOXIOUSCLOUD",
	"POISONBLOOD",
	"STERILIZE",
	"THUNDERSTORM",
}

NOXIOUSCLOUD = add_spell
{
	name = 		"Noxious Cloud",
	school = 	{SCHOOL_AIR},
	level = 	5,
	mana = 		3,
	mana_max = 	30,
	fail = 		20,
	stick =
	{
			charge =    { 5, 7 },
			[TV_ORB] =
			{
				rarity = 		15,
				base_level =	{ 1, 15 },
				max_level =		{ 25, 50 },
			},
	},
	tactical =
	{
		[ai.tactics.ATTACK] = 9
	}
	spell = 	function()
			local ret, dir, type

			ret, dir = get_aim_dir()
			if not ret then return SPELL_NOTHING end
			if get_cast_level() >= 60 then type = dam.THICK_POISON
			else type = dam.POIS end
			fire_cloud(type, dir, 7 + get_cast_level(150), 3, 5 + get_cast_level(40), player.speed())
			return true
	end,
	info = 	function()
			return "dam "..(7 + get_cast_level(150)).." rad 3 dur "..(5 + get_cast_level(40))
	end,
	desc =	{
			"Creates a cloud of poison",
			"The cloud will persist for some turns, damaging all monsters passing by",
			"At level 60 it turns into a thick gas attacking all living beings"
	}
}

AIRWINGS = add_spell
{
	name = 		"Wings of Winds",
	school = 	{SCHOOL_AIR, SCHOOL_CONVEYANCE},
	level = 	43,
	mana = 		30,
	mana_max = 	40,
	fail = 		60,
	stick =
	{
			charge =    { 7, 5 },
			[TV_ORB] =
			{
				rarity = 		27,
				base_level =	{ 1, 10 },
				max_level =		{ 20, 50 },
			},
	},
	inertia = 	{ 1, 10 },
	tactical =
	{
		[ai.tactics.ENHANCEMENT] = 10
	}
	spell = 	function()
			local high
			if get_cast_level() >= 100 then
				high = 20
			elseif get_cast_level() >= 80 then
				high = 15
			elseif get_cast_level() >= 60 then
				high = 10
			elseif get_cast_level() >= 40 then
				high = 5
			elseif get_cast_level() >= 20 then
				high = 2
			else
				high = 1
			end

			timed_effect(timed_effect.FLY, rng(10) + 5 + get_cast_level(25), high)
			return true
	end,
	info = 	function()
			return "dur "..(5 + get_cast_level(25)).."+d10"
	end,
	desc =	{
			"Grants the power of controlled flight"
	}
}

INVISIBILITY = add_spell
{
	name = 		"Invisibility",
	school = 	{SCHOOL_AIR},
	level = 	31,
	mana = 		10,
	mana_max = 	20,
	fail = 		50,
	inertia = 	{ 1, 30 },
	tactical =
	{
		[ai.tactics.ENHANCEMENT] = 8
		[ai.tactics.ANNOY] = 3
	}
	spell = 	function()
			timed_effect(timed_effect.INVISIBILITY, rng(20) + 15 + get_cast_level(50), 20 + get_cast_level(50))
			return true
       	end,
	info = 	function()
			return "dur "..(15 + get_cast_level(50)).."+d20 power "..(20 + get_cast_level(50))
	end,
	desc =	{
			"Grants invisibility"
	}
}

POISONBLOOD = add_spell
{
	name = 		"Poison Blood",
	school = 	{SCHOOL_AIR},
	level = 	23,
	mana = 		10,
	mana_max = 	20,
	fail = 		30,
	stick =
	{
			charge =    { 10, 15 },
			[TV_ORB] =
			{
				rarity = 		45,
				base_level =	{ 1, 25 },
				max_level =		{ 35, 50 },
			},
	},
	inertia = 	{ 1, 35 },
	tactical =
	{
		[ai.tactics.ENHANCEMENT] = 5
		[ai.tactics.ATTACK] = 5
	}
	spell = 	function()
			local obvious = nil
			obvious = timed_effect(timed_effect.RES_POISON, rng(30) + 25 + get_cast_level(25), get_cast_level(70))
			obvious = is_obvious(timed_effect(timed_effect.MELEE_PROJECT, rng(30) + 25 + get_cast_level(25), dam.POIS, 10 + get_cast_level(30), 0, PROJECT_STOP | PROJECT_KILL | PROJECT_ITEM), obvious)
			return obvious
	end,
	info = 	function()
			return "dur "..(25 + get_cast_level(25)).."+d30 res "..get_cast_level(70).." dam "..(10 + get_cast_level(30))
	end,
	desc =	{
			"Grants resist poison",
			"Also imbues your weapon with poison, adding poison damage to your attacks"
	}
}

dam.LIGHTNING = dam.combine({{dam.ELEC, 34}, {dam.SOUND, 33}, {dam.LITE, 33}}, false, false, "lightning", nil, function()
				return iif(rng(5)<4, color.LIGHT_BLUE, color.YELLOW)
			end)

THUNDERSTORM = add_spell
{
	name = 		"Thunderstorm",
	school = 	{SCHOOL_AIR, SCHOOL_NATURE},
	level = 	49,
	mana = 		40,
	mana_max = 	60,
	fail = 		60,
	stick =
	{
			charge =    { 5, 5 },
			[TV_ORB] =
			{
				rarity = 		85,
				base_level =	{ 1, 5 },
				max_level =		{ 25, 50 },
			},
	},
	inertia = 	{ 2, 15 },
	tactical =
	{
		[ai.tactics.ATTACK] = 6
	}
	spell = 	function()
		timed_effect(timed_effect.RANDOMPROJECT, rng(10) + 10 + get_cast_level(25), dam.LIGHTNING, 5 + get_cast_level(10), 10 + get_cast_level(25))
	end,
	info = 	function()
		return "dam "..(5 + get_cast_level(10)).."d"..(10 + get_cast_level(25)).." dur "..(10 + get_cast_level(25)).."+d10"
	end,
	desc =	{
			"Charges up the air around you with electricity",
			"Each turn it will throw a thunder bolt at a random monster in sight",
			"The thunder does 3 types of damage, one third of lightning",
			"one third of sound and one third of light"
	}
}

STERILIZE = add_spell
{
	name = 		"Sterilize",
	school = 	{SCHOOL_AIR},
	level = 	39,
	mana = 		10,
	mana_max = 	100,
	fail = 		50,
	stick =
	{
			charge =    { 7, 5 },
			[TV_ORB] =
			{
				rarity = 		20,
				base_level =	{ 1, 10 },
				max_level =		{ 20, 50 },
			},
	},
	tactical =
	{
		[ai.tactics.ANNOY] = 10
	}
	spell = 	function()
			message("IMPLEMENT STERILIZE")

			-- set_no_breeders(rng(30) + 20 + get_cast_level(70))

			return true
	end,
	info = 	function()
			return "dur "..(20 + get_cast_level(70)).."+d30"
	end,
	desc =	{
			"Prevents explosive breeding for a while."
	}
}
