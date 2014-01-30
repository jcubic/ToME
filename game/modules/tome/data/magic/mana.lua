-- The mana school

declare_global_constants {
	"DELCURSES",
	"MANASHIELD",
	"MANATHRUST",
	"RESISTS",
	"get_manathrust_dam",
}

function get_manathrust_dam()
	return 3 + get_level(MANATHRUST, 50), 1 + get_level(MANATHRUST, 20)
end

MANATHRUST = add_spell
{
	name = 		"Manathrust",
	school = 	SCHOOL_MANA,
	level = 	1,
	mana = 		1,
	mana_max =  	25,
	fail = 		10,
	stick =
	{
			charge =    { 7, 10 },
			[TV_ORB] =
			{
				rarity = 		5,
				base_level =	{ 1, 20 },
				max_level =		{ 15, 33 },
			},
	},
	tactical =
	{
		[ai.tactics.ATTACK] = 10
	}
	spell = 	function()
			local ret, dir

			ret, dir = get_aim_dir()
			if not ret then return SPELL_NOTHING end

			if has_ability(AB_ARCANE_LANCE) and rng.percent(50) then
				return fire_beam(dam.PURE, dir, rng.roll(get_manathrust_dam()))
			else
				return fire_bolt(dam.PURE, dir, rng.roll(get_manathrust_dam()))
			end
	end,
	info = 	function()
			return "dam "..rng.desc_roll(get_manathrust_dam())
	end,
	desc =	{
			"Conjures up mana into a powerful bolt",
			"The damage is irresistible and will increase with level"
		}
}

DELCURSES = add_spell
{
	name = 		"Remove Curses",
	school = 	SCHOOL_MANA,
	level = 	19,
	mana = 		20,
	mana_max = 	40,
	fail = 		30,
	stick =
	{
			charge =    { 3, 8 },
			[TV_ORB] =
			{
				rarity = 		70,
				base_level =	{ 1, 5 },
				max_level =		{ 15, 50 },
			},
	},
	inertia = 	{ 1, 10 },
	spell = 	function()
			local done

			if get_cast_level(50) >= 20 then done = remove_all_curse()
			else done = remove_curse() end
			if done then message("The curse is broken!") end
			return done
	end,
	info = 	function()
			return ""
	end,
	desc =	{
			"Remove curses of worn objects",
			"At level 40 switches to *remove curses*"
		}
}

RESISTS = add_spell
{
	name = 		"Elemental Shield",
	school = 	SCHOOL_MANA,
	level = 	39,
	mana = 		17,
	mana_max = 	20,
	fail = 		40,
	inertia = 	{ 2, 25 },
	tactical =
	{
		[ai.tactics.ENHANCEMENT] = 10
	}
	spell = 	function()
			local dur = rng(10) + 15 + get_cast_level(50)
			local obvious =      timed_effect(timed_effect.RES_FIRE, dur, get_cast_level(80))
		       	obvious = is_obvious(timed_effect(timed_effect.RES_COLD, dur, get_cast_level(80)), obvious)
		       	obvious = is_obvious(timed_effect(timed_effect.RES_ACID, dur, get_cast_level(80)), obvious)
		       	obvious = is_obvious(timed_effect(timed_effect.RES_ELECTRICITY, dur, get_cast_level(80)), obvious)
			return obvious
	end,
	info = 	function()
			return "dur "..(15 + get_cast_level(50)).."+d10 res "..get_cast_level(80).."%"
	end,
	desc =	{
			"Provide resistances to the four basic elements",
		}
}

MANASHIELD = add_spell
{
	name = 		"Disruption Shield",
	school = 	SCHOOL_MANA,
	level = 	89,
	mana = 		75,
	mana_max = 	90,
	fail = 		90,
	spell = 	function()
			local ratio = 200
			if has_ability(AB_IMP_MANA_SHIELD2) then ratio = 100
			elseif has_ability(AB_IMP_MANA_SHIELD1) then ratio = 150 end
			timed_effect(timed_effect.MANA_SHIELD, 3 + get_cast_level(10) + rng(5), ratio)
			return true
	end,
	info = 	function()
			return "dur "..(3 + get_cast_level(10)).."+d5"
	end,
	desc =	{
			"Uses mana instead of hp to take damage",
			"The spell breaks as soon as an other spell is cast and lasts only a short time."
		}
}
