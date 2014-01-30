--
-- Code for generating a random object
--

-- Create the full item kind alloc table after everthing else is
-- initalized.  Also make a list of egos by tval, and a list of
-- all the existing tvals
hook(hook.INIT_GAME,
function(start_or_end)
	if start_or_end == "end" then
		tvals_list.create()
		rand_obj.create_full_alloc_kind_table()
		rand_obj.create_egos_by_k_idx_table()
		rand_obj.create_arts_by_k_idx_table()
	end
end)

new_flag("FORCE_EGO")
new_flag("MAX_EGO_LEVEL")
new_flag("MIN_EGO_LEVEL")

constant("tvals_list", {})
tvals_list.hash  = {}
tvals_list.array = {}

-- Only called once, at game initilization time
function tvals_list.create()
	for i = 2, max_k_idx do
		tvals_list.hash[ k_info[i].tval ] = 1
	end

	local tval = next(tvals_list.hash, nil)
	while tval do
		tinsert(tvals_list.array, tval)
		tval = next(tvals_list.hash, tval)
	end
end

constant("rand_obj", {})
rand_obj.full_alloc_kind_table = {}

rand_obj.egos_by_k_idx          = { default = {} }
rand_obj.arts_by_k_idx          = {}

-- Only called once, at game initilization time
function rand_obj.create_full_alloc_kind_table()
	local table = rand_obj.full_alloc_kind_table

	for i = 2, max_k_idx do
		if not has_flag(k_info[i], FLAG_NORM_ART) and not has_flag(k_info[i], FLAG_SPECIAL_GENE) then
			for j = 1, MAX_KIND_ALLOCATIONS do
				local entry = {}

				entry.level = k_info[i].locale[j]
				entry.prob  = k_info[i].chance[j]
				entry.index = i - 1

				if entry.prob and entry.prob > 0 then
					tinsert(table, entry)
				end
			end -- for j = 1, MAX_KIND_ALLOCATIONS do
	    end -- NORM_ART
	end

	sort(table,
		 function(itemA, itemB)
			 return itemA.level < itemB.level
		 end)
end -- rand_obj.create_full_alloc_kind_table()

-- Only called once, at game initilization time
function rand_obj.create_egos_by_k_idx_table()
	for i = 2, max_e_idx do
		local ego   = e_info[i]
		rand_obj.egos_by_k_idx[ego.ego_group] = rand_obj.egos_by_k_idx[ego.ego_group] or {}
	end

	for k, _ in rand_obj.egos_by_k_idx do
		rand_obj.create_egos_by_k_idx_table_by_group(k)
	end
end

function rand_obj.create_egos_by_k_idx_table_by_group(group)
	local table = rand_obj.egos_by_k_idx[group]

	for i = 1, max_k_idx do
		table[i] = {}
	end

	for i = 2, max_e_idx do
		local ego   = e_info[i]
		if ego.ego_group == group then
			for j = 1, MAX_TVALS_PER_EGO do
				if ego.tval[j] ~= -1 then
					for k = ego.min_sval[j], ego.max_sval[j] do
						local k_idx = lookup_kind(ego.tval[j], k)

						if k_idx ~= 0 then
							tinsert(table[k_idx + 1], i - 1)
						end
					end
				end -- if ego.tval[j] ~= -1 then
			end -- for j = 1, MAX_KIND_ALLOCATIONS do

			foreach_flags(ego.item_kinds_ok, function(tvals, tval)
				local svals = tvals[tval]
                		for j = 1, flag_max_key(svals) do
					local k_idx = lookup_kind(tval, svals[j])

					if k_idx ~= 0 then
						tinsert(%table[k_idx + 1], %i - 1)
					end
				end
			end)
		end
	end -- for i = 2, max_e_idx do
end

-- Only called once, at game initilization time
function rand_obj.create_arts_by_k_idx_table()
	local table = rand_obj.arts_by_k_idx

	for i = 1, max_k_idx do
		table[i] = {}
	end

	for i = 1, max_a_idx do
		local art   = a_info[i]
       		local k_idx = lookup_kind(art.tval, art.sval)
       		if k_idx ~= 0 then
       			tinsert(table[k_idx + 1], i - 1)
       		end
	end -- for i = 1, max_a_idx do
