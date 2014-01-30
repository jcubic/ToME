-- Movement AI: The snakesssss

-- WHAT ? a local not in a function ?
-- This is a trick thanks to lua great lexical scoping:
-- We declare a local in a do ... end block, s o it does not exists outside of it
-- But we use it ina subfunction which can keep on usnig it forever but without
-- having to have the table on the global namespace.
-- Lua rocks doesn't it ? ;)
do
local snake_table =
{
	[-1] =
	{
		[-1] = {{-1, 0} {-1, 0}}
		[ 0] = {{-1,-1} {-1, 1}}
		[ 1] = {{-1, 0} { 1, 0}}
	}
	[ 0] =
	{
		[-1] = {{-1, 1} {-1,-1}}
		[ 0] = {{ 0, 0} { 0, 0}} -- This one should not really happen
		[ 1] = {{-1, 1} { 1, 1}}
	}
	[ 1] =
	{
		[-1] = {{ 1, 0} {-1, 0}}
		[ 0] = {{ 1, 1} {-1, 1}}
		[ 1] = {{ 1, 0} { 1, 0}}
	}
}

-- Snake AI: move toward the target in random directionnal partterns
ai.new
{
	name	= "SNAKE"
	state	=
	{
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
			if dy ~= 0 then dy = dy / abs(dy) end
			if dx ~= 0 then dx = dx / abs(dx) end
			-- Get the random directions table for snake movements
			local dir = %snake_table[dy][dx]

			-- Compute the next coordinates
			monst.ai_move_y = monst.fy + rng.arange(dir[1][1], dir[1][2])
			monst.ai_move_x = monst.fx + rng.arange(dir[2][1], dir[2][2])
		end
		-- We moooooveee(or aaaattttaaaaccckkkkk)
		monst.ai_action = ai.action.MOVE
	end
}
end
