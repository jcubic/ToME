-----------------------------------------------------------------------
-- Tunnelling subsystem
-----------------------------------------------------------------------

constant("tunnel", {})
settag(tunnel, TAG_NAMESPACE)

safe_new_flag("TUNNELABLE")
safe_new_flag("NO_TUNNEL_MSG")
safe_new_flag("DIGGER_MIMIC")
safe_new_flag("TUNNEL_FAIL_MSG")
safe_new_flag("TUNNEL_WORKING_MSG")
safe_new_flag("TUNNEL_DONE_MSG")
safe_new_flag("ON_TUNNEL_MISMATCH", true)
safe_new_flag("DIG_POWER")
safe_new_flag("ADD_DIG_POWER")

hook.new_hook_type("GET_DIGGER")

tunnel.extra_msgs = {}

--------------------------------------------------------------------------
--------------------------------------------------------------------------

--
-- Customizable subsystem stuff
--

function tunnel.max_by_flags(array, feat)
	local found = false
	local val   = -infinity

	for i = 1, flag_max_key(feat.flags) do
		if feat.flags[i] and array[i] then
			val   = max(val, array[i])
			found = true
		end
	end

	if found then
		return val
	end
end -- tunnel.max_by_flags()

function tunnel.index_by_flags(array, feat)
	for i = 1, flag_max_key(feat.flags) do
		if feat.flags[i] and array[i] then
			return array[i]
		end
	end
end -- tunnel.index_by_flags()

tunnel.no_digger_msgs_by_flag = {}
tunnel.no_tunnel_msgs = {}
tunnel.working_msgs_by_feat = {}
tunnel.working_msgs_by_flag = {}
tunnel.done_msgs_by_feat = {}
tunnel.done_msgs_by_flag = {}
tunnel.no_progress_msgs_by_flags = {}
tunnel.min_power_by_flags = {}
tunnel.hardness_by_flags = {}

function tunnel.add_no_digger_msgs_by_flag(t)
	for k, v in t do
		tunnel.no_digger_msgs_by_flag[k] = v
	end
end

function tunnel.add_no_tunnel_msgs(t)
	for k, v in t do
		tunnel.no_tunnel_msgs[k] = v
	end
end

function tunnel.add_working_msgs_by_feat(t)
	for k, v in t do
		tunnel.working_msgs_by_feat[k] = v
	end
end

function tunnel.add_working_msgs_by_flag(t)
	for k, v in t do
		tunnel.working_msgs_by_flag[k] = v
	end
end

function tunnel.add_done_msgs_by_feat(t)
	for k, v in t do
		tunnel.done_msgs_by_feat[k] = v
	end
end

function tunnel.add_done_msgs_by_flag(t)
	for k, v in t do
		tunnel.done_msgs_by_flag[k] = v
	end
end

function tunnel.add_no_progress_msgs_by_flags(t)
	for k, v in t do
		tunnel.no_progress_msgs_by_flags[k] = v
	end
end

function tunnel.add_min_power_by_flags(t)
	for k, v in t do
		tunnel.min_power_by_flags[k] = v
	end
end

function tunnel.add_hardness_by_flags(t)
	for k, v in t do
		tunnel.hardness_by_flags[k] = v
	end
end

-- Have to wait features are loaded to use them.
hook(hook.INFO_DATA_LOADED,
function()
	tunnel.add_no_tunnel_msgs {
		[FEAT_LESS] = "You cannot tunnel a staircase.",
		[FEAT_MORE] = "You cannot tunnel a staircase.",
		[FEAT_WAY_MORE] = "You cannot tunnel a pathway.",
		[FEAT_WAY_MORE] = "You cannot tunnel a pathway.",
		[FEAT_SHAFT_DOWN] = "You cannot tunnel a shaft.",
		[FEAT_SHAFT_UP] = "You cannot tunnel a shaft."
	}

	tunnel.add_working_msgs_by_feat {
		[FEAT_RUBBLE] = "You dig in the @feat@."
	}

	tunnel.add_done_msgs_by_feat {
		[FEAT_RUBBLE] = "You have removed the @feat@."
	}
end)

