--
-- Configure the automatic creation of temporary resists for every
-- damage type we make
--
dam.on_damage_creation = function(t)
	timed_effect.create
	{
		name = "RES_"..strupper(gsub(t.desc, "( )", "_"))
		desc = "Resist "..t.desc
		type = "magical"
		status = "beneficial"
		parameters = { {"power",66} }
		on_gain = "You feel resistant to "..t.desc.."!"
		on_lose = "You feel less resistant to "..t.desc.."."
		bonus = function()
			local t = %t
			player.resists[t.index] = 100 - (((100 - (player.resists[t.index] or 0)) * (100 - timed_effect.get("power"))) / 100)
		end
	}
end

function dam.res.get_temp_name(dam_type)
	local desc = get_dam_type_info(dam_type, "desc")
	return "RES_" .. strupper(gsub(desc, "( )", "_"))
end

function dam.res.sum(list)
	local high_res  = 0
	local high_vuln = 0

	for i = 1, getn(list) do
		if list[i] > high_res then
			high_res = list[i]
		elseif list[i] < high_vuln then
			high_vuln = list[i]
		end
	end

	return dam.res.stack({high_res, high_vuln})
end

-------------------------------------------------------
--------------------- Pure damage ---------------------
-------------------------------------------------------

dam.PURE = dam.add
{
	gfx	       = 0,
	desc       = "damage",
	color      = { color.SLATE },
	text_color = "#w",
}

-------------------------------------------------------
-------------------- Melee damages --------------------
-------------------------------------------------------
dam.SLASH = dam.add
{
	gfx        = 0,
	desc       = "slashing",
	color      = { color.SLATE },
	text_color = "#w",
}
dam.CRUSH = dam.add
{
	gfx	    = 0,
	desc    = "crushing",
	color   = { color.SLATE },
	text_color = "#w",
}
dam.PIERCE = dam.add
{
	gfx        = 0,
	desc       = "piercing",
	color      = { color.SLATE },
	text_color = "#w",
}

-------------------------------------------------------
--------------------- Base damages --------------------
-------------------------------------------------------

-- Grid fire damage is its own function so it can be re-used by
-- dark fire

dam.FIRE = dam.add
{
	gfx	       = 0,
	desc       = "fire",
	color      = function() return iif(rng(6)<4, color.YELLOW, iif(rng(4)==1, color.RED, color.LIGHT_RED)) end,
	text_color = "#r",
	grid       = function(state)
		local y, x = state.y, state.x
		local c_ptr = cave(y, x)
		local f_ptr = f_info[c_ptr.feat + 1]

		if cave_feat_is(c_ptr, FLAG_PERMANENT) then return nil end

		if f_ptr.flags[FLAG_DEAD_TREE_FEAT] then
			cave_set_feat(y, x, f_ptr.flags[FLAG_DEAD_TREE_FEAT])
			-- Silly thing to destroy trees when you worship yavanna
			god.inc_piety(god.YAVANNA, -50)
		elseif c_ptr.feat == FEAT_ICE then
			local k = rng(100)
			if k < 10 then cave_set_feat(y, x, FEAT_DIRT)
			elseif k < 30 then temporary_terrain.set(y, x, FEAT_POND_WATER, rng(5,15))
			end
		elseif cave_feat_is(c_ptr, FLAG_FLOOR) then
			local k = rng(100)
			if k < 10 then temporary_terrain.set(y, x, FEAT_BURNING_GROUND, rng(5,15), FEAT_ASH)
			elseif k < 25 then cave_set_feat(y, x, FEAT_ASH, 20)
			end
		elseif c_ptr.feat == FEAT_SANDWALL or
			c_ptr.feat == FEAT_SANDWALL_H or
			c_ptr.feat == FEAT_SANDWALL_K then
			local k = rng(100)
			if k < 30 then
				cave_set_feat(y, x, FEAT_GLASS_WALL)
				player.update = (player.update | PU_VIEW | PU_MONSTERS | PU_MON_LITE)
			end
		end
	end
	object = function(state)
		if rng.percent(-state.resist) then
			if state.o_ptr.artifact_id == 0 then
				return { kill=true, note_kill=" burn@s@ up!" }
			end
		end
	end,
	player	= function(state)
		dam.destroy_items(state.dam_type, iif(state.dam < 30, 1, iif(state.dam < 60, 2, 3)))
		return { fuzzy=state.fuzzy or "You are hit by fire!", obvious=true }
	end,
}

dam.COLD = dam.add
{
	color      = function() return iif(rng(6)<4, color.WHITE, color.LIGHT_WHITE) end,
	text_color = "#W",
	gfx        = 1,
	desc       = "cold",
	grid       = function(state)
		local y, x = state.y, state.x
		local chance = 40
		if dam.aux.boring_grid(y, x) and rng.percent(chance) then
			if cave(y, x).feat ~= FEAT_ICE then
				if rng.percent(70) then
					temporary_terrain.set(y, x, FEAT_ICE_SHARDS, rng(5,15), rng.percent(10) and FEAT_POND_WATER or nil)
				else
					temporary_terrain.set(y, x, FEAT_FROZEN_GROUND, rng(5,15))
				end
			end
		end
	end,
	object = function(state)
		if rng.percent(-state.resist) then
			if state.o_ptr.artifact_id == 0 then
				return { kill=true, note_kill=" shatter@s@!" }
			end
		end
	end,
	player	= function(state)
		dam.destroy_items(state.dam_type, iif(state.dam < 30, 1, iif(state.dam < 60, 2, 3)))
		return { fuzzy=state.fuzzy or "You are hit by cold!", obvious=true }
	end,
}

