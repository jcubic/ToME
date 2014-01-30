------------------------------
-- Unified magic device system
------------------------------

constant("devices", {})
settag(devices, TAG_NAMESPACE) -- Tag as a namespace

new_flag("DEVICE_USE")
new_flag("DEVICE_TYPE")

hook.new_hook_type("DEVICE_CAN_USE")
new_flag("DEVICE_CAN_USE", true)

hook.new_hook_type("DEVICE_CHANGE_SKILL")
new_flag("DEVICE_CHANGE_SKILL", true)

hook.new_hook_type("DEVICE_CHANGE_FAIL")
new_flag("DEVICE_CHANGE_FAIL", true)

hook.new_hook_type("DEVICE_ON_INSUFF_MANA")
new_flag("DEVICE_ON_INSUFF_MANA", true)

hook.new_hook_type("DEVICE_ON_FAIL")
new_flag("DEVICE_ON_FAIL", true)

hook.new_hook_type("DEVICE_USE_PRE")
new_flag("DEVICE_USE_PRE", true)

hook.new_hook_type("DEVICE_CHANGE_TIME")
new_flag("DEVICE_CHANGE_TIME", true)

hook.new_hook_type("DEVICE_USE_POST")
new_flag("DEVICE_USE_POST", true)

devices.subsystems = {}

-- Convenience function for when an object only has a single device spell
getter.single_device_spell =
function(t)
	local out = flag_new()

	out[1] = getter.flags(t)

	return out
end

function devices.add_subsystem(t)
	assert(t.name, "New devices subsystem has no name.")
	assert(not devices.subsystems[t.name],
		   "Devices subsystem '" .. t.name .. "' already exists")
	assert(t.can_use, "Devices subsystem '" .. t.name ..
		   "' has no can_use()")
	assert(t.get_spell_name, "Devices subsystem '" .. t.name ..
		   "' has no get_spell_name()")
	assert(t.get_spell_desc, "Devices subsystem '" .. t.name ..
		   "' has no get_spell_desc()")
	assert(t.get_skill_level, "Devices subsystem '" .. t.name ..
		   "' has no get_skill_level()")
	assert(t.get_mana_cost, "Devices subsystem '" .. t.name ..
		   "' has no get_mana_cost()")
	assert(t.get_fail_pct, "Devices subsystem '" .. t.name ..
		   "' has no get_fail_pct()")
	assert(t.use_device, "Devices subsystem '" .. t.name ..
		   "' has no use_device()")
	assert(t.get_time, "Devices subsystem '" .. t.name ..
		   "' has no get_time()")

	devices.subsystems[t.name] = t
end -- devices.add_subsystem()

function devices.is_device(obj)
	return has_flag(obj, FLAG_DEVICE_USE)
end

function devices.get_subsys_and_flags(obj, index)
	if not devices.is_device(obj) then
		message(color.RED, "ERROR: Object " .. object_desc(obj) ..
				" is not a device")
		return nil, nil
	end

	local spell_flags = obj.flags[FLAG_DEVICE_USE][index]

	if not spell_flags then
		message(color.RED, "ERROR: device index is out of range")
		return nil, nil
	end

	if not spell_flags[FLAG_DEVICE_TYPE] then
		message(color.RED, "ERROR: index device spell has no DEVICE_TYPE")
		return nil, nil
	end

	local subsys = devices.subsystems[spell_flags[FLAG_DEVICE_TYPE]]

	if not subsys then
		message(color.RED, "ERROR: devices subsystem '" ..
				spell_flags[FLAG_DEVICE_TYPE] .. "' is not registered")
		return nil, nil
	end

	return subsys, spell_flags
end -- devices.get_subsys_and_flags()

function devices.get_spell_desc(obj, index, who)
	local subsys, spell_flags = devices.get_subsys_and_flags(obj, index)
	local skill

	if not subsys then
		return nil
	end

	return subsys.get_spell_desc(obj, spell_flags, who)
end

function devices.get_spell_info(obj, index, who)
	local subsys, spell_flags = devices.get_subsys_and_flags(obj, index)
	local skill

	if not subsys then
		return nil
	end

	return subsys.get_spell_info(obj, spell_flags, who)
end

function devices.get_skill(obj, index, who)
	local subsys, spell_flags = devices.get_subsys_and_flags(obj, index)
	local skill

	if not subsys then
		return nil
	end

	-- Get the skill level with which we'll do the device spell
	local skill = subsys.get_skill_level(obj, spell_flags, who)

	devices.new_skill = nil

	local ret = hook.process(hook.DEVICE_CHANGE_SKILL, obj, spell_flags,
							 skill, who)

	if ret then
		return false
	elseif devices.new_skill then
		skill = devices.new_skill
	end

	skill = item_hooks.process_all_chain(obj, FLAG_DEVICE_CHANGE_SKILL,
										 skill, spell_flags, who)

	return skill
end -- devices.get_skill()

