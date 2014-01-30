-- Stuff for loading infofiles

-- Default flag getters
global("getter", {})

-- Functions registry accessor
constant("get_function_registry", function(domain, ref)
	return __functions_registry[__info_cache[domain]][ref]
end)

-- Functions registry accessor
constant("get_function_registry_from_flag", function(flags, flag)
	return __functions_registry[flag_get(flags, flag)][flag_get2(flags, flag)]
end)

-- Serialized function accessor
constant("get_function_from_flag", function(flags, flag)
	return bytecode.load(flags[flag])
end)

-- Accessor for serialized *or* registry
constant("get_func_from_flag_or_registry", function(flags, flag)
	if flag_get2(flags, flag) ~= 0 then
		return __functions_registry[flag_get(flags, flag)][flag_get2(flags,
																	 flag)]
	else
		return bytecode.load(flags[flag])
	end
end)

constant("TAG_EXEC_FCT", newtag())

-- Random getter
getter.random = function(min, max)
	-- Min must be first
	if min > max then min, max = max, min end

	return { [2] = min, [1] = max, flags = FLAG_FLAG_RAND }
end

-- Mbonus getter
getter.mbonus = function(min, max)
	return { [2] = min, [1] = max, flags = FLAG_FLAG_MBONUS | FLAG_FLAG_RAND }
end

-- Random + mbonus getter
getter.rand_mbonus = function(rand, mbonus)
	return { [2] = rand, [1] = mbonus, flags = FLAG_FLAG_RAND_MBONUS | FLAG_FLAG_RAND }
end

-- Lite status
getter.lit = function(bright, dark)
	return { [2] = dark or -1, [1] = bright or -1, flags = FLAG_FLAG_BOTH }
end

getter.spell_chance = function(spell, chance)
	local spl = find_spell(spell)
	return { [1] = spl, [2] = chance }
end

getter.friends = function(chance, number)
	return { [1] = chance, [2] = number }
end

-- Flags getter
getter.flags = function(f)
	local flags = grab_flags(f, flag_new(), nil)
	return flags
end

-- Array getter
getter.array = function(array)
	local flags = flag_new()
	for k, v in array do
		flags[k] = v
	end
	return flags
end

-- Ego item kinds allowed
constant("TAG_EGO_ALLOWANCE", newtag())
getter.ego_allow = function(t)
	return settag(t, TAG_EGO_ALLOWANCE)
end

-- For a flagset who's keys are ASCII symbols
getter.chars = function(array)
	local flags = flag_new()
	for k, v in array do
		flags[strbyte(k)] = v
	end
	return flags
end

-- For letting multiple egos and the base object share a single flag
getter.__shared_flags_indexes = {}
getter.__get_new_shared_index = function(flag)
	local indexes = getter.__shared_flags_indexes
	local new

	if not indexes[flag] then
		indexes[flag] = 1
	end

	new = indexes[flag]

	indexes[flag] = indexes[flag] + 1

	return new
end

getter.shared_flag = function(flag, val)
	local array   = flag_new()

	array[getter.__get_new_shared_index(flag)] = val

	return array
end

-- HACK HACK HACK
getter.__auto_share_flags     = false
getter.auto_share_flags = function(val)
	getter.__auto_share_flags = val
end

-- AI states getter
getter.ai_states = function(f)
	return grab_flags(f, flag_new(), nil, function(name) return ai[name] end)
end

-- Speeds getter
getter.speeds = function(f)
	return grab_flags(f, flag_new(), nil, function(speed) return getglobal("SPEED_"..speed) end)
end

-- Skills getter
getter.skills = function(f)
	return grab_flags(f, flag_new(), nil,
				function(skill)
					local s = find_skill(skill)
					if s == -1 then
						s = getglobal("SKILL_"..skill)
					end
					return s
				end
	)
end

-- Spells getter
getter.spells = function(t)
	local res = flag_new()
	local nb = 1
	for i = 1, getn(t) do
		local spl = find_spell(t[i])
		if spl >= 0 then
			flag_set(res, spl, nb)
			nb = nb + 1
		else
			error("Unknown spell in getter.spells: "..t[i])
		end
	end
	return res
end

-- Spells getter
getter.limit_spells = function(t)
	local res = flag_new()
	for k, e in t do
		local spl = find_spell(k)
		if spl >= 0 then
			flag_set(res, spl, e)
		else
			error("Unknown spell in getter.limit_spells: "..k)
		end
	end
	return res
end

-- Action getter
getter.action = function(t)
	if type(t) == "number" then return t end
	if type(t) == "string" then
		return settag
		(
			{
				fct = function(obj)
					local idx = pop_object_action_index()
					obj.load_script = obj.load_script.."add_object_action("..idx..", "..%t..")\n"
					return idx
				end
			},
			TAG_EXEC_FCT
		)
	end
end

-- Adjust levels getter
getter.levels = function(min, max)
	return settag
	(
		{
			fct = function(dun)
				local idx = pop_levels_adjuster_index()
				dun.load_script = dun.load_script.."add_levels_adjuster("..idx..", '"..%min.."', '"..%max.."')\n"
				return idx
			end
		},
		TAG_EXEC_FCT
	)
end

-- Body parts getter
getter.body_parts = function(f)
	return grab_flags(f, flag_new(), nil, function(inven) return getglobal("INVEN_"..inven) end)
end

-- Skill getter
getter.skill = function(skill)
	local sk = find_skill(skill)
	assert(sk ~= -1, "Unknown skill "..skill)
	return sk
end

-- Object tval/sval getter
getter.object = function(tv, sv)
	return { [1] = tv, [2] = sv, flags = FLAG_FLAG_BOTH }
end

-- activation placeholders
getter.hardcore = function(tv, sv)
	return 1
end

-- Make an inventory
getter.inventory = function(size)
	local inven = flag_new(2)
	inven[0] = size
	return inven
end

-- A list of slots
getter.slots = function(f)
	return grab_flags(f, flag_new(), nil, function(id) return id end)
end

-- Resistances
getter.resists = function(f)
	return grab_flags(f, flag_new(), nil, function(res) return dam[res] end)
end

-- Absorbs, same as resistances
getter.absorbs = getter.resists

-- Stats
getter.stats = function(f)
	return grab_flags(f, flag_new(), nil, function(stat) return stat end)
end

-- Damages getter
getter.damages = function(f)
	return grab_flags(f, flag_new(), nil, function(res) return dam[res] end)
end

-- To make obvious flags
constant("TAG_OBVIOUS", newtag())
constant("obvious",
function(v)
	return settag({ val = v }, TAG_OBVIOUS)
end)

-- To delay putting function into function registry
new_flag("DELAY_REGISTER")
constant("TAG_DELAYED_REGISTER", newtag())
constant("delay_registry",
function(v)
	return settag({ val = v }, TAG_DELAYED_REGISTER)
end)
constant("__delayed_register_func_list", {})
constant("do_delayed_func_registry",
function(flags, source)
	if tag(flags) == TAG_FLAG then
		-- Need these two variables since we shouldn't change a flagset
		-- while iterating through its keys
		local flag_delay = { delete = false }
		local __funcs = flag_new()

		foreach_flags(flags,
			function(flags, key)
				local source  = %source
				local __funcs = %__funcs
				if key == FLAG_DELAY_REGISTER then
					local funcs = flag_get_flags(flags, key)

					foreach_flags(funcs,
						function(funcs, key)
							local func =
								__delayed_register_func_list[funcs[key]]
							local vv =
								get_flag_value["function"](func, %source)

							flag_set_full(%__funcs, key, FLAG_FLAG_BOTH,
										  vv[1], vv[2])
						end) -- foreach_flags()
					%flag_delay.delete = true
				else
					local node = flag_get_node(flags, key)

					if (node.flags & FLAG_FLAG_FLAGS) ~= 0 then
						do_delayed_func_registry(flag_get_flags(flags, key),
												 source)
					end
				end
			end) -- foreach_flags()

		if flag_delay.delete then
			flag_remove(flags, FLAG_DELAY_REGISTER)
			flag_add(flags, __funcs)
		end
		flag_free(__funcs, true)
	elseif type(flags) == "table" then
		for i, v in flags do
			if tag(v) == TAG_DELAYED_REGISTER then
				error("Error in delayed function registration with '" ..
					  (source.name or "unknown") .. "'")
			elseif tag(v) == TAG_FLAG or type(v) == "table" then
				do_delayed_func_registry(v, source)
			end
		end
	end
end) -- do_delayed_func_registry()

-- get the value of a flag
constant("get_flag_value",
{
	["function"] = function(v, source)
		-- Each object has 100 functions slots available, use the first
		-- free one
		if not source then
			if done_loading_data_files then
				-- If the data has finished loading, then the function
				-- can't go into the function registry, so instead we
				-- must dump the function's bytecode and store it
				-- directly into the flag
				return bytecode.dump_to_data_buffer(v)
			else
				-- Still loading fata files, but no source.  This must
				-- be a flagset that's nested in another flagset which
				-- *does* have a source.  Delay putting the function
				-- into the function registry until that flagset with
				-- source gets loaded, at which time the source will
				-- get propogated down to us, and we'll then know
				-- where to store the function in the registry.
				return delay_registry(v)
			end
		end

		local idx = source.__index__ * 100
		if not __functions_registry[source.__type__] then __functions_registry[source.__type__] = {} end
		while __functions_registry[source.__type__][idx] do idx = idx + 1 end
		__functions_registry[source.__type__][idx] = v
		return { [1] = source.__type__, [2] = idx, flags = FLAG_FLAG_BOTH }
	end
	["string"] = function(v) return v end
	["number"] = function(v) return v end
	["table"] = function(v, ...)
		if tag(v) == TAG_EXEC_FCT then
			return call(v.fct, arg)
		else
			return v
		end
	end
	["flags_type"] = function(v) return v end
})

