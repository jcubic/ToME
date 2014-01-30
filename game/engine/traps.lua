-- Functions and variables dealing with traps XXX

declare_global_constants {"traps"}

traps = {}

hook.new_hook_type("TRAP_RANDOM_PRE")
hook.new_hook_type("TRAP_RANDOM_POST")
hook.new_hook_type("TRAP_MAKE_POST")
hook.new_hook_type("TRAP_PLACE_PRE")
hook.new_hook_type("TRAP_PLACE_POST")
hook.new_hook_type("TRAP_DISARM_ATTEMPT")
hook.new_hook_type("TRAP_DISARM_PRE")
hook.new_hook_type("TRAP_DISARM_POST")
hook.new_hook_type("TRAP_TRIP_PRE")
hook.new_hook_type("TRAP_TRIP_POST")

hook.new_hook_type("TRAP_RECURSION_START")
hook.new_hook_type("TRAP_RECURSION_STOP")

new_flag("BEING_PROCESSED")
new_flag("GONE")

new_flag("TRAP_PLACE_PRE")
new_flag("TRAP_PLACE_POST")
new_flag("TRAP_DISARM_ATTEMPT")
new_flag("TRAP_DISARM_PRE")
new_flag("TRAP_DISARM_POST")
new_flag("TRAP_TRIP_PRE")
new_flag("TRAP_TRIP_POST")

-- Player flag signifying perfect disarming
new_flag("PERFECT_DISARMING")

---------------------------------------

--
-- Traps can be removed by hooks (or the trap itself) at weird times
-- during trap processing, and we don't want to continue processing if
-- the trap is gone, and it's associated memory freed, so we need a way
-- to determine if the current trap has been removed by the hook.  We
-- could prohibit hooks from doing this, but that might prohibit some
-- cool things from being done.  We can't rely on the hook or trap to tell us
-- if the trap has been removed, since, for example, a trap/hook could
-- invoke an arbitrary spell which removed the trap, and the trap/hook
-- can't tell the difference between spells which will do this and spells
-- which won't.  Additionally, the processing of one trap might cause
-- a *second* trap to be processed, which would then remove the first
-- trap, and the first trap would have no way of realizing this.
--
-- SO, we use this rather hackish means of tracking recursion depth
-- to defer the freeing of the trap data structure if trap code is
-- being processed.  This lets us store the knowledge that the trap
-- has been removed into the trap data structure itself.
--

traps.recursion_depth = 0
traps.defered_deletes = {}

function traps.inc_recursion_depth()
	if traps.recursion_depth == 0 then
		hook.process(hook.TRAP_RECURSION_START)
	end

	traps.recursion_depth = traps.recursion_depth + 1
end -- traps.inc_recursion_depth()

function traps.dec_recursion_depth()
	assert(traps.recursion_depth >= 1,
		   "Trap recursion depth has gone non-positive!")

	traps.recursion_depth = traps.recursion_depth - 1

	if traps.recursion_depth == 0 then
		hook.process(hook.TRAP_RECURSION_STOP)

		for i = 1, getn(traps.defered_deletes) do
			flag_free(traps.defered_deletes[i], true)
		end

		traps.defered_deletes = {}
	end
end -- traps.dec_recursion_depth()

function traps.start_processing_trap(trap)
	trap[FLAG_BEING_PROCESSED] = trap[FLAG_BEING_PROCESSED] + 1
end

function traps.end_processing_trap(trap)
	trap[FLAG_BEING_PROCESSED] = trap[FLAG_BEING_PROCESSED] - 1
end

--------------------------------------------

-- Locations a trap can be placed
traps.LOCATION_DOOR   = 1
traps.LOCATION_FLOOR  = 2
traps.LOCATION_OBJECT = 4

traps.locations = {traps.LOCATION_DOOR, traps.LOCATION_FLOOR,
			traps.LOCATION_OBJECT}
traps.all_locs_mask = 0

traps.trap_list_by_location       = {}
traps.trap_list_by_location.all   = {}
traps.trap_list_by_location.monst = {}

for i = 1, getn(traps.locations) do
	traps.trap_list_by_location.all[traps.locations[i]]  = {}
	traps.trap_list_by_location.monst[traps.locations[i]] = {}

	traps.all_locs_mask = traps.all_locs_mask | traps.locations[i]
end

traps.random_trap_data       = {}
traps.random_trap_data.all   = {}
traps.random_trap_data.monst = {}

traps.trap_list     = {}
traps.max_trap_idx  = 0
traps.traps_by_name = {}

---------------------------------------------------------------------------

function traps.add(t)
	assert(t.name, "No trap name!")
	assert(t.level, "No trap level for trap " .. t.name)
	assert(t.attr, "No trap attr for trap " .. t.name)
	assert(t.minlevel, "No trap minlevel for trap " .. t.name)
	assert(t.level, "No trap level for trap " .. t.name)
	assert(t.proba, "No trap proba for trap " .. t.name)
	assert(t.power, "No trap power for trap " .. t.name)
	assert(getn(t.level) == 2,
	       "Need exactly two items in trap.level for trap " .. t.name)

	t.locations = t.locations or traps.all_locs_mask

	t.char = t.char or '^'
	t.char = strbyte(t.char)

	traps.max_trap_idx = traps.max_trap_idx + 1

	t.index = traps.max_trap_idx

	traps.trap_list[traps.max_trap_idx] = t

	if not t.never_random then
		for j = 1, getn(traps.locations) do
			local loc = traps.locations[j]

			if t.locations & loc ~= 0 then
				tinsert(traps.trap_list_by_location.all[loc], t)

				if not t.player_only then
					tinsert(traps.trap_list_by_location.monst[loc], t)
				end
			end
		end
	end -- if not t.never_random then

	traps.traps_by_name[strlower(t.name)] = t.index

	return traps.max_trap_idx
end -- traps.add()

---------------------------------------------------------------------------

-- Detect floor/door traps within a certain radius
function traps.detect(radius)
	if not radius then
		radius = DEFAULT_RADIUS
	end

	local detected = false
	local new      = false

	for y = player.py - radius, player.py + radius do
		for x = player.px - radius, player.px + radius do
			if in_bounds(y, x) and
				(distance(player.py, player.px,
					  y, x) <= radius) then
				local c_ptr = cave(y, x)

				c_ptr.info = c_ptr.info | CAVE_DETECT

				local all_traps   = get_num_location_traps(y, x)
				local known_traps = get_num_location_traps(y, x, true)

				if all_traps > 0 then
					detected   = true
					c_ptr.info = c_ptr.info | CAVE_TRDT
					c_ptr.info = c_ptr.info | CAVE_MARK
				end

				if all_traps > known_traps then
					new = true
				end

				flag_learn_all(c_ptr.activations)
			end -- within radius
		end -- x loop
	end -- y loop

	if new then
		player.redraw[FLAG_PR_MAP] = 1
		message("You sense the prescence of new traps!")
	elseif detected then
		message("You re-sense some old traps.")
	end

	return detected
