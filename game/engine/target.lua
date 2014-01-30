-- Targetting subsystem

constant("target", {})

target.MONSTER = 1
target.TRAP = 2
target.OBJECT = 3
target.CAVE = 4
target.FREE_LOOK = 5
target.FREE_LOOK_FALLBACK = 6

target.PATH_GOOD_COLOR = color.LIGHT_GREEN
target.PATH_WRONG_COLOR = color.LIGHT_RED

-- For monsters that shouldn't be auto-targeted, like trappers and mimics
new_flag("NO_AUTO_TARGET")

-- Flagset of terrain to ignore on this level, in spite of FLAG_NOTICE
new_flag("TERRAIN_IGNORE")

function target.targetable_monster(flags, m_idx, y, x)
	local monst = monster(cave(y, x).m_idx)
	if not monst.ml then return false end
	if flags.only_ennemy == true and is_friend(monst) >= 0 then
		return false
	end
	if not flags.no_los and not projectable(player.py, player.px, y, x) then
		return false
	end
	if has_flag(monst, FLAG_NO_AUTO_TARGET) then
		return false
	end

	return true
end

function target.targetable_object(flags, y, x)
	if not flags.no_los and not projectable(player.py, player.px, y, x) then
		return false
	end
	local inven = cave(y, x).inventory
	local max = flag_max_key(inven)
	for i = 1, max do
		local obj = inven[i]
		if obj then
			if obj.marked > 0 and not has_flag(obj, FLAG_NO_AUTO_TARGET) then
				return true
			end
		end
	end

	-- If the grid has a mimic monster which is out of line of sight
	-- and the player isn't sensing it somehow, and the object is marked,
	-- then return true, so out-of-sight mimics will act like objects
	-- to the targeter.
	if cave(y, x).m_idx > 0 then
		local monst = monster(cave(y, x).m_idx)

		if has_flag(monst, FLAG_MIMIC) and not monst.ml and
			not projectable(player.py, player.px, y, x)
		then
			local obj =
				flag_get_obj(monst.inventory[INVEN_INVEN], 1)

			if obj then
				if obj.marked > 0 and
					not has_flag(obj, FLAG_NO_AUTO_TARGET)
				then
					return true
				end
			end
		end
	end -- if cave(y, x).m_idx > 0 then

	return false
end

function target.targetable_trap(flags, y, x)
	if not flags.no_los and not projectable(player.py, player.px, y, x) then return false end
	if flag_used(cave(y, x).activations) == 0 then return false end
	return true
end

function target.targettable_cave(flags, y, x)
	if not flags.no_los and not projectable(player.py, player.px, y, x) then
		return false
	end

	local feat = cave(y, x).feat

	-- If the grid has a mimic monster which is out of line of sight
	-- and the player isn't sensing it somehow, uses that feature instead
	-- of the real feature, so out-of-sight mimics will act like terrain
	if cave(y, x).m_idx > 0 then
		local monst = monster(cave(y, x).m_idx)

		if has_flag(monst, FLAG_MIMIC) and not monst.ml and
			not projectable(player.py, player.px, y, x)
		then
			local obj =
				flag_get_obj(monst.inventory[INVEN_INVEN], 1)

			-- If it's not holding an object then it's a feature mimic
			if not obj then
				feat = monst.flags[FLAG_MIMIC]
			end
		end
	end

	if not has_flag(f_info(feat), FLAG_NOTICE) then return false end

	if has_flag(level_flags, FLAG_TERRAIN_IGNORE) and
		level_flags.flags[FLAG_TERRAIN_IGNORE][feat] then
		return false
	end

	return true
end