-- Helper for grab_flags
constant("__grab_flags_do_set", function(flags, ff, vv, obvious)
	if type(vv) == "number" then
		flag_set(flags, ff, vv)
	elseif type(vv) == "string" then
		flag_set_ptr(flags, ff, FLAG_TYPE_STRING,
					 __wrap_ptr.dup_string_to_void(vv))
	elseif tag(vv) == TAG_DELAYED_REGISTER then
		local delayed_funcs = flag_get_flags(flags, FLAG_DELAY_REGISTER)

		if not delayed_funcs then
			delayed_funcs = flag_new()
			flag_set_full(flags, FLAG_DELAY_REGISTER, FLAG_FLAG_FLAGS,
						  0, 0, delayed_funcs)

		end

		tinsert(__delayed_register_func_list, vv.val)

		flag_set(delayed_funcs, ff,
				 getn(__delayed_register_func_list))
	elseif type(vv) == "table" then
		flag_set_full(flags, ff, vv.flags or FLAG_FLAG_BOTH, vv[1], vv[2])
	elseif tolua.type(vv) == "buffer_type" then
		flag_set_ptr(flags, ff, FLAG_TYPE_DATA_BUFFER, vv)
	elseif tolua.type(vv) == "flags_type" then
		flag_set_full(flags, ff, FLAG_FLAG_FLAGS, 0, 0, vv)
		if obvious then
			flag_learn_all(vv)
		end
	else
		error("Unknown type "..tolua.type(vv))
	end
	if obvious then flag_learn(flags, ff, true) end

end)

-- Grab flag
constant("grab_flags", function(flags_table, flags, source, resolver)
	if source then
		do_delayed_func_registry(flags_table, source)
	end

	if flags_table then
		for f, v in flags_table do
			local obvious = nil
			local vv = v
			local ff
			if resolver then
				ff = resolver(f)
			else
				ff = getglobal("FLAG_"..f)
			end
			if tag(vv) == TAG_OBVIOUS then
				obvious = true
				vv = vv.val
			end
			if not get_flag_value[tolua.type(vv)] then
				vv = get_flag_value[type(vv)](vv, source)
			else
				vv = get_flag_value[tolua.type(vv)](vv, source)
			end
			if __shared_flag[ff] and getter.__auto_share_flags then
				local container = flag_new()
				local index = getter.__get_new_shared_index(ff)
				__grab_flags_do_set(container, index, vv, obvious)
				flag_set_full(flags, ff, FLAG_FLAG_FLAGS, 0, 0, container)
			else
				__grab_flags_do_set(flags, ff, vv, obvious)
			end
			if obvious then flag_learn(flags, ff, true) end
		end
	end
	return flags
end)


--
-- Modifiers
--
global("mods", {})
mods.add = function(nb)
	return { __mego__.MEGO_ADD, nb }
end
mods.sub = function(nb)
	return { __mego__.MEGO_SUB, nb }
end
mods.percent = function(nb)
	return { __mego__.MEGO_PRC, nb }
end
mods.eq = function(nb)
	return { __mego__.MEGO_FIX, nb }
end
mods.convert = function(m)
	return lshift(m[2], 2) + m[1]
end

--
-- Used by new_item_kind() to do do item kind validation that's specific
-- to particular tvals
--
global("item_kind_validators", {})
constant("register_item_kind_validator",
function (validator, tvals)
	if type(tvals) == "number" then
		tvals = {tvals}
	end

	for i = 1, getn(tvals) do
		local tval = tvals[i]

		if item_kind_validators[tval] then
			error("Already validator for " .. tval)
		end

		item_kind_validators[tval] = validator
	end
end)

--
-- Used to indicate an item kind that is special and shouldn't have the
-- tval specific validator called on it
--
global("item_kind_validator_skips", {})
constant("item_kind_skip_validator",
function(tval, sval)
	if not item_kind_validator_skips[tval] then
		item_kind_validator_skips[tval] = {}
	end

	item_kind_validator_skips[tval][sval] = true
end)

--
-- k_info loader
--
global("__k_num", 1)
global("__k_info", {})
__k_info.data          = {}
__k_info.desc          = {}
__k_info.name          = {}
__k_info.symbol        = {}
__k_info.name_and_symb = {}
constant("new_item_kind",
function (t, defaults)
	assert(getter.__auto_share_flags,
		   "Automatic flag sharing needs to be on during item kind " ..
			   "creation")

	t.__type__ = __info_cache.k_info
	assert(t.name, "no name for new item kind")

	if strfind(t.name, "&") and not strfind(t.name, "& ") then
		error(t.name .. ": there must be a space after the '&'")
	end

	defaults = defaults or {}
	-- Get defaults, then copy actual values over the defaults
	local tmp = table_copy(defaults)
	if tmp.flags and t.default_flags_to_ignore then
		for k, v in t.default_flags_to_ignore do
			tmp.flags[k] = nil
		end
	end
	table_copy(tmp, t)
	t = tmp

	default_value(t, "desc", "")
	t.text = convert_desc(t.desc)

	assert(t.tval, "no tval for " .. t.name)
	assert(t.sval, "no sval for " .. t.name)

	if not t.cost then
		-- Default cost 0 so modules don't have to set it
		t.cost = 0
	end
	assert(t.weight, "no weight for " .. t.name)
	assert(t.level, "no level for " .. t.name)
	assert(t.display, "no display for " .. t.name)
	assert(t.color, "no color for " .. t.name)

	-- Do tval specific validations
	if item_kind_validators[t.tval] then
		if not (item_kind_validator_skips[t.tval] and
			item_kind_validator_skips[t.tval][t.sval]) then
			item_kind_validators[t.tval](t)
		end
	end

	if not t.__index__ then t.__index__ = __k_num end
	__k_num = max(t.__index__ + 1, __k_num)

	t.d_char = strbyte(t.display)
	t.d_attr = t.color

	t.locale = {0,0,0,0}
	t.chance = {0,0,0,0}
	if t.allocation then
		for i = 1, getn(t.allocation) do
			t.locale[i] = t.allocation[i][1]
			t.chance[i] = t.allocation[i][2]
		end
	end

	default_value(t, "dd", 0)
	default_value(t, "ds", 0)
	default_value(t, "ac", 0)
	default_value(t, "to_h", 0)
	default_value(t, "to_d", 0)
	default_value(t, "to_a", 0)

	default_value(t, "power", -1)

	if type(t.power) == "string" then
		local p = find_power(t.power)
		assert(p >= 0, "unkown power "..t.power)
		t.power = p
	end

	default_value(t, "btval", 0)
	default_value(t, "bsval", 0)

	if t.dice then t.dd = t.dice[1]; t.ds = t.dice[2] end

	-- Defaults to nothing to do
	default_value(t, "load_script", "")

	t.flags = grab_flags(t.flags, flag_new(), t)

	if t.tval_color then
		t.load_script = t.load_script..t.tval_color.."\n"
	end
	if t.tval_desc then
		t.load_script = t.load_script..t.tval_desc.."\n"
	end
	if t.tval_name then
		t.load_script = t.load_script..t.tval_name.."\n"
	end
	if t.tval_symbol then
		t.load_script = t.load_script..t.tval_symbol.."\n"
	end
	if t.tval_name_and_symb then
		t.load_script = t.load_script..t.tval_name_and_symb.."\n"
	end

	-- Need to define a variable to this index?
	if t.define_as then define_as(t) end
	if t.define_sval_as then define_as(t, "sval") end

	-- Now we are done, let's exec the scripts if needed
	dostring(t.load_script)

	tinsert(__k_info.data, t)

	-- If we are out of the normal loading, allow individual entries to be changed
	if done_loading_data_files then
		load_cache_object_kind(t)
	end

	return t.__index__
end)
constant("new_item_kinds",
function (t)
	getter.auto_share_flags(true)

	for tval, items in t do
		local color, name, desc = nil, nil, nil
		local symbol, name_and_symb = nil, nil
		local defaults = {}

		if items.color then
			color = "tval_to_attr["..(tval + 1).."] = "..items.color
			items.color = nil
		end

		-- Do name_and_symb before items.symbol is unset
		if items.name then
			if items.symbol then
				name_and_symb = "__k_info.name_and_symb["..tval.."] = " ..
					format("%q .. ' (' .. %q .. ')'", items.name, items.symbol)
			else
				name_and_symb = "__k_info.name_and_symb["..tval.."] = " ..
					format("%q", items.name)
			end
		end

		if items.symbol then
			if items.symbol ~= "NONE" then
				symbol = "__k_info.symbol["..tval.."] = " ..
					format("%q", items.symbol)
			end
			items.symbol = nil
		end

		if items.desc then
			desc = "__k_info.desc["..tval.."] = "..format("%q", convert_desc(items.desc))
			items.desc = nil
		end
		if items.name then
			name = "__k_info.name["..tval.."] = "..format("%q", items.name)
			items.name = nil
		end

		if items.defaults then
			defaults = items.defaults
			items.defaults = nil
		end

		for sval, item in items do
			if color then item.tval_color = color; color = nil end
			if desc then item.tval_desc = desc; desc = nil end
			if name then item.tval_name = name; name = nil end
			if symbol then item.tval_symbol = symbol; symbol = nil end
			if name_and_symb then
				item.tval_name_and_symb = name_and_symb
				name_and_symb = nil
			end
			item.tval = tval
			item.sval = sval
			if item.define_as then print(tval, item.define_as) end
			new_item_kind(item, defaults)
		end
	end

	getter.auto_share_flags(false)
end)