dam.ACID = dam.add
{
	color      = function() return iif(rng(5)<3, color.YELLOW, color.LIGHT_GREEN) end,
	text_color = "#G",
	gfx        = 4,
	desc       = "acid",
	grid       = function(state)
		local y, x = state.y, state.x
		local c_ptr = cave(y, x)
		local f_ptr = f_info[c_ptr.feat + 1]

		if cave_feat_is(c_ptr, FLAG_PERMANENT) then return nil end
		if f_ptr.flags[FLAG_DEAD_TREE_FEAT] then
			cave_set_feat(y, x, f_ptr.flags[FLAG_DEAD_TREE_FEAT])
			-- Silly thing to destroy trees when you worship yavanna
			god.inc_piety(god.YAVANNA, -50)
		end
	end,
	object = function(state)
		if rng.percent(-state.resist) then
			if state.o_ptr.artifact_id == 0 then
				return {kill=true, note_kill=" melt@s@!" }
			end
		end
	end,
	player	= function(state)
		dam.destroy_items(state.dam_type, iif(state.dam < 30, 1, iif(state.dam < 60, 2, 3)))
		return { fuzzy=state.fuzzy or "You are hit by acid!", obvious=true }
	end,
}

dam.ELEC = dam.add
{
	color      = function() return iif(rng(7)<6, color.WHITE, iif(rng(4)==1, color.BLUE, color.LIGHT_BLUE)) end,
	text_color = "#b",
	gfx	       = 2,
	desc       = "electricity",
	grid       = function(state)
		local y, x = state.y, state.x
		local c_ptr = cave(y, x)
		local f_ptr = f_info[c_ptr.feat + 1]

		if cave_feat_is(c_ptr, FLAG_PERMANENT) then return nil end
		if f_ptr.flags[FLAG_DEAD_TREE_FEAT] then
			cave_set_feat(y, x, f_ptr.flags[FLAG_DEAD_TREE_FEAT])
			-- Silly thing to destroy trees when you worship yavanna
			god.inc_piety(god.YAVANNA, -50)
		end
	end,
	object = function(state)
		if rng.percent(-state.resist) then
			if state.o_ptr.artifact_id == 0 then
				return {kill=true, note_kill=" melt@s@!" }
			end
		end
	end,
	player	= function(state)
		dam.destroy_items(state.dam_type, iif(state.dam < 30, 1, iif(state.dam < 60, 2, 3)))
		return { fuzzy=state.fuzzy or "You are hit by electricity!", obvious=true }
	end,
}

-- Poison is split into two parts, pure damage and pure lasting
-- The idea for a new poison system would probably be better, but
-- that needs engine changes
-- http://wiki.t-o-m-e.net/IdeaArchive_2fA_20little_20bit_20poisoned_3f

dam.POISONING = dam.add
{
	color      = function() return iif(rng(5)<3, color.LIGHT_GREEN, color.GREEN) end,
	text_color = "#g",
	gfx        = 3,
	desc       = "slow poison",
	monster    = function(state)
		local time, power = state.dam, 1

		if time > 100 then    power = time / 100; time = 100
		elseif time > 10 then power = max(3, time / 10); time = 10 + (time / 5) end

		local cnt, pow = monster_effect.get(state.m_ptr, monster_effect.POISON)
		monster_effect(state.who, state.m_ptr, monster_effect.POISON, (cnt or 0) + time, max(power, pow or 0))
		return { dam=0 }
	end,
	player     = function(state)
		local time, power = state.dam, 1

		if time > 100 then    power = time / 100; time = 100
		elseif time > 10 then power = max(3, time / 10); time = 10 + (time / 5) end

		timed_effect(timed_effect.POISON, (timed_effect.get(timed_effect.POISON) or 0) + time, max(power, timed_effect.get(timed_effect.POISON, "power") or 0))
		return { dam=0, fuzzy=state.fuzzy or "You are hit by poison!" }
	end,
}

dam.POISDAM = dam.add
{
	color      = function() return iif(rng(5)<3, color.LIGHT_GREEN, color.GREEN) end,
	text_color = "#g",
	gfx        = 3,
	desc       = "fast poison",
	player     = function(state)
		return { fuzzy=state.fuzzy or "You are hit by poison!", obvious=true }
	end,
}

dam.POIS = dam.combine({dam.POISDAM, dam.POISONING}, false, false, "poison", nil,
			function()
				return iif(rng(5)<3, color.LIGHT_GREEN, color.GREEN)
			end)

-- Make the damages implicit after combining them
dam.POISONING = dam.implicit_resist(dam.POISONING, {dam.POIS})
dam.POISDAM = dam.implicit_resist(dam.POISDAM, {dam.POIS})

-------------------------------------------------------
------------------ "Annoying" damages -----------------
-------------------------------------------------------

dam.CONFUSION = dam.reserve()
dam.CONFUSE = dam.add
{
	gfx	       = 0,
	desc	   = "confusion",
	color	   = {color.RED, color.GREEN, color.BLUE, color.YELLOW},
	text_color = "#U",
	color_desc = "#Wc#so#on#Bf#Ru#Gs#yi#Uo#Rn#w",
	monster    = function(state)
		return{dam = 0, conf = state.dam}
	end
	player	= function(state)
		local time = state.dam

		if time > 100 then time = 100
		elseif time > 10 then time = 10 + (time / 5) end

		if state.resist < 50 and not rng.percent(state.resist * 2) then
			timed_effect.inc(timed_effect.CONFUSED, time)
		end

		return { dam=0, obvious=true,
			fuzzy=state.fuzzy or "You are hit by something very confusing!"}
	end,
}
dam.CONFUSEDAM = dam.add
{
	gfx	       = 0,
	desc	   = "confusion damage",
	color	   = {color.RED, color.GREEN, color.BLUE, color.YELLOW},
	text_color = "#U",
	player	= function(state)
		return { obvious=true,
			fuzzy=state.fuzzy or "You are hit by something very confusing!"}
	end,
}
dam.combine({dam.CONFUSE, dam.CONFUSEDAM}, true, false, "confusion",
			dam.CONFUSION,
			{color.RED, color.GREEN, color.BLUE, color.YELLOW})

dam.FEAR = dam.add
{
	gfx	        = 0,
	desc	    = "fear",
	text_color  = "#s",
	color	    = { color.SLATE },
	bypass_symb = true

	monster = function(state)
		local time = state.dam

		if time > 100 then time = 100
		elseif time > 10 then time = 10 + (time / 5) end

		return { dam=0 fear=time }
	end,
	player	   = function(state)
		local time = state.dam

		if time > 100 then time = 100
		elseif time > 10 then time = 10 + (time / 5) end

		timed_effect.inc(timed_effect.AFRAID, time)
		return { dam=0, obvious=true,
			fuzzy=state.fuzzy or "You are hit by something fearful!"}
	end,
}

