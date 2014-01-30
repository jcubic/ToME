-- handle the demonology school

declare_global_constants {
	"CONTROL_DEMON",
	"DEMON_WHIP",
	"DEMON_CLOAK",
	"DEMON_FIELD",
	"DEMON_MADNESS",
	"DEMON_SUMMON",
	"DISCHARGE_MINION",
	"DOOM_SHIELD",
	"UNHOLY_WORD",
	"HELL_CLAWS",
	"DEMON_FLIGHT",
	"DEMONIC_HOWL",
	"do_demon_flight",
	"TIMER_DEMON_FLIGHT",
}

-- Demonblade
DEMON_WHIP = add_spell
{
	["name"] =      "Demon Whip",
	["school"] =    {SCHOOL_DEMON},
	["level"] =     1,
	["mana"] =      4,
	["mana_max"] =  44,
	["fail"] =      10,
	["random"] =    0,
	["stick"] =
	{
			["charge"] =    { 3, 7 },
			[TV_ORB] =
			{
				["rarity"] =	    75,
				["base_level"] =	{ 1, 17 },
				["max_level"] =		{ 20, 40 },
			},
	},
	["spell"] =     function()
			local type, rad

			type = dam.FIRE
			if get_cast_level() >= 60 then type = dam.DARK_FIRE end

			rad = 0
			if get_cast_level() >= 90 then rad = 1 end

			return timed_effect(
				timed_effect.MELEE_PROJECT,
				rng(20) + get_cast_level(80),
				type,
				4 + get_cast_level(40),
				rad,
				PROJECT_STOP | PROJECT_KILL
			)
	end,
	["info"] =      function()
			return "dur "..(get_cast_level(80)).."+d20 dam "..(4 + get_cast_level(40)).."/blow"
	end,
	["desc"] =      {
			"Imbues your whip with fire to deal more damage",
			"At level 60 it deals hellfire damage",
			"At level 90 it spreads over a 1 radius zone around your target",
	}
}

DEMON_MADNESS = add_spell
{
	["name"] =      "Demon Madness",
	["school"] =    {SCHOOL_DEMON},
	["level"] =     10,
	["mana"] =      5,
	["mana_max"] =  20,
	["fail"] =      25,
	["random"] =    0,
	tactical =
	{
		[ai.tactics.ATTACK] = 7
		[ai.tactics.ANNOY] = 3
	}
	["spell"] =     function()
			local cy, cx = get_spell_caster_info()
			local ret, dir, type, y1, x1, y2, x2

			ret, dir = get_aim_dir()
			if not ret then return SPELL_NOTHING end

			type = dam.CHAOS
			if rng.percent(33) then type = dam.CONFUSION end
			if rng.percent(33) then type = dam.CHARM end

			-- Calc the coordinates of arrival
			y1, x1 = get_target(dir)
			y2 = cy - (y1 - cy)
			x2 = cx - (x1 - cx)

			local obvious = nil
			obvious = project(spell_caster, 1 + get_cast_level(4, 0),
				y1, x1,
				20 + get_cast_level(200),
				type, PROJECT_STOP | PROJECT_GRID | PROJECT_ITEM | PROJECT_KILL)
			obvious = is_obvious(project(spell_caster, 1 + get_cast_level(4, 0),
				y2, x2,
				20 + get_cast_level(200),
				type, PROJECT_STOP | PROJECT_GRID | PROJECT_ITEM | PROJECT_KILL), obvious)
			return obvious
	end,
	["info"] =      function()
			return "dam "..(20 + get_cast_level(200)).." rad "..(1 + get_cast_level(4, 0))
	end,
	["desc"] =      {
			"Fire 2 balls in opposite directions of randomly chaos, confusion or charm",
	}
}

DEMON_FIELD = add_spell
{
	["name"] =      "Demon Field",
	["school"] =    {SCHOOL_DEMON},
	["level"] =     20,
	["mana"] =      20,
	["mana_max"] =  60,
	["fail"] =      60,
	["random"] =    0,
	tactical =
	{
		[ai.tactics.ATTACK] = 7
	}
	["spell"] =     function()
			local ret, dir

			ret, dir = get_aim_dir()
			if not ret then return SPELL_NOTHING end
			return fire_cloud(dam.NEXUS, dir, 20 + get_cast_level(70), 7, 30 + get_cast_level(100), player.speed())
	end,
	["info"] =      function()
			return "dam "..(20 + get_cast_level(70)).." dur "..(30 + get_cast_level(100))
	end,
	["desc"] =      {
			"Fires a cloud of deadly nexus over a radius of 7",
	}
}

-- Demonshield

