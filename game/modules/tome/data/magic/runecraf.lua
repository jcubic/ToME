-- runecraf.lua
--
-- Runecraft implementation in LUA
--

-- TODO:
--  Balance :-)
--  Names for memorized spells (needs ability to save strings)
--  Default names would do

declare_global_constants {
	"MAX_RUNES",
}

-- ordinary runes have a single RUNE_SHAPE flag, whose value
-- represents the shape.  However, the RUNE_SHAPE_foo flags are
-- defined for the benefit of runestones.

safe_new_flag("RUNE_SHAPE")
safe_new_flag("RUNE_EXCL_SHAPES")

safe_new_flag("RUNE_WEIGHT")

safe_new_flag("RUNE_STONE")

safe_new_flag("RUNE_DAM")
safe_new_flag("RUNE_DAM_NAME")
safe_new_flag("RUNE_DAM_INFO")
safe_new_flag("RUNE_PROJ")
safe_new_flag("RUNE_PROJ_NOT")

safe_new_flag("RUNE_HARDNESS")

constant("runes", {})
runes.shapes = {}

function runes.add_shape(parms)
	local flagname = "RUNE_SHAPE_"..parms.name
	new_flag(flagname)

	local index = getglobal("FLAG_"..flagname)

	if not parms["exclude"] then
		parms["exclude"] = {"NONE"}
	elseif type(parms["exclude"]) ~= "table" then
		error("'exclude' field of runeshape must be an array of strings")
	end

	runes.shapes[index]=parms
	return index
end

-- Flag getter
getter.runeshape = function(value)
	local flagname = "FLAG_RUNE_SHAPE_"..value
	local shape = getglobal(flagname)
	if shape then
		return shape
	else
		error("Unknown RUNE_SHAPE :"..value)
	end
end

hook{
	[hook.OBJECT_DESC] = function(obj, to_file, trim_down)
		if known_flag(obj, FLAG_RUNE_DAM) then
			local dam
			if has_flag(obj, FLAG_RUNE_DAM_NAME) then
				dam = obj.flags[FLAG_RUNE_DAM_NAME]
			else
				dam = get_flag(obj, FLAG_RUNE_DAM)
				dam = get_dam_type_info(dam, "desc")
			end
			term.text_out("It has a primary rune of ")
			term.text_out(color.LIGHT_RED, dam)
			if known_flag(obj, FLAG_RUNE_HARDNESS) then
				term.text_out(color.LIGHT_RED, " (Hardness " ..
							  get_flag(obj, FLAG_RUNE_HARDNESS)..")")
			end
			term.text_out(" embeded in it.  ")
		end
		if known_flag(obj, FLAG_RUNE_SHAPE) then
			local shape
			shape = book_capitals(strlower(runes.shapes[get_flag(obj, FLAG_RUNE_SHAPE)].name))

			term.text_out("It has a secondary rune of ")
			term.text_out(color.LIGHT_RED, shape.."(Weight "..get_flag(obj, FLAG_RUNE_WEIGHT)..")")
			term.text_out(" embeded in it.  ")
		end
	end,
}

--
-- List of rune shapes
--
runes.add_shape(
	{
		["name"] = "SELF",
		["exclude"] = {"ALL"},
		["cast"] = function(type, dam, ty, tx, flags, flags_not)
			local flg = 0
			flg = (flg | flags) & negate_bits(flags_not)
			flg = flg | PROJECT_KILL | PROJECT_CASTER
			project(0, 0, player.py, player.px, dam, type, flg)
		end
	})

runes.add_shape(
	{
		["name"] = "SPHERE",
		["exclude"] = {"SELF", "SWARM", "WAVE", "VIEW"},
		["info"] = function(type, min_dam, max_dam)
			return "radius " .. (2 + sroot(min_dam) / 5) .. " to " ..
				(2 + sroot(max_dam) / 5)
		end,
		["cast"] = function(type, dam, ty, tx, flags, flags_not)
			local radius = 2 + sroot(dam)/5 -- XXX
			local flg = (PROJECT_THRU | PROJECT_STOP | PROJECT_KILL |
						 PROJECT_ITEM | PROJECT_GRID)
			flg = (flg | flags) & negate_bits(flags_not)
			-- XXX order dam,type is different?
			project(0, radius, ty, tx, dam, type, flg)
		end
	})

