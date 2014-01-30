-------------------------------------------------------------------
-- Fates subsytem
--------------------------------------------------------------------

constant("fates", {})
settag(fates, TAG_NAMESPACE)

constant("TAG_FATE_TYPE", newtag())
constant("TAG_FATE_STYLE", newtag())

add_loadsave("fates.max_fate_idx", 0)
add_loadsave("fates.curr_fates", {})
add_loadsave("fates.types_state", {})
add_loadsave("fates.styles_state", {})

safe_new_flag("NO_NEW_FATES")
safe_new_flag("NO_EXEC_FATES")
safe_new_flag("FATES_OK")
safe_new_flag("DURATION_FATE")

hook.new_hook_type("NEW_RND_FATE_PRE")
hook.new_hook_type("NEW_FATE_POST")
hook.new_hook_type("FATE_LEARN_PRE")
hook.new_hook_type("FATE_LEARN_POST")
hook.new_hook_type("FATE_EXEC_PRE")
hook.new_hook_type("FATE_EXEC_POST")
hook.new_hook_type("FATE_REMOVED")

hook(hook.GAME_START,
function()
	-- If a fate type wants to be usable with all fate styles,
	-- then we have to generate the list of all styles after
	-- style registration is done.
	local all_styles = {}

	for i = 2, fates.max_style_idx do
		tinsert(all_styles, fates.styles[i])
	end

	for i = 1, fates.max_type_idx do
		if type(fates.types[i].styles[1]) == "string" then
			fates.types[i].styles = all_styles
		end
	end

	-- Validate and fix up saved list of fates
	local to_blank = {}

	for idx, fate in fates.curr_fates do
		local removed = false

		fate.__refs = (fate.__refs or 0) + 1

		if fate.__refs > 1 then
			message("#vERROR: Fate '" .. fate.type_name .. ":" ..
					(fate.desc or "uknown_desc") .. " referenced more " ..
					  " than once via different indices.")
			tinsert(to_blank, idx)
			removed = true
		end

		if not removed and idx ~= fate.index then
			message("#vERROR: Fate '" .. fate.type_name .. ":" ..
				  (fate.desc or "uknown_desc") .. " has conflicting " ..
					  "indexes.")

			fate.index = idx
		end

		if fate.__gone then
			message("#vERROR: Fate '" .. fate.type_name .. ":" ..
					(fate.desc or "uknown_desc") .. " should be gone.")
			tinsert(to_blank, idx)
		elseif not removed and (fate.done or fate.delete or fate.remove) then
			message("#vERROR: Fate '" .. fate.type_name .. ":" ..
					(fate.desc or "uknown_desc") .. " saved in the middle " ..
					"of being removed.")
			fates.remove_fate(fate)

			fate.done   = nil
			fate.delete = nil
			fate.remove = nil

			removed = true
		end
	end -- for idx, fate in fates.curr_fates do

	for i = 1, getn(to_blank) do
		fates.curr_fates[to_blank[i]] = nil
	end

	-- Fill in convenience arrays, invoke relavant "on_game_start"
	-- callbacks.
	for idx, fate in fates.curr_fates do
		fate.__refs = nil

		if fates.styles[fate.style].array then
			tinsert(fates.styles[fate.style].array, fate)
		end

		if fates.styles[fate.style].on_game_start then
			fates.styles[fate.style].on_game_start(fate)
		end

		local fate_type = fates.types[fate.type_name]

		if fate_type.on_game_start then
			fate_type.on_game_start(fate, fate_type)
		end

		if fate.dun_level then
			local lev = fate.dun_level
			local arr = fates.dun_level_usage
			arr[lev] = (arr[lev] or 0) + 1
		end
	end -- for idx, fate in fates.curr_fates do

	fates.set_ok_dun_list()
	fates.set_ok_dun_level_list()
end) -- hook(hook.GAME_START)

-----------------------------------------------------------------------------
-----------------------------------------------------------------------------

fates.max_type_idx = 0
fates.types = {}

function fates.add_type(t)
	assert(t.name, "Fate type doesn't have name.")
	assert(not fates.types[strlower(t.name)],
		   "Fate type named '" .. t.name .. "' already registered.")
	assert(t.create, "Fate type '" .. t.name ..
		   "' doesn't have 'create' field")
	assert(t.get_chance, "Fate type '" .. t.name .. "' doesn't have "..
	   "'get_chance' field")
	assert(t.styles, "Fate type '" .. t.name ..
		   "' doesn't have 'styles' field")
	assert(type(t.styles) == "table",  "Styles for fate type '" .. t.name ..
		   "' must be a table.")
	assert(getn(t.styles) > 0,  "Styles for fate type '" .. t.name ..
		   "' must have at least one element.")
	assert(t.execute, "Fate type '" .. t.name ..
		   "' doesn't have 'execute' field")

	fates.max_type_idx = fates.max_type_idx + 1
	t.index = fates.max_type_idx

	fates.types[t.index] = t
	fates.types[t.name] = t
	fates.types[strlower(t.name)] = t

	if t.hooks then
		hook(t.hooks)
	end

	settag(t, TAG_FATE_TYPE)

	return t.index
end -- fates.add_type()

fates.max_style_idx = 0
fates.styles = {}

function fates.add_style(t)
	assert(t.name, "Fate style doesn't have a name.")
	assert(not fates.styles[strlower(t.name)],
		   "Fate style named '" .. t.name .. "' already registered.")
	assert(t.desc, "Fate style doesn't have description.")
	assert(t.get_chance, "Fate style '" .. t.name .. "' doesn't have "..
	   "'get_chance' field")

	fates.max_style_idx = fates.max_style_idx + 1
	t.index = fates.max_style_idx

	fates.styles[t.index] = t
	fates.styles[t.name] = t
	fates.styles[strlower(t.name)] = t

	if t.convenience_arr then
		t.array = {}
		fates[t.convenience_arr] = t.array
	end

	settag(t, TAG_FATE_STYLE)

	return t.index
end -- fates.add_style()

-----------------------------------------------------------------------------
-----------------------------------------------------------------------------

function fates.validate(fate, fate_type)
	assert(fate.type, "Fate doesn't have a type.")
	assert(fates.types[fate.type], "Fate doesn't have a valid type.")
	assert(fate.style,
		   "Fate type '" .. fate_type.name .. "' doesn't have a style.")
	assert(fates.styles[fate.style],
		   "Fate type '" .. fate_type.name .. "' doesn't have a valid style.")

	assert((fate_type.desc or fate_type.partial_desc or
			fates.styles[fate.style].partial_desc),
		   "Fate type '" .. fate_type.name .. "' returned a fate with " ..
			   " no desc.")

	if fates.styles[fate.style].validate then
		fates.styles[fate.style].validate(fate, fate_type)
	end

	if fate_type.validate then
		fate_type.validate(fate, fate_type)
	end

	local desc = fates.desc_fate(fate)
	assert(desc and not (desc == "" or desc == fates.DESC_ERR),
		   "Fate couldn't be described.")
end -- fates.validate()

function fates.ok_types_from_spec(fate_spec)
	local ok_types = {}

	assert(not(fate_spec.quality and
			   (fate_spec.min_quality or fate_spec.max_quality)),
		   "Can't specify a fate with both a quality and " ..
			   "min/max quality.")

	if fate_spec.min_quality and fate_spec.max_quality then
		assert(fate_spec.min_quality <= fate_spec.max_quality,
			   "Fate min quality can't be greater than fate max quality.")

		if fate_spec.min_quality == fate_spec.max_quality
		then
			fate_spec.quality     = fate_spec.min_quality
			fate_spec.min_quality = nil
			fate_spec.max_quality = nil
		end
	end

	if fate_spec.ok_styles == nil then
		-- Nothing to do
	elseif type(fate_spec.ok_styles) ~= "table" then
		error("ok_styles should be an array of styles, or nil")
	else
		for i = 1, getn(fate_spec.ok_styles) do
			if tag(fate_spec.ok_styles[i]) ~= TAG_FATE_STYLE then
				error("ok_styles should be an array of styles")
			end
		end
	end

	for i = 1, fates.max_type_idx do
		local fate_type = fates.types[i]

		ok_types[i] = true

		if fate_type.get_chance(fate_spec) <= 0 then
			ok_types[i] = false
		end

		if fate_type.dont_create and fate_type.dont_create(fate_spec)
		then
			ok_types[i] = false
		end

		if fate_spec.ok_styles then
			local okay = false

			for j = 1, getn(fate_spec.ok_styles) do
				local want_style = fate_spec.ok_styles[j]

				if fate_type.styles[want_style.index] and
					want_style.get_chance(fate_spec, want_style, fate_type) > 0
				then
					okay = true
					break
				end
			end

			if not okay then
				ok_types[i] = false
			end
		end -- if fate_spec.styles then

		if fate_spec.quality then
			if not (fate_type.quality or fate_type.min_quality or
					fate_type.max_quality)
			then
				ok_types[i] = false
			end

			if fate_type.quality and
				fate_type.quality ~= fate_spec.quality
			then
				ok_types[i] = false
			end

			if fate_type.min_quality and
				fate_type.min_quality > fate_spec.quality
			then
				ok_types[i] = false
			end

			if fate_type.max_quality and
				fate_type.max_quality < fate_spec.quality
			then
				ok_types[i] = false
			end
		end -- if fate_spec.quality then

		if fate_spec.min_quality then
			if not (fate_type.quality or fate_type.max_quality) then
				ok_types[i] = false
			end

			if fate_type.quality and
				fate_type.quality < fate_spec.min_quality
			then
				ok_types[i] = false
			end

			if fate_type.max_quality and
				fate_type.max_quality < fate_spec.min_quality
			then
				ok_types[i] = false
			end
		end -- if fate_spec.min_quality then

		if fate_spec.max_quality then
			if not (fate_type.quality or fate_type.min_quality) then
				ok_types[i] = false
			end

			if fate_type.quality and
				fate_type.quality > fate_spec.max_quality
			then
				ok_types[i] = false
			end

			if fate_type.min_quality and
				fate_type.min_quality > fate_spec.max_quality
			then
				ok_types[i] = false
			end
		end -- if fate_spec.max_quality then
	end -- for i = 1, fates.max_type_idx do

	local out = {}

	for i = 1, fates.max_type_idx do
		if ok_types[i] then
			tinsert(out, fates.types[i])
		end
	end

	return out
