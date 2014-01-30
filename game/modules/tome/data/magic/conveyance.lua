-- handle the conveyance school
declare_global_constants {
	"BLINK",
	"DISARM",
	"PROBABILITY_TRAVEL",
	"RECALL",
	"TELEAWAY",
	"TELEPORT",
	"destroy_door",
	"unlock_all",
	"disarm_all",
	"destroy_doors_touch",
	"disarm_traps_touch",
	"unlock_all_touch",
	"pop_through_wall",
}

BLINK = add_spell
{
	["name"]      = "Phase Door",
	["school"]    = {SCHOOL_CONVEYANCE},
	["level"]     = 1,
	["mana"]      = 1,
	["mana_max"]  =  3,
	["fail"]      = 10,
	["action"]    = "@Source@ blinks.",
	["inertia"]   = { 1, 5 },
	["no_target"] = true,

	["symb_rebel"]     = true,
	["symb_rebel_msg"] = "@Source@ blinks you.",

	tactical =
	{
		[ai.tactics.ESCAPE] = 4
		[ai.tactics.TACTIC] = 8
	}
	["spell"] = 	function()
			local range = 10 + get_cast_level(8)
			if spell_caster == 0 then
				if get_cast_level() >= 60 then
					local oy, ox = player.py, player.px
					local path = {}
					local ret, dir = get_aim_dir{no_los=true no_path=true save_path=path}
					if not ret then return SPELL_NOTHING end
					local ty, tx
					if dir == 5 then
						for i = getn(path.path), 1, -1 do
							ty = path.path[i][1]
							tx = path.path[i][2]
							if distance(player.py, player.px, ty, tx) <= range then break end
						end
					else
						-- Use the given direction
						local dy, dx = explode_dir(dir)
						ty = player.py + (dy * range)
						tx = player.px + (dx * range)
					end
					player.py = ty
					player.px = tx
					lite_spot(oy, ox)

					teleport_player(range / 3)
					return true
				else
					teleport_player(range)
					return true
				end
			else
				teleport_away(spell_caster, range)
				return true
			end
	end,
	["info"] = 	function()
	       		return "distance "..(10 + get_cast_level(8))
	end,
	["desc"] =	{
			"Teleports you on a small scale range",
			"At level 60 it allows to specify the general target area of the blink",
	}
}

function destroy_door(who, y, x)
	local c_ptr   = cave(y, x)
	local feat    = f_info[c_ptr.feat + 1]
	local obvious = false

	if not feat.flags[FLAG_DOOR] then
		return obvious
	end

	if feat.flags[FLAG_PERMANENT] then
		-- Can't destroy permanent features.
		return obvious
	end

	if player_has_los_bold(y, x) and not player.has_intrinsic(FLAG_BLIND) then
		obvious = true
		message("There is a bright flash of light!")
		player.redraw[FLAG_PR_MAP] = 1
	end

	c_ptr.feat = FEAT_FLOOR

	return obvious
end -- destroy_door()

function unlock_all(who, y, x)
	local c_ptr   = cave(y, x)
	local feat    = f_info[c_ptr.feat + 1]
	local obvious = false

	if c_ptr.feat ~= FEAT_BROKEN and feat.flags[FLAG_DOOR] and
		not feat.flags[FLAG_OPEN] and feat.flags[FLAG_OPEN_DOOR] then
		c_ptr.feat = feat.flags[FLAG_OPEN_DOOR]
		if player_has_los_bold(y, x) then
			message("Click!")
			obvious = true

			if not player.has_intrinsic(FLAG_BLIND) then
				player.redraw[FLAG_PR_MAP] = 1
			end
		end
	end

	-- Needs to be put in a table to be modifiable by inner-function
	local obvious_table = {obvious}

	for_inventory_inven(c_ptr.inventory,
			function(obj)
				if obj.flags[FLAG_LOCKED] then
					obj.flags[FLAG_LOCKED]   = nil
					obj.flags[FLAG_UNLOCKED] = true

					if player_has_los_bold(%y, %x) then
						message("Click!")
						%obvious_table[1] = true
					end
				end
			end)

	return obvious_table[1]
