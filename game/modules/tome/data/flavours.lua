--
-- Define the item flavours
--
-- It is possible to use the first_non_fixed attribute to have
-- a part of the list not randomized, see the colors parts for an examlpe
--


flavour.rocks = {
	{ "agate", color.LIGHT_WHITE },
	{ "alexandrite", color.GREEN },
	{ "amethyst", color.VIOLET },
	{ "aquamarine", color.LIGHT_BLUE },
	{ "azurite", color.LIGHT_BLUE },
	{ "beryl", color.LIGHT_GREEN },
	{ "black opal", color.LIGHT_DARK },
	{ "bloodstone", color.RED },
	{ "calcite", color.WHITE },
	{ "carnelian", color.RED },
	{ "corundum", color.SLATE },
	{ "diamond", color.WHITE },
	{ "dilithium", color.LIGHT_WHITE },
	{ "emerald", color.GREEN },
	{ "fluorite", color.LIGHT_GREEN },
	{ "fluorspar", color.BLUE },
	{ "garnet", color.RED },
	{ "granite", color.LIGHT_DARK },
	{ "jade", color.LIGHT_GREEN },
	{ "jasper", color.UMBER },
	{ "jet", color.LIGHT_DARK },
	{ "lapis lazuli", color.BLUE },
	{ "malachite", color.GREEN },
	{ "marble", color.WHITE },
	{ "moonstone", color.LIGHT_WHITE },
	{ "obsidian", color.LIGHT_DARK },
	{ "onyx", color.LIGHT_RED },
	{ "opal", color.LIGHT_WHITE },
	{ "quartz", color.LIGHT_WHITE },
	{ "quartzite", color.LIGHT_WHITE },
	{ "rhodonite", color.LIGHT_RED },
	{ "ruby", color.RED },
	{ "sapphire", color.BLUE },
	{ "tiger eye", color.YELLOW },
	{ "topaz", color.YELLOW },
	{ "turquoise", color.LIGHT_BLUE },
}

flavour.woods = {
	{ "ashen", color.SLATE },
	{ "aspen", color.LIGHT_UMBER },
	{ "balsa", color.LIGHT_UMBER },
	{ "bamboo", color.LIGHT_UMBER },
	{ "banyan", color.LIGHT_UMBER },
	{ "birch", color.LIGHT_UMBER },
	{ "cedar", color.LIGHT_UMBER },
	{ "cottonwood", color.LIGHT_UMBER },
	{ "cypress", color.LIGHT_UMBER },
	{ "dogwood", color.LIGHT_UMBER },
	{ "elm", color.LIGHT_UMBER },
	{ "eucalyptus", color.LIGHT_UMBER },
	{ "hawthorn", color.LIGHT_UMBER },
	{ "hemlock", color.LIGHT_UMBER },
	{ "hickory", color.UMBER },
	{ "ironwood", color.UMBER },
	{ "locust", color.LIGHT_UMBER },
	{ "mahogany", color.RED },
	{ "maple", color.LIGHT_UMBER },
	{ "mistletoe", color.GREEN },
	{ "mulberry", color.LIGHT_UMBER },
	{ "oak", color.LIGHT_UMBER },
	{ "pine", color.LIGHT_UMBER },
	{ "redwood", color.RED },
	{ "rosewood", color.RED },
	{ "spruce", color.LIGHT_UMBER },
	{ "sycamore", color.LIGHT_UMBER },
	{ "teak", color.LIGHT_UMBER },
	{ "walnut", color.UMBER },
	{ "willow", color.LIGHT_UMBER },
}

flavour.orb_special = {
	{ "gnarled", color.UMBER },
	{ "silver", color.LIGHT_WHITE },
	{ "runed", color.UMBER },
	{ "golden", color.YELLOW },
	{ "ivory", color.LIGHT_WHITE },
}

