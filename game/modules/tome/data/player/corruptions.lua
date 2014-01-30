-- Random teleportation will ask for confirmation 70% of the time
-- But 30% of the time it will teleport, without asking
corruption.RANDOM_TELEPORT = corruption.new
{
	color       = color.GREEN
	name	    = "Random teleportation"
	get_text    = "Space seems to fizzle around you."
	lose_text   = "Space solidify again around you."
	desc	    =
	{
		"Randomly teleports you around.",
	}
	-- No oppose field, it will be automatically set when we declare the anti-telep corruption to oppose us
	hooks       =
	{
		[hook.MOVE_PRE] = function(y, x, dir)
			if rng.number(300) == 1 then
				if rng.percent(70) then
					if get_check("Teleport?") then
						teleport_player_directed(70, dir)
					end
				else
					term.disturb(0, 0)
					message("Your corruption takes over you, you teleport!")
					teleport_player_directed(70, dir)
				end
				return true
			end
		end
	}
}
