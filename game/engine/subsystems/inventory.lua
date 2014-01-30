-- The inventory subsystem
--
-- Load it with: load_subsystem("inventory")
--
-- This is the "default" inventory system as it exists for ToME
-- This will handle wielding, takeoff, display, destruction and so on
--
-- Parameters:
-- * no_key_bind: When set to true it will prevent any key bindings, so taht you can do your own
-- * show_action: SPecial function to call when showing an item

-- Load the generic object using subsystem
load_subsystem("use_object")

constant("inventory", {})

inventory.show_action_fct = get_subsystem_param("inventory", "show_action") or function() return false end

inventory.wield_item_tester = function(obj, item)
	-- Only takeoff wielded stuff
	local inven_nb = item_slot_to_inven(item)
	if inven_nb >= INVEN_PACK then return false end

	local slot = wield_slot(obj)

	-- Check for a usable slot
	if item_slot_to_inven(slot) >= INVEN_PACK then return true end

	-- Assume not wearable
	return false
end

-- Wield or wear a single item from the pack or floor
function inventory.wield(item, silent)
	if item and not inventory.wield_item_tester(get_object(item), item) then return use_object.CANNOT_USE end

	local slot, inven
	local num = 1
	local q_ptr
	local obj
	local act
	local o_name

	-- Get an item
	if not item then
		local ret
		ret, item = get_item("Wear/Wield which item? ", "You have nothing you can wear or wield.", USE_INVEN | USE_FLOOR, inventory.wield_item_tester)
		if not ret then return end
	end

	-- Get the item
	obj = get_object(item)

	-- Check inscription to see if the command command is prevented
	if check_prevent_cmd(obj) then return end

	-- Check the slot
	slot = wield_slot(obj)
	inven = item_slot_to_inven(slot)

	-- Prevent wielding into a cursed slot
	if get_object(slot) and is_cursed(get_object(slot)) then
		if not silent then message(format("The %s you are %s appears to be cursed.", object_desc(get_object(slot)), describe_use(slot))) end

		-- Cancel the command
		return
	end

	if is_cursed(obj) and game_options.wear_confirm and (is_known(obj) or (obj.ident & (IDENT_SENSE) ~= 0)) then
		if silent or not get_check("Really use the "..object_desc(obj).." {cursed}? ") then return end
	end

	local ret, fnum, fslot = hook.process(hook.WIELD_INFO, obj, item, slot)
	if ret then
		num = fnum
		slot = fslot
	end

	-- Can we wield?
	if hook.process(hook.WIELD_PRE, obj, item, slot) then return end

	if get_object(slot) then
		-- Can we take it off
		if hook.process(hook.TAKEOFF_PRE, obj, slot) then return end
	end

	-- Take a turn
	energy_use = get_player_energy(SPEED_INVEN)

	-- Get local object
	q_ptr = new_object()

	-- Obtain local object
	object_copy(q_ptr, obj)

	-- Modify quantity
	q_ptr.number = num

	-- Decrease the item (from the pack)
	item_increase(item, -num)
	item_optimize(item)

	-- Access the wield slot
	obj = get_object(slot)

	-- Take off existing item
	if obj then
		-- Take off existing item
		inven_takeoff(slot, 255, false)
	end

	set_object(slot, q_ptr)
	obj = q_ptr

	-- Where is the item now
	act = inventory_slot_names[1+inven].wear

	-- Message
	if not silent then message(format("%s %s (%c).", act, object_desc(obj, true, 3), index_to_label(slot))) end

	-- Cursed!
	if is_cursed(obj) then
		-- Warn the player
		if not silent then message("Oops! It feels deathly cold!") end

		-- Note the curse
		obj.ident = obj.ident | IDENT_SENSE
		obj.sense = SENSE_CURSED
	end

	-- Take care of item sets
	-- DGDGDGDG -- Yes indeed, please do it

	-- Inform scripts
	hook.process(hook.WIELD_POST, obj, item, slot)

	-- Recalculate bonuses
	player.update = player.update | (PU_BONUS | PU_BODY | PU_TORCH | PU_HP | PU_MANA | PU_SPELLS)

	-- Redraw monster hitpoint
	player.redraw[FLAG_PR_MH] = true

	player.window[FLAG_PW_INVEN] = true
	player.window[FLAG_PW_EQUIP] = true
	player.window[FLAG_PW_PLAYER] = true
end


inventory.takeoff_item_tester = function(obj, item)
	-- Only takeoff wielded stuff
	local inven_nb = item_slot_to_inven(item)
	return (inven_nb >= INVEN_PACK)
end