runes.add_shape
{
		["name"] = "WAVE",
		["exclude"] = {"SELF", "ARROW", "RAY", "SPHERE", "SWARM", "VIEW"},
		["info"] = function(type, min_dam, max_dam)
			return "duration " .. (5 + sroot(min_dam) / 4) .. " to " ..
				(5 + sroot(max_dam) / 4)
		end,
		["cast"] = function(type, dam, ty, tx, flags, flags_not)
				local time = 5 + sroot(dam) / 4
				local flg = (PROJECT_STOP | PROJECT_GRID | PROJECT_ITEM |
							 PROJECT_KILL | PROJECT_STAY)
				flg = (flg | flags) & negate_bits(flags_not)

				wiz_print("Wave time: "..time)

				project_time = time
				project_time_effect = EFF_WAVE
				project(0, 0, ty, tx, dam, type, flg)
		 end
}

runes.add_shape(
	{
		["name"] = "RAY",
		["exclude"] = {"SELF", "ARROW", "WAVE", "SWARM", "VIEW"}
		["target"]=1,
		["cast"] = function(type, dam, ty, tx, flags, flags_not)
			local flg = PROJECT_THRU | PROJECT_KILL | PROJECT_BEAM
			flg = (flg | flags) & negate_bits(flags_not)
			project(0, 0, ty, tx, dam, type, flg)
		end

	})

runes.add_shape(
	{
		["name"] = "ARROW",
		["exclude"] = {"SELF", "RAY", "SWARM", "WAVE", "VIEW"}
		["target"]=1,
		["cast"] = function(type, dam, ty, tx, flags, flags_not)
			local flg = PROJECT_THRU | PROJECT_KILL | PROJECT_STOP
			flg = (flg | flags) & negate_bits(flags_not)
			project(0, 0, ty, tx, dam, type, flg)
		end

	})

runes.add_shape(
	{
		["name"] = "VIEW",
		["exclude"] = {"ALL"},
		["cast"] = function(type, dam, ty, tx, flags, flags_not) project_los(type, dam) end
	})

runes.add_shape(
	{
		["name"] = "SWARM",
		["exclude"] = {"SELF", "ARROW", "RAY", "SPHERE", "WAVE", "VIEW"}
		["info"] = function(type, min_dam, max_dam)
			return "radius " .. (2 + sroot(min_dam) / 6) .. " to " ..
				(2 + sroot(max_dam) / 6)
		end,
		["cast"] = function(type, dam, ty, tx, flags, flags_not)
			local radius = 2 + sroot(dam)/6 -- XXX
			local flg = (PROJECT_THRU | PROJECT_KILL | PROJECT_ITEM |
						 PROJECT_GRID)
			flg = (flg | flags) & negate_bits(flags_not)
			project_meteor(radius, type, dam, flg)
		end

	})


-- loadsaves for the timer and assorted infos
player.future_rune_data = {}
time.save_timer("player.TIMER_FUTURE_RUNECRAFT")
add_loadsave("player.future_rune_data", {})
hook(hook.BIRTH, function() player.future_rune_data = {} end)

player.TIMER_FUTURE_RUNECRAFT = time.new_timer
{
	["delay"] = 	10,
	["enabled"] = 	nil,
	["callback"] = 	function()
			player.TIMER_FUTURE_RUNECRAFT.enabled = nil
			message("The future rune fires!")

			for i = 1, getn(player.future_rune_data.shapes) do
				runes.shape_cast(player.future_rune_data.shapes[i], player.future_rune_data.type, player.future_rune_data.dam, player.future_rune_data.ty, player.future_rune_data.tx, player.future_rune_data.flags, player.future_rune_data.flags_not)
			end
	end,
}


runes.add_shape(
	{
		["name"] = "FUTURE",
		["cast"] = function(type, dam, ty, tx, flags, flags_not, rune_shapes)
			local time = rng(dam / 5, dam / 3) + 3
			wiz_print("Future rune execution time: " .. time .. " at coord " .. ty .. "x" .. tx)
			player.TIMER_FUTURE_RUNECRAFT.enabled = true
			player.TIMER_FUTURE_RUNECRAFT.delay = time
			player.TIMER_FUTURE_RUNECRAFT.countdown = time
			player.future_rune_data = {
				type      = type,
				dam       = dam,
				ty        = ty,
				tx        = tx,
				shapes    = {},
				flags     = flags,
				flags_not = flags_not,
			}
			foreachi (rune_shapes, function (i, s)
				if s.shape ~= FLAG_RUNE_SHAPE_FUTURE then
					tinsert(player.future_rune_data.shapes, s.shape)
				end
			end)
		end

	})

