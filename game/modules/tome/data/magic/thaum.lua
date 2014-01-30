-- thaum.lua

-- Defines the Thaumaturgy school of magic

-- Thaumaturgy is quite different from the 'book' schools of magic,
-- and also different from the magic 'powers' (necromancy, symbiosis)
-- Its spells don't get more powerful as you go up levels in the
-- skill, but most importantly: they are randomly allocated for each
-- new character.

-- As of ToME 3.0 it is possible to define a school of magic like
-- Thaumaturgy entirely in LUA. This file hopefully also serves as a
-- useful example to module authors wishing to write totally different
-- magic systems.

-- This file was originally written by Jules Bean (JMLB) for ToME
-- 3.0.0. Any unattributed comments are either mine, or left over from
-- the original C code.

-- XXX balance out new forms
-- XXX some waves should carry light?

declare_global_constants{
	"THAUM_SHAPE_BALL",
	"THAUM_SHAPE_BEAM",
	"THAUM_SHAPE_BLAST",
	"THAUM_SHAPE_BOLT",
	"THAUM_SHAPE_SWARM",
	"THAUM_SHAPE_VIEW",
	"THAUM_SHAPE_WAVE",
	"THAUM_SHAPE_WAVE2",
	"THAUM_SHAPE_CLOUD",
	"THAUM_SHAPE_STORM",
	"THAUM_SHAPE_SWORD",
	"add_thaum_shape",
	"atm_compose",
	"atm_id",
	"atm_mana",
	"atm_sides",
	"cast_thaum",
	"new_thaum_spell",
	"make_new_thaum_spell",
	"add_thaum_type",
	"thaum_batch_shortinfo",
	"thaum_longinfo",
	"thaum_make_spells",
	"thaum_shortinfo",
	"thaum_spell_cast",
	"thaum_spell_chance",
	"thaum_spell_desc",
	"thaum_spell_finish",
	"thaum_spell_info",
	"thaum_spell_name",
	"thaum_choose_type",
	"thaum_choose_dam",
	"thaum_interpolate",
	"thaum_parse_name"
}
declare_globals {
	"__thaum_shapes",
	"__thaum_shapes_num",
	"thaum_attack_type_info",
	"thaum_attack_types",
	"thaum_attack_types_num",
	"thaum_spells",
}

-- The spells are stored in the global array 'thaum_spells',
-- which is initially empty

thaum_spells = {}

-- We want to load and save this array.

add_loadsave("thaum_spells", {})
hook(hook.BIRTH, function() thaum_spells = {} end)


-- Modify spells based on type
--
-- This is a possibly too-cute use of first-class functions to do
-- the modification.

