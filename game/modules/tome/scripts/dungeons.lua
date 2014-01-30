constant("dungeons", {})

new_flag("NUM_FOUNTAINS")
new_flag("NUM_JUMPGATES")

function dungeons.get_num_feats(flag, mean, dev)
	if has_flag(level_flags, flag) then
		mean = flag_get(level_flags.flags, flag)
		if mean == 0 then
			return 0
		end

		local tmp = flag_get2(level_flags.flags, flag)

		if tmp ~= 0 then
			dev = tmp
		end
	end

	return rng.normal(mean, dev)
end

function dungeons.to_even_coords(...)
	for i = 1, getn(arg) do
		arg[i] = arg[i] - imod(arg[i], 2)
	end

	return unpack(arg)
end

hook(hook.LEVEL_END_GEN,
function()
	if dun_level == 0 or is_quest(dun_level) ~= 0 or
		has_flag(level_flags, FLAG_SPECIAL)
	then
		return
	end

	local disp   = { {0, 0}, {0, 1}, {1, 0}, {1, 1}}
	local double = has_flag(level_flags, FLAG_DOUBLE)

	-- Place jumpgates
	local num = dungeons.get_num_feats(FLAG_NUM_JUMPGATES, 2, 3)
	for i = 1, num do
		local y1, x1 = find_naked_bold(ALLOC_SET_ROOM)
		local y2, x2 = find_naked_bold(ALLOC_SET_ROOM)

		if not y1 or not y2 then
			wiz_print("Couldn't place random jumpgate")
			return
		end

		-- Need two different points for jumpgate pair
		local tries = 0
		while y1 == y2 and x1 == x2 and tries < 2 do
			y2, x2 = find_naked_bold(ALLOC_SET_ROOM)
			if not y2 then
				wiz_print("Couldn't place random jumpgate")
				return
			end

			tries = tries + 1
		end

		-- Looks like there's no room left
		if y1 == y2 and x1 == x2 then
			wiz_print("Out of room for random jumpgates")
			break
		end

		if double then
			y1, x1, y2, x2 = dungeons.to_even_coords(y1, x1, y2, x2)

			for j = 1, 4 do
				local tmp = disp[j]
				features.make_between_pair(y1 + tmp[1], x1 + tmp[2],
										   y2 + tmp[1], x2 + tmp[2])
			end
		else
			features.make_between_pair(y1, x1, y2, x2)
		end
	end

	-- Place fountains
	num = dungeons.get_num_feats(FLAG_NUM_FOUNTAINS, 1, 3)
	for i = 1, num do
		local y, x = find_naked_bold(ALLOC_SET_ROOM)

		if not y then
			wiz_print("Couldn't place random fountain")
			return
		end

		if double then
			y, x = dungeons.to_even_coords(y, x)
		end

		if rng.percent(30) then
			cave_set_feat(y, x, FEAT_EMPTY_FOUNTAIN)
		else
			cave_set_feat(y, x, FEAT_FOUNTAIN)
		end

		if double then
			local potions = cave(y, x).flags[FLAG_FOUNTAIN]
			local feat    = iif(potions, FEAT_FOUNTAIN, FEAT_EMPTY_FOUNTAIN)

			for j = 2, 4 do
				local tmp   = disp[j]
				local c_ptr = cave(y + tmp[1], x + tmp[2])

				c_ptr.feat = feat

				if potions then
					local new_obj = new_object()

					object_copy(new_obj, potions)
					new_obj.number = potions.number
					c_ptr.flags[FLAG_FOUNTAIN] = new_obj
				end
			end -- for j = 2, 4 do
		end -- if double then
	end -- for i = 1, num do
end) -- hook(hook.LEVEL_END_GEN)

----------------------------------------------------------------------------
---------------- Take care of decaying persistent levels -------------------
----------------------------------------------------------------------------

function dungeons.maintain_decaying_level(loaded_turn, decayed_monsters, decayed_objects, decayed_effects)
	-- Realloc some monsters
	if decayed_monsters > 0 and not level_flags.flags[FLAG_NO_NEW_MONSTER] then
		local alloc_m = decayed_monsters + rng(-decayed_monsters / 2, decayed_monsters / 2)
		for i = 1, alloc_m do alloc_monster(0, true) end
	end
end


