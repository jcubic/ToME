--
-- This file takes care of the schools of magic
--

declare_global_constants {
	"SCHOOL_AIR",
	"SCHOOL_CONVEYANCE",
	"SCHOOL_DEMON",
	"SCHOOL_DEVICE",
	"SCHOOL_DIVINATION",
	"SCHOOL_EARTH",
	"SCHOOL_ERU",
	"SCHOOL_FIRE",
	"SCHOOL_GEOMANCY",
	"SCHOOL_MANA",
	"SCHOOL_MANWE",
	"SCHOOL_MELKOR",
	"SCHOOL_META",
	"SCHOOL_MIND",
	"SCHOOL_MUSIC",
	"SCHOOL_NATURE",
	"SCHOOL_TEMPORAL",
	"SCHOOL_TULKAS",
	"SCHOOL_UDUN",
	"SCHOOL_ULMO",
	"SCHOOL_WATER",
	"SCHOOL_YAVANNA",
}

-- Create the schools
SCHOOL_MANA = add_school
{
	name = "Mana",
	skill = "SKILL_MANA",
	spell_power = true,
	sorcery = true,
	gods =
	{
		-- Eru Iluvatar provides the Mana school at half the prayer skill
		[god.ERU] =
		{
			skill = "SKILL_PRAY",
			mul = 1,
			div = 2,
		},
	},
	hooks =
	{
		[hook.CALC_MANA] = function(msp)
			if get_skill(SKILL_MANA) >= 70 then
				msp = msp + (msp * ((get_skill_scale(SKILL_MANA, 50) - 34)) / 100)
				return true, msp
			end
		end
	},
}
SCHOOL_FIRE = add_school
{
	name = "Fire",
	skill = "SKILL_FIRE",
	spell_power = true,
	sorcery = true,
}
SCHOOL_AIR = add_school
{
	name = "Air",
	skill = "SKILL_AIR",
	spell_power = true,
	sorcery = true,
	gods =
	{
		-- Manwe Sulimo provides the Air school at 2/3 the prayer skill
		[god.MANWE] =
		{
			skill = "SKILL_PRAY",
			mul = 2,
			div = 3,
		},
	},
}
SCHOOL_WATER = add_school
{
	name = "Water",
	skill = "SKILL_WATER",
	spell_power = true,
	sorcery = true,
	gods =
	{
		-- Yavanna Kementari provides the Water school at 1/2 the prayer skill
		[god.YAVANNA] =
		{
			skill = "SKILL_PRAY",
			mul = 1,
			div = 2,
		},
	},
}
SCHOOL_EARTH = add_school
{
	name = "Earth",
	skill = "SKILL_EARTH",
	spell_power = true,
	sorcery = true,
	gods =
	{
		-- Tulkas provides the Earth school at 4/5 the prayer skill
		[god.TULKAS] =
		{
			skill = "SKILL_PRAY",
			mul = 4,
			div = 5,
		},
		-- Yavanna Kementari provides the Earth school at 1/2 the prayer skill
		[god.YAVANNA] =
		{
			skill = "SKILL_PRAY",
			mul = 1,
			div = 2,
		},
	},
}
SCHOOL_CONVEYANCE = add_school
{
	name = "Conveyance",
	skill = "SKILL_CONVEYANCE",
	spell_power = true,
	sorcery = true,
        mindcraft = { mul=1 div=1 }
	gods =
	{
		-- Manwe Sulimo provides the Conveyance school at 1/2 the prayer skill
		[god.MANWE] =
		{
			skill = "SKILL_PRAY",
			mul = 1,
			div = 2,
		},
	},
}
SCHOOL_GEOMANCY = add_school
{
	name = "Geomancy",
	skill = "SKILL_GEOMANCY",
	spell_power = true,
	-- Require to wield a Mage Staff, as the spells requires the caster to stomp the floor with it
	depend = function()
		-- Require at least one point in each school
		if get_skill(SKILL_FIRE) == 0 then return end
		if get_skill(SKILL_AIR) == 0 then return end
		if get_skill(SKILL_EARTH) == 0 then return end
		if get_skill(SKILL_WATER) == 0 then return end

		local obj = player.inventory[INVEN_MAINHAND][1]
		if obj and (obj.tval == TV_MSTAFF) then return true end
	end,
}
SCHOOL_DIVINATION = add_school
{
	name = "Divination",
	skill = "SKILL_DIVINATION",
	spell_power = true,
	sorcery = true,
        mindcraft = { mul=1 div=1 }
	gods =
	{
		-- Eru Iluvatar provides the Divination school at 2/3 the prayer skill
		[god.ERU] =
		{
			skill = "SKILL_PRAY",
			mul = 2,
			div = 3,
		},
	},
}
SCHOOL_TEMPORAL = add_school
{
	name = "Temporal",
	skill = "SKILL_TEMPORAL",
	spell_power = true,
	sorcery = true,
        mindcraft = { mul=1 div=1 }
	gods =
	{
		-- Yavanna Kementari provides the Temoral school at 1/6 the prayer skill
		[god.YAVANNA] =
		{
			skill = "SKILL_PRAY",
			mul = 1,
			div = 6,
		},
	},
}
SCHOOL_NATURE = add_school
{
	name = "Nature",
	skill = "SKILL_NATURE",
	spell_power = true,
	sorcery = true,
	gods =
	{
		-- Yavanna Kementari provides the Nature school at 1/2 the prayer skill
		[god.YAVANNA] =
		{
			skill = "SKILL_PRAY",
			mul = 1,
			div = 2,
		},
	},
}
SCHOOL_META = add_school
{
	name = "Meta",
	skill = "SKILL_META",
	spell_power = true,
	sorcery = true,
	gods =
	{
		-- Manwe Sulimo provides the Meta school at 1/3 the prayer skill
		[god.MANWE] =
		{
			skill = "SKILL_PRAY",
			mul = 1,
			div = 3,
		},
	},
}
SCHOOL_MIND = add_school
{
	name = "Mind",
	skill = "SKILL_MIND",
	spell_power = true,
	sorcery = true,
        mindcraft = { mul=1 div=1 }
	gods =
	{
		-- Eru Iluvatar provides the Mind school at 1/3 the prayer skill
		[god.ERU] =
		{
			skill = "SKILL_PRAY",
			mul = 1,
			div = 3,
		},
		-- Melkor Bauglir provides the Mind school at 1/3 the prayer skill
		[god.MELKOR] =
		{
			skill = "SKILL_PRAY",
			mul = 1,
			div = 3,
		},
	},
}
SCHOOL_UDUN = add_school
{
	name = "Udun",
	skill = "SKILL_UDUN",
	bonus_level = function()
		return (player.lev / 6)
	end,
}
SCHOOL_DEMON = add_school
{
	name = "Demon",
	skill = "SKILL_DAEMON",
	no_random = true,
}

