-- Combinaision AI: cast spells and move toward target

ai.new
{
	name	= "SPELL_AND_MOVE"
	state	=
	{
		-- Parameters
		MOVE_AI = ai.NONE
		CASTING_AI = ai.NONE
		NO_SPELL_FLEE = 0

		-- State machine flags
		ENOUGH_MANA = true
	}
	init	= function(monst, state)
		ai.init(monst, flag_get(state, FLAG_MOVE_AI))
		ai.init(monst, flag_get(state, FLAG_CASTING_AI))
	end
	exec	= function(m_idx, monst, state)
		if rng.percent(monst.freq_spell) then
			ai.exec(flag_get(state, FLAG_CASTING_AI), m_idx, monst)
			if monst.ai_action == ai.action.OTHER then
				-- We casted a spell, assume we have enough mana
				flag_set(state, FLAG_ENOUGH_MANA, true)
				return
			end
		end
		-- tell the movement AI to flee if needed
		local old_range = flag_get(state, FLAG_BEST_RANGE)
		if monst.ai_action == ai.action.NO_CAST or not flag_exists(state, FLAG_ENOUGH_MANA) then
			-- We did not cast, assume we have no mana
			flag_remove(state, FLAG_ENOUGH_MANA)
			if flag_get(state, FLAG_NO_SPELL_FLEE) == true then flag_set(state, FLAG_BEST_RANGE, 100)
			else flag_set(state, FLAG_BEST_RANGE, 0)
			end
		end
		ai.exec(flag_get(state, FLAG_MOVE_AI), m_idx, monst)
		flag_set(state, FLAG_BEST_RANGE, old_range)
	end
}
