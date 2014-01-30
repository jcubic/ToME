-- ToME custom AIs

ai.load("townspeople")

ai.load("smart_cast")


-- Redefine some AI stuff for ToME specifics
new_flag("LURE_TARGET")

ai.simple_target = ai.target
function ai.target(m)
	local ty, tx = ai.simple_target(m)
	if flag_exists(m.ai_states, FLAG_LURE_TARGET) and ai.target_enemy(m) then
		local ret, y, x
		local room = 0

		-- Count room grids next to player
		for i = 1, 9 do
			-- Check grid
			if cave(ty + ddy_ddd[i], tx + ddx_ddd[i]).info & CAVE_ROOM ~= 0 then
				-- One more room grid
				room = room + 1
			end
		end

		-- Not in a room and strong player
		if room < 8 and player.chp() > ((player.mhp() * 3) / 4) then
			-- Find hiding place
			ret, y, x = find_hiding(m)
			if ret then return y, x end
		end
	end

	-- Return the normal target by default
	return ty, tx
end

