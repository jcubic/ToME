--
-- The artifact/... activation subsystem
--
-- Load it with: load_subsystem("activations")
--
-- Parameters:
-- * no_key_bind: When set to true it will prevent any key bindings,
--                so taht you can do your own

-- Load the generic object using subsystem
load_subsystem("use_object")

constant("activations", {})
settag(activations, TAG_NAMESPACE) -- Tag as a namespace

safe_new_flag("ACT_SPELL")
safe_new_flag("ACT_COST_BASE")
safe_new_flag("ACT_COST_DICE")
safe_new_flag("ACT_COST_SIDES")
safe_new_flag("ACTIVATE_NO_WIELD");

if globals().flagsedit then flagsedit.register_flag_hints
{
	[FLAG_ACT_SPELL]   = flagsedit.HINT_FUNC,
} end

---------------------------
-- Subsystem parameters
---------------------------
activations.can_use = get_subsystem_param("activations",
										  "can_use") or
	function(obj, spell_flags, who)
		return true
	end

activations.get_mana_cost = get_subsystem_param("activations",
												"get_mana_cost") or
	function(obj, spell_flags, skill, who)
		local cost = spell_flags[FLAG_ACT_COST_BASE] or 0

		if spell_flags[FLAG_ACT_COST_DICE] and
			spell_flags[FLAG_ACT_COST_SIDES]
		then
			cost = cost + rng.roll(spell_flags[FLAG_ACT_COST_DICE],
								   spell_flags[FLAG_ACT_COST_SIDES])
		end

		return cost
	end

activations.is_free_use = get_subsystem_param("activations",
											  "is_free_use") or
	function(obj, spell_flags, skill, who)
		if spell_flags[FLAG_ACT_COST_BASE] and
			spell_flags[FLAG_ACT_COST_BASE] > 0
		then
			return false
		end

		if spell_flags[FLAG_ACT_COST_DICE] and
			spelL_flags[FLAG_ACT_COST_SIDES] then
			return false
		end

		return true
	end

activations.get_skill_level = get_subsystem_param("activations",
												  "get_skill_level") or
	function(obj, spell_flags, who)
		error("get_skill_level needs to be defined for activations " ..
			  "subsystem")
	end

activations.get_fail_pct = get_subsystem_param("activations",
											   "get_fail_pct") or
	function(obj, spell_flags, skill, who)
		error("get_fail_pct needs to be defined for activations " ..
			  "subsystem")
	end

activations.get_time = get_subsystem_param("activations",
										   "get_time") or
	function(obj, spell_flags, skill, who)
		error("get_time needs to be defined for activations " ..
			  "subsystem")
	end

activations.do_whine =  get_subsystem_param("activations",
											"do_whine") or
	function(obj, spell_flags, skill, who)
		if player.has_intrinsic(FLAG_BLIND) then
			message("It whines.")
		else
			message("It whines, glows and fades...")
		end
	end

activations.on_insuff_mana = get_subsystem_param("activations",
												 "on_insuff_mana") or
	function(obj, spell_flags, skill, mana_cost, who)
		return false
	end

activations.on_fail = get_subsystem_param("activations",
										  "on_fail") or
	function(obj, spell_flags, fail_pct, skill, who)
	end

----------------------
-- Utility functions
----------------------
function activations.get_activations(obj, known)
	if not has_flag(obj, FLAG_DEVICE_USE) or
		(known and not known_flag(obj, FLAG_DEVICE_USE))
	then
		return nil
	end

	local device_flags = obj.flags[FLAG_DEVICE_USE]
	local act_list     = {}

	for i = 0, flag_max_key(device_flags) do
		if device_flags[i] and (flag_is_known(device_flags, i) or
								not known)
		then
			local spell_flags = device_flags[i]

			if spell_flags[FLAG_ACT_SPELL] then
				local tmp = {}
				tmp.index = i
				tmp.spell_flags = spell_flags

				tinsert(act_list, tmp)
			end
		end
	end

	return act_list
end -- activations.get_activations()

function activations.get_num_activations(obj, known)
	local act_list = activations.get_activations(obj, known)

	if not act_list then
		return 0
	end

	return getn(act_list)
end

