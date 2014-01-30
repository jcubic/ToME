------------------------------------------------------------
----------------------- Combat talents ---------------------
------------------------------------------------------------
new_ability
{
	define_as = "AB_SWIFT_KILL"
	name = "Swift Kill"
	desc =
	{
		"Any attack using melee weapons or your bare hands",
		"takes half as much time if it kills its target."
		"Prereq: Weaponmastery@40 or Barehand@40, Dex@30",
	}
	cost = 12
	allow = function() 
		return (get_skill(SKILL_MASTERY) >= 40 or 
			get_skill(SKILL_HAND) >= 40) and
			player.stat_top[A_DEX] >= 30
	end
}

new_ability
{
	define_as = "AB_MAX_BLOW1"
	name = "Extra Max Blow(1)"
	desc =
	{
		"Increases your max possible blows number by 1",
		"Prereq: Combat@20",
	}
	cost = 7
	skills = { Combat = 20 }
}

new_ability
{
	define_as = "AB_MAX_BLOW2"
	name = "Extra Max Blow(2)"
	desc =
	{
		"Increases your max possible blows number by 1",
		"Prereq: Combat@40, Extra Max Blow(1)",
	}
	cost = 7
	skills = { Combat = 40 }
	abilities = { ["Extra Max Blow(1)"] = true }
}

new_ability
{
	define_as = "AB_TOUCH_DEATH"
	name = "Touch of death"
	desc =
	{
		"Your melee blows can insta-kill, but you only receive 1/3 of the experience",
		"Prereq: Necromancy@100, Combat@80, DEX@50, STR@40",
	}
	cost = 15
	skills = { Necromancy = 100, Combat = 80 }
	stats = { [A_DEX] = 50, [A_STR] = 40 }
	action = { nil, "Activate touch of death" }
}

new_ability
{
	define_as = "AB_FAR_ATTACK"
	name = "Far reaching attack"
	desc =
	{
		"You can attack an enemy one square far using a long polearm.",
		"At high levels of Polearm-mastery skill, you can even hit two enemies at once.",
		"Prereq: Combat@30, Polearm-mastery@30",
	}
	cost = 10
	action = { combat.default_weapon.far_attack_MKEY, "Far reaching attack" }
	skills = { Combat = 30, ["Polearm-mastery"] = 30 }
}

------------------------------------------------------------
------------------------ Misc talents ----------------------
------------------------------------------------------------
new_ability
{
	define_as = "AB_TREE_WALK"
	name = "Tree walking"
	desc =
	{
		"Allows you to walk in dense forest",
		"Prereq: Nature skill@40",
	}
	cost = 7
	skills = { Nature = 40 }
}

new_ability
{
	define_as = "AB_AMMO_CREATION"
	name = "Ammo creation"
	desc =
	{
		"Allows you to create shots from rubble and arrows from skeletons",
		"Prereq: Archery@20",
	}
	cost = 8
	skills = { Archery = 20 }
	action = { nil, "Forge ammo" }
}

new_ability
{
	define_as = "AB_QUIVER"
	name = "Quiver mastery"
	desc =
	{
		"Allows to use a quiver to store ammo and fire from it.",
		"Prereq: Archery@20",
	}
	cost = 10
	skills = { Archery = 20 }
}

new_ability
{
	define_as = "AB_TRAPPING"
	name = "Trapping"
	desc =
	{
		"Ability to set monster traps",
		"Prereq: Disarming@30",
	}
	cost = 10
	action = { nil, "Set trap" }
	skills = { Disarming = 30 }
}

new_ability
{
	define_as = "AB_COMBAT_RIDING"
	name = "Mounted Combat"
	desc =
	{
		"Allows you to fight more effectively when riding.",
		"Prereq: Riding@40",
	}
	cost = 15
	skills = { Riding = 40 }
}

------------------------------------------------------------
----------------------- Magic talents ----------------------
------------------------------------------------------------
new_ability
{
	define_as = "AB_PERFECT_CASTING"
	name = "Perfect casting"
	desc =
	{
		"Allows you to reach 0% failure rate on spells",
		"Prereq: Magic skill@70",
	}
	cost = 6
	skills = { Magic = 70 }
}

new_ability
{
	define_as = "AB_QUICK_MIND"
	name = "Quick mind"
	desc =
	{
		"Allows you to cast spells with a speed bonus of +7",
		"Prereq: Magic skill@90, Perfect casting",
	}
	cost = 20
	skills = { Magic = 90 }
	abilities = { ["Perfect casting"] = true }
}

