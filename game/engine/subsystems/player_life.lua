-- The player life subsystem
--
-- Load it with: load_subsystem("player_life")
--
-- Most modules probably want it, otherwise the player will not
-- have HP, and thus is much harder to kill
--
-- This defines counter.LIFE as the life counter
-- and take_hit(), hp_player()
-- it also adds:
-- player.chp() and player.mhp()
-- Setup the FLAG_REGEN_LIFE intrinsic to the value to regen(1000=1HP per turn, 500=1 HP every 2 turn, ...)

new_flag("REGEN_LIFE")
new_flag("DRAIN_LIFE")
new_flag("REGEN_LIFE_TURN")

hook.new_hook_type("PLAYER_TAKE_HIT")

declare_global_constants {
	"force_die_from_msg",
	"clear_die_from_msg",
}

counter.create
{
	name = "LIFE"
	min = -1
	max = 10
	reset = "max"
	on_decrease = function(value, modif, reason)
		local ret, new_modif = hook.process(hook.PLAYER_TAKE_HIT, value, modif)
		if ret then
			modif = new_modif
		end

		if wizard then message("You take hit for #G"..-modif.."#w damage.") end

		-- Die!
		if value + modif < 0 then
			message(color.RED, "You die.")
			kill_player(reason)
		end
		player.redraw[FLAG_PR_HP] = true
		player.window[FLAG_PW_PLAYER] = true
		return true, modif
	end
	on_increase = function(value, modif, reason)
		if value < counter.max(counter.LIFE) and not counter.state(counter.LIFE, "silent") then
			counter.state(counter.LIFE, "silent", false)

			-- Heal 0-4
			if modif > 0 then
				if modif < 5 then
					message("You feel a little better.");
				-- Heal 5-14
				elseif modif < 15 then
					message("You feel better.");
				-- Heal 15-34
				elseif modif < 35 then
					message("You feel much better.");
				-- Heal 35+
				else
					message("You feel very good.");
				end
			end
		end

		player.redraw[FLAG_PR_HP] = true
		player.window[FLAG_PW_PLAYER] = true
		return true, modif
	end
	auto_regen = function()
		local nb = player.hp_regen_rate()
		if nb ~= 0 then
			local amt = nb / 1000
			local im = imod(abs(nb), 1000)
			if im ~= 0 then
				if ((turn/10) * im) / 1000 > ((turn/10 - 1) * im) / 1000 then
					local sign = nb / abs(nb)
					amt = amt + (1 * sign)
				end
			end

			if amt ~= 0 then
				counter.state(counter.LIFE, "silent", true)

				-- Can't die from HP draining, only be reduced to 0
				-- HP
				local cur = counter.get(counter.LIFE)
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

constant("__die_from_msges", {nil})
function force_die_from_msg(msg)
	tinsert(__die_from_msges, msg)
end

function clear_die_from_msg()
	tremove(__die_from_msges)

	-- Ensure first entry exists and is nil
	__die_from_msges[1] = nil
end

function take_hit(dam, die_from)
	counter.dec(counter.LIFE, dam,
				__die_from_msges[getn(__die_from_msges)] or die_from)
end

function hp_player(heal)
	counter.inc(counter.LIFE, heal)
end

function player.chp(val)
	if val then counter.set(counter.LIFE, val, "setting life") end
	return counter.get(counter.LIFE)
end
function player.mhp(val)
	return counter.max(counter.LIFE, val)
end

function player.hp_regen_rate()
	return player.intrinsic(FLAG_REGEN_LIFE) -
		player.intrinsic(FLAG_DRAIN_LIFE)
end
