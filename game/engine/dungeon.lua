-- Internal lua file in charge of dungeon stuff

declare_global_constants {
	"dungeon",
	"find_dungeon",
	"explode_dir",
	"level_generate",
	"level_generator",
	"place_dungeon",
	"rotate_dir",
	"wild_feat",
	"set_symbols_info",
	"get_symbol_info",
	"pop_levels_adjuster_index",
	"add_levels_adjuster",
	"get_adjusted_level",
	"load_map",
	"town",
	"feat_adjacent",
}
declare_globals {
	"__level_generators",
	"__symbols_info",
}

hook(hook.INFO_DATA_LOADED, function()
	constant("TAG_FEATURE", tag(f_info[1]))
end)

hook(hook.INIT_GAME,
function(is_end)
	if is_end == "end" then
		constant("TAG_GRID", tag(cave(1, 1)))
	end
end)

function place_dungeon(y, x, d_idx)
	if d_idx then
		wild_map(y, x).entrance = 1000 + d_idx
       	else
		wild_map(y, x).entrance = 0
	end
end

function dungeon(d_idx)
	return d_info[1 + d_idx]
end

function find_dungeon(name)
	for i = 1, max_d_idx do
		if d_info[i].name == name then
			return i - 1
		end
	end
	return -1
end

function wild_feat(wild)
	return wf_info[1 + wild.feat]
end

function explode_dir(dir)
	return ddy[dir + 1], ddx[dir + 1]
end

function rotate_dir(dir, mov)
	if mov > 0 then
		if dir == 7 then dir = 8
		elseif dir == 8 then dir = 9
		elseif dir == 9 then dir = 6
		elseif dir == 6 then dir = 3
		elseif dir == 3 then dir = 2
		elseif dir == 2 then dir = 1
		elseif dir == 1 then dir = 4
		elseif dir == 4 then dir = 7
		end
	elseif mov < 0 then
		if dir == 7 then dir = 4
		elseif dir == 4 then dir = 1
		elseif dir == 1 then dir = 2
		elseif dir == 2 then dir = 3
		elseif dir == 3 then dir = 6
		elseif dir == 6 then dir = 9
		elseif dir == 9 then dir = 8
		elseif dir == 8 then dir = 7
		end
	end

	return dir
end

-- Check if the map is a filename or directly a map
function load_map(m, y, x)
--[[	if strsub(map, 1, 5) == "#!map" then
		%load_map(true, map, y, x)
	else
		%load_map(FALSE, map, y, x)
	end]]
	map.load(m, y, x)
end

-- Place a trap for a specific level
function place_trap(y, x, faction, level)
	local old_dun = dun_level
	dun_level = level or old_dun
	%place_trap(y, x, faction)
	dun_level = old_dun
end

-- Level generators processing
__level_generators = {}

function level_generator(t)
	assert(t.name, "no generator name")
	assert(t.gen, "no generator function")

	if not t.stairs then t.stairs = true end
	if not t.monsters then t.monsters = true end
	if not t.objects then t.objects = true end
	if not t.miscs then t.miscs = true end

	__level_generators[t.name] = t.gen
	add_scripted_generator(t.name, t.stairs, t.monsters, t.objects, t.miscs)
end

function level_generate(name)
	assert(__level_generators[name], "Unknown level generator '"..name.."'")
	return __level_generators[name]()
end

--[[ Example
level_generator
{
	["name"]	= "test",
	["gen"]	 = function()
			print("zog")
			for i = 1, 30 do
				cave(i, 2).feat = 1
			end
			return new_player_spot(get_branch())
	end,
}
]]

-- Load a symbols list
__symbols_info = {}
function set_symbols_info(t)
	for i = 1, getn(t) do
		local inf = t[i]

		__symbols_info[inf[1]] = inf[2]
	end
end

function get_symbol_info(sym)
	-- convert to string if needed
	if type(sym) == "number" then sym = strchar(sym) end

	if __symbols_info[sym] then
		return __symbols_info[sym]
	else
		return "Unknown symbol"
	end
end

-- If "feat", is found in a grid next to (y,x) then return true, else false.
function feat_adjacent(y, x, feat)
	local j,i
	for i = -1, 1 do for j = -1, 1 do
		if in_bounds(y + j, x + i) and cave(y + j, x + i).feat == feat then
			return true
		end
	end end
	return false
end

-- Levels adjusters
global("__levels_adjuster_next", 1)
global("__levels_adjuster", {})

function pop_levels_adjuster_index()
	__levels_adjuster_next = __levels_adjuster_next + 1
	return __levels_adjuster_next - 1
end

function add_levels_adjuster(idx, min, max)
	__levels_adjuster[idx] =
	function()
		return dostring("return "..%min), dostring("return "..%max)
	end
end

