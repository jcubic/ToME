-- This file is the core of the Automatizer
-- Please do not touch unless you know what you are doing

new_flag("NO_AUTO_DEST")

declare_globals{"rule_aux"}
declare_globals{"auto_aux"}
rule_aux = {}

constant("auto", {})

auto.rules      = {}
auto.rules_max  = 0
auto.rules_skip = {}

auto.result = {}
auto.result.result   = nil
auto.result.CONTINUE = 1
auto.result.STOP     = 2

-- Rule apply function, does .. nothing
declare_global_constants"auto_nothing"
function auto_nothing(obj, item)
	return
end

-- Rule apply function, stops processing of rules
declare_global_constants"auto_stop"
function auto_stop(obj, item)
	if auto.rules_skip["stop"] then
		auto.result.result = auto.result.CONTINUE
		return true
	end

	auto.result.result = auto.result.STOP
	return true
end

-- Rule apply function, specify types of rules to skip
declare_global_constants"auto_skip"
function auto_skip(obj, item, types)
	if auto.rules_skip["skip"] then
		auto.result.result = auto.result.CONTINUE
		return true
	end

	local type_list = strsplit(types, ",")

	for i = 1, getn(type_list) do
		auto.rules_skip[type_list[i]] = true
	end

	auto.result.result = auto.result.CONTINUE
	return true
end

-- Rule apply function, sets or alters the item's incscription
declare_global_constants"auto_inscribe"
function auto_inscribe(obj, item, note, mode)
	if auto.rules_skip["inscribe"] then
		auto.result.result = auto.result.CONTINUE
		return true
	end

	if not mode or strlen(mode) == 0 then
		mode = "normal"
	end

	if mode == "normal" then
		-- Do nothing if there's already an inscription
		if obj.note ~= 0 then return end
	elseif mode == "replace" then
		-- Do nothing if replacing the inscription would cause no change
		if obj.note ~= 0 then
			if note == quark_str(obj.note) then return end
		end
		obj.note = quark_add(note)
	elseif mode == "append" then
		if obj.note ~= 0 then
			local old_note = quark_str(obj.note)

			-- Do nothing if the inscription already contains the note
			if strfind(old_note, note) then return end

			note = old_note .. " " .. note
		end
	elseif mode == "prepend" then
		if obj.note ~= 0 then
			local old_note = quark_str(obj.note)

			-- Do nothing if the inscription already contains the note
			if strfind(old_note, note) then return end

			note = note .. " " .. old_note
		end
	else
		message(color.VIOLET, "*ERROR* Unknown auto-inscribe mode '" ..
				mode "'")
		return
	end

	message("<Auto-Inscribe {"..note.."}>")
	obj.note = quark_add(note)

	auto.result.result = auto.result.CONTINUE
	return true
end

-- Rule apply function, pickup object
declare_global_constants"auto_pickup"
function auto_pickup(obj, item)
	if auto.rules_skip["pickup"] then
		auto.result.result = auto.result.CONTINUE
		return true
	end

	if item >= 0 then
		-- Item is already being carried by player
		return
	end

	if not inven_carry_okay(obj) then return end
	message("<Auto-pickup>")
	object_pickup(item)

	-- Stop, since the object's slot number has just changed due to
	-- being picked up
	auto.result.result = auto.result.STOP
	return true
end

-- Rule apply function, destroy item
declare_global_constants"auto_destroy"
function auto_destroy(obj, item, force)
	if auto.rules_skip["destroy"] then
		auto.result.result = auto.result.CONTINUE
		return true
	end

	if known_flag(obj, FLAG_NO_AUTO_DEST) then
		return
	end

	if not force then
		force = "none"
	end

	-- be carefull to what we can destroy
	-- Unaware things won't be destroyed, unless the user forces them
	-- to be
	if not is_aware(obj) and force ~= "all" and force ~= "unaware" then
		return
	end

	-- Inscribed things won't be destroyed! (unless the user forces them
	-- to be)
	if obj.note ~= 0 and force ~= "all" and force ~= "inscribed" then
		return
	end

	-- Keep Artifacts -- they cannot be destroyed anyway
	if is_artifact(obj) then return end

	-- Cannot destroy CURSE_NO_DROP objects
	if has_flag(obj, FLAG_CURSE_NO_DROP) and (obj.ident & IDENT_CURSED) ~= 0 then return end

	message("<Auto-destroy>")

	item_increase(item, -obj.number)
	item_describe(item)
	item_optimize(item)

	-- Item is gone, nothing more to do, stop processing rules
	auto.result.result = auto.result.STOP
	return true
end

---------------------------------------------------------------------
-- Automizer clauses
---------------------------------------------------------------------

--
-- Report the status of an object
--

auto.quality_by_tval = {}
auto.quality_by_slot = {}