new_ability
{
	define_as = "AB_UNDEAD_FORM"
	name = "Undead Form"
	desc =
	{
		'Ability to turn into a weak undead being when you "die".',
		"You must then kill enough monsters to absorb enough life energy",
		"to come back to life.",
		"Prereq: Necromancy@60, INT@45",
	}
	cost = 15
	skills = { Necromancy = 60 }
	stats = { [A_INT] = 45 }
}

-- Mana skill abilities
new_ability
{
	define_as = "AB_IMP_MANA_SHIELD1"
	name = "Improved Disruption Shield(1)"
	desc =
	{
		"Lowers the mana per hp ratio of the disruption shield to 1.5(instead of 2)",
		"Prereq: Mana@90 or Sorcery@90",
	}
	cost = 6
	allow = function() return get_skill(SKILL_MANA) >= 90 or get_skill(SKILL_SORCERY) >= 90 end
}

new_ability
{
	define_as = "AB_IMP_MANA_SHIELD2"
	name = "Improved Disruption Shield(2)"
	desc =
	{
		"Lowers the mana per hp ratio of the disruption shield to 1(instead of 2)",
		"Prereq: Mana@50 or Sorcery@50, Improved Disruption Shield(1)",
	}
	cost = 6
	allow = function() return get_skill(SKILL_MANA) >= 100 or get_skill(SKILL_SORCERY) >= 100 end
	abilities = { ["Improved Disruption Shield(1)"] = true }
}

-- The special mimicry forms related to high magic levels
new_ability
{
	define_as = "AB_FIRE_SHAPE"
	name = "High Magic Mimicry(Fire Form)"
	desc =
	{
		"Ability to shift into a Fire elemental.",
		"Prereq: Mimicry@70, Fire@100",
	}
	skills = { Fire = 100 Mimicry = 70 }
	cost = 10
}
new_ability
{
	define_as = "AB_AIR_SHAPE"
	name = "High Magic Mimicry(Wind Form)"
	desc =
	{
		"Ability to shift into a Wind elemental.",
		"Prereq: Mimicry@70, Air@100",
	}
	skills = { Air = 100 Mimicry = 70 }
	cost = 10
}
new_ability
{
	define_as = "AB_WATER_SHAPE"
	name = "High Magic Mimicry(Water Form)"
	desc =
	{
		"Ability to shift into a Water elemental.",
		"Prereq: Mimicry@70, Water@100",
	}
	skills = { Water = 100 Mimicry = 70 }
	cost = 10
}
new_ability
{
	define_as = "AB_EARTH_SHAPE"
	name = "High Magic Mimicry(Stone Form)"
	desc =
	{
		"Ability to shift into a Stone elemental.",
		"Prereq: Mimicry@70, Stone@100",
	}
	skills = { Earth = 100 Mimicry = 70 }
	cost = 10
}

new_ability
{
	define_as = "AB_ARCANE_LANCE"
	name = "Arcane Lance"
	desc =
	{
		"Manathrust can beam 50% of the time",
		"Prereq: Mana@50 or Sorcery@50",
	}
	cost = 20
	allow = function() return get_skill(SKILL_MANA) >= 50 or get_skill(SKILL_SORCERY) >= 50 end
}

------------------------------------------------------------
----------------------- Racial talents ---------------------
------------------------------------------------------------

-- Beornings
-- Hidden for non-beornings
new_ability
{
	define_as = "AB_BEORNING"
	name = "Bear-form"
	desc =
	{
		"Ability to turn into a powerful bear form at will.",
		"Prereq: Beorning race",
	}
	action = { nil, "Bear-form" }
	cost = 1000
}


------------------------------------------------------------
----------------------- Tactics talents --------------------
------------------------------------------------------------
new_ability
{
	define_as = "AB_BERSERK_MODE"
	name = "Berserk"
	desc =
	{
		"Learns a new movement mode in which your melee damage is increased but your",
		"searching, sneaking and armour are greatly reduced.",
		"Prereq: Combat@30",
	}
	cost = 7
	skills = { Combat = 30 }
}

------------------------------------------------------------
----------------------- Misc talents------------------------
------------------------------------------------------------
new_ability
{
	define_as = "AB_INCARNATE"
	name = "Incarnate"
	desc =
	{
		"Incarnate into a body."
	}
	action = {nil, "Incarnate"}
	cost = 1000
	hidden = true
}
