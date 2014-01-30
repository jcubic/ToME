-- .map files loader

global("map", {})

map.maps = {}
map.cur_flags = {}

map.town_coord = {}
map.town_long_name = {}

-- Marker for randomly selected stuff
map.ANY = {}

-- Starts a map definition
function map.define(name)
	assert(not map.maps[name], "map "..name.." already defined!")
	map.cur_map = { name=name symbols={ [' ']={} } default_symbol={} has_map=false }
	map.maps[name] = map.cur_map
end

-- Sets player starting position
function map.set_player(y, x)
	map.cur_map.py = y
	map.cur_map.px = x
end

-- Sets old player starting position
function map.set_old_player(y, x)
	map.cur_map.oldpy = y
	map.cur_map.oldpx = x
end

-- Define global symbols attibutes
function map.set_default_symbol_attributes(t)
	map.cur_map.default_symbol = t
end

-- Define a map symbol
function map.symbol(t)
	assert(t.symbol, "map symbol without a symbol!")

	-- Add defaults
	for k, e in map.cur_map.default_symbol do
		if not t[k] then t[k] = e end
	end

	if not t.info then t.info = 0 end
	if not t.mimic then t.mimic = 0 end
	map.cur_map.symbols[t.symbol] = t
end

function map.__test_quest_cond(q, cond)
	if type(cond) == "function" then
		return cond()
	else
		return quest(q).status == cond
	end
end

-- Define a map quest symbol
function map.quest_symbol(t)
	assert(t.quest, "mat quest symbol without quest!")
	assert(t.symbol, "map quest symbol without a symbol!")
	assert(t.symbols, "map quest symbol without symbols!")
	assert(t.symbols.default, "map quest symbol without default!")

	local default = t.symbols.default
	t.symbols.default = nil
	for conds, symb in t.symbols do
		-- Set the symbol in each
		symb.symbol = t.symbol

		-- A single cond
		if type(conds) ~= "table" then
			if map.__test_quest_cond(t.quest, conds) then map.symbol(symb) return end
		else
			for _, cond in conds do
				if map.__test_quest_cond(t.quest, cond) then map.symbol(symb) return end
			end
		end
	end
	-- None ? ok use the default
	default.symbol = t.symbol
	map.symbol(default)
end

-- Define the map, checks for unknown symbols, grab size
function map.map(m)
	-- Scan the map
	map.cur_map.has_map = true
	map.cur_map.wid = 0
	map.cur_map.hgt = getn(m)
	map.cur_map.map = {}
	for j = 1, getn(m) do
		local line = m[j]

		map.cur_map.map[j] = {}

		-- Find width
		map.cur_map.wid = max(map.cur_map.wid, strlen(line))

		-- Verify and grab data
		for i = 1, strlen(line) do
			local symb = strsub(line, i, i)
			assert(map.cur_map.symbols[symb], "unknown symbol '"..symb.."' in map "..map.cur_map.name.." line "..j.." char "..i)
			map.cur_map.map[j][i] = map.cur_map.symbols[symb]
		end
	end
end

function map.level_size_to_map()
	map.cur_map.auto_level_size = true
end

-- Dispose of a map when it is not usefull anymore
function map.dispose(name)
	if map.cur_map and map.cur_map.name == name then map.cur_map = nil end
	map.maps[name] = nil
end

-- Imports a map, usualy a symbols list
function map.import(name)
	tome_dofile_anywhere(TENGINE_DIR_DATA, name)
end

-- Loads a map, execute it and dispose it
function map.load(name, y, x, only_map, recenter)
	-- Loads to 0,0 by default
	if not x then x = 0 end
	if not y then y = 0 end

	tome_dofile_anywhere(TENGINE_DIR_DATA, name)

	assert(map.cur_map, "map "..name.." loaded, but no map found in running map state!")

	map.execute(map.cur_map, y, x, only_map, recenter)
	if map.cur_map.has_map then y, x = map.cur_map.hgt + y, map.cur_map.wid + x end
	map.dispose(map.cur_map.name)

	return y, x
end