-- Identity function (don't change)
function atm_id(spell) return spell end

-- Increase(decrease) sides by ratio
function atm_sides(num, denom)
	return function (spell)
		spell.sides = bound(spell.sides*%num/%denom, 1)
		return spell
	end
end

-- Increase(decrease) mana by ratio
function atm_mana(num, denom)
	return function (spell)
		spell.mana = bound(spell.mana*%num/%denom, 1)
		return spell
	end
end

-- compose n functions
function atm_compose(...)
	return function (spell)
		for i = 1 , getn(%arg) do
			spell = %arg[i](spell)
		end
		return spell
	end
end

----------------------------------------------------------------------
----------------------------------------------------------------------

-- Now we set up the different shapes for the attacks.

__thaum_shapes = {}
__thaum_shapes_num = 1

function add_thaum_shape(s)
	__thaum_shapes[__thaum_shapes_num] = s

	__thaum_shapes_num = __thaum_shapes_num + 1
	return (__thaum_shapes_num - 1)
end

function thaum_spell_info(spell)
	return __thaum_shapes[spell.shape].make_info(spell)
end

function thaum_spell_desc(spell)
	return __thaum_shapes[spell.shape].make_desc(spell)
end

function thaum_spell_name(spell)
	return __thaum_shapes[spell.shape].make_name(spell)
end

function thaum_spell_finish(spell)
	if (__thaum_shapes[spell.shape].finish) then
		return __thaum_shapes[spell.shape].finish(spell)
	else
		return spell
	end
end

function thaum_spell_cast(spell)
	return __thaum_shapes[spell.shape].cast(spell)
end


-- The shapes currently specify functions to do the following:
--
-- make_name: dynamically build a short name for the user
-- make_info: dynamically build a short info summary
-- make_desc: dynamically build a long description
--
-- finish:
--   Put any 'finishing touches' to spell after creation.
--
-- cast:
--   Cast the spell. Failure has already been checked. Return false if
--   the user cancelled the spell and it won't take up mana or time.

THAUM_SHAPE_BOLT = add_thaum_shape {
	["chance"] = {[1]=10, [50]=10}, -- constant chance at any level
	["make_name"] =
		function(spell) return book_capitals(get_dam_type_info(spell.dam_type, "desc")).." Bolt" end,
	["make_info"] =
		function(spell)return "dam "..spell.dice.."d"..spell.sides end,
	["make_desc"] =
		function(spell)
			return {
			{color.LIGHT_BLUE, "This spell fires a"},
			{color.YELLOW, " bolt"}, {color.LIGHT_BLUE, " of"},
			{color.LIGHT_GREEN, " "..get_dam_type_info(spell.dam_type, "desc")},
			{color.LIGHT_BLUE, " at a single target, doing "},
			{color.LIGHT_RED, spell.dice.."d"..spell.sides},
			{color.LIGHT_BLUE, " damage ("}, {color.WHITE, "average "..(spell.dice*(spell.sides+1)/2)},
			{color.LIGHT_BLUE, ") unless your target resists the attack."}
			}
		end,
	-- No 'finish' method
	["cast"] =
 		function(spell)
			local proj_flags = (PROJECT_KILL | PROJECT_ITEM | PROJECT_GRID |
								PROJECT_STOP)
			local tx, ty

			local ret, dir = get_aim_dir()

			if not ret then return false end -- user cancelled

			-- Hack -- Use an actual "target"

			if (dir == 5) then
				tx = target_col
				ty = target_row

			else
				-- Use the given direction
				local dy, dx = explode_dir(dir)
				ty = player.py + (dy * 99)
				tx = player.px + (dx * 99)

				-- Mega-Hack -- Both beam and bolt spells should
			  --  continue through this fake target. --dsb

				proj_flags = proj_flags | PROJECT_THRU
			end

			-- Why project and not fire_bolt?  Because historically thaum
			-- spells all have _KILL, _ITEM, _GRID, which fire_bolt doesn't
			-- use.

			proj_flags = (proj_flags |
						  thaum_attack_type_info[spell.dam_type].proj_flags)

			-- Don't want bolts to go through walls
			proj_flags = proj_flags & negate_bits(PROJECT_WALL)

			project(0, 0, ty, tx,
							rng.roll(spell.dice, spell.sides),
							spell.dam_type, proj_flags)

			return true -- cast successful, use time and mana
		end,
}

THAUM_SHAPE_BEAM = add_thaum_shape {
	-- slowly gets more likely up to level 10
	["chance"] = {[1]=0, [2]=1, [10]=10, [50]=10},
	["make_name"] =
		function(spell) return book_capitals(get_dam_type_info(spell.dam_type, "desc")).." Beam" end,
	["make_info"] =
		function(spell)return "dam "..spell.dice.."d"..spell.sides end,
	["make_desc"] =
		function(spell)
			return {
			{color.LIGHT_BLUE, "This spell fires a"},
			{color.YELLOW, " beam"}, {color.LIGHT_BLUE, " of"},
			{color.LIGHT_GREEN, " "..get_dam_type_info(spell.dam_type, "desc")},
			{color.LIGHT_BLUE, " damaging all opponents in a straight line for "},
			{color.LIGHT_RED, spell.dice.."d"..spell.sides},
			{color.LIGHT_BLUE, " damage ("}, {color.WHITE, "average "..(spell.dice*(spell.sides+1)/2)},
			{color.LIGHT_BLUE, ") unless they resist the attack."}
		}
		end,
	["finish"] = atm_compose(atm_mana(3, 2), atm_sides(3, 5)),
	["cast"] =
		function(spell)
			local proj_flags = PROJECT_KILL | PROJECT_ITEM | PROJECT_GRID | PROJECT_BEAM | PROJECT_THRU

			-- Hack -- Spell needs a target
			local ret, dir = get_aim_dir()

			--- XXX false should be nil
			if not ret then return false end -- user cancelled
			local tx, ty

			-- Hack -- Use an actual "target"
			if (dir == 5) then
				tx = target_col
				ty = target_row
			else
				-- Use the given direction
				local dy, dx = explode_dir(dir)
				ty = player.py + (dy * 99)
				tx = player.px + (dx * 99)
			end

			proj_flags = (proj_flags |
						  thaum_attack_type_info[spell.dam_type].proj_flags)

			project(0, 0, ty, tx,
							rng.roll(spell.dice, spell.sides),
							spell.dam_type, proj_flags)

			return true -- cast successful, use time and mana
		end,
}

THAUM_SHAPE_BALL = add_thaum_shape {
	-- no chance until level 5
	["chance"] = {[1]=0, [4]=0, [5]=1, [20]=10, [50]=20},
	["make_name"] =
		function(spell) return book_capitals(get_dam_type_info(spell.dam_type, "desc")).." Ball" end,
	["make_info"] =
		function(spell)return "dam "..spell.dice.."d"..spell.sides.." rad "..spell.radius end,
	["make_desc"] =
		function(spell)
			return {
			{color.LIGHT_BLUE, "This spell fires a"},
			{color.YELLOW, " ball"}, {color.LIGHT_BLUE, " of"},
			{color.LIGHT_GREEN, " "..get_dam_type_info(spell.dam_type, "desc")},
			{color.LIGHT_BLUE, " with"}, {color.ORANGE, " radius "..spell.radius},
			{color.LIGHT_BLUE, " at your chosen target. It inflicts "},
			{color.LIGHT_RED, spell.dice.."d"..spell.sides},
			{color.LIGHT_BLUE, " damage ("}, {color.WHITE, "average "..(spell.dice*(spell.sides+1)/2)},
			{color.LIGHT_BLUE, ") at the centre of the ball, and lower damage"},
			{color.LIGHT_BLUE, " further out; damage will be further reduced if"},
			{color.LIGHT_BLUE, " your target resists the attack."}
			}
		end,
	["finish"] = atm_compose(
			 atm_mana(3, 2),
			 atm_sides(2, 3),
			 function(s)
				 s.radius = 1 + (s.level/10) + rng(s.mana/15)
				 return s
			 end
			 ),
	["cast"] =
		function(spell)
			local proj_flags = PROJECT_KILL | PROJECT_ITEM | PROJECT_GRID | PROJECT_STOP
			local tx, ty

			-- XXX Bug: thaum ball spells can't jump over targets. As far as
			-- I can tell this has always been true. We should turn off
			-- PROJECT_STOP if an actual target is selected, that's what
			-- fire_ball does.

      -- Hack -- Spell needs a target
			local ret, dir = get_aim_dir()
			if not ret then return false end -- user cancelled

			-- Hack -- Use an actual "target"
			if (dir == 5) then
				tx = target_col
				ty = target_row
			else
				-- Use the given direction
				local dy, dx = explode_dir(dir)
				ty = player.py + (dy * 99)
				tx = player.px + (dx * 99)

				-- Mega-Hack -- Both beam and bolt spells should continue
				-- through this fake target. --dsb
				-- Ball spells too -- JMLB
				proj_flags = proj_flags | PROJECT_THRU
			end

			proj_flags = (proj_flags |
						  thaum_attack_type_info[spell.dam_type].proj_flags)

			project(0, spell.radius, ty, tx,
							rng.roll(spell.dice, spell.sides),
							spell.dam_type, proj_flags)

			return true -- cast successful, use time and mana
		end,
}

THAUM_SHAPE_BLAST = add_thaum_shape {
	-- no chance until level 5
	["chance"] = {[1]=0, [4]=0, [5]=1, [20]=5, [50]=5},
	["make_name"] =
		function(spell) return book_capitals(get_dam_type_info(spell.dam_type, "desc")).." Blast" end,
	["make_info"] =
		function(spell)return "dam "..spell.dice.."d"..spell.sides.." rad "..spell.radius end,
	["make_desc"] =
		function(spell)
			return {
			{color.LIGHT_BLUE, "This spell fires a"},
			{color.YELLOW, " ball"}, {color.LIGHT_BLUE, " of"},
			{color.LIGHT_GREEN, " "..get_dam_type_info(spell.dam_type, "desc")},
			{color.LIGHT_BLUE, " with"}, {color.ORANGE, " radius "..spell.radius},
			{color.YELLOW, " centered on you. "}, {color.LIGHT_BLUE, "It inflicts "},
			{color.LIGHT_RED, spell.dice.."d"..spell.sides},
			{color.LIGHT_BLUE, " damage ("}, {color.WHITE, "average "..(spell.dice*(spell.sides+1)/2)},
			{color.LIGHT_BLUE, ") at the centre of the ball, and lower damage"},
			{color.LIGHT_BLUE, " further out; damage will be further reduced if"},
			{color.LIGHT_BLUE, " your target resists the attack."}
			}
		end,
	["finish"] = atm_compose(
			 atm_sides(2, 3),
			 function(s)
				 s.radius = 1 + (s.level/8) + rng(s.mana/10)
				 return s
			 end
			 ),
	["cast"] =
		function(spell)
			local proj_flags = PROJECT_KILL | PROJECT_ITEM | PROJECT_GRID | PROJECT_BLAST
			-- XXX Hack: PROJECT_BLAST, PROJECT_VIEWABLE and
			-- PROJECT_METEOR_SHOWER don't mean anything to the project
			-- function, so they shouldn't really be PROJECT flags. They
			-- were only ever PROJECT flags so that thaumaturgy (and
			-- runecraft) could keep all its 'attack shape' info in
			-- proj_flags, which was an XXX XXX XXX Hack if you ask me!

			local tx, ty

			-- Blast targets self

			ty = player.py
			tx = player.px

			proj_flags = (proj_flags |
						  thaum_attack_type_info[spell.dam_type].proj_flags)

			project(0, spell.radius, ty, tx,
							rng.roll(spell.dice, spell.sides),
							spell.dam_type, proj_flags)

			return true -- cast successful, use time and mana
		end,
}

THAUM_SHAPE_SWARM = add_thaum_shape {
	-- no chance until level 20
	["chance"] = {[1]=0, [19]=0, [20]=1, [50]=20},
	["make_name"] =
		function(spell) return book_capitals(get_dam_type_info(spell.dam_type, "desc")).." Swarm" end,
	["make_info"] =
		function(spell)
			return "dam "..spell.dice.."d"..spell.sides.." "..(spell.radius+1).."-"..(spell.radius*2).." balls"
		end,
	["make_desc"] =
		function (spell)
			return {
			{color.LIGHT_BLUE, "This spell calls down a"},
			{color.YELLOW, " devastating swarm"}, {color.LIGHT_BLUE, " of"},
			{color.ORANGE, " "..(spell.radius+1).." to "..(spell.radius*2)},
			{color.LIGHT_GREEN, " "..get_dam_type_info(spell.dam_type, "desc")},
			{color.LIGHT_BLUE, " balls each of"}, {color.ORANGE, " radius "..2},
			{color.YELLOW, " all around you. "},
			{color.LIGHT_BLUE, "Each ball in the swarm inflicts "},
			{color.LIGHT_RED, spell.dice.."d"..spell.sides},
			{color.LIGHT_BLUE, " damage ("}, {color.WHITE, "average "..(spell.dice*(spell.sides+1)/2)},
			{color.LIGHT_BLUE, ") at its centre, and lower damage"},
			{color.LIGHT_BLUE, " further out; damage will be further reduced if"},
			{color.LIGHT_BLUE, " your target resists the attack."}
			}
		end,
	["finish"] = atm_compose(
			 atm_mana(2, 1),
			 atm_sides(1, 5),
			 function(s)
				 -- Note, for meteor shower, num of
				 -- meteors is in fact radius + 1d(radius)
				 s.radius = 2 + (s.level/20) + rng(s.mana/20)
				 return s
			 end
			 ),
	["cast"] =
		function(spell)
			local proj_flags = PROJECT_KILL | PROJECT_ITEM | PROJECT_GRID | PROJECT_METEOR_SHOWER
			-- XXX Hack: PROJECT_BLAST, PROJECT_VIEWABLE and
			-- PROJECT_METEOR_SHOWER don't mean anything to the project
			-- function, so they shouldn't really be PROJECT flags. They
			-- were only ever PROJECT flags so that thaumaturgy (and
			-- runecraft) could keep all its 'attack shape' info in
			-- proj_flags, which was an XXX XXX XXX Hack if you ask me!

			-- Meteors go around player but they don't need a target,
			-- project_meteor takes care of it.

			proj_flags = (proj_flags |
						  thaum_attack_type_info[spell.dam_type].proj_flags)

			project_meteor(spell.radius, spell.dam_type,
				 rng.roll(spell.dice, spell.sides),
				 proj_flags)

			return true -- cast successful, use time and mana
		end,
}

THAUM_SHAPE_VIEW = add_thaum_shape {
	-- no chance until level 10
	["chance"] = {[1]=0, [9]=0, [10]=1, [20]=10, [50]=10},
	["make_name"] =
		function(spell) return book_capitals(get_dam_type_info(spell.dam_type, "desc")).." (view)" end,
	["make_info"] =
		function(spell)
			return "dam "..spell.dice.."d"..spell.sides
		end,
	["make_desc"] =
		function (spell)
			return {
			{color.LIGHT_BLUE, "This spell"},
			{color.YELLOW, " attacks every monster in sight"},
			{color.LIGHT_BLUE, " with the power of"},
			{color.LIGHT_GREEN, " "..get_dam_type_info(spell.dam_type, "desc")},
			{color.LIGHT_BLUE, " doing "},
			{color.LIGHT_RED, spell.dice.."d"..spell.sides},
			{color.LIGHT_BLUE, " damage ("}, {color.WHITE, "average "..(spell.dice*(spell.sides+1)/2)},
			{color.LIGHT_BLUE, ") if they do not resist the attack form."}
			}
		end,
	["finish"] = atm_compose(atm_mana(5, 2), atm_sides(1, 3)),
	["cast"] =
		function(spell)
			local proj_flags = PROJECT_KILL | PROJECT_ITEM | PROJECT_GRID | PROJECT_VIEWABLE
			-- XXX Hack: PROJECT_BLAST, PROJECT_VIEWABLE and
			-- PROJECT_METEOR_SHOWER don't mean anything to the project
			-- function, so they shouldn't really be PROJECT flags. They
			-- were only ever PROJECT flags so that thaumaturgy (and
			-- runecraft) could keep all its 'attack shape' info in
			-- proj_flags, which was an XXX XXX XXX Hack if you ask me!

			-- project_hack is exported to lua under the (more natural?)
			-- name project_los -- JMLB

			project_los(spell.dam_type, rng.roll(spell.dice, spell.sides))

			return true -- cast successful, use time and mana
		end,
}

THAUM_SHAPE_CLOUD = add_thaum_shape {
	["chance"] = {[1]=0, [4]=1, [10]=5, [20]=10, [50]=10},
	["make_name"] = function(spell) return book_capitals(get_dam_type_info(spell.dam_type, "desc")).." Cloud" end,
	["make_info"] = function(spell)
				return "dam "..spell.dice.."d"..spell.sides.." rad "..
				spell.radius.." dur "..spell.dur
			end,
	["make_desc"] =
		function (spell)
			return {
				{color.LIGHT_BLUE, "This spell produces a"},
				{color.ORANGE, " radius "..spell.radius},
				{color.YELLOW, " thick cloud"},
				{color.LIGHT_BLUE, " of"},
				{color.LIGHT_GREEN, " "..get_dam_type_info(spell.dam_type, "desc")},
				{color.LIGHT_BLUE, " doing "},
				{color.LIGHT_RED, spell.dice.."d"..spell.sides},
				{color.LIGHT_BLUE, " damage ("}, {color.WHITE, "average "..(spell.dice*(spell.sides+1)/2)},
				{color.LIGHT_BLUE, ") to monsters passing through, if they do not resist"},
				{color.LIGHT_BLUE, " the attack form. The cloud will"},
				{color.YELLOW, " last for "..spell.dur.." turns."},
			}
		end,
	["finish"] = atm_compose(
			 atm_mana(5, 2),
			 atm_sides(1, 3),
			 function(s)
				 s.radius = 1 + (s.level/8) + rng(s.mana/10)
				 s.dur = 3 + (s.level/5) + rng(s.mana/5)
				 return s
			 end
			 ),
	["cast"] =
		function(spell)
			local ret, dir = get_aim_dir()
			if not ret then return false end -- user cancelled

			fire_cloud(spell.dam_type, dir, rng.roll(spell.dice, spell.sides), spell.radius, spell.dur, player.speed())

			return true -- cast successful, use time and mana
		end,
}

THAUM_SHAPE_WAVE = add_thaum_shape {
	["chance"] = {[1]=0, [4]=1, [10]=5, [20]=10, [50]=10},
	["make_name"] = function(spell) return book_capitals(get_dam_type_info(spell.dam_type, "desc")).." Circular Wave" end,
	["make_info"] = function(spell)
				return "dam "..spell.dice.."d"..spell.sides.." rad "..
					spell.radius
			end,
	["make_desc"] =
		function (spell)
			return {
				{color.LIGHT_BLUE, "This spell sends out an"},
				{color.YELLOW, " expanding circular wave"},
				{color.LIGHT_BLUE, " of"},
				{color.LIGHT_GREEN, " "..get_dam_type_info(spell.dam_type, "desc")},
				{color.LIGHT_BLUE, " centred on you. The wave does "},
				{color.LIGHT_RED, spell.dice.."d"..spell.sides},
				{color.LIGHT_BLUE, " damage ("}, {color.WHITE, "average "..(spell.dice*(spell.sides+1)/2)},
				{color.LIGHT_BLUE, ") to monsters it hits, if they do not resist"},
				{color.LIGHT_BLUE, " the attack form. The wave will expand to a maximum"},
				{color.ORANGE, " radius of "..spell.radius.."."},
			}
		end,
	["finish"] = atm_compose(
			 atm_mana(5, 2),
			 atm_sides(1, 3),
			 function(s)
				 s.radius = 3 + (s.level/8) + rng(s.mana/10)
				 return s
			 end
		 ),
	["cast"] =
		function(spell)
			fire_wave(spell.dam_type, 0, rng.roll(spell.dice, spell.sides), 0 , spell.radius, player.speed(), EFF_WAVE)

			return true -- cast successful, use time and mana
		end,
}

THAUM_SHAPE_WAVE2 = add_thaum_shape {
	["chance"] = {[1]=0, [4]=1, [10]=5, [20]=10, [50]=10},
	["make_name"] = function(spell) return book_capitals(get_dam_type_info(spell.dam_type, "desc")).." Wave" end,
	["make_info"] = function(spell)
				return "dam "..spell.dice.."d"..spell.sides.." dist "..
					spell.radius
			end,
	["make_desc"] =
		function (spell)
			return {
				{color.LIGHT_BLUE, "This spell sends out a"},
				{color.YELLOW, " powerful wave"},
				{color.LIGHT_BLUE, " of"},
				{color.LIGHT_GREEN, " "..get_dam_type_info(spell.dam_type, "desc")},
				{color.LIGHT_BLUE, " doing "},
				{color.LIGHT_RED, spell.dice.."d"..spell.sides},
				{color.LIGHT_BLUE, " damage ("}, {color.WHITE, "average "..(spell.dice*(spell.sides+1)/2)},
				{color.LIGHT_BLUE, ") to monsters it hits, if they do not resist"},
				{color.LIGHT_BLUE, " the attack form. The wave will travel a"},
				{color.ORANGE, " maximum distance of "..spell.radius.."."},
			}
		end,
	["finish"] = atm_compose(
			 atm_mana(5, 2),
			 atm_sides(1, 3),
			 function(s)
				 s.radius = 3 + (s.level/6) + rng(s.mana/8)
				 return s
			 end
		 ),
	["cast"] =
		function(spell)
			local ret, dir = get_rep_dir()
			if not ret then return false end

			fire_wave(spell.dam_type, 0, rng.roll(spell.dice, spell.sides), 0 , spell.radius, player.speed(), EFF_WAVE + getglobal("EFF_DIR"..dir))

			return true -- cast successful, use time and mana
		end,
}

THAUM_SHAPE_STORM = add_thaum_shape {
	["chance"] = {[1]=0, [4]=1, [10]=5, [20]=10, [50]=10},
	["make_name"] = function(spell) return book_capitals(get_dam_type_info(spell.dam_type, "desc")).." Storm" end,
	["make_info"] = function(spell)
				return "dam "..spell.dice.."d"..spell.sides.." rad "..
					spell.radius.." dur "..spell.dur
			end,
	["make_desc"] =
		function (spell)
			return {
				{color.LIGHT_BLUE, "This spell surrounds you with a"},
				{color.ORANGE, " radius "..spell.radius},
				{color.YELLOW, " fierce storm"},
				{color.LIGHT_BLUE, " of"},
				{color.LIGHT_GREEN, " "..get_dam_type_info(spell.dam_type, "desc")},
				{color.LIGHT_BLUE, " doing "},
				{color.LIGHT_RED, spell.dice.."d"..spell.sides},
				{color.LIGHT_BLUE, " damage ("}, {color.WHITE, "average "..(spell.dice*(spell.sides+1)/2)},
				{color.LIGHT_BLUE, ") to monsters caught within, if they do not resist"},
				{color.LIGHT_BLUE, " the attack form."},
			}
		end,
	["finish"] = atm_compose(
			 atm_mana(5, 2),
			 atm_sides(1, 3),
			 function(s)
				 s.radius = (s.level/15) + rng(s.mana/20)
				 s.dur = 10 + (s.level/3) + rng(s.mana/3)
				 return s
			 end
		 ),
	["cast"] =
		function(spell)
			fire_wave(spell.dam_type, 0, rng.roll(spell.dice, spell.sides), spell.radius, spell.dur, player.speed(), EFF_STORM)

			return true -- cast successful, use time and mana
		end,
}

THAUM_SHAPE_SWORD = add_thaum_shape {
	["chance"] = {[1]=0, [4]=1, [10]=10, [50]=10},
	["make_name"] = function(spell) return book_capitals(get_dam_type_info(spell.dam_type, "desc")).." Sword" end,
	["make_info"] = function(spell)
				return "dam "..spell.dice.."d"..spell.sides.."/blow rad "..
					spell.radius.." dur "..spell.dur
			end,
	["make_desc"] =
		function (spell)
			return {
				{color.LIGHT_BLUE, "This spell"},
				{color.YELLOW, " imbues your weapon"},
				{color.LIGHT_BLUE, " with an aura of"},
				{color.LIGHT_GREEN, " "..get_dam_type_info(spell.dam_type, "desc")},
				{color.LIGHT_BLUE, " doing an additional"},
				{color.LIGHT_RED, " "..spell.dice.."d"..spell.sides},
				{color.LIGHT_BLUE, " damage ("}, {color.WHITE, "average "..(spell.dice*(spell.sides+1)/2)},
				{color.LIGHT_BLUE, ") each time you hit a monster, if they do not resist"},
				{color.LIGHT_BLUE, " the attack form. The aura also affects other monsters within a"},
				{color.ORANGE, " radius of "..spell.radius.."."},
			}
		end,
	["finish"] = atm_compose(
			 atm_mana(5, 2),
			 atm_sides(1, 10),
			 function(s)
				 s.radius = rng.number(s.level/16) + rng.number(s.mana/30)
				 s.dur = 10 + (s.level/3) + rng(s.mana/3)
				 return s
			 end
		 ),
	["cast"] =
		function(spell)
			-- XXX maybe complain if caster has no wielded weapon?
			timed_effect(
		  		timed_effect.MELEE_PROJECT,
		  		spell.dur,
				spell.dam_type,
				rng.roll(spell.dice, spell.sides),
				spell.radius,
				PROJECT_STOP | PROJECT_KILL | PROJECT_ITEM
			)
			return true -- cast successful, use time and mana
		end,
}

----------------------------------------------------------------------
----------------------------------------------------------------------

-- A table of information for the types. Contains English names
-- for naming spells and also the damage modification function

thaum_attack_type_info = {}

thaum_attack_types = {}
thaum_attack_types_num = 1

function add_thaum_type(type, mod, shapes_exclude, proj_flags)
	if not mod            then mod            = atm_id end
	if not shapes_exclude then shapes_exclude = {} end
	if not proj_flags     then proj_flags     = 0 end

	thaum_attack_type_info[type] = {
		["mod"]            = mod,
		["shapes_exclude"] = shapes_exclude,
		["proj_flags"]     = proj_flags
	}
	thaum_attack_types[thaum_attack_types_num] = type
	thaum_attack_types_num = thaum_attack_types_num + 1
end

-- XXX most are commented out since the don't exist in tome3 yet

--add_thaum_type(dam.ARROW)
--add_thaum_type(dam.MISSILE)
--add_thaum_type(dam.MANA)
add_thaum_type(dam.WATER, atm_sides(6, 5))
add_thaum_type(dam.SUN_FIRE, atm_sides(6, 5))
--add_thaum_type(dam.METEOR, atm_sides(6, 5))
add_thaum_type(dam.ICE, atm_sides(4, 5))
add_thaum_type(dam.GRAVITY, atm_sides(1, 2))
add_thaum_type(dam.INERTIA, atm_sides(1, 2))
add_thaum_type(dam.FORCE, atm_sides(1, 2))
add_thaum_type(dam.TIME, atm_sides(6, 5))
add_thaum_type(dam.ACID, atm_sides(3, 2))
add_thaum_type(dam.ELEC, atm_sides(3, 2))
add_thaum_type(dam.FIRE, atm_sides(3, 2))
add_thaum_type(dam.COLD, atm_sides(3, 2))
add_thaum_type(dam.POISDAM, atm_sides(6, 5))
add_thaum_type(dam.LITE, atm_sides(6, 5))
add_thaum_type(dam.DARK, atm_sides(6, 5))
add_thaum_type(dam.VALINOREAN_FIRE, atm_sides(3, 4))
add_thaum_type(dam.DARK_FIRE, atm_sides(3, 4))
--add_thaum_type(dam.NETHER, atm_sides(3, 2))
add_thaum_type(dam.CONFUSION, atm_sides(4, 5))
add_thaum_type(dam.SOUND, atm_sides(1, 2))
add_thaum_type(dam.SHARDS, atm_sides(4, 5))
add_thaum_type(dam.NEXUS, atm_sides(6, 5))
add_thaum_type(dam.CHAOS, atm_sides(3, 2))
add_thaum_type(dam.DISENCHANT, atm_sides(1, 2))

add_thaum_type(dam.KILL_WALL, nil,
			   {
				   [THAUM_SHAPE_CLOUD] = true,
				   [THAUM_SHAPE_STORM] = true,
				   [THAUM_SHAPE_VIEW]  = true,
				   [THAUM_SHAPE_WAVE]  = true,
				   [THAUM_SHAPE_WAVE2] = true,
				   [THAUM_SHAPE_SWORD] = true
			   },
			   PROJECT_WALL)

--add_thaum_type(dam.DESTRUCTION)

----------------------------------------------------------------------
----------------------------------------------------------------------

-- Now the functions to actually create the spells.

-- thaum_make_spells checks if you have as many spells as you should,
-- and if not, makes some more until you do

function thaum_make_spells()
	local current, should, new

	new = 0
	current = getn(thaum_spells)
	should = get_skill_scale(SKILL_THAUMATURGY, 100)

	while current < should do
		current = current + 1
		make_new_thaum_spell(current)
		new = new + 1
	end

	if new > 0 then
		message("You have gained "..new.." Thaumaturgy spells")
	end
end

-- We need thaum_make_spells to be called after skills have changed,
-- and also at birth

hook {
	[hook.RECALC_SKILLS] = thaum_make_spells,
	-- Note the 'thaum_spells = {}' to zero the array if it has been
	-- loaded from the save file of a (dead) ancestor
	[hook.INIT_DONE] = function ()
		thaum_spells = {}
		thaum_make_spells()
	end,
}

-- The dirty function to create the spell
--
-- Things I'd like to do differently:
--
-- 1) Generalise the probability distribution selecting which
--    shape you get.  Perhaps have a different function for
--    low levels than high levels (reserve view/swarm for high
--    levels, maybe)
--
-- 2) Move more of the shape-specific stuff into the shape definitions
--    higher up in this file.  Ideally, of course, it should all be there
--    so new shapes can be added without changing this function