end -- unlock_all()

-- Flash of light for magic disarming of floor traps.
hook(hook.TRAP_DISARM_POST,
function(manual, trap, who, y, x, obj)
	if not manual and not obj and player_has_los_bold(y, x) and
		not player.has_intrinsic(FLAG_BLIND) then
			message("There is a bright flash of light!")
	end
end)

function disarm_all(who, y, x)
	local c_ptr    = cave(y, x)
	local feat     = f_info[c_ptr.feat + 1]
	local obvious  = false

	for_inventory_inven(c_ptr.inventory,
			function(obj)
				traps.disarm_all_object_traps(false, %who, obj)
			end)

	local ret, ret_arr = traps.disarm_all_location_traps(false, who, y, x)

	if getn(ret_arr) > 0 and player_has_los_bold(y, x) and
		not player.has_intrinsic(FLAG_BLIND) then
			obvious = true
			player.redraw[FLAG_PR_MAP] = 1
	end

	return obvious
end -- disarm_all()

function destroy_doors_touch(who, y, x)
	local obvious = false

	who = who or 0
	y   = y   or player.py
	x   = x   or player.px

	for yy = y - 1, y + 1 do
		for xx = x - 1, x + 1 do
			if in_bounds(yy, xx) then
				obvious = destroy_door(who, yy, xx) or obvious
			end
		end
	end

	return obvious
end -- destroy_doors_touch()

function disarm_traps_touch(who, y, x)
	local obvious = false

	who = who or 0
	y   = y   or player.py
	x   = x   or player.px

	for yy = y - 1, y + 1 do
		for xx = x - 1, x + 1 do
			if in_bounds(yy, xx) then
				obvious = disarm_all(who, yy, xx) or obvious
			end
		end
	end

	return obvious
end -- disarm_traps_touch()

function unlock_all_touch(who, y, x)
	local obvious = false

	who = who or 0
	y   = y   or player.py
	x   = x   or player.px

	for yy = y - 1, y + 1 do
		for xx = x - 1, x + 1 do
			if in_bounds(yy, xx) then
				obvious = unlock_all(who, yy, xx) or obvious
			end
		end
	end

	return obvious
end -- disarm_all_touch()

DISARM = add_spell
{
	["name"] = 	"Disarm",
	["school"] = 	{SCHOOL_CONVEYANCE},
	["level"] = 	5,
	["mana"] = 	2,
	["mana_max"] = 	4,
	["fail"] = 	15,
	["stick"] =
	{
			["charge"] =    { 10, 15 },
			[TV_ORB] =
			{
				["rarity"] = 		4,
				["base_level"] =	{ 1, 10 },
				["max_level"] =		{ 10, 50 },
			},
	},
	["spell"] = 	function()
			local obvious
			obvious = destroy_doors_touch()
			if get_cast_level(50) >= 10 then
				obvious = is_obvious(disarm_traps_touch(), obvious)
			end
			return obvious
	end,
	["info"] = 	function()
			return ""
	end,
	["desc"] =	{
			"Destroys doors",
			"At level 20 it disarms traps",
	}
}

TELEPORT = add_spell
{
	["name"] = 	"Teleportation",
	["school"] = 	{SCHOOL_CONVEYANCE},
	["level"] = 	19,
	["mana"] = 	8,
	["mana_max"] = 	14,
	["fail"] = 	30,
	["stick"] =
	{
			["charge"] =    { 7, 7 },
			[TV_ORB] =
			{
				["rarity"] = 		50,
				["base_level"] =	{ 1, 20 },
				["max_level"] =		{ 20, 50 },
			},
	},
	["inertia"]   = { 1, 10 },
	["no_target"] = true,

	["symb_rebel"]     = true,
	["symb_rebel_msg"] = "@Source@ teleports you.",
	tactical =
	{
		[ai.tactics.ESCAPE] = 10
		[ai.tactics.TACTIC] = 1
	}
	["spell"] = 	function()
			if spell_caster == 0 then
				player.energy = player.energy -
					((get_player_energy(SPEED_CAST) *
					  (25 - get_cast_level(50))) / 100)
				teleport_player(100 + get_cast_level(100))
			else
				teleport_away(spell_caster, 100 + get_cast_level(100))
			end
			return true
	end,
	["action"] =	"@Source@ teleports away.",
	["info"] = 	function()
			return "distance "..(100 + get_cast_level(100))
	end,
	["desc"] =	{
			"Teleports you around the level. The casting time decreases with level",
	}
}