target.build_list =
{
	[target.MONSTER] = function(flags)
		local coords = {}
		for y = panel_row_min, panel_row_max do
			for x = panel_col_min, panel_col_max do
				if in_bounds(y, x) and cave(y, x).m_idx > 0 and
					target.targetable_monster(flags, cave(y, x).m_idx, y, x)
				then
					local d = distance(player.py, player.px, y, x)
					tinsert(coords, { d = d y = y x = x })
				end
			end
		end
		return coords
	end
	[target.OBJECT] = function(flags)
		local coords = {}
		for y = panel_row_min, panel_row_max do
			for x = panel_col_min, panel_col_max do
				if (in_bounds(y, x) and
					target.targetable_object(flags, y, x))
				then
					local d = distance(player.py, player.px, y, x)
					tinsert(coords, { d = d y = y x = x })
				end
			end
		end
		return coords
	end
	[target.TRAP] = function(flags)
		local coords = {}
		for y = panel_row_min, panel_row_max do
			for x = panel_col_min, panel_col_max do
				if in_bounds(y, x) and (cave(y, x).info & CAVE_TRDT > 0) and
					target.targetable_trap(flags, y, x)
				then
					local d = distance(player.py, player.px, y, x)
					tinsert(coords, { d = d y = y x = x })
				end
			end
		end
		return coords
	end
	[target.CAVE] = function(flags)
		local coords = {}
		for y = panel_row_min, panel_row_max do
			for x = panel_col_min, panel_col_max do
				if in_bounds(y, x) and (cave(y, x).info & CAVE_MARK > 0) and
					target.targettable_cave(flags, y, x)
				then
					local d = distance(player.py, player.px, y, x)
					tinsert(coords, { d = d y = y x = x })
				end
			end
		end
		return coords
	end
}

function target.get_path(y2, x2)
	local size = panel_row_max - panel_row_min
	local y, x
	local path = {}

	if size < panel_col_max - panel_col_min then size = panel_col_max - panel_col_min end

	-- Start at the initial location
	y = player.py
	x = player.px

	for dist = 0, size do
		tinsert(path, {y, x})
		if ((x == x2) and (y == y2)) then break end

		-- Calculate the new location
		y, x = mmove2(y, x, player.py, player.px, y2, x2)
	end
	return path
end

function target.erase_old_path()
	if target.__old_path then
		local path = target.__old_path
		for i = 1, getn(path) do
			lite_spot(path[i][1], path[i][2])
		end
		target.__old_path = nil
	end
end

target.describe_stuff =
{
	[target.MONSTER] = function(flags, mode, descs, m_idx, monst)
		tinsert(descs, strcap(monster_desc(monst, 0)))
	end
	[target.OBJECT] = function(flags, mode, descs, c_ptr, y, x)
		for_inventory_inven(c_ptr.inventory, function(obj)
			if obj.marked > 0 then
				tinsert(%descs, strcap(object_desc(obj, 0)))
			end
		end)
	end
	[target.TRAP] = function(flags, mode, descs, c_ptr, y, x)
		local desc
		if get_num_location_traps(y, x, true, -1, false, false) > 1 then desc = "multiple traps"
		else
			local trap = traps.get_trap(-1, -1, y, x)
			local idx = flag_get(trap, FLAG_TRAP_IDX)
			local level = flag_get(trap, FLAG_LEVEL)

			if traps.is_ident(idx) then
				desc = traps.ident_name(idx, trap)
				if wizard then desc = desc.."(level "..level..")" end
			else
				desc = "an unknown trap";
			end
		end
		tinsert(descs, "Trap: "..desc)
	end
	[target.CAVE] = function(flags, mode, descs, c_ptr, y, x)
		if (c_ptr.info & CAVE_MARK > 0) or player_can_see_bold(y, x) then
			local feat = c_ptr.feat
			if c_ptr.mimic > 0 then feat= c_ptr.mimic end
			tinsert(descs, strcap(f_info[1 + feat].name))
		else
			tinsert(descs, "Unknown grid")
		end
	end
}

-- Describe the target
function target.describe(flags, mode, y, x)
	local c_ptr = cave(y, x)
	local descs = {}

	if (mode[target.MONSTER] or mode[target.FREE_LOOK]) and
	   c_ptr.m_idx > 0 and monster(c_ptr.m_idx).ml then
		target.describe_stuff[target.MONSTER](flags, mode, descs, c_ptr.m_idx, monster(c_ptr.m_idx))
	end

	if (mode[target.OBJECT] or mode[target.FREE_LOOK]) and
		(not is_inven_empty(c_ptr.inventory) or is_obj_mimic_at(y, x))
	then
		target.describe_stuff[target.OBJECT](flags, mode, descs, c_ptr, y, x)
	end

	if (mode[target.TRAP] or mode[target.FREE_LOOK]) and
	   (c_ptr.info & CAVE_TRDT > 0) and flag_used(c_ptr.activations) > 0 then
		target.describe_stuff[target.TRAP](flags, mode, descs, c_ptr, y, x)
	end

	if (mode[target.CAVE] or mode[target.FREE_LOOK]) then
		target.describe_stuff[target.CAVE](flags, mode, descs, c_ptr, y, x)
	end

	return descs