end

-- Modules should redefine this function unless they want the default
-- to be *all* items in k_info[] being available for random geneartion
function rand_obj.default_item_kind_tester()
	return true
end

-- Each item_kind_tester base/extra pair is only applied once per game to
-- the full allocation table.  After that, it's retrieved from the cache.
rand_obj.alloc_table_cache = {}

function rand_obj.default_extra_tester(k_idx)
	return true
end

function rand_obj.get_alloc_table(item_kind_tester)
	local base  = item_kind_tester[1]
	local extra = item_kind_tester[2]
	local cache = rand_obj.alloc_table_cache

	extra = extra or rand_obj.default_extra_tester

	if not cache[base] then
		cache[base] = {}
	end

	if cache[base] and cache[base][extra] then
		local  info = cache[base][extra]
		return info[1], info[2]
	end

	local full_table = rand_obj.full_alloc_kind_table
	local table      = {}
	local total      = 0

	for i = 1, getn(full_table) do
		if base(full_table[i].index) and extra(full_table[i].index) then
			local entry = table_copy(full_table[i])

			total       = total + entry.prob
			entry.total = total

			tinsert(table, entry)
		end
	end -- for i = 1, getn(full_table) do

	local size = getn(table)

	-- Nested tables in Lua are *slow* (gah!), so getting a random
	-- k_idx is actually implemented in C.
	local c_table = rand_k_idx.make_c_table(table)

	-- Need size, since alloc_table is a C array which doesn't contain
	-- info on its own size
	cache[base][extra] = {c_table, size}

	return c_table, size
end -- rand_obj.get_alloc_table()

function rand_obj.rand_k_idx(obj_level, obj_level_min, item_kind_tester)
	if not item_kind_tester then
		item_kind_tester = {rand_obj.default_item_kind_tester, nil}
	end

	-- Need size, since alloc_table is a C array which doesn't contain
	-- info on its own size
	local alloc_table, size = rand_obj.get_alloc_table(item_kind_tester)

	-- Nested tables in Lua are *slow* (gah!), so getting a random
	-- k_idx is actually implemented in C.
	return rand_k_idx.get(alloc_table, size, obj_level, obj_level_min)
end -- rand_obj.rand_k_idx()

-- Modules may override this function to change the chance of an
-- artifact being randomly generated
function rand_obj.get_artifact_chance(obj_level, item_kind)
	return 0
end

-- Modules may override this function to change the chance of a random item
-- getting a single ego
function rand_obj.get_single_ego_chance(obj_level, item_kind, ego_info)
	return 0
end

-- This is the chance of an item gets a second ego if it already has one
-- ego; if an item has a 0% chance of getting a single ego, then it
-- will never get two egos, regardless of the double chance.
function rand_obj.get_double_ego_chance(obj_level, item_kind, ego_info)
	return 0
end

-- Get egos that the base object can get according to some filter
-- By default, only egos of level between 0 and the object_level can be chosen
function rand_obj.get_filtered_egos(ego_list, ego_info, k_idx)
	local filtered_egos = {}
	local max_level = k_info(k_idx).flags[FLAG_MAX_EGO_LEVEL] or object_level
	local min_level = k_info(k_idx).flags[FLAG_MIN_EGO_LEVEL] or 0

	for i = 1, getn(ego_list) do
		local ego_idx = ego_list[i]

		if ego_info.filter(ego_idx, ego_info, k_idx) and 
			e_info(ego_idx).level <= max_level and
			e_info(ego_idx).level >= min_level and
			not flag_intersects(e_info(ego_idx).forbid_flags, k_info(k_idx).flags) then
			local ego          = e_info[ego_idx + 1]
			local filtered_ego = {}

			filtered_ego.alloc   = ego.allocation
			filtered_ego.ego_idx = ego_idx
			tinsert(filtered_egos, filtered_ego)
		end
	end -- for i = 1, getn(ego_list) do

	return filtered_egos
end

function rand_obj.tweak_ego_allocs(filtered_egos, obj_level, item_kind)
end

