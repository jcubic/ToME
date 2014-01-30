--
-- This file takes care of the schools of magic
--

declare_global_constants {
	"SCHOOL_DEVICE",
	"make_breath",
}

-- Create the schools
SCHOOL_DEVICE = add_school
{
	name = "Device",
	skill = "SKILL_NONE",
}

-- Define how to decide failure rates
function player.spell_chance_get_fails(stat)
	local fail_adj = player.adj_mag_stat[player.stat(stat) + 1]
	local fail_min = player.adj_mag_fail[player.stat(stat) + 1]
	return fail_adj, fail_min
end

-- Functions to automate breath, ball and bolt spells and add trap info
-- for balls and bolts. trap damage is now scaled with level, and may be
-- too powerful at high depths. I have generally made max trap damage
-- equal to max breath damage of the same type, with exception to some
-- of the irresistable types which are more powerful as traps than as
-- breaths. no irresistable traps do more than 600 damage.

function make_breath(type)
	local name = get_dam_type_info(type, "desc")
	add_monster_spell
	{
		name	= "Breathe "..name,
		action	= "@Source@ breathes "..name.." at @target@.",
		spell	= function()
			local ret, dir = get_aim_dir()
			if not ret then return SPELL_NOTHING end
			local dam = get_cast_level(100)
			fire_cone(%type, dir, dam, 1 + get_cast_level(3))
			return true
		end,
	}
end
make_breath(dam.FIRE)
make_breath(dam.COLD)
