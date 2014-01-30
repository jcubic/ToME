-- Monster spells
-- Monsters can also uses player spells, this file is only for spells players cant use

declare_global_constants {
	"monster_breath",
	"monster_saving_throw",
	"split_monster_player",
	"make_storm",
	"make_breath",
	"make_bolt",
	"make_ball",
	"make_summon",
	"make_stat_spell",
	"fill_traps",
	"make_webs",
	"SCHOOL_MONSTER_STORM",
	"SCHOOL_MONSTER_BREATH",
	"SCHOOL_MONSTER_BOLT",
	"SCHOOL_MONSTER_BALL",
}

-- tries to do a monster saving throw
function monster_saving_throw(source, m_ptr)
	local lev
	if source == 0 then
		lev = player.lev - 10
	else
		lev = source.level - 10
	end
	if lev < 1 then lev = 1 end

	return (m_ptr.level > rng(lev) + 10)
end


-- Helper function to split player/monster effects
function split_monster_player(play, monst)
	return function()
			local ret, x, y = tgt_pt()
			if not ret then return SPELL_NOTHING end
			local c_ptr = cave(y, x)
			local source = get_attacking_monst(spell_caster) or player

			if c_ptr.m_idx == 0 then
				%play(source)
			else
				%monst(source, monster(c_ptr.m_idx), monster_desc(monster(c_ptr.m_idx), 0), spell_caster, c_ptr.m_idx)
			end
			return true
	end
end


add_monster_spell
{
	school        = SCHOOL_MONSTER_ABILITY,
	antimagic     = FALSE,
	name	      = "Shriek",
	memory        = "shriek",
	action        = "@Source@ makes a high pitched shriek.",
	fail_action   = "",
	ignore_unseen = true,
	no_target     = true,
	symb_rebel    = true,
	tactical  = {[ai.tactics.ANNOY] = 10},
	spell	  = function()
		if player.player_body_spell() then
			message("You make a high pitched shriek!")
			aggravate_monsters(0)
		elseif __monst_helper.helper then
			aggravate_monsters(0)
		else
			local y, x = get_spell_caster_info()
			wake_monsters(y, x, 40)
			return true
		end
	end,
}
add_monster_spell
{
	school    = SCHOOL_MONSTER_ABILITY,
	antimagic = FALSE,
	name	      = "Howl",
	memory        = "howl",
	action	      = "@Source@ gives a bloodcurdling howl!",
	fail_action   = "",
	ignore_unseen = true,
	no_target     = true,
	symb_rebel    = true,
	tactical  = {[ai.tactics.ANNOY] = 10},
	spell	  = function()
		if player.player_body_spell() then
			message("You give a bloodcurdling howl!")
			aggravate_monsters(0)
		elseif __monst_helper.helper then
			aggravate_monsters(0)
		else
			local y, x = get_spell_caster_info()
			wake_monsters(y, x, 40)
			return true
		end
	end,
}

add_monster_spell
{
	name	      = "Scare",
	action	      = "@Source@ casts a fearful illusion at @target@.",
	symb_no_rebel = true
	tactical      =
	{
	 [ai.tactics.ANNOY] = 4
	 [ai.tactics.ATTACK] = 6
	},
	spell	= split_monster_player
	(
		function(source)
			if rng.percent(player.resist(dam.FEAR)) then
				message("You refuse to be frightened.")
			elseif rng(100) < player.skill_sav then
				message("You refuse to be frightened.")
			else
				timed_effect.set(timed_effect.AFRAID, rng(get_cast_level(50)) + 4)
			end
		end,
		function(source, m_ptr, m_name)
			if tolua.type(source) == "monster_type" then
				local r_ptr = race_info(m_ptr)
				if has_flag(r_ptr, FLAG_NO_FEAR) then
					monster_msg(strcap(m_name).." refuses to be frightened.", m_ptr.ml)
				elseif monster_saving_throw(source, m_ptr) then
					monster_msg(strcap(m_name).." refuses to be frightened.", m_ptr.ml)
				else
					monster_msg(strcap(m_name).." flees in terror!", (m_ptr.monfear == 0) and (m_ptr.ml));
					m_ptr.monfear = m_ptr.monfear + rng(get_cast_level(50)) + 4
				end
			end
		end
	),
}