TELEAWAY = add_spell
{
	["name"] = 	"Teleport Away",
	["school"] = 	{SCHOOL_CONVEYANCE},
	["level"] = 	45,
	["mana"] = 	15,
	["mana_max"] = 	40,
	["fail"] = 	60,
	["stick"] =
	{
			["charge"] =    { 3, 5 },
			[TV_ORB] =
			{
				["rarity"] = 		75,
				["base_level"] =	{ 1, 20 },
				["max_level"] =		{ 20, 50 },
			},
	},
	tactical =
	{
		[ai.tactics.ESCAPE] = 10
		[ai.tactics.TACTIC] = 2
	}
	["spell"] = 	function()
	       		local ret, dir

			if get_cast_level(50) >= 20 then
				return project_los(dam.TELEPORT_AWAY, 100)
			elseif get_cast_level(50) >= 10 then
				ret, dir = get_aim_dir()
				if not ret then return SPELL_NOTHING end
				return fire_ball(dam.TELEPORT_AWAY, dir, 100, 3 + get_cast_level(4))
			else
				ret, dir = get_aim_dir()
				if not ret then return SPELL_NOTHING end
				return fire_beam(dam.TELEPORT_AWAY, dir, 100)
			end
	end,
	["info"] = 	function()
			return ""
	end,
	["desc"] =	{
			"Teleports a line of monsters away",
			"At level 20 it turns into a ball",
			"At level 40 it teleports all monsters in sight"
	}
}

RECALL = add_spell
{
	["name"] = 	"Recall",
	["school"] = 	{SCHOOL_CONVEYANCE},
	["level"] = 	59,
	["mana"] = 	25,
	["mana_max"] = 	25,
	["fail"] =      60,
	["spell"] = 	function()
			local ret, x, y, c_ptr
			ret, x, y = tgt_pt()
			if not ret then return SPELL_NOTHING end
			c_ptr = cave(y, x)
			if (y == player.py) and (x == player.px) then
				local d = 21 - get_cast_level(15)
				if d < 0 then
					d = 0
				end
				local f = 15 - get_cast_level(10)
				if f < 1 then
					f = 1
				end
				if level_flags.flags[FLAG_NO_RECALL_OUT] then
					local msg = level_flags.flags[FLAG_NO_RECALL_OUT]

					if type(msg) == "string" then
						message(msg)
					else
						message("The spells seems dampened.")
					end
					return SPELL_NOTHING
				end
				recall_player(d, f)
				return true
			elseif c_ptr.m_idx > 0 then
				swap_position(y, x)
				return true
			elseif flag_max_key(c_ptr.inventory) > 0 then
				--[[
				set_target(y, x)
				if get_cast_level(50) >= 15 then
					fetch(5, 10 + get_cast_level(150), FALSE)
				else
					fetch(5, 10 + get_cast_level(150), true)
				end
				]]

				local max_weight = 10 + get_cast_level(150)
				local need_los = get_cast_level(50) < 15
				
				-- Test if the object can be gotten
				if flag_max_key(cave(player.py, player.px).inventory) > 0 then
					message("You can't fetch when you're already standing on something.")
					return SPELL_NOTHING
				end
				if distance(player.py, player.px, y, x) > 15 then
					message("You can't fetch something that far away!")
					return SPELL_NOTHING
				end
				if need_los and not player_has_los_bold(y,x) then
					message("You have no direct line of sight to that location.")
					return SPELL_NOTHING
				end

				-- Get the last item (the one at the bottom of the pile)
				local o_idx = flag_max_key(c_ptr.inventory)
				local o_ptr = c_ptr.inventory[o_idx]

				if o_ptr.weight > max_weight then
					message("The object is too heavy.")
					return SPELL_NOTHING
				end

				local o_desc = strcap(object_desc(o_ptr, true, -1))
				message(o_desc .. " flies to your feet.")

				-- That code is not very clean...
				local o_copy = new_object()
				object_copy(o_copy, o_ptr)
				o_copy.number = 1
				drop_near(o_copy, 0, player.py, player.px)

				o_ptr.number = o_ptr.number - 1
				if o_ptr.number == 0 then
					flag_remove(c_ptr.inventory, o_idx)
				end

				note_spot(player.py, player.px)
				player.redraw[FLAG_PR_MAP] = true

				return true
			end
	end,
	["info"] = 	function()
			local d = 21 - get_cast_level(15)
			if d < 0 then
				d = 0
			end
			local f = 15 - get_cast_level(10)
			if f < 1 then
				f = 1
			end
			return "dur "..f.."+d"..d.." weight "..(1 + get_cast_level(15)).."lb"
	end,
	["desc"] =	{
			"Cast on yourself it will recall you to the surface/dungeon.",
			"Cast at a monster you will swap positions with the monster.",
			"Cast at an object it will fetch the object to you."
	}
}

