--
 -- For various object-related hooks, inform the object itself of the
-- hook happening
--

constant("item_hooks", {})
settag(item_hooks, TAG_NAMESPACE) -- Tag as a namespace

function item_hooks.process_one(obj, flag, ...)
	local func
	local list = obj.flags[flag]

	if not list then
		return nil
	end

	local max  = flag_max_key(list)

	for i = 1, list.size do
		if (list.node[i].flags & FLAG_FLAG_USED) ~= 0 then
			local key = list.node[i].key

			func = get_function_registry_from_flag(list, key)
			local ret = func(obj, unpack(arg))

			if ret then
				return ret
			end
		end
	end
end

function item_hooks.process_all(obj, flag, ...)
	local ret  = nil
	local func
	local list = obj.flags[flag]

	if not list then
		return nil
	end

	local max  = flag_max_key(list)

	for i = 1, list.size do
		if (list.node[i].flags & FLAG_FLAG_USED) ~= 0 then
			local key = list.node[i].key

			func = get_function_registry_from_flag(list, key)
			ret = ret or func(obj, unpack(arg))
		end
	end

	return ret
end

function item_hooks.process_all_chain(obj, flag, chain_arg, ...)
	local func
	local list = obj.flags[flag]

	if not list then
		return chain_arg
	end

	local max  = flag_max_key(list)

	for i = 1, list.size do
		if (list.node[i].flags & FLAG_FLAG_USED) ~= 0 then
			local key = list.node[i].key

			func = get_function_registry_from_flag(list, key)
			chain_arg = func(obj, chain_arg, unpack(arg))

			if not chain_arg then
				return nil
			end
		end
	end

	return chain_arg
end

------------------------------------------------------------

function item_hooks.collect_pack_maybe_floor(y, x, flag, floor)
	local items = {}

	-- Pack
	for_inventory(player, INVEN_INVEN, INVEN_TOTAL,
				  function(obj, inven, slot, item)

					  if obj.flags[%flag] then
						  tinsert(%items, {obj=obj, item=item,
									  inven=inven, slot=slot})
					  end
				  end)

	if floor then
		-- Floor.  Do floor second, since the list is iterated over
		-- backwards, and we want to floor items befre pack items
		local c_ptr = cave(y, x)
		for_inventory_inven(cave(y, x).inventory,
							function(obj, item)
								if obj.flags[%flag] then
									tinsert(%items, {obj=obj, item=-item})
								end
							end)
	end

	return items
end

function item_hooks.collect_floor_and_pack(y, x, flag)
	return item_hooks.collect_pack_maybe_floor(y, x, flag, true)
end

function item_hooks.collect_pack(flag)
	return item_hooks.collect_pack_maybe_floor(-1, -1, flag, false)
end

function item_hooks.collect_equip(flag)
	local items = {}

	-- Equipment
	for_inventory(player, INVEN_PACK, INVEN_TOTAL,
				  function(obj, inven, slot, item)

					  if obj.flags[%flag] then
						  tinsert(%items, {obj=obj, item=item,
									  inven=inven, slot=slot})
					  end
				  end)

	return items
end

function item_hooks.process_one_floor_and_pack(y, x, flag, ...)
	local items = item_hooks.collect_floor_and_pack(y, x, flag)

	for i = getn(items), 1, -1 do
		local item = items[i]
		local ret

		ret = item_hooks.process_one(item.obj, flag, item.item, item.inven,
									 item.slot, unpack(arg))

		if ret then
			return ret
		end
	end -- for i = getn(items), 1, -1 do
end

function item_hooks.process_all_floor_and_pack(y, x, flag, ...)
	local items = item_hooks.collect_floor_and_pack(y, x, flag)
	local ret

	for i = getn(items), 1, -1 do
		local item = items[i]

		ret = item_hooks.process_all(item.obj, flag, item.item, item.inven,
									 item.slot, unpack(arg)) or ret
	end -- for i = getn(items), 1, -1 do

	return ret
end