add_monster_spell
{
	name	= "Slow",
	action	= "@Source@ drains power from @target_possessive@ muscles!",
	tactical	  =
	{
	 [ai.tactics.ANNOY] = 6
	 [ai.tactics.ATTACK] = 6
	 [ai.tactics.ESCAPE] = 4
	},
	spell	= split_monster_player
	(
		function(source)
			if (player.free_act) then
				message("You are unaffected!")
			elseif (rng(100) < player.skill_sav) then
				message("You resist the effects!")
			else
				timed_effect.set(timed_effect.SLOW, rng(get_cast_level(50)) + 4, 10)
			end
		end,
		function(source, m_ptr, m_name)
			if tolua.type(source) == "monster_type" then
				local r_ptr = race_info(m_ptr)
				if has_flag(r_ptr, FLAG_UNIQUE) then
					monster_msg(strcap(m_name).." is unaffected.", m_ptr.ml)
				elseif monster_saving_throw(source, m_ptr) then
					monster_msg(strcap(m_name).." is unaffected.", m_ptr.ml)
				else
					m_ptr.mspeed = m_ptr.mspeed - 10
					monster_msg(strcap(m_name).." starts moving slower.", m_ptr.ml)
				end
			end
		end
	),
}

add_monster_spell
{
	name	= "Blind",
	action	= "@Source@ casts a spell, burning @target_possessive@ eyes.",
	tactical	  =
	{
	 [ai.tactics.ANNOY] = 5
	 [ai.tactics.ATTACK] = 6
	 [ai.tactics.ESCAPE] = 4
	},
	spell	= split_monster_player
	(
		function(source)
			if player.resist_blind then
				message("You are unaffected!")
			elseif rng(100) < player.skill_sav then
				message("You resist the effects!")
			else
				timed_effect.set(timed_effect.BLIND, rng(get_cast_level(50)) + 4)
			end
		end,
		function(source, m_ptr, m_name)
			if tolua.type(source) == "monster_type" then
				local r_ptr = race_info(m_ptr)
				if has_flag(r_ptr, FLAG_NO_CONF) then
					monster_msg(strcap(m_name).." is unaffected!.", m_ptr.ml)
				elseif monster_saving_throw(source, m_ptr) then
					monster_msg(strcap(m_name).." is unaffected!", m_ptr.ml)
				else
					monster_msg(strcap(m_name).." is blinded!", m_ptr.ml)
					m_ptr.confused = m_ptr.confused + rng(get_cast_level(50)) + 4
				end
			end
		end
	),
}

add_monster_spell
{
	name	= "Cause Wounds",
	action	= "@Source@ points at @target@, screaming the word 'DIE!'.",
	tactical	  = {[ai.tactics.ATTACK] = 8}
	spell	= split_monster_player
	(
		function(source)
			if (rng.percent(player.skill_sav)) then
				message("You resist the effects!")
			else
				take_hit(rng.roll(get_cast_level(50), 8), monster_desc(source, 136))
			end
		end,
		function(source, m_ptr, m_name, source_idx, m_idx)
			if tolua.type(source) == "monster_type" then
				if monster_saving_throw(source, m_ptr) then
					monster_msg(strcap(m_name).." resists.", m_ptr.ml)
				else
					mon_take_hit_mon(source_idx, m_idx, rng.roll(get_cast_level(50), 8), nil, " is destroyed.")
				end
			end
		end
	),
}

add_monster_spell
{
	name	= "Disease",
	action	= "@Source@ points at @target@, uttering strange words.",
	tactical	  = {[ai.tactics.ATTACK] = 8}
	spell	= split_monster_player
	(
		function(source)
			if (rng.percent(player.skill_sav)) then
				message("You resist the effects!")
			else
				timed_effect(timed_effect.DISEASE, 50, (player.mhp() * 2 * get_cast_level()) / 100)
			end
		end,
		function(source, m_ptr, m_name, source_idx, m_idx)
			if tolua.type(source) == "monster_type" then
				if m_ptr.flags[FLAG_UNIQUE] or monster_saving_throw(source, m_ptr) then
					monster_msg(strcap(m_name).." resists.", m_ptr.ml)
				else
					monster_effect(spell_caster, m_ptr, monster_effect.DISEASE, 50, (m_ptr.maxhp * 2 * get_cast_level()) / 100)
				end
			end
		end
	),
}

