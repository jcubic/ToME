-- Old Forest: Monster races definitions
new_monster_races
{
-------------------------- # Trees (Old Forest)--------------------------
	['#'] =
	{
		defaults =
		{
			body = default_body.blob
			flags =
			{
				ALLOW_IN_OLD_FOREST=true
				WEIRD_MIND=90
			}
		}
		[1] =
		{
			define_as = "MONSTER_OLD_MAN_WILLOW"
			name = "old man willow"
			level = 25 rarity = 5
			desc =
			{
				"The ancient grey willow tree, ruler of the Old Forest. He despises",
				'trespassers in his territory.  "...a huge willow-tree, old and hoary"',
				"Enormous it looked, its sprawling branches going up like racing arms",
				"with may long-fingered hands, its knotted and twisted trunk gaping in",
				"wide fissures that creaked faintly as the boughs moved.",
			}
			color = color.WHITE
			speed = 0 life = {34,20} ac = 30
			aaf = 10 sleep = 10
			exp = 150
			weight = 10000
			blows =
			{
				{"TOUCH","PARALYZE",{6,9}},
				{"TOUCH","PARALYZE",{6,9}},
				{"CRUSH","CRUSH",{9,12}},
			}
			flags =
			{
				RESIST=getter.resists{ACID=100 COLD=100 FIRE=-66 CRUSH=15 SLASH=10 PIERCE=15}
				EMPTY_MIND=true COLD_BLOOD=true SPECIAL_GENE=true FORCE_MAXHP=true UNIQUE=true
				DROP_NUMBER=getter.random(1,2) AI = ai.SPELL_AND_MOVE
				DROP_THEME = getter.flags{
					THEME_TREASURE=50
					THEME_MAGIC=50
				}
				MANA_CURR = 60 MANA_MAX = 60 MANA_REGENERATE=900
				AI = ai.SPELL_AND_MOVE
				AI_STATES = getter.flags
				{
					CASTING_AI=ai.SMART_CAST
					MOVE_AI=ai.NEVER_MOVE
				}
			}
			spells =
			{
				frequency=7
				["Hold"] = { level=50 chance=90 }
				["Teleport to"] = { level=100 chance=90 }
			}
		}
		[2] =
		{
			name = "tree golem"
			level = 18 rarity = 3
			desc =
			{
				"A dead tree ahead of you begins to wave its sharp branches. Is it an Ent?",
				"The glowing symbols carved on it's trunk reveal otherwise, it is the",
				"creation of a sorceror who doesn't take kindly to strangers in the",
				"forest!"
			}
			color = color.TAN
			speed = 5 life = {20,20} ac = 30
			aaf = 10 sleep = 0
			exp = 60
			weight = 8000
			blows =
			{
				{"CLAW","SLASH",{6,9}},
				{"CLAW","SLASH",{6,9}},
			}
			flags =
			{
				RESIST=getter.resists{ACID=100 COLD=100 CONFUSION=100 ELEC=100 FEAR=100 FIRE=-66 CRUSH=15 SLASH=10 PIERCE=15}
				EMPTY_MIND=true COLD_BLOOD=true AI = ai.ZOMBIE REGENERATE=0
				AI_STATES=getter.flags{FEAR_LEVEL=0}
				DROP_NUMBER=getter.random(1,2)
				DROP_THEME = getter.flags {
					THEME_JUNK=80
					THEME_MAGIC=20
				}
				PASS_WALL=getter.flags{PASS_TREES=1}
			}
		}
		[3] =
		{
			name = "forest fire"
			level = 13 rarity = 3
			desc =
			{
				"You see a glow ahead of you, illuminating a tree. It's not illuminating,",
				"the tree is on FIRE!"
			}
			color = color.RED
			speed = -10 life = {10,2} ac = 5
			aaf = 0 sleep = 0
			exp = 60
			weight = 8000
			blows =
			{
				{"BURN","FIRE",{6,4}}
			}
			flags =
			{
				RESIST=getter.resists{ACID=100 COLD=-66 CONFUSION=100 ELEC=100 DARK=100 FEAR=100 FIRE=100 LITE=100 CRUSH=90 SLASH=90 PIERCE=90}
				CHAR_MULTI=true EMPTY_MIND=true MULTIPLY=true NONLIVING=true
				AI = ai.RANDOM_MOVE AI_STATES=getter.flags{MOVE_RATE=100}
				KILL_WALL=getter.flags{DEAD_TREE_FEAT=true}
				SPEEDS=getter.array{[SPEED_DIG]=100}
			}
		}
	}
-------------------------- Ants (Old Forest)--------------------------
	['a'] =
	{
		defaults =
		{
			body = default_body.insect
			flags =
			{
				ALLOW_IN_OLD_FOREST=true COLD_BLOOD=true
				ANIMAL=true SEE_INVIS=15
				WEIRD_MIND=90
				FACTION=FACTION_ANIMAL
			}
		}
		[1] =
		{
			name = "tree ant"
			level = 10 rarity = 1
			desc =
			{
				"Large ants with powerful, sickle-like mandibles. They are",
				"carrying tree branches in their claws, back to the nest.",
			}
			color = color.GREEN
			speed = 0 life = {6,5} ac = 15
			aaf = 10 sleep = 10
			exp = 40
			weight = 300
			blows =
			{
				{"BITE","SLASH",{6,7}},
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=8 SLASH=12 PIERCE=8}
				BASH_DOOR=true DROP_SKELETON=true
				MORTAL=true FRIENDS=getter.friends(90,24)
				AI = ai.GHOUL
				KILL_WALL=getter.flags{DEAD_TREE_FEAT=true}
				SPEEDS=getter.array{[SPEED_DIG]=100}
			}
		}
	}
