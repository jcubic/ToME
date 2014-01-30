-- Find the shards of narsil
add_quest
{
	global = "QUEST_NARSIL"
	name = "The shards of Narsil"
	desc = function()
		if quest(QUEST_NARSIL).status == QUEST_STATUS_UNTAKEN or
			quest(QUEST_NARSIL).status == QUEST_STATUS_FINISHED
		then
			return nil
		end

		local lines = {
			"#yThe shards of Narsil (Danger level: 25)",
			"You are an agent of Elrond of Rivendell, an elf-friend.",
			"He, through Gandalf the grey, asked you to look around " ..
				"the world for the",
			"shards of Narsil so that the broken sword might be reforged."
		}

		if quest_data(QUEST_NARSIL).hilt_found and quest_data(QUEST_NARSIL).blade_found then
			tinsert(lines,
					"You found the shards, bring them back to Elrond " ..
						"in Imladris with all haste!")
		else
			if quest_data(QUEST_NARSIL).clue_given == 2 then
				local y, x = dungeon(DUNGEON_ANDUIN_SWAMP).iy, dungeon(DUNGEON_ANDUIN_SWAMP).ix
				local y2, x2 = map.town_coord.Bree.y, map.town_coord.Bree.x
				local y3, x3 = map.town_coord.Edoras.y, map.town_coord.Edoras.x
				local dir2, dist2 = compass(y2, x2, y, x), approximate_distance(y2, x2, y, x)
				local dir3, dist3 = compass(y3, x3, y, x), approximate_distance(y3, x3, y, x)

				tinsert(lines, "The swamp is "..dist2.." to the "..dir2..
						" of Bree and")
				tinsert(lines,	dist3.." to the "..dir3.." of Edoras.")
			else
				tinsert(lines, "You have no clue as to where to start...")
			end
		end

		return lines
	end -- desc
	level = 25
	data =
	{
	}
	hooks =
	{
		[hook.GAME_START] = function(new)
			if new == true and not no_new_quests() then
				acquire_quest(QUEST_NARSIL, true)
			end
		end -- [hook.GAME_START]

		-- Activate a timer after level 20, if not in the Halls of Mandos
		[hook.PLAYER_LEVEL] = function()
			if player.lev >= 16 and
				quest(QUEST_NARSIL).status ~= QUEST_STATUS_UNTAKEN and
				not quest_data(QUEST_NARSIL).clue_given
			then
				quest_data(QUEST_NARSIL).clue_given = 1
				time.timer.QUEST_NARSIL.enabled = true
				time.timer.QUEST_NARSIL.countdown = rng(200, 500)
			end
		end -- [hook.PLAYER_LEVEL]

		-- Activate time after escaping the Halls of Mandos.
		[hook.LEVEL_END_GEN] = function()
			if no_new_quests() then
				return
			end
			if quest(QUEST_NARSIL).status == QUEST_STATUS_UNTAKEN then
				acquire_quest(QUEST_NARSIL, true)
			end
			if player.lev >= 16 and
				not quest_data(QUEST_NARSIL).clue_given
			then
				quest_data(QUEST_NARSIL).clue_given = 1
				time.timer.QUEST_NARSIL.enabled = true
				time.timer.QUEST_NARSIL.countdown = rng(200, 500)
			end
		end -- [hook.LEVEL_END_GEN]

		[hook.GET_POST] = function(obj, slot)
			local ok = false
			if obj.artifact_id == ART_NARSIL_HILT then
				message("This shard seems different, even special.")
				quest_data(QUEST_NARSIL).hilt_found = true
				ok = true
			end
			if obj.artifact_id == ART_NARSIL_BLADE then
				message("This shard seems different, even special.")
				quest_data(QUEST_NARSIL).blade_found = true
				ok = true
			end
			if ok and quest_data(QUEST_NARSIL).blade_found and quest_data(QUEST_NARSIL).hilt_found then
				sound.ambient.change_music(false, "shards-found", true)
				message(color.YELLOW, "You have found the two missing shards of Narsil!")
				message(color.YELLOW, "Bring them back to Elrond in Imladris with all haste!")
				change_quest_status(QUEST_NARSIL, QUEST_STATUS_COMPLETED)
			end
		end -- [hook.GET_POST]

		-- 43 42
		[hook.WILD_SMALL_MOVE_POST] = function()
			-- Gives a hint for the next quest
			if not quest_data(QUEST_NARSIL).isengard_seen and player.px == 42 and (player.py == 43 or player.py == 44 or player.py == 45) then
				message(color.MOCCASIN, "As you pass by the Gap of Rohan, you notice strange smoke coming from the north.")
				message(color.MOCCASIN, "The wind carries war shouts, something evil is going on in Isengard!")
				message(color.MOCCASIN, "Elrond must be told about it when you come back to Rivendell.")
				quest_data(QUEST_NARSIL).isengard_seen = true
			end
		end -- [hook.WILD_SMALL_MOVE_POST]
	}
}

