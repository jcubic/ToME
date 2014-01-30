-- SYSTEM FILE

--
-- Object mana/charges functions
--

constant("obj_mana", {})
settag(obj_mana, TAG_NAMESPACE) -- Tag as a namespace

obj_mana.auto_charge_types = {}

obj_mana.auto_charge_types.TIME = 1

-------------------------------------------------

new_flag("MANA_SOURCE_OBJ")

new_flag("MANA_CHANGE_DO")

new_flag("AUTO_CHARGE_TYPE")
new_flag("AUTO_CHARGE_RATE")
new_flag("AUTO_CHARGE_PACK")
new_flag("AUTO_CHARGE_WIELD")
new_flag("AUTO_CHARGE_COND")

--
-- Accessor functions
--

function obj_mana.to_source(obj)
	if obj.flags[FLAG_MANA_CURR] then
		return obj
	elseif obj.flags[FLAG_MANA_SOURCE_OBJ] then
		return obj.flags[FLAG_MANA_SOURCE_OBJ]
	else
		return nil
	end
end

function obj_mana.exists(obj, known)
	if known then
		if (known_flag(obj, FLAG_MANA_CURR) or
			known_flag(obj, FLAG_MANA_SOURCE_OBJ)) then
			return true
		end
	else
		if (obj.flags[FLAG_MANA_CURR] or obj.flags[FLAG_MANA_SOURCE_OBJ]) then
			return true
		end
	end

	return false
end -- obj_mana.exists()

function obj_mana.is_autocharging(obj, known, typ)
	if not obj_mana.exists(obj, known) then
		return false
	end

	local source = obj_mana.to_source(obj)

	if known then
		if not (known_flag(obj, FLAG_AUTO_CHARGE_TYPE ) or
				known_flag(source, FLAG_AUTO_CHARGE_TYPE)) then
			return false
		end
	else
		if not (obj.flags[FLAG_AUTO_CHARGE_TYPE] or
				source.flags[FLAG_AUTO_CHARGE_TYPE]) then
			return false
		end
	end

	if typ then
		if obj.flags[FLAG_AUTO_CHARGE_TYPE] and not
			obj.flags[FLAG_AUTO_CHARGE_TYPE] == typ
		then
			return false
		end

		if source.flags[FLAG_AUTO_CHARGE_TYPE] and not
			source.flags[FLAG_AUTO_CHARGE_TYPE] == typ
		then
			return false
		end
	end

	return true
end -- obj_mana.is_autocharging()

function obj_mana.get_curr(obj)
	obj = obj_mana.to_source(obj)

	if not obj then
		return nil
	end

	return obj.flags[FLAG_MANA_CURR]
end

function obj_mana.get_max(obj)
	obj = obj_mana.to_source(obj)

	if not obj then
		return nil
	end

	return obj.flags[FLAG_MANA_MAX]
end

function obj_mana.is_full(obj)
	obj = obj_mana.to_source(obj)

	if not obj or not obj.flags[FLAG_MANA_MAX] then
		return nil
	end

	return (obj.flags[FLAG_MANA_CURR] == obj.flags[FLAG_MANA_MAX])
end

---------------------------------------------------------------

--
-- Modification functions
--

hook.new_hook_type("MANA_CHANGE_PRE")
new_flag("MANA_CHANGE_PRE", true)

hook.new_hook_type("MANA_CHANGE_POST")
new_flag("MANA_CHANGE_POST", true)

function obj_mana.change(obj, amnt)
	local sink   = nil
	local source = obj

	if obj.flags[FLAG_MANA_SOURCE_OBJ] then
		sink   = obj
		source = obj.flags[FLAG_MANA_SOURCE_OBJ]
	end

	obj_mana.new_amnt = nil

	local ret = hook.process(hook.MANA_CHANGE_PRE, source, amnt, sink)

	if ret then
		return nil
	elseif obj_mana.new_amnt then
		amnt = obj_mana.new_amnt
	end

	amnt = item_hooks.process_all_chain(source, FLAG_MANA_CHANGE_PRE,
										amnt, sink)

	if not amnt then
		return nil
	end

	-- Changing a flag resets its known state to unknown, so restore it
	-- to the previous state after the change.  If whatever is
	-- causing the mana change wants the the mana amount to become
	-- unknown, it can change the known status itself
	local known = flag_is_known(source.flags, FLAG_MANA_CURR)

	if source.flags[FLAG_MANA_CHANGE_DO] then
		local fun
		func = get_function_registry_from_flag(source.flags,
											   FLAG_MANA_CHANGE_DO)

		amnt = func(source, amnt, sink)
	else
		source.flags[FLAG_MANA_CURR] = source.flags[FLAG_MANA_CURR] + amnt
	end

	flag_learn(source.flags, FLAG_MANA_CURR, known)

	local curr = source.flags[FLAG_MANA_CURR]
	local max  = source.flags[FLAG_MANA_MAX]

	if curr < 0 then
		source.flags[FLAG_MANA_CURR] = 0
	elseif max and curr > max then
		source.flags[FLAG_MANA_CURR] = max
	end

	hook.process(hook.MANA_CHANGE_POST, source, amnt, sink)

	item_hooks.process_all(source, FLAG_MANA_CHANGE_POST, amnt, sink)

	return amnt
