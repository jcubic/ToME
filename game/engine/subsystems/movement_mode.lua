-- The movement modes subsystem
--
-- Load it with: load_subsystem("movement_mode")
--
-- This subsystem allows to define movement modes that the player can switch at will

constant("mov_mode", {})

mov_mode.__list = {}

player.movement_mode = 1
add_loadsave("player.movement_mode", 1)
hook(hook.BIRTH, function() player.movement_mode = 1 end)

function mov_mode.new(t)
	assert(t.name, "No movement mode name")
	assert(t.desc, "No movement mode desc")
	tinsert(mov_mode.__list, t)
	t.__index__ = getn(mov_mode.__list)
end

function mov_mode.get_info(mode, info)
	if mov_mode.__list[mode] then return mov_mode.__list[mode][info] else return nil end
end

function mov_mode.current()
	return mov_mode.__list[player.movement_mode].name
end

function mov_mode.select()
	-- Make a list
	local list = {}
	for k, e in mov_mode.__list do
		if tag(e) == TAG_TABLE then
			if not e.allow or e.allow() then
				list[k] = e
			end
		end
	end

	local act = choose_from_list(
		list,
		"Movement modes",
		"Select which mode?",
		function(y, x, a)
			if a then term.blank_print(a.name..iif(a.__index__ == player.movement_mode, " [current]", ""), y, x) end
		end,
		function(y, a)
			if a then
				for i = 1, getn(a.desc) do
					-- Use text_out so that ToME does the line wrapping for us
					term.text_out(color.YELLOW, a.desc[i].." ")
				end
			end
		end
	)

	if act then
		player.movement_mode = act.__index__
		if act.update then act.update() end
		energy_use = get_player_energy(SPEED_GLOBAL)
	end
end

hook(hook.CALC_BONUS_END, function()
	if mov_mode.__list[player.movement_mode] and mov_mode.__list[player.movement_mode].bonus then
		mov_mode.__list[player.movement_mode].bonus()
	end
end)

if not get_subsystem_param("movement_mode", "no_key_bind") then
	hook(hook.KEYPRESS, function (key)
		if key == strbyte('S') then mov_mode.select() return true end
	end)
end

-- Load modes
tome_dofile_anywhere(TENGINE_DIR_DATA, "player/movement_mode.lua")