tunnel.add_working_msgs_by_flag {
	[FLAG_DOOR]          = "You bash the door.",
	[FLAG_SUBST_RUBBLE]  = "You dig in the @feat@."
	[FLAG_SUBST_GRANITE] = "You tunnel into the @feat@."
}

tunnel.add_done_msgs_by_flag {
	[FLAG_DOOR]          = "You have bashed in the door.",
	[FLAG_SUBST_RUBBLE]  = "You have removed the @feat@.",
	[FLAG_SUBST_GRANITE] = "You have removed the @feat@."
}

tunnel.add_min_power_by_flags {
	[FLAG_SUBST_RUBBLE]  = 0,
	[FLAG_SUBST_DOOR]    = 30,
	[FLAG_JAMMED]        = 40,
	[FLAG_SUBST_GRANITE] = 40,
}

tunnel.add_hardness_by_flags {
	[FLAG_SUBST_RUBBLE]  = 200,
	[FLAG_SUBST_DOOR]    = 1200,
	[FLAG_JAMMED]        = 1600,
	[FLAG_SUBST_GRANITE] = 1600,
}

----------------------------------------

tunnel.no_digger_msg = get_subsystem_param("tunnel", "no_digger_msg")

function tunnel.format_tunnel_msg(msg, feat, how_name, how_plur)
	how_name = how_name or tunnel.how_name or "NO DIGGER"
	how_plur = how_plur or tunnel.how_plur

	msg = gsub(msg, "(@Digger@)", strcap(how_name))
	msg = gsub(msg, "(@digger@)", how_name)
	msg = gsub(msg, "(@Feat@)", strcap(feat.name))
	msg = gsub(msg, "(@feat@)", feat.name)

	if how_plur then
		msg = gsub(msg, "(@s@)", "")
		msg = gsub(msg, "(@es@)", "")
		msg = gsub(msg, "(@it@)", "they")
		msg = gsub(msg, "(@does@)", "do")
		msg = gsub(msg, "(@doesn't@)", "don't")
		msg = gsub(msg, "(@has@)", "have")
		msg = gsub(msg, "(@hasn't@)", "haven't")
	else
		msg = gsub(msg, "(@s@)", "s")
		msg = gsub(msg, "(@es@)", "es")
		msg = gsub(msg, "(@it@)", "it")
		msg = gsub(msg, "(@does@)", "does")
		msg = gsub(msg, "(@doesn't@)", "doesn't")
		msg = gsub(msg, "(@has@)", "has")
		msg = gsub(msg, "(@hasn't@)", "hasn't")
	end

	return msg
end -- tunnel.format_tunnel_msg

----------------------------------------------------

-- First, we have to add a FLAG_ADD_DIG_POWER flagset back into the
-- intrinsic flags at the start of each bonus calculation round,
-- so it'll be there to add to.
hook(hook.CALC_BONUS_BEGIN,
function()
	player.intrinsic_flags[FLAG_ADD_DIG_POWER] = flag_new(2)
end)

