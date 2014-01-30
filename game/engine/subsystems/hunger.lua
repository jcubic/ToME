-- The food subsystem
--
-- Load it with: load_subsystem("hunger")
--
-- The intrinsic FLAG_FOOD_VALUE must be set to the amount of
-- food to be consumed per turn(default should be 10)
--
-- Parameters:
-- * no_key_bind: When set to true it will prevent any key bindings, so taht you can do your own
-- * on_gorged: hook.CALC_BONUS called when the player is gorged

-- Load the generic object using subsystem
load_subsystem("use_object")

constant("food", {})

new_flag("EAT_PRE")
new_flag("ON_EAT")
new_flag("CORPSE_EAT_PRE")
new_flag("CORPSE_EAT_MSG")
new_flag("ON_CORPSE_EAT")
safe_new_flag("FOOD_VALUE")

food.FOOD_HARD_MAX = 20000 -- Maximal counter value
food.FOOD_MAX = 15000	-- Food value (Bloated)
food.FOOD_FULL = 10000	-- Food value (Normal)
food.FOOD_ALERT = 2000	-- Food value (Hungry)
food.FOOD_WEAK = 1000	-- Food value (Weak)
food.FOOD_FAINT = 500	-- Food value (Fainting)
food.FOOD_STARVE = 100	-- Food value (Starving)
food.DEFAULT_VALUE = food.FOOD_MAX

food.messages_up =
{
	[1] = "You are still weak."
	[2] = "You are still hungry."
	[3] = "You are no longer hungry."
	[4] = "You are full!"
	[5] = "You have gorged yourself!"
}
food.messages_down =
{
	[0] = "You are getting faint from hunger!"
	[1] = "You are getting weak from hunger!"
	[2] = "You are getting hungry."
	[3] = "You are no longer full."
	[4] = "You are no longer gorged."
}

food.on_gorged = get_subsystem_param("hunger", "on_gorged") or function()
	player.inc_speed(-10)
	-- Digest faster
	player.inc_intrinsic(FLAG_FOOD_VALUE, 100)
end


-- Food, food .. ahhh food..
-- Hunger is just a timed effect
-- except it starts filled up
timed_effect.create
{
	name = "FOOD"
	desc = "Hunger"
	type = "physical"
	status = "hidden"
	parameters = {}
	redraw = FLAG_PR_HUNGER
	amount_from_intrinsic = FLAG_FOOD_VALUE
	max_value = food.FOOD_HARD_MAX
	on_change = function(new_val)
		local old_aux, new_aux
		local notice = false
		local cur_val = timed_effect.get(timed_effect.FOOD) or 0

		-- Fainting / Starving
		if (cur_val < food.FOOD_FAINT) then
			old_aux = 0
		-- Weak
		elseif (cur_val < food.FOOD_WEAK) then
			old_aux = 1
		-- Hungry
		elseif (cur_val < food.FOOD_ALERT) then
			old_aux = 2
		-- Normal
		elseif (cur_val < food.FOOD_FULL) then
			old_aux = 3
		-- Full
		elseif (cur_val < food.FOOD_MAX) then
			old_aux = 4
		-- Gorged
		else
			old_aux = 5
		end

		-- Fainting / Starving
		if (new_val < food.FOOD_FAINT) then
			new_aux = 0
		-- Weak
		elseif (new_val < food.FOOD_WEAK) then
			new_aux = 1
		-- Hungry
		elseif (new_val < food.FOOD_ALERT) then
			new_aux = 2
		-- Normal
		elseif (new_val < food.FOOD_FULL) then
			new_aux = 3
		-- Full
		elseif (new_val < food.FOOD_MAX) then
			new_aux = 4
		-- Gorged
		else
			new_aux = 5
		end

		-- Food increase
		if (new_aux > old_aux) then
			-- Describe the state
			message(food.messages_up[new_aux])
			-- Change
			notice = true
			player.redraw[FLAG_PR_HUNGER] = true

		-- Food decrease
		elseif (new_aux < old_aux) then
			-- Disturb when hunger state decreased
			term.disturb(0, true)

			-- Describe the state
			message(food.messages_down[new_aux])
			-- Change
			notice = true
			player.redraw[FLAG_PR_HUNGER] = true

			-- Knock the player out of travel mode if weak
			if new_val < food.FOOD_WEAK and player.wild_mode then
				player.wilderness_x = player.px
				player.wilderness_y = player.py
				change_wild_mode()
			end
		end

		-- Starve to death (slowly)
		if new_val < food.FOOD_STARVE then
			-- Take damage
			take_hit((food.FOOD_STARVE - new_val) / 10, "starvation")
			term.disturb(true, true)
		end

		-- Do not go below 1
		if new_val < 1 then timed_effect(timed_effect.FOOD, 1 + (cur_val - new_val)) end
	end
	on_reset = function()
		timed_effect(timed_effect.FOOD, food.DEFAULT_VALUE - 1)
	end
	bonus = function()
		if timed_effect.get() > food.FOOD_MAX then
			food.on_gorged()
		end
	end
}