end -- fates.ok_types_from_spec()

fates.get_rand_quality =
	get_subsystem_param("fates", "get_rand_quality") or
	function(fate_spec)
		local ok_types    = fate_spec.ok_types
		local min_quality = infinity
		local max_quality = -infinity

		for i = 1, getn(ok_types) do
			min_quality = min(min_quality,
							  ok_types[i].min_quality or
								  ok_types[i].quality or
								  infinity)
			max_quality = max(max_quality,
							  ok_types[i].max_quality or
								  ok_types[i].quality or
								  -infinity)
		end

		min_quality = fate_spec.min_quality or min_quality
		max_quality = fate_spec.max_quality or max_quality

		return rng(min_quality, max_quality)
	end

function fates.acquire_fate(fate_spec, force)
	if fates.max_type_idx == 0 then
		message("#vERROR: Couldn't acquire random fate, no fate " ..
				  "types registered.")
		return nil
	end

	if (player.has_intrinsic(FLAG_NO_NEW_FATES) or
		has_flag(level_flags, FLAG_NO_NEW_FATES)) and not force
	then
		return nil
	end

	fate_spec = fate_spec or {}

	if fate_spec.quality then
		fate_spec.min_quality = nil
		fate_spec.max_quality = nil
	end

	local ok_types = fates.ok_types_from_spec(fate_spec)

	if fate_spec.ok_types then
		ok_types = fate_spec.ok_types
	else
		ok_types           = fates.ok_types_from_spec(fate_spec)
		fate_spec.ok_types = ok_types
	end

	-- Let hooks manipulate which kinds of types are ok
	local old_ok_types = table_copy(ok_types)
	if hook.process(hook.NEW_RND_FATE_PRE, fate_spec, ok_types) and not force
	then
		return nil
	end

	-- Hook might have given us a quality
	if fate_spec.quality then
		fate_spec.min_quality = nil
		fate_spec.max_quality = nil
	end

	-- Hook might have removed all ok types, which we'll want to
	-- circumvent if we're trying to force a fate.
	if getn(ok_types) == 0 and force then
		ok_types = old_ok_types
	end

	if getn(ok_types) == 0 then
		message("#vERROR: Couldn't acquire random fate, spec rules " ..
				  "out all fate types.")
		return nil
	end

	if fate_spec.min_quality or fate_spec.max_quality then
		fate_spec.quality = fates.get_rand_quality(fate_spec)
		fate_spec.min_quality = nil
		fate_spec.max_quality = nil
	end

	local alloc_table = {}
	local total       = 0

	-- Let chance for acquiring different types of fate vary each
	-- time, so that things like luck can make certain types of fates
	-- more or less likely.
	for i = 1, getn(ok_types) do
		total = total + ok_types[i].get_chance(fate_spec) 
		alloc_table[i] = total
	end

	local alloc_num = rng(total)
	local fate_type

	for i = 1, getn(ok_types) do
		if alloc_num <= alloc_table[i] then
			fate_type = ok_types[i]
			break
		end
	end

	-- fate_type struct might have data that is used in creating the
	-- fate, so pass it along.
	local fate = fate_type.create(fate_spec, fate_type)
	local style = fates.styles[fate.style]

	if style.on_create then
		style.on_create(fate, fate_spec, style)
	end

	if not fate then
		message("#vERROR: Couldn't create random fate.")
		return nil
	end

	-- Non-destructively shallow-copy fate_spec onto fate
	for k, v in fate_spec do
		fate[k] = fate[k] or v
	end
	fate.ok_types  = nil
	fate.ok_styles = nil

	fates.add_specific_fate(fate, fate_type)

	return fate
end -- fates.acquire_fate()

function fates.add_specific_fate(fate, fate_type)
	assert(not (fate.done or fate.remove or fate.delete),
		   "Fate was generated to be immediatly removed.")
	assert(not fate.__gone, "Fate was removed before being added.")
	assert(not fate.index, "Fate already has index.")

	fate.type       = fate_type.index
	fate.type_name  = fate_type.name

	fates.validate(fate, fate_type)

	-- Save fate in loadsave array, but only after it's been validated.
	fates.max_fate_idx = fates.max_fate_idx + 1
	fates.curr_fates[fates.max_fate_idx] = fate

	-- Finish setting up fate
	fate.index = fates.max_fate_idx

	-- Convenience arrays
	if fates.styles[fate.style].array then
		tinsert(fates.styles[fate.style].array, fate)
	end

	if fates.styles[fate.style].on_fate_acquire then
		fates.styles[fate.style].on_fate_acquire(fate, fate_type)
	end

	hook.process(hook.NEW_FATE_POST, fate, fate_type)
end -- fates.add_specific_fate()

function fates.exec_fate(fate)
	if fate.stale then
		if wizard then
			message(color.VIOLET,
					"ERROR: Can't exec stale fate: " ..
					fates.desc_fate(fate))
		end
		return false
	end

	if player.has_intrinsic(FLAG_NO_EXEC_FATES) or
		has_flag(level_flags, FLAG_NO_EXEC_FATES)
	then
		return false
	end

	local fate_idx

	if type(fate) == "number" then
		fate_idx = fate
		fate     = fates.curr_fates[fate_idx]

		assert(fate, "fate_idx " .. fate_idx .. " doesn't exist")
	elseif type(fate) == "table" then
		fate_idx = fate.index

		assert(fate_idx, "Fate doesn't have field 'fate_idx'")
		assert(fates.curr_fates[fate_idx], "fate idx " .. fate_idx ..
			   " doesn't exist")
	else
		error("Can't execute fate of type '" .. type(fate) .. "'.")
	end

	assert(not (fate.done or fate.delete),
		   "fate <" .. fates.desc_fate(fate) .. "> should be gone")
	assert(fates.curr_fates[fate_idx].index == fate_idx,
		   "Fate index mismatch for .. <" .. fates.desc_fate(fate) .. ">")

	assert(fate.style ~= fates.styles.PERMANENT.index,
		   "Can't execute permanent fates.")
		   
	local fate_type = fates.types[fate.type]

	if hook.process(hook.FATE_EXEC_PRE, fate, fate_type) then
		return true
	end

	if not fate_type.execute(fate) then
		if wizard then
			message(volor.VIOLET,
					"WARN: Fate failed to execute: " ..
						fates.desc_fate(fate))
		end
		return false
	end

	if fates.styles[fate.style].on_fate_exec then
		fates.styles[fate.style].on_fate_exec(fate, fate_type)
	end

	hook.process(hook.FATE_EXEC_POST, fate, fate_type)

	if fate.done or fate.delete or fate.remove then
		fates.remove_fate(fate)

		fate.done   = nil
		fate.delete = nil
		fate.remove = nil
	end -- if fate.done or fate.delete then

	return true
end -- fates.exec_fate()

function fates.remove_fate(fate)
	assert(not fate.__gone,
		   "Fate has already been removed.")

	local fate_type = fates.types[fate.type]

	fates.curr_fates[fate.index] = nil

	if fates.styles[fate.style].array then
		local arr = fates.styles[fate.style].array

		for i = 1, getn(arr) do
			if arr[i].index == fate.index then
				tremove(arr, i)
				break
			end
		end
	end

	if fate_type.on_remove then
		fate_type.on_remove(fate, fate_type)
	end

	if fates.styles[fate.style].on_fate_remove then
		fates.styles[fate.style].on_fate_remove(fate)
	end

	hook.process(hook.FATE_REMOVED, fate)

	fate.__gone = true
end -- fates.remove_fate()

function fates.get_curr_fates()
	local list = {}

	for idx, fate in fates.curr_fates do
		tinsert(list, fate)
	end

	return list
end

-----------------------------------------------------------------------------
-- Dungeon and dungeon level convenience code
-----------------------------------------------------------------------------

fates.ok_duns = {}
fates.ok_dun_levels = {}
fates.dun_level_usage = {}
fates.min_ok_dun_lev = infinity
fates.max_ok_dun_lev = -infinity

function fates.default_set_ok_dun_list()
	for i = 1, max_d_idx - 1 do
		local dun  = d_info(i)

		-- Okay if it can be reached from the surface, or if
		-- FLAG_FATES_OK set, but not okay if the whole dungeon
		-- excludes the execution of fates.
		if not dun.flags[FLAG_NO_EXEC_FATES] and
			((dun.iy ~= -1 and dun.ix ~= -1 and not
			  dun.flags[FLAG_NO_SURFACE_ENTRY]) or
				 dun.flags[FLAG_FATES_OK])
		then
			fates.ok_duns[i] = true
		end
	end
end -- fates.default_set_ok_dun_list()

fates.set_ok_dun_list = get_subsystem_param("fates", "set_ok_dun_list") or
	fates.default_set_ok_dun_list
		
fates.set_ok_dun_level_list = get_subsystem_param("fates",
												  "set_ok_dun_level_list") or
function()
	for d_idx, v in fates.ok_duns do
		local dun = d_info(d_idx)
		local tmp = {}

		fates.min_ok_dun_lev = min(fates.min_ok_dun_lev, dun.mindepth)
		fates.max_ok_dun_lev = max(fates.max_ok_dun_lev, dun.maxdepth)

		for i = dun.mindepth, dun.maxdepth do
			tmp[i] = true
			fates.dun_level_usage[i] = fates.dun_level_usage[i] or 0
		end

		-- If the dungeon has a persistant level on which fates are
		-- never executed, then don't count that level (though the same
		-- level depth in a different dungeon might be okay)
		if __d_info.levels[d_idx] then
			local levels = __d_info.levels[d_idx]

			for lev, data in levels do
				if data.flags and not
					(data.flags[FLAG_SAVE_LEVEL] and
					 data.flags[FLAG_NO_EXEC_FATES])
				then
					tmp[lev] = false
				end
			end -- for lev, data in levels do
		end -- if __d_info.levels[d_idx] then

		for k, v in tmp do
			fates.ok_dun_levels[k] = fates.ok_dun_levels[k] or v
		end
	end -- for d_idx, v in fates.ok_duns do