function tunnel.default_get_dig_power(how_type, how_what, known, feat,
									  y, x)
	local power = 0
	if how_type == kill_wall.how_type.INTRINSIC then
		local flags = player.intrinsic_flags[FLAG_KILL_WALL]

		if flags and flag_intersects(flags, feat.flags) then
			-- Engine default is that monsters always kill walls
			-- with 100% chance.
			power = infinity
		else
			return nil
		end
	elseif how_type == kill_wall.how_type.OBJECT then
		local flags = how_what.flags[FLAG_DIG_POWER]

		if flags then
			local tmp = {val = 0}
			foreach_flags(flags,
				function(flags, key)
					if %feat.flags[key] and
						(not %known or flag_is_known(flags, key))
					then
						%tmp.val = max(%tmp.val, flags[key])
					end
				end) -- foreach_flags
			power = tmp.val
		else
			return nil
		end
	else
		error("Only know how to handle object and intrinsic " ..
			  "kill_wall types.")
	end

	-- Don't add in bonus digging power if it needs to be known,
	-- since determining which bonuses are known and which aren't
	-- would be a big pain.
	if known then
		return power
	end

	-- No need to chance it it's infinite
	if power >= infinity then
		return power
	end

	local flags = player.intrinsic_flags[FLAG_ADD_DIG_POWER]

	if not flags then
		return power
	end

	local tmp = {min = 0, max=0}
	foreach_flags(flags,
		function(flags, key)
			if %feat.flags[key] then
				%tmp.min = min(%tmp.min, flags[key])
				%tmp.max = max(%tmp.max, flags[key])
			end
		end) -- foreach_flags

	power = power + tmp.max + tmp.min

	return power
end -- tunnel.default_get_dig_power()

tunnel.get_dig_power = get_subsystem_param("tunnel",
										   "get_dig_power") or
	tunnel.default_get_dig_power

------------------------------------------

tunnel.is_plural = get_subsystem_param("tunnel", "is_plural") or
function(obj)
	return false
end

tunnel.get_digger_name = get_subsystem_param("tunnel",
											 "get_digger_name") or
function(how_type, how_what)
	if how_type == kill_wall.how_type.INTRINSIC then
		return "your hands", true
	end

	if how_type == kill_wall.how_type.OBJECT then
		local obj = how_what

		if is_artifact(obj) then
			return "the " .. object_desc(obj), tunnel.is_plural(obj)
		else
			return "your " .. object_desc(obj), tunnel.is_plural(obj)
		end
	end

	return "CAN'T GET DIGGER NAME", false
end -- tunnel.get_digger_name()

tunnel.get_digger_msgs = get_subsystem_param("tunnel",
											 "get_digger_msgs") or
function(feat, y, x, how_type, how_what)
	return nil, nil
end -- tunnel.get_digger_msgs()

tunnel.get_digger = get_subsystem_param("tunnel", "get_digger") or
function(feat, y, x)
	local whats = {}

	if has_flag(feat, FLAG_DIGGER_MIMIC) then
		feat = f_info(feat.flags[FLAG_DIGGER_MIMIC])
	end

	-- Can player do it without any object?
	local power = tunnel.get_dig_power(kill_wall.how_type.INTRINSIC, nil,
									   true, feat, y, x)
	if power and power >= 1 then
		local name, plur =
			tunnel.get_digger_name(kill_wall.how_type.INTRINSIC)
		tinsert(whats, {
					power    = power,
					how_type = kill_wall.how_type.INTRINSIC,
					how_what = nil,
					how_name = name,
					how_plur = plur,
				})
	end

	-- Go through equipment looking for diggers.
	for_inventory(player, INVEN_PACK, INVEN_TOTAL,
		function(obj, pos, slot, item)
			local power = tunnel.get_dig_power(kill_wall.how_type.OBJECT,
											   obj, true, %feat, %y, %x)

			if power and power >= 1 then
				local name, plur =
					tunnel.get_digger_name(kill_wall.how_type.OBJECT,
										   obj)
				tinsert(%whats, {
							power    = power,
							how_type = kill_wall.how_type.OBJECT,
							how_what = obj,
							how_name = name,
							how_plur = plur,
						})
			end -- if power and power >= 1 then
		end) -- for_inventory()

	-- Let hooks add potential diggers.
	hook.process(hook.GET_DIGGER, feat, y, x, whats)

	if getn(whats) == 0 then
		-- No diggers found.
		return nil
	end

	-- Pick digger with biggest power.
	sort(whats,
		 function(a, b)
			 return a.power > b.power -- Sort in reverse order.
		 end)
	local digger = whats[1]

	local tunnel_working_msg, tunnel_done_msg
	if digger.how_type == kill_wall.how_type.OBJECT then
		tunnel_working_msg = digger.how_what.flags[FLAG_TUNNEL_WORKING_MSG]
		tunnel_done_msg = digger.how_what.flags[FLAG_TUNNEL_DONE_MSG]
	end
	tunnel_working_msg, tunnel_done_msg =
		tunnel.get_digger_msgs(feat, y, x, digger.how_type,
							   digger.how_what)

	digger.tunnel_working_msg = digger.tunnel_working_msg or
		tunnel_working_msg
	digger.tunnel_done_msg = digger.tunnel_done_msg or
		tunnel_done_msg

	return digger
