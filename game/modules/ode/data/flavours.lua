--
-- Define the item flavours
--
-- It is possible to use the first_non_fixed attribute to have
-- a part of the list not randomized, see the colors parts for an examlpe
--

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