-- Takeoff
function inventory.takeoff(item, silent)
	if item and not inventory.takeoff_item_tester(get_object(item), item) then return use_object.CANNOT_USE end

	local obj

	-- Get an item
	if not item then
		local ret
		ret, item = get_item("Take off which item? ", "You are not wearing anything to take off.", USE_EQUIP, inventory.takeoff_item_tester)
		if not ret then return end
	end

	obj = get_object(item)

	-- Item is cursed
	if is_cursed(obj) and not wizard then
		-- Oops
		if not silent then message("Hmmm, it seems to be cursed.") end

		-- Nope
		return
	end

	-- Take a partial turn
	energy_use = get_player_energy(SPEED_INVEN) / 2

	-- Take off the item
	inven_takeoff(item, 255, false)

	-- Recalculate bonuses
	player.update = player.update | (PU_BONUS | PU_BODY | PU_TORCH | PU_HP | PU_MANA | PU_SPELLS)

	-- Redraw monster hitpoint
	player.redraw[FLAG_PR_MH] = true

	player.window[FLAG_PW_INVEN] = true
	player.window[FLAG_PW_EQUIP] = true
	player.window[FLAG_PW_PLAYER] = true
end

inventory.drop_item_tester = function(obj, item)
	return true
end

-- Handle an object which the player couldn't destroy
function inventory.cant_destroy(obj, silent)
	if not has_flag(obj, FLAG_COULDNT_DESTROY) then
		obj.flags[FLAG_COULDNT_DESTROY] = true
		-- Combine the pack
		player.notice = player.notice | (PN_COMBINE)

		-- Window stuff
		player.window[FLAG_PW_INVEN] = true
		player.window[FLAG_PW_EQUIP] = true
	end

	local name = object_desc(obj, true, 3)

	-- Message
	if not silent then
		msg_format("You cannot destroy %s.", name)
	end
end

-- Drop
function inventory.drop(item, silent)
	if item and not inventory.drop_item_tester(get_object(item), item) then return use_object.CANNOT_USE end

	local amt = 1
	local obj

	-- Get an item
	if not item then
		local ret
		ret, item = get_item("Drop which item? ", "You have nothing to drop.", (USE_EQUIP | USE_INVEN), inventory.drop_item_tester)
		if not ret then return end
	end

	-- Get the item (in the pack)
	obj = get_object(item)

	-- Check inscription to see if the command command is prevented
	if check_prevent_cmd(obj) then return end

	-- Hack -- Cannot remove cursed items
	if is_cursed(obj) then
		if item >= INVEN_PACK then
			-- Oops
			if not silent then message("Hmmm, it seems to be cursed.") end

			-- Nope
			return
		else
			if has_flag(obj, FLAG_CURSE_NO_DROP) then
				inventory.cant_destroy(obj, silent)

				-- Nope
				return
			end
		end
	end


	-- See how many items
	if obj.number > 1 then
		-- Get a quantity
		amt = get_quantity(nil, obj.number)

		-- Allow user abort
		if amt <= 0 then return end
	end

	-- Take a partial turn
	energy_use = get_player_energy(SPEED_INVEN) / 2

	inven_drop(item, amt, player.py, player.px, silent)
end

inventory.destroy_item_tester = function(obj, item)
	-- Dont destroy wielded stuff
	local inven_nb = item_slot_to_inven(item)
	return (inven_nb < INVEN_PACK)
end

-- Destroy an item
function inventory.destroy(item, silent)
	if item and not inventory.destroy_item_tester(get_object(item), item) then return use_object.CANNOT_USE end

	local amt = 1
	local old_number
	local force = false
	local obj, q_ptr

	-- Hack -- force destruction
	if command_arg > 0 then force = true end

	-- Get an item
	if not item then
		local ret
		ret, item = get_item("Destroy which item? ", "You have nothing to destroy.", (USE_FLOOR | USE_INVEN), inventory.destroy_item_tester)
		if not ret then return end
	end

	-- Get the item (in the pack)
	obj = get_object(item)

	-- Check inscription to see if the command command is prevented
	if check_prevent_cmd(obj) then return end

	-- See how many items
	if obj.number > 1 then
		-- Get a quantity
		amt = get_quantity(nil, obj.number)

		-- Allow user abort
		if amt <= 0 then return end
	end

	-- Describe the object
	old_number = obj.number
	obj.number = amt
	local o_name = object_desc(obj, true, 3)
	obj.number = old_number

	-- Allow hooks to alter (or supress) the destruction message.
	inventory.destroy_msg = "You destroy "..o_name..". "

	-- Verify unless quantity given
	if not force then
		if not (game_options.auto_destroy and object_value(obj) < 1) then
			-- Make a verification
			if not get_check("Really destroy "..o_name.."? ") then return end
		end
	end

	-- Check script: can we destroy it?
	if hook.process(hook.PLAYER_DESTROY_PRE, obj, amt) then return end

	-- Do not take a turn to make manual destruction equal with the automatizer
	energy_use = 0

	if has_flag(obj, FLAG_CURSE_NO_DROP) and is_cursed(obj) then
		inventory.cant_destroy(obj, silent)

		-- Nope
		return
	end

	-- Artifacts cannot be destroyed
	if is_artifact(obj) then
		energy_use = 0

		inventory.cant_destroy(obj, silent)

		return
	end

	-- Inform script that object is destroyed
	hook.process(hook.PLAYER_DESTROY_POST, obj, amt)

	-- Message
	if not silent and (inventory.destroy_msg and
					   inventory.destroy_msg ~= "")
	then
		message(inventory.destroy_msg)
	end

	-- Get local object just for the unabsorb code
	-- Is there a better way?
	q_ptr = object_dup(obj)

	-- Set the amount
	q_ptr.number = amt

	-- Special unabsorb handling
	object_unabsorb(obj,q_ptr)

	-- Now delete it (yes, it is silly...)
	delete_object(q_ptr)

	-- Eliminate the item (from the pack)
	item_increase(item, -amt)
	item_describe(item)
	item_optimize(item)
