-- Find the shards of narsil
add_quest
{
	global = "QUEST_BARROWS_KING"
	name = "The Lost Heirloom of Farlina"
	desc = function()
		if quest(QUEST_BARROWS_KING).status == QUEST_STATUS_UNTAKEN or quest(QUEST_BARROWS_KING).status == QUEST_STATUS_FINISHED
		then
			return nil
		end

		local lines = {
			"#yThe Lost Heirloom of Farlina (Danger level: 12)",
			"Farlina Snowfoot has asked you to venture in the Barrows to the west of Bree",
			"just outside its borders and find her heirloom.",
		}
		if have_object(USE_INVEN, HAVE_ARTIFACT, ART_FARLINA_HEIRLOOM) then
			tinsert(lines, "You have found it, bring it back to her.")
		else
			tinsert(lines, "It is probably in possession of some specter of the Barrows.")
		end
		return lines
	end -- desc
	level = 12
	data =
	{
	}
	hooks =
	{
	}
}
