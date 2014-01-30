add_module
{
	name        = "ToME",
	shortname   = "tome",
	version     = { 3, 0, 0 },
	author      = { "DarkGod", "darkgod@t-o-m-e.net" },
	desc        =
	{
		"The Tales of Middle-earth, the standard and official game."
	},

	base_dungeon = 0,

	max_money = 999999999

	max_plev = 100,
	max_exp = 99999999

	allow_skill_raise = function(idx)
		if (skill(idx).value + skill(idx).mod) / skills.STEP_VALUE > player.lev + 4 then
			return format("Cannot raise a skill value above %d until you get more experienced.", player.lev + 4)
		end
	end,
	max_skill_level = 100,
--	max_skill_overage = 8
	skill_per_level = function()
		return 6
	end,
}
