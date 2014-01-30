--
-- Utility stuff for quests
--

new_flag("NO_NEW_QUESTS")

declare_global_constants {
	"no_new_quests"
}

function no_new_quests()
	-- Check dungeon flags, because level_flags won't be set up
	-- if GAME_START is called from a new game
	if has_flag(d_info(current_dungeon_idx), FLAG_NO_NEW_QUESTS) then
		return true
	end

	-- Also check level flags, since level flags can be changed by
	-- a particular level of a dungeon, or be set by other code as
	-- a flag that will go away when the level does.
	if has_flag(level_flags, FLAG_NO_NEW_QUESTS) then
		return true
	end

	return false
end