end -- obj_mana.change()

hook.new_hook_type("MANA_USE_PRE")
new_flag("MANA_USE_PRE", true)

hook.new_hook_type("MANA_USE_POST")
new_flag("MANA_USE_POST", true)

-- Test if we have enough mana for the job.  Returns the amount of
-- mana that would have been used
function obj_mana.test_use(obj, amnt)
	local sink   = nil
	local source = obj

	if obj.flags[FLAG_MANA_SOURCE_OBJ] then
		sink   = obj
		source = obj.flags[FLAG_MANA_SOURCE_OBJ]
	end

	obj_mana.new_amnt = nil

	local ret = hook.process(hook.MANA_USE_PRE, source, amnt, sink)

	if ret then
		return false
	elseif obj_mana.new_amnt then
		amnt = obj_mana.new_amnt
	end

	amnt = item_hooks.process_all_chain(source, FLAG_MANA_USE_PRE,
										amnt, sink)

	if not amnt then
		return false
	end

	-- Do we have enough mana for this use?
	if source.flags[FLAG_MANA_CURR] < amnt then
		return false
	end

	return amnt
end -- obj_mana.test_use()

function obj_mana.use(obj, amnt)
	local sink   = nil
	local source = obj

	if obj.flags[FLAG_MANA_SOURCE_OBJ] then
		sink   = obj
		source = obj.flags[FLAG_MANA_SOURCE_OBJ]
	end

	obj_mana.new_amnt = nil

	local ret = hook.process(hook.MANA_USE_PRE, source, amnt, sink)

	if ret then
		return nil
	elseif obj_mana.new_amnt then
		amnt = obj_mana.new_amnt
	end

	amnt = item_hooks.process_all_chain(source, FLAG_MANA_USE_PRE,
										amnt, sink)

	if not amnt then
		return nil
	end

	-- Do we have enough mana for this use?
	if source.flags[FLAG_MANA_CURR] < amnt then
		return nil
	end

	obj_mana.change(obj, -amnt) -- Actually do it

	hook.process(hook.MANA_USE_POST, source, amnt, sink)

	item_hooks.process_all(source, FLAG_MANA_USE_POST, amnt, sink)

	return amnt
end -- obj_mana.use()

hook.new_hook_type("MANA_DRAIN_PRE")
new_flag("MANA_DRAIN_PRE", true)

hook.new_hook_type("MANA_DRAIN_POST")
new_flag("MANA_DRAIN_POST", true)

function obj_mana.drain(obj, amnt)
	local sink   = nil
	local source = obj

	if obj.flags[FLAG_MANA_SOURCE_OBJ] then
		sink   = obj
		source = obj.flags[FLAG_MANA_SOURCE_OBJ]
	end

	obj_mana.new_amnt = nil

	local ret = hook.process(hook.MANA_DRAIN_PRE, source, amnt, sink)

	if ret then
		return nil
	elseif obj_mana.new_amnt then
		amnt = obj_mana.new_amnt
	end

	amnt = item_hooks.process_all_chain(source, FLAG_MANA_DRAIN_PRE,
										amnt, sink)

	if not amnt then
		return nil
	end

	obj_mana.change(obj, -amnt) -- Actually do it

	hook.process(hook.MANA_DRAIN_POST, source, amnt, sink)

	item_hooks.process_all(source, FLAG_MANA_DRAIN_POST, amnt, sink)

	return amnt
end -- obj_mana.drain()

hook.new_hook_type("MANA_CHARGE_PRE")
new_flag("MANA_CHARGE_PRE", true)

