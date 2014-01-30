--
-- Undead forms
-- Those can NOT be choosen at birth
-- They are graned by special actions in the game
--

new_player_descriptor
{
	type = "undead"
	name = "Vampire"
	desc = {
		"Vampires are powerful undead, wielding great powers they still fear the",
		"sunlight and cannot easily satiate their hunger.",
	}
}
new_player_descriptor
{
	type = "undead"
	name = "Spectre"
	desc = {
		"Spectres only partially exist in the mortal world and so they can",
		"pass through walls. They are somewhat physically weak.",
	}
	stats = { [A_STR]=-5, [A_INT]=1, [A_WIS]=1, [A_DEX]=2, [A_CON]=-3, [A_CHR]=-6, }
	luck = -3
	mana = 105
	life_rating = -6
	experience = 80
	infravision = 3
	levels =
	{
--DGDGDGDG		[ 1] = { SEE_INVIS=1 HOLD_LIFE=1 SLOW_DIGEST=1 RES_COLD=1 RES_POIS=1 RES_NETHER=1 }
	}
}
new_player_descriptor
{
	type = "undead"
	name = "Skeleton"
	desc = {
		"Yet an other kind of undead. Their physical 'body' is not very vulnerable",
		"to sharp things.",
	}
	stats = { [A_INT]=-2, [A_WIS]=-2, [A_CON]=1, [A_CHR]=-4, }
	luck = -3
	mana = 70
	infravision = 1
	levels =
	{
--DGDGDGDG		[ 1] = { SEE_INVIS=1 HOLD_LIFE=1 RES_POIS=1 RES_SHARDS=1 }
--DGDGDGDG		[10] = { RES_COLD=1 }
	}
}
new_player_descriptor
{
	type = "undead"
	name = "Zombie"
	desc = {
		"Strong and dumb is a zombie.",
	}
	stats = { [A_STR]=2, [A_INT]=-6, [A_WIS]=-6, [A_DEX]=1, [A_CON]=4, [A_CHR]=-5, }
	luck = -4
	mana = 70
	life_rating = 4
	experience = 45
	infravision = 1
	flags = { PLACE=birth.place.BEFORE UNDEAD=1 NO_FOOD=1 NO_SUBRACE_CHANGE=1 }
	levels =
	{
--DGDGDGDG		[ 1] = { SEE_INVIS=1 HOLD_LIFE=1 SLOW_DIGEST=1 RES_POIS=1 }
--DGDGDGDG		[ 5] = { RES_COLD=1 }
	}
}