add_monster_spell
{
	name	= "Drain Mana",
	action	= "@Source@ tries to drain @target_possessive@ mana.",
	tactical	  =
	{
	 [ai.tactics.ATTACK] = 6
	 [ai.tactics.MANA] = 8
	},
	spell	= split_monster_player
	(
		function(source)
			if (player.csp() > 0) then
				local r1
				local rlev = get_cast_level(300)
				if rlev < 1 then rlev = 1 end

				-- Disturb if legal */
				term.disturb(1, 0)

				-- Basic message
				message(strcap(monster_desc(source, 0)).." draws psychic energy from you!")

				-- Attack power
				r1 = rng(rlev)

				-- Full drain
				if (r1 >= player.csp()) then
					r1 = player.csp()
					increase_mana(-r1)

				-- Partial drain
				else
					increase_mana(-r1)
				end

				-- Power up the monster
				local mana, mana_max = source.flags[FLAG_MANA_CURR] or 0, source.flags[FLAG_MANA_MAX] or 0
				if (mana < mana_max) then
					mana = mana + r1
					if (mana > mana_max) then mana = mana_max end
					source.flags[FLAG_MANA_CURR] = mana
				end
			end
		end,
		function(source, m_ptr, m_name)
			local rlev = get_cast_level(300)
			if rlev < 1 then rlev = 1 end

			-- Attack power
			local r1 = rng(rlev)

			-- Basic message
			monster_msg(strcap(monster_desc(source, 0)).." draws psychic energy from "..m_name..".", (source.ml) and (m_ptr.ml))

				-- Power up the monster
			local mana, mana_max = source.flags[FLAG_MANA_CURR] or 0, source.flags[FLAG_MANA_MAX] or 0
			if (mana < mana_max) then
				if (flag_used(m_ptr.spells) == 0 or
					not m_ptr.flags[FLAG_MANA_CURR]) then
					monster_msg(strcap(m_name).." is unaffected!", (source.ml) and (m_ptr.ml))
				else
					if m_ptr.flags[FLAG_MANA_CURR] <= 0 then
						monster_msg(strcap(m_name) .. " has no mana to drain.",
									(source.ml) and (m_ptr.ml))
						return
					end

					if r1 > m_ptr.flags[FLAG_MANA_CURR] then
						r1 = m_ptr.flags[FLAG_MANA_CURR]
					end

					m_ptr.flags[FLAG_MANA_CURR] =
						m_ptr.flags[FLAG_MANA_CURR] - r1
					mana = mana + r1
					print("mana absorbed = " .. r1)
					if (mana > mana_max) then mana = mana_max end
					source.flags[FLAG_MANA_CURR] = mana
				end
			end
		end
	),
}

add_monster_spell
{
	name	= "Darkness",
	action = {
		blind_like_unseen  = true,
 		casterunseen_nohead      = "",
		casterunseen_head        = "@Source@ mumbles.",
		casterseen_nohand_nohead = "Shadows flicker around @source@.",
		casterseen_nohand_head   = "@Source@ mumbles in shadows.",
		default                  = "@Source@ gestures in shadows.",
	}
	tactical	  =
	{
	 [ai.tactics.ATTACK] = 8
	 [ai.tactics.ANNOY] = 9
	 [ai.tactics.ESCAPE] = 4
	},
	spell	= function()
			local y, x = get_spell_caster_info()

			if spell_caster > 0 then
				monster_msg(strcap(monster_desc(monster(spell_caster), 0)).." is engulfed by darkness.", (spell_caster > 0) and (monster(spell_caster).ml))
			else
				message("You are engulfed by darkness.")
			end

			project(spell_caster, 3, y, x, 10, dam.DARK, PROJECT_GRID | PROJECT_KILL)

			-- Unlite up the room
			unlite_room(y, x)
			return true
	end,
}

add_monster_spell
{
	name	= "Animate Dead",
	action	= "@Source@ gestures at a corpse.",
	tactical = {[ai.tactics.SUMMON] = 8},
	spell	= function()
		message("IMPLEMENT ANIMATE DEAD!")
		return SPELL_NOTHING
	end,
}

add_monster_spell
{
	name	= "Smash Brain",
	action	= "@Source@ gazes intently at @target@.",
	tactical = {[ai.tactics.ATTACK] = 8},
	spell	= function()
		message("IMPLEMENT SMASH BRAIN!")
		return SPELL_NOTHING
	end,
}

add_monster_spell
{
	name	= "Mind Blast",
	action	= "@Source@ gazes intently at @target@.",
	tactical = {[ai.tactics.ATTACK] = 9},
	spell	= function()
		message("IMPLEMENT MIND BLAST!")
		return SPELL_NOTHING
	end,
}

add_monster_spell
{
	name	= "Hand of Doom",
	action	= "@Source@ invokes the Hand of Doom @target@.",
	tactical = {[ai.tactics.ATTACK] = 8},
	spell	= function()
		message("IMPLEMENT HAND OF DOOM!")
		return SPELL_NOTHING
	end,
}