-------------------------- Beetles (Old Forest)--------------------------
	['K'] =
	{
		defaults =
		{
			body = default_body.insect
			flags =
			{
				ALLOW_IN_OLD_FOREST=true WEIRD_MIND=90 COLD_BLOOD=true
				ANIMAL=true SEE_INVIS=15 FACTION=FACTION_ANIMAL
			}
		}
		[1] =
		{
			name = "killer brown beetle"
			level = 13 rarity = 1
			desc = "It is a vicious insect with a tough carapace."
			color = color.UMBER
			speed = 0 life = {13,8} ac = 20
			aaf = 10 sleep = 30
			exp = 38
			weight = 500
			blows =
			{
				{"BITE","PIERCE",{7,7}},
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=7 SLASH=10 PIERCE=7}
				BASH_DOOR=true AERIAL=true CAN_FLY=2 DROP_CORPSE=true
				MORTAL=true AI = ai.SNAKE
			}
		}
		[2] =
		{
			name = "killer stag beetle"
			level = 22 rarity = 1
			desc = "It is a giant beetle with vicious claws and massive horns."
			color = color.GREEN
			speed = 0 life = {18,9} ac = 30
			aaf = 12 sleep = 30
			exp = 80
			weight = 500
			blows =
			{
				{"CLAW","SLASH",{3,12}},
				{"BUTT","PIERCE",{6,8}},
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=10 SLASH=12 PIERCE=12}
				DROP_CORPSE=true BASH_DOOR=true
				AERIAL=true CAN_FLY=2 MORTAL=true AI = ai.SNAKE
			}
		}
		[3] =
		{
			name = "killer white beetle"
			level = 23 rarity = 1
			desc = "It is looking for prey."
			color = color.WHITE
			speed = 0 life = {18,8} ac = 30
			aaf = 14 sleep = 30
			exp = 85
			weight = 500
			blows =
			{
				{"BITE","PIERCE",{7,6}},
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=10 SLASH=12 PIERCE=12}
				BASH_DOOR=true DROP_CORPSE=true
				AERIAL=true CAN_FLY=2 MORTAL=true AI=ai.SNAKE
			}
		}
		[4] =
		{
			name = "killer red beetle"
			level = 25 rarity = 1
			desc = "A giant beetle with poisonous mandibles."
			color = color.RED
			speed = 0 life = {21,9} ac = 30
			aaf = 14 sleep = 30
			exp = 90
			weight = 600
			blows =
			{
				{"BITE","LOSE_STR",{7,7}},
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=10 SLASH=12 PIERCE=12}
				BASH_DOOR=true DROP_CORPSE=true
				AERIAL=true CAN_FLY=3 MORTAL=true AI = ai.SNAKE
			}
		}
		[5] =
		{
			name = "killer fire beetle"
			level = 27 rarity = 1
			desc = "It is a giant beetle wreathed in flames."
			color = color.LIGHT_RED
			speed = 0 life = {23,9} ac = 30
			aaf = 14 sleep = 30
			exp = 95
			weight = 600
			blows =
			{
				{"BITE","PIERCE",{7,5}},
				{"SPIT","FIRE",{8,6}},
			}
			flags =
			{
				RESIST=getter.resists{COLD=-66 FIRE=100 CRUSH=10 SLASH=12 PIERCE=12}
				BASH_DOOR=true DROP_CORPSE=true AI = ai.SNAKE
				AERIAL=true CAN_FLY=3 MORTAL=true HAS_LITE=true
			}
			default_flags_to_ignore =
			{
				COLD_BLOOD=true
			}
		}
		[6] =
		{
			name = "killer slicer beetle"
			level = 30 rarity = 2
			desc =
			{
				"It is a beetle with deadly sharp cutting mandibles and a rock-hard",
				"carapace.",
			}
			color = color.YELLOW
			speed = 0 life = {28,10} ac = 35
			aaf = 14 sleep = 30
			exp = 200
			weight = 600
			blows =
			{
				{"BITE","SLASH",{13,13}},
				{"BITE","SLASH",{13,13}},
			}
			flags =
			{
				RESIST=getter.resists{COLD=-66 FIRE=100 CRUSH=15 SLASH=18 PIERCE=18}
				BASH_DOOR=true DROP_CORPSE=true	AERIAL=true CAN_FLY=3 MORTAL=true AI = ai.SNAKE
			}
		}
		[7] =
		{
			name = "death watch beetle"
			level = 31 rarity = 3
			desc = "It is a giant beetle that produces a chilling sound."
			color = color.LIGHT_DARK
			speed = 0 life = {25,12} ac = 30
			aaf = 16 sleep = 30
			exp = 220
			weight = 800
			blows =
			{
				{"BITE","PIERCE",{10,9}},
				{"WAIL","TERRIFY",{10,9}},
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=10 SLASH=12 PIERCE=12}
				BASH_DOOR=true DROP_CORPSE=true AERIAL=true CAN_FLY=3 MORTAL=true AI = ai.ZOMBIE
			}
		}
		[8] =
		{
			name = "killer iridescent beetle"
			level = 37 rarity = 2
			desc = "It is a giant beetle, whose carapace shimmers with vibrant energies."
			color = color.VIOLET
			speed = 0 life = {30,12} ac = 20
			aaf = 16 sleep = 30
			exp = 850
			weight = 600
			blows =
			{
				{"CLAW","ELEC",{7,7}},
				{"CLAW","ELEC",{7,7}},
				{"GAZE","PARALYZE",{0,0}},
			}
			flags =
			{
				RESIST=getter.resists{CRUSH=10 SLASH=12 PIERCE=12 ELEC=100}
				ATTR_MULTI=true FORCE_MAXHP=true BASH_DOOR=true
				DROP_CORPSE=true AERIAL=true CAN_FLY=3 MORTAL=true HAS_LITE=true
				BLOW_RESPONSE=getter.array{[AURA_ELEC]={6,6}} AI = ai.SNAKE
			}
		}
	}
	['m'] =