flavour.metals = {
	{ "adamantite", color.VIOLET },
	{ "aluminium", color.LIGHT_BLUE },
	{ "brass", color.LIGHT_UMBER },
	{ "bronze", color.LIGHT_UMBER },
	{ "cast iron", color.LIGHT_DARK },
	{ "chromium", color.WHITE },
	{ "copper", color.UMBER },
	{ "electrum", color.YELLOW },
	{ "gold", color.YELLOW },
	{ "iron", color.SLATE },
	{ "lead", color.SLATE },
	{ "mithril", color.LIGHT_BLUE },
	{ "nickel", color.LIGHT_WHITE },
	{ "platinum", color.WHITE },
	{ "pewter", color.SLATE },
	{ "silver", color.LIGHT_WHITE },
	{ "steel", color.LIGHT_WHITE },
	{ "tin", color.LIGHT_WHITE },
	{ "titanium", color.WHITE },
	{ "tungsten", color.WHITE },
	{ "uridium", color.LIGHT_RED },
	{ "zirconium", color.LIGHT_WHITE },
	{ "zinc", color.LIGHT_WHITE },

	{ "adamantite-plated", color.VIOLET },
	{ "aluminium-plated", color.LIGHT_BLUE },
	{ "brass-plated", color.LIGHT_UMBER },
	{ "bronze-plated", color.LIGHT_UMBER },
	{ "copper-plater", color.UMBER },
	{ "gold-plated", color.YELLOW },
	{ "mithril-plated", color.LIGHT_BLUE },
	{ "uridium-plated", color.LIGHT_RED },
}

--------------------------------------------------------------------------

flavour.add_generator(TV_WAND,
function(flavs, number)
	-- NOTE: Ignore number and generate as many as possible, since
	-- there are potentially hundreds of possible wand types, but
	-- only three different wand svals

	number = infinity

	flavour.make_from_list(flavs, number, flavour.woods)
	flavour.make_from_list(flavs, number, flavour.woods, "long")
	flavour.make_from_list(flavs, number, flavour.woods, "short")
	flavour.make_from_list(flavs, number, flavour.woods, "thin")
	flavour.make_from_list(flavs, number, flavour.woods, "thick")
	flavour.make_from_list(flavs, number, flavour.woods, "curved")
	flavour.make_from_list(flavs, number, flavour.woods, "hexagonal")
	flavour.make_from_list(flavs, number, flavour.woods, "octogonal")
	flavour.make_from_list(flavs, number, flavour.woods, "triangular")
end)
--------------------------------------------------------------------------

flavour.add_generator(TV_ORB,
function(flavs, number)
	-- NOTE: Ignore number and generate as many as possible, since
	-- there are potentially hundreds of possible wand types, but
	-- only three different wand svals

	number = infinity

	flavour.make_from_list(flavs, number, flavour.rocks)
	flavour.make_from_list(flavs, number, flavour.orb_special)
end)

-------------------------------------------------------------------------

flavour.add_generator(TV_RING,
function(flavs, number)
	local special = {
		{ "bone", color.WHITE },
		{ "double", color.ORANGE },
		{ "engagement", color.YELLOW },
		{ "glass", color.WHITE },
		{ "pearl", color.WHITE },
		{ "plain", color.YELLOW },
		{ "rusty", color.RED },
		{ "scarab", color.LIGHT_GREEN },
		{ "serpent", color.GREEN },
		{ "spikard", color.BLUE },
		{ "tortoise shell", color.GREEN },
		{ "transparent", color.WHITE },
		{ "wedding", color.YELLOW },
		{ "wire", color.UMBER },
		{ "wooden", color.UMBER },
	}

	if flavour.make_from_list(flavs, number, special) then
		return
	end

	if flavour.make_from_list(flavs, number, flavour.rocks) then
		return
	end

	if flavour.make_from_list(flavs, number, flavour.metals) then
		return
	end

	if flavour.make_from_list(flavs, number, flavour.rocks,
							  "uncut ") then
		return
	end

	if flavour.make_from_list(flavs, number, flavour.metals,
							  "engraved ") then
		return
	end
end)

--------------------------------------------------------------------------


