-- Do nothing, unless target is close, then attack
ai.new
{
	name	= "NEVER_MOVE"
	exec	= function(m_idx, monst, state)
		local y, x = ai.target(monst)
		if not ai.adjacent(monst, y, x) then
			-- Do nothing
			monst.ai_action = ai.action.REST
		else
			-- Attack
			monst.ai_action = ai.action.MOVE
			monst.ai_move_y = y
			monst.ai_move_x = x
		end
	end
}