function new_thaum_spell(level, allow_shapes, dam_type)
	local proj_flags
	local dice, sides, chance, mana, radius
	local destruc_gen
	local simple_gen
	local shape

	mana = level/3 + rng((level+1)/3+1) + rng((level+2)/3+1) - 1

	dice = rng((mana+10)/12 + 1) + rng((mana+16)/18 + 1) + mana/3 - 1
	sides = mana + level + rng((mana+1)/3 + 1) + rng((mana+2)/3 + 1) + rng((mana+3)/3 + 1)

	-- Calculate relative probabilities by interpolating
	shape = thaum_choose_type(level, allow_shapes, dam_type)

	if not shape then
		return nil
	end

	if not dam_type then
		dam_type = thaum_choose_dam(level, shape)
	end

	proj_flags = thaum_attack_type_info[dam_type].proj_flags

	local spell =
	{
		["level"] = level,
		["mana"] = mana,
		["dice"] = dice,
		["sides"] = sides,
		["radius"] = radius,
		["dam_type"] = dam_type,
		["proj_flags"] = proj_flags,
		["shape"] = shape,
	}

	-- modify damage based on type (JMLB).
	-- The more powerful types do less damage.
	spell = thaum_attack_type_info[dam_type].mod(spell)

	-- modify damage and mana cost based on shape
	return thaum_spell_finish(spell)