end -- traps.detect()

-----------------------------------------------------------------------
-- Trap activation
-----------------------------------------------------------------------

function traps.is_activatable_trap(trap, who, friendly_traps)
	if not trap then
		return false
	end

	if tag(trap) ~= TAG_FLAG then
		return false
	end

	if not trap[FLAG_TRAP_IDX] then
		return false
	end

	if trap[FLAG_GONE] then
		return false
	end

	if who and who ~= -1 then
		local faction = who_to_faction(who)
		local friendliness = factions.get_friendliness(trap[FLAG_FACTION],
													  faction)

		if (friendly_traps and friendliness < 0) or
			(not friendly_traps and friendliness >= 0) then
			return false
		end
	end

	return true
end -- traps.is_activatable_trap()

function traps.activate_location_traps(who, y, x, remote, friendly_traps)
	local triggered = false
	local obvious   = false
	local c_ptr     = cave(y, x)
	local trap_list = c_ptr.activations
	local max_key   = flag_max_key(trap_list)
	local can_see

	if remote then
		can_see = (player.py == y and player.px == x) or
			(not player.has_intrinsic(FLAG_BLIND) and player_can_see_bold(y, x))
	else
		can_see = (who == 0) or
			(not player.has_intrinsic(FLAG_BLIND) and player_can_see_bold(y, x))
	end

	traps.inc_recursion_depth()
	for i = 1, max_key do
		local trap = trap_list[i]

		if traps.is_activatable_trap(trap, who, friendly_traps) then
			local tmp_trig, tmp_obv

			if not flag_is_known(trap_list, i) and who == 0 then
				message("You found a trap!")
				flag_learn(trap_list, i, true)
				c_ptr.info = c_ptr.info | CAVE_TRDT
				player.redraw[FLAG_PR_MAP] = 1
			end

			traps.start_processing_trap(trap)
			tmp_trig, tmp_obv = traps.activate_trap(trap, who, y, x,
													remote)
			traps.end_processing_trap(trap)

			if not flag_is_known(trap_list, i) and who ~=0 and tmp_trig and
				tmp_obv and can_see then
				message("You noticed a trap!")
				flag_learn(trap_list, i, true)
				c_ptr.info = c_ptr.info | CAVE_TRDT
				player.redraw[FLAG_PR_MAP] = 1
			end

			triggered = triggered or tmp_trig
			obvious   = obvious   or tmp_obv
		end -- if traps.is_activatable_trap(trap, who)

		-- Monster killed by trap, stop
		if who > 0 and monster(who).r_idx == 0 then
			break
		end
	end -- for i = 1, max_key do
	traps.dec_recursion_depth()

	return triggered, obvious
end -- traps.activate_location_traps()

function traps.activate_object_traps(who, obj, remote, friendly_traps)
	local y, x = obj.iy, obj.ix
	assert(y ~= 0 and x ~= 0,
	       "Activating object traps not on the ground not implemented")

	local trap_list = flag_get_flags(obj.flags, FLAG_TRAP)

	if not trap_list then
		if wizard then
			message(color.VIOLET,
					"ERROR: No traps found on object  " .. object_desc(obj))
		end
		return false
	end

	local can_see

	if remote then
		can_see = (player.py == y and player.px == x) or
			(not player.has_intrinsic(FLAG_BLIND) and player_can_see_bold(y, x))
	else
		can_see = (who == 0) or
			(not player.has_intrinsic(FLAG_BLIND) and player_can_see_bold(y, x))
	end

	local max_key   = flag_max_key(trap_list)
	local triggered = false
	local obvious   = false

	traps.inc_recursion_depth()
	for i = 1, max_key do
		local trap = trap_list[i]

		if traps.is_activatable_trap(trap, who, friendly_traps) then
			local tmp_trig, tmp_obv

			if not flag_is_known(trap_list, i) and who == 0 then
				message("You found a trap on the " .. object_desc(obj) ..
						"!")
				flag_learn(trap_list, i, true)
				obj.flags[FLAG_TRAP_AWARE] = true
			end

			traps.start_processing_trap(trap)
			tmp_trig, tmp_obv = traps.activate_trap(trap, who, y, x,
													remote, obj)
			traps.end_processing_trap(trap)

			if not flag_is_known(trap_list, i) and who ~=0 and
				tmp_trig and tmp_obv and can_see then
				message("You noticed a trap on the " .. object_desc(obj) ..
						"!")
				flag_learn(trap_list, i, true)
				obj.flags[FLAG_TRAP_AWARE] = true
			end

			triggered = triggered or tmp_trig
			obvious   = obvious   or tmp_obv
		end -- if traps.is_activatable_trap(trap, who)
	end -- for i = 1, max_key do
	traps.dec_recursion_depth()

	return triggered, obvious
end -- traps.activate_object_traps()

-- This should be overridden by the module if it wishes to allow
-- monsters a chance to avoid activating traps which are hostile
-- to them.
function traps.monster_avoid_activation(trap, who, y, x, obj)
	return false
end

function traps.find_trap_target(trap_info, y, x, obj)
	if obj then
		y = obj.iy
		x = obj.ix

		if y == 0 and x == 0 then
			-- Can only (as of now) activate trapped objects which are
			-- on the ground
			return nil, nil
		end

		-- Is a target standing on or next to the object?
		for yy = y - 1, y + 1 do
			for xx = x - 1 , x + 1 do
				if in_bounds(yy, xx) then
					if player.py == yy and player.px == xx then
						return true, 0
					elseif cave(yy, xx).m_idx > 0 and
						not trap_info.player_only then
						return true, cave(yy, xx).m_idx
					end
				end
			end
		end
	else
		if (player.py == y and player.px == x) then
			return true, 0
		elseif cave(y, x).m_idx > 0 and not trap_info.player_only then
			return true,  cave(y, x).m_idx
		end
	end
end -- traps.find_trap_target()