end

inventory.examine_item_tester = function(obj, item)
	return true
end

-- Examine an item
function inventory.examine(item)
	if item and not inventory.examine_item_tester(get_object(item), item) then
		return use_object.CANNOT_USE
	end

	local obj

	-- Get an item
	if not item then
		local ret
		ret, item = get_item("Examine which item? ",
							 "You have nothing to examine.",
							 (USE_FLOOR | USE_INVEN | USE_EQUIP),
							 inventory.examine_item_tester)
		if not ret then return end
	end

	-- Get the item (in the pack)
	obj = get_object(item)

	-- Describe
	message(color.LIGHT_BLUE, object_desc(obj, true, 3))

	-- Describe it fully
	if not object_out_desc(obj, nil, false, true) then
		message("You see nothing special.")
	end
end

-- Display inventory
function inventory.show_inven()
	local total_weight = calc_total_weight()
	local ret, item
	local obj

	-- Call upon the auto_id subsystem, if loaded
	if has_subsystem("auto_id") then auto_id.do_inventory(player) end

	-- Get an item
	ret, item = get_item(format("Carrying %d.%d pounds (%d%% of capacity)", total_weight / 10, imod(total_weight, 10), (total_weight * 100) / ((player.weight_limit()) / 2)), "You have nothing in your inventory.", (USE_INVEN | USE_DISPLAY | USE_SHOW_NO_ITEM))
	if not ret then return end

	-- Get the item (in the pack)
	obj = get_object(item)

	if not inventory.show_action_fct(item, obj) then inventory.examine(item) end
end


-- Display equipment
function inventory.show_equip()
	local total_weight = calc_total_weight()
	local ret, item
	local obj

	-- Call upon the auto_id subsystem, if loaded
	if has_subsystem("auto_id") then auto_id.do_inventory(player) end

	-- Get an item
	ret, item = get_item(format("Carrying %d.%d pounds (%d%% of capacity)", total_weight / 10, imod(total_weight, 10), (total_weight * 100) / ((player.weight_limit()) / 2)), "You have no body parts onto which you could put equipment.", (USE_EQUIP | USE_DISPLAY | USE_SHOW_NO_ITEM))
	if not ret then return end

	-- Get the item (in the pack)
	obj = get_object(item)

	if not inventory.show_action_fct(item, obj) then inventory.examine(item) end
end

-- Init
if not get_subsystem_param("inventory", "no_key_bind") then
	hook(hook.KEYPRESS, function (key)
		if key == strbyte('I') then inventory.examine() return true end
		if key == strbyte('i') then inventory.show_inven() return true end
		if key == strbyte('e') then inventory.show_equip() return true end
		if key == strbyte('w') then inventory.wield() return true end
		if key == strbyte('t') then inventory.takeoff() return true end
		if key == strbyte('d') then inventory.drop() return true end
		if key == strbyte('k') then inventory.destroy() return true end
	end)
end

windows.type_define
{
	flag = FLAG_PW_INVEN
	name = "Inventory"
	display = function()
		local y, item_nbs, first, last, last_displayed, key_table = __core_objects.display_inventory
		(
			player.inventory,
			INVEN_INVEN, INVEN_INVEN,
			nil, true,
			nil, nil,
			false, 0, false, true
		)
	end
}
windows.type_define
{
	flag = FLAG_PW_EQUIP
	name = "Equipment"
	display = function()
		local y, item_nbs, first, last, last_displayed, key_table = __core_objects.display_inventory
		(
			player.inventory,
			INVEN_PACK, INVEN_TOTAL - 1,
			nil, true,
			nil, nil,
			false, 0, false, true
		)
	end
}