function auto.add_quality_by_tvals(t)
	table_copy(auto.quality_by_tval, t)
end

function auto.add_quality_by_slots(t)
	table_copy(auto.quality_by_slot, t)
end

-- To be overriden by module to provide a quality string for identified
-- objects which aren't covered by auto.quality_by_tval or
-- auto.quality_by_slot
function auto.known_object_quality(obj)
	if is_artifact(obj) then
		local value = object_value_real(obj)
		if value >= 0 then
			return "special"
		else
			return "terrible"
		end
	end

	return nil
end -- known_object_quality()

declare_global_constants"object_quality"
function object_quality(obj)
	local sense = {
		[SENSE_BROKEN]     = "bad",
		[SENSE_CURSED]     = "bad",
		[SENSE_WORTHLESS]  = "very bad",
		[SENSE_AVERAGE]    = "average",
		[SENSE_UNCURSED]   = "average"
		[SENSE_GOOD_LIGHT] = "good",
		[SENSE_GOOD_HEAVY] = "good",
		[SENSE_EXCELLENT]  = "very good",
		[SENSE_SPECIAL]    = "special",
		[SENSE_TERRIBLE]   = "terrible",
	}

	if not is_known(obj) then
		if obj.sense == SENSE_NONE then
			return "unknown"
		else
			return sense[obj.sense] or "unknown"
		end
	end

	local quality = auto.known_object_quality(obj)

	if not quality or quality == "" then
		local slot = wield_slot_ideal(obj, true)
		if slot ~= -1 then
			local func = auto.quality_by_slot[slot + 1]
			if func then
				quality = func(obj)
			end
		end
	end

	if not quality or quality == "" then
		local func = auto.quality_by_tval[obj.tval]
		if func then
			quality = func(obj)
		end
	end
		
	if not quality or quality == "" then
		if obj.sense == SENSE_NONE then
			quality = "unknown"
		else
			quality = sense[obj.sense] or "unknown"
		end
	end		

	return quality
end -- object_quality()

declare_global_constants"hunger_in_range"
function hunger_in_range(min, max)
	local hungers = {
		["dead"]     = 0,
		["starving"] = PY_FOOD_STARVE,
		["fainting"] = PY_FOOD_FAINT,
		["weak"]     = PY_FOOD_WEAK,
		["hungry"]   = PY_FOOD_ALERT,
		["full"]     = PY_FOOD_FULL,
		["gorged"]   = PY_FOOD_MAX,
		["inf"]      = infinity,
		["infinity"] = infinity
	}

	min = hungers[min]
	if not min then
		message(color.VIOLET, "*ERROR* No such hunger type as '" ..
				min .. "'")
		return
	end

	max = hungers[max]
	if not max then
		message(color.VIOLET, "*ERROR* No such hunger type as '" ..
				max .. "'")
		return
	end

	if player.food >= min and player.food <= max then
		return true
	end
end -- hunger_in_range()