function item_hooks.process_all_chain_floor_and_pack(y, x, flag,
													 chain_arg, ...)
	local items = item_hooks.collect_floor_and_pack(y, x, flag)

	for i = getn(items), 1, -1 do
		local item = items[i]

		chain_arg = item_hooks.process_all(item.obj, flag, chain_arg,
										   item.item, item.inven,
										   item.slot, unpack(arg))

		if not chain_arg then
			return nil
		end
	end -- for i = getn(items), 1, -1 do

	return chain_arg
end

function item_hooks.process_one_pack(flag, ...)
	local items = item_hooks.collect_pack(flag)

	for i = getn(items), 1, -1 do
		local item = items[i]
		local ret

		ret = item_hooks.process_one(item.obj, flag, item.item, item.inven,
									 item.slot, unpack(arg))

		if ret then
			return ret
		end
	end -- for i = getn(items), 1, -1 do
end

function item_hooks.process_all_pack(flag, ...)
	local items = item_hooks.collect_pack(flag)
	local ret

	for i = getn(items), 1, -1 do
		local item = items[i]

		ret = item_hooks.process_all(item.obj, flag, item.item, item.inven,
									 item.slot, unpack(arg)) or ret
	end -- for i = getn(items), 1, -1 do

	return ret
end

function item_hooks.process_one_standard(flag, ...)
	local flags_list = {player.descriptor.flags,
		player.body_monster.flags, level_flags.flags}

	for i = 1, getn(flags_list) do
		local flags = flags_list[i]

		if flags[flag] then
			local func = get_function_registry_from_flag(flags, flag)

			local ret= func(unpack(arg))

			if ret then
				return ret
			end
		end
	end

	local items = item_hooks.collect_equip(flag)
	for i = getn(items), 1, -1 do
		local item = items[i]
		local ret

		ret = item_hooks.process_one(item.obj, flag, unpack(arg))

		if ret then
			return ret
		end
	end -- for i = getn(items), 1, -1 do
end

function item_hooks.process_all_standard(flag, ...)
	local ret

	local flags_list = {player.descriptor.flags,
		player.body_monster.flags, level_flags.flags}

	for i = 1, getn(flags_list) do
		local flags = flags_list[i]

		if flags[flag] then
			local func = get_function_registry_from_flag(flags, flag)

			ret = func(unpack(arg)) or ret
		end
	end

	local items = item_hooks.collect_equip(flag)
	for i = getn(items), 1, -1 do
		local item = items[i]

		ret = item_hooks.process_all(item.obj, flag, unpack(arg)) or ret
	end -- for i = getn(items), 1, -1 do

	return ret
end

function item_hooks.process_all_chain_pack(flag, chain_arg, ...)
	local items = item_hooks.collect_pack(flag)

	for i = getn(items), 1, -1 do
		local item = items[i]

		chain_arg = item_hooks.process_all(item.obj, flag, chain_arg,
										   item.item, item.inven,
										   item.slot, unpack(arg))

		if not chain_arg then
			return nil
		end
	end -- for i = getn(items), 1, -1 do

	return chain_arg
end

----------------------------------------------------------------------------

new_flag("GET_PRE", true)
hook(hook.GET_PRE,
function(obj, item)
	return item_hooks.process_one(obj, FLAG_GET_PRE, item)
end)

new_flag("GET_POST", true)
hook(hook.GET_POST,
function(obj, item)
	return item_hooks.process_all(obj, FLAG_GET_POST, item)
end)


new_flag("DROP_PRE", true)
hook(hook.DROP_PRE,
function(obj, item)
	return item_hooks.process_one(obj, FLAG_DROP_PRE, item)
end)

new_flag("DROP_POST", true)
hook(hook.DROP_POST,
function(obj, item)
	return item_hooks.process_all(obj, FLAG_DROP_POST, item)
end)

new_flag("WIELD_PRE", true)
hook(hook.WIELD_PRE,
function(obj, item)
	return item_hooks.process_one(obj, FLAG_WIELD_PRE, item)
end)

new_flag("WIELD_POST", true)
hook(hook.WIELD_POST,
function(obj, item)
	return item_hooks.process_all(obj, FLAG_WIELD_POST, item)
end)