dam.CUT = dam.add
{
	gfx	        = 0,
	desc	    = "bleeding",
	color	    = { color.RED },
	text_color  = "#r",
	bypass_symb = true

	monster = function(state)
		local time, power = state.dam, 1

		if time > 100 then    power = time / 100; time = 100
		elseif time > 10 then power = max(3, time / 10); time = 10 + (time / 5) end

		local cnt, pow = monster_effect.get(state.m_ptr, monster_effect.CUT)
		monster_effect(state.who, state.m_ptr, monster_effect.CUT, (cnt or 0) + time, max(power, pow or 0))
		return { dam=0 }
	end
	player	= function(state)
		local time, power = state.dam, 1

		if state.resist >= 50 and rng.percent(state.resist * 2) then
			return {dam=0, obvious=true}
		end

		if time > 100 then    power = time / 100; time = 100
		elseif time > 10 then power = max(3, time / 10); time = 10 + (time / 5) end

		timed_effect(timed_effect.CUT,
					 (timed_effect.get(timed_effect.CUT) or 0) + time,
					 max(power, timed_effect.get(timed_effect.CUT, "power") or 0))
		return { dam=0, obvious=true,
			fuzzy=state.fuzzy or "You are hit by something sharp!" }
	end,
}

dam.STUN = dam.add
{
	gfx	        = 0,
	desc	    = "stunning",
	color	    = { color.UMBER },
	text_color  = "#u",
	bypass_symb = true

	monster	   = function(state)
		local time, power = state.dam, 1

		if time > 100 then    power = time / 100; time = 100
		elseif time > 10 then power = max(5, time / 10); time = 10 + (time / 5) end

		local cnt, pow = monster_effect.get(state.m_ptr, monster_effect.STUN)
		monster_effect(state.who, state.m_ptr, monster_effect.STUN, (cnt or 0) + time, max(power, pow or 0))
		return { dam=0 }
	end,
	player	= function(state)
		local time, power = state.dam, 1

		if state.resist >= 50 and rng.percent(state.resist * 2) then
			return {dam=0, obvious=true}
		end

		if time > 100 then    power = time / 100; time = 100
		elseif time > 10 then power = max(3, time / 10); time = 10 + (time / 5) end

		timed_effect(timed_effect.STUN,
					 (timed_effect.get(timed_effect.STUN) or 0) + time,
					 max(power, timed_effect.get(timed_effect.STUN, "power") or 0))
		return { dam=0, obvious=true,
			fuzzy=state.fuzzy or "You are hit by something stunning!", }
	end,
}

dam.SLOW = dam.add
{
	gfx	        = 0,
	desc	    = "slowness",
	color	    = { color.UMBER },
	text_color  = "#u",
	bypass_symb = true

	monster	   = function(state)
		local time, power = state.dam, 1

		if time > 100 then    power = time / 100; time = 100
		elseif time > 10 then power = max(5, time / 10); time = 10 + (time / 5) end

		local cnt, pow = monster_effect.get(state.m_ptr, monster_effect.SLOW)
		monster_effect(state.who, state.m_ptr, monster_effect.SLOW, (cnt or 0) + time, max(power, pow or 0))
		return { dam=0 }
	end,
	player	= function(state)
		local time, power = state.dam, 1

		if state.resist >= 50 and rng.percent(state.resist * 2) then
			return {dam=0, obvious=true}
		end

		if time > 100 then    power = time / 100; time = 100
		elseif time > 10 then power = max(3, time / 10); time = 10 + (time / 5) end

		timed_effect(timed_effect.SLOW,
					 (timed_effect.get(timed_effect.SLOW) or 0) + time,
					 max(power, timed_effect.get(timed_effect.SLOW, "power") or 0))
		return { dam=0, obvious=true,
			fuzzy=state.fuzzy or "You are hit by something quite slow!", }
	end,
}

dam.BLIND = dam.add
{
	gfx	        = 0,
	desc	    = "blindness",
	color	    = { color.UMBER },
	text_color  = "#w",
	bypass_symb = true

	monster	   = function(state)
		return { dam=0 }
	end,
	player	= function(state)
		-- Blind ?
		if (state.resist < 100 and not rng.percent(state.resist)) then
			timed_effect(timed_effect.BLIND, rng(3, 7))
		end

		return { dam=0, obvious=true }
	end,
}

-------------------------------------------------------
-------------------- General damages ------------------
-------------------------------------------------------

-- Light and darkness
dam.LITE = dam.add
{
	color      = function() return iif(rng(4) == 1, color.ORANGE, color.YELLOW) end,
	text_color = "#y"
	gfx        = 1,
	desc       = "lite",
	grid       = function(state)
		local y, x = state.y, state.x
		-- Turn on the light
		cave(y, x).info = cave(y, x).info | CAVE_GLOW

		-- Notice & redraw
		if cave_plain_floor(y, x) and player_has_los_bold(y, x) then
			cave(y, x).info = cave(y, x).info | CAVE_MARK
		end

		note_spot(y, x)
		lite_spot(y, x)

		player.update = player.update | PU_VIEW
		--player.redraw[FLAG_PR_MAP] = 1
	end,
	monster	= function(state)
		if state.resist < 0 then
			return { msg=" cringes from the light!",
				msg_die=" shrivels away in the light!" }
		end
	end,
	player	= function(state)
		-- Blind ?
		if not player.has_intrinsic(FLAG_BLIND) then
			dam.call(dam.BLIND, "player", state)
		end
		if state.resist < 0 then
			dam.add_msg(state, "The light scorches your flesh!")
			return { fuzzy=state.fuzzy or "You are hit by light!",
				obvious=true}
		end

		if player.has_intrinsic(FLAG_BLIND) then
			return { fuzzy=state.fuzzy or "You are hit by something!" }
		else
			return { fuzzy=state.fuzzy or "You are hit by light!",
				obvious=true}
		end
	end,
}
dam.DARK = dam.add
{
	color      = function() return iif(rng(4) == 1, color.DARK, color.LIGHT_DARK) end,
	text_color = "#D",
	gfx        = 1,
	desc       = "darkness",
	grid       = function(state)
		local y, x = state.y, state.x
		-- Turn off the light
		cave(y, x).info = cave(y, x).info & negate_bits(CAVE_GLOW)
		if cave_plain_floor(y, x) then
			cave(y, x).info = cave(y, x).info & negate_bits(CAVE_MARK)
			note_spot(y, x)
		end

		-- Notice & redraw
		lite_spot(y, x)

		player.update = player.update | PU_VIEW
	end,
	player	= function(state)
		-- Blind ?
		if not player.has_intrinsic(FLAG_BLIND) then
			dam.call(dam.BLIND, "player", state)
		end
		if player.has_intrinsic(FLAG_BLIND) then
			return { fuzzy=state.fuzzy or "You are hit by something!" }
		else
			return { fuzzy=state.fuzzy or "You are hit by darkness!",
				obvious=true}
		end
	end,
}