function traps.get_trap_msg(trap, who, y, x, obj)
	local trap_idx  = trap[FLAG_TRAP_IDX]
	local trap_info = traps.trap_list[trap_idx]
	local msg       = ""

	if trap_info.no_message then
		-- No message
	elseif who == 0 and trap_info.message_you then
		-- Message specific to trap being tripped by player.
		msg = trap_info.message_you
	elseif player_has_los_bold(y, x) and
		(player.has_intrinsic(FLAG_BLIND) or not player_can_see_bold(y, x)) then
		-- Player has LOS on trap, but can't see the trap
		msg = trap_info.message_no_see
	elseif not player_has_los_bold(y,x) then
		-- Player doesn't have LOS on trap
		msg = trap_info.message_no_los
	elseif trap_info.message then
		-- Player can see the trap
		msg = trap_info.message
	elseif traps.is_ident(trap_idx) then
		-- Player can see the trap
		msg = "@Target@ trigger@s@ a @trap@."
	else
		-- Player can see the trap
		msg = "@Target@ trigger@s@ an unknown trap."
	end

	if not msg or strlen(msg) == 0 then
		return ""
	end

	msg = gsub(msg, "(@Trap@)", strcap(trap_info.name))
	msg = gsub(msg, "(@trap@)", trap_info.name)

	if strfind(msg, "@") and who < 0 then
		-- Trap message needs target info to fill it out, but there
		-- is no target for the trap.
		return ""
	end

	local target = "???"
	local isare = "??"
	local sss = "???"
	local target_possessive = "???"

	if who > 0 then
		target = monster_desc(monster(who), 4)
		target_possessive = monster_desc(monster(who), 6)
		isare = "is"
		sss = "s"
	else
		target = "you"
		target_possessive = "your"
		isare = "are"
		sss = ""
	end

	-- Make the message
	msg = gsub(msg, "(@Target@)", strcap(target))
	msg = gsub(msg, "(@target@)", target)
	msg = gsub(msg, "(@Target_possessive@)", strcap(target_possessive))
	msg = gsub(msg, "(@target_possessive@)", target_possessive)
	msg = gsub(msg, "(@isare@)", isare)
	msg = gsub(msg, "(@s@)", sss)

	msg = strcap(msg)

	return msg
end -- traps.get_trap_msg()

function traps.activate_trap(trap, who, y, x, remote, obj)
	local faction   = trap[FLAG_FACTION]
	local trap_idx  = trap[FLAG_TRAP_IDX]
	local trap_info = traps.trap_list[trap_idx]
	local feat      = f_info[cave(y, x).feat + 1]
	local func

	-- Don't activate trap which is already being processed in some
	-- other way
	if trap[FLAG_BEING_PROCESSED] > 1 then
		return false, false
	end

	if remote or who == -1 then
		local has_target, new_who = traps.find_trap_target(trap_info,
														   y, x, obj)

		if has_target then
			who = new_who
		end

		if trap_info.player_only and not (has_target and who == 0) then
			return false, false
		end

		if trap_info.needs_target and not has_target then
			return false, false
		end
	end -- if remote then

	if who ~= 0 and trap_info.player_only then
		return false, false
	end

	-- Give the monster a chance to avoid activating a trap
	-- (this is to be determined by the module)
	if who > 0 and
		traps.monster_avoid_activation(trap, who, y, x, obj) then
		return false, false
	end

	if trap[FLAG_GONE] then return false end -- Trap is gone, stop

	-- Should the trap even be set off?
	if trap_info.pre_trip and
		trap_info.pre_trip(trap, who, y, x, obj) then
		return false, false
	end

	if trap[FLAG_GONE] then return false end -- Trap is gone, stop

	if hook.process(hook.TRAP_TRIP_PRE, trap, who, y, x, obj) then
		return false, false
	end

	if trap[FLAG_GONE] then return false end -- Trap is gone, stop

	-- Does the object or terrain object to being triggered?
	if obj and obj.flags[FLAG_TRAP_TRIP_PRE] then
		func = get_function_registry_from_flag(obj.flags,
						      FLAG_TRAP_TRIP_PRE)
	elseif not obj and
		f_info[cave(y, x).feat + 1].flags[FLAG_TRAP_TRIP_PRE] then
		local flags = f_info[cave(y, x).feat + 1].flags

		func = get_function_registry_from_flag(flags,
						       FLAG_TRAP_TRIP_PRE)
	end

	if func and func(trap, who, y, x, obj) then
		return false, false
	end

	if trap[FLAG_GONE] then return false end -- Trap is gone, stop

	local msg = traps.get_trap_msg(trap, who, y, x, obj)
	if strlen(msg) > 0 then
		message(msg)
	end

	-- And finaly invoke the trap effect
	local obvious = trap_info.trap(trap, who, y, x, obj)

	if obvious then
		if player.has_intrinsic(FLAG_HALLUCINATE) then
			-- Trap is too distorted to recognize.
			obvious = false
		elseif y == player.py and x == player.px then
			-- We're fine... but should we be able to recognize the
			-- trap if we're blind?  Do we feel/sense it because we're
			-- standing right on top of it?
		elseif not player.has_intrinsic(FLAG_BLIND) and
			player_can_see_bold(y, x)
		then
			-- We're fine
		else
			-- Can't see it and not standing on top of it, so the result
			-- isn't actually obvious.
			obvious = false
		end
	end

	if obvious then
		traps.ident(trap_idx)
	end

	if strlen(msg) == 0 and not trap_info.no_message then
		if obvious then
			msg = "The " .. traps.ident_name(trap_idx) .. " was activated."
			message(msg)
		end
	end

	if trap[FLAG_GONE] then return true, obvious end -- Trap is gone, stop

	-- If the trap was hiding it's true faction, we've become
	-- aware of it.
	if trap[FLAG_APPARENT_FACTION] then
	    if who == 0 or (not player.has_intrinsic(FLAG_BLIND) and
						player_can_see_bold(y, x)) then
		    trap[FLAG_APPARENT_FACTION] = nil
		end
	end

	if trap_info.post_trip then
		trap_info.post_trip(trap, who, y, x, ret, obj)
	end

	if trap[FLAG_GONE] then return true, obvious end -- Trap is gone, stop

	hook.process(hook.TRAP_TRIP_POST, trap, who, y, x, obj)

	if trap[FLAG_GONE] then return true, obvious end -- Trap is gone, stop

	if obj and obj.flags[FLAG_TRAP_TRIP_POST] then
		func = get_function_registry_from_flag(obj.flags,
						      FLAG_TRAP_TRIP_POST)
	elseif not obj and feat.flags[FLAG_TRAP_TRIP_POST] then
		local flags = feat.flags

		func = get_function_registry_from_flag(flags,
						       FLAG_TRAP_TRIP_POST)
	end

	if func then
		func(trap, who, y, x, obj)
	end

	return true, obvious