new_flag("TAKEOFF_PRE", true)
hook(hook.TAKEOFF_PRE,
function(obj, slot)
	return item_hooks.process_one(obj, FLAG_TAKEOFF_PRE, slot)
end)

new_flag("TAKEOFF_POST", true)
hook(hook.TAKEOFF_POST,
function(obj, item, amt, forced)
	return item_hooks.process_all(obj, FLAG_TAKEOFF_POST, item, amt, forced)
end)

new_flag("PLAYER_DESTROY_PRE", true)
hook(hook.PLAYER_DESTROY_PRE,
function(obj, amt)
	return item_hooks.process_one(obj, FLAG_PLAYER_DESTROY_PRE, amt)
end)

new_flag("PLAYER_DESTROY_POST", true)
hook(hook.PLAYER_DESTROY_POST,
function(obj, amt)
	return item_hooks.process_all(obj, FLAG_PLAYER_DESTROY_POST, amt)
end)

--
-- Inform object that it was identified, *AND* apply the automizer to
-- the object, since we might perform a new auto-rule on it now that
-- it's been identified
--
new_flag("IDENTIFY_POST", true)
hook(hook.IDENTIFY_POST,
function(obj, slot, type)
	local ret  = item_hooks.process_all(obj, FLAG_IDENTIFY_POST,
										obj, slot, type)

	-- If object is on the ground, the slot number should be negative
	if obj.iy > 0 and obj.ix > 0 and slot > 0 then
		slot = -slot
	end

	-- If object is on the ground, only apply the automizer if the
	-- player is standing on top of it
	if (obj.iy == 0 and obj.ix == 0) or (obj.iy == player.py and
					     obj.ix == player.px) then
		apply_rules(obj, slot)
	end

	return ret
end)

new_flag("OPEN_PRE", true)
hook(hook.OPEN_PRE,
function(y, x, dir_or_idx, obj)
	if obj then
		return item_hooks.process_one(obj, FLAG_OPEN_PRE, y, x, dir_or_idx)
	else
		local diry, dirx = explode_dir(dir_or_idx)
		local dy = y + diry
		local dx = x + dirx
		local feat = f_info[cave(dy, dx).feat + 1]

		if has_flag(feat, FLAG_OPEN_PRE) then
			local func =
				get_function_registry_from_flag(feat.flags,
							FLAG_OPEN_PRE)

			return func(y, x, dir_or_idx)
		end -- if has_flag(feat, FLAG_OPEN_PRE) then
	end -- else
end)

new_flag("OPEN_POST", true)
hook(hook.OPEN_POST,
function(y, x, dir_or_idx, obj)
	if obj then
		return item_hooks.process_all(obj, FLAG_OPEN_POST, dir_or_idx, y, x)
	else
		local diry, dirx = explode_dir(dir_or_idx)
		local dy = y + diry
		local dx = x + dirx
		local feat = f_info[cave(dy, dx).feat + 1]


		if has_flag(feat, FLAG_OPEN_POST) then
			local func =
				get_function_registry_from_flag(feat.flags,
							FLAG_OPEN_POST)

			return func(y, x, dir_or_idx)
		end -- if has_flag(feat, FLAG_OPEN_POST) then
	end -- else
end)

new_flag("CLOSE_PRE", true)
hook(hook.CLOSE_PRE,
function(y, x, dir_or_idx, obj)
	if obj then
		return item_hooks.process_one(obj, FLAG_CLOSE_PRE, y, x, dir_or_idx)
	else
		local diry, dirx = explode_dir(dir_or_idx)
		local dy = y + diry
		local dx = x + dirx
		local feat = f_info[cave(dy, dx).feat + 1]

		if has_flag(feat, FLAG_CLOSE_PRE) then
			local func =
				get_function_registry_from_flag(feat.flags,
							FLAG_CLOSE_PRE)

			return func(y, x, dir_or_idx)
		end -- if has_flag(feat, FLAG_CLOSE_PRE) then
	end -- else
end)