function runes.shape_needs_target(shape)
	return runes.shapes[shape].target
end

function runes.shape_cast(shape, type, dam, ty, tx, flags, flags_not, shapes)
	return runes.shapes[shape].cast(type, dam, ty, tx, flags, flags_not,
									shapes)
end

-- Ripped from cmd7.c

-- runespell notional struct:
--
-- spell = {
--   mana = number,
--   type = dam.SOMETHING,
--   hardness = number,
--   runes.shapes = array of {
--       shape = RUNE_SHAPE_FOO
--       weight = number
--   },
-- }

function runes.exec(spell, cost)
	local dir
	local power_rune = 0
	local mana_used
	local plev = get_skill(SKILL_RUNECRAFT)

	local chance

	local rad = 0
	local ty, tx = -1, -1
	local dam = 0
	local flg = 0

	local power, pre_weight_power, weight = runes.calc_power(spell)

	wiz_print("Power : "..pre_weight_power..", Weight : "..weight)
	wiz_print("Weighted Power : "..power)

	if (pre_weight_power < 1) then
		message("You don't even manage to warm the runes.")
		return runes.get_mana(spell) * cost / 100
	end

	-- We have been asked to spend 'power' mana on the spell.
	-- cost is a discount factor (100 = normal cost)

	local sides, dice = runes.calc_dice(power)

	dam = rng.roll(sides, dice)

	wiz_print(format("Rune %dd%d = dam %d", sides, dice, dam))

	--  Extract the base spell failure rate
	chance = runes.spell_chance(spell, plev)

	wiz_print("Failure chance: "..chance.."%")

	--  Failure ?
	if (rng.number(100) < chance) then
		local insanity = (player.msane() - player.csane()) * 100 / player.msane()
		local sfail

		--  Flush input if told so
		if (flush_failure) then message.flush() end

		--  Insane players can see something strange
		if (rng.number(100) < insanity) then
			sfail = get_rnd_line("sfail.txt")
			message("A cloud of %s appears above you.", sfail)
		else
			--  Normal failure messages
			message("You failed to get the spell off!")
		end

		-- sound(SOUND_FAIL)

	else -- success!
		-- Do we need a target?

		local target = fold(spell.rune_shapes, nil,
			function(t, shape)
				return t or runes.shape_needs_target(shape.shape)
			end)

		if target then
			-- At least one of the runes involved wants a target

			local ret, dir = get_aim_dir()

			if (ret == false) then return nil end

			--  Hack -- Use an actual "target"
			if (dir == 5) then
				tx = target_col
				ty = target_row
			else
				tx = player.px + (ddx(dir) * 99)
				ty = player.py + (ddy(dir) * 99)
			end
		else
			ty = player.py
			tx = player.px
		end

		local is_future = false
		for i = 1, getn(spell.rune_shapes) do
			if spell.rune_shapes[i].shape == FLAG_RUNE_SHAPE_FUTURE then
				is_future = true
			end
		end

		if is_future then
			wiz_print("Future rune dectected, only executing it now")
			runes.shape_cast(FLAG_RUNE_SHAPE_FUTURE, spell.dam, dam, ty, tx,
							 spell.proj_flags, spell.proj_flags_not,
							 spell.rune_shapes)
		else
			foreachi (spell.rune_shapes, function (i, s)
				runes.shape_cast(s.shape, %spell.dam, %dam, %ty, %tx,
								 %spell.proj_flags, %spell.proj_flags_not)
			end)
		end
	end
	return runes.get_mana(spell) * cost / 100
end -- runes.exec()

function runes.calc_power(spell)
	local plev = get_skill(SKILL_RUNECRAFT)
	local power

	local runeweight

	power = runes.get_mana(spell)

	-- A scaling factor based on skill and rune hardness
	power = power * (plev - spell.hardness + 70) / 50

	local pre_weight_power = power

	-- A scaling factor based on the rune attack shapes used. More
	-- powerful runes will do less damage, and so will using multiple
	-- runes together

	runeweight = fold(spell.rune_shapes, 0,
		function(n, shape)
			return n + shape.weight
		end)

	power = power * 10 / runeweight

	return power, pre_weight_power, runeweight
end -- runes.calc_power()