DOOM_SHIELD = add_spell
{
	["name"] =      "Doom Shield",
	["school"] =    {SCHOOL_DEMON},
	["level"] =     1,
	["mana"] =      2,
	["mana_max"] =  30,
	["fail"] =      10,
	["random"] =    0,
	tactical =
	{
		[ai.tactics.ENHANCEMENT] = 7
	}
	["spell"] =     function()
			return set_shield(rng(10) + 20 + get_cast_level(100), -300 + get_cast_level(100), SHIELD_COUNTER, 1 + get_cast_level(14), 10 + get_cast_level(15))
	end,
	["info"] =      function()
			return "dur "..(20 + get_cast_level(100)).."+d10 dam "..(1 + get_cast_level(14)).."d"..(10 + get_cast_level(15))
	end,
	["desc"] =      {
			"Raises a mirror of pain around you, doing very high damage to your foes",
			"that dare hit you, but greatly reduces your armor",
	}
}

UNHOLY_WORD = add_spell
{
	["name"] =      "Unholy Word",
	["school"] =    {SCHOOL_DEMON},
	["level"] =     25,
	["mana"] =      15,
	["mana_max"] =  45,
	["fail"] =      55,
	["random"] =    0,
	tactical =
	{
		[ai.tactics.HEAL] = 6
	}
	["spell"] =     function()
			local ret, x, y, c_ptr
			ret, x, y = tgt_pt()
			if not ret then return SPELL_NOTHING end
			c_ptr = cave(y, x)

			-- ok that is a monster
			if c_ptr.m_idx > 0 then
				local m_ptr = monster(c_ptr.m_idx)
				if m_ptr.faction ~= FACTION_PLAYER then
					message("You can only target a friend.")
					return
				end

				-- Oups he is angry now
				if rng.percent(30 - get_cast_level(25, 0)) then
					local m_name = monster_desc(m_ptr, 0).." turns against you."
					message(strupper(strsub(m_name, 0, 1))..strsub(m_name, 2))
				else
					local m_name = monster_desc(m_ptr, 0)
					message("You consume "..m_name..".")

					local heal = (m_ptr.hp * 100) / m_ptr.maxhp
					heal = ((30 + get_cast_level(50, 0)) * heal) / 100

					hp_player(heal)

					delete_monster_idx(c_ptr.m_idx)
				end
				return true
			end
	end,
	["info"] =      function()
			return "heal mhp% of "..(30 + get_cast_level(50, 0)).."%"
	end,
	["desc"] =      {
			"Kills a pet to heal you",
			"There is a chance that the pet won't die but will turn against you",
			"it will decrease with higher level",
	}
}

DEMON_CLOAK = add_spell
{
	["name"] =      "Demon Cloak",
	["school"] =    {SCHOOL_DEMON},
	["level"] =     20,
	["mana"] =      10,
	["mana_max"] =  40,
	["fail"] =      70,
	["random"] =    0,
	["spell"] =     function()
			return set_tim_reflect(rng(5) + 5 + get_cast_level(15, 0))
	end,
	["info"] =      function()
			return "dur "..(5 + get_cast_level(15, 0)).."+d5"
	end,
	["desc"] =      {
			"Raises a mirror that can reflect bolts and arrows for a time",
	}
}


-- Demonhorn
DEMON_SUMMON = add_spell
{
	["name"] =      "Summon Demon",
	["school"] =    {SCHOOL_DEMON},
	["level"] =     5,
	["mana"] =      10,
	["mana_max"] =  50,
	["fail"] =      30,
	["random"] =    0,
	tactical =
	{
		[ai.tactics.SUMMON] = 10
	}
	["spell"] =     function()
			local level = dun_level
			local minlevel = 4
			if level < minlevel then level=minlevel end
			summon_specific_level = 5 + get_cast_level(100)
			if get_cast_level() >= 70 then type = SUMMON_HI_DEMON end
			return summon_monster(player.py, player.px, level,
								  current_spell_info.faction,
								  {flags = {DEMON = true}})
	end,
	["info"] =      function()
			return "level "..(5 + get_cast_level(100))
	end,
	["desc"] =      {
			"Summons a leveled demon to your side",
			"At level 70 it summons a high demon",
	}
}

