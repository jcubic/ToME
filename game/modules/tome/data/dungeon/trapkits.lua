--------------------------------------------------------------
-- Trap kits (monster traps)
--------------------------------------------------------------
declare_global_constants {"TRAP_TRAPKIT"}

-- Hook trapping ability invocation
add_mkey
{
	mkey 	= "Trapping",
	type 	= "ability",
	fct 	= function()
		local y = player.py
		local x = player.px
		local c_ptr = cave(y, x)
		local feat  = f_info[c_ptr.feat + 1]

		if player.has_intrinsic(FLAG_BLIND) or no_lite() then
			message("You need to be able to see to set a trap.")
			return
		end

		if player.has_intrinsic(FLAG_CONFUSED) then
			message("You are too confused confused to set a trap.")
			return
		end

		if player.has_intrinsic(FLAG_HALLUCINATE) then
			message("You can't set traps while hallucinating.")
			return
		end

		if not feat.flags[FLAG_FLOOR] then
			message("You can only set a trap on the floor.")
			return
		end

		if feat.flags[FLAG_DOOR] then
			message("You can't set a trap in a doorway.")
			return
		end

		if not is_inven_empty(c_ptr.inventory) then
			message("You can't set a trap on the floor when " ..
				"there's an object in the way.")
			return
		end

		if get_num_location_traps(y, x, true) > 0 then
			message("This square is already trapped.")
			return
		end

		-- Nothing obvious preventing us from setting a trap, so get
		-- the trapkit to use
		local question = "Which trap kit?"
		local no_trap = "You have no trap kit with which to set a trap."

		local ret, kit_item = get_item(question, no_trap, USE_INVEN,
			function(obj) return obj.tval == TV_TRAPKIT end,
			"t")

		if not ret then
			return
		end

		local trapkit = get_object(kit_item)

		if check_prevent_cmd(trapkit, 't') then
			return
		end

		-- Get the ammo which should be put in the trap
		local func
		local ammo_objs

		if trapkit.flags[FLAG_NO_AMMO] then
			ammo_objs = {}
		else
			func = get_function_registry_from_flag(trapkit.flags,
											   FLAG_SELECT_AMMO)

			ammo_objs = func(trapkit)

			if not ammo_objs or getn(ammo_objs) == 0 then
				return
			end
		end

		-- Now we're actually making the attempt, so use energy
		energy_use = get_player_energy(SPEED_DISARM)

		local trap = traps.make_trap(traps.trap_list[TRAP_TRAPKIT],
						 FACTION_PLAYER)

		if not trap then
			return
		end

		-- If there's an not-yet-aware-of trap here (created under the
		-- player's feet?), then become aware of it when attempting to
		-- set the trap.
		if get_num_location_traps(y, x) > 0 then
			message("While attempting to set the trap, you discover " ..
				"that one is already set here!")
			traps.find_one_location_trap(y, x)
			return
		end

		-- No problems yet, attempt to place the trapkit
		local temp_kit = object_dup(trapkit)
		temp_kit.number = 1

		trap[FLAG_TRAPKIT_OBJ] = temp_kit

		if not trapkit.flags[FLAG_NO_AMMO] then
			local obj_list = flag_new()

			for i = 1, getn(ammo_objs) do
				local obj

				obj        = object_dup(ammo_objs[i].obj)
				obj.number = ammo_objs[i].number

				obj_list[i] = obj
			end

			trap[FLAG_AMMO_OBJ] = obj_list
		end

		if not traps.place_location_trap(trap, y, x, true) then
			-- Was the trap freed during placement?
			if not trap[FLAG_GONE] then
				-- flag_free() takes care of deleting the objects
				flag_free(trap, true)
			end

			return
		end

		-- Trap kit placed, remove trapkit and ammo from inventory
		message("The " .. object_desc(temp_kit) .. " has been placed " ..
			"and armed.")

		-- Do this via bulk_item_increase(), since if there's only one
		-- trapkit in the trapkits stack, then the stack will be
		-- removed from inventory, and thus the ammo's item index
		-- might be altered.
		local item_list = { {kit_item, -1} }

		for i = 1, getn(ammo_objs) do
			tinsert(item_list, {ammo_objs[i].item, -ammo_objs[i].number})
		end

		bulk_item_increase(item_list)
	end
}

-----------------------------------------------------------------
-- Functions that go into the trap definition
-----------------------------------------------------------------