function runes.calc_dice(power)
	local dice, sides

	-- Magic formula alert ;-P
	-- Note that making dice and sides both prop. to sroot(mana)
	-- you make damage prop. to mana, which is reasonable enough
	-- (although different from school magic, where damage tends to
	-- be proportional to mana^2, at least asymptotically)

	dice = 37 * sroot(power) / 10

	--  I don't know what the following comment is referring to - JMLB
	--  To reduce the high level power, while increasing the low levels

	sides = dice / 3

	if (sides < 1) then sides = 1 end

	--  Use the spell multiplicator
	if (player.to_s / 2) >= 1 then
		dice = dice * player.to_s / 2
	end

	return dice, sides
end

function runes.spell_chance(spell, level)
	local chance

	-- Base failure is based on hardness
	chance = spell.hardness * 3

	-- Increase if you are trying cast too much of your max mana at once
	local mana = runes.get_mana(spell)
	if mana > 5 then
		chance = chance + (mana - 5) * 300 / player.msp()
	end

	-- Reduce by runecasting proficiency
	chance = chance - 3 * get_skill_scale(SKILL_RUNECRAFT, 50)

	-- Reduce by INT bonus
	chance = chance - (((player.stat(A_INT) - 10) * 2) / 3)

	-- Extract the minimum failure rate
	local minfail
	if player.stat(A_INT) < 10 then
		minfail = (10 - player.stat(A_INT)) * 10
	elseif player.stat(A_INT) < 40 then
		minfail = 10 - (((player.stat(A_INT) - 10) * 30) / 100)
	else
		minfail = 0
	end

	chance = bound(chance, minfail, 95)

	return chance
end

runes.memorized = {}
add_loadsave("runes.memorized", {})
hook(hook.BIRTH, function() runes.memorized = {} end)

MAX_RUNES = 22 -- max number you are allowed to memorise


add_mkey
{
	["mkey"] 	= "Runecraft",
	["type"]    = "skill",
	["fct"] 	= function()
		local act =
			choose_from_list(runes.actions, "Actions",
			"Perform which action?",
			runes.action_shortinfo)

		if act and (act["do"]) then act["do"]() end
	end
}