dam.SOUNDDAM = dam.add
{
	color      = function()
					 return iif(rng(4) == 1, color.VIOLET, color.LIGHT_WHITE)
				 end,
	text_color = "#W",
	gfx        = 1,
	desc       = "sound damage",
	player     = function(state)
		return { fuzzy=state.fuzzy or "You are hit by a loud noise!" }
	end,

}
dam.SOUND = dam.combine({dam.SOUNDDAM, dam.STUN}, true, false, "sound", nil,
			function()
				return iif(rng(4) == 1, color.DARK, color.LIGHT_DARK)
			end)

dam.SOUNDDAM = dam.implicit_resist(dam.SOUNDDAM, {dam.SOUND})


dam.SHARDSDAM = dam.add
{
	color      = function()
					 return iif(rng(4) == 1, color.UMBER, color.LIGHT_UMBER)
				 end,
	text_color = "#D",
	gfx        = 1,
	desc       = "shards damage",
	grid       = function(state)
		local y, x = state.y, state.x
		local c_ptr = cave(y, x)
		local f_ptr = f_info[c_ptr.feat + 1]

		if cave_feat_is(c_ptr, FLAG_PERMANENT) then return nil end

		if f_ptr.flags[FLAG_DEAD_TREE_FEAT] then
			cave_set_feat(y, x, f_ptr.flags[FLAG_DEAD_TREE_FEAT])
			-- Silly thing to destroy trees when you worship yavanna
			god.inc_piety(god.YAVANNA, -50)
		end
	end,
	player     = function(state)
		return { fuzzy=state.fuzzy or "You are hit by a something sharp!" }
	end,

}
dam.SHARDS = dam.combine({dam.SHARDSDAM, dam.CUT}, true, false, "shards", nil,
			function()
				return iif(rng(4) == 1, color.UMBER, color.LIGHT_UMBER)
			end)

dam.SHARDSDAM = dam.implicit_resist(dam.SHARDSDAM, {dam.SHARDS})

-- XXX Sound resistance prevents you from being pushed back???
dam.FORCE = dam.add
{
	color      = function() return iif(rng(5)<3, color.LIGHT_WHITE, color.ORANGE) end,
	text_color = "#W",
	gfx        = 1,
	desc       = "force",
	grid       = function(state)
		local y, x = state.y, state.x
		local c_ptr = cave(y, x)
		local f_ptr = f_info[c_ptr.feat + 1]

		if cave_feat_is(c_ptr, FLAG_PERMANENT) then return nil end

		if f_ptr.flags[FLAG_DEAD_TREE_FEAT] then
			cave_set_feat(y, x, f_ptr.flags[FLAG_DEAD_TREE_FEAT])
			-- Silly thing to destroy trees when you worship yavanna
			god.inc_piety(god.YAVANNA, -50)
		end
	end,
	monster = function(state)
		return { move=1 }
	end,
	player	= function(state)
		return { fuzzy=state.fuzzy or "You are hit by a kinetic force!",
			move=1 }
	end,
}

dam.NEXUS = dam.add
{
	color      = function() return iif(rng(5)<3, color.LIGHT_RED, color.VIOLET) end,
	text_color = "#R",
	gfx        = 1,
	desc       = "nexus",
	grid       = function(state)
		local y, x = state.y, state.x
		local c_ptr = cave(y, x)
		local f_ptr = f_info[c_ptr.feat + 1]

		if cave_feat_is(c_ptr, FLAG_PERMANENT) then return nil end

		if f_ptr.flags[FLAG_DEAD_TREE_FEAT] then
			cave_set_feat(y, x, f_ptr.flags[FLAG_DEAD_TREE_FEAT])
			-- Silly thing to destroy trees when you worship yavanna
			god.inc_piety(god.YAVANNA, -50)
		end
	end,
	player = function(state)
		state.fuzzy = state.fuzzy or "You are hit by something strange!"

		if state.resist >= 50 or rng.percent(state.resist * 2) then
			return
		end

		local no_tele = has_flag(level_flags, FLAG_NO_TELEPORT) or
			player.has_intrinsic(FLAG_NO_TELE)

		local chance = rng(7)
		local save   = (rng(100) <= player.skill_sav)

		if chance == 1 then
			-- Swap stats
			if save then
				dam.add_msg(state, "You resist the effects!")
			else
				dam.add_msg(state, "Your body starts to scramble...")
				corrupt_player()
			end
		elseif chance == 2 then
			-- Teleport level
			if not no_tele then
				if save then
					dam.add_msg(state, "You resist the effects!")
				else
					teleport_player_level()
				end
			end
		elseif state.who > 0 then
			if not no_tele then
				if chance <= 4 then
					-- Teleport to monster
					local m_ptr = monster(state.who)
					teleport_player_to(m_ptr.fy, m_ptr.fx)
				else
					-- Teleport away
					teleport_player(200)
				end
			end
		else
			if not no_tele then
				-- Teleport away
				teleport_player(200)
			end
		end
	end,
}

