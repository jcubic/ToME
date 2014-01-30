-- Handles thhe temporal school

declare_global_constants {
	"BANISHMENT",
	"ESSENCESPEED",
	"MAGELOCK",
	"SLOWMONSTER",
	"TIME_STOP",
}

MAGELOCK = add_spell
{
	name = 		"Magelock",
	school = 	{SCHOOL_TEMPORAL},
	level = 	1,
	mana = 		1,
	mana_max = 	35,
	fail = 		10,
	stick =
	{
			charge =    { 7, 5 },
			[TV_ORB] =
			{
				rarity = 		30,
				base_level =	{ 1, 5 },
				max_level =		{ 15, 45 },
			},
	},
	spell = function()
			local ret, x, y

			ret, x, y = tgt_pt()
			if not ret then return SPELL_NOTHING end
			local c_ptr = cave(y, x)

			if not los(player.py, player.px, y, x) then
				message("You must select a point within your line of sight.")
				return SPELL_NOTHING
			end

			if c_ptr.flags[FLAG_GLYPH_POWER] then
				message("A glyph is already there.")
				return SPELL_NOTHING
			end

			-- open closed doors
			if cave_feat_is(c_ptr, FLAG_DOOR)  then
				local f_ptr = f_info(c_ptr.feat)
				if cave_feat_is(c_ptr, FLAG_SECRET) then
					message("You've found a secret door!")
					cave_set_feat(y, x, f_ptr.flags[FLAG_SECRET])
					c_ptr.mimic=0
					f_ptr = f_info(c_ptr.feat)
				end

				if cave_feat_is(c_ptr, FLAG_JAMMED) then
					message("The door appears to be jammed.")
					return true
				elseif not (f_ptr.flags[FLAG_OPEN] or
							f_ptr.flags[FLAG_CLOSED])
				then
					message("The door does not respond to the spell.")
					return true
				end

				if f_ptr.flags[FLAG_OPEN] then
					local open = f_ptr.flags[FLAG_OPEN]

					if c_ptr.feat == FEAT_BROKEN then
						message("The door is magically fixed!")
						cave_set_feat(y, x, FEAT_OPEN)
						f_ptr = f_info(FEAT_OPEN)
						open  = f_ptr.flags[FLAG_OPEN]
					end

					if tag(open) == TAG_NUMBER and open == 0 then
						message("The door refuses to close.")
						return true
					elseif tag(open) == TAG_STRING then
						message(open)
						return true
					end

					if c_ptr.feat ~= FEAT_OPEN then
						message("The spell cannot work on that type of door.")
						return SPELL_NOTHING
					end

					local feat = FEAT_DOOR_LOCKED_HEAD +
						get_cast_level(FEAT_DOOR_LOCKED_TAIL -
									   FEAT_DOOR_LOCKED_HEAD, 0)
					feat = min(max(feat, FEAT_DOOR_LOCKED_TAIL),
							   FEAT_DOOR_LOCKED_HEAD)
					cave_set_feat(y, x, feat)
					message("The door closes with a click.")
				else
					local closed = f_ptr.flags[FLAG_CLOSED]

					if tag(closed) == TAG_NUMBER and closed == 0 then
						message("The door refuses to open.")
						return true
					elseif tag(closed) == TAG_STRING then
						message(closed)
						return true
					end

					cave_set_feat(y, x, f_ptr.flags[FLAG_CLOSED])
					if f_ptr.flags[FLAG_LOCKED] then
						message("The door opens with a click.")
					else
						message("The door opens.")
					end
				end
				return true
			end

			if get_cast_level() >= 5 then
				local feat = FEAT_DOOR_LOCKED_HEAD +
					get_cast_level(FEAT_DOOR_LOCKED_TAIL -
								   FEAT_DOOR_LOCKED_HEAD, 0)
				feat = min(max(feat, FEAT_DOOR_LOCKED_TAIL),
						   FEAT_DOOR_LOCKED_HEAD)

				if get_cast_level() >= 60 then
					-- Automatically make it a glyph of warding if placed
					-- underneath the player.
					if (y == player.py and x == player.px) or
						get_check("Place glyph of warding instead of a " ..
								  "locked door?")
					then
						feat = FEAT_GLYPH_WARDING
						if not features.can_place_glyph(y, x, feat) then
							return true
						end
					end
				end

				if feat ~= FEAT_GLYPH_WARDING then
					if y == player.py and x == player.px then
						message("You cannot place a door on top of yourself.")
						return true
					elseif (cave_feat_is(c_ptr, FLAG_FLOOR) or
							cave_feat_is(c_ptr, FLAG_PERMANENT))
					then
						message("You cannot place it there.")
						return true
					end
				end

				cave_set_feat(y, x, feat)
				message("Something appears.")
				return true
			end
			message("There is nothing to affect here.")
			return true
	end,
	info = 	function()
		       	return ""
	end,
	desc =	{
			"Magically locks or open a door at a distance",
			"At level 5 it can place a locked door anywhere in your field of vision"
			"At level 60 it can place a glyph of warding to repel monsters anywhere in your field of vision"
	}
}