function devices.get_fail_pct(obj, index, skill, who)
	local subsys, spell_flags = devices.get_subsys_and_flags(obj, index)
	local fail_pct

	if not subsys then
		return nil
	end

	-- Given the skill level, determine the failure rate and see if
	-- this attempt to use fails
	local fail_pct = subsys.get_fail_pct(obj, spell_flags, skill, who)

	devices.new_fail_pct = nil

	local ret = hook.process(hook.DEVICE_CHANGE_FAIL, obj, spell_flags,
					   fail_pct, skill, who)

	if ret then
		return false
	elseif devices.new_fail_pct then
		fail_pct = devices.new_fail_pct
	end

	fail_pct = item_hooks.process_all_chain(obj, FLAG_DEVICE_CHANGE_FAIL,
											fail_pct, skill, spell_flags, who)

	if not fail_pct then
		return false
	end

	if fail_pct < 0 then
		fail_pct = 0
	end

	return fail_pct
end -- devices.get_fail_pct()

function devices.use(obj, index, who)
	local subsys, spell_flags = devices.get_subsys_and_flags(obj, index)

	if not subsys then
		return false
	end

	-- Can we even try using it?
	if not subsys.can_use(obj, spell_flags, who) then
		return false
	end

	if hook.process(hook.DEVICE_CAN_USE, obj, spell_flags, who) then
		return false
	end

	if item_hooks.process_one(obj, FLAG_DEVICE_CAN_USE, spell_flags, who) then
		return false
	end

	-- Get the skill level with which we'll do the device spell
	local time = subsys.get_time(obj, spell_flags, skill, who)

	devices.new_time = nil

	hook.process(hook.DEVICE_CHANGE_TIME, obj, spell_flags, skill, who)

	if devices.new_time then
		time = devices.new_time
	end

	energy_use = time

	time = item_hooks.process_all_chain(obj, FLAG_DEVICE_CHANGE_TIME, time,
										spell_flags, skill, who)

	if time then
		time = energy_use
	end

	local skill = devices.get_skill(obj, index, who)

	if not skill then
		return false
	end

	-- Do we have enough mana?  NOTE: the object mana framework takes
	-- care of things like "of Cheapness" egos, so we don't have to
	-- worry about it; we just use the raw mana cost, untouched by
	-- any hooks.
	local mana_cost = subsys.get_mana_cost(obj, spell_flags, skill, who)

	if mana_cost > 0 and not obj_mana.test_use(obj, mana_cost) then
		-- Not enough mana, stop right here
		if subsys.on_insuff_mana then
			subsys.on_insuff_mana(obj, spell_flags, skill, mana_cost, who)
		end

		hook.process(hook.DEVICE_ON_INSUFF_MANA, obj, spell_flags,
					 skill, mana_cost, who)

		item_hooks.process_all(obj, FLAG_DEVICE_ON_INSUFF_MANA,
							   spell_flags, skill, mana_cost, who)
		return false
	end


	local fail_pct = devices.get_fail_pct(obj, index, skill, who)

	if not fail_pct then
		return false
	end

	local fail = rng.percent(fail_pct)

	if not fail then
		fail = hook.process(hook.DEVICE_USE_PRE, obj, spell_flags. fail_pct,
							skill, who)
	end

	if not fail then
		fail = item_hooks.process_one(obj, FLAG_DEVICE_USE_PRE,
									  fail_pct, skill, spell_flags, who)
	end

	if fail then
		if subsys.on_fail then
			subsys.on_fail(obj, spell_flags, fail_pct, skill, who)
		end

		hook.process(hook.DEVICE_ON_FAIL, obj, spell_flags, fail_pct,
					 skill, who)

		item_hooks.process_all(obj, FLAG_DEVICE_ON_FAIL,
							   spell_flags, fail_pct, skill, who)
		return false
	end

	-- Okay, we have enough mana AND passed the failure check, now
	-- consume the mana and do the spell
	if subsys.pre_mana_use then
		if subsys.pre_mana_use(obj, spell_flags, skill, mana_cost, who) then
			return false
		end
	end

	mana_cost = obj_mana.use(obj, mana_cost)

	if not mana_cost then
		-- This shouldn't happen, we already checked if there was enough
		-- mana.
		message(color.RED, "IMPOSSIBLE: not enough object mana!")
		return false
	end

	if not subsys.use_device(obj, spell_flags, skill, mana_cost, who) then
		-- Spell cancelled by user?
		return false
	end

	-- Done!  Inform stuff that we're done.
	if subsys.on_success then
		subsys.on_success(obj, spell_flags, fail_pct, skill, mana_cost,
						  time, who)
	end

	hook.process(hook.DEVICE_USE_POST, obj, spell_flags, fail_pct,
				 skill, mana_cost, time, who)
	
	item_hooks.process_all(obj, FLAG_DEVICE_USE_POST, spell_flags,
						   fail_pct, skill, mana_cost, time, who)

	return true
end -- devices.use()

