--
-- Race backgrounds
--

new_player_descriptor
{
	type = "race_background"
	name = "Normal"
	desc = {
		"A normal member of your race, with no specific background.",
	}
	flags = { PLACE=birth.place.NONE }
}
new_player_descriptor
{
	type = "race_background"
	name = "Barbarian"
	desc = {
		"Hardy members of their race they are strong fighters but poor spellcasters.",
	}
	stats = { [A_STR]=2, [A_INT]=-3, [A_WIS]=-2, [A_DEX]=1, [A_CON]=1, [A_CHR]=-3, }
	luck = 1
	mana = 50
	life_rating = 2
	experience = 25
	flags = { PLACE=birth.place.AFTER }
	levels =
	{
		[10] = {
			RESIST = getter.resists{FEAR=100}
		}
	}
}
new_player_descriptor
{
	type = "race_background"
	name = "Hermit"
	desc = {
		"Through years of isolation hermits can manage to increase their mana",
		"reserves but at the cost of an increased physical weakness.",
	}
	stats = { [A_STR]=-3, [A_INT]=1, [A_WIS]=1, [A_DEX]=-3, [A_CON]=-3, [A_CHR]=1, }
	mana = 120
	life_rating = -4
	experience = 20
	infravision = 1
	flags = { PLACE=birth.place.AFTER }
}
new_player_descriptor
{
	type = "race_background"
	name = "LostSoul"
	desc = {
		"In some very rare occasions souls can come back from the Halls of Mandos.",
	}
	flags = {
		PLACE=birth.place.AFTER NO_SUBRACE_CHANGE=1
		STARTING_DUNGEON = "DUNGEON_MANDOS"
		STARTING_LEVEL   = 98
	}
	starting_objects =
	{
		{ obj=function(obj) object_prep(obj, TV_SCROLL, SV_SCROLL_IDENTIFICATION) end min=25 max=25 },
		{ obj=function(obj) object_prep(obj, TV_SCROLL, SV_SCROLL_SATISFY_HUNGER) end min=25 max=25 },
	}
}

print("FINISH UNDEAD FORMS")