hook.new_hook_type("MANA_CHARGE_POST")
new_flag("MANA_CHARGE_POST", true)

function obj_mana.charge(obj, amnt, is_auto)
	local sink   = nil
	local source = obj

	if obj.flags[FLAG_MANA_SOURCE_OBJ] then
		sink   = obj
		source = obj.flags[FLAG_MANA_SOURCE_OBJ]
	end

	obj_mana.new_amnt = nil

	local ret = hook.process(hook.MANA_CHARGE_PRE, source, amnt,
							 is_auto, sink)

	if ret then
		return nil
	elseif obj_mana.new_amnt then
		amnt = obj_mana.new_amnt
	end

	amnt = item_hooks.process_all_chain(source, FLAG_MANA_CHARGE_PRE,
										amnt, is_auto, sink)

	if not amnt then
		return nil
	end

	obj_mana.change(obj, amnt) -- Actually do it

	hook.process(hook.MANA_CHARGE_POST, source, amnt, is_auto, sink)

	item_hooks.process_all(source, FLAG_MANA_CHARGE_POST, amnt, is_auto, sink)

	return amnt
end -- obj_mana.charge()

-------------------------------------------------

--
-- Auto-charging
--

function obj_mana.do_auto_charge(obj, auto_type, pack_or_wield)
	local sink

	if obj.flags[FLAG_MANA_SOURCE_OBJ] then
		sink = obj
		obj  = obj.flags[FLAG_MANA_SOURCE_OBJ]
	end

	-- Don't auto-recharge items without a maximum mana, since
	-- otherwise the mana would keep going up and up.
	if not obj.flags[FLAG_MANA_MAX] then
		return
	end

	-- No need to recharge if its full
	if obj_mana.is_full(obj) then
		return
	end

	local amnt = 0

	if obj.flags[FLAG_AUTO_CHARGE_TYPE] == auto_type then
		local cond = true

		if obj.flags[FLAG_AUTO_CHARGE_COND] then
			local func
			func = get_function_registry_from_flag(obj.flags,
												   FLAG_MANA_CHANGE_DO)

			cond = func(obj, obj, sink, pack_or_wield)
		end

		if cond and obj.flags[pack_or_wield] then
			amnt = amnt + obj.flags[FLAG_AUTO_CHARGE_RATE]
		end
	end

	if sink and sink.flags[FLAG_AUTO_CHARGE_TYPE] == auto_type then
		local cond = true

		if sink.flags[FLAG_AUTO_CHARGE_COND] then
			local func
			func = get_function_registry_from_flag(sink.flags,
												   FLAG_MANA_CHANGE_DO)

			cond = func(sink, obj, sink, pack_or_wield)
		end

		if sink.flags[pack_or_wield] then
			amnt = amnt + sink.flags[FLAG_AUTO_CHARGE_RATE]
		end
	end

	-- Object didn't recharge for whatever reason
	if amnt == 0 then
		return
	end

	obj_mana.charge(sink or obj, amnt, true)

	-- We're full, alert user if object is inscribed with '!!'
	if obj_mana.is_full(obj) then
		if not flag_is_known(obj.flags, FLAG_MANA_CURR) or
			not flag_is_known(obj.flags, FLAG_MANA_MAX) then
			-- Player won't know its recharged unsless s/he knows
			-- both the current mana and the maximum mana.
			return
		end

		local alert = false

		if notefind(obj, "!!") then
			alert = true
		end

		if sink and notefind(sink, "!!") then
			alert = true
		end

		if alert then
			local name = object_desc(sink or obj)

			message("The " .. name .. " has finished recharging.")
		end
	end
end -- obj_mana.do_auto_charge()

-- Don't do time-based recharging in the wilderness
hook(hook.PROCESS_WORLD_NON_WILD,
function()
	local auto_type = obj_mana.auto_charge_types.TIME

	-- Iterate over inventory
	for_inventory_inven(player.inventory[INVEN_INVEN],
		function(obj)
			if obj_mana.exists(obj) then
				obj_mana.do_auto_charge(obj, %auto_type,
										FLAG_AUTO_CHARGE_PACK)
			end
		end)

	-- Iterate over equipment
	for_inventory(player, INVEN_PACK, INVEN_TOTAL,
		function(obj)
			if obj_mana.exists(obj) then
				obj_mana.do_auto_charge(obj, %auto_type,
										FLAG_AUTO_CHARGE_WIELD)
			end
		end)
end)