end -- traps.activate_trap()

----------------------------------------------------------------------------
-- Trap disarming
----------------------------------------------------------------------------

-- Functions return DISARM_SUCCESS, DISARM_FAILURE, or DISARM_TRIGGERED.
-- If DISARM_TRIGGERED is returned, it is up to the caller to cause
-- the trap to be triggered

-- NOTE: The hooks/call-backs which can stop the process of disarming
-- do so, not by returing "true" like usual, but by returning the
-- DISARM_FOO value they wish the disarm function to return; they should
-- return "false" or "nil" to not distrupt anything.  They can
-- even cause DISARM_SUCCESS to be returned, to fool the caller into
-- believing that the trap was actually disarmed, though doing so
-- might lead to bizarre and unexpected behavior.
function traps.get_trap_to_disarm(manual, who, trap_list)
	local faction = who_to_faction(who)
	local max_key = flag_max_key(trap_list)
	local trap
	local friendly_trap

	for i = 1, max_key do
		local tmp = trap_list[i]

		if tmp and tmp[FLAG_TRAP_IDX] and not tmp[FLAG_GONE] then
			if who ~= 0 or not manual or flag_is_known(trap_list, i) then
				if factions.get_friendliness(tmp[FLAG_FACTION],
											faction) < 0 then
					trap = tmp
					break
				else
					friendly_trap = tmp
				end
			end
		end
	end -- for i = 1, max_key do

	if not trap and friendly_trap then
		trap = friendly_trap
	end

	return trap
end -- traps.get_trap_to_disarm()

function traps.disarm_location_traps(manual, who, y, x, amount)
	local trap_list = cave(y, x).activations
	local ret       = DISARM_SUCCESS
	local ret_arr   = {}

	if amount < 1 then
		amount = get_num_traps(trap_list)
	end

	traps.inc_recursion_depth()
	for i = 1, amount do
		local trap = traps.get_trap_to_disarm(manual, who, trap_list)

		if not trap then
			break
		end

		traps.start_processing_trap(trap)
		local tmp = traps.disarm(manual, trap, who, y, x)

		traps.end_processing_trap(trap)

		if tmp == DISARM_FAILURE then
			if tmp ~= DISARM_TRIGGERED then
				ret = DISARM_FAILURE
			end
		elseif tmp == DISARM_TRIGGERED then
			ret = DISARM_TRIGGERED
			if manual then
				break
			end
		end
		tinsert(ret_arr, tmp)
	end -- for i = 1, amount do
	traps.dec_recursion_depth()

	return ret, ret_arr
end -- traps.disarm_location_traps()

function traps.disarm_one_location_trap(manual, who, y, x)
	return traps.disarm_location_traps(manual, who, y, x, 1)
end

function traps.disarm_all_location_traps(manual, who, y, x)
	return traps.disarm_location_traps(manual, who, y, x, -1)
end

function traps.disarm_object_traps(manual, who, obj, amount)
	local trap_list = flag_get_flags(obj.flags, FLAG_TRAP)
	local ret       = DISARM_SUCCESS
	local ret_arr   = {}

	if not trap_list then
		return ret
	end

	if amount < 1 then
		amount = get_num_traps(trap_list)
	end

	local y, x = obj.iy, obj.ix

	traps.inc_recursion_depth()
	for i = 1, amount do
		local trap = traps.get_trap_to_disarm(manual, who, trap_list)

		if not trap then
			break
		end

		traps.start_processing_trap(trap)
		local tmp = traps.disarm(manual, trap, who, y, x, obj)
		traps.end_processing_trap(trap)

		if tmp == DISARM_FAILURE then
			if tmp ~= DISARM_TRIGGERED then
				ret = DISARM_FAILURE
			end
		elseif tmp == DISARM_TRIGGERED then
			ret = DISARM_TRIGGERED
			if manual then
				break
			end
		end
		tinsert(ret_arr, tmp)
	end -- for i = 1, amount do
	traps.dec_recursion_depth()

	return ret
end -- traps.disarm_object_traps()

function traps.disarm_one_object_trap(manual, who, obj)
	return traps.disarm_object_traps(manual, who, obj, 1)
end

function traps.disarm_all_object_traps(manual, who, obj)
	return traps.disarm_object_traps(manual, who, obj, -1)
end

function traps.disarm(manual, trap, who, y, x, obj)
	local trap_idx  = trap[FLAG_TRAP_IDX]
	local trap_info = traps.trap_list[trap_idx]
	local feat      = f_info[cave(y, x).feat + 1]
	local func      = nil
	local ret

	-- First, call call-backs/hooks that might prevent even the
	-- *attempt* of disarming
	if trap_info.disarm_attempt then
		ret = trap_info.disarm_attempt(manual, trap, who, y, x, obj)
		if ret then
			return ret
		end
	end

	-- If trap is gone, then disarming was a success
	if trap[FLAG_GONE] then return DISARM_SUCCESS end

	ret = hook.process(hook.TRAP_DISARM_ATTEMPT, manual, trap, who,
			   y, x, obj)
	if ret then
		return ret
	end

	-- If trap is gone, then disarming was a success
	if trap[FLAG_GONE] then return DISARM_SUCCESS end

	if obj and obj.flags[FLAG_TRAP_DISARM_ATTEMPT] then
		-- Does the object object to being disarmed?
		func =get_function_registry_from_flag(obj.flags,
						      FLAG_TRAP_DISARM_ATTEMPT)
	elseif not obj and feat.flags[FLAG_TRAP_DISARM_ATTEMPT] then
		-- Does the terrain object to being disarmed?
		local flags = feat.flags

		func =get_function_registry_from_flag(flags,
						      FLAG_TRAP_DISARM_ATTEMPT)
	end

	if func then
		ret = func(manual, trap, who, y, x, obj)

		if ret then
			return ret
		end
	end

	-- If trap is gone, then disarming was a success
	if trap[FLAG_GONE] then return DISARM_SUCCESS end

	if who > 0 then
		return traps.disarm_by_monster(manual, trap, who, y, x, obj)
	else
		return traps.disarm_by_player(manual, trap, y, x, obj)
	end
end -- traps.disarm()

function traps.disarm_by_player(manual, trap, y, x, obj)
	if manual then
		return traps.manual_disarm_by_player(trap, y, x, obj)
	else
		return traps.magic_disarm_by_player(trap, y, x, obj)
	end
end -- traps.disarm_by_player()