function traps.trapkit_name_func(trap_idx, trap)
	if not trap then
		return "Trapkit"
	end

	local trapkit = trap[FLAG_TRAPKIT_OBJ]
	return strcap(object_desc(trapkit))
end -- traps.trapkit_name_func()

function traps.get_ammo_list(trap)
	local ammo_list = {}

	if trap[FLAG_AMMO_OBJ] then
		local ammo_flag = trap[FLAG_AMMO_OBJ]
		local max_key   = flag_max_key(ammo_flag)

		for i = 1, max_key do
			if ammo_flag[i] then
				tinsert(ammo_list, ammo_flag[i])
			end
		end
	end

	return ammo_list
end

-- Do some verification, and pass it along to the hooks
function traps.trapkit_pre_placement(trap, y, x)
	if not trap[FLAG_TRAPKIT_OBJ] then
		message(color.VIOLET, "Trapkit trap has no tapkit obj!")
		return true
	end

	local trapkit = trap[FLAG_TRAPKIT_OBJ]
	if not trap[FLAG_AMMO_OBJ] and not trapkit.flags[FLAG_NO_AMMO] then
		message(color.VIOLET, "Trapkit trap has no ammo obj!")
		return true
	end

	local ammo_list = traps.get_ammo_list(trap)

	if item_hooks.process_one(trapkit, FLAG_TRAP_PLACE_PRE, trap, ammo_list,
							  y, x) then
		return true
	end
end -- traps.trapkit_pre_placement()

-- Stuff various info into the trap flagset, and pass it on to the hooks
function traps.trapkit_post_placement(trap, y, x)
	local trapkit = trap[FLAG_TRAPKIT_OBJ]
	local ammo    = trap[FLAG_AMMO_OBJ]

	local hiddenness = 0
	if trapkit.flags[FLAG_HIDDENNESS] then
		hiddenness = hiddenness + 10 * trapkit.flags[FLAG_HIDDENNESS]
	end
	if trap[FLAG_HIDDENNESS] then
		hiddenness = hiddenness + trap[FLAG_HIDDENNESS]
	end
	trap[FLAG_HIDDENNESS] = hiddenness

	local difficulty = 0
	if trapkit.flags[FLAG_DIFFICULTY] then
		difficulty = difficulty + trapkit.flags[FLAG_DIFFICULTY]
	end
	if trap[FLAG_DIFFICULTY] then
		difficulty = difficulty + trapkit.flags[FLAG_DIFFICULTY]
	end
	trap[FLAG_DIFFICULTY] = difficulty

	local ammo_list = traps.get_ammo_list(trap)

	item_hooks.process_all(trapkit, FLAG_TRAP_PLACE_POST, trap, ammo_list,
						   y, x)
end -- traps.trapkit_post_placement()

-- Simply pass it on to the hooks
function traps.trapkit_pre_trip(trap, who, y, x, obj)
	if obj then
		message(color.VIOLET, "Trapkit shouldn't be on an object!")
		return true
	end

	local trapkit   = trap[FLAG_TRAPKIT_OBJ]
	local ammo_list = traps.get_ammo_list(trap)

	if item_hooks.process_one(trapkit, FLAG_TRAP_TRIP_PRE, trap, ammo_list,
							  who, y, x) then
		return true
	end
end -- traps.trapkit_pre_trip()

-- Take care of the trapkit and ammo objects.  The hooks might take
-- care of the objects instead of this function.
function traps.trapkit_do_unset(trap, trapkit, who, y, x)
	-- Don't free the object structures, since the objects are going
	-- to be put somewhere, so flag_remove_zap() is used
	flag_remove_zap(trap, FLAG_TRAPKIT_OBJ)

	local ammo_list = {}

	if trap[FLAG_AMMO_OBJ] then
		local ammo_flag = trap[FLAG_AMMO_OBJ]
		local max_key   = flag_max_key(ammo_flag)

		for i = 1, max_key do
			if ammo_flag[i] then
				tinsert(ammo_list, ammo_flag[i])
				flag_remove_zap(ammo_flag, i)
			end
		end
	end

	if item_hooks.process_one(trapkit, FLAG_TRAPKIT_UNSET, trap, ammo_list,
							  who, y, x) then
		-- One of the hooks took care of putting the trapkit
		-- and ammo objects somewhere
		return
	end

	-- Put the trapkit (and any remaining ammo) near where the trap
	-- was set
	drop_near(trapkit, -1, y, x)

	for i = 1, getn(ammo_list) do
		drop_near(ammo_list[i], -1 , y, x)
	end