declare_global_constants"auto_slot_compare"
function auto_slot_compare(obj, attrib, method, exactness, all, min, max)
	assert(method, "Atuomizer object comparison needs a compare method")

	-- Must be something wieldable
	if not obj.flags[FLAG_WIELD_SLOT] then
		return false
	end

	-- Can only compare the objec if it's identified
	if not is_known(obj) then
		return false
	end

	local slot_size = player.inventory_limits(obj.flags[FLAG_WIELD_SLOT])

	-- Player can't wield objects that go in that slot
	if slot_size == 0 then
		return false
	end

	attrib    = strlower(attrib)
	method    = strlower(method)
	exactness = strlower(exactness or "slot")

	if type(min) == "string" then
		min = tonumber(min)
	end
	min = min or -infinity

	if type(max) == "string" then
		max = tonumber(max)
	end
	max = max or infinity

	local num_compares = 0
	-- Iterate over all worm objects in the same wield slot.  For example,
	-- a player wearing two rings has two objects in the ring slot.
	for i = 1, slot_size do
		local eq = get_object(compute_slot(obj.flags[FLAG_WIELD_SLOT], i))

		if not eq then
			-- User has no more items worn in this slot
			break
		end

		if exactness == "tval" and eq.tval ~= obj.tval then
			-- Skip this object
		elseif exactness == "sval" and (eq.tval ~= obj.tval or
										eq.sval ~= obj.sval) then
			-- Skip this object
		elseif not is_known(eq) then
			-- Skip unidetnified equipment if we only need to match
			-- one object.  If we need to match all, then having even
			-- one of them be unidentified means we can't be sure
			-- the clause is true, so return false.
			if all then
				return false
			end
		else
			local obj_val
			local eq_val

			if attrib == "value" then
				obj_val = object_value_real(obj)
				eq_val  = object_value_real(eq)
			elseif attrib == "base ac" then
				obj_val = obj.ac or 0
				eq_val  = eq.ac  or 0
			elseif attrib == "ac bonus" then
				obj_val = obj.to_a or 0
				eq_val  = eq.to_a  or 0
			elseif attrib == "total ac" then
				obj_val = (obj.ac or 0) + (obj.to_a or 0)
				eq_val  = (eq.ac or 0) + (eq.to_a or 0)
			elseif attrib == "hit bouns" then
				obj_val = obj.to_h or 0
				eq_val  = eq.to_h  or 0
			elseif attrib == "dam bonus" then
				obj_val = obj.to_d or 0
				eq_val  = eq.to_d  or 0
			else
				message(color.VIOLET,
						"*ERROR* Unknown automizer comparison " ..
							"type '" .. attrib .. "'")
				return false
			end

			local value

			if method == "diff" or method == "subtract" then
				value = obj_val - eq_val
			elseif method == "ratio" or method == "divide" then
				if obj_val == 0 then
					if eq_val == 0 then
						-- Undefined
						return false
					elseif eq_val > 0 then
						value = infinity
					else
						value = -infinity
					end
				else --if obj_val == 0 then
					-- Do percentages, so multiply by 100
					value = obj_val * 100 / eq_val
				end
			else -- if method == "ratio" or method == "divide" then
				message(color.VIOLET,
						"*ERROR* Unknown automizer comparison " ..
							"method '" .. method .. "'")
				return false
			end

			local result = (value >= min and value <= max)

			if result and not all then
				-- If we only need to match one, is true if any match
				return true
			elseif not result and all then
				-- If we need to match all, is false if any don't match
				return false
			end

			num_compares = num_compares + 1
		end -- if is_known(eq) then
	end -- Iterate over all objects in same wield slot

	if num_compares == 0 then
		return false
	end

	-- If we got here, and "all" was set, it means all objects in the
    -- wield slot matched, so the rule succeeded, thus we return true.
	-- If "all" was not set, then none of the objects matched, and we
	-- return false.
	return all
end -- auto_slot_compare()

declare_global_constants"auto_found_type"
function auto_found_type(obj, found_type)
	local types = {
		[OBJ_FOUND_MONSTER]  = "monster",
		[OBJ_FOUND_FLOOR]    = "floor",
		[OBJ_FOUND_VAULT]    = "vault",
		[OBJ_FOUND_SPECIAL]  = "special",
		[OBJ_FOUND_RUBBLE]   = "rubble",
		[OBJ_FOUND_REWARD]   = "reward",
		[OBJ_FOUND_STORE]    = "store",
		[OBJ_FOUND_STOLEN]   = "stolen",
		[OBJ_FOUND_SELFMADE] = "selfmade",
		[OBJ_FOUND_WISH]     = "wish",
		[OBJ_FOUND_DEBUG]    = "debugging",
		[OBJ_FOUND_START]    = "start",
	}

	local rand_types = {
		[OBJ_FOUND_MONSTER]  = 1,
		[OBJ_FOUND_FLOOR]    = 1,
		[OBJ_FOUND_VAULT]    = 1,
		[OBJ_FOUND_RUBBLE]   = 1,
	}

	local non_rand_types = {
		[OBJ_FOUND_SPECIAL]  = 1,
		[OBJ_FOUND_REWARD]   = 1,
		[OBJ_FOUND_STORE]    = 1,
		[OBJ_FOUND_STOLEN]   = 1,
		[OBJ_FOUND_SELFMADE] = 1,
		[OBJ_FOUND_WISH]     = 1,
		[OBJ_FOUND_DEBUG]    = 1,
		[OBJ_FOUND_START]    = 1,
	}

	found_type = strlower(found_type)

	if found_type == "random" and rand_types[obj.found] then
		return true
	end

	if found_type == "non-random" and rand_types[obj.found] then
		return true
	end

	if types[obj.found] == found_type then
		return true
	end

	return false
end

---------------------------------------------------------------------------
-- Code for actually putting together the rules
---------------------------------------------------------------------------

auto.clauses = {}
function auto.add_clause(name, clause)
	auto.clauses[name] = clause
end

