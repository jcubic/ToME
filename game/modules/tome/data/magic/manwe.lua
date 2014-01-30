-- Handle Manwe Sulimo magic school

declare_global_constants {
	"MANWE_AVATAR",
	"MANWE_BLESS",
	"MANWE_CALL",
	"MANWE_SHIELD",
}

MANWE_SHIELD = add_spell
{
	["name"] =      "Wind Shield",
	["school"] =    {SCHOOL_MANWE},
	["level"] =     19,
	["mana"] =      100,
	["mana_max"] =  500,
	["fail"] = 	30,
	-- Uses piety to cast
	["piety"] =     true,
	["stat"] =      A_WIS,
	["random"] = 	"prayer",
	["spell"] = 	function()
			local dur = get_cast_level(50) + 10 + rng(20)
			local obvious

			obvious = set_protevil(dur)
			if get_cast_level() >= 20 then
				local type

				type = 0
				if get_cast_level() >= 40 then
					type = SHIELD_COUNTER
				end
				obvious = is_obvious(set_shield(dur, get_cast_level(30), type, 1 + get_cast_level(2), 1 + get_cast_level(6)), obvious)
			end
			return obvious
	end,
	["info"] = 	function()
			local desc = "dur "..(get_cast_level(50) + 10).."+d20"

			if get_cast_level() >= 20 then
				desc = desc.." AC "..(get_cast_level(30))
			end
			if get_cast_level() >= 40 then
				desc = desc.." dam "..rng.desc_roll(1 + get_cast_level(2), 1 + get_cast_level(6))
			end
			return desc
	end,
	["desc"] =	{
			"It surrounds you with a shield of wind that deflects blows from evil monsters",
			"At level 20 it increases your armour rating",
			"At level 40 it retaliates against monsters that melee you",
	}
}

MANWE_AVATAR = add_spell
{
	["name"] =      "Avatar",
	["school"] =    {SCHOOL_MANWE},
	["level"] =     69,
	["mana"] =      1000,
	["mana_max"] =  1000,
	["fail"] = 	80,
	-- Uses piety to cast
	["piety"] =     true,
	["stat"] =      A_WIS,
	["random"] = 	"prayer",
	["spell"] = 	function()
			return set_mimic(get_cast_level(20) + rng(10), resolve_mimic_name("Maia"), player.lev)
	end,
	["info"] =      function()
			return "dur "..(get_cast_level(20)).."+d10"
	end,
	["desc"] =	{
			"It turns you into a full grown Maia",
	}
}

MANWE_BLESS = add_spell
{
	["name"] =      "Manwe's Blessing",
	["school"] =    {SCHOOL_MANWE},
	["level"] =     1,
	["mana"] =      10,
	["mana_max"] =  100,
	["fail"] = 	20,
	-- Uses piety to cast
	["piety"] =     true,
	["stat"] =      A_WIS,
	["random"] = 	"prayer",
	["spell"] = 	function()
			local dur = get_cast_level(70) + 30 + rng(40)
			local obvious

			obvious = set_blessed(dur)
			obvious = is_obvious(set_afraid(0), obvious)
			obvious = is_obvious(set_lite(dur), obvious)
			if get_cast_level() >= 20 then
				obvious = is_obvious(set_hero(dur), obvious)
			end
			if get_cast_level() >= 40 then
				obvious = is_obvious(set_shero(dur), obvious)
			end
			if get_cast_level() >= 60 then
				obvious = is_obvious(set_holy(dur), obvious)
			end
			return obvious
	end,
	["info"] =      function()
			return "dur "..(get_cast_level(70) + 30).."+d40"
	end,
	["desc"] =	{
			"Manwe's Blessing removes your fears, blesses you and surrounds you with",
			"holy light",
			"At level 20 it also grants heroism",
			"At level 40 it also grants super heroism",
			"At level 60 it also grants holy luck and life protection",
	}
}

MANWE_CALL = add_spell
{
	["name"] =      "Manwe's Call",
	["school"] =    {SCHOOL_MANWE},
	["level"] =     39,
	["mana"] =      200,
	["mana_max"] =  500,
	["fail"] = 	40,
	-- Uses piety to cast
	["piety"] =     true,
	["stat"] =      A_WIS,
	["random"] = 	"prayer",
	["spell"] = 	function()
			local y, x, m_idx

			y, x = find_position(player.py, player.px)
			m_idx = place_monster_one(y, x, test_monster_name("Great eagle"), 0, FALSE, FACTION_PLAYER)

			if m_idx ~= 0 then
				monster_set_level(m_idx, 20 + get_cast_level(70, 0))
				return true
			end
			return
	end,
	["info"] =      function()
			return "level "..(get_cast_level(70) + 20)
	end,
	["desc"] =	{
			"Manwe's Call summons a Great Eagle to help you battle the forces",
			"of Morgoth"
	}
}
