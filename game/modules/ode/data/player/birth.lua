--
-- Birth sequence
--

-- Setup random names table
birth.set_random_names
{
	DEFAULT = "Human"
	Human =
	{
		[1] =
		{
			"Ab", "Ac", "Ad", "Af", "Agr", "Ast", "As", "Al", "Adw", "Adr", "Ar",
			"B", "Br", "C", "Cr", "Ch", "Cad", "D", "Dr", "Dw", "Ed", "Eth", "Et",
			"Er", "El", "Eow", "F", "Fr", "G", "Gr", "Gw", "Gal", "Gl", "H", "Ha",
			"Ib", "Jer", "K", "Ka", "Ked", "L", "Loth", "Lar", "Leg", "M", "Mir",
			"N", "Nyd", "Ol", "Oc", "On", "P", "Pr", "R", "Rh", "S", "Sev", "T",
			"Tr", "Th", "V", "Y", "Z", "W", "Wic",
		},
		[2] =
		{
			"a", "ae", "au", "ao", "are", "ale", "ali", "ay", "ardo", "e", "ei",
			"ea", "eri", "era", "ela", "eli", "enda", "erra", "i", "ia", "ie",
			"ire", "ira", "ila", "ili", "ira", "igo", "o", "oa", "oi", "oe",
			"ore", "u", "y",
		},
		[3] =
		{
			"a", "and", "b", "bwyn", "baen", "bard", "c", "ctred", "cred", "ch",
			"can", "d", "dan", "don", "der", "dric", "dfrid", "dus", "f", "g",
			"gord", "gan", "l", "li", "lgrin", "lin", "lith", "lath", "loth",
			"ld", "ldric", "ldan", "m", "mas", "mos", "mar", "mond", "n",
			"nydd", "nidd", "nnon", "nwan", "nyth", "nad", "nn", "nnor", "nd",
			"p", "r", "ron", "rd", "s", "sh", "seth", "sean", "t", "th", "tha",
			"tlan", "trem", "tram", "v", "vudd", "w", "wan", "win", "wyn", "wyr",
			"wyr", "wyth",
		}
	}
}

-- Setup the birth sequence
birth.sequence
{
	-- Load the base template, only one possibility, no choice will
	-- be displayed
	{ descriptor_type = "base" },
	{ descriptor_type = "class" title = "Select your role" },
	
	-- Ok now compute the complete and final descriptor
	{ action = "finalize" },

	-- Make up a random name based on the race(and not subrace) descriptor
	{ action = "random_name" random_name_descriptor = "base" },
	
	-- Setup some stats
	{ custom = function()
		player.stat_cur[A_STR] = 5
		player.stat_max[A_STR] = 5
		player.stat_cur[A_CON] = 5
		player.stat_max[A_CON] = 5
		player.au = 0
	end},

	-- Ask for a name
	{ action = "ask_name" },
}
