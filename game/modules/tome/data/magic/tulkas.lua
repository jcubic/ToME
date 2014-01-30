-- Handle Tulkas magic school

declare_global_constants {
	"TULKAS_AIM",
	"TULKAS_SPIN",
	"TULKAS_WAVE",
}

TULKAS_AIM = add_spell
{
	["name"] =      "Divine Aim",
	["school"] =    {SCHOOL_TULKAS},
	["level"] =     1,
	["mana"] =      30,
	["mana_max"] =  500,
	["fail"] = 	20,
	-- Uses piety to cast
	["piety"] =     true,
	["stat"] =      A_WIS,
	["random"] = 	"prayer",
	["spell"] = 	function()
			local dur = get_cast_level(50) + rng(10)
			local obvious

			obvious = set_strike(dur)
			if get_cast_level() >= 40 then
				obvious = is_obvious(set_tim_deadly(dur), obvious)
			end
			return obvious
	end,
	["info"] = 	function()
			return "dur "..(get_cast_level(50)).."+d10"
	end,
	["desc"] =	{
			"It makes you more accurate in combat",
			"At level 40 all your blows are critical hits",
	}
}

TULKAS_WAVE = add_spell
{
	["name"] =      "Wave of Power",
	["school"] =    {SCHOOL_TULKAS},
	["level"] =     39,
	["mana"] =      200,
	["mana_max"] =  200,
	["fail"] = 	75,
	-- Uses piety to cast
	["piety"] =     true,
	["stat"] =      A_WIS,
	["random"] = 	"prayer",
	["spell"] = 	function()
			local ret, dir = get_aim_dir()
			if not ret then return SPELL_NOTHING end

			return fire_bolt(dam.ATTACK, dir, get_cast_level(player.num_blow))
	end,
	["info"] = 	function()
			return "blows "..(get_cast_level(player.num_blow))
	end,
	["desc"] =	{
			"It allows you to project a number of melee blows across a distance",
	}
}

TULKAS_SPIN = add_spell
{
	["name"] =      "Whirlwind",
	["school"] =    {SCHOOL_TULKAS},
	["level"] =     19,
	["mana"] =      100,
	["mana_max"] =  100,
	["fail"] = 	45,
	-- Uses piety to cast
	["piety"] =     true,
	["stat"] =      A_WIS,
	["random"] = 	"prayer",
	["spell"] = 	function()
			return fire_ball(dam.ATTACK, 0, 1, 1)
	end,
	["info"] = 	function()
			return ""
	end,
	["desc"] =	{
			"It allows you to spin around and hit all monsters nearby",
	}
}