end -- tunnel.get_digger()

------------------------------

tunnel.progress_msg_chance = get_subsystem_param("tunnel",
												 "progress_msg_chance") or
function(power, hardness, feat)
	if command_rep == 0 or power < 1 then
		return 100
	else
		return 0
	end
end

tunnel.no_progress_msg = get_subsystem_param("tunnel",
											 "no_progress_msg") or
	"@Digger@ @does@ not have any effect on the @feat@."

tunnel.do_progress_msg = get_subsystem_param("tunnel",
											 "do_progress_msg") or
function(power, hardness, feat)
	local msg
	if power < 1 then
		msg = tunnel.index_by_flags(tunnel.no_progress_msgs_by_flags,
									feat)
		msg = msg or tunnel.no_progress_msg
	else
		local avg_reps = hardness / power

		if avg_reps >= 1000 then
			msg = "This will take an extremely long time."
		elseif avg_reps >= 750 then
			msg = "This will take a very, very long time."
		elseif avg_reps >= 500 then
			msg = "This will take a very long time."
		elseif avg_reps >= 250 then
			msg = "This will take a long time."
		elseif avg_reps >= 100 then
			msg = "This will take some time."
		elseif avg_reps >= 50 then
			msg = "This should be over soon."
		elseif avg_reps >= 10 then
			msg = "This should be over very soon."
		else
			msg = "This should be over in no time."
		end
	end

	if msg then
		if command_rep == 0 or power < 1 then
			tinsert(tunnel.extra_msgs, tunnel.format_tunnel_msg(msg, feat))
		else
			message(tunnel.format_tunnel_msg(msg, feat))
		end
	end
end -- tunnel.do_progress_msg()

tunnel.kill_wall_result = get_subsystem_param("tunnel",
											  "kill_wall_result") or
function(y, x)
	local c_ptr = cave(y, x)
	local feat  = f_info(c_ptr.feat)

	local min_power = feat.flags[FLAG_TUNNELABLE]
	local hardness  = flag_get2(feat.flags, FLAG_TUNNELABLE)

	if not min_power then
		min_power = tunnel.max_by_flags(tunnel.min_power_by_flags,
										feat)
		hardness = tunnel.max_by_flags(tunnel.hardness_by_flags,
									   feat)
	end

	assert(min_power, "No min_power defined for " .. feat.name)
	assert(hardness, "No hardness defined for " .. feat.name)

	local how_type = tunnel.how_type
	local how_what = tunnel.how_what
	local power = tunnel.get_dig_power(how_type, how_what, false,
									   feat, y, x)
	power = power or 0
	power = power - min_power

	if power > rng.number(hardness) then
		return KILLWALL_DONE, false
	end

	if c_ptr.mimic ~= 0 then
		feat = f_info(c_ptr.mimic)
	end

	if rng.percent(tunnel.progress_msg_chance(power, hardness)) then
		tunnel.do_progress_msg(power, hardness, feat)
	end

	if power < 1 then
		return KILLWALL_FAILURE, false
	end

	return KILLWALL_WORKING, false
end -- tunnel.kill_wall_result()

tunnel.do_sound = get_subsystem_param("tunnel", "do_sound") or
function(result, y, x, dir, feat_real, feat_fake)
end