function __k_info.get_tval_info(info, tv)
	return __k_info[info][tv]
end

-- Default one
getter.auto_share_flags(true)
new_item_kind
{
	name = "something"
	display = '&' color = color.WHITE
	tval = 0 sval = 0
	cost = 0 weight = 0 level = 0
}
getter.auto_share_flags(false)

--
-- e_info loader
--
global("__e_num", 1)
global("__e_info", {})
__e_info.data = {}
constant("new_item_ego",
function (t)
	assert(getter.__auto_share_flags,
		   "Automatic flag sharing needs to be on during item ego " ..
			   "creation")

	t.__type__ = __info_cache.e_info
	assert(t.name, "no name for new item ego")
	default_value(t, "desc", "")
	t.text = convert_desc(t.desc)

	if t.name[1] == "before" then t.before = true end
	t.name = t.name[2]

	default_value(t, "value_add", 0)
	default_value(t, "base_value_pct", 100)
	assert(t.allocation, "no allocation for " .. t.name)
	assert(t.level, "no level for " .. t.name)
	assert(t.item_kinds, "no acceptable item_kinds for " .. t.name)

	t.ego_group = t.ego_group or "default"

	if not t.__index__ then t.__index__ = __e_num end
	__e_num = max(t.__index__ + 1, __e_num)

	t.max_to_h = t.to_h
	t.max_to_d = t.to_d
	t.max_to_a = t.to_a

	default_value(t, "power", -1)
	if type(t.power) == "string" then
		local p = find_power(t.power)
		assert(p >= 0, "unkown power "..t.power .. " for " .. t.name)
		t.power = p
	end

	default_value(t, "rating", 0)

	-- Load acceptable items
	t.tval = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }
	t.min_sval = {}
	t.max_sval = {}
        t.item_kinds_ok = flag_new()
	local nb = 1
	for tval, range in t.item_kinds do
		if tag(range) == TAG_EGO_ALLOWANCE then
                	t.item_kinds_ok[tval] = flag_new()
                	for i = 1, getn(range) do
                        	t.item_kinds_ok[tval][i] = range[i]
                        end
		elseif tag(range) == TAG_TABLE then
		      	t.tval[nb] = tval
	       		t.min_sval[nb] = range[1]
			t.max_sval[nb] = range[2]
		      	nb = nb + 1
		else
		      	t.tval[nb] = tval
			t.min_sval[nb] = 0
			t.max_sval[nb] = 255
		      	nb = nb + 1
		end
	end

	-- Defaults to nothing to do
	default_value(t, "load_script", "")

	-- Acceptance flags
	if t.need_flags then
        t.need_flags = grab_flags(t.need_flags, flag_new(), t)
	else
		t.need_flags = flag_new()
	end

	if t.forbid_flags then
        t.forbid_flags = grab_flags(t.forbid_flags, flag_new(), t)
	else
		t.forbid_flags = flag_new()
	end

	-- Flags
	local parse_flags = t.flags
	t.rar = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
	t.flags = { flag_new(), flag_new(), flag_new(), flag_new(), flag_new(), flag_new(), flag_new(), flag_new(), flag_new(), flag_new() }
	if parse_flags then
		local nb = 1
        	for rarity, flags in parse_flags do
			if type(rarity) == "number" then
				t.rar[nb] = rarity
				t.flags[nb] = grab_flags(flags, t.flags[nb], t)
				if t.flags[nb][FLAG_CHANCE] then
					t.rar[nb] = t.flags[nb][FLAG_CHANCE]
				end
				nb = nb + 1
			end
		end
	end

	-- Need to define a variable to this index?
	if t.define_as then define_as(t) end

	-- Now we are done, let's exec the scripts if needed
	dostring(t.load_script)

	tinsert(__e_info.data, t)

	-- If we are out of the normal loading, allow individual entries to be changed
	if done_loading_data_files then
		load_cache_ego_type(t)
	end

	return t.__index__
end)

--
-- a_info loader
--
global("__a_num", 1)
global("__a_info", {})
__a_info.data = {}
constant("new_artifact",
function (t)
	assert(getter.__auto_share_flags,
		   "Automatic flag sharing needs to be on during artifact " ..
			   "creation.")

	t.__type__ = __info_cache.a_info
	assert(t.name, "no name for new artifact")
	default_value(t, "desc", "")
	t.text = convert_desc(t.desc)

	assert(t.base_object, "no base object for " .. t.name)
	t.tval = t.base_object.tval
	t.sval = t.base_object.sval

	local k_idx = lookup_kind(t.tval, t.sval)

	assert(k_idx ~= 0, "Artifact " .. t.name .. " has a bad tval/sval combo")

	default_value(t, "cost", 0)
	assert(t.weight, "no weight for " .. t.name)
	assert(t.level, "no level for " .. t.name)
	assert(t.rarity, "no rarity for " .. t.name)

	if not t.__index__ then t.__index__ = __a_num end
	__a_num = max(t.__index__ + 1, __a_num)

	default_value(t, "d_attr", -1)
	default_value(t, "x_attr", -1)
	if t.color then
		t.d_attr = t.color
		t.x_attr = t.color
	end

	default_value(t, "dd", 0)
	default_value(t, "ds", 0)
	default_value(t, "ac", 0)
	default_value(t, "to_h", 0)
	default_value(t, "to_d", 0)
	default_value(t, "to_a", 0)

	default_value(t, "power", -1)

	if type(t.power) == "string" then
		local p = find_power(t.power)
		assert(p >= 0, "unkown power "..t.power .. " for " .. t.name)
		t.power = p
	end

	if t.dice then t.dd = t.dice[1]; t.ds = t.dice[2] end

	-- Defaults to nothing to do
	default_value(t, "load_script", "")

	t.flags = grab_flags(t.flags, flag_new(), t)

	-- Need to define a variable to this index?
	if t.define_as then define_as(t) end

	-- Now we are done, let's exec the scripts if needed
	dostring(t.load_script)

	tinsert(__a_info.data, t)

	-- If we are out of the normal loading, allow individual entries to be changed
	if done_loading_data_files then
		load_cache_artifact_type(t)
	end

	return t.__index__
end)

--
-- ra_info loader
--
global("__ra_num", 1)
global("__ra_info", {})
__ra_info.data = {}
constant("new_randart_part",
function (t)
	t.__type__ = __info_cache.ra_info
	assert(t.rarity, "no rarity")
	assert(t.min_level, "no min_level")
	assert(t.limit_number, "no limit_number")
	assert(t.generation_power, "no generation_power")
	assert(t.item_kinds, "no acceptable item_kinds")

	if not t.__index__ then t.__index__ = __ra_num end
	__ra_num = max(t.__index__ + 1, __ra_num)

	t.value = t.generation_power
	t.max = t.limit_number

	t.max_to_h = t.to_h
	t.max_to_d = t.to_d
	t.max_to_a = t.to_a

	default_value(t, "power", -1)
	if type(t.power) == "string" then
		local p = find_power(t.power)
		assert(p >= 0, "unkown power "..t.power)
		t.power = p
	end

	default_value(t, "rating", 0)

	t.mrarity = t.rarity[2]
	t.rarity = t.rarity[1]

	-- Load acceptable items
	t.tval = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }
	t.min_sval = {}
	t.max_sval = {}
	local nb = 1
	for tval, range in t.item_kinds do
		t.tval[nb] = tval
		t.min_sval[nb] = range[1]
		t.max_sval[nb] = range[2]
		nb = nb + 1
	end

	-- Defaults to nothing to do
	default_value(t, "load_script", "")

	-- Antagonist flags
	t.aflags = flag_new()
	if t.oppose_flags then t.aflags = grab_flags(t.oppose_flags, t.aflags, t) end

	-- Flags
	local flags = flag_new()
	if t.flags then flags = grab_flags(t.flags, flags, t) end
	t.flags = flags

	-- Need to define a variable to this index?
	if t.define_as then define_as(t) end

	-- Now we are done, let's exec the scripts if needed
	dostring(t.load_script)

	tinsert(__ra_info.data, t)

	-- If we are out of the normal loading, allow individual entries to be changed
	if done_loading_data_files then
		load_cache_randart_part_type(t)
	end

	return t.__index__
end)