-- Recursive function to generate a rule function tree
declare_global_constants"gen_rule_fct"
function gen_rule_fct(r)
	-- It is a test rule (or, and, ...)
	if r.label == "and" or r.label == "or" then
		local i
		local fct_tbl = {}
		for i = 1, getn(r) do
			if r[i].label ~= "comment" then
				tinsert(fct_tbl, gen_rule_fct(r[i]))
			end
		end
		if r.label == "and" then
			return function(object)
				local fcts = %fct_tbl
				local i
				for i = 1, getn(fcts) do
					if not fcts[i](object) then return end
				end
				return true
			end
		elseif r.label == "or" then
			return function(object)
				local fcts = %fct_tbl
				local i
				for i = 1, getn(fcts) do
					if fcts[i](object) then return true end
				end
			end
		end
	-- It is a condition rule (name, type, level, ...)
	else
		if r.label == "not" then
			local f
			if not r[1] then
				f = function (object) return true end
			else
				f = gen_rule_fct(r[1])
			end
			return function(object) return not %f(object) end
		elseif r.label == "inventory" then
			local f
			if not r[1] then
				f = function(object) return end
			else
				f = gen_rule_fct(r[1])
			end
			return function(object)
				local ret = {["match"] = false}
				local func = %f
				for_inventory_inven(player.inventory[INVEN_INVEN],
					function(obj)
						if %func(obj) then
							%ret.match = true
					end
				end) -- for_inventory_inven
				return ret.match
			end
		elseif r.label == "equipment" then
			local f
			if not r[1] then
				f = function(object) return end
			else
				f = gen_rule_fct(r[1])
			end
			return function(object)
				local ret = {["match"] = false}
				local func = %f
				for_inventory_(player, INVEN_PACK, INVEN_TOTAL,
					function(obj)
						if %func(obj) then
							%ret.match = true
					end
				end) -- for_inventory_inven
				return ret.match
			end
		elseif r.label == "name" then
			return function(object) if strlower(object_desc(object, -1, 0)) == strlower(%r[1]) then return true end end
		elseif r.label == "contain" then
			return function(object) if strfind(strlower(object_desc(object, -1, 0)), strlower(%r[1])) then return true end end
		elseif r.label == "symbol" then
			return function(object) if strchar(get_kind(object).d_char) == %r[1] then return true end end
		elseif r.label == "inscribed" then
			return function(object) if object.note ~= 0 and strfind(strlower(quark_str(object.note)), strlower(%r[1])) then return true end end
		elseif r.label == "discount" then
			local d1 = r.args.min
			local d2 = r.args.max
			if tonumber(d1) == nil then d1 = getglobal(d1) else d1 = tonumber(d1) end
			if tonumber(d2) == nil then d2 = getglobal(d2) else d2 = tonumber(d2) end
			return function(object) if is_aware(object) and object.discount >= %d1 and object.discount <= %d2 then return true end end
		elseif r.label == "tval" then
			local tv = r[1]
			if tonumber(tv) == nil then tv = getglobal(tv) else tv = tonumber(tv) end
			return function(object) if object.tval == %tv then return true end end
		elseif r.label == "sval" then
			assert(r.args.min and r.args.max, "sval rule lacks min or max")
			local sv1 = r.args.min
			local sv2 = r.args.max
			if tonumber(sv1) == nil then sv1 = getglobal(sv1) else sv1 = tonumber(sv1) end
			if tonumber(sv2) == nil then sv2 = getglobal(sv2) else sv2 = tonumber(sv2) end
			return function(object) if is_aware(object) and object.sval >= %sv1 and object.sval <= %sv2 then return true end end
		elseif r.label == "quality" then
			return function(object) if object_quality(object) == strlower(%r[1]) then return true end end
		elseif r.label == "state" then
			if r[1] == "identified" then
				return function(object) if is_known(object) then return true end end
			elseif r[1] == "aware" then
				return function(object) if is_aware(object) then return true end end
			elseif r[1] == "sensed" then
				return function(object) if object.sense and object.sense ~= SENSE_NONE then return true end end
			elseif r[1] == "open" then
				return function(object) if known_flag(object, FLAG_OPEN) then return true end end
			elseif r[1] == "closed" then
				return function(object) if known_flag(object, FLAG_CLOSED) then return true end end
			elseif r[1] == "locked" then
				return function(object) if known_flag(object, FLAG_LOCKED) then return true end end
			elseif r[1] == "unlocked" then
				return function(object) if known_flag(object, FLAG_UNLOCKED) then return true end end
			elseif r[1] == "trapped" then
				return function(object) if has_flag(object, FLAG_TRAP_AWARE) then return true end end
			else
				message(color.VIOLET, "Unknown automizer 'state' type '" ..
						r[1] .. "'")
				return function(object) return nil end
			end
		elseif r.label == "race" then
			return function(object) if strlower(get_race_name()) == strlower(%r[1]) then return true end end
		elseif r.label == "subrace" then
			return function(object) if strlower(get_subrace_name()) == strlower(%r[1]) then return true end end
		elseif r.label == "class" then
			return function(object) if strlower(get_class_name()) == strlower(%r[1]) then return true end end
		elseif r.label == "level" then
			assert(r.args.min and r.args.max, "level rule lacks min or max")
			return function(object) if player.lev >= tonumber(%r.args.min) and player.lev <= tonumber(%r.args.max) then return true end end
		elseif r.label == "skill" then
			assert(r.args.min and r.args.max, "skill rule lacks min or max")
			local s = find_skill_i(r[1])
			assert(s ~= -1, "no skill "..r[1])
			return function(object) if get_skill(%s) >= tonumber(%r.args.min) and get_skill(%s) <= tonumber(%r.args.max) then return true end end
		elseif r.label == "ability" then
			local s = find_ability(r[1])
			assert(s ~= -1, "no ability "..r[1])
			return function(object) if has_ability(%s) == true then return true end end
		elseif r.label == "dungeon" then
			return function(object) if strlower(d_info[current_dungeon_idx + 1].name) == strlower(%r[1]) then return true end end
		elseif r.label == "hunger" then
			assert(r.args.min and r.args.max, "hunger rule lacks min or max")
			return function(object) return hunger_in_range(%r.args.min, %r.args.max) end
		elseif r.label == "money" then
			assert(r.args.min and r.args.max, "money rule lacks min or max")
			return function(object) if player.au >= tonumber(%r.args.min) and player.au <= tonumber(%r.args.max) then return true end end
		elseif r.label == "value" then
			assert(r.args.min and r.args.max, "value rule lacks min or max")
			return function(object) if not is_known(object) then return end if object_value_real(object) >= tonumber(%r.args.min) and object_value_real(object) <= tonumber(%r.args.max) then return true end end
		elseif r.label == "slotcomp" then
			assert(r.args.min or r.args.max, "compare rule needs at least one of min or max")
			return function(object) return auto_slot_compare(object, %r[1], %r.args.method, %r.args.exactness, %r.args.all, %r.args.min, %r.args.max) end
		elseif r.label == "found" then
			return function(object) return auto_found_type(object, %r[1]) end
		else
			local func = auto.clauses[r.label]
			assert(func, "Unknown automizer clause '" .. r.label .. "'")
			return func(r)
		end
	end
