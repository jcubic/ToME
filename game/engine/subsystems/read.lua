--
-- The read/scrolls subsystem
--
-- Load it with: load_subsystem("read")
--
-- Parameters:
-- * no_key_bind: When set to true it will prevent any key bindings, so taht you can do your own

-- Load the generic object using subsystem
load_subsystem("use_object")

new_flag("PRE_READ")
new_flag("ON_READ")

constant("scrolls", {})

scrolls.item_tester = function(obj) return obj.flags[FLAG_ON_READ] ~= nil end

-- Read a scroll (from the pack or floor).
function scrolls.use(item)
	if item and not scrolls.item_tester(get_object(item), item) then return use_object.CANNOT_USE end

	-- Check some conditions
	if player.has_intrinsic(FLAG_BLIND) then
		message("You can't see anything.")
		return
	end
	if no_lite() then
		message("You have no light to read by.")
		return
	end
	if player.has_intrinsic(FLAG_CONFUSED) then
		message("You are too confused!")
		return
	end

	-- Set up the extra finder
--DGDGDGDG	get_item_hook_find_obj_what = "Scroll full name? "
--	get_item_extra_hook = get_item_hook_find_obj

	-- Get an item
	if not item then
		local ret
		ret, item = get_item("Read which scroll? ", "You have nothing to read.", USE_INVEN | USE_FLOOR,
		   scrolls.item_tester,
		   'r'
		)
		if not ret then return end
	end

	-- Get the item
	local obj = get_object(item)

	if check_prevent_cmd(obj, 'r') then
		return
	end

	local func

	if obj.flags[FLAG_PRE_READ] then
		-- Check if there is an embeded lua bytecode
		func = get_function_registry_from_flag(obj.flags, FLAG_PRE_READ)

		if func and func(obj) then
			-- Object disallowed being read
			return
		end
	end

	if item_hooks.process_one_standard(FLAG_PRE_READ, obj) then
		return
	end

	-- Take a turn
	energy_use = get_player_energy(SPEED_READ)

	-- Not identified yet
	local ident = nil

	-- Object level
	local lev = k_info[obj.k_idx].level

	-- Assume the scroll will get used up
	local used_up = true

	-- Check if there is an embeded lua bytecode
	func = get_function_registry_from_flag(obj.flags, FLAG_ON_READ)
	used_up, ident = func(obj)

	item_hooks.process_all_standard(FLAG_ON_READ, obj)

	-- Combine / Reorder the pack (later)
	player.notice = player.notice | PN_COMBINE | PN_REORDER

	-- The item was tried
	set_tried(obj)

	-- An identification was made
	if ident and not is_aware(obj) then
		set_aware(obj)
		gain_exp((lev + (player.lev / 4)) / player.lev)
	end

	-- Window stuff
	player.window[FLAG_PW_INVEN] = true
	player.window[FLAG_PW_EQUIP] = true
	player.window[FLAG_PW_PLAYER] = true

	-- Allow certain scrolls to be "preserved"
	if not used_up then return end

	-- Destroy a scroll in the pack
	item_increase(item, -1)
	item_describe(item)
	item_optimize(item)
end

-- Init
if not get_subsystem_param("read", "no_key_bind") then
	hook(hook.KEYPRESS, function (key)
		if key == strbyte('r') then scrolls.use() return true end
	end)
end