--
-- ab_info loader
--
global("__ab_num", 0)
global("__ab_info", {})
__ab_info.data = {}
__ab_info.names = {}
constant("new_ability",
function(t)
	t.__type__ = __info_cache.ab_info
	assert(t.name, "No name for new ability")
	assert(t.desc, "No desc for " .. t.name)
	assert(t.cost, "No cost for " .. t.name)

	if not t.__index__ then t.__index__ = __ab_num end
	__ab_num = max(t.__index__ + 1, __ab_num)

	t.desc = convert_desc(t.desc, "\n")
	t.action_desc = { "", "" }
	t.action_mkey = { 0, 0 }

	if t.action then
		t.action_mkey[ACTIVATE_SKILL+1] = t.action[1]
		t.action_desc[ACTIVATE_SKILL+1] = t.action[2]

		if not t.action_mkey[ACTIVATE_SKILL+1] then
			t.action_mkey[ACTIVATE_SKILL+1] = __mkey_fct.__abilities[t.name]

			if not t.action_mkey[ACTIVATE_SKILL+1] then
				print("mkey needs to be implemented for ability " .. t.name)
			end
		end
	end

	if t.browse then
		t.action_mkey[BROWSE_SKILL+1] = t.browse[1]
		t.action_desc[BROWSE_SKILL+1] = t.browse[2]

		if not t.action_mkey[BROWSE_SKILL+1] then
			t.action_mkey[BROWSE_SKILL+1] = t.action_mkey[ACTIVATE_SKILL+1]
		end
	end

	t.stat = {-1,-1,-1,-1,-1,-1}
	if t.stats then
		for s, v in t.stats do
			t.stat[s + 1] = v
		end
	end
	local skills = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}
	local skill_levels = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}
	if t.skills then
		local i = 1
		for s, l in t.skills do
			local sk = s
			if type(sk) == "string" then sk = find_skill(sk) end
			skills[i] = sk
			skill_levels[i] = l
			i = i + 1
		end
	else
	end
	t.skills = skills
	t.skill_levels = skill_levels
	t.need_abilities = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}
	t.forbid_abilities = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}
	if t.abilities then
		local i = 1
		for a, ok in t.abilities do
			local ab = a
			if type(ab) == "string" then ab = __ab_info.names[ab] end
			if ok == true then
				t.need_abilities[i] = ab
			else
				t.forbid_abilities[i] = ab
			end
			i = i + 1
		end
	end

	if t.allow and type(t.allow) == "function" then
		local ret = get_flag_value["function"](t.allow, t)
		t.allow = ret[2]
	else
		t.allow = -1
	end

	-- Defaults to nothing to do
	default_value(t, "load_script", "")

	-- Need to define a variable to this index?
	if t.define_as then define_as(t) end

	-- Now we are done, let's exec the scripts if needed
	dostring(t.load_script)

	-- If we are out of the normal loading, allow individual entries to be changed
	if done_loading_data_files then
		load_cache_ability_type(t)
	end

	tinsert(__ab_info.data, t)
	__ab_info.names[t.name] = t.__index__
	return t.__index__
end)

--
-- Stores things
--
global("__ow_num", 0)
global("__ow_info", {})
__ow_info.data = {}
constant("new_store_owner",
function(t)
	t.__type__ = __info_cache.ow_info
	assert(t.name, "no name for new store owner")
	assert(t.max_cost, "no max cost for " .. t.name)
	assert(t.min_inflate, "no min inflate for " .. t.name)
	assert(t.max_inflate, "no max inflate for " .. t.name)
	assert(t.haggle_per, "no haggle for " .. t.name)
	assert(t.insult_max, "no insult max for " .. t.name)
	assert(t.costs, "no costs for " .. t.name)

	-- Defaults to nothing to do
	default_value(t, "load_script", "")

	if not t.__index__ then t.__index__ = __ow_num end
	__ow_num = max(t.__index__ + 1, __ow_num)

	-- Swap liked and normal
	t.costs[1], t.costs[2] = t.costs[2], t.costs[1]

	t.races = { flag_new(), flag_new() }
	t.classes = { flag_new(), flag_new() }
--[[ DGDGDGDG
	if t.hated then
		for _, name in t.hated do
			local race = find_race(name)
			local i = "races"
			if race == -1 then race = find_class(name); i = "classes" end
			assert(race ~= -1, "unknown race/class "..name)
			flag_set(t[i][1], race, true)
		end
	end
	if t.loved then
		for _, name in t.loved do
			local race = find_race(name)
			local i = "races"
			if race == -1 then race = find_class(name); i = "classes" end
			assert(race ~= -1, "unknown race/class "..name)
			flag_set(t[i][2], race, true)
		end
	end
]]

	-- Need to define a variable to this index?
	if t.define_as then define_as(t) end

	-- Now we are done, let's exec the scripts if needed
	dostring(t.load_script)


	-- If we are out of the normal loading, allow individual entries to be changed
	if done_loading_data_files then
		load_cache_owner_type(t)
	end

	tinsert(__ow_info.data, t)
	return t.__index__
end)

global("__ba_num", 0)
global("__ba_info", {})
__ba_info.next_action = 100
__ba_info.data = {}
constant("new_store_action",
function(t)
	t.__type__ = __info_cache.ba_info
	assert(t.name, "no name for new store action")
	assert(t.action, "no action for " .. t.name)
	assert(t.action_restr, "no action for " .. t.name)
	assert(t.letter, "no letter for " .. t.name)
	assert(t.costs, "no costs for" .. t.name)

	-- Defaults to nothing to do
	default_value(t, "load_script", "")

	if not t.__index__ then t.__index__ = __ba_num end
	__ba_num = max(t.__index__ + 1, __ba_num)

	default_value(t, "letter_aux", 0)
	t.letter     = strbyte(t.letter)
	t.letter_aux = strbyte(t.letter_aux)

	if t.action_restr == "all" then t.action_restr = 0
	elseif t.action_restr == "not hated" then t.action_restr = 1
	elseif t.action_restr == "loved" then t.action_restr = 2
	else
		error("Unknown action retriction for " .. t.name)
	end

	-- Automatic hooking!
	if type(t.action) == "string" then
		local exec = [[
		add_building_action
		{
			index = ]]..__ba_info.next_action..[[,
			action = ]]..t.action..[[
		}
]]
		dostring(exec)
		t.action = __ba_info.next_action
		__ba_info.next_action = __ba_info.next_action + 1
		t.load_script = exec
	elseif type(t.action) == "function" then
		local ret = get_flag_value["function"](t.action, t)
		t.action = -ret[2]
	end

	-- Need to define a variable to this index?
	if t.define_as then define_as(t) end

	if tag(t.costs) == TAG_FUNCTION then
		local ret = get_flag_value["function"](t.costs, t)
		assert(ret[2] < 32768, "Too many store actions for action cost to " ..
			   "be defeined as a function.")
		t.costs = {-ret[2]}
	elseif tag(t.costs) == TAG_TABLE then
		for i = 1, getn(t.costs) do
			assert(tag(t.costs[i]) == TAG_NUMBER,
				   "Store action costs must be a function or an array " ..
					   "of numbers (store action " .. t.name .. ")")
			assert(t.costs[i] >= 0, "Store actions costs may not be " ..
				   "negative (store action " .. t.name .. ")")
		end
		assert(getn(t.costs) == 3, "Store action " .. t.name ..
			   " must have only 3 entries in the cost table.")

		-- Everything checked out okay, no swap liked and normal
		t.costs[1], t.costs[2] = t.costs[2], t.costs[1]
	else
		error("Invalid type '" .. type(t.costs) .. " for store action " ..
			  "costs field (store action " .. t.name .. ")")
	end

	-- Now we are done, let's exec the scripts if needed
	dostring(t.load_script)

	-- If we are out of the normal loading, allow individual entries to be changed
	if done_loading_data_files then
		load_cache_store_action_type(t)
	end
	tinsert(__ba_info.data, t)
	return t.__index__
end)

global("__st_num", 0)
global("__st_info", {})
__st_info.data = {}
constant("new_store",
function(t)
	t.__type__ = __info_cache.st_info
	assert(t.name, "no name for new store")
	assert(t.actions, "no actions for " .. t.name)
	assert(t.owners, "no ownsers for " .. t.name)
	assert(t.display, "no display for " .. t.name)
	assert(t.color, "no color for " .. t.name)

	-- Defaults to nothing to do
	default_value(t, "load_script", "")

	if not t.__index__ then t.__index__ = __st_num end
	__st_num = max(t.__index__ + 1, __st_num)

	t.d_char = strbyte(t.display)
	t.d_attr = t.color

	default_value(t, "max_items", 24)
	t.max_objs = t.max_items

	-- Flags
	local flags = flag_new()
	if t.flags then flags = grab_flags(t.flags, flags, t) end
	t.flags = flags

	-- Allowed items
	t.table = flag_new()
	if t.item_kinds then
		local nb = 1
		for _, e in t.item_kinds do
			if type(e.item) == "number" then
				-- This is a tval, indicating any random item with
				-- this tval is okay.  Store as a negative number
				-- to make differant from k_info[] indexes.
				e.item = -e.item
			elseif type(e.item) == "string" then
				local str = e.item
				e.item = test_item_name(e.item)
				if e.item == 0 then
					print(t.name .. ": item '" .. str .. "' doesn't exist")
				end
			elseif type(e.item) == "table" then
				local tval, sval = e.item[1], e.item[2]
				e.item = lookup_kind(e.item[1], e.item[2])
				if e.item == 0 then
					sval = sval_to_str(tval, sval) or sval
					tval = tval_to_str(tval) or tval
					print(t.name .. ": tval/sval combo " .. tval ..
						  "/" .. sval .. " doesn't exist")
				end
			end

			if e.item ~= 0 then
				flag_set_full(t.table, nb, FLAG_FLAG_BOTH, e.item, e.chance)
				nb = nb + 1
			end
		end
	end

	-- Buyable items
	-- If a table, convert it to a function
	if type(t.buy) == "table" then
		local tvals_fct_code = nil
		for _, e in t.buy do
			if not tvals_fct_code then
				tvals_fct_code = "return function(obj) if obj.tval=="..e.." "
			else
				tvals_fct_code = tvals_fct_code .. "or obj.tval=="..e.." "
			end
		end
		tvals_fct_code = tvals_fct_code .. "then return true end end"
		t.buy = bytecode.load(tvals_fct_code)()
	end
	t.buy = get_flag_value["function"](t.buy, t)[2]

	-- If only one possible owner, then use it always
	if type(t.owners) == "number" then
		t.owners = { t.owners, t.owners, t.owners, t.owners }
	end

	-- Stock objects maker
	default_value(t, "store_maker", "table_reader")
	-- Apply a default 75% rule
	default_value(t, "store_make_num", (t.max_items * 75) / 100)

	-- Need to define a variable to this index?
	if t.define_as then define_as(t) end

	-- Now we are done, let's exec the scripts if needed
	dostring(t.load_script)

	-- If we are out of the normal loading, allow individual entries to be changed
	if done_loading_data_files then
		load_cache_store_info_type(t)
	end

	tinsert(__st_info.data, t)
	return t.__index__
end)

