-- Movement AI: t's

new_flag("TOWN_HOT_SPOT")

constant("ai_townspeople", {})
ai_townspeople.hotspots = nil

-- On level regen, clear the hotspots
hook(hook.LEVEL_END_GEN, function()
	if ai_townspeople.hotspots then
		local list = ai_townspeople.hotspots
		ai_townspeople.hotspots = nil
		flag_free(list, true)
	end
end)

ai.new
{
	name	= "TOWNSPEOPLE"
	state	=
	{
		PATH = getter.flags{}
		MOVE_RATE = 20
		CURRENT_SPOT = -1
	}
	init	= function(monst, state)
		local list = flag_new()

		if not ai_townspeople.hotspots then
			-- Find hot spots
			local nb = 1
			for i = 2, cur_wid - 2 do
				for j = 2, cur_hgt - 2 do
					if cave(j, i).flags[FLAG_TOWN_HOT_SPOT] then
						flag_set_full(list, nb, FLAG_FLAG_BOTH, j, i)
						nb = nb + 1
					end
				end
			end
			state[FLAG_TOWN_HOT_SPOT] =flag_dup(list)
			ai_townspeople.hotspots = list
		else
			state[FLAG_TOWN_HOT_SPOT] = flag_dup(ai_townspeople.hotspots)
		end
	end
	exec	= function(m_idx, monst, state)
		local range = flag_get(state, FLAG_BEST_RANGE)
		if range == -1 then range = flag_get(state, FLAG_MIN_RANGE) end
		local dist = ai.distance(monst)

		-- Fear and range
		if (monst.monfear > 0) or (monst.hp * 100 / monst.maxhp <= 90 and not has_flag(monst, FLAG_NO_FEAR)) then
			local y, x = ai.target(monst)
			y = monst.fy * 2 - y
			x = monst.fx * 2 - x
			monst.ai_action = ai.action.MOVE
			return
		end

		local spot = flag_get(state, FLAG_CURRENT_SPOT)
		if spot < -1 then
			flag_inc(state, FLAG_CURRENT_SPOT, 1)
			monst.ai_action = ai.action.REST
		elseif spot < 1 then
			flag_set(state, FLAG_CURRENT_SPOT, flag_get_rand(flag_get_flags(state, FLAG_TOWN_HOT_SPOT)))
			spot = flag_get(state, FLAG_CURRENT_SPOT)
		end
		if rng.percent(flag_get(state, FLAG_MOVE_RATE)) or spot < 1 then
			monst.ai_action = ai.action.REST
			return
		end

		local list = flag_get_flags(state, FLAG_TOWN_HOT_SPOT)
		local y, x = flag_get(list, spot), flag_get2(list, spot)

		local path = flag_get_flags(state, FLAG_PATH)
		local next = flag_max_key(path)
		local ret, ty, tx, finish
		if next >= 1 then
			ret = true
			ty, tx = flag_get(path, next), flag_get2(path, next)
			flag_remove(path, next)
		else
			ret, ty, tx, finish = pathfind.get_first(monst, monst.fy, monst.fx, y, x)
			if ret then
				local nb = 1
				finish, ty, tx = pathfind.get_next()
				flag_empty(path)
				nb = 15 + rng(10)
				while not finish and nb >= 1 do
					local ny, nx
					finish, ny, nx = pathfind.get_next()
					flag_set_full(path, nb, FLAG_FLAG_BOTH, ny, nx)
					nb = nb - 1
				end
			end
		end

		if distance(monst.fy, monst.fx, y, x) < 3 then flag_set(state, FLAG_CURRENT_SPOT, -1 - rng(1,10)) end

		if ret then
			monst.ai_action = ai.action.MOVE
			monst.ai_move_y = ty
			monst.ai_move_x = tx
		else
			monst.ai_action = ai.action.REST
		end
	end
}
