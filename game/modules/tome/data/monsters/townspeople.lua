-- Monster races definitions
new_monster_races
{
	------------------------ TOWNSPEOPLE -----------------------
	['t'] =	{
		defaults = {
			level = 0 rarity = 1
			speed = 0 sleep  = 0
			exp   = 0

			body = default_body.humanoid
			flags = {
				NON_INNATE_ARMOUR=true
				DROP_THEME = getter.flags{ THEME_GOLD=100 }
				MALE=true
--				RAND_MOVEMENT=75
				DROP_CORPSE=true TAKE_ITEM=true OPEN_DOOR=true WILD_TOWN=true
				WILD_ONLY=true MORTAL=true
				FACTION=FACTION_TOWN
				MOVE_BODY=true
				ALLOW_IN_TOWN_BREE=true
				AI=ai.TOWNSPEOPLE
			}
		} -- defaults


		[1] =
		{
			name = "merchant"
			desc =
			{
				"The typical ponce around town, with purse jingling, and looking for more",
				"amulets of adornment to buy.",
			}
			color = color.ORANGE
			life = {3,3} ac = 1
			aaf = 10
			weight = 1500
			blows =
			{
				{"HIT","CRUSH",{1,3}},
			}
			flags =
			{
				DROP_60=true BASH_DOOR=true HAS_LITE=true
				TOWN_PEOPLE_CAN_ELF=true TOWN_PEOPLE_CAN_HUMAN=true
				TOWN_PEOPLE_CAN_DWARF=true TOWN_PEOPLE_CAN_HOBBIT=true
			}
		}
		[2] =
		{
			name = "wanderer"
			desc =
			{
				"A simple traveler that came here for whatever purpose suits him.",
			}
			color = color.LIGHT_WHITE
			life = {2,3} ac = 1
			aaf = 10
			weight = 1500
			blows =
			{
				{"HIT","CRUSH",{1,3}},
			}
			flags =
			{
				DROP_60=true BASH_DOOR=true HAS_LITE=true
				TOWN_PEOPLE_CAN_ELF=true TOWN_PEOPLE_CAN_HUMAN=true
				TOWN_PEOPLE_CAN_DWARF=true TOWN_PEOPLE_CAN_HOBBIT=true
			}
		}
		[3] =
		{
			name = "child"
			desc =
			{
				"A local town's child.",
			}
			color = color.RED
			life = {1,1} ac = 0
			aaf = 14
			weight = 700
			body = { INVEN=16 }
			blows =
			{
			}
			flags =
			{
				DROP_THEME = getter.flags{ THEME_JUNK=100 }
				TOWN_PEOPLE_CAN_ELF=true TOWN_PEOPLE_CAN_HUMAN=true
				TOWN_PEOPLE_CAN_DWARF=true TOWN_PEOPLE_CAN_HOBBIT=true
			}
		}
		[4] =
		{
			name = "farmer"
			desc =
			{
				"A local town's farmer.",
			}
			color = color.GREEN
			life = {2,4} ac = 3
			aaf = 10
			weight = 1500
			blows =
			{
				{"HIT","PIERCE",{1,2}},
				{"HIT","CRUSH",{1,2}},
			}
			flags =
			{
				DROP_60=true
				TOWN_PEOPLE_CAN_ELF=true TOWN_PEOPLE_CAN_HUMAN=true
				TOWN_PEOPLE_CAN_HOBBIT=true
			}
		}
		[5] =
		{
			name = "miner"
			desc =
			{
				"A local town's miner.",
			}
			color = color.UMBER
			life = {2,4} ac = 3
			aaf = 10
			weight = 1500
			blows =
			{
				{"HIT","CRUSH",{2,2}},
			}
			flags =
			{
				DROP_60=true
				TOWN_PEOPLE_CAN_HUMAN=true TOWN_PEOPLE_CAN_DWARF=true
			}
		}
	}


	['@'] =	{
		defaults = {

			body = default_body.humanoid
		} -- defaults
                ;
		------------------------ Bree -----------------------
		{
			define_as = "RACE_FARLINA_SNOWFOOT"
			name = "Farlina Snowfoot"
			desc = "Farlina seems worried."
			level = 1 rarity = 1
			color = color.RED
			speed = 0 life = {10,10} ac = 2
			aaf = 40 sleep = 0
			exp = 0
			weight = 730
			flags =
			{
				FEMALE=true FORCE_MAXHP=true SPECIAL_GENE=true NEVER_MOVE=true
				NEVER_BLOW=true GOOD=true NO_TARGET=true MORTAL=true UNIQUE=true
				NO_DEATH=true NO_PUSHBACK=true SAVED_LEVEL_NO_DECAY=true
				FACTION=FACTION_TOWN
				CHATABLE=function(name) dialogs.FARLINA_SNOWFOOT() end
				AI=ai.NEVER_MOVE

			}
		}
		------------------------ Lothlorien -----------------------
		{
			define_as = "RACE_GALADRIEL"
			name = "Lady Galadriel"
			desc = "The beautiful lady Galadriel, Queen of the elves of Lothlorien."
			level = 100 rarity = 1
			color = color.YELLOW
			speed = 50 life = {100,100} ac = 250
			aaf = 40 sleep = 0
			exp = 0
			weight = 730
			flags =
			{
				FEMALE=true FORCE_MAXHP=true SPECIAL_GENE=true NEVER_MOVE=true
				NEVER_BLOW=true GOOD=true NO_TARGET=true UNIQUE=true
				NO_DEATH=true NO_PUSHBACK=true SAVED_LEVEL_NO_DECAY=true
				FACTION=FACTION_ELF NEUTRAL=true
				CAN_SPEAK=function(name) monster_random_say{
					name.." requests that you speak with her as soon as possible.",
					"You feel "..name.."'s eyes reaching into your soul.",
				}end
				CHATABLE=function(name) dialogs.GALADRIEL() end
			}
		}
		{
			define_as = "RACE_ELROND"
			name = "Lord Elrond, the half-elven"
			desc = "One of the greatest Elves of Middle-earth, ruler of Rivendell. He is wise and powerful."
			level = 100 rarity = 1
			color = color.AQUAMARINE
			speed = 50 life = {100,100} ac = 250
			aaf = 40 sleep = 0
			exp = 0
			weight = 730
			flags =
			{
				MALE=true FORCE_MAXHP=true SPECIAL_GENE=true NEVER_MOVE=true
				NEVER_BLOW=true GOOD=true NO_TARGET=true UNIQUE=true
				NO_DEATH=true NO_PUSHBACK=true SAVED_LEVEL_NO_DECAY=true
				FACTION=FACTION_ELF NEUTRAL=true
				CAN_SPEAK=function(name) monster_random_say{
					name.." welcomes you to Rivendell.",
				}end
				CHATABLE=function(name) dialogs.ELROND() end
			}
		}
		------------------------ Khazad-dum -----------------------
		{
			define_as = "RACE_BALIN"
			name = "Balin, Lord of Moria"
			desc = "Describe me!"
			level = 100 rarity = 1
			color = color.UMBER
			speed = 50 life = {100,100} ac = 250
			aaf = 40 sleep = 0
			exp = 0
			weight = 1500
			flags =
			{
				MALE=true FORCE_MAXHP=true SPECIAL_GENE=true NEVER_MOVE=true
				NEVER_BLOW=true GOOD=true NO_TARGET=true UNIQUE=true
				NO_DEATH=true NO_PUSHBACK=true SAVED_LEVEL_NO_DECAY=true
				FACTION=FACTION_DWARF NEUTRAL=true
				CAN_SPEAK=function(name)
					monster_random_say{
						name.." grumbles about orcs.",
					}
				end
				CHATABLE=function(name)
					dialogs.BALIN()
				end
			}
		}
	}
}