end

-- Sets the target to something
-- Displays a nice "line of sight" line
function target.set_target(flags, coord, desc)
	target.erase_old_path()

	term.prt(desc, 0, 0)

	-- Set the target
	if cave(coord.y, coord.x).m_idx > 0 then
		local idx = cave(coord.y, coord.x).m_idx
		health_track(idx)
		target_who = idx
		local monst = monster(idx)
		monster_race_track(monst.r_idx, monst.ego)
		handle_stuff()
	else
		target_who = 0
	end
	target_row = coord.y
	target_col = coord.x

	-- Display the path
	local path = target.get_path(coord.y, coord.x)
	target.__old_path = path
	if flags.save_path and type(flags.save_path) == "table" then flags.save_path.path = path end

	if getn(path) == 1 then
		term.move_cursor_relative(path[1][1], path[1][2])
		term.fresh()
	else
		local c = target.PATH_GOOD_COLOR
		project_range = flags.max_range or -1
		for i = 2, getn(path) do
			local s = strbyte('*')
			if i == getn(path) then
				term.move_cursor_relative(path[i][1], path[i][2])
				term.fresh()
			elseif not flags.no_path then
				if c == target.PATH_GOOD_COLOR then
					if not projectable(player.py, player.px, path[i][1], path[i][2]) then c = target.PATH_WRONG_COLOR
					elseif flags.path_color_monster_block and cave(path[i][1], path[i][2]).m_idx > 0 then c = target.PATH_WRONG_COLOR end
				end
				print_rel(s, c, path[i][1], path[i][2])
			end
		end
		project_range = -1
	end

	-- Make sure that the cursor ends up at coord
	term.move_cursor_relative(coord.y, coord.x)
	term.fresh()
end

-- Directed selection
function target.pick(coords, y1, x1, dy, dx)
	local v
	local x2, y2, x3, y3, x4, y4
	local b_i, b_v = -1, 999999

	-- Scan the locations
	for i = 1, getn(coords) do
		-- Point 2
		x2 = coords[i].x
		y2 = coords[i].y

		-- Directed distance
		x3 = (x2 - x1)
		y3 = (y2 - y1)

		-- Verify quadrant
		if not ((dx ~= 0) and (x3 * dx <= 0)) and
			not((dy ~= 0) and (y3 * dy <= 0)) then
			-- Absolute distance
			x4 = abs(x3)
			y4 = abs(y3)

			-- Verify quadrant
			if not (dy ~= 0 and dx == 0 and (x4 > y4)) and
				not (dx ~= 0 and dy == 0 and (y4 > x4)) then
				v = (x3 * x3) + (y3 * y3)

				-- Track best
				if (v < b_v) then
					-- Track best
					b_i = i
					b_v = v
				end
			end
		end
	end

	-- Result
	return (b_i)
end

-- Returns the coords of this dir
function target.get_coords(dir)
	if dir == 5 then
		return target_row, target_col
	end
end

function target.grab_targets(coords, flags, mode)
	for mod, _ in mode do
		if target.build_list[mod] then merge_tables(coords, target.build_list[mod](flags)) end
	end
	sort(coords, function(a,b) return a.d < b.d end)
end