DISCHARGE_MINION = add_spell
{
	["name"] =      "Discharge Minion",
	["school"] =    {SCHOOL_DEMON},
	["level"] =     10,
	["mana"] =      20,
	["mana_max"] =  50,
	["fail"] =      30,
	["random"] =    0,
	["spell"] =     function()
			local ret, x, y, c_ptr
			ret, x, y = tgt_pt()
			if not ret then return SPELL_NOTHING end
			c_ptr = cave(y, x)

			-- ok that is a monster
			if c_ptr.m_idx > 0 then
				local m_ptr = monster(c_ptr.m_idx)
				if m_ptr.faction ~= FACTION_PLAYER then
					message("You can only target a pet.")
					return
				end

				local dam = m_ptr.hp
				delete_monster_idx(c_ptr.m_idx)
				dam = (dam * (20 + get_cast_level(60, 0))) / 100
				if dam > 100 + get_cast_level(500, 0) then
					dam = 100 + get_cast_level(500, 0)
				end

				-- We use project instead of fire_ball because we must tell it exactly where to land
				return project(spell_caster, 2,
					y, x,
					dam,
					dam.GRAVITY, PROJECT_STOP | PROJECT_GRID | PROJECT_ITEM | PROJECT_KILL)
			end
	end,
	["info"] =      function()
			return "dam "..(20 + get_cast_level(60, 0)).."% max "..(100 + get_cast_level(500, 0))
	end,
	["desc"] =      {
			"The targeted pet will explode in a burst of gravity",
	}
}

CONTROL_DEMON = add_spell
{
	["name"] =      "Control Demon",
	["school"] =    {SCHOOL_DEMON},
	["level"] =     25,
	["mana"] =      30,
	["mana_max"] =  70,
	["fail"] =      55,
	["random"] =    0,
	["spell"] =     function()
			local ret, dir = get_aim_dir()
			if not ret then return SPELL_NOTHING end
			return fire_ball(dam.CONTROL_DEMON, dir, 50 + get_cast_level(250), 0)
	end,
	["info"] =      function()
			return "power "..(50 + get_cast_level(250))
	end,
	["desc"] =      {
			"Attempts to control a demon",
	}
}

-- Demonclaws
HELL_CLAWS = add_spell
{
	["name"] =      "Hell Claws",
	["school"] =    {SCHOOL_DEMON},
	["level"] =     5,
	["mana"] =      10,
	["mana_max"] =  50,
	["fail"] =      30,
	["random"] =    0,
	["spell"] =     function()
			local type

			if get_cast_level() >= 30 then
				type = dam.DARK
			else
				type = dam.DARK_WEAK
			end

			return timed_effect(
				timed_effect.MELEE_PROJECT,
				rng(20) + get_cast_level(80),
				type,
				4 + get_cast_level(40),
				rad,
				PROJECT_STOP | PROJECT_KILL
			)
	end,
	["info"] =      function()
			return "dur "..(get_cast_level(80)).."+d20 dam "..(4 + get_cast_level(40)).."/blow"
	end,
	["desc"] =      {
			"Wreathes the Demonclaws in swirling, unearthly darkness.",
			"This causes blows done with a Demonblade or in weaponless",
			"combat to do bonus darkness damage.",
			"At spell level 15, does a stronger form of darkness damage",
	}
}

DEMON_FLIGHT = add_spell
{
	["name"] =      "Demon Flight",
	["school"] =    {SCHOOL_DEMON},
	["level"] =     15,
	["mana"] =      10,
	["mana_max"] =  30,
	["fail"] =      15,
	["random"] =    0,
	["spell"] =     function()
			local dur, level
			dur = rng(10) + 10 + get_cast_level(25)
			level = get_cast_level(50)
			return do_demon_flight(level, dur)
	end,
	["info"] =      function()
			return "dur "..(get_cast_level(25) + 10).."+d10"
	end,
	["desc"] =      {
			"Imbues you with the spirit of a flying demon -- giving you a ",
			"substantial boost of speed, as well as the ability to levitate.",
			"At higher levels, grants even greater abilities to escape ",
			"confinement or pursuit.",
			"At spell level 5, grants free action.",
			"At spell level 10, grants flight.",
			"At spell level 15, grants a short-duration wraith form.",
	}
}

DEMONIC_HOWL = add_spell
{
	["name"] =      "Demonic Howl",
	["school"] =    {SCHOOL_DEMON},
	["level"] =     20,
	["mana"] =      12,
	["mana_max"] =  30,
	["fail"] =      15,
	["random"] =    0,
	tactical =
	{
		[ai.tactics.ATTACK] = 5
		[ai.tactics.ANNOY] = 7
	}
	["spell"] =     function()
			local dam = get_cast_level(50) * 5 + rng(30)

			if spell_caster == WHO_PLAYER and get_cast_level() > 40 then
				return project_los(dam.DEMONIC_HOWL, dam)
			else
				local ret, dir = get_aim_dir()
				if not ret then return SPELL_NOTHING end
				return fire_cone(dam.DEMONIC_HOWL, dir, dam, 3)
			end
	end,
	["info"] =      function()
			return "dam "..(get_cast_level(50) * 5).."+d30"
	end,
	["desc"] =      {
			"Projects an unholy cry which extends as a cone in a given direction.",
			"Foes caught within the cone suffer sound damage, and may be stunned or made afraid. ",
			"Summoned demons within the cone, however, receive combat bonuses. ",
			"At spell level 40, affects all monsters within sight.",
	}
}