end -- set_ok_dun_level_list()

fates.get_min_rand_level = get_subsystem_param("fates",
											   "get_min_rand_level") or
function()
	return 0
end -- get_min_rand_level()

fates.get_max_rand_level = get_subsystem_param("fates",
											   "get_max_rand_level") or
function()
	return player.lev
end -- get_max_rand_level()

fates.get_rand_level = get_subsystem_param("fates", "get_rand_level") or
function(min_level, max_level)
	min_level = max(min_level or fates.get_min_rand_level(), 0)
	max_level = min(max_level or fates.get_min_rand_level(), infinity)

	return rng(min_level, max_level)
end  -- get_rand_level()

fates.get_min_rand_dun_level = get_subsystem_param("fates",
												   "get_min_rand_dun_level") or
function()
	return fates.min_ok_dun_lev
end -- get_min_rand_level()

fates.get_max_rand_dun_level = get_subsystem_param("fates",
												   "get_max_rand_dun_level") or
function()
	return fates.max_ok_dun_lev
end -- get_max_rand_level()

fates.get_rand_dun_level = get_subsystem_param("fates",
											   "get_rand_dun_level") or
function(min_level, max_level, least_used_first)
	min_level = max(min_level or fates.get_min_rand_dun_level(),
					fates.min_ok_dun_lev)
	max_level = min(max_level or fates.get_max_rand_dun_level(),
					fates.max_ok_dun_lev)

	-- If least_used_first is set, then randomly pick a level out of
	-- the list of levels with the least number of existing fates
	-- that are already using that level, and that also fall between
	-- min_level and max_level.  Otherwise, just pay attention to
	-- min_level and max_level
	local by_use  = {}
	local min_use = infinity

	for k, v in fates.ok_dun_levels do
		if k >= min_level and k <= max_level then
			local use
			if least_used_first then
				use = fates.dun_level_usage[k] + 1
			else
				use = 1
			end
			by_use[use] = by_use[use] or {}

			tinsert(by_use[use], k)
			min_use = min(min_use, use)
		end	
	end

	local list = by_use[min_use]

	if not list or getn(list) == 0 then
		if wizard then
			message(color.VIOLET,
					"ERROR: Couldn't get random dun level for fate.  " ..
						"No ok dun levels between " .. min_level .. " and " ..
						max_level .. "?")
		end

		return nil
	end

	return list[rng(getn(list))]
end -- get_rand_dun_level()

fates.dun_level_tolerance = get_subsystem_param("fates",
												"dun_level_tolerance") or
	0

function fates.reserve_dun_level(level, tolerance)
	tolerance = tolerance or fates.dun_level_tolerance

	local min_lev = max(fates.min_ok_dun_lev, level - tolerance)
	local max_lev = min(fates.max_ok_dun_lev, level + tolerance)

	local level_list = {}

	for i = min_lev, max_lev do
		if fates.ok_dun_levels[i] then
			tinsert(level_list, i)
		end
	end

	if getn(level_list) == 0 then
		if wizard then
			message(color.VIOLET,
					"ERROR: couldn't reserve level for fate.")
		end
		return nil
	end

	-- Go for the least used level first, then for the level that's closest
	-- to the requested level; if both are the same, then sort randomly.
	sort(level_list,
		 function(a, b)
			 if a == b then
				 -- Lua does weird things if you don't return false
				 -- when two elements in a sort are the same.
				 return false
			 elseif fates.dun_level_usage[a] ~= fates.dun_level_usage[b] then
				 return fates.dun_level_usage[a] < fates.dun_level_usage[b]
			 elseif abs(%level - a) ~= abs(%level - b) then
				 return abs(%level - a) < abs(%level - b)
			 else
				 return (rng(0, 1) == 0)
			 end
		 end)

	level = level_list[1]
	fates.dun_level_usage[level] = fates.dun_level_usage[level] + 1

	return level
end -- fates.reserve_dun_level()

function fates.release_dun_level(level)
	if not fates.dun_level_usage[level] or
		fates.dun_level_usage[level] <= 0
	then
		message(color.VIOLET,
				"ERROR: Level " .. level .. "was never reserved for " ..
					"a fate.")
		return
	end

	fates.dun_level_usage[level] = fates.dun_level_usage[level] - 1
end -- fates.release_dun_level()

-----------------------------------------------------------------------------
-- Description code
-----------------------------------------------------------------------------
function fates.partial_desc_aux(fate, fate_type, type_str, style_str)
	if fate.cloudy_style and fate.cloudy_type then
		return "Your fate is entirely cloudy."
	end

	type_str = fates.get_desc_verb(fate, fate_type) .. " " .. type_str

	style_str = "if " .. style_str

	if fate.cloudy_style then
		style_str = "somehow"
	end

	if fate.cloudy_type then
		type_str = "You are fated to something unknown"
	end

	return type_str .. " " .. style_str .. "."
end -- fates.partial_desc_aux()

function fates.get_desc_verb(fate, fate_type)
	if fate.chance and fate.chance < 100 then
		return "You might"
	end

	if fate.quality and fate.quality < 0 then
		return "You are doomed to"
	end

	return "You are fated to"
end -- fates.get_desc_verb()

fates.DESC_ERR = "{error describing fate}"

function fates.desc_fate(fate)
	local fate_type = fates.types[fate.type]

	if fate.desc then
		if type(fate.desc) == "function" then
			return fate.desc(fate, fate_type)
		else
			return fate.desc
		end
	end

	if fate_type.desc then
		if type(fate_type.desc) == "function" then
			return fate_type.desc(fate, fate_type)
		else
			return fate_type.desc
		end
	end

	if not fates.styles[fate.style].partial_desc then
		message(color.VIOLET,
				"ERROR: Can't do partial descs for fates of style '" ..
					fates.styles[fate.style].desc .. "' (fate_type = '" ..
					fate_type.name .. "', fate_idx = " ..
					fate.index)
		return fates.DESC_ERR
	end

	local partial_str

	if fate.partial_desc then
		if type(fate.partial_desc) == "function" then
			partial_str = fate.partial_desc(fate, fate_type)
		else
			partial_str = fate.partial_desc
		end
	elseif fate_type.partial_desc then
		if type(fate_type.partial_desc) == "function" then
			partial_str = fate_type.partial_desc(fate, fate_type)
		else
			partial_str = fate_type.partial_desc
		end
	else
		message(color.VIOLET,
				"ERROR: No partial desc for fate of type '" ..
					fate_type.name .. "', fate_idx = " .. fate.index)
		return fates.DESC_ERR
	end

	if not partial_str then
		message(color.VIOLET,
				"ERROR: Partial desc string for fate of type '" ..
					fate_type.name .. "' is nil")
		return fates.DESC_ERR
	end

	if type(partial_str) ~= "string" then
		message(color.VIOLET,
				"ERROR: Partial desc string for fate of type '" ..
					fate_type.name .. "' is of type '" ..
					type(partial_str) .. "' (fate_idx = " ..
					fate.index .. ")")
		return fates.DESC_ERR
	end

	if partial_str == ""  then
		message(color.VIOLET,
				"ERROR: Partial desc string for fate of type '" ..
					fate_type.name .. "' is empty (fate_idx = " ..
					fate.index .. ")")
		return fates.DESC_ERR
	end

	local func = fates.styles[fate.style].partial_desc
	local out  = func(fate, fate_type, partial_str)

	if not out or out == "" then
		message(color.VIOLET,
				"ERROR: Unknown error describing fate of type '" .. 
					fate_type.name .. "', fate_idx = " ..
					(fate.index or 'unknown'))
		return fates.DESC_ERR
	end

	return out
end -- fates.desc_fate()

-----------------------------------------------------------------------------
-----------------------------------------------------------------------------

-- Known and unknown fates

function fates.get_unknown_fates()
	local list = fates.get_curr_fates()
	local out  = {}

	for i = 1, getn(list) do
		if not list[i].known and not list[i].hidden then
			tinsert(out, list[i])
		end
	end

	return out
end -- fates.get_unknown_fates()

function fates.get_known_fates()
	local list = fates.get_curr_fates()
	local out  = {}

	for i = 1, getn(list) do
		if list[i].known then
			tinsert(out, list[i])
		end
	end

	return out
end -- fates.get_unknown_fates()

function fates.learn_fates(num, strs)
	local unknown     = fates.get_unknown_fates()
	local num_unknown = getn(unknown)

	strs = strs or {}

	if getn(unknown) == 0 then
		if strs.no_fates then
			message(strs.no_fates)
		end
		return 0
	end

	local cant_learn = {}
	local i = 1
	while i <= getn(unknown) do
		local fate = unknown[i]
		if fate.no_learn then
			tinsert(cant_learn, tremove(unknown, i))
		elseif fates.styles[fate.style].no_learn and
			fates.styles[fate.style].no_learn(fate)
		then
			tinsert(cant_learn, tremove(unknown, i))
		elseif fates.types[fate.type].no_learn and
			fates.types[fate.type].no_learn(fate)
		then
			tinsert(cant_learn, tremove(unknown, i))
		elseif hook.process(hook.FATE_LEARN_PRE, fate) then
			tinsert(cant_learn, tremove(unknown, i))
		else
			i = i + 1 
		end
	end

	local to_learn
	local remainder = 0

	if num >= getn(unknown) then
		to_learn  = unknown
		remainder = num - getn(unknown)
	else
		-- Select a random set of fates to learn
		to_learn = {}
		for i = 1, num do
			tinsert(to_learn, tremove(unknown, rng(getn(unknown))))
		end
	end

	if getn(to_learn) == 0 and getn(cant_learn) > 0 and remainder > 0 then
		if strs.no_learnable then
			message(strs.no_learnable)
			return 0
		end

		local fate = cant_learn[rng(getn(cant_learn))]

		if fate.no_learn_msg then
			message(fate.no_learn_msg)
		elseif fates.styles[fate.style].no_learn_msg then
			fates.styles[fate.style].no_learn_msg(fate)
		elseif fates.types[fate.type].no_learn_msg then
			fates.types[fate.type].no_learn_msg(fate)
		elseif strs.no_learn then
			message(strs.no_learn)
		else
			message("Something prevents you from learning of your fate.")
		end

		return 0
	end -- Unlearnable fates?

	local lines = {}

	for i = 1, getn(to_learn) do
		local fate = to_learn[i]

		if fates.types[fate.type].on_learn then
			fates.types[fate.type].on_learn(fate)
		end

		if fates.styles[fate.style].on_learn then
			fates.styles[fate.style].on_learn(fate)
		end

		if fate.stale then
			tinsert(lines, strs.stale_fate or
					"That fate seems to have already come to pass.")
			fates.remove_fate(fate)
		else
			fate.known = true
			tinsert(lines, fates.desc_fate(fate))
		end

		hook.process(hook.FATE_LEARN_POST, fate)
	end

	if strs.learn_prefix then
		lines[1] = strs.learn_prefix .. lines[1]
	end

	for i = 1, getn(lines) do
		message(lines[i])
	end

	if getn(to_learn) >= num_unknown and getn(cant_learn) == 0 then
		message(strs.learned_all or "#yYou have learned of all your fates.")
	else
		message(strs.more_fates or
				"#vThere are still more fates for you to learn of.")
	end

	return getn(to_learn)