-- Executes a map
function map.execute(name, y, x, only_map, recenter)
	local m

	-- Already got a map ?
	if type(name) == "table" then m = name else m = map.maps[name] end
	assert(m, "Unknown map "..tostring(name))

	if not m.map then return end

	-- Loads to 0,0 by default
	if not x then
		if recenter then
			x = x - m.wid / 2
		else
			x = 0
		end
	end
	if not y then
		if recenter then
			y = y - m.hgt / 2
		else
			y = 0
		end
	end

	-- Auto adjust level size
	if m.auto_level_size then
		dungeon_generator.set_level_size(m.hgt, m.wid)
	end

	local found_str
	do
		local fields = {"found_name", "long_name", "name"}
		local name

		for i = 1, getn(fields) do
			name = __d_info.get_level_info(current_dungeon_idx,
										   dun_level, fields[i])
			if name then
				break
			end
		end

		if not name then
			name = d_info(current_dungeon_idx).name
		end

		if has_flag(level_flags, FLAG_OBJ_FOUND) then
			found_str = "You found it " .. level_flags.flags[FLAG_OBJ_FOUND]
		else
			found_str = "You found it on special level %s"
		end

		local level = dun_level - d_info(current_dungeon_idx).mindepth + 1
		if has_flag(level_flags, FLAG_TOWER) then
			level = -level
		end

		found_str = gsub(found_str, "%%d", level)
		found_str = gsub(found_str, "%%s", name)
	end

	-- parse the map
	for j = 1, m.hgt do
		for i = 1, m.wid do
			local symb = m.map[j][i]

			local yy, xx = y + j - 1, x + i - 1
			local c_ptr = cave(yy, xx)

			-- Set the feature
			if symb.feat then
				c_ptr.info = symb.info
				c_ptr.mimic = symb.mimic
				flag_empty(c_ptr.flags)
				if tag(symb.feat) == TAG_NUMBER then
					cave_set_feat(yy, xx, symb.feat)
				else
					cave_set_feat(yy, xx, symb.feat())
				end
			end

			if symb.special or symb.special2 then
				print("WARNING: map '" .. tostring(name) .. "' uses old 'special'")
			end

			if symb.name then
				c_ptr.flags[FLAG_TERRAIN_NAME] = symb.name
			end

			if symb.on_walk then
				c_ptr.flags[FLAG_TERRAIN_TEXT] = symb.on_walk
			end

			if symb.building then
				c_ptr.flags[FLAG_CONTAINS_BUILDING] = symb.building
			end

			if symb.owner then
				c_ptr.flags[FLAG_STORE_OWNER] = symb.owner
			end

			if symb.dungeon then
				c_ptr.flags[FLAG_DUNGEON] = symb.dungeon
			end

			if symb.branch then
				c_ptr.flags[FLAG_LEVEL_CHANGER] = symb.branch
			end

			if symb.flags then
				for k, v in symb.flags do
					if tag(v) == TAG_TABLE then
						error("Map symbols can't store Lua tables in " ..
							  "a grid's flags (map '" .. tostring(name) .. "')")
					end
				end
				local flags = getter.flags(symb.flags)
				flag_add(c_ptr.flags, flags)
			end

			-- Monster
			if not only_map and symb.monster then
				if symb.monster == map.ANY then
					local level = monster_level;
					if not symb.monster_sleep then symb.monster_sleep = true end

					if not symb.monster_level then symb.monster_level = 0 end
					monster_level = quest(player.inside_quest).level + symb.monster_level
					place_monster(yy, xx, symb.monster_sleep, false)
					monster_level = level
				else
					local faction = FACTION_DUNGEON
					if symb.monster_faction then faction = symb.monster_faction end
					if not symb.monster_sleep then symb.monster_sleep = true end

					-- Place it
					set_monster_generation(symb.monster, true)

					if not symb.monster_ego then
						place_monster_aux(yy, xx, symb.monster, symb.monster_sleep, false, faction)
					else
						place_monster_one(yy, xx, symb.monster, symb.monster_ego, symb.monster_sleep, faction)
					end
					set_monster_generation(symb.monster, false)
				end
			end

			-- inventory size
			if symb.inventory_size then
				cave(yy, xx).inventory[INVEN_LIMIT_KEY] = symb.inventory_size
			end

			-- object
			if not only_map and symb.object then
				if symb.object == map.ANY then
					local level = object_level;

					if not symb.object_level then symb.object_level = 0 end
					object_level = quest(player.inside_quest).level + symb.object_level
					if (rng.number(100) < 75) then
						place_object(yy, xx, false, false, OBJ_FOUND_SPECIAL)
					elseif (rng.number(100) < 80) then
						place_object(yy, xx, true, false, OBJ_FOUND_SPECIAL)
					else
						place_object(yy, xx, true, true, OBJ_FOUND_SPECIAL)
					end
					object_level = level
				else
					local o_ptr = new_object()
					set_object_generation(symb.object, true)

					-- Create the item
					object_prep(o_ptr, symb.object)

					-- Apply magic (no messages, no artifacts)
					apply_magic(o_ptr, dun_level, false, true, false);
					o_ptr.found = OBJ_FOUND_SPECIAL

					set_object_generation(symb.object, false)

					drop_near(o_ptr, -1, yy, xx)
				end
			end

			-- artifact
			if not only_map and symb.artifact then
				local art = a_info[1 + symb.artifact]
				local obj = new_object()

				object_prep(obj, lookup_kind(art.tval, art.sval))

				obj.artifact_id = symb.artifact

				apply_magic(obj, -1, true, true, true)

				drop_near(obj, -1, yy, xx)
			end

			-- Set "found where" string for objects, so it will
			-- be more informative than just "You found it on a
			-- special level"
			for_inventory_inven(cave(yy, xx).inventory,
				function(obj)
					if not obj.flags[FLAG_OBJ_FOUND] then
						obj.flags[FLAG_OBJ_FOUND] = %symb.object_found or
							%found_str
					end
				end)

			-- trap
			if symb.trap then
				if symb.trap == map.ANY then
					local lvl = dun_level
					if symb.trap_level then lvl = symb.trap_level end
					place_trap(yy, xx, lvl, FACTION_DUNGEON)
				else
					error("map file forcing trap type unimplemented!")
				end
			end
		end
	end

	if map.cur_map.py then player.py = map.cur_map.py end
	if map.cur_map.px then player.px = map.cur_map.px end

	if map.cur_map.oldpy and player.oldpy == 0 then player.oldpy = map.cur_map.oldpy end
	if map.cur_map.oldpx and player.oldpx == 0 then player.oldpx = map.cur_map.oldpx end
