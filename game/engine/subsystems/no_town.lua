-- The "no town" subsystem
--
-- Load it with: load_subsystem("no_town")
--
-- This simply makes the player start directly in a dungeon.
--
-- Parameters:
-- * dungeon: the dungeon index to start in
-- * level: the dungeon level to start in

-- Starts right out in the dungeon
hook(hook.INIT_DONE, function()
	current_dungeon_idx = find_dungeon(get_subsystem_param("no_town", "dungeon"))
	dun_level = get_subsystem_param("no_town", "level")
	old_dun_level = get_subsystem_param("no_town", "old_level") or old_dun_level
end)
