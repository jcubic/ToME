--
-- The temporary terrain subsystem
--
-- Load it with: load_subsystem("temporary_terrain")
--
-- Parameters:

constant("temporary_terrain", {})

new_flag("TEMP_TERRAIN_ID")
new_flag("TEMP_TERRAIN_TIMEOUT")
new_flag("TEMP_TERRAIN_OLD_FEAT")

temporary_terrain.list = {}
temporary_terrain.__next = 1
add_loadsave("temporary_terrain.list", {})
add_loadsave("temporary_terrain.__next", 1)

local real_cave_set_feat = cave_set_feat
cave_set_feat = function(y, x, feat)
	if cave(y, x).flags[FLAG_TEMP_TERRAIN_ID] then
		local id = cave(y, x).flags[FLAG_TEMP_TERRAIN_ID]
		temporary_terrain.list[id] = nil
		cave(y, x).flags[FLAG_TEMP_TERRAIN_ID] = nil
		cave(y, x).flags[FLAG_TEMP_TERRAIN_TIMEOUT] = nil
		cave(y, x).flags[FLAG_TEMP_TERRAIN_OLD_FEAT] = nil
	end
	%real_cave_set_feat(y, x, feat)
end

function temporary_terrain.set(y, x, feat, time, turn_into)
	local flags = cave(y, x).flags
	local old_feat = flags[FLAG_TEMP_TERRAIN_OLD_FEAT] or cave(y, x).feat
	cave_set_feat(y, x, feat)
	flags[FLAG_TEMP_TERRAIN_ID] = temporary_terrain.__next
	flags[FLAG_TEMP_TERRAIN_OLD_FEAT] = turn_into or old_feat
	flags[FLAG_TEMP_TERRAIN_TIMEOUT] = time

	temporary_terrain.list[temporary_terrain.__next] = { y = y x = x }
	temporary_terrain.__next = temporary_terrain.__next + 1
end

function temporary_terrain.decay(time)
	local remove = {}
	for id, data in temporary_terrain.list do
		local flags = cave(data.y, data.x).flags
		flag_inc(flags, FLAG_TEMP_TERRAIN_TIMEOUT, -time)
		if flag_get(flags, FLAG_TEMP_TERRAIN_TIMEOUT) <= 0 then
			tinsert(remove, id)
			%real_cave_set_feat(data.y, data.x, flags[FLAG_TEMP_TERRAIN_OLD_FEAT])
			flags[FLAG_TEMP_TERRAIN_ID] = nil
			flags[FLAG_TEMP_TERRAIN_TIMEOUT] = nil
			flags[FLAG_TEMP_TERRAIN_OLD_FEAT] = nil
		end
	end
	for id in remove do
		temporary_terrain.list[id] = nil
	end
end

hook
{
	[hook.PROCESS_WORLD_NON_WILD] = function()
		temporary_terrain.decay(1)
	end
	[hook.GEN_LEVEL_BEGIN] = function()
		temporary_terrain.list = {}
		temporary_terrain.__next = 1
	end
	-- Recover saved levels
	[hook.GEN_LEVEL_AFTER_LOAD] = function(loaded)
		if loaded then
			for j = 1, cur_hgt - 2 do
				for i = 1, cur_wid - 2 do
					if cave(j, i).flags[FLAG_TEMP_TERRAIN_ID] then
						temporary_terrain.list[cave(j, i).flags[FLAG_TEMP_TERRAIN_ID]] = {y = j x = i}
					end
				end
			end
		end
	end
}