-----------------------------------------------------------------------------
-----------------------------------------------------------------------------

--
-- Actually doing the tunneling
--

function tunnel.exec(y, x, dir, feat_real, feat_fake)
	local f_ptr_real = f_info(feat_real)
	local f_ptr_fake = iif(feat_fake, f_info(feat_fake), nil)
	local f_ptr      = f_ptr_fake or f_ptr_real
	local feat       = feat_fake  or feat_real

	if has_flag(f_ptr, FLAG_NO_TUNNEL_MSG) or tunnel.no_tunnel_msgs[feat] or
		(has_flag(f_ptr, FLAG_PERMANENT) and has_flag(f_ptr, FLAG_WALL))
	then
		local msg = f_ptr.flags[FLAG_NO_TUNNEL_MSG]
		msg = msg or tunnel.no_tunnel_msgs[feat]
		msg = msg or "The @feat@ is too hard to tunnel through."
		message(tunnel.format_tunnel_msg(msg, f_ptr))

		return KILLWALL_FAILURE
	end

	energy_use = get_player_energy(SPEED_DIG)

	-- Digger saved from previous repitition?  Saving digger might
	-- not be needed for speed purposes, but the digger shouldn't
	-- change during a single repitition of digging because of
	-- other stuff happening in the dungeon.
	if not tunnel.digger_info then
		-- No digger saved, get one
		local digger_info = tunnel.get_digger(f_ptr, y, x)

		if not digger_info then
			if has_flag(f_ptr, FLAG_WALL) or not
				(has_flag(f_ptr, FLAG_KILL_WALL_PRE) or
				 has_flag(f_ptr, FLAG_KILL_WALL_DO))
			then
				-- No digger found, do nothing
				local msg =
					tunnel.index_by_flags(tunnel.no_digger_msgs_by_flag,
										  f_ptr)
				msg = msg or tunnel.no_digger_msg
				if msg then
					msg = tunnel.format_tunnel_msg(msg, f_ptr)
					message(msg)
				end

				energy_use = 0
				return KILLWALL_FAILURE
			else
				-- We've been called even though the square isn't a
				-- wall.  The square must be some weird type of
				-- feature which defines a KILL_WALL_PRE or
				-- KILL_WALL_DO even though it isn't a wall.
				digger_info = {
					how_type = kill_wall.how_type.NOTHING
					how_name = ""
				}
			end
		end

		local how_type = digger_info.how_type
		local how_what = digger_info.how_what

		local power = tunnel.get_dig_power(how_type, how_what, false,
										   f_ptr_real, y, x)

		if (not power or power < 1) and how_type == kill_wall.how_type.NOTHING
		then
			power = 1
		end

		if f_ptr_fake and (digger_info.mismatch_func or
						   has_flag(f_ptr_real, FLAG_ON_TUNNEL_MISMATCH) or
						   (how_type == kill_wall.how_type.OBJECT and
							has_flag(how_what, FLAG_ON_TUNNEL_MISMATCH)))
		then
			local digger_info_real = tunnel.get_digger(f_ptr_real, y, x)

			if digger_info_real.how_type ~= how_type or
				digger_info_real.how_what ~= how_what
			then
				local ret
				local func = digger_info.mismatch_func
				if func then
					ret = func(y, x, dir, f_ptr_real, f_ptr_fake,
							   digger_info, digger_info_real)
					if ret then
						return ret
					end
				end

				local flag = FLAG_ON_TUNNEL_MISMATCH
				if has_flag(r_ptr_real, flag) then
					func =
						get_function_registry_from_flag(f_ptr_real.flags,
														flag)
					ret = func(y, x, dir, f_ptr_real, f_ptr_fake,
							   digger_info, digger_info_real)
					if ret then
						return ret
					end
				end

				if how_type == kill_wall.how_type.OBJECT then
					ret = item_hooks.process_one(how_what, flag, y, x, dir,
												 f_ptr_real, f_ptr_fake,
												 digger_info,
												 digger_info_real)
					if ret then
						return ret
					end
				end
			end -- if diggers don't match then
		end -- if f_ptr_fake and ... then

		-- Save stuff for future repititions (and as pseudo-global
		-- variables for other code to look at)
		tunnel.how_type = digger_info.how_type
		tunnel.how_what = digger_info.how_what
		tunnel.how_name = digger_info.how_name
		tunnel.how_plur = digger_info.how_plur
		tunnel.power    = power

		tunnel.digger_info = digger_info
	end -- if not tunnel.how_type then

	-- Now we have a digger, so do it.
	local ret
	ret = kill_wall.do_kill(y, x, WHO_PLAYER, tunnel.how_type,
							tunnel.how_what)

	if ret == KILLWALL_DONE then
		-- handle_stuff() to get CAVE_MARK updated.
		handle_stuff()

		local c_ptr = cave(y, x)
		if c_ptr.info & CAVE_MARK ~= 0 then
			-- Re-map the spot if we can see it.
			note_spot(y, x)
			lite_spot(y, x)
		end

		-- If we can see the result, then we give messages according
		-- to what it really was.  If we can't see it and it was
		-- mimimicing something, we give messages according to what
		-- the player thought it was.
		if c_ptr.info & CAVE_MARK ~= 0 then
			f_ptr = f_ptr_real
			feat  = feat_real
		end

		local msg = tunnel.digger_info.tunnel_done_msg
		msg = msg or f_ptr.flags[FLAG_TUNNEL_DONE_MSG]
		msg = msg or tunnel.done_msgs_by_feat[feat]
		msg = msg or
			tunnel.index_by_flags(tunnel.done_msgs_by_flag,
								  f_ptr)
		msg = msg or "You have removed the @feat@."

		-- HACK: handle destroying secret doors.
		if c_ptr.info & CAVE_MARK ~= 0 and
			has_flag(f_ptr_real, FLAG_SECRET) and
			c_ptr.feat == FEAT_BROKEN
		then
			msg = "The @feat@ appears to have actually been a secret door!"
		end

		msg = tunnel.format_tunnel_msg(msg, f_ptr,
									   tunnel.how_name,
									   tunnel.how_plur)
		message(msg)
	elseif ret == KILLWALL_WORKING then
		local msg = tunnel.digger_info.tunnel_working_msg
		msg = msg or f_ptr.flags[FLAG_TUNNEL_WORKING_MSG]
		msg = msg or tunnel.working_msgs_by_feat[feat]
		msg = msg or
			tunnel.index_by_flags(tunnel.working_msgs_by_flag,
								  f_ptr)
		if msg then
			msg = tunnel.format_tunnel_msg(msg, f_ptr,
										   tunnel.how_name,
										   tunnel.how_plur)
			message(msg)
		end
	end

	tunnel.do_sound(ret, y, x, dir, feat_real, feat_fake)

	tunnel.handle_extra_msgs()

	return ret
