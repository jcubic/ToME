hook(hook.INIT, function(newgame)
	if not newgame then
		return
	end

	term.save()
	term.clear()

	if (term.drop_text_left(color.LIGHT_BLUE, "Art thou an adventurer,", 10, 0)) then term.load() return end
	if (term.drop_text_right(color.LIGHT_BLUE, "One who passes through the waterfalls we call danger", 11, -1)) then term.load() return end
	if (term.drop_text_left(color.LIGHT_BLUE, "to find the true nature of the legends beyond them?", 12, 0)) then term.load() return end
	if (term.drop_text_right(color.LIGHT_BLUE, "If this is so, then seeketh me.", 13, -1)) then term.load() return end

	if (term.drop_text_left(color.WHITE, "[Press any key to continue]", 23, -1)) then term.load() return end

	term.putch(0, 0, color.DARK, 32)
	term.inkey_scan = FALSE
	term.inkey()

	term.clear()

	if (term.drop_text_left(color.LIGHT_BLUE, "DarkGod", 8, 0)) then term.load() return end
	if (term.drop_text_right(color.WHITE, "in collaboration with", 9, -1)) then term.load() return end
	if (term.drop_text_left(color.LIGHT_GREEN, "Eru Iluvatar,", 10, 0)) then term.load() return end
	if (term.drop_text_right(color.LIGHT_GREEN, "Manwe", 11, -1)) then term.load() return end
	if (term.drop_text_left(color.WHITE, "and", 12, 0)) then term.load() return end
	if (term.drop_text_right(color.LIGHT_GREEN, "All the T.o.M.E. contributors(see credits.txt)", 13, -1)) then term.load() return end

	if (term.drop_text_left(color.WHITE, "present", 15, 1)) then term.load() return end
	if (term.drop_text_right(color.YELLOW, "T.o.M.E.", 16, 0)) then term.load() return end

	if (term.drop_text_left(color.WHITE, "[Press any key to continue]", 23, -1)) then term.load() return end
	term.putch(0, 0, color.DARK, 32)

	term.inkey_scan = FALSE

	term.inkey()

	term.load()
	return
end)
--[[
hook(hook.GAME_START, function(new)
	if new then
		do_play_cmovie("tome-intro.cmv", false)
	end
end)
]]
