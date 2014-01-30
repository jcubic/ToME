--------------------------------------------------------------------------
------------------------ Monster timed effects ---------------------------
--------------------------------------------------------------------------

-- When cut, people start to bleed, bleeding is bad for their health
-- it may lead to dangerous conditions, even death
monster_effect.create
{
	name = "CUT"
	desc = "Bleeding"
	type = "physical"
	status = "detrimental"
	on_gain = "%s starts to bleed."
	on_lose = "%s stops bleeding."
	on_timeout = function(m_idx, monst, count, power)
		power = min(40, max(1, count / 10)) * power
		monster_take_hit(monster_effect.who(monst, monster_effect.CUT), m_idx, power, " bleeds to death.")
	end
}

-- Poison is nasty,
-- it may lead to dangerous conditions, even death
monster_effect.create
{
	name = "POISON"
	desc = "Poisoned"
	type = "physical"
	status = "detrimental"
	on_gain = "%s is poisoned!"
	on_lose = "%s is no longer poisoned."
	on_timeout = function(m_idx, monst, count, power)
		power = min(40, max(1, count / 10)) * power
		monster_take_hit(monster_effect.who(monst, monster_effect.POISON), m_idx, power, " dies of poison.")
	end
}

-- Stuns make it harder to move around and to cast spells
monster_effect.create
{
	name = "STUN"
	desc = "Stunned"
	type = "physical"
	status = "detrimental"
	on_gain = "%s is stunned."
	on_lose = "%s is no longer stunned."
	on_timeout = function(m_idx, monst, count, power)
		power = min(50, max(1, count / 5)) * min(40, power * 10)
		monst.energy = monst.energy - ((get_monster_energy(monst, SPEED_GLOBAL) * power) / 100)
	end
}

-- Paralyzed monsters have no energy with which to moved
monster_effect.create
{
	name = "PARALYZE"
	desc = "Paralyzed"
	type = "physical"
	status = "detrimental"
	on_gain = "%s is paralyzed."
	on_lose = "%s is no longer paralyzed."
	on_timeout = function(m_idx, monst, count, power)
		monst.energy = min(0 - get_monster_energy(monst, SPEED_GLOBAL),
						   monst.energy)
	end
}

-- Slow is .. really not fast
monster_effect.create
{
	name = "SLOW"
	desc = "Slowed"
	type = "magical"
	status = "detrimental"
	on_gain = "%s is slowed down."
	on_lose = "%s is no longer slwoed down."
	on_timeout = function(m_idx, monst, count, power)
		message("Implement monster slow!")
	end
}

-- Blind make it harder to see things
monster_effect.create
{
	name = "BLIND"
	desc = "Blind"
	type = "physical"
	status = "detrimental"
	on_gain = "%s is blinded!"
	on_lose = "%s can see again."
	on_timeout = function(m_idx, monst, count, power)
		message("Implement monster blind!")
	end
}

-- Confused makes you .. confused
monster_effect.create
{
	name = "CONFUSED"
	desc = "Confused"
	type = "mental"
	status = "detrimental"
	on_gain = "%s is confused!"
	on_lose = "%s is less confused now."
	on_timeout = function(m_idx, monst, count, power)
		message("Implement monster conf!")
	end
}

-- When diseased, people start to suffer, suffering is bad for their health
-- it may lead to dangerous conditions, even death
monster_effect.create
{
	name = "DISEASE"
	desc = "Disease"
	type = "magical"
	status = "detrimental"
	on_gain = "%s gets very sick."
	on_lose = "%s is no longer sick."
	on_timeout = function(m_idx, monst, count, power)
		local full_power = power
		if count > 1 then power = power / count end
		monst.energy = monst.energy - ((get_monster_energy(monst, SPEED_GLOBAL) * power) / 100)
		monster_effect.set_param(monst, monster_effect.DISEASE, full_power - power)
	end
}
