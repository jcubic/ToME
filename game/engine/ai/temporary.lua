-- Do normal things using the given AI, and disappear after a while
ai.new
{
	name	= "TEMPORARY"
	state	=
	{
		TIME_TO_LIVE = 20
		REAL_AI = ai.NONE
	}
	exec	= function(m_idx, monst, state)
		flag_inc(state, FLAG_TIME_TO_LIVE, -1)
		if flag_get(state, FLAG_TIME_TO_LIVE) <= 0 then
                        -- Sorry, time's up
			message(format("%s vanishes.", strcap(monster_desc(monst, 0))))
			delete_monster_idx(m_idx)
			return
		else
			-- Ok do your things
			ai.exec(flag_get(state, FLAG_REAL_AI), m_idx, monst)
		end
	end
}
