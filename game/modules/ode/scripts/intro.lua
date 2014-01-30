declare_global_constants {
	"drop_text_left",
	"drop_text_right",
}

function drop_text_left(c, str, y, o)
	local i = strlen(str)
	local x = 39 - (strlen(str) / 2) + o
	while (i > 0)
	do
		local a = 0
		local time = 0

		if (strbyte(str, i) ~= strbyte(" ", 1)) then
			while (a < x + i - 1)
			do
				term.putch(a - 1, y, c, 32)
				term.putch(a, y, c, strbyte(str, i))
				time = time + 1
				if time >= 4 then
					term.xtra(TERM_XTRA_DELAY, 1)
					time = 0
				end
				term.redraw_section(a - 1, y, a, y)
				a = a + 1

				term.inkey_scan = true
				if (term.inkey() ~= 0) then
					return true
				end
			end
		end

		i = i - 1
	end
	return false
end

function drop_text_right(c, str, y, o)
	local x = 39 - (strlen(str) / 2) + o
	local i = 1
	while (i <= strlen(str))
	do
		local a = 79
		local time = 0

		if (strbyte(str, i) ~= strbyte(" ", 1)) then
			while (a >= x + i - 1)
			do
				term.putch(a + 1, y, c, 32)
				term.putch(a, y, c, strbyte(str, i))
				time = time + 1
				if time >= 4 then
					term.xtra(TERM_XTRA_DELAY, 1)
					time = 0
				end
				term.redraw_section(a, y, a + 1, y)
				a = a - 1

				term.inkey_scan = true
				if (term.inkey() ~= 0) then
					return true
				end
			end
		end

		i = i + 1
	end
	return false
end

hook(hook.INIT, function()
	term.save()
	term.clear()

	if (true == drop_text_left(color.LIGHT_BLUE, "Art thou an adventurer,", 10, 0)) then term.load() return end
	if (true == drop_text_right(color.LIGHT_BLUE, "One who passes through the waterfalls we call danger", 11, -1)) then term.load() return end
	if (true == drop_text_left(color.LIGHT_BLUE, "to find the true nature of the legends beyond them?", 12, 0)) then term.load() return end
	if (true == drop_text_right(color.LIGHT_BLUE, "If this is so, then seeketh me.", 13, -1)) then term.load() return end

	if (true == drop_text_left(color.WHITE, "[Press any key to continue]", 23, -1)) then term.load() return end

	term.putch(0, 0, color.DARK, 32)
	term.inkey_scan = false
	term.inkey()

	term.clear()

	if (true == drop_text_left(color.LIGHT_BLUE, "DarkGod", 8, 0)) then term.load() return end
	if (true == drop_text_right(color.WHITE, "in collaboration with", 9, -1)) then term.load() return end
	if (true == drop_text_left(color.LIGHT_GREEN, "Eru Iluvatar,", 10, 0)) then term.load() return end
	if (true == drop_text_right(color.LIGHT_GREEN, "Manwe", 11, -1)) then term.load() return end
	if (true == drop_text_left(color.WHITE, "and", 12, 0)) then term.load() return end
	if (true == drop_text_right(color.LIGHT_GREEN, "All the T.o.M.E. contributors(see credits.txt)", 13, -1)) then term.load() return end

	if (true == drop_text_left(color.WHITE, "present", 15, 1)) then term.load() return end
	if (true == drop_text_right(color.YELLOW, "O.D.E.", 18, 0)) then term.load() return end

	if (true == drop_text_left(color.WHITE, "[Press any key to continue]", 23, -1)) then term.load() return end
	term.putch(0, 0, color.DARK, 32)

	term.inkey_scan = false

	term.inkey()

	term.load()
	return
end)
