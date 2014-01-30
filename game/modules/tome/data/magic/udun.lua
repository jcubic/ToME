-- handle the udun school

declare_global_constants {
	"DRAIN",
	"FLAMEOFUDUN",
	"GENOCIDE",
	"WRAITHFORM",
	"levels_in_book",
	"udun_in_book",
}

DRAIN = add_spell
{
	["name"] = 	"Drain",
	["school"] = 	{SCHOOL_UDUN, SCHOOL_MANA},
	["level"] = 	1,
	["mana"] = 	0,
	["mana_max"] = 	0,
	["fail"] = 	20,
	["spell"] = 	function()
			local ret, item, obj, o_name, add

			-- Ask for an item
			ret, item = get_item("What item to drain?", "You have nothing you can drain", USE_INVEN,
				function (obj)
					if (obj.tval == TV_WAND) or (obj.tval == TV_ORB) then
						return true
					end
					return FALSE
				end
			)

			if ret then
				-- get the item
				obj = get_object(item)

				add = 0
				if (obj.tval == TV_ORB) or (obj.tval == TV_WAND) then
					local kind = get_kind(obj)

					add = kind.level * obj.pval * obj.number

					-- Destroy it!
					inven_item_increase(item, -99)
					inven_item_describe(item)
					inven_item_optimize(item)
				end
				increase_mana(add)
			end
			return true
	end,
	["info"] = 	function()
			return ""
	end,
	["desc"] =	{
			"Drains the mana contained in wands and orbs to increase yours",
	}
}

GENOCIDE = add_spell
{
	["name"] = 	"Genocide",
	["school"] = 	{SCHOOL_UDUN, SCHOOL_NATURE},
	["level"] = 	49,
	["mana"] = 	50,
	["mana_max"] = 	50,
	["fail"] = 	90,
	["stick"] =
	{
			["charge"] =    { 2, 2 },
			[TV_ORB] =
			{
				["rarity"] = 		85,
				["base_level"] =	{ 1, 1 },
				["max_level"] =		{ 5, 15 },
			},
	},
	["spell"] = 	function()
			local type

			type = 0
			if get_cast_level() >= 20 then type = 1 end
			if type == 0 then
				genocide(true)
				return true
			else
				if get_check("Genocide all monsters near you? ") then
					mass_genocide(true)
				else
					genocide(true)
				end
				return true
			end
	end,
	["info"] = 	function()
			return ""
	end,
	["desc"] =	{
			"Genocides all monsters of a race on the level",
			"At level 20 it can genocide all monsters near you"
	}
}

WRAITHFORM = add_spell
{
	["name"] = 	"Wraithform",
	["school"] = 	{SCHOOL_UDUN, SCHOOL_CONVEYANCE},
	["level"] = 	59,
	["mana"] = 	20,
	["mana_max"] = 	40,
	["fail"] = 	95,
	["inertia"] = 	{ 4, 30 },
	tactical =
	{
		[ai.tactics.ENHANCEMENT] = 7
		[ai.tactics.NO_LOS] = true
	}
	["spell"] = 	function()
		       	return set_shadow(rng(30) + 20 + get_cast_level(40))
	end,
	["info"] = 	function()
			return "dur "..(20 + get_cast_level(40)).."+d30"
	end,
	["desc"] =	{
			"Turns you into an immaterial being",
	}
}

FLAMEOFUDUN = add_spell
{
	["name"] = 	"Flame of Udun",
	["school"] = 	{SCHOOL_UDUN, SCHOOL_FIRE},
	["level"] = 	69,
	["mana"] = 	70,
	["mana_max"] = 	100,
	["fail"] = 	95,
	["inertia"] = 	{ 7, 15 },
	["spell"] = 	function()
			return set_mimic(rng(15) + 5 + get_cast_level(30), resolve_mimic_name("Balrog"), get_cast_level(50))
	end,
	["info"] = 	function()
			return "dur "..(5 + get_cast_level(30)).."+d15"
	end,
	["desc"] =	{
			"Turns you into a powerful Balrog",
	}
}


-- Return the number of Udun/Melkor spells in a given book
function udun_in_book(sval, pval)
	local i, y, index, sch, s

	i = 0

	-- Hack if the book is 255 it is a random book
	if sval == 255 then
		school_book[sval] = {pval}
	end
	-- Parse all spells
	for index, s in school_book[sval] do
		for index, sch in __spell_school[s] do
			if sch == SCHOOL_UDUN then i = i + 1 end
			if sch == SCHOOL_MELKOR then i = i + 1 end
		end
	end
	return i
end

-- Return the total level of spells
function levels_in_book(sval, pval)
	local i, y, index, sch, s

	i = 0

	-- Hack if the book is 255 it is a random book
	if sval == 255 then
		school_book[sval] = {pval}
	end

	-- Parse all spells
	for index, s in school_book[sval] do
		i = i + __tmp_spells[s].level
	end
	return i
end