runes.actions = {
	{
		["name"] = "Directly cast a Rune Spell",
		["do"] = function()
			if not runes.caster_ok() then return end

			local spell

			spell = runes.get_spell(true)

			if spell then
				if player.csp() >= runes.get_mana(spell) then
					-- Cast at full cost
					runes.do_cast(spell, 100)
				else
					message("You do not have enough mana.")
				end
			end
		end
	},
	{
		["name"] = "Memorize a Rune Spell",
		["do"] = function()
			-- Not when confused
			if player.has_intrinsic(FLAG_CONFUSED) then
				message("You are too confused!")
				return
			end

			if (getn(runes.memorized) >= MAX_RUNES) then
				message("You have already memorized the maximun number of runespells!")
				return
			end

			local rune_spell = runes.get_spell(false, true)

			if rune_spell then
				local name = get_string("Name this runespell: ")

				if not name then
					player.window[FLAG_PW_PLAYER] = true
					return
				end

				name = clean_whitespace(name)

				if name == "" then
					player.window[FLAG_PW_PLAYER] = true
					return
				end

				rune_spell.name = name

				tinsert(runes.memorized, rune_spell)

				message("You commit the spell to memory.")

				-- Take a turn
				energy_use = get_player_energy(SPEED_RUNECRAFT)

				-- Window stuff
				player.window[FLAG_PW_PLAYER] = true
			end
		end
	},
	{
		["name"] = "Rename a runespell",
		["do"] = function()
			-- Not when confused
			if player.has_intrinsic(FLAG_CONFUSED) then
				message("You are too confused!")
				return
			end

			if (getn(runes.memorized) == 0) then
				message("You have no rune spells memorized!")
				return
			end

			local spell

			spell = choose_from_list(runes.memorized,
				"Rename which spell?",
				"Spells",
				runes.spell_shortinfo, runes.spell_longinfo)

			if spell then
				local index

				for i=1, getn(runes.memorized) do
					if runes.memorized[i] == spell then index = i break end
				end

				if index then
					-- Window stuff
					player.window[FLAG_PW_PLAYER] = true

					local msg = "Rename '" .. runes.memorized[index].name ..
						"' to what? "
					local name = get_string(msg)

					if not name then
						return
					end

					name = name clean_whitespace(name)

					if name == "" then
						return
					end

					runes.memorized[index].name = name

					message("You renamed the spell.")

					-- Don't take any energy for simply renaming a spell
				else
					message("Serious Bug: spell isn't there any more")
				end
			end
		end
	},
	{
		["name"] = "Reorder runespells",
		["do"] = function()
			-- Not when confused
			if player.has_intrinsic(FLAG_CONFUSED) then
				message("You are too confused!")
				return
			end

			if (getn(runes.memorized) == 0) then
				message("You have no rune spells memorized!")
				return
			end

			if (getn(runes.memorized) == 1) then
				message("You only have one rune spell memorized.")
				return
			end

			local spell

			spell = choose_from_list(runes.memorized,
				"Move which spell?",
				"Spells",
				runes.spell_shortinfo, runes.spell_longinfo)

			if spell then
				local index

				for i=1, getn(runes.memorized) do
					if runes.memorized[i] == spell then index = i break end
				end

				if index then
					-- Window stuff
					player.window[FLAG_PW_PLAYER] = true

					local msg = "Move to which letter?"
					local ret, key = get_com("Move to what letter?")

					if not ret then
						return
					end

					local char = strlower(strchar(key))
					local new_index = strbyte(char) - strbyte('a') + 1

					if new_index < 1 or new_index > getn(runes.memorized) then
						message("Invalid letter.")
						return
					end

					if new_index == index then
						message("But that's the letter the spell already " ..
								"has!")
						return
					end

					-- Move spell position
					local spell_struct = tremove(runes.memorized, index)
					tinsert(runes.memorized, new_index, spell_struct)

					message("You moved the spell's position.")

					-- Don't take any energy for simply reordering spell
					-- list
				else
					message("Serious Bug: spell isn't there any more")
				end
			end
		end
	},
	{
		["name"] = "Forget a Rune Spell",
		["do"] = function()
			-- Not when confused
			if player.has_intrinsic(FLAG_CONFUSED) then
				message("You are too confused!")
				return
			end

			if (getn(runes.memorized) == 0) then
				message("You have no rune spells memorized!")
				return
			end

			local spell

			spell = choose_from_list(runes.memorized,
				"Forget which spell?",
				"Spells",
				runes.spell_shortinfo, runes.spell_longinfo)

			if spell then
				local index

				for i=1, getn(runes.memorized) do
					if runes.memorized[i] == spell then index = i break end
				end

				if index then
					tremove(runes.memorized, index)

					message("You forget the spell.")

					-- Take a turn
					energy_use = get_player_energy(SPEED_RUNECRAFT)

					-- Window stuff
					player.window[FLAG_PW_PLAYER] = true
				else
					message("Serious Bug: spell isn't there any more")
				end
			end
		end
	},
	{
		["name"] = "Cast a Rune Spell from Memory",
		["do"] = function()
			if not runes.caster_ok() then return end

			if (getn(runes.memorized) == 0) then
				message("You have no rune spells memorized!")
				return
			end

			local spell

			spell = choose_from_list(runes.memorized,
				"Cast which spell?",
				"Spells",
				runes.spell_shortinfo, runes.spell_longinfo)
			if spell then
				-- Need the runes
				if not runes.test_spell(spell) then
					message("You lack some essential rune(s) for this runespell!")
					return
				end

				if player.csp() >= runes.get_mana(spell) then
					-- Cast at full cost
					runes.do_cast(spell, 100)
				else
					message("You do not have enough mana.")
				end
			end
		end
	},
	{["name"] = "Carve a Rune Spell onto a Runestone",
		["do"] = function ()
			if not runes.caster_ok() then return end

			local spell

			local ret, item =
				get_item("Which runestone?", "You have no blank runestones.",
				USE_INVEN,
				function(o)
					if o.tval == TV_RUNE2
						and has_flag(o, FLAG_RUNE_STONE)
						and not has_flag(o, FLAG_RUNE_DAM) then
						return true
					else return false end
				end)

			if ret == false then return end

			spell = runes.get_spell()

			if spell then
				local o = get_object(item)
				set_flag(o, FLAG_RUNE_DAM, spell.dam)
				set_flag(o, FLAG_RUNE_STONE, runes.get_mana(spell))
				set_flag(o, FLAG_RUNE_HARDNESS, spell.hardness)
				foreachi(spell.rune_shapes, function(i, s)
					set_flag(%o, s.shape, s.weight)
				end)
			end
		end
	},

	{["name"] = "Cast from a Runestone",
		["do"] = function()
			if not runes.caster_ok() then return end

			local spell={}

			local ret, item =
				get_item("Which runestone?", "You have no usable runestones.",
				USE_INVEN,
				function(o)
					if o.tval == TV_RUNE2
						and has_flag(o, FLAG_RUNE_STONE)
						and has_flag(o, FLAG_RUNE_DAM) then
						return true
					else return false end
				end)

			if ret == false then return end

			local o = get_object(item)
			spell.dam=get_flag(o, FLAG_RUNE_DAM)
			spell.hardness=get_flag(o, FLAG_RUNE_HARDNESS)
			spell.mana=get_flag(o, FLAG_RUNE_STONE)
			spell.mana_type=runes.mana_types.ABSOLUTE
			spell.rune_shapes={}

			for k, v in runes.shapes do
				local w = get_flag(o, k)
				if w > 0 then
					tinsert(spell.rune_shapes, {shape=k, weight=w})
				end
			end

			if player.csp() * 80 / 100 >= spell.mana then
				runes.do_cast(spell, 80) -- 80% cost
			else
				message("You do not have enough mana.")
			end
		end
	},
}

