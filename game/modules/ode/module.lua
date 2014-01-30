add_module
{
	name        = "ODE",
	shortname   = "ode",
	version     = { 1, 0, 0 },
	author      = { "DarkGod", "darkgod@t-o-m-e.net" },
	desc        =
	{
		"One Day (roguelike) Example",
	},

	base_dungeon = 1

	max_plev = 6,
	max_skill_level = 5,
	skill_per_level = function()
		if player.lev < 6 then
			return 1
		else
			return 2
		end
	end,
}