end

function make_new_thaum_spell(spellnum)
	-- make the spell
	-- store the proud new spell in the array!
	thaum_spells[spellnum] = new_thaum_spell((spellnum + 1) / 2)
	-- To allow spell repeats using the 'n' key
	thaum_spells[spellnum].index = spellnum
end

function thaum_choose_type(level, allow_shapes, dam_type)
	local exclude
	local probtable = {}
	local totprob = 0
	local roll

	if dam_type then
		exclude = thaum_attack_type_info[dam_type].shapes_exclude
	else
		exclude = {}
	end

	for i=1, getn(__thaum_shapes) do
		local relprob = thaum_interpolate(__thaum_shapes[i].chance, level)
		if (not allow_shapes or allow_shapes[i]) and not exclude[i] then
			tinsert(probtable, {relprob, i})
		end
	end

	if getn(probtable) == 0 then
		error("thaum_choose_type(): No allowable shape/damage combinations")
	end

	for i=1, getn(probtable) do
		totprob = totprob + probtable[i][1]
	end

	roll = rng(totprob)

	for i=1, getn(probtable) do
		local relprob, shape = probtable[i][1], probtable[i][2]
		if (roll <= relprob) then
			return shape
		else
			roll = roll-relprob
		end
	end

	message("Error in thaum_choose_type") -- shouldn't be possible
