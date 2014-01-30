-- Global(not dungeon/... specific) monster races definitions
new_monster_races
{
	['#'] =
	{
		defaults =
		{
			flags =
			{
				ALLOW_IN_FOREST=true
				ALLOW_IN_PLAIN=true
			}
		}
		[1] =
		{
			name = "poison ivy"
			level = 10 rarity = 4
			desc = "A mass of vegetation.  It seems to be growing..."
			color = color.GREEN
			speed = -10 life = {5,5} ac = 5
			aaf = 5 sleep = 5
			exp = 10 body = default_body.blob
			weight = 50
			blows =
			{
				{"HIT","POISON",{3,6}},
				{"HIT","POISON",{3,6}},
			}
			flags =
				{
				AI=ai.NEVER_MOVE
				RESIST=getter.resists{CONFUSION=50 FEAR=100 FIRE=-50 POIS=100 PIERCE=20 SLASH=-10}
				FORCE_SLEEP=true STUPID=true EMPTY_MIND=true FRIENDS=getter.friends(100,7)
				ANIMAL=true COLD_BLOOD=true NEVER_MOVE=true
				MULTIPLY=true NO_SLEEP=true MORTAL=true DROP_60=true
				NO_CUT=true SEE_INVIS=15 NEVER_MOVE=true
				KILL_WALL=getter.flags{DEAD_TREE_FEAT=true}
				SPEEDS=getter.array{[SPEED_DIG]=100}
				ALLOW_IN_PLAIN=true
			}
		}
		[2] =
		{
			name = "tangleweed"
			level = 10 rarity = 4
			desc =
			{
				"A mass of vegetation.  As you pass near it, it reaches out tendrils to",
				"ensnare you.  You can just make out skeletons of its previous victims",
				"deep within the thickets.",
			}
			color = color.GREEN
			speed = -10 life = {5,5} ac = 5
			aaf = 5 sleep = 5
			exp = 10 body = default_body.blob
			weight = 50
			blows =
			{
				{"HIT","PARALYZE",{0,0}},
				{"HIT","PARALYZE",{0,0}},
			}
			flags =
			{
				AI=ai.NEVER_MOVE
				RESIST=getter.resists{CONFUSION=50 FEAR=100 FIRE=-50 PIERCE=20 SLASH=-10}
				FORCE_SLEEP=true NEVER_MOVE=true STUPID=true EMPTY_MIND=true FRIENDS=getter.friends(100,7)
				ANIMAL=true COLD_BLOOD=true SEE_INVIS=15 NEVER_MOVE=true
				NO_SLEEP=true MORTAL=true DROP_60=true NO_CUT=true
				SPEEDS=getter.array{[SPEED_DIG]=100}
				ALLOW_IN_PLAIN=true
			}
		}
		[3] =
		{
			name = "ent sapling"
			level = 15 rarity = 3
			desc = 
			{
			 	"This tree is young, only a few metres high, but has pulled up its roots",
			 	"and is walking Arda! Though it is still spindly, it has the knobbly",
			 	"fingers and craggy brows of all ents!"
			}
			color = color.LIGHT_GREEN
			speed = -10 life = {10,12} ac = 30
			aaf = 30 sleep = 30
			exp = 100 body = default_body.humanoid
			weight = 5000
			blows =
			{
				{"PUNCH","CRUSH",{6,6}},
				{"PUNCH_OFFHAND","CRUSH",{6,6}},
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=12 PIERCE=15 SLASH=8 FIRE=-66}
				FACTION=FACTION_ENT SPEEDS=getter.speeds{WALK=10}
				AI=ai.ZOMBIE DROP_THEME=getter.flags{THEME_JUNK=70 THEME_FOOD=30}
			}
		}
		[4] =
		{
			name = "ent"
			level = 30 rarity = 3
			desc = 
			{
			 	"This huge sentient tree turns slowly on its roots, and you spy massive",
			 	"woody fingers clench and unclench. It looks slow, but you would not like",
			 	"to get in its way if it got up a head of speed!",
			}
			color = color.YELLOW_GREEN
			speed = 0 life = {15,15} ac = 40
			aaf = 30 sleep = 50
			exp = 220 body = default_body.humanoid
			weight = 9000
			blows =
			{
				{"PUNCH","CRUSH",{8,8}},
				{"PUNCH_OFFHAND","CRUSH",{8,8}},
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=15 PIERCE=18 SLASH=10 FIRE=-66}
				FACTION=FACTION_ENT SPEEDS=getter.speeds{WALK=10}
				AI=ai.ZOMBIE FRIENDS=getter.friends(30,4)
				DROP_THEME=getter.flags{THEME_JUNK=30 THEME_FOOD=30 THEME_CANTRIPS=40}
			}
		}
		[5] =
		{
			name = "huorn"
			level = 40 rarity = 5
			desc = 
			{
			 	"This towering tree is wreathed in darkness and its twisted branches",
				"writhe wihtout wind to stir them. This Huorn is a dark and terrible",
				"Ent, not entirely under anyone's control." ,
			}
			color = color.DARK_SEA_GREEN
			speed = 10 life = {20,20} ac = 55
			aaf = 30 sleep = 50
			exp = 200 body = default_body.humanoid
			weight = 9000
			blows =
			{
				{"CHARGE","CRUSH",{12,10}},
				{"CLAW","SLASH",{10,10}},
				{"CLAW","SLASH",{10,10}},
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=15 DARK=66 PIERCE=18 SLASH=10 FIRE=-66}
				FACTION=FACTION_ENT SPEEDS=getter.speeds{WALK=10} FRIENDS=getter.friends(50,8)
				WEIRD_MIND=true DROP_THEME=getter.flags{THEME_JUNK=10 THEME_FOOD=40 THEME_CANTRIPS=50}
				MANA_CURR=10 MANA_MAX=10 MANA_REGENERATE=50
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
				MOVE_AI=ai.RANDOM_MOVE
				CASTING_AI=ai.SMART_CAST
				FEAR_LEVEL=0
				MOVE_RATE=10}
			}
			spells=
			{
				frequency=8
					["Phase Door"] = {level=20 chance=75}
			}
		}
		[6] =
		{
			name = "old ent"
			level = 65 rarity = 3
			desc = 
			{
			 	"This massive ent is an old, old tree whose line reaches back to the",
				"first age. It is king of the forest and wise beyond measure. It's'",
				"branches are covered in moss and its bark is as thick as plate armour!'",
			}
			color = color.DARK_GREEN
			speed = 0 life = {30,20} ac = 30
			aaf = 30 sleep = 100
			exp = 200 body = default_body.humanoid
			weight = 15000
			blows =
			{
				{"CHARGE","CRUSH",{15,15}},
				{"PUNCH","CRUSH",{20,11}},
			}
			flags =
			{
				RESIST=getter.resists{CONFUSION=50 CRUSH=20 COLD=66 DARK=66 FEAR=100 PIERCE=25 SLASH=15}
				FACTION=FACTION_ENT SPEEDS=getter.speeds{WALK=20 CAST=-20}
				DROP_THEME=getter.flags{THEME_DEVICE=30 THEME_CANTRIPS=40 THEME_TREASURE=30}
				FORCE_MAXHP=true NO_SLEEP=true NO_STUN=true REGENERATE=5000
				ESCORT=true ESCORT_CHARS="#" REFLECTING=10
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
				MOVE_AI=ai.ZOMBIE
				CASTING_AI=ai.SMART_CAST}
			}
			spells=
			{
				frequency=5
					["Missile"] = {level=90 chance=75}
					["Summon Kin"] = {level=75 chance =90}
					["Summon Animals"] = {level=65 chance =80}
			}
		}
	}
	['$'] =
	{
		defaults =
		{
			body = default_body.blob
			flags =
			{
				ALLOW_IN_CAVE=true
				ALLOW_IN_OCEAN=true
				ALLOW_IN_PLAIN=true
				ALLOW_IN_DESERT=true

				ONLY_GOLD=true COLD_BLOOD=true FACTION=FACTION_DUNGEON
				BASH_DOOR=true NO_SLEEP=true NO_CUT=true
			}
		}
		[1] =
		{
			name = "creeping copper coins"
			level = 4 rarity = 3
			desc =
			{
				"It appears to be a pile of copper coins, until it starts crawling towards you",
				"on tiny legs.",
			}
			color = color.UMBER
			speed = -10 life = {8,9} ac = 20
			aaf = 3 sleep = 10
			exp = 9
			weight = 0
			blows =
			{
				{"HIT","CRUSH",{6,6}},
				{"TOUCH","POISON",{6,6}},
			}
			flags =
			{
				RESIST=getter.resists{ACID=-66 CONFUSION=50 ELEC=100 POIS=100}
				CHAR_MULTI=true DROP_NUMBER=getter.random(1,2) AI=ai.SNAKE
				DROP_THEME=getter.flags{THEME_GOLD=100}
			}
		}
		[2] =
		{
			name = "creeping silver coins"
			level = 6 rarity = 3
			desc =
			{
				"It appears to be a pile of silver coins, until it starts crawling towards you",
				"on tiny legs.",
			}
			color = color.SLATE
			speed = -10 life = {12,10} ac = 24
			aaf = 4 sleep = 10
			exp = 18
			weight = 0
			blows =
			{
				{"HIT","CRUSH",{6,10}},
				{"TOUCH","POISON",{6,10}},
			}
			flags =
			{
				RESIST=getter.resists{ACID=-66 CONFUSION=50 ELEC=100 POIS=100}
				CHAR_MULTI=true AI=ai.SNAKE DROP_THEME=getter.flags{THEME_GOLD=100}
			}
		}
		[3] =
		{
			name = "creeping gold coins"
			level = 10 rarity = 3
			desc =
			{
				"It appears to be a pile of gold coins, until it starts crawling towards you",
				"on tiny legs.",
			}
			color = color.YELLOW
			speed = -10 life = {20,10} ac = 30
			aaf = 5 sleep = 10
			exp = 32
			weight = 0
			blows =
			{
				{"HIT","CRUSH",{6,12}},
				{"TOUCH","POISON",{6,10}},
			}
			flags =
			{
				RESIST=getter.resists{ACID=-66 CONFUSION=50 ELEC=100 POIS=100}
				DROP_THEME=getter.flags{THEME_GOLD=100} AI=ai.SNAKE
			}
		}
		[4] =
		{
			name = "creeping mithril coins"
			level = 13 rarity = 3
			desc =
			{
				"It appears to be a pile of sentient mithril coins that doesn't like being",
				"picked up.",
			}
			color = color.LIGHT_BLUE
			speed = 0 life = {20,8} ac = 35
			aaf = 5 sleep = 10
			exp = 45
			weight = 0
			blows =
			{
				{"HIT","CRUSH",{8,11}},
				{"TOUCH","POISON",{8,10}},
			}
			flags =
			{
				RESIST=getter.resists{ACID=100 CONFUSION=50 ELEC=100 POIS=100 SLASH=10 PIERCE=15}
				CHAR_MULTI=true AI=ai.ZOMBIE DROP_THEME=getter.flags{THEME_GOLD=100}

			}
		}
		[5] =
		{
			name = "creeping adamantite coins"
			level = 27 rarity = 3
			desc =
			{
				"It appears to be a pile of coins made of precious adamant, slithering toward",
				"you on lots of tiny legs.",
			}
			color = color.LIGHT_GREEN
			speed = 10 life = {20,25} ac = 40
			aaf = 5 sleep = 10
			exp = 60
			weight = 0
			blows =
			{
				{"BITE","POISON",{6,6}},
				{"TOUCH","POISON",{4,6}},
				{"HIT","CRUSH",{3,12}},
				{"HIT","CRUSH",{3,12}},
			}
			flags =
			{
				RESIST=getter.resists{ACID=100 CONFUSION=50 ELEC=100 POIS=100 SLASH=15 PIERCE=20 CRUSH=10}
				CHAR_MULTI=true AI=ai.ZOMBIE DROP_THEME=getter.flags{THEME_GOLD=100}
			}
		}
	}
	[','] =
	{
		defaults =
		{
			body = default_body.blob
			flags =
			{
				ALLOW_IN_EVERYWHERE=true ALLOW_IN_CAVE=true
				ALLOW_IN_SWAMP=true	COLD_BLOOD=true DROP_CORPSE=true
			}
		}
		[1] =
		{
			name = "grey mushroom patch"
			level = 1 rarity = 1
			desc = "Yum! It looks quite tasty."
			color = color.SLATE
			speed = 0 life = {1,2} ac = 1
			aaf = 2 sleep = 0
			exp = 1
			weight = 10
			blows =
			{
				{"SPORE","CONFUSE",{3,4}},
			}
			flags =
			{
				RESIST=getter.resists{CONFUSION=50 FEAR=100 POIS=100}
				NEVER_MOVE=true STUPID=true EMPTY_MIND=true DROP_CORPSE=true NO_SLEEP=true
				NO_CUT=true AI=ai.NEVER_MOVE
			}
		}
		[2] =
		{
			name = "shrieker mushroom patch"
			level = 2 rarity = 1
			desc = "Yum!  It looks quite tasty.  It doesn't sound so nice, though..."
			color = color.LIGHT_RED
			speed = 0 life = {1,1} ac = 1
			aaf = 4 sleep = 0
			exp = 1
			weight = 40
			flags =
			{
				RESIST=getter.resists{CONFUSION=50 FEAR=100 POIS=100}
				FORCE_SLEEP=true NEVER_MOVE=true NEVER_BLOW=true STUPID=true EMPTY_MIND=true
				DROP_CORPSE=true NO_SLEEP=true AI = ai.RANDOM_CAST
				NO_CUT=true
			}
			spells =
			{
				frequency = 4
				["Shriek"] = { level=50 chance=100 }
			}
		}
		[3] =
		{
			name = "yellow mushroom patch"
			level = 2 rarity = 1
			desc = "Yum!  It looks quite tasty."
			color = color.YELLOW
			speed = 0 life = {1,1} ac = 1
			aaf = 2 sleep = 0
			exp = 2
			weight = 30
			blows =
			{
				{"SPORE","TERRIFY",{3,6}},
			}
			flags =
			{
				RESIST=getter.resists{CONFUSION=50 FEAR=100 POIS=100}
				NEVER_MOVE=true STUPID=true EMPTY_MIND=true
				NO_SLEEP=true MORTAL=true NO_CUT=true AI=ai.NEVER_MOVE
			}
		}
		[4] =
		{
			name = "spotted mushroom patch"
			level = 3 rarity = 1
			desc = "Yum!  It looks quite tasty."
			color = color.ORANGE
			speed = 0 life = {1,1} ac = 1
			aaf = 2 sleep = 0
			exp = 3
			weight = 30
			blows =
			{
				{"SPORE","POISON",{6,4}},
			}
			flags =
			{
				RESIST=getter.resists{CONFUSION=50 FEAR=100 POIS=100}
				NEVER_MOVE=true STUPID=true EMPTY_MIND=true
				NO_SLEEP=true NO_CUT=true AI=ai.NEVER_MOVE
			}
		}
		[5] =
		{
			name = "purple mushroom patch"
			level = 6 rarity = 2
			desc = "Yuk!  It doesn't look so tasty."
			color = color.VIOLET
			speed = 0 life = {1,1} ac = 1
			aaf = 2 sleep = 0
			exp = 15
			weight = 40
			blows =
			{
				{"SPORE","LOSE_CON",{3,2}},
				{"SPORE","LOSE_CON",{3,2}},
				{"SPORE","LOSE_CON",{3,2}},
			}
			flags =
			{
				RESIST=getter.resists{CONFUSION=50 FEAR=100 POIS=100}
				NEVER_MOVE=true CAN_SWIM=true STUPID=true EMPTY_MIND=true NO_SLEEP=true
				NO_CUT=true AI=ai.NEVER_MOVE
			}
		}
		[6] =
		{
			name = "clear mushroom patch"
			level = 10 rarity = 2
			desc = "Yum! It smells quite tasty.  If you could only see it..."
			color = color.LIGHT_BLUE
			speed = 10 life = {1,1} ac = 1
			aaf = 4 sleep = 0
			exp = 3
			weight = 30
			blows =
			{
				{"SPORE","POISON",{3,1}},
			}
			flags =
			{
				RESIST=getter.resists{CONFUSION=50 FEAR=100 POIS=100}
				ATTR_CLEAR=true NEVER_MOVE=true INVISIBLE=true COLD_BLOOD=true STUPID=true
				EMPTY_MIND=true CAN_SWIM=true MULTIPLY=true NO_SLEEP=true NO_CUT=true AI=ai.NEVER_MOVE
			}
		}
		[8] =
		{
			name = "magic mushroom patch"
			level = 15 rarity = 2
			desc = "Yum!  It looks quite tasty. It seems to glow with an unusual light."
			color = color.LIGHT_BLUE
			speed = 30 life = {1,1} ac = 10
			aaf = 40 sleep = 0
			exp = 10
			weight = 50
			blows =
			{
				{"SPORE","CONFUSE",{0,0}},
				{"SPORE","CONFUSE",{0,0}},
				{"SPORE","BLIND",{0,0}},
				{"SPORE","ACID",{0,0}},
			}
			flags =
			{
				RESIST=getter.resists{CONFUSION=50 FEAR=100 POIS=100}
				FORCE_SLEEP=true NEVER_MOVE=true STUPID=true RES_TELE=true CAN_SWIM=true
				NO_SLEEP=true NO_CUT=true AI = ai.SPELL_AND_MOVE
				MANA_CURR=40 MANA_MAX=40 MANA_REGENERATE=200
				AI_STATES=getter.flags
				{
					MIN_RANGE=2
					BEST_RANGE=4
					CASTING_AI=ai.SMART_CAST
					MOVE_AI=ai.NEVER_MOVE
				}
			}
			spells =
			{
				frequency = 2
				["Darkness"] = { level=80 chance=96 }
				["Phase Door"] = { level=59 chance=98 }
				["Scare"] = { level=74 chance=95 }
				["Slow Monster"] = { level=49 chance=96 }
			}
		}
		[9] =
		{
			name = "shambling mound"
			level = 18 rarity = 2
			desc =
			{
				"A pile of rotting vegetation that slides towards you with a disgusting",
				"stench, waking all it nears.",
			}
			color = color.GREEN
			speed = 0 life = {22,7} ac = 10
			aaf = 20 sleep = 40
			exp = 75
			weight = 3000
			blows =
			{
				{"HIT","SLASH",{6,6}},
				{"HIT","CRUSH",{6,6}},
			}
			flags =
			{
				RESIST=getter.resists{CONFUSION=50 FEAR=100 DARK_FIRE=50 POIS=66 CRUSH=10}
				STUPID=true EMPTY_MIND=true OPEN_DOOR=true DROP_THEME=getter.flags{THEME_GOLD=100}
				BASH_DOOR=true EVIL=true NO_SLEEP=true NO_CUT=true
				MANA_CURR=10 MANA_MAX=10 MANA_REGENERATE=100
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags
				{
					CASTING_AI=ai.SMART_CAST
					MOVE_AI=ai.SNAKE
				}
			}
			spells =
			{
				frequency = 4
				["Shriek"] = { level=50 chance=86 }
			}
		}
		[10] =
		{
			name = "memory moss"
			level = 32 rarity = 3
			desc =
			{
				"A mass of vegetation. You don't remember seeing anything like it",
				"before.",
			}
			color = color.BLUE
			speed = 0 life = {1,2} ac = 1
			aaf = 30 sleep = 5
			exp = 150
			weight = 50
			blows =
			{
				{"HIT","CONFUSE",{3,4}},
				{"HIT","CONFUSE",{3,4}},
			}
			flags =
			{
				RESIST=getter.resists{CONFUSION=50 FEAR=100 POIS=66}
				FORCE_SLEEP=true NEVER_MOVE=true STUPID=true EMPTY_MIND=true
				NO_SLEEP=true MORTAL=true NO_CUT=true
				MANA_CURR=10 MANA_MAX=10 MANA_REGENERATE=100
				AI = ai.SPELL_AND_MOVE AI_STATES=getter.flags
				{
					CASTING_AI=ai.SMART_CAST
					MOVE_AI=ai.NEVER_MOVE
				}
			}
			spells =
			{
				frequency = 6
				["Forget"] = { level=80 chance=100 }
			}
		}
		[11] =
		{
			name = "blood sprout"
			level = 50 rarity = 1
			desc = "A kind of giant mycorrhiza, corrupted into a carnivore by Morgoth."
			color = color.GREEN
			speed = 30 life = {5,6} ac = 1
			aaf = 10 sleep = 0
			exp = 3
			weight = 50
			blows =
			{
				{"TOUCH","CRUSH",{10,12}},
				{"TOUCH","CRUSH",{10,12}},
				{"TOUCH","PIERCE",{10,12}},
				{"TOUCH","SLASH",{10,12}},
			}
			flags =
			{
				RESIST=getter.resists{ACID=100 COLD=100 CONFUSION=50 ELEC=100 FEAR=100 POIS=100}
				STUPID=true EMPTY_MIND=true NO_SLEEP=true MORTAL=true
				MULTIPLY=true AI = ai.ZOMBIE
				KILL_WALL=getter.flags{DEAD_TREE_FEAT=true}
				SPEEDS=getter.array{[SPEED_DIG]=100}
			}
		}
	}
	['.'] =
	{
		defaults =
		{
			body = default_body.blob
			flags =
			{
				ALLOW_IN_EVERYWHERE=true ALLOW_IN_CAVE=true
			}
		}
		[1] =
		{
			name = "trapper"
			level = 36 rarity = 1
			desc =
			{
				"This creature traps unsuspecting victims and paralyzes ",
				"them, to be slowly digested later."
			}
			color = color.LIGHT_BLUE
			speed = 10 life = {100,10} ac = 1
			aaf = 4 sleep = 0
			exp = 580
			weight = 30
			blows =
			{
				{"HIT","CRUSH",{12,8}},
				{"HIT","CRUSH",{12,8}},
				{"HIT","PARALYZE",{15,3}},
				{"HIT","PARALYZE",{15,3}},
			}
			flags =
			{
				RESIST=getter.resists{CONFUSION=50 FEAR=100 POIS=100} ATTR_CLEAR=true
				ATTR_CLEAR=true NEVER_MOVE=true INVISIBLE=true COLD_BLOOD=true
				STUPID=true	EMPTY_MIND=true CAN_SWIM=true NO_SLEEP=true NO_CUT=true
				NO_AUTO_TARGET=true REGENERATE=400 AI = ai.NEVER_MOVE
				DROP_THEME=getter.flags{THEME_COMBAT=30 THEME_MAGIC=20 THEME_TOOLS=20 THEME_JUNK=30}
			}
		}
	}
	['a'] =
	{
		defaults =
		{
			body = default_body.insect
			flags =
			{
				ALLOW_IN_EVERYWHERE=true ANIMAL=true SEE_INVIS=15
				WEIRD_MIND=90 BASH_DOOR=true DROP_SKELETON=true
				MORTAL=true FACTION=FACTION_ANIMAL COLD_BLOOD=true
			}
		}
		[1] =
		{
			name = "worker ant"
			level = 1 rarity = 1
			desc = "A large ant with powerful mandibles."
			color = color.UMBER
			speed = 0 life = {2,5} ac = 3
			aaf = 10 sleep = 10
			exp = 3
			weight = 300
			blows =
			{
				{"BITE","PIERCE",{2,6}},
			}
			flags=
			{
				AI = ai.SNAKE ALLOW_IN_PLAIN=true
				RESIST=getter.resists{CRUSH=10 SLASH=10}
			}
		}
		[2] =
		{
			name = "soldier ant"
			level = 2 rarity = 1
			desc = "It is about three feet long."
			color = color.LIGHT_DARK
			speed = 0 life = {3,8} ac = 15
			aaf = 8 sleep = 80
			exp = 8
			weight = 500
			blows =
			{
				{"BITE","PIERCE",{5,4}},
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=10 SLASH=10}
				AI = ai.SNAKE ALLOW_IN_PLAIN=true
			}
		}
		[3] =
		{
			name = "giant white ant"
			level = 3 rarity = 1
			desc = "It is about two feet long and has sharp pincers."
			color = color.WHITE
			speed = 0 life = {3,8} ac = 16
			aaf = 8 sleep = 80
			exp = 7
			weight = 800
			blows =
			{
				{"BITE","PIERCE",{4,4}},
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=10 SLASH=10}
				AI=ai.SNAKE ALLOW_IN_PLAIN=true
			}
		}
		[4] =
		{
			name = "giant red ant"
			level = 9 rarity = 2
			desc = "It is large and has venomous mandibles."
			color = color.RED
			speed = 0 life = {4,8} ac = 26
			aaf = 12 sleep = 60
			exp = 22
			weight = 600
			blows =
			{
				{"BITE","PIERCE",{5,8}},
				{"STING","LOSE_STR",{6,5}},
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=10 SLASH=10}
				FRIENDS=getter.friends(60,20) AI= ai.GHOUL
				AI_STATES=getter.flags{MOVE_RATE=10}
				ALLOW_IN_PLAIN=true
			}
		}
		[5] =
		{
			name = "giant silver ant"
			level = 23 rarity = 1
			desc = "A giant silver ant that spits a caustic fluid!."
			color = color.LIGHT_WHITE
			speed = 0 life = {10,10} ac = 33
			aaf = 10 sleep = 40
			exp = 45
			weight = 800
			blows =
			{
				{"BITE","PIERCE",{10,6}},
				{"SPIT","ACID",{8,6}},
 			}
			flags =
			{
				RESIST=getter.resists{CRUSH=10 SLASH=10}
				FRIENDS=getter.friends(60,20) AI= ai.GHOUL
				AI_STATES=getter.flags{MOVE_RATE=10}
				ALLOW_IN_PLAIN=true
			}
		}
		[6] =
		{
			name = "giant grey ant"
			level = 26 rarity = 1
			desc = "It is an ant encased in shaggy grey fur."
			color = color.SLATE
			speed = 0 life = {18,10} ac = 36
			aaf = 10 sleep = 40
			exp = 90
			weight = 700
			blows =
			{
				{"BITE","PIERCE",{9,11}},
			}
			flags =
			{
 				RESIST=getter.resists{CRUSH=10 SLASH=10}
				KILL_BODY=true FRIENDS=getter.friends(60,20)
				AI=ai.ZOMBIE
				AI_STATES=getter.flags{FEAR_LEVEL=0}
			}
		}
		[7] =
		{
			name = "giant army ant"
			level = 30 rarity = 3
			desc =
			{
				"An armoured form moving with purpose. Powerful on its own, flee when",
				"hordes of them march.",
			}
			color = color.ORANGE
			speed = 10 life = {20,12} ac = 40
			aaf = 10 sleep = 40
			exp = 110
			weight = 600
			blows =
			{
				{"BITE","PIERCE",{6,12}},
				{"BITE","PIERCE",{6,12}},
				{"STING","POISON",{6,9}},
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=15 SLASH=15 PIERCE=10 CONFUSION=50 FEAR=100}
				KILL_BODY=true FRIENDS=getter.friends(60,30)
				AI= ai.ZOMBIE
				AI_STATES=getter.flags{FEAR_LEVEL=0}
			}
		}
		[8] =
		{
			name = "giant fire ant"
			level = 35 rarity = 1
			desc = "A giant ant covered in shaggy fur. Its powerful jaws glow with heat."
			color = color.LIGHT_RED
			speed = 0 life = {22,12} ac = 40
			aaf = 14 sleep = 40
			exp = 350
			weight = 700
			blows =
			{
				{"BITE","FIRE",{12,10}},
				{"BITE","FIRE",{12,10}},
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=15 SLASH=15 PIERCE=10 COLD=-66 FIRE=100}
				FORCE_MAXHP=true KILL_BODY=true FRIENDS=getter.friends(60,25)
				AI= ai.ZOMBIE
				AI_STATES=getter.flags{LURE_FROM_CORRIDOR=true}
			}
			default_flags_to_ignore =
			{
				COLD_BLOOD=true
			}
		}
		[9] =
		{
			name = "the queen ant"
			level = 37 rarity = 2
			desc = "She's upset because you hurt her children."
			color = color.VIOLET
			speed = 10 life = {18,100} ac = 50
			aaf = 30 sleep = 10
			exp = 1000
			weight = 2000
			blows =
			{
				{"BITE","PIERCE",{6,12}},
				{"BITE","PIERCE",{6,12}},
			}
			flags =
			{
				RESIST=getter.resists{SLASH=15 PIERCE=10 CRUSH=15}
				UNIQUE=true FEMALE=true DROP_CORPSE=true FORCE_SLEEP=true
				FORCE_MAXHP=true ONLY_ITEM=true
				ESCORT=true ESCORTED_BY_GROUPS=true
				DROP_NUMBER=getter.random(2,4) DROP_GOOD=true
				AI= ai.SPELL_AND_MOVE
				SPEEDS = getter.speeds{WALK=-30}
				AI_STATES=getter.flags
				{
					MOVE_AI= ai.ZOMBIE
					CASTING_AI = ai.SMART_CAST
				}
			}
			spells =
			{
				frequency = 2
				["Summon Kin"] = { level=80 chance=90 }
			}
		}
	}
	['b'] =
	{
		defaults =
		{
			body = default_body.bird
			flags =
			{
				ALLOW_IN_EVERYWHERE=true ALLOW_IN_CAVE=true ANIMAL=true
				FACTION=FACTION_ANIMAL DROP_CORPSE=true AERIAL=true CAN_FLY=3 SEE_INVIS=15
				DROP_CORPSE=true DROP_SKELETON=true
			}
		}
		[1] =
		{
			name = "fruit bat"
			level = 1 rarity = 1
			desc = "A fast-moving pest."
			color = color.ORANGE
			speed = 10 life = {1,6} ac = 10
			aaf = 20 sleep = 10
			exp = 1
			weight = 20
			blows =
			{
				{"BITE","PIERCE",{3,1}},
			}
			flags =
			{
				MORTAL=true AI = ai.SNAKE
				AI_STATES=getter.flags
				{
					MIN_RANGE=2
					BEST_RANGE=2
				}
				ALLOW_IN_FOREST=true ALLOW_IN_SWAMP=true
			}
		}
		[2] =
		{
			name = "giant brown bat"
			level = 6 rarity = 1
			desc = "It screeches as it attacks."
			color = color.UMBER
			speed = 20 life = {3,8} ac = 15
			aaf = 10 sleep = 30
			exp = 10
			weight = 600
			blows =
			{
				{"BITE","PIERCE",{3,3}},
			}
			flags =
			{
				MORTAL=true AI = ai.SNAKE
				ALLOW_IN_FOREST=true ALLOW_IN_MOUNTAIN=true
			}
		}
		[3] =
		{
			name = "giant tan bat"
			level = 10 rarity = 2
			desc = "A giant bat, the beating of whose wings produces a strangely unnerving noise."
			color = color.LIGHT_UMBER
			speed = 20 life = {4,8} ac = 30
			aaf = 12 sleep = 50
			exp = 18
			weight = 600
			blows =
			{
				{"BITE","TERRIFY",{6,6}},
				{"CLAW","SLASH",{6,4}},
				{"CLAW","SLASH",{6,4}},
			}
			flags =
			{
				FORCE_SLEEP=true MORTAL=true
				AI = ai.SNAKE
				AI_STATES=getter.flags
				{
					MIN_RANGE=2
					BEST_RANGE=3
				}
				ALLOW_IN_FOREST=true ALLOW_IN_MOUNTAIN=true
			}
		}
		[4] =
		{
			name = "mongbat"
			level = 13 rarity = 2
			desc = "Devil-bats, notoriously difficult to kill."
			color = color.LIGHT_UMBER
			speed = 0 life = {12,10} ac = 80
			aaf = 20 sleep = 8
			exp = 65
			weight = 800
			blows =
			{
				{"CLAW","CRUSH",{3,4}},
				{"CLAW","PIERCE",{3,4}},
				{"BITE","POISON",{3,8}},
			}
			flags =
			{
				RESIST=getter.resists{COLD=100 ELEC=100 DARK_FIRE=50 POIS=100}
				EVIL=true FRIENDS=getter.friends(100,20) FORCE_MAXHP=true REGENERATE=1500
				WEIRD_MIND=90 MORTAL=true
				MORTAL=true AI = ai.ZOMBIE
				AI_STATES=getter.flags
				{
					LURE_FROM_CORRIDOR=true
				}
			}
		}
		[5] =
		{
			name = "vampire bat"
			level = 24 rarity = 2
			desc = "A blood-sucking bat that flies at your neck hungrily."
			color = color.LIGHT_DARK
			speed = 10 life = {9,10} ac = 60
			aaf = 12 sleep = 50
			exp = 150
			weight = 50
			blows =
			{
				{"BITE","EXP_40",{6,4}},
				{"BITE","EXP_40",{6,4}},
			}
			flags =
			{
				RESIST=getter.resists{COLD=100 CONFUSION=50 DARK_FIRE=true FEAR=100 DARK_FIRE=50 POIS=100}
				COLD_BLOOD=true REGENERATE=0 EVIL=true
				UNDEAD=true NONLIVING=true NO_SLEEP=true NO_CUT=true
				KILLED_VERB="destroyed" SOUND_KILL=sound.N_KILL
				SEE_INVIS=30 FACTION=FACTION_UNDEAD
				MORTAL=true AI = ai.SNAKE
				AI_STATES=getter.flags{FEAR_LEVEL=0}
			}
			-- Undeads don't drop corpses
			default_flags_to_ignore = {DROP_CORPSE=true DROP_SKELETON=true}
		}
		[6] =
		{
			name = "bat of gorgoroth"
			level = 28 rarity = 3
			desc =
			{
				"Fed with horrid meats and grown to an enourmous size,",
				"this horror seeks livelier prey on which to feed.",
			}
			color = color.GREEN
			speed = 10 life = {22,12} ac = 70
			aaf = 16 sleep = 30
			exp = 1000
			weight = 150
			blows =
			{
				{"BITE","POISON",{3,12}},
				{"CLAW","SLASH",{6,4}},
				{"WAIL","FEAR",{6,4}},
			}
			flags =
			{
				RESIST=getter.resists{DARK=66 DARK_FIRE=50 POIS=66}
				WEIRD_MIND=90 BASH_DOOR=true EVIL=true
				AI_ANNOY=true DROP_CORPSE=true FACTION=FACTION_SAURON
				MORTAL=true AI = ai.SNAKE
				AI_STATES=getter.flags{LURE_FROM_CORRIDOR=true}
			}
		}
		[7] =
		{
			name = "bat of morgul"
			level = 32 rarity = 2
			desc =
			{
				"It is a fast moving creature of shadow, a gigantic black bat",
				"surrounded by flickering bright red flames.",
			}
			color = color.LIGHT_RED
			speed = 10 life = {24,14} ac = 75
			aaf = 16 sleep = 30
			exp = 250
			weight = 150
			blows =
			{
				{"BITE","DARK_FIRE",{5,12}},
				{"BITE","DARK_FIRE",{5,12}},
				{"BITE","DARK_FIRE",{5,12}},
			}
			default_flags_to_ignore =
			{
				ANIMAL=true
				SEE_INVIS=true
				FACTION=FACTION_ANIMAL
			}
			flags =
			{
				RESIST=getter.resists{DARK_FIRE=100 FIRE=100}
				WEIRD_MIND=90 BASH_DOOR=true
				AI_ANNOY=true LURE_FROM_CORRIDOR=true EVIL=true
				FACTION=FACTION_SAURON BLOW_RESPONSE=getter.array{[AURA_FIRE]={2,4}}
			}
		}
	}
	['B'] =
	{
		defaults =
		{
			body = default_body.bird
			flags =
			{
				ALLOW_IN_EVERYWHERE=true ANIMAL=true SEE_INVIS=15
				ANIMAL=true FACTION=FACTION_ANIMAL
				DROP_CORPSE=true DROP_SKELETON=true
			}
		}
		[1] =
		{
			name = "chaffinch"
			level = 0 rarity = 3
			desc = "Utterly harmless, except when angry."
			color = color.RED
			speed = 0 life = {1,1} ac = 60
			aaf = 30 sleep = 10
			exp = 0
			weight = 80
			blows =
			{
				{"BITE","PIERCE",{1,1}},
			}
			flags =
			{
				DROP_SKELETON=true HAS_EGG=true
				IMPRESED=true MORTAL=true AERIAL=true CAN_FLY=3
				MORTAL=true AI = ai.ZOMBIE
				AI_STATES=getter.flags
				{
					MIN_RANGE=2
					BEST_RANGE=4
				}
				ALLOW_IN_TOWN_BREE=true
			}
			default_flags_to_ignore =
			{
				ALLOW_IN_EVERYWHERE=true
   }
		}
		[2] =
		{
			name = "sparrow"
			level = 0 rarity = 3
			desc = "Utterly harmless, except when angry."
			color = color.LIGHT_UMBER
			speed = 0 life = {1,1} ac = 60
			aaf = 30 sleep = 10
			exp = 0
			weight = 90
			blows =
			{
				{"BITE","PIERCE",{1,1}},
			}
			flags =
			{
				AERIAL=true CAN_FLY=3
				DROP_SKELETON=true HAS_EGG=true IMPRESED=true
				MORTAL=true AI = ai.ZOMBIE
				AI_STATES=getter.flags
				{
					MIN_RANGE=2
					BEST_RANGE=4
				}
				ALLOW_IN_TOWN_BREE=true
			}
			default_flags_to_ignore =
			{
				ALLOW_IN_EVERYWHERE=true
			}				
		}
		[3] =
		{
			name = "crow"
			level = 2 rarity = 2
			desc = "It is a hooded crow, gray except for the black wings and head."
			color = color.SLATE
			speed = 0 life = {3,5} ac = 20
			aaf = 40 sleep = 0
			exp = 8
			weight = 300
			blows =
			{
				{"BITE","PIERCE",{3,3}},
				{"BITE","PIERCE",{3,3}},
			}
			flags =
			{
				DROP_CORPSE=true HAS_EGG=true MORTAL=true AERIAL=true CAN_FLY=3
				AI=ai.ZOMBIE FRIENDS=getter.friends(40,6) SPEED=getter.speeds{WALK=10}
				ALLOW_IN_FOREST=true
			}
		}
		[4] =
		{
			name = "raven"
			level = 4 rarity = 2
			desc = "Larger than a crow, and pitch black."
			color = color.LIGHT_DARK
			speed = 0 life = {4,5} ac = 12
			aaf = 40 sleep = 0
			exp = 8
			weight = 500
			blows =
			{
				{"BITE","PIERCE",{3,4}},
				{"BITE","PIERCE",{3,4}},
			}
			flags =
			{
				ANIMAL=true DROP_CORPSE=true
				MORTAL=true HAS_EGG=true AERIAL=true CAN_FLY=3 SPEED=getter.speeds{WALK=10}
				AI=ai.ZOMBIE FRIENDS=getter.friends(40,6)
				ALLOW_IN_TOWN_BREE=true
			}
		}
		[5] =
		{
			name = "hunting hawk"
			level = 8 rarity = 2
			desc = "Trained to hunt and kill without fear."
			color = color.UMBER
			speed = 5 life = {8,8} ac = 30
			aaf = 30 sleep = 10
			exp = 22
			weight = 800
			blows =
			{
				{"CLAW","PIERCE",{6,3}},
				{"CLAW","PIERCE",{6,3}},
				{"BITE","PIERCE",{6,4}},
			}
			flags =
			{
				RESIST=getter.resists{FEAR=100} AERIAL=true CAN_FLY=15 SPEED=getter.speeds{WALK=10}
				DROP_CORPSE=true HAS_EGG=true MORTAL=true
				AI=ai.ZOMBIE AI_STATES=getter.flags{FEAR_LEVEL=0}
				ALLOW_IN_FOREST=true
			}
		}
		[6] =
		{
			name = "eagle"
			level = 12 rarity = 2
			desc = "A magnificent huge predatory bird."
			color = color.UMBER
			speed = 10 life = {10,10} ac = 30
			aaf = 30 sleep = 10
			exp = 22
			weight = 600
			blows =
			{
				{"CLAW","PIERCE",{9,3}},
				{"CLAW","PIERCE",{9,3}},
				{"BITE","SLASH",{6,6}},
			}
			flags =
			{
				DROP_CORPSE=true HAS_EGG=true MORTAL=true AERIAL=true CAN_FLY=15 SPEED=getter.speeds{WALK=10}
				AI=ai.ZOMBIE ALLOW_IN_FOREST=true ALLOW_IN_MOUNTAIN=true ALLOW_IN_COLD=true
			}
			default_flags_to_ignore =
			{
				ALLOW_IN_EVERYWHERE=true
			}				
		}
		[7] =
		{
			name = "crebain"
			level = 16 rarity = 4
			desc =
			{
				"A type of crow, specially bred by the forces of evil as spies; their",
				"rudimentary intelligence guided by an evil mind has tracked you down,",
				"and now they seek to alert other evil creatures to your presence.",
			}
			color = color.LIGHT_DARK
			speed = 0 life = {3,5} ac = 12
			aaf = 40 sleep = 0
			exp = 20
			weight = 500
			blows =
			{
				{"CLAW","PIERCE",{6,4}},
				{"CLAW","PIERCE",{6,4}},
			}
			flags =
			{
				RESIST=getter.resists{DARK_FIRE=50} MORTAL=true
				EVIL=true FRIENDS=getter.friends(100,15) DROP_CORPSE=true HAS_EGG=true
				FACTION=FACTION_SAURON AERIAL=true CAN_FLY=3 SPEED=getter.speeds{WALK=10}
				AI=ai.SPELL_AND_MOVE
				AI_STATES=getter.flags
				{
					CASTING_AI=ai.SMART_CAST
					MOVE_AI=ai.SNAKE
					LURE_FROM_CORRIDOR=true
				}
				ALLOW_IN_FOREST=true ALLOW_IN_PLAIN=true ALLOW_IN_RIVER=true
				ALLOW_IN_OCEAN=true ALLOW_IN_MOUNTAIN=true ALLOW_IN_SWAMP=true
				ALLOW_IN_COLD=true
			}
			spells =
			{
				frequency = 8
				["Shriek"] = { level=50 chance=98 }
			}
		}
		[8] =
		{
			name = "great eagle"
			level = 20 rarity = 2
			desc =
			{
				"Greater and more intelligent than most of its kind, this great eagle is",
				"a messenger between the forces of good.",
			}
			color = color.UMBER
			speed = 20 life = {100,6} ac = 55
			aaf = 20 sleep = 20
			exp = 180
			weight = 1000
			blows =
			{
				{"CLAW","PIERCE",{6,15}},
				{"CLAW","PIERCE",{6,15}},
				{"BITE","SLASH",{9,5}},
			}
			flags =
			{
				GOOD=true DROP_CORPSE=true HAS_EGG=true AERIAL=true CAN_FLY=15
				FACTION=FACTION_PLAYER AI=ai.ZOMBIE
				ALLOW_IN_FOREST=true ALLOW_IN_MOUNTAIN=true ALLOW_IN_COLD=true
			}
			default_flags_to_ignore =
			{
				ALLOW_IN_EVERYWHERE=true
			}				
		}
		[9] =
		{
			name = "gwaihir the windlord"
			level = 24 rarity = 6
			desc =
			{
				"The greatest of eagles in the Third Age of Middle-earth, Gwaihir rescued",
				"Gandalf the Wizard from Orthanc, and has twice brought his flock to the",
				"aid of Sauron's enemies in battle - first outside the gates of Erebor in",
				"the Battle of Five Armies, and then before the Black Gate of Mordor itself.",
			}
			color = color.UMBER
			speed = 20 life = {85,12} ac = 60
			aaf = 20 sleep = 20
			exp = 400
			weight = 1200
			blows =
			{
				{"CLAW","PIERCE",{15,6}},
				{"CLAW","PIERCE",{15,6}},
				{"BITE","SLASH",{12,8}},
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=10 PIERCE=5 SLASH=10}
				UNIQUE=true FORCE_MAXHP=true DROP_CORPSE=true
				FACTION=FACTION_PLAYER AERIAL=true CAN_FLY=25 AI=ai.SPELL_AND_MOVE
				AI_STATES=getter.flags
				{
					CASTING_AI=ai.SMART_CAST
					MOVE_AI=ai.ZOMBIE
				}
				ALLOW_IN_PLAIN=true ALLOW_IN_FOREST=true
				ALLOW_IN_MOUNTAIN=true ALLOW_IN_COLD=true
			}
			default_flags_to_ignore =
			{
				ALLOW_IN_EVERYWHERE=true
			}				
			spells =
			{
				frequency = 10
				["Summon Kin"] = { level=80 chance=80 }
			}
		}
		[10] =
		{
			name = "meneldor the swift"
			level = 24 rarity = 6
			desc =
			{
				"Among all the eagles of Middle-earth he is the swiftest, and in his time",
				"has borne messages between all of the Wise.  It was Meneldor who bore the",
				"Ring-bearer away from the destruction of Mount Doom.",
			}
			color = color.UMBER
			speed = 30 life = {80,10} ac = 70
			aaf = 30 sleep = 20
			exp = 380
			weight = 1200
			blows =
			{
				{"CLAW","PIERCE",{10,9}},
				{"CLAW","PIERCE",{10,9}},
				{"BITE","SLASH",{12,9}},
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=10 PIERCE=5 SLASH=10}
				UNIQUE=true FORCE_MAXHP=true DROP_CORPSE=true
				FACTION=FACTION_PLAYER GOOD=true AERIAL=true CAN_FLY=25
				AI=ai.ZOMBIE
				ALLOW_IN_PLAIN=true ALLOW_IN_FOREST=true
				ALLOW_IN_MOUNTAIN=true ALLOW_IN_COLD=true
			}
			default_flags_to_ignore =
			{
				ALLOW_IN_EVERYWHERE=true
			}				
		}
		[11] =
		{
			name = "thorondor, lord of eagles"
			level = 30 rarity = 6
			desc =
			{
				"Among the mightiest of birds, Thorondor is the messenger of the Valar, and",
				"brings news of Middle-earth to Valinor itself.  Nothing that can be seen",
				"from the airs of the world is hidden from him.",
			}
			color = color.UMBER
			speed = 30 life = {85,12} ac = 75
			aaf = 20 sleep = 20
			exp = 600
			weight = 1600
			blows =
			{
				{"CLAW","PIERCE",{16,12}},
				{"CLAW","PIERCE",{16,12}},
				{"BITE","SLASH",{24,10}},
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=10 PIERCE=5 SLASH=10}
				UNIQUE=true FORCE_MAXHP=true DROP_CORPSE=true
				FACTION=FACTION_PLAYER GOOD=true SEE_INVIS=100 AERIAL=true CAN_FLY=25
				AI=ai.ZOMBIE
				ALLOW_IN_PLAIN=true ALLOW_IN_FOREST=true
				ALLOW_IN_MOUNTAIN=true ALLOW_IN_COLD=true
			}
			default_flags_to_ignore =
			{
				ALLOW_IN_EVERYWHERE=true
			}				
		}
		[12] =
		{
			name = "giant roc"
			level = 40 rarity = 3
			desc =
			{
				"A vast legendary bird, its iron talons rake the most impenetrable of",
				"surfaces and its screech echoes through the many winding dungeon corridors.",
			}
			color = color.UMBER
			speed = 0 life = {80,15} ac = 65
			aaf = 20 sleep = 10
			exp = 1000
			weight = 6000
			blows =
			{
				{"CLAW","SLASH",{20,12}},
				{"CLAW","SLASH",{20,12}},
				{"BITE","ELEC",{25,13}},
			}
			flags =
			{
				RESIST=getter.resists{ACID=-66 CRUSH=15 ELEC=100 PIERCE=10 SLASH=15} MORTAL=true
				BASH_DOOR=true DROP_CORPSE=true HAS_EGG=true AERIAL=true CAN_FLY=25 AI=ai.ZOMBIE
			}
		}
		[13] =
		{
			name = "steed of the nazgul"
			level = 48 rarity = 3
			desc =
			{
				"A terrifying sight:  a winged creature greater than any bird you have ever",
				"seen, and with no feathers on its horrid, black, leathery wings. Descended",
				"from a creature of an older world perhaps, bred by Sauron to be a winged",
				"steed for his Ringwraiths.",
			}
			color = color.LIGHT_DARK
			speed = 10 life = {25,80} ac = 80
			aaf = 30 sleep = 5
			exp = 4000
			weight = 4500
			blows =
			{
				{"CLAW","PIERCE",{12,8}},
				{"CLAW","CRUSH",{12,8}},
				{"BITE","EXP_40",{18,6}},
				{"BITE","EXP_40",{18,6}},
				{"WAIL","FEAR",{0,0}},
			}
			flags =
			{
				RESIST=getter.resists{COLD=100 CRUSH=15 DARK_FIRE=50 FEAR=100 PIERCE=10 POIS=100 SLASH=15}
				EVIL=true BASH_DOOR=true MORTAL=true AERIAL=true CAN_FLY=25
				FACTION=FACTION_SAURON AI=ai.SPELL_AND_MOVE
				DROP_THEME=getter.flags{THEME_COMBAT=60 THEME_MAGIC=40}
				AI_STATES=getter.flags
				{
					CASTING_AI=ai.SMART_CAST
					MOVE_AI=ai.ZOMBIE
				}
			}
			spells =
			{
				frequency = 6
				["Breathe Darkness"] = { level=100 chance=100 }
				["Breathe Darkfire"] = { level=100 chance=100 }
				["Breathe Sound"] = { level=100 chance=100 }
			}
		}
	}
	['c'] =
	{
		defaults =
		{
			body = default_body.insect
			flags =
			{
				ALLOW_IN_EVERYWHERE=true FACTION=FACTION_ANIMAL
				WEIRD_MIND=90 DROP_SKELETON=true
				ANIMAL=true MORTAL=true SEE_INVIS=15 COLD_BLOOD=true
			}
		}
		[1] =
		{
			name = "giant house centipede"
			level = 1 rarity = 1
			desc =
			{
				"It is about four feet long and poisonous. Fifteen",
				"pairs of legs undulate along its single-sectioned body.",
			}
			color = color.WHITE
			speed = 0 life = {3,6} ac = 8
			aaf = 7 sleep = 40
			exp = 3
			weight = 500
			blows =
			{
				{"BITE","PIERCE",{6,2}},
				{"STING","POISON",{3,2}},
			}
			flags =
			{
				RESIST=getter.resists{SLASH=10 PIERCE=10 POIS=66}
				BASH_DOOR=true
				AI=ai.SNAKE
			}
		}
		[2] =
		{
			name = "giant garden centipede"
			level = 2 rarity = 1
			desc =
			{
				"It is about four feet long and poisonous, with",
				"scores of legs along its sinuous, many-sectioned body!",
			}
			color = color.YELLOW
			speed = 0 life = {4,4} ac = 12
			aaf = 8 sleep = 30
			exp = 7
			weight = 500
			blows =
			{
				{"BITE","PIERCE",{6,3}},
				{"STING","POISON",{6,2}},
			}
			flags =
			{
				RESIST=getter.resists{SLASH=10 PIERCE=10 POIS=66}
				BASH_DOOR=true
				AI=ai.SNAKE
			}
		}
		[3] =
		{
			name = "giant forest centipede"
			level = 3 rarity = 1
			desc =
			{

				"It is about four feet long and carnivorous. Its many pairs of legs", 
				"move it at a frigtening speed!",
			}
			color = color.GREEN
			speed = 5 life = {4,6} ac = 16
			aaf = 5 sleep = 10
			exp = 9
			weight = 500
			blows =
			{
				{"BITE","PIERCE",{6,4}},
				{"STING","POISON",{6,2}},
			}
			flags =
			{
				RESIST=getter.resists{SLASH=10 PIERCE=10 POIS=66}
				BASH_DOOR=true AI=ai.SNAKE ALLOW_IN_FOREST=true SPEED=getter.speeds{WALK=10}
			}
		}
		[4] =
		{
			name = "giant red centipede"
			level = 8 rarity = 1
			desc =
			{
				"It is about eight feet long and carnivorous,",
				"with feathery antennae waving at the front",
				"of its sinuous body!",
			}
			color = color.RED
			speed = 00 life = {8,10} ac = 12
			aaf = 8 sleep = 20
			exp = 30
			weight = 800
			blows =
			{
				{"BITE","PIERCE",{5,12}},
				{"STING","POISON",{6,8}},
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=10 SLASH=15 PIERCE=15 POIS=66}
				BASH_DOOR=true SEE_INVIS=70 AI=ai.ZOMBIE SPEED=getter.speeds{WALK=10}
			}
		}
		[5] =
		{
			name = "stegocentipede"
			level = 14 rarity = 2
			desc = "It is a vast armoured centipede with massive mandibles and a spiked tail."
			color = color.UMBER
			speed = 10 life = {13,10} ac = 20
			aaf = 12 sleep = 30
			exp = 60
			weight = 1200
			blows =
			{
				{"BITE","PIERCE",{6,12}},
				{"BITE","PIERCE",{6,12}},
				{"STING","POISON",{6,12}},
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=12 SLASH=18 PIERCE=18 POIS=66}
				BASH_DOOR=true AI=ai.ZOMBIE
			}
		}
		[6] =
		{
			name = "giant clear centipede"
			level = 15 rarity = 2
			desc =
			{
				"It would be about four feet long and carnivorous,",
				"if you could only see it!",
			}
			color = color.LIGHT_BLUE
			speed = 0 life = {7,8} ac = 20
			aaf = 12 sleep = 30
			exp = 30
			weight = 400
			blows =
			{
				{"BITE","PIERCE",{6,9}},
				{"STING","POISON",{9,4}},
			}
			flags =
			{
				RESIST=getter.resists{SLASH=10 PIERCE=10 POIS=66}
				ATTR_CLEAR=true INVISIBLE=true WEIRD_MIND=90 BASH_DOOR=true
				AI=ai.SNAKE
			}
		}
		[7] =
		{
			name = "carrion crawler"
			level = 25 rarity = 2
			desc =
			{
				"A hideous centipede covered in slime and with glowing tentacles around its",
				"head.",
			}
			color = color.ORANGE
			speed = 0 life = {20,15} ac = 30
			aaf = 15 sleep = 10
			exp = 60
			weight = 700
			blows =
			{
				{"STING","PARALYZE",{6,6}},
				{"STING","PARALYZE",{6,6}},
			}
			flags =
			{
				RESIST=getter.resists{SLASH=10 PIERCE=10 POIS=100}
				BASH_DOOR=true FRIENDS=getter.friends(70,15)
				AI=ai.SNAKE
			}
		}
	}
	['C'] =
	{
		defaults =
		{
			body = default_body.quadruped
			flags =
			{
				ALLOW_IN_EVERYWHERE=true ANIMAL=true SEE_INVIS=15
				FACTION=FACTION_ANIMAL DROP_SKELETON=true DROP_CORPSE=true

			}
		}
		[1] =
		{
			name = "scruffy little dog"
			level = 0 rarity = 3
			desc = "A thin flea-ridden mutt, growling as you get close."
			color = color.LIGHT_UMBER
			speed = 0 life = {1,3} ac = 1
			aaf = 20 sleep = 5
			exp = 0
			weight = 300
			blows =
			{
				{"BITE","PIERCE",{3,1}},
			}
			flags =
			{
				MORTAL=true ALLOW_IN_TOWN_BREE=true AI=ai.ZOMBIE
			}
			default_flags_to_ignore =
			{
				ALLOW_IN_EVERYWHERE=true
			}				
		}
		[2] =
		{
			name = "jackal"
			level = 1 rarity = 1
			desc = "It is a yapping snarling dog, dangerous when in a pack."
			color = color.LIGHT_UMBER
			speed = 0 life = {3,4} ac = 6
			aaf = 10 sleep = 10
			exp = 2
			weight = 400
			blows =
			{
				{"BITE","PIERCE",{6,3}},
			}
			flags =
			{
				FRIENDS=getter.friends(100,10) LURE_FROM_CORRIDOR=true
				MORTAL=true AI=ai.ZOMBIE
				AI_STATES=getter.flags{LURE_FROM_CORRIDOR=true}
				ALLOW_IN_SWAMP=true
			}
		}
		[3] =
		{
			name = "fang, farmer maggot's dog"
			level = 2 rarity = 2
			desc =
			{
				"A rather vicious dog belonging to Farmer Maggot. It thinks you are",
				"stealing mushrooms.",
			}
			color = color.WHITE
			speed = 10 life = {8,6} ac = 20
			aaf = 30 sleep = 0
			exp = 30
			weight = 700
			blows =
			{
				{"BITE","PIERCE",{6,8}},
			}
			flags =
			{
				UNIQUE=true SPECIAL_GENE=true FORCE_MAXHP=true
				BASH_DOOR=true MORTAL=true AI=ai.SNAKE
			}
		}
		[4] =
		{
			name = "grip, farmer maggot's dog"
			level = 2 rarity = 2
			desc =
			{
				"A rather vicious dog belonging to Farmer Maggot. It thinks you are",
				"stealing mushrooms.",
			}
			color = color.WHITE
			speed = 10 life = {8,6} ac = 20
			aaf = 30 sleep = 0
			exp = 30
			weight = 600
			blows =
			{
				{"BITE","PIERCE",{6,8}},
			}
			flags =
			{
				UNIQUE=true SPECIAL_GENE=true FORCE_MAXHP=true
				BASH_DOOR=true MORTAL=true AI=ai.SNAKE
			}
		}
		[5] =
		{
			name = "wolf, farmer maggot's dog"
			level = 2 rarity = 2
			desc =
			{
				"A rather vicious dog belonging to Farmer Maggot. It thinks you are",
				"stealing mushrooms.",
			}
			color = color.WHITE
			speed = 10 life = {8,6} ac = 20
			aaf = 30 sleep = 0
			exp = 30
			weight = 650
			blows =
			{
				{"BITE","PIERCE",{6,8}},
			}
			flags =
			{
				UNIQUE=true SPECIAL_GENE=true FORCE_MAXHP=true
				BASH_DOOR=true MORTAL=true AI=ai.SNAKE
			}
		}
		[6] =
		{
			name = "wolf"
			level = 10 rarity = 1
			desc = "It howls and snaps at you."
			color = color.UMBER
			speed = 10 life = {10,4} ac = 25
			aaf = 30 sleep = 20
			exp = 30
			weight = 600
			blows =
			{
				{"BITE","PIERCE",{6,5}},
			}
			flags =
			{
				FRIENDS=getter.friends(95,10)
				BASH_DOOR=true MORTAL=true
				AI=ai.SNAKE AI_STATES=getter.flags{LURE_FROM_CORRIDOR=true}
				ALLOW_IN_FOREST=true ALLOW_IN_MOUNTAIN=true
				ALLOW_IN_COLD=true
			}
		}
		[7] =
		{
			name = "white wolf"
			level = 12 rarity = 1
			desc =
			{
				"A large and muscled wolf from the northern wastes. Its breath is cold and",
				"icy and its fur coated in frost.",
			}
			color = color.WHITE
			speed = 10 life = {8,7} ac = 25
			aaf = 30 sleep = 20
			exp = 35
			weight = 700
			blows =
			{
				{"BITE","PIERCE",{6,4}},
				{"BITE","PIERCE",{6,4}},
			}
			flags =
			{
				RESIST=getter.resists{COLD=66 FIRE=-66}
				FRIENDS=getter.friends(95,8) LURE_FROM_CORRIDOR=true
				BASH_DOOR=true MORTAL=true ALLOW_IN_COLD=true
				AI=ai.SNAKE AI_STATES=getter.flags{LURE_FROM_CORRIDOR=true}
			}
		}
		[8] =
		{
			name = "warg"
			level = 16 rarity = 2
			desc =
			{
				"It is a large wolf with eyes full of cunning.These",
				"have been bred to serve orcs, and to hunt men!",
			}
			color = color.LIGHT_DARK
			speed = 10 life = {9,8} ac = 30
			aaf = 20 sleep = 40
			exp = 48
			weight = 700
			blows =
			{
				{"BITE","PIERCE",{6,6}},
				{"BITE","PIERCE",{6,6}},
			}
			flags =
			{
				RESIST=getter.resists{DARK_FIRE=50}
				BASH_DOOR=true EVIL=true FACTION=FACTION_SAURON
				MORTAL=true FRIENDS=getter.friends(95,15) FACTION=FACTION_SAURON
				AI=ai.SNAKE AI_STATES=getter.flags{LURE_FROM_CORRIDOR=true}
				ALLOW_IN_FOREST=true ALLOW_IN_MOUNTAIN=true
			}
		}
		[9] =
		{
			name = "werewolf"
			level = 20 rarity = 2
			desc = "It is a huge wolf with eyes that glow with human intelligence."
			color = color.LIGHT_DARK
			speed = 0 life = {20,22} ac = 35
			aaf = 15 sleep = 70
			exp = 150
			weight = 900
			blows =
			{
				{"CLAW","SLASH",{6,5}},
				{"CLAW","SLASH",{6,5}},
				{"BITE","PIERCE",{6,10}},
			}
			flags =
			{
				RESIST=getter.resists{DARK_FIRE=50}
				TAKE_ITEM=true OPEN_DOOR=true
				EVIL=true MORTAL=true BASH_DOOR=true
				AI=ai.SNAKE AI_STATES=getter.flags{FEAR_LEVEL=20}
				FACTION=FACTION_SAURON
			}
		}
		[10] =
		{
			name = "wolf chieftain"
			level = 26 rarity = 5
			desc =
			{
				"A great wolf-chieftain whose pack is in the service of the Dark Lord,",
				"and whose howls strike fear into even the boldest heart.",
			}
			color = color.LIGHT_DARK
			speed = 10 life = {24,24} ac = 30
			aaf = 20 sleep = 5
			exp = 140
			weight = 1000
			blows =
			{
				{"CLAW","SLASH",{9,5}},
				{"CLAW","SLASH",{9,5}},
				{"BITE","PIERCE",{3,30}},
				{"WAIL","TERRIFY",{0,0}},
			}
			flags =
			{
				RESIST=getter.resists{ACID=100 COLD=100 FEAR=100 DARK_FIRE=50}
				FORCE_MAXHP=true SMART=true ESCORT=true ESCORTED_BY_GROUPS=true
				EVIL=true MORTAL=true OPEN_DOOR=true BASH_DOOR=true MALE=true
				FACTION=FACTION_SAURON
				AI=ai.SPELL_AND_MOVE
				MANA_CURR=50 MANA_MAX=50 MANA_REGENERATE=2500
				AI_STATES=getter.flags
				{
					CASTING_AI=ai.SMART_CAST
					MOVE_AI=ai.ZOMBIE
					LURE_FROM_CORRIDOR=true FEAR_LEVEL=20
				}
			}
			spells =
			{
				frequency = 8
				["Darkness"] = { level=80 chance=85 }
				["Howl"] = { level=50 chance=100}
			}
		}
		[11] =
		{
			name = "hellhound"
			level = 35 rarity = 3
			desc = "It is a giant dog that glows with heat. Flames pour from its nostrils."
			color = color.RED
			speed = 10 life = {50,10} ac = 60
			aaf = 25 sleep = 30
			exp = 600
			weight = 600
			blows =
			{
				{"BITE","FIRE",{10,10}},
				{"CLAW","SLASH",{12,5}},
				{"CLAW","SLASH",{12,5}},
			}
			flags =
			{
				RESIST=getter.resists{COLD=-66 FIRE=100 DARK_FIRE=50}
				FORCE_SLEEP=true FORCE_MAXHP=true
				BASH_DOOR=true MOVE_BODY=true EVIL=true HAS_LITE=true
				FRIENDS=getter.friends(80,10) LURE_FROM_CORRIDOR=true
				BLOW_RESPONSE=getter.array{[AURA_FIRE]={2,12}}
				AI=ai.SPELL_AND_MOVE
				AI_STATES=getter.flags
				{
					CASTING_AI=ai.SMART_CAST
					MOVE_AI=ai.ZOMBIE
					LURE_FROM_CORRIDOR=true FEAR_LEVEL=20
				}
				FACTION=FACTION_SAURON ALLOW_IN_LAVA=true
			}
			spells =
			{
				frequency = 5
				["Breathe Fire"] = { level=100 chance=100 }
			}
		}
		[12] =
		{
			name = "greater hellhound"
			level = 78 rarity = 2
			desc = "It is a giant dog that glows with heat. Flames pour from its nostrils."
			color = color.RED
			speed = 15 life = {50,30} ac = 70
			aaf = 25 sleep = 30
			exp = 650
			weight = 600
			blows =
			{
				{"BITE","FIRE",{21,11}},
				{"CLAW","SLASH",{12,6}},
				{"CLAW","SLASH",{12,6}},
			}
			flags =
			{
				RESIST=getter.resists{COLD=-66 FIRE=100 DARK_FIRE=50}
				FORCE_SLEEP=true FORCE_MAXHP=true
				BASH_DOOR=true MOVE_BODY=true
				EVIL=true HAS_LITE=true
				FRIENDS=getter.friends(80,10) LURE_FROM_CORRIDOR=true
				BLOW_RESPONSE=getter.array{[AURA_FIRE]={3,4}}
				AI=ai.SPELL_AND_MOVE
				AI_STATES=getter.flags
				{
					CASTING_AI=ai.SMART_CAST
					MOVE_AI=ai.ZOMBIE
					LURE_FROM_CORRIDOR=true
				}
				FACTION=FACTION_SAURON ALLOW_IN_LAVA=true
			}
			spells =
			{
				frequency = 5
				["Breathe Fire"] = { level=100 chance=100 }
			}
		}
		[13] =
		{
			name = "draugluin, sire of all werewolves"
			level = 83 rarity = 2
			desc =
			{
				"Draugluin provides Sauron with a fearsome personal guard. He is an",
				"enormous wolf inhabited by a human spirit. He is chief of all his kind.",
			}
			color = color.VIOLET
			speed = 20 life = {90,100} ac = 80
			aaf = 80 sleep = 90
			exp = 42000
			weight = 1000
			blows =
			{
				{"CLAW","SLASH",{13,14}},
				{"CLAW","SLASH",{13,14}},
				{"BITE","POISON",{16,16}},
				{"BITE","POISON",{16,16}},
			}
			flags =
			{
				RESIST=getter.resists{DARK_FIRE=50 POIS=100}
				UNIQUE=true MALE=true FORCE_SLEEP=true FORCE_MAXHP=true
				ONLY_ITEM=true ESCORT=true ESCORTED_BY_GROUPS=true
				DROP_NUMBER=getter.random(1,2) DROP_GOOD=true
				MOVE_BODY=true TAKE_ITEM=true OPEN_DOOR=true BASH_DOOR=true
				FACTION=FACTION_SAURON MANA_CURR=200 MANA_MAX=200 MANA_REGENERATE=6000
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags
				{
					CASTING_AI=ai.SMART_CAST
					MOVE_AI=ai.PATHFIND
				}
			}
			spells =
			{
				frequency = 3
				["Scare"] = { level=85 chance=99 }
				["Summon Hounds"] = { level=80 chance=99 }
				["Summon Monsters"] = { level=80 chance=90 }
				["Howl"] = { level=80 chance=99 }
			}
		}
		[14] =
		{
			name = "huan, wolfhound of the valar"
			level = 93 rarity = 2
			desc =
			{
				"The wolfhound of the Valar, Huan has served many masters in his time, from",
				"Celegorm son of Feanor to Beren son of Barahir: but now he runs wild and",
				"acknowledges no master save himself, as he hunts alone for his nemesis -",
				"Carcharoth, the terrible wolf of Angband.",
			}
			color = color.LIGHT_WHITE
			speed = 20 life = {90,100} ac = 90
			aaf = 50 sleep = 10
			exp = 42000
			weight = 3400
			blows =
			{
				{"CLAW","SLASH",{18,18}},
				{"CLAW","SLASH",{18,18}},
				{"BITE","COLD",{20,19}},
				{"BITE","COLD",{20,19}},
			}
			flags =
			{
				RESIST=getter.resists{ACID=100 COLD=100 ELEC=100}
				UNIQUE=true MALE=true FORCE_SLEEP=true
				FORCE_MAXHP=true ONLY_ITEM=true DROP_NUMBER=getter.random(4,8) DROP_GOOD=true
				SMART=true OPEN_DOOR=true BASH_DOOR=true KILL_BODY=true
				GOOD=true BLOW_RESPONSE=getter.array{[AURA_COLD]={8,24}}
				MANA_CURR=200 MANA_MAX=200 MANA_REGENERATE=6000
				AI = ai.SPELL_AND_MOVE AI_STATES=getter.flags
				{
					CASTING_AI=ai.SMART_CAST
					MOVE_AI=ai.PATHFIND
				}
			}
			spells =
			{
				frequency = 5
				["Breathe Cold"] = { level=100 chance=100 }
				["Breathe Lite"] = { level=100 chance=100 }
				["Breathe Sound"] = { level=100 chance=100 }
			}
		}
		[15] =
		{
			name = "carcharoth, the jaws of thirst"
			level = 94 rarity = 1
			desc =
			{
				"The first guard of Angband, Carcharoth, also known as 'The Red Maw', is",
				"the largest wolf to ever walk the earth. He is highly intelligent and a",
				"deadly opponent in combat.",
			}
			color = color.LIGHT_DARK
			speed = 20 life = {100,100} ac = 90
			aaf = 80 sleep = 10
			exp = 42000
			weight = 3400
			blows =
			{
				{"CLAW","SLASH",{20,19}},
				{"CLAW","SLASH",{20,19}},
				{"BITE","POISON",{22,21}},
				{"BITE","FIRE",{22,21}},
			}
			flags =
			{
				RESIST=getter.resists{COLD=-66 FIRE=100 DARK_FIRE=50 POIS=100}
				UNIQUE=true MALE=true DROP_CORPSE=true FORCE_SLEEP=true
				FORCE_MAXHP=true ONLY_ITEM=true DROP_NUMBER=getter.random(4,8) DROP_GOOD=true
				SMART=true OPEN_DOOR=true BASH_DOOR=true MOVE_BODY=true
				EVIL=true FACTION=FACTION_SAURON
				BLOW_RESPONSE=getter.array{[AURA_FIRE]={12,24}}
				MANA_CURR=400 MANA_MAX=400 MANA_REGENERATE=9000
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags
				{
					CASTING_AI=ai.SMART_CAST
					MOVE_AI=ai.PATHFIND
					FEAR_LEVEL=25
				}
			}
			spells =
			{
				frequency = 3
				["Breathe Darkness"] = { level=100 chance=96 }
				["Breathe Fire"] = { level=100 chance=100 }
				["Breathe Darkfire"] = { level=100 chance=100 }
				["Breathe Poison"] = { level=100 chance=100 }
				["Summon Hounds"] = { level=100 chance=99 }
			}
		}
	}
	['d'] =
	{
		defaults =
		{
			body = default_body.worms
			flags =
			{
				ALLOW_IN_EVERYWHERE=true COLD_BLOOD=true
				DRAGON=true SEE_INVIS=20 DROP_CORPSE=true DROP_SKELETON=true
			}
		}
		[1] =
		{
			name = "cold-drake worm"
			level = 24 rarity = 3
			desc =
			{
				"You thought dragons used eggs, but this worm has the scales, and the bad",
				"breath, and the fiery eyes, of a real Dragon. It is of the lesser breed",
				", unable to make fire.",
			}
			color = color.WHITE
			speed = -5 life = {10,15} ac = 30
			aaf = 10 sleep = 80
			exp = 40
			weight = 4500
			blows =
			{
				{"CLAW","SLASH",{4,9}},
				{"CLAW","SLASH",{4,9}},
				{"BITE","PIERCE",{5,15}},
			}
			flags =
			{
				RESIST=getter.resists{COLD=66 DARK_FIRE=50}
				DROP_60=true ONLY_GOLD=true
				OPEN_DOOR=true BASH_DOOR=true EVIL=true MORTAL=true FRIENDS=getter.friends(90,10)
				DROP_THEME=getter.flags{THEME_TREASURE=100}
				AI=ai.SNAKE ALLOW_IN_COLD=true
			}
		}
		[2] =
		{
			name = "long-worm infant"
			level = 26 rarity = 3
			desc =
			{
				"You thought dragons used eggs, but this worm has the scales, and the bad",
				"breath, and the fiery eyes, of a real Dragon. Of the long-worm breed, this",
				"long, thin beast twists restlessly."
			}
			color = color.LIGHT_BLUE
			speed = 5 life = {10,13} ac = 35
			aaf = 10 sleep = 80
			exp = 45
			weight = 4500
			blows =
			{
				{"CLAW","CRUSH",{4,9}},
				{"CLAW","SLASH",{4,9}},
				{"BITE","PIERCE",{5,15}},
			}
			flags =
			{
				RESIST=getter.resists{DARK_FIRE=50}
				DROP_60=true ONLY_GOLD=true
				OPEN_DOOR=true BASH_DOOR=true EVIL=true MORTAL=true
				FRIENDS=getter.friends(90,12) AI=ai.SNAKE
				DROP_THEME=getter.flags{THEME_TREASURE=100}
			}
		}
		[3] =
		{
			name = "uruloki worm"
			level = 20 rarity = 3
			desc =
			{
				"You thought dragons used eggs, but this worm has the scales, and the bad",
				"breath, and the fiery eyes, of a real Dragon. The Urloki are born with",
				"fire in their blood, and you see flames flickering from its mouth."
			}
			color = color.RED
			speed = 0 life = {10,15} ac = 30
			aaf = 10 sleep = 80
			exp = 50
			weight = 4500
			blows =
			{
				{"CLAW","CRUSH",{9,5}},
				{"CLAW","SLASH",{9,5}},
				{"BITE","PIERCE",{5,15}},
			}
			flags =
			{
				RESIST=getter.resists{FIRE=66 DARK_FIRE=50}
				DROP_60=true ONLY_GOLD=true
				OPEN_DOOR=true BASH_DOOR=true EVIL=true MORTAL=true
				FRIENDS=getter.friends(90,8) AI=ai.SPELL_AND_MOVE
				DROP_THEME=getter.flags{THEME_TREASURE=100}
				AI_STATES=getter.flags
				{
					CASTING_AI=ai.SMART_CAST
					MOVE_AI=ai.SNAKE
				}
				ALLOW_IN_MOUNTAIN=true ALLOW_IN_LAVA=true
			}
			default_flags_to_ignore =
			{
				COLD_BLOOD=true
			}
			spells =
			{
				frequency = 6
				["Breathe fire"] = { level=100 chance=100 }
			}
		}
		[4] =
		{
			name = "wereworm hatchling"
			level = 20 rarity = 3
			desc =
			{
				"You thought dragons used eggs, but this worm has the scales, and the bad",
				"breath, and the fiery eyes, of a real Dragon. This wingless, legless worm",
				"is lying half-buried in the earth, its spadelike head glistening with teeth!",
			}
			color = color.UMBER
			speed = 0 life = {12,15} ac = 25
			aaf = 80 sleep = 20
			exp = 45
			weight = 5000
			blows =
			{
				{"BITE","PIERCE",{10,10}},
				{"SPIT","ACID",{8,8}},
			}
			flags =
			{
				RESIST=getter.resists{ACID=66 DARK_FIRE=50 POIS=100 SLASH=10}
				BASH_DOOR=true EVIL=true CAN_SWIM=true
				MORTAL=true AI=ai.ZOMBIE
				DROP_THEME=getter.flags{THEME_TREASURE=100}
				ALLOW_IN_SWAMP=true
			}
		}	
	}
	['E'] =
	{
		defaults =
		{
			flags=
			{
				ALLOW_IN_EVERYWHERE=true
				NONLIVING=true NO_SLEEP=true NO_CUT=true EMPTY_MIND=true COLD_BLOOD=true
				FACTION=FACTION_DUNGEON NO_STUN=true
				DROP_CORPSE=false DROP_SKELETON=false MORTAL=false
			}		
		}	
		[1] =
		{
			name = "fire elemental"
			level = 50 rarity = 3
			desc = 
			{
				"This roaring column of fire is not only sentient, it is hostile to boot!",
			}
			color = color.RED
			speed = 10 life = {20,10} ac = 60
			aaf = 30 sleep = 0
			exp = 500 body = default_body.blob
			weight = 0
			blows =
			{
				{"HIT","FIRE",{8,8}},
				{"HIT","FIRE",{8,8}},
				{"HIT","FIRE",{8,8}},
			}
			flags =
			{
				RESIST=getter.resists{COLD=-100 POIS=100 FIRE=200}
				AI=ai.SPELL_AND_MOVE ATTR_MULTI=true AERIAL=true CAN_FLY=3
				BLOW_RESPONSE=getter.array{[AURA_FIRE]={6,4}}
				AI_STATES=getter.flags
				{
					MOVE_AI=ai.SNAKE
					CASTING_AI=ai.SMART_CAST
					FEAR_LEVEL=0
				}
				PASS_WALL=getter.flags{PASS_GASEOUS=1}
				ALLOW_IN_LAVA=true
			}
			default_flags_to_ignore =
			{
				COLD_BLOOD=true
			}
			spells =
			{
				["Breathe Fire"] = {level = 100 chance = 100}
			}
		}
		[2] =
		{
			name = "air elemental"
			level = 50 rarity = 3
			desc = 
			{
				"This twisting column of air is alive, and hungry!",
			}
			color = color.LIGHT_BLUE
			speed = 20 life = {20,10} ac = 90
			aaf = 30 sleep = 0
			exp = 500 body = default_body.blob
			weight = 0
			blows =
			{
				{"ENGULF","CONFUSE",{4,4}},
				{"ENGULF","CRUSH",{6,6}},
				{"ENGULF","ELEC",{8,8}},
			}
			flags =
			{
				RESIST=getter.resists{ELEC=100 POIS=100}
				AI=ai.SPELL_AND_MOVE AERIAL=true CAN_FLY=25 ATTR_MULTI=true
				AI_STATES=getter.flags
				{
					MOVE_AI=ai.SNAKE
					CASTING_AI=ai.SMART_CAST
					FEAR_LEVEL=0
				}
				PASS_WALL=getter.flags{PASS_GASEOUS=8}
			}
			spells =
			{
				["Bolt of Electricity"] = {level = 50 chance = 80}
			}
		}
		[3] =
		{
			name = "earth elemental"
			level = 50 rarity = 3
			desc = 
			{
				"This lumpy figure has emerged fully formed from the earth, hostile to",
				"soft things."
			}
			color = color.CHOCOLATE
			speed = 0 life = {30,10} ac = 30
			aaf = 30 sleep = 0
			exp = 500 body = default_body.humanoid
			weight = 0
			blows =
			{
				{"PUNCH","CRUSH",{10,10}},
				{"PUNCH_OFFHAND","CRUSH",{10,10}},
			}
			flags =
			{
				RESIST=getter.resists{ACID=-66 COLD=80 CRUSH=40 ELEC=80 FIRE=80 PIERCE=40 POIS=100 SLASH=40}
				AI=ai.ZOMBIE REFLECTING=60
				PASS_WALL=getter.flags{PASS_STONE=100}
			}
		}
		[4] =
		{
			name = "water elemental"
			level = 50 rarity = 3
			desc = 
			{
				"A sinuous, curving whirlpool of icy water, suspended in the air forming",
				"and reforming as you watch.",
			}
			color = color.AQUAMARINE
			speed = 10 life = {20,10} ac = 70
			aaf = 30 sleep = 0
			exp = 500
			weight = 0
			blows =
			{
				{"ENGULF","COLD",{8,8}},
				{"ENGULF","COLD",{8,8}},
			}
			flags =
			{
				RESIST=getter.resists{ACID=60 COLD=100 FIRE=-100 POIS=100 WATER=200}
				AI=ai.SPELL_AND_MOVE ATTR_MULTI=true AQUATIC=true REGENERATE=5000 
				MANA_CURR=50 MANA_MAX=50 MANA_REGENERATE=250
				AI_STATES=getter.flags {
					CASTING_AI=ai.SMART_CAST
					MOVE_AI=ai.SNAKE
				}
				PASS_WALL=getter.flags{PASS_LIQUID=8}
				ALLOW_IN_SWAMP=true ALLOW_IN_RIVER=true ALLOW_IN_OCEAN=true
			}
			spells=
			{
				["Ice Storm"] = {level=70 chance=85}
			}
		}
	}
	['f'] =
	{
		defaults =
		{
			body = default_body.quadruped
			flags =
			{
				ALLOW_IN_EVERYWHERE=true DROP_CORPSE=true DROP_SKELETON=true
				ANIMAL=true SEE_INVIS=15
			}
		}
		[1] =
		{
			name = "scrawny cat"
			level = 0 rarity = 3
			desc = "A skinny little furball with sharp claws and a menacing look."
			color = color.LIGHT_UMBER
			speed = 0 life = {1,2} ac = 1
			aaf = 30 sleep = 10
			exp = 0
			weight = 100
			blows =
			{
				{"CLAW","SLASH",{3,1}},
			}
			flags =
			{
				MORTAL=true AI=ai.SNAKE
				FACTION=FACTION_ANIMAL FRIENDS=getter.friends(30,4)
				AI_STATES=getter.flags
				{
					BEST_RANGE=2
					MIN_RANGE=1
				}
				ALLOW_IN_TOWN_BREE=true
			}
			default_flags_to_ignore =
			{
				ALLOW_IN_EVERYWHERE=true
			}				
		}
		[2] =
		{
			name = "wild cat"
			level = 2 rarity = 2
			desc =
			{
				"A larger than normal feline, hissing loudly. Its velvet claws conceal a",
				"fistful of needles.",
			}
			color = color.LIGHT_UMBER
			speed = 5 life = {3,5} ac = 12
			aaf = 40 sleep = 0
			exp = 8
			weight = 200
			blows =
			{
				{"CLAW","SLASH",{1,3}},
				{"BITE","PIERCE",{3,4}},
			}
			flags =
			{
				BASH_DOOR=true MORTAL=true FACTION=FACTION_ANIMAL
				AI=ai.SNAKE	ALLOW_IN_PLAIN=true
			}
		}
		[3] =
		{
			name = "panther"
			level = 10 rarity = 2
			desc =
			{
				"A large black cat, stalking you with intent. It thinks you're its next",
				"meal.",
			}
			color = color.LIGHT_DARK
			speed = 10 life = {10,9} ac = 25
			aaf = 40 sleep = 0
			exp = 40
			weight = 1300
			blows =
			{
				{"CLAW","SLASH",{6,6}},
				{"CLAW","SLASH",{6,6}},
				{"BITE","PIERCE",{9,8}},
			}
			flags =
			{
				BASH_DOOR=true MORTAL=true FACTION=FACTION_ANIMAL
				AI=ai.ZOMBIE
				ALLOW_IN_PLAIN=true ALLOW_IN_FOREST=true
			}
		}
		[4] =
		{
			name = "tiger"
			level = 12 rarity = 2
			desc =
			{
				"One of the largest of its species, a sleek orange and black shape creeps",
				"towards you, ready to pounce.",
			}
			color = color.ORANGE
			speed = 10 life = {14,12} ac = 35
			aaf = 40 sleep = 0
			exp = 50
			weight = 1500
			blows =
			{
				{"CLAW","SLASH",{9,7}},
				{"CLAW","SLASH",{9,7}},
				{"BITE","PIERCE",{15,6}},
			}
			flags =
			{
				BASH_DOOR=true FACTION=FACTION_ANIMAL MORTAL=true
				AI=ai.ZOMBIE AI_STATES=getter.flags{FEAR_LEVEL=10}
				ALLOW_IN_PLAIN=true ALLOW_IN_FOREST=true
				ALLOW_IN_MOUNTAIN=true
			}
		}
		[5] =
		{
			name = "sabre-tooth cat"
			level = 20 rarity = 2
			desc =
			{
				"A fierce and dangerous cat, its huge tusks and sharp claws would lacerate",
				"even the strongest armour.",
			}
			color = color.YELLOW
			speed = 10 life = {20,18} ac = 35
			aaf = 40 sleep = 0
			exp = 180
			weight = 1800
			blows =
			{
				{"CLAW","SLASH",{6,12}},
				{"CLAW","SLASH",{6,12}},
				{"BITE","SLASH",{15,10}},
				{"BITE","SLASH",{15,10}},
			}
			flags =
			{
				BASH_DOOR=true FACTION=FACTION_ANIMAL MORTAL=true
				AI=ai.ZOMBIE
				ALLOW_IN_PLAIN=true ALLOW_IN_FOREST=true
			}
		}
		[6] =
		{
			name = "tevildo, prince of cats"
			level = 66 rarity = 3
			desc =
			{
				"A huge black cat, with eyes gleaming both red and green.",
				"A gold collar gleams at its throat, and it hisses evilly at you!",
			}
			color = color.LIGHT_DARK
			speed = 20 life = {52,100} ac = 150
			aaf = 100 sleep = 0
			exp = 30000
			weight = 0
			blows =
			{
				{"CLAW","CONFUSE",{18,17}},
				{"CLAW","LOSE_DEX",{18,17}},
				{"CLAW","BLIND",{18,17}},
				{"BITE","PARALYZE",{25,25}},
			}
			default_flags_to_ignore =
			{
				ANIMAL=true
			}
			flags =
			{
				RESIST=getter.resists{COLD=100 CONFUSION=100 DARK_FIRE=50 FIRE=100 POIS=100}
				UNIQUE=true MALE=true FORCE_SLEEP=true FORCE_MAXHP=true
				EVIL=true ONLY_ITEM=true DROP_NUMBER=getter.random(4,8)
				DROP_GOOD=true INVISIBLE=true
				ESCORT=true ESCORTED_BY_GROUPS=true
				OPEN_DOOR=true BASH_DOOR=true FACTION=FACTION_SAURON SMART=true
				DROP_THEME=getter.flags{THEME_DEVICE=80 THEME_TREASURE=20}
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags
				{
					MOVE_AI=ai.ZOMBIE
					CASTING_AI=ai.SMART_CAST
					FEAR_LEVEL=30
				}
			}
			spells =
			{
				frequency = 3
				["Summon Kin"] = { level=80 chance=99 }
				["Teleport To"] = { level=80 chance=90 }
			}
		}
	}
	['F'] =
	{
		defaults =
		{
			body = default_body.bird
			flags =
			{
				ALLOW_IN_EVERYWHERE=true COLD_BLOOD=true
				ANIMAL=true SEE_INVIS=15
			}
		}
		[1] =
		{
			name = "giant dragon fly"
			level = 14 rarity = 2
			desc = "It is a large dragonfly, buzzing anxiously."
			color = color.WHITE
			speed = 20 life = {3,8} ac = 25
			aaf = 12 sleep = 50
			exp = 60
			weight = 150
			blows =
			{
				{"BITE","PIERCE",{6,4}},
			}
			flags =
			{
				FORCE_SLEEP=true AERIAL=true CAN_FLY=3
				WEIRD_MIND=90 MORTAL=true FACTION=FACTION_ANIMAL
				AI=ai.SNAKE
			}
		}
		[2] =
		{
			name = "giant firefly"
			level = 24 rarity = 4
			desc =
			{
				"While pretty from a distance, up close, these winged",
				"insects are blinding!",
			}
			color = color.WHITE
			speed = 10 life = {3,2} ac = 20
			aaf = 10 sleep = 50
			exp = 5
			weight = 100
			blows =
			{
				{"BITE","BLIND",{3,4}},
			}
			flags =
			{
				HAS_LITE=1 MULTIPLY=true
				ATTR_MULTI=1
				AI=ai.RANDOM_MOVE
			}
		}
	}
	['g'] =
	{
		defaults =
		{
			body = default_body.humanoid
			flags =
			{
				ALLOW_IN_EVERYWHERE=true NONLIVING=true REGENERATE=0
				FACTION=FACTION_DUNGEON COLD_BLOOD=true SEE_INVIS=80
			}
		}
		[1] =
		{
			name = "flesh golem"
			level = 14 rarity = 2
			desc = "A shambling humanoid monster with long scars."
			color = color.LIGHT_RED
			speed = 0 life = {16,10} ac = 15
			aaf = 12 sleep = 10
			exp = 50
			weight = 3000
			blows =
			{
				{"HIT","CRUSH",{9,4}},
				{"HIT","CRUSH",{9,4}},
			}
			flags =
			{
				RESIST=getter.resists{COLD=100 CONFUSION=50 ELEC=100 FEAR=100}
				EMPTY_MIND=true BASH_DOOR=true CAN_SWIM=true NO_SLEEP=true
				MORTAL=true NO_CUT=true
				KILLED_VERB="destroyed" SOUND_KILL=sound.N_KILL
				AI=ai.ZOMBIE AI_STATES=getter.flags{FEAR_LEVEL=0}
			}
		}
		[2] =
		{
			name = "clay golem"
			level = 15 rarity = 2
			desc =
			{
			"It is a massive statue made out of hard-baked clay. At first you",
			"think it it lifeless, but then you see a dully glowing glyph on",
			"its forehead, and see the clenching and unclenching of its crude",
			"fists.",
			}
			color = color.LIGHT_UMBER
			speed = 0 life = {16,10} ac = 20
			aaf = 12 sleep = 10
			exp = 60
			weight = 3200
			blows =
			{
				{"PUNCH","CRUSH",{6,8}},
				{"PUNCH_OFFHAND","CRUSH",{6,8}},
			}
			flags =
			{
				RESIST=getter.resists{ACID=100 COLD=100 CONFUSION=50 FEAR=100 FIRE=100 POIS=100 PIERCE=10 SLASH=10}
				EMPTY_MIND=true COLD_BLOOD=true BASH_DOOR=true NO_SLEEP=true
				AI=ai.ZOMBIE AI_STATES=getter.flags{FEAR_LEVEL=0}
				NO_CUT=true
			}
		}
		[3] =
		{
			name = "aquatic golem"
			level = 19 rarity = 1
			desc =
			{
				"An ingenious gnomish invention -- a golem designed for usage deep",
				"underwater. A glass dome covers its head, through which you see a rune",
				"glowing balefully , metal pincers grasping at nothing at the end of its",
				"three many-jointed arms.",
			}
			color = color.BLUE
			speed = -10 life = {25,20} ac = 15
			aaf = 35 sleep = 10
			exp = 130
			weight = 0
			blows =
			{
				{"CLAW","PIERCE",{12,12}},
				{"CLAW","PIERCE",{12,12}},
				{"CLAW","PIERCE",{12,12}},
			}
			flags =
			{
				RESIST=getter.resists{ACID=33 COLD=100 CONFUSION=50 CRUSH=20 ELEC=100 FEAR=100 FIRE=100 PIERCE=15 POIS=100 SLASH=18 SOUND=-66}
				COLD_BLOOD=true EMPTY_MIND=true AQUATIC=true NO_SLEEP=true NO_CUT=true
				AI=ai.ZOMBIE AI_STATES=getter.flags{FEAR_LEVEL=0}
				DROP_THEME=getter.flags{THEME_JUNK=100}
			}
		}
		[4] =
		{
			name = "stone golem"
			level = 19 rarity = 2
			desc =
			{
				"It is a massive statue carved from granite, centuries of corrosion",
				"pocking its surface. But looking recently carved is the rune on its",
				"forehead, light spilling out of it."
			}
			color = color.LIGHT_WHITE
			speed = -5 life = {28,15} ac = 15
			aaf = 12 sleep = 10
			exp = 120
			weight = 3500
			blows =
			{
				{"PUNCH","CRUSH",{15,12}},
				{"PUNCH_OFFHAND","CRUSH",{15,12}},
			}
			flags =
			{
				RESIST=getter.resists{COLD=100 CRUSH=20 CONFUSION=50 ELEC=100 FEAR=100 FIRE=100 PIERCE=30 POIS=100 SLASH=30}
				COLD_BLOOD=true EMPTY_MIND=true BASH_DOOR=true NO_SLEEP=true
				NONLIVING=true NO_CUT=true REFLECTING=30
				AI=ai.ZOMBIE AI_STATES=getter.flags{FEAR_LEVEL=0}
			}
		}
		[5] =
		{
			name = "iron golem"
			level = 22 rarity = 2
			desc = "It is a massive statue of cast iron that clanks steadily towards you."
			color = color.SLATE
			speed = 0 life = {80,20} ac = 25
			aaf = 12 sleep = 10
			exp = 200
			weight = 3800
			blows =
			{
				{"HIT","CRUSH",{30,12}},
			}
			flags =
			{
				RESIST=getter.resists{ACID=-66 COLD=100 CONFUSION=50 CRUSH=35 ELEC=100 FEAR=100 FIRE=100 PIERCE=35 POIS=100 SLASH=35}
				FORCE_SLEEP=true EMPTY_MIND=true COLD_BLOOD=true BASH_DOOR=true
				NO_SLEEP=true NONLIVING=true NO_CUT=true REFLECTING=20
				MANA_CURR=50 MANA_MAX=50 MANA_REGENERATE=250
				DROP_THEME=getter.flags{THEME_JUNK=100}
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags
				{
					MOVE_AI=ai.ZOMBIE
					CASTING_AI=ai.SMART_CAST
					FEAR_LEVEL=0
				}
			}
			spells =
			{
				frequency = 7
				["Slow Monster"] = { level=30 chance=89 }
			}
		}
		[6] =
		{
			name = "steel golem"
			level = 32 rarity = 2
			desc =
			{
				"It is a massive, gleaming statue that stalks towards you on well-oiled",
				"legs. You feel it could break into a run at any moment!",
			}
			color = color.STEEL_BLUE
			speed = 5 life = {80,30} ac = 35
			aaf = 12 sleep = 10
			exp = 350
			weight = 3800
			blows =
			{
				{"HIT","CRUSH",{18,18}},
				{"HIT","CRUSH",{18,18}},
			}
			flags =
			{
				RESIST=getter.resists{ACID=-66 COLD=100 CONFUSION=50 CRUSH=40 ELEC=100 FEAR=100 FIRE=100 PIERCE=40 POIS=100 SLASH=40}
				FORCE_SLEEP=true EMPTY_MIND=true COLD_BLOOD=true BASH_DOOR=true
				NO_SLEEP=true NONLIVING=true NO_CUT=true REFLECTING=60
				MANA_CURR=70 MANA_MAX=70 MANA_REGENERATE=1500
				DROP_THEME=getter.flags{THEME_JUNK=100}
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags
				{
					FEAR_LEVEL=0
					MOVE_AI=ai.ZOMBIE
					CASTING_AI=ai.SMART_CAST
				}
			}
			spells =
			{
				frequency = 7
				["Essence of Speed"] = { level=52 chance=89}
			}
		}
		[7] =
		{
			name = "pukelman"
			level = 25 rarity = 3
			desc = "A stumpy figure carved from stone, with glittering eyes."
			color = color.LIGHT_DARK
			speed = 0 life = {80,15} ac = 30
			aaf = 12 sleep = 10
			exp = 600
			weight = 10000
			blows =
			{
				{"HIT","CRUSH",{9,12}},
				{"HIT","CRUSH",{9,12}},
			}
			flags =
			{
				RESIST=getter.resists{COLD=100 CONFUSION=50 CRUSH=20 ELEC=100 FEAR=100 FIRE=100 PIERCE=25 POIS=100 SLASH=25}
				FORCE_SLEEP=true EMPTY_MIND=true COLD_BLOOD=true BASH_DOOR=true
				NO_SLEEP=true NO_CUT=true
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags
				{
					FEAR_LEVEL=0
					MOVE_AI=ai.ZOMBIE
					CASTING_AI=ai.SMART_CAST
				}
			}
			spells =
			{
				frequency = 4
				["Bolt of Acid"] = { level=16 chance=83 }
				["Summon Kin"] = { level=60 chance=91 }
			}
		}
		[7] =
		{
			name = "mithril golem"
			level = 34 rarity = 4
			desc =
			{
				"It is a massive statue of purest mithril. It has been fashioned with",
				"ridiculous intricacy and beauty, its form a picture of perfection.",
				"It turns to you and hisses, bringing up fists that reflect like mirrors.",
			}
			color = color.LIGHT_BLUE
			speed = 10 life = {80,15} ac = 50
			aaf = 12 sleep = 0
			exp = 850
			weight = 10000
			blows =
			{
				{"HIT","CRUSH",{18,8}},
				{"HIT","CRUSH",{18,8}},
				{"HIT","CRUSH",{18,8}},
				{"HIT","CRUSH",{18,8}},
			}
			flags =
			{
				RESIST=getter.resists{COLD=100 CONFUSION=50 CRUSH=50 ELEC=100 FEAR=100 FIRE=100 PIERCE=50 POIS=100 SLASH=50}
				ONLY_GOLD=true DROP_NUMBER=getter.random(5,5) EMPTY_MIND=true COLD_BLOOD=true BASH_DOOR=true
				REFLECTING=80 NO_SLEEP=true NONLIVING=true NO_CUT=true REFLECTING=80
				AI=ai.ZOMBIE AI_STATES=getter.flags{FEAR_LEVEL=0}
			}
		}
		[8] =
		{
			name = "silent watcher"
			level = 35 rarity = 1
			desc =
			{
				"A figure carved from stone, with three vulture faces whose eyes glow",
				"with a malevolent light. None escape their dreadful vigilance.",
			}
			color = color.SLATE
			speed = 0 life = {80,25} ac = 10
			aaf = 60 sleep = 0
			exp = 700
			weight = 4000
			flags =
			{
				RESIST=getter.resists{ACID=-66 COLD=100 CONFUSION=50 CRUSH=30 DARK=22 ELEC=100 FEAR=100 FIRE=100 LITE=-100 DARK_FIRE=50 PIERCE=30 POIS=100 SLASH=30}
				EMPTY_MIND=true COLD_BLOOD=true NONLIVING=true NEVER_MOVE=true
				COLD_BLOOD=true NO_SLEEP=true NO_STUN=true NONLIVING=true
				RES_TELE=true NO_CUT=true MANA_CURR=200 MANA_MAX=200 MANA_REGENERATE=2500
				AI=ai.SMART_CAST
			}
			spells =
			{
				frequency = 3
				["Scare"] = { level=80 chance=86 }
				["Confuse"] = { level=74 chance=86 }
				["Drain Mana"] = { level=80 chance=89 }
				["Hold"] = { level=70 chance=90 }
				["Shriek"] = { level=50 chance=85 }
				["Summon Monster"] = { level=80 chance=90 }
				["Summon Monsters"] = { level=80 chance=91 }
			}
		}
		[9] =
		{
			name = "colossus"
			level = 36 rarity = 4
			desc =
			{
				"A huge statue of an ancient king, torn from the mountainside",
				"by terrible magicks and animated with evil purpose. Its long",
				"strides bring it closer and closer towards you, as it swings",
				"its slow fists with earth-shattering power.",
			}
			color = color.LIGHT_GREEN
			speed = 0 life = {25,100} ac = 50
			aaf = 15 sleep = 10
			exp = 900
			weight = 35000
			blows =
			{
				{"PUNCH","CRUSH",{20,15}},
				{"PUNCH_OFFHAND","CRUSH",{20,15}},
				{"KICK","CRUSH",{20,15}},
				{"KICK","CRUSH",{20,15}},
			}
			flags =
			{
				RESIST=getter.resists{ACID=-66 COLD=100 CONFUSION=50 CRUSH=30 ELEC=100 FEAR=100 FIRE=100 PIERCE=30 POIS=100 SLASH=30}
				FORCE_MAXHP=true EMPTY_MIND=true COLD_BLOOD=true BASH_DOOR=true NONLIVING=true REFLECTING=true
				NO_SLEEP=true MORTAL=true NO_CUT=true REFLECTING=80
				AI=ai.SPELL_AND_MOVE LIMIT_SPELLS = getter.limit_spells{Arrow = 30}
				AI_STATES=getter.flags
				{
					MOVE_AI=ai.ZOMBIE
					CASTING_AI=ai.SMART_CAST
					FEAR_LEVEL=0
				}
			}
			spells =
			{
				frequency = 8
				["Arrow"] = { level=18 chance=100 }
			}
		}
		[10] =
		{
			name = "bone golem"
			level = 61 rarity = 3
			desc =
			{
				"A skeletal form, black as night, constructed from the bones of its",
				"previous victims. A thousand rib-bones make up its chest and dozens",
				"of skulls stare sightlessly at you, save for the great horned one atop",
				"its spine.",
			}
			color = color.WHITE
			speed = 10 life = {38,100} ac = 80
			aaf = 20 sleep = 50
			exp = 21000
			weight = 5000
			blows =
			{
				{"HIT","DARK_FIRE",{14,14}},
				{"HIT","DARK_FIRE",{14,14}},
				{"CLAW","LOSE_STR",{11,10}},
				{"CLAW","LOSE_STR",{11,10}},
			}
			flags =
			{
				RESIST=getter.resists{COLD=100 CRUSH=-33 CONFUSION=50 ELEC=100 FEAR=100 FIRE=100 DARK_FIRE=100 POIS=100}
				FORCE_SLEEP=true FORCE_MAXHP=true EMPTY_MIND=true COLD_BLOOD=true OPEN_DOOR=true
				BASH_DOOR=true EVIL=true UNDEAD=true RES_TELE=true NO_SLEEP=true
				NONLIVING=true NO_CUT=true SEE_INVIS=80 FACTION=FACTION_SAURON
				DROP_THEME=getter.flags{THEME_MAGIC=80 THEME_JUNK=20}
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags
				{
					MOVE_AI=ai.ZOMBIE
					CASTING_AI=ai.SMART_CAST
					FEAR_LEVEL=10
				}
			}
			spells =
			{
				frequency = 3
				["Ball of Darkfire"] = { level=80 chance=91 }
--DGDGDGDG				["Brain Smash"] = { level=40 chance=98 }
				["Cause Wounds"] = { level=64 chance=96 }
				["Drain Mana"] = { level=80 chance=89 }
				["Summon High Undead"] = { level=80 chance=97 }
				["Teleport To"] = { level=80 chance=88 }
			}
		}
		[11] =
		{
			name = "bronze golem"
			level = 65 rarity = 3
			desc = "A gigantic four-armed animated bronze statue, glowing with great heat."
			color = color.ORANGE
			speed = 10 life = {35,100} ac = 80
			aaf = 25 sleep = 50
			exp = 26000
			weight = 5500
			blows =
			{
				{"HIT","CRUSH",{19,19}},
				{"HIT","CRUSH",{19,19}},
				{"HIT","CRUSH",{19,19}},
				{"HIT","CRUSH",{19,19}},
			}
			flags =
			{
				RESIST=getter.resists{CONFUSION=50 CRUSH=40 ELEC=100 FEAR=100 FIRE=100 PIERCE=40 POIS=100 SLASH=40}
				FORCE_MAXHP=true FORCE_SLEEP=true EMPTY_MIND=true COLD_BLOOD=true OPEN_DOOR=true
				BASH_DOOR=true RES_TELE=true NO_SLEEP=true NO_STUN=true NONLIVING=true
				NO_CUT=true BLOW_RESPONSE=getter.array{[AURA_FIRE]={7,7}}
				AI=ai.SPELL_AND_MOVE MANA_CURR=100 MANA_MAX=100 MANA_REGENERATE=500
				AI_STATES=getter.flags{
				MOVE_AI=ai.ZOMBIE
				CASTING_AI=ai.SMART_CAST
				}
			}
			default_flags_to_ignore =
			{
				COLD_BLOOD=true
			}
			spells =
			{
				frequency = 3
				["Ball of Electricity"] = { level=90 chance=90 }
				["Bolt of Darkfire"] = { level=32 chance=92 }
				["Breathe Fire"] = { level=100 chance=100 }
				["Fireflash"] = { level=40 chance=93 }
				["Summon High Demons"] = { level=80 chance=90 }
				["Teleport To"] = { level=90 chance=92 }
			}
		}
	}
['h'] =
		{
		defaults =
			{
				body=default_body.humanoid
				flags={
					OPEN_DOOR=true BASH_DOOR=true TAKE_ITEM=true
					DROP_CORPSE=true DROP_SKELETON=true
					NON_INNATE_ARMOUR=true
					}
			} -- defaults
		[1] =
		{
			name = "renegade hobbit"
			level = 2 rarity = 1
			desc = 
			{
				"A rather shifty hobbit, who has abandoned the shire and opted to make",
				"his own shifty living on the margins of the towns. He has an eye for",
				"your possessions, including your life!",
			}
			color = color.TAN
			speed = 0 life = {4,4} ac = 20
			aaf = 20 sleep = 5
			exp = 6
			weight = 800
			blows =
			{
				{"WEAPON","SLASH",{2,4}},
				{"TOUCH","EAT_GOLD",{0,0}},
				{"TOUCH","EAT_ITEM",{0,0}},
			}
			flags =
			{
				MALE=true DROP_60=true HOBBIT=true
				DROP_THEME=getter.flags{ THEME_FOOD=50 THEME_TOOLS=25 THEME_COMBAT=25 }
				AI=ai.GHOUL AI_STATES=getter.flags{FEAR_LEVEL=25}
				ALLOW_IN_EVERYWHERE=true
			}
		}
		[2] =
		{
			name = "dwarf tinker"
			level = 4 rarity = 2
			desc = 
			{
				"Carrying what looks like his entire life on his back, this dwarf makes",
				"a living from mending pots and ironwork, many of which appear to be",
				"jingling from his clothing.",
			}
--- Junkpot!
			color = color.SLATE
			speed = 0 life = {8,8} ac = 12
			aaf = 30 sleep = 60
			exp = 13
			weight = 2400
			blows =
			{
				{"WEAPON","SLASH",{5,4}},				
				{"WEAPON","SLASH",{5,4}},				
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=10 DARK=50 SLASH=10 PIERCE=10}
				FACTION=FACTION_DWARF DWARF=true REGENERATE=400
				AI=ai.ZOMBIE SPEEDS=getter.speeds{WALK=-2}
				DROP_THEME=getter.flags{ THEME_JUNK=50 THEME_TOOLS=25 THEME_AXES=25}
				ALLOW_IN_EVERYWHERE=true
		}
		[3] =
		{
			name = "avari"
			level = 8 rarity = 3
			desc = 
			{
				"This haggard elf, cast out from his lands, retains some of his",
				"ancestral lore, but none of their honour. Watch yourself!"
			}
--- Not evil, but hostile!
			color = color.OLIVE_DRAB
			speed = 0 life = {6,6} ac = 20
			aaf = 60 sleep = 20
			exp = 35
			weight = 1900
			blows =
			{
				{"WEAPON","SLASH",{7,4}},
			}
			flags =
			{
				RESIST=getter.resists{LITE=50}
				FACTION=FACTION_DUNGEON ELF=true PASS_WALL=getter.flags{PASS_TREES=100}
				MANA_CURR=20 MANA_MAX=20 MANA_REGENERATE=100
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
				CASTING_AI=ai.SMART_CAST
				MOVE_AI=ai.ZOMBIE}
				DROP_THEME=getter.flags{ THEME_TREASURE=50 THEME_MAGIC=25 THEME_COMBAT=25}
				ALLOW_IN_EVERYWHERE=true
				}
			}
			spells =
			{
				frequency=4
					["Confuse"]={ level=20 chance=85}
					["Manathrust"] ={level=20 chance=75}				
			}
		}
		[4] =                                                       
		{
			name = "silvan pathbreaker"
			level = 15 rarity = 2
			desc = 
			{
				"Lithe and swift, this elf is a scout for the King of the Wood Elves,",
				"clearing the path for the much larger force which follows."
			}
--- Archer, warrior, sniper
			color = color.LIGHT_GREEN
			speed = 0 life = {8,8} ac = 30
			aaf = 60 sleep = 10
			exp = 45
			weight = 2000
			blows =
			{
				{"WEAPON","SLASH",{5,4}},
				{"WEAPON","SLASH",{5,4}},				
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=8 LITE=50 PIERCE=8 SLASH=8}
				FACTION=FACTION_ELF ELF=true PASS_WALL=getter.flags{PASS_TREES=100}
				LIMIT_SPELLS = getter.limit_spells{Arrow = 30} SPEEDS=getter.speeds{WALK=5}
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
				MIN_RANGE=2
				BEST_RANGE=5
				NO_SPELL_FLEE=false
				MOVE_AI=ai.ZOMBIE
				CASTING_AI=ai.SMART_CAST}
				DROP_THEME=getter.flags{ THEME_TREASURE=50 THEME_BOWS=25 THEME_COMBAT=25}
				ALLOW_IN_CAVE=false ALLOW_IN_FOREST=true ALLOW_IN_PLAIN=true 
				ALLOW_IN_MOUNTAIN=false ALLOW_IN_COLD=false

			}
			spells =
			{
				frequency=4
					["Arrow"]={ level=25 chance=85}
			}
		}
		[5] =
		{
			name = "dwarvish miner"
			level = 18 rarity = 4
			desc = 
			{
				"This dwarf is mining for riches down here in the cave, armed and",
				"armoured in case of interruption. A dim lantern is fixed to his helmet",
				"and his face is intent on the rock he is chipping at. Best not to come" 
				"between a dwarf and his riches.",
			}
--- Pick-axe in hand and a gleam in his eye
			color = color.DARK_SLATE_GRAY
			speed = 0 life = {9,9} ac = 22
			aaf = 20 sleep = 40
			exp = 50
			weight = 2200
			blows =
			{
				{"WEAPON","PIERCE",{4,6}},
				{"WEAPON","SLASH",{5,6}},								
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=20 DARK=50 PIERCE=20 SLASH=20}
				FACTION=FACTION_DWARF DWARF=true
				HAS_LITE=true FRIENDS=getter.friends(70,6)
				DROP_THEME=getter.flags{ THEME_TREASURE=40 THEME_TOOLS=40 THEME_COMBAT=20}
				AI=ai.ZOMBIE REGENERATE=500 SPEEDS=getter.speeds{WALK=-2}
				ALLOW_IN_CAVE=true ALLOW_IN_FOREST=false ALLOW_IN_PLAIN=false 
				ALLOW_IN_MOUNTAIN=true ALLOW_IN_COLD=false
				ALLOW_IN_CAVE=true ALLOW_IN_FOREST=false ALLOW_IN_PLAIN=false 
				ALLOW_IN_MOUNTAIN=true ALLOW_IN_COLD=false
			}
		}
		[6] =
		{
			name = "lizard man"
			level = 20 rarity = 3
			desc =                                                                
			{
				"This hideous fanged being hisses slowly at you. Though of the lizard",
				"race, it holds a sword in its hand and has painted it scaly hide with",
				"ochre and mud.",
			}
--- Sssssss... we hates the warm-bloods!
			color = color.SALMON
			speed = 0 life = {8,8} ac = 35
			aaf = 20 sleep = 40
			exp = 55
			weight = 2000
			blows =
			{
				{"BITE","PIERCE",{4,4}},
				{"WEAPON","SLASH",{5,6}},								
			}
			flags =
			{
				RESIST=getter.resists{COLD=-66 POIS=66}
				FACTION=FACTION_DUNGEON COLD_BLOOD=true 
				HAS_LITE=true FRIENDS=getter.friends(70,6)
				DROP_THEME=getter.flags{ THEME_JUNK=20 THEME_TOOLS=20 THEME_SWORD=60}
				FRIENDS=getter.friends(100,10) AI=ai.ZOMBIE CAN_SWIM=true
				ALLOW_IN_CAVE=true ALLOW_IN_FOREST=false ALLOW_IN_PLAIN=false 
				ALLOW_IN_MOUNTAIN=true ALLOW_IN_COLD=false ALLOW_IN_SWAMP=true
			}
		}
		[7] =
		{
			name = "hobbit slinger"
			level = 22 rarity = 3
			desc = 
			{
				"This hobbit eyes you speculatively and licks his finger, raising it to",
				"test the wind. In his other hand hangs a sling from which he can launch",
				"iron shot with deadly accuracy!",				
			}
--- Not as hardy as their TOME cousins, but still hghly annoying
			color = color.CHOCOLATE
			speed = 0 life = {6,6} ac = 30
			aaf = 50 sleep = 20
			exp = 55
			weight = 1400
			blows =
			{
				{"WEAPON","SLASH",{4,4}}
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=6 PIERCE=6 SLASH=6}
				FACTION=FACTION_DUNGEON HOBBIT=true FRIENDS=getter.friends(100,7)
				LIMIT_SPELLS=getter.limit_spells{Missile = 30} SPEEDS=getter.speeds{CAST=5}
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
				MOVE_AI=ai.GHOUL
				CASTING_AI=ai.SMART_CAST
				BEST_RANGE=5
				MIN_RANGE=2
				FEAR_LEVEL=25}
				ALLOW_IN_EVERYWHERE=true
				DROP_THEME=getter.flags{THEME_TREASURE=20 THEME_BOWS=60 THEME_COMBAT=20}
			}
			spells =
			{
				frequency=3
				["Missile"]={level=50 chance=85}
			}
		}
		[8] =
		{
			name = "sindarin messenger"
			level = 30 rarity = 4
			desc = 
			{
				"This lightly armoured elf is carrying a scroll-tube at his belt. He is",
				"a royal messenger and has full authority from the King to take whatever",
				"steps are necessary to get his message through!"
			}
--- He has places to be, and won't let you stop him! Fast and with a touch of guile
			color = color.GREEN
			speed = 5 life = {9,9} ac = 45
			aaf = 50 sleep = 0
			exp = 120
			weight = 1700
			blows =
			{
				{"WEAPON","SLASH",{6,5}},
			}
			flags =
			{
				RESIST=getter.resists{CONFUSE=50 CRUSH=10 FEAR=100 LITE=50 PIERCE=10 SLASH=10}
				FACTION=FACTION_ELF ELF=true HAS_LITE=true SPEEDS=getter.speeds{WALK=10}
				MANA_CURR=150 MANA_MAX=150 MANA_REGENERATE=750 PASS_WALL=getter.flags{PASS_TREES=100}
				GOOD=true AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
				MOVE_AI=ai.ZOMBIE
				CASTING_AI=ai.SMART_CAST
				FEAR_LEVEL=30}
				DROP_THEME=getter.flags{THEME_TREASURE=50 THEME_COMBAT=25 THEME_CANTRIPS=25}
				ALLOW_IN_EVERYWHERE=true
			}
			spells =
			{
				frequency=3
				["Grow Trees"]={level=20 chance=85}				
				["Confuse"]={level=30 chance=85}				
				["Noxious Cloud"]={level=20 chance=75}
				["Essence of Speed"] ={level=2 chance=75}				
				["Stun"] ={level=10 chance=70}				
			}
		}

		[9] =
		{
			name = "dwarvish warrior"
			level = 33 rarity = 2
			desc = 
			{
				"Though short in stature, these warriors are clad in the finest armour",
				"in Arda and their prowess at arms is respected by their friends and",
				"feared by their foes. "
			}
--- Hardy, hits hard and found in groups. 
			color = color.OLD_LACE
			speed = 0 life = {11,10} ac = 30
			aaf = 30 sleep = 50
			exp = 110
			weight = 3000
			blows =
			{
				{"WEAPON","SLASH",{7,7}},
				{"WEAPON","SLASH",{7,7}},								
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=30 DARK=55 PIERCE=30 SLASH=30}
				FACTION=FACTION_DWARF DWARF=true FRIENDS=getter.friends(100,15)
				SPEEDS=getter.speeds{WALK=-3} REFLECTING=15 FRIENDS=getter.friends(90,14)
				HAS_LITE=true REGENERATE=700 AI=ai.ZOMBIE
				DROP_THEME=getter.flags{THEME_TREASURE=50 THEME_BOWS=25 THEME_COMBAT=25}
				ALLOW_IN_CAVE=true ALLOW_IN_MOUNTAIN=true
			}
		}
		[10] =
		{
			name = "lizard king"
			level = 20 rarity = 3
			desc =                                                                
			{
				"This appears to a normal lizard man. Then it stands erect, and a bright",
				"orange crest rises up on top of its head. This lizard man is a king of",
				"its people and has risen to this position by eating those who would",
				"challenge him!",
				
			}
--- Sssssss... we and all my hatchmates hates the warm-bloods!
			color = color.ORANGE
			speed = 10 life = {10,10} ac = 45
			aaf = 20 sleep = 40
			exp = 120
			weight = 2000
			blows =
			{
				{"BITE","PIERCE",{5,5}},
				{"WEAPON","SLASH",{6,7}},
				{"WAIL","FEAR",{6,4}},
			}
			flags =
			{
				RESIST=getter.resists{COLD=-66 POIS=66}
				FACTION=FACTION_DUNGEON COLD_BLOOD=true HAS_LITE=true WEIRD_MIND=true
				ESCORT=40 ESCORT_NAME="lizard man"
				DROP_THEME=getter.flags{ THEME_TREASURE=20 THEME_SWORD=80}
				FRIENDS=getter.friends(100,10) AI=ai.ZOMBIE CAN_SWIM=true
				ALLOW_IN_CAVE=true ALLOW_IN_FOREST=false ALLOW_IN_PLAIN=false 
				ALLOW_IN_MOUNTAIN=true ALLOW_IN_COLD=false ALLOW_IN_SWAMP=true
			}
		}
		[11] =
		{
			name = "silvan elf archer"
			level = 34 rarity = 2
			desc = 
			{
				"This all elf is clad in leathers and has but a small sword to protect",
				"himself. But for the great bow and the quiver of elven arrows mark him",
				"as one of the Elf-King's most valuable, and lethal servants."
			}
--- TWANG... thunk.... TWANG... thunk
			color = color.DARK_GREEN
			speed = 0 life = {8,8} ac = 30
			aaf = 60 sleep = 40
			exp = 130
			weight = 2000
			blows =
			{
				{"WEAPON","SLASH",{6,6}},				
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=10 LITE=50 PIERCE=10 SLASH=10}
				DROP_THEME=getter.flags{THEME_BOWS=50 THEME_COMBAT=30 THEME_TREASURE=20}
				FACTION=FACTION_ELF ELF=true SPEEDS=getter.speeds{CAST=10}
				LIMIT_SPELLS = getter.limit_spells{Arrow = 40}
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
				CASTING_AI=ai.SMART_CAST
				MOVE_AI=ai.ZOMBIE
				BEST_RANGE=5
				MIN_RANGE=2
				LURE_TARGET=true}
				DROP_THEME=getter.flags{ THEME_TREASURE=20 THEME_BOWS=50 THEME_COMBAT=25}
				ALLOW_IN_CAVE=false ALLOW_IN_FOREST=true ALLOW_IN_PLAIN=true 
				ALLOW_IN_MOUNTAIN=false
			}
			spells =
			{
				frequency=2
				["Arrow"] = {level=40 chance=90}
			}
		}
		[12] =
		{
			name = "sindarin warrior"
			level = 33 rarity = 3
			desc = 
			{
				"This lean elven warrior is the mainstay of the elven forces. Lightly",
				"mailed, and armed with a slender, gleaming blade, they are the", 
				"scourge of evil!"
			}
--- Lightly Armoured, but a 
			color = color.LIGHT_GREEN
			speed = 10 life = {10,10} ac = 37
			aaf = 20 sleep = 60
			exp = 150
			weight = 1800
			blows =
			{
				{"WEAPON","SLASH",{7,7}},
				{"WEAPON","VALINOREAN_FIRE",{7,7}},				
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=10 FEAR=55 LITE=66 POIS=66 PIERCE=10 SLASH=10}
				FACTION=FACTION_ELF ELF=true GOOD=true HAS_LITE=true REFLECTING=10
				FRIENDS=getter.friends(90,11) AI=ai.ZOMBIE
				DROP_THEME=getter.flags{THEME_SWORD=50 THEME_COMBAT=50}
				ALLOW_IN_CAVE=false ALLOW_IN_FOREST=true ALLOW_IN_PLAIN=true 
				ALLOW_IN_MOUNTAIN=false ALLOW_IN_COLD=false
			}
		}
		[13] =
		{
			name = "hobbit thief"
			level = 34 rarity =2
			desc =
			{
				"There it is again! With dificulty, you see the small humanoid sheltering",
				"under a grey-brown cloak. This hobbit has left the shire to make his",
				"nefarious living elsewhere, and may be looking to make it off you!",
			}
			color = color.CHOCOLATE
			speed = 10 life = {8,9} ac=60
			aaf = 90 sleep = 10
			exp = 220
			weight = 1200
			blows =
			{
				{"WEAPON","PIERCE",{4,7}},
				{"TOUCH","EAT_GOLD",{0,0}}
				{"TOUCH","EAT_ITEM",{0,0}}
			}
			flags =
			{
				RESIST=getter.resists{SLASH=5 CRUSH=5 PIERCE=5 DARK=66}
				DROP_THEME=getter.flags{THEME_KNIFE=20 THEME_BOWS=20 THEME_TREASURE=30 THEME_TOOLS=30}
				FACTION=FACTION_DUNGEON HOBBIT=true SMART=true INVISIBLE=true
				SPEEDS=getter.speeds{WALK=5} SEE_INVIS=60 
				PASS_WALL=getter.flags{PASS_TREES=100} LIMIT_SPELLS = getter.limit_spells{Missile = 20}
				AI=ai.SPELL_AND_MOVE
				AI_STATES=getter.flags{
					MOVE_AI=ai.SNAKE
					CASTING_AI=ai.SMART_CAST
					MIN_RANGE=1
					BEST_RANGE=5
					LURE_TARGET=true}
				DROP_THEME=getter.flags{THEME_TOOLS=40 THEME_BOWS=20 THEME_COMBAT=20 THEME_TREASURE=20}
				ALLOW_IN_EVERYWHERE=true

			}
			spells =
			{
				frequency=5
				["Missile"] = {level= 20 chance = 65}
				["Trap Filling"] = {level= 55 chance = 80}
			}
		}
		[14] =
		{
			name = "sindarin ranger"
			level = 38 rarity = 3
			desc = 
			{
				"At one with the forest, this elf sits cross-legged on the ground. A slim",
				"sword lays across his knees and a greatbow is slung at his back. He is a",
				"ranger of the Sindarin elves, and in a forest he acknowledges no equal."
			}
--- A little bow, a little sword and a good deal of rat cunning
			color = color.YELLOW_GREEN
			speed=10 life = {10,10} ac =45
			aaf = 80 sleep = 30
			exp = 300
			weight = 1900
			blows =
			{
				{"WEAPON","SLASH",{6,8}},
				{"WEAPON","SLASH",{6,8}},
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=8 LITE=50 PIERCE=8 POIS=33 SLASH=8}
				DROP_THEME=getter.flags{THEME_BOWS=40 THEME_COMBAT=30 THEME_MAGIC=20 THEME_TREASURE=10}
				FACTION=FACTION_ELF ELF=true HAS_LITE=true MANA_MAX=90 MANA_CURR=90
				MANA_REGENERATE=500 LIMIT_SPELLS = getter.limit_spells{Arrow = 60}
				GOOD=true PASS_WALL=getter.flags{PASS_TREES=100}
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
				CASTING_AI=ai.SMART_CAST
				MOVE_AI=ai.ZOMBIE 
				LURE_TARGET=true}
				ALLOW_IN_FOREST=true ALLOW_IN_PLAIN=true
				DROP_THEME=getter.flags{THEME_TREASURE=10 THEME_COMBAT=30 THEME_BOWS=30 THEME_MAGIC=30}
			}
			spells =
			{
				frequency=4
					["Arrow"]={ level=70 chance=85}
					["Summon Animal"] = {level=5 chance=65}
					["Healing"] = {level=35 chance=70}
					["Confuse"] = {level=45 chance=70}
					["Grow Trees"] = {level=50 chance=65}
			}
		}
		[15] =
		{
			name = "dwarven paladin"
			level = 40 rarity = 3
			desc = 
			{
				"This dwarf is a ball of glistening muscle and clad in a simple robe. ",
				"He is a disciple of Tulkas and has been granted access to many of the",
				"Valar's mysteries, and his skin is as hard and white as marble.",				
			}
--- This devotee of Tulkas will vex even the most hardy of adventurers
			color = color.RED
			speed = 10 life = {20,20} ac = 70
			aaf = 40 sleep = 20
			exp = 450
			weight = 300
			blows =
			{
				{"PUNCH","CRUSH",{6,6}},
				{"PUNCH_OFFHAND","CRUSH",{6,6}},				
				{"KICK","CRUSH",{8,10}},
			}
			flags =
			{
				RESIST=getter.resists{COLD=50 CONFUSION=50 CRUSH=40 ELEC=50 FEAR=100 FIRE=50 POIS=50 PIERCE=40 SLASH=30}
				FACTION=FACTION_DWARF DWARF=true KILL_WALL=getter.flags{WALL=true}
				HAS_LITE=true REFLECTING=50 HAS_LITE=true MANA_CURR=100 MANA_MAX=100
				MANA_REGENERATE=500	AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
				CASTING_AI=ai.SMART_CAST
				MOVE_AI=ai.ZOMBIE}
				ALLOW_IN_FOREST=true ALLOW_IN_PLAIN=true
				DROP_THEME=getter.flags{THEME_TREASURE=10 THEME_COMBAT=30 THEME_BOWS=30 THEME_MAGIC=30}				
			}
			spells =
			{
				frequency=3
					["Strike"]={ level=12 chance=85}
					["Shake"]={ level=20 chance=80}
					["Stone Prison"]={ level=22 chance=85}
			}
		}
		[16] =
		{
			name = "defender of the shire"
			level = 40 rarity =5 
			desc = 
			{
				"This doughty hobbit has appointed himself Defender of the Shire, and",
				"has rallied like-minded hobbits around him. Mailed and armed with an"
				"lead-weighted club, he is a cunning warrior.",
			}
--- A doughty hobbit who has assigned to himself the defence of the shire
			color = color.SANDY_BROWN
			speed = 15 life = {12,12} ac = 65
			aaf = 80 sleep = 20
			exp = 500
			weight = 1500
			blows =
			{
				{"WEAPON","CRUSH",{6,6}},			
				{"WEAPON","CRUSH",{6,6}},											
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=14 FEAR=100 PIERCE=14 SLASH=14}
				FACTION=FACTION_ELF HOBBIT=true UNIQUE=true SPEEDS=getter.speeds{WALK=-5}
				ESCORT=true ESCORTED_BY_GROUPS=true ESCORT_FLAGS=getter.flags{HOBBIT=true}
				LIMIT_SPELLS = getter.limit_spells{Missile = 30} FORCE_MAXHP=true
				AI=ai.SPELL_AND_MOVE SMART=true AI_STATES=getter.flags{
				MOVE_AI=ai.PATHFIND
				CASTING_AI=ai.SMART_CAST
				BEST_RANGE=5
				MIN_RANGE=2
				FEAR_LEVEL=25}
				ALLOW_IN_PLAIN=true ALLOW_IN_FOREST=true ALLOW_IN_TOWN_BREE=true
				DROP_THEME=getter.flags{THEME_COMBAT=30 THEME_BOWS=30 THEME_HAFTED=40}				
			}
			spells =
			{
				frequency=3
				["Missile"]={level=50 chance=85}
				["Create Traps"] = {level= 55 chance = 80}
			}
		}
		[17] =
		{
			name = "silvan druid"
			level = 50 rarity = 3
			desc = 
			{
				"This elf is a friend of fur, fin and forest, and can summon all of them",
				"to his aid in combat. He is also not without resources of his own!"
			}
--- Yavanna-worshipper
			color = color.GREEN
			speed = 10 life = {10,10} ac = 50
			aaf = 70 sleep = 20
			exp = 700
			weight = 1400
			blows =
			{
				{"WEAPON","CRUSH",{4,4}},
				{"WEAPON","CRUSH",{4,4}},
			}
			flags =
			{
				RESIST=getter.resists{POIS=66}
				DROP_THEME=getter.flags{THEME_MAGIC=60 THEME_HAFTED=20 THEME_FOOD=20}
				FACTION=FACTION_ANIMAL ELF=true CAN_SWIM=true
				REGENERATE=5000 PASS_WALL=getter.flags{PASS_TREES=100}
				MANA_CURR=350 MANA_MAX=350 MANA_REGENERATE=1750
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
				MOVE_AI=ai.ZOMBIE
				CASTING_AI=ai.SMART_CAST
				}
				ALLOW_IN_PLAIN=true ALLOW_IN_FOREST=true
				DROP_THEME=getter.flags{THEME_MAGIC=60 THEME_HAFTED=20 THEME_HAFTED=20}				
			}
			spells=
			{
				frequency=3
					["Summon Animal"] = {level=10 chance=65}
					["Summon Animals"] = {level=10 chance=65}
					["Summon Plant"] = {level=20 chance=65}
					["Healing"] = {level=40 chance=70}
					["Grow Trees"] = {level=48 chance=65}
					["Ice Storm"] = {level=6 chance=65}
					["Geyser"] = {level=48 chance =70}
			}
		}
		[18] =
		{
			name = "dwarven mine leader"
			level = 60 rarity = 4
			desc = 
			{
				"This noble dwarf is a mine-leader, and has responsibility for the entire",
				"workings down here. His armour is thick and his face is covered in a",
				"mask, allowing him to work where mine-gasses would overcome lesser",
				"dwarfs."
			}
--- dwarves, dwarves, dwarves... oh... and a couple of spells 
			color = color.ANTIQUE_WHITE
			speed = 10 life = {25,20} ac =  30
			aaf = 40 sleep = 40
			exp = 800
			weight = 3000
			blows =
			{
				{"WEAPON","SLASH",{8,8}},
				{"WEAPON","SLASH",{8,8}},
				{"WEAPON","SLASH",{8,8}},				
			}
			flags =
			{
				RESIST=getter.resists{ACID=66 CRUSH=30 FIRE=66 PIERCE=30 POIS=100 SLASH=30}
				FACTION=FACTION_DWARF DWARF=true ESCORT=30 ESCORTED_BY_GROUPS=true
				ESCORT_NAME="dwarvish miner" SPEEDS=getter.speeds{WALK=-5}
				HAS_LITE=true REFLECTING=30 SMART=true
				DROP_THEME=getter.flags{ THEME_TREASURE=40 THEME_TOOLS=40 THEME_AXES=20}
				MANA_MAX=75 MANA_CURR=75 MANA_REGENERATE=425		
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
				MOVE_AI=ai.ZOMBIE
				CASTING_AI=ai.SMART_CAST}
 				ALLOW_IN_MOUNTAIN=true ALLOW_IN_CAVE=true
				DROP_THEME=getter.flags{THEME_TOOLS=30 THEME_COMBAT=70}				
			}
			spells =
			{
				frequency=3
					["Shake"] = {level=10 chance=80}					
					["Strike"] = {level=4 chance=75}					
			}
		}

		[19] =
		{
			name = "hobbit traitor"
			level = 67 rarity = 4
			desc = 
			{
				"This hobbit has betrayed his kind, travelled to Minas Ithil and offerred",
				"himself as a servant to the dark lord. The Dark-Lord is pleased with",
				"his new servant, and has scrawled runes in the foul language of Mordor",
				"all over the hobbit's body. He is now not only lost, but a terrible",
				"threat!",				
			}
--- Trained by Morgoth, this is a fearsome and subtle opponent
			color = color.FIREBRICK
			speed = 15 life = {14,14} ac = 60
			aaf = 60 sleep = 20
			exp = 850
			weight = 1600
			blows =
			{
				{"WEAPON","SLASH",{4,4}},
				{"WEAPON","POISON",{4,4}},
				{"WEAPON","CONFUSE",{4,4}},
				{"WEAPON","DARK_FIRE",{4,4}},				
			}
			flags =
			{
				RESIST=getter.resists{ACID=66 COLD=66 DARK=66 DARK_FIRE=66 ELEC=66 FIRE=66 POIS=66}
				FACTION=FACTION_SAURON HOBBIT=true EVIL=true INVISIBLE=true SMART=true
				NO_AUTO_TARGET=true MANA_CURR=200 MANA_MAX=200 MANA_REGENERATE=1000
				BLOW_RESPONSE=getter.array{[AURA_DARK_FIRE]={4,4}}
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
				MOVE_AI=ai.ZOMBIE
				CASTING_AI=ai.SMART_CAST
				FEAR_LEVEL=30}
 				ALLOW_IN_EVERYWHERE=true
				DROP_THEME=getter.flags{THEME_KNIFE=30 THEME_MAGIC=70}				
			}
			spells =
			{
					frequency=3
					["Phase Door"] = {level=48 chance=90}					
			    ["Animate Dead"] = {level=20 chance=90}
			    ["Disease"] = {level=30 chance=90}
			    ["Trap Filling"] = {level=40 chance=90}
			    ["Drain Mana"] = {level=40 chance=90}
			}
		}
		[20] =
		{
			name = "avari sorcerer"
			level = 70 rarity = 2
			desc = 
			{
				"Filled with the mysteries of Valinor and the wisdom of Elfkind, this",
				"oucast has fallen prey to the temptations of Melkor, and serves Sauron",
				"willingly. Power crackles from his fingers and shines out balefully",
				"from his eyes.",
			}
--- Evil Zap! Evil Kapow! Evil Blink
			color = color.PURPLE
			speed = 0 life = {9,9} ac = 30
			aaf = 40 sleep = 30
			exp = 900
			weight = 1100
			flags =
			{
				RESIST=getter.resists{CONFUSION=50 DARK_FIRE=55 POIS=66}
				DROP_THEME=getter.flags{THEME_MAGIC=80 THEME_TREASURE=20}
				FACTION=FACTION_SAURON ELF=true HAS_LITE=true SPEEDS=getter.speeds{CAST=10}
				MANA_CURR=500 MANA_MAX=500 MANA_REGENERATE=3000 SMART=true
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
					CASTING_AI=ai.SMART_CAST
					MOVE_AI=ai.ZOMBIE
					BEST_RANGE=3
					MIN_RANGE=1
					NO_SPELL_FLEE=true}
					ALLOW_IN_EVERYWHERE=true
					DROP_THEME=getter.flags{THEME_MAGIC=100}				
			}
			spells=
			{
				frequency=2
					["Fireflash"] = {level=60 chance=85}
					["Darkness"] = {level=75 chance=85}
					["Confuse"] = {level=70 chance=85}
					["Manathrust"] = {level=78 chance=85}
					["Essence of Speed"] = {level=50 chance=70}
					["Drain Mana"] = {level=60 chance=80}
					["Phase Door"] = {level=78 chance=95}
					["Teleportation"] = {level=60 chance=90}
					["Summon Monster"] = {level=60 chance=85}
					["Tidal Wave"] = {level=48 chance=85}
					["Stone Prison"] = {level=30 chance=85}
					["Disease"] = {level=36 chance=85}
			}
		}
		[21] =
		{
			name = "knight of stone"
			level = 73 rarity = 2
			desc = 
			{
				"These gleaming dwarven warriors are the cream of the Dwarven race and",
				"their armour gleams with the deepest secrets of their craft. Their ",
				"blades are keen and their eyes unwavering in their dedication to ",
				"the honour of their king!"
			}
--- Knights of Stone, extremely well armoured
			color = color.WHITE
			speed = 15 life = {16,16} ac = 50
			aaf = 30 sleep = 50
			exp = 800
			weight = 3500
			blows =
			{
				{"WEAPON","SLASH",{12,12}},
				{"WEAPON","SLASH",{12,12}},				
				{"WEAPON","SLASH",{12,12}},
			}
			flags =
			{
				RESIST=getter.resists{COLD=66 CRUSH=50 ELEC=66 FIRE=66 SLASH=50 PIERCE=50 POIS=66}
				FACTION=FACTION_DWARF DWARF=true REFLECTING=60 HAS_LITE=true POWERFUL=true 
				SPEEDS=getter.speeds{WALK=-5} REGENERATE=1500 FRIENDS=getter.friends(100,24)
				AI=ai.ZOMBIE AI_STATES=getter.flags{LURE_TARGET=true}
				ALLOW_IN_CAVE=true ALLOW_IN_MOUNTAIN=true
				DROP_THEME=getter.flags{THEME_COMBAT=80 THEME_TREASURE=20}				
			}
		}

		[22] =
		{
			name = "sindarin sorceror"
			level = 80 rarity = 3
			desc = 
			{
				"An aura of light surrounds this elf, dressed in robes of great finery",
				"adorned with silver and gold runes of power. This High Elf is truly",
				"deserving of his name, an embodiment of all the power and wisdom of",
				"his people.",
			}                                     
--- Good Zap! Good Kapow! Good Blink!
			color = color.VIOLET
			speed = 10 life = {11,11} ac = 30
			aaf = 40 sleep = 30
			exp = 900
			weight = 1100
			flags =
			{
				RESIST=getter.resists{CONFUSION=50 FEAR=55 POIS=66}
				DROP_THEME=getter.flags{THEME_MAGIC=80 THEME_TREASURE=20}
				FACTION=FACTION_ELF ELF=true HAS_LITE=true SPEEDS=getter.speeds{CAST=5}
				GOOD=true MANA_CURR=600 MANA_MAX=600 MANA_REGENERATE=3600 SMART=true
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
					CASTING_AI=ai.SMART_CAST
					MOVE_AI=ai.ZOMBIE
					BEST_RANGE=2
					MIN_RANGE=1
					NO_SPELL_FLEE=true}
				ALLOW_IN_EVERYWHERE=true
				DROP_THEME=getter.flags{THEME_MAGIC=80 THEME_TREASURE=20}				
			}
			spells=
			{
				frequency=2
					["Teleport Away"] = {level=44 chance=85}
					["Slow Monster"] = {level=70 chance=85}
					["Grow Trees"] = {level=78 chance=85}
					["Confuse"] = {level=80 chance=85}
					["Geyser"] = {level=88 chance=85}
					["Essence of Speed"] = {level=60 chance=70}
					["Healing"] = {level=70 chance=80}
					["Phase Door"] = {level=88 chance=95}
					["Teleportation"] = {level=40 chance=90}
					["Summon Plant"] = {level=40 chance=85}
					["Summon Animal"] = {level=40 chance=85}
					["Tidal Wave"] = {level=58 chance=85}
					["Stone Prison"] = {level=40 chance=85}
					["Ice Storm"] = {level=46 chance=85}
					["Shake"] = {level=36 chance =85}
					["Noxious Cloud"] = {level=84 chance =85}
			}
		}
		[23] =
		{
			name = "elven war leader"
			level = 84 rarity = 3
			desc = 
			{
				"Leading the hosts of elfkind, this war-leader is kin to Galadriel and",
				"is of like majesty, though his gifts lie more in war than magic. He",
				"is revered by his men and his bright blade and commanding voice are",
				"a shining beacon of command!",
			}
--- Leading the hosts of Lothlorien forth!
			color = color.DARK_SEA_GREEN
			speed = 15 life = {18,18} ac = 75
			aaf = 60 sleep = 10
			exp = 1200
			weight =  2000
			blows =
			{
				{"WEAPON","VALINOREAN_FIRE",{14,14}},
				{"WEAPON","VALINOREAN_FIRE",{14,14}},								
			}
			flags =
			{
				RESIST=getter.resists{COLD=66 CRUSH=20 ELEC=66 FIRE=66 LITE=50 PIERCE=20 POIS=66 SLASH=20}
				FACTION=FACTION_ELF ELF=true GOOD=true HAS_LITE=true POWERFUL=true
				ESCORT=50 ESCORTED_BY_GROUPS=true ESCORT_FLAGS=getter.flags{ELF=true}
				MANA_CURR=150 MANA_MAX=150 MANA_REGENERATE=750
				SMART=true AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
				MOVE_AI=ai.PATHFIND
				CASTING_AI=ai.SMART_CAST}
				ALLOW_IN_FOREST=true ALLOW_IN_PLAIN=true ALLOW_IN_MOUNTAIN=true
				DROP_THEME=getter.flags{THEME_SWORD=30 THEME_TREASURE=30 THEME_CANTRIPS=40}				
			}
			spells =
			{
				frequency=2
					["Bolt of Valinorean Fire"] = {level=44 chance=85}
					["Summon Kin"] = {level=70 chance=85}				
			}
		}

		[24] =
		{
			name = "dwarven king"
			level = 85 rarity = 3
			desc = 
			{
				"Proud, tall for his race and as immovable and enduring as a mountain,",
				"this dwarvish king is the leader of his people and a fearsome warrior",
				"in his own right. His axe is deceptively plain, but his bearing and",
				"grace even in heavy plate mail marks him as of royal blood!",
			}
--- A fearsome army of stone obeys his commands!
			color = color.LIGHT_DARK
			speed = 20 life = {24,24} ac = 70
			aaf = 30 sleep = 60
			exp = 1500
			weight = 3000
			blows =
			{
				{"WEAPON","SLASH",{14,14}},
				{"WEAPON","SLASH",{14,14}},				
				{"WEAPON","SLASH",{14,14}},		
			}
			flags =
			{
				RESIST=getter.resists{COLD=66 CONFUSION=55 CRUSH=60 ELEC=66 FEAR=100 FIRE=66 LITE=50 PIERCE=60 POIS=66 SLASH=60}
				FACTION=FACTION_DWARF DWARF=true HAS_LITE=true NO_STUN=true POWERFUL=true
				ESCORT=100 ESCORTED_BY_GROUPS=true ESCORT_FLAGS=getter.flags{DWARF=true}
				REFLECTING=80 AI=ai.PATHFIND
				ALLOW_IN_EVERYWHERE=true
				DROP_THEME=getter.flags{THEME_AXES=40 THEME_TREASURE=30 THEME_CANTRIPS=30}								
			}
		}
		[25] =
		{
			name = "corrupted sindarin elven sorceror"
			level = 90 rarity = 3
			desc = 
			{
				"This once noble-elf is now a corrupted shell of a being, laughing and",
				"crying by turns. Melkor's whispers slowly drove him mad, but did not",
				"affect his mastery of magic!",
			}
--- Necro, Plue Evil Zap, Evil Kapow, Evil Blink
			color = color.ORCHID
			speed = 20 life = {15,15} ac = 40
			aaf = 30 sleep = 50
			exp = 2000
			weight = 1500
			flags =
			{
				RESIST=getter.resists{ACID=66 COLD=66 CONFUSION=50 DARK_FIRE=66 ELEC=66 FEAR=100 FIRE=100}
				FACTION=FACTION_SAURON ELF=true CHAR_MULTI=true EVIL=true HAS_LITE=true
				MANA_CURR=500 MANA_MAX=500 MANA_REGENERATE=2500 NO_SLEEP=true NO_STUN=true
				DROP_THEME=getter.flags{THEME_MAGIC=80 THEME_TREASURE=20}
				LIMIT_SPELLS = getter.limit_spells{Manaflow=2}
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
				CASTING_AI=ai.SMART_CAST
				MOVE_AI=ai.RANDOM_MOVE
				MOVE_RATE=33
				NO_SPELL_FLEE=true
				BEST_RANGE=4
				MIN_RANGE=2} 
				ALLOW_IN_EVERYWHERE=true
				DROP_THEME=getter.flags{THEME_MAGIC=80 THEME_TREASURE=20}				
			}
			spells=
			{
				frequency=3
					["Ball of Darkfire"] = {level=60 chance=80}
					["Slow Monster"] = {level=80 chance=80}
					--- ["Dark Storm"] = {level=95 chance=80}
					["Confuse"] = {level=90 chance=80}
					["Bolt of Chaos"] = {level=80 chance=80}
					["Essence of Speed"] = {level=70 chance=65}
					["Healing"] = {level=80 chance=75}
					["Phase Door"] = {level=98 chance=90}
					["Teleportation"] = {level=80 chance=85}
					["Summon Monsters"] = {level=85 chance=80}
					["Tidal Wave"] = {level=68 chance=80}
					["Stone Prison"] = {level=50 chance=80}
					["Ice Storm"] = {level=56 chance=80}
					["Shake"] = {level=46 chance =80}
					["Noxious Cloud"] = {level=94 chance =80}
					["Manaflow"] = {level=1 chance =80}
					["Summon Elemental"] = {level=85 chance=80}
					["Hand of Doom"] = {level=70 chance =80}
			}
		}
	}
	['H'] =
	{
		defaults =
		{
			body = default_body.bird
			flags =
			{
				ALLOW_IN_EVERYWHERE=true DROP_CORPSE=true DROP_SKELETON=true
			}
		}
		[1] =
		{
			name = "white harpy"
			level = 2 rarity = 1
			desc = "A flying, screeching bird with a woman's face."
			color = color.WHITE
			speed = 0 life = {3,5} ac = 17
			aaf = 16 sleep = 10
			exp = 5
			weight = 500
			blows =
			{
				{"CLAW","SLASH",{3,2}},
				{"CLAW","SLASH",{3,2}},
				{"BITE","PIERCE",{3,6}},
			}
			flags =
			{
				RESIST=getter.resists{DARK_FIRE=50} AERIAL=true CAN_FLY=3
				FEMALE=true AERIAL=true CAN_FLY=3 SEE_INVIS=15
				ANIMAL=true EVIL=true MORTAL=true
				AI=ai.ZOMBIE ALLOW_IN_MOUNTAIN=true
				DROP_THEME=getter.flags{THEME_JUNK=70 THEME_FOOD=30}
			}
		}
		[2] =
		{
			name = "black harpy"
			level = 9 rarity = 1
			desc = "A woman's face on the body of a vicious black bird."
			color = color.LIGHT_DARK
			speed = 0 life = {3,8} ac = 22
			aaf = 16 sleep = 10
			exp = 19
			weight = 600
			blows =
			{
				{"CLAW","SLASH",{6,2}},
				{"CLAW","SLASH",{6,2}},
				{"BITE","PIERCE",{9,3}},
			}
			flags =
			{
				RESIST=getter.resists{DARK_FIRE=50}
				FEMALE=true AERIAL=true CAN_FLY=3 SPEED=getter.speeds{WALK=10} SEE_INVIS=15
				ANIMAL=true EVIL=true MORTAL=true
				AERIAL=true CAN_FLY=3 AI=ai.ZOMBIE ALLOW_IN_MOUNTAIN=true
				DROP_THEME=getter.flags{THEME_JUNK=70 THEME_FOOD=30}
			}
		}
		[3] =
		{
			name = "owlbear"
			level = 10 rarity = 1
			desc = "A bizarre bear-creature with the claws and the face of an owl."
			color = color.ORANGE
			speed = 0 life = {14,14} ac = 18
			aaf = 20 sleep = 20
			exp = 35
			weight = 2000
			blows =
			{
				{"CLAW","SLASH",{4,9}},
				{"CLAW","SLASH",{4,9}},
				{"CRUSH","CRUSH",{6,10}},
			}
			flags =
			{
				ANIMAL=true OPEN_DOOR=true BASH_DOOR=true
				MORTAL=true SEE_INVIS=15 AI=ai.ZOMBIE
			}
		}
		[4] =
		{
			name = "hippocampus"
			level = 11 rarity = 1
			desc = "A truly strange hybrid of a horse and a fish."
			color = color.LIGHT_BLUE
			speed = 10 life = {20,9} ac = 14
			aaf = 12 sleep = 10
			exp = 40
			weight = 900
			blows =
			{
				{"BITE","CRUSH",{6,5}},
				{"KICK","CRUSH",{4,15}},
				{"KICK","CRUSH",{4,15}},
			}
			flags =
			{
				AQUATIC=true ANIMAL=true MORTAL=true
				SEE_INVIS=15 AI=ai.ZOMBIE
				ALLOW_IN_OCEAN=true ALLOW_IN_RIVER=true
			}
		}
		[5] =
		{
			name = "hippogryph"
			level = 11 rarity = 1
			desc = "A strange hybrid of eagle and horse. It looks weird."
			color = color.LIGHT_UMBER
			speed = 0 life = {20,12} ac = 30
			aaf = 12 sleep = 10
			exp = 50
			weight = 2100
			blows =
			{
				{"BITE","SLASH",{12,5}},
				{"CLAW","SLASH",{9,9}},
				{"KICK","CRUSH",{9,9}},
			}
			flags =
			{
				BASH_DOOR=true AERIAL=true CAN_FLY=3 SEE_INVIS=15
				ANIMAL=true MORTAL=true AERIAL=true CAN_FLY=3
				AI=ai.ZOMBIE ALLOW_IN_PLAIN=true ALLOW_IN_MOUNTAIN=true
			}
		}
		[6] =
		{
			name = "griffon"
			level = 15 rarity = 1
			desc =
			{
				"It is half lion, half eagle. It flies menacingly towards you,",
				"screaming loudly.",
			}
			color = color.UMBER
			speed = 0 life = {30,20} ac = 35
			aaf = 12 sleep = 10
			exp = 70
			weight = 2500
			blows =
			{
				{"BITE","SLASH",{9,15}},
				{"CLAW","SLASH",{10,10}},
				{"CLAW","SLASH",{10,10}},
			}
			flags =
			{
				BASH_DOOR=true AERIAL=true CAN_FLY=15 SEE_INVIS=15
				ANIMAL=true MORTAL=true
				AI=ai.ZOMBIE ALLOW_IN_PLAIN=true ALLOW_IN_FOREST=true
				ALLOW_IN_MOUNTAIN=true
			}
		}
		[7] =
		{
			name = "sphinx"
			level = 17 rarity = 2
			desc = "It will eat you if you cannot answer its riddle."
			color = color.ORANGE
			speed = 0 life = {60,8} ac = 40
			aaf = 20 sleep = 20
			exp = 80
			weight = 6000
			blows =
			{
				{"CLAW","SLASH",{12,12}},
				{"CLAW","SLASH",{12,12}},
			}
			flags =
			{
				FORCE_SLEEP=true ONLY_GOLD=true DROP_NUMBER=getter.random(1,2)
				OPEN_DOOR=true BASH_DOOR=true ANIMAL=true
				MORTAL=true SEE_INVIS=15 AERIAL=true CAN_FLY=15 AI=ai.SPELL_AND_MOVE
				DROP_THEME=getter.flags{THEME_TREASURE=100}
				AI_STATES=getter.flags
				{
					MOVE_AI=ai.ZOMBIE
					CASTING_AI=ai.SMART_CAST
				}
				ALLOW_IN_MOUNTAIN=true
			}
			spells =
			{
				frequency = 8
				["Scare"] = { level=80 chance=85 }
			}
		}
		[8] =
		{
			name = "swamp thing"
			level = 17 rarity = 2
			desc = "A creature that was once human, but is now as green as moss."
			color = color.GREEN
			speed = 0 life = {12,12} ac = 35
			aaf = 20 sleep = 30
			exp = 80
			weight = 2000
			blows =
			{
				{"GAZE","FEAR",{0,0}},
				{"CLAW","POISON",{9,15}},
				{"CLAW","POISON",{9,15}},
			}
			flags =
			{
				RESIST=getter.resists{POIS=66 FEAR=100 FIRE=-66}
				CAN_SWIM=true OPEN_DOOR=true BASH_DOOR=true
				MORTAL=true AI=ai.ZOMBIE REGENERATE=1600 ALLOW_IN_SWAMP=true
			}
		}
		[9] =
		{
			name = "chimaera"
			level = 20 rarity = 2
			desc =
			{
				"It is a strange concoction of goat, lion and dragon, with the heads of all",
				"three beasts.",
			}
			color = color.RED
			speed = 10 life = {20,20} ac = 15
			aaf = 12 sleep = 10
			exp = 220
			weight = 1600
			blows =
			{
				{"BUTT","CRUSH",{10,10}},
				{"BITE","PIERCE",{13,10}},
				{"BITE","FIRE",{14,8}},
			}
			flags =
			{
				RESIST=getter.resists{COLD=-66 CRUSH=10 FIRE=66 PIERCE=10 SLASH=10}
				FORCE_SLEEP=true AERIAL=true CAN_FLY=3 BASH_DOOR=true
				MORTAL=true HAS_LITE=true AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags
				{
					MOVE_AI=ai.ZOMBIE
					CASTING_AI=ai.SMART_CAST
				}
				ALLOW_IN_MOUNTAIN=true
			}
			spells =
			{
				frequency = 10
				["Breathe Fire"] = { level=100 chance=100 }
			}
		}
		[10] =
		{
			name = "gorgimaera"
			level = 27 rarity = 2
			desc =
			{
				"The result of evil experiments, this travesty of nature should never be",
				"alive.  It has three heads - goat, dragon and gorgon - all attached to a",
				"lion's body.",
			}
			color = color.ORANGE
			speed = 0 life = {30,25} ac = 20
			aaf = 12 sleep = 10
			exp = 400
			weight = 2300
			blows =
			{
				{"BUTT","CRUSH",{12,15}},
				{"BITE","FIRE",{15,10}},
				{"GAZE","PARALYZE",{12,10}},
			}
			flags =
			{
				RESIST=getter.resists{COLD=-66 CRUSH=15 FIRE=66 PIERCE=15 SLASH=15}
				FORCE_SLEEP=true BASH_DOOR=true AERIAL=true CAN_FLY=3
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags
				{
					MOVE_AI=ai.ZOMBIE
					CASTING_AI=ai.SMART_CAST
				}
			}
			spells =
			{
				frequency = 8
				["Breathe Fire"] = { level=100 chance=100 }
			}
		}
		[11] =
		{
			name = "manticore"
			level = 30 rarity = 2
			desc =
			{
				"It is a winged lion's body with a human torso and a tail covered in",
				"vicious spikes.",
			}
			color = color.YELLOW
			speed = 10 life = {25,10} ac = 35
			aaf = 12 sleep = 10
			exp = 300
			weight = 1900
			blows =
			{
				{"CLAW","SLASH",{20,5}},
				{"CLAW","SLASH",{20,5}},
				{"BITE","PIERCE",{10,12}},
			}
			flags =
			{
				RESIST=getter.resists{DARK_FIRE=50}
				FORCE_SLEEP=true FORCE_MAXHP=true BASH_DOOR=true AERIAL=true CAN_FLY=3
				EVIL=true MORTAL=true AI=ai.SPELL_AND_MOVE
				AI_STATES=getter.flags
				{
					MOVE_AI=ai.ZOMBIE
					CASTING_AI=ai.SMART_CAST
				}
				ALLOW_IN_FOREST=true
			}
			spells =
			{
				frequency = 5
				["Arrow"] = { level=40 chance=100 }
			}
		}
		[12] =
		{
			name = "gorgon"
			level = 31 rarity = 2
			desc =
			{
				"A bull-like creature whose skin is made of steel plates. Watch out for",
				"its deadly breath!",
			}
			color = color.BLUE
			speed = 0 life = {30,30} ac = 30
			aaf = 12 sleep = 20
			exp = 275
			weight = 3000
			blows =
			{
				{"BUTT","PIERCE",{18,9}},
				{"BUTT","PIERCE",{18,9}},
				{"BITE","POISON",{15,10}},
				{"KICK","CRUSH",{7,12}},
			}
			flags =
			{
				RESIST=getter.resists{ACID=100 CRUSH=35 PIERCE=40 POIS=100 SLASH=40}
				FORCE_SLEEP=true ANIMAL=true MOVE_BODY=true SEE_INVIS=15
				BASH_DOOR=true MORTAL=true
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags
				{
					MOVE_AI=ai.ZOMBIE
					CASTING_AI=ai.SMART_CAST
				}
				ALLOW_IN_FOREST=true
			}
			spells =
			{
				frequency = 8
				["Breathe Poison"] = { level=100 chance=100 }
			}
		}
		[13] =
		{
			name = "minotaur"
			level = 40 rarity = 2
			desc =
			{
				"It is a cross between a human and a bull. It snorts as it spies",
				"you, and breaks into a run."
			}
			color = color.LIGHT_UMBER
			speed = 20 life = {100,20} ac = 35
			aaf = 13 sleep = 10
			exp = 2100
			weight = 15000
			blows =
			{
				{"BUTT","PIERCE",{14,12}},
				{"BUTT","CRUSH",{14,12}},
				{"BUTT","CRUSH",{10,6}},
			}
			flags =
			{
				RESIST=getter.resists{DARK_FIRE=50}
				BASH_DOOR=true EVIL=true MORTAL=true
				AI=ai.ZOMBIE
				DROP_THEME=getter.flags{THEME_COMBAT=70 THEME_JUNK=30}
			}
		}
		[15] =
		{
			name = "behemoth"
			level = 49 rarity = 3
			desc = "A great water-beast, with an almost unpenetrable skin."
			color = color.LIGHT_BLUE
			speed = 10 life = {20,100} ac = 40
			aaf = 25 sleep = 30
			exp = 16000
			weight = 6000
			blows =
			{
				{"BITE","FIRE",{14,14}},
				{"BITE","FIRE",{14,14}},
				{"CRUSH","CRUSH",{21,15}},
				{"CRUSH","CRUSH",{21,15}},
			}
			flags =
			{
				RESIST=getter.resists{ACID=100 COLD=100 CRUSH=50 CONFUSION=50 FEAR=100 FIRE=100 PIERCE=50 POIS=100 SLASH=50}
				FORCE_SLEEP=true FORCE_MAXHP=true CAN_SWIM=true ANIMAL=true AQUATIC=true
				NO_SLEEP=true MORTAL=true SEE_INVIS=15
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags
				{
					MOVE_AI=ai.ZOMBIE
					CASTING_AI=ai.SMART_CAST
				}
				ALLOW_IN_OCEAN=true ALLOW_IN_RIVER=true
			}
			spells =
			{
				frequency = 9
				["Breathe Fire"] = { level=100 chance=100 }
			}
		}
		[16] =
		{
			name = "maulotaur"
			level = 50 rarity = 2
			desc =
			{
				"It is a belligerent minotaur with a destructive magical arsenal, armed",
				"with a hammer. It bounds towards you, snorting spells.",
			}
			color = color.SLATE
			speed = 20 life = {250,20} ac = 45
			aaf = 13 sleep = 10
			exp = 4500
			weight = 40000
			blows =
			{
				{"HIT","CRUSH",{10,10}},
				{"HIT","CRUSH",{10,10}},
				{"BUTT","PIERCE",{8,6}},
				{"BUTT","PIERCE",{8,6}},
			}
			flags =
			{
				RESIST=getter.resists{FIRE=100 DARK_FIRE=50}
				ONLY_ITEM=true DROP_60=true DROP_GOOD=true RES_TELE=true BASH_DOOR=true
				STUPID=true EVIL=true FORCE_SLEEP=true FORCE_MAXHP=true
				MANA_CURR=150 MANA_MAX=150 MANA_REGENERATE=4500
				MORTAL=true AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags
				{
					MOVE_AI=ai.ZOMBIE
					CASTING_AI=ai.SMART_CAST
				}
				DROP_THEME=getter.flags{THEME_MAGIC=40 THEME_COMBAT=30 THEME_TREASURE=30}
			}
			spells =
			{
				frequency = 5
				["Bolt of Darkfire"] = { level=32 chance=80 }
				["Fireflash"] = { level=40 chance=85 }
				["Strike"] = { level=36 chance=95 }
			}
		}
		[17] =
		{
			name = "the minotaur lord"
			level = 58 rarity = 4
			desc =
			{
				"A fearsome bull-headed monster, he swings a mighty axe as he curses",
				"all that defy him.",
			}
			color = color.VIOLET
			speed = 20 life = {36,100} ac = 60
			aaf = 30 sleep = 30
			exp = 18000
			weight = 16000
			blows =
			{
				{"BUTT","PIERCE",{12,13}},
				{"BUTT","CRUSH",{12,13}},
				{"HIT","SLASH",{12,12}},
				{"HIT","SLASH",{12,12}},
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=10 FIRE=100 DARK_FIRE=50 PIERCE=10 POIS=100 SLASH=10}
				UNIQUE=true MALE=true FORCE_SLEEP=true FORCE_MAXHP=true BASH_DOOR=true
				MANA_CURR=500 MANA_MAX=500 MANA_REGENERATE=8000
				EVIL=true MORTAL=true AI=ai.SPELL_AND_MOVE
				AI_STATES=getter.flags
				{
					MOVE_AI=ai.ZOMBIE
					CASTING_AI=ai.SMART_CAST
					FEAR_LEVEL=20
				}
				DROP_THEME=getter.flags{THEME_MAGIC=40 THEME_COMBAT=30 THEME_TREASURE=30}
			}
			spells =
			{
				frequency = 6
				["Arrow"] = { level=18 chance=100 }
				["Ball of Electricity"] = { level=94 chance=99 }
				["Bolt of Darkfire"] = { level=35 chance=96 }
				["Breathe Darkfire"] = { level=100 chance=100 }
				["Manathrust"] = { level=30 chance=98 }
				["Slow"] = { level=80 chance=90 }
				["Strike"] = { level=40 chance=95 }
			}
		}
	}
	['i'] =
	{
		defaults =
		{
			body = default_body.none
			flags =
			{
				ALLOW_IN_EVERYWHERE=true ALLOW_IN_SWAMP=true
				FACTION=FACTION_ANIMAL DROP_CORPSE=true
			}
		}
		[1] =
		{
			name = "clear icky thing"
			level = 1 rarity = 1
			desc = "It is a smallish, slimy, icky, blobby creature."
			color = color.LIGHT_BLUE
			speed = 0 life = {4,5} ac = 6
			aaf = 12 sleep = 10
			exp = 2
			weight = 500
			blows =
			{
				{"TOUCH","POISON",{3,2}},
			}
			flags =
			{
				ATTR_CLEAR=true CAN_SWIM=true INVISIBLE=true
				EMPTY_MIND=true REGENERATE=1000
				AI=ai.RANDOM_MOVE AI_STATES=getter.flags
				{
					MOVE_RATE=75
				}
			}
		}
		[2] =
		{
			name = "white icky thing"
			level = 1 rarity = 1
			desc = "It is a smallish, slimy, icky creature."
			color = color.WHITE
			speed = 0 life = {2,5} ac = 7
			aaf = 12 sleep = 10
			exp = 1
			weight = 500
			blows =
			{
				{"TOUCH","POISON",{3,2}},
			}
			flags =
			{
				CAN_SWIM=true EMPTY_MIND=true
				REGENERATE=1000  AI=ai.RANDOM_MOVE AI_STATES=getter.flags
				{
					MOVE_RATE=75
				}
			}
		}
		[3] =
		{
			name = "blubbering icky thing"
			level = 2 rarity = 1
			desc =
			{
				"It is a larger, slimy, icky, hungry creature. It makes a horrible",
				"squelching sound as it moves, leaving trails of mucus behind it.",
			}
			color = color.LIGHT_WHITE
			speed = 0 life = {8,6} ac = 4
			aaf = 14 sleep = 10
			exp = 8
			weight = 400
			blows =
			{
				{"CRAWL","POISON",{6,4}},
				{"CRAWL","EAT_FOOD",{0,0}},
				{"DROOL","*",{0,0}},
				{"DROOL","*",{0,0}},
			}
			flags =
			{
				RESIST=getter.resists{POIS=100}
				DROP_90=true CAN_SWIM=true EMPTY_MIND=true
				TAKE_ITEM=true KILL_BODY=true REGENERATE=1000
				AI=ai.RANDOM_MOVE AI_STATES=getter.flags
				{
					MOVE_RATE=50
				}
			}
		}
		[4] =
		{
			name = "green icky thing"
			level = 7 rarity = 2
			desc = "It is a smallish, slimy, icky, acidic creature."
			color = color.GREEN
			speed = 0 life = {5,8} ac = 10
			aaf = 14 sleep = 20
			exp = 18
			weight = 500
			blows =
			{
				{"TOUCH","ACID",{12,5}},
			}
			flags =
			{
				RESIST=getter.resists{ACID=100}
				CAN_SWIM=true EMPTY_MIND=true
				KILL_ITEM=true AI=ai.SNAKE
			}
		}
		[5] =
		{
			name = "bloodshot icky thing"
			level = 9 rarity = 3
			desc =
			{
				"It is a strange, slimy, icky creature. Veins pop out of",
				"its surface and pulse obscenely."
			}
			color = color.RED
			speed = 0 life = {7,8} ac = 11
			aaf = 14 sleep = 20
			exp = 24
			weight = 60
			blows =
			{
				{"TOUCH","POISON",{3,4}},
				{"CRAWL","ACID",{6,4}},
			}
			flags =
			{
				RESIST=getter.resists{POIS=100}
				EMPTY_MIND=true CAN_SWIM=true
				REGENERATE=1000 AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags
				{
					CASTING_AI=ai.SMART_CAST
					MOVE_AI=ai.RANDOM_MOVE
					MOVE_RATE=50
				}
			}
			spells =
			{
				frequency = 11
				["Drain Mana"] = { level=80 chance=74 }
			}
		}
		[6] =
		{
			name = "blue icky thing"
			level = 14 rarity = 4
			desc =
			{
				"It is a strange, slimy, icky creature, with rudimentary intelligence,",
				"and evil cunning. It hungers for food, and you look tasty.",
			}
			color = color.BLUE
			speed = -10 life = {10,8} ac = 15
			aaf = 15 sleep = 20
			exp = 20
			weight = 600
			blows =
			{
				{"CRAWL","POISON",{12,4}},
				{"CRAWL","EAT_FOOD",{0,0}},
				{"HIT","CRUSH",{3,4}},
				{"HIT","CRUSH",{3,4}},
			}
			flags =
			{
				RESIST=getter.resists{DARK_FIRE=50 POIS=100}
				FORCE_SLEEP=true OPEN_DOOR=true BASH_DOOR=true CAN_SWIM=true
				EVIL=true MULTIPLY=true REGENERATE=1000
				MANA_CURR=50 MANA_MAX=50 MANA_REGENERATE=800
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags
				{
					CASTING_AI=ai.SMART_CAST
					MOVE_AI=ai.RANDOM_MOVE
					MOVE_RATE=50
				}
			}
			spells =
			{
				frequency = 8
				["Blind"] = { level=32 chance=82 }
				["Confuse"] = { level=34 chance=80 }
				["Scare"] = { level=38 chance=84 }
			}
		}
		[7] =
		{
			name = "the icky queen"
			level = 20 rarity = 5
			desc =
			{
					"And you thought her offspring were icky! This massive, bloated lump",
					"of flesh oozes along the floor, flickering with tiny electrical currents",
					"across its surface. You can also see a multicoloured array of icky things",
					"pulsing obscenely underneath its skin!",
			}
			color = color.VIOLET
			speed = 10 life = {40,30} ac = 20
			aaf = 20 sleep = 10
			exp = 400
			weight = 3000
			blows =
			{
				{"CRAWL","POISON",{7,7}},
				{"CRAWL","EAT_FOOD",{0,0}},
				{"TOUCH","ACID",{14,14}},
			}
			flags =
			{
				RESIST=getter.resists{ACID=100 COLD=100 ELEC=100 FIRE=100 DARK_FIRE=50 POIS=100}
				UNIQUE=true FEMALE=true FORCE_MAXHP=true SMART=true
				ESCORT=true ESCORTED_BY_GROUPS=true
				WEIRD_MIND=90 TAKE_ITEM=true OPEN_DOOR=true BASH_DOOR=true
				MANA_CURR=80 MANA_MAX=80 MANA_REGENERATE=1200
				EVIL=true MORTAL=true REGENERATE=1800 AI=ai.SPELL_AND_MOVE
				DROP_THEME=getter.flags{THEME_MAGIC=30 THEME_JUNK=10 THEME_TREASURE=30 THEME_COMBAT=30}
				AI_STATES=getter.flags
				{
					CASTING_AI=ai.SMART_CAST
					MOVE_AI=ai.ZOMBIE
				}
			}
			spells =
			{
				frequency = 5
				["Blind"] = { level=60 chance=97 }
				["Confuse"] = { level=49 chance=90 }
				["Drain Mana"] = { level=80 chance=91 }
				["Scare"] = { level=60 chance=99 }
				["Summon Kin"] = { level=90 chance=98 }
			}
		}
	}
	['I'] =
	{
		defaults =
		{
			body = default_body.none
			flags =
			{
				ALLOW_IN_EVERYWHERE=true FACTION=FACTION_ANIMAL
				ANIMAL=true SEE_INVIS=15 COLD_BLOOD=true
			}
		}
		[1] =
		{
			name = "swarm of midges"
			level = 1 rarity = 1
			desc =
			{
				"A single midge may be harmless, but there's a whole swarm of",
				"them here, and it seems to be growing!",
			}
			color = color.UMBER
			speed = -10 life = {1,5} ac = 4
			aaf = 20 sleep = 10
			exp = 1
			weight = 100
			blows =
			{
				{"STING","PIERCE",{3,1}},
			}
			flags =
			{
				RESIST=getter.resists{FIRE=-66}
				WEIRD_MIND=90 AERIAL=true CAN_FLY=3
				MORTAL=true NO_CUT=true MULTIPLY=true
				AI=ai.RANDOM_MOVE
				AI_STATES=getter.flags
				{
					MOVE_RATE=25
				}
				ALLOW_IN_PLAIN=true ALLOW_IN_FOREST=true ALLOW_IN_SWAMP=true
			}
		}
		[2] =
		{
			name = "giant cockroach"
			level = 3 rarity = 1
			desc =
			{
				"It is six inches long with long feathery antennae. No wait, there's",
				"another one!",
			}
			color = color.CHOCOLATE
			speed = 5 life = {1,1} ac = 15
			aaf = 6 sleep = 20
			exp = 1
			weight = 100
			blows =
			{
				{"BITE","PIERCE",{3,1}},
			}
			flags =
			{
				MULTIPLY=true WEIRD_MIND=90
				MORTAL=true
				AI=ai.RANDOM_MOVE AI_STATES=getter.flags
				{
					MOVE_RATE=75
				}
				ALLOW_IN_PLAIN=true ALLOW_IN_FOREST=true ALLOW_IN_SWAMP=true SPEED=getter.speeds{WALK=5}
			}
		}
		[3] =
		{
			name = "killer bee"
			level = 9 rarity = 2
			desc = "It is poisonous and aggressive."
			color = color.YELLOW
			speed = 0 life = {2,4} ac = 25
			aaf = 12 sleep = 10
			exp = 22
			weight = 50
			blows =
			{
				{"STING","POISON",{6,4}},
			}
			flags =
			{
				RESIST=getter.resists{POIS=66}
				WEIRD_MIND=90 FRIENDS=getter.friends(60,12) AERIAL=true CAN_FLY=3 SPEED=getter.speeds{WALK=10}
				MORTAL=true	ALLOW_IN_PLAIN=true ALLOW_IN_FOREST=true
				AI=ai.ZOMBIE AI_STATES=getter.flags{LURE_FROM_CORRIDOR=true}
			}
		}
		[4] =
		{
			name = "giant fruit fly"
			level = 10 rarity = 3
			desc = "A fast-breeding, annoying pest."
			color = color.LIGHT_GREEN
			speed = 10 life = {2,2} ac = 25
			aaf = 8 sleep = 10
			exp = 4
			weight = 100
			blows =
			{
				{"BITE","PIERCE",{6,2}},
			}
			flags =
			{
				AERIAL=true CAN_FLY=3 WEIRD_MIND=90
				MORTAL=true MULTIPLY=true
				AI=ai.RANDOM_MOVE AI_STATES=getter.flags
				{
					MOVE_RATE=75
				}
			}
		}
		[6] =
		{
			name = "giant black louse"
			level = 14 rarity = 1
			desc = "It is six inches long."
			color = color.LIGHT_DARK
			speed = 10 life = {1,2} ac = 15
			aaf = 6 sleep = 10
			exp = 3
			weight = 100
			blows =
			{
				{"BITE","PIERCE",{6,2}},
			}
			flags =
			{
				AERIAL=true CAN_FLY=3 WEIRD_MIND=90
				MORTAL=true MULTIPLY=true
				AI=ai.RANDOM_MOVE AI_STATES=getter.flags
				{
					MOVE_RATE=75
				}
			}
		}
		[7] =
		{
			name = "giant flea"
			level = 14 rarity = 3
			desc = "It makes you itch just to look at it."
			color = color.SLATE
			speed = 10 life = {2,2} ac = 20
			aaf = 6 sleep = 10
			exp = 3
			weight = 90
			blows =
			{
				{"BITE","PIERCE",{3,6}},
			}
			flags =
			{
				WEIRD_MIND=90 MORTAL=true MULTIPLY=true
				AI=ai.RANDOM_MOVE AI_STATES=getter.flags
				{
					MOVE_RATE=50
				}
			}
		}
		[9] =
		{
			name = "hummerhorn"
			level = 16 rarity = 4
			desc = "A giant buzzing wasp, its stinger drips venom."
			color = color.YELLOW
			speed = 20 life = {2,2} ac = 30
			aaf = 8 sleep = 10
			exp = 15
			weight = 100
			blows =
			{
				{"BITE","PIERCE",{6,2}},
				{"STING","PARALYZE",{9,3}},
			}
			flags =
			{
				AERIAL=true CAN_FLY=3 WEIRD_MIND=90
				AI=ai.RANDOM_MOVE AI_STATES=getter.flags
				{
					MOVE_RATE=75
				}
			}
		}
		[10] =
		{
			name = "neekerbreeker"
			level = 19 rarity = 4
			desc =
			{
				"Believed to be an evil relative of the cricket, this creature gets its name",
				"from its incessant squeaking, which can best be described as \"neek-breek,",
				"neek-breek\".  The noise can drive people frantic, and worse still, can be",
				"heard for quite some distance, alerting other monsters to your presence.",
			}
			color = color.LIGHT_DARK
			speed = 10 life = {3,2} ac = 25
			aaf = 8 sleep = 10
			exp = 4
			weight = 100
			blows =
			{
				{"BITE","POISON",{3,4}},
			}
			flags =
			{
				RESIST=getter.resists{DARK_FIRE=50}
				WEIRD_MIND=90 ALLOW_IN_SWAMP=true
				EVIL=true MULTIPLY=true
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags
				{
					MOVE_AI=ai.RANDOM_MOVE
					CASTING_AI=ai.SMART_CAST
					MOVE_RATE=75
				}
			}
			spells =
			{
				frequency = 12
				["Shriek"] = { level=50 chance=96 }
			}
		}
		[11] =
		{
			name = "giant firefly"
			level = 24 rarity = 4
			desc =
			{
				"Clouds of these monsters light up the dungeon - so brightly that you can",
				"barely see through them.",
			}
			color = color.RED
			speed = 10 life = {3,2} ac = 18
			aaf = 8 sleep = 10
			exp = 6
			weight = 100
			blows =
			{
				{"BITE","BLIND",{3,6}},
			}
			flags =
			{
				AERIAL=true CAN_FLY=3 WEIRD_MIND=90
				MULTIPLY=true HAS_LITE=true
				AI=ai.RANDOM_MOVE AI_STATES=getter.flags
				{
					MOVE_RATE=75
				}
			}
		}
	}
	['j'] =
	{
		defaults =
		{
			body = default_body.blob
			flags =
			{
				ALLOW_IN_EVERYWHERE=true COLD_BLOOD=true SYMBIOTE=true
			}
		}
		[1] =
		{
			name = "white jelly"
			level = 2 rarity = 1
			desc = "It's a large pile of white flesh."
			color = color.WHITE
			speed = 10 life = {8,8} ac = 1
			aaf = 2 sleep = 99
			exp = 10
			weight = 2000
			blows =
			{
				{"TOUCH","POISON",{3,2}},
			}
			flags =
			{
				RESIST=getter.resists{CONFUSION=50 DARK=22 FEAR=100 LITE=-100 POIS=100}
				NEVER_MOVE=true CAN_SWIM=true STUPID=true EMPTY_MIND=true NO_SLEEP=true NO_CUT=true
				REGENERATE=700 AI=ai.NEVER_MOVE
				SYMB_BONUS=getter.flags{REGENERATE=700}
				SYMB_BONUS_SCALED = getter.flags{RESIST=getter.resists{POIS=100}}

			}
		}
		[2] =
		{
			name = "green ooze"
			level = 3 rarity = 2
			desc = "It's green and it's oozing."
			color = color.GREEN
			speed = 10 life = {3,4} ac = 10
			aaf = 8 sleep = 80
			exp = 4
			weight = 300
			blows =
			{
				{"CRAWL","ACID",{6,3}},
			}
			flags =
			{
				RESIST=getter.resists{ACID=100 CONFUSION=50 FEAR=100 POIS=100}
				DROP_90=true STUPID=true EMPTY_MIND=true
				CAN_SWIM=true NO_SLEEP=true NO_CUT=true AI=ai.RANDOM_MOVE
				AI_STATES=getter.flags
				{
					MOVE_RATE=75 FEAR_LEVEL=0
				}
				SYMB_BONUS_SCALED = getter.flags{RESIST=getter.resists{ACID=100 POIS=100}}
			}
		}
		[3] =
		{
			name = "silver jelly"
			level = 3 rarity = 2
			desc =
			{
				"It is a large pile of silver flesh that sucks all light from its",
				"surroundings.",
			}
			color = color.LIGHT_WHITE
			speed = 10 life = {10,10} ac = 1
			aaf = 2 sleep = 99
			exp = 12
			weight = 2000
			blows =
			{
				{"TOUCH","EAT_LITE",{6,3}},
				{"TOUCH","EAT_LITE",{6,3}},
			}
			flags =
			{
				RESIST=getter.resists{CONFUSION=50 CONFUSION=50 FEAR=100 DARK=22 POIS=100}
				NEVER_MOVE=true CAN_SWIM=true STUPID=true EMPTY_MIND=true NO_SLEEP=true NO_CUT=true
				REGENERATE=600 AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags
				{
					MOVE_AI=ai.NEVER_MOVE
					CASTING_AI=ai.SMART_CAST
					FEAR_LEVEL=0
				}
				SYMB_BONUS=getter.flags{REGENERATE=700 RESIST=getter.resists{LITE=-77}}
				SYMB_BONUS_SCALED = getter.flags{RESIST=getter.resists{DARK=22 POIS=100}}
			}
			spells =
			{
				frequency = 15
				["Drain Mana"] = { level=80 chance=76 }
			}
		}
		[4] =
		{
			name = "yellow jelly"
			level = 3 rarity = 1
			desc = "It's a large pile of yellow flesh, glistening wetly."
			color = color.YELLOW
			speed = 10 life = {10,8} ac = 1
			aaf = 2 sleep = 99
			exp = 12
			weight = 2000
			blows =
			{
				{"TOUCH","POISON",{6,3}},
			}
			flags =
			{
				RESIST=getter.resists{CONFUSION=50 DARK=22 FEAR=100 LITE=-100 POIS=100}
				NEVER_MOVE=true CAN_SWIM=true STUPID=true EMPTY_MIND=true NO_SLEEP=true
				NO_CUT=true BLOW_RESPONSE=getter.array{[AURA_POIS]={2,4}}
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags
				{
					MOVE_AI=ai.NEVER_MOVE
					CASTING_AI=ai.SMART_CAST
					FEAR_LEVEL=0
				}
				SYMB_BONUS=getter.flags{BLOW_RESPONSE=getter.array{[AURA_POIS]={6,4}}}
				SYMB_BONUS_SCALED = getter.flags{RESIST=getter.resists{POIS=100}}
			}
			spells =
			{
				frequency = 15
				["Drain Mana"] = { level=80 chance=68 }
			}
		}
		[5] =
		{
			name = "blue jelly"
			level = 4 rarity = 1
			desc =
			{
				"It's a large pile of pulsing blue flesh, hoar frost glistening.",
				"along its base.",
			}
			color = color.BLUE
			speed = 0 life = {12,10} ac = 1
			aaf = 2 sleep = 99
			exp = 14
			weight = 2000
			blows =
			{
				{"TOUCH","COLD",{2,6}},
			}
			flags =
			{
				RESIST=getter.resists{COLD=100 CONFUSION=50 DARK=22 FEAR=100 FIRE=-66 LITE=-100}
				NEVER_MOVE=true COLD_BLOOD=true STUPID=true EMPTY_MIND=true CAN_SWIM=true
				NO_SLEEP=true NO_CUT=true REGENERATE=700 AI=ai.NEVER_MOVE
				SYMB_BONUS_SCALED = getter.flags{REGENERATE=700 RESIST=getter.resists{COLD=100}}
			}
		}
		[6] =
		{
			name = "blue ooze"
			level = 5 rarity = 1
			desc = "It's blue and it's oozing."
			color = color.BLUE
			speed = 0 life = {3,4} ac = 10
			aaf = 8 sleep = 80
			exp = 7
			weight = 300
			blows =
			{
				{"CRAWL","COLD",{3,9}},
			}
			flags =
			{
				RESIST=getter.resists{COLD=100 CONFUSION=50 FEAR=100 FIRE=-66}
				DROP_60=true STUPID=true EMPTY_MIND=true
				CAN_SWIM=true NO_SLEEP=true NO_CUT=true AI=ai.RANDOM_MOVE
				AI_STATES=getter.flags{MOVE_RATE=75 FEAR_LEVEL=0}
				SYMB_BONUS_SCALED=getter.flags{RESIST=getter.resists{COLD=100}}
			}
		}
		[7] =
		{
			name = "green jelly"
			level = 5 rarity = 1
			desc =
			{
				"It is a large pile of pulsing green flesh. The ground",
				"bubbles near its base.",
			}
			color = color.GREEN
			speed = 10 life = {22,10} ac = 1
			aaf = 2 sleep = 99
			exp = 18
			weight = 2500
			blows =
			{
				{"TOUCH","ACID",{6,3}},
			}
			flags =
			{
				RESIST=getter.resists{ACID=100 CONFUSION=50 DARK=22 FEAR=100 LITE=-100}
				NEVER_MOVE=true STUPID=true EMPTY_MIND=true CAN_SWIM=true NO_SLEEP=true
				NO_CUT=true REGENERATE=700 BLOW_RESPONSE=getter.array{[AURA_ACID]={2,4}}
				AI=ai.NEVER_MOVE
				SYMB_BONUS=getter.flags{BLOW_RESPONSE=getter.array{[AURA_ACID]={6,4}}}
				SYMB_BONUS_SCALED = getter.flags{RESIST=getter.resists{ACID=100} REGENERATE=700}
			}
		}
		[8] =
		{
			name = "rot jelly"
			level = 5 rarity = 1
			desc =
			{
				"It is a large pile of rotting flesh, whose touch spoils your food.  The terrible",
				"smell it exudes is also very hard to get rid of...",
			}
			color = color.UMBER
			speed = 10 life = {20,12} ac = 1
			aaf = 2 sleep = 99
			exp = 13
			weight = 2000
			blows =
			{
				{"TOUCH","EAT_FOOD",{9,3}},
				{"TOUCH","LOSE_CHR",{6,3}},
			}
			flags =
			{
				RESIST=getter.resists{CONFUSION=50 DARK=22 FEAR=100 LITE=-100 POIS=100}
				NEVER_MOVE=true STUPID=true EMPTY_MIND=true CAN_SWIM=true NO_SLEEP=true NO_CUT=true
				REGENERATE=500 AI=ai.NEVER_MOVE
				SYMB_BONUS_SCALED = getter.flags{REGENERATE=500 RESIST=getter.resists{POIS=100}}
			}
		}
		[9] =
		{
			name = "red jelly"
			level = 7 rarity = 1
			desc = "It is a large pulsating mound of red flesh."
			color = color.RED
			speed = 0 life = {26,8} ac = 1
			aaf = 2 sleep = 99
			exp = 26
			weight = 2500
			blows =
			{
				{"TOUCH","LOSE_STR",{3,5}},
			}
			flags =
			{
				RESIST=getter.resists{CONFUSION=50 DARK=22 FEAR=100 LITE=-100}
				NEVER_MOVE=true STUPID=true EMPTY_MIND=true CAN_SWIM=true
				NO_SLEEP=true REGENERATE=700 AI=ai.NEVER_MOVE
				SYMB_BONUS=getter.flags{SUST_STATS = getter.stats{[A_STR]=true}}
				SYMB_BONUS_SCALED = getter.flags{REGENERATE=700}
			}
		}
		[10] =
		{
			name = "undead mass"
			level = 10 rarity = 2
			desc =
			{
				"A sickening mound of decaying flesh, bones, hands and so on. It seems to",
				"be growing.",
			}
			color = color.UMBER
			speed = 0 life = {8,8} ac = 4
			aaf = 70 sleep = 5
			exp = 33
			weight = 200
			blows =
			{
				{"TOUCH","DISEASE",{3,6}},
				{"TOUCH","LOSE_CON",{3,6}},
			}
			flags =
			{
				RESIST=getter.resists{COLD=100 CONFUSION=50 DARK=22 FEAR=100 LITE=-100 DARK_FIRE=100 POIS=100}
				UNDEAD=true NONLIVING=true EMPTY_MIND=true
				NO_SLEEP=true COLD_BLOOD=true EVIL=true FACTION=FACTION_UNDEAD
				NO_CUT=true MULTIPLY=true SEE_INVIS=15 AI=ai.NEVER_MOVE
			}
				default_flags_to_ignore = {SYMBIOTE=true}
		}
		[11] =
		{
			name = "grape jelly"
			level = 12 rarity = 3
			desc = "Yum! It looks quite tasty.  It is a pulsing mound of glowing flesh."
			color = color.VIOLET
			speed = 0 life = {52,10} ac = 1
			aaf = 2 sleep = 99
			exp = 60
			weight = 2600
			blows =
			{
				{"TOUCH","EXP_10",{0,0}},
			}
			flags =
			{
				RESIST=getter.resists{CONFUSION=50 DARK=22 FEAR=100 LITE=-100 POIS=100}
				NEVER_MOVE=true STUPID=true EMPTY_MIND=true CAN_SWIM=true NO_SLEEP=true
				NO_CUT=true REGENERATE=700 AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags
				{
					MOVE_AI=ai.NEVER_MOVE
					CASTING_AI=ai.SMART_CAST
					FEAR_LEVEL=0
				}
				SYMB_BONUS_SCALED = getter.flags{REGENERATE=700 MANA=5 RESIST=getter.resists{POIS=100}}
			}
			spells =
			{
				frequency = 11
				["Drain Mana"] = { level=80 chance=87 }
			}
		}
		[12] =
		{
			name = "spotted jelly"
			level = 12 rarity = 3
			desc = "A strange jelly thing, covered in discoloured blotches."
			color = color.ORANGE
			speed = 10 life = {13,8} ac = 18
			aaf = 12 sleep = 1
			exp = 20
			weight = 2500
			blows =
			{
				{"TOUCH","ACID",{6,10}},
				{"TOUCH","ACID",{6,10}},
			}
			flags =
			{
				RESIST=getter.resists{ACID=100 CONFUSION=50 DARK=22 FEAR=100 LITE=-100 POIS=100}
				NEVER_MOVE=true STUPID=true EMPTY_MIND=true COLD_BLOOD=true CAN_SWIM=true
				NO_SLEEP=true NO_CUT=true BLOW_RESPONSE=getter.array{[AURA_ACID]={6,3}}
				REGENERATE=700 AI=ai.NEVER_MOVE
				SYMB_BONUS=getter.flags{BLOW_RESPONSE=getter.array{[AURA_ACID]={8,5}}}
				SYMB_BONUS_SCALED = getter.flags{RESIST=getter.resists{ACID=100 POIS=100} REGENERATE=700}
			}
		}
		[13] =
		{
			name = "ochre jelly"
			level = 13 rarity = 3
			desc =
			{
				"A fast moving highly acidic jelly thing, that is eating away the floor it",
				"rests on.",
			}
			color = color.LIGHT_UMBER
			speed = 10 life = {13,10} ac = 15
			aaf = 12 sleep = 1
			exp = 40
			weight = 2300
			blows =
			{
				{"TOUCH","ACID",{6,10}},
				{"TOUCH","ACID",{6,10}},
			}
			flags =
			{
				RESIST=getter.resists{ACID=100 CONFUSION=50 FEAR=100 POIS=100}
				STUPID=true EMPTY_MIND=true COLD_BLOOD=true TAKE_ITEM=true OPEN_DOOR=true
				BASH_DOOR=true CAN_SWIM=true NO_SLEEP=true NO_CUT=true
				REGENERATE=700 AI=ai.ZOMBIE
				SYMB_BONUS_SCALED = getter.flags{RESIST=getter.resists{ACID=100 POIS=100} REGENERATE=700 SPEED=10}
			}
		}
		[14] =
		{
			name = "gibbering mouther"
			level = 14 rarity = 4
			desc =
			{
				"A chaotic mass of pulsating flesh, mouths and eyes. The mouths",
				"mutter spells whilst the eyes glare at you with hatred.",
			}
			color = color.ORANGE
			speed = 0 life = {8,6} ac = 8
			aaf = 15 sleep = 20
			exp = 20
			weight = 2600
			blows =
			{
				{"CRAWL","POISON",{7,7}},
			}
			flags =
			{
				RESIST=getter.resists{FEAR=50 DARK_FIRE=50 POIS=100}
				NEVER_MOVE=true EVIL=true CAN_SWIM=true EMPTY_MIND=true
				NO_CUT=true MULTIPLY=true REGENERATE=600
				MANA_CURR=40 MANA_MAX=40 MANA_REGENERATE=800
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags
				{
					MOVE_AI=ai.NEVER_MOVE
					CASTING_AI=ai.SMART_CAST
					FEAR_LEVEL=0
				}
				SYMB_BONUS_SCALED = getter.flags{RESIST=getter.resists{POIS=100} REGENERATE=600}
				SYMB_BONUS=getter.flags{RESIST=getter.resists{DARK_FIRE=50} EVIL=true}
			}
			spells =
			{
				frequency = 7
				["Breathe Lite"] = { level=100 chance=84 }
				["Confuse"] = { level=34 chance=80 }
				["Scare"] = { level=38 chance=84 }
			}
		}
		[15] =
		{
			name = "gelatinous cube"
			level = 16 rarity = 4
			desc =
			{
				"It is a strange, vast gelatinous structure that assumes cubic proportions",
				"as it lines all four walls of the corridors it patrols. Through its",
				"transparent jelly structure you can see treasures it has engulfed, and a",
				"few corpses as well.",
			}
			color = color.LIGHT_GREEN
			speed = 0 life = {36,18} ac = 10
			aaf = 12 sleep = 1
			exp = 80
			weight = 40000
			blows =
			{
				{"TOUCH","ACID",{6,9}},
				{"TOUCH","ACID",{6,9}},
			}
			flags =
			{
				RESIST=getter.resists{ACID=100 COLD=100 CONFUSION=50 FEAR=50 ELEC=100 FIRE=100}
				FORCE_MAXHP=true STUPID=true EMPTY_MIND=true
				COLD_BLOOD=true TAKE_ITEM=true OPEN_DOOR=true BASH_DOOR=true CAN_SWIM=true
				NO_SLEEP=true NO_CUT=true REGENERATE=1100 BLOW_RESPONSE=getter.array{[AURA_ACID]={6,6}}
				DROP_THEME=getter.flags{THEME_MAGIC=40 THEME_COMBAT=30 THEME_TREASURE=30}
				AI=ai.ZOMBIE
				SYMB_BONUS_SCALED = getter.flags{RESIST=getter.resists{ACID=100 ELEC=100 COLD=100 FIRE=100} REGENERATE=1100 BLOW_RESPONSE=getter.array{[AURA_ACID]={6,6}}}
			}
		}
		[16] =
		{
			name = "black ooze"
			level = 23 rarity = 1
			desc = "It is a strangely moving puddle of darkness."
			color = color.LIGHT_DARK
			speed = -20 life = {6,8} ac = 6
			aaf = 10 sleep = 1
			exp = 7
			weight = 400
			blows =
			{
				{"TOUCH","ACID",{6,6}},
			}
			flags =
			{
				RESIST=getter.resists{DARK=22 FEAR=100 POIS=100}
				DROP_60=true STUPID=true EMPTY_MIND=true CAN_SWIM=true
				TAKE_ITEM=true KILL_BODY=true OPEN_DOOR=true BASH_DOOR=true MORTAL=true
				NO_CUT=true MULTIPLY=true AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags
				{
					MOVE_AI=ai.NEVER_MOVE
					CASTING_AI=ai.SMART_CAST
					FEAR_LEVEL=0
				}
				SYMB_BONUS_SCALED = getter.flags{RESIST=getter.resists{POIS=100}}
			}
			spells =
			{
				frequency = 11
				["Drain Mana"] = { level=80 chance=92 }
			}
		}
		[17] =
		{
			name = "acidic cytoplasm"
			level = 35 rarity = 5
			desc = "A disgusting animated blob of destruction. Flee its gruesome hunger!"
			color = color.SLATE
			speed = 10 life = {40,10} ac = 18
			aaf = 12 sleep = 1
			exp = 180
			weight = 3000
			blows =
			{
				{"TOUCH","ACID",{6,9}},
				{"TOUCH","ACID",{6,9}},
				{"TOUCH","ACID",{6,9}},
				{"TOUCH","ACID",{6,9}},
			}
			flags =
			{
				RESIST=getter.resists{ACID=100 COLD=100 CONFUSION=50 ELEC=100 FEAR=100 FIRE=100 POIS=100}
				FORCE_MAXHP=true TAKE_ITEM=true COLD_BLOOD=true 
				CAN_SWIM=true STUPID=true EMPTY_MIND=true OPEN_DOOR=true BASH_DOOR=true
				NO_SLEEP=true NO_CUT=true BLOW_RESPONSE=getter.array{[AURA_ACID]={7,7}} REGENERATE=1100
				DROP_THEME=getter.flags{THEME_MAGIC=40 THEME_COMBAT=30 THEME_TREASURE=30}
				AI=ai.ZOMBIE
				SYMB_BONUS_SCALED = getter.flags{RESIST=getter.resists{ACID=100 ELEC=100 COLD=100 FIRE=100} REGENERATE=1100 SPEED=10 BLOW_RESPONSE=getter.array{[AURA_ACID]={9,9}}}
			}
		}
	}
	['J'] =
	{
		defaults =
		{
			body = default_body.worms
			flags =
			{
				ALLOW_IN_EVERYWHERE=true FACTION=FACTION_ANIMAL
				ANIMAL=true SEE_INVIS=15 COLD_BLOOD=true
				DROP_SKELETON=true DROP_CORPSE=true
			}
		}
		[1] =
		{
			name = "giant kingsnake"
			level = 1 rarity = 1
			desc =
			{
				"It is about eight feet long and has colourful bands",
				"down the length of its body"
			}
			color = color.ORANGE
			speed = 0 life = {4,6} ac = 20
			aaf = 4 sleep = 99
			exp = 3
			weight = 800
			blows =
			{
				{"BITE","PIERCE",{6,6}},
			}
			flags =
			{
				CAN_SWIM=true BASH_DOOR=true
				HAS_EGG=true MORTAL=true AI=ai.SNAKE
			}
		}
		[2] =
		{
			name = "small boa"
			level = 1 rarity = 1
			desc = "It is about twelve feet long."
			color = color.WHITE
			speed = -5 life = {6,6} ac = 20
			aaf = 4 sleep = 99
			exp = 2
			weight = 600
			blows =
			{
				{"BITE","PIERCE",{6,2}},
				{"CRUSH","CRUSH",{9,6}},
			}
			flags =
			{
				BASH_DOOR=true HAS_EGG=true MORTAL=true AI=ai.SNAKE
			}
		}
		[3] =
		{
			name = "giant hognose"
			level = 2 rarity = 1
			desc = "It is about ten feet long with a heavy snout"
			color = color.YELLOW
			speed = -5 life = {5,8} ac = 20
			aaf = 5 sleep = 75
			exp = 9
			weight = 1000
			blows =
			{
				{"BITE","PIERCE",{6,6}},
				{"BUTT","CRUSH",{3,6}},
			}
			flags =
			{
				CAN_SWIM=true
				BASH_DOOR=true HAS_EGG=true MORTAL=true AI=ai.SNAKE
			}
		}
		[4] =
		{
			name = "large boa"
			level = 4 rarity = 1
			desc = "It is about sixteen feet long."
			color = color.SLATE
			speed = -5 life = {6,8} ac = 20
			aaf = 6 sleep = 50
			exp = 14
			weight = 1300
			blows =
			{
				{"BITE","PIERCE",{6,5}},
				{"CRUSH","CRUSH",{8,9}},
			}
			flags =
			{
				CAN_SWIM=true
				BASH_DOOR=true HAS_EGG=true MORTAL=true AI=ai.SNAKE
			}
		}
		[5] =
		{
			name = "giant copperhead snake"
			level = 5 rarity = 1
			desc = "Over ten feet long, it has a copper head and sharp venomous fangs."
			color = color.ORANGE
			speed = 0 life = {6,6} ac = 24
			aaf = 6 sleep = 1
			exp = 15
			weight = 200
			blows =
			{
				{"BITE","POISON",{7,7}},
				{"BITE","PIERCE",{3,4}},
			}
			flags =
			{
				RESIST=getter.resists{POIS=100}
				CAN_SWIM=true ALLOW_IN_PLAIN=true
				BASH_DOOR=true HAS_EGG=true MORTAL=true AI=ai.SNAKE
			}
		}
		[6] =
		{
			name = "giant rattlesnake"
			level = 6 rarity = 1
			desc =
			{
				"It is recognised by the hard-scaled end of its body that is often rattled",
				"to frighten its prey.",
			}
			color = color.RED
			speed = 0 life = {6,7} ac = 24
			aaf = 6 sleep = 1
			exp = 20
			weight = 200
			blows =
			{
				{"BITE","POISON",{8,8}},
				{"BITE","PIERCE",{3,4}},
			}
			flags =
			{
				RESIST=getter.resists{SLASH=10 POIS=100}
				CAN_SWIM=true ALLOW_IN_PLAIN=true
				BASH_DOOR=true HAS_EGG=true MORTAL=true AI=ai.SNAKE
			}
		}
		[7] =
		{
			name = "emperor cobra"
			level = 12 rarity = 2
			desc =
			{
				"This huge snake must be at least twenty feet long",
				"and its giant hood conceals massive jaws."
			}
			color = color.GREEN
			speed = 0 life = {10,10} ac = 22
			aaf = 8 sleep = 1
			exp = 35
			weight = 300
			blows =
			{
				{"SPIT","BLIND",{4,6}},
				{"BITE","POISON",{10,9}},
			}
			flags =
			{
				RESIST=getter.resists{POIS=100}
				BASH_DOOR=true CAN_SWIM=true HAS_EGG=true
				MORTAL=true AI=ai.SNAKE
			}
		}
		[8] =
		{
			name = "black mamba"
			level = 12 rarity = 3
			desc = "It has glistening black skin, a sleek body, and highly venomous fangs."
			color = color.LIGHT_DARK
			speed = 10 life = {10,8} ac = 30
			aaf = 10 sleep = 1
			exp = 40
			weight = 300
			blows =
			{
				{"BITE","POISON",{10,11}},
			}
			flags =
			{
				RESIST=getter.resists{POIS=100}
				BASH_DOOR=true CAN_SWIM=true
				HAS_EGG=true MORTAL=true AI=ai.SNAKE
				ALLOW_IN_PLAIN=true ALLOW_IN_FOREST=true ALLOW_IN_SWAMP=true
			}
		}
		[9] =
		{
			name = "electric eel"
			level = 20 rarity = 2
			desc = "This serpentine creature can create a deadly voltage. Better watch out!"
			color = color.LIGHT_BLUE
			speed = 0 life = {15,15} ac = 35
			aaf = 15 sleep = 70
			exp = 145
			weight = 500
			blows =
			{
				{"TOUCH","ELEC",{6,7}},
				{"TOUCH","ELEC",{6,7}},
				{"TOUCH","ELEC",{6,7}},
			}
			flags =
			{
				RESIST=getter.resists{ELEC=100}
				AQUATIC=true 
				COLD_BLOOD=true MORTAL=true AI=ai.RANDOM_MOVE AI_STATES=getter.flags
				{
					MOVE_RATE=25
				}
				ALLOW_IN_RIVER=true ALLOW_IN_SWAMP=true
			}
		}
		[10] =
		{
			name = "giant python"
			level = 30 rarity = 2
			desc =
			{
				"This huge serpent lies in long loops upon the floor, and you see",
				"massive muscles ripple under its heavy scales.",
			}
			color = color.UMBER
			speed = 0 life = {40,15} ac = 10
			aaf = 15 sleep = 70
			exp = 140
			weight = 5000
			blows =
			{
				{"BITE","PIERCE",{10,11}},
				{"CRUSH","CRUSH",{12,12}},
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=10 SLASH=10 PIERCE=10}
				HAS_EGG=true MORTAL=true AI=ai.ZOMBIE
			}
		}
	}
-------------------------- Orcs --------------------------
	['o'] =
		{
		defaults =
			{
				body=default_body.humanoid
				flags={
					ALLOW_IN_EVERYWHERE=true ORC=true
					EVIL=true OPEN_DOOR=true BASH_DOOR=true
					TAKE_ITEM=true DROP_CORPSE=true DROP_SKELETON=true
					NON_INNATE_ARMOUR=true
					}
			} -- defaults
		[1] =
		{
			name="snotling"
			level=4 rarity=1
			desc =
			{
				"An immature orc with a short sword, running around",
				"screaming. Despite its youth, it's still quite",
				"dangerous.",
			}
			color=color.UMBER
			speed=0 life={8,4} ac=15
			aaf=20 sleep=60
			exp=18 weight=1200
			blows=
			{
				{"WEAPON","SLASH",{6,3}}
			}
			flags =
			{
				RESIST=getter.resists{DARK_FIRE=50 LITE=-66}
				FRIENDS=getter.friends(80,8)
				FACTION=FACTION_SAURON
				DROP_THEME=getter.flags {
					THEME_JUNK=80
					THEME_COMBAT=20
				}
				AI=ai.RANDOM_MOVE AI_STATES=getter.flags{MOVE_RATE=15}
				ALLOW_IN_FOREST=true
			}
		}
		[2] =
		{
			name="snaga"
			level=8 rarity=1
			desc =
			{
				"One of the weaker 'slave' races of orcs, this skinny specimen",
				"is clad in a loincloth and is carrying a heavy bludgeon.",
			}
			color=color.MOCCASIN
			speed=0 life={8,8} ac=15
			aaf=20 sleep=60
			exp=20 weight=1400
			blows={
					{"WEAPON","CRUSH",{6,6}}
				}
			flags =
			{
				RESIST=getter.resists{DARK_FIRE=50 LITE=-66}
				DROP_THEME=getter.flags {THEME_JUNK=80 THEME_COMBAT=20}
				AI=ai.ZOMBIE AI_STATES=getter.flags{FEAR_LEVEL=30}
				FACTION=FACTION_SAURON FRIENDS=getter.friends(75,12)
				ALLOW_IN_PLAIN=true ALLOW_IN_COLD=true
			}
		}
		[3] =
		{
			name="orcish raider"
			level=15 rarity=1
			desc =
			{
				"A lightly armoured party of orcs, come down from the Misty Mountains",
				"to make mischief upon the races of men. These are more robbers than",
				"soldiers of Morgoth, but their cruel, curved swords are sharp nonetheless!",
			}
			color=color.SANDY_BROWN
			speed=0 life={10,7} ac=25
			aaf=40 sleep=30
			exp=30 weight=1900
			blows=
			{
				{"WEAPON","SLASH",{8,8}}
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=5 DARK_FIRE=50 LITE=-66 PIERCE=5 SLASH=5}
				AI=ai.ZOMBIE FRIENDS=getter.friends(95,8) HAS_LITE=true
				FACTION=FACTION_SAURON SPEEDS=getter.speeds{WALK=5}
				DROP_THEME=getter.flags {
					THEME_JUNK=20
					THEME_TREASURE=30
					THEME_COMBAT=50
				}
			}
		}
		[4] =
		{
			name="orcs of the white hand"
			level=18 rarity=1
			desc =
			{
				"An armoured party of orcs, with white handprints on their shields.",
				"Their armour is mismatched and their weapons crudely forged and dirty",
				", but they have a discipline and purpose unusual amongst their kind...",
			}
			color=color.OLIVE_DRAB
			speed=0 life={10,7} ac=30
			aaf=50 sleep=40
			exp=35 weight=2100
			blows=
			{
				{"WEAPON","SLASH",{7,6}},
				{"WEAPON","SLASH",{7,6}}
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=10 DARK_FIRE=50 LITE=-66 PIERCE=10 SLASH=10}
				AI=ai.ZOMBIE FRIENDS=getter.friends(95,12)
				FACTION=FACTION_ISENGARD
				DROP_THEME=getter.flags {
					THEME_TREASURE=40
					THEME_COMBAT=60
				}
			}
		}
		[5] =
		{
			name="orcish archer"
			level=22 rarity=2
			desc =
			{
				"A lithe orc with a huge greatbow and a quiver bristling with arrows",
				"he peers into the distance, seeking out new targets to snipe at.",
			}
			color=color.OLIVE_DRAB
			speed=0 life={10,7} ac=20
			aaf=60 sleep=20
			exp=55 weight=1900
			blows=
			{
				{"WEAPON","SLASH",{7,6}},
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=5 DARK_FIRE=50 LITE=-66 PIERCE=5 SLASH=5}
				SPEEDS=getter.speeds{CAST=6} LIMIT_SPELLS = getter.limit_spells{Arrow=40}
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
					CASTING_AI=ai.SMART_CAST
					MOVE_AI=ai.ZOMBIE
					BEST_RANGE=4
					MIN_RANGE=2
					LURE_TARGET=true
				}
				FACTION=FACTION_SAURON
				DROP_THEME=getter.flags {
					THEME_JUNK=20
					THEME_TREASURE=30
					THEME_COMBAT=50
				}
			}
			spells=
			{
				frequency=2
				["Arrow"]={ level=60 chance=80 }
			}
		}
		[6] =
		{
			name="uruk-hai"
			level=25 rarity=1
			desc =
			{
				"They are the fighting Uruk-Hai! An heavily armoured party of orcs,",
				"with white handprints on their shields and war-chants on their lips.",
				"Most orcs are simple warriors, but these are terrible foemen!",
			}
			color=color.WHITE
			speed=0 life={10,10} ac=35
			aaf=60 sleep=20
			exp=55 weight=2200
			blows=
			{
				{"WEAPON","SLASH",{8,8}},
				{"WEAPON","SLASH",{8,8}},
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=12 DARK_FIRE=50 LITE=-66 PIERCE=12 SLASH=12}
				LIMIT_SPELLS = getter.limit_spells{Arrow=15}
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
					MOVE_AI=ai.ZOMBIE
					CASTING_AI=ai.SMART_CAST
					LURE_TARGET=true
					FEAR_LEVEL=20
				}
					SPEEDS=getter.speeds{CAST=-10} FRIENDS=getter.friends(95,15)
					FACTION=FACTION_ISENGARD
					DROP_THEME=getter.flags{THEME_TREASURE=20 THEME_COMBAT=80}
			}
			spells=
			{
				frequency=6
				["Arrow"]={level=40 chance=60}
			}
		}
		[7] =
		{
			name="orcish crossbowman"
			level=24 rarity=2
			desc =
			{
				"This heavy-set orc is staggering under the weight of",
				"a heavy iron crossbow of orcish design. Orcs are slipshod",
				"in many crafts but excel in the engines of war!"
			}
			color=color.SLATE
			speed=0 life={8,8} ac=15
			aaf=60 sleep=40
			exp=60 weight=2500
			blows={
					{"WEAPON","SLASH",{5,6}}
				}
			flags =
			{
				RESIST=getter.resists{CRUSH=7 DARK_FIRE=50 LITE=-66 PIERCE=7 SLASH=7}
				DROP_THEME=getter.flags {THEME_TREASURE=80 THEME_COMBAT=20}
				SPEEDS=getter.speeds{CAST=-10} FACTION=FACTION_SAURON
  			LIMIT_SPELLS = getter.limit_spells{Arrow=28}
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
						CASTING_AI=ai.SMART_CAST
						MOVE_AI=ai.ZOMBIE
						BEST_RANGE=4
						MIN_RANGE=2
						LURE_TARGET=true
					}
			}
			spells=
			{
				frequency=3
				["Arrow"]={ level=90 chance=90 }
			}
		}
		[8] =
		{
			name="orcish slaver"
			level=28 rarity=2
			desc =
			{
				"This lightly armoured orc carries a many-thonged whip and has as",
				"vicious a tongue as his weapon. He exists only to motivate orcish",
				"slaves, be they men or other orcs.",
			}
			color=color.DARK_ORCHID
			speed=10 life={8,6} ac=27
			aaf=50 sleep=30
			exp=80 weight=2000
			blows=
			{
				{"WEAPON","SLASH",{4,4}},
				{"WEAPON","SLASH",{4,4}},
			}
			flags =
			{
				RESIST=getter.resists{DARK_FIRE=50 LITE=-66}
				DROP_THEME=getter.flags {THEME_JUNK=80 THEME_COMBAT=20} ESCORT=20
				ESCORTED_BY_GROUPS=5 ESCORT_NAME="snaga"
				FACTION=FACTION_SAURON HAS_LITE=true AI=ai.SPELL_AND_MOVE
				AI_STATES=getter.flags{
					MOVE_AI=ai.ZOMBIE
					CASTING_AI=ai.SMART_CAST
					FEAR_LEVEL=40
				}
			}
			spells=
			{
				frequency=4
				["Shriek"]={ level=50 chance=90 }
			}
		}
		[9] =
		{
			name="orcish shaman"
			level=30 rarity=2
			desc =
			{
				"The orcs too have magic of a sort, and here is one of their shamans, ",
				"covered in relics and bones. She mutters, and you see light flicker from",
				"the top of her skull-crowned staff. Foul her magic may be, but it is",
				"still powerful!",
			}
			color=color.CRIMSON
			speed=0 life={8,6} ac=27
			aaf=80 sleep=80
			exp=120 weight=1800
			blows=
				{
					{"WEAPON","CRUSH",{6,4}}
				}
			flags =
			{
				RESIST=getter.resists{CONFUSION=50 DARK_FIRE=50 FEAR=100 LITE=-66}
				HAS_LITE=true SEE_INVIS=40 FEMALE=true FACTION=FACTION_SAURON
				DROP_THEME=getter.flags{THEME_JUNK=40 THEME_MAGIC= 30 THEME_TREASURE=30}
				SPEEDS=getter.speeds{CAST=7}
				MANA_CURR=120 MANA_MAX=120 MANA_REGENERATE=600
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
					CASTING_AI=ai.SMART_CAST
					MOVE_AI=ai.ZOMBIE
					BEST_RANGE=4
					MIN_RANGE=2
					LURE_TARGET=true
					NO_SPELL_FLEE=true
				}
			}
			spells =
			{
				frequency=4
				["Scare"]={ level=60 chance=70 }
				["Confuse"]={ level=34 chance=70}
				["Darkness"]={ level=50 chance=80}
				["Healing"]={level=40 chance=60}
				["Noxious Cloud"]={level=44 chance=60}
				["Cause Wounds"]={level=36 chance=60}
			}
		}
		[10] =
		{
			name="orcish assassin"
			level=35 rarity=2
			desc =
			{
				"This small orc is almost invisible in his dark grey clothes, and he",
				"will only show himself with poison-tipped steel!",
			}
			color=color.DARK_KHAKI
			speed=10 life={9,9} ac=50
			aaf=80 sleep=10
			exp=300 weight=1700
			blows=
			{
				{"WEAPON","POISON",{20,4}},
				{"WEAPON","PIERCE",{7,8}},
			}
			flags =
			{
				RESIST=getter.resists{DARK_FIRE=50 LITE=-66 POIS=30}
				INVISIBLE=true WEIRD_MIND=true SEE_INVIS=40
				DROP_THEME=getter.flags {THEME_TREASURE=10 THEME_COMBAT=90}
				FACTION=FACTION_SAURON LIMIT_SPELLS = getter.limit_spells{["Bolt of Poison"]=15}
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
					CASTING_AI=ai.SMART_CAST
					MOVE_AI=ai.ZOMBIE
					BEST_RANGE=4
					MIN_RANGE=2
					LURE_TARGET=true
				}
 			}
			spells =
			{
				frequency=4
				["Bolt of Poison"]={ level=60 chance=70 }
			}
		}
		[11] =
		{
			name="half orc"
			level=35 rarity=1
			desc =
			{
				"These tall and swarthy warriors almost look like men, but the trace",
				"of orcish fang and the gutteral sounds of the dark tongue confirm your",
				"suspicions, these are an unnatural mixture of orcish and Edain blood."
			}
			color=color.LIGHT_DARK
			speed=10 life={12,12} ac=40
			aaf=50 sleep=30
			exp=180 weight=2200
			blows=
			{
				{"WEAPON","SLASH",{12,9}},
				{"WEAPON","PIERCE",{12,9}},
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=14 DARK_FIRE=50 LITE=-33 PIERCE=14 SLASH=14}
				DROP_THEME=getter.flags{THEME_TREASURE=20 THEME_COMBAT=80} --- DGDGDG HUMAN=true
				FACTION=FACTION_ISENGARD FRIENDS=getter.friends(95,20)
				SPEEDS=getter.speeds{WALK=-5}
				AI=ai.ZOMBIE AI_STATES=getter.flags{FEAR_LEVEL=5 LURE_TARGET=true}
				ALLOW_IN_PLAIN=true ALLOW_IN_COLD=true
			}
		}
		[12] =
		{
			name="orcish commander"
			level=40 rarity=3
			desc =
			{
				"This orc is dressed in fine armour and walks with an erect carriage and",
				"almost noble countenance. He is one of Sauron's captains, commanding a",
				"great orcish horde and his great spear is as much banner as weapon.",
			}
			color=color.CRIMSON
			speed=10 life={20,12} ac=55
			aaf=80 sleep=0
			exp=400 weight=3000
			blows=
			{
				{"WEAPON","PIERCE",{12,10}},
				{"WEAPON","PIERCE",{12,10}},
				{"WEAPON","PIERCE",{12,10}},
			}
			flags =
			{
				RESIST=getter.resists{CONFUSION=50 CRUSH=18 DARK_FIRE=50 FEAR=100 LITE=-66 PIERCE=18 SLASH=18}
				ESCORT=true ESCORTED_BY_GROUPS=true SMART=true HAS_LITE=true FORCE_MAXHP=true FACTION=FACTION_SAURON
				DROP_THEME=getter.flags {THEME_TREASURE=40 THEME_COMBAT=60}
				AI=ai.PATHFIND AI_STATES=getter.flags{FEAR_LEVEL=15}
			}
		}
		[13] =
		{
			name="uruk-hai commander"
			level=43 rarity=3
			desc =
			{
				"Padding like a great cat, this Uruk-Hai must be near to seven feet tall",
				"and wears his brutal authority like a mantle. He is the finest, the toughest",
				"and most disturbingly the brightest that orc-kind has to offer."
			}
			color=color.CRIMSON
			speed=5 life={20,18} ac=55
			aaf=80 sleep=0
			exp=450 weight=3400
			blows=
			{
				{"WEAPON","SLASH",{18,12}},
				{"WEAPON","SLASH",{18,12}},
			}
			flags =
			{
				RESIST=getter.resists{CONFUSION=50 CRUSH=20 DARK_FIRE=50 FEAR=100 LITE=-33 PIERCE=20 SLASH=20}
				ESCORT=true ESCORTED_BY_GROUPS=true SMART=true HAS_LITE=true FORCE_MAXHP=true
				LIMIT_SPELLS = getter.limit_spells{Arrow=20}
				DROP_THEME=getter.flags {THEME_TREASURE=20 THEME_COMBAT=80}
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
					FEAR_LEVEL=20
					MOVE_AI=ai.PATH_FIND
					CASTING_AI=ai.SMART_CAST
				}
				FACTION=FACTION_ISENGARD
			}
			spells =
			{
				frequency=4
				["Arrow"]={ level=60 chance=60}
			}
		}
		[14] =
		{
			name="orcish sapper"
			level=50 rarity=2
			desc =
			{
				"This orc looks towards you with a blank expression, his mind destroyed",
				"by too much orc-liquor. His body is strapped with orcish devices which",
				"will explode upon whomever he throws himself at.",
			}
			color=color.FIREBRICK
			speed=10 life={10,6} ac=30
			aaf=40 sleep=10
			exp=120 weight=2000
			blows={
					{"EXPLODE","CRUSH",{30,30}}
				}
			flags =
			{
				RESIST=getter.resists{CONFUSION=50 FEAR=100 DARK_FIRE=50 LITE=-66}
				CHAR_MULTI=true AI=ai.ZOMBIE AI_STATES=getter.flags{FEAR_LEVEL=0}
				FACTION=FACTION_SAURON WEIRD_MIND=true
			}
		}
		[15] =
		{
			name="orcish abomination"
			level=54 rarity=2
			desc =
			{
				"Clawed and fanged far beyond the norm, this hulking beast is a failure",
				"of Orthanc's twisted experiments. But it appears that its main failure",
				"was its control, as it appears consumed by animalistic rage.",
			}
			color=color.DARK_SEA_GREEN
			speed=30 life={25,10} ac=60
			aaf=40 sleep=60
			exp=550 weight=3200
			blows={
					{"CLAW","SLASH",{14,10}},
					{"CLAW","SLASH",{14,10}},
					{"BITE","PIERCE",{12,9}},
				}
			flags =
			{
				RESIST=getter.resists{CONFUSION=50 DARK_FIRE=50 FEAR=100 POIS=66 LITE=-66}
				DROP_THEME=getter.flags {THEME_JUNK=80 THEME_TREASURE=20 THEME_COMBAT=20}
				SPEEDS=getter.speeds{WALK=-10} AI=ai.ZOMBIE AI_STATES=getter.flags{FEAR=0}
				REGENERATE=8000 WEIRD_MIND=true AERIAL=true CAN_FLY=3 -- swinging through the trees
				FACTION=FACTION_ISENGARD
			}
		}
		[16] =
		{
			name="elite guard of mordor"
			level=58 rarity=1
			desc =
			{
				"Armed with the finest weapons Mordor has and armoured in the black,",
				"spiked armour of Sauron's personal guard, these fearsome orcish ",
				"warriors are the worst nightmare of the other inhabitants of Arda.",
			}
			color=color.DARK_SLATE_GRAY
			speed=10 life={18,12} ac=50
			aaf=60 sleep=20
			exp=400 weight=3200
			blows=
			{
				{"WEAPON","SLASH",{14,10}},
				{"WEAPON","PIERCE",{10,10}},
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=20 DARK_FIRE=50 FEAR=100 LITE=-66 PIERCE=20 POIS=33 SLASH=20}
				REFLECTING=15 FACTION=FACTION_SAURON HAS_LITE=true FRIENDS=getter.friends(95,22)
				DROP_THEME=getter.flags {THEME_TREASURE=20 THEME_COMBAT=80}
				AI=ai.ZOMBIE AI_STATES=getter.flags{FEAR_LEVEL=0 LURE_TARGET=true}
			}
		}
		[17] =
		{
			name="the chosen of isengard"
			level=60 rarity=1
			desc =
			{
				"These heavy infantry soliders are savage foemen, fearless in the service",
				"of the White Hand and equipped by him with fine weapons and a hatred for",
				"all who oppose his will!",
			}
			color=color.OLD_LACE
			speed=15 life={18,12} ac=60
			aaf=60 sleep=20
			exp=400 weight=3000
			blows=
			{
				{"WEAPON","SLASH",{13,13}}
				{"WEAPON","SLASH",{13,13}}
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=16 DARK_FIRE=50 FEAR=100 LITE=-66 PIERCE=16 POIS=50 SLASH=16}
				REFLECTING=10 FACTION=FACTION_SAURON HAS_LITE=true FRIENDS=getter.friends(95,25)
				DROP_THEME=getter.flags {THEME_TREASURE=20 THEME_COMBAT=80}
				AI=ai.ZOMBIE AI_STATES=getter.flags{FEAR_LEVEL=0 LURE_TARGET=true}
			}
		}
	}
['p'] =
		{
		defaults =
			{
				body=default_body.humanoid
				flags={
					ALLOW_IN_EVERYWHERE=true HUMAN=true
					OPEN_DOOR=true BASH_DOOR=true TAKE_ITEM=true
					DROP_CORPSE=true DROP_SKELETON=true
					NON_INNATE_ARMOUR=true
					}
			} -- defaults
		[1] =
		{
			name = "ruffian"
			level = 1 rarity = 1
			desc =
			{
				"This gang of toughs are expert at intimidation. You don't scare that",
				"easily... do you?"
			}
			color = color.BLUE
			speed = 0 life = {4,5} ac = 10
			aaf = 40 sleep = 70
			exp = 8
			weight = 1800
			blows =
			{
				{"WEAPON","SLASH",{3,3}},
			}
			flags =
			{
				RESIST=getter.resists{DARK_FIRE=50}
				DROP_THEME=getter.flags {THEME_COMBAT=30 THEME_JUNK=50 THEME_GOLD=20}
				EVIL=true
				FACTION=FACTION_DUNGEON FRIENDS=getter.friends(95,6)
				AI=ai.ZOMBIE AI_STATES=getter.flags{FEAR_LEVEL=20} HAS_LITE=true
			}
		}
		[2] =
		{
			name = "bravo"
			level = 1 rarity = 1
			desc =
			{
				"This novice warrior holds his sword nervously and his armour is almost",
				"without a scratch, but he still looks dangerous!"
			}
			color = color.UMBER
			speed = 0 life = {5,5} ac = 15
			aaf = 30 sleep = 70
			exp = 10
			weight = 2000
			blows =
			{
				{"WEAPON","SLASH",{2,7}},
				{"WEAPON","SLASH",{2,7}},
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=5 PIERCE=5 SLASH=5}
				DROP_THEME=getter.flags {THEME_COMBAT=50 THEME_TREASURE=50}
				FACTION=FACTION_DUNGEON FRIENDS=getter.friends(60,6) HAS_LITE=true
				AI=ai.ZOMBIE
			}
		}
		[3] =
		{
			name = "conjuror"
			level = 1 rarity = 1
			desc =
			{
				"This callow youth is only one step above a street conjuror, but still",
				"works some dangerous magicks!"
			}
			color = color.CRIMSON
			speed = 0 life = {4,4} ac =6
			aaf = 40 sleep = 50
			exp = 8
			weight = 1650
			blows =
			{
				{"WEAPON","SLASH",{2,3}},
			}
			flags =
			{
				FACTION=FACTION_DUNGEON HAS_LITE=true AI=ai.SPELL_AND_MOVE
				DROP_THEME=getter.flags{THEME_MAGIC=70 THEME_CANTRIPS=20 THEME_JUNK=10}
				MANA_CURR=20 MANA_MAX=20 MANA_REGENERATE=100
				AI_STATES=getter.flags{
					CASTING_AI=ai.SMART_CAST
					MOVE_AI=ai.ZOMBIE
					BEST_RANGE=3
					MIN_RANGE=1
					NO_SPELL_FLEE=true
				}
			}
			spells =
			{
				frequency = 2
				["Manathrust"] = { level=10 chance=80}
				["Phase Door"] = { level=10 chance=86}
			}
		}
		[4] =
		{
			name = "paladin"
			level = 4 rarity =2
			desc =
			{
				"This disciple of Tulkas appears unarmed, but his fists have the hard",
				"calluses of a bare-knucked fighter, and his skin the stony sheen of a",
				"mystic armour granted by his Valar.",
			}
			color = color.SANDY_BROWN
			speed = 0 life = {6,5} ac =20
			aaf = 40 sleep = 60
			exp = 15
			weight = 1800
			blows =
			{
				{"PUNCH","CRUSH",{5,4}},
				{"PUNCH_OFFHAND","CRUSH",{5,4}},
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=10 FEAR=100 PIERCE=10 SLASH=10}
				DROP_THEME=getter.flags {THEME_CANTRIPS=60 THEME_COMBAT=20 THEME_TREASURE=30}
				FACTION=FACTION_ELF HAS_LITE=true
				AI=ai.ZOMBIE AI_STATES=getter.flags{FEAR_LEVEL=0}
			}
		}
		[5] =
		{
			name = "hunter"
			level = 4 rarity =1
			desc =
			{
				"This relaxed woodsman leans on his greatbow. A quiver of arrows hangs",
				"from his belt, and his eyes keenly search the surrounding landscape.",
			}
			color = color.LIGHT_GREEN
			speed = 0 life = {6,5} ac =10
			aaf = 80 sleep =40
			exp = 15
			weight = 1600
			blows =
			{
				{"WEAPON","PIERCE",{4,3}},
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=5 PIERCE=5 SLASH=5}
				DROP_THEME=getter.flags{THEME_BOWS=50 THEME_COMBAT=20 THEME_TREASURE=30}
				FACTION=FACTION_DUNGEON AI=ai.SPELL_AND_MOVE
				LIMIT_SPELLS = getter.limit_spells{Arrow = 30}
				AI_STATES=getter.flags{
					CASTING_AI=ai.SMART_CAST
					MOVE_AI=ai.GHOUL
				}
				ALLOW_IN_FOREST=true
			}
			spells=
			{
				["Arrow"] = {level= 20 chance = 75}
			}
		}
		[6] =
		{
			name = "easterling"
			level = 8 rarity =5
			desc =
			{
				"With their curved scimitars and fierce cries, these humans from the",
				"east of Arda are among the most dedicated enemies of Gondor.",
			}
			color = color.DARK_RED
			speed = 0 life = {6,8} ac =20
			aaf = 40 sleep = 60
			exp = 20
			weight = 2000
			blows =
			{
				{"WEAPON","SLASH",{3,7}},
				{"WEAPON","SLASH",{3,7}},
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=8 PIERCE=8 SLASH=8}
				DROP_THEME=getter.flags{THEME_COMBAT=60 THEME_TREASURE=20 THEME_JUNK=10}
				FACTION=FACTION_SAURON HAS_LITE=true AI=ai.ZOMBIE
				FRIENDS=getter.friends(85,10)
			}
		}
		[7] =
		{
			name = "pikeman of morgoth"
			level = 8 rarity =1
			desc =
			{
				"Grim and unsmiling, these soldiers of Morgoth hold heavy pikes before",
				"then, tightly holding together in close formation. Anyone trying to ride",
				"through this swarm would feel their sting!",
			}
			color = color.LIGHT_DARK
			speed = 0 life = {6,6} ac =30
			aaf = 40 sleep = 60
			exp = 18
			weight = 1900
			blows =
			{
				{"WEAPON_CHARGE","PIERCE",{5,10}},
			}
			flags =
			{
				RESIST=getter.resists{DARK_FIRE=50}
				DROP_THEME=getter.flags{THEME_COMBAT=30 THEME_POLEARM=30 THEME_TREASURE=30 THEME_JUNK=10}
				FACTION=FACTION_SAURON EVIL=true HAS_LITE=true
				AI=ai.ZOMBIE AI_STATES=getter.flags{LURE_TARGET=true}
				FRIENDS=getter.friends(95,15)
			}
		}
		[8] =
		{
			name = "gondorian footman"
			level = 14 rarity = 1
			desc =
			{
				"Dressed in fine armour and wielding sharp blades of Gondor, these",
				"fierce warriors bear the symbol of the white tree on their gleaming",
				"shields."
			}
			color = color.WHITE
			speed = 0 life = {8,6} ac =40
			aaf = 60 sleep = 40
			exp = 30
			weight = 2700
			blows =
			{
				{"WEAPON","SLASH",{3,10}},
				{"WEAPON","SLASH",{3,10}},
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=15 FEAR=50 SLASH=15 PIERCE=15}
				DROP_THEME=getter.flags{THEME_COMBAT=70 THEME_TREASURE=20 THEME_JUNK=10}
				FACTION=FACTION_GONDOR HAS_LITE=true REFLECTING=10
				AI=ai.ZOMBIE AI_STATES=getter.flags{FEAR_LEVEL=10}
				FRIENDS=getter.friends(90,11)
			}
		}
		[9] =
		{
			name = "gondorian archer"
			level = 18 rarity =2
			desc =
			{
				"Dressed in fine scale mail and bearing a great bow of horn and yew,",
				"this bowman of Gondor looks keenly into the middle distance, quiver of",
				"broadheads hanging within easy reach on his belt.",
			}
			color = color.ANTIQUE_WHITE
			speed = 0 life = {6,6} ac =30
			aaf = 80 sleep =30
			exp = 40
			weight = 2000
			blows =
			{
				{"WEAPON","SLASH",{6,6}},
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=10 SLASH=10 PIERCE=10}
				DROP_THEME=getter.flags{THEME_BOWS=50 THEME_COMBAT=20 THEME_TREASURE=20 THEME_JUNK=10}
				FACTION=FACTION_GONDOR HAS_LITE=true
				LIMIT_SPELLS = getter.limit_spells{Arrow = 30}
				SPEEDS=getter.speeds{CAST=5}
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
				LURE_TARGET=true
				MOVE_AI=ai.ZOMBIE
				CASTING_AI=ai.SMART_CAST
				MIN_RANGE=1
				BEST_RANGE=4}
			}
			spells =
			{
				frequency = 3
				["Arrow"] = { level=50 chance=85}
			}
		}
		[10] =
		{
			name = "gondorian captain"
			level = 25 rarity = 2
			desc =
			{
				"This thickset warrior strides across the ground, dark hair covered by",
				"a steel helmet topped with a captain's plume. The naked blade in his hand",
				"is held easily at his side as he scans the situation before him with the",
				"careful eye of a seasoned warrior",
			}
			color = color.LIGHT_WHITE
			speed = 10 life = {12,12} ac =40
			aaf = 70 sleep = 60
			exp = 150
			weight = 2300
			blows =
			{
				{"WEAPON","SLASH",{10,9}},
				{"WEAPON","SLASH",{10,9}},
			}
			flags =
			{
				RESIST=getter.resists{CONFUSION=66 CRUSH=18 FEAR=50 PIERCE=18 SLASH=18}
				DROP_THEME=getter.flags{THEME_SWORD=30 THEME_ARMOR=30 THEME_TREASURE=20 THEME_CANTRIPS=20}
				FACTION=FACTION_GONDOR ESCORT=true ESCORTED_BY_GROUPS=true SMART=true
				HAS_LITE=true AI=ai.ZOMBIE AI_STATES=getter.flags{FEAR_LEVEL=20}
			}
		}
		[11] =
		{
			name = "easterling captain"
			level = 25 rarity = 2
			desc =
			{
				"This swarthy warrior stands arrogantly in the midst of his troop, the",
				"brass roundels on his armour gleaming dully in the light. He and his",
				"men have come to make war upon the men of Gondor, for plunder and the",
				"satisfaction of ancient hatred!",
			}
			color = color.CRIMSON
			speed = 10 life = {12,10} ac =45
			aaf = 70 sleep = 60
			exp = 150
			weight = 2100
			blows =
			{
				{"WEAPON","SLASH",{7,7}},
				{"WEAPON","SLASH",{7,7}},
				{"WEAPON","POISON",{6,6}},
			}
			flags =
			{
				RESIST=getter.resists{CONFUSION=66 CRUSH=14 FEAR=14 PIERCE=14 SLASH=14}
				DROP_THEME=getter.flags{THEME_SWORD=30 THEME_ARMOR=30 THEME_TREASURE=20 THEME_CANTRIPS=20}
				ESCORT=true ESCORTED_BY_GROUPS=true ESCORT_NAME="easterling"
				FACTION=FACTION_SAURON ESCORT=true SMART=true HAS_LITE=true
				AI=ai.ZOMBIE AI_STATES=getter.flags{FEAR_LEVEL=20}
			}
		}
		[12] =
		{
			name = "easterling shaman"
			level = 28 rarity = 2
			desc =
			{
				"This Easterling is garbed in blackest silk and garish jerwellery, his",
				"eyes barely visible beneath his gold and ivory mask. Those eyes, however",
				"radiate with dark power, bequeathed by his patron, Sauron!",
			}
			color = color.ORCHID
			speed = 0 life = {8,8} ac =25
			aaf = 90 sleep =30
			exp = 180
			weight = 1900
			blows =
			{
				{"WEAPON","CRUSH",{4,4}},
			}
			flags =
			{
				RESIST=getter.resists{CONFUSION=50 FEAR=100 DARK_FIRE=50}
				DROP_THEME=getter.flags{THEME_MAGIC=60 THEME_TREASURE=30 THEME_JUNK=10}
				HAS_LITE=true SEE_INVIS=40 FACTION=FACTION_SAURON
				SPEEDS=getter.speeds{CAST=5} EVIL=true
				MANA_CURR=150 MANA_MAX=150 MANA_REGENERATE=750
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
					CASTING_AI=ai.SMART_CAST
					MOVE_AI=ai.ZOMBIE
					BEST_RANGE=4
					MIN_RANGE=1
					LURE_TARGET=true
					NO_SPELL_FLEE=true}
			}
			spells =
			{
				frequency=2
				["Darkness"] = {level = 40 chance = 90}
				["Scare"] = {level = 50 chance = 90}
				["Noxious Cloud"] = {level = 27 chance = 75}
				["Fireflash"] = {level = 13 chance = 65}
				["Healing"] = {level = 13 chance = 70}
				["Phase Door"] = {level = 31 chance = 85}
			}
		}
		[13] =
		{
			name = "thief"
			level = 30 rarity =2
			desc =
			{
				"This shifty character is barely visible under his dark cloak, and his",
				"blade is darkened with pitch. Rake thin and agile, you realise that this",
				"cutpurse is far more dangerous than he appears!"
			}
			color = color.SLATE
			speed = 5 life = {9,10} ac =50
			aaf = 90 sleep = 10
			exp = 220
			weight = 1800
			blows =
			{
				{"WEAPON","PIERCE",{7,7}},
				{"WEAPON","PIERCE",{7,7}},
				{"TOUCH","EAT_GOLD",{0,0}}
				{"TOUCH","EAT_ITEM",{0,0}}
			}
			flags =
			{
				----Speedy, Stealthy, Trap-y
				RESIST=getter.resists{SLASH=5 CRUSH=5 PIERCE=5 DARK=66}
				DROP_THEME=getter.flags{THEME_KNIFE=20 THEME_BOWS=20 THEME_TREASURE=30 THEME_TOOLS=30}
				FACTION=FACTION_DUNGEON SMART=true INVISIBLE=true
				SPEEDS=getter.speeds{WALK=5} SEE_INVIS=60
				LIMIT_SPELLS = getter.limit_spells{Arrow = 20}
				AI=ai.SPELL_AND_MOVE
				AI_STATES=getter.flags{
					MOVE_AI=ai.SNAKE
					CASTING_AI=ai.SMART_CAST
					MIN_RANGE=1
					BEST_RANGE=5
					LURE_TARGET=true}
			}
			spells =
			{
				frequency=5
				["Arrow"] = {level= 20 chance = 65}
				["Create Traps"] = {level= 55 chance = 80}
			}
		}
		[14] =
		{
			name = "unhorsed rohirrim"
			level = 30 rarity = 2
			desc =
			{
				"This son of Rohan has lost his horse, but still has his sword and the",
				"proud demeanour of his people!"
			}
			color = color.CADET_BLUE
			speed = 0 life = {15,15} ac = 30
			aaf = 40 sleep = 60
			exp = 200
			weight = 3000
			blows =
			{
				{"WEAPON","SLASH",{10,8}},
				{"WEAPON","SLASH",{10,8}},
			}
			flags =
			{
				----Armoured, tough, but slow
				RESIST=getter.resists{CRUSH=17 PIERCE=17 SLASH=17}
				DROP_THEME=getter.flags{THEME_COMBAT=60 THEME_TREASURE=20 THEME_JUNK=20}
				FACTION=FACTION_ROHAN REFLECTING=15
				SPEEDS=getter.speeds{WALK=-5}
				AI=ai.ZOMBIE
			}
		}
		[15] =
		{
			name = "dark priest"
			level = 33 rarity =1
			desc =
			{
				"A strange keening reaches your ears and you identify it's source as a",
				"huddled figure sitting on the ground, garbed in dark and noisome clothes",
				"A disciple of Melkor sings his praises in a disquieting discord."
			}
			color = color.DARK_GREEN
			speed = 0 life = {10,10} ac =30
			aaf = 60 sleep =60
			exp = 250
			weight = 1750
			blows =
			{
				{"WEAPON","POISON",{4,4}},
			}
			flags =
			{
				----Evil Priest (duh!) of Morgoth
				RESIST=getter.resists{POIS=66 CONFUSION=50 FEAR=100 DARK_FIRE=66}
				DROP_THEME=getter.flags{THEME_MAGIC=70 THEME_TREASURE=20 THEME_JUNK=10}
				FACTION=FACTION_SAURON EVIL=true HAS_LITE=true SMART=true
				MANA_CURR=250 MANA_MAX=250 MANA_REGENERATE=1250
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
					CASTING_AI=ai.SMART_CAST
					MOVE_AI=ai.ZOMBIE
					BEST_RANGE=3
					MIN_RANGE=1
					NO_SPELL_FLEE=true
					LURE_TARGET=true}
			}
			spells =
			{
				["Darkness"] = {level= 50 chance = 90}
				["Scare"] = {level= 60 chance = 90}
				["Noxious Cloud"] = {level= 55 chance = 75}
				["Disease"] = {level= 50 chance = 70}
				["Drain Mana"] = {level= 36 chance = 70}
				["Animate Dead"] = {level= 60 chance = 85}
				["Phase door"] = {level=60 chance=90}
			}
		}
		[16] =
		{
			name = "assassin"
			level = 35 rarity = 2
			desc =
			{
				"Like a ghost, this figure slips over the ground. An evil metal crossbow",
				"hangs by his side, and his hand hovers over the hilt of a razor-sharp",
				"shortsword. It seems someone wants you dead, and he's the man to make",
				"it happen!",
			}
			color = color.OLIVE_DRAB
			speed = 10 life = {13,13} ac =50
			aaf = 90 sleep =10
			exp = 300
			weight = 1800
			blows =
			{
				{"WEAPON","POISON",{8,7}},
				{"WEAPON","POISON",{8,7}},
			}
			flags =
			{
				----Singular, traps, invisible, poison attacks
				RESIST=getter.resists{DARK=66}
				DROP_THEME=getter.flags{THEME_SWORD=30 THEME_BOWS=20 THEME_TREASURE=10 THEME_TOOLS=20 THEME_CANTRIPS=20}
				FACTION=FACTION_DUNGEON INVISIBLE=true NO_AUTO_TARGET=true
				ATTR_CLEAR=true SMART=true AI=ai.SPELL_AND_MOVE
				LIMIT_SPELLS = getter.limit_spells{["Bolt of Poison"]=15}
				AI_STATES=getter.flags{
				CASTING_AI=ai.SMART_CAST
				MOVE_AI=ai.PATHFIND
				LURE_TARGET=true
				}
			}
			spells =
			{
			frequency=4
				["Bolt of Poison"]={ level=50 chance=70}
				["Create Traps"] = {level= 40 chance = 80}
			}
		}
		[17] =
		{
			name = "easterling swordsman"
			level = 37 rarity = 2
			desc =
			{
				"This Easterling carries a very large curved sword, and appears to be",
				"very well versed in it's use, swinging it in swift and graceful curves",
				"quickly enough that you can barely see!"
			}
			color = color.VIOLET
			speed = 10 life = {12,12} ac = 70
			aaf = 30 sleep =70
			exp = 330
			weight = 2000
			blows =
			{
				{"WEAPON","SLASH",{9,8}},
				{"WEAPON","SLASH",{9,8}},
				{"WEAPON","SLASH",{9,8}},
			}
			flags =
			{
				----Lots of attacks and a high AC.
				RESIST=getter.resists{CRUSH=8 SLASH=8 PIERCE=8}
				DROP_THEME=getter.flags{THEME_SWORD=50 THEME_ARMOR=30 THEME_TREASURE=20}
				FACTION=FACTION_SAURON HAS_LITE=true
				SPEEDS=getter.speeds{WALK=-5}
				AI=ai.ZOMBIE
			}
		}
		[18] =
		{
			name = "knight of morgoth"
			level = 40 rarity =2
			desc =
			{
				"Clad in armour black as pitch and armed with a sword glowing with dark",
				"power, this disciple of Morgoth is dedicated to his service and to your"
				"destruction!"
			}
			color = color.LIGHT_DARK
			speed = 0 life = {14,14} ac =45
			aaf = 40 sleep =40
			exp = 380
			weight = 3100
			blows =
			{
				{"WEAPON","DARK_FIRE",{10,9}},
				{"WEAPON","DARK_FIRE",{10,9}},
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=16 DARK_FIRE=66 PIERCE=16 SLASH=16 FEAR=100}
				DROP_THEME=getter.flags{THEME_SWORD=30 THEME_COMBAT=30 THEME_MAGIC=20 THEME_TREASURE=20}
				FACTION=FACTION_SAURON EVIL=true HAS_LITE=true AI=ai.SPELL_AND_MOVE
				FRIENDS=getter.friends(40,3) CORRUPTED=true
				MANA_MAX=80 MANA_CURR=80 MANA_REGENERATE=350
				AI_STATES=getter.flags{
				FEAR_LEVEL=0
				MOVE_AI=ai.ZOMBIE
				CASTING_AI=ai.SMART_CAST}
			}
			spells =
			{
				["Darkness"] ={level=40 chance=80}
				["Scare"] = {level=40 chance=80}
				["Disease"] = {level=50 chance=70}
			}
		}
		[19] =
		{
			name = "ranger"
			level = 40 rarity = 1
			desc =
			{
				"This tall and relaxed hunter is more than he appears,and both the great",
				"bow by his side and the weapon on his hip seem well used!",
			}
			color = color.LIGHT_GREEN
			speed=10 life = {13,13} ac =45
			aaf = 70 sleep = 30
			exp = 300
			weight = 2100
			blows =
			{
				{"WEAPON","SLASH",{8,8}},
				{"WEAPON","SLASH",{8,8}},
			}
			flags =
			{
				----More or less the same as current (+ Tree Walking)
				RESIST=getter.resists{CRUSH=10 PIERCE=10 POIS=33 SLASH=10}
				DROP_THEME=getter.flags{THEME_BOWS=40 THEME_COMBAT=30 THEME_MAGIC=20 THEME_TREASURE=10}
				FACTION=FACTION_ELF HAS_LITE=true MANA_MAX=100 MANA_CURR=100
				MANA_REGENERATE=500 LIMIT_SPELLS = getter.limit_spells{Arrow = 40}
				PASS_WALL=getter.flags{PASS_TREES=100}
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
				CASTING_AI=ai.SMART_CAST
				MOVE_AI=ai.ZOMBIE LURE_TARGET=true
				}
				ALLOW_IN_FOREST=true
			}
			spells =
			{
				frequency=4
					["Arrow"]={ level=60 chance=85}
					["Healing"] = {level=15 chance=70}
					["Grow Trees"] = {level=23 chance=65}
					["Slow Monster"] = {level=15 chance=65}
			}
		}
		[20] =
		{
			name = "warlock"
			level = 44 rarity = 1
			desc =
			{
				"This figure appears in a blaze of magic fire, holding a gleaming metal",
				"sword that seems to slice reality just as much as the air. This warlock",
				"is a master of both blade and spell, and will use both to destroy you!"
			}
			color = color.ORANGE
			speed = 0 life = {12,12} ac = 30
			aaf = 50 sleep = 50
			exp = 350
			weight = 2000
			blows =
			{
				{"WEAPON","SLASH",{8,8}},
				{"WEAPON","FIRE",{9,9}},
			}
			flags =
			{
				----Warrior/Mage
				RESIST=getter.resists{CONFUSE=50 FEAR=100}
				DROP_THEME=getter.flags{THEME_COMBAT=30 THEME_MAGIC=40 THEME_TREASURE=30}
				FACTION=FACTION_SAURON HAS_LITE=true EVIL=true
				MANA_CURR=200 MANA_MAX=200 MANA_REGENERATE=1000
				BLOW_RESPONSE=getter.array{[AURA_FIRE]={8,8}}
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
				FEAR_LEVEL=20
				CASTING_AI=ai.SMART_CAST
				MOVE_AI=ai.ZOMBIE
				NO_SPELL_FLEE=true}
			}
			spells=
			{
				frequency=2
				["Fireflash"] = {level=28 chance=75}
				["Noxious Cloud"] = {level=42 chance=75}
				["Manathrust"] = {level=46 chance=85}
				["Essence of Speed"] = {level=18 chance=70}
				["Healing"] = {level=20 chance=80}
				["Phase Door"] = {level=28 chance=95}
				["Teleportation"] = {level=10 chance=90}
			}
		}
		[21] =
		{
			name = "corrupted geomancer"
			level = 50 rarity = 2
			desc =
			{
			"Flickering with eldritch energy, this powerful mage of the elements has been",
			"corrupted by Morgoth. With such mastery of the mysteries of the world,he",
			"will be a very difficult foe to defeat!",
			}
			color = color.STEEL_BLUE
			speed = 0 life = {9,9} ac = 50
			aaf = 30 sleep = 60
			exp = 400
			weight = 1800
			blows =
			{
				{"WEAPON","COLD",{4,4}},
				{"WEAPON","ELEC",{4,4}},
				{"WEAPON","CRUSH",{6,4}},
				{"WEAPON","FIRE",{4,4}},
			}
			flags =
			{
				----High AC (stone skin), summon golems and various attack spells.
				RESIST=getter.resists{ACID=66 COLD=66 FIRE=66 ELEC=66}
				DROP_THEME=getter.flags{THEME_MAGIC=70 THEME_TREASURE=30}
				FACTION=FACTION_DUNGEON HAS_LITE=true CORRUPTED=true
				KILL_WALL=getter.flags{WALL=true} AERIAL=true CAN_FLY=20
				ESCORT={3,6} ESCORT_CHARS="E" SPEEDS=getter.array{[SPEED_DIG]=300 CAST=10}
				MANA_CURR=300 MANA_MAX=300 MANA_REGENERATE=1500
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
				CASTING_AI=ai.SMART_CAST
				MOVE_AI=ai.SNAKE
				BEST_RANGE=3
				MIN_RANGE=1
				NO_SPELL_FLEE=true}
			}
			spells =
			{
				frequency=2
				["Fireflash"] = {level = 34 chance=85}
				["Ice Storm"] = {level = 10 chance=70}
				["Summon Elemental"] = {level = 15 chance=80}
				["Healing"] = {level = 20 chance=90}
				["Stone Prison"] = {level = 4 chance=80}
				["Tidal Wave"] = {level = 22 chance=80}
				["Noxious Cloud"] = {level = 48 chance=80}
				["Vapor"] = {level = 50 chance=90}
			}
		}
		[22] =
		{
			name = "archer"
			level = 50 rarity = 2
			desc =
			{
				"This yeoman is a master archer, the calluses on his fingers and the far-",
				"away look in his eye as much a warning as the finely polished bow of yew",
				"in his hands. He is an expert at killing at range.",
			}
			color = color.SANDY_BROWN
			speed = 10 life = {15,15} ac = 40
			aaf = 90 sleep = 20
			exp = 450
			weight = 2000
			blows =
			{
				{"WEAPON","SLASH",{5,5}},
				{"WEAPON","SLASH",{5,5}},
			}
			flags =
			{
				----Singular, attacks, fast.
				RESIST=getter.resists{CRUSH=10 PIERCE=10 SLASH=10}
				DROP_THEME=getter.flags{THEME_BOWS=50 THEME_COMBAT=30 THEME_TREASURE=20}
				FACTION=FACTION_DUNGEON SPEEDS=getter.speeds{CAST=10}
				LIMIT_SPELLS = getter.limit_spells{Arrow = 60}
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
				CASTING_AI=ai.SMART_CAST
				MOVE_AI=ai.ZOMBIE
				BEST_RANGE=5
				MIN_RANGE=2
				LURE_TARGET=true
				}
			}
			spells =
			{
				frequency=2
				["Arrow"] = {level=80 chance=90}
			}
		}
		[23] =
		{
			name = "druid"
			level = 51 rarity = 2
			desc =
			{
				"This human walks in simple robes, verging on grubby, and is crowned by",
				"a wreath of woven holly. This druid worships Yavanna Kementari, and his",
				"loyalties lie more with the creatures of air and forest than with men.",
			}
			color = color.DARK_GREEN
			speed = 10 life = {15,15} ac = 40
			aaf = 60 sleep = 40
			exp = 500
			weight = 2100
			blows =
			{
				{"WEAPON","CRUSH",{8,8}},
				{"WEAPON","CRUSH",{8,8}},
			}
			flags =
			{
				----Similar to current but with high regen
				RESIST=getter.resists{POIS=66}
				DROP_THEME=getter.flags{THEME_MAGIC=60 THEME_HAFTED=20 THEME_FOOD=20}
				FACTION=FACTION_ANIMAL CAN_SWIM=true
				REGENERATE=5000 PASS_WALL=getter.flags{PASS_TREES=100}
				MANA_CURR=300 MANA_MAX=300 MANA_REGENERATE=1500
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
				MOVE_AI=ai.ZOMBIE
				CASTING_AI=ai.SMART_CAST
				}
				ALLOW_IN_FOREST=true
			}
			spells=
			{
				frequency=3
					["Summon Animal"] = {level=5 chance=75}
					["Summon Animals"] = {level=5 chance=75}
					["Summon Plant"] = {level=5 chance=75}
					["Healing"] = {level=35 chance=90}
					["Grow Trees"] = {level=43 chance=85}
					["Ice Storm"] = {level=11 chance=75}
					["Geyser"] = {level=53 chance=95}
					["Vapor"] = {level=51 chance =94}
					["Tidal Wave"] = {level=23 chance =80}
			}
		}
		[24] =
		{
			name = "dark mage"
			level = 53 rarity = 1
			desc =
			{
				"Clad in blue robes speckled with runes, this mage fairly crackles with",
				"power, and the eyes peering over his tightly cropped black beard glow",
				"dull red.",
			}
			color = color.BLUE
			speed = 0 life = {9,9} ac = 30
			aaf = 30 sleep = 50
			exp = 550
			weight = 1550
			blows =
			{
				{"WEAPON","SLASH",{4,4}},
			}
			flags =
			{
				----Standard Mage, cowardly except in extremis
				RESIST=getter.resists{CONFUSION=50}
				DROP_THEME=getter.flags{THEME_MAGIC=70 THEME_TREASURE=30}
				FACTION=FACTION_SAURON HAS_LITE=true
				MANA_CURR=300 MANA_MAX=300 MANA_REGENERATE=1500
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
				MOVE_AI=ai.ZOMBIE
				CASTING_AI=ai.SMART_CAST
		    NO_SPELL_FLEE=true
		    BEST_RANGE=2
		    MIN_RANGE=1
		    LURE_TARGET=true
				}
			}
			spells=
			{
				frequency=2
				["Fireflash"] = {level=37 chance=75}
				["Slow Monster"] = {level=37 chance=75}
				["Darkness"] = {level=55 chance=75}
				["Confuse"] = {level=42 chance=75}
				["Manathrust"] = {level=55 chance=85}
				["Essence of Speed"] = 	{level=22 chance=70}
				["Healing"] = {level=27 chance=80}
				["Phase Door"] = {level=55 chance=95}
				["Teleportation"] = {level=36 chance=90}
			}
		}
		[25] =
		{
			name = "easterling chieftain"
			level = 58 rarity = 2
			desc =
			{
			 	"Respelndent in his barbaric finery, this giant lord of the Easterlings drips",
			 	"with finery set into gleaming steel armour, painted with a lacquer the colour",
			 	"of blood. He is a scourge of Gondor, and legions of warriors answer his call!",
			}
			color = color.FIREBRICK
			speed = 25 life = {18,18} ac = 65
			aaf = 70 sleep = 10
			exp = 700
			weight = 3500
			blows =
			{
				{"WEAPON","POISON",{10,10}},
				{"WEAPON","POISON",{10,10}},
				{"WEAPON","POISON",{10,10}},
			}
			flags =
			{
				----Berserker-level, with escort
				RESIST=getter.resists{CONFUSION=50 CRUSH=20 DARK_FIRE=66 FEAR=100 PIERCE=20 POIS=66 SLASH=20}
				DROP_THEME=getter.flags{THEME_COMBAT=60 THEME_CANTRIPS=20 THEME_TREASURE=20}
				FACTION=FACTION_SAURON EVIL=true HAS_LITE=true ESCORT=true ESCORTED_BY_GROUPS=true
				ESCORT_NAME="easterling" REFLECTING=20
				SMART=true AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
				MOVE_AI=ai.ZOMBIE
				CASTING_AI=ai.SMART_CAST
				FEAR_LEVEL=0}
				SPEEDS=getter.speeds{WALK=-10 CAST=-20}
			}
			spells=
			{
				frequency=10
					["Summon Kin"] = {level=40 chance=70}
			}
		}
		[26] =
		{
			name = "gondorian commander"
			level = 58 rarity = 2
			desc =
			{
			 	"Tall and noble, this great lord of Gondor bears an ancient sword of Gondor",
			 	"alive with flame and a great host of warriors ready to cry aloud the name",
			 	"of Gondor and smite the forces of darkness.",
			}
			color = color.HONEYDEW
			speed = 20 life = {20,20} ac = 50
			aaf = 70 sleep = 10
			exp = 700
			weight = 3600
			blows =
			{
				{"WEAPON","VALINOREAN_FIRE",{15,15}},
				{"WEAPON","VALINOREAN_FIRE",{15,15}},
			}
			flags =
			{
				----Berserker-level, with escort
				RESIST=getter.resists{CONFUSION=50 CRUSH=25 FEAR=100 FIRE=66 PIERCE=25 SLASH=25}
				DROP_THEME=getter.flags{THEME_COMBAT=60 THEME_CANTRIPS=20 THEME_TREASURE=20}
				FACTION=FACTION_GONDOR GOOD=true HAS_LITE=true ESCORT=true ESCORTED_BY_GROUPS=true
				REFLECTING=20
				SMART=true AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
				MOVE_AI=ai.ZOMBIE
				CASTING_AI=ai.SMART_CAST
				FEAR_LEVEL=0}
				SPEEDS=getter.speeds{WALK=-10 CAST=-20}
			}
			spells=
			{
				frequency=10
					["Summon Kin"] = {level=50 chance=70}
			}
		}
		[27] =
		{
			name = "legionnaire of morgoth"
			level = 62 rarity = 1
			desc =
			{
				"These warriors wear armour dark and twisted, as if burned in the fires",
				"of Orodruin, and their gutteral war-chants are spoken in the foul tongue",
				"of Mordor. They are Sauron's elite guard, and are filled with his power!"
			}
			color = color.DARK_ORCHID
			speed = 10 life = {13,13} ac = 55
			aaf = 20 sleep = 60
			exp = 450
			weight = 2800
			blows =
			{
				{"WEAPON","DARK_FIRE",{10,10}},
				{"WEAPON","DARK_FIRE",{10,10}},
			}
			flags =
			{
				----Hardened Warrior (Groups) Darkfire attacks
				RESIST=getter.resists{CRUSH=15 DARK_FIRE=66 FIRE=66 PIERCE=15 SLASH=15}
				DROP_THEME=getter.flags{THEME_COMBAT=60 THEME_MAGIC=20 THEME_TREASURE=20}
				FACTION=FACTION_SAURON EVIL=true CORRUPTED=true HAS_LITE=true AI=ai.SPELL_AND_MOVE
				MANA_CURR=70 MANA_MAX=70 MANA_REGENERATE=350 FRIENDS=getter.friends(100,8)
				REFLECTING=10 AI_STATES=getter.flags{
				MOVE_AI=ai.ZOMBIE
				CASTING_AI=ai.SMART_CAST}
			}
			spells=
			{
				frequency=8
				["Cause Wounds"] = {level=60 chance=80}
				["Bolt of Darkfire"] = {level=40 chance=70}
				["Darkness"]={level=50 chance=75}
			}
		}
		[28] =
		{
			name = "master thief"
			level = 65 rarity =2
			desc =
			{
				"A man-shaped shadow detaches itself from a wall and glides towards you.",
				"This thief is a master of his stealthy crafts, and has cunning, guile and",
				"a bevy of tricks, devices and traps to separate you from both treasure",
				"and your ability to breathe.",
			}
			color = color.DARK
			speed = 20 life = {20,12} ac =80
			aaf = 90 sleep = 0
			exp = 770
			weight = 1800
			blows =
			{
				{"WEAPON","BLIND",{14,7}},
				{"WEAPON","CONFUSE",{14,7}},
				{"WEAPON","POISON",{14,7}},
				{"TOUCH","EAT_GOLD",{0,0}}
				{"TOUCH","EAT_ITEM",{0,0}}
			}
			flags =
			{
				----Speedy, Stealthy, Trap-y
				RESIST=getter.resists{SLASH=5 CRUSH=5 PIERCE=5 DARK=66}
				DROP_THEME=getter.flags{THEME_KNIFE=20 THEME_BOWS=20 THEME_TREASURE=30 THEME_TOOLS=30}
				FACTION=FACTION_DUNGEON SMART=true INVISIBLE=true NO_AUTO_TARGET=true
				ATTR_CLEAR=true SPEEDS=getter.speeds{WALK=10} SEE_INVIS=90
				LIMIT_SPELLS = getter.limit_spells{Arrow = 30}
				AI=ai.SPELL_AND_MOVE
				AI_STATES=getter.flags{
					MOVE_AI=ai.SNAKE
					CASTING_AI=ai.SMART_CAST
					LURE_TARGET=true
					FEAR_LEVEL=10}
			}
			spells =
			{
				frequency=3
				["Arrow"] = {level= 60 chance = 65}
				["Create Traps"] = {level= 80 chance = 80}
				["Trap Filling"] = {level= 65 chance = 80}
			}
		}
		[29] =
		{
			name = "necromancer"
			level = 67 rarity = 2
			desc =
			{
				"Tall and sepuchural, this mage is adorned with a variety of charms and",
				"amulets to protect himself against evil. Little wonder, given that he",
				"is surrounded by his undead minions, whose hatred for the living is",
				"surpassed only by their fear of their master!",
			}
			color = color.WHITE
			speed = 10 life = {11,11} ac = 40
			aaf = 40 sleep = 40
			exp = 800
			weight = 1600
			blows =
			{
				{"WEAPON","SLASH",{4,8}},
				{"WEAPON","DARK_FIRE",{4,16}},
			}
			flags =
			{
				RESIST=getter.resists{CONFUSION=50 DARK_FIRE=66 FEAR=100 POIS=66}
				DROP_THEME=getter.flags{THEME_MAGIC=70 THEME_TREASURE=30}
				FACTION=FACTION_UNDEAD HAS_LITE=true SMART=true
				MANA_CURR=400 MANA_MAX=400 MANA_REGENERATE=2000
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
					CASTING_AI=ai.SMART_CAST
					MOVE_AI=ai.ZOMBIE
				}
				ESCORT=true ESCORT_CHARS="szGWVL"
			}
			spells=
			{
				frequency=2
					["Scare"] = {level=71 chance=80}
					["Animate Dead"] = {level=71 chance=80}
					["Stun"] = {level=71 chance=75}
					["Summon Undead"] = {level=80 chance=80}
					["Summon High Undead"] = {level=70 chance=80}
					["Disease"] = {level=70 chance=80}
			}
		}
		[30] =
		{
			name = "old druid"
			level = 73 rarity = 2
			desc =
			{
				"This old druid is whipcord thin, and brown as a nut. Age amongst his",
				"brotherhood denotes power rather than decripitude, and he is amongst",
				"the most powerful and most dangerous of nature's champions!",
			}
			color = color.DARK_SEA_GREEN
			speed = 10 life = {25,20} ac = 60
			aaf = 60 sleep = 20
			exp = 900
			weight = 1700
			blows =
			{
				{"WEAPON","CRUSH",{10,11}},
				{"WEAPON","COLD",{10,11}},
				{"WEAPON","ACID",{10,11}},
			}
			flags =
			{
				----Similar to current but with high regen
				RESIST=getter.resists{CONFUSION=50 CRUSH=10 PIERCE=10 POIS=100 SLASH=10}
				DROP_THEME=getter.flags{THEME_MAGIC=60 THEME_HAFTED=20 THEME_FOOD=20}
				FACTION=FACTION_ANIMAL NO_STUN=true
				REGENERATE=9000 PASS_WALL=getter.flags{PASS_TREES=100}
				MANA_CURR=400 MANA_MAX=400 MANA_REGENERATE=2000
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
					MOVE_AI=ai.ZOMBIE
					CASTING_AI=ai.SMART_CAST
				}
				ESCORT=true ESCORT_FLAGS=getter.flags{ANIMAL=true}
			}
			spells=
			{
				frequency=3
					["Summon Animal"] = {level=28 chance=85}
					["Summon Animals"] = {level=85 chance=85}
					["Summon Plant"] = {level=75 chance=85}
					["Healing"] = {level=58 chance=90}
					["Grow Trees"] = {level=66 chance=75}
					["Slow Monster"] = {level=50 chance=80}
					["Tidal Wave"] = {level=71 chance=80}
					["Essence of Speed"] = {level=48 chance=80}
					["Stone Prison"] = {level=20 chance=80}
					["Ice Storm"] = {level=27 chance=80}
					["Strike"] = {level=18 chance =85}
					["Shake"] = {level=24 chance =85}
			}
		}
		[31] =
		{
			name = "corrupted mindcrafter"
			level = 75 rarity = 2
			desc =
			{
				"This disciple of Saruman is a master of the mind, able to bend men and",
				"even anumals to his will. You force your gaze away from his, for even"
				"the strongest of men would eventually fall under his sway."
			}
			color = color.AQUAMARINE
			speed = 5 life = {18,15} ac = 45
			aaf = 90 sleep = 10
			exp = 900
			weight = 190
			blows =
			{
				{"WEAPON","CONFUSE",{4,4}},
				{"WEAPON","PARALYZE",{4,4}},
				{"WEAPON","FEAR",{4,4}},
			}
			flags =
			{
				----Confuse, slow, Psionic attacks and cowardice to boot
				RESIST=getter.resists{CONFUSION=50 FEAR=100}
				DROP_THEME=getter.flags{THEME_MAGIC=70 THEME_TREASURE=30} CHAR_MULTI=true
				FACTION=FACTION_ISENGARD WEIRD_MIND=true SMART=true NO_SLEEP=true
				SPEEDS=getter.speeds{CAST=10} NO_STUN=true
				MANA_CURR=500 MANA_MAX=500 MANA_REGENERATE=2000 NO_STUN=true
				CORRUPTED=true AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
				CASTING_AI=ai.SMART_CAST
				MOVE_AI=ai.SNAKE
				BEST_RANGE=3
				MIN_RANGE=1
				NO_SPELL_FLEE=true
				}
			}
			spells=
			{
				frequency=2
					["Neural Blast"] = {level=76 chance=85}
					["Confuse"] = {level=69 chance=85}
					["Scare"] = {level=69 chance=85}
					["Stun"] = {level=49 chance=85}
					["Charm"] = {level=77 chance=85}
					["Slow Monster"] = {level=59 chance=85}
					["Phase Door"] = {level=38 chance=90}
					["Teleportation"] = {level=20 chance=90}
					["Teleport Away"] = {level=33 chance=88}
			}
		}
		[32] =
		{
			name = "alchemist"
			level = 77 rarity = 5
			desc =
			{
				"Clad from head to toe in heavy leathers, wearing black glass goggles",
				"this strange man is a master alchemist, expert at crafting all manner",
				"of magical device, most of which he seems to have brought with him!"
			}
			color = color.UMBER
			speed = 10 life = {16,17} ac = 60
			aaf = 40 sleep = 60
			exp = 940
			weight = 2000
			blows =
			{
				{"WEAPON","SLASH",{6,6}},
				{"WEAPON","SLASH",{6,6}},
			}
			flags =
			{
				----Wands (spell attacks) with limited “charges” and high base resists
				RESIST=getter.resists{ACID=80 COLD=80 CRUSH=8 DISENCHANT=66 LITE=66 FIRE=80 ELEC=80 PIERCE=8 POIS=66 SLASH=8}
				DROP_THEME=getter.flags{THEME_DEVICE=60 THEME_CANTRIPS=20 THEME_TREASURE=20}
				FACTION=FACTION_DUNGEON HAS_LITE=true MANA_CURR=9999 MANA_MAX=9999 MANA_REGENERATE=0
				SPEEDS=getter.speeds{CAST=10} AI=ai.SPELL_AND_MOVE  AI_STATES=getter.flags{
				CASTING_AI=ai.SMART_CAST
				MOVE_AI=ai.ZOMBIE
				BEST_RANGE=2
				MIN_RANGE=1}
				LIMIT_SPELLS = getter.limit_spells{["Noxious Cloud"] = 6, Thunderstorm = 3, Teleportation = 11, ["Stone Prison"] = 6, Shake = 5, Fireflash = 10, Manathrust = 17, Confuse = 10, Healing = 8, ["Ice Storm"] = 4, ["Essence of Speed"] = 7}
			}
			spells =
			{
				frequency=3
					["Noxious Cloud"] = {level=75 chance=95}
					["Thunderstorm"] = {level=65 chance=95}
					["Teleportation"] = {level=80 chance=95}
					["Stone Prison"] = {level=65 chance=95}
					["Shake"] = {level=70 chance=95}
					["Fireflash"] = {level=75 chance=95}
					["Manathrust"] = {level=80 chance=95}
					["Confuse"] = {level=70 chance=95}
					["Healing"] = {level=80 chance=95}
					["Ice Storm"] = {level=80 chance=95}
					["Essence of Speed"] = {level=80 chance=95}
			}
		}
		[33] =
		{
			name = "witch-soldiers of mordor"
			level = 80 rarity = 2
			desc =
			{
				"These terrible warriors are the most dangerous soldiers that Sauron has",
				"and their mastery of both blade and spell make them man-for-man without",
				"peer throughout Arda. Their skull-masks are made from mithril and their",
				"black cloaks seem to suck the light from the air...",
			}
			color = color.OLD_LACE
			speed = 30 life = {20,15} ac = 70
			aaf = 30 sleep = 30
			exp = 980
			weight = 3000
			blows =
			{
				{"WEAPON","SLASH",{12,12}},
				{"WEAPON","POISON",{12,12}},
				{"WEAPON","DARK_FIRE",{12,12}},
			}
			flags =
			{
				----Like warlocks, but tougher and in groups.
				RESIST=getter.resists{COLD=66 DARK=66 DARK_FIRE=66 FIRE=66 LITE=-30 POIS=66}
				DROP_THEME=getter.flags{THEME_COMBAT=60 THEME_MAGIC=30 THEME_TREASURE=10}
				FACTION=FACTION_SAURON HAS_LITE=true REFLECTING=15 EVIL=true
				FORCE_MAXHP=true SPEEDS=getter.speeds{CAST=-20 WALK=-10}
				FRIENDS=getter.friends(100,11) REGENERATE=2000
				MANA_CURR=200 MANA_MAX=200 MANA_REGENERATE=1000
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
				CASTING_AI=ai.SMART_CAST
				MOVE_AI=ai.ZOMBIE
				FEAR_LEVEL=0
				LURE_TARGET=true}
			}
			spells=
			{
				frequency=5
				["Darkness"] ={level=80 chance=95}
				["Drain Mana"] = {level=80 chance=85}
				["Disease"] = {level=60 chance=85}
				["Bolt of Darkfire"] ={level=80 chance=80}
				["Summon Undead"] = {level=60 chance=80}
			}
		}
		[34] =
		{
			name = "sorceror"
			level = 84 rarity = 3
			desc =
			{
				"This red-clad mage is a master of all the world's magick, and uses these",
				"masteries to plunder unwary adventures like yourself! He looks at",
				"you, wondering which spell he will select to destroy you!",
			}
			color = color.RED
			speed = 0 life = {11,11} ac = 40
			aaf = 40 sleep = 30
			exp = 1100
			weight = 1600
			flags =
			{
				----Magic Only, and a coward.
				RESIST=getter.resists{ACID=66 COLD=66 CONFUSION=50  FIRE=66 ELEC=66 POIS=66}
				DROP_THEME=getter.flags{THEME_MAGIC=80 THEME_TREASURE=20}
				FACTION=FACTION_DUNGEON HAS_LITE=true SPEEDS=getter.speeds{CAST=10}
				MANA_CURR=650 MANA_MAX=650 MANA_REGENERATE=3250
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
					CASTING_AI=ai.SMART_CAST
					MOVE_AI=ai.ZOMBIE
					BEST_RANGE=3
					MIN_RANGE=1
					NO_SPELL_FLEE=true
					}
			}
			spells=
			{
				frequency=2
					["Fireflash"] = {level=69 chance=85}
					["Slow"] = {level=69 chance=85}
					["Darkness"] = {level=75 chance=85}
					["Confuse"] = {level=79 chance=85}
					["Manathrust"] = {level=87 chance=85}
					["Essence of Speed"] = {level=59 chance=70}
					["Healing"] = {level=69 chance=80}
					["Phase Door"] = {level=87 chance=95}
					["Teleportation"] = {level=69 chance=90}
					["Summon Monster"] = {level=75 chance=85}
					["Tidal Wave"] = {level=57 chance=85}
					["Essence of Speed"] = {level=59 chance=85}
					["Stone Prison"] = {level=39 chance=85}
					["Ice Storm"] = {level=45 chance=85}
					["Shake"] = {level=35 chance =85}
					["Noxious Cloud"] = {level=83 chance=85}
			}
		}
		[35] =
		{
			name = "demonologist"
			level = 85 rarity =2
			desc =
			{
				"A demon! No... despite the horns, claws and the shadowy wings, there is",
				"a human within this horrific figure, a man steeped so deeply in demon",
				"lore, he is able to ape their shape and their magic with ease!",
			}
			color = color.ORANGE
			speed = 15 life = {20,20} ac = 0
			aaf = 40 sleep = 40
			exp = 1300
			weight = 4000
			blows =
			{
				{"WEAPON","DARK_FIRE",{13,9}},
				{"CLAW_OFFHAND","SLASH",{12,10}},
			}
			flags =
			{
				----Summon those evil buggers like there's no tomorrow
				RESIST=getter.resists{FIRE=100 DARK_FIRE=66 COLD=-66 LITE=-66}
				DROP_THEME=getter.flags{THEME_DEMON=50 THEME_MAGIC=30 THEME_TREASURE=20}
				FACTION=FACTION_SAURON BLOW_RESPONSE=getter.array{[AURA_SHARDS]={12,14}}
				EVIL=true CORRUPTED=true REFLECTING=30 AERIAL=true CAN_FLY=20 MANA_CURR=250
				MANA_MAX=250 DEMON=true ESCORT=true ESCORTED_BY_GROUPS=true
				ESCORT_CHARS="Uu"
				MANA_REGENERATE=1000 AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
					CASTING_AI=ai.SMART_CAST
				  MOVE_AI=ai.ZOMBIE}
			}
			spells=
			{
				frequency=8
					["Demon Madness"] = {level=69 chance=85}
					["Summon Demon"] = {level=79 chance=80}
					["Demon Field"] = {level=49 chance=70}
					["Healing"] = {level=60 chance=70}
					["Demonic Howl"] = {level=69 chance=70}
			}
		}
		[36] =
		{
			name = "warrior lord"
			level = 87 rarity = 2
			desc =
			{
				"This warrior is clad in the heaviest of plate yet moves fast as the wind",
				"and holds a great lance in his hand as if it were no more than a sapling!",
				"He is the cream of the warrior class and a fearsome foe for even the",
				"mightiest of lords!"
			}
			color = color.CHOCOLATE
			speed = 30 life = {30,20} ac = 85
			aaf = 50 sleep = 20
			exp = 1500
			weight = 3000
			blows =
			{
				{"WEAPON","PIERCE",{15,15}},
				{"WEAPON","PIERCE",{15,15}},
				{"WEAPON","PIERCE",{15,15}},
			}
			flags =
			{
				----Fast, hits like a bugger and has reflect/mundane resists
				RESIST=getter.resists{ACID=66 COLD=66 CONFUSION=50 CRUSH=30 ELEC=66 FEAR=100 FIRE=66 PIERCE=30 SLASH=30}
				DROP_THEME=getter.flags{THEME_POLEARM=50 THEME_COMBAT=30 THEME_TREASURE=20}
				FACTION=FACTION_DUNGEON HAS_LITE=true SMART=true SPEEDS=getter.speeds{CAST=-20 WALK=-10}
				LIMIT_SPELLS = getter.limit_spells{Arrow = 30} NO_STUN=true NO_SLEEP=true
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
					CASTING_AI=ai.SMART_CAST
					MOVE_AI=ai.ZOMBIE
				}
			}
			spells=
			{
				frequency=7
					["Arrow"] = {level=65 chance=80}
			}
		}
		[37] =
		{
			name = "dagger of mordor"
			level = 87 rarity = 2
			desc =
			{
				"Sauron has many secrets in his fortress, and here is one of the deadliest,",
				"an assassin trained since birth to kill quietly and without detection. A",
				"cloak of grey hides many devices of compact death!"
			}
			color = color.SLATE
			speed = 30 life = {13,13} ac =90
			aaf = 90 sleep =0
			exp = 1800
			weight = 1800
			blows =
			{
				{"WEAPON","POISON",{12,12}},
				{"WEAPON","CONFUSE",{8,9}},
				{"WEAPON_OFFHAND","PARALYZE",{8,9}},
				{"WEAPON_OFFHAND","PIERCE",{8,9}},
			}
			flags =
			{
				----Singular, traps, invisible, poison attacks
				RESIST=getter.resists{DARK=66 POIS=100}
				DROP_THEME=getter.flags{THEME_SWORD=30 THEME_BOWS=20 THEME_TREASURE=10 THEME_TOOLS=20 THEME_CANTRIPS=20}
				FACTION=FACTION_SAURON INVISIBLE=true NO_AUTO_TARGET=true EVIL=true
				MANA_MAX=50 MANA_CURR=50 MANA_REGENERATE=250
				LIMIT_SPELLS = getter.limit_spells{["Bolt of Poison"]=35}
				ATTR_CLEAR=true SMART=true AI=ai.SPELL_AND_MOVE
				AI_STATES=getter.flags{
				CASTING_AI=ai.SMART_CAST
				MOVE_AI=ai.PATHFIND
				LURE_TARGET=true
				FEAR_LEVEL=15
				}
			}
			spells =
			{
			frequency=4
				["Bolt of Poison"] = {level=80 chance=85}
				["Trap Filling"] = {level= 80 chance = 80}
				["Phase Door"] = {level=40 chance=80}
				["Teleportation"] = {level=40 chance=80}
			}
		}
		[38] =
		{
			name = "old sorceror"
			level = 95 rarity = 6
			desc =
			{
			 	"Ths old, fine-boned man wears a simple robe, and is without jewels",
			 	"or ornament. He is also one of the most powerful mages in Arda, with",
			 	"power pulsing in the air above him!",
			}
			color = color.GOLD
			speed = 20 life = {15,15} ac = 60
			aaf = 40 sleep = 30
			exp = 2300
			weight = 1600
			flags =
			{
				----Magic Only, and a coward.
				RESIST=getter.resists{ACID=66 COLD=66 CONFUSION=50 FIRE=66 ELEC=66 POIS=66}
				DROP_THEME=getter.flags{THEME_MAGIC=80 THEME_TREASURE=20}
				FACTION=FACTION_DUNGEON HAS_LITE=true SPEEDS=getter.speeds{CAST=10}
				MANA_CURR=1000 MANA_MAX=1000 MANA_REGENERATE=5000 NO_SLEEP=true
				NO_STUN=true SMART=true
				LIMIT_SPELLS = getter.limit_spells{Manaflow=2}
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
					CASTING_AI=ai.SMART_CAST
					MOVE_AI=ai.ZOMBIE
					BEST_RANGE=3
					MIN_RANGE=1
					NO_SPELL_FLEE=true}
			}
			spells=
			{
				frequency=2
					["Fireflash"] = {level=79 chance=85}
					["Slow"] = {level=79 chance=85}
					["Darkness"] = {level=95 chance=85}
					["Confuse"] = {level=89 chance=85}
					["Manathrust"] = {level=97 chance=85}
					["Essence of Speed"] = {level=69 chance=70}
					["Healing"] = {level=79 chance=80}
					["Phase Door"] = {level=97 chance=95}
					["Teleportation"] = {level=79 chance=90}
					["Summon Monsters"] = {level=85 chance=85}
					["Tidal Wave"] = {level=867 chance=85}
					["Stone Prison"] = {level=49 chance=85}
					["Ice Storm"] = {level=55 chance=85}
					["Shake"] = {level=45 chance =85}
					["Noxious Cloud"] = {level=93 chance =85}
					["Manaflow"] = {level=1 chance =85}
					["Summon High Undead"] = {level=85 chance =85}
					["Hand of Doom"] = {level=70 chance =85}
					["Teleport Away"] = {level=53 chance =85}
			}
		}
		[39] =
		{
			name = "knight commander of mordor"
			level = 95 rarity = 5
			desc =
			{
				"Encased in gleaming silver armour, this human looks almost noble until",
				"you see the foul standard of Mordor engraved on his shield. He is a",
				"High Commander, answering to Sauron himself. He is mighty in both the",
				"arts of war and of magic. You will not find a more fearsome living",
				"servant of darkness in the whole of Arda!"
			}
			color = color.LIGHT_STEEL_BLUE
			speed = 30 life = {30,25} ac = 90
			aaf = 90 sleep = 0
			exp = 2600
			weight = 3200
			blows =
			{
				{"WEAPON","DARK_FIRE",{15,15}},
				{"WEAPON","DARK_FIRE",{15,15}},
				{"WEAPON","CRUSH",{15,15}},
				{"WEAPON","SLASH",{15,15}},
			}
			flags =
			{
				----The witch-king, pre-wraithness. Also with an escort of evil.
				RESIST=getter.resists{ACID=66 COLD=66 CONFUSION=100 CRUSH=25 DARK_FIRE=66 ELEC=66 FEAR=100 FIRE=66 PIERCE=25 POIS=66 SLASH=25}
				DROP_THEME=getter.flags{THEME_COMBAT=60 THEME_MAGIC=30 THEME_TREASURE=10}
				FACTION=FACTION_SAURON FORCE_MAXHP=true NO_SLEEP=true NO_STUN=true
				--ESCORT=true ESCORT_NAME={"mordor","sauron","morgoth"} ESCORTED_BY_GROUPS=true
				ESCORT=true ESCORT_NAME="mordor" ESCORTED_BY_GROUPS=true
				HAS_LITE=true SMART=true EVIL=true SPEEDS=getter.speeds{CAST=-15}
				CORRUPTED=true MANA_CURR=300 MANA_MAX=300 MANA_REGENERATE=1500
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
				CASTING_AI=ai.SMART_CAST
				MOVE_AI=ai.PATHFIND
				FEAR_LEVEL=15}
			}
			spells=
			{
				frequency=5
					["Darkness"] ={level=80 chance=95}
					["Drain Mana"] = {level=90 chance=85}
					["Disease"] = {level=80 chance=85}
					["Bolt of Darkfire"] ={level=80 chance=80}
					["Summon Undead"] = {level=60 chance=80}
					["Summon Kin"] = {level =80 chance=80}
					["Summon High Undead"] = {level =80 chance=80}
					["Hand of Doom"] = {level =80 chance=80}
					["Teleportation"] = {level =80 chance=80}
			}
		}
	}
	['q'] =
		{
		defaults =
			{
				body=default_body.quadruped
				flags={
					ALLOW_IN_PLAIN=true ALLOW_IN_FOREST=true
					OPEN_DOOR=false BASH_DOOR=true
					}
			} -- defaults
		[1] =
		{
			name = "cart horse"
			level = 5 rarity = 3
			desc = 
			{
				"This old nag has seen better days."
			}
			color = color.LIGHT_DARK
			speed = 0 life = {10,4} ac = 9
			aaf = 20 sleep = 70
			exp = 4
			weight = 3000
			blows =
			{
				{"KICK","CRUSH",{4,4}}
			}
			flags =
			{
				AI=ai.ZOMBIE RIDEABLE=4 FEMALE=true
				FACTION=FACTION_ANIMAL STUPID=true
			}
		}
		[2] =
		{
			name = "black bear"
			level = 8 rarity = 2
			desc = 
			{
				"This small bear is usually shy around humans."
			}
			color = color.LIGHT_DARK
			speed = 0 life = {6,6} ac = 15
			aaf = 80 sleep = 40
			exp = 30
			weight = 1800
			blows =
			{
				{"CLAW", "SLASH", {3,5}},
				{"CLAW", "SLASH", {3,5}},
			}
			flags =
			{
				AI=ai.ZOMBIE STUPID=true PASS_WALL=getter.flags{PASS_TREES=100} 
				FACTION=FACTION_ANIMAL
			}
		}
		[3] =
		{
			name = "rohirrim scout"
			level = 10 rarity = 2
			desc = 
			{
				"This lightly armed and armoured horseman rides one of the lean",
				"racing horses of Rohir.",
			}
			color = color.LIGHT_BLUE
			speed = 5 life = {7,7} ac = 30
			aaf = 70 sleep = 20
			exp = 80
			weight = 4700
			blows =
			{
				{"KICK","CRUSH",{5,5}}, 
				{"WEAPON","SLASH",{6,6}},
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=5 SLASH=8 PIERCE=5}
				AI=ai.ZOMBIE SPEEDS=getter.speeds{WALK=20}
				FACTION=FACTION_ROHAN ANIMAL=true HUMAN=true
				DROP_THEME=getter.flags{THEME_COMBAT=50 THEME_FOOD=30 THEME_TREASURE=20}
			}
		}
		[4] =
		{
			name = "easterling skirmisher"
			level = 20 rarity = 2
			desc = 
			{
				"Waving their curved blades wildly, these mounted troops hunt in packs",
				"and fall upon the weak!",
			}
			color = color.DARK_RED
			speed = 0 life = {8,8} ac = 30
			aaf = 50 sleep = 50
			exp =  60
			weight = 4700
			blows =
			{
				{"WEAPON","SLASH",{8,8}}
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=5 DARK_FIRE=50 SLASH=8}
				AI=ai.ZOMBIE FRIENDS=getter.friends(80,7) 
				AI_STATES=getter.flags{LURE_FROM_CORRIDOR=true} SPEEDS=getter.speeds{WALK=20}
				FACTION=FACTION_SAURON ANIMAL=true HUMAN=true
				DROP_THEME=getter.flags{THEME_COMBAT=50 THEME_FOOD=30 THEME_TREASURE=20}
			}
		}
		[5] =
		{
			name = "brown bear"
			level = 23 rarity = 2
			desc = 
			{
				"This large bear is big enough to cause some serious damage if it got",
				"close to you! Best keep your distance.",
			}
			color = color.CHOCOLATE
			speed = 0 life = {9,8} ac = 20
			aaf = 60 sleep = 40
			exp = 180
			weight = 2100
			blows =
			{
				{"CLAW","SLASH",{4,4}},
				{"CLAW","SLASH",{4,4}}, 
				{"BITE","PIERCE",{6,6}},
			}
			flags =
			{
				RESIST=getter.resists{FEAR=50}
				ANIMAL=true AI=ai.ZOMBIE FACTION=FACTION_ANIMAL
				PASS_WALL=getter.flags{PASS_TREES=100} 
			}
		}
		[6] =
		{
			name = "warg rider"
			level = 30 rarity = 2
			desc = 
			{
				"Each of these massive Wargs are topped by an orc wielding a wicked",
				"looking flail. They both howl at you, and it's a toss-up as to who",
				"looks the more bestial!"
			}
			color = color.DARK_SLATE_GRAY
			speed = 10 life = {7,8} ac = 40
			aaf = 40 sleep = 40
			exp = 170
			weight =  3000
			blows =
			{
				{"WEAPON","SLASH",{6,6}}, 
				{"BITE","PIERCE",{8,8}},
			}
			flags =
			{
				RESIST=getter.resists{DARK_FIRE=66 SLASH=6 PIERCE=4 CRUSH=6 LITE=-66}
				AI=ai.ZOMBIE AI_STATES=getter.flags{LURE_FROM_CORRIDOR=true}  
				ANIMAL=true ORC=true EVIL=true FRIENDS=getter.friends(80,8)
				FACTION=FACTION_SAURON
				DROP_THEME=getter.flags{THEME_HAFTED=30 THEME_ARMOR=30 THEME_JUNK=20 THEME_TREASURE=20}
			}
		}
		[7] =
		{
			name = "rohirrhim lancer"
			level = 35 rarity = 4
			desc = 
			{
				"Holding long and deadly spears, these fierce horsemen form the backbone",
				"of the forces of Rohan, man and beast alike in skill and valour!",
			}
			color = color.BLUE
			speed = 5 life = {11,11} ac = 50
			aaf = 28 sleep = 30
			exp = 200
			weight = 5500
			blows =
			{
				{"WEAPON","PIERCE",{10,8}}, 
				{"KICK","CRUSH",{6,6}}, 
			}        
			flags =
			{
				RESIST=getter.resists{SLASH=12 PIERCE=12 CRUSH=12}
				HUMAN=true ANIMAL=true SPEEDS=getter.speeds{WALK=10} AI=ai.ZOMBIE
				AI_STATES=getter.flags{LURE_FROM_CORRIDOR=true} FACTION=FACTION_ROHAN 
				FRIENDS=getter.friends(80,7)
				DROP_THEME=getter.flags{THEME_POLEARM=30 THEME_COMBAT=40 THEME_TREASURE=30}
			}
		}
		[8] =
		{
			name = "cave bear"
			level = 35 rarity = 5
			desc = 
			{
				"This massive bear is a throwback to an earlier age of Arda, its"
				"great claws and fangs a relic to a simpler, more violent time!"
			}
			color = color.DARK_KHAKI
			speed = 5 life = {12,12} ac = 40
			aaf = 40 sleep = 40
			exp = 350
			weight = 4000
			blows =
			{
				{"CLAW","SLASH",{6,6}}, 
				{"CLAW","CRUSH",{6,6}}, 
				{"BITE","PIERCE",{8,8}},
			}
			flags =
			{
				RESIST=getter.resists{CONFUSION=100 FEAR=50}
				ANIMAL=true POWERFUL=true AI=ai.ZOMBIE AI_STATES=getter.flags{FEAR_LEVEL=5}
				FACTION=FACTION_ANIMAL REGENERATE=1000
			}
		}
		[9] =
		{
			name = "war bear"
			level = 40 rarity = 2
			desc = 
			{
				"With rings and spikes driven into their very flesh and their eyes burning",
				"with the suffering of the pits of Barad-Dur, these bears are lost to",
				"Sauron's corruption and hunt in packs to tear and rend his foes!",
			}
			color = color.SALMON
			speed = 5 life = {10,10} ac = 30
			aaf = 60 sleep = 60
			exp = 250
			weight =3000 
			blows =
			{
				{"CLAW","SLASH",{6,4}}, 
				{"CLAW","CRUSH",{6,4}}, 
				{"BITE","PIERCE",{6,8}}				
			}
			flags =
			{
				RESIST=getter.resists{CONFUSION=100 DARK_FIRE=50 SLASH=8 FEAR=50}
				ANIMAL=true EVIL=true AI=ai.ZOMBIE FRIENDS=getter.friends(95,15)
				AI_STATES=getter.flags{
				FEAR_LEVEL=5
				LURE_FROM_CORRIDOR=true}
				CORRUPTED=true FACTION=FACTION_SAURON REGENERATE=1000
			}
		}
		[10] =
		{
			name = "meara"
			level = 40 rarity = 4
			desc = 
			{
				"Proud and free, this great warhorse paws the earth nervously. and",
				"tosses its mane Trained for battle by some unknown lord, it is",
				"here, riderless. "
			}
			color = color.KHAKI
			speed = 10 life = {12,12} ac = 50
			aaf = 20 sleep = 30
			exp = 200
			weight = 4000
			blows =
			{
				{"KICK","CRUSH",{6,6}}, 
				{"KICK","CRUSH",{6,6}},
			}
			flags =
			{
				ANIMAL=true RIDEABLE=40 SPEED=getter.speeds{WALK=15} NO_STUN=true AI=ai.ZOMBIE
				FACTION=FACTION_ANIMAL
			}
		}
		[11] =                                                              
		{
			name = "easterling horseman"
			level = 40 rarity = 3
			desc = 
			{
				"These grim-faced cavalry from the east wield large swords and are",
				"poosesed with hatred for all who oppose their lord Sauron!",
			}
			color = color.RED
			speed = 10 life = {12,12} ac = 40
			aaf = 30 sleep = 30
			exp =  300
			weight = 5000
			blows =
			{
				{"WEAPON","SLASH",{8,8}}, 
				{"KICK","CRUSH",{6,6}}, 
			}
			flags =
			{
				RESIST=getter.resists{SLASH=10 PIERCE=10 CRUSH=10 DARK_FIRE=50}
				HUMAN=true ANIMAL=true SPEEDS=getter.speeds{WALK=10} AI=ai.ZOMBIE
				FACTION=FACTION_SAURON AI_STATES=getter.flags{LURE_FROM_CORRIDOR=true} 
				FRIENDS=getter.friends(80,13)
				DROP_THEME=getter.flags{THEME_SWORD=40 THEME_COMBAT=30 THEME_TREASURE=20 THEME_JUNK=10}
  			}
		}
		[12] =
		{
			name = "beorning"
			level = 50 rarity = 4
			desc = 
			{
				"This huge beast seems has a disturbing look of intelligence in its",
				"eyes, although its massive paws and fanged snout belong firmly to",
				"the bear family. ",
			}
			color = color.SLATE
			speed = 10 life = {15,15} ac = 35
			aaf = 60 sleep = 30
			exp =  500
			weight = 4000
			blows =
			{
				{"CLAW","SLASH", {8,8}}, 
				{"CLAW","CRUSH", {8,8}}, 
				{"BITE","PIERCE", {10,9}},
			}
			flags =
			{
				RESIST=getter.resists{CONFUSION=50 FEAR=100}
				HUMAN=true ANIMAL=true AI=ai.SPELL_AND_MOVE  
				AI_STATES=getter.flags{
				MOVE_AI=ai.ZOMBIE
				CASTING_AI=ai.SMART_CAST
				FEAR_LEVEL=20} 
				REGENERATE=5000
				PASS_WALL=getter.flags{PASS_TREES=100} 
				MANA_CURR=150 MANA_MAX=150 MANA_REGENERATE=750 
				FACTION=FACTION_ANIMAL
				DROP_THEME=getter.flags{THEME_FOOD=50 THEME_CANTRIPS=20 THEME_JUNK=30}
			}
			spells =
			{
				frequency=5
				["Summon Animal"] = { level=30 chance=50 }
			}
		}
		[13] =
		{
			name = "rohirrhim mounted archer"
			level = 57 rarity = 3
			desc = 
			{
				"This Rohirrhim archer appears to not be holding the great lance of his",
				"comptriots, and you relax. Then you spy the deadly little recurve bow",
				"strapped to the saddle and you tense up again. This horseman is as",
				"deadly a killer as his compatriots"
			}
			color = color.CADET_BLUE
			speed = 5 life = {12,12} ac = 45
			aaf = 30 sleep = 0
			exp =  450
			weight = 5500
			blows =
			{
				{"WEAPON","SLASH", {6,6}}, 
			}
			flags =
			{
				RESIST=getter.resists{SLASH=9 PIERCE=9 CRUSH=9}
				ANIMAL=true HUMAN=true LIMIT_SPELLS = getter.limit_spells{Arrow = 40}
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
				BEST_RANGE=6 
				MIN_RANGE=3 
				MOVE_AI=ai.SNAKE 
				CASTING_AI=ai.SMART_CAST
				} 
				SPEED=getter.speeds{WALK=10}
				FACTION=FACTION_ROHAN
				DROP_THEME=getter.flags{THEME_BOWS=50 THEME_COMBAT=30 THEME_TREASURE=20}
			}
			spells =
			{
				frequency = 3
				["Arrow"] = { level=40 chance=80 }
			}
		}
		[14] =
		{
			name = "easterling cavalry captain"
			level = 56 rarity = 3
			desc = 
			{
				"Resplendent in silks and mailed leathers, this barbarian commander sits",
				"astride a desert destrier, huge scimitar in hand and his dark eyes",
				"gleaming with both calculation and hatred!",
			}
			color = color.CRIMSON
			speed = 10 life = {16,16} ac = 60
			aaf = 60 sleep = 60
			exp = 875
			weight = 6500
			blows =
			{
				{"WEAPON","SLASH", {9,8}}, 
				{"WEAPON","SLASH", {9,8}}, 
				{"WEAPON","SLASH", {9,8}}, 
			}
			flags =
			{
				RESIST = obvious(getter.resists{CONFUSION=33 CRUSH=14 DARK_FIRE=50 FEAR=100 SLASH=14 PIERCE=14})				
				SMART=true FACTION=FACTION_SAURON AI=ai.ZOMBIE 
				AI_STATES=getter.flags{LURE_FROM_CORRIDOR=true FEAR_LEVEL=20}
				ESCORT=true ESCORTED_BY_GROUPS=true ESCORT_CHARS="qp" ESCORT_NAME="easterling"
				EVIL=true ANIMAL=true HUMAN=true
				DROP_THEME=getter.flags{THEME_SWORD=40 THEME_COMBAT=30 THEME_TREASURE=30}
			}
		}
		[15] =
		{
			name = "rohirrhim heavy lancer"
			level = 65 rarity = 2
			desc =                                           
			{
				"This gleaming warrior is seated upon the biggest, heaviest warhorse",
				"in all of Rohan, which is itself clad in heavy mail. The long lance and",
				"kite-like shield complete one of the deadlist cavalry fighters in Arda!",
			}
			color = color.STEEL_BLUE
			speed = 8 life = {16,14} ac = 40
			aaf = 30 sleep = 60
			exp = 500
			weight = 7000
			blows =
			{
				{"WEAPON","PIERCE", {14,9}}, 				
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=25 FEAR=10 PIERCE=25 SLASH=25}
				SPEED=getter.speeds{WALK=8} ANIMAL=true HUMAN=true
				FACTION=FACTION_ROHAN REFLECTING=30
				AI=ai.ZOMBIE AI_STATES=getter.flags{LURE_FROM_CORRIDOR=true} 
				FRIENDS=getter.friends(90,8)
				DROP_THEME=getter.flags{THEME_POLEARM=30 THEME_COMBAT=40 THEME_TREASURE=30}
			}
		}
		[16] =
		{
			name = "rohirrhim cavalry commander"
			level = 70 rarity = 3
			desc = 
			{
				"Proud sons of Rohan, arise! This noble warrior sits upon a great steed",
				"finely mailed and armed. A hero to his people and a terror to the foes",
				"of Rohan, this flaxen-haired horseman commands a troop of lancers."
			}
			color = color.ROYAL_BLUE
			speed = 10 life = {17,16} ac = 45
			aaf = 60 sleep = 40
			exp = 900
			weight = 7500
			blows =
			{
				{"WEAPON","SLASH", {10,8}}, 
				{"WEAPON","SLASH", {10,8}}, 
			}
			flags =
			{
				RESIST=getter.resists{CONFUSION=50 CRUSH=25 FEAR=100 PIERCE=25 SLASH=25}
				SPEED=getter.speeds{WALK=12} ANIMAL=true HUMAN=true
				FACTION=FACTION_ROHAN REFLECTING=30 ANIMAL=true HUMAN=true				
				SMART=true AI=ai.ZOMBIE 
				AI_STATES=getter.flags{LURE_FROM_CORRIDOR=true FEAR_LEVEL=20}
				ESCORT=true ESCORTED_BY_GROUPS=true
				DROP_THEME=getter.flags{THEME_SWORD=40 THEME_COMBAT=30 THEME_TREASURE=30}				
			}
		}
		[17] =
		{
			name = "orcish houndmaster"
			level = 75 rarity = 3
			desc = 
			{
				"Mounted on a huge white wolf, this tall orc bears a spear in one hand",
				"and a whip in the other hand to keep Sauron's packs in line. These two",
				"crave manflesh, and will try to feast on yours!",
			}
			color = color.WHITE
			speed = 15 life = {15,15} ac = 65
			aaf = 60 sleep = 40
			exp = 950
			weight = 6000
			blows =
			{
				{"WEAPON","PIERCE", {9,8}}, 
				{"WEAPON","SLASH", {5,5}},
				{"BITE","PIERCE", {10,8}}, 
				{"BITE","CRUSH", {10,8}},  			
			}
			flags =
			{
				RESIST=getter.resists{DARK_FIRE=66 SLASH=8 PIERCE=8 CRUSH=8 LITE=-66}
				ANIMAL=true ORC=true EVIL=true FACTION=FACTION_SAURON UNIQUE=true
			  ESCORT=true ESCORTED_BY_GROUPS=true ESCORT_CHARS="C" ESCORT_FLAGS=getter.flags{HUMAN=false}
			  AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
				LURE_FROM_CORRIDOR=true
				MOVE_AI=ai.PATHFIND
				CASTING_AI=ai.SMART_CAST}
				DROP_THEME=getter.flags{THEME_POLEARM=30 THEME_COMBAT=20 THEME_FOOD=20 THEME_TREASURE=20 THEME_JUNK=10}
			}
			spells =
			{
				frequency=5
				["Scare"] = {level=80 chance=80}
				["Howl"] = {level=80 chance=100}
				["Summon Kin"] = {level=70 chance=70}	
			}
		}
		[18] =
		{
			name = "elder beorning"
			level = 80 rarity = 4
			desc = 
			{
				"Streaks of grey mark the muzzle of this huge bear, although the fire",
				"in its eyes seems to have only grown with age, and it regards you",
				"without fear. This kin of Beorn is wise beyond measure and cunning in",
				"both human knowledge and the deeper mysteries of nature. You would be",
				"be wise not to quarrel with it!"
			}
			color = color.LIGHT_DARK
			speed = 10 life = {20,20} ac = 65
			aaf = 70 sleep = 30
			exp =  1200
			weight = 5000
			blows =
			{
				{"CLAW","SLASH", {10,10}}, 
				{"CLAW","CRUSH", {10,10}}, 
				{"BITE","PIERCE", {12,11}},
			}
			flags =
			{
				RESIST=getter.resists{CONFUSION=50 FEAR=100 POIS=66}
				HUMAN=true ANIMAL=true POWERFUL=true AI=ai.SPELL_AND_MOVE 
				AI_STATES=getter.flags{
				CASTING_AI=ai.SMART_CAST
				MOVE_AI=ai.ZOMBIE
				FEAR_LEVEL=20} 
				REGENERATE=8000	PASS_WALL=getter.flags{PASS_TREES=100} 
				MANA_CURR=250 MANA_MAX=250 MANA_REGENERATE=1000 
				FACTION=FACTION_ANIMAL
				DROP_THEME=getter.flags{THEME_FOOD=30 THEME_MAGIC=40 THEME_TREASURE=30}
			}
			spells =
			{
				frequency=3
				["Summon Animals"] = { level=34 chance=80}
				["Summon Kin"] = { level=70 chance=80}
				["Healing"] = { level=64 chance=85}
				["Grow Trees"] = { level=72 chance=90}
			}
		}
		[19] =
		{
			name = "nightmare"
			level = 80 rarity = 3
			desc = 
			{
				"Ahead of you, a bone-white horse stands and paws the ground. When you",
				"get closer, you realise it's not just bone-white, but bone. It is a",
				"gruesome Nightmare, an undead horse created from the terrified screams",
				"of the sleeping and released into the waking world.",
			}
			color = color.GREEN
			speed = 10 life = {40,30} ac = 60
			aaf = 90 sleep = 0
			exp = 3000
			weight = 6000
			blows =
			{
				{"GAZE","CONFUSE", {5,5}}, 
				{"GAZE","PARALYZE", {5,5}}, 
				{"GAZE","FEAR", {5,5}}, 
				{"BITE","DARK_FIRE", {10,10}}, 
				{"KICK","CRUSH", {10,10}}, 
				{"KICK","CRUSH", {10,10}}, 				
			}
			flags =
			{
				RESIST=getter.resists{ACID=66 COLD=100 CONFUSION=50 CRUSH=-10 DARK_FIRE=true ELEC=66 FEAR=100 FIRE=66 DARK_FIRE=50 PIERCE=50 POIS=100}
				COLD_BLOOD=true REGENERATE=0 EVIL=true UNDEAD=true NONLIVING=true 
				EMPTY_MIND=true PASS_WALL=getter.flags{PASS_INCORP=100} 
				NO_SLEEP=true NO_CUT=true KILLED_VERB="destroyed" SOUND_KILL=sound.N_KILL
				AERIAL=true CAN_FLY=25 SEE_INVIS=30 FACTION=FACTION_UNDEAD SPEEDS=getter.speeds{WALK=15}
				DROP_THEME=getter.flags{THEME_JUNK=60 THEME_TREASURE=40}
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
				MOVE_AI=ai.PATHFIND
				CASTING_AI=ai.SMART_CAST}
			}
			spells =
			{
				frequency=3
				["Shriek"] = { level=70 chance=80}
				["Summon Wraiths"] = { level=70 chance=80}				
			}
		}
	}
	['Q'] =
		{
		defaults =
			{
				body=default_body.quadruped
				flags={
					ALLOW_IN_PLAIN=true ALLOW_IN_FOREST=true ANIMAL=true
					OPEN_DOOR=false BASH_DOOR=true POWERFUL=true
					}
			} -- defaults
		[1] =
		{
			name = "king of araw"
			level = 8 rarity = 2
			desc = 
			{
				"The oxen that live on the lands around the Sea of Rhun are hardier",
				"and wilder than any others in Middle-earth. Legends claim that they", 
				"are descended from the cattle of the Huntsman of the Valar, Orome",
				"himself, and so they are named the Kine of Araw (Araw being the Sindarin",
				"form of Orome's name). It is a huge four-legged beast that won't attack",
				"unless provoked.",
			}
			--- From Theme, thank you Furiosity
			color = color.RED
			speed = -5 life = {18,18} ac = 3
			aaf = 40 sleep = 85
			exp = 80
			weight =  20000
			blows =
			{
				{"KICK","CRUSH",{8,8}}
				{"BUTT","PIERCE",{11,11}}
			}
			flags =
			{
	  		FACTION=FACTION_ANIMAL
	  		STUPID=true AI=ai.ZOMBIE AI_STATES=getter.flags{FEAR_LEVEL=80}
	  		SPEEDS=getter.speeds{WALK=5} FRIENDS=getter.friends(100,12)
			}
		}
		[2] =
		{
			name = "mumak calf"
			level = 16 rarity = 3
			desc = 
			{
				"This young calf is about as small as a Mumak gets, with its head ony",
				"at the height of a man and it's tusks mere stubs on its face. It lows",
				"mournfully at you.",
			}
			color = color.OLD_LACE
			speed = 0 life = {12,12} ac = 3
			aaf = 40 sleep = 20
			exp = 30
			weight = 8000
			blows =
			{
				{"BUTT","CRUSH", {5,5}}	
			}
			flags =
			{
				FACTION=FACTION_ANIMAL
				AI=ai.RANDOM_MOVE AI_STATES=getter.flags{MOVE_RATE=40}
			}
		}
		[3] =
		{
			name = "razorback"
			level = 20 rarity = 3
			desc = 
			{
				"This wild pig has grown to a massive size that dwarfs many horses! It",
				"is the lord of its domain and bows down to no one, man or beast. Beware!",
				"You may find yourself adorning its gigantic tusks!"
			}
			color = color.UMBER
			speed = 10 life = {14,14} ac = 30
			aaf = 80 sleep = 60
			exp = 250
			weight = 6500
			blows =
			{
				{"BUTT","PIERCE", {8,8}}	
				{"BUTT","PIERCE", {8,8}}	
			}
			flags =
			{
				RESIST=getter.resists{CONFUSION=50 CRUSH=10 FEAR=100}		
				FACTION=FACTION_ANIMAL AI=ai.SPELL_AND_MOVE WEIRD_MIND=true STUPID=true
				AI_STATES=getter.flags
				{
					MOVE_AI=ai.ZOMBIE
					CASTING_AI=ai.SMART_CAST
					FEAR_LEVEL=0
				}
				
			}
			spells =
			{
				frequency=10
				["Shriek"] = { level=30 chance=100 }	
			}
		}
		[4] =
		{
			name = "slave mumak"
			level = 25 rarity = 2
			desc = 
			{
				"This mumak has been bred for heavy labour in the southern lands, and",
				"has a rider seated on its neck with a long, flexible whip. The clipped",
				"tusks should not fool you, for it still has the mumakil's thick, grey hide",
				"and monstrous strength!"
			}
			color = color.DARK_ORCHID
			speed = -5 life = {30,30} ac = 20
			aaf = 60 sleep = 40
			exp = 260
			weight = 120000
			blows =
			{
				{"BUTT","CRUSH", {14,14}}				
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=8 PIERCE=4 SLASH=8}
				SPEEDS=getter.speeds{WALK=5} AI=ai.ZOMBIE
				FACTION=FACTION_SAURON FRIENDS=getter.friends(30,3)
			}
		}
		[5] =
		{
			name = "mumak cow"
			level = 30 rarity = 2
			desc = 
			{
				"This large female mumak is relatively docile, and will not anger easily",
				"but you would be unwise to exctie her wrath, as she is still a large and",
				"powerful beast. You will often see them with mumak calves."
			}
			color = color.LIGHT_DARK
			speed = -5 life = {35,35} ac = 30
			aaf = 20 sleep = 80
			exp = 300
			weight = 130000
			blows =
			{
				{"BUTT","PIERCE", {18,16}}								
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=8 PIERCE=4 SLASH=8}
				SPEEDS=getter.speeds{WALK=10} AI=ai.ZOMBIE RIDEABLE=60
				FACTION=FACTION_ANIMAL FRIENDS=getter.friends(60,6)
				ESCORT={0,1} ESCORT_NAME='mumak calf'
			}
		}
		[6] =
		{
			name = "rhinocerous"
			level = 35 rarity = 3
			desc = 
			{
				"This horn-nosed beast is the terror of the plans, with a grumpy attitude",
				"and enough mass and power to give pause to a troll! It has a thick,",
				"armoured hide and stumpy legs that give it a surprising turn of speed!"	
			}
			color = color.OLD_LACE
			speed = 5 life = {13,13} ac = 40
			aaf = 60 sleep = 70
			exp = 350
			weight = 100000
			blows =
			{
				{"BUTT","PIERCE", {12,10}}								
				{"BUTT","CRUSH", {12,10}}								
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=12 FEAR=100 PIERCE=8 SLASH=12}
				SPEEDS=getter.speeds{WALK=10} AI=ai.ZOMBIE NO_STUN=true
				FACTION=FACTION_ANIMAL REFLECTING=10
			}
		}
		[7] =
		{
			name = "bull mumak"
			level = 40 rarity = 2
			desc = 
			{
				"This majestic bull is the master of his harem of females, his tusks",
				"long and sharp. The thickness of his scarred and pitted hide renders",
				"him all but immune to the attacks of punier creatures, and the ground",
				"shakes with his power. He is a King of Mumakil and not to be trifled",
				"with!",
			}
			color = color.DARK_SLATE_GRAY
			speed = 5 life = {40,40} ac = 40
			aaf = 40 sleep = 60
		 	exp = 900
			weight = 180000
			blows =
			{
				{"BUTT","PIERCE", {20,16}}								
				{"CRUSH","CRUSH", {20,20}}												
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=15 PIERCE=10 SLASH=15}
				FACTION=FACTION_ANIMAL REFLECTING=10 SPEED=getter.speeds{WALK=10}
				RIDEABLE=70 ESCORT={6,10} ESCORT_NAME='mumak cow' AI=ai.SPELL_AND_MOVE 
				AI_STATES=getter.flags{
					MOVE_AI=ai.ZOMBIE
					CASTING_AI=ai.SMART_CAST}
			}
			spells =
			{
				frequency=10
				["Howl"] = { level=80 chance=100 }					
			}
		}
		[8] =
		{
			name = "rogue mumak"
			level = 50 rarity = 4
			desc = 
			{
				"This bull mumak is separate from the herd, and for good reason, as it ",
				"is in musk and nearly mad from it. It charges blindly at imaginary",
				"rivals and uproots mighty trees merely because they are in his way!"
			}
			color = color.DARK_RED
			speed = 10 life = {50,40} ac = 20
			aaf = 40 sleep = 0
			exp = 1500
			weight = 180000
			blows =
			{
				{"BUTT","PIERCE", {20,20}}								
				{"CRUSH","CRUSH", {24,20}}												
			}
			flags =
			{
				RESIST=getter.resists{CONFUSION=50 CRUSH=15 FEAR=100 PIERCE=10 SLASH=15}
				FACTION=FACTION_ANIMAL REFLECTING=10 KILL_BODY=true RIDEABLE=85
				WEIRD_MIND=true SPEED=getter.speeds{WALK=10}
				AI=ai.SPELL_AND_MOVE 
				AI_STATES=getter.flags{
					MOVE_AI=ai.RANDOM_MOVE
					CASTING_AI=ai.SMART_CAST
					MOVE_RATE=25}
			}
			spells =
			{
				frequency=8
				["Howl"] = { level=80 chance=100 }					
			}
		}
		[9] =
		{
			name = "war mumak"
			level = 50 rarity = 2
			desc = 
			{
				"Jingling with with the weight of great brass rings and painted for war",
				"by its Easterling riders, this mumak is a terrible foeman. Its tusks",
				"are capped in bronze and its eyes are wild with the cruel prodding",
				"from its riders, it is one of the most fearsome cavalry on Middle Earth!",
			}
			color = color.RED
			speed = 10 life = {50,50} ac = 35
			aaf = 40 sleep = 0
			exp = 1800
			weight = 200000
			blows =
			{
				{"BUTT","PIERCE", {20,20}}								
				{"CRUSH","CRUSH", {24,20}}												
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=20 FEAR=100 PIERCE=25 SLASH=20}
				FACTION=FACTION_SAURON REFLECTING=30 KILL_BODY=true
				FRIENDS=getter.friends(80,5) SPEED=getter.speeds{WALK=5}
				AI=ai.SPELL_AND_MOVE 
				AI_STATES=getter.flags{
					MOVE_AI=ai.ZOMBIE
					CASTING_AI=ai.SMART_CAST
					FEAR_LEVEL=5}
			}
		}
		[10] =
		{
			name = "battle mumak"
			level = 60 rarity = 3
			desc = 
			{
				"These mumakil have a great howdah on their backs filled with half a",
				"dozen easterlng archers. This mumak has range with which to sting its",
				"enemies and its great tusks and heavy armour to smite foes which stray",
				"too close!",
			}
			color = color.FIREBRICK
			speed = 10 life = {50,40} ac = 40
			aaf = 40 sleep = 0
			exp = 2000
			weight = 220000
			blows =
			{
				{"BUTT","PIERCE", {20,20}}								
				{"CRUSH","CRUSH", {24,20}}												
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=18 FEAR=100 PIERCE=20 SLASH=18}
				FACTION=FACTION_SAURON REFLECTING=30 KILL_BODY=true
				FRIENDS=getter.friends(80,5) SPEED=getter.speeds{WALK=10 CAST=20}
				LIMIT_SPELLS = getter.limit_spells{Arrow = 180} AI=ai.SPELL_AND_MOVE 
				AI_STATES=getter.flags{
					MOVE_AI=ai.ZOMBIE
					CASTING_AI=ai.SMART_CAST
					FEAR_LEVEL=5}
			}
			spells =
			{
				frequency=2
				["Arrow"] = { level=50 chance=70 }
			}
		}
		[11] =
		{
			name = "mumak of isengard"
			level = 80 rarity = 5
			desc = 
			{
				"A towering amalgam of flesh and steel, this mumak has been raised in",
				"the depths of Isengard and has been the subject of many cruel",
				"experiments and alterations. Spiked armour have been bolted into its",
				"flesh, and it has been fed many foul potions and unguents. Atop its",
				"back sits a rattling orcish machine, feeding further elixirs into it", 
				"from great bubbling vats. Its eyes show both madness and an inhuman",
				"cunning. It will kill you, if it can!"
			}
			color = color.STEEL_BLUE
			speed = 0 life = {60,50} ac = 20
			aaf = 80 sleep = 10
			exp = 8000
			weight = 300000
			blows =
			{
				{"BUTT","PIERCE", {25,25}}								
				{"CRUSH","CRUSH", {30,20}}															
			}
			flags =
			{
				RESIST=getter.resists{ACID=66 COLD=66 CONFUSION=50 CRUSH=30 DARK_FIRE=50 ELEC=-66 FEAR=100 FIRE=66 POIS=66 PIERCE=35 SLASH=30}
				UNIQUE=true CHAR_MULTI=true FORCE_MAXHP=true NO_SLEEP=true
				REFLECTING=30 SPEEDS=getter.speeds{WALK=10 CAST=-10}
				FACTION=FACTION_ISENGARD REGENERATE=24000 WEIRD_MIND=true NO_STUN=true 
				MANA_MAX=9999 MANA_CURR=9999 MANA_REGENERATE=0
				LIMIT_SPELLS = getter.limit_spells{["Essence of Speed"] = 4, Healing = 8, Invisibility = 2, ["Wings of Winds"] = 3}
				AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
					MOVE_AI=ai.PATHFIND
					CASTING_AI=ai.SMART_CAST
					FEAR_LEVEL=5}
					CAN_SPEAK=function(name) monster_random_say{
					name.. "trumpets deafeningly.",
					"The machine on "..name.."'s back rattles ominously.",
					name.. "paws the earth.",
				}end
			}
			spells =
			{
				frequency=6
				["Healing"] = { level=64 chance=100}
				["Essence of Speed"] = { level=54 chance=100}
				["Invisibility"] = { level=50 chance=100}
				["Wings of Winds"] = { level=21 chance=100}
			}
		}
	}
	-- Trolls
	['T'] =
		{
		defaults =
			{
				body = default_body.humanoid
				flags =
				{
					ALLOW_IN_EVERYWHERE=true FACTION=FACTION_SAURON TROLL=true
					EVIL=true OPEN_DOOR=true BASH_DOOR=true
					DROP_SKELETON=true DROP_CORPSE=true
				}
			} -- defaults
		[1] =
		{
			name = "forest troll"
			level = 20 rarity = 1
			desc =
				{
					"Green-skinned and ugly, this massive humanoid",
					"glares at you , clenching wart-covered green fists.",
				}
			color = color.GREEN
			speed = 5 life = {30,20} ac = 30
			aaf =  20 sleep = 40
			exp = 100 weight = 3000
			blows = {
					{"BARE_MAINHAND","CRUSH",{6,6}}
					{"BARE_OFFHAND","CRUSH",{6,6}}
					{"BITE","PIERCE",{6,5}}
				}
			flags =
			{
				RESIST=getter.resists{DARK_FIRE=50 SLASH=15 PIERCE=10 CRUSH=15 POIS=66}
				
				STUPID=true REGENERATE=2500 AI=ai.ZOMBIE
				DROP_NUMBER = getter.random(1,2)
				DROP_THEME = getter.flags {
					THEME_JUNK  = 60
					THEME_TREASURE  = 20
					THEME_COMBAT = 20
				}
				ALLOW_IN_FOREST=true
			}
		}
		[2] =
		{
			name = "stone troll"
			level = 26 rarity = 1
			desc =
				{
					"A giant troll with scabrous black skin. With a shudder, you",
					" notice the belt of dwarf skulls around his massive waist.",
				}
			color = color.DARK_SLATE_GRAY
			speed = 0 life = {30,25} ac = 25
			aaf =  20 sleep = 50
			exp = 200 weight = 3700
			blows = {
					{"CLAW_MAINHAND","SLASH",{7,7}}
					{"CLAW_OFFHAND","SLASH",{7,7}}
					{"BITE","PIERCE",{6,7}}
				}
			flags =
			{
			----ROCK REMOVER? Does it exist? Or should acid be the GF vulnerability
				RESIST=getter.resists{ACID=-66 DARK_FIRE=50 SLASH=25 PIERCE=20 CRUSH=20 POIS=66}
				NO_CUT=true
				DROP_NUMBER = getter.random(1,2) REGENERATE=1400 REFLECTING=15
				AI=ai.ZOMBIE
				DROP_THEME = getter.flags {
					THEME_JUNK  = 40
					THEME_TREASURE  = 40
					THEME_COMBAT = 20
				}
			}
		}
		[3] =
		{
			name = "cave troll"
			level = 33 rarity = 1
			desc =
				{
					"This huge troll wields a massive spear and has",
					"a disturbingly intelligent look in its piggy eyes.",
				}
			color = color.SLATE
			speed = 0 life = {35,25} ac = 40
			aaf =  60 sleep = 0
			exp = 350 weight = 3800
			blows = {
					{"WEAPON","PIERCE",{9,10}}
					{"WEAPON","PIERCE",{9,10}}
					{"BITE","PIERCE",{6,6}}
				}
			flags =
			{
				RESIST=getter.resists{CONFUSE=100 SLASH=16 PIERCE=16 CRUSH=14 POIS=66}
				SMART=true REGENERATE=4000
				DROP_NUMBER = getter.random(3,2) AI=ai.ZOMBIE
				DROP_THEME = getter.flags {
					THEME_JUNK  = 10
					THEME_TREASURE  = 40
					THEME_COMBAT = 50
				}
				ALLOW_IN_CAVE=true
			}
		}
		[4] =
		{
			name = "mountain troll"
			level = 23 rarity = 1
			desc =
				{
					"A large and athletic troll with an extremely tough and",
					"warty hide.",
				}
			color = color.UMBER
			speed = 10 life = {33,20} ac = 50
			aaf =  60 sleep = 0
			exp = 200 weight = 3200
			blows = {
					{"BARE_MAINHAND","CRUSH",{6,7}}
					{"BARE_OFFHAND","CRUSH",{6,7}}
					{"BITE","PIERCE",{6,6}}
				}
			flags =
			{
				RESIST=getter.resists{SLASH=16 DARK_FIRE=50 PIERCE=16 CRUSH=14 POIS=66}
				STUPID=true REGENERATE=6000
				DROP_NUMBER = getter.random(1,3) AI=ai.ZOMBIE
				DROP_THEME = getter.flags {
					THEME_JUNK  = 30
					THEME_TREASURE  = 30
					THEME_COMBAT = 30
				}
				ALLOW_IN_MOUNTAIN=true
			}
		}
		[5] =
		{
			name = "ice troll"
			level = 28 rarity = 1
			desc =
				{
					"A troll with a white hide and clawed hands. Ice",
					"has formed around its fangs.",
				}
			color = color.WHITE
			speed = 0 life = {34,25} ac = 45
			aaf =  60 sleep = 30
			exp = 300 weight = 3200
			blows = {
					{"CLAW_MAINHAND","SLASH",{8,8}}
					{"CLAW_OFFHAND","SLASH",{8,8}}
					{"BITE","COLD",{7,8}}
				}
			flags =
			{
				RESIST=getter.resists{DARK_FIRE=50 SLASH=18 PIERCE=18 CRUSH=16 POIS=66 COLD=66 FIRE=-66}
				REGENERATE=4000 AI=ai.ZOMBIE
				DROP_NUMBER = getter.random(2,2)
				DROP_THEME = getter.flags {
					THEME_JUNK  = 10
					THEME_TREASURE  = 70
					THEME_COMBAT = 20
				}
				ALLOW_IN_COLD=true
			}
		}
		[6] =
		{
			name = "water troll"
			level = 35 rarity = 1
			desc =
				{
					"Covered in slime, and reeking of the brackish water",
					"in which it lives, this aquatic troll's webbed claws",
					"are perfectly adapted to its underwater life.",
				}
			color = color.LIGHT_BLUE
			speed = 10 life = {40,20} ac = 40
			aaf =  60 sleep = 40
			exp = 400 weight = 3200
			blows = {
					{"CLAW_MAINHAND","SLASH",{9,10}}
					{"CLAW_OFFHAND","SLASH",{9,10}}
					{"BITE","POISON",{9,9}}
					{"BITE","POISON",{9,9}}
				}
			flags =
			{
				RESIST=getter.resists{SLASH=23 PIERCE=20 CRUSH=23 POIS=66 FIRE=-66 WATER=66}
				STUPID=true REGENERATE=4000
				AI=ai.ZOMBIE DROP_NUMBER = getter.random(1,3)
				DROP_THEME = getter.flags {
					THEME_JUNK  = 60
					THEME_TREASURE  = 40
				}
				ALLOW_IN_SWAMP=true ALLOW_IN_RIVER=true ALLOW_IN_OCEAN=true
			}
		}
		[7] =
		{
			name = "war troll"
			level = 44 rarity = 1
			desc =
				{
					"Carrying a massive scimitar, and with metal studs",
					"and rings driven into its very flesh, this fearsome",
					"troll unusually hunts in packs.",
				}
			color = color.DARK_ORCHID
			speed = 10 life = {50,25} ac = 55
			aaf =  80 sleep = 20
			exp = 700 weight = 4000
			blows = {
					{"WEAPON","SLASH",{12,12}}
					{"WEAPON","SLASH",{12,12}}
					{"BITE","PIERCE",{9,9}}
					{"BITE","POISON",{9,9}}
				}
			flags =
			{
				RESIST=getter.resists{FEAR=100 SLASH=30 PIERCE=25 CRUSH=25 POIS=66}
				SMART=true FRIENDS=getter.friends(100,16)
				DROP_NUMBER = getter.random(3,2) REGENERATE=5000 REFLECTING=15
				AI=ai.ZOMBIE AI_STATES=getter.flags{LURE_FROM_CORRIDOR=true}
				DROP_THEME = getter.flags {
					THEME_JUNK  = 10
					THEME_TREASURE  = 20
					THEME_COMBAT = 70
				}
			}
		}
		[8] =
		{
			name = "bert, the stone troll"
			level = 34 rarity = 7
			desc =
				{
					"Big, brawny, powerful and with a taste for hobbit.",
					"He has friends called Bill and Tom.",
				}
			color = color.WHITE
			speed = 10 life = {13,100} ac = 60
			aaf =  20 sleep = 0
			exp = 2000 weight = 50000
			blows = {
					{"CLAW_MAINHAND","CRUSH",{12,12}}
					{"CLAW_OFFHAND","CRUSH",{12,12}}
					{"BITE","PIERCE",{11,11}}
				}
			flags =
			{
				RESIST=getter.resists{ACID=-66 COLD=100 SLASH=35 PIERCE=35 CRUSH=35 POIS=100}
				SPECIAL_GENE=true FORCE_MAXHP=true
				UNIQUE=true MOVE_BODY=true NO_CUT=true STUPID=true REGENERATE=8000 REFLECTING=40
				AI=ai.ZOMBIE 
				CAN_SPEAK=function(name) monster_random_say{
					name.. "says 'Mutton yesterday, mutton today, and blimey, if it don't look like mutton again tomorrer.'",
					name.. "yells 'Shut yer mouth!''",
					name.. "says 'What's a burrahobbit got to do with my pocket, anyways?'",
					name.. "says 'Manflesh. What I wouldn't give for a nice side o' manflesh now and then.'",
					name.. "grumbles 'No good roasting 'em now, it'll take all night!'",
					name.. "snarls 'I won't take that from you!'",
					name.. "says 'Ar... I can't stand the sight o' you, unskinned and unboiled!'",
					name.. "says 'E's only a little feller! Let 'im go!'",
				}end
				DROP_NUMBER = getter.random(3,2)
				DROP_THEME = getter.flags {THEME_JUNK  = 40 THEME_TREASURE  = 40 THEME_COMBAT = 20
				}
			}
		}
		[9] =
		{
			name = "bill, the stone troll"
			level = 34 rarity = 7
			desc =
				{
					"Big, brawny, powerful and with a taste for hobbit.",
					"He has friends called Tom and Bert.",
				}
			color = color.WHITE
			speed = 10 life = {13,100} ac = 60
			aaf =  20 sleep = 0
			exp = 2000 weight = 50000
			blows = {
					{"CLAW_MAINHAND","CRUSH",{12,12}}
					{"CLAW_OFFHAND","CRUSH",{12,12}}
					{"BITE","PIERCE",{11,11}}
				}
			flags =
			{
				RESIST=getter.resists{ACID=-66 COLD=100 SLASH=35 PIERCE=35 CRUSH=35 POIS=100}
				SPECIAL_GENE=true FORCE_MAXHP=true
				UNIQUE=true MOVE_BODY=true NO_CUT=true STUPID=true REGENERATE=8000 REFLECTING=40
				AI=ai.ZOMBIE
				CAN_SPEAK=function(name) monster_random_say{
					name.. "says 'Mutton yesterday, mutton today, and blimey, if it don't look like mutton again tomorrer.'",
					name.. "yells 'Shut yer mouth!''",
					name.. "says 'What's a burrahobbit got to do with my pocket, anyways?'",
					name.. "says 'Manflesh. What I wouldn't give for a nice side o' manflesh now and then.'",
					name.. "grumbles 'No good roasting 'em now, it'll take all night!'",
					name.. "snarls 'I won't take that from you!'",
					name.. "says 'Ar... I can't stand the sight o' you, unskinned and unboiled!'",
					name.. "says 'E's only a little feller! Let 'im go!'",
				}end
				DROP_NUMBER = getter.random(3,2)
				DROP_THEME = getter.flags {THEME_JUNK  = 40 THEME_TREASURE  = 40 THEME_COMBAT = 20
				}
			}
		}
		[10] =
		{
			name = "tom, the stone troll"
			level = 34 rarity = 7
			desc =
				{
					"Big, brawny, powerful and with a taste for hobbit.",
					"He has friends called Bill and Bert.",
				}
			color = color.WHITE
			speed = 10 life = {13,100} ac = 60
			aaf =  20 sleep = 0
			exp = 2000 weight = 50000
			blows = {
					{"CLAW_MAINHAND","CRUSH",{12,12}}
					{"CLAW_OFFHAND","CRUSH",{12,12}}
					{"BITE","PIERCE",{11,11}}
				}
			flags =
			{
				RESIST=getter.resists{ACID=-66 COLD=100 SLASH=35 PIERCE=35 CRUSH=35 POIS=100}
				SPECIAL_GENE=true FORCE_MAXHP=true
				UNIQUE=true MOVE_BODY=true NO_CUT=true STUPID=true REGENERATE=8000 REFLECTING=40
				AI=ai.ZOMBIE
				CAN_SPEAK=function(name) monster_random_say{
					name.. "says 'Mutton yesterday, mutton today, and blimey, if it don't look like mutton again tomorrer.'",
					name.. "yells 'Shut yer mouth!''",
					name.. "says 'What's a burrahobbit got to do with my pocket, anyways?'",
					name.. "says 'Manflesh. What I wouldn't give for a nice side o' manflesh now and then.'",
					name.. "grumbles 'No good roasting 'em now, it'll take all night!'",
					name.. "snarls 'I won't take that from you!'",
					name.. "says 'Ar... I can't stand the sight o' you, unskinned and unboiled!'",
					name.. "says 'E's only a little feller! Let 'im go!'",
				}end
				DROP_NUMBER = getter.random(3,2)
				DROP_THEME = getter.flags {THEME_JUNK  = 40 THEME_TREASURE  = 40 THEME_COMBAT = 20
				}
			}
		}
		[11] =
		{
			name = "ulik the troll"
			level = 51 rarity = 4
			desc =
				{
					"Ulik is the strongest troll ever to live. He could",
					"challenge the Valar and pound them with his great",
					"strength and his skin is hard as a mountain.",
				}
			color = color.VIOLET
			speed = 25 life = {30,100} ac = 70
			aaf =  30 sleep = 0
			exp = 18000 weight = 80000
			blows = {
					{"BARE_MAINHAND","CRUSH",{26,27}}
					{"CLAW_MAINHAND","SLASH",{22,22}}
					{"CLAW_OFFHAND","SLASH",{22,22}}
					{"BITE","POISON",{15,16}}
				}
			flags =
			{
				RESIST=getter.resists{ACID=66 COLD=100 CONFUSION=100 ELEC=66 FEAR=100 FIRE=66 SLASH=35 PIERCE=35 CRUSH=35 POIS=100 }
				SPECIAL_GENE=true FORCE_MAXHP=true REGENERATE=9000
				KILL_BODY=true SMART=true REFLECTING=60 AI=ai.ZOMBIE
				DROP_NUMBER = getter.random(3,4)
				CAN_SPEAK=function(name) monster_random_say{
					name.." roars defiance at you!",
					name.."beats its breast and screams!",
					"Tulkas fear me, think you are braver?",
					"Your manflesh is weak, I will feast on it!",
					"I am my Lord's hammer... you are beneath him!",
				}end
				DROP_THEME = getter.flags {THEME_TREASURE  = 40 THEME_COMBAT = 60
				}
			}
		}
	}
--- worms!
	['w'] =
	{
		defaults =
		{
			body = default_body.worms
			flags =
			{
				ALLOW_IN_EVERYWHERE=true FACTION=FACTION_ANIMAL
				ANIMAL=true SEE_INVIS=15 COLD_BLOOD=true
				DROP_CORPSE=true
			}
		}
		[1] =
		{
			name = "white worm mass"
			level = 1 rarity = 1
			desc = "It is a large slimy mass of worms."
			color = color.WHITE
			speed = -10 life = {4,4} ac = 1
			aaf = 7 sleep = 10
			exp = 2
			weight = 30
			blows =
			{
				{"CRAWL","POISON",{3,2}},
			}
			flags =
			{
				RESIST=getter.resists{DARK=22 FEAR=100 LITE=-100 POIS=100}
				CAN_SWIM=true STUPID=true WEIRD_MIND=90
				MORTAL=true NO_CUT=true MULTIPLY=true
				AI=ai.RANDOM_MOVE AI_STATES=getter.flags{MOVE_RATE=100}
				ALLOW_IN_SWAMP=true
				SYMBIOTE=true SYMB_BONUS_SCALED = getter.flags{RESIST=getter.resists{POIS=100}}
				SYMB_BONUS=getter.flags{REGENERATE=1500}
				
			}
		}
		[2] =
		{
			name = "green worm mass"
			level = 2 rarity = 1
			desc =
			{
				"It is a large slimy mass of worms, with acidic steam rising from",
				"their bodies.",
			}
			color = color.GREEN
			speed = -10 life = {6,4} ac = 3
			aaf = 7 sleep = 10
			exp = 3
			weight = 40
			blows =
			{
				{"CRAWL","ACID",{3,3}},
			}
			flags =
			{
				RESIST=getter.resists{ACID=100 DARK=22 FEAR=100 LITE=-100 POIS=100}
				STUPID=true WEIRD_MIND=90
				CAN_SWIM=true MORTAL=true NO_CUT=true MULTIPLY=true
				AI=ai.RANDOM_MOVE AI_STATES=getter.flags{MOVE_RATE=100}
				ALLOW_IN_SWAMP=true
				SYMBIOTE=true SYMB_BONUS_SCALED = getter.flags{RESIST=getter.resists{ACID=100}}
				SYMB_BONUS=getter.flags{REGENERATE=1500}
			}
		}
		[3] =
		{
			name = "clear worm mass"
			level = 3 rarity = 2
			desc = "It is a disgusting mass of poisonous worms."
			color = color.LIGHT_BLUE
			speed = -10 life = {4,4} ac = 1
			aaf = 7 sleep = 10
			exp = 4
			weight = 200
			blows =
			{
				{"CRAWL","POISON",{3,2}},
			}
			flags =
			{
				RESIST=getter.resists{DARK=22 FEAR=100 LITE=-100 POIS=100}
				ATTR_CLEAR=true CAN_SWIM=true STUPID=true
				WEIRD_MIND=90 INVISIBLE=true MORTAL=true NO_CUT=true
				MULTIPLY=true
				AI=ai.RANDOM_MOVE AI_STATES=getter.flags{MOVE_RATE=100}
				ALLOW_IN_SWAMP=true
				SYMBIOTE=true SYMB_BONUS_SCALED = getter.flags{RESIST=getter.resists{POIS=100}}
				SYMB_BONUS=getter.flags{REGENERATE=1500 INVIS=15}
			}
		}
		[4] =
		{
			name = "yellow worm mass"
			level = 3 rarity = 2
			desc = "It is a large slimy mass of worms, covered in numbing yellow goo!"
			color = color.YELLOW
			speed = -10 life = {4,8} ac = 4
			aaf = 7 sleep = 10
			exp = 4
			weight = 200
			blows =
			{
				{"CRAWL","LOSE_DEX",{3,3}},
			}
			flags =
			{
				RESIST=getter.resists{DARK=22 FEAR=100 LITE=-100}
				CAN_SWIM=true STUPID=true WEIRD_MIND=90
				HURT_LITE=true NO_FEAR=true MORTAL=true NO_CUT=true
				MULTIPLY=true
				AI=ai.RANDOM_MOVE AI_STATES=getter.flags{MOVE_RATE=100}
				ALLOW_IN_SWAMP=true
				SYMBIOTE=true
				SYMB_BONUS=getter.flags{REGENERATE=1500 SUST_STATS = getter.stats{[A_STR]=true}}
			}
		}
		[5] =
		{
			name = "blue worm mass"
			level = 4 rarity = 1
			desc = "It is a large slimy mass of worms with frost rimming their bodies."
			color = color.BLUE
			speed = -10 life = {5,8} ac = 8
			aaf = 7 sleep = 10
			exp = 5
			weight = 40
			blows =
			{
				{"CRAWL","COLD",{6,6}},
			}
			flags =
			{
				RESIST=getter.resists{DARK=22 COLD=100 FEAR=100 FIRE=-66 LITE=-100}
				STUPID=true WEIRD_MIND=90 COLD_BLOOD=true
				CAN_SWIM=true MULTIPLY=true MORTAL=true NO_CUT=true
				AI=ai.RANDOM_MOVE AI_STATES=getter.flags{MOVE_RATE=100}
				ALLOW_IN_COLD=true
				SYMBIOTE=true SYMB_BONUS_SCALED = getter.flags{RESIST=getter.resists{COLD=100}}
				SYMB_BONUS=getter.flags{REGENERATE=1500 RESIST=getter.resists{FIRE=-66}}
			}
		}
		[6] =
		{
			name = "giant leech"
			level = 5 rarity = 1
			desc = "Yech! The disgusting thing only wants your blood!"
			color = color.UMBER
			speed = -10 life = {6,8} ac = 18
			aaf = 10 sleep = 50
			exp = 20
			weight = 30
			blows =
			{
				{"BITE","PIERCE",{6,3}},
				{"BITE","PIERCE",{6,3}},
			}
			flags =
			{
				AQUATIC=true WEIRD_MIND=90 ALLOW_IN_COLD=true
				AI=ai.RANDOM_MOVE AI_STATES=getter.flags{MOVE_RATE=50}
			}
		}
		[7] =
		{
			name = "red worm mass"
			level = 5 rarity = 1
			desc = "It is a large slimy mass of worms, glowing with heat!"
			color = color.RED
			speed = -10 life = {5,8} ac = 12
			aaf = 7 sleep = 10
			exp = 6
			weight = 40
			blows =
			{
				{"CRAWL","FIRE",{3,6}},
			}
			flags =
			{
				RESIST=getter.resists{COLD=-66 DARK=22 FEAR=100 FIRE=100 LITE=-100}
				STUPID=true EMPTY_MIND=true BASH_DOOR=true
				CAN_SWIM=true MULTIPLY=true MORTAL=true NO_CUT=true HAS_LITE=true
				AI=ai.RANDOM_MOVE AI_STATES=getter.flags{MOVE_RATE=100}
				ALLOW_IN_LAVA=true
				SYMBIOTE=true SYMB_BONUS_SCALED = getter.flags{RESIST=getter.resists{FIRE=100}}
				SYMB_BONUS=getter.flags{REGENERATE=1500 RESIST=getter.resists{COLD=-66}}
			}
		}
		[8] =
		{
			name = "giant slug"
			level = 6 rarity = 1
			desc =
			{
				"It is slowly making its way towards you, eating everything in",
				"its path...",
			}
			color = color.LIGHT_UMBER
			speed = -10 life = {12,10} ac = 15
			aaf = 10 sleep = 25
			exp = 25
			weight = 600
			blows =
			{
				{"BITE","ACID",{6,6}},
				{"BITE","ACID",{6,6}},
			}
			flags =
			{
				EMPTY_MIND=true KILL_ITEM=true KILL_BODY=true CAN_SWIM=true
				MORTAL=true AI=ai.SPELL_AND_MOVE
				AI_STATES=getter.flags
				{
					MOVE_AI=ai.GHOUL
					CASTING_AI=ai.SMART_CAST
				}
				ALLOW_IN_SWAMP=true
			}
			spells =
			{
				frequency = 10
				["Breathe Acid"] = { level=100 chance=97 }
			}
		}
		[9] =
		{
			name = "carrion worm mass"
			level = 12 rarity = 4
			desc =
			{
				"It is a disgusting mass of dark worms, eating each other, the floor,",
				"the air, you...",
			}
			color = color.UMBER
			speed = -10 life = {5,8} ac = 15
			aaf = 10 sleep = 3
			exp = 6
			weight = 200
			blows =
			{
				{"TOUCH","EXP_10",{0,0}},
			}
			flags =
			{
				RESIST=getter.resists{DARK=22 FEAR=100 LITE=-100}
				CAN_SWIM=true STUPID=true WEIRD_MIND=90
				BASH_DOOR=true MULTIPLY=true NO_CUT=true KILL_BODY=true
				AI=ai.RANDOM_MOVE AI_STATES=getter.flags{MOVE_RATE=100}
			}
		}
		[10] =
		{
			name = "sandworm"
			level = 27 rarity = 6
			desc = "Offbreed of the Sandworm Queen, they are harmless alone..."
			color = color.YELLOW
			speed = 5 life = {10,15} ac = 40
			aaf = 10 sleep = 80
			exp = 12
			weight = 4500
			blows =
			{
				{"CLAW","POISON",{7,7}},
				{"CLAW","POISON",{7,7}},
				{"CLAW","POISON",{7,7}},
				{"BITE","PIERCE",{8,9}},
			}
			flags =
			{
				RESIST=getter.resists{CHARM_SYMBIOTE=33 ELEC=100 FIRE=100 DARK_FIRE=50 POIS=100}
				FORCE_MAXHP=true POWERFUL=true EVIL=true
				EMPTY_MIND=true MORTAL=true MULTIPLY=true
				AI=ai.RANDOM_MOVE AI_STATES=getter.flags{MOVE_RATE=25}
				ALLOW_IN_DESERT=true
				SYMBIOTE=true SYMB_BONUS_SCALED = getter.flags{RESIST=getter.resists{DARK_FIRE=50 ELEC=100 POIS=100}}
				SYMB_BONUS=getter.flags{REGENERATE=1500 EVIL=true}
			}
		}
		[11] =
		{
			name = "giant glow worm"
			level = 29 rarity = 4
			desc =
			{
				"Hidden on the ceiling, this creature attracts its prey with",
				"its mesmerising lights and enfolds them in its sticky webs!",
			}
			color = color.LIGHT_BLUE
			speed = -10 life = {31,3} ac = 10
			aaf = 14 sleep = 30
			exp = 400
			weight = 1000
			blows =
			{
				{"ENGULF","PARALYZE",{0,0}},
				{"GAZE","CONFUSE",{0,0}},
				{"BITE","PIERCE",{6,8}},
			}
			flags =
			{
				HAS_LITE=true INVISIBLE=true FRIENDS=getter.friends(100,7)
				NEVER_MOVE=true AERIAL=true CAN_FLY=1 AI=ai.NEVER_MOVE
				ALLOW_IN_CAVE=true
				SYMBIOTE=true
				SYMB_BONUS=getter.flags{REGENERATE=1500 INVIS=15 AERIAL=true CAN_FLY=1 SPEEDS=getter.speeds{WALK=-10}}
			}
		}
	}
}