end -- fates.learn_fates()

-----------------------------------------------------------------------------
-----------------------------------------------------------------------------

-- For displaying the list of current fates with the curr_know subsystem

fates.get_no_fates_disp_lines =
	get_subsystem_param("fates", "get_no_fates_disp_lines") or
	function(lines)
		tinsert(lines, "#yYou have no fates to display.")
	end

fates.can_display_fate =
	get_subsystem_param("fates", "can_display_fate") or
	function(fate)
		return game_options.cheat_xtra or
			(fate.known and not fate.hidden)
	end

fates.get_undisplayed_fate_lines =
	get_subsystem_param("fates", "get_undisplayed_fate_lines") or
	function(undisp_fates, lines)
	end

function fates.show_fates()
	local ret, wid, hgt = term.get_size()
	local by_style     = {}
	local undisp_fates = {}

	for idx, fate in fates.curr_fates do
		if fates.can_display_fate(fate) then
			local style = fate.style

			if fate.style_cloudy then
				style = fates.styles.CLOUDY.index
			end

			by_style[fate.style] = by_style[fate.style] or {}

			tinsert(by_style[fate.style], fate)
		else
			tinsert(undisp_fates, fate)
		end
	end

	local lines = {}
	fates.get_undisplayed_fate_lines(undisp_fates, lines)

	local style_order = {}
	for style, list in by_style do
		tinsert(style_order, style)

		if fates.styles[style].sort then
			sort(list, fates.styles[style].sort)
		else
			sort(list,
				 function(a, b)
					 return fates.desc_fate(a) < fates.desc_fate(b)
				 end)
		end
	end -- for desc, list in types do
	sort(style_order)

	if getn(style_order) == 0 then
		fates.get_no_fates_disp_lines(lines)
		return lines
	end

	for i = 1, getn(style_order) do
		local style = fates.styles[style_order[i]]
		local list  = by_style[style_order[i]]

		local style_desc = style.display_desc or style.desc

		tinsert(lines, "    -- " .. book_capitals(style_desc) .. " fates --")

		for j = 1, getn(list) do
			local fate_desc  = fates.desc_fate(list[j])
			local fate_lines = line_breaks(fate_desc, wid - 5)

			tinsert(lines, "* " .. tremove(fate_lines, 1))

			for k = 1, getn(fate_lines) do
				tinsert(lines, "  " .. fate_lines[k])
			end
		end -- for j = 1, getn(list) do

		tinsert(lines, "")
	end -- for i = 1, getn(style_order)

	return lines
end -- fates.show_fates()

fates.curr_know_menu_item = {
	desc  = "Display current fates",
	title = "Fate status",
	func  = fates.show_fates,
}

-----------------------------------------------------------------------------
-----------------------------------------------------------------------------

------------------------------------------------------------------
-- Hooks for receiving and executing fates
------------------------------------------------------------------
hook(hook.LEVEL_END_GEN,
function()
	-- Remove duration fate from previous level (if there was any)
	flag_remove(level_flags.flags, FLAG_DURATION_FATE)

	if has_flag(level_flags, FLAG_SAVE_LEVEL) then
		-- If this is a saved level, then find any uniques and mark
		-- them as *really* being on a saved level, as opposed to
		-- setting "on_saved" to true as a hack to reserve them
		for idx, fate in fates.curr_fates do
			if fate.r_idx and r_info(fate.r_idx).cur_num > 0 then
				fate.on_saved = true
			end
		end
	end

	if dun_level == 0 then
		-- Nothing to do on the surface
		return
	end

	-- Pick a random "during stay" fate, if any apply.  Both a "during
	-- stay" and "on enter" fate can happen on the same level.  If
	-- an "on enter" fate wants to not have a "during stay" fate happen
	-- on the same level, then it's execution code can remove
	-- FLAG_DURATION_FATE from the level flags.
	local stay_fates = {}

	for i = 1, getn(fates.curr_during_lev_fates) do
		local fate = fates.curr_during_lev_fates[i]

		if fate.dun_level == dun_level then
			tinsert(stay_fates, fate)
		end
	end

	for i = 1, getn(fates.curr_during_dun_fates) do
		local fate = fates.curr_during_dun_fates[i]

		if fate.dungeon == current_dungeon_idx then
			tinsert(stay_fates, fate)
		end
	end

	if getn(stay_fates) > 0 then
		local fate = stay_fates[rng(getn(stay_fates))]

		flag_set_full(level_flags.flags, FLAG_DURATION_FATE,
					  FLAG_FLAG_BOTH, fate.chance_per_turn, fate.index)
	end

	-- First search for a fate that is certain to happen on etnering,
	-- and do it if found.  If there are no certain fates that apply,
	-- try the uncertain ones to see if they pass their chance check.
	-- Prefer level specific certain fates over dungeon specific level
	-- fates.
	local certain = {}
	local maybe   = {}

	for i = 1, getn(fates.curr_enter_lev_fates) do
		local fate = fates.curr_enter_lev_fates[i]

		if fate.dun_level == dun_level then
			if fate.chance == 100 then
				tinsert(certain, fate)
			else
				tinsert(maybe, fate)
			end
		end
	end

	if getn(certain) > 0 then
		fates.exec_fate(certain[rng(getn(certain))])
		return
	end

	for i = 1, getn(fates.curr_enter_dun_fates) do
		local fate = fates.curr_enter_dun_fates[i]

		if fate.dungeon == current_dungeon_idx then
			if fate.chance == 100 then
				tinsert(certain, fate)
			else
				tinsert(maybe, fate)
			end
		end
	end

	if getn(certain) > 0 then
		fates.exec_fate(certain[rng(getn(certain))])
		return
	end

	-- No definite fates, see if any of the possible fates happen.  Test
	-- them in a random order.
	while getn(maybe) > 0 do
		local idx  = rng(getn(maybe))
		local fate = tremove(maybe, idx)

		if rng.percent(fate.chance) then
			fates.exec_fate(fate)
			return
		end
	end
end) -- hook(hook.LEVEL_END_GEN)

------

fates.new_fate_chance_per_turn = 
	get_subsystem_param("fates", "new_fate_chance_per_turn") or 0

hook(hook.PROCESS_WORLD_NON_WILD,
function()
	if has_flag(level_flags, FLAG_DURATION_FATE) and
		rng(level_flags.flags[FLAG_DURATION_FATE]) == 1
	then
		fates.exec_fate(flag_get2(level_flags.flags, FLAG_DURATION_FATE))
		return
	end

	if fates.new_fate_chance_per_turn > 0 and
		rng(fates.new_fate_chance_per_turn) == 1
	then
		fates.acquire_fate()
	end
end) -- hook(hook.PROCESS_WORLD_NON_WILD)

------

fates.new_fate_chance_per_wild_turn = 
	get_subsystem_param("fates", "new_fate_chance_per_wild_turn") or 0

hook(hook.PROCESS_WORLD_WILD,
function()
	for i = 1, getn(fates.curr_during_wild_fates) do
		local fate      = fates.curr_during_wild_fates[i]
		local fate_type = fates.types[fate.type]

		if fate_type.do_now(fate) then
			fates.exec_fate(fate)
			return
		end
	end

	if fates.new_fate_chance_per_wild_turn > 0 and
		rng(fates.new_fate_chance_per_wild_turn) == 1
	then
		fates.acquire_fate()
	end
end) -- hook(hook.PROCESS_WORLD_WILD)

------

hook(hook.WILD_SMALL_MOVE_POST,
function()
	for i = 1, getn(fates.curr_move_wild_fates) do
		local fate      = fates.curr_move_wild_fates[i]
		local fate_type = fates.types[fate.type]

		if fate_type.do_now(fate) then
			fates.exec_fate(fate)
			return true
		end
	end
end) -- hook(hook.WILD_SMALL_MOVE_POST)

------
 
fates.new_fate_chance_per_clevel = 
	get_subsystem_param("fates", "new_fate_chance_per_clevel") or 0

hook(hook.PLAYER_LEVEL,
function(num_levels)
	if fates.new_fate_chance_per_clevel <= 0 then
		return
	end

	for i = 1, num_levels do
		if rng(fates.new_fate_chance_per_clevel) == 1 then
			fates.acquire_random_fate()
		end
	end
end) -- hook(hook.PLAYER_LEVEL)

------

hook(hook.MONSTER_DEATH,
function(m_idx, who)
	if who ~= WHO_PLAYER then
		return
	end

	local monst = monster(m_idx)

	if fates.curr_kill_mon_fates[monst.r_idx] then
		local fate = fates.curr_kill_mon_fates[monst.r_idx]
		fates.exec_fate(fate)
	end
end) -- hook(hook.MONSTER_DEATH)