function runes.action_shortinfo (y, x, a) if a then term.blank_print(a.name, y, x) end end

function runes.spell_shortinfo (y, x, s)
	if s then
		term.blank_print(format("%-20s %4d %4d%% %s", s.name,
								runes.get_mana(s), runes.spell_chance(s),
								runes.info_field(s)),
						 y, x)
	else
		term.blank_print(format("%-20s %-4s %-4s%% %s", "Name", "Mana",
								"Fail", "Info"),
						 y, x)
	end
end -- runes.spell_shortinfo()

function runes.spell_longinfo (y, spell)
	term.print(color.LIGHT_BLUE,
			   "Damage type: " .. runes.damage_name(spell),
			   y, 0)

	local shapes = {}
	for i = 1, getn(spell.rune_shapes) do
		tinsert(shapes, runes.shapes[spell.rune_shapes[i].shape].name)
	end
	local shapes_str = strlower(strjoin(shapes, ", "))

	term.print(color.LIGHT_BLUE, "Shapes:      " .. shapes_str, y + 1, 0)

	local mana_info
	if spell.mana_type == runes.mana_types.ABSOLUTE then
		mana_info = spell.mana .. " SP, always"
	elseif spell.mana_type == runes.mana_types.SKILL then
		mana_info = spell.mana .. "% of Runecrafting skill level"
	elseif spell.mana_type == runes.mana_types.MAX_SP then
		mana_info = spell.mana .. "% of maximum SP"
	else
		error("Impossible!")
	end

	term.print(color.LIGHT_BLUE, "Mana:        " .. mana_info, y + 2, 0)
end -- runes.spell_longinfo()

function runes.damage_name(spell)
	return spell.dam_name or get_dam_type_info(spell.dam, "desc")
end

function runes.info_field(spell)
	local spell_info = {}

	local power, pre_weight_power = runes.calc_power(spell)

	if (pre_weight_power < 1) then
		return "not enough power to cast"
	end

	local dice, sides = runes.calc_dice(power)
	local min_dam  = dice
	local max_dam  = dice * sides
	local dam_type = spell.dam

	if spell.dam_info then
		local idx1 = spell.dam_info[1]
		local idx2 = spell.dam_info[2]
		local func = __functions_registry[idx1][idx2]

		local str = func(spell, min_dam, max_dam)

		if not str then
			return ""
		elseif str ~= "" then
			tinsert(spell_info, str)
		end
	else
		tinsert(spell_info, "dam " .. dice .. "d" .. sides)
	end

	local shape_info = {}
	for i = 1, getn(spell.rune_shapes) do
		local shape = spell.rune_shapes[i].shape
		local func  = runes.shapes[shape].info

		if func then
			tinsert(shape_info, func(dam_type, min_dam, max_dam))
		end
	end

	if getn(shape_info) > 0 then
		local shape_str = strjoin(shape_info, ", ")
		tinsert(spell_info, shape_str)
	end

	return strjoin(spell_info, ", ")
end -- runes.info_field()