end

function thaum_choose_dam(level, shape)
	local types = {}

	for i = 1, getn(thaum_attack_types) do
		local dam = thaum_attack_types[i]

		if not thaum_attack_type_info[dam].shapes_exclude[shape] then
			tinsert(types, dam)
		end
	end

	if getn(types) == 0 then
		error("All damage types were excluded")
	end

	return types[rng(getn(types))]
end

-- This is another candidate for being a global utility function

function thaum_interpolate(table, index)
	local below, above

	for k, v in table do
		if k <= index and (below == nil or k > below) then below = k end
		if k >= index and (above == nil or k < above) then above = k end
	end

	if not (below and above) then
		message("Error: interpolate out of bounds")
		return nil
	end

	if (above == below) then return table[above] end

	return table[below] + (table[above] - table[below])
		* (index - below) / (above - below)
end

-- Now we've added the spells, the user needs a user interface to cast them
-- First we add an MKEY for them (note that MKEYS have to be enabled in
-- s_info.txt or somewhere similar to show up for the user)

add_mkey
{
	["mkey"] 	= "Thaumaturgy",
	["type"]    = "skill",
	["fct"] 	= function()
		local spell
		local batches={}
		local batch

		-- No magic
		if player.antimagic > 0 then
			message("Your anti-magic field disrupts any magic attempts.")
			return nil
		end

		-- No magic
		if player.anti_magic then
			message("Your anti-magic shell disrupts any magic attempts.")
			return nil
		end

		if getn(thaum_spells) == 0 then
			message("You have no thaumaturgy spells!") -- this shouldn't happen
			return
		end

		-- Check for a repeated action
		local ret, rep = repeat_keys.pull()
		if ret then
			-- Spells can be removed when restarting over an old character
			if rep < 0 or rep > getn(thaum_spells) then
				message("Invalid thaumaturgy spell selected by an invalid macro.")
				return
			else
				cast_thaum(thaum_spells[rep])
				return
			end
		end

		for i = 1 , ((getn(thaum_spells)-1)/10)+1 do
			batches[i]=i
		end

		batch = choose_from_list(batches, "Batches", "Cast from which batch?", thaum_batch_shortinfo)

		if batch then
			local batch_spells={}
			local last

			-- Build this batch

			last = batch*10
			if last > getn(thaum_spells) then last = getn(thaum_spells) end

			for i=1+(batch-1)*10, last do
				tinsert(batch_spells, thaum_spells[i])
			end

			-- Choose from batch

			spell = choose_from_list(batch_spells, "Spells", "Cast which spell?", thaum_shortinfo, thaum_longinfo)
			if spell then
				repeat_keys.push(spell.index)
				cast_thaum(spell)
			end
		end
	end,
}

