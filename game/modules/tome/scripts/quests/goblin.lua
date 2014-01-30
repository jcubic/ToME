-- The Great Goblin
-- Does this quest need to exist?

add_quest
{
	global = "QUEST_GREAT_GOBLIN"
	name = "The Great Goblin"
	desc = function()
		if quest(QUEST_GREAT_GOBLIN).status == QUEST_STATUS_UNTAKEN or
			quest(QUEST_GREAT_GOBLIN).status == QUEST_STATUS_FINISHED
		then
			return nil
		end

		local lines = {
			"#yThe Great Goblin (Danger level: 20)",
			"Implement me!"
		}

		return lines
	end -- desc
	level = 20
	data =
	{
	}
	hooks =
	{
	}
}