function runes.caster_ok()
	-- Not when confused
	if player.has_intrinsic(FLAG_CONFUSED) then
		message("You are too confused!")
		return nil
	end

	-- Require some mana
	if (player.csp() <= 0) then
		message("You have no mana!")
		return nil
	end

	-- No magic
	if (player.antimagic > 0) then
		message("Your anti-magic field disrupts any magic attempts.")
		return nil
	end

	-- No magic
	if player.anti_magic then
		message("Your anti-magic shell disrupts any magic attempts.")
		return nil
	end

	-- OK
	return 1
end

function runes.do_cast(spell, cost)
	-- Execute at normal mana cost
	local mana_used
	mana_used = runes.exec(spell, cost)
	if mana_used then
		player.csp(player.csp() - mana_used)

		energy_use = get_player_energy(SPEED_RUNECRAFT)
	end
end

runes.mana_types = {
	ABSOLUTE = 1,
	SKILL    = 2,
	MAX_SP   = 3
}

function runes.choose_spell_mana(spell, allow_mana_relative)
	if not allow_mana_relative then
		local power = get_quantity("How much mana? ", player.csp())

		if (power < 1) then power = 1 end

		spell.mana      = power
		spell.mana_type = runes.mana_types.ABSOLUTE
		return
	end

	local ret, key = get_com("Specify mana as [A]bsolute, or relative to " ..
							 "[S]kill or [M]ax SP?")

	if not ret then
		spell.mana      = 1
		spell.mana_type = runes.mana_types.ABSOLUTE
		return
	end

	local char = strlower(strchar(key))

	if not (char == 'a' or char == 's' or char == 'm') then
		spell.mana      = 1
		spell.mana_type = runes.mana_types.ABSOLUTE
		return
	end		

	if char == 'a' then
		local power = get_quantity("How much mana? ", player.msp())

		if (power < 1) then power = 1 end

		spell.mana      = power
		spell.mana_type = runes.mana_types.ABSOLUTE
		return
	end

	if char == 's' then
		local skill         = get_skill(SKILL_RUNECRAFT)
		local max_skill_pct = player.msp() * 100 / skill

		local pct = get_quantity("What percent of skill level? ",
								 max_skill_pct)

		pct = max(pct, 1)

		spell.mana      = pct
		spell.mana_type = runes.mana_types.SKILL
		return
	end

	if char == 'm' then
		local pct = get_quantity("What percent of maximum SP? ", 100)

		pct = max(pct, 1)

		spell.mana      = pct
		spell.mana_type = runes.mana_types.MAX_SP
		return
	end

	error("Impossible!")
end -- runes.choose_spell_mana()

function runes.get_mana(spell)
	local amnt = spell.mana
	local typ  = spell.mana_type

	if typ == runes.mana_types.ABSOLUTE then
		return amnt
	elseif typ == runes.mana_types.SKILL then
		return max(amnt * get_skill(SKILL_RUNECRAFT) / 100, 1)
	elseif typ == runes.mana_types.MAX_SP then
		return max(amnt * player.msp() / 100, 1)
	else
		error("Impossible!")
	end
end

