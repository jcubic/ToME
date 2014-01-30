-- Handle Eru Iluvatar magic school

declare_global_constants {
	"ERU_LISTEN",
	"ERU_PROT",
	"ERU_SEE",
	"ERU_UNDERSTAND",
}

ERU_SEE = add_spell
{
	["name"] = 	"See the Music",
	["school"] = 	{SCHOOL_ERU},
	["level"] = 	1,
	["mana"] = 	1,
	["mana_max"] = 	50,
	["fail"] = 	20,
	-- Uses piety to cast
	["piety"] =     true,
	["stat"] =      A_WIS,
	-- Unnafected by blindness
	["blind"] =     FALSE,
	["random"] = 	"prayer",
	["spell"] = 	function()
			local obvious
			obvious = set_tim_invis(rng(20) + 10 + get_cast_level(100))
			if get_cast_level() >= 60 then
				wiz_lite_extra()
				obvious = true
			elseif get_cast_level() >= 20 then
				map_area(player.py, player.px, 15 + get_cast_level(50))
				obvious = true
			end
			if get_cast_level() >= 40 then
				obvious = is_obvious(set_blind(0), obvious)
			end
			return obvious
	end,
	["info"] = 	function()
			return "dur "..(10 + get_cast_level(100)).."+d20"
	end,
	["desc"] =	{
			"Allows you to 'see' the Great Music from which the world",
			"originates, allowing you to see unseen things",
			"At level 20 it allows you to see your surroundings",
			"At level 40 it allows you to cure blindness",
			"At level 60 it allows you to fully see all the level"
	}
}

ERU_LISTEN = add_spell
{
	["name"] = 	"Listen to the Music",
	["school"] = 	{SCHOOL_ERU},
	["level"] = 	13,
	["mana"] = 	15,
	["mana_max"] = 	200,
	["fail"] = 	25,
	-- Uses piety to cast
	["piety"] =     true,
	["stat"] =      A_WIS,
	["random"] = 	"prayer",
	["spell"] = 	function()
			if get_cast_level() >= 60 then
				ident_all()
				identify_pack()
				return true
			elseif get_cast_level() >= 28 then
				identify_pack()
				return true
			else
				return ident_spell()
			end
	end,
	["info"] = 	function()
			return ""
	end,
	["desc"] =	{
			"Allows you to listen to the Great Music from which the world",
			"originates, allowing you to understand the meaning of things",
			"At level 28 it allows you to identify all your pack",
			"At level 60 it allows you to identify all items on the level",
	}
}

ERU_UNDERSTAND = add_spell
{
	["name"] = 	"Know the Music",
	["school"] = 	{SCHOOL_ERU},
	["level"] = 	59,
	["mana"] = 	200,
	["mana_max"] = 	600,
	["fail"] = 	50,
	-- Uses piety to cast
	["piety"] =     true,
	["stat"] =      A_WIS,
	["random"] = 	"prayer",
	["spell"] = 	function()
			if get_cast_level() >= 20 then
				identify_pack_fully()
				return true
			else
				return identify_fully()
			end
	end,
	["info"] = 	function()
			return ""
	end,
	["desc"] =	{
			"Allows you to understand the Great Music from which the world",
			"originates, allowing you to know the full abilities of things",
			"At level 20 it allows you to *identify* all your pack",
	}
}

ERU_PROT = add_spell
{
	["name"] = 	"Lay of Protection",
	["school"] = 	{SCHOOL_ERU},
	["level"] = 	69,
	["mana"] = 	400,
	["mana_max"] = 	400,
	["fail"] = 	80,
	-- Uses piety to cast
	["piety"] =     true,
	["stat"] =      A_WIS,
	["random"] = 	"prayer",
	["spell"] = 	function()
			return fire_ball(dam.MAKE_GLYPH, 0, 1, 1 + get_cast_level(2, 0))
	end,
	["info"] = 	function()
			return "rad "..(1 + get_cast_level(2, 0))
	end,
	["desc"] =	{
			"Creates a circle of safety around you",
	}
}