--
-- Skills
--

global("__s_num", 1)
global("__s_info", {})
__s_info.next_action = 100
__s_info.names = {}
__s_info.indexes = {}
__s_info.data = {}
__s_info.load_script = ""
constant("new_skill",
function(t)
	t.__type__ = __info_cache.s_info
	assert(t.name, "No name for new skill")
	assert(t.desc, "No desc for " .. t.name)

	if not t.__index__ then t.__index__ = __s_num end
	__s_num = max(t.__index__ + 1, __s_num)

	t.desc = convert_desc(t.desc, "\n")
	t.action_desc = { "", "" }
	t.action_mkey = { 0, 0 }

	if t.action then
		t.action_mkey[ACTIVATE_SKILL+1] = t.action[1]
		t.action_desc[ACTIVATE_SKILL+1] = t.action[2]

		if not t.action_mkey[ACTIVATE_SKILL+1] then
			t.action_mkey[ACTIVATE_SKILL+1] = __mkey_fct.__skills[t.name]

			if not t.action_mkey[ACTIVATE_SKILL+1] then
				print("mkey needs to be implemented for skill " .. t.name)
			end
		end
	end

	if t.browse then
		t.action_mkey[BROWSE_SKILL+1] = t.browse[1]
		t.action_desc[BROWSE_SKILL+1] = t.browse[2]

		if not t.action_mkey[BROWSE_SKILL+1] then
			t.action_mkey[BROWSE_SKILL+1] = t.action_mkey[ACTIVATE_SKILL+1]
		end
	end

	t.flags = grab_flags(t.flags, flag_new(), t)

	default_value(t, "friends", {})
	default_value(t, "exclude", {})

	-- Make a resolver
	local resolve_actions = function(friends, exclude)
		local ret = flag_new()
		local friends = %t.friends
		local exclude = %t.exclude
		for name, val in friends do
			assert(__s_info.names[name], "Unknown skill "..name)
			ret[__s_info.names[name] ] = val
		end
		for _, name in exclude do
			assert(__s_info.names[name], "Unknown skill "..name)
			ret[__s_info.names[name] ] = SKILL_EXCLUSIVE
		end
		return ret
	end

	-- Use it immediatly
	if done_loading_data_files then
		t.action = resolve_actions(t.friends, t.exclude)
	-- Use it latter
	else
		t.action = function()
			return %resolve_actions(%t.friends, %t.exclude)
		end
	end

	-- Defaults to nothing to do
	default_value(t, "load_script", "")

	t.order = t.__index__

	-- Need to define a variable to this index?
	if t.define_as then define_as(t) end

	-- Now we are done, let's exec the scripts if needed
	dostring(t.load_script)


	-- If we are out of the normal loading, allow individual entries to be changed
	if done_loading_data_files then
		load_cache_skill_type(t)
	end

	__s_info.names[t.name] = t.__index__
	__s_info.indexes[t.__index__] = t
	tinsert(__s_info.data, t)
	return t
end)

constant("set_skill_tree", function(t, parent)
	-- Resolve names
	for i = 1, getn(__s_info.data) do
		local s = __s_info.data[i]

		if type(s.action) == "function" then
			local act = s.action()
			s.action = act
		end
	end

	if not parent then parent = -1 end
	for name, subskills in t do
		local s, idx
		if done_loading_data_files then
			idx = find_skill(name)
			s = skill(idx)
		else
			idx = __s_info.names[name]
			s = __s_info.indexes[idx]
		end

		s.father = parent

		set_skill_tree(subskills, idx)
	end
end)

--
-- r_info loader
--
global("__r_num", 0)
global("__r_info", {})
__r_info.data = {}
constant("new_monster_race",
function (t, defaults)
	t.__type__ = __info_cache.r_info
	assert(t.name, "no name for new monster race")
	default_value(t, "desc", "")
	default_value(t, "freq_spell", mods.add(100))
	default_value(t, "freq_inate", mods.add(100))
	t.freq_spell = mods.convert(t.freq_spell)
	t.freq_inate = mods.convert(t.freq_inate)
	t.text = convert_desc(t.desc)

	if not t.__index__ then t.__index__ = __r_num end
	__r_num = max(t.__index__ + 1, __r_num)

	-- Get defaults, then copy actual values over the defaults
	defaults = defaults or {}
	local tmp = table_copy(defaults)
	if tmp.flags and t.default_flags_to_ignore then
		for k, v in t.default_flags_to_ignore do
			tmp.flags[k] = nil
		end
	end
	table_copy(tmp, t)
	t = tmp

	-- Defaults to nothing to do
	default_value(t, "load_script", "")

	assert(t.race, "no race for " .. t.name)
	assert(t.sval, "no sval for " .. t.name)
	assert(t.color, "no color for " .. t.name)
	t.d_char = strbyte(t.race)
	t.d_attr = t.color

	assert(t.speed, "no speed for " .. t.name)
	assert(t.weight, "no weight for " .. t.name)
	assert(t.level, "no level for " .. t.name)
	assert(t.ac, "no ac for " .. t.name)
	assert(t.sleep, "no sleep for " .. t.name)
	assert(t.life, "no life for " .. t.name)
	assert(t.exp, "no exp for " .. t.name)
	assert(t.rarity, "no rarity for " .. t.name)

	t.mexp = t.exp

	t.speed = 110 + t.speed

	t.hdice = t.life[1]
	t.hside = t.life[2]

	t.flags = grab_flags(t.flags, flag_new(), t)

	if t.flags[FLAG_AI_STATES] then
		assert(tag(t.flags[FLAG_AI_STATES]) == TAG_FLAG,
			   "AI_STATES for " .. t.name .. " is not a flagset")
	end

	if t.flags[FLAG_CAN_PASS] then
		assert(tag(t.flags[FLAG_CAN_PASS]) == TAG_FLAG,
			   "CAN_PASS for " .. t.name .. " is not a flagset")
	end

	default_value(t, "drops", { treasure=20 combat=20 magic=20 tools=20 })
	default_value(t, "body", { INVEN=23 })
	t.body_parts = getter.body_parts(t.body)
	t.obj_theme = t.drops

	local spells = flag_new()
	if t.spells then
		-- Spells frequency
		if t.spells.frequency  then
			t.freq_spell = 100 / t.spells.frequency
			t.freq_inate = 100 / t.spells.frequency
		end

		-- Spells
		for name, info in t.spells do
			if name ~= "frequency" then
				local spl = find_spell(name)
				assert(spl ~= -1, "Unknown spell "..name.." in "..t.race.."/"..t.sval)
				flag_set_full(spells, spl, FLAG_FLAG_BOTH, info.level, info.chance)
			end
		end
	end
	t.spells = spells

	t.blow = flag_new(4)
	if t.blows then
		for nb = 1, getn(t.blows) do
			local attack = t.blows[nb]

			local b = monster_blow_new()

			b.d_dice = attack[3][1]
			b.d_side = attack[3][2]
			b.method = find_monster_attack_method(attack[1])
			b.effect = find_monster_attack_type(attack[2])

			flag_set_ptr(t.blow, nb - 1, FLAG_TYPE_MONSTER_BLOW, b);
		end
	end

	-- Finish the flags
	-- WILD_TOO without any other wilderness flags enables all flags
	if has_flag(t, FLAG_WILD_TOO) then
		bool_flag(t, FLAG_DUNGEON)
		bool_flag(t, FLAG_WILD_SHORE)
		bool_flag(t, FLAG_WILD_OCEAN)
		bool_flag(t, FLAG_WILD_WASTE)
		bool_flag(t, FLAG_WILD_WOOD)
		bool_flag(t, FLAG_WILD_VOLCANO)
		bool_flag(t, FLAG_WILD_MOUNTAIN)
		bool_flag(t, FLAG_WILD_SWAMP)
		bool_flag(t, FLAG_WILD_GRASS)
	end

	if ((not has_flag(t, FLAG_DUNGEON)) and
                       (not has_flag(t, FLAG_WILD_TOWN)) and
                       (not has_flag(t, FLAG_WILD_SHORE)) and
                       (not has_flag(t, FLAG_WILD_OCEAN)) and
                       (not has_flag(t, FLAG_WILD_WASTE)) and
                       (not has_flag(t, FLAG_WILD_WOOD)) and
                       (not has_flag(t, FLAG_WILD_VOLCANO)) and
                       (not has_flag(t, FLAG_WILD_MOUNTAIN)) and
                       (not has_flag(t, FLAG_WILD_SWAMP)) and
                       (not has_flag(t, FLAG_WILD_GRASS))) then
		bool_flag(t, FLAG_DUNGEON)
	end

	-- Need to define a variable to this index?
	if t.define_as then define_as(t) end

	-- Now we are done, let's exec the scripts if needed
	dostring(t.load_script)

	tinsert(__r_info.data, t)

	-- If we are out of the normal loading, allow individual entries to be changed
	if done_loading_data_files then
		load_cache_monster_race(t)
	end

	return t.__index__
end)
constant("new_monster_races_sval_counters", {})
constant("new_monster_races",
function (t)
	for d_char, races in t do
		local defaults = {}

		if races.defaults then
			defaults       = races.defaults
			races.defaults = nil
		end

		for i, race in races do
			race.race = d_char
			local sval = new_monster_races_sval_counters[d_char]
			if not sval then
				sval = 1
				new_monster_races_sval_counters[d_char] = 2
			else
				new_monster_races_sval_counters[d_char] = new_monster_races_sval_counters[d_char] + 1
			end
			race.sval = sval
			new_monster_race(race, defaults)
		end
	end
end)

