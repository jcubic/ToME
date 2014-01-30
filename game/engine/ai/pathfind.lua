-- Movement AI: Pathfind to your destination
-- Do *NOT* use this too heavily or performances *will* crumble
-- Beware that it is very unforgiving and it WILL find the player

ai.new
{
	name	= "PATHFIND"
	state	=
	{
		MIN_RANGE = -1
		BEST_RANGE = -1
		FEAR_LEVEL = 10

		PATH = getter.flags{}
		CURRENT_SPOT = -1
	}
	exec	= function(m_idx, monst, state)
		local range = flag_get(state, FLAG_BEST_RANGE)
		if range == -1 then range = flag_get(state, FLAG_MIN_RANGE) end
		local y, x = ai.target(monst)
		local dist = ai.distance(monst)

		-- Fear and range
		if (monst.monfear > 0) or (monst.hp * 100 / monst.maxhp <= flag_get(state, FLAG_FEAR_LEVEL) and not has_flag(monst, FLAG_NO_FEAR)) then
			if has_flag(monst, FLAG_SMART) then
				local ok, y2, x2 = find_safety(monst)
				if ok then
					y = y2
					x = x2
				else
					y = monst.fy * 2 - y
					x = monst.fx * 2 - x
				end
			else
				y = monst.fy * 2 - y
				x = monst.fx * 2 - x
			end
		end

		-- Ok first, if we are in LOS, just run for it(or get some range)
		if los(monst.fy, monst.fx, y, x) then
			if range > -1 and dist <= range then
				monst.ai_action = ai.action.MOVE
				monst.ai_move_y = monst.fy * 2 - y
				monst.ai_move_x = monst.fx * 2 - x
			else
				monst.ai_action = ai.action.MOVE
				monst.ai_move_y = y
				monst.ai_move_x = x
			end
			return
		end

		-- We do not have LOS, let's be smart!
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
				nb = 5 + rng(5)
				while not finish and nb >= 1 do
					local ny, nx
					finish, ny, nx = pathfind.get_next()
					flag_set_full(path, nb, FLAG_FLAG_BOTH, ny, nx)
					nb = nb - 1
				end
			end
		end

		if ret then
			monst.ai_action = ai.action.MOVE
			monst.ai_move_y = ty
			monst.ai_move_x = tx
		else
			monst.ai_action = ai.action.REST
		end
	end
}