add_monster_spell
{
	name = 	"Web",
	action = "@Source@ sprays webbing everywhere"
	antimagic     = FALSE,
	tactical	  =
	{
	 [ai.tactics.ANNOY] = 4
	 [ai.tactics.ATTACK] = 6
	},
		spell = 	function()
		local y, x = get_spell_caster_info()
		make_webs(y, x, 2 + get_cast_level(7))
		return true
	end,
}

function make_webs(y, x, rad)

	for a = 1, rad * rad do
		local i = (rng(0, (rad * 2) + 1) - rad +
				   rng(0, (rad * 2) + 1) - rad) / 2
		local j = (rng(0, (rad * 2) + 1) - rad +
				   rng(0, (rad * 2) + 1)-rad) / 2

		if in_bounds(y + j, x + i) and
			distance(y, x, y + j, x + i) <= rad and
			cave_clean_bold(y + j, x + i) then
			cave_set_feat(y + j, x + i, FEAT_WEB);
		end
	end
end
add_monster_spell
{
	school    = SCHOOL_MONSTER_ABILITY,
	antimagic = FALSE,
	name	  = "Arrow",
	memory    = "fire an arrow",
	action	    = "@Source@ fires an arrow at @target@.",
	fail_action = {
		blind_like_unseen  = true,
 		casterunseen       = "",
		default            = "@Source@ fumbles firing an arrow.",
	},
	symb_no_rebel = true,
	tactical      = {[ai.tactics.ATTACK] = 10},
	spell = function()
			local ret, dir = get_aim_dir()
			if not ret then return SPELL_NOTHING end
			fire_bolt(dam.PIERCE, dir, rng.roll(get_cast_level(50), 6))
			return true
	end,
}

add_monster_spell
{
	school        = SCHOOL_MONSTER_ABILITY,
	antimagic     = FALSE,
	name	      = "Missile",
	memory        = "fire a missile",
	action	      = "@Source@ fires a missile at @target@.",
	symb_no_rebel = true
	tactical      = {[ai.tactics.ATTACK] = 10},
	spell = function()
			local ret, dir = get_aim_dir()
			if not ret then return SPELL_NOTHING end
			fire_bolt(dam.CRUSH, dir, rng.roll(get_cast_level(50), 6))
			return true
	end,
}
-- fills an area with traps. maxpercent is the max probability of generating
-- a trap (this is used at the outer part of the radius). maxpercent > 100
-- does work and is useful
function fill_traps(y, x, outer, inner, faction, maxpercent)
	if not maxpercent then maxpercent = 100 end
	for i = x - outer, x + outer do
		for j = y - outer, y + outer do
			local dist = distance(y,x,j,i)
			if in_bounds(j, i) and rng.percent(maxpercent*(dist-inner)/(outer-inner)) then
				if not traps.location_has_trap(j, i) then
					place_trap(j,i,faction)
				end
			end
		end
	end
end

add_monster_spell
{
	name	= "Create Traps",
	action	= "@Source@ casts a spell and cackles evilly.",
	tactical =
	{
	 [ai.tactics.ATTACK] = 4
	 [ai.tactics.ANNOY] = 6
	 [ai.tactics.TACTIC] = 6
	},
	spell	= function()
		local ret, x, y = tgt_pt()
		if not ret then return false end
		fill_traps(y, x, 1, 0, current_spell_info.faction)
		return false
	end,
}

add_monster_spell
{
	name =	"Trap Filling",
	action = "@Source@ casts a spell, and laughs hysterically.",
	tactical =
	{
	 [ai.tactics.ATTACK] = 6
	 [ai.tactics.ANNOY] = 9
	 [ai.tactics.TACTIC] = 8
	},
	spell = function()
		local ret, x, y = tgt_pt()
		if not ret then return false end
		fill_traps(y, x, 8, 3, current_spell_info.faction)
		return false
	end,
}

-- stat reduction
function make_stat_spell(name,stat,minlvl,blvl,alvl)
	add_monster_spell
	{
		name	= name,

		action = {
			blind_target = "", -- Blind and monster is target, no msg
			casterunseen_nohead = "@Target@ @isare@ enveloped in a " ..
				"thick cloud!",
			casterseen_hand = "@Source@ points at @target@, and @target@ " ..
				"@isare@ enveloped in a thick cloud!",
			default  = "@Source@ mumbles, and and @target@ @isare@ " ..
				"enveloped in a thick cloud!",
		}
		tactical =
		{
			[ai.tactics.ATTACK] = 7
			[ai.tactics.ANNOY] = 8
		},
		spell	= split_monster_player(
		function(source)
			local lvl = get_cast_level(50)
			local stat = %stat
			local type = STAT_DEC_TEMPORARY
			if lvl > 12 then type = STAT_DEC_NORMAL
			elseif lvl > 45 then type = STAT_DEC_PERMANENT end
			player.update = player.update | PU_BONUS
			return do_dec_stat(stat, type)
		end,
		function(source, m_ptr, m_name)
			return false
		end
		),
	}