----------------------------------------------------------------------------
----------------------------------------------------------------------------

fates.add_style {
	-- NOTE: This is just for display purpose, and never actually gets
	-- used.
	name = "CLOUDY",
	desc = "cloudy",
	get_chance =
		function(fate_spec, fate_style, fate_type)
			return 0
		end,
	dont_create =
		function()
			return true
		end
}

fates.add_style {
	name = "MISC",
	desc = "misc.",
	get_chance =
		function(fate_spec, fate_style, fate_type)
			return 0
		end,
}

fates.add_style {
	name = "PERMANENT",
	desc = "permanent",
	get_chance =
		function(fate_spec, fate_style, fate_type)
			return 0
		end,
}

fates.add_style {
	name = "ON_ENTER_LEVEL",
	desc = "on entering a level",

	sort = 
		function(a, b)
			return a.dun_level < b.dun_level
		end,

	get_chance =
		function(fate_spec, fate_style, fate_type)
			return 100
		end,

	convenience_arr = "curr_enter_lev_fates",

	on_create =
		function(fate, fate_spec, fate_style)
			if fate.dun_level or fate_spec.dun_level then
				return
			end

			local lev
			if fate_spec.want_dun_level then
				lev = fate_spec.want_dun_level
			else
				lev = fates.get_rand_dun_level(nil, nil, true)
			end

			lev = fates.reserve_dun_level(lev, fate_spec.tolerance)

			fate.dun_level = lev
		end -- on_create()

	validate =
		function(fate, fate_type)
			assert(fate.chance,
				   "Fate type '" .. fate_type.name .. "': fates of style '" ..
					   fates.styles[fate.style].desc ..
					   "' needs a chance field.")
			assert(fate.dun_level,
				   "Fate type '" .. fate_type.name .. "': fates of style '" ..
					   fates.styles[fate.style].desc ..
					   "' needs a dun_level field.")
		end, -- validate()

	partial_desc =
		function(fate, fate_type, partial_str)
			local style_str = "you go to level " .. fate.dun_level
			return fates.partial_desc_aux(fate, fate_type, partial_str,
										  style_str)
		end, -- partial_desc()

	wiz_fate_prompt_post =
		function(fate, fate_type, fate_style)
			fate.dun_level = fate.dun_level or 1
			local str = get_string("Which level? (currently "..
								   (fate.dun_level or "nothing") .. ")")
			if not str or str == "" then
				return
			end

			if fate.dun_level then
				fates.release_dun_level(fate.dun_level)
			end
			fate.dun_level = tonumber(str) or fate.dun_level
			fates.reserve_dun_level(fate.dun_level, 0)

			fate.chance = fate.chance or 100
			str = get_string("What percent chance? (currently " ..
							 (fate.chance or "nothing") .. ")")
			if not str or str == "" then
				return
			end
			fate.chance = tonumber(str) or fate.chance
		end -- wiz_fate_prompt
}

fates.add_style {
	name = "DURING_LEVEL",
	desc = "while staying on level",

	sort = 
		function(a, b)
			return a.dun_level < b.dun_level
		end,

	get_chance =
		function(fate_spec, fate_style, fate_type)
			return 100
		end,
	convenience_arr = "curr_during_lev_fates",

	validate =
		function(fate, fate_type)
			assert(fate.chance_per_turn,
				   "Fate type '" .. fate_type.name .. "': fates of style '" ..
					   fates.styles[fate.style].desc ..
					   "' needs a chance_per_turn field.")
			assert(fate.dun_level,
				   "Fate type '" .. fate_type.name .. "': fates of style '" ..
					   fates.styles[fate.style].desc ..
					   "' needs a dun_level field.")
		end,

	partial_desc =
		function(fate, fate_type, partial_str)
			local style_str

			if fate.quality and fate.quality < 0 then
				style_str = "you tary too long on level " ..
					fate.dun_level
			else
				style_str = "you brave level " .. fate.dun_level ..
					" for long enough"
			end
			return fates.partial_desc_aux(fate, fate_type, partial_str,
										  style_str)
		end,

	on_fate_remove =
		function(fate)
			flag_remove(level_flags.flags, FLAG_DURATION_FATE)
		end,

	wiz_fate_prompt_post =
		function(fate, fate_type, fate_style)
			fate.dun_level = fate.dun_level or 1
			local str = get_string("Which level? (currently "..
								   (fate.dun_level or "nothing") .. ")")
			if not str or str == "" then
				return
			end
			fate.dun_level = tonumber(str) or fate.dun_level

			fate.chance = fate.chance or 100
			str = get_string("What 1-in-chance per turn? (currently " ..
							 (fate.chance or "nothing") .. ")")
			if not str or str == "" then
				return
			end
			fate.chance = tonumber(str) or fate.chance
		end -- wiz_fate_prompt
}

fates.add_style {
	name = "ON_ENTER_DUN",
	desc = "on entering a dungeon",

	sort = 
		function(a, b)
			return a.dungeon < b.dungeon
		end,

	get_chance =
		function(fate_spec, fate_style, fate_type)
			return 100
		end,
	convenience_arr = "curr_enter_dun_fates",

	on_validate =
		function(fate, fate_type)
			assert(fate.chance,
				   "Fate type '" .. fate_type.name .. "': fates of style '" ..
					   fates.styles[fate.style].desc ..
					   "' needs a chance field.")
			assert(fate.level,
				   "Fate type '" .. fate_type.name .. "': fates of style '" ..
					   fates.styles[fate.style].desc ..
					   "' needs a level field.")
		end,

	partial_desc =
		function(fate, fate_type, partial_str)
			local style_str = "you go to " .. d_info(fate.dungeon)
			return fates.partial_desc_aux(fate, fate_type, partial_str,
										  style_str)
		end,
}

fates.add_style {
	name = "DURING_DUN",
	desc = "while staying in a dungeon",

	sort = 
		function(a, b)
			return a.dungeon < b.dungeon
		end,

	get_chance =
		function(fate_spec, fate_style, fate_type)
			return 100
		end,
	convenience_arr = "curr_during_dun_fates",

	validate =
		function(fate, fate_type)
			assert(fate.chance_per_turn,
				   "Fate type '" .. fate_type.name .. "': fates of style '" ..
					   fates.styles[fate.style].desc ..
					   "' needs a chance_per_turn field.")
			assert(fate.level,
				   "Fate type '" .. fate_type.name .. "': fates of style '" ..
					   fates.styles[fate.style].desc ..
					   "' needs a level field.")
		end,

	partial_desc =
		function(fate, fate_type, partial_str)
			local style_str
			if fate.quality and fate.quality < 0 then
				style_str = "you tary too long at " ..
					d_info(type.dungeon).name
			else
				style_str = "you brave " .. d_info(type.dungeon).name ..
					" for long enough"
			end
			return fates.partial_desc_aux(fate, fate_type, partial_str,
										  style_str)
		end,

	on_fate_remove = 
		function(fate)
			flag_remove(level_flags.flags, FLAG_DURATION_FATE)
		end,
}

fates.add_style {
	name = "DURING_WILD",
	desc = "while traveling through the wilderness",

	get_chance =
		function(fate_spec, fate_style, fate_type)
			return 100
		end,
	convenience_arr = "curr_during_wild_fates",

	validate =
		function(fate, fate_type)
			assert(fate_type.do_now,
				   "Fate type '" .. fate_type.name .. "' returned a " ..
					   "during wilderness style fate, but has no " ..
					   "do_now() field.")
		end
}

fates.add_style {
	name = "MOVE_WILD",
	desc = "on moving a square through the wilderness",

	get_chance =
		function(fate_spec, fate_style, fate_type)
			return 100
		end,
	convenience_arr = "curr_move_wild_fates",

	validate =
		function(fate, fate_type)
			assert(fate_type.do_now,
				   "Fate type '" .. fate_type.name .. "' returned a " ..
					   "move wilderness style fate, but has no " ..
					   "do_now() field.")
		end
}

fates.add_style {
	name = "ON_KILL_MON",
	desc = "on killing a monster",

	get_chance =
		function(fate_spec, fate_style, fate_type)
			return 100
		end,

	validate =
		function(fate, fate_type)
			assert(fate.r_idx,
				   "Fate type '" .. fate_type.name .. "': fates of style '" ..
					   fates.styles[fate.style].desc ..
					   "' needs an r_idx field.")
		end,

	partial_desc =
		function(fate, fate_type, partial_str)
			local style_str
			local monst = r_inf(fate.r_idx)
			local name = strlower(monst.name)

			if has_flag(monst, FLAG_UNIQUE) then
				-- Some uniques can be killed more than once, so...
				if not fate.count then
					style_str = "you kill " .. book_capitals(name)
				elseif fate.count == 1 then
					style_str = "you kill " .. book_capitals(name) ..
						" once more"
				else
					style_str = "you kill " .. book_capitals(name) ..
						" " .. fate.count .. " more times"
				end
			elseif not fate.count then
				style_str = "you kill a " .. name
			elseif count == 1 then
				style_str = "you kill one more " .. name
			else
				style_str = "you kill " .. fate.count .. " more " ..
					pluralize(name)
			end

			return fates.partial_desc_aux(fate, fate_type, partial_str,
										  style_str)
		end,

	on_game_start =
		function(fate)
			fates.curr_kill_mon_fates[fate.r_idx] = fate
		end,

	on_fate_acquire =
		function(fate)
			fates.curr_kill_mon_fates[fate.r_idx] = fate
		end,

	on_fate_exec =
		function(fate)
			local monst = r_info(fate.r_idx)

			if has_flag(monst, FLAG_UNIQUE) and monst.max_num == 0 then
				-- Only one chance to kill a unique
				fate.done = true
			end

			if fate.count and fate.count == 0 then
				-- Only get one chance for "kill X number of monsts"
				fate.done = true
			end
		end,

	on_fate_remove =
		function(fate)
			fates.curr_kill_mon_fates[fate.r_idx] = nil
		end,
}
fates.curr_kill_mon_fates   = {}

----------------------------------------------------------------------------
-- Default fate types
----------------------------------------------------------------------------