-- loadsaves for the TIMER
time.save_timer("TIMER_DEMON_FLIGHT")

-- helps set FA safely
TIMER_DEMON_FLIGHT = time.new_timer
{
	["delay"] = 	10,
	["enabled"] = 	nil,
	["callback"] = 	function()
			-- Stop now
			TIMER_DEMON_FLIGHT.enabled = nil
			player.update = player.update | PU_BONUS
			message("You feel less able to move freely.")
	end,
}

function do_demon_flight(level, dur)

		-- spell_level > 15 give wraithfrom
		if level >= 15 then
			player.tim_wraith = player.tim_wraith + dur
		end

		-- spell level > 10 give flight
		if level >= 10 then
			if player.tim_fly == 0 then set_tim_fly(dur) end
		end

		-- spell_level >5 give FA
		if level >= 5 then
			-- set the timer, to unset the effect
			TIMER_DEMON_FLIGHT.enabled = true
			TIMER_DEMON_FLIGHT.delay = dur
			TIMER_DEMON_FLIGHT.countdown = dur
			player.update = player.update | PU_BONUS
			message("You feel able to move freely.")
		end

		-- only give levitation if the person is not flying
		if player.tim_ffall == 0 and player.tim_fly == 0 then set_tim_ffall(dur) end

		-- everyone gets the speed boost
		timed_effect(timed_effect.FAST, dur, 3 + get_cast_level(12))
		return
end

-- new spell type for demonologists - like dam.SOUND, but gives combat bonuses to friendly demons!
dam.DEMONIC_HOWL = add_damage_type
{
	["color"]       = { color.LIGHT_RED, 0 },
	["monster"]	= function(who, dam, rad, y, x, flags, m_ptr)
			local m_name = monster_desc(m_ptr, 0)
	       		local r_ptr = race_info(m_ptr)
	       		local num = 4

			if has_flag(r_ptr, FLAG_DEMON) and m_ptr.faction == FACTION_PLAYER then
				monster_msg(strcap(m_name).." feels more powerful!.", m_ptr.ml)
				m_ptr.hp = m_ptr.hp + dam
				dam = 0
				while num > 1 do
					if m_ptr.blow[num].method_idx ~= 0 then
						m_ptr.blow[num].d_dice = m_ptr.blow[num].d_dice + 2
					end
					num = num - 1
				end
			else
				if has_flag(r_ptr, FLAG_NO_STUN) then
					monster_msg(strcap(m_name).." is unaffected!", m_ptr.ml)
				elseif monster_saving_throw(who, m_ptr) then
					monster_msg(strcap(m_name).." is unaffected!", m_ptr.ml)
				else
					monster_msg(strcap(m_name).." is stunned!", m_ptr.ml)
					m_ptr.stunned = m_ptr.stunned + (dam/7) + 10
				end
				if has_flag(r_ptr, FLAG_NO_FEAR) then
					monster_msg(strcap(m_name).." is unaffected!", m_ptr.ml)
				elseif monster_saving_throw(who, m_ptr) then
					monster_msg(strcap(m_name).." is unaffected!", m_ptr.ml)
				else
					monster_msg(strcap(m_name).." is stunned!", m_ptr.ml)
					m_ptr.monfear = m_ptr.monfear + (dam/7) + 10
				end
			end

			return true, true, dam
	end,
	["angry"]       = function() return true, FALSE end,
	-- potions need a chance to shatter. This isn't working.
	["object"]	= function(who, dam, rad, y, x, flags, o_ptr)
			local breakage = nil
			if o_ptr.tval == TV_POTION then
				breakage = rng.percent(50)
				if breakage then message("The potion shatters!") return true, true, breakage end
			end
			return true
	end,
	["player"]	= function(who, dam, rad, y, x, flags)
			if player.resist_sound then
				message("You are unaffected!")
			elseif rng.number(100) < player.skill_sav then
				message("You resist the effects!")
			else
				set_stun((dam/7) + 10)
			end

			return true, true, dam
	end,
}

-- ok we need to have different wield slots
hook
{
	[hook.CALC_BONUS] =	function()
			-- If the timer is still counting, set free action
			if TIMER_DEMON_FLIGHT.enabled then
				player.set_intrinsic(FLAG_FREE_ACT)
			end
			return
	end,
}