end

declare_global_constants"auto_destroy_maker"
function auto_destroy_maker(force)
    return function(...)
	arg.n = arg.n + 1
	arg[getn(arg)] = %force
	return call(auto_destroy, arg)
    end
end

declare_global_constants"auto_inscribe_maker"
function auto_inscribe_maker(inscription, mode)
    return function(...)
	arg.n = arg.n + 1
	arg[getn(arg)] = %inscription
	arg.n = arg.n + 1
	arg[getn(arg)] = %mode
	return call(auto_inscribe, arg)
    end
end

declare_global_constants"auto_skip_maker"
function auto_skip_maker(types)
    return function(...)
	arg.n = arg.n + 1
	arg[getn(arg)] = %types
	return call(auto_skip, arg)
    end
end

-- Generate a rule from a table
declare_global_constants"gen_full_rule"
function gen_full_rule(t)
	-- only honor rules for this module
	if not t.args.module then
		t.args.module = "ToME"
	end

	if not ((t.args.module == "all") or (t.args.module == game_module)) then
		return function() end
	end

	-- Check for which action to do
	local apply_fct = auto_nothing
	if t.args.type == "destroy" then
		apply_fct = auto_destroy_maker(t.args.force)
	elseif t.args.type == "pickup" then apply_fct = auto_pickup
	elseif t.args.type == "stop" then apply_fct = auto_stop
	elseif t.args.type == "inscribe" then
		apply_fct = auto_inscribe_maker(t.args.inscription, t.args.mode)
	elseif t.args.type == "skip" then
		apply_fct = auto_skip_maker(t.args.types)
	end

	-- create the function tree
	local rf
	if t[1] then
		rf = gen_rule_fct(t[1])
	else
		rf = function (object) end
	end

	-- create the final function
	return function(...)
		local rf = %rf
		if rf(arg[1]) then
			if call(%apply_fct, arg) == true then return true end
		end
	end
end

-- Create a function that checks for the rules(passed in xml form)
declare_global_constants"add_ruleset"
function add_ruleset(s)
	local tbl = xml:collect(s)
	local i

	-- Add all rules
	for i = 1, getn(tbl) do
		local t = tbl[i]

		if t.label == "rule" then
			-- Create the function tree
			local fct = gen_full_rule(t)

			-- Create the test function
			auto.rules[auto.rules_max] =
			{
				["table"] =	t,
				["fct"] =	fct
			}
			auto.rules_max = auto.rules_max + 1
		end
	end
end

-- Apply the current rules to an object
-- call with at least (object, idx)
declare_global_constants"apply_rules"
function apply_rules(...)
	local i
	local rule_applied = FALSE

	-- Resut list of rules to skip
	auto.rules_skip = {}

	for i = 0, auto.rules_max - 1 do
		-- Don't stop on the first rule that works.  This way, we can,
		-- for example, do both a auto-inscribe rule and an auto-pickup
		-- rule in one sweep
		auto.result.result = auto.result.CONTINUE
		if call(auto.rules[i].fct, arg) then
			rule_applied = true

			if auto.result.result == auto.result.STOP then
				return true
			end
		end

	end
	return rule_applied