--
-- d_info loader
--
global("__d_num", 0)
global("__d_info", {})
__d_info.data = {}
__d_info.levels = {}
constant("new_dungeon_type",
function (t)
	t.__type__ = __info_cache.d_info
	assert(t.name, "no name for new dungeon type")
	default_value(t, "desc", "")
	t.text = convert_desc(t.desc)
	local short_name = {}
	short_name[1] = strbyte(t.short_name, 1)
	short_name[2] = strbyte(t.short_name, 2)
	short_name[3] = strbyte(t.short_name, 3)
	t.short_name = short_name

	if not t.__index__ then t.__index__ = __d_num end
	__d_num = max(t.__index__ + 1, __d_num)

	assert(t.min_monsters, "no min_monsters for " .. t.name)
	assert(t.alloc_chance, "no alloc_chance for " .. t.name)
	assert(t.mindepth, "no mindepth for " .. t.name)
	assert(t.maxdepth, "no maxdepth for " .. t.name)
	assert(t.min_player_level, "no min_player_level for " .. t.name)
	t.min_plev = t.min_player_level

	t.min_m_alloc_level = t.min_monsters
	t.max_m_alloc_chance = t.alloc_chance

	-- Defaults to nothing to do
	default_value(t, "load_script", "")

	t.flags = grab_flags(t.flags, flag_new(), t)

	default_value(t, "generator", "dungeon")

	default_value(t, "size_x", 3)
	default_value(t, "size_y", 3)
	default_value(t, "fill_method", 1)

	if t.entries then
		if t.entries.top then
			t.ix = t.entries.top.x
			t.iy = t.entries.top.y
		end

		if t.entries.bottom then
			t.ox = t.entries.bottom.x
			t.oy = t.entries.bottom.y
		end
	end
	default_value(t, "ix", -1)
	default_value(t, "iy", -1)
	default_value(t, "ox", -1)
	default_value(t, "oy", -1)

	-- Floors
	t.floor1 = 0
	t.floor_percent1 = { 0, 0 }
	t.floor2 = 0
	t.floor_percent2 = { 0, 0 }
	t.floor3 = 0
	t.floor_percent3 = { 0, 0 }
	local nb = 1
	for feat, chances in t.floors do
		local c = chances
		if type(c) == "number" then c = { c, c } end
		t["floor"..nb] = feat
		t["floor_percent"..nb] = c
		nb = nb + 1
	end

	-- Walls
	t.fill_type1 = 0
	t.fill_percent1 = { 0, 0 }
	t.fill_type2 = 0
	t.fill_percent2 = { 0, 0 }
	t.fill_type3 = 0
	t.fill_percent3 = { 0, 0 }
	local nb = 1
	for feat, chances in t.walls do
		if feat == "inner" then
			t.inner_wall = chances
		elseif feat == "outer" then
			t.outer_wall = chances
		else
			local c = chances
			if type(c) == "number" then c = { c, c } end
			t["fill_type"..nb] = feat
			t["fill_percent"..nb] = c
			nb = nb + 1
		end
	end

	default_value(t, "objects", { treasure=20 combat=20 magic=20 tools=20 })
	t.objs = t.objects

	-- Attacks
	t.d_dice = {0,0,0,0}
	t.d_side = {0,0,0,0}
	t.d_type = {0,0,0,0}
	t.d_frequency = {0,0,0,0}
	if t.attacks then
		local nb = 1
		for attack, dice in t.attacks do
			t.d_dice[nb] = dice[1]
			t.d_side[nb] = dice[2]
			t.d_type[nb] = attack[1]
			t.d_frequency[nb] = attack[2]
			nb = nb + 1
		end
	end

	-- Rules
	local rules =
	{
		{ mode = DUNGEON_MODE_NONE, percent = 100, r_char = {}, flags = flag_new(), spells = flag_new() },
		{ mode = DUNGEON_MODE_NONE, percent = 0, r_char = {}, flags = flag_new(), spells = flag_new() },
		{ mode = DUNGEON_MODE_NONE, percent = 0, r_char = {}, flags = flag_new(), spells = flag_new() },
		{ mode = DUNGEON_MODE_NONE, percent = 0, r_char = {}, flags = flag_new(), spells = flag_new() },
		{ mode = DUNGEON_MODE_NONE, percent = 0, r_char = {}, flags = flag_new(), spells = flag_new() },
	}
	local cvt =
	{
		["all"] = DUNGEON_MODE_NONE,
		["and"] = DUNGEON_MODE_AND,
		["not and"] = DUNGEON_MODE_NAND,
		["or"] = DUNGEON_MODE_OR,
		["not or"] = DUNGEON_MODE_NOR,
	}
	local nb = 1
	if t.rules then
		nb = 0
		for typ, rule in t.rules do
			nb = nb + 1
			rules[nb].mode = cvt[typ[2]]
			rules[nb].percent = typ[1]
			if rule.races then
				rules[nb].r_char = {}
				for i = 1, getn(rule.races) do
					rules[nb].r_char[i] = strbyte(rule.races[i])
				end
			end
			if rule.flags then
				rules[nb].flags = grab_flags(rule.flags, rules[nb].flags, rule)
			end
			if rule.spells then
				for _, name in rule.spells do
					local spl = find_spell(name)
					assert(spl ~= -1, "Unknown spell "..name)
					flag_set(rules[nb].spells, spl, 1)
				end
			end
		end
	end
	t.rules = rules

	-- Lets remap the flat percents
	t.rule_percents = {}
	local lims = { 0, 0, 0, 0, 0 }
	lims[1] = t.rules[1].percent
	for y = 2, nb do
		lims[y] = lims[y - 1] + t.rules[y].percent
	end
	for z = 0, 99 do
		t.rule_percents[z + 1] = 0
		for y = nb, 1, -1 do
			if z < lims[y] then t.rule_percents[z + 1] = y - 1 end
		end
	end

	-- Specific levels
	if t.levels then
		-- Convert functions first
		for level, data in t.levels do
			if data.flags then
				for k, e in data.flags do
					if tag(e) == TAG_FUNCTION then
						data.flags[k] = get_flag_value["function"](e, t)
					end
				end
			end
		end

		local save = pickle(t.levels)
		t.load_script = t.load_script.."\n__d_info.levels["..t.__index__.."]=unpickle([["..save.."]])\n"
	end

	-- Need to define a variable to this index?
	if t.define_as then define_as(t) end

	-- Now we are done, let's exec the scripts if needed
	dostring(t.load_script)

	tinsert(__d_info.data, t)

	-- If we are out of the normal loading, allow individual entries to be changed
	if done_loading_data_files then
		load_cache_dungeon_info_type(t)
	end

	return t.__index__
end)
function __d_info.get_level_info(dungeon, level, info)
	-- Grab from wilderness feat
	if (dun_level == 0) and (not player.wild_mode) and (info == "flags") then
		local w = wild_map(player.wilderness_y, player.wilderness_x)
		flag_add(level_flags.flags, wf_info[1 + w.feat].flags)
	elseif __d_info.levels[dungeon] and __d_info.levels[dungeon][level] and __d_info.levels[dungeon][level][info] then
		if info == "flags" then
			-- If in a dungeon
			level_flags.flags(__d_info.levels[dungeon][level][info])
			return nil
		else
			return __d_info.levels[dungeon][level][info]
		end
	else
		return nil
	end
end

