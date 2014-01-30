-- The "no wilderness map" subsystem
--
-- Load it with: load_subsystem("no_wilderness_map")
--
-- This simply makes the player unable to use the wilderness map
--
-- Parameter(optionnal)
-- * message: A message to display when a attempt is made

hook(hook.KEYPRESS, function (key)
	if dun_level == 0 and key == strbyte('<') then
		if get_subsystem_param("no_wilderness_map", "message") then
			message(get_subsystem_param("no_wilderness_map", "message"))
		end
		return true, true
	end
end)