end

-- Clear the current rules
declare_global_constants"clean_ruleset"
function clean_ruleset()
	auto.rules_max = 0
	auto.rules = {}
end

-----------------------------------------------------------------
-- helper functions for the GUI
-----------------------------------------------------------------

auto_aux = {}
auto_aux.stack = { n = 0 }
auto_aux.idx = 1
auto_aux.rule = 1
function auto_aux:go_right()
	if auto_aux.rule[1] and type(auto_aux.rule[1]) == "table" then
		tinsert(auto_aux.stack, auto_aux.idx)
		tinsert(auto_aux.stack, auto_aux.rule)
		auto_aux.rule = auto_aux.rule[1]
		auto_aux.idx = 1
	end
end

function auto_aux:go_left(sel)
	local n = getn(auto_aux.stack)

	if n > 0 then
		auto_aux.idx = auto_aux.stack[n - 1]
		auto_aux.rule = auto_aux.stack[n]
		tremove(auto_aux.stack)
		tremove(auto_aux.stack)
	end
end

function auto_aux:go_down()
	if getn(auto_aux.stack) > 1 then
		if auto_aux.stack[getn(auto_aux.stack)][auto_aux.idx + 1] then
			auto_aux.idx = auto_aux.idx + 1
			auto_aux.rule = auto_aux.stack[getn(auto_aux.stack)][auto_aux.idx]
		end
	end
end

function auto_aux:go_up()
	if getn(auto_aux.stack) > 1 then
		if auto_aux.stack[getn(auto_aux.stack)][auto_aux.idx - 1] then
			auto_aux.idx = auto_aux.idx - 1
			auto_aux.rule = auto_aux.stack[getn(auto_aux.stack)][auto_aux.idx]
		end
	end
end

function auto_aux:scroll_up()
	xml.write_off_y = xml.write_off_y - 1
end

function auto_aux:scroll_down()
	xml.write_off_y = xml.write_off_y + 1
end

function auto_aux:scroll_left()
	xml.write_off_x = xml.write_off_x + 1
end

function auto_aux:scroll_right()
	xml.write_off_x = xml.write_off_x - 1
end

function auto_aux:adjust_current(sel)
	if auto.rules_max == 0 then return end

	xml.write_off_y = 0
	xml.write_off_x = 0
	auto_aux.idx = 1
	auto_aux.stack = { n = 0 }
	auto_aux.rule = auto.rules[sel].table
end

function auto_aux:move_up(sel)
	if sel > 0 then
		local u = auto.rules[sel - 1]
		local d = auto.rules[sel]
		auto.rules[sel - 1] = d
		auto.rules[sel] = u
		return sel - 1
	end
	return sel
end

function auto_aux:move_down(sel)
	if sel < auto.rules_max - 1 then
		local u = auto.rules[sel]
		local d = auto.rules[sel + 1]
		auto.rules[sel + 1] = u
		auto.rules[sel] = d
		return sel + 1
	end
	return sel
end

function auto_aux:new_rule(sel, nam, typ, arg)
	local r


	-- nam can also directly be the table itself
	if type(nam) == "table" then
		r =
		{
			["table"] = nam,
			["fct"] = function (object) end
		}
	elseif typ == "inscribe" then
		if arg == "" then
		    arg = input_box("Inscription?", 79)
		end
		r =
		{
			["table"] =
			{
				label = "rule",
				args = { name = nam, module = game_module, type = typ, inscription = arg },
			},
			["fct"] = function (object) end
		}
	else
		r =
		{
			["table"] =
			{
				label = "rule",
				args = { name = nam, module = game_module, type = typ },
			},
			["fct"] = function (object) end
		}
	end
	tinsert(auto.rules, sel, r)
	auto.rules_max = auto.rules_max + 1
end

function auto_aux:rename_rule(sel, nam)
	if sel >= 0 and sel < auto.rules_max then
		auto.rules[sel].table.args.name = nam
	end
end

function auto_aux:save_ruleset()
	xml.write = xml.write_file

	hook.print("clean_ruleset()\nadd_ruleset\n[[\n")
	local i
	for i = 0, auto.rules_max - 1 do
		xml:print_xml(auto.rules[i].table, '')
	end
	hook.print("]]\n")

	xml.write = xml.write_screen
end

function auto_aux:del_self(sel)
	if auto_aux.rule.label == "rule" then
		tremove(auto.rules, sel)
		auto.rules_max = auto.rules_max - 1
		return sel - 1
	else
		local idx = auto_aux.idx
		auto_aux:go_left(sel)
		tremove(auto_aux.rule, idx)
		return sel
	end
end

