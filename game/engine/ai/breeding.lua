-- Breeding AI add-on : Make at an attempt at breeding, and if it fails, 
-- act according to another AI.

ai.new
{
	name	= "BREEDING"
	state	=
	{
		-- Parameters
		ACTION_AI = ai.NONE

		BREEDING_RATE = 50
		BREEDING_DELAY = 2
	}
	init	= function(monst, state)
		ai.init(monst, flag_get(state, FLAG_ACTION_AI))
	end
	exec	= function(m_idx, monst, state)
		if rng.percent(flag_get(state, FLAG_BREEDING_RATE))
			and flag_get(state, FLAG_BREEDING_DELAY) == 0
			and ai_multiply(m_idx) then
			-- Just bred, do nothing this turn
			monst.ai_action = ai.action.REST
		else
			-- Act normally
			ai.exec(flag_get(state, FLAG_ACTION_AI), m_idx, monst)
		end

		-- Decrement the delay flag
		flag_set(state, FLAG_BREEDING_DELAY, 
			max(0, flag_get(state, FLAG_BREEDING_DELAY) - 1))
	end
}
