-- Time passes faster in the wilderness
hook(hook.WILD_SMALL_MOVE_POST, function()
--	turn = turn + time.HOUR * 11 * 10
end)

---------------------------------------------------------------------
-- Forbid wilderness levels that are dungeon entries
---------------------------------------------------------------------
hook(hook.ENTERED_DUNGEON, function(type)
	if type == DUNGEON_WILDERNESS then
		if wild_map(player.wilderness_y, player.wilderness_x).entrance > 1000 then
			if not player.wild_mode then change_wild_mode() end
		end
	end
end)