food.item_tester = function(obj) return obj.flags[FLAG_ON_EAT] ~= nil end

-- Eat something (from the pack or floor).
function food.use(item)
	if item and not potions.item_tester(get_object(item), item) then return use_object.CANNOT_USE end

	if not item then
		-- Get an item
		local ret
		get_item_extra_hook_lua = default_item_extra_hook
		ret, item = get_item("Eat what?", "You have nothing to eat.",
							 USE_INVEN | USE_FLOOR | USE_EXTRA,
							 food.item_tester, "E")
		if not ret then return end
	end

	-- Get the item
	local obj = get_object(item)

	if check_prevent_cmd(obj, 'E') then
		return
	end

	local func

	if obj.flags[FLAG_EAT_PRE] then
		-- Check if there is an embeded lua bytecode
		func = get_function_registry_from_flag(obj.flags, FLAG_PRE_EAT)

		if func and func(obj) then
			-- Object disallowed being eaten
			return
		end
	end

	if item_hooks.process_one_standard(FLAG_EAT_PRE, obj) then
		return
	end

	-- Take a turn
	energy_use = get_player_energy(SPEED_EAT)

	-- Object level
	local lev

	if obj.artifact_id > 0 then
		lev = a_info[obj.artifact_id + 1].level
	else
		lev = k_info[obj.k_idx + 1].level
	end

	-- Not identified yet
	local ident   = nil
	local used_up = true

	-- Check if there is an embeded lua bytecode

	if obj.flags[FLAG_FOOD_VALUE] then
		func = get_function_registry_from_flag(obj.flags, FLAG_ON_EAT)
		used_up, ident = func(obj)
	else
		-- The ON_EAT flag is the food value, so just use the default
		-- eating function
		local food_value = get_flag(obj, FLAG_ON_EAT)
		food.eat_good(obj, food_value)
	end

	item_hooks.process_all_standard(FLAG_ON_EAT, obj)

	-- Combine / Reorder the pack (later)
	player.notice = player.notice | PN_COMBINE | PN_REORDER

	-- The item was tried
	if not obj.artifact_id then
		set_tried(obj)
	end

	-- An identification was made
	if ident and not is_aware(obj) then
		set_aware(obj)
		gain_exp((lev + (player.lev / 4)) / player.lev)
	end

	-- Window stuff
	player.window[FLAG_PW_INVEN] = true
	player.window[FLAG_PW_EQUIP] = true
	player.window[FLAG_PW_PLAYER] = true

	if not used_up then
		-- Some things, like corpses, can't be consumed in just one
		-- sitting
		return
	end

	-- Destroy a food item in the pack
	item_increase(item, -1)
	item_describe(item)
	item_optimize(item)
end

-- Init
if not get_subsystem_param("hunger", "no_key_bind") then
	hook(hook.KEYPRESS, function (key)
		if key == strbyte('E') then food.use() return true end
	end)
end