flavour.add_generator(TV_AMULET,
function(flavs, number)
	local special = {
		{ "amber", color.YELLOW },
		{ "bone", color.WHITE },
		{ "carved oak", color.UMBER },
		{ "coral", color.WHITE },
		{ "crystal", color.LIGHT_BLUE },
		{ "dragon tooth", color.LIGHT_WHITE },
		{ "driftwood", color.LIGHT_UMBER },
		{ "glass", color.WHITE },
		{ "ivory", color.WHITE },
		{ "meteoric iron", color.LIGHT_DARK },
		{ "origami paper", color.WHITE },
		{ "sea shell", color.LIGHT_BLUE },
		{ "tortoise shell", color.GREEN },
	}

	if flavour.make_from_list(flavs, number, special) then
		return
	end

	if flavour.make_from_list(flavs, number, flavour.rocks) then
		return
	end

	if flavour.make_from_list(flavs, number, flavour.metals) then
		return
	end

	if flavour.make_from_list(flavs, number, flavour.rocks,
							  "uncut ") then
		return
	end

	if flavour.make_from_list(flavs, number, flavour.metals,
							  "engraved ") then
		return
	end
end)

---------------------------------------------------------------------------

flavour.colors_unusual = {
	{ "azure", color.LIGHT_BLUE },
	{ "chartreuse", color.LIGHT_GREEN },
	{ "crimson", color.RED },
	{ "cyan", color.LIGHT_BLUE },
	{ "gold", color.YELLOW },
	{ "indigo", color.VIOLET },
	{ "ivory white", color.LIGHT_WHITE },
	{ "magenta", color.RED },
	{ "pink", color.LIGHT_RED },
	{ "puce", color.VIOLET },
	{ "silver", color.LIGHT_WHITE },
	{ "tangerine", color.ORANGE },
	{ "vermilion", color.RED },
}

flavour.colors_rainbow = {
	{ "red", color.RED },
	{ "orange", color.ORANGE },
	{ "yellow", color.YELLOW },
	{ "green", color.GREEN },
	{ "blue", color.BLUE },
	{ "violet", color.VIOLET },
}

flavour.colors_basic = {
	{ "red", color.RED },
	{ "orange", color.ORANGE },
	{ "yellow", color.YELLOW },
	{ "green", color.GREEN },
	{ "blue", color.BLUE },
	{ "violet", color.VIOLET },
	{ "brown", color.UMBER },
	{ "black", color.LIGHT_DARK },
	{ "grey", color.SLATE },
}

flavour.colors_light_dark = {
	{ "dark red", color.RED },
	{ "dark green", color.GREEN },
	{ "light green", color.LIGHT_GREEN },
	{ "dark blue", color.BLUE },
	{ "light blue", color.LIGHT_BLUE },
	{ "light brown", color.LIGHT_UMBER },
}

flavour.add_generator(TV_POTION,
function(flavs, number)
	local special = {
		{ "gloopy green", color.GREEN },
		{ "hazy", color.LIGHT_WHITE },
		{ "misty", color.LIGHT_WHITE },
		{ "pungent", color.LIGHT_RED },
		{ "smoky", color.LIGHT_DARK },
	}

	if flavour.make_from_list(flavs, number, special) then
		return
	end

	if flavour.make_from_list(flavs, number, flavour.colors_unusual) then
		return
	end

	if flavour.make_from_list(flavs, number, flavour.colors_basic) then
		return
	end

	if flavour.make_from_list(flavs, number, flavour.colors_light_dark) then
		return
	end

	if flavour.make_from_list(flavs, number, flavour.colors_rainbow,
							  "metallic ") then
		return
	end

	if flavour.make_from_list(flavs, number, flavour.colors_rainbow,
							  "glowing ") then
		return
	end

	---

	if flavour.make_from_list(flavs, number, flavour.colors_basic,
							  "cloudy ") then
		return
	end

	if flavour.make_from_list(flavs, number, flavour.colors_basic,
							  nil, "speckled ") then
		return
	end

	if flavour.make_from_list(flavs, number, flavour.colors_basic,
							  "viscous ") then
		return
	end

	if flavour.make_from_list(flavs, number, flavour.colors_basic,
							  "clotted ") then
		return
	end

	if flavour.make_from_list(flavs, number, flavour.colors_basic,
							  "oily ") then
		return
	end

	if flavour.make_from_list(flavs, number, flavour.colors_basic,
							  "watery ") then
		return
	end

	if flavour.make_from_list(flavs, number, flavour.colors_basic,
							  "bubbling ") then
		return
	end

	---

	if flavour.make_from_list(flavs, number, flavour.colors_light_dark,
							  "cloudy ") then
		return
	end

	if flavour.make_from_list(flavs, number, flavour.colors_light_dark,
							  nil, "speckled ") then
		return
	end

	if flavour.make_from_list(flavs, number, flavour.colors_light_dark,
							  "viscous ") then
		return
	end

	if flavour.make_from_list(flavs, number, flavour.colors_light_dark,
							  "clotted ") then
		return
	end

	if flavour.make_from_list(flavs, number, flavour.colors_light_dark,
							  "oily ") then
		return
	end

	if flavour.make_from_list(flavs, number, flavour.colors_light_dark,
							  "watery ") then
		return
	end

	if flavour.make_from_list(flavs, number, flavour.colors_light_dark,
							  "bubbling ") then
		return
	end

	---

	if flavour.make_from_list(flavs, number, flavour.colors_unusual,
							  "viscous ") then
		return
	end

	if flavour.make_from_list(flavs, number, flavour.colors_unusual,
							  "clotted ") then
		return
	end

	if flavour.make_from_list(flavs, number, flavour.colors_unusual,
							  "oily ") then
		return
	end

	if flavour.make_from_list(flavs, number, flavour.colors_unusual,
							  "watery ") then
		return
	end

	if flavour.make_from_list(flavs, number, flavour.colors_unusual,
							  "bubbling ") then
		return
	end

	if flavour.make_from_list(flavs, number, flavour.colors_unusual,
							  nil, "speckled ") then
		return
	end
end)