function thaum_batch_shortinfo(y, x, b)
	if b then
		term.blank_print(color.WHITE, "Batch "..(1 + (b - 1) * 10).."-"..(b * 10), y, x)
	end
end

-- The mkey function uses choose_from_list to build a pretty list of spells.
-- That has callbacks to the functions you pass it.  In particular,
-- thaum_shortinfo is called to display one line of information about
-- each spell. spell == nil means display the column header

function thaum_shortinfo(y, x, spell)
	if spell then
		local colour
		if player.csp() < spell.mana then colour = color.ORANGE
		else colour = color.LIGHT_GREEN end

		term.blank_print(colour, format("%-30s   %3d %4s %3d%s %s", thaum_spell_name(spell), spell.level, spell.mana, thaum_spell_chance(spell), "%", thaum_spell_info(spell)), y, x)
	else
		term.blank_print(color.WHITE, format("%-30s   %-3s %-4s %-4s %s", "Name", "Lvl", "Mana", "Fail", "Info"), y, x)
	end
end

-- This callback draws the long description if the user requests one by
-- using a capital letter ('F' to describe spell 'f').

function thaum_longinfo(y, spell)
	local desc = thaum_spell_desc(spell)

	for i = 1, getn(desc) do
	  -- Use text_out so that ToME does the line wrapping for us
		term.text_out(desc[i][1], desc[i][2])
	end