-- ON_EAT convenience functions
function food.eat_good(obj, food_value)
	-- Only eating one of them, so temporarily set the number of objects
	-- to one to get the correct pluralization
	local old_num = obj.number
	obj.number = 1

	if not food_value then
		food_value = get_flag(obj, FLAG_FOOD_VALUE)
	end

	message("The " .. object_desc(obj) .. " was good.")
	timed_effect.inc(timed_effect.FOOD, food_value)
	obj.number = old_num

	return true, nil
end

function food.eat_bland(obj, food_value)
	-- Only eating one of them, so temporarily set the number of objects
	-- to one to get the correct pluralization
	local old_num = obj.number
	obj.number = 1

	if not food_value then
		food_value = get_flag(obj, FLAG_FOOD_VALUE)
	end

	message("The " .. object_desc(obj) .. " was bland.")
	timed_effect.inc(timed_effect.FOOD, food_value)
	obj.number = old_num

	return true, nil
end

function food.flag_key_equal(f1, f2, key)
	if not (flag_exists(f1, key) or flag_exists(f2, key)) then
		return true
	end

	if (not flag_exists(f1, key)) or (not flag_exists(f2, key)) then
		return false
	end

	if flag_get2(f1, key) ~= 0 or flag_get2(f2, key) ~= 0 then
		if flag_get2(f1, key) ~= flag_get2(f2, key) then
			return false
		end

		return (flag_get(f1, key) == flag_get(f2, key))
	end

	return (f1[key] == f2[key])
end -- food.flag_key_equal()

-- FLAG_ON_CORPSE_EAT and FLAG_CORPSE_EAT_PRE might be same between, say,
-- monster and race if it was simply copied over on monster creation from
-- the race flags, but different if something changed the monster's flag
-- after it was created.  Make sure the same function isn't used twice.
function food.get_func_list(source_list, key, default_func)
	local func_list = {}

	for i = getn(source_list), 1, -1 do
		local flags = source_list[i]
		local same = false
		for j = i - 1, 1, -1 do
			if food.flag_key_equal(flags, source_list[j], key)
			then
				same = true
				break
			end
		end -- for j = i - 1, 1, -1 do
		if not same and flags[key] then
			local func
			func = get_func_from_flag_or_registry(flags, key)
			tinsert(func_list, 1, func)
		end
	end -- for i = getn(source_list), 1, -1 do

	if default_func then
		tinsert(func_list, default_func)
	end

	return func_list
end

-- This can be overridden by the module, if the module wants to have
-- a default way of preventing corpses from being eaten (or cause
-- effects for event *attempting* to eat a corpse) without having
-- to define a CORPSE_EAT_PRE function for every race and ego.
function food.default_eat_corpse_pre(obj)
end

-- This can be overridden by the module, if the module wants to have
-- any default effects for corpse eating without having to define
-- an ON_CORPSE_EAT function for every race and ego.
function food.default_eat_corpse_effect(obj)
end