end

make_stat_spell("Weakness",A_STR,1,1,45)
make_stat_spell("Stupidity",A_INT,1,1,45)
make_stat_spell("Naivety",A_WIS,1,1,45)
make_stat_spell("Fumbling fingers",A_DEX,1,1,45)
make_stat_spell("Wasting",A_CON,1,1,45)
make_stat_spell("Beauty",A_CHR,1,1,45)

add_monster_spell
{
	name	= "Hold",

	action = {
		-- Player can't see the caster looking or gesturing at the
		-- targeted monster.
		casterunseen_target = ""

		-- Player can see the casting monster, but its casting it at
		-- another monster and the caster has no head or hands to
		-- do anything visible with.
		casterseen_target_nohand_nohead = "",

		-- Unseen castring monster trying to paralyze player.
		casterunseen_player = "@Source@ tries reaching into your mind!",

		casterseen_hand_nohead = "@Source@ gestures at @target@.",
		casterseen_targetunseen_head = "@Source@ stares at something.",

		casterseen_head = "@Source@ stares deep into @target_possessive@ eyes.",

		default = ""
	}
		tactical =
		{
			[ai.tactics.ATTACK] = 7
			[ai.tactics.ESCAPE] = 6
		},
	spell	= split_monster_player
	(
		function(source)
			if (player.free_act) then
				message("You are unaffected!")
			elseif (rng(100) < player.skill_sav) then
				msg_format("You resist the effects!")
			else
				timed_effect.inc(timed_effect.PARALYZED,
								 rng(get_cast_level(50)) + 4)
			end
		end,
		function(source, m_ptr, m_name, source_idx, m_idx)
			local r_ptr = race_info(m_ptr)
			if (has_flag(r_ptr, FLAG_UNIQUE)) or
				(has_flag(r_ptr, FLAG_FREE_ACT)) then
				monster_msg(strcap(m_name).." is unaffected.", m_ptr.ml)
			elseif monster_saving_throw(source, m_ptr) then
				monster_msg(strcap(m_name).." resists.", m_ptr.ml)
			else
				monster_effect.inc(source, m_ptr, monster_effect.PARALYZE,
								   rng(get_cast_level(50)) + 4)
			end
		end
	),
}

add_monster_spell
{
 	name	= "Teleport To",

	action = {
		casterseen = "@Source@ commands @target@ to @source_possessive@ side.",
		player = "@Source@ commands @target@ to @source_possessive@ side.",

		default = ""
	}
	symb_no_rebel = true
	tactical =
	{
		[ai.tactics.ATTACK] = 5
		[ai.tactics.ANNOY] = 4
	},
	spell	= split_monster_player
	(
		function(source)
			teleport_player_to(source.fy, source.fx)
		end,
		function(source, m_ptr, m_name, source_idx, m_idx)
			teleport_monster_to(m_idx, source.fy, source.fx)
		end
	),
}

add_monster_spell
{
	name	= "Teleport Level",

	action = {
		casterunseen = "",
		hand         = "@Source@ gestures at a spot below @target@.",
		nohand_head  = "@Source@ stares at a spot below @target@.",
		default = ""
	}
	tactical =
	{
		[ai.tactics.ATTACK] = 5
		[ai.tactics.ANNOY] = 4
		[ai.tactics.ESCAPE] = 6
	}

	spell	= split_monster_player
	(
		function(source)
			if (player.resist_nexus) then
				message("You are unaffected!")
			elseif (rng(100) < player.skill_sav) then
				message("You resist the effects!")
			else
				teleport_player_level()
			end
		end,
		function(source, m_ptr, m_name, source_idx, m_idx)
			-- DGDGDGDG implement me
		end
	),
}

add_monster_spell
{
	name	= "Forget",
	action	= "@Source@ tries to blank @target_possessive@ mind.",
	tactical =
	{
		[ai.tactics.ANNOY] = 8
	},
	spell	= split_monster_player
	(
		function(source)
			if (rng(100) < player.skill_sav) then
				message("You resist the effects!")
			elseif (lose_all_info()) then
				message("Your memories fade away.")
			end
		end,
		function(source, m_ptr, m_name, source_idx, m_idx)
			-- DGDGDGDG implement me
		end
	),
}