function traps.manual_disarm_by_player(trap, y, x, obj)
	local trap_idx  = trap[FLAG_TRAP_IDX]
	local trap_info = traps.trap_list[trap_idx]
	local skill     = player.skill_dis
	local power     = trap[FLAG_TRAP_POWER]
	local faction   = trap[FLAG_FACTION]
	local c_ptr     = cave(y, x)
	local exta_energy
	local chance
	local func

	-- Make sure we're aware of the trap
	if obj and not obj.flags[FLAG_TRAP_AWARE] then
		if wizard then
			message(color.VIOLET,
				"Can't disarm object trap you're unaware of.")
		end
		return DISARM_FAILURE
	end

	-- Take a turn
	local extra_energy = get_player_energy(SPEED_DISARM)

	if faction == FACTION_PLAYER then
		-- Disarming traps you set yourself is very quick
		extra_energy = extra_energy / 4
	elseif factions.get_friendliness(faction, FACTION_PLAYER) >= 0 then
		-- Disarming friendly/neutral traps is quick, but not as quick
		-- as your own traps
		extra_energy = extra_energy / 2
	end

	energy_use = energy_use + extra_energy

	-- Penalize and reward some conditions.
	if player.has_intrinsic(FLAG_BLIND) or no_lite() then
		skill = skill / 10
	end
	if player.has_intrinsic(FLAG_PERFECT_DISARMING) then
		skill = power + 101
	end
	if player.has_intrinsic(FLAG_CONFUSED) or player.has_intrinsic(FLAG_HALLUCINATE) then
		skill = skill / 10
	end

	chance = skill - power

	-- Always a small chance of success
	if chance < 2 then chance = 2 end

	-- Always successfully disarm friendly and neutral traps
	if factions.get_friendliness(faction, FACTION_PLAYER) >= 0 then
		chance = 101
	end

	if rng.percent(chance) then
		local ret = traps.do_disarm(true, trap, 0, y, x, obj)

		if ret ~= DISARM_SUCCESS or not trap[FLAG_GONE] then
			return ret
		end

		-- Give the player a chance of identifying the trap from
		-- the manual disarm. The player must be able to see the
		-- trap clearly to identify it.
		if rng.percent(chance) and not player.has_intrinsic(FLAG_BLIND) and
			not player.has_intrinsic(FLAG_CONFUSED) and not no_lite() then
			traps.ident(trap_idx)
		end

		message("You disarm the " .. traps.ident_name(trap_idx) .. ".")
		gain_exp(power)

		return DISARM_SUCCESS
	elseif chance > 5 and rng.number(chance) > 5 then
		-- Disarm failed, but the trap wasn't tripped
		if flush_failure then
			flush()
		end

		message("You failed to disarm the " ..
				   traps.ident_name(trap_idx) .. ".")

		return DISARM_FAILURE
	else
		-- Disarm failed, *and* the trap was tripped
		message("In attempting to disarm the trap, you " ..
				"trigger it instead!")
		return DISARM_TRIGGERED
	end
end -- traps.manual_disarm_by_player()

function traps.magic_disarm_by_player(trap, y, x, obj)
	return traps.do_disarm(false, trap, 0, y, x, obj)
end -- traps.magic_disarm_by_player()

function traps.disarm_by_monster(manual, trap, who, y, x, obj)
	if manual then
		return traps.manual_disarm_by_monster(trap, who, y, x, obj)
	else
		return traps.magic_disarm_by_monster(trap, who, y, x, obj)
	end
end -- traps.disarm_by_monster()

function traps.manual_disarm_by_monster(trap, who, y, x, obj)
	error("Must be implemented by module")
end

function traps.magic_disarm_by_monster(trap, who, y, x, obj)
	return traps.do_disarm(false, trap, who, y, x, obj)
end

-- Actually remove the trap flagset from the location or object
function traps.do_disarm(manual, trap, who, y, x, obj)
	if obj then
		return traps.do_disarm_object(manual, trap, who, y, x, obj)
	else
		return traps.do_disarm_location(manual, trap, who, y, x)
	end
end -- traps.do_disarm()

function traps.remove_object_trap(trap, obj)
	local trap_list       = obj.flags[FLAG_TRAP]
	local trap_idx        = nil

	-- This recursion depth and trap_is_gone() stuff is rather hackish,
	-- so do lots of checking to make sure it's being used properly.
	assert(traps.recursion_depth > 0,
		   "traps.inc_recursion_depth() must be used before removing trap.")

	assert(trap[FLAG_BEING_PROCESSED] > 0,
		   "traps.start_processing_trap() must be used on trap before " ..
			   "removing it.")

	if trap[FLAG_GONE] then
		error("Removing trap that's already gone!!")
	end

	for i = 1, trap_list.size do
		local flags = flag_get_flags(trap_list, trap_list.node[i].key)

		if flags == trap then
			trap_idx = i
		end
	end

	assert(trap_idx, "No matching trap found")

	-- Defer freeing the trap until trap recursion is through
	flag_remove_zap(trap_list, trap_list.node[trap_idx].key)
	tinsert(traps.defered_deletes, trap)

	trap[FLAG_GONE] = true

	-- Set FLAG_TRAP_AWARE, so we can tell the difference between
	-- never-trapped objects, and trapped objects which have been
	-- disarmed
	obj.flags[FLAG_TRAP_AWARE] = 1
end -- traps.remove_object_trap()

function traps.do_disarm_object(manual, trap, who, y, x, obj)
	local trap_idx  = trap[FLAG_TRAP_IDX]
	local trap_info = traps.trap_list[trap_idx]
	local func
	local ret

	if trap_info.pre_disarm then
		ret = trap_info.pre_disarm(manual, trap, who, y, x, obj)

		if ret then
			return ret
		end
	end

	-- If trap is gone, then disarming was a success
	if trap[FLAG_GONE] then return DISARM_SUCCESS end

	ret = hook.process(hook.TRAP_DISARM_PRE, manual, trap, who,
			   y, x, obj)
	if ret then
		return ret
	end

	-- If trap is gone, then disarming was a success
	if trap[FLAG_GONE] then return DISARM_SUCCESS end

	if obj.flags[FLAG_TRAP_DISARM_PRE] then
		func = get_function_registry_from_flag(obj.flags,
						       FLAG_TRAP_DISARM_PRE)

		ret = func(manual, trap, who, y, x, obj)

		if ret then
			return ret
		end
	end

	-- If trap is gone, then disarming was a success
	if trap[FLAG_GONE] then return DISARM_SUCCESS end

	-- Call post-disarm functions while flag still exists
	if trap_info.post_disarm then
		trap_info.post_disarm(manual, trap, who, y, x, obj)
	end

	-- If trap is gone, then disarming was a success
	if trap[FLAG_GONE] then return DISARM_SUCCESS end

	hook.process(hook.TRAP_DISARM_POST, manual, trap, who, y, x, obj)

	-- If trap is gone, then disarming was a success
	if trap[FLAG_GONE] then return DISARM_SUCCESS end

	if obj.flags[FLAG_TRAP_DISARM_POST] then
		func = get_function_registry_from_flag(obj.flags,
						       FLAG_TRAP_DISARM_POST)

		func(manual, trap, who, y, x, obj)
	end

	traps.remove_object_trap(trap, obj)

	return DISARM_SUCCESS