-- Target something
function target.target(key_handler, flags, mode)
	local ret = true
	local old_target_who, old_target_row, old_target_col = target_who, target_row, target_col
	local coords = {}
	flags = flags or {}
	local screen = term.save_to()
	local _, screen_wid, screen_hgt = term.get_size()

	if target_who > 0 then
		local target_mon = monster(target_who)

		if not target_mon or target_mon.r_idx == 0 then
			target_who     = -1
			old_target_who = -1
			target_row     = -1
			target_col     = -1
		end
	end

	if not mode[target.FREE_LOOK] then
		target.grab_targets(coords, flags, mode)
	else
		tinsert(coords, {d = 0, y = player.py, x = player.px})
	end

	if getn(coords) == 0 then
		if (not flags.direction_on_no_target) and
		   (not mode[target.FREE_LOOK_FALLBACK]) then
			return
		else
			tinsert(coords, {d = 0, y = player.py, x = player.px})
			mode[target.FREE_LOOK] = true
			mode[target.CAVE] = true
		end
	end

	-- Locate the last target if possible
	local cur = -1
	if not mode[target.FREE_LOOK] then
		for i = 1, getn(coords) do
			local m_idx = cave(coords[i].y, coords[i].x).m_idx
			if m_idx > 0 and m_idx == target_who and monster(m_idx).ml then
				cur = i

				-- Monster might have moved, change target and row to
				-- match new location
				local monst = monster(m_idx)
				target_row = monst.fy
				target_col = monst.fx
				break
			end
		end

		if cur == -1 then
			for i = 1, getn(coords) do
				if target_row == coords[i].y and
					target_col == coords[i].x
				then
					cur = i
					break
				end
			end
		end

		-- Is old target off-panel or out of line-of-sight?
		if target_row < panel_row_min or target_row > panel_row_max or
			target_col < panel_col_min or target_col > panel_col_max or
			not los(player.py, player.px, target_row, target_col)
		then
			cur = -1
		end
	else
		if target_row >= panel_row_min and target_row <= panel_row_max and target_col >= panel_col_min and target_col <= panel_col_max and los(player.py, player.px, target_row, target_col) then
			coords[1] = {d = 0, y = target_row, x = target_col}
		end
	end
	if cur == -1 then
		cur = 1
	end

	local orig_row_min, orig_row_max = panel_row_min, panel_row_max
	local orig_col_min, orig_col_max = panel_col_min, panel_col_max

	local descs, cur_desc = nil, 1
	local no_load, no_cycle = false, false
	local prev
	repeat
		if not no_load and (orig_row_min == panel_row_min and
							orig_row_max == panel_row_max and
							orig_col_min == panel_col_min and
							orig_col_max == panel_col_max)
		then
			term.load_from(screen, false)
		end

		no_load = false
		no_cycle = false

		prev = cur

		-- What are we looking at ?
		if cur < 1 then cur = 1 end
		descs = target.describe(flags, mode, coords[cur].y, coords[cur].x)

		-- Mode wants to do something? (like display help)
		if target.extra_display then target.extra_display(flags, mode, coords, cur, cur_desc) end

		if getn(descs) > 0 then
			if cur_desc > getn(descs) then cur_desc = 1 end

			-- Sets the target
			target.set_target(flags, coords[cur], descs[cur_desc])
		else
			-- Sets the target
			target.set_target(flags, coords[cur], 'BUG!')
		end

		-- Lets see what the user do
		-- By default space changes target and anything else is handled
		-- By a key handler, if not then anything else accepts the target
		local raw = term.inkey()
		local key = strchar(raw)

		if game_options.rogue_like_commands then
			local table = {
				b = '1',
				j = '2',
				n = '3',
				h = '4',
				l = '6',
				y = '7',
				k = '8',
				u = '9'
			}

			key = table[key] or key

			if key == '.' then
				key = '5'
			end
		else
			if key == 'g' then
				key = '5'
			end
		end

		local is_dir = false
		if strbyte(key) >= strbyte('0') and
			strbyte(key) <= strbyte('9') then
			is_dir = true
		end

		-- Quit and do not register the target
		if raw == ESCAPE then
			ret = false
			break
		-- Next target
		elseif key == ' ' or key == '*' then
		-- Recenter on player
		elseif key == 'p' then
			coords = {{d = 0, y = player.py, x = player.px}}
			verify_panel()
		-- When in cave mode just process every spaces
		elseif key == 'o' then
			coords = {{d = 0, y = target_row, x = target_col}}
			mode[target.FREE_LOOK] = true
			no_cycle = true
			cur = 1
			cur_desc = 1
		elseif key == 'r' and cave(coords[cur].y, coords[cur].x).m_idx > 0 then
			local monst = monster(cave(coords[cur].y, coords[cur].x).m_idx)
			term.prt("", 1, 0)
			memory.main(monst.r_idx, monst.ego)
			no_load = true
			no_cycle = true
			-- Move around when full look mode
		elseif mode[target.FREE_LOOK] and is_dir then
			local dy, dx = explode_dir(tonumber(key))
			local y, x = coords[cur].y + dy, coords[cur].x + dx

			-- Don't change panel unnecessarily
			if (x < panel_col_min and dx > 0) or
				(x > panel_col_max and dx < 0)
			then
				x = coords[cur].x
			end
			if (y < panel_row_min and dy > 0) or
				(y > panel_row_max and dy < 0)
			then
				y = coords[cur].y
			end				

			-- Stay in bounds
			if x < 1 then
				x  = 1
				dx = 0
			elseif x >= (cur_wid - 1) then
				x  = cur_wid - 2
				dx = 0
			end
			if y < 1 then
				y  = 1
				dy = 0
			elseif y >= (cur_hgt - 1) then
				y  = cur_hgt - 2
				dy = 0
			end

			coords[cur] = {d = 0, y = y, x = x}

			if (y > panel_row_max) or
				(y < panel_row_min) or
				(x > panel_col_max) or
				(x < panel_col_min)
			then
				change_panel(dy, dx) 
			end
		-- Pick a next target based on the general direction used
		elseif not flags.no_dir_pick and is_dir then
			local dy, dx = explode_dir(tonumber(key))
			local i = target.pick(coords, coords[cur].y, coords[cur].x, dy, dx)
			if i >= 1 then
				cur = i - 1
			else
				term.load_from(screen, true)
				if change_panel(dy, dx) then
					local new_coords = {}
					target.grab_targets(new_coords, flags, mode)

					i = target.pick(new_coords, coords[cur].y, coords[cur].x, dy, dx)
					if i >= 1 then
						cur = i - 1
						cur_desc = 1
						no_cycle = true
						coords = new_coords
					else
						cur = prev - 1
						change_panel(-dy, -dx)
					end
				else
					cur = prev - 1
				end
				screen = term.save_to()
			end
		else
			-- Let handlers .. handle
			if key_handler then
				local done, r = key_handler(key, raw, flags, mode)
				if done then ret = r break end
			else
				break
			end
		end
		if not no_cycle then
			if cur_desc == getn(descs) or key == '*' or is_dir then
				cur_desc = 1
				cur = cur + 1
				if cur > getn(coords) then cur = 1 end
			else
				cur_desc = cur_desc + 1
			end
		end
	until false

	if not ret then
		target_who, target_row, target_col = old_target_who, old_target_row, old_target_col
	end

	local msg = nil

	if target_who > 0 then
		local monst = monster(target_who)

		if has_flag(monst, FLAG_NO_TARGET) then
			msg = "You can not target " .. monster_desc(monst, 0) .. "."

			target_who = -1
			ret = false
		else
			health_track(target_who)
			monster_race_track(monst.r_idx, monst.ego)
		end
	end

	target.erase_old_path()
	term.prt("", 0, 0)
	term.load_from(screen, true)
	verify_panel()

	if msg then
		message(msg)
	end

	return ret