-------------------------- m molds (Old Forest)--------------------------
	{
		defaults =
		{
			flags =
			{
				ALLOW_IN_OLD_FOREST=true DROP_CORPSE=true
				FACTION=FACTION_ANIMAL
			}
		}
		[1] =
		{
			name = "symbiant adept"
			level = 13 rarity = 1
			desc =
			{
				"You stare at the slight figure approaching you, who seems",
				"covered in a light sheen of moss. It hefts its mace",
				"thoughtfully."
			}
			color = color.LIGHT_GREEN
			speed = 0 life = {9,9} ac = 20
			aaf = 30  sleep = 50
			exp = 60 weight = 2000 body = default_body.humanoid
			blows =
			{
				{"WEAPON","CRUSH",{10,10}},
				{"SPORE","POISON",{7,7}},
			}
			flags =
			{
				RESIST=getter.resists{FIRE=-66 LITE=-100 CRUSH=5 POIS=66}
				OPEN_DOOR=true TAKE_ITEM=true OPEN_DOOR=true BASH_DOOR=true MORTAL=true
				AI = ai.SPELL_AND_MOVE
				MANA_CURR=30 MANA_MAX=30 MANA_REGENERATE=300
				AI_STATES = getter.flags
				{
					MOVE_AI= ai.ZOMBIE
					CASTING_AI = ai.SMART_CAST
				}
			}
			spells =
			{
				frequency=8
				["Healing"] = {level = 5 chance = 70}
			}
		}
		[2] =
		{
			name = "symbiant"
			level = 17 rarity = 1
			desc =
			{
				"This man-shaped figure strides towards you, but you'd be",
				"hard pressed to identify it as human, with its entire body",
				"body covered in a heavy layer of moss. The sword in its hand",
				"still looks sharp (if rusty)!"
			}
			color = color.SLATE
			speed = 5 life = {15,12} ac = 25
			aaf = 80  sleep = 10
			exp = 150 weight = 2200 body = default_body.humanoid
			blows =
			{
				{"WEAPON","SLASH",{13,10}},
				{"WEAPON","SLASH",{13,10}},
				{"SPORE","POISON",{6,8}},
				{"SPORE","FEAR",{6,8}},
			}
			flags =
			{
				RESIST=getter.resists{FIRE=-66 LITE=-100 CRUSH=5 SLASH=5 POIS=66}
				OPEN_DOOR=true TAKE_ITEM=true OPEN_DOOR=true BASH_DOOR=true MORTAL=true
				SMART=true REGENERATE=1500 AI = ai.SPELL_AND_MOVE
				MANA_CURR=60 MANA_MAX=60 MANA_REGENERATE=600
				AI_STATES = getter.flags
				{
					MOVE_AI= ai.ZOMBIE
					CASTING_AI = ai.SMART_CAST
					FEAR_LEVEL=20
				}
			}
			spells =
			{
				frequency = 4
				["Healing"] = {level = 10 chance = 85}
				["Phase Door"] = {level = 10 chance = 80}
				["Summon Kin"] = {level = 20 chance = 70}
			}
		}
		[3] =
		{
			name = "ancient symbiant"
			level = 21 rarity = 1
			desc = {
				"This creature looks fat on first glance, then you realise",
				"that it is merely covered in layer upon layer of mold and",
				"vegetation, although the eyes that peer out from the mass",
				"at you crackle with power and intelligence."
			}
			color = color.SLATE
			speed = 10 life = {20,20} ac = 38
			aaf = 100  sleep = 0
			exp = 150 weight = 3000 body = default_body.humanoid
			blows =
			{
				{"WEAPON","SLASH",{14,10}},
				{"WEAPON","SLASH",{14,10}},
				{"SPORE","POISON",{11,11}},
				{"SPORE","CONFUSE",{11,11}},
			}
			flags =
			{
				RESIST=getter.resists{FIRE=-66 CONFUSION=100 FEAR=100 CRUSH=8 SLASH=12 PIERCE=5 POIS=95}
				OPEN_DOOR=true TAKE_ITEM=true OPEN_DOOR=true BASH_DOOR=true MORTAL=true
				ESCORT=true ESCORTED_BY_GROUPS=true ESCORT_CHARS="#mj" MANA_CURR=120
				MANA_MAX=120 MANA_REGENERATE=2000 SMART=true REGENERATE=6000
				AI = ai.SPELL_AND_MOVE AI_STATES = getter.flags
				{
					MOVE_AI= ai.ZOMBIE
					CASTING_AI = ai.SMART_CAST
					FEAR_LEVEL=25
				}
			}
			spells =
			{
				frequency=2
				["Healing"] = {level = 15 chance = 90}
				["Phase Door"] = {level = 23 chance = 90}
				["Teleportation"] = {level = 5 chance = 80}
				["Shriek"] = {level = 50 chance = 90}
				["Summon Kin"] = {level = 30 chance = 80}
				["Summon Monsters"] = {level = 25 chance = 70}
			}
		}
	}