end -- traps.do_disarm_object()

function traps.remove_location_trap(trap, y, x)
	local c_ptr           = cave(y, x)
	local trap_list       = c_ptr.activations
	local trap_idx        = nil

	-- This recursion depth and trap_is_gone() stuff is rather hackish,
	-- so do lots of checking to make sure it's being used properly.
	assert(traps.recursion_depth > 0,
		   "traps.inc_recursion_depth() must be used before removing trap.")

	assert(trap[FLAG_BEING_PROCESSED] > 0,
		   "traps.start_processing_trap() must be used on trap before " ..
			   "removing it.")

	if trap[FLAG_GONE] then
		error("Removing trap that's already gone!!")
	end

	for i = 1, trap_list.size do
		local flags = flag_get_flags(trap_list, trap_list.node[i].key)

		if flags == trap then
			trap_idx = i
		end
	end

	assert(trap_idx, "No matching trap found")

	-- Defer freeing the trap until trap recursion is through
	flag_remove_zap(trap_list, trap_list.node[trap_idx].key)
	tinsert(traps.defered_deletes, trap)

	trap[FLAG_GONE] = true

	if get_num_traps(trap_list) == 0 then
		c_ptr.info = c_ptr.info & negate_bits(CAVE_TRDT)

		player.redraw[FLAG_PR_MAP] = 1
	end -- no traps left at location
end -- traps.remove_location_trap()

function traps.do_disarm_location(manual, trap, who, y, x)
	local trap_idx  = trap[FLAG_TRAP_IDX]
	local trap_info = traps.trap_list[trap_idx]
	local c_ptr     = cave(y, x)
	local feat      = f_info[c_ptr.feat + 1]
	local func
	local ret

	if trap_info.pre_disarm then
		ret = trap_info.pre_disarm(manual, trap, who, y, x)

		if ret then
			return ret
		end
	end

	-- If trap is gone, then disarming was a success
	if trap[FLAG_GONE] then return DISARM_SUCCESS end

	ret = hook.process(hook.TRAP_DISARM_PRE, manual, trap, who,
			   y, x)
	if ret then
		return ret
	end

	-- If trap is gone, then disarming was a success
	if trap[FLAG_GONE] then return DISARM_SUCCESS end

	if feat.flags[FLAG_TRAP_DISARM_PRE] then
		local flags = feat.flags

		func = get_function_registry_from_flag(flags,
						       FLAG_TRAP_DISARM_PRE)

		ret = func(manual, trap, y, x)

		if ret then
			return ret
		end
	end

	-- If trap is gone, then disarming was a success
	if trap[FLAG_GONE] then return DISARM_SUCCESS end

	-- Call post-disarm functions while flag still exists
	if trap_info.post_disarm then
		trap_info.post_disarm(manual, trap, who, y, x)
	end

	-- If trap is gone, then disarming was a success
	if trap[FLAG_GONE] then return DISARM_SUCCESS end

	hook.process(hook.TRAP_DISARM_POST, manual, trap, who, y, x)

	-- If trap is gone, then disarming was a success
	if trap[FLAG_GONE] then return DISARM_SUCCESS end

	if feat.flags[FLAG_TRAP_DISARM_POST] then
		local flags = feat.flags

		func = get_function_registry_from_flag(flags,
						       FLAG_TRAP_DISARM_POST)

		func(manual, trap, y, x)
	end

	-- If trap is gone, then disarming was a success
	if trap[FLAG_GONE] then return DISARM_SUCCESS end

	traps.remove_location_trap(trap, y, x)

	return DISARM_SUCCESS
end -- traps.do_disarm_location()

------------------------------------------------------------------------
-- Trap information
------------------------------------------------------------------------

function traps.has_trap(trap_list)
	if not trap_list then
		return false
	end

	local max_key = flag_max_key(trap_list)

	for i = 1, max_key do
		local trap = trap_list[i]

		if tag(trap) == TAG_FLAG and trap[FLAG_TRAP_IDX] and not trap[FLAG_GONE] then
			return true
		end
	end
	return false
end

function traps.location_has_trap(y, x)
	local c_ptr = cave(y, x)
	return traps.has_trap(c_ptr.activations)
end

function traps.object_has_trap(obj)
	local trap_list = obj.flags[FLAG_TRAP]
	return traps.has_trap(trap_list)
end

function traps.find_one_trap(trap_list)
	if not trap_list then
		return false
	end

	local max_key = flag_max_key(trap_list)

	for i = 1, max_key do
		local trap = trap_list[i]

		if tag(trap) == TAG_FLAG and trap[FLAG_TRAP_IDX] and
			not trap[FLAG_GONE] and not flag_is_known(trap_list, i) then
			flag_learn(trap_list, i, true)
			return true
		end
	end

	return false
end

function traps.find_one_location_trap(y, x)
	local c_ptr = cave(y, x)
	local found = traps.find_one_trap(c_ptr.activations)

	if found then
		message("You found a trap!")
		c_ptr.info = c_ptr.info | CAVE_TRDT
		player.redraw[FLAG_PR_MAP] = 1
	end

	return found
end

function traps.find_one_object_trap(obj)
	local trap_list = obj.flags[FLAG_TRAP]
	local found     = traps.find_one_trap(trap_list)

	if found then
		message("You found a trap on the " .. object_desc(obj) .. "!")
		obj.flags[FLAG_TRAP_AWARE] = true
	end

	return found
end

function traps.find_all_traps(trap_list)
	if not trap_list then
		return 0
	end

	local all_traps = get_num_traps(trap_list)
	local known_traps = get_num_traps(trap_list, true)

	flag_learn_all(trap_list)

	return (all_traps - known_traps)
end

function traps.find_all_location_traps(y, x)
	local c_ptr = cave(y, x)
	local found = traps.find_all_traps(c_ptr.activations)

	if found > 0 then
		if found == 1 then
			message("You found a trap!")
		else
			message("You found some traps!")
		end
		c_ptr.info = c_ptr.info | CAVE_TRDT
		player.redraw[FLAG_PR_MAP] = 1
	end

	return (found > 0)