end -- traps.trapkit_do_unset()

-- What to do to the trapkit after it's been tripped.
traps.TRAPKIT_NOTHING = 1
traps.TRAPKIT_UNSET   = 2
traps.TRAPKIT_REARM   = 3

-- Decide whether or not to rearm.  Also, pass it along to the hooks.
function traps.trapkit_do_rearm(trap, trapkit, ammo, who, y, x)
	if not trapkit.flags[FLAG_REARM_CHANCE] then
		return traps.TRAPKIT_UNSET
	end

	local rearm_chance = trapkit.flags[FLAG_REARM_CHANCE]

	if not rng.percent(rearm_chance) then
		return traps.TRAPKIT_UNSET
	end

	local ammo_list = traps.get_ammo_list(trap)

	if item_hooks.process_one(trapkit, FLAG_REARM_PRE, trap, ammo_list,
							  who, y, x) then
		return traps.TRAPKIT_UNSET
	end

	-- Rearming means... just doing nothing, for the moment.

	item_hooks.process_all(trapkit, flags, FLAG_REARM_POST, ammo_list, who,
						   y, x)

	return traps.TRAPKIT_NOTHING
end -- traps.trapkit_do_rearm()

-- Handle the actual tripping of the trap.  And inform the hooks.
function traps.trapkit_trip(trap, who, y, x, obj)
	if obj then
		message(color.VIOLET, "Trapkit shouldn't be on an object!")
		return false
	end

	local trapkit   = trap[FLAG_TRAPKIT_OBJ]
	local ammo_list = traps.get_ammo_list(trap)

	local target
	if who == 0 then
		target = player
	else
		target = monster(who)
	end

	if who == 0 and trap[FLAG_FACTION] ~= FACTION_PLAYER then
		-- Trecherous traps always trigger for the player
	elseif item_hooks.process_one(trapkit, FLAG_TRAP_CANT_TRIP, trap,
								  ammo_list, who, y, x) then
		-- Trapkit or it's egos says that it CAN'T be tripped by
		-- "who"
		return false
	elseif target.flags[FLAG_PASS_WALL] then
		-- By default, traps trigger for anything that isn't
		-- incorporeal
		return false
	end

	if who == 0 then
		message("You trigger your own trap!")
	else
		-- Can we see at all?
		if not player.has_intrinsic(FLAG_BLIND) then
			-- Can we see the trap location?
			if player_can_see_bold(y, x) then
				if target.ml then
					message(strcap(monster_desc(target, 4)) ..
							" sets off your trap.")
				else
					-- Invisible
					message("Something sets off your trap.")
				end
			end
		end
	end

	-- If the trap is being triggered by a monster, then
	-- engine/traps.lua calls module-defined hooks to see if
	-- the monster notices, and to do the disarming.  These
	-- are defined in traps.monster_avoid_activation() and
	-- traps.disarm_by_monster()

	local func
	func = get_function_registry_from_flag(trapkit.flags,
					       FLAG_TRAPKIT_TRIP)

	-- Let the trapkit deal with fire off the ammo
	local action, exhausted = func(trap, trapkit, ammo_list, who, y, x)

	local num_objs_left = 0

	-- Clean up objects stacks that have been used up.
	if not trapkit.flags[FLAG_NO_AMMO] then
		local objs_flag = trap[FLAG_AMMO_OBJ]
		local max_key   = flag_max_key(objs_flag)

		for i = 1, max_key do
			local obj = objs_flag[i]

			if obj then
				num_objs_left = num_objs_left + obj.number

				if obj.number == 0 then
					-- Flag code takes care of deleting the object
					-- structure
					objs_flag[i] = nil
				end
			end -- if obj then
		end -- for i = 1, max_key do
	end -- if not trapkit.flags[FLAG_NO_AMMO] then

	-- Ammo list might have had empty ammo stacks removed from it, so
	-- regenerate it
	ammo_list = traps.get_ammo_list(trap)

	-- Most trapkits use up objects, so we can check for ammo exhaustion
	-- here, but traps that don't use up objects (like device traps, which
	-- use up charges) can figure it out on there own and return the
	-- exhaustion status from the TRAPKIT_TRIP function.
	if num_objs_left == 0 then
		exhausted = true
	end

	if exhausted then
		item_hooks.process_all(trapkit, FLAG_AMMO_EXHAUSTED, trap,
							   ammo_list, who, y, x)
	end

	-- Don't allow re-arming if the trapkit has exhausted it's ammo supply.
	-- If, for some bizarre reason, a trapkit wants to remain set even
	-- after the ammo has been exhausted, then TRAPKIT_TRIP should return
	-- traps.TRAPKIT_NOTHING as it's desired action.
	if exhausted and action == traps.TRAPKIT_REARM then
		action = traps.TRAPKIT_UNSET
	end

	-- The trapkit wants to re-arm; do we allow it?
	if action == traps.TRAPKIT_REARM then
		action = traps.trapkit_do_rearm(trap, trapkit, ammo_list,
						who, y, x)
	end

	-- Did we rearm?
	if action == traps.TRAPKIT_UNSET then
		-- Do this here, rather than doing it via hooks from
		-- engine/traps.lua, since unsetting the trapkit removes the
		-- trap from the floor's activations, and thus it won't be
		-- there by the item engine/traps.lua calls the hooks.
		traps.trapkit_post_trip(trap, who, y, x)

		traps.inc_recursion_depth()
		traps.start_processing_trap(trap)

		traps.trapkit_do_unset(trap, trapkit, who, y, x)

		-- If the trap is unset via disarming, then this is
		-- already taken care of, but it's being unset because
		-- it hasn't ben rearmed, so we have to do it ourselves.
		traps.remove_location_trap(trap, y, x)

		traps.end_processing_trap(trap)
		traps.dec_recursion_depth()
	end