end


-- Finally, actually cast a spell.
-- This function also should be factored into shape-independent
-- stuff and shape-dependent stuff, and the shape-dependent stuff
-- moved into the shape definitions above.

function cast_thaum(spell)
	local proj_flags, dir, chance

	local ty , tx

	tx = 0
	ty = 0

	-- Thaum spells just can't be cast without mana
	-- XXX actually could imagine some quite funky thaum
	-- overload effects
	if player.csp() < spell.mana then
		message("You do not have enough mana.")
		return
	end

	-- Spell failure chance
	chance = thaum_spell_chance(spell)

	if (rng.number(100) >= chance) then
		-- Spell Success
		if (thaum_spell_cast(spell) == false) then
			-- Spell cancelled, so don't spend time or mana
			return
		end
	else
		-- Spell Failed!

		local insanity = (player.msane() - player.csane()) * 100 / player.msane()

		-- Flush input if told so
		if (flush_failure) then message.flush() end

		-- Insane players can see something strange
		if (rng.number(100) < insanity) then
			local sfail
			get_rnd_line("sfail.txt", sfail)
			msg_format("A cloud of %s appears above you.", sfail)
		-- Normal failure messages
		else
			message("You failed to get the spell off!")
		end

		-- sound not exported yet?
		-- sound(SOUND_FAIL)
	end

	-- Success or failure we still take time, use mana, and
	-- redraw stuff

	-- Use mana
	player.csp(player.csp() - spell.mana)

	-- Take a turn
	energy_use = get_player_energy(SPEED_CAST)
