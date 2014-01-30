-- Quests in Khazad-dum

add_quest
{
	global = "QUEST_WATCHER"
	name = "The watcher in the water"
	desc = function()
		if quest(QUEST_WATCHER).status == QUEST_STATUS_UNTAKEN or
			quest(QUEST_WATCHER).status == QUEST_STATUS_FINISHED
		then
			return nil
		end

		local lines = {
			"#yThe watcher in the water (Danger level: 30)",
			"The west gate of Khazad-dum is guarded by a horrible, tentacled monster.",
			"Destroy this foul creature and return to Balin with news of your success."
		}

		return lines
	end -- desc
	level = 30
	data =
	{
	}
	hooks =
	{
		-- Handled in Watcher definition
	}
}

add_quest
{
	global = "QUEST_SIEGE_OF_KHAZAD_DUM"
	name = "Siege of Khazad-dum"
	desc = function()
		if quest(QUEST_SIEGE_OF_KHAZAD_DUM).status == QUEST_STATUS_UNTAKEN or
			quest(QUEST_SIEGE_OF_KHAZAD_DUM).status == QUEST_STATUS_FINISHED
		then
			return nil
		end

		local lines = {
			"#Siege of Khazad-dum (Danger level: 30)",
			"The east gate of Khazad-dum has been sieged by a large group of orcs and trolls!",
			"Break the siege by completely destroying these creatures and return to Balin with",
			"news of your success.",
		}

		return lines
	end -- desc
	level = 30
	data =
	{
	}
	hooks =
	{
		[hook.MONSTER_DEATH] = function(m_idx)
			if current_dungeon_idx == DUNGEON_KZD_EAST then
				quest_data(QUEST_SIEGE_OF_KHAZAD_DUM).siege_remains = false
				-- Quest is not complete so long as anything is alive
				for_each_monster(function(m_idx, monst)
					if not has_flag(monst, FLAG_PERMANENT) then
						quest_data(QUEST_SIEGE_OF_KHAZAD_DUM).siege_remains = true
					end
				end)
				if quest_data(QUEST_SIEGE_OF_KHAZAD_DUM).siege_remains == false then
					if quest(QUEST_SIEGE_OF_KHAZAD_DUM).status == QUEST_STATUS_TAKEN then
						change_quest_status(QUEST_SIEGE_OF_KHAZAD_DUM, QUEST_STATUS_COMPLETED)
					else
						quest(QUEST_SIEGE_OF_KHAZAD_DUM).status = QUEST_STATUS_FINISHED
					end
				end
			end
		end
	}
}

add_quest
{
	global = "QUEST_DURINS_BANE"
	name = "Durin's Bane"
	desc = function()
		if quest(QUEST_DURINS_BANE).status == QUEST_STATUS_UNTAKEN or
			quest(QUEST_DURINS_BANE).status == QUEST_STATUS_FINISHED
		then
			return nil
		end

		local lines = {
			"#Durin's Bane (Danger level: 50)",
			"The original city of Khazad-dum was destroyed when its inhabitants dug too deep,",
			"and unearthed a demon of the previous age. This demon lies hidden somewhere in the",
			"mines of Moria beneath Khazad-dum. If the precious mithril in the mines is to be",
			"claimed, this demon must be destroyed. Find it, destroy it, and return to Balin.",
		}

		return lines
	end -- desc
	level = 50
	data =
	{
	}
	hooks =
	{
	}
}