-------------------------- Orcs (Old Forest) --------------------------
	['o'] =
		{
		defaults =
			{
				body = default_body.humanoid
				flags =
				{
					ALLOW_IN_OLD_FOREST=true MOVE_BODY=true
					OPEN_DOOR=true ORC=true EVIL=true
					SEE_INVIS=-15
					NON_INNATE_ARMOUR=true
				}
			} -- defaults
		[1] =
		{
			name = "orc sniper"
			level = 8 rarity = 3
			desc =
				{
					"Slow moving and almost invisible, this orc lurks",
					"in the trees and launches arrows from his powerful",
					"bow."
				}
			color = color.GREEN
			speed = -10 life = {4,10} ac = 14
			aaf =  60 sleep = 0
			exp = 60 weight = 1800
			blows = 
			{
					{"WEAPON","SLASH",{4,4}}
				}
			flags =
			{
				RESIST=getter.resists{PIERCE=5 SLASH=5}
				DROP_CORPSE=true TAKE_ITEM=true INVISIBLE=true
				FACTION=FACTION_SAURON AI = ai.SPELL_AND_MOVE
				LIMIT_SPELLS = getter.spells{["Arrow"] = 45}
				SPEEDS = getter.speeds{ WALK=-10 CAST=10 }
				AI_STATES = getter.flags
				{
					BEST_RANGE=4
					MIN_RANGE = 2
					MOVE_AI = ai.ZOMBIE
					CASTING_AI = ai.SMART_CAST
				}
				DROP_NUMBER = getter.random(1,4)
				DROP_THEME = getter.flags {
					THEME_JUNK  = 20
					THEME_TREASURE  = 20
					THEME_COMBAT = 60
				}
				PASS_WALL=getter.flags{PASS_TREES=1}
			}
			spells =
			{
				frequency = 2
				["Arrow"] = {level=32 chance=100}
			}
		}
	}