end

-- Make get_aim_dir our own
get_aim_dir = function(flags)
	local p = { dir = 5 }

	flags = flags or {}
	flags.direction_on_no_target = true
	flags.only_ennemy = flags.only_ennemy or true

	local ret, rep = repeat_keys.pull()
	if ret and rep == 5 then
		if target_who > 0 then
			if target.targetable_monster(flags, target_who, monster(target_who).fy, monster(target_who).fx) then
				target_row = monster(target_who).fy
				target_col = monster(target_who).fx
				return true, 5
			else
				target_who = 0
			end
		end
	end

	ret = target.target(nil, flags, {[target.MONSTER]=true})

	repeat_keys.push(p.dir)

	return ret, p.dir
end

hook(hook.KEYPRESS, function(key)
	if key == strbyte('*') then
		if target.target
		(
			function(key, raw)
				if key == 't' then
					return true, true
				end
			end,
			{ target_key = 't' no_los=true },
			{[target.MONSTER]=true [target.FREE_LOOK_FALLBACK]=true}
		) then
			message("Target set.")
		end
		return true
	end
	if key == strbyte('l') then
		target.target(function() end, { no_los=true no_path=true }, {[target.MONSTER]=true [target.OBJECT]=true [target.TRAP]=true [target.CAVE]=true [target.FREE_LOOK_FALLBACK]=true})
		return true
	end
end)

-- Clear out old targeting info when current target dies
hook(hook.MONSTER_DEATH,
function(m_idx, who)
	if getn(old_target_who) > 0 then
		if old_target_who[1] == m_idx then
			old_target_who[1] = -1
			old_target_col[1] = -1
			old_target_row[1] = -1
		end
	elseif target_who == m_idx then
		target_who = -1
		target_col = -1
		target_row = -1
	end
end)
