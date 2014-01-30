-- Handles thhe divination school

declare_global_constants {
	"IDENTIFY",
	"REVEALWAYS",
	"SENSEHIDDEN",
	"SENSEMONSTERS",
	"STARIDENTIFY",
	"VISION",
}

STARIDENTIFY = add_spell
{
	["name"] = 	"Greater Identify",
	["school"] = 	{SCHOOL_DIVINATION},
	["level"] = 	69,
	["mana"] = 	30,
	["mana_max"] = 	30,
	["fail"] = 	80,
	["spell"] = 	function()
			if get_check("Cast on yourself?") then
				self_knowledge()
			else
				identify_fully()
			end
			return true
	end,
	["info"] = 	function()
		       	return ""
	end,
	["desc"] =	{
			"Asks for an object and fully identify it, providing the full list of powers",
			"Cast at yourself it will reveal your powers"
	}
}

IDENTIFY = add_spell
{
	["name"] = 	"Identify",
	["school"] = 	{SCHOOL_DIVINATION},
	["level"] = 	15,
	["mana"] = 	10,
	["mana_max"] = 	50,
	["fail"] = 	40,
	["stick"] =
	{
			["charge"] =    { 7, 10 },
			[TV_ORB] =
			{
				["rarity"] = 		45,
				["base_level"] =	{ 1, 15 },
				["max_level"] =		{ 15, 40 },
			},
	},
	["spell"] = 	function()
			if get_cast_level(50) >= 27 then
				local obvious
				obvious = identify_pack()
				obvious = is_obvious(fire_ball(dam.KNOWLEDGE, 0, 1,
											   get_cast_level(3)),
									 obvious)
				if obvious == true then
					player.notice = player.notice | PN_COMBINE | PN_REORDER
				end
				return obvious
			elseif get_cast_level(50) >= 17 then
				local obvious
				obvious = identify_pack()
				obvious = is_obvious(fire_ball(dam.KNOWLEDGE, 0, 1, 0),
									 obvious)
				if obvious == true then
					player.notice = player.notice | PN_COMBINE | PN_REORDER
				end
				return obvious
			else
				if ident_spell() then return true else return SPELL_NOTHING end
			end
	end,
	["info"] = 	function()
			if get_cast_level(50) >= 27 then
				return "rad "..(get_cast_level(3))
			else
				return ""
			end
	end,
	["desc"] =	{
			"Asks for an object and identifies it",
			"At level 34 it identifies all objects in the inventory",
			"At level 54 it identifies all objects in the inventory and in a",
			"radius on the floor, as well as probing monsters and ",
			"identifying traps in that radius."
	}
}

VISION = add_spell
{
	["name"] = 	"Vision",
	["school"] = 	{SCHOOL_DIVINATION},
	["level"] = 	29,
	["mana"] = 	7,
	["mana_max"] = 	55,
	["fail"] = 	45,
	["stick"] =
	{
			["charge"] =    { 4, 6 },
			[TV_ORB] =
			{
				["rarity"] = 		60,
				["base_level"] =	{ 1, 5 },
				["max_level"] =		{ 10, 30 },
			},
	},
	["inertia"] = 	{ 2, 200 },
	["spell"] = 	function()
			if get_cast_level(50) >= 25 then
				wiz_lite_extra()
			else
				map_area(player.py, player.px, 10 + get_cast_level(40))
			end
			return true
	end,
	["info"] = 	function()
			return "rad "..(10 + get_cast_level(40))
	end,
	["desc"] =	{
			"Detects the layout of the surrounding area",
			"At level 50 it maps and lights the whole level",
	}
}