-- Using the generator just means we don't get rooms, and puts the edges in.
-- (would be better not to have edges like this, perhaps? -- TheFalcon)
level_generator
{
	name	= "gen_land_of_rhun"
	gen	= function()
		for j = 0, cur_hgt-1 do
			cave(j,0).mimic = FEAT_PERM_SOLID
			cave(j,cur_wid-1).mimic = FEAT_PERM_SOLID
		end
		for i = 1, cur_wid - 2 do
			cave(0,i).mimic = FEAT_PERM_SOLID
			cave(cur_hgt-1,i).mimic = FEAT_PERM_SOLID
		end
		for i = 1, cur_wid - 2 do
			for j = 1, cur_hgt - 2 do
				-- border of dirt / tall grass = grass
				if cave(j, i).feat == FEAT_DIRT and feat_adjacent(j, i, FEAT_TALL_GRASS) then
					cave(j, i).feat = FEAT_GRASS
				end
			end
		end
		-- need to scan again due to change.
		for i = 1, cur_wid - 2 do
			for j = 1, cur_hgt - 2 do
				-- border of dirt / grass = short grass
				if cave(j, i).feat == FEAT_DIRT and feat_adjacent(j, i, FEAT_GRASS) then
					cave(j, i).feat = FEAT_SHORT_GRASS
				end
			end
		end
		-- another scan to be even smoother. Can be merged if too slow.
		for i = 1, cur_wid - 2 do
			for j = 1, cur_hgt - 2 do
				-- border of grass / tall grass = long grass
				if cave(j, i).feat == FEAT_GRASS and feat_adjacent(j, i, FEAT_TALL_GRASS) and not feat_adjacent(j, i, FEAT_DIRT) then
--					cave(j, i).feat = FEAT_LONG_GRASS
					cave_set_feat(j, i, FEAT_LONG_GRASS)
				end
				-- scatter some flowers in the remaining dirt
				if cave(j, i).feat == FEAT_DIRT then
					if rng(10) == 3 then
						cave(j, i).feat = FEAT_ELANOR
					end
				end
			end
		end
		return new_player_spot(get_branch())
	end
}

---------------------

dungeons.detect_stairs = function(radius)
	-- XXX TODO
	message("dungeons.detect_stairs unimplemented!")
	return true
end

dungeons.detect_doors = function(radius)
	-- XXX TODO
	message("dungeons.detect_doors unimplemented!")
	return true
end

dungeons.destroy_doors = function(radius)
	-- XXX TODO
	message("dungeons.destroy_doors unimplemented!")
	return true
end

































-- DarkGod's test generator ... not taht good yet

constant("gen_forest", {})

function gen_forest.connect(y0, x0, y1, x1)
	local reverse = false
	if abs(y1 - y0) > abs(x1 - x0) then
		reverse = true
		y0, x0, y1, x1 = x0, y0, x1, y1
	end

	local cur = y0
	local old_cur = cur
	for j = x0, x1 do
		if rng(0, abs(y1 - cur)) == 0 then
			cur = cur + rng(-1, 1)
		else
			local dir = (y1 - cur) / abs(y1 - cur)
			cur = cur + dir
		end
		if not reverse then
			if not in_bounds(cur, j) then cur = old_cur end
			cave_set_feat(cur, j - 1, FEAT_GRASS)
			cave_set_feat(cur, j, FEAT_GRASS)
		else
			if not in_bounds(j, cur) then cur = old_cur end
			cave_set_feat(j - 1, cur, FEAT_GRASS)
			cave_set_feat(j, cur, FEAT_GRASS)
		end
		old_cur = cur
	end
	return cur
end

level_generator
{
	name	= "gen_forest"
	stairs = FALSE
	monsters = FALSE
	objects = FALSE
	miscs = FALSE

	gen	= function()
		for j = 0, cur_hgt-1 do
			cave_set_feat(j, 0, FEAT_PERM_OUTER)
			cave_set_feat(j, cur_wid - 1, FEAT_PERM_OUTER)
			cave(j,0).mimic = FEAT_TREES
			cave(j,cur_wid-1).mimic = FEAT_TREES
		end
		for i = 0, cur_wid - 1 do
			cave_set_feat(0, i, FEAT_PERM_OUTER)
			cave_set_feat(cur_hgt - 1, i, FEAT_PERM_OUTER)
			cave(0,i).mimic = FEAT_TREES
			cave(cur_hgt-1,i).mimic = FEAT_TREES
		end

		local waypoints = { {} {} {} {} {} {} }
		local nb_path = cur_hgt / 10
		local nb = getn(waypoints)
		for i = 1, nb_path do
			tinsert(waypoints[1], { y=rng(2, cur_hgt - 3) x=1 })
			local y, x = waypoints[1][i].y, waypoints[1][i].x
			for w = 2, nb - 1 do
				y=rng(-cur_hgt / 4, cur_hgt / 4) + y
				x=rng(w * (cur_wid / nb), (w + 1) * (cur_wid / nb))
				if y < 1 then y = 1 end
				if y > cur_wid - 2 then y = cur_wid - 2 end
				tinsert(waypoints[w], { y=y x=x })
			end
			tinsert(waypoints[nb], { y=rng(2, cur_hgt - 3) x=cur_wid - 2 })
		end

--		for i =

		for i = 1, nb_path do
			cave_set_feat(waypoints[1][i].y, waypoints[1][i].x, FEAT_WAY_LESS)
			for w = 2, getn(waypoints) do
				waypoints[w][i].y = gen_forest.connect(waypoints[w - 1][i].y, waypoints[w - 1][i].x, waypoints[w][i].y, waypoints[w][i].x)
				cave_set_feat(waypoints[w][i].y, waypoints[w][i].x, FEAT_TREE_MALLORN)
			end
			cave_set_feat(waypoints[getn(waypoints)][i].y, waypoints[getn(waypoints)][i].x, FEAT_WAY_MORE)
		end

		recursive_river(40, 1, 30, 20, FEAT_GRASS, FEAT_TALL_GRASS, 1)

		player.py=10
		player.px=10

		return true
	end
}