dam.CHAOSDAM = dam.add
{
	color      = function() return rng.range(1, 15) end
	text_color = "#w",
	gfx        = 1,
	desc       = "chaos",
	color_desc = "#oc#Bh#Ra#Go#Us#w",
	grid       = function(state)
		local y, x = state.y, state.x
		local c_ptr = cave(y, x)
		local f_ptr = f_info[c_ptr.feat + 1]

		if cave_feat_is(c_ptr, FLAG_PERMANENT) then return nil end

		if f_ptr.flags[FLAG_DEAD_TREE_FEAT] then
			cave_set_feat(y, x, f_ptr.flags[FLAG_DEAD_TREE_FEAT])
			-- Silly thing to destroy trees when you worship yavanna
			god.inc_piety(god.YAVANNA, -50)
		end
	end,

	monster = function(state)
		if state.resist < 50 and not rng.percent(state.resist * 2) then
			return {poly = true}
		end
	end

	object = function(state)
		if state.o_ptr.artifact_id == 0 then
			return { kill=true, note_kill="@isare@ destroyed!" }
		end
	end

	player = function(state)
		state.fuzzy = state.fuzzy or "You are hit by a wave of anarchy!"

		if state.resist >= 50 or rng.percent(state.resist * 2) then
			return
		end

		local time = state.dam

		if time > 100 then time = 100
		elseif time > 10 then time = 10 + (time / 5) end

		timed_effect.inc(timed_effect.HALLUCINATE, time)
	end,
}
dam.CHAOS = dam.combine({dam.CHAOSDAM, dam.CONFUSION}, false, false, "chaos", nil,
			function()
				return rng.range(1, 15)
			end)

dam.CHAOSDAM = dam.implicit_resist(dam.CHAOSDAM, {dam.CHAOS})

function dam.time_reduce_stat(stat)
	player.stat_cur[stat] = player.stat_cur[stat] * 3 / 4

	if player.stat_cur[stat] < 3 then
		player.stat_cur[stat] = 3
	end
end

dam.TIME = dam.add
{
	color      = function() return iif(rng(2)==1, color.WHITE, color.LIGHT_DARK) end,
	text_color = "#W",
	gfx        = 1,
	desc       = "time",
	grid       = function(state)
		local y, x = state.y, state.x
		local c_ptr = cave(y, x)
		local f_ptr = f_info[c_ptr.feat + 1]

		if cave_feat_is(c_ptr, FLAG_PERMANENT) then return nil end

		if f_ptr.flags[FLAG_DEAD_TREE_FEAT] then
			cave_set_feat(y, x, f_ptr.flags[FLAG_DEAD_TREE_FEAT])
			-- Silly thing to destroy trees when you worship yavanna
			god.inc_piety(god.YAVANNA, -50)
		end
	end
	player = function(state)
		state.fuzzy = state.fuzzy or "You are hit by a blast from the past!"

		if player.flags[FLAG_RES_CONTINUUM] then
			local resist = 100 - (100 * 4 / (6 + rng(6)))

			state.dam, state.resist = dam.apply_resistance(state.dam, resist)

			dam.add_msg(state, "You feel as if time is passing you by.")

			return
		end

		if state.resist >= 50 and rng.percent(state.resist * 2) then
			return
		end

		local chance = rng(10)

		if chance <= 5 then
			dam.add_msg(state, "You feel that life has been clocked back.")

			lose_exp(100 + (player.exp * 2 / 100))
		elseif chance <= 9 then
			local stats = {
				{A_STR, "strong"}, {A_INT, "bright"},
				{A_WIS, "wise"}, {A_DEX, "agile"},
				{A_CON, "hardy"}, {A_CHR, "beautiful"}}

			local stat = stats[rng(getn(stats))]

			dam.add_msg(state, "You're not as " .. stat[2] ..
						" as you used to be...")

			dam.time_reduce_stat(stat[1])

			player.update = player.update | PU_BONUS
			player.redraw[FLAG_PR_STATS]  = true
			player.window[FLAG_PW_PLAYER] = true
		else
			dam.add_msg(state, "You're not as powerful as you used to be...")

			for i = 0, stats.MAX do
				dam.time_reduce_stat(i)
			end
			player.update = player.update | PU_BONUS
			player.redraw[FLAG_PR_STATS]  = true
		end
	end
}

dam.DISENDAM = dam.add
{
	color      = function() return iif(rng(5)~=1, color.LIGHT_BLUE, color.VIOLET) end,
	text_color = "#g",
	gfx        = 1,
	desc       = "disenchantment",
}
dam.UN_BONUS = dam.add
{
	color      = function() return iif(rng(5)~=1, color.LIGHT_BLUE, color.VIOLET) end,
	text_color = "#g",
	gfx        = 1,
	desc       = "disenchantment",

	monster = function(state)
		-- XXX The un_bonus part of disenchantment is unimplemented.
		return {dam = 0}
	end
	player = function(state)
		-- XXX The un_bonus part of disenchantment is unimplemented.
		return {dam = 0}
	end
}
dam.DISENCHANT = dam.combine({dam.DISENDAM, dam.UN_BONUS}, false, false, "disenchantment", nil,
			function()
				return iif(rng(5)~=1, color.LIGHT_BLUE, color.VIOLET)
			end)

dam.DISENDAM = dam.implicit_resist(dam.DISENDAM, {dam.DISENCHANT})
dam.UN_BONUS = dam.implicit_resist(dam.UN_BONUS, {dam.DISENCHANT})

function dam.mon_slow_helper(state)
	local chance
	if state.dam <= 10 then
		chance = 1
	else
		chance = rng(state.dam - 10)
	end

	if (state.m_ptr.level - 10) > chance  then
		state.obvious = false
		return false
	elseif state.m_ptr.mspeed > 60 then
		state.m_ptr.mspeed = state.m_ptr.mspeed - 10
		state.msg = " starts moving slower."
		return true
	end
end