--
-- f_info loader
--
global("__f_num", 0)
global("__f_info", {})
__f_info.data = {}
constant("new_feature_type",
function (t)
	t.__type__ = __info_cache.f_info
	assert(t.name, "no name for new feature type")
	default_value(t, "text", "")
	default_value(t, "tunnel", "You cannot tunnel through that.")
	default_value(t, "tunnel_done", "You have finished the tunnel.")
	default_value(t, "block", "a wall blocking your way")

	if not t.__index__ then t.__index__ = __f_num end
	__f_num = max(t.__index__ + 1, __f_num)

	if t.on_walk then t.text = t.on_walk end
	if t.on_tunnel then t.tunnel = t.on_tunnel end
	if t.on_tunnel_done then t.tunnel_done = t.on_tunnel_done end
	if t.on_block then t.block = t.on_block end

	assert(t.display, "no display for " .. t.name)
	assert(t.color, "no color for " .. t.name)

	t.d_char = strbyte(t.display)
	t.d_attr = t.color

	default_value(t, "priority", 20)

	t.shimmer = {}
	if t.shimmers then t.shimmer = t.shimmers end

	-- Defaults to nothing to do
	default_value(t, "load_script", "")

	-- Now grab the flags
	t.flags = grab_flags(t.flags, flag_new(), t)

	-- Is there a special can enter function?
	if t.can_enter and type(t.can_enter) == "function" then
		local ret = get_flag_value["function"](t.can_enter, t)
		t.can_enter = ret[2]
	else
		t.can_enter = -1
	end

	-- Attacks
	t.d_dice = {0,0,0,0}
	t.d_side = {0,0,0,0}
	t.d_type = {0,0,0,0}
	t.d_frequency = {0,0,0,0}
	if t.attacks then
		local nb = 1
		for attack, dice in t.attacks do
			if not attack[1] or attack[1] == 0 then
				message(color.VIOLET,
						"Feature " .. t.name .. " has undefined attack type!")
			end
			t.d_dice[nb] = dice[1]
			t.d_side[nb] = dice[2]
			t.d_type[nb] = attack[1]
			t.d_frequency[nb] = attack[2]
			nb = nb + 1
		end
	end

	-- Need to define a variable to this index?
	if t.define_as then define_as(t) end

	-- Now we are done, let's exec the scripts if needed
	dostring(t.load_script)

	default_value(t, "mimic", t.__index__)

	tinsert(__f_info.data, t)

	-- If we are out of the normal loading, allow individual entries to be changed
	if done_loading_data_files then
		load_cache_feature_type(t)
	end
	return t.__index__
end)

-- Add a opened/closed doors pair
-- This simply adds the OPEN and CLOSED falg to both of them
constant("new_open_pair_feature_type",
function (t)
	local open_idx = new_feature_type(t.open)
	local open = __f_info.data[getn(__f_info.data)]

	local close_idx = new_feature_type(t.close)
	local close = __f_info.data[getn(__f_info.data)]

	open.flags[FLAG_OPEN]    = close_idx
	close.flags[FLAG_CLOSED] = open_idx
end)

--
-- wf_info loader
--
global("__wf_num", 0)
global("__wf_info", {})
__wf_info.data = {}
constant("new_wilderness_terrain",
function (t)
	t.__type__ = __info_cache.wf_info
	assert(t.name, "no name for new wilderness terrain")
	assert(t.long_name, "no long name for " .. t.name)
	t.text = t.long_name

	if not t.__index__ then t.__index__ = __wf_num end
	__wf_num = max(t.__index__ + 1, __wf_num)

	-- Defaults to nothing to do
	default_value(t, "load_script", "")

	t.terrain = t.generator

	t.flags = grab_flags(t.flags, flag_new(), t)

	t.terrain_idx = t.type

	t.d_char = strbyte(t.char)

	-- Rules
	local rules =
	{
		{ mode = DUNGEON_MODE_NONE, percent = 100, r_char = {}, flags = flag_new(), spells = flag_new() },
		{ mode = DUNGEON_MODE_NONE, percent = 0, r_char = {}, flags = flag_new(), spells = flag_new() },
		{ mode = DUNGEON_MODE_NONE, percent = 0, r_char = {}, flags = flag_new(), spells = flag_new() },
		{ mode = DUNGEON_MODE_NONE, percent = 0, r_char = {}, flags = flag_new(), spells = flag_new() },
		{ mode = DUNGEON_MODE_NONE, percent = 0, r_char = {}, flags = flag_new(), spells = flag_new() },
	}
	local cvt =
	{
		["all"] = DUNGEON_MODE_NONE,
		["and"] = DUNGEON_MODE_AND,
		["not and"] = DUNGEON_MODE_NAND,
		["or"] = DUNGEON_MODE_OR,
		["not or"] = DUNGEON_MODE_NOR,
	}
	local nb = 1
	if t.rules then
		nb = 0
		for typ, rule in t.rules do
			nb = nb + 1
			rules[nb].mode = cvt[typ[2]]
			rules[nb].percent = typ[1]
			if rule.races then
				rules[nb].r_char = {}
				for i = 1, getn(rule.races) do
					rules[nb].r_char[i] = strbyte(rule.races[i])
				end
			end
			if rule.flags then
				rules[nb].flags = grab_flags(rule.flags, rules[nb].flags, rule)
			end
			if rule.spells then
				for _, name in rule.spells do
					local spl = find_spell(name)
					assert(spl ~= -1, "Unknown spell "..name)
					flag_set(rules[nb].spells, spl, 1)
				end
			end
		end
		t.rules_set = true
	else
		t.rules_set = false
	end
	t.rules = rules

	-- Lets remap the flat percents
	t.rule_percents = {}
	local lims = { 0, 0, 0, 0, 0 }
	lims[1] = t.rules[1].percent
	for y = 2, nb do
		lims[y] = lims[y - 1] + t.rules[y].percent
	end
	for z = 0, 99 do
		t.rule_percents[z + 1] = 0
		for y = nb, 1, -1 do
			if z < lims[y] then t.rule_percents[z + 1] = y - 1 end
		end
	end

	-- Need to define a variable to this index?
	if t.define_as then define_as(t) end

	-- Now we are done, let's exec the scripts if needed
	dostring(t.load_script)

	tinsert(__wf_info.data, t)

	-- If we are out of the normal loading, allow individual entries to be changed
	if done_loading_data_files then
		load_cache_wilderness_type_info(t)
	end
	return t.__index__
end)

-- initialize wildc2i array
constant("init_wf_info_wildc2i", function()
	for i = 1, max_wf_idx do
		wildc2i[wf_info[i].d_char + 1] = i - 1
	end
end)

--
-- v_info loader
--
global("__v_num", 0)
global("__v_info", {})
__v_info.data = {}
constant("new_vault_type",
function (t)
	t.__type__ = __info_cache.v_info
	assert(t.name, "no name for new vault type")
	assert(t.layout, "no layout for " .. t.name)

	if not t.__index__ then t.__index__ = __v_num end
	__v_num = max(t.__index__ + 1, __v_num)

	-- Defaults to nothing to do
	default_value(t, "load_script", "")

	assert(t.type, "no type for " .. t.name)
	assert(t.rating, "no rating for " .. t.name)

	t.typ = t.type
	t.rat = t.rating

	-- Default to ToME 2 behaviour
	t.lvl = t.level or iif(t.type == 7, 5, 10)

	-- Flags
	default_value(t, "flags", {})
	t.flags = grab_flags(t.flags, flag_new(), t)

	-- Scan the layout
	if type(t.layout) == "table" then
		t.wid = 0
		t.hgt = getn(t.layout)
		t.text = ""
		for i = 1, getn(t.layout) do
			local line = t.layout[i]

			-- Find width
			t.wid = max(t.wid, strlen(line))

			-- Add it
			t.text = t.text..line
		end
	else
		-- Load the vault from a standard map file
		t.hgt, t.wid = load_map_size("dungeon/vaults/"..t.layout)
		t.text = t.layout
		t.dynamic = true
	end

	t.mon = {}
	t.item = {}

	-- Need to define a variable to this index?
	if t.define_as then define_as(t) end

	-- Now we are done, let's exec the scripts if needed
	dostring(t.load_script)

	tinsert(__v_info.data, t)

	-- If we are out of the normal loading, allow individual entries to be changed
	if done_loading_data_files then
		load_cache_vault_type(t)
	end
	return t.__index__
end)