end

-- Calculate spell failure chance for a thaum spell


function thaum_spell_chance(spell, skill_level)
	local chance, minfail

	-- Extract the base spell failure rate
	chance = spell.level + 25

	-- Reduce failure rate by "effective" level adjustment
	skill_level = skill_level or get_skill_scale(SKILL_THAUMATURGY, 50)
	chance = chance - 3 * (skill_level - spell.level)

	-- Reduce failure rate by INT/WIS adjustment XXX?
	chance = chance - (((player.stat(A_INT) - 10) * 2) / 3)

	-- Not enough mana to cast doesn't matter, since
	-- it's totally impossible to cast thaum spells when not enough mana

	-- Extract the minimum failure rate
	if player.stat(A_INT) < 10 then
		minfail = (10 - player.stat(A_INT)) * 10
	elseif player.stat(A_INT) < 40 then
		minfail = 10 - (((player.stat(A_INT) - 10) * 30) / 100)
	else
		minfail = 0
	end

	-- Minimum failure rate
	if (chance < minfail) then chance = minfail end

	chance = chance + player.intrinsic(FLAG_SPELL_FAILURE)

	-- Always a 5 percent chance of working
	if (chance > 95) then chance = 95 end

	-- Return the chance
	return (chance)
end


function thaum_parse_name(str)
	local dam_type = -1
	local dam_name = ""

	str = strlower(clean_whitespace(str))

	for i = 1, getn(thaum_attack_types) do
		local name = get_dam_type_info(thaum_attack_types[i], "desc")

		name = name .. " "

		if starts_with(str, name) or strfind(str, " " .. name, 1, 1) then
			if strlen(name) > strlen(dam_name) then
				dam_type = thaum_attack_types[i]
				dam_name = name
			end
		end
	end

	if dam_type == -1 then
		return nil
	end

	local spell     = nil
	local full_name = ""

	for i = 1, getn(__thaum_shapes) do
		local tmp_spell = {dam_type = dam_type, shape = i}
		local name      = strlower(thaum_spell_name(tmp_spell))

		if ends_with(str, name) or strfind(str, name .. " ", 1, 1) then
			if strlen(name) > strlen(full_name) then
				spell     = tmp_spell
				full_name = name
			end
		end
	end

	if not spell then
		return nil
	end

	-- Forbidden dam_type/shape combo?
	if thaum_attack_type_info[dam_type].shapes_exclude[spell.shape] then
		return nil
	end

	local pos = strfind(str, full_name, 1, 1)
	local prefix = strsub(str, 1, pos)
	local suffix = strsub(str, pos + strlen(full_name), -1)

	prefix = clean_whitespace(prefix)
	suffix = clean_whitespace(suffix)

	return prefix, suffix, spell
end -- thaum_parse_name()