dam.INERTIA = dam.add
{
	color      = function() return iif(rng(5)<3, color.SLATE, color.LIGHT_WHITE) end,
	text_color = "#W",
	gfx        = 1,
	desc       = "inertia",
	monster = function(state)
		dam.mon_slow_helper(state)
	end

	player = function(state)
		state.fuzzy = state.fuzzy or "You are hit by something strange!"

		if state.resist >= 50 or rng.percent(state.resist * 2) then
			return {obvious=false}
		end

		local time  = state.dam / 20
		local power = state.dam / 10

		timed_effect(timed_effect.SLOW,
					 (timed_effect.get(timed_effect.SLOW) or 0) + time,
					 max(power, timed_effect.get(timed_effect.SLOW, "power") or 0))

		return { obvious=true }
	end,
}

dam.GRAVITY = dam.add
{
	color      = function() return iif(rng(3)==1, color.LIGHT_UMBER, color.UMBER) end,
	text_color = "#W",
	gfx        = 1,
	desc       = "gravity",
	monster = function(state)
		local m_ptr       = state.m_ptr
		local resist_tele = false

		if m_ptr.flags[FLAG_RES_TELE] then
			if m_ptr.flags[FLAG_UNIQUE] then
				state.msg = " is unaffected!"
				resist_tele = true
			elseif m_ptr.level > rng(100) then
				state.msg = " resists!"
				resist_tele = true
			end
		end

		-- We get the C code to do the teleportation via seting the
		-- "dist" field of the state
		if not resist_tele then
			state.dist = 10
		end

		if not dam.mon_slow_helper(state) then
			state.dist = 0
		end
	end

	-- Levititing/flying/whatever reduces damage suffered and prevents
	-- player from being slowed or stunned.
	player = function(state)
		state.fuzzy = state.fuzzy or "You are hit by something heavy!"
		if player.has_intrinsic(FLAG_FLY) then
			state.dam = state.dam * 2 / 3
		end

		if has_flag(level_flags, FLAG_NO_TELEPORT) then
			return
		end

		if state.resist >= 50 or rng.percent(state.resist * 2) then
			return {obvious=false}
		end

		teleport_player(5)

		if not player.has_intrinsic(FLAG_FLY) then
			local t_state = dam.aux.state_copy(state)

			t_state.dam = max(35, rng(state.dam / 3 + 5))
			dam.call(dam.STUN, "player", t_state)

			t_state.dam = max(100, rng(state.dam))
			dam.call(dam.INERTIA, "player", t_state)
		end

		return { obvious=true }
	end,
}

-------------------------------------------------------
--------------------- High damages --------------------
-------------------------------------------------------

-- Basically the same as PURE, but defined so that monsters will
-- be described as breathing "mana" rather than "damage", and also
-- so it can have its own colors.
dam.MANA = dam.add
{
	desc       = "mana",
	color      = function() return iif(rng(5) ~= 1, color.VIOLET, color.LIGHT_BLUE) end,
	text_color = "#B",

	object = function(state)
		if state.o_ptr.artifact_id == 0 then
			return { kill=true, note_kill="@isare@ destroyed!" }
		end
	end

	player = function(state)
		state.fuzzy = state.fuzzy or "You are hit by an auro of magic!"
		return {obvious = true}
	end
}

-- Like fire, but can also stun, and not affected by fire resistance.
-- We do this as a derived damage which does "dam.call(dam.STUN, ...)"
-- rather than making it a combined damage of fire and stun so that
-- fire resistance won't work against it.
dam.SUN_FIRE = dam.derive
{
	source = dam.FIRE,
	desc = "Arien fire"
	modifier = function(target, state)
		dam.call(dam.STUN, target, state)

		if player.has_intrinsic(FLAG_BLIND) then
			state.fuzzy = "You are hit by something *HOT*!"
		else
			state.fuzzy   = "You are hit by direct sun fire!"
			state.obvious = true
		end
	end
}

-- Hurt evil more and good less
dam.VALINOREAN_FIRE = dam.derive
{
	source = dam.FIRE,
	desc = "valinorean fire"
	modifier = function(target, state)
		local resist = 0

		if target == "player" then
			if player.has_intrinsic(FLAG_BLIND) then
				state.fuzzy = "You are hit by holy fire!"
			end
		elseif target == "monster" then
			-- Compute how hard it hits
			if state.m_ptr.flags[FLAG_EVIL] then resist = resist - 50 end
			if state.m_ptr.flags[FLAG_UNDEAD] then resist = resist - 50 end
			if state.m_ptr.flags[FLAG_DEMON] then resist = resist - 50 end
			if state.m_ptr.flags[FLAG_GOOD] then resist = resist + 200 end
		end

		-- Reduce damage based on resistance
		state.dam, state.resist = dam.apply_resistance(state.dam, resist)
	end
}

-- Burz-ghash, the dark fire
dam.DARK_FIRE = dam.derive
{
	source = dam.DARK,
	desc = "darkfire"
	modifier = function(target, state)
		local resist = 0

		if target == "grid" or target == "object" then
			-- Damage grid and objects like fire
			dam.call(dam.FIRE, target, state)
		elseif target == "player" then
			if player.has_intrinsic(FLAG_BLIND) then
				state.fuzzy   = "You are hit fire!"
				state.obvious = false
			else
				state.fuzzy   = "You are hit by dark fire!"
				state.obvious = true
			end
		elseif target == "monster" then
			-- Compute how hard it hits
			if state.m_ptr.flags[FLAG_EVIL] then resist = resist + 25 end
			if state.m_ptr.flags[FLAG_UNDEAD] then resist = resist + 25 end
			if state.m_ptr.flags[FLAG_DEMON] then resist = resist + 25 end
			if state.m_ptr.flags[FLAG_GOOD] then resist = resist - 125 end
		end

		-- Reduce damage based on resistance
		state.dam, state.resist = dam.apply_resistance(state.dam, resist)
	end
}

-- Hurts all that breath
dam.THICK_POISON = dam.derive
{
	source = dam.POISDAM,
	desc = "thick poison"
	modifier = function(target, state)
		-- Compute how hard it hits
		local resist = 0
		if target == "monster" then
			if state.m_ptr.flags[FLAG_NONLIVING] or
				state.m_ptr.flags[FLAG_UNDEAD] then
				resist = 100
			end
		elseif target == "player" then
			if player.has_intrinsic(FLAG_MAGIC_BREATH) then
				resist = 100
			end
		end

		-- Reduce damage based on it
		state.dam, state.resist = dam.apply_resistance(state.dam, resist)
	end
}