end -- tunnel.exec()

function tunnel.handle_extra_msgs()
	if not tunnel.extra_msgs then
		tunnel.extra_msgs = {}
	end

	for i = 1, getn(tunnel.extra_msgs) do
		message(tunnel.extra_msgs[i])
	end

	tunnel.extra_msgs = {}
end -- tunnel.handle_extra_msgs()

-- Override default kill_wall.player_result()
function kill_wall.player_result(y, x, how_type, how_what)
	local result, did_replace =
		tunnel.kill_wall_result_aux(y, x, how_type, how_what)

	return result, did_replace
end

-- Invoke hooks, then invoke subsystem parameter tunnel.kill_wall_result()
function tunnel.kill_wall_result_aux(y, x, how_type, how_what)
	if how_type ~= kill_wall.how_type.OBJECT then
		local result, did_replace =
			tunnel.kill_wall_result(y, x)
		result = result or KILLWALL_FAILURE

		return result, did_replace
	end

	-- Is the player digging away at a wall that isn't reall there?
	local c_ptr = cave(y, x)
	if c_ptr.mimic ~= 0 and not has_flag(f_info(c_ptr.feat), FLAG_WALL) then
		local msg = "@Digger@ passes through the @feat@ like it isn't there."
		message(tunnel.format_tunnel_msg(msg, f_info(c_ptr.mimic)))
		return KILLWALL_FAILURE
	end

	local obj         = how_what
	local did_replace = {val = false}
	local result, ret

	result = item_hooks.process_one(obj, FLAG_KILL_WALL_PRE,
									y, x, did_replace)

	if result then
		-- Whatever happened happend in the pre hook, do nothing
	elseif has_flag(obj, FLAG_KILL_WALL_DO) then
		-- Object will do the digging itself
		result = item_hooks.process_one(obj, FLAG_KILL_WALL_DO,
										y, x, did_replace)
	else
		-- Do it normally with paramater-izable function
		result, did_replace.val =
			tunnel.kill_wall_result(y, x)
	end
	result = result or KILLWALL_FAILURE

	-- Inform object of result, let it change return value.
	if result == KILLWALL_FAILURE then
		ret = item_hooks.process_one(obj, FLAG_KILL_WALL_FAIL,
									 y, x, did_replace)
	elseif result == KILLWALL_WORKING then
		ret = item_hooks.process_one(obj, FLAG_KILL_WALL_WORKING,
									 y, x, did_replace)
	elseif result == KILLWALL_DONE then
		ret = item_hooks.process_one(obj, FLAG_KILL_WALL_DONE,
									 y, x, did_replace)
	end

	ret = ret or result

	return ret, did_replace.val
