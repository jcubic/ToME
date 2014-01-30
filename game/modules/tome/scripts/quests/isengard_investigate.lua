-- Investigate the surroundnigs of isengard, report to elrond
-- encounter the ents on the way
add_quest
{
	global = "QUEST_ISENGARD_INVESTIGATE"
	name = "The fires of Isengard"
	desc = function()
		if quest(QUEST_ISENGARD_INVESTIGATE).status == QUEST_STATUS_UNTAKEN or
			quest(QUEST_ISENGARD_INVESTIGATE).status == QUEST_STATUS_FINISHED
		then
			return nil
		end

		local lines = {
			"#yThe fires of Isengard (Danger level: 30)",
			"Travel to Isengard and scout its surroundings for clue about what is happening",
			"there, but do not engage combat. Scout and return quickly, for much is at risk!"
		}

		return lines
	end -- desc
	level = 25
	data =
	{
	}
	hooks =
	{
		[hook.MOVE_PRE] = function(y, x)
			if player.wild_mode then
				if (quest(QUEST_ISENGARD_INVESTIGATE).status ~= QUEST_STATUS_FINISHED and quest(QUEST_ISENGARD_INVESTIGATE).status ~= QUEST_STATUS_FAILED_DONE) and y == dungeon(DUNGEON_ISENGARD).iy and x == dungeon(DUNGEON_ISENGARD).ix then
					message(color.GOLD, "Something tells you it would not be wise to go to Isengard just yet.")
					return true
				end
			end
		end
		[hook.WILD_SMALL_MOVE_POST] = function()
			if quest(QUEST_ISENGARD_INVESTIGATE).status == QUEST_STATUS_TAKEN and not quest_data(QUEST_ISENGARD_INVESTIGATE).ambush then
				if player.px == 44 and player.py == 43 then
					message(color.MOCCASIN, "As you sneak around you notice a bunch of orcs bearing a strange emblem on their shields.")
					message(color.MOCCASIN, "The emblem is that of a white hand, crudely painted on the shields.")
					message(color.MOCCASIN, "Oh no it seems an orc spotted you!")
					quest_data(QUEST_ISENGARD_INVESTIGATE).ambush = 1
					player.wilderness_x = player.px
					player.wilderness_y = player.py
					change_wild_mode()

					current_dungeon_idx = DUNGEON_FIRES_OF_ISENGARD
					dun_level = dungeon(DUNGEON_FIRES_OF_ISENGARD).mindepth

					-- Let the player panic for a few turns
					time.timer.QUEST_ISENGARD_INVESTIGATE.enabled = true
				elseif (player.px == 42 or player.px == 43) and player.py == 43 then
					message(color.MOCCASIN, "You see fumes to the north, coming from Isengard, you should continue your investigation further.")
				end
			end
		end
	}
}

-- The Timer to call for Fangorn tothe rescue
time.save_timer("time.timer.QUEST_ISENGARD_INVESTIGATE")
time.timer.QUEST_ISENGARD_INVESTIGATE = time.new_timer
{
	["delay"] =     5
	["enabled"] =   nil
	["callback"] = 	function()
		-- Stop now
		time.timer.QUEST_ISENGARD_INVESTIGATE.enabled = nil

		quest_data(QUEST_ISENGARD_INVESTIGATE).ambush = 2

		message(color.YELLOW_GREEN, "You hear a very loud, deep, voice shooting:")
		message(color.YELLOW_GREEN, "'Those burarum, those evileyed - blackhanded - bowlegged - flinthearted - clawfingered - foulbellied - bloodthirsty, morimaite - sincahonda, hoom, well, since you are hasty folk and their full name is as long as years of torment, those vermin of orcs.'")
		message(color.YELLOW_GREEN, "'Hold on young one, the Ents are coming!'")
		message(color.YELLOW_GREEN, "Sudently the trees behind you are starting to move, quite fast indeed, they rush the orc patrol!")

		set_monster_generation(RACE_FANGORN_QUEST_FIRES_OF_ISENGARD, true)
		set_monster_generation(RACE_HUORN_QUEST_FIRES_OF_ISENGARD, true)
		place_monster_aux(17, 2, RACE_FANGORN_QUEST_FIRES_OF_ISENGARD, false, false, FACTION_ENT)
		place_monster_aux(16, 2, RACE_HUORN_QUEST_FIRES_OF_ISENGARD, false, false, FACTION_ENT)
		set_monster_generation(RACE_HUORN_QUEST_FIRES_OF_ISENGARD, false)
		set_monster_generation(RACE_FANGORN_QUEST_FIRES_OF_ISENGARD, false)
	end
}
