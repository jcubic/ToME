-- Options management system

declare_global_constants
{
	"option_page",
	"option_print_prep",
	"option_print_screen",
	"option_display",
	"option_get_value",
	"get_option",
	"set_option",
}
declare_globals
{
	"__options",
	"__options_max",
}

__options_max = 0
__options = {}

-- Options are saved, naturaly
add_loadsave("game_options", {})

function option_page(t, recurs)
	assert(t.name, "no name")

	if not recurs then
		__options_max = __options_max + 1
		__options[__options_max] = t
	end

	__options[t.name] = t

	if t.options then
		if not t.enabled then t.disabled = true end
		t.dev = FALSE
		for i = 1, getn(t.options) do
			local option = t.options[i]

			-- If it is set for the branch, set it
			if t.can_set then option.can_set = t.can_set end

			option_page(option, not nil)
		end
	else
		set_option(t.name, t.default)
	end
end

function option_print_prep(options, level)
	local t = {}

	if options.hidden then return {} end
	if not game_options.use_advanced_options and options.advanced then return {} end

	options.level = level
	tinsert(t, options)

	if options.options and options.dev == true then
		for i = 1, getn(options.options) do
			merge_tables(t, option_print_prep(options.options[i], level + 1))
		end
	end

	return t
end

function option_get_value(option)
	if option.on_display then
		return option.on_display(get_option(option.name))
	else
		if get_option(option.name) then
			return "On", 3
		else
			return "Off", 3
		end
	end
end

function option_print_screen(options, max, begin, sel)
	local ret, wid, hgt = term.get_size()

	local t = {}

	for i = 1, max do
		merge_tables(t, option_print_prep(options[i], 0))
	end

	term.clear()
	term.blank_print(color.WHITE, game_module.." Options Screen", 0, 28);
	local keys = "#BEnter#W to develop a branch, #Bup#W/#Bdown#W to move, #Bright#W/#Bleft#W to modify, #B?#W for help";
	message.display(0, 1, strlen(keys), color.WHITE, keys);


	-- Display
	for i = begin, min(getn(t), hgt - 4 + begin - 1) do
		local val, len, c = "", 0, nil
		local before, after = " ", " "

		if not t[i].disabled then
			val, len, c = option_get_value(t[i])
		end

		if i == sel then
			c = color.LIGHT_BLUE
			before, after = "[", "]"
		elseif not c then
			c = color.RED
			if t[i].disabled then
				c = color.WHITE
			elseif get_option(t[i].name) then
				c = color.LIGHT_GREEN
			end
		end

		local symb = '.'
		if t[i].dev then
			if t[i].dev == true then symb = '-' else symb = '+' end
		elseif t[i].advanced then
			symb = 'A'
		end

		term.blank_print(c, before..symb..after..t[i].name, i + 4 - begin, 4 * t[i].level);
		term.blank_print(c, val, i + 4 - begin, wid - 1 - len);
	end
	return t, getn(t)
end

function set_option(name, value)
	local opt = __options[name]
	if type(value) == "function" then
		value()
	else
		if opt.on_set then
			opt.on_set(value)
		else
			game_options[opt.variable] = value
		end
	end
end

function get_option(name)
	local opt = __options[name]
	return game_options[opt.variable]
end

function option_display(subset)
	local ret, wid, hgt = term.get_size()
	local options, options_max = __options, __options_max
	local begin, sel = 1, 1

	if subset then
		options = __options[subset].options
		options_max = getn(options)
	end

	term.save()

	while not nil do
		local t, max_t = option_print_screen(options, options_max, begin, sel)

		if sel > max_t then
			sel = max_t
		end

		local c = strchar(term.inkey())

		if strbyte(c) == ESCAPE then
			break
		elseif c == "\r" then
			if t[sel].dev then
				if t[sel].dev == true then t[sel].dev = FALSE else t[sel].dev = true end
			end
		elseif c == "2" then
			-- Down key
			sel = sel + 1
			if sel > hgt - 3 - begin then
				begin = begin + 1
			end
			if sel > max_t then
				sel = max_t
				begin = max(1, sel - (hgt - 5))
			end
		elseif c == "8" then
			-- Up key
			sel = sel - 1
			if sel < begin then
				begin = begin - 1
			end
			if sel < 1 then
				sel, begin = 1, 1
			end
		elseif c == "6" then
			-- Left key (Off)
			if t[sel].dev then
				t[sel].dev = true
			elseif not t[sel].disabled then
				if (not t[sel].can_set) or (t[sel].can_set(true)) then
					set_option(t[sel].name, true)
				end
			end
		elseif c == "4" then
			-- Right key (On)
			if t[sel].dev then
				t[sel].dev = FALSE
			elseif not t[sel].disabled then
				if (not t[sel].can_set) or (t[sel].can_set(nil)) then
					set_option(t[sel].name, nil)
				end
			end
		elseif c == "3" then
			-- Page down key
			local diff = hgt - 3

			sel = sel + diff
			begin = begin + diff

			if sel > max_t then
				sel = max_t
				begin = max(1, sel - (hgt - 5))
			end
		elseif c == "9" then
			-- Page up key
			local diff = hgt - 3

			sel   = sel - diff
			begin = begin - diff

			if begin < 1 then
				begin = 1
			end
			if sel < 1 then
				sel = 1
			end
		end
	end

	term.load()
end