end -- tunnel.kill_wall_result_aux()

function tunnel.do_tunnel(y, x, dir)
	tunnel.was_disturbed = true

	local c_ptr = cave(y, x)
	if c_ptr.info & CAVE_MARK == 0 then
		-- Player doesn't know what the grid is, so doesn't know what
		-- to use to tunnel through it, or what sort of alteration to
		-- make to it at all.
		if tunnel.no_mark_msg then
			message(tunnel.no_mark_msg)
		end
		return false
	end

	local f_ptr_real, f_ptr_fake, feat_real, feat_fake
	feat_real  = c_ptr.feat
	f_ptr_real = f_info(feat_real)

	if has_flag(f_ptr_real, FLAG_FIND_INTO) then
		feat_real  = f_ptr_real.flags[FLAG_FIND_INTO]
		f_ptr_real = f_info(feat_real)
	end

	if c_ptr.mimic ~= 0 and c_ptr.mimic ~= c_ptr.feat then
		feat_fake  = c_ptr.mimic
		f_ptr_fake = f_info(feat_fake)
	end

	local f_ptr = f_ptr_fake or f_ptr_real
	local feat  = feat_fake or feat_real

	if tunnel.prev_feat and tunnel.prev_feat ~= feat then
		-- What we were tunnelling suddenly changed on us, stop.
		tunnel.prev_feat = nil
		tunnel.how_type  = nil
		term.disturb(0, 0)
		tunnel.flsuh(true)
		return true, false
	end

	-- Let other code deal with opening or bashing doors.
	if has_flag(f_ptr, FLAG_DOOR) and not has_flag(f_ptr, FLAG_SECRET) then
		tunnel.flsuh()
		return false
	end

	-- We can tunnel a non-wall if it has a hook defined for
	-- KILL_WALL_PRE or KILL_WALL_DO.
	if not (has_flag(f_ptr, FLAG_WALL) or
			has_flag(f_ptr, FLAG_KILL_WALL_PRE) or
			has_flag(f_ptr, FLAG_KILL_WALL_DO))
	then
		-- "Alter grid" includes disarming traps.   But if there
		-- are no traps on the grid, then the non-wall grid might
		-- want to give a message about how it can't be tunneled.
		if get_num_location_traps(y, x, true, FACTION_PLAYER,
								  false, true) == 0
		then
			local msg = f_ptr.flags[FLAG_NO_TUNNEL_MSG]
			msg = msg or tunnel.no_tunnel_msgs[feat]
			if msg then
				if has_flag(f_ptr, FLAG_WALL) or
					has_flag(f_ptr, FLAG_DIGGER_MIMIC)
				then
					tunnel.exec(y, x, dir, feat_real, feat_fake)
				else
					message(tunnel.format_tunnel_msg(msg, f_ptr))
				end
				tunnel.flsuh()
				return true, false
			end
		end

		tunnel.flsuh()
		return false
	end

	tunnel.prev_feat = feat

	if tunnel.exec(y, x, dir, feat_real, feat_fake) == KILLWALL_WORKING then
		-- Not done, return TRUE to caller to let it know that it should
		-- keep going with repeats if auto-repetition is on.
		tunnel.flsuh()
		return true, true
	else
		-- Either failed or done, so no need to repeat command anymore.
		tunnel.flsuh(true)
		return true, false
	end