auto_aux.types_desc =
{
	["and"] =
	{
		"Check is true if all rules within it are true",
		xml:collect([[<and><foo1>...</foo1><foo2>...</foo2><foo3>...</foo3></and>]]),
		function ()
			return xml:collect("<and></and>")
		end,
	},
	["or"] =
	{
		"Check is true if at least one rule within it is true",
		xml:collect([[<or><foo1>...</foo1><foo2>...</foo2><foo3>...</foo3></or>]]),
		function ()
			return xml:collect("<or></or>")
		end,
	},
	["not"] =
	{
		"Invert the result of its child rule",
		xml:collect([[<not><foo1>...</foo1></not>]]),
		function ()
			return xml:collect("<not></not>")
		end,
	},
	["comment"] =
	{
		"Comments are meaningless",
		xml:collect([[<comment>Comment explaining something</comment>]]),
		function ()
			local n = input_box("Comment?", 79)
			if n == "" then return end
			return xml:collect("<comment>"..n.."</comment>")
	       	end,
	},
	["name"] =
	{
		"Check is true if object name matches name",
		xml:collect([[<name>potion of healing</name>]]),
		function ()
			local n = input_box("Object name to match?", 79)
			if n == "" then return end
			return xml:collect("<name>"..n.."</name>")
	       	end,
	},
	["contain"] =
	{
		"Check is true if object name contains word",
		xml:collect([[<contain>healing</contain>]]),
		function ()
			local n = input_box("Word to find in object name?", 79)
			if n == "" then return end
			return xml:collect("<contain>"..n.."</contain>")
	       	end,
	},
	["inscribed"] =
	{
		"Check is true if object inscription contains word",
		xml:collect([[<inscribed>=g</inscribed>]]),
		function ()
			local n = input_box("Word to find in object inscription?", 79)
			if n == "" then return end
			return xml:collect("<inscribed>"..n.."</inscribed>")
	       	end,
	},
	["discount"] =
	{
		"Check is true if object discount is between 2 values",
		xml:collect([[<sval min='50' max='100'></sval>]]),
		function ()
			local s = "<discount "

			local n = input_box("Min discount?", 79)
			if n == "" then return end
			s = s.."min='"..n.."' "

			n = input_box("Max discount?", 79)
			if n == "" then return end
			s = s.."max='"..n.."'></discount>"
			return xml:collect(s)
	       	end,
	},
	["symbol"] =
	{
		"Check is true if object symbol is ok",
		xml:collect([[<symbol>!</symbol>]]),
		function ()
			local n = input_box("Symbol to match?", 1)
			if n == "" then return end
			return xml:collect("<symbol>"..n.."</symbol>")
	       	end,
	},
	["status"] =
	{
		"Check is true if object status is ok",
		xml:collect([[<status>good</status>]]),
		function ()
			local n = msg_box("[t]errible, [v]ery bad, [b]ad, [a]verage, [G]ood, [V]ery good, [S]pecial?")
			local t =
			{
				["t"] = "terrible",
				["v"] = "very bad",
				["b"] = "bad",
				["a"] = "average",
				["G"] = "good",
				["V"] = "very good",
				["S"] = "special",
			}
			if not t[strchar(n)] then return end
			return xml:collect("<status>"..t[strchar(n)].."</status>")
	       	end,
	},
	["state"] =
	{
		"Check is true if object is identified/unidentified",
		xml:collect([[<state>identified</state>]]),
		function ()
			local n = msg_box("[i]dentified, [p]seudo-id'd, [n]on identified?")
			local t =
			{
				["i"] = "identified",
				["p"] = "sensed",
				["n"] = "not identified",
			}
			if not t[strchar(n)] then return end
			return xml:collect("<state>"..t[strchar(n)].."</state>")
	       	end,
	},
	["tval"] =
	{
		"Check is true if object tval(from k_info.txt) is ok",
		xml:collect([[<tval>55</tval>]]),
		function ()
			local n = input_box("Tval to match?", 79)
			if n == "" then return end
			return xml:collect("<tval>"..n.."</tval>")
	       	end,
	},
	["sval"] =
	{
		{
			"Check is true if object sval(from k_info.txt) is between",
			"2 values",
		},
		xml:collect([[<sval min='0' max='100'></sval>]]),
		function ()
			local s = "<sval "

			local n = input_box("Min sval?", 79)
			if n == "" then return end
			s = s.."min='"..n.."' "

			n = input_box("Max sval?", 79)
			if n == "" then return end
			s = s.."max='"..n.."'></sval>"
			return xml:collect(s)
	       	end,
	},
	["race"] =
	{
		"Check is true if player race is ok",
		xml:collect([[<race>dunadan</race>]]),
		function ()
			local n = input_box("Player race to match?", 79)
			if n == "" then return end
			return xml:collect("<race>"..n.."</race>")
	       	end,
	},
	["subrace"] =
	{
		"Check is true if player subrace is ok",
		xml:collect([[<subrace>vampire</subrace>]]),
		function ()
			local n = input_box("Player subrace to match?", 79)
			if n == "" then return end
			return xml:collect("<subrace>"..n.."</subrace>")
	       	end,
	},
	["class"] =
	{
		"Check is true if player class is ok",
		xml:collect([[<class>sorceror</class>]]),
		function ()
			local n = input_box("Player class to match?", 79)
			if n == "" then return end
			return xml:collect("<class>"..n.."</class>")
	       	end,
	},
	["level"] =
	{
		"Check is true if player level is between 2 values",
		xml:collect([[<level min='20' max='50'></level>]]),
		function ()
			local s = "<level "

			local n = input_box("Min player level?", 79)
			if n == "" then return end
			s = s.."min='"..n.."' "

			n = input_box("Max player level?", 79)
			if n == "" then return end
			s = s.."max='"..n.."'></level>"

			return xml:collect(s)
	       	end,
	},
	["skill"] =
	{
		"Check is true if player skill level is between 2 values",
		xml:collect([[<skill min='10' max='20'>Divination</skill>]]),
		function ()
			local s = "<skill "

			local n = input_box("Min skill level?", 79)
			if n == "" then return end
			s = s.."min='"..n.."' "

			n = input_box("Max skill level?", 79)
			if n == "" then return end
			s = s.."max='"..n.."'>"

			n = input_box("Skill name?", 79)
			if n == "" then return end
			if find_skill_i(n) == -1 then return end
			s = s..n.."</skill>"

			return xml:collect(s)
	       	end,
	},
	["ability"] =
	{
		"Check is true if player has the ability",
		xml:collect([[<ability>Ammo creation</ability>]]),
		function()
			local n = input_box("Ability name?", 79)
			if n == "" then return end
			if find_ability(n) == -1 then return end
			return xml:collect("<ability>"..n.."</ability>")
		end,
	},
	["inventory"] =
	{
		{
			"Check is true if something in player's inventory matches",
			"the contained rule",
		},
		xml:collect([[<inventory><foo1>...</foo1></inventory>]]),
		function ()
			return xml:collect("<inventory></inventory>")
		end,
	},
	["equipment"] =
	{
		{
			"Check is true if something in player's equipment matches",
			"the contained rule",
		},
		xml:collect([[<equipment><foo1>...</foo1></equipment>]]),
		function ()
			return xml:collect("<equipment></equipment>")
		end,
	},
}

