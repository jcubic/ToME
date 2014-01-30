-- The player mana subsystem
--
-- Load it with: load_subsystem("player_mana")
--
-- This defines counter.MANA as the mana counter
-- it also adds:
-- player.csp() and player.msp()
--
-- Setup the FLAG_REGEN_MANA intrinsic to the value to regen(1000=1mana per turn, 500=1mana every 2 turn, ...)

declare_global_constants
{
	"increase_mana",
}

new_flag("REGEN_MANA")
new_flag("DRAIN_MANA")

counter.create
{
	name = "MANA"
	min = 0
	max = 10
	reset = "max"
	on_decrease = function(value, modif, reason)
		player.redraw[FLAG_PR_MANA] = true
		player.window[FLAG_PW_PLAYER] = true
	end
	on_increase = function(value, modif, reason)
		player.redraw[FLAG_PR_MANA] = true
		player.window[FLAG_PW_PLAYER] = true
	end
	auto_regen = function()
		local nb = player.mana_regen_rate()
		if nb ~= 0 then
			local amt  = nb / 1000
			local im   = imod(abs(nb), 1000)

			if im ~= 0 then
				if ((turn/10) * im) / 1000 > ((turn/10 - 1) * im) / 1000 then
					local sign = nb / abs(nb)
					amt = amt + (1 * sign)
				end

			end

			if amt ~= 0 then
				-- Ensure mana doesn't go negative
				local cur = counter.get(counter.MANA)
				if cur + amt < 0 then
					amt = -cur
				end

				if cur == -amt and cur > 0 then
					term.disturb(0, 0)
				end					

				return amt
			end
		end
	end
}

function player.csp(val)
	if val then counter.set(counter.MANA, val, "setting mana") end
	return counter.get(counter.MANA)
end
function player.msp(val)
	return counter.max(counter.MANA, val)
end

function player.mana_regen_rate()
	return player.intrinsic(FLAG_REGEN_MANA) -
		player.intrinsic(FLAG_DRAIN_MANA)
end

-- modify mana
-- returns true if there is a pb
function increase_mana(amt)
	counter.inc(counter.MANA, amt)
	return false
end