-- Global breathing function
function monster_breath(type, damage, div)
	local ret, dir = get_aim_dir()
	if not ret then return SPELL_NOTHING end

	fire_cone(type, dir, get_caster_damage(damage, div), 2)
	return true
end

-- Functions to automate breath, ball and bolt spells and add trap info
-- for balls and bolts. trap damage is now scaled with level, and may be
-- too powerful at high depths. I have generally made max trap damage
-- equal to max breath damage of the same type, with exception to some
-- of the irresistable types which are more powerful as traps than as
-- breaths. no irresistable traps do more than 600 damage.

SCHOOL_MONSTER_STORM = add_school
{
	name   = "Monster storm spells",
	memory = "invoke storms of",
	skill  = "SKILL_MONSTER",
}

function make_storm(type)
	local name = strcap(get_dam_type_info(type, "desc"))
	add_monster_spell
	{
		school = SCHOOL_MONSTER_STORM,
		name   = name .. " Storm",
		memory = get_dam_color_desc(type),
		action = {
			-- Player is blind, and monster is casting spell at
			-- another monster.
			blind_target = "",

			default = "@Source@ invokes a "..name.." storm upon @target@.",
		}
		tactical = {[ai.tactics.ATTACK] = 9},
		spell	= function()
			local ret, dir = get_aim_dir()
			if not ret then return SPELL_NOTHING end
			local dam = get_cast_level(50) * 5 + rng.roll(10, 10)
			fire_ball(%type, dir, dam, 4)
			return true
		end,
	}
end

SCHOOL_MONSTER_BREATH = add_school
{
	name   = "Monster breath spells",
	memory = "breathe",
	skill  = "SKILL_MONSTER",
}

function make_breath(type)
	local name = get_dam_type_info(type, "desc")
	add_monster_spell
	{
		school    = SCHOOL_MONSTER_BREATH,
		antimagic = FALSE,
		name      = "Breathe "..name,
		memory    = get_dam_color_desc(type),
		action    = {
			-- Player is blind, and monster is casting spell at
			-- another monster.
			blind_target = "",

			default = "@Source@ breathes "..name.." at @target@.",
		}
		tactical = {[ai.tactics.ATTACK] = 9},
		spell	= function()
			local ret, dir = get_aim_dir()
			if not ret then return SPELL_NOTHING end
			local dam = get_cast_level(1000)
			fire_cone(%type, dir, (dam / 3) + rng((2 * dam) / 3), 1 + get_cast_level(3))
			return true
		end,
	}
end

SCHOOL_MONSTER_BOLT = add_school
{
	name   = "Monster bolt spells",
	memory = "produces bolts of",
	skill  = "SKILL_MONSTER",
}

function make_bolt(type, tactical)
	local name = get_dam_type_info(type, "desc")
	add_monster_spell
	{
		school = SCHOOL_MONSTER_BOLT,
		name   = "Bolt of "..name,
		memory = get_dam_color_desc(type),
		tactical = tactical or {}
		action = {
			-- Player is blind, and monster is casting spell at
			-- another monster.
			blind_target = "",

			default = "@Source@ casts a "..name.." bolt at @target@.",
		}
		tactical = {[ai.tactics.ATTACK] = 8},
		spell	= function()
				local ret, dir = get_aim_dir()
				if not ret then return SPELL_NOTHING end
				fire_bolt(%type, dir, rng.roll(get_cast_level(50), 8))
				return true
				end
	}
end

SCHOOL_MONSTER_BALL = add_school
{
	name   = "Monster ball spells",
	memory = "produces balls of",
	skill  = "SKILL_MONSTER",
}

function make_ball(type)
	local name = get_dam_type_info(type, "desc")
	add_monster_spell
	{
		school = SCHOOL_MONSTER_BALL,
		name   = "Ball of "..name,
		memory = get_dam_color_desc(type),
		action = {
			-- Player is blind, and monster is casting spell at
			-- another monster.
			blind_target = "",

			default = "@Source@ casts a "..name.." ball at @target@.",
		}
		tactical = {[ai.tactics.ATTACK] = 9},
		spell	= function()
			local ret, dir = get_aim_dir()
			if not ret then return SPELL_NOTHING end
			local dam = get_cast_level(700)
			fire_ball(%type, dir, rng(dam / 2) + (dam / 2), 2)
			return true
			end
	}
end