function get_adjusted_level(idx)
	return __levels_adjuster[idx]()
end

function town(idx)
	return town_info[idx + 1]
end

-- Hooks
new_flag("LEVEL_GENERATE_PRE")
hook(hook.GEN_LEVEL_BEGIN,
function()
	if level_flags.flags[FLAG_LEVEL_GENERATE_PRE] then
		local func = get_function_registry_from_flag(level_flags.flags, FLAG_LEVEL_GENERATE_PRE)
		return func()
	end
end)

new_flag("LEVEL_GENERATE")
hook(hook.GEN_LEVEL,
function()
	if level_flags.flags[FLAG_LEVEL_GENERATE] then
		local func = get_function_registry_from_flag(level_flags.flags, FLAG_LEVEL_GENERATE)
		return func()
	end
end)

new_flag("LEVEL_REGENERATE")
hook(hook.LEVEL_REGEN,
function()
	if level_flags.flags[FLAG_LEVEL_REGENERATE] then
		local func = get_function_registry_from_flag(level_flags.flags, FLAG_LEVEL_REGENERATE)
		return func()
	end
end)

new_flag("LEVEL_GENERATE_POST")
hook(hook.LEVEL_END_GEN,
function()
	if level_flags.flags[FLAG_LEVEL_GENERATE_POST] then
		local func = get_function_registry_from_flag(level_flags.flags, FLAG_LEVEL_GENERATE_POST)
		return func()
	end
end)


---------------------------------------------------------------
--------------------- Semi permanent levels -------------------
---------------------------------------------------------------

new_flag("SAVED_LEVEL_DECAY_OBJECTS")
new_flag("SAVED_LEVEL_DECAY_MONSTERS")
new_flag("SAVED_LEVEL_DECAY_EFFECTS")
new_flag("SAVED_LEVEL_DECAY_EXTRA")
new_flag("SAVED_LEVEL_NO_DECAY")

hook(hook.GEN_LEVEL_AFTER_LOAD, function(loaded, loaded_turn)
	if not loaded then return end

	local decayed_monsters, decayed_objects, decayed_effects = 0, 0, 0

	-- Do we decay monsters?
	-- Only if some time has passed
	if level_flags.flags[FLAG_SAVED_LEVEL_DECAY_MONSTERS] then
		local list = {}
		for_each_monster(function(m_idx, monst)
			-- Die or not ?
			if has_flag(monst, FLAG_SAVED_LEVEL_NO_DECAY) then
				return
			end
			if %loaded_turn + 10 * rng(flag_get(level_flags.flags, FLAG_SAVED_LEVEL_DECAY_MONSTERS), flag_get2(level_flags.flags, FLAG_SAVED_LEVEL_DECAY_MONSTERS)) < turn then
				tinsert(%list, m_idx)
			end
		end)

		decayed_monsters = getn(list)
		for idx in list do
			delete_monster_idx(idx)
		end
	end

	-- Do we decay objects?
	-- Only if some time has passed
	if level_flags.flags[FLAG_SAVED_LEVEL_DECAY_OBJECTS] then
		for y = 0, cur_hgt - 1 do
			for x = 0, cur_wid - 1 do
				-- Die or not ?
				if loaded_turn + 10 * rng(flag_get(level_flags.flags, FLAG_SAVED_LEVEL_DECAY_OBJECTS), flag_get2(level_flags.flags, FLAG_SAVED_LEVEL_DECAY_OBJECTS)) < turn then
					local inven = cave(y, x).inventory
					if flag_max_key(inven) >= 1 then
						local size = inven[INVEN_LIMIT_KEY]
						preserve_artifacts_in_inven(inven)
						flag_empty(inven)
						inven[INVEN_LIMIT_KEY] = size
						decayed_objects = decayed_objects + 1
					end
				end
			end
		end
	end

	-- Do we decay spell effects?
	-- Only if some time has passed
	if level_flags.flags[FLAG_SAVED_LEVEL_DECAY_EFFECTS] then
		local delta_time = turn - loaded_turn

		-- Delete spell effects that have "expired"
		for i = 0, MAX_EFFECTS - 1 do
			local eff = lasting_effects(i)
			if eff.time > 0 and eff.time < delta_time / 10 then
				eff.time = 0
				decayed_effects = decayed_effects + 1
			end
		end

		-- Handle temporary terrains too if available
		if has_subsystem("temporary_terrain") then
			temporary_terrain.decay(delta_time / 10)
		end
	end

	-- Extra stuff
	if level_flags.flags[FLAG_SAVED_LEVEL_DECAY_EXTRA] then
		local func = get_function_registry_from_flag(level_flags.flags, FLAG_SAVED_LEVEL_DECAY_EXTRA)
		if func then func(loaded_turn, decayed_monsters, decayed_objects, decayed_effects) end
	end
end)