--------------------------------------------------------------------------
flavour.add_generator(TV_MUSHROOM,
function(flavs, number)

	if flavour.make_from_list(flavs, number, flavour.colors_basic) then
		return
	end

	if flavour.make_from_list(flavs, number, flavour.colors_basic,
							  "silmy") then
		return
	end

	if flavour.make_from_list(flavs, number, flavour.colors_basic,
							  "furry") then
		return
	end

	if flavour.make_from_list(flavs, number, flavour.colors_basic,
							  "wrinkled") then
		return
	end

	if flavour.make_from_list(flavs, number, flavour.colors_basic,
							  "spotted") then
		return
	end
end)

--------------------------------------------------------------------------

flavour.syllables =	{
	"a", "ab", "ag", "aks", "ala", "an", "ankh", "app",
	"arg", "arze", "ash", "aus", "ban", "bar", "bat", "bek",
	"bie", "bin", "bit", "bjor", "blu", "bot", "bu",
	"byt", "comp", "con", "cos", "cre", "dalf", "dan",
	"den", "der", "doe", "dok", "eep", "el", "eng", "er", "ere", "erk",
	"esh", "evs", "fa", "fid", "flit", "for", "fri", "fu", "gan",
	"gar", "glen", "gop", "gre", "ha", "he", "hyd", "i",
	"ing", "ion", "ip", "ish", "it", "ite", "iv", "jo",
	"kho", "kli", "klis", "la", "lech", "man", "mar",
	"me", "mi", "mic", "mik", "mon", "mung", "mur", "nag", "nej",
	"nelg", "nep", "ner", "nes", "nis", "nih", "nin", "o",
	"od", "ood", "org", "orn", "ox", "oxy", "pay", "pet",
	"ple", "plu", "po", "pot", "prok", "re", "rea", "rhov",
	"ri", "ro", "rog", "rok", "rol", "sa", "san", "sat",
	"see", "sef", "seh", "shu", "ski", "sna", "sne", "snik",
	"sno", "so", "sol", "sri", "sta", "sun", "ta", "tab",
	"tem", "ther", "ti", "tox", "trol", "tue", "turs", "u",
	"ulk", "um", "un", "uni", "ur", "val", "viv", "vly",
	"vom", "wah", "wed", "werg", "wex", "whon", "wun", "x",
	"yerg", "yp", "zun", "tri", "blaa", "jah", "bul", "on",
	"foo", "ju", "xuxu",
}

flavour.add_generator(TV_SCROLL,
function(flavs, number)
	flavour.title_generator(flavs, flavour.syllables,
							color.WHITE, number)
end)

--------------------------------------------------------------------------
