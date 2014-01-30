---------------------------------------------------------------------------
-- Killing walls (tunnelling)
---------------------------------------------------------------------------
constant("kill_wall", {})
settag(kill_wall, TAG_NAMESPACE)

new_flag("KILL_WALL_PRE", true)
new_flag("KILL_WALL_DO", true)
new_flag("KILL_WALL_FAIL", true)
new_flag("KILL_WALL_WORKING", true)
new_flag("KILL_WALL_DONE", true)

hook.new_hook_type("KILL_WALL_PRE")
hook.new_hook_type("KILL_WALL_FAIL")
hook.new_hook_type("KILL_WALL_WORKING")
hook.new_hook_type("KILL_WALL_DONE")

kill_wall.how_type = {
	NOTHING   = 1,
	INTRINSIC = 2,
	DAM_TYPE  = 3,
	SPELL     = 4,
	OBJECT    = 5
}

function kill_wall.do_kill(y, x, who, how_type, how_what)
	local c_ptr = cave(y, x)
	local f_ptr = f_info(c_ptr.feat)

	if has_flag(f_ptr, FLAG_PERMANENT) then
		return KILLWALL_FAILURE
	end

	local func
	local ret  = hook.process(hook.KILL_WALL_PRE, y, x, who,
							  how_type, how_what)
	if ret then
		return ret
	end

	if has_flag(f_ptr, FLAG_KILL_WALL_PRE) then
		func = get_function_registry_from_flag(f_ptr, FLAG_KILL_WALL_PRE)
		ret = func(y, x, who, how_type, how_what)
		if ret then
			return ret
		end
	end

	-- Find out what happened.  The function called right here usually
	-- doesn't do the feature change itself, but leaves it up to us.
	local did_replace = false
	if has_flag(f_ptr, FLAG_KILL_WALL_DO) then
		func = get_function_registry_from_flag(f_ptr, FLAG_KILL_WALL_DO)
		ret, did_replace = func(y, x, who, how_type, how_what)
	else
		ret, did_replace = kill_wall.kill_result(y, x, who, how_type, how_what)
	end
	ret = ret or KILLWALL_FAILURE

	if ret == KILLWALL_DONE then
		-- We usually do the terrain replacement ourselves.
		if not did_replace then
			-- Replace with random floor feat appropriate for the
			-- dungeon, unless the feature has something else in mind.
			c_ptr.mimic = 0
			if has_flag(f_ptr, FLAG_DESTROY_INTO) then
				cave_set_feat(y, x,
							  f_ptr.flags[FLAG_DESTROY_INTO])
			else
				place_floor(y, x)
			end
		end
		-- Unmap the spot
		player.update = (player.update | PU_VIEW | PU_UN_VIEW |
						 PU_FLOW | PU_DISTANCE | PU_MONSTERS |
						 PU_MON_LITE)
		c_ptr.info = c_ptr.info & negate_bits(CAVE_MARK)
		lite_spot(y, x)

		if has_flag(f_ptr, FLAG_KILL_WALL_DONE) then
			func = get_function_registry_from_flag(f_ptr,
												   FLAG_KILL_WALL_DONE)
			func(y, x, who, how_type, how_what)
		end
		hook.process(hook.KILL_WALL_DONE, y, x, who, how_type, how_what)
	elseif ret == KILLWALL_WORKING then
		if has_flag(f_ptr, FLAG_KILL_WALL_WORKING) then
			func = get_function_registry_from_flag(f_ptr,
												   FLAG_KILL_WALL_WORKING)
			func(y, x, who, how_type, how_what)
		end
		hook.process(hook.KILL_WALL_WORKING, y, x, who, how_type, how_what)
	elseif ret == KILLWALL_FAILURE then
		if has_flag(f_ptr, FLAG_KILL_WALL_FAIL) then
			func = get_function_registry_from_flag(f_ptr,
												   FLAG_KILL_WALL_FAIL)
			func(y, x, who, how_type, how_what)
		end
		hook.process(hook.KILL_WALL_FAIL, y, x, who, how_type, how_what)
	else
		error("Invalid killwall return type.")
	end

	return ret
end -- kill_wall.do_kill

function kill_wall.mon_kill(y, x, who)
	return kill_wall.do_kill(y, x, who, kill_wall.how_type.INTRINSIC)
end -- kill_wall.mon_kill()

function kill_wall.kill_result(y, x, who, how_type, how_what)
	local result, did_replace
	if who == WHO_PLAYER then
		result, did_replace =
			kill_wall.player_result(y, x, how_type, how_what)
	else
		result, did_replace =
			kill_wall.mon_result(y, x, who, how_type, how_what)
	end
	return result, did_replace
end

-- NOTE: Modules should override these functions, since it only provides the
-- bare minimum for a tunneling system: tunnelling always succeeds on
-- the first try, and doesn't do anything interesting, like giving
-- a "You hear a grinding sound" message when a monster bores through
-- rock.
--
-- These functions return whether or not the dig/tunnel/kill failed
-- completely, worked but isn't done yet, or finished.  The second
-- return argument is whether or not the function changed the
-- square's feature itself: true if it did, false if it didn't and
-- wants the framework to change the feature in the default manner.
function kill_wall.mon_result(y, x, who, how_type, how_what)
	return KILLWALL_DONE, false
end

function kill_wall.player_result(y, x, how_type, how_what)
	return KILLWALL_DONE, false
end

