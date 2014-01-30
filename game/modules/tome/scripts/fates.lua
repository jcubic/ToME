load_subsystem("fates",
{
	dun_level_tolerance = 5,

	get_min_rand_level =
		function()
			return max(player.lev * 2 / 3, 1)
		end,

	get_min_rand_dun_level =
		function()
			return max(player.lev * 2 / 3, 1)
		end,

 	get_max_rand_level =
		function()
			return (player.lev * 4 / 3)
		end,

	get_max_rand_dun_level =
		function()
			return (player.lev * 4 / 3)
		end,

	get_undisplayed_fate_lines =
		function(undisp_fates, lines)
			-- Player doesn't know about hidden fates, don't count them.
			local i = 1
			while i <= getn(undisp_fates) do
				if undisp_fates[i].hidden then
					tremove(undisp_fates, i)
				else
					i = i + 1 
				end
			end

			local num = getn(undisp_fates)

			if num == 0 then
				return
			end

			tinsert(lines, "#vThere " ..
					iif(num == 1, "is a fate", "are " .. num .. " fates") ..
						" of which you have no knowledge.")
			tinsert(lines, "#vYou should see a soothsayer quickly.")
			tinsert(lines, "")
		end,

	get_no_fates_disp_lines = 
		function(lines)
			tinsert(lines, "#yThere are no fates of which you have " ..
					"any knowledge.")
		end
}) -- load_subsystem()

hook(hook.NEW_FATE_POST,
function(fate)
	if not (fate.silent or fate.hidden or fate.learned) then
		local feeling = fate.feeling_gain_msg or
			"#vMore of your prophecy has been unearthed!"
		local sooth = fate.sooth_msg or
			"#vYou should see a soothsayer quickly."

		message(feeling)
		message(sooth)
	end
end)

hook(hook.FATE_EXEC_PRE,
function(fate)
	-- Fates won't come to pass while the player is resting
	if timed_effect.get(timed_effect.RESTING) then
		return true
	end
end)

hook(hook.FATE_EXEC_POST,
function(fate)
	if fate.stale then
		fate.remove = true
	else
		-- If fate isn't stale yet, then we're not done with it.
		return
	end

	if fate.hide_feeling or fates.hide_feelings then
		return
	end

	if (fate.style == fates.styles.ON_ENTER_LEVEL.index or
		fate.style == fates.styles.ON_ENTER_DUN.index)
	then
		local feeling
		if fate.done then
			feeling = fate.feeling_meet_msg or
				"You feel that you will meet your fate here."
		else
			local msg = "You feel that your fate has slipped from " ..
				"your grasp."

			if quality and qualaity < 0 then
					msg = "You feel that you have evaded your fate."
			end
					
			feeling = fate.feeling_no_meet_msg or msg
		end
		message(feeling)
	end
end) -- hook(hook.FATE_REMOVED)