end

function traps.find_all_object_traps(obj)
	local trap_list = obj.flags[FLAG_TRAP]
	local found     = traps.find_all_traps(trap_list)

	if found > 0 then
		if found == 1 then
			message("You found a trap on the " ..
					object_desc(obj) .. "!")
		else
			message("You found some traps on the " ..
					object_desc(obj) .. "!")
		end
		obj.flags[FLAG_TRAP_AWARE] = true
	end

	return (found > 0)
end

function traps.trap_match(trap, faction, wanted_idx)
	if faction ~= -1 then
		if factions.get_friendliness(trap[FLAG_FACTION],
					    faction) >= 0 then
			return false
		end
	end

	local trap_idx = trap[FLAG_TRAP_IDX]

	if wanted_idx ~= -1 and wanted_idx ~= trap_idx then
		return false
	end

	return true
end -- traps.trap_match()


function traps.get_trap(faction, wanted_idx, y, x)
	local c_ptr     = cave(y, x)
	local trap_list = c_ptr.activations

	if not trap_list then
		return nil, nil
	end

	for i = 1, trap_list.size do
		if trap_list.node[i].flags & FLAG_FLAG_USED then
			local trap = flag_get_flags(trap_list,
						    trap_list.node[i].key)

			if trap and
				traps.trap_match(trap, faction,
						 wanted_idx) then
				return trap, i
			end
		end
	end

	return nil, nil
end -- traps.get_trap()

traps.status = {}
add_loadsave("traps.status", {})
hook(hook.BIRTH, function() traps.status = {} end)

function traps.ident(trap_idx)
	if traps.trap_list[trap_idx].never_know then
		if wizard then
			message(color.VIOLET,
				"Trying to identify unknowable trap")
		end
		return
	end

	if not traps.status[trap_idx] then traps.status[trap_idx] = {} end

	if not traps.status[trap_idx].ident then
		message("You identified the trap as " ..
			traps.trap_list[trap_idx].name .. ".")
		traps.status[trap_idx].ident = true
	end
end -- trap.ident()

function traps.is_ident(trap_idx)
	if wizard then return true end

	if traps.trap_list[trap_idx].never_know then
		return false
	end

	if not traps.status[trap_idx] then traps.status[trap_idx] = {} end

	return traps.status[trap_idx].ident
end -- traps.is_ident()

function traps.ident_name(trap_idx, trap)
	if traps.trap_list[trap_idx].name_func then
		return traps.trap_list[trap_idx].name_func(trap_idx, trap)
	end

	if traps.is_ident(trap_idx) then
		return traps.trap_list[trap_idx].name
	else
		return "unknown trap"
	end
end -- trap.ident_name()

function traps.get_known(know_all)
	local trap_list = {}

	for i = 1, traps.max_trap_idx do
		if ((traps.status[i] and traps.status[i].ident) or know_all) and
			not traps.trap_list[i].dont_dump
		then
			tinsert(trap_list, i)
			-- tinsert(trap_list, traps.ident_name(i))
		end
	end
	return trap_list
end -- traps.get_known()

hook
{
	-- Wipe the trap info on birth
	[hook.BIRTH] = function()
		for i = 1, traps.max_trap_idx do
			if traps.trap_list[i].easy_know then
				traps.status[i]       = {}
				traps.status[i].ident = true
			end
		end

	end,

	[hook.GAME_START] = function ()
		traps.random_trap_data.all   = {}
		traps.random_trap_data.monst = {}
		traps.prev_dun_level         = -1

		-- Sort traps by minlevel
		local sort_func = function(a, b)
							  return(a.minlevel <
									 b.minlevel)
						  end

		local trap_list = traps.trap_list_by_location.all
		for i = 1, getn(traps.locations) do
			sort(trap_list[traps.locations[i]], sort_func)
		end

		trap_list = traps.trap_list_by_location.monst
		for i = 1, getn(traps.locations) do
			sort(trap_list[traps.locations[i]], sort_func)
		end
	end,
}

-------------------------------------------------------------------------
-- Trap creation and placement
-------------------------------------------------------------------------
function traps.make_trap(trap_info, faction, obj)
	local trap = flag_new(2)
	local level = m_bonus(trap_info.level[2], dun_level)

	-- A negative value for trap_info.level[1] means that the
	-- trap's level should linearly and non-randomly increase with
	-- the dungeon level
	if  trap_info.level[1] < 0 then
		level = level + (-1 * trap_info.level[1] * dun_level)
	end

	trap[FLAG_TRAP_IDX]        = trap_info.index
	trap[FLAG_LEVEL]           = level
	trap[FLAG_TRAP_CHAR]       = trap_info.char
	trap[FLAG_TRAP_ATTR]       = trap_info.attr
	trap[FLAG_TRAP_POWER]      = trap_info.power
	trap[FLAG_FACTION]         = faction
	trap[FLAG_BEING_PROCESSED] = 0

	if trap_info.post_make then
		trap_info.post_make(trap, trap_info, faction, obj)
	end

	hook.process(hook.TRAP_MAKE_POST, trap, trap_info, faction, obj)

	return trap
end -- traps.make_trap()

function traps.make_random_trap(faction, location, obj)
	local trap_info = traps.get_random_trap(location, obj, faction)

	if not trap_info then
		return nil
	end

	return traps.make_trap(trap_info, faction, obj)
end -- traps.make_random_trap()

-- NOTE: The trap data is sorted by min level, so if the current dun_level
-- is too small for a trap's min level, then we should look below that
-- trap's index on further iterations, since traps at that index and
-- above are guarenteed to also have too large a min level.  This speeds
-- up random trap generation, and also makes it much less likely that
-- no suitable traps will be found in the 100 attempts that the function
-- takes.