------------------
-- Meeting uniques
------------------
function fates.get_unique_r_idx(fate_spec)
	fate_spec = fate_spec or {}

	if fate_spec.r_idx then
		return fate_spec.r_idx
	end

	local level = fate_spec.mon_level or infinity

	-- By default, we don't care about monster friendlyness.
	local min_friend = 1
	local max_friend = -1

	if fate_spec.min_friend or fate_spec.max_friend then
		min_friend = fate_spec.min_friend or -infinity
		max_friend = fate_spec.max_friend or infinity
	elseif fate_spec.quality or fate_spec.min_quality or
		fate_spec.max_quality then
		if (fate_spec.quality and fate_spec.quality > 0) or
			(fate_spec.min_quality and fate_spec.min_quality > 0)
		then
			min_friend = 1
			max_friend = infinity
		elseif (fate_spec.quality and fate_spec.quality < 0) or
			(fate_spec.max_quality and fate_spec.max_quality < 0)
		then
			max_friend = -1
			min_friend = -infinity
		end
	end

	-- Don't want to go to the trouble of being fated to
	-- meet never dying monsters.
	local flags_need   = flag_new()
	local flags_forbid = flag_new()

	flags_need[FLAG_UNIQUE]     = true

	-- Exclude monsters with the NEW_MONSTER_PRE flag (which can
	-- act like SPECIAL_GENE) and monsters with the ON_DEATH
	-- flag (since it might do things like keep the unique from
	-- truely dying, which can mess up figuring out whether or
	-- not the fate is truely stale).
	flags_forbid[FLAG_NEW_MONSTER_PRE] = true
	flags_forbid[FLAG_ON_DEATH]        = true

	local r_idx = get_rand_r_idx(level, FACTION_PLAYER, min_friend, max_friend,
								 flags_need, flags_forbid,
								 nil, nil, nil, nil)

	flag_free(flags_need, true)
	flag_free(flags_forbid, true)

	return r_idx
end -- fates.get_unique_r_idx()

function fates.get_ok_unique_list(lev)
	local list = {}

	for r_idx = 1, max_r_idx - 1 do
		local monst = r_info(r_idx)

		-- Exclude monsters with the NEW_MONSTER_PRE flag (which can
		-- act like SPECIAL_GENE) and monsters with the ON_DEATH
		-- flag (since it might do things like keep the unique from
		-- truely dying, which can mess up figuring out whether or
		-- not the fate is truely stale).
		if has_flag(monst, FLAG_UNIQUE) and
			not has_flag(monst, FLAG_SPECIAL_GENE) and
			not has_flag(monst, FLAG_NEVER_GENE) and
			not has_flag(monst, FLAG_NEW_MONSTER_PRE) and
			not has_flag(monst, FLAG_ON_DEATH) and
			not monst.on_saved and
			monst.max_num > 0 and monst.cur_num == 0 and
			monst.level <= lev
		then
			tinsert(list, r_idx)
		end
	end

	return list
end -- fates.get_ok_unique_list()

fates.add_type {
	name      = "MEET_UNIQUE",
	type_desc = "Meet a unique monster fate",

	min_quality = -1, max_quality = 1,

	get_chance =
		function(fate_spec)
			return 100
		end,

	dont_create =
		function(fate_spec)
			fate_spec = fate_spec or {}

			fate_spec.mon_level = fate_spec.mon_level or
				fates.get_max_rand_level()

			if fates.get_unique_r_idx(fate_spec) == 0 then
				return true
			end

			return false
		end -- dont_create()

	styles = {fates.styles.ON_ENTER_LEVEL},

	create =
		function(fate_spec, fate_type)
			local fate = {
				chance = fate_spec.chance or 100,
				style = fates.styles.ON_ENTER_LEVEL.index,
			}

			if fate_spec.r_idx then
				local monst = r_info(fate_spec.r_idx)

				assert(has_flag(monst, FLAG_UNIQUE),
					   monst.name .. " is not unique")
				assert(monst.cur_num == 0,
					   monst.name .. " already exists on this level")
				assert(monst.max_num > 0,
					   monst.name .. " is dead")
				assert(not monst.on_saved,
					   monst.name .. " is saved on a persistant level")

				fate.r_idx = fate_spec.r_idx

				if not fate.chance or fate.chance >= 100 then
					r_info(fate.r_idx).on_saved = true
				end

				return fate
			end

			if not fate_spec.mon_level then
				-- Make sure random level isn't too low to generate any
				-- uniques.
				local list    = fates.get_ok_unique_list(infinity)
				local min_lev = infinity
				for i = 1, getn(list) do
					min_lev = min(min_lev, r_info(list[i]).level)
				end

				fate_spec.mon_level =
					fates.get_rand_dun_level(min_lev, nil, true)
			end

			local r_idx = fates.get_unique_r_idx(fate_spec)

			if r_idx == 0 then
				if wizard then
					message(color.VIOLET,
							"ERROR: Couldn't create meet-unique fate, " ..
								"couldn't get random r_idx")
				end
				return nil
			end

			fate.r_idx = r_idx
			local monst = r_info(r_idx)

			-- Reserve unique for certain fate, otherwise the player
			-- might meet and kill the unique before the fate
			-- conditions being met.
			if not fate.chance or fate.chance >= 100 then
				monst.on_saved = true
			end

			if fate_spec.quality and fate_spec.quality ~= 0 then
				-- If the fate is good or bad, the unique will try
				-- to be right next to you, to harm you for a bad
				-- fate or help you for a good fate.  It will also
				-- be awake to do so.
				fate.min_dist = 1
				fate.awake    = true
			end

			return fate
		end, -- create()

	execute = 
		function(fate)
			fate.stale = true
			fate.done  = true

			local monst = r_info(fate.r_idx)
			if monst.max_num == 0 or (fate.chance >= 100 and
									  not monst.on_saved)
			then
				-- Unique has died before being met, or race entry
				-- is improperly set up; should never happen.
				monst.on_saved = false
				fate.remove    = true
				fate.done      = false

				message("You experience a strange fit of vertigo for " ..
						"a moment.")

				if wizard then
					if not (has_flag(monst, FLAG_NO_DEATH) or monst.on_saved)
					then
						message(color.VIOLET,
								"ERROR: unique " .. monst.name ..
									"not on_saved, even though it was " ..
									"fated to be here.")
					end

					if monst.max_num == 0 then
						message(color.VIOLET,
								"ERROR: unique " .. monst.name ..
									"already dead, even though it was " ..
									"fated to be here.")
					end
				end -- if wizard then
				return false
			end -- if not monst.on_saved or monst.max_num == 0 then

			if monst.cur_num > 0 then
				-- Monster is already on level, should never happen.
				message("You experience a strange feeling of deja-vu.")

				-- Find unique and stuff it's idx into the fate.
				foreach_monster(function(monster, m_idx)
					if monser.r_idx == %fate.r_idx then
						%fate.m_idx = m_idx
					end
				end) -- foreach_monster()

				if wizard then
					message(color.VIOLET,
							"WARN: Fated-to-meet-unique already on " ..
								"level.")
				end

				return false
			end -- if monst.cur_num()

			-- Unreserve before generating
			monst.on_saved = false

			local dist      = fate.min_dist or 10
			local max_dist  = fate.max_dist or max(10, dist)
			local tries     = 0
			local max_tries = max(100, (max_dist - dist + 1) * 10)
			local placed    = false
			local oy, ox

			while not placed and tries < max_tries do
				oy, ox = get_pos_player(dist)

				local c_ptr = cave(oy, ox)

				if m_allow_special[fate.r_idx + 1] or
					c_ptr.info & CAVE_FREE == 0 
				then
					placed = true
					break
				end
				tries = tries + 1

				if imod(tries, 10) == 0 then
					dist = min(dist + 1, max_dist)
				end
			end -- while not placed and tries < 100 do

			if oy == 0 and ox == 0 then
				if wizard then
					message("WARN: get_pos_player() couldn't find " ..
							"location for meet-unique fate.")
				end
			end

			if not placed then
				-- Couldn't place, delay fate.
				fate.stale = false
				fate.done  = false
				if not fate.chance or fate.chance >= 100 then
					-- Not done, reserve again.
					monst.on_saved = true
				end

				if wizard then
					message("WARN: couldn't place '" .. monst.name ..
							"' for meet-unique fate.")
				end

				if not (fate.hide_feeling or fates.hide_feelings) then
					message("You feel that you have avoided your fate... " ..
							"for now.")
				end

				return false
			end

			local faction = (fate.faction or get_flag(monst, FLAG_FACTION) or
							 FACTION_DUNGEON)
			local ego_idx = 0
			local sleep   = false

			if fate.awake then
				sleep = false
			elseif fate.sleep then
				sleep = true
			elseif fate.sleep_chance then
				sleep = rng.percent(fate_spec.sleep_chance)
			elseif fate.chance then
				sleep = (fate.chance == 100)
			end

			-- Allow SPECIAL_GENE uniques to be generated, since if the
			-- monster has SPECIAL_GENE then the r_idx must have specifically
			-- been set in the fate spec, and not randomly generated.
			local old_val = m_allow_special[fate.r_idx]
			m_allow_special[fate.r_idx] = true
			local m_idx = place_monster_one(oy, ox, fate.r_idx, ego_idx,
											sleep, faction)
			m_allow_special[fate.r_idx] = old_val

			if m_idx == 0 then
				-- Couldn't place, delay fate.
				fate.stale = false
				fate.done  = false
				if not fate.chance or fate.chance >= 100 then
					-- Not done, reserve again.
					monst.on_saved = true
				end

				if wizard then
					message("ERROR: place_monster_one() failed for fate " ..
							"meeting of " .. monst.name)
				end

				if not fate.hide_feeling then
					message("You feel that you have avoided your fate... " ..
							"for now.")
				end

				return false
			end -- if m_idx == 0 then

			-- If the monster has just arrived, does the player notice?
			if fate.chance_per_turn and monster(m_idx).ml and
				not player.has_intrinsic(FLAG_HALLUCINATE)
			then
				message("You suddenly notice " ..
						book_capitals(monst.name) .. ".")
			end

			fate.m_idx = m_idx

			return true
		end, -- exec_func()

	mark_as_stale =
		function(fate)
			fate.stale = true

			if fate.known and not fate.cloudy_type then
				fates.remove_fate(fate)

				if fate.chance and fate.chance < 100 then
					-- Not guaranteed to meet the unique elswhere
					return
				end

				if (fate.dun_level and fate.dun_level == dun_level) or
					(fate.dungeon and fate.dungeon == current_dungeon_idx)
				then
					-- Player was fated to meet the unique here, but
					-- it got generated in a weird way
					return
				end

				local monst = r_info(fate.r_idx)
				local name = book_capitals(monst.name)

				message("Weren't you supposed to meet " .. name ..
						" elsewhere?")
			end
		end -- make_stale()

	hooks = {
		-- Can't meet a unique if it's dead, so if there's any
		-- remaining fates to meet this unique, make them stale.
		[hook.MONSTER_DEATH] =
			function(m_idx, who)
				local monst = monster(m_idx)

				if has_flag(monst, FLAG_UNIQUE) then
					local list = fates.get_curr_fates()

					for i = 1, getn(list) do
						local fate = list[i]
						if fate.type == fates.types.MEET_UNIQUE.index and
							fate.r_idx == monst.r_idx
						then
							fates.types[fate.type].mark_as_stale(fate)
						end
					end
				end
			end -- [hook.MONSTER_DEATH]
	} -- hooks

	partial_desc =
		function(fate)
			local monst = r_info(fate.r_idx)
			return "meet " .. book_capitals(monst.name)
		end,

	wiz_fate_prompt_pre =
		function(fate_spec, fate_type, fate_style)
			local lev   = fate_spec.mon_level or fates.get_max_rand_level()
			local list  = fates.get_ok_unique_list(lev)
			local names = {}
			assert(getn(list) > 0, "No uniques left to choose from.")

			sort(list,
				 function(a, b)
					 return r_info(a).level < r_info(b).level
				 end)

			for i = 1, getn(list) do
				tinsert(names, book_capitals(r_info(list[i]).name))
			end

			local sel = choose_from_list_simple(names, "Choose unique:",
												true)

			if not sel then
				return
			end

			fate_spec.r_idx = list[sel]
		end -- wiz_fate_prompt_pre
} -- Meet unique fate

