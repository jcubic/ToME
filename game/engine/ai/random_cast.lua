-- Casting AI: The dumb random cast

ai.new
{
	name	= "RANDOM_CAST"
	exec	= function(m_idx, monst, state)
		local y, x = ai.target(monst)
		local spl = flag_get_rand(monst.spells)

		local ret = ai.action.NO_CAST
		if spl > 0 then
			ai.cast.use_spell(m_idx, monst, spl, y, x)
			ret = ai.cast.can(true)
		end

		if not ret then
			monst.ai_action = ai.cast.cast()
		else
			monst.ai_action = ret
		end
	end
}