--
-- A player descriptor(sex, race, class, whatever)
--
global("__descriptor_num", 0)
global("__descriptor_info", {})
__descriptor_info.data = {}
constant("new_player_descriptor",
function (t)
	t.__type__ = __info_cache.descriptor_info
	assert(t.name, "no name for new player descriptor")
	assert(t.type, "no type for " .. t.name)
	t.title = t.name
	default_value(t, "desc", "???")
	t.desc = convert_desc(t.desc)

	if not t.__index__ then t.__index__ = __descriptor_num end
	__descriptor_num = max(t.__index__ + 1, __descriptor_num)

	t.id = t.__index__

	-- Defaults to nothing to do
	default_value(t, "load_script", "")

	default_value(t, "titles", {})

	t.adj = flag_new()
	if t.stats then
		for s, v in t.stats do
			t.adj[s] = v
		end
	end

	t.exp = t.experience
	t.mhp = t.life_rating
	t.infra = t.infravision

	if t.blows then
		t.blow_num = t.blows.num
		t.blow_mul = t.blows.mul
		t.blow_wgt = t.blows.weight
	end

	-- Misc abilities
	if not t.misc_abilities then t.misc_abilities = {} end
	t.dis = t.misc_abilities.disarming or 0
	t.dev = t.misc_abilities.devices or 0
	t.sav = t.misc_abilities.saving_throw or 0
	t.stl = t.misc_abilities.stealth or 0
	t.srh = t.misc_abilities.search or 0
	t.fos = t.misc_abilities.freq_search or 0
	t.thn = t.misc_abilities.to_melee or 0
	t.thb = t.misc_abilities.to_distance or 0

	-- Age/weight/height
	if t.age then
		t.b_age = t.age[1]
		t.m_age = t.age[2]
	end
	if t.height then
		t.m_b_ht = t.height.male[1]
		t.m_m_ht = t.height.male[2]
		t.f_b_ht = t.height.female[1]
		t.f_m_ht = t.height.female[2]
	end
	if t.weight then
		t.m_b_wt = t.weight.male[1]
		t.m_m_wt = t.weight.male[2]
		t.f_b_wt = t.weight.female[1]
		t.f_m_wt = t.weight.female[2]
	end

	-- Flags
	default_value(t, "flags", {})
	t.flags = grab_flags(t.flags, flag_new(), t)

	-- Powers
	default_value(t, "powers", {})
	local pows = {-1,-1,-1,-1}
	for i = 1, getn(t.powers) do
		local p = find_power(t.powers[i])
		assert(p >= 0, "unkown power "..t.powers[i])
		pows[i] = p
	end
	t.powers = pows

        -- Class choices
        default_value(t, "descriptor_choices", {})
	t.choice = flag_new()
	if type(t.descriptor_choices) == "table" then
		local nb_types = 1
		for type, data in t.descriptor_choices do
			local type_choices = flag_new()
			-- Save the type
			type_choices[0] = type

			-- Do the permissions
			local perms = { allow = flag_new(), nogood = flag_new(), never = flag_new() }
			local nb = 1
			for name, perm in data do
				perms[perm][nb] = name
				nb = nb + 1
			end
			type_choices[1] = perms.allow
			type_choices[2] = perms.nogood
			type_choices[3] = perms.never

			t.choice[nb_types] = type_choices
			nb_types = nb_types + 1
		end
	end

        -- History
        t.chart = ""

	-- Body parts
	default_value(t, "body", { })
	t.body_parts = getter.body_parts(t.body)

	-- Obj flags
	default_value(t, "levels", {})
	t.oflags = {}
	for i = 0, PY_MAX_LEVEL + 1 do
		if t.levels[i] then
			t.oflags[i + 1] = grab_flags(t.levels[i], flag_new(), t)
		else
			t.oflags[i + 1] = flag_new()
		end
	end

	-- Skills
	default_value(t, "skills", {})
	t.skill_base = flag_new()
	t.skill_mod = flag_new()
	for sk, data in t.skills do
		local skill = find_skill(sk)
		assert(skill ~= -1, "unknown skill "..sk)
		flag_set_full(t.skill_base, skill, FLAG_FLAG_BOTH, data[1][2], data[1][1]);
		flag_set_full(t.skill_mod, skill, FLAG_FLAG_BOTH, data[2][2], data[2][1]);
	end

	-- Abilities
	local abilities = flag_new()
	if t.abilities then
		for ab, level in t.abilities do
			local a = find_ability(ab)
			assert(a ~= -1, "unknown ability "..ab)
			abilities[a] = level
		end
	end
	t.abilities = abilities

	-- Objects
	t.objects = flag_new()
	if t.starting_objects then
		local nb = 1
		for i = 1, getn(t.starting_objects) do
			local data = t.starting_objects[i]
			local d = flag_new()
			assert(data.obj, "No starting object data")
			if type(data.obj) == "function" then
				local ret = get_flag_value["function"](data.obj, t)
				d[0] = -ret[2]
			else
				d[0] = data.obj
			end
			d[1] = data.min or 1
			d[2] = data.max or 1
			t.objects[nb] = d
			nb = nb + 1
		end
	end

	-- Need to define a variable to this index?
	if t.define_as then define_as(t) end

	-- Now we are done, let's exec the scripts if needed
	dostring(t.load_script)

	tinsert(__descriptor_info.data, t)

	-- If we are out of the normal loading, allow individual entries to be changed
	if done_loading_data_files then
		load_cache_player_descriptor(t)
	end
	return t.__index__
end)


--
-- set_info loader
--
global("__set_num", 1)
global("__set_info", {})
__set_info.data = {}
constant("new_artifact_set",
function (t)
	t.__type__ = __info_cache.set_info
	assert(t.name, "no name for new artfiact set")
	default_value(t, "desc", "")
	t.text = convert_desc(t.desc)

	if not t.__index__ then t.__index__ = __set_num end
	__set_num = max(t.__index__ + 1, __set_num)

	-- Defaults to nothing to do
	default_value(t, "load_script", "")

	assert(t.artifacts, "no artifacts in the set '" .. t.name .. "'")

	t.arts =
	{
		{ flags = { flag_new(), flag_new(), flag_new(), flag_new(), flag_new(), flag_new() } },
		{ flags = { flag_new(), flag_new(), flag_new(), flag_new(), flag_new(), flag_new() } },
		{ flags = { flag_new(), flag_new(), flag_new(), flag_new(), flag_new(), flag_new() } },
		{ flags = { flag_new(), flag_new(), flag_new(), flag_new(), flag_new(), flag_new() } },
		{ flags = { flag_new(), flag_new(), flag_new(), flag_new(), flag_new(), flag_new() } },
		{ flags = { flag_new(), flag_new(), flag_new(), flag_new(), flag_new(), flag_new() } },
	}
	t.num = 0

	-- Parse all artifatcs and items needed
	for art, data in t.artifacts do
		local artifact = t.arts[t.num + 1]
		t.num = t.num + 1

		artifact.a_idx = art
		for need, flags in data do
			artifact.flags[need] = grab_flags(flags, artifact.flags[need], t)
		end
	end

	-- Need to define a variable to this index?
	if t.define_as then define_as(t) end

	-- Now we are done, let's exec the scripts if needed
	dostring(t.load_script)

	tinsert(__set_info.data, t)

	-- If we are out of the normal loading, allow individual entries to be changed
	if done_loading_data_files then
		load_cache_set_type(t)
	end
	return t.__index__
end)

--
-- re_info loader
--
global("__re_num", 1)
global("__re_info", {})
__re_info.data = {}
constant("new_monster_ego",
function (t)
	t.__type__ = __info_cache.re_info
	assert(t.name, "no name for new monster ego")
	assert(t.place, "no name place for " .. t.name)
	t.before = (t.place == "before")

	if not t.__index__ then t.__index__ = __re_num end
	__re_num = max(t.__index__ + 1, __re_num)

	-- Defaults to nothing to do
	default_value(t, "load_script", "")

	default_value(t, "race", strchar(__mego__.MEGO_CHAR_ANY))
	default_value(t, "color", __mego__.MEGO_CHAR_ANY)
	t.d_char = strbyte(t.race)
	t.d_attr = t.color

	default_value(t, "speed", mods.add(0))
	default_value(t, "weight", mods.add(0))
	default_value(t, "level", mods.add(0))
	default_value(t, "ac", mods.add(0))
	default_value(t, "aff", mods.add(0))
	default_value(t, "sleep", mods.add(0))
	default_value(t, "life", {mods.add(0),mods.add(0)})
	default_value(t, "exp", mods.add(0))
	assert(t.rarity, "no rarity")

	t.speed = mods.convert(t.speed)
	t.weight = mods.convert(t.weight)
	t.level = mods.convert(t.level)
	t.ac = mods.convert(t.ac)
	t.aff = mods.convert(t.aff)
	t.sleep = mods.convert(t.sleep)
	t.mexp = mods.convert(t.exp)

	t.hdice = mods.convert(t.life[1])
	t.hside = mods.convert(t.life[2])

	t.flags = grab_flags(t.need_flags, flag_new(), t)
	t.hflags = grab_flags(t.forbid_flags, flag_new(), t)

	t.mflags = grab_flags(t.added_flags, flag_new(), t)
	t.nflags = grab_flags(t.removed_flags, flag_new(), t)

	t.r_char = {}
	t.nr_char = {}
	if t.allow_races then
		for _, race in t.allow_races do
			tinsert(t.r_char, strbyte(race))
		end
	end
	if t.forbid_races then
		for _, race in t.forbid_races do
			tinsert(t.nr_char, strbyte(race))
		end
	end

	local spells = flag_new()
	if t.added_spells then
		-- Spells frequency
		if t.added_spells.frequency then
			t.freq_spell = 100 / t.added_spells.frequency
			t.freq_inate = 100 / t.added_spells.frequency
		end

		-- Spells
		for name, info in t.added_spells do
			if name ~= "frequency" then
				local spl = find_spell(name)
				assert(spl ~= -1, "Unknown spell "..name)
				flag_set_full(spells, spl, FLAG_FLAG_BOTH, info.level, info.chance)
			end
		end
	end
	t.msflags = spells

	t.remove_all_spells = FALSE
	if t.removed_spells == "all" then
		t.remove_all_spells = true
		t.nsflags = flag_new()
	else
		local spells = flag_new()
		if t.removed_spells then
			-- Spells
			for name, info in t.removed_spells do
				if name ~= "frequency" then
					local spl = find_spell(name)
					assert(spl ~= -1, "Unknown spell "..name)
					flag_set_full(spells, spl, FLAG_FLAG_BOTH, info.level, info.chance)
				end
			end
		end
		t.nsflags = spells
	end

	t.blow = flag_new(4)
	t.blowm_dice = flag_new(4)
	t.blowm_side = flag_new(4)
	if t.blows then
		for nb = 1, getn(t.blows) do
			local attack = t.blows[nb]
			local dice = attack[3]

			local b = monster_blow_new()
			b.d_dice = dice[1][2]
			b.d_sides = dice[2][2]
			b.method = find_monster_attack_method(attack[1])
			b.effect = find_monster_attack_type(attack[2])

			flag_set_ptr(t.blow, nb - 1, FLAG_TYPE_MONSTER_BLOW, b);
			t.blowm_dice[nb - 1] = dice[1][1]
			t.blowm_side[nb - 1] = dice[2][1]
		end
	end

	-- Need to define a variable to this index?
	if t.define_as then define_as(t) end

	-- Now we are done, let's exec the scripts if needed
	dostring(t.load_script)

	tinsert(__re_info.data, t)

	-- If we are out of the normal loading, allow individual entries to be changed
	if done_loading_data_files then
		load_cache_monster_ego(t)
	end

	return t.__index__
end)
