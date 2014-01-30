new_artifact_set
{
	name = "Light Bringers"
	desc = "It is from a group of Elven items that strive to bring light to the world."
	artifacts =
	{
		-- Phial of Galadriel
		[ART_PHIAL] =
		{
			[2] = { STATS = getter.stats{[A_WIS]=3 [A_CHR]=4} RESIST = getter.resists{FIRE = 90 DARK=70} }
		}
		-- Sting
		[ART_NARYA] =
		{
			[2] = { STEALTH=2 REGEN=1 }
		}
	}
}
