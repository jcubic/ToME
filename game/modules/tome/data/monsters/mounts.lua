-- Player mounts

new_monster_races
{
	---------------------- horses -----------------------
	['q'] =
	{
		defaults = {
			body = default_body.quadruped
			flags =
			{
				SPECIAL_GENE=true
				ANIMAL=true FACTION=FACTION_ANIMAL
			}
		} -- defaults
		;
		{
			define_as = "RACE_MOUNT_HORSE"
			name = "horse"
			level = 10 rarity = 1
			desc =
			{
				"A rideable horse.",
			}
			color = color.UMBER
			speed = 10 life = {10,8} ac = 30
			aaf = 10 sleep = 10
			exp = 25
			weight = 4000
			blows =
			{
				{"KICK","CRUSH",{1,6}},
				{"BITE","PIERCE",{1,8}},
			}
			flags =
			{
				FORCE_MAXHP=true FORCE_SLEEP=true
				DROP_CORPSE=true ANIMAL=true
				MORTAL=true
				RIDEABLE=5
			}
		},
		{
			define_as = "RACE_MOUNT_TEST"
			name = "magical pony of fluffiness"
			level = 10 rarity = 1
			desc =
			{
				"A rideable horse.",
			}
			color = color.PINK
			speed = 20 life = {10,8} ac = 30
			aaf = 10 sleep = 10
			exp = 25
			weight = 4000
			blows =
			{
				{"KICK","CRUSH",{1,6}},
				{"BITE","PIERCE",{1,8}},
			}
			flags =
			{
				FORCE_MAXHP=true FORCE_SLEEP=true
				DROP_CORPSE=true ANIMAL=true
				MORTAL=true
				CAN_FLY=3
				RIDEABLE=5
			}
			spells =
			{
				frequency = 5
				["Phase Door"] = { level=10 chance=80 }
			}
		},
	}
}