---------------------------
-- Probability travel stuff
---------------------------

PROBABILITY_TRAVEL = add_spell
{
	["name"] = 	"Probability Travel",
	["school"] = 	{SCHOOL_CONVEYANCE},
	["level"] = 	59,
	["mana"] = 	30,
	["mana_max"] = 	50,
	["fail"] = 	90,
	["stick"] =
	{
			["charge"] =    { 1, 2 },
			[TV_ORB] =
			{
				["rarity"] = 		97,
				["base_level"] =	{ 1, 5 },
				["max_level"] =		{ 8, 25 },
			},
	},
	["inertia"] = 	{ 6, 40 },
	["spell"] = 	function()
			local noupdown = player.has_intrinsic(FLAG_PROB_TRAVEL_NO_UPDOWN)
			local nowalls  = player.has_intrinsic(FLAG_PROB_TRAVEL_NO_WALLS)

			if noupdown and nowalls then
				message("Something blocks the probability travel spell.")
				return
			end

			timed_effect.inc(timed_effect.PROB_TRAVEL,
							 rng(20) + get_cast_level(60))
	end,
	["info"] = 	function()
			return "dur "..get_cast_level(60).."+d20"
	end,
	["desc"] =	{
		"Renders you immaterial, when you hit a wall you travel through it ",
		"and instantly appear on the other side of it. You can also float ",
		"up through the ceiling or down through the floor at will."
	}
}

