-- handle the mind school

declare_global_constants {
	"ARMOROFFEAR",
	"CHARM",
	"CONFUSE",
	"STUN",
	"NEURALBLAST",
	"MINDWAVE",
}

CHARM = add_spell
{
	["name"] = 	"Charm",
	["school"] = 	{SCHOOL_MIND},
	["level"] = 	1,
	["mana"] = 	1,
	["mana_max"] = 	20,
	["fail"] = 	10,
	["stick"] =
	{
			["charge"] =    { 7, 5 },
			[TV_ORB] =
			{
				["rarity"] = 		35,
				["base_level"] =	{ 1, 15 },
				["max_level"] =		{ 20, 40 },
			},
	},
	-- XXX What tactics *should* Charm have for the monster AI?
	tactical =
	{
		[ai.tactics.TACTIC] = 2
	}
	["spell"] = 	function()
			message("IMPLEMENT CHARM")
--[[
			if get_cast_level(50) >= 35 then
				return project_los(dam.CHARM, 10 + get_cast_level(150))
			elseif get_cast_level(50) >= 15 then
				local ret, dir = get_aim_dir()
				if not ret then return SPELL_NOTHING end
				return fire_ball(dam.CHARM, dir, 10 + get_cast_level(150), 3)
			else
				local ret, dir = get_aim_dir()
				if not ret then return SPELL_NOTHING end
				return fire_bolt(dam.CHARM, dir, 10 + get_cast_level(150))
			end
]]
	end,
	["info"] = 	function()
			return "power "..(10 + get_cast_level(150))
	end,
	["desc"] =	{
			"Tries to manipulate the mind of a monster to make it friendly",
			"At level 30 it turns into a ball",
			"At level 70 it affects all monsters in sight"
	}
}

CONFUSE = add_spell
{
	["name"] = 	"Confuse",
	["school"] = 	{SCHOOL_MIND},
	["level"] = 	9,
	["mana"] = 	5,
	["mana_max"] = 	30,
	["fail"] = 	20,
	["stick"] =
	{
			["charge"] =    { 3, 4 },
			[TV_ORB] =
			{
				["rarity"] = 		45,
				["base_level"] =	{ 1, 5 },
				["max_level"] =		{ 20, 40 },
			},
	},
	tactical =
	{
		[ai.tactics.ATTACK] = 2
		[ai.tactics.ANNOY] = 7
	}
	["spell"] = 	function()
			if get_cast_level(50) >= 35 then
				return project_los(dam.CONFUSE, 10 + get_cast_level(150))
			elseif get_cast_level(50) >= 15 then
				local ret, dir = get_aim_dir()
				if not ret then return SPELL_NOTHING end
				return fire_ball(dam.CONFUSE, dir, 10 + get_cast_level(150), 3)
			else
				local ret, dir = get_aim_dir()
				if not ret then return SPELL_NOTHING end
				return fire_bolt(dam.CONFUSE, dir, 10 + get_cast_level(150))
			end
	end,
	["info"] = 	function()
			return "power "..(10 + get_cast_level(150))
	end,
	["desc"] =	{
			"Tries to manipulate the mind of a monster to confuse it",
			"At level 30 it turns into a ball",
			"At level 70 it affects all monsters in sight"
	}
}

ARMOROFFEAR = add_spell
{
	["name"] = 	"Armor of Fear",
	["school"] = 	SCHOOL_MIND,
	["level"] = 	19,
	["mana"] = 	10,
	["mana_max"] = 	50,
	["fail"] = 	35,
	["inertia"] = 	{ 2, 20 },
	["spell"] = 	function()
			return timed_effect(timed_effect.AURA, 10 + get_cast_level(100), AURA_FEAR, 1 + get_cast_level(7), 5 + get_cast_level(20))
	end,
	["info"] = 	function()
			return "dur "..(10 + get_cast_level(100)).." power "..(1 + get_cast_level(7)).."d"..(5 + get_cast_level(20))
	end,
	["desc"] =	{
			"Creates a shield of pure fear around you. Any monster attempting to hit you",
			"must save or flee",
		}
}

STUN = add_spell
{
	["name"] = 	"Stun",
	["school"] = 	{SCHOOL_MIND},
	["level"] = 	29,
	["mana"] = 	10,
	["mana_max"] = 	90,
	["fail"] = 	45,
	tactical =
	{
		[ai.tactics.ATTACK] = 2
		[ai.tactics.ANNOY] = 8
	}
	["spell"] = 	function()
			if get_cast_level(50) >= 20 then
				local ret, dir = get_aim_dir()
				if not ret then return SPELL_NOTHING end
				return fire_ball(dam.STUN, dir, 10 + get_cast_level(150), 3)
			else
				local ret, dir = get_aim_dir()
				if not ret then return SPELL_NOTHING end
				return fire_bolt(dam.STUN, dir, 10 + get_cast_level(150))
			end
	end,
	["info"] = 	function()
			return "power "..(10 + get_cast_level(150))
	end,
	["desc"] =	{
			"Tries to manipulate the mind of a monster to stun it",
			"At level 40 it turns into a ball",
	}
}

MINDWAVE = add_spell
{
	["name"] = 	"Mindwave",
	["school"] = 	{SCHOOL_MIND},
	["level"] = 	39,
	["mana"] = 	10,
	["mana_max"] = 	50,
	["fail"] = 	45,
	tactical =
	{
		[ai.tactics.ATTACK] = 7
	}
	["spell"] = 	function()
			message("IMPLEMENT MINDWAVE")
	end,
	["info"] = 	function()
			return "power "..(10 + get_cast_level(150))
	end,
	["desc"] =	{
			"Shoots a ball of mind warping forces",
			"At level 16 it affects all monsters in sight",
	}
}

NEURALBLAST = add_spell
{
	["name"] = 	"Neural Blast",
	["school"] = 	{SCHOOL_MIND},
	["level"] = 	3,
	["mana"] = 	1,
	["mana_max"] = 	30,
	["fail"] = 	20,
	tactical =
	{
		[ai.tactics.ATTACK] = 7
	}
	["spell"] = 	function()
			message("IMPLEMENT NEURAL BLAST")
	end,
	["info"] = 	function()
			return "power "..(10 + get_cast_level(150))
	end,
	["desc"] =	{
			"Blasts the mind of its target",
	}
}