new_flag("CLOSE_POST", true)
hook(hook.CLOSE_POST,
function(y, x, dir_or_idx, obj)
	if obj then
		return item_hooks.process_all(obj, FLAG_CLOSE_POST, dir_or_idx, y, x)
	else
		local diry, dirx = explode_dir(dir_or_idx)
		local dy = y + diry
		local dx = x + dirx
		local feat = f_info[cave(dy, dx).feat + 1]


		if has_flag(feat, FLAG_CLOSE_POST) then
			local func =
				get_function_registry_from_flag(feat.flags,
							FLAG_CLOSE_POST)

			return func(y, x, dir_or_idx)
		end -- if has_flag(feat, FLAG_CLOSE_POST) then
	end -- else
end)

new_flag("OBJECT_VALUE", true)
hook(hook.OBJECT_VALUE,
function(obj, value)
	local new_val = item_hooks.process_all_chain(obj, FLAG_OBJECT_VALUE,
												 value)

	if not new_val then
		return false
	end

	new_val = item_hooks.object_value(obj, new_val)

	return true, new_val
end)
-- Placeholder function that the module can override
function item_hooks.object_value(obj, value)
	return value
end

----------------------------------------------------------------------------
----------------------------------------------------------------------------

-- Corpse hooks

-- Inform the *race* that a corpse is about to be created.
new_flag("CORPSE_CREATE_PRE")
hook(hook.CORPSE_CREATE_PRE,
function(r_idx)
	local race = r_info[r_idx + 1]

	if race.flags[FLAG_CORPSE_CREATE_PRE] then
		local func = get_function_registry_from_flag(race.flags,
							     FLAG_CORPSE_CREATE_PRE);
		return func()
	end
end)

-- Inform the *race* (and ego) that a corpse has been created, and
-- copy over any flags the monster wants applied to the coprse.
new_flag("CORPSE_FLAGS")
new_flag("CORPSE_CREATE_POST")
hook(hook.CORPSE_CREATE_POST,
function(obj, mon)
	if mon.flags[FLAG_CORPSE_FLAGS] then
		flag_add(obj.flags, mon.flags[FLAG_CORPSE_FLAGS])
	end

	local race = mon.sr_ptr or r_info(mon.r_idx)
	if race.flags[FLAG_CORPSE_CREATE_POST] then
		local func = get_function_registry_from_flag(race.flags,
							     FLAG_CORPSE_CREATE_POST);
		func(obj)
	end

	if mon.ego ~= 0 then
		local ego = re_info(mon.ego)

		if ego.flags[FLAG_CORPSE_CREATE_POST] then
			local func =
				get_function_registry_from_flag(ego.flags,
												FLAG_CORPSE_CREATE_POST)
			func(obj)
		end
	end

	-- Save original weight and decay values
	obj.flags[FLAG_ORIG_WEIGHT] = obj.weight
	obj.flags[FLAG_ORIG_DECAY]  = obj.flags[FLAG_MONSTER_DECAY]
end)

----------------------------------------------------------------------------
----------------------------------------------------------------------------

-- Player body hooks

new_flag("BODY_CHANGE_PRE", true)
hook(hook.BODY_CHANGE_PRE,
function(old_body, new_body)
	local func

	if old_body.flags[FLAG_BODY_CHANGE_PRE] then
		func = get_func_from_flag_or_registry(old_body.flags,
											  FLAG_BODY_CHANGE_PRE)
		if func(old_body, new_body, 1) then
			return true
		end
	end

	if new_body.flags[FLAG_BODY_CHANGE_PRE] then
		func = get_func_from_flag_or_registry(new_body.flags,
											  FLAG_BODY_CHANGE_PRE)
		if func(old_body, new_body, 2) then
			return true
		end
	end

	return item_hooks.process_one_standard(FLAG_BODY_CHANGE_PRE,
										  old_body, new_body)
end)

