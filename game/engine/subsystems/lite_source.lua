--
-- The fuelable lites subsystem
--
-- Load it with: load_subsystem("lite_source")
--
-- Parameters:
-- * no_key_bind: When set to true it will prevent any key bindings, so taht you can do your own
-- * lite_slot: Lite equipment slot

-- Load the generic object using subsystem
load_subsystem("use_object")

constant("lite_sources", {})

lite_sources.slot = get_subsystem_param("lite_source", "lite_slot")

new_flag("FUEL_SOURCE")
new_flag("FUEL_MAX")
new_flag("ON_FUEL")

lite_sources.item_tester = function(obj)
	local lite = player.inventory[lite_sources.slot][1]
	if not lite or not lite.flags[FLAG_FUEL_LITE] then return false end
	return obj.flags[FLAG_FUEL_SOURCE] == lite.flags[FLAG_FUEL_LITE]
end

-- Refill a lite source
function lite_sources.refill(item)
	if item and not lite_sources.item_tester(get_object(item), item) then return use_object.CANNOT_USE end

	if player.has_intrinsic(FLAG_CONFUSED) then
		message("You are too confused!")
		return
	end

	-- Get an item
	if not item then
		local ret
		ret, item = get_item("Refill your light source with what? ", "You have nothing to refill your light source with.", USE_INVEN | USE_FLOOR,
		   lite_sources.item_tester,
		   'r'
		)
		if not ret then return end
	end

	-- Get the item
	local obj = get_object(item)

	if check_prevent_cmd(obj, 'F') then
		return
	end

	-- Check for special actions
	local lite = player.inventory[lite_sources.slot][1]

	if obj.flags[FLAG_ON_FUEL] then
		get_function_registry_from_flag(obj.flags, FLAG_ON_FUEL)(lite, obj)
	end

	if lite.flags[FLAG_ON_FUEL] then
		get_function_registry_from_flag(lite.flags, FLAG_ON_FUEL)(lite, obj)
	end

	-- Refuel
	if lite.flags[FLAG_FUEL] and obj.flags[FLAG_FUEL] then
		message("You fuel your " .. object_desc(lite) .. ".")
		lite.flags[FLAG_FUEL] = lite.flags[FLAG_FUEL] + obj.flags[FLAG_FUEL]
		if lite.flags[FLAG_FUEL_MAX] and lite.flags[FLAG_FUEL] > lite.flags[FLAG_FUEL_MAX] then
			lite.flags[FLAG_FUEL] = lite.flags[FLAG_FUEL_MAX]
		end
	end

	-- Take a turn
	energy_use = get_player_energy(SPEED_GLOBAL)

	-- Combine / Reorder the pack (later)
	player.notice = player.notice | PN_COMBINE | PN_REORDER

	-- Window stuff
	player.window[FLAG_PW_INVEN] = true
	player.window[FLAG_PW_EQUIP] = true
	player.window[FLAG_PW_PLAYER] = true

	-- Destroy a scroll in the pack
	item_increase(item, -1)
	item_describe(item)
	item_optimize(item)
end

-- Init
if not get_subsystem_param("lite_source", "no_key_bind") then
	hook(hook.KEYPRESS, function (key)
		if key == strbyte('F') then lite_sources.refill() return true end
	end)
end