end

-- Loads a map, get the size and dispose it
declare_global_constants"load_map_size"
function load_map_size(name)
	local y, x

	map.only_size = true
	tome_dofile_anywhere(TENGINE_DIR_DATA, name)
	map.only_size = false

	assert(map.cur_map, "map "..name.." loaded, but no map found in running map state!")

	y, x = map.cur_map.hgt, map.cur_map.wid

	map.dispose(map.cur_map.name)
	return y, x
end

-- Wilderness map functions
map.wild_load = {}
map.wild_size = {}
map.wild = map.wild_size

function map.wild_size.map(m)
	-- Scan the map
	map.cur_map.wid = 0
	map.cur_map.hgt = getn(m)
	for j = 1, getn(m) do
		local line = m[j]

		-- Find width
		map.cur_map.wid = max(map.cur_map.wid, strlen(line))
	end
	max_wild_x = map.cur_map.wid
	max_wild_y = map.cur_map.hgt
end

function map.wild_size.start(y, x)
end

function map.wild_size.entrance(t)
end

function map.wild_load.map(m)
	for i = 1, max_d_idx do
		local dungeon = d_info[i]

		if not dungeon.flags[FLAG_NO_SURFACE_ENTRY] then
			if dungeon.iy ~= -1 and dungeon.ix ~= -1 then
				wild_map(dungeon.iy, dungeon.ix).entrance = (i - 1) + 1000
				if dungeon.flags[FLAG_ENTRANCE_MIMIC] then
					wild_map(dungeon.iy, dungeon.ix).mimic = dungeon.flags[FLAG_ENTRANCE_MIMIC]
				end
			end

			if dungeon.oy ~= -1 and dungeon.ox ~= -1 then
				wild_map(dungeon.oy, dungeon.ox).entrance = (i - 1) + 1000
				if dungeon.flags[FLAG_ENTRANCE_MIMIC] then
					wild_map(dungeon.oy, dungeon.ox).mimic = dungeon.flags[FLAG_ENTRANCE_MIMIC]
				end
			end
		end
	end

	-- Scan the map
	map.cur_map.wid = 0
	map.cur_map.hgt = getn(m)
	for j = 1, getn(m) do
		local line = m[j]

		-- Find width
		map.cur_map.wid = max(map.cur_map.wid, strlen(line))

		-- Verify and grab data
		for i = 1, strlen(line) do
			local symb = strsub(line, i, i)
			symb = strbyte(symb)
			local idx = wildc2i[symb + 1]
			wild_map(j - 1, i - 1).feat = idx

			local wild = wf_info[idx + 1]

			if globals()['FEAT_TOWN'] and wild.feat == FEAT_TOWN then
				local coord = {y = j - 1, x = i - 1}
				map.town_coord[wild.name] =  coord
				map.town_long_name[wild.name] = wild.text
			end

			-- If this is a town/dungeon entrance, note
			-- its coordinates.  (Have to check for
			-- duplicate Morias...)
			if (wild.entrance > 0) and not wild.wild_x then
				wild.wild_x = i - 1
				wild.wild_y = j - 1
			end
		end
	end
end

function map.wild_load.start(y, x)
	if player.wilderness_x == 0 and player.wilderness_y == 0 then
		player.wilderness_y, player.wilderness_x = y, x
	end
end

function map.wild_load.entrance(t)
        wild_map(t.y, t.x).entrance = t.dungeon + 1000
	if dungeon(t.dungeon).flags[FLAG_ENTRANCE_MIMIC] then
		wild_map(t.y, t.x).mimic = dungeon(t.dungeon).flags[FLAG_ENTRANCE_MIMIC]
	end
end