-------------------------- Humans (Old Forest) --------------------------
	['p'] =
	{
	    	defaults =
	    	{
				body = default_body.humanoid
				flags =
				{
					ALLOW_IN_OLD_FOREST=true OPEN_DOOR=true MOVE_BODY=true
					DROP_CORPSE=true DROP_SKELETON=true
					NON_INNATE_ARMOUR=true
				}
			}
		[1] =
		{
			name = "crazed woodsman"
			level = 12 rarity = 1
			desc =
			{
				"A human with an axe clad in goatskins mutters distractedly",
				"to himself. He wandered into the Old Forest long ago and",
				"appears to have been driven quite, quite mad."
			}
			color = color.WHITE
			speed = 5 life = {8,8} ac = 12
			aaf = 10  sleep = 80
			exp = 20 weight = 2000
			blows =
			{
				{"WEAPON","SLASH",{9,8}},
				{"WEAPON","SLASH",{9,8}},
			}
			flags =
			{
				RESIST=getter.resists{CONFUSION=50 FEAR=100 CRUSH=5}
				RAND_MOVEMENT=25 NO_STUN=true MORTAL=true
				DROP_NUMBER = getter.random(2,1) AI= ai.SPELL_AND_MOVE
				AI_STATES=getter.flags
				{
					MOVE_AI=ai.RANDOM_MOVE
					CASTING_AI=ai.SMART_CAST
					MOVE_RATE=25
					FEAR_LEVEL=0
				}
				DROP_THEME = getter.flags
				{
					THEME_TREASURE  = 20
					THEME_JUNK = 60
					THEME_COMBAT = 20
				}
			}
			spells =
			{
				frequency=10
				["Shriek"] = { level=50 chance=85}
			}
		}
	}

-------------------------- Spiders (Old Forest)--------------------------
	['S'] =
	{
		defaults =
		{
			body = default_body.spider
			flags =
			{
				ALLOW_IN_OLD_FOREST=true COLD_BLOOD=true
				PASS_WALL=getter.flags{
					PASS_WEB=1
					PASS_CLIMB=1
				}
				ANIMAL=true SEE_INVIS=15
				FACTION=FACTION_ANIMAL
			}
		}
		[1] =
		{
			name = "tree spider"
			level = 15 rarity = 1
			desc =
				{
					"You pass a tree, then turn to stare hard at it. What you thought",
					"were waving branches appear to be waving legs, eight of them!"
				}
			color = color.GREEN
			speed = 15 life = {10,8} ac = 30
			aaf = 80  sleep = 10
			exp = 30 weight = 800
			blows =
			{
				{"BITE","POISON",{7,7}},
				{"BITE","POISON",{7,7}},
			}
			flags =
			{
				RESIST=getter.resists{POIS=100 CRUSH=5 PIERCE=10 SLASH=10}
				DROP_SKELETON=true COLD_BLOOD=true AI = ai.ZOMBIE
				PASS_WALL=getter.flags
				{
					PASS_WEB=1
					PASS_CLIMB=1
					PASS_TREES=1
				}
			}
			spells =
			{
				frequency=3
				["Web"] = {level=40 chance=80}
			}
		}
		[2] =
		{
			name = "huntsman"
			level = 17 rarity = 1
			desc =
				{
					"This huge spider rushes at you, baring its huge hairy fangs!",
				}
			color = color.UMBER
			speed = 20 life = {10,8} ac = 40
			aaf = 80  sleep = 10
			exp = 38 weight = 800
			blows =
			{
				{"BITE","POISON",{9,8}},
				{"BITE","POISON",{9,8}},
			}
			flags =
			{
				RESIST=getter.resists{POIS=100 CRUSH=5 PIERCE=10 SLASH=10}
				DROP_SKELETON=true COLD_BLOOD=true AI = ai.ZOMBIE
			}
		}
	}
}