--------------------
-- Finding artifacts
--------------------
function fates.get_ok_art_list(lev)
	local list = {}

	for a_idx = 1, max_a_idx - 1 do
		local art = a_info(a_idx)

		if not has_flag(art, FLAG_SPECIAL_GENE) and
			not has_flag(art, FLAG_NEVER_GENE) and
			not art.known and art.cur_num == 0 and
			art.level <= lev and art.rarity > 0
		then
			tinsert(list, a_idx)
		end
	end

	return list
end -- fates.get_ok_art_list()

function fates.get_art_idx(fate_spec)
	fate_spec = fate_spec or {}

	if fate_spec.a_idx then
		return fate_spec.a_idx
	end

	local level = fate_spec.art_level or infinity
	local arts  = fates.get_ok_art_list(level)

	if getn(arts) == 0 then
		return nil
	end

	while true do
		local a_idx = arts[rng(getn(arts))]

		if rng(a_info(a_idx).rarity) == 1 then
			return a_idx
		end
	end
end -- fates.get_art_idx()

function fates.art_name(art)
	if type(art) == "number" then
		art = a_info(art)
	end
	local name = "the " .. clean_whitespace(gsub(art.name, "[&~]", ""))
	return book_capitals(name)
end -- fates.art_name()

fates.add_type {
	name      = "FIND_ART",
	type_desc = "Find an artifact fate",

	quality = 1,

	get_chance =
		function(fate_spec)
			return 100
		end,

	dont_create =
		function(fate_spec)
			fate_spec = fate_spec or {}

			if fate_spec.quality and fate.spec_quality ~= 1 then
				return true
			end

			if fate_spec.min_quality and fate_spec.min_quality > 1 then
				return true
			end

			if fate_spec.max_quality and fate_spec.max_quality < 1 then
				return true
			end

			if fate_spec.a_idx then
				return false
			end

			local level = fate_spec.art_level or fates.get_max_rand_level()
			local arts  = fates.get_ok_art_list(level)

			if getn(arts) == 0 then
				return true
			end

			return false
		end -- dont_create()

	styles = {fates.styles.ON_ENTER_LEVEL},

	create =
		function(fate_spec, fate_type)
			local fate = {
				chance = fate_spec.chance or 100,
				style = fates.styles.ON_ENTER_LEVEL.index,
			}

			if fate_spec.a_idx then
				local art = a_info(fate_spec.a_idx)

				assert(art.cur_num == 0,
					   art.name .. " already exists")
				assert(not art.known,
					   art.name .. " is already known")

				fate.a_idx  = fate_spec.a_idx
				art.cur_num = 1

				return fate
			end

			if not fate_spec.art_level then
				-- Make sure random level isn't too low to generate any
				-- artifacts.
				local list    = fates.get_ok_art_list(infinity)
				local min_lev = infinity
				for i = 1, getn(list) do
					min_lev = min(min_lev, r_info(list[i]).level)
				end

				fate_spec.art_level =
					fates.get_rand_dun_level(min_lev, nil, true)
			end

			local a_idx = fates.get_art_idx(fate_spec)

			if not a_idx or a_idx == 0 then
				if wizard then
					message(color.VIOLET,
							"ERROR: Couldn't create find-art fate, " ..
								"couldn't get random a_idx")
				end
				return nil
			end

			-- Reserve artifact, otherwise the player might find the
			-- artfiact before the fate occurs.
			fate.a_idx  = a_idx
			local art   = a_info(a_idx)
			art.cur_num = 1

			return fate
		end, -- create()

	execute = 
		function(fate)
			fate.stale = true
			fate.done  = true

			local art = a_info(fate.a_idx)

			if art.known then
				-- Artifact has already been ID'd by player
				fate.remove = true
				fate.done   = false

				message("You experience a strange feeling of deja-vu.")

				if wizard then
					message(color.VIOLET,
							"ERROR: artifact " .. art.name .. " already " ..
								"known, even though it was fated to " ..
								"be here.")
				end

				return false
			end

			if fate.chance and fate.chance < 100 and art.cur_num > 0 then
				-- Artifact has already been generated, but hasn't been
				-- reserved; this is okay.
				return false
			elseif art.cur_num > 1 then
				-- Artifact has already been generated in spite of
				-- being reserved; shouldn't happen.
				fate.remove = true
				fate.done   = false

				message("You experience a strange fit of vertigo for " ..
						"a moment.")

				if wizard then
					message(color.VIOLET,
							"ERROR: artifact " .. art.name .. " already " ..
								"exists, even though it was fated to " ..
								"be here.")
				end

				return false
			end

			-- Unreserve before generating
			art.cur_num = 0

			local dist      = fate.min_dist or 10
			local max_dist  = fate.max_dist or max(10, dist)
			local tries     = 0
			local max_tries = max(100, (max_dist - dist + 1) * 10)
			local placed    = false
			local oy, ox

			while not placed and tries < max_tries do
				oy, ox = get_pos_player(dist)

				local c_ptr = cave(oy, ox)

				if c_ptr.info & CAVE_FREE == 0 
				then
					placed = true
					break
				end
				tries = tries + 1

				if imod(tries, 10) == 0 then
					dist = min(dist + 1, max_dist)
				end
			end -- while not placed and tries < 100 do

			if oy == 0 and ox == 0 then
				if wizard then
					message("WARN: get_pos_player() couldn't find " ..
							"location for find-art fate.")
				end
			end

			if not placed then
				-- Couldn't place, delay fate.
				fate.stale = false
				fate.done  = false
				if not fate.chance or fate.chance >= 100 then
					-- Not done, reserve again.
					art.cur_num = 1
				end

				if wizard then
					message("WARN: couldn't place '" .. art.name ..
							"' for find-art fate.")
				end

				if not (fate.hide_feeling or fates.hide_feelings) then
					message("You feel that your fate is just beyond " ..
							"your grasp.")
				end

				return false
			end

			local obj   = new_object()
			local k_idx = lookup_kind(art.tval, art.sval)

			object_wipe(obj)
			object_prep(obj, k_idx)
			obj.artifact_id = fate.a_idx
			apply_magic(obj, -1, true, true, true)

			if art.d_attr ~= -1 then
				obj.d_attr = art.d_attr
			end
			if art.x_attr ~= -1 then
				obj.x_attr = art.x_attr
			end

			local o_idx = drop_near(obj, -1, oy, ox)

			-- Shouldn't happen, but be paranoid.
			if o_idx == 0 then
				-- Couldn't place, delay fate.
				fate.stale = false
				fate.done  = false
				if not fate.chance or fate.chance >= 100 then
					-- Not done, reserve again.
					art.cur_num = 1
				end

				if wizard then
					message("WARN: drop_near() for '" .. art.name ..
							"' failed for find-art fate.")
				end

				if not (fate.hide_feeling or fates.hide_feelings) then
					message("You feel that your fate is just beyond " ..
							"your grasp.")
				end
				return false
			end

			fate.obj   = obj
			fate.o_idx = o_idx

			return true
		end, -- exec_func()

	mark_as_stale =
		function(fate)
			fate.stale = true

			if fate.known and not fate.cloudy_type then
				fates.remove_fate(fate)

				if fate.chance and fate.chance < 100 then
					-- Not guaranteed to find the artifact elswhere
					return
				end

				if (fate.dun_level and fate.dun_level == dun_level) or
					(fate.dungeon and fate.dungeon == current_dungeon_idx)
				then
					-- Player was fated to find the artifact here, but
					-- it got generated in a weird way
					return
				end

				local name = fates.art_name(fate.a_idx)
				message("Weren't you supposed to find " .. name ..
						"elsewhere?")
			end
		end -- make_stale()

	hooks = {
		-- If an artifact is identified, then any fate about finding it
		-- becomes stale.
		[hook.IDENTIFY_POST] =
			function(obj, item, full)
				if is_artifact(obj) then
					local list = fates.get_curr_fates()

					for i = 1, getn(list) do
						local fate = list[i]
						if fate.type == fates.types.FIND_ART.index and
							fate.a_idx == obj.artifact_id
						then
							fates.types[fate.type].mark_as_stale(fate)
						end
					end
					
				end
			end -- [hook.IDENTIFY_POST]
	} -- hooks

	partial_desc =
		function(fate)
			return "find " .. fates.art_name(fate.a_idx)
		end,

	wiz_fate_prompt_pre =
		function(fate_spec, fate_type, fate_style)
			local lev   = fate_spec.art_level or fates.get_max_rand_level()
			local list  = fates.get_ok_art_list(lev)
			local names = {}
			assert(getn(list) > 0, "No artifacts left to choose from.")

			sort(list,
				 function(a, b)
					 return a_info(a).level < a_info(b).level
				 end)

			for i = 1, getn(list) do
				tinsert(names, fates.art_name(list[i]))
			end

			local sel = choose_from_list_simple(names, "Choose artifact:",
												true)

			if not sel then
				return
			end

			fate_spec.a_idx = list[sel]
		end -- wiz_fate_prompt_pre
} -- Meet unique fate