SENSEHIDDEN = add_spell
{
	["name"] = 	"Sense Hidden",
	["school"] = 	{SCHOOL_DIVINATION},
	["level"] = 	9,
	["mana"] = 	2,
	["mana_max"] = 	10,
	["fail"] = 	25,
	["stick"] =
	{
			["charge"] =    { 1, 15 },
			[TV_ORB] =
			{
				["rarity"] = 		20,
				["base_level"] =	{ 1, 15 },
				["max_level"] =		{ 10, 50 },
			},
	},
	["inertia"] = 	{ 1, 10 },
	["spell"] = 	function()
			traps.detect(10 + get_cast_level(40, 0))
			if get_cast_level(50) >= 15 then
				timed_effect(timed_effect.SEE_INVISIBILITY,
							 rng(10) + 20 + get_cast_level(40))
			end
			player.redraw[FLAG_PR_DTRAP] = 1
			-- Always obvious, since the DTrap status line is updated
			return true
	end,
	["info"] = 	function()
			if get_cast_level(50) >= 15 then
				return "rad "..(10 + get_cast_level(40)).." dur "..(10 + get_cast_level(40)).."+d20"
			else
				return "rad "..(10 + get_cast_level(40))
			end
	end,
	["desc"] =	{
			"Detects the traps in a certain radius around you",
			"At level 30 it allows you to sense invisible for a while"
	}
}

REVEALWAYS = add_spell
{
	["name"] = 	"Reveal Ways",
	["school"] = 	{SCHOOL_DIVINATION},
	["level"] = 	17,
	["mana"] = 	3,
	["mana_max"] = 	15,
	["fail"] = 	20,
	["stick"] =
	{
			["charge"] =    { 6, 6 },
			[TV_ORB] =
			{
				["rarity"] = 		35,
				["base_level"] =	{ 1, 15 },
				["max_level"] =		{ 25, 50 },
			},
	},
	["inertia"] = 	{ 1, 10 },
	["spell"] = 	function()
			local obvious
			obvious = detect_doors(10 + get_cast_level(40, 0))
			obvious = is_obvious(detect_stairs(10 + get_cast_level(40, 0)), obvious)
			return obvious
	end,
	["info"] = 	function()
			return "rad "..(10 + get_cast_level(40))
	end,
	["desc"] =	{
			"Detects the doors/stairs/ways in a certain radius around you",
	}
}

SENSEMONSTERS = add_spell
{
	["name"] = 	"Sense Monsters",
	["school"] = 	{SCHOOL_DIVINATION},
	["level"] = 	1,
	["mana"] =      1,
	["mana_max"] =  20,
	["fail"] = 	10,
	["stick"] =
	{
			["charge"] =    { 5, 10 },
			[TV_ORB] =
			{
				["rarity"] = 		37,
				["base_level"] =	{ 1, 10 },
				["max_level"] =		{ 15, 40 },
			},
	},
	["inertia"] = 	{ 1, 10 },
	["spell"] = 	function()
			local obvious
			local rad = 10 + get_cast_level(40, 0)
			obvious = monsters.detect(rad)
			if get_cast_level() >= 60 then
				local esp = { ANIMAL = rad }
				if get_cast_level() >= 100 then esp.ESP = rad end
				if get_cast_level() >= 90 then esp.DEMON = rad esp.DRAGON = rad end
				if get_cast_level() >= 80 then esp.TROLL = rad esp.GIANT = rad end
				if get_cast_level() >= 70 then esp.ORC = rad end
				obvious = is_obvious(timed_effect(timed_effect.ESP, 10 + rng(10) + get_cast_level(10), esp), obvious)
			end
			return obvious
	end,
	["info"] = 	function()
			if get_cast_level() >= 60 then
				return "rad "..(10 + get_cast_level(40)).." dur "..(10 + get_cast_level(10)).."+d10"
			else
				return "rad "..(10 + get_cast_level(40))
			end
	end,
	["desc"] =	{
			"Detects all monsters near you",
			"At level 60 it allows you to sense the minds of animals for a while"
			"At level 70 it allows you to sense the minds of orcs for a while"
			"At level 80 it allows you to sense the minds of trolls and giants for a while"
			"At level 90 it allows you to sense the minds of demons and dragons for a while"
			"At level 100 it allows you to sense the minds of everything for a while"
	}
}
