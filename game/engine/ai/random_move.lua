-- Movement AI: The zombie

-- Very basic "zombie-like" AI: move toward the target, and hit it.
ai.new
{
	name	= "RANDOM_MOVE"
	state	=
	{
		MOVE_RATE = 50
		MIN_RANGE = -1
		BEST_RANGE = -1
		FEAR_LEVEL = 10
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
		else
			if range > -1 and dist <= range and los(monst.fy, monst.fx, y, x) then
				y = monst.fy * 2 - y
				x = monst.fx * 2 - x
			end
		end

		-- Compute the quadrant in which the destination is
		local dy, dx = y - monst.fy, x - monst.fx

		-- If we are adjacent we just hit the target, we may move semi randomly but we know when to strike!
		if dy >= -1 and dy <= 1 and dx >= -1 and dx <= 1 then
			monst.ai_move_y = y
			monst.ai_move_x = x
		else
			if rng.percent(flag_get(state, FLAG_MOVE_RATE)) then
				y = monst.fy + rng(-1, 1)
				x = monst.fx + rng(-1, 1)
			end

			-- Compute the next coordinates
			monst.ai_move_y = y
			monst.ai_move_x = x
		end
		-- We moooooveee(or aaaattttaaaaccckkkkk)
		monst.ai_action = ai.action.MOVE
	end
}