--------
-- Misc.
--------
fates.add_type {
	name      = "TEST",
	type_desc = "Test fate styles.",

	get_chance =
		function(fate_spec)
			return 0
		end,

	styles = {"ALL"},

	create  =
		function(fate_spec, fate_type)
			local fate = {style = fate_spec.ok_styles[1].index}
			return fate
		end, -- create()

	execute = 
		function(fate)
			message("#yAttemtping to executing test fate.")
			return true
		end -- exec_func()

	partial_desc =
		function(fate)
			return "see if this fate style works"
		end
} -- add test fate

-----------------------------------------------------------------------------
-----------------------------------------------------------------------------

hook(hook.DEBUG_COMMAND,
function(cmd)
	if strchar(cmd) == '+' then
		term.save()
		wizcmd.acquire_fate()
		term.load()
		return true
	end

	return false
end)

function wizcmd.acquire_fate()
	assert(fates.max_type_idx  > 0, "No fate types to choose from")
	assert(fates.max_style_idx > 0, "No fate styles to choose from")

	local number = get_string("Number of fates? [return for 1] ")

	if not number then
		return
	end

	number = clean_whitespace(number)

	if number == "" then
		number = 1
	else
		number = tonumber(number)

		if not number or number < 1 then
			return
		end
	end

	local ret, key =
		get_com("[r]andom or [s]pecific fate?")

	if not ret then
		return
	end

	key = strlower(strchar(key))

	if key == "r" then
		for i = 1, number do
			local fate_spec = {silent = (number > 1)}
			if not fates.acquire_fate(fate_spec, true) then
				message("fates.acquire_fate() failed, bailing; only " ..
						(i - 1 ) .. " fates acquired")
				return
			end
		end
		message(number .. " fates acquired")
		return
	elseif key ~= "s" then
		return
	end

	-- User specificies type and style
	local fate_spec = {}

	-- Get type
	local list = {"RANDOM", "GOOD", "BAD"}
	local reverse = {nil}

	for i = 1, fates.max_type_idx do
		local fate_type = fates.types[i]

		local desc = fate_type.type_desc

		if not desc then
			desc = fate_type.name
		else
			desc = desc .. " [" .. fate_type.name .. "]"
		end

		if fate_type.get_chance(fate_spec) <= 0 then
			desc = desc .. " (0% chance)"
		end

		if fate_type.dont_create and fate_type.dont_create(fate_spec) then
			-- Skip
		else
			tinsert(list, desc)
			tinsert(reverse, fate_type)
		end

	end

	local num_types = getn(list) - 1
	if num_types == 0 then
		message(color.VIOLET, "ERROR: No types available.")
		return
	end

	local sel = choose_from_list_simple(list, "Choose type", true)
	if not sel then
		return
	end

	local type_random, fate_type
	local style_list = {}

	if sel <= 3 then
		type_random = true

		if sel == 2 then
			fate_spec.min_quality = 1
		elseif sel == 3 then
			fate_spec.max_quality = -1
		end

		fate_spec.ok_types = fates.ok_types_from_spec(fate_spec)

		if sel == 2 or sel == 3 then
			fate_spec.quality = fates.get_rand_quality(fate_spec)
		end

		local style_idxes = {}
		for i = 1, fates.max_type_idx do
			local fate_type = fates.types[i]

			for j = 1, getn(fate_type.styles) do
				style_idxes[fate_type.styles[i].index] = true
			end
		end

		for idx, v in style_idxes do
			tinsert(style_list, fates.styles[idx])
		end
	else
		type_random = false
		fate_type   = reverse[sel - 3]

		style_list = fate_type.styles

		fate_spec.ok_types = {fate_type}
	end

	if getn(style_list) == 0 then
		message(color.VIOLET,
				"ERROR: no applicable styles.")
		return
	end

	-- Get style
	local fate_style, style_random

	if getn(style_list) == 1 then
		style_random = false
		fate_style = style_list[1]
	else
		list = {}
		reverse = {}

		if not type_random then
			tinsert(list, "RANDOM")
			tinsert(reverse, nil)
		end

		for i = 1, getn(style_list) do
			local style = style_list[i]

			local str = style.desc

			if style.get_chance(fate_spec, fate_style, fate_type) < 0 then
				str = str .. " (0% chance)"
			end

			if style.dont_create and
				style.dont_create(fate_spec, fate_style, fate_type)
			then
				-- Skip
			else
				tinsert(list, str)
				tinsert(reverse, style)
			end
		end

		local num_styles = getn(list) - iif(type_random, 1, 0)
		if num_styles == 0 then
			message(color.VIOLET,
					"ERROR: No available styles")
			return
		end

		sel = choose_from_list_simple(list, "Choose style", true)
		if not sel then
			return
		end

		if type_random then
			style_random = false
			fate_style   = style_list[sel]
		else
			if sel == 1 then
				style_random = true
			else
				fate_style = style_list[sel - 1]
			end
		end
	end -- if getn(style_list) ~= 1 then

	if not fate_style then
		message(color.VIOLET,
				"ERROR: not fate style was chosen.")
		return
	end

	fate_spec.ok_styles = {fate_style}

	if type_random then
		-- Select random type based on chosen style
		local ok_types = {}

		-- Find types that can use this style
		for i = 1, fates.max_type_idx do
			local fate_type = fates.types[i]
			local okay      = true

			if fate_type.get_chance(fate_spec) <= 0 then
				okay = false
			end

			if fate_type.dont_create and fate_type.dont_create(fate_spec)
			then
				okay = false
			end

			local found = false

			for j = 1, getn(fate_type.styles) do
				if fate_style.index == fate_type.styles[j].index then
					found = true
					break
				end
			end

			if not found then
				okay = false
			end

			if okay then
				tinsert(ok_types, fate_type)
			end
		end -- for i = 1, fates.max_type_idx do

		if getn(ok_types) == 0 then
			message(color.VIOLET,
					"ERROR: No fate types use chosen fate style")
			return
		end

		local total       = 0
		local alloc_table = {}

		for i = 1, getn(ok_types) do
			total = total + ok_types[i].get_chance(fate_spec) 
			alloc_table[i] = total
		end

		local alloc_num = rng(total)

		for i = 1, getn(ok_types) do
			if alloc_num <= alloc_table[i] then
				fate_type = ok_types[i]
				break
			end
		end

		fate_spec.ok_types = {fate_type}
	end -- if random_type then

	-- Style and type might have means of being even more specific
	if not type_random and fate_type.wiz_spec_prompt then
		fate_type.wiz_spec_prompt(fate_spec, fate_type, fate_style)
	end

	if not style_random and fate_style.wiz_spec_prompt then
		fate_style.wiz_spec_prompt(fate_spec, fate_type, fate_style)
	end

	if number == 1 then
		-- Allow user to be still more specific if it's just for a
		-- single fate.
		if not type_random and fate_type.wiz_fate_prompt_pre then
			fate_type.wiz_fate_prompt_pre(fate_spec, fate_type, fate_style)
		end

		if not style_random and fate_style.wiz_fate_prompt_pre then
			fate_style.wiz_fate_prompt_pre(fate_spec, fate_type, fate_style)
		end

		local fate  = fate_type.create(fate_spec, fate_type)
		if not fate then
			message(color.VIOLET,
					"fate_type.create() failed")
			return
		end

		local style = fates.styles[fate.style]
		if style.on_create then
			style.on_create(fate, fate_spec, style)
		end

		-- Non-destructively copy fate_spec onto fate
		for k, v in fate_spec do
			fate[k] = fate[k] or v
		end
		fate.ok_types  = nil
		fate.ok_styles = nil

		-- Let user fiddle with already created fate
		if not type_random and fate_type.wiz_fate_prompt_post then
			fate_type.wiz_fate_prompt_post(fate, fate_type, fate_style)
		end

		if not style_random and fate_style.wiz_fate_prompt_post then
			fate_style.wiz_fate_prompt_post(fate, fate_type, fate_style)
		end

		fates.add_specific_fate(fate, fate_type)

		return
	end -- if number == 1 then

	-- Grant multiple fates of the same kind
	fate_spec.silent = (number > 1)
	for i = 1, number do
		local fate  = fate_type.create(fate_spec, fate_type)
		if not fate then
			message(color.VIOLET,
					"fate_type.create() failed, bailing; only " ..
						(i - 1) .. " fates created")
			return
		end

		local style = fates.styles[fate.style]
		if style.on_create then
			style.on_create(fate, fate_spec, style)
		end

		-- Non-destructively copy fate_spec onto fate
		for k, v in fate_spec do
			fate[k] = fate[k] or v
		end
		fate.ok_types  = nil
		fate.ok_styles = nil

		fates.add_specific_fate(fate, fate_type)
	end
	message(number .. " fates acquired")
end -- wizcmd.acquire_fate()