new_flag("BODY_CHANGE", true)
hook(hook.BODY_CHANGE,
function(old_body, new_body)
	local func

	if old_body.flags[FLAG_BODY_CHANGE] then
		func = get_func_from_flag_or_registry(old_body.flags,
											  FLAG_BODY_CHANGE)
		func(old_body, new_body, 1)
	end

	if new_body.flags[FLAG_BODY_CHANGE] then
		func = get_func_from_flag_or_registry(new_body.flags,
											  FLAG_BODY_CHANGE)
		func(old_body, new_body, 2)
	end

	item_hooks.process_all_standard(FLAG_BODY_CHANGE,
								   old_body, new_body)
end)

new_flag("BODY_CHANGE_POST", true)
hook(hook.BODY_CHANGE_POST,
function(old_body, new_body)
	local func

	if old_body.flags[FLAG_BODY_CHANGE_POST] then
		func = get_func_from_flag_or_registry(old_body.flags,
											  FLAG_BODY_CHANGE_POST)
		func(old_body, new_body, 1)
	end

	if new_body.flags[FLAG_BODY_CHANGE_POST] then
		func = get_func_from_flag_or_registry(new_body.flags,
											  FLAG_BODY_CHANGE_POST)
		func(old_body, new_body, 2)
	end

	item_hooks.process_all_standard(FLAG_BODY_CHANGE_POST,
								   old_body, new_body)
end)

----------------------------------------------------------------------------
----------------------------------------------------------------------------

-- Movement hooks

new_flag("MOVE_PRE_DIR", true)
hook(hook.MOVE_PRE_DIR,
function(dir, do_pickup, run, disarm)
	local c_ptr   = cave(player.py, player.px)
	local out_dir = nil
	local func

	if cave_feat_is(c_ptr, FLAG_MOVE_PRE_DIR) then
		func = get_function_registry_from_flag(f_info[c_ptr.feat + 1].flags,
											   FLAG_MOVE_PRE_DIR)

		out_dir = func(dir, do_pickup, run, disarm)
	end

	if c_ptr.flags[FLAG_MOVE_PRE_DIR] then
		func = get_function_from_flag(c_ptr.flags, FLAG_MOVE_PRE_DIR)
		out_dir = func(dir, do_pickup, run, disarm) or out_dir
	end

	out_dir =
		item_hooks.process_all_chain_floor_and_pack(player.py, player.px,
													FLAG_MOVE_PRE_DIR,
													out_dir)
	if out_dir then return true, out_dir end
end)

new_flag("MOVE_PRE_ORIG", true)
new_flag("MOVE_PRE_DEST", true)
hook(hook.MOVE_PRE,
function(y, x, dir, do_pickup, run, disarm)
	local orig = cave(player.py, player.px)
	local dest = cave(y, x)
	local func

	if cave_feat_is(orig, FLAG_MOVE_PRE_ORIG) then
		func = get_function_registry_from_flag(f_info[oirg.feat + 1].flags,
											   FLAG_MOVE_PRE_ORIG)

		if func(y, x, dir, do_pickup, run, disarm) then
			return true
		end
	end

	if orig.flags[FLAG_MOVE_PRE_ORIG] then
		func = get_function_from_flag(oirg.flags, FLAG_MOVE_PRE_ORIG)

		if func(y, x, dir, do_pickup, run, disarm) then
			return true
		end
	end

	if item_hooks.process_one_floor_and_pack(player.py, player.px,
											 FLAG_MOVE_PRE_ORIG,
											 y, x, dir, do_pickup, run,
											 disarm) then
		return true
	end

	if cave_feat_is(dest, FLAG_MOVE_PRE_DEST) then
		func = get_function_registry_from_flag(f_info[dest.feat + 1].flags,
											   FLAG_MOVE_PRE_DEST)

		if func(y, x, dir, do_pickup, run, disarm) then
			return true
		end
	end

	if dest.flags[FLAG_MOVE_PRE_DEST] then
		func = get_function_from_flag(dest.flags, FLAG_MOVE_PRE_DEST)

		if func(y, x, dir, do_pickup, run, disarm) then
			return true
		end
	end

	if item_hooks.process_one_floor_and_pack(y, x, FLAG_MOVE_PRE_DEST,
											 y, x, dir, do_pickup, run,
											 disarm) then
		return true
	end
end) -- MOVE_PRE

