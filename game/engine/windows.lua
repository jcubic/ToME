-- Windows handling

constant("windows", {})

function windows.configure()
	local save = term.save_to()
	local sel, wsel = 1, 1
	-- Yes yes I know ... ugly
	local names = { "Term-1", "Term-2", "Term-3", "Term-4", "Term-5", "Term-6", "Term-7", }
	local screen_name = current_module.name .. " Windows settings"
	local ret, wid, hgt = term.get_size()

	while true do
		term.clear()
		term.print(screen_name, 0, (wid - strlen(screen_name)) / 2)

		for j = 1, term.MAX_WINDOW - 1 do
			local a = (j == wsel) and color.LIGHT_GREEN or color.WHITE
			term.print(a, names[j], 2 + imod((j - 1), 2), 25 + j * 5 - strlen(names[j]) / 2)
		end

		for i = 1, getn(windows.__types) do
			local type = windows.__types[i]
			local line_a = (i == sel) and color.LIGHT_BLUE or color.WHITE
			term.print(line_a, type.name, i + 4, 0)

			for j = 1, term.MAX_WINDOW - 1 do
				local c = term.window_settings(j)[type.flag] and 'X' or '.'
				local a = (j == wsel) and color.LIGHT_GREEN or line_a

				if j == wsel and i == sel then
					a = color.YELLOW
					term.print(a, '['..c..']', i + 4, j * 5 + 25 - 1)
				else
					term.print(a, c, i + 4, j * 5 + 25)
				end
			end
		end

		local raw = term.inkey()
		local key = strchar(raw)

		if raw == ESCAPE then break
		elseif key == '2' then
			sel = sel + 1
			if sel > getn(windows.__types) then sel = 1 end
		elseif key == '8' then
			sel = sel - 1
			if sel < 1 then sel = getn(windows.__types) end
		elseif key == '6' then
			wsel = wsel + 1
			if wsel > term.MAX_WINDOW - 1 then wsel = 1 end
		elseif key == '4' then
			wsel = wsel - 1
			if wsel < 1 then wsel = term.MAX_WINDOW - 1 end
		else
			term.window_settings(wsel)[windows.__types[sel].flag] = not term.window_settings(wsel)[windows.__types[sel].flag]
		end
	end

	term.load_from(save, true)
end

windows.__types = {}

function windows.type_define(t)
	assert(t.flag, "No window type flag")
	assert(t.name, "No window type name")

	tinsert(windows.__types, t)
end

function windows.display()
	for type in windows.__types do
		if type.display and (player.window[type.flag] or player.window[FLAG_PW_ALL]) then
			for i = 1, term.MAX_WINDOW - 1 do
				if term.window_settings(i)[type.flag] then
					if term.set_current_window(i) then
						type.display()
						term.fresh()
					end
				end
			end
		end
	end
	term.set_current_window(0)
end

windows.type_define
{
	flag = FLAG_PW_MESSAGE
	name = "Message recall"
}
windows.type_define
{
	flag = FLAG_PW_MONSTER
	name = "Monster recall"
}
windows.type_define
{
	flag = FLAG_PW_M_LIST
	name = "Visible monsters"
}