function rand_obj.pick_ego_from_alloc_table(filtered_egos, obj_level, k_idx)
	rand_obj.tweak_ego_allocs(filtered_egos, obj_level, k_idx)

	local total_alloc = 0
	for i = 1, getn(filtered_egos) do
		total_alloc            = total_alloc + filtered_egos[i].alloc
		filtered_egos[i].alloc = total_alloc
	end

	local chance = rng.number(total_alloc)

	for i = 1, getn(filtered_egos) do
		if chance < filtered_egos[i].alloc then
			return filtered_egos[i].ego_idx
		end
	end
end

function rand_obj.get_single_ego(obj_level, k_idx, ego_info, group)
	local ego_list = rand_obj.egos_by_k_idx[group or "default"][k_idx + 1]

	if getn(ego_list) == 0 then
		return
	end

	ego_info = ego_info or {}
	if not ego_info.filter then
		ego_info.filter = function(ego_idx, ego_info, k_idx) return true end
	end

	local filtered_egos = rand_obj.get_filtered_egos(ego_list, ego_info, k_idx)

	if getn(filtered_egos) == 0 then
		return
	end

	local total_prob    = 0

	for i = 1, getn(filtered_egos) do
		total_prob = total_prob + filtered_egos[i].alloc
	end -- for i = 1, getn(ego_list) do

	-- We might end up with a situation where a very rare ego is
	-- the only one that can apply to this item.  In that case,
	-- we don't want to "randomly" pick the ego out of a list of
	-- one and apply it with a 100% certainy; the same holds for
	-- a short list of a few rare egos types.
	--
	-- Thus, we add up the allocation numbers of all the egos which
	-- can apply to this objects, and use that as a percentage chance
	-- that an ego will be applied.  A single ego with an allocation
	-- of 1 will have a 1% chance of being applied, while with
	-- 100 egos, all of them alloction of 1, there's a 100% chance
	-- that one of them will be applied.  An ego can have an alloc
	-- number of 100 or more to esnure that it will always be applied
	-- if it ends up being the only one which *can* be applied.
	--
	-- An ego can be ensured in spite of ego rarity by setting
	-- "force_ego" of the ego_info table.
	if not ego_info.force_ego and rng.number(100) > total_prob then
		return
	end

	return rand_obj.pick_ego_from_alloc_table(filtered_egos, obj_level, k_idx)
end -- rand_obj.get_single_ego()

function rand_obj.get_double_ego(obj_level, k_idx, ego_info, group)
	local ego_list = rand_obj.egos_by_k_idx[group or "default"][k_idx + 1]

	if getn(ego_list) == 0 then
		return
	end

	ego_info = ego_info or {}
	if not ego_info.filter then
		ego_info.filter = function(ego_idx, ego_info, k_idx) return true end
	end

	local filtered_egos = rand_obj.get_filtered_egos(ego_list, ego_info,
													 k_idx)

	if getn(filtered_egos) == 0 then
		return
	end

	local pre_egos  = {}
	local post_egos = {}

	local total_prob_pre  = 0
	local total_prob_post = 0


	for i = 1, getn(filtered_egos) do
		local ego_idx = filtered_egos[i].ego_idx
		local ego     = e_info[ego_idx + 1]

		if ego.before then
			tinsert(pre_egos, filtered_egos[i])
			total_prob_pre = total_prob_pre + ego.allocation
		else
			tinsert(post_egos, filtered_egos[i])
			total_prob_post = total_prob_post + ego.allocation
		end
	end

	local ego1 = nil
	local ego2 = nil

	-- See comment in rand_obj.get_single_ego() for the logic behind
	-- 'rng.number(100) < total_prob_pre'
	if ego_info.force_ego or rng.number(100) < total_prob_pre then
		ego1 = rand_obj.pick_ego_from_alloc_table(pre_egos, obj_level,
												  k_idx)
	end

	if ego_info.force_ego or rng.number(100) < total_prob_post then
		ego2 = rand_obj.pick_ego_from_alloc_table(post_egos, obj_level,
												  k_idx)
	end

	if not ego1 then
		ego1, ego2 = ego2, nil
	end

	if not ego1 and not ego2 then
		-- Try to have at *least* one ego
		if rng.number(100) < (total_prob_pre + total_prob_post) then
			ego1 = rand_obj.pick_ego_from_alloc_table(filtered_egos,
													  obj_level, k_idx)
		end
	end

	return ego1, ego2
