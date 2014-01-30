-- Handle screen display

declare_global_constants {
	"__display_draw_hook",
	"record_screen_mode",
	"overlay_screen_mode",
	"set_screen_mode",
}
declare_globals {
	"__display",
}

-- Setup the table to be tcorrect drawing info
function record_screen_mode(t)
	for i = 1, getn(t) do
		local disp = t[i]
		assert(disp.flag, "no display flag")
		assert(disp.y, "no display y")
		assert(disp.x, "no display y")
		assert(disp.width, "no display width")
		assert(disp.height, "no display height")
		assert(disp.display, "no display function")

		if type(disp.x) == "function" then disp.fct_x = disp.x; disp.x = nil end
		if type(disp.y) == "function" then disp.fct_y = disp.y; disp.y = nil end
		if type(disp.width) == "function" then disp.fct_width = disp.width; disp.width = nil end
		if type(disp.height) == "function" then disp.fct_height = disp.height; disp.height = nil end

		if not disp.clean then disp.clean = true end

		if type(disp.flag) == "number" then disp.flag = { disp.flag } end

		disp.execute = function()
			local x, y, w, h
			local ret, wid, hgt = term.get_size()

			if %disp.x then x = %disp.x else x = %disp.fct_x(hgt, wid) end
			if %disp.y then y = %disp.y else y = %disp.fct_y(hgt, wid) end
			if %disp.width then w = %disp.width else w = %disp.fct_width(hgt, wid) end
			if %disp.height then h = %disp.height else h = %disp.fct_height(hgt, wid) end

			if %disp.clean == true then
				for j = y, y + h - 1 do
					term.print(color.LIGHT_DARK, strrep(' ', w), j, x)
				end
			end
			%disp.display(y, x, h, w)
		end
	end
	t.__ok = "this_is_a_screen_display"
	return t
end

-- Setup an "overlay" mode
function overlay_screen_mode(base, t)
	record_screen_mode(t)

	local reverse = {}
	for i = 1, getn(t) do
		reverse[t[i].name] = i
	end

	local new = {}

	for j = 1, getn(base) do
		local nd = base[j]
		if reverse[nd.name] then
			tinsert(new, t[reverse[nd.name]])
			reverse[nd.name] = nil
		else
			tinsert(new, nd)
		end
	end

	for name, e in reverse do
		tinsert(new, t[e])
	end
	new.__ok = "this_is_a_screen_display"
	return new
end

-- Use a screen mode
__display = {}
function set_screen_mode(t, redraw)
	assert(t.__ok == "this_is_a_screen_display", "set_screen_display passed a non screen mode")
	__display = t

	-- Redraw all now
	if redraw then do_cmd_redraw() end
end


-- Do the actual drawing when need
function __display_draw_hook()
	for i = 1, getn(__display) do
		local disp = __display[i]
		local ok = nil

		for i = 1, getn(disp.flag) do
			if flag_exists(player.redraw, disp.flag[i]) then ok = true break end
		end
		if ok and (not disp.can_display or disp.can_display()) then
			disp.execute()
		end
	end
	flag_empty(player.redraw)
end
hook.add_script(hook.REDRAW, "__display_draw_hook", "__display_draw_hook")
