--
-- Birth sequence
--

-- Point based stats generation
birth.gen_stats_point = function()
	local poyntz = 20

	hook.process(hook.CHARACTER_GENERATED)

	local cur =
	{
		sel = 0
		points = {}
		stat_base = {}
	}
	-- Redefine the display_stat function of the engine to display the selected stat
	local old_display = character_sheet.display_stat
	character_sheet.display_stat = function(stat)
	        local text = format("%2d", player.stat_ind[stat] + (%cur.points[stat] or 0))

		if %cur.sel == stat then
			return format(":#y>%19s", text)
		else
			return format(":#G%20s", text)
		end
	end

	-- Save the base stats after backing out race/class adjustments.
	for i = 0, stats.MAX do
		cur.stat_base[i] = player.stat_ind[i] - player.stat_add[i]
	end

	while not nil do
		player.chp(player.mhp()) player.csp(player.msp()) player.csane(player.msane())
		character_sheet.display(1)
		term.blank_print("Arrow Keys to arrange points, ENTER when done. "..poyntz.." points left)", 0, 0)

		local key = strchar(term.inkey())

		if key == '8' and cur.sel > 0 then
			cur.sel = cur.sel - 1
		elseif key == '2' and cur.sel < stats.MAX then
			cur.sel = cur.sel + 1
		elseif key == '6' or key == '+' then

			if poyntz > 0 and player.stat_max[cur.sel] < 25 then
				poyntz = poyntz - 1
				player.stat_ind[cur.sel] = player.stat_ind[cur.sel] + 1
				player.stat_cur[cur.sel] = player.stat_cur[cur.sel] + 1
				player.stat_max[cur.sel] = player.stat_max[cur.sel] + 1
				player.update = player.update | PU_BONUS | PU_HP | PU_MANA | PU_SANITY
				update_stuff()
			end

		elseif key == '4' or key == '-' then
			if player.stat_max[cur.sel] > 5 then
				poyntz = poyntz + 1
				player.stat_ind[cur.sel] = player.stat_ind[cur.sel] - 1
				player.stat_cur[cur.sel] = player.stat_cur[cur.sel] - 1
				player.stat_max[cur.sel] = player.stat_max[cur.sel] - 1
				player.update = player.update | PU_BONUS | PU_HP | PU_MANA | PU_SANITY
				update_stuff()
			end
		elseif key == "\r" or key == ESCAPE then
			local ok = true
			if poyntz > 0 then
				local ret = msg_box("You still have "..poyntz.." to use, are you sure you do not want to? (y/n)?")
				if ret and strlower(strchar(ret)) ~= 'y' then
					ok = false
				end
			end

			if ok then
				for i = 0, stats.MAX do player.stat_cur[i] = player.stat_max[i] end
				player.update = player.update | PU_BONUS | PU_HP | PU_MANA | PU_SANITY
				update_stuff()
				break
			end
		end
	end

	-- Set the normal displayt stat function back
	character_sheet.display_stat = old_display
