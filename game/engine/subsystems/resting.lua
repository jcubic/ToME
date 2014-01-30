-- The rest subsystem
--
-- Load it with: load_subsystem("resting")
--
-- Modules may pass a "stop_check" parameter function to the subsystem init
-- to add stop conditions

new_flag("NO_RESTING")

constant("rest", {})

-- Rest until all conditions are met
rest.CHECK_NONE = 0
rest.CHECK_ALL  = 1

--- @fn
-- @brief Redefine to make stop conditions
-- @return Boolean true to stop resting
rest.stop_check = get_subsystem_param("resting", "stop_check") or function() return false end

-- Resting is simply a timed effect
timed_effect.create
{
	name = "RESTING"
	desc = "Resting"
	type = "physical"
	status = "hidden"
	parameters = { {"check", rest.CHECK_NONE} }
	redraw = FLAG_PR_STATE
	on_timeout = function()
		-- Update display sometimes
		local time = timed_effect.get(timed_effect.RESTING)

		if time < 100 or imod(time, 10) == 0 then
			player.redraw[FLAG_PR_STATE] = true
		end

		-- Stop asked by the user
		term.inkey_scan = true
		if term.inkey() ~= 0 then
			-- Stop!
			timed_effect(timed_effect.RESTING, 0)
			return
		end

		-- Should we stop once the player is allright?
		if timed_effect.get("check") == rest.CHECK_ALL then
			if not rest.stop_check() then return end

			-- Stop!
			timed_effect(timed_effect.RESTING, 0)
		end
	end
	on_gain = function()
			player.calc_bonuses(true)
			player.redraw[FLAG_PR_STATE] = true
	end
	on_lose = function()
			player.redraw[FLAG_PR_STATE] = true
			player.calc_bonuses(true)
	end
	bonus = function()
		-- Do not do anything when resting
		player.set_intrinsic(FLAG_PARALYZED, true)
	end
}

function rest.check_no_rest_flags()
	local c_ptr = cave(player.py, player.px)

	return or_msg("You cannot rest.", player.intrinsic_flags[FLAG_NO_RESTING],
				  c_ptr.flags[FLAG_NO_RESTING],
				  f_info(c_ptr.feat).flags[FLAG_NO_RESTING],
				  level_flags.flags[FLAG_NO_RESTING])
end

function rest.rest()
	local str, time, check

	-- Do any NO_RESTING flags prevent the player from resting?
	if rest.check_no_rest_flags() then
		return
	end

	term.save()

	-- Prompt for time if needed
	if command_arg <= 0 then
		-- Ask for duration
		str = get_string("Rest (0-9999, '&' as needed): ", "&")
		if not str then term.load() return end

		-- Rest until done
		if str == '&' then
			time = 65000
			check = rest.CHECK_ALL
		else
			time = tonumber(str)
			check = rest.CHECK_NONE
		end
	else
		time = command_arg
		check = rest.CHECK_NONE
	end

	timed_effect(timed_effect.RESTING, time, check)

	-- Check whether or not to stop after the timed effect has been
	-- set, since resting might change bonuses (like regeneation
	-- rates) which could influence rest.stop_check()
	if rest.check_no_rest_flags() then
		timed_effect(timed_effect.RESTING, 0)
		term.load()
		return
	end

	-- Don't start resting indefinitely if we're going to immediatly stop.
	if str and str == '&' and rest.stop_check() then
		timed_effect(timed_effect.RESTING, 0)
	end

	term.load()
end

-- Init
if not get_subsystem_param("resting", "no_key_bind") then
	hook(hook.KEYPRESS, function (key)
		if key == strbyte('R') then rest.rest() return true end
	end)
end

-- Stop resting when disturbed
hook(hook.DISTURB, function () timed_effect(timed_effect.RESTING, 0) end)