end -- traps.trapkit_trip()

-- Simply pass it along to the hooks.
function traps.trapkit_post_trip(trap, who, y, x, obj)
	if obj then
		message(color.VIOLET, "Trapkit shouldn't be on an object!")
		return true
	end

	local trapkit = trap[FLAG_TRAPKIT_OBJ]

	local ammo_list = traps.get_ammo_list(trap)

	item_hooks.process_all(trapkit, FLAG_TRAP_TRIP_POST, trap, ammo_list,
						   who, y, x)
end -- traps.trapkit_post_trip()

-- Simply pass it along to the hooks.
function traps.trapkit_disarm_attempt(manual, trap, who, y, x, obj)
	local trapkit = trap[FLAG_TRAPKIT_OBJ]

	local ammo    = trap[FLAG_AMMO_OBJ]

	local ret
	ret = item_hooks.process_one(trapkit, FLAG_TRAP_DISARM_ATTEMPT, trap,
								 ammo, who, y, x)

	if ret then
		return ret
	end
end -- traps.trapkit_disarm_attempt()

-- Simply pass it along to the hooks.
function traps.trapkit_pre_disarm(manual, trap, who, y, x, obj)
	local trapkit = trap[FLAG_TRAPKIT_OBJ]
	local ammo    = trap[FLAG_AMMO_OBJ]

	local ret
	ret = item_hooks.process_one(trapkit, FLAG_TRAP_DISARM_PRE, manual,
								 trap, ammo, who, y, x)

	if ret then
		return ret
	end
end -- traps.trapkit_pre_disarm()

-- It's been disarmed, so unset.  Also, pass it along to the hooks.
function traps.trapkit_post_disarm(manual, trap, who, y, x, obj)
	local trapkit   = trap[FLAG_TRAPKIT_OBJ]
	local ammo_list = traps.get_ammo_list(trap)

	-- It's been disarmed, so unset it
	traps.trapkit_do_unset(trap, trapkit, who, y, x)

	local flags = {FLAG_TRAP_DISARM_POST}
	item_hooks.process_all(trapkit, FLAG_TRAP_DISARM_POST, manual, trap,
						   ammo_list, who, y, x)
end -- traps.trapkit_post_disarm()

-------------------------------------------------------------------
-- The actual trap definition.
-------------------------------------------------------------------
TRAP_TRAPKIT = traps.add {
	name         = "Trapkit",
	needs_target = true,
	never_random = true,
	dont_dump    = true,
	easy_know    = true,
	no_message   = true,
	minlevel     = 1,
	level        = {0, 0},
	proba        = 1,
	power        = 1,
	attr         = color.VIOLET,
	char         = ';',

	name_func      = traps.trapkit_name_func,
	pre_placement  = traps.trapkit_pre_placement,
	post_placement = traps.trapkit_post_placement,
	disarm_attempt = traps.trapkit_disarm_attempt,
	pre_disarm     = traps.trapkit_pre_disarm,
	post_disarm    = traps.trapkit_post_disarm,
	pre_trip       = traps.trapkit_pre_trip,
	trap           = traps.trapkit_trip
} -- TRAP_TRAPKIT