new_flag("AUTO_PICKUP")

new_flag("MOVE_POST_ORIG", true)
new_flag("MOVE_POST_DEST", true)
hook(hook.MOVE_POST,
function(oy, ox, dir, do_pickup, run, disarm)
	local orig = cave(oy, ox)
	local dest = cave(player.py, player.px)
	local func

	if cave_feat_is(orig, FLAG_MOVE_POST_ORIG) then
		func = get_function_registry_from_flag(f_info[orig.feat + 1].flags,
											   FLAG_MOVE_POST_ORIG)

		func(oy, ox, dir, do_pickup, run, disarm)
	end

	if orig.flags[FLAG_MOVE_POST_ORIG] then
		func = get_function_from_flag(orig.flags, FLAG_MOVE_POST_OIRG)
		func(oy, ox, dir, do_pickup, run, disarm)
	end

	item_hooks.process_all_floor_and_pack(oy, ox, FLAG_MOVE_POST_ORIG,
										  oy, ox, dir, do_pickup, run,
										  disarm)

	if cave_feat_is(dest, FLAG_MOVE_POST_DEST) then
		func = get_function_registry_from_flag(f_info[dest.feat + 1].flags,
											   FLAG_MOVE_POST_DEST)

		func(oy, ox, dir, do_pickup, run, disarm)
	end

	if dest.flags[FLAG_MOVE_POST_DEST] then
		func = get_function_from_flag(dest.flags, FLAG_MOVE_POST_DEST)
		func(oy, ox, dir, do_pickup, run, disarm)
	end

	item_hooks.process_all_floor_and_pack(player.py, player.px,
										  FLAG_MOVE_POST_DEST,
										  oy, ox, dir, do_pickup, run,
										  disarm)

	-- Automatically pick up objects with the AUTO_PICKUP flag.
	-- We don't want to do this inside of an object hook function,
	-- since picking up an object puts a copy of the object into
	-- the player's pack and then deletes the one on the ground,
	-- even though the one on the ground is still being processed
	-- by the object-hook invoking code.
	local items = {}
	for_inventory_inven(cave(player.py, player.px).inventory,
				  function(obj, item)
					  if obj.flags[FLAG_AUTO_PICKUP] then
						  tinsert(%items, {obj=obj, item=-item})
					  end
				  end)

	for i = getn(items), 1, -1 do
		if tag(items[i].obj.flags[FLAG_AUTO_PICKUP]) == TAG_STRING then
			message(items[i].obj.flags[FLAG_AUTO_PICKUP])
		end
		object_pickup_simple(items[i].obj, items[i].item)
	end

end) -- MOVE_POST

new_flag("STAIR_DO")
hook(hook.STAIR_DO,
function(updown, ask_leave)
	local c_ptr = cave(player.py, player.px)
	local func
	local ret

	if cave_feat_is(c_ptr, FLAG_STAIR_DO) then
		func = get_function_registry_from_flag(f_info[c_ptr.feat + 1].flags,
											   FLAG_STAIR_DO)

		ret = func(updown, ask_leave)

		if ret then
			return true, ret
		end
	end

	if c_ptr.flags[FLAG_STAIR_DO] then
		func = get_function_from_flag(c_ptr.flags, FLAG_STAIR_DO)
		ret = func(updown, ask_leave)

		if ret then
			return true, ret
		end
	end

	return false
end)

new_flag("STAIR_POST")
hook(hook.STAIR_POST,
function(updown)
	local flags = d_info[current_dungeon_idx + 1].flags

	if flags[FLAG_STAIR_POST] then
		local func
		func = get_function_registry_from_flag(flags,
											   FLAG_STAIR_POST)
		func(updown)
	end
end)

----------------------------------------------------------------------------
----------------------------------------------------------------------------

-- Level hooks

