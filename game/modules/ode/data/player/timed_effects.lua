-- When invisibile, monsters have an harder time seeing you
-- But "wraith" like monsters(invisibile ones) see you easily
-- and you are more vulnerable to them
timed_effect.create
{
	name = "INVISIBILITY"
	parameters = { {"power",0} }
	on_gain = "Your body fades away!"
	on_lose = "Your body is once again visible."
	bonus = function()
		player.inc_intrinsic(FLAG_INVIS, timed_effect.get("power"))
	end
}

-- Allows you to see invisible things
timed_effect.create
{
	name = "SEE_INVISIBILITY"
	parameters = {}
	on_gain = "Your eyes seem much more perceptive."
	on_lose = "Your eyes get back to their normal sensory level."
	bonus = function()
		player.set_intrinsic(FLAG_SEE_INVIS, true)
	end
}

-- Fast !!
timed_effect.create
{
	name = "SPEED"
	parameters = { {"power",0} }
	on_gain = "You feel faster."
	on_lose = "You feel slower."
	bonus = function()
		player.inc_speed(timed_effect.get("power"))
	end
}