-- The God specific schools, all tied to the prayer skill
SCHOOL_ERU = add_school
{
	name = "Eru Iluvatar",
	skill = "SKILL_PRAY",
	spell_power = true,
	god = god.ERU,
}
SCHOOL_MANWE = add_school
{
	name = "Manwe Sulimo",
	skill = "SKILL_PRAY",
	spell_power = true,
	god = god.MANWE,
}
SCHOOL_TULKAS = add_school
{
	name = "Tulkas",
	skill = "SKILL_PRAY",
	spell_power = true,
	god = god.TULKAS,
}
SCHOOL_MELKOR = add_school
{
	name = "Melkor Bauglir",
	skill = "SKILL_PRAY",
	spell_power = true,
	god = god.MELKOR,
}
SCHOOL_YAVANNA = add_school
{
	name = "Yavanna Kementari",
	skill = "SKILL_PRAY",
	spell_power = true,
	god = god.YAVANNA,
}
SCHOOL_ULMO = add_school
{
	name = "Ulmo",
	skill = "SKILL_PRAY",
	spell_power = true,
	god = god.ULMO,
}

-- Not a real school, rather a palcehodler for stick only spells
SCHOOL_DEVICE = add_school
{
	name = "Device",
	skill = "SKILL_DEVICE",
}

-- Music "spells"
SCHOOL_MUSIC = add_school
{
	name = "Music",
	skill = "SKILL_MUSIC",
}

-- Define how to decide failure rates
function player.spell_chance_get_fails(stat)
	local fail_adj = (((player.stat(A_INT) - 10) * 2) / 3)

	-- Extract the minimum failure rate
	local minfail
	if player.stat(A_INT) < 10 then
		minfail = (10 - player.stat(A_INT)) * 10
	elseif player.stat(A_INT) < 40 then
		minfail = 10 - (((player.stat(A_INT) - 10) * 30) / 100)
	else
		minfail = 0
	end

	return fail_adj, minfail
end

-- Special spell level for special skills(sorcery, mindcraft, ...)
function player.spell_get_extra_level(s, sch)
	local so, mc = 0, 0
        local erase = false

	-- Are we under sorcery effect ?
	if __schools[sch].sorcery then
		so = get_skill_raw(SKILL_SORCERY)
	end

	-- Mindcraft spell ?
	if __schools[sch].mindcraft and mindcraft.in_use then
        	return (get_skill_raw(SKILL_MINDCRAFT) * __schools[sch].mindcraft.mul) / __schools[sch].mindcraft.div, true
        end

        return so
end

-- Special spell level bonus
function player.spell_get_power_bonus(s, sch)
	return get_skill_scale(SKILL_SPELL, 40)
end

-- Put some spells
load_spells("fire.lua")
load_spells("mana.lua")
load_spells("water.lua")
load_spells("air.lua")
load_spells("earth.lua")
load_spells("conveyance.lua")
load_spells("nature.lua")
load_spells("divination.lua")
load_spells("temporal.lua")
load_spells("meta.lua")
load_spells("mind.lua")
load_spells("udun.lua")
load_spells("geomancy.lua")

-- God's specific spells
load_spells("eru.lua")
load_spells("manwe.lua")
load_spells("tulkas.lua")
load_spells("melkor.lua")
load_spells("yavanna.lua")

-- Specific schools
load_spells("demonology.lua")

-- Device spells
load_spells("stick.lua")

-- Musics
load_spells("music.lua")

-- Mosnter spells
load_spells("monster.lua")

-- Thaumaturgy system
load_spells("thaum.lua")

-- Runecraft system
load_spells("runecraf.lua")

-- Alchemy system
load_spells("alchemy.lua")

-- Necromantic system
load_spells("necromancy.lua")

-- Antimagic system
load_spells("antimagic.lua")

-- Mindcraft system
load_spells("mindcraft.lua")
