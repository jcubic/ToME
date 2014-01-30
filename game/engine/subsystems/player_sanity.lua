-- The player sanity subsystem
--
-- Load it with: load_subsystem("player_sanity")
--
-- This defines counter.SANITY as the sanity counter
-- and take_sanity_hit(), hp_insanity()
-- it also adds:
-- player.csane() and player.msane()

declare_global_constants
{
	"take_sanity_hit",
	"hp_insanity",
}


hook.new_hook_type("PLAYER_TAKE_SANITY_HIT")
counter.create
{
	name = "SANITY"
	min = -1
	max = 10
	reset = "max"
	on_decrease = function(value, modif, reason)
		local ret, new_modif = hook.process(hook.PLAYER_TAKE_SANITY_HIT, value, modif)
		if ret then
			modif = new_modif
		end

		-- Die!
		if value + modif < 0 then
			message(color.RED, "You die.")
			kill_player(reason)
		end
		player.redraw[FLAG_PR_SANITY] = true
		player.window[FLAG_PW_PLAYER] = true
		return true, modif
	end
	on_increase = function(value, modif, reason)
		if value < counter.max(counter.SANITY) then
			-- Heal 0-4
			if modif < 5 then
				message("Your mind feels a little better.");
			-- Heal 5-14
			elseif modif < 15 then
				message("Your mind feels better.");
			-- Heal 15-34
			elseif modif < 35 then
				message("Your mind feels much better.");
			-- Heal 35+
			else
				message("Your mind feels very good.");
			end
		end

		player.redraw[FLAG_PR_SANITY] = true
		player.window[FLAG_PW_PLAYER] = true
		return true, modif
	end
}

function take_sanity_hit(dam, die_from)
	counter.dec(counter.SANITY, dam, die_from)
end

function hp_insanity(heal)
	counter.inc(counter.SANITY, heal)
end

function player.csane(val)
	if val then counter.set(counter.SANITY, val, "setting sanity") end
	return counter.get(counter.SANITY)
end
function player.msane(val)
	return counter.max(counter.SANITY, val)
end