-- All the various breaths
make_breath(dam.ACID)
make_breath(dam.CHAOS)
make_breath(dam.COLD)
make_breath(dam.CONFUSION)
make_breath(dam.DARK)
make_breath(dam.DARK_FIRE)
make_breath(dam.DISENCHANT)
make_breath(dam.ELEC)
make_breath(dam.FIRE)
make_breath(dam.FORCE)
make_breath(dam.GRAVITY)
make_breath(dam.ICE)
make_breath(dam.INERTIA)
make_breath(dam.LITE)
make_breath(dam.MANA)
make_breath(dam.NEXUS)
make_breath(dam.SUN_FIRE)
make_breath(dam.POIS)
make_breath(dam.SHARDS)
make_breath(dam.SOUND)
make_breath(dam.TIME)
make_breath(dam.VALINOREAN_FIRE)
make_breath(dam.WATER)

-- All the various bolts
make_bolt(dam.ACID)
make_bolt(dam.CHAOS)
make_bolt(dam.COLD)
make_bolt(dam.CONFUSION)
make_bolt(dam.DARK)
make_bolt(dam.DARK_FIRE, { [ai.tactics.ATTACK] = 9 [ai.tactics.ANNOY] = 1 })
make_bolt(dam.DISENCHANT)
make_bolt(dam.ELEC)
make_bolt(dam.FIRE)
make_bolt(dam.FORCE)
make_bolt(dam.GRAVITY)
make_bolt(dam.ICE)
make_bolt(dam.INERTIA)
make_bolt(dam.LITE)
make_bolt(dam.MANA)
make_bolt(dam.NEXUS)
make_bolt(dam.SUN_FIRE)
make_bolt(dam.POIS)
make_bolt(dam.SHARDS)
make_bolt(dam.SOUND)
make_bolt(dam.TIME)
make_bolt(dam.VALINOREAN_FIRE)
make_bolt(dam.WATER)

-- All the various balls
make_ball(dam.ACID)
make_ball(dam.CHAOS)
make_ball(dam.COLD)
make_ball(dam.CONFUSION)
make_ball(dam.DARK)
make_ball(dam.DARK_FIRE)
make_ball(dam.DISENCHANT)
make_ball(dam.ELEC)
make_ball(dam.FIRE)
make_ball(dam.FORCE)
make_ball(dam.GRAVITY)
make_ball(dam.ICE)
make_ball(dam.INERTIA)
make_ball(dam.LITE)
make_ball(dam.MANA)
make_ball(dam.NEXUS)
make_ball(dam.SUN_FIRE)
make_ball(dam.POIS)
make_ball(dam.SHARDS)
make_ball(dam.SOUND)
make_ball(dam.TIME)
make_ball(dam.VALINOREAN_FIRE)
make_ball(dam.WATER)

-- All the various storms
make_storm(dam.CHAOS)
make_storm(dam.DARK)
make_storm(dam.MANA)

-- Summon spells
function make_summon(name, desc, num, minlvl, blvl, alvl, diff, proba, typ)
	add_monster_spell
	{
		name	= "Summon "..name,
		action	= "@Source@ magically summons "..desc.."!",
		tactical = {[ai.tactics.SUMMON] = 10},
		spell	= function()
			local ret, x, y = tgt_pt()
			if not ret then return SPELL_NOTHING end

			local faction
			if spell_caster == WHO_PLAYER then
				-- Make sure we have line of sight
				if not los(y, x, player.py, player.px) then
					message("You must have line-of-sight for this " ..
							"spell's target.")
					return SPELL_NOTHING
				end

				if __monst_helper.helper and __monst_helper.rebelling then
					faction =
						factions.get_hostile_faction(__monst_helper.helper)
				else
					faction = player.faction
				end
			elseif spell_caster < 0 then
				faction = FACTION_DUNGEON
			else
				faction = monster(spell_caster).faction
			end

			-- If a summon spell is cast by a monster, then it shouldn't
			-- summon a monster which would ordinarily be hostile to
			-- the monster, only to then force it into the monster's
			-- faction.  On the other hand, if the spell is cast by
			-- the player, it's okay to summon monsters normally hostile
			-- to the player and then force them to be friendly.
			if not (faction == player.faction and __monst_helper.rebelling)
			then
				if not %typ.flags_forbid then
					%typ.flags_forbid = {}
				end

				%typ.flags_forbid.FRIENDLY = true
				%typ.flags_forbid.PET      = true

				%typ.min_friend = 1
			end

			for i = 1, %num do
				local ret, summoned =
					summon_monster(y, x, get_cast_level(50),
								   faction, %typ)

				-- Force friendliness to player if player did summoning
				if spell_caster == WHO_PLAYER and
					not __monst_helper.rebelling
				then
					for j = 1, getn(summoned) do
						factions.change_faction(summoned[j],
												FACTION_PLAYER)
					end
				end
			end -- for i = 1, %num do
			return true
		end
	}