function auto_aux:display_desc(sel)
	local d = auto_aux.types_desc[sel][1]
	if type(d) == "string" then
		term.blank_print(color.WHITE, d, 1, 17)
	else
		local k, e, i
		i = 0
		for k, e in d do
			term.blank_print(color.WHITE, e, 1 + i, 17)
			i = i + 1
		end
	end
end

function auto_aux:add_child(sel)
	-- <rule> and <not> contain only one match
	if (auto_aux.rule.label == "rule" or auto_aux.rule.label == "not") and auto_aux.rule[1] then return end

	-- Only <and> and <or> can contain
	if auto_aux.rule.label ~= "rule" and auto_aux.rule.label ~= "and" and auto_aux.rule.label ~= "or"  and auto_aux.rule.label ~= "not" then return end

	-- get it
	local r = auto_aux.types_desc[sel][3]()
	if not r then return end

	-- Ok add it
	tinsert(auto_aux.rule, r[1])
end

function auto_aux.regen_ruleset()
	local i
	for i = 0, auto.rules_max - 1 do
		auto.rules[i].fct = gen_full_rule(auto.rules[i].table)
	end
end


-- Easily add new rules
declare_global_constants"easy_add_rule"
function easy_add_rule(typ, mode, do_status, obj)
	local detect_rule

	if mode == "tval" then
		detect_rule = "<tval>"..obj.tval.."</tval>"
	elseif mode == "tsval" then
		detect_rule = "<and><tval>"..obj.tval.."</tval><sval min='"..obj.sval.."' max='"..obj.sval.."'></sval></and>"
	elseif mode == "name" then
		detect_rule = "<name>"..strlower(object_desc(obj, -1, 0)).."</name>"
	end

	if do_status == true then
		local quality = object_quality(obj)
		if quality and not (quality == "") then
			detect_rule = "<and>"..detect_rule.."<quality>"..quality.."</quality></and>"
		end
	end

	local rule = "<rule module='"..game_module.."' name='"..typ.."' type='"..typ.."'>"..detect_rule.."</rule>"
	auto_aux:new_rule(0, xml:collect(rule)[1], '')
	auto_aux.regen_ruleset()
	message("Rule added. Please go to the Automatizer screen (press = then T)")
	message("to save the modified ruleset.")
end