-- The Timer to give clue
time.save_timer("time.timer.QUEST_NARSIL")
time.timer.QUEST_NARSIL = time.new_timer
{
	["delay"] =     1
	["enabled"] =   nil
	["callback"] = 	function()
		-- Stop now
		time.timer.QUEST_NARSIL.enabled = nil

		quest_data(QUEST_NARSIL).clue_given = 2
		message(color.YELLOW, "A bird flies around your head, somehow you seem to understand it:")
		message(color.YELLOW, "'Gandalf sent me, he found a possible location for the shards of Narsil.")
		message(color.YELLOW, "Look for them in a swamp near the great Anduin river.")

		local y, x = dungeon(DUNGEON_ANDUIN_SWAMP).iy, dungeon(DUNGEON_ANDUIN_SWAMP).ix
		local y2, x2 = map.town_coord.Bree.y, map.town_coord.Bree.x
		local y3, x3 = map.town_coord.Edoras.y, map.town_coord.Edoras.x
		local dir2, dist2 = compass(y2, x2, y, x), approximate_distance(y2, x2, y, x)
		local dir3, dist3 = compass(y3, x3, y, x), approximate_distance(y3, x3, y, x)
		message(color.YELLOW, "The swamp is "..dist2.." to the "..dir2.." of Bree and")
		message(color.YELLOW, dist3.." to the "..dir3.." of Edoras.'")
		term.disturb(1, 0)
	end
}

-- Level generator for the swamps of the Anduin river
-- Small pools of water, dirt, some trees and rubbles here and there
level_generator
{
	name= "gen_swamp_anduin"
	gen	= function()
		local day = time.is_daylight(turn)

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
		-- Some trees
		for i = 1, cur_wid - 2 do
			for j = 1, cur_hgt - 2 do
				-- border of dirt / grass = short grass
				if cave(j, i).feat == FEAT_SHORT_GRASS and feat_adjacent(j, i, FEAT_GRASS) then
					cave(j, i).feat = FEAT_TREES
				end
			end
		end

		local rubbles = {}

		-- another scan to be even smoother. Can be merged if too slow.
		for i = 1, cur_wid - 2 do
			for j = 1, cur_hgt - 2 do
				-- border of grass / tall grass = long grass
				if cave(j, i).feat == FEAT_GRASS and feat_adjacent(j, i, FEAT_TALL_GRASS) and not feat_adjacent(j, i, FEAT_DIRT) then
					if rng.percent(5) then
						cave_set_feat(j, i, FEAT_SMALL_RUBBLE)
						tinsert(rubbles, {y = j x = i})
					else
						cave_set_feat(j, i, FEAT_LONG_GRASS)
					end
				end

				-- Light up in the daylight
				if day then cave(j, i).info = cave(j, i).info | CAVE_GLOW end
			end
		end

		-- Place the shards
		local hilt = -1
		if not quest_data(QUEST_NARSIL).hilt_found then
			hilt = rng(getn(rubbles))
			wiz_print("hilt "..rubbles[hilt].y.." "..rubbles[hilt].x)
			local hilt_obj = create_artifact(ART_NARSIL_HILT)
			local drops = flag_new()
			drops[1] = hilt_obj
			cave(rubbles[hilt].y, rubbles[hilt].x).flags[FLAG_DROP_OBJECTS] = drops
		end

		if not quest_data(QUEST_NARSIL).blade_found then
			local blade = rng(getn(rubbles))
			while blade == hilt do blade = rng(getn(rubbles)) end
			wiz_print("blade "..rubbles[blade].y.." "..rubbles[blade].x)
			local blade_obj = create_artifact(ART_NARSIL_BLADE)
			local drops = flag_new()
			drops[1] = blade_obj
			cave(rubbles[blade].y, rubbles[blade].x).flags[FLAG_DROP_OBJECTS] = drops
		end

		return new_player_spot(get_branch())
	end
	-- default for monsters, objects, no rubble or traps, no stairs.
	miscs = FALSE
	stairs = FALSE
}