end

make_summon("Undead","an undead adverary",1,10,15,120, 4,25,
			{flags = {UNDEAD = true}})
make_summon("Undead2","undead",6,20,15,105, 7,20,
			{flags = {UNDEAD = true}})
make_summon("High Undead","undead",6,30,15,105, 7,10,
			{flags = {UNDEAD = true}, chars = "LVW", allow_uniques = true})
make_summon("Demon","a demon",1,10,20,120, 7,25,
			{flags = {DEMON = true}})
make_summon("Demons","demons",6,20,15,105, 7,20,
			{flags = {DEMON = true}})
make_summon("High Demons","demons",6,30,15,105, 7,10,
			{flags = {DEMON = true}, chars = "U"})
make_summon("Dragons","dragons",6,20,15,105, 7,20,
			{flags = {DRAGON = true}})
make_summon("High Dragons","dragons",6,30,15,105, 7,10,
			{flags = {DRAGON = true}, chars = "D", allow_uniques = true})
make_summon("Monster","a monster",1, 2, 7,130, 2,50, {})
make_summon("Monsters","monsters",8,30,10,100, 5,20, {})
make_summon("Ants","ants",6,11, 1,100, 5,40,
			{chars = "a"})
make_summon("Animal(1)","an animal",1, 1, 6,140, 5,50,
			{flags = {ANIMAL = true}})
make_summon("Animals","animals",6,11, 1,100, 5,40,
			{flags = {ANIMAL = true}})
make_summon("Angels","angels",6,11, 1,100, 5,40,
			{chars = "A"})
make_summon("Elemental","elemental",6,11, 1,100, 5,40,
			{chars = "E"})
make_summon("Symbiote","symbiotic monsters",6,11, 1,100, 5,40,
			{flags = {SYMBIOTE = true}})
make_summon("Hydra","hydras",6,40,10,100, 7,10,
			{chars = "M"})
make_summon("Plant","Plants",6,40,10,100, 7,10,
			{chars = "#"})
make_summon("Spiders","spiders",6,30,10,100, 7,10,
			{chars = "S"})
make_summon("Hounds","hounds",6,30,10,100, 7,20,
			{chars = "CZ"})
make_summon("Thunderlords","thunderlords",6,30,10,100, 7,20,
			{chars = "B", flags = {THUNDERLORD = true}})
make_summon("Wraith","a wraith",1,20,20,120,10,20,
			{flags = {WRAITH = true, UNIQUE = true}})
make_summon("Wraiths","wraiths",6,35,15,105,10,10,
			{flags = {WRAITH = true, UNIQUE = true}})
make_summon("Unique","a unique adversary",1,10,10,130, 3,10,
			{flags = {UNIQUE = true}})
make_summon("Uniques","uniques",6,50, 1,100, 3, 3,
			{flags = {UNIQUE = true}})


-- "Summon Kin" is special, since the type of monster it summons
-- varies based on who does the summoning.
add_monster_spell
{
	name	= "Summon Kin",
	action	= "@Source@ magically summons @source_possessive@ kin!",
	tactical = {[ai.tactics.SUMMON] = 10},
	spell	=
		function()
			local ret, x, y = tgt_pt()
			if not ret then return SPELL_NOTHING end
			local faction, char
			local typ = {}

			if spell_caster == WHO_PLAYER then
				assert(__monst_helper.helper,
					   "set_monst_helper() must be called to use " ..
						   "Summon Kin")
				-- Make sure we have line of sight
				if not los(y, x, player.py, player.px) then
					message("You must have line-of-sight for this " ..
							"spell's target.")
					return SPELL_NOTHING
				end

				char = strchar(__monst_helper.helper.d_char)
				if __monst_helper.rebelling then
					faction =
						factions.get_hostile_faction(__monst_helper.helper)
					typ.min_friend = 1
				else
					faction = player.faction
				end
			elseif spell_caster < 0 then
				faction = FACTION_DUNGEON
				error("Summon kin by negative factions not supported")
			else
				faction        = monster(spell_caster).faction
				char           = strchar(monster(spell_caster).d_char)
				typ.min_friend = 1
			end

			typ.chars = char

			for i = 1, 11 do
				local ret, summoned =
					summon_monster(y, x, get_cast_level(50), faction, typ)

				-- Force friendliness to player if player did summoning
				if spell_caster == WHO_PLAYER and
					not __monst_helper.rebelling
				then
					for j = 1, getn(summoned) do
						factions.change_faction(summoned[j],
												FACTION_PLAYER)
					end
				end
			end
			return true
		end
}