-----------------------------------------------------------------
-- Let monsters detect and disarm player traps
-----------------------------------------------------------------
function traps.monster_avoid_activation(trap, who, y, x, obj)
	local monst      = monster(who)
	local difficulty = 25
	local skill      = 0
	local smartness

	-- Mindless creatures never detect traps
	if monst.flags[FLAG_EMPTY_MIND] then
		return false
	end

	-- Nonsmart animals never detect traps
	if monst.flags[FLAG_ANIMAL] and	not monst.flags[FLAG_SMART] then
		return false
	end

	-- Some traps are more well hidden than others
	if trap[FLAG_HIDDENNESS] then
		difficulty = difficulty + trap[FLAG_HIDDENNESS]
	end

	-- Luck affects trapkits
	difficulty = difficulty + luck(-33, 33)

	-- Hihger level monsters are better at detecting traps
	smartness  = monst.level

	-- Some monsters are better (or worse) at disarming traps
	-- than others
	if monst.flags[FLAG_MONST_DISARM_SKILL] then
		skill = monst.flags[FLAG_MONST_DISARM_SKILL]
	end
	smartness = smartness + skill

	-- Smart monster are better at detecting traps
	if monst.flags[FLAG_SMART] then
		smartness = smartness + 10
	end

	-- Monster has already detected a player-set trap
	if monst.flags[FLAG_TRAP_NOTED] then
		smartness = smartness + 20
	end

	-- Stupid monsters are very bad at detecting traps
	if monst.flags[FLAG_STUPID] then
		smartness = smartness - 150
	end

	-- Does monster even notice it?
	if rng.number(300) <= (difficulty - smartness + 150) then
		return false
	end

	-- Next trap will be easier to notice
	monst.flags[FLAG_TRAP_NOTED] = 1

	return traps.disarm_by_monster(true, trap, who, y, x, obj)
end

function traps.manual_disarm_by_monster(trap, who, y, x, obj)
	local monst      = monster(who)
	local difficulty
	local skill      = 0
	local smartness

	-- Mindless creatures never disarm traps
	if monst.flags[FLAG_EMPTY_MIND] then
		return false
	end

	-- Nonsmart animals never disarm traps
	if monst.flags[FLAG_ANIMAL] and	not monst.flags[FLAG_SMART] then
		return false
	end

	-- Some monsters are better (or worse) at disarming traps
	-- than others
	if monst.flags[FLAG_MONST_DISARM_SKILL] then
		skill = monst.flags[FLAG_MONST_DISARM_SKILL]
	end

	-- Luck affects trapkits
	local difficulty = luck(-12, 12)

	-- Some traps are harder to disarm than others
	if trap[FLAG_DIFFICULTY] then
		difficulty = trap[FLAG_DIFFICULTY]
	end

	-- Higher level monsters are better at disarming
	smartness = monst.level / 5 + skill

	-- If this monster has already disarmed a player trap, then
	-- disarming another one will be easier
	if monst.flags[FLAG_TRAP_DISARMED] then
		smartness = smartness + 20
	end

	-- Smart monsters are great at disarming
	if monst.flags[FLAG_SMART] then
		smartness = smartness * 2
	end

	-- Stupid monsters are very bad at disarming traps
	if monst.flags[FLAG_STUPID] then
		smartness = smartness - 150
	end

	-- Did the disarm succeed?
	if rng.number(120) <= (difficulty - smartness + 80) then
		return false
	end

	-- Attempt to do a manual disarm, which might fail
	local ret = traps.do_disarm(true, trap, who, y, x, obj)

	if ret ~= DISARM_SUCCESS or not trap[FLAG_GONE] then
		return ret
	end

	if not player.has_intrinsic(FLAG_BLIND) and player_can_see_bold(y, x) then
		message(strcap(monster_desc(monst, 4)) .. " disarms your trap.")
	end

	-- The disarming succeeded
	return ret
end -- traps.disarm_by_monster()

-----------------------------------------------------------------
-- Utility functions for item kind and ego definitions
-----------------------------------------------------------------