function runes.get_spell(allow_mana_redo, allow_mana_relative)
	local item

	local rune_combine = 0
	local ret, item
	local object
	local spell = {
		["rune_shapes"]    = {},
		["proj_flags"]     = 0,
		["proj_flags_not"] = 0,
	}

	-- Get an item
	local ret, item =
		get_item("Which primary rune?", "You have no primary runes.",
		USE_INVEN,
		function(o)
			return has_flag(o, FLAG_RUNE_DAM)
		end)

	if not ret then return nil end

	-- Get the item (in the pack)
	object = get_object(item)

	spell.dam = get_flag(object, FLAG_RUNE_DAM)
	spell.hardness = get_flag(object, FLAG_RUNE_HARDNESS)

	if spell.dam == 0 or spell.hardness == 0 then
		message("Serious Bug: RUNE1 without DAM or HARDNESS")
		return
	end

	spell.proj_flags = spell.proj_flags | get_flag(object, FLAG_RUNE_PROJ)
	spell.proj_flags_not = (spell.proj_flags_not |
							get_flag(object, FLAG_RUNE_PROJ_NOT))
	spell.dam_name = object.flags[FLAG_RUNE_DAM_NAME]

	if object.flags[FLAG_RUNE_DAM_INFO] then
		spell.dam_info = {
			get_flag(object, FLAG_RUNE_DAM_INFO),
			get_flag2(object, FLAG_RUNE_DAM_INFO)
		}
	end

	local dam_exclude_shapes = object.flags[FLAG_RUNE_EXCL_SHAPES] or {}

	while not nil do
		-- Restrict choices to unused secondary runes
		local aux_q = ""
		if (getn(spell.rune_shapes)>0) then
			aux_q=" ["..getn(spell.rune_shapes).." so far]"
		end
		ret, item = get_item("Which secondary rune?"..aux_q,
			"You have no more secondary runes",
			USE_INVEN,
			function (o)
				if has_flag(o, FLAG_RUNE_SHAPE) and
					(not has_flag(o, FLAG_RUNE_STONE)) then
					local this_shape = get_flag(o, FLAG_RUNE_SHAPE)
					local this_info  = runes.shapes[this_shape]

					-- Incompatible with damage rune?
					if %dam_exclude_shapes[this_shape] then
						return nil
					end

					-- Check already used shapes
					for i = 1, getn(%spell.rune_shapes) do
						local shape_table = %spell.rune_shapes[i]
						local used_shape  = shape_table["shape"]
						local used_info   = runes.shapes[used_shape]

						-- Already using that shape?
						if used_shape == this_shape then
							return nil
						end

						-- Is this rune stand-alone?
						if this_info["exclude"][1] == "ALL" then
							return nil
						end

						-- Is the already used rune stand-alone?
						if used_info["exclude"][1] == "ALL" then
							return nil
						end

						-- Incompatible shapes?
						for i = 1, getn(this_info["exclude"]) do
							if this_info["exclude"][i] ==
								used_info["name"] then
								return nil
							end
						end

						for i = 1, getn(used_info["exclude"]) do
							if used_info["exclude"][i] ==
								this_info["name"] then
								return nil
							end
						end
					end -- Check already used shapes

					-- Is a secondary rune, not a runsestone, not
					-- already used, and no conlficts with already
					-- used shapes
					return true
				end -- If object is secondary rune and not a runestone

				-- Not a secondary rune
				return nil
			end -- End anonymous function
		) -- get_item()

		if not ret then break end

		-- Get the item (in the pack)
		object = get_object(item)
		local new_shape = {
			["shape"] = get_flag(object, FLAG_RUNE_SHAPE),
			["weight"] = get_flag(object, FLAG_RUNE_WEIGHT),
		}

		if (new_shape.shape == 0 or new_shape.weight == 0) then
			message(color.VIOLET, "Serious bug: RUNE2 without SHAPE and WEIGHT")
			return
		end

		spell.proj_flags = spell.proj_flags | get_flag(object, FLAG_RUNE_PROJ)
		spell.proj_flags_not = (spell.proj_flags_not |
								get_flag(object, FLAG_RUNE_PROJ_NOT))

		tinsert(spell.rune_shapes, new_shape)
	end -- Get secondary runes

	if getn(spell.rune_shapes) == 0 then
		message("You have not selected any secondary runes!")
		return
	end

	runes.choose_spell_mana(spell, allow_mana_relative)

	while allow_mana_redo and runes.spell_chance(spell ) > 0 do
		local ret, opt = get_com("Spell has a " .. runes.spell_chance(spell) ..
								 "% chance of failure.  " ..
									 "(A)ccept, (c)hange mana, or (g)ive up?")

		if not ret then
			break
		end

		local char = strchar(opt)

		if char == "a" then
			break
		end

		if char == "g" then
			return nil
		end

		runes.choose_spell_mana(spell, allow_mana_relative)
	end

	return spell
end -- runes.get_spell()

function runes.test_spell(spell)
	-- collect inventory runes in two tables
	local got_dams = {}
	local got_shapes = {}

	for i = 1, player.inventory_limits(INVEN_INVEN) do
		local object = player.inventory[INVEN_INVEN][i]

		if (object) then
			-- Does the rune1(type) match ?
			if has_flag(object,FLAG_RUNE_DAM) then
				local dam = get_flag(object, FLAG_RUNE_DAM)

				assert(dam ~= 0)

				got_dams[dam] = 1
			end

			if has_flag(object,FLAG_RUNE_SHAPE) and get_flag(object, FLAG_RUNE_STONE) == 0 then
				local shape = get_flag(object, FLAG_RUNE_SHAPE)
				got_shapes[shape] = 1
			end
		end
	end

	-- Need all runes to be present

	if not got_dams[spell.dam] then return nil end

	return fold(spell.rune_shapes, 1,
		function (b, s) return b and %got_shapes[s.shape] end
	)
end
