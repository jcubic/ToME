--
-- The quaff/potions subsystem
--
-- Load it with: load_subsystem("quaff")
--
-- Parameters:
-- * no_key_bind: When set to true it will prevent any key bindings, so taht you can do your own

-- Load the generic object using subsystem
load_subsystem("use_object")

new_flag("PRE_QUAFF")
new_flag("ON_QUAFF")
new_flag("ON_SHATTER")
-- Beware this could be defined by the hunger subsystem
safe_new_flag("FOOD_VALUE")

constant("potions", {})

potions.item_tester = function(obj) return obj.flags[FLAG_ON_QUAFF] ~= nil end

-- Quaff a potion (from the pack or floor).
function potions.use(item)
	if item and not potions.item_tester(get_object(item), item) then return use_object.CANNOT_USE end

	-- Set up the extra finder
--DGDGDGDG	get_item_hook_find_obj_what = "Scroll full name? "
--	get_item_extra_hook = get_item_hook_find_obj

	-- Get an item
	if not item then
		local ret
		ret, item = get_item("Quaff which potion? ", "You have nothing to quaff.", USE_INVEN | USE_FLOOR,
		   potions.item_tester,
		   'q'
		)
		if not ret then return end
	end

	-- Get the item
	local obj = get_object(item)

	if check_prevent_cmd(obj, 'q') then
		return
	end

	potions.use_aux(obj)

	-- Destroy a potion in the pack or on the floor
	item_increase(item, -1)
	item_describe(item)
	item_optimize(item)
end

-- Core portion of potions.use() split off, so that things like
-- quaffing from fountains can re-use it
function potions.use_aux(obj)
	local func

	if obj.flags[FLAG_PRE_QUAFF] then
		-- Check if there is an embeded lua bytecode
		func = get_function_registry_from_flag(obj.flags, FLAG_PRE_QUAFF)

		if func and func(obj) then
			-- Object disallowed being quaffed
			return
		end
	end

	if item_hooks.process_one_standard(FLAG_PRE_QUAFF, obj) then
		return
	end

	-- Take a turn
	energy_use = get_player_energy(SPEED_QUAFF)

	-- Not identified yet
	local ident = nil

	-- Object level
	local lev = k_info[obj.k_idx].level

	-- Assume the scroll will get used up
	local used_up = true

	-- Check if there is an embeded lua bytecode
	func = get_function_registry_from_flag(obj.flags, FLAG_ON_QUAFF)
	ident = func(obj)

	item_hooks.process_all_standard(FLAG_ON_QUAFF, obj)

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

	if has_subsystem("hunger") then
		timed_effect.inc(timed_effect.FOOD, get_flag(obj, FLAG_FOOD_VALUE))
	end
end


-- Init
if not get_subsystem_param("quaff", "no_key_bind") then
	hook(hook.KEYPRESS, function (key)
		if key == strbyte('q') then potions.use() return true end
	end)
end

-- Test all
function potions.test_all()
	for i = 1, max_k_idx do
		local item = k_info[i]
		if item.flags[FLAG_ON_QUAFF] then
			print(get_function_registry_from_flag(item.flags, FLAG_ON_QUAFF)(item))
		end
	end
end
