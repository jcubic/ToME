-- handle the nature school

declare_global_constants {
	"GROWTREE",
	"HEALING",
	"RECOVERY",
	"REGENERATION",
	"SUMMONANNIMAL",
	"grow_trees",
}

GROWTREE = add_spell
{
	["name"] = 	"Grow Trees",
	["school"] = 	{SCHOOL_NATURE, SCHOOL_TEMPORAL},
	["level"] = 	11,
	["mana"] = 	6,
	["mana_max"] = 	30,
	["fail"] = 	35,
	["inertia"] = 	{ 5, 50 },
    ["action"] = "@Source@ causes trees to spourt up " ..
		"around @source_reflexive@.",
	["no_target"] = true,
	["tactical"] =
	{
		[ai.tactics.NO_LOS] = true
		[ai.tactics.ESCAPE] = 10
	}
	["spell"] = 	function()
			local y, x = get_spell_caster_info()
			grow_trees(y, x, 2 + get_cast_level(7))
			return true
	end,
	["info"] = 	function()
			return "rad "..(2 + get_cast_level(7))
	end,
	["desc"] =	{
			"Makes trees grow extremely quickly around you",
	}
}

HEALING = add_spell
{
	["name"] = 	"Healing",
	["school"] = 	{SCHOOL_NATURE},
	["level"] = 	19,
	["mana"] = 	15,
	["mana_max"] = 	50,
	["fail"] = 	45,
	["stick"] =
	{
			["charge"] =    { 2, 3 },
			[TV_ORB] =
			{
				["rarity"] = 		90,
				["base_level"] =	{ 1, 5 },
				["max_level"] =		{ 20, 40 },
			},
	},
    ["action"] = "@Source@ casts @spell@.",
	["no_target"] = true,
	["tactical"] =
	{
		[ai.tactics.NO_LOS] = true
		[ai.tactics.HEAL] = 10
	}
	["spell"] = 	function()
			-- Player
			if spell_caster == 0 then
				return hp_player(player.mhp() * (15 + get_cast_level(35)) / 100)
			else -- Monster
				local monst = monster(spell_caster)
				local val = monst.maxhp * (15 + get_cast_level(35)) / 100

				-- Heal
				monst.hp = monst.hp + val

				-- No overflow
				if monst.hp > monst.maxhp then monst.hp = monst.maxhp end

				-- Redraw
				flag_set(player.redraw, FLAG_PR_HEALTH,1)
			end
	end,
	["info"] = 	function()
			return "heal "..(15 + get_cast_level(35)).."% = "..(player.mhp() * (15 + get_cast_level(35)) / 100).."hp"
	end,
	["desc"] =	{
			"Heals a percent of hitpoints",
	}
}

RECOVERY = add_spell
{
	["name"] = 	"Recovery",
	["school"] = 	{SCHOOL_NATURE},
	["level"] = 	29,
	["mana"] = 	10,
	["mana_max"] = 	25,
	["fail"] = 	60,
	["stick"] =
	{
			["charge"] =    { 5, 10 },
			[TV_ORB] =
			{
				["rarity"] = 		50,
				["base_level"] =	{ 1, 5 },
				["max_level"] =		{ 10, 30 },
			},
	},
	["inertia"] = 	{ 2, 100 },
	["spell"] = 	function()
			local obvious
			obvious = timed_effect(timed_effect.POISON, (timed_effect.get(timed_effect.POISON) or 0) / 2)
			if get_cast_level(50) >= 5 then
				obvious = is_obvious(timed_effect(timed_effect.POISON, 0), obvious)
				obvious = is_obvious(timed_effect(timed_effect.CUT, 0), obvious)
			end
			if get_cast_level(50) >= 10 then
				obvious = is_obvious(do_res_stat(A_STR, true), obvious)
				obvious = is_obvious(do_res_stat(A_CON, true), obvious)
				obvious = is_obvious(do_res_stat(A_DEX, true), obvious)
				obvious = is_obvious(do_res_stat(A_WIS, true), obvious)
				obvious = is_obvious(do_res_stat(A_INT, true), obvious)
				obvious = is_obvious(do_res_stat(A_CHR, true), obvious)
			end
			if get_cast_level(50) >= 15 then
				obvious = is_obvious(restore_level(), obvious)
			end
			return obvious
	end,
	["info"] = 	function()
			return ""
	end,
	["desc"] =	{
			"Reduces the length of time that you are poisoned",
			"At level 10 it cures poison and cuts",
			"At level 20 it restores drained stats",
			"At level 30 it restores lost experience"
	}
}

REGENERATION = add_spell
{
	["name"] = 	"Regeneration",
	["school"] = 	{SCHOOL_NATURE},
	["level"] = 	39,
	["mana"] = 	30,
	["mana_max"] = 	55,
	["fail"] = 	70,
	["inertia"] = 	{ 4, 40 },
	["spell"] = 	function()
			return timed_effect(timed_effect.LIFE_REGEN, rng(10) + 5 + get_cast_level(50), (5 + get_cast_level(10)) * 1000)
	end,
	["info"] = 	function()
			return "dur "..(5 + get_cast_level(50)).."+d10 "..(5 + get_cast_level(10)).."HP/turn"
	end,
	["desc"] =	{
			"Increases your body's regeneration rate",
	}
}


SUMMONANNIMAL = add_spell
{
	["name"] =      "Summon Animal",
	["school"] = 	{SCHOOL_NATURE},
	["level"] = 	49,
	["mana"] = 	25,
	["mana_max"] = 	50,
	["fail"] = 	90,
	["stick"] =
	{
			["charge"] =    { 1, 3 },
			[TV_ORB] =
			{
				["rarity"] = 		85,
				["base_level"] =	{ 1, 5 },
				["max_level"] =		{ 15, 45 },
			},
	},
	["no_target"] = true,
	["tactical"] =
	{
		[ai.tactics.SUMMON] = 9
		[ai.tactics.ANNOY] = 4
	}
	["spell"] = 	function()
			summon_specific_level = 25 + get_cast_level(50)
			local y, x, faction

			if spell_caster < 0 then
				error("Summon Animal can only be cast by player and monsters")
			elseif spell_caster == 0 then
				y       = player.py
				x       = player.px
				faction = FACTION_PLAYER
			else
				local mon = monster(spell_caster)
				y = mon.fy
				x = mon.fx
				faction = mon.faction
			end

			return summon_monster(y, x, dun_level, faction,
								  {flags = {ANIMAL = true}})
	end,
	["info"] = 	function()
			return "level "..(25 + get_cast_level(50))
	end,
	["desc"] =	{
			"Summons a leveled animal to your aid",
	}
}

function grow_trees(y, x, rad)

	for a = 1, rad * rad do
		local i = (rng(0, (rad * 2) + 1) - rad +
				   rng(0, (rad * 2) + 1) - rad) / 2
		local j = (rng(0, (rad * 2) + 1) - rad +
				   rng(0, (rad * 2) + 1)-rad) / 2

		if in_bounds(y + j, x + i) and
			distance(y, x, y + j, x + i) <= rad and
			cave_clean_bold(y + j, x + i) and
			has_flag(f_info(cave(y + j, x + i).feat),
					 FLAG_SUPPORT_GROWTH) then
            cave_set_feat(y + j, x + i, FEAT_TREES);
		end
	end
end