dam.NATURE = dam.add
{
	gfx	= 0,
	color	= function() return iif(rng(6) == 1, color.GREEN, color.LIGHT_GREEN) end,
	grid	= function(state)
		local y, x = state.y, state.x
		local c_ptr = cave(y, x)
		local f_ptr = f_info[c_ptr.feat + 1]

		if cave_feat_is(cave(y, x), FLAG_PERMANENT) then return nil end
		if f_ptr.flags[FLAG_DEAD_TREE_FEAT] then
			cave_set_feat(y, x, f_ptr.flags[FLAG_DEAD_TREE_FEAT])
			-- Silly thing to destroy trees when you worship yavanna
			god.inc_piety(god.YAVANNA, -50)
		elseif cave_feat_is(cave(y, x), FLAG_FLOOR) then
			local k = rng(100)
			if k < 10 then cave_set_feat(y, x, FEAT_FLOWER)
			elseif k < 25 then cave_set_feat(y, x, FEAT_GRASS)
			end
		end
	end,
	monster = function(state)
		-- Animals are resistant to nature atatcks
		if state.m_ptr.flags[FLAG_ANIMAL] then
			state.dam, state.resist = dam.apply_resistance(state.dam, 80)
		end
	end
	player	= function(state)
		return { fuzzy=stateu.fuzzy or "You are hit by something natural!",
			obvious=true }
	end,
}

dam.ICE = dam.combine({ {dam.COLD, 100}, {dam.CUT,50}, {dam.STUN,10}},
					  true, false, "ice", nil,
					  function()
						  return iif(rng(6) == 1, color.BLUE, color.WHITE)
					  end)

dam.WATER = dam.combine({ {dam.COLD, 70}, {dam.STUN,30}}, true, false,
						"water", nil,
						function()
							return iif(rng(6) == 1,
									   color.BLUE,
									   color.LIGHT_BLUE)
						end)

-------------------------------------------------------
----------------- Specialized damages -----------------
-------------------------------------------------------

-- Special drowning "damage"
dam.DROWN = dam.add
{
	color       = { color.BLUE }
	desc        = "drowning"
	bypass_symb = true

	player = function(state)
		-- Drown in deep water unless the player have levitation, water walking
		-- water breathing, or magic breathing.
		if calc_total_weight() > (player.weight_limit() / 2) then
			if player.has_intrinsic(FLAG_FEATHER) or player.intrinsic(FLAG_FLY) >= 1 or player.walk_water > 0 or player.has_intrinsic(FLAG_MAGIC_BREATH) or player.has_intrinsic(FLAG_WATER_BREATH) then
				return { dam = 0 }
			else
				dam.add_msg(state, "You are drowning!")
			end
		else
			return { dam = 0 }
		end
	end
}

dam.KILL_WALL = dam.add
{
	color      = { color.WHITE }
	text_color = "#w",
	desc       = "wall destruction"
	grid       = function(state)
		local y, x  = state.y, state.x
		local c_ptr = cave(y, x)
		local f_ptr = f_info[c_ptr.feat + 1]

		if f_ptr.flags[FLAG_PERMANENT] then
			return nil
		end
		if not f_ptr.flags[FLAG_WALL] then
			return nil
		end
		if not f_ptr.flags[FLAG_CAN_PASS] then
			return nil
		end

		if f_ptr.flags[FLAG_CAN_PASS][FLAG_PASS_STONE] then
			cave_set_feat(y, x, FEAT_FLOOR)
			-- Visibility change
			player.update = (player.update | PU_VIEW | PU_MONSTERS |
							 PU_MON_LITE | PU_FLOW)
		end
	end,
	monster = function(state)
			return { dam=0 }
		end,
	player = function(state)
			return { dam=0 }
		end,
}

dam.STONE_WALL = dam.add
{
	color      = { color.WHITE }
	text_color = "#w",
	desc       = "wall creation"
	grid       = function(state)
		local y, x  = state.y, state.x
		local c_ptr = cave(y, x)
		local f_ptr = f_info[c_ptr.feat + 1]

		if c_ptr.m_idx > 0 then
			return nil
		end
		if y == player.py and x == player.px then
			return nil
		end
		if f_ptr.flags[FLAG_PERMANENT] then
			return nil
		end
		if f_ptr.flags[FLAG_WALL] then
			return nil
		end

		cave_set_feat(y, x, FEAT_WALL_SOLID)

		-- Visibility change
		player.update = (player.update | PU_VIEW | PU_MONSTERS |
						 PU_MON_LITE | PU_FLOW)
	end
	monster = function(state)
			return { dam=0 }
		end,
	player = function(state)
			return { dam=0 }
		end,
}

dam.INSTA_DEATH = dam.add
{
	color   = { color.DARK, 0 }
	text_color = "#d"
	desc       = "instant death"
	monster = function(state)
       		if not rng.percent(5) or state.m_ptr.flags[FLAG_UNIQUE] or state.m_ptr.flags[FLAG_UNDEAD] or state.m_ptr.flags[FLAG_NONLIVING] then
       			return { dam=0 }
       		else
       			-- Reduce the exp gained this way
       			state.m_ptr.level = state.m_ptr.level / 3
       			return { dam=20000 msg=" faints.", msg_die=" is sucked out of life." }
       		end
	end
}

dam.KNOWLEDGE = dam.add
{
	color       = { color.DARK, 0 }
	text_color  = "#d"
	desc        = "knowledge"
	bypass_symb = true

	-- Identify objects
	object = function(state)
		set_aware(state.o_ptr)
		set_known(state.o_ptr)

		local item = generate_item_floor_index(state.y, state.x,
											   state.item_nb)
		hook.process(hook.IDENTIFY_POST, state.o_ptr, item, "normal")
	end,

	-- Self knowledge for player
	player	= function(state)
		message("You begin to know yourself a little better...")
		self_knowledge()
		return {dam = 0}
	end,

	-- Identify traps on grid of which the player is aware
	grid = function(state)
		local y, x = state.y, state.x
		local c_ptr = cave(y, x)

		foreach_flags(c_ptr.activations,
			function(activations, key)
				if not flag_is_known(activations, key) then
					return
				end

				local trap = activations[key]
				if trap[FLAG_TRAP_IDX] then
					traps.ident(trap[FLAG_TRAP_IDX])
				end
			end)
	end,

	-- Probe monster
	monster = function(state)
		do_probe(state.m_idx)
		state.m_ptr.csleep = 0 -- Wake monster up

		return {dam = 0, angry = 0}
	end,
} -- dam.KNOWLEDGE

