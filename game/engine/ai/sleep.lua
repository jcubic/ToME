-- Do nothing, have a chance to wakeup
-- NOTE: Modules may redefine the sleep AI to do their own wakeup things
ai.new
{
	name	= "SLEEP"
	state	=
	{
		SLEEP_NEXT_AI = ai.NONE
	}
	exec	= function(m_idx, monst, state)
		-- Ok we are done, swicth AI and execute right now
		if monst.csleep <= 0 then
			ai.exec(flag_get(state, FLAG_SLEEP_NEXT_AI), m_idx, monst)
			set_monster_ai(monst, flag_get(state, FLAG_SLEEP_NEXT_AI))
			return
		end

		-- Ok do we "want" to wake up?
		local notice = 0

		-- handle non-aggravation
		if not player.has_intrinsic(FLAG_AGGRAVATE) then notice = rng(0, 1023) end

		-- See if monster "notices" player
		if (notice * notice * notice) <= lshift(1, 30 - player.skill_stl) then
			-- amount of "waking"
			local d = 1

			-- Wake up faster near the player
			if monst.cdis < 50 then d = 100 / monst.cdis end

			-- Hack -- handle aggravation
			if player.has_intrinsic(FLAG_AGGRAVATE) then d = monst.csleep end

			-- Still asleep
			if monst.csleep > d then
				-- Monster wakes up "a little bit"
				monst.csleep = monst.csleep - d
			-- Just woke up
			else
				-- Reset sleep counter
				monst.csleep = 0

				-- Notice the "waking up"
				if monst.ml then
					-- Acquire the monster name
					local m_name = monster_desc(monst, 0)

					-- Dump a message
					message(strcap(m_name).." wakes up.")
				end
			end
		end

		monst.ai_action = ai.action.REST
	end
}