end

function rand_obj.upgrade_obj_to_artifact(obj_level, obj)
	if obj.number ~= 1 then return false end

	local art_list = rand_obj.arts_by_k_idx[obj.k_idx + 1]

	if getn(art_list) == 0 then
		return
	end

	for i = 1, getn(art_list) do
		local art = a_info(art_list[i])

		-- Basic conditions
		if art.cur_num == 0 and art.tval == obj.tval and art.sval == obj.sval and not (art.flags[FLAG_SPECIAL_GENE] and not a_allow_special[1 + art_list[i]]) then
		if art.level <= dun_level or rng(0, (art.level - dun_level) * 2) then
		if rng(0, art.rarity - luck(-(art.rarity / 2), art.rarity / 2)) == 0 then
			obj.artifact_id = art_list[i]
			apply_magic(obj, obj_level, nil, nil, nil)
			return true
		end
		end
		end
	end
	return false
end

-- Modules may override this function to generate random objects in a
-- different manner.
--
-- Note : if delay_magic is set, then 
--        apply_magic(obj, obj_level, true, false, false) 
--        _must_ be called before returning the object to the game.
--
-- NOTE: The results of the item_kind_tester function are cached
-- using the function pointer as a key.  Thus, any functions generated
-- on-the-fly should be generated just once, and then stored and
-- re-used, or it will bloat the cache, in addition to not getting any
-- of the speed benifit of the cache.
function rand_obj.get_obj(obj_level, item_kind_tester, egos, artifact_chance, add_chances, delay_magic)
	if not obj_level then
		obj_level = object_level
	end

	local obj_level_min = 0
	if type(obj_level) == "table" then
		obj_level_min = obj_level.min
		obj_level     = obj_level.max
	end

	local k_idx = rand_obj.rand_k_idx(obj_level, obj_level_min, item_kind_tester)

	if not k_idx then
		return
	end

	local item_kind = k_info(k_idx)
	local obj       = create_object(item_kind.tval, item_kind.sval)

	if not obj then
		return
	end

	if add_chances then
		artifact_chance = rand_obj.get_artifact_chance(obj_level, k_idx) + (artifact_chance or 0)
	elseif not artifact_chance then
		artifact_chance = rand_obj.get_artifact_chance(obj_level, k_idx)
	end
	if item_kind.flags[FLAG_INSTA_ART] then
		artifact_chance = 100
	end

	-- Allow for fractional percentages
	if (artifact_chance * 100) > rng.number(10000) then
		-- If object couldn't be upgraded to an artifact, then
		-- give it a chance to acquire egos
		if rand_obj.upgrade_obj_to_artifact(obj_level, obj) then
			return obj
		end

		-- We are forced into artifacting but we could not ? fail
		if item_kind.flags[FLAG_INSTA_ART] then
			return
		end
	end

	egos = egos or {}
	if add_chances then
		egos.single_chance = rand_obj.get_single_ego_chance(obj_level, k_idx, egos) + (egos.single_chance or 0)
	elseif not egos.single_chance then
		egos.single_chance = rand_obj.get_single_ego_chance(obj_level, k_idx, egos)
	end

	-- Some objects are always egos
	if has_flag(item_kind, FLAG_FORCE_EGO) then
		egos.single_chance = egos.single_chance + get_flag(item_kind, FLAG_FORCE_EGO)
	end

	if add_chances then
		egos.double_chance = rand_obj.get_double_ego_chance(obj_level, k_idx, egos) + (egos.double_chance or 0)
	elseif egos.single_chance and not egos.double_chance then
		egos.double_chance = rand_obj.get_double_ego_chance(obj_level, k_idx, egos)
	end

	local ego1 = nil
	local ego2 = nil

	-- Allow for fractional percntages
	if (egos.single_chance * 100) > rng.number(10000) then
		if (egos.double_chance * 100) > rng.number(10000) then
			ego1, ego2 = rand_obj.get_double_ego(obj_level, k_idx, egos)
		else
			ego1 = rand_obj.get_single_ego(obj_level, k_idx, egos)
		end
	end

	if ego1 then
		obj.ego_id[1]  = ego1
	end
	if ego2 then
		obj.ego_id[2] = ego2
	end

	if not delay_magic then
		apply_magic(obj, obj_level, true, nil, nil)
	end

	return obj