end

function tunnel.flsuh(force)
	if force or tunnel.was_disturbed then
		tunnel.how_type  = nil
		tunnel.how_what  = nil
		tunnel.how_name  = nil
		tunnel.prev_feat = nil
		tunnel.power     = nil

		tunnel.digger_info = nil

		tunnel.handle_extra_msgs()
	end

	tunnel.was_disturbed = false
end -- tunnel.flsuh()

----------------
-- Various hooks
----------------

hook(hook.ALTER_GRID, tunnel.do_tunnel)

if not get_subsystem_param("tunnel", "no_key_bind") then
hook(hook.KEYPRESS, function(key)
	if key == strbyte('T') then
		local y, x, dir, ret
		local more = false

		-- Allow repeated command
		if command_arg > 0 then
			-- Set repeat count
			command_rep = command_arg - 1

			-- Redraw the state
			player.redraw[FLAG_PR_STATE] = true

			-- Cancel the arg
			command_arg = 0
		end

		-- Get a direction
		ret, dir = get_rep_dir()
		if not ret then
			term.disturb(0, 0)
			tunnel.flsuh()
			return true
		end

		-- Get location
		y = player.py + ddy(dir)
		x = player.px + ddx(dir)

		more = tunnel.do_tunnel(y, x, dir)

		-- Cancel repetition unless we can continue
		if not more then term.disturb(0, 0) end
		tunnel.flsuh()
		return true
	end
end)
end

-- Clear state if we stop tunneling due to interruption.
hook(hook.DISTURB,
function()
	if tunnel.digger_info then
		tunnel.was_disturbed = true
	end
end)

-- Handle automatic ("easy") tunneling if we run into a wall.
hook(hook.HIT_WALL,
function(y, x, dir)
	local c_ptr = cave(y, x)

	if not game_options.easy_tunnel or c_ptr.info & CAVE_MARK == 0 then
		return false
	end

	local feat_real = c_ptr.feat
	local feat_fake
	if c_ptr.mimic ~= 0 and c_ptr.mimc ~= feat_real then
		feat_fake = c_ptr.mimic
	end

	-- Wall must be must be diggable.
	local f_ptr = f_info(feat_fake or feat_real)
	if has_flag(f_ptr, FLAG_PERMANENT) or
		not (has_flag(f_ptr, FLAG_WALL) or
			 has_flag(f_ptr, FLAG_KILL_WALL_PRE) or
			 has_flag(f_ptr, FLAG_KILL_WALL_DO)) then
		return false
	end

	-- Is diggable, do it.
	tunnel.exec(y, x, dir, feat_real, feat_fake)
	term.disturb(0, 0)
	tunnel.flsuh()
	return true
end)