-- This information is cached across invocations of traps.get_random_trap(),
-- to fruther speed things up.
function traps.get_random_trap(location, obj, faction)
	local locations = {traps.LOCATION_DOOR, traps.LOCATION_FLOOR,
		traps.LOCATION_OBJECT}
	local trap_info
	local arr, random_trap_data
	local max_idx

	assert(location ~= 0, "A location must be specified.")

	if traps.prev_dun_level ~= dun_level then
		-- Cached max index info is no longer good, regenerate it
		for i = 1, getn(traps.locations) do
			local loc = traps.locations[i]
			local random_data = {}

			random_data.max_idx =
				getn(traps.trap_list_by_location.all[loc])
			traps.random_trap_data.all[loc] = random_data

			random_data = {}
			random_data.max_idx =
				getn(traps.trap_list_by_location.monst[loc])
			traps.random_trap_data.monst[loc] = random_data
		end
		traps.prev_dun_level = dun_level
	end

	if not faction then
		faction = FACTION_DUNGEON
	end

	if factions.get_friendliness(faction, FACTION_PLAYER) >= 0 then
		arr              = traps.trap_list_by_location.monst[location]
		random_trap_data = traps.random_trap_data.monst[location]
	else
		arr              = traps.trap_list_by_location.all[location]
		random_trap_data = traps.random_trap_data.all[location]
	end

	max_idx = random_trap_data.max_idx
	if max_idx <= 0 then
		return nil
	end

	for i = 1, 100 do
		local idx = rng(max_idx)
		trap_info = arr[idx]

		if (dun_level < trap_info.minlevel) then
			max_idx = idx - 1
			random_trap_data.max_idx = max_idx

			if max_idx <= 0 then
				return nil
			end
		elseif (rng.percent(trap_info.proba)) then
			-- Check if the trap objects to being picked
			if trap_info.pre_make and
				trap_info.pre_random(location, obj) then
				-- Go for another iteration
			elseif hook.process(hook.TRAP_RANDOM_PRE, trap_info,
					    location, obj) then
				-- Go for another iteration
			else
				-- We're done!!
				hook.process(hook.TRAP_RANDOM_POST, trap_info,
					     location, obj)
				return trap_info
			end
		end -- elseif (rng.percent(trap_info.proba)) then
	end -- for i = 1, 100 do

	if wizard then
		message("ERROR: no random trap found!")
	end

	return nil
end -- traps.get_random_trap()

function traps.place_location_trap(trap, y, x, know)
	local trap_idx  = trap[FLAG_TRAP_IDX]
	local trap_info = traps.trap_list[trap_idx]
	local c_ptr     = cave(y, x)
	local feat      = f_info[c_ptr.feat + 1]
	local trap_list = c_ptr.activations

	-- Location based traps can only be placed on floor or door
	-- spots
	if not feat.flags[FLAG_FLOOR] and not feat.flags[FLAG_DOOR] then
		return false
	end

	-- Does the trap not want to be placed?
	if trap_info.pre_placement and
		trap_info.pre_placement(trap, y, x) then
		return false
	end

	if hook.process(hook.TRAP_PLACE_PRE, trap, y, x) then
		return false
	end

	if feat.flags[FLAG_TRAP_PLACE_PRE] then
		local flags = feat.flags
		local func

		func = get_function_registry_from_flag(flags,
						       FLAG_TRAP_PLACE_PRE)

		if func(trap, y, x) then
			return false
		end
	end

	local trap_key = flag_max_key(trap_list) + 1
	trap_list[trap_key] = trap

	traps.inc_recursion_depth(trap)

	if trap_info.post_placement then
		trap_info.post_placement(trap, y, x)
	end

	if trap[FLAG_GONE] then
		-- Trap removed right after being placed?  Weird.
		traps.dec_recursion_depth()
		return false
	end

	if feat.flags[FLAG_TRAP_PLACE_POST] then
		local flags = feat.flags
		local func

		func = get_function_registry_from_flag(flags,
						       FLAG_TRAP_PLACE_POST)

		func(trap, y, x)
	end

	if trap[FLAG_GONE] then
		-- Trap removed right after being placed?  Weird.
		traps.dec_recursion_depth()
		return false
	end

	traps.dec_recursion_depth(trap)

	if know then
		flag_learn(trap_list, trap_key, true)
		c_ptr.info = c_ptr.info | CAVE_TRDT
		player.redraw[FLAG_PR_MAP] = 1
	end

	return true
end -- traps.place_location_trap()

function traps.place_random_location_trap(y, x, faction)
	local trap
	local c_ptr = cave(y, x)
	local feat  = f_info[c_ptr.feat + 1]

	if feat.flags[FLAG_DOOR] then
		trap = traps.make_random_trap(faction, traps.LOCATION_DOOR)
	elseif feat.flags[FLAG_FLOOR] then
		trap = traps.make_random_trap(faction, traps.LOCATION_FLOOR)
	else
		-- Only place location traps on doors and floors
		return false
	end

	if not trap then
		return false
	end

	return traps.place_location_trap(trap, y, x)
end -- traps.place_random_location_trap()

function traps.place_object_trap(trap, obj, know)
	local trap_idx  = trap[FLAG_TRAP_IDX]
	local trap_info = traps.trap_list[trap_idx]
	local y, x      = obj.iy, obj.ix

	if not obj.flags[FLAG_TRAP] then
		obj.flags[FLAG_TRAP] = flag_new(2)
	end

	local trap_list = obj.flags[FLAG_TRAP]

	-- Does the trap not want to be placed?
	if trap_info.pre_placement and
		trap_info.pre_placement(trap, obj) then
		return false
	end

	if hook.process(hook.TRAP_PLACE_PRE, trap, obj) then
		return false
	end

	if obj.flags[FLAG_TRAP_PLACE_PRE] then
		local func

		func = get_function_registry_from_flag(obj.flags,
						       FLAG_TRAP_PLACE_PRE)

		if func(trap, obj) then
			return false
		end
	end

	local trap_key = flag_max_key(trap_list) + 1
	trap_list[trap_key] = trap

	traps.inc_recursion_depth(trap)

	if trap[FLAG_GONE] then
		-- Trap removed right after being placed?  Weird.
		traps.dec_recursion_depth()
		return false
	end

	if trap_info.post_placement then
		trap_info.post_placement(trap, y, x)
	end

	if trap[FLAG_GONE] then
		-- Trap removed right after being placed?  Weird.
		traps.dec_recursion_depth()
		return false
	end

	if obj.flags[FLAG_TRAP_PLACE_POST] then
		local func

		func = get_function_registry_from_flag(obj.flags,
						       FLAG_TRAP_PLACE_POST)

		func(trap, y, x)
	end

	if trap[FLAG_GONE] then
		-- Trap removed right after being placed?  Weird.
		traps.dec_recursion_depth()
		return false
	end

	traps.dec_recursion_depth(trap)

	if know then
		flag_learn(trap_list, trap_key, true)
	end

	return true
end -- traps.place_object_trap()

function traps.place_random_object_trap(obj, faction)
	local trap = traps.make_random_trap(faction, traps.LOCATION_OBJECT)

	if not trap then
		return false
	end

	return traps.place_object_trap(trap, obj)
end -- traps.place_random_object_trap()