end -- rand_obj.get_obj()

--
-- Themed object creation
--

rand_obj.themed           = {}
rand_obj.themed.functions = {}

-- Register a new object theme type.  The theme function must return
-- a single object
function rand_obj.themed.register_new(name, func)
	if tag(name) ~= TAG_STRING then
		error("arg #1 to rand_obj.themed.register_new() must be a string")
		return
	end

	if tag(func) ~= TAG_FUNCTION and tag(func) ~= TAG_TABLE then
		error("arg #2 to rand_obj.themed.register_new() must be a function or table")
		return
	end

	new_flag(name)

	local flag = getglobal("FLAG_" .. name)

	-- If we only got a table, make teh basic function out of it
	if tag(func) == TAG_TABLE then
		local filler = function(k_idx)
			return %func[k_info[k_idx + 1].tval]
		end

		func = function(obj_level, theme_flags, extra_filter, egos, artifact_chance, add_chances)
			return rand_obj.get_obj(obj_level, {%filler, extra_filter}, egos, artifact_chance, add_chances)
		end
	end

	rand_obj.themed.functions[flag] = func
end

function rand_obj.get_themed_obj(obj_level, theme_flags, extra_filter, egos, artifact_chance, add_chances)
	local num_flags = 0
	local total     = 0
	local table     = {}
	local flag

	-- Construct table used to randomly select a theme according
	-- to how likely it is compared to all the other themes

	-- Only iterate over registered theme types, since the flag group
	-- might contain information that isn't a theme type
	local flag = next(rand_obj.themed.functions, nil)

	while flag do
		if flag_exists(theme_flags, flag) then
			num_flags        = num_flags + 1
			total            = total + flag_get(theme_flags, flag)
			table[num_flags] = {total, flag}
		end

		flag = next(rand_obj.themed.functions, flag)
	end

	if num_flags == 0 then
		error("Empty theme flagset")
		return
	end

	if total == 0 then
		error("Zero chance for all themes")
		return
	end

	local chance = rng.number(total)

	for j = 1, num_flags do
		if chance < table[j][1] then
			local flag = table[j][2]
			local func = rand_obj.themed.functions[flag]
			local obj = func(obj_level, theme_flags, extra_filter, egos,
							 artifact_chance, add_chances)

			if not obj then
				message(color.VIOLET,
						"ERROR: Theme " .. get_flag_name(flag) ..
						" returned no object!")
			end
			return obj
		end -- if chance < chance_table[j][1] then
	end -- for j = 1, num_flags do
	message(color.VIOLET,
			"ERROR: rand_obj.get_themed_obj() selected no theme!")
end

-- NOTE: The only purpose of this function is to be called by the C
-- function make_object().  Use of the C function will probably be
-- phased out, at which point this will be obsolete.
function rand_obj.make_object(good, great, theme)
	local flags = nil

	if not theme then
		theme = flag_get_flags(dungeon(current_dungeon_idx).flags, FLAG_OBJ_THEME)
	end

	if not theme then
		error("make_object(): No theme for object")
		return
	end

	local artifact_chance = nil
	local egos = nil
	if great then
		egos = { single_chance=70 double_chance=4 }
		artifact_chance = 5
	end
	local obj = rand_obj.get_themed_obj(nil, theme, nil, egos, artifact_chance, true)

	if not obj then
		return nil
	end

	apply_magic(obj, object_level, true, good, great)
	return obj
end -- rand_obj.make_object()

-- XXX
-- A debugging function to randomly acquire lots of objcets
--
function rand_obj.acquire(num)
	local flags = flag_new()

--	flag_set(flags, FLAG_THEME_ARMOR,   100)
--	flag_set(flags, FLAG_THEME_COMBAT,   100)
--	flag_set(flags, FLAG_THEME_TREASURE, 100)
	flag_set(flags, FLAG_THEME_MAGIC,    100)

	local obj_level = 4

	for i = 1, num do
		local obj = rand_obj.get_themed_obj(obj_level, flags)

		if obj then
			drop_near(obj, -1, player.py, player.px)
		end
	end
end