function traps.ammo_by_tval(...)
	local trapkit, tvals = unpack(arg)

	if type(tvals) == "number" then
		tvals = {tvals}
	end

	local tval_table = {}

	for i = 1, getn(tvals) do
		tval_table[tvals[i]] = true
	end

	local question1 = "Which ammo to use?"
	local question2 = "Which further ammo to use (ESC if done)?"
	local no_ammo   = "No ammo to use."

	local max_ammo     = trapkit.flags[FLAG_AMMO_PER_TRIGGER]
	local rearm_chance = trapkit.flags[FLAG_REARM_CHANCE]

	if rearm_chance and rearm_chance > 0 then
		max_ammo = 99
	end

	local item_list

	item_list = get_item_multi(question1, question2, no_ammo,
							   max_ammo, max_ammo, USE_INVEN,
			function(obj) return %tval_table[obj.tval] end,
		"t")

	return item_list
end -- traps.ammo_by_tval()

--
-- Functions for catapult, arrow and crossbow bolt traps.
--
function traps.test_hit_fire(chance, ac)
	local k

	-- Percentile dice
	k = rng.number(100)

	if wizard then
		message("Trapkit ammo: chance " .. chance .. " versues ac " ..
			ac)
	end

	-- Instant miss or hit
	if (k < 10) then
		if k < 5 then
			return true
		else
			return FALSE
		end
	end

	-- Never hit
	if (chance <= 0) then return (FALSE) end

	-- Power competes against armor
	if (rng.number(chance + luck( -10, 10)) < (ac * 3 / 4)) then
		return false
	end

	-- Assume hit
	return true
end -- traps.test_hit_fire()

-- NOTE: This really should re-use as much of script/archery.lua as possible
function traps.fire_missile_ammo(trap, trapkit, ammo_list, who, y, x)
	local c_ptr = cave(y, x)

	local target
	local target_name
	local verb
	local trap_visible, target_visible

	if who == 0 then
		target      = player
		target_name = "You"
		verb        = "are"

		trap_visible   = not player.has_intrinsic(FLAG_BLIND)
		target_visible = true
	else
		target      = monster(who)
		target_name = strcap(monster_desc(target, 4))
		verb        = "is"

		trap_visible   = player_can_see_bold(y, x) and (not player.has_intrinsic(FLAG_BLIND))
		target_visible = target.ml
	end

	local num_shots      = trapkit.flags[FLAG_AMMO_PER_TRIGGER]
	local ammo_stack_pos = 1
	local ammo_stack     = ammo_list[1]
	local ammo_name      = object_desc(ammo_stack, FALSE, 0)

	for i = 1, num_shots do
		local bonus  = trapkit.to_h + ammo_stack.to_h
		local chance = bonus * combat.archery.BTH_PLUS_ADJ
		local hit    = false

		if trap[FLAG_GONE] then
			-- Last firing of the ammo somehow caused the trap to be
			-- disarmed or otherwise removed.
			return traps.TRAPKIT_NOTHING
		end

		chance = chance + trap[FLAG_TRAP_POWER]

		if traps.test_hit_fire(chance, target.ac) then
			hit = true

			if trap_visible then
				message(target_name .. " " .. verb .. " " ..
					"hit by the " .. ammo_name .. ".")
			end
		else
			if trap_visible and target_visible then
				message("The " .. ammo_name .. " misses.")
			elseif trap_visible and not target_visisble then
				message("Something sets off the trsap.")
			end
		end

		-- Breakage chance
		chance = 10
		if ammo_stack.flags[FLAG_BREAKAGE_CHANCE] then
			chance = ammo_stack.flags[FLAG_BREAKAGE_CHANCE]
		end

		local drop = object_dup(ammo_stack)
		drop.number = 1

		drop_near(drop, chance, y, x)

		ammo_stack.number = ammo_stack.number - 1

		-- If we've exhausted the current ammo stack, try to move onto
		-- the next one.
		if ammo_stack.number == 0 then
			-- No more ammo stacks left, we're done
			if ammo_stack_pos == getn(ammo_list) then
				break
			end

			ammo_stack_pos = ammo_stack_pos + 1
			ammo_stack     = ammo_list[ammo_stack_pos]
			ammo_name      = object_desc(ammo_stack, FALSE, 0)
		end -- if ammo_stack.number == 0 then
	end -- for i = 1, num_shots do

	-- See if the trapkit can rearm itself
	return traps.TRAPKIT_REARM
end -- traps.fire_missile_ammo()
