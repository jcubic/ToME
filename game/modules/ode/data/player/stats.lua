--
-- Define the player main stats
--

-- Define the min/max of stats
stats.MIN_VALUE = 1
stats.MAX_VALUE = 12
stats.MAX_NATURAL = 10

-- Now define all the stats
player.define_stat
{
	define_as = "A_STR"
	name = "Strength"
	short = "STR"
	reduced = "Str"
	desc_positive = "strong"
	desc_negative = "weak"
}
player.define_stat
{
	define_as = "A_CON"
	name = "Constitution"
	short = "CON"
	reduced = "Con"
	desc_positive = "healthy"
	desc_negative = "sickly"
}

-- Define the player experience levels
player.define_experience_ladder
{
	10,
	20,
	40,
	80,
	100,
}