---------------------------------------------
-- Devices subsystem
---------------------------------------------
devices.add_subsystem {
	name = "activate",

	can_use =
		function(obj, spell_flags, who)
			-- Paranoia
			local source = obj_mana.to_source(obj)

			if not (source and spell_flags[FLAG_ACT_SPELL] and
					source.flags[FLAG_MANA_CURR] and
						source.flags[FLAG_MANA_MAX]) then
				message(color.VIOLET,
						"ERROR: This isn't a proper activatable object!")
				return false
			end

			if not (obj.flags[FLAG_AUTO_CHARGE_TYPE] or
					source.flags[FLAG_AUTO_CHARGE_TYPE]) then
				message(color.VIOLET,
						"ERROR: Activatable object doesn't auto-charge")
				return false
			end

			local func = get_function_registry_from_flag(spell_flags,
														 FLAG_ACT_SPELL)

			if not func then
				message(color.VIOLET,
						"ERROR: FLAG_ACT_SPELL isn't a Lua function")
				return false
			end

			return activations.can_use(obj, spell_flags, who)
		end, -- can_use()

	get_spell_name =
		function(obj, spell_flags, who)
			local func = get_function_registry_from_flag(spell_flags,
														 FLAG_ACT_SPELL)

			local name, desc, info = func(nil)

			return name
		end,

	get_spell_desc =
		function(obj, spell_flags, who)
			local func = get_function_registry_from_flag(spell_flags,
														 FLAG_ACT_SPELL)

			local name, desc, info = func(nil)

			return desc
		end,

	get_spell_info =
		function(obj, spell_flags, who)
			local func = get_function_registry_from_flag(spell_flags,
														 FLAG_ACT_SPELL)

			local name, desc, info = func(nil)

			return info
		end,

	get_skill_level =
		function(obj, spell_flags, who)
			return activations.get_skill_level(obj, spell_flags, who)
		end,

	get_mana_cost =
		function(obj, spell_flags, skill, who)
			return activations.get_mana_cost(obj, spell_flags, skill, who)
		end,

	on_insuff_mana =
		function(obj, spell_flags, skill, mana_cost, who)
			-- Paranoia
			if obj_mana.is_full(obj) then
				message(color.VIOLET,
						"ERROR: Object is fully charged yet still has " ..
							"insufficient mana.")
				return
			end

			if activations.on_insuff_mana(obj, spell_flags, skill,
										   mana_cost, who)
			then
				-- Nothing
			else
				activations.do_whine(obj, spell_flags, skill, who)
			end
		end, -- on_insuff_mana()

	get_fail_pct =
		function(obj, spell_flags, skill, who)
			return activations.get_fail_pct(obj, spell_flags, skill, who)
		end,

	on_fail =
		function(obj, spell_flags, fail_pct, skill, who)
			activations.on_fail(obj, spell_flags, fail_pct, skill, who)

			message("You failed to activate it properly.")
		end,

	pre_mana_use =
		function(obj, spell_flags, skill, mana_cost, who)
			-- Can only activate when full, or if this is something
			-- which can be used for free
			if not (obj_mana.is_full(obj) or
					activations.is_free_use(obj, spell_flags, who)) then
				if activations.on_insuff_mana(obj, spell_flags, skill,
											  mana_cost, who)
				then
					-- Nothing
				else
					activations.do_whine(obj, spell_flags, skill, who)
				end

				-- Return true to stop device
				return true
			end

			-- Return false, everything okay
			return false
		end -- pre_mana_use()

	use_device =
		function(obj, spell_flags, skill, mana_cost, who)
			local func = get_function_registry_from_flag(spell_flags,
														 FLAG_ACT_SPELL)

			return func(obj, spell_flags, skill, mana_cost, who)
		end,

	get_time =
		function(obj, spell_flags, skill, who)
			return activations.get_time(obj, spell_flags, skill, who)
		end,
} -- Activatable subsystem for decives framework

----------------------------------------------------------------------------
-- User interface
----------------------------------------------------------------------------

activations.item_tester =
	function(obj)
		local source = obj_mana.to_source(obj)

		if not source then
			-- Activatable object with removable mana source?
			return false
		end

		return (activations.get_num_activations(obj, true) > 0)
	end -- activations.item_tester()

-- Activate an item (from equipment or inventory).
function activations.use(item)
	if item and not activations.item_tester(get_object(item)) then
		return use_object.CANNOT_USE
	end

	-- Get an item
	if not item then
		local ret
		ret, item = get_item("Activate which item? ",
							 "You have nothing to activate.",
							 USE_INVEN | USE_EQUIP,
		   activations.item_tester,
		   'A'
		)
		if not ret then return end
	end

	-- Get the item
	local obj = get_object(item)

	if check_prevent_cmd(obj, 'A') then
		return
	end

	local act_list = activations.get_activations(obj, true)

	if not act_list or getn(act_list) == 0 then
		message(color.VIOLET, "ERROR: item has no (known) activations!")
		return
	elseif getn(act_list) > 1 then
		message("Using items with multiple activations not implemented yet.")
		return
	end

	local spell_idx   = act_list[1].index
	local spell_flags = act_list[1].spell_flags

	-- Wearable items have to be worn
	if not spell_flags[FLAG_ACTIVATE_NO_WIELD] and
		(item < 0 or item_slot_to_inven(item) < INVEN_PACK)
	then
		message("You must wear it to activate it.")
		return
	end

	return devices.use(obj, spell_idx, WHO_PLAYER)
end

-- Init
if not get_subsystem_param("activations", "no_key_bind") then
	hook(hook.KEYPRESS, function (key)
		if key == strbyte('A') then activations.use() return true end
	end)
end