hook(hook.STAIR_DO,
function(updown, ask_leave)
	if not player.has_intrinsic(FLAG_PROB_TRAVEL_UPDOWN) then
		return false
	end

	local dungeon = d_info[current_dungeon_idx + 1]
	local c_ptr   = cave(player.py, player.px)
	local levels

	if updown == "down" then
		levels = 1
	else
		levels = -1
	end

	-- Don't mess with quest changers.
	if cave_feat_is(c_ptr, FLAG_QUEST_CHANGER) then
		return false
	end

	-- Is the level changer going in the direction the player
	-- wants to go?  If so, let the C code handle it.
	if cave_feat_is(c_ptr, FLAG_LEVEL_CHANGER) then
		local change = cave_feat_get(c_ptr, FLAG_LEVEL_CHANGER)

		if (levels * change) > 0 then
			return false
		end
	end

	if dun_level == 0 then
		if updown == "down" then
			message("You can't use probability travel to enter a dungeon " ..
					"from the surface.")
		else
			message("You can't use probability travel to float up into " ..
					"the sky.")
		end
		return true, 0
	end

	if has_flag(level_flags, FLAG_FLAT) then
		message("You can't float up or down levels in a flat dungeon.")
		return true, 0
	end

	if updown == "down" and dungeon.maxdepth == dun_level then
		message("You are unable to float down through the floor.")
		return true, 0
	elseif updown == "up" and dungeon.mindepth == dun_level then
		message("You are unable to float up through the ceiling.")
		return true, 0
	end

	-- Anything stopping us?
	if (has_flag(level_flags, FLAG_PROB_TRAVEL_NO_UPDOWN) or
		cave_feat_is(c_ptr, FLAG_PROB_TRAVEL_NO_UPDOWN) or
			has_flag(c_ptr, FLAG_PROB_TRAVEL_NO_UPDOWN) or
			player.has_intrinsic(FLAG_PROB_TRAVEL_NO_UPDOWN)) then
		local msg = "Some powerfull force prevents you from "

		if updown == "down" then
			msg = msg .. "floating down through the floor."
		else
			msg = msg .. "floating up through the ceiling."
		end

		message(msg)

		return true, 0
	end

	-- Do it.
	local msg
	if updown == "down" then
		msg = "You float down through the floor."
	else
		msg = "You float up through the ceiling."
	end

	return true, levels, msg
end)

hook(hook.HIT_WALL,
function(y, x, dir)
	if not player.has_intrinsic(FLAG_PROB_TRAVEL_WALLS) then
		return false
	end

	if not in_bounds(y, x) then
		return false
	end

	if (player.has_intrinsic(FLAG_PROB_TRAVEL_NO_WALLS) or
		has_flag(level_flags, FLAG_PROB_TRAVEL_NO_WALLS)) then
		message("Some powerful force prevents you from travelling " ..
				"through the wall.")
		return false
	end

	local wall_ptr = cave(y, x)

	-- Must be a wall to start probability travel
	if not cave_feat_is(wall_ptr, FLAG_WALL) then
		return false
	end

	if (cave_feat_is(wall_ptr, FLAG_PROB_TRAVEL_NO_WALLS) or
		has_flag(wall_ptr, FLAG_PROB_TRAVEL_NO_WALLS)) then
		message("The wall seems strangely solid...")
		return false
	end

	if not pop_through_wall(dir) then
		message("You are unable to travel through the wall.")
		return false
	end

	return true
end)

-- This function is also used for the Dwarven racial ability
-- "find secret passages".
function pop_through_wall(dir)
	local dy = ddy[dir]
	local dx = ddx[dir]

	-- Must go somewhere
	if dy == 0 and dx == 0 then
		return false
	end

	local y = player.py
	local x = player.px

	while true do
		y = y + dy
		x = x + dx

		-- Give up if we hit the edge of the level
		if not in_bounds(y, x) then
			return false
		end

		local c_ptr = cave(y, x)

		-- Can't stop if there's a monster there, or the feature or grid
		-- has PROB_TRAVEL_NO_END set
		if (c_ptr.m_idx == 0 and
			not cave_feat_is(c_ptr, FLAG_PROB_TRAVEL_NO_END) and
				not has_flag(c_ptr, FLAG_PROB_TRAVEL_NO_END)) then
			-- We stop if the player can enter the feautre, OR
			-- if the feature isn't a wall (like a pit) since
			-- probability travel is about going through walls.
			if (player_can_enter(c_ptr.feat) or
				not cave_feat_is(c_ptr, FLAG_WALL)) then
				break
			end
		end
	end -- while true do
	-- Found a spot!

	-- Redraw old spot
	lite_spot(player.py, player.px)

	-- Redraw new spot
	lite_spot(y, x)

	-- Move and check for new panel
	player.py = y
	player.px = x
	verify_panel()

	-- Update stuff
	player.update = (player.update | PU_VIEW | PU_FLOW | PU_MON_LITE |
					 PU_DISTANCE)
	player.window[FLAG_PW_OVERHEAD] = true
	handle_stuff()

	return true
end -- pop_through_wall()
