-- Barrow-Downs: Monster races definitions
new_monster_races
{
-------------------------- Canines (Barrow Downs)--------------------------
	['C'] = {
		defaults =
		{
			flags =
			{
				ALLOW_IN_BARROW_DOWNS=true
				ANIMAL=true SEE_INVIS=15
			}
			body = default_body.quadruped
		}
		[1] =
		{
			name = "bloodfang, the wolf"
			level = 9 rarity = 1
			desc = "It has been terrorising the nearby villages."
			color = color.LIGHT_RED
			speed = 10 life = {8,6} ac = 30
			aaf = 30 sleep = 50
			exp = 30
			weight = 1600
			blows =
			{
				{"BITE","PIERCE",{3,12}},
				{"BITE","PIERCE",{3,12}},
			}
			flags =
			{
				BASH_DOOR=true DROP_CORPSE=true
				UNIQUE=true FORCE_MAXHP=true MORTAL=true
				FACTION=FACTION_ANIMAL
				AI = ai.ZOMBIE
				AI_STATES = getter.flags{FEAR_LEVEL=10}
			}
		}
		[2] =
		{
			name = "lone wolf"
			level = 9 rarity = 1
			desc = "A lone wolf, abandoned by its pack."
			color = color.WHITE
			speed = 5 life = {5,6} ac = 30
			aaf = 30 sleep = 70
			exp = 30
			weight = 600
			blows =
			{
				{"BITE","PIERCE",{3,12}},
			}
			flags =
			{
			BASH_DOOR=true DROP_CORPSE=true
			MORTAL=true SPEEDS=getter.speeds{WALK=5}
			FACTION=FACTION_ANIMAL AI = ai.SPELL_AND_MOVE
			AI_STATES = getter.flags
				{
					MOVE_AI = ai.GHOUL
					CASTING_AI = ai.SMART_CAST
				}
			}
			spells =
			{
				frequency=10
				["Howl"] = { level=50 chance=85}
			}
		}
		[3] =
		{
			name = "fox"
			level = 6 rarity = 1
			desc =
			{
				"A dog-like carnivore of wood and farmland distinctive for",
				"it's reddish coat and plaintive cries.",
			}
			color = color.ORANGE
			speed = 5 life = {6,4} ac = 30
			aaf = 40 sleep = 70
			exp = 25
			weight = 600
			blows =
			{
				{"BITE","PIERCE",{6,5}}
			}
			flags =
			{
				DROP_CORPSE=true MORTAL=true SMART=true SPEEDS=getter.speeds{WALK=5}
				FACTION=FACTION_ANIMAL AI=ai.SPELL_AND_MOVE
				AI_STATES = getter.flags
					{
						MOVE_AI = ai.GHOUL
						CASTING_AI = ai.SMART_CAST
					}
			}
			spells =
			{
				frequency=10
				["Shriek"] = { level=50 chance=85}
			}
		}
	}
-------------------------- Humanoids (Barrowdowns) --------------------------
	['h'] =
	{
		defaults =
		{
				body = default_body.humanoid
				flags =
				{
					ALLOW_IN_BARROW_DOWNS=true
					OPEN_DOOR=true MOVE_BODY=true
					NON_INNATE_ARMOUR=true
				}
		} -- defaults
		[1] =
		{
			name = "hobbit delinquent"
			level = 2 rarity = 1
			desc =
			{
				"Young whippersnapper! This brat doesn't look a day over 30, and",
				"has an eye for your wallet, your dinner and your possessions!",
			}
			color = color.WHITE
			speed = 0 life = {5,3} ac = 12
			aaf = 30 sleep = 70
			exp = 20 weight = 700
			blows =
			{
				{"TOUCH","EAT_GOLD",{0,0}},
				{"TOUCH","EAT_ITEM",{0,0}},
			}
			flags =
			{
			SMART=true MORTAL=true DROP_CORPSE=true DROP_SKELETON=true
			TAKE_ITEM=true AI = ai.SNAKE
			AI_STATES=getter.flags
				{
					FEAR_LEVEL=30
				}
			}
		}
	}
-------------------------- Humans (Barrowdowns) --------------------------
	['p'] =
		{
		defaults =
			{
			body = default_body.humanoid
			flags =
			{
				ALLOW_IN_BARROW_DOWNS=true DROP_CORPSE=true DROP_SKELETON=true
				OPEN_DOOR=true MOVE_BODY=true
				NON_INNATE_ARMOUR=true
			}
			} -- defaults
		[1] =
		{
		define_as = "MONSTER_BILL_FERNY"
		---From Unangband, via Theme
		name = "bill ferny"
		level = 4 rarity = 2
		desc =
		{
			"A swarthy man of Bree. He has black brows and dark scornful",
			"eyes; his large mouth curled in a sneer. He sells anything to",
			"anybody and enjoys making mischief for his own amusement.",
		}
		color = color.BLUE
		speed = 0 life = {6,10}
		aaf = 60 sleep = 0 ac = 20
		exp = 90 weight = 200
		blows = 
		{
			{"WEAPON","CRUSH",{9,3}},
			{"WEAPON","CRUSH",{9,3}},
		}
		flags = 
		{
			CAN_SPEAK=true FORCE_MAXHP=true EVIL=true UNIQUE=true HAS_LITE=true
			FACTION=FACTION_SAURON AI = ai.ZOMBIE
			AI_STATES=getter.flags{FEAR_LEVEL=30}
			DROP_NUMBER = getter.random(4,2)
			DROP_THEME = getter.flags {
					THEME_TREASURE  = 40
					THEME_COMBAT = 60
					}
			}
		}
		[2] =
		{
			name = "southerner"
			level = 4 rarity = 2
			desc =
			{
				"A swarthy man from the south, visiting in Bree. He watches",
				"you suspicously as he fingers his knife.",
			}
			color = color.BLUE
			speed = 0 life = {6,8}
			aaf = 40 sleep = 80 ac = 15
			exp = 90 weight = 200
			blows = {
					{"WEAPON","SLASH",{6,3}},
					{"WEAPON","SLASH",{6,3}},
				}
			flags =
			{
				HAS_LITE=true FACTION=FACTION_SAURON
				AI = ai.ZOMBIE
				DROP_NUMBER = getter.random(4,2)
				DROP_THEME = getter.flags {
					THEME_TREASURE  = 40
					THEME_COMBAT = 60
					}
			}
		}
	}
-------------------------- Orcs (Barrowdowns) --------------------------
	['o'] =
		{
		defaults =
			{
				body = default_body.humanoid
				flags = {
					ALLOW_IN_BARROW_DOWNS=true
					OPEN_DOOR=true MOVE_BODY=true SEE_INVIS=-15
					NON_INNATE_ARMOUR=true
					}
			} -- defaults
		[1] =
		{
			name = "orc scout"
			level = 7 rarity = 3
			desc =
				{
					"This orc appears to be a scout for the main party,",
					"armed with a bow and a large hunting horn."
				}
			color = color.GREEN
			speed = 5 life = {4,10} ac = 14
			aaf =  60 sleep = 0
			exp = 60 weight = 2200
			blows = {
					{"WEAPON","SLASH",{6,6}}
				}
			flags =
			{
				RESIST=getter.resists{DARK_FIRE=50 PIERCE=5 SLASH=5}
				DROP_CORPSE=true AI_ANNOY=true TAKE_ITEM=true ORC=true EVIL=true
				FACTION=FACTION_SAURON AI = ai.SPELL_AND_MOVE
				LIMIT_SPELLS = getter.spells{["Arrow"] = 25}
				AI_STATES=getter.flags
					{
						MIN_RANGE=1
						BEST_RANGE=3
						MOVE_AI=ai.ZOMBIE
						CASTING_AI= ai.SMART_CAST
					}
				DROP_NUMBER = getter.random(1,4)
				DROP_THEME = getter.flags {
					THEME_JUNK  = 20
					THEME_TREASURE  = 20
					THEME_COMBAT = 60
				}
			}
			spells =
			{
				frequency = 2
				["Arrow"] = {level=24 chance=100}
				["Shriek"] = {level=50 chance=100}
			}
		}
	}
-------------------------- Quadrupeds (Barrow Downs)--------------------------
	['q'] = {
		defaults =
		{
			flags =
			{
				ALLOW_IN_BARROW_DOWNS=true FACTION=FACTION_ANIMAL
				ANIMAL=true SEE_INVIS=15
			}
			body = default_body.quadruped
		}
		[1] =
		{
		name = "boar of everholt"
		-- From Theme
		level = 5 rarity = 1
		desc =
			{
				"A monstrous beast that inhabited Everholt in the Firien Wood,",
				"around the feet of the Halifirien in the White Mountains. It",
				"is twice the size of a regular boar, and twice as aggressive.",
				"King Folca of Rohan met his end at the tusks of this creature.",
			}
		color = color.RED
		speed = 6 life = {13,12} ac = 12
		aaf =  20 sleep = 80
		exp = 60 weight = 2600
		blows =
		{
			{"BUTT","CRUSH",{8,8}},
			{"BUTT","PIERCE",{7,8}},
			{"BUTT","PIERCE",{7,8}},
		}
		flags =
			{
				RESIST=getter.resists{CONFUSION=50 FEAR=100 CRUSH=5 PIERCE=5 SLASH=5}
				DROP_CORPSE=true WEIRD_MIND=90 UNIQUE=true FORCE_MAXHP=true
				AI = ai.ZOMBIE
			}
		}
		[2] =
		{
			name = "bill the pony"
			level = 2 rarity = 3
			desc =
			{
				"Once owned by Bill Ferny, this scrawny pony looks",
				"wistfully your way. It wants to be friends.",
			}
			color = color.WHITE
			speed = 0 life = {8,4}
			aaf = 40 sleep = 80 ac = 12
			exp = 10 weight = 800
			blows = {
					{"KICK","CRUSH",{8,4}}
				}
			flags =
			{
				DROP_CORPSE=true WEIRD_MIND=90 UNIQUE=true FORCE_MAXHP=true
				FACTION=FACTION_PLAYER AI = ai.ZOMBIE
				AI_STATES=getter.flags
				{
					MIN_RANGE=2
					BEST_RANGE=3
				}
			}
		}
		[3] =
		{
			name = "boar"
			level = 3 rarity = 1
			desc = 	"An agressive wild pig, with huge tusks!",
			color = color.ORANGE
			speed = 7 life = {6,5} ac = 15
			aaf = 30 sleep = 70
			exp = 15 weight = 1600
			blows = {
					{"BUTT","CRUSH",{7,5}},
					{"BUTT","PIERCE",{9,3}},
				}
			flags =
			{
				DROP_CORPSE=true WEIRD_MIND=90
				AI = ai.ZOMBIE ALLOW_IN_PLAIN=true
			}
		}
	}
-------------------------- Skeletons (Barrow Downs)--------------------------
	['s'] =
		{
		defaults = {
			body = default_body.humanoid
			flags = {
				ALLOW_IN_BARROW_DOWNS=true
				OPEN_DOOR=true STUPID=true
				EVIL=true FACTION=FACTION_UNDEAD
				UNDEAD=true NONLIVING=true NO_SLEEP=true NO_CUT=true
				COLD_BLOOD=true SEE_INVIS=15 REGENERATE = 0
				KILLED_VERB="destroyed" SOUND_KILL=sound.N_KILL
				-- Example AI definition, please change me
				AI = ai.GHOUL
				AI_STATES = getter.flags
				{
					MOVE_RATE=10
					FEAR_LEVEL=0
				}
			}
		} -- defaults
		[1] =
		{
			define_as = "MONSTER_BARROWS_WARRIOR"
			name = "barrows warrior"
			level = 3 rarity = 3
			desc =
			{
				"A figure approaches, and you note with a shudder his",
				"empty eye sockets and the mouldy ribs that poke out from",
				"holes in his armour. His rusty sword still looks sharp!",
			}
			color = color.GREEN
			speed = 0 life = {4,5} ac = 15
			aaf = 30 sleep = 90
			exp = 30
			weight = 0
			body = default_body.humanoid
			blows =
			{
				{"WEAPON","SLASH",{9,3}},
			}
			flags =
			{
				RESIST=getter.resists{COLD=100 CONFUSION=50 DARK_FIRE=50 FEAR=100 POIS=100 CRUSH=-10 PIERCE=15}
				DROP_NUMBER = getter.random(2,1)
				DROP_THEME = getter.flags {
					THEME_TOOLS  = 20
					THEME_COMBAT = 80
				}
			}
		}
		[2] =
		{
			define_as = "MONSTER_BARROWS_LEGIONNAIRE"
			name = "barrows legionnaire"
			level = 3 rarity = 4
			desc =
			{
				"A crowd of warriors approach, and you note their empty ",
				"eye sockets and the mouldy ribs poke out from the holes",
				"in their armour. The rusty swords still look sharp!",
			}
			color = color.GREEN
			speed = 0 life = {4,5} ac = 15
			aaf = 30 sleep = 90
			exp = 200
			weight = 0
			body = default_body.humanoid
			blows =
			{
				{"WEAPON","SLASH",{9,3}},
			}
			flags =
			{
				RESIST=getter.resists{COLD=100 CONFUSION=50 DARK_FIRE=50 FEAR=100 POIS=100 CRUSH=-10 PIERCE=15}
				FRIENDS=getter.friends(100,12) STUPID=true
				AI_STATES=getter.flags{LURE_FROM_CORRIDOR=true}
				DROP_NUMBER = getter.random(2,1)
				DROP_THEME = getter.flags {
					THEME_TOOLS  = 20
					THEME_COMBAT = 80
				}
			}
		}
		[3] =
		{
			define_as = "MONSTER_SKELETON_JACKAL"
			name = "skeleton jackal"
			level = 2 rarity = 4
			desc =
			{
				"A clicking sound approaches you and you see a dog-like",
				"form approaching. It has the form of a jackal, but",
				"appears to have been dead for some time.",
			}
			color = color.UMBER
			speed = 0 life = {5,3} ac = 10
			aaf = 30 sleep = 90
			exp = 80
			weight = 0
			body = default_body.quadruped
			blows =
			{
				{"BITE","PIERCE",{6,3}},
			}
			flags =
			{
				RESIST=getter.resists{COLD=100 CONFUSION=50 DARK_FIRE=50 FEAR=100 POIS=100 CRUSH=-10 PIERCE=15}
				STUPID=true FACTION=FACTION_UNDEAD
			}
		}
	}
	-------------------------- Wraiths (Barrow Downs)--------------------------
	['W'] =	{
		defaults = {
			body = default_body.humanoid
			flags = {
				ALLOW_IN_BARROW_DOWNS=true
				OPEN_DOOR=true FACTION=FACTION_UNDEAD
				EVIL=true UNDEAD=true NONLIVING=true NO_SLEEP=true NO_CUT=true
				KILLED_VERB="destroyed" SOUND_KILL=sound.N_KILL
				COLD_BLOOD=true SEE_INVIS=15 REGENERATE = 0
			}
		} -- defaults

		[1] =
		{
			define_as = "MONSTER_BARROWS_KING"
			name = "king of the barrows"
			level = 12 rarity = 3
			desc =
			{
				"A tall, whispering figure with a circlet of gold on his brow. He ",
				"has lived in the Barrow-Downs for centuries after his first death",
				"Now he only wishes to suck the life of all who cross his way.",
			}
			color = color.VIOLET
			speed = 7 life = {10,14} ac = 25
			aaf = 30 sleep = 0
			exp = 200
			weight = 0
			body = default_body.humanoid
			blows =
			{
				{"WEAPON","CRUSH",{9,3}},
				{"WAIL","PARALYZE",{6,6}},
				{"TOUCH","DARK_FIRE",{7,8}},
				{"TOUCH","DARK_FIRE",{7,8}},
			}
			flags =
			{
				RESIST=getter.resists{COLD=100 CONFUSION=50 DARK_FIRE=50 FEAR=100 POIS=100}
				FORCE_MAXHP=true SMART=true SPECIAL_GENE=true UNIQUE=true CAN_SWIM=true
				AERIAL=true CAN_FLY=2 ESCORT=true ESCORT_NAME="barrows"
				DROP_NUMBER = getter.random(2,6)
				AI = ai.SPELL_AND_MOVE
				MANA_CURR = 120 MANA_MAX = 120 MANA_REGENERATE=2000
				AI_STATES = getter.flags
				{
					MOVE_AI = ai.ZOMBIE
					CASTING_AI = ai.SMART_CAST
				}
				DROP_THEME = getter.flags{
					THEME_TOOLS  = 20
					THEME_MAGIC = 35
					THEME_COMBAT = 35
				}
				-- Creates Farlina Snowfoot heirloom
				CREATE_DROP = function(m_idx, m_ptr, r_ptr)
					local ret = default_monster_drop(m_idx, m_ptr, r_ptr)

					local obj = create_artifact(ART_FARLINA_HEIRLOOM)
					if obj then
						make_item_fully_identified(obj)
						obj.found      = OBJ_FOUND_MONSTER
						obj.found_aux1 = m_ptr.r_idx
						obj.found_aux2 = m_ptr.ego
						obj.found_aux3 = current_dungeon_idx
						obj.found_aux4 = level_or_feat(current_dungeon_idx, dun_level)
						monster_carry(m_ptr, m_idx, obj)
					end
					return ret
				end
			}
			spells =
			{
				frequency = 4
				["Bolt of cold"] = { level=16 chance=91 }
				["Cause Wounds"] = { level=24 chance=100 }
				["Weakness"] = { level=28 chance=90 }
				["Scare"] = { level=28 chance=90 }
				["Drain Mana"] = { level=40 chance=90 }
			}
		}
		[2] =
		{
			define_as = "MONSTER_BARROWS_WIGHT"
			name = "barrows wight"
			level = 6 rarity = 3
			desc =
			{
				"A whispering translucent figure, with a skeletal hand extending",
				"from the ends of his robe. He died in battle long ago, and will",
				"suck the life out of all who cross his way.",
			}
			color = color.UMBER
			speed = 7 life = {5,8} ac = 25
			aaf = 30 sleep = 0
			exp = 140
			weight = 0
			body = default_body.humanoid
			blows =
			{
				{"WAIL","PARALYZE",{6,6}},
				{"TOUCH","DARK_FIRE",{6,3}},
			}
			flags =
			{
				RESIST=getter.resists{COLD=100 CONFUSION=50 DARK_FIRE=50 FEAR=100 POIS=100}
				FORCE_MAXHP=true CAN_SWIM=true AERIAL=true CAN_FLY=2
				FACTION=FACTION_UNDEAD FRIENDS=getter.friends(50,6)
				DROP_NUMBER = getter.random(2,2)
				DROP_THEME = getter.flags
				{
					THEME_TOOLS  = 20
					THEME_MAGIC = 35
					THEME_COMBAT = 35
				}
				AI = ai.SPELL_AND_MOVE
				MANA_CURR = 40 MANA_MAX = 40 MANA_REGENERATE=500
				AI_STATES = getter.flags
				{
					MOVE_AI = ai.ZOMBIE
					CASTING_AI = ai.SMART_CAST
				}
			}
			spells =
			{
				frequency = 2
				["Weakness"] = { level=28 chance=90 }
				["Scare"] = { level=28 chance=90 }
				["Drain Mana"] = { level=36 chance=90 }
			}
		}
	}
	-------------------------- Zombies (Barrow Downs)--------------------------
	['z'] =	{
		defaults = {
			body = default_body.humanoid
			flags =
			{
				ALLOW_IN_BARROW_DOWNS=true
				FACTION=FACTION_UNDEAD STUPID=true AI = ai.ZOMBIE
				EVIL=true UNDEAD=true NONLIVING=true NO_SLEEP=true NO_CUT=true
				COLD_BLOOD=true EMPTY_MIND=true SEE_INVIS=15
				KILLED_VERB="destroyed" SOUND_KILL=sound.N_KILL
				AI_STATES=getter.flags{FEAR_LEVEL=0} REGENERATE=0
			}
		} -- defaults
		[1] =
		{
			name = "zombie human"
			level = 4 rarity = 3
			desc =
			{
				"A shambling figure approaches, along with a terrible stench",
				"of decay. With horror, you see that this human has been dead",
				"for some time.",
			}
			color = color.LIGHT_DARK
			speed = -3 life = {12,10} ac = 10
			aaf = 35 sleep = 90
			exp = 30
			weight = 1600
			body = default_body.humanoid
			blows =
			{
				{"CLAW","SLASH",{4,3}},
				{"CLAW_OFFHAND","SLASH",{4,3}},
			}
			flags =
			{
				RESIST=getter.resists{COLD=100 DARK_FIRE=50 POIS=100 SLASH=10 PIERCE=5 FIRE=-66}
				DROP_NUMBER = getter.random(1,2)
				DROP_THEME = getter.flags {
					THEME_COMBAT = 40
					THEME_JUNK = 60
				}

			}
		}
		[2] =
		{
			name = "zombie hobbit"
			level = 3 rarity = 2
			desc =
			{
				"This hobbit appears to have been dead for some time, but his",
				"milky white eyes still seem to focus on you hungrily",
			}
			color = color.WHITE
			speed = 0 life = {9,8} ac = 18
			aaf = 30 sleep = 90
			exp = 22
			weight = 800
			body = default_body.humanoid
			blows =
			{
				{"CLAW","SLASH",{4,2}},
				{"BITE","PIERCE",{4,2}},
			}
			flags =
			{
				RESIST=getter.resists{COLD=100 DARK_FIRE=50 POIS=100 SLASH=10 PIERCE=5 FIRE=-66}
				DROP_NUMBER = getter.random(1,2)
				DROP_THEME = getter.flags {
					THEME_TREASURE = 40
					THEME_JUNK = 60
				}
			}
		}
	}
}