end

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

	Dwarf =
	{
		[1] =
		{
			"B", "D", "F", "G", "Gl", "H", "K", "L", "M", "N", "R", "S", "T", "Th", "V",
		}
		[2] =
		{
			"a", "e", "i", "o", "oi", "u",
		}
		[3] =
		{
			"bur", "fur", "gan", "gnus", "gnar", "li", "lin", "lir", "mli", "nar", "nus", "rin", "ran", "sin", "sil", "sur",
		}
	}

	Elf =
	{
		[1] =
		{
			"Al", "An", "Bal", "Bel", "Cal", "Cel", "El", "Elr", "Elv", "Eow", "Ear", "F", "Fal", "Fel", "Fin", "G", "Gal", "Gel", "Gl", "Is", "Lan", "Leg", "Lom", "N", "Nal", "Nel",  "S", "Sal", "Sel", "T", "Tal", "Tel", "Thr", "Tin",
		}
		[2] =
		{
			"a", "adrie", "ara", "e", "ebri", "ele", "ere", "i", "io", "ithra", "ilma", "il-Ga", "ili", "o", "orfi", "u", "y",
		}
		[3] =
		{
			"l", "las", "lad", "ldor", "ldur", "linde", "lith", "mir", "n", "nd", "ndel", "ndil", "ndir", "nduil", "ng", "mbor", "r", "rith", "ril", "riand", "rion", "s", "thien", "viel", "wen", "wyn",
		}
	}

	Hobbit =
	{
		[1] =
		{
			"B", "Ber", "Br", "D", "Der", "Dr", "F", "Fr", "G", "H", "L", "Ler", "M", "Mer", "N", "P", "Pr", "Per", "R", "S", "T", "W",
		}
		[2] =
		{
			"a", "e", "i", "ia", "o", "oi", "u",
		}
		[3] =
		{
			"bo", "ck", "decan", "degar", "do", "doc", "go", "grin", "lba", "lbo", "lda", "ldo", "lla", "ll", "lo", "m", "mwise", "nac", "noc", "nwise", "p", "ppin", "pper", "tho", "to",
		}
	}

	Ent =
	{
		[1] =
		{
			"Tree","Root","Bark","Beam","Leaf",
		}
		[2] =
		{
			"-",
		}
		[3] =
		{
			"tender","planter","shepherd","watcher","grower","warden",
		}
	}
}

-- Possible race brackground name placement
birth.place = { BEFORE=0 AFTER=1 NONE=2 }

-- Setup the birth sequence
birth.sequence
{
	-- Load the base template, only one possibility, no choice will
	-- be displayed
	{ descriptor_type = "base" },
	-- Ask for race
	{
		title = "Select your race"
		desc = {
			"Race determines your basic physical and mental statistisc and some races may"
			"have special powers."
			"If you do not know what to choose, Humans are the best for beginners."
		}
		descriptor_type = "race"
	},
	-- Ask for subrace
	{
		title = "Select your subrace"
		desc = {
			"The race you have selected comes in different 'flavors',"
			"each of them has different traits and may have special powers."
			"If you do not know what to choose, the first choice is the best for beginners."
		}
		descriptor_type = "subrace"
	},
	-- Ask for racial background
	{
		title = "Select your racial background"
		desc = {
			"The background represents the history of your character,"
			"how he lived in the past or his way of life."
			"If you do not know what to choose, 'Normal' is the best for beginners."
			"#RWARNING: Do not try a Lost Soul unless you know what you are doing."
		}
		descriptor_type = "race_background"
	},
	-- Ask for sex
	{
		title = "Select your sex"
		desc = {
			"Please select a sex for your character."
			"Sex usualy has no big influence on gameplay, so choose what you prefer."
		}
		descriptor_type = "sex"
	},
	-- Ask for class
	{
		title = "Select your class"
		desc = {
			"Your class is what you are good at doing, this determines the skills your get"
			"when you start playing."
			"If you do not know what to choose, Warriors are the best for beginners."
		}
		descriptor_type = "class"
	},
	-- Ask for class spec
	{
		title = "Select your class specialization"
		desc = {
			"The class you have selected comes in different 'flavors', changing the skills"
			"you start with, providing new ones or specializing in some."
			"If you do not know what to choose, the first choice is the best for beginners."
		}
		descriptor_type = "subclass"
	},
	-- Ok now compute the complete and final descriptor
	{ action = "finalize" },

	-- Make up a random name based on the race(and not subrace) descriptor
	{ action = "random_name" random_name_descriptor = "race" },

	-- Point based stat generation
	{ custom = birth.gen_stats_point },

	-- Ask for a name
	{ action = "ask_name" },
}

-- Recalculate, and reset, maximum for HP/SP/SN as the very last
-- thing before the start of a game, so that the relevant skills
-- and stats are properly set, and autoworn initial equipment
-- is taken into account.
-- Ask for skills just after birth, to allocate the first points
hook(hook.GAME_START, function(new)
	if new then
		do_cmd_skill()
		update_stuff()

		player.calc_hitpoints()
		player.chp(player.mhp())

		player.calc_mana()
		player.csp(player.msp())

		player.calc_sanity()
		player.csane(player.msane())
	end
end)