dam.HEAL = dam.add
{
	color       = { color.DARK, 0 }
	text_color  = "#d"
	desc        = "healing"
	bypass_symb = true

	player	= function(state)
		return {fuzzy=state.fuzzy or "You are hit by something invigorating!",
			obvious = true, dam = -state.dam, msg = "You are healed."}
	end,
		
	monster = function(state)
		return {obvious = true, dam = -state.dam, angry = 0}
	end,
}

dam.RAISE_DEAD = dam.add
{
	color       = { color.DARK, 0 }
	text_color  = "#d"
	desc        = "raise dead"
	bypass_symb = true

	-- Zombify corpses
	object = function(state)
		local corpse = state.o_ptr

		if corpse.tval ~= TV_CORPSE or corpse.sval ~= SV_CORPSE_CORPSE then
			return
		end

		local monst = corpse.flags[FLAG_MONSTER_OBJ]

		if not monst then
			if wizard then
				message(color.LIGHT_RED,
						"ERROR in raise dead: corpse without FLAG_MONSTER_OBJ")
			end
			return
		end

		local see_corpse = false
		if not player.has_intrinsic(FLAG_BLIND) and
			player_can_see_bold(state.y, state.x)
		then
			see_corpse = true
		end

		if monst.sr_ptr and wizard then
			message(color.LIGHT_RED,
					"WARNING in raise dead: " .. monster_desc(monst, 128) ..
						"has special race")
		end

		local r_idx = monst.r_idx
		local race  = r_info(r_idx)

		if has_flag(race, FLAG_UNIQUE) then
			if see_corpse then
				msg_format("The %s twitches, then lays still.",
						   object_desc(corpse))
			end
			return
		end

		place_monster_one_no_drop = true
		local ret = place_monster_one(state.y, state.x, r_idx,
									  RACE_EGO_ZOMBIE1, 0, 0)
		place_monster_one_no_drop = false

		if ret then
			local faction = who_to_faction(state.who)
			local mon     = monster(ret)

			factions.change_faction(mon, faction)

			if see_corpse then
				msg_format("The %s rises, and stands on its own.",
						   object_desc(corpse))
			end
			return {kill = true} -- Remove the corpse
		else
			if see_corpse then
				msg_format("The %s twitches, then lays still.",
						   object_desc(corpse))
			end
		end
	end, -- object

	-- Heal monsters
	monster = function(state)
		return {dam = -state.dam, angry = 0}
	end,

	-- Restore player's life levels
	player	= function(state)
		restore_level()
		return {fuzzy=state.fuzzy or "You are hit by pure anti-death energy!",
			obvious = true, dam = 0}
	end,
} -- dam.RAISE_DEAD

dam.PROTECTION = dam.add
{
	color       = { color.LIGHT_GREEN, 0 }
	text_color  = "#G"
	desc        = "protection"
	bypass_symb = true

	grid = function(state)
		local y, x = state.y, state.x
		local c_ptr = cave(y, x)

		if cave_feat_is(c_ptr, FLAG_FLOOR)  and
			not cave_feat_is(c_ptr, FLAG_PERMANENT)
		then
			cave_set_feat(y, x, FEAT_GLYPH_WARDING)
		end
	end

	monster = function(state)
		return {dam = 0, angry = 0}
	end,

	player	= function(state)
		local dur = rng(10) + sroot(state.dam)
		local ac  = sroot(state.dam)

		local curr_ac = timed_effect.get(timed_effect.ARMOR, "power")

		if not curr_ac or ac > curr_ac then
			timed_effect(timed_effect.ARMOR, dur, ac)
		else
			timed_effect(timed_effect.ARMOR, dur, curr_ac)
		end

		return {dam = 0}
	end,
}

dam.TELEPORT_AWAY = dam.add
{
	desc	= "teleport away",
	color	= { color.BLUE },
	text_color = "#b",
	monster = function(state)
		return { dam=0, dist=state.dam, angry = 0 }
	end,
	player	= function(state)
		teleport_player(state.dam)
		return { dam=0 }
	end,
}


-------------------------------------------------------
------------------- Barehand damages ------------------
-------------------------------------------------------
dam.CRUSH_STUN = dam.combine({ {dam.CRUSH, 100}, {dam.STUN,150}}, true, false, "crushing", nil, { color.SLATE })
dam.CRUSH_SLOW = dam.combine({ {dam.CRUSH, 100}, {dam.SLOW,150}}, true, false, "crushing", nil, { color.SLATE })
dam.SLASH_STUN = dam.combine({ {dam.SLASH, 100}, {dam.STUN,70}}, true, false, "slashing", nil, { color.SLATE })
dam.SLASH_WOUND = dam.combine({ {dam.SLASH, 100}, {dam.CUT,70}}, true, false, "slashing", nil, { color.SLATE })
dam.SLASH_SLOW = dam.combine({ {dam.SLASH, 100}, {dam.SLOW,70}}, true, false, "slashing", nil, { color.SLATE })
dam.SLASH_STUN_WOUND = dam.combine({ {dam.SLASH, 100}, {dam.STUN,35}, {dam.CUT,35}}, true, false, "slashing", nil, { color.SLATE })
dam.SLASH_SLOW_WOUND = dam.combine({ {dam.SLASH, 100}, {dam.SLOW,35}, {dam.CUT,35}}, true, false, "slashing", nil, { color.SLATE })
dam.CRUSH_FORCE = dam.combine({ {dam.CRUSH, 100}, {dam.FORCE,50}}, true, false, "crushing", nil, { color.SLATE })
