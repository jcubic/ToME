-- Need a dummy skill
new_skill{ name = "" desc = "" __index__ = 0 }
new_skill{ define_as = "SKILL_NONE" name = "" desc = "" __index__ = 1 }

-- Define a new skill, it will be handled in combat.lua
new_skill
{
	define_as = "SKILL_COMBAT"
	name = "Combat"
	desc =
	{
		"Increases combat power",
	}
}
-- Define a new skill, it will be handled in calc.lua
new_skill
{
	define_as = "SKILL_DEFENSE"
	name = "Defense"
	desc =
	{
		"Increases defense power",
	}
}

set_skill_tree
{
	["Combat"] = {}
	["Defense"] = {}
}