new_flag("GEN_LEVEL", true)
hook(hook.GEN_LEVEL,
function()
	if has_flag(level_flags, FLAG_GEN_LEVEL) then
		local func
		func = get_function_registry_from_flag(level_flags.flags,
											   FLAG_GEN_LEVEL)
		func()
	end
end)

global("prev_dungeon_idx", 0)

new_flag("NEW_LEVEL", true)
hook(hook.NEW_LEVEL,
function()
	if has_flag(level_flags, FLAG_NEW_LEVEL) then
		local func
		func = get_function_registry_from_flag(level_flags.flags,
											   FLAG_NEW_LEVEL)
		func()
	end

	if current_dungeon_idx ~= prev_dungeon_idx then
		hook.process(hook.LEFT_DUNGEON, prev_dungeon_idx)
	end

	prev_dungeon_idx = current_dungeon_idx
end)

new_flag("LEVEL_REGEN", true)
hook(hook.LEVEL_REGEN,
function()
	if has_flag(level_flags, FLAG_LEVEL_REGEN) then
		local func
		func = get_function_registry_from_flag(level_flags.flags,
											   FLAG_LEVEL_REGEN)
		func()
	end
end)

new_flag("LEVEL_END_GEN", true)
hook(hook.LEVEL_END_GEN,
function()
	if has_flag(level_flags, FLAG_LEVEL_END_GEN) then
		local func
		func = get_function_registry_from_flag(level_flags.flags,
											   FLAG_LEVEL_END_GEN)
		func()
	end

	item_hooks.process_all_pack(FLAG_LEVEL_END_GEN)
end)

new_flag("GEN_LEVEL_BEGIN", true)
hook(hook.GEN_LEVEL_BEGIN,
function()
	if has_flag(level_flags, FLAG_GEN_LEVEL_BEGIN) then
		local func
		func = get_function_registry_from_flag(level_flags.flags,
											   FLAG_GEN_LEVEL_BEGIN)
		func()
	end
end)

new_flag("LEVEL_LOOP", true)
hook(hook.LEVEL_LOOP,
function()
	if has_flag(level_flags, FLAG_LEVEL_LOOP) then
		local func
		func = get_function_registry_from_flag(level_flags.flags,
											   FLAG_LEVEL_LOOP)
		func()
	end
end)

----------------------------------------------------------------------------
----------------------------------------------------------------------------

-- Dungeon hooks
hook.new_hook_type("LEFT_DUNGEON")
new_flag("LEFT_DUNGEON")
hook(hook.LEFT_DUNGEON,
function()
	local flags = d_info[prev_dungeon_idx + 1].flags
	if flags[FLAG_LEFT_DUNGEON] then
		local func
		func = get_function_registry_from_flag(flags,
											   FLAG_LEFT_DUNGEON)
		func()
	end

	item_hooks.process_all_pack(FLAG_LEFT_DUNGEON)
end)

new_flag("ENTER_DUNGEON", true)
hook(hook.ENTER_DUNGEON,
function(dun_idx)
	if has_flag(level_flags, FLAG_ENTER_DUNGEON) then
		local func
		func = get_function_registry_from_flag(level_flags.flags,
											   FLAG_ENTER_DUNGEON)
		if func(dun_idx) then
			return true
		end
	end

	if dun_idx ~= current_dungeon_idx then
		local flags = d_info[dun_idx + 1].flags

		if flags[FLAG_ENTER_DUNGEON] then
			local func
			func = get_function_registry_from_flag(flags,
												   FLAG_ENTER_DUNGEON)
			if func(dun_idx) then
				return true
			end
		end
	end

	return item_hooks.process_one_pack(FLAG_ENTER_DUNGEON)
end)

new_flag("ENTERED_DUNGEON", true)
hook(hook.ENTERED_DUNGEON,
function(dun_idx)
	local flags = d_info[dun_idx + 1].flags
	if flags[FLAG_ENTERED_DUNGEON] then
		local func
		func = get_function_registry_from_flag(flags,
											   FLAG_ENTERED_DUNGEON)
		func(dun_idx)
	end

	item_hooks.process_all_pack(FLAG_ENTERED_DUNGEON, dun_idx)
end)