SLOWMONSTER = add_spell
{
	name = 		"Slow Monster",
	school = 	{SCHOOL_TEMPORAL},
	level = 	19,
	mana = 		10,
	mana_max = 	15,
	fail = 		35,
	stick =
	{
			charge =    { 5, 5 },
			[TV_ORB] =
			{
				rarity = 		23,
				base_level =	{ 1, 15 },
				max_level =		{ 20, 50 },
			},
	},
	tactical =
	{
	 [ai.tactics.ANNOY] = 6
	 [ai.tactics.ATTACK] = 6
	 [ai.tactics.ESCAPE] = 4
	},

	spell	= function()
	       		local ret, dir
			ret, dir = get_aim_dir()
			if not ret then return SPELL_NOTHING end
			if get_cast_level() >= 40 then
				return fire_ball(dam.SLOW, dir, 40 + get_cast_level(160), 1)
			else
				return fire_bolt(dam.SLOW, dir, 40 + get_cast_level(160))
			end
	end
	info = 	function()
			if get_cast_level() >= 40 then
			       	return "power "..(40 + get_cast_level(160)).." rad 1"
			else
			       	return "power "..(40 + get_cast_level(160))
			end
	end,
	desc =	{
			"Magically slows down the passing of time around a monster",
			"At level 40 it affects a zone"
	}
}

ESSENCESPEED = add_spell
{
	name = 		"Essence of Speed",
	school = 	{SCHOOL_TEMPORAL},
	level = 	29,
	mana = 		20,
	mana_max = 	40,
	fail = 		50,
	stick =
	{
			charge =    { 3, 3 },
			[TV_ORB] =
			{
				rarity = 		80,
				base_level =	{ 1, 1 },
				max_level =		{ 10, 39 },
			},
	},
	action    = "@Source@ casts Essence of Speed.",
	inertia   = { 5, 20 },
	no_target = true
	tactical =
	{
		[ai.tactics.ENHANCEMENT] = 10
		[ai.tactics.NO_LOS] = true
	}
	spell = 	function()
			if spell_caster == 0 then
				if not timed_effect.get(timed_effect.FAST) then return timed_effect(timed_effect.FAST, 10 + rng(10) + get_cast_level(50), 5 + get_cast_level(15)) end
			else
				local source = monster(spell_caster)
				-- Allow quick speed increases to base+10
				if (source.mspeed < source.speed + 10) then
					if source.ml then
						msg_format(strcap(monster_desc(source, 0))..
								   " starts moving faster.")
					end
					source.mspeed = source.mspeed + 10

				-- Allow small speed increases to base+20 */
				elseif (source.mspeed < source.speed + 20) then
					if source.ml then
						msg_format(strcap(monster_desc(source, 0))..
								   " starts moving faster.")
					end
					source.mspeed = source.mspeed + 2
				end
			end
			return true
	end,
	info = 	function()
		       	return "dur "..(10 + get_cast_level(50)).."+d10 speed "..(5 + get_cast_level(15))
	end,
	desc =	{
			"Magically increases the passing of time around you",
	}
}

BANISHMENT = add_spell
{
	name = 		"Banishment",
	school = 	{SCHOOL_TEMPORAL, SCHOOL_CONVEYANCE},
	level = 	59,
	mana = 		30,
	mana_max = 	40,
	fail = 		95,
	stick =
	{
			charge =    { 1, 3 },
			[TV_ORB] =
			{
				rarity = 		98,
				base_level =	{ 1, 15 },
				max_level =		{ 10, 36 },
			},
	},
	inertia = 	{ 5, 50 },
	spell = 	function()
			message("IMPLEMENT BANISHMENT")
--[[
			local obvious
			obvious = project_los(dam.AWAY_ALL, 40 + get_cast_level(160))
			if get_cast_level() >= 30 then
				obvious = is_obvious(project_los(dam.STASIS, 20 + get_cast_level(120)), obvious)
			end
			return obvious
]]
	end,
	info = 	function()
		     	return "power "..(40 + get_cast_level(160))
	end,
	desc =	{
			"Disrupt the space/time continuum in your area and teleports all monsters away",
			"At level 30 it also may lock them in a time bubble for some turns"
	}
}

TIME_STOP = add_spell
{
	name = 		"Timeweave Folding",
	school = 	{SCHOOL_TEMPORAL,},
	level = 	99,
	mana = 		90,
	mana_max = 	120,
	fail = 		95,
	cooldown = 	500,
	spell = 	function()
			-- Note that we add one more turn than the description says, because it is imediately
			-- "eaten" by the spell cast itself
			return timed_effect(timed_effect.TIME_STOP, 2 + get_cast_level(5) + 1, 1000)
	end,
	info = 	function()
		     	return "turns "..(2 + get_cast_level(5))
	end,
	desc =	{
			"Disrupt the time continuum in your area, slowing time greatly for anybody but you ",
			"and increasing time greatly for you. In effect this nearly stops time for a few turns.",
			"With a base speed of +0 you would move 1000 times faster with this spell on."
	}
}