function food.eat_corpse(obj)
	if obj.tval ~= TV_CORPSE then
		message(color.VIOLET, "*ERROR* food.eat_corpse() being called on " ..
				"non-corpse object '" .. object_desc(obj) .. "'")
		return
	end

	local monst = obj.flags[FLAG_MONSTER_OBJ]

	local race, ego
	if monst then
		if monst.sr_ptr then
			race = monst.sr_ptr
		else
			race = r_info(monst.r_idx)
		end

		if monst.ego ~= 0 then
			ego = re_info(monst.ego)
		end
	end

	if obj.sval == SV_CORPSE_CORPSE or obj.sval == SV_CORPSE_SKELETON then
		if not monst then
			message(color.LIGHT_RED, "ERROR: corpse/skeleton without " ..
					"FLAG_MONSTER_OBJ")
			return
		end
	end

	food.eat_msg = nil
	if obj.flags[FLAG_CORPSE_EAT_MSG] then
		food.eat_msg = obj.flags[FLAG_CORPSE_EAT_MSG]
	elseif monst and monst.flags[FLAG_CORPSE_EAT_MSG] then
		food.eat_msg = monst.flags[FLAG_CORPSE_EAT_MSG]
	end

	local source_list = {player.body_monster.flags, obj.flags}
	if monst then
		tinsert(source_list, monst.flags)
	end
	if race then
		tinsert(source_list, race.flags)
	end
	if ego then
		tinsert(source_list, ego.flags)
	end

	-- Eat one pound by deafult
	food.weight_eaten = 10
	if obj.sval == SV_CORPSE_MEAT then
		food.weight_eaten = obj.weight
	end

	-- Invoke various corpse eating pre-hooks, to see if we can eat
	-- the corpse (and maybe cause bad things for even attempting to
	-- eat it).  Also, let hooks change the eating message, and the
	-- amount of meat eaten.
	local func_list = food.get_func_list(source_list, FLAG_CORPSE_EAT_PRE,
										 food.default_eat_corpse_pre)
	for i = 1, getn(func_list) do
		if func_list[i](obj) then
			-- Don't eat it
			return
		end
	end

	local no_meat = false
	if food.force_eat then
		-- Force eating of corpse, even if there ordinarily wouldn't
		-- be enough 
		food.force_eat = nil
	elseif obj.sval == SV_CORPSE_MEAT then
		-- Pieces of meat don't strip meat off a corpse
	elseif obj.sval == SV_CORPSE_SKELETON or obj.sval == SV_CORPSE_SKULL then
		-- Corpses are more substantial than skeletons
		if obj.weight <= (obj.flags[FLAG_ORIG_WEIGHT] / 3) then
			no_meat = true
		end
	else
		if obj.weight <= (obj.flags[FLAG_ORIG_WEIGHT] / 7) then
			no_meat = true
		end
	end

	if no_meat then
		message("Not enough left to eat.")
		food.eat_msg = nil
		return
	end

	local weight_eaten = food.weight_eaten or 10
	if obj.sval == SV_CORPSE_MEAT then
		weight_eaten = obj.weight
	end
	food.weight_eaten = nil

	if obj.weight < weight_eaten then
		weight_eaten = obj.weight
	end

	-- Invoke various corpse eating hooks
	local func_list = food.get_func_list(source_list, FLAG_ON_CORPSE_EAT,
										 food.default_eat_corpse_effect)
	for i = 1, getn(func_list) do
		local last_func = func_list[i](obj, weight_eaten)

		if last_func then
			break
		end
	end

	if food.eat_msg then
		if food.eat_msg ~= "" then
			message(food.eat_msg)
		end
		food.eat_msg = nil
	elseif obj.sval == SV_CORPSE_SKELETON or obj.sval == SV_CORPSE_SKULL then
		message("Crunchy.")
	else
		message("Ugh!  Raw meat!")
	end

	local food_value
	if food.food_value then
		food_value = food.food_value
		food.food_value = nil
	else
		-- Different corpses can have different nutrition per pound of meat
		food_value = get_flag(obj, FLAG_FOOD_VALUE) * weight_eaten / 10

		if food_value > 0 and obj.flags[FLAG_MONSTER_DECAY] and
			obj.flags[FLAG_MONSTER_DECAY] < obj.flags[FLAG_ORIG_DECAY]
		then
			-- Corpses are less nutritious as they decay
			food_value = food_value * obj.flags[FLAG_MONSTER_DECAY] /
				obj.flags[FLAG_ORIG_DECAY]
		end
	end

	timed_effect.inc(timed_effect.FOOD, food_value)

	if obj.sval == SV_CORPSE_MEAT then
		-- Piece of meat was used up
		return true
	else
		if weight_eaten >= obj.weight then
			-- Eaten all up, yum!
			return true
		else
			obj.weight = obj.weight - weight_eaten
		end
	end
end

-- Drop out of travel mode when starving
hook(hook.FORBID_TRAVEL,
function()
	if timed_effect.get(timed_effect.FOOD) < food.FOOD_WEAK then
		message("You are too hungry to travel.")
		return true
	end
end)
