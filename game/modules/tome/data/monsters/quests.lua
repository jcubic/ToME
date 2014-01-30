-- Monster races definitions
new_monster_races
{
	['p'] =	{
		defaults = {

			body = default_body.humanoid
			flags = {
				NON_INNATE_ARMOUR=true
			}

		} -- defaults
                ;

		----------------- The Necromancer quest ---------------
		{
			define_as = "RACE_NECROMANCER_DOL_GULDUR"
			name = "the necromancer of dol guldur"
			level = 75 rarity = 3
			desc =
			{
				"The dark master of the terrible fortress of southern Mirkwood.  It is",
				"rumoured that this is in fact none other than Sauron in disguise:",
				"although if this is so, he has yet to reveal his full power - and perhaps",
				"will not do so while his deception lasts, in the hope of keeping it going.",
			}
			color = color.VIOLET
			speed = 20 life = {100,100} ac = 130
			aaf = 60 sleep = 10
			exp = 40000
			weight = 1900
			blows =
			{
				{"WEAPON","DARK_FIRE",{10,10}},
				{"WEAPON","EXP_80",{6,9}},
				{"WEAPON","BLIND",{7,9}},
				{"WEAPON","CONFUSE",{7,9}},
			}
			flags =
			{
				RESIST=getter.resists{COLD=90 CONFUSION=50 ELEC=90 FIRE=90 DARK_FIRE=50}
				UNIQUE=true MALE=true POWERFUL=true RES_TELE=true
				FORCE_SLEEP=true FORCE_MAXHP=true SMART=true DROP_CORPSE=true DROP_SKELETON=true
				ONLY_ITEM=true DROP_GREAT=true DROP_GOOD=true SPECIAL_GENE=true
				INVISIBLE=true OPEN_DOOR=true BASH_DOOR=true EVIL=true NO_SLEEP=true
				ON_DEATH = function(monst)
					message(color.YELLOW, "You see the spirit of the necromancer rise and flee...")
					message(color.YELLOW, "It looks like it was indeed Sauron...")
					message(color.YELLOW, "You should report that to Galadriel as soon as possible.")
					change_quest_status(QUEST_NECROMANCER, QUEST_STATUS_COMPLETED)
				end
			}
			spells =
			{
				frequency = 2
				["Bolt of Darkfire"] = { level=40 chance=99 }
				["Blind"] = { level=100 chance=98 }
				["Fireflash"] = { level=44 chance=99 }
				["Geyser"] = { level=80 chance=99 }
				["Manathrust"] = { level=60 chance=99 }
				["Healing"] = { level=46 chance=99 }
				["Hold"] = { level=100 chance=99 }
				["Darkness"] = { level=100 chance=99 }
				["Drain Mana"] = { level=66 chance=99 }
				["Cause Wounds"] = { level=80 chance=99 }
				["Teleportation"] = { level=30 chance=99 }
				["Create Traps"] = { level=100 chance=99 }
				["Phase Door"] = { level=30 chance=99 }
			}
		},

		------------------------ One Ring & Sauron quest -----------------------
		{
			define_as = "RACE_SAURON"
			name = "Sauron, the sorcerer"
			level = 100 rarity = 1
			desc =
			{
				"Mighty in spells and enchantments,he created the One Ring.",
				"His eyes glow with power and with his gaze he seeks to destroy",
				"your soul. He has many servants, and rarely fights without them.",
			}
			color = color.VIOLET
			speed = 30 life = {300,200} ac = 200
			aaf = 100 sleep = 0
			exp = 50000
			weight = 2300
			blows =
			{
				{"WEAPON","DARK_FIRE",{15,12}},
				{"WEAPON","CRUSH",{20,12}},
				{"WEAPON","DARK_FIRE",{15,12}},
				{"WEAPON","CRUSH",{20,12}},
			}
			flags =
			{
				RESIST=getter.resists{COLD=100 CONFUSION=50 ELEC=100 FEAR=100 FIRE=100 POIS=100}
				UNIQUE=true MALE=true REFLECTING=90 FORCE_SLEEP=true SPECIAL_GENE=true
				FORCE_MAXHP=true ONLY_ITEM=true
				DROP_GOOD=true DROP_GREAT=true SMART=true OPEN_DOOR=true
				BASH_DOOR=true MOVE_BODY=true REGENERATE=true NO_SLEEP=true EVIL=true
				RES_TELE=true HAS_LITE=true
				-- Finish quests/resurects sauron
				ON_DEATH = function(monst)
					-- Resurect ?
					if quest(QUEST_ONE_RING).status < QUEST_STATUS_FINISHED then
						message("Sauron will not be permanently defeated until the One Ring is either destroyed or used...")
						race_info(monst).max_num = 1
					else
						change_quest_status(QUEST_SAURON, QUEST_STATUS_COMPLETED)
						winner_state = WINNER_NORMAL
						has_won = WINNER_NORMAL
						player.redraw[FLAG_PR_TITLE] = true

						-- Congratulations
						if quest(QUEST_ONE_RING).status == QUEST_STATUS_FINISHED then
							message(color.LIGHT_GREEN, "*** CONGRATULATIONS ***");
							message(color.LIGHT_GREEN, "You have banished Sauron's foul spirit from Ea, and as you watch, a cleansing");
							message(color.LIGHT_GREEN, "winds roars through the dungeon, dispersing the darkfire mists around where the");
							message(color.LIGHT_GREEN, "body fell. You feel thanks, and a touch of sorrow, from the Valar");
							message(color.LIGHT_GREEN, "for your deed. You will be forever heralded, your deed forever legendary.");
							message(color.LIGHT_GREEN, "You may retire when you are ready.");
						else
							message(color.VIOLET, "*** CONGRATULATIONS ***");
							message(color.VIOLET, "You have banished Sauron from Arda, and made Ea a safer place.");
							message(color.VIOLET, "As you look down at the dispersing mists around Sauron, a sudden intuition");
							message(color.VIOLET, "grasps you. Fingering the One Ring, you gather the darkfire mists around");
							message(color.VIOLET, "yourself, and inhale deeply their seductive power.");
							message(color.VIOLET, "You will be forever feared, your orders forever obeyed.");
							message(color.VIOLET, "You may retire when you are ready.");
						end
					end
				end
				-- Forbid generation when the necromancer is not dead
				NEW_MONSTER_PRE = function()
					if quest(QUEST_NECROMANCER).status ~= QUEST_STATUS_FINISHED then return true, false end
				end
			}
			spells =
			{
				frequency = 2
				["Ball of Darkfire"] = { level=96 chance=99 }
				["Blind"] = { level=100 chance=98 }
				["Confuse"] = { level=100 chance=97 }
				["Fireflash"] = { level=360 chance=99 }
				["Geyser"] = { level=100 chance=99 }
				["Manathrust"] = { level=80 chance=99 }
				["Scare"] = { level=100 chance=99 }
				["Healing"] = { level=100 chance=99 }
				["Hold"] = { level=100 chance=99 }
				["Darkness"] = { level=100 chance=99 }
				["Drain Mana"] = { level=10 chance=99 }
				["Cause Wounds"] = { level=90 chance=99 }
				["Teleportation"] = { level=40 chance=99 }
				["Trap Filling"] = { level=100 chance=99 }
				["Phase Door"] = { level=50 chance=99 }
			}
		},
	}
}

-------------------- Bree: Thieves quest ----------------
new_monster_races
{
	['p'] = {
		defaults = {

			body = default_body.humanoid
		} -- defaults
		;
		{
			define_as = "RACE_THIEVES_GUILD_ROGUE"
			name = "rogue of the thieves guild"
			level = 2 rarity = 1
			desc = "A rather shifty individual."
			color = color.BLUE
			speed = 0 life = {8,4} ac = 12
			aaf = 20 sleep = 5
			exp = 6
			weight = 1400
			blows =
			{
				{"WEAPON","SLASH",{1,6}},
				{"TOUCH","EAT_GOLD",{0,0}},
			}
			flags =
			{
				MALE=true DROP_60=true DROP_SKELETON=true DROP_CORPSE=true TAKE_ITEM=true
				OPEN_DOOR=true BASH_DOOR=true EVIL=true MORTAL=true AI = ai.GHOUL
				DROP_THEME = getter.flags{ THEME_TREASURE=50 THEME_TOOLS=25 THEME_COMBAT=25 }
			}
		},
		{
			define_as = "RACE_THIEVES_GUILD_GUARD"
			name = "guard of the thieves guild"
			level = 2 rarity = 1
			desc = "He looks inexperienced but tough."
			color = color.UMBER
			speed = 0 life = {10,4} ac = 16
			aaf = 20 sleep = 5
			exp = 6
			weight = 1600
			blows =
			{
				{"WEAPON","SLASH",{1,7}},
				{"WEAPON","CRUSH",{1,6}},
			}
			flags =
			{
				MALE=true WILD_TOO=true DROP_SKELETON=true DROP_CORPSE=true
				OPEN_DOOR=true BASH_DOOR=true MORTAL=true HAS_LITE=true AI = ai.GHOUL
				DROP_THEME = getter.flags{ THEME_TREASURE=25 THEME_TOOLS=25 THEME_COMBAT=50 }
			}
		},
		{
			define_as = "RACE_THIEVES_GUILD_CONJURER"
			name = "conjurer of the thieves guild"
			level = 2 rarity = 1
			desc = "He is leaving behind a trail of dropped spell components."
			color = color.RED
			speed = 0 life = {6,4} ac = 6
			aaf = 20 sleep = 5
			exp = 6
			weight = 1400
			blows =
			{
				{"WEAPON","SLASH",{1,4}},
			}
			flags =
			{
				MALE=true FORCE_SLEEP=true DROP_SKELETON=true DROP_CORPSE=true
				DROP_60=true OPEN_DOOR=true BASH_DOOR=true MORTAL=true HAS_LITE=true
				DROP_THEME = getter.flags{ THEME_TREASURE=25 THEME_TOOLS=25 THEME_MAGIC=50 }
				MANA_CURR = 20 MANA_MAX = 20 MANA_REGENERATE=520
				AI = ai.SPELL_AND_MOVE
				AI_STATES = getter.flags
				{
					MIN_RANGE=2
					BEST_RANGE=4
					CASTING_AI=ai.SMART_CAST
					MOVE_AI=ai.GHOUL NO_SPELL_FLEE=true
				}
			}
			spells =
			{
				frequency = 3
				["Blind"] = { level=20 chance=95 }
				["Confuse"] = { level=20 chance=95 }
				["Manathrust"] = { level=4 chance=90 }
				["Phase Door"] = { level=10 chance=97 }
			}
		},
		{
			define_as = "RACE_THIEVES_GUILD_BANDIT"
			name = "bandit of the thieves guild"
			level = 10 rarity = 2
			desc = "He is after your cash, and your life!"
			color = color.BLUE
			speed = 0 life = {8,8} ac = 24
			aaf = 20 sleep = 10
			exp = 26
			weight = 1500
			blows =
			{
				{"WEAPON","SLASH",{2,5}},
				{"TOUCH","EAT_GOLD",{0,0}},
			}
			flags =
			{
				MALE=true TAKE_ITEM=true
				OPEN_DOOR=true BASH_DOOR=true DROP_SKELETON=true DROP_CORPSE=true EVIL=true
				MORTAL=true HAS_LITE=true DROP_60=true AI = ai.GHOUL
				AI_STATES = getter.flags
				{
					FEAR_LEVEL=30
				}
				DROP_THEME = getter.flags{ THEME_TREASURE=20 THEME_JUNK=20 THEME_COMBAT=60 }
			}
		},
		-- The "boss" of the thieves guild
		{
			define_as = "RACE_WORMTONGUE"
			name = "Wormtongue, agent of Saruman"
			level = 10 rarity = 2
			desc = {
				"He's been spying for Saruman. He is a snivelling wretch with no morals.",
				"It seems he organised the local thieves guild, for whatever purpose he will not say.",
			}
			color = color.LIGHT_BLUE
			speed = 0 life = {28,10} ac = 30
			aaf = 20 sleep = 20
			exp = 150
			weight = 1500
			blows =
			{
				{"WEAPON","SLASH",{1,7}},
				{"WEAPON","POISON",{1,7}},
				{"TOUCH","EAT_GOLD",{0,0}},
				{"INSULT","HURT",{0,0}},
			}
			flags =
			{
				SPECIAL_GENE=true FACTION=FACTION_ISENGARD
				RESIST=getter.resists{DARK_FIRE=50 POIS=100}
				UNIQUE=true MALE=true FORCE_SLEEP=true FORCE_MAXHP=true
				ONLY_ITEM=true DROP_GOOD=true DROP_GREAT=true TAKE_ITEM=true
				OPEN_DOOR=true BASH_DOOR=true DROP_SKELETON=true EVIL=true RES_TELE=true
				MORTAL=true HAS_LITE=true AI = ai.SPELL_AND_MOVE
				MANA_CURR = 60 MANA_MAX = 60 MANA_REGENERATE=1000
				AI_STATES = getter.flags
				{
					CASTING_AI=ai.SMART_CAST
					MOVE_AI=ai.GHOUL NO_SPELL_FLEE=true
					FEAR_LEVEL=30
				}
				DROP_NUMBER=getter.random(1,2)
				DROP_THEME = getter.flags{ THEME_TREASURE=10 THEME_COMBAT=50 THEME_MAGIC=35 THEME_JUNK=5 }
				CAN_SPEAK=function(name) monster_random_say{
					name.." coats his blade with some poison.",
					name.." says 'The white hand will be pleased... Yes very pleased!'",
				}end
				ON_DEATH = function(monst)
					message(color.YELLOW, "As Wormtongue lies dead on the floor you cannot understand what an agent of saruman would be doing here.")
					message(color.YELLOW, "Even more worying is the fact that he was comploting with those ruffians...")
					change_quest_status(QUEST_THIEVES, QUEST_STATUS_COMPLETED)
				end
			}
			spells =
			{
				frequency = 1
				["Create Traps"] = { level=100 chance=100 }
				["Geyser"] = { level=10 chance=95 }
				["Healing"] = { level=2 chance=75 }
				["Manathrust"] = { level=16 chance=95 }
				["Bolt of poison"] = { level=8 chance=99 }
			}
		}
	}
}

-- Fangorn, used in many quests
new_monster_races
{
	['#'] =	{
		defaults = {

			body = default_body.humanoid
		} -- defaults
                ;

		-- Fangorn when he comes to the player's rescue in the fires of Isengard quest
		{
			define_as = "RACE_FANGORN_QUEST_FIRES_OF_ISENGARD"
			name = "Fangorn the treebeard, lord of the ents"
			level = 80 rarity = 3
			desc =
			{
				"The first being to awoke on Arda, apart from the Valar themselves.  He is the"
				"first, oldest, greatest and most respected of all the Ents:  and though he is"
				"slow to anger, he is a terrible foe when roused."
			}
			color = color.YELLOW_GREEN
			speed = 70 life = {70,100} ac = 200
			aaf = 30 sleep = 15
			exp = 15500
			weight = 6000
			blows =
			{
				{"CRUSH","CRUSH",{13,18}},
				{"CRUSH","CRUSH",{13,18}},
				{"CRUSH","CRUSH",{13,18}},
				{"CRUSH","CRUSH",{13,18}},
			}
			flags =
			{
				RESIST=getter.resists{FIRE=-66 COLD=50 CRUSH=80 SLASH=50 PIERCE=70}
				FORCE_MAXHP=true SPECIAL_GENE=true
				FORCE_SLEEP=true UNIQUE=true
				MOVE_BODY=true DROP_CORPSE=true SMART=true
				TAKE_ITEM=true BASH_DOOR=true NO_SLEEP=true GOOD=true
				NO_CUT=true FACTION=FACTION_ENT
				PASS_WALL=getter.flags{PASS_TREES=100}
				KILL_WALL=getter.flags{WALL=true}
				SPEEDS=getter.array{[SPEED_DIG]=100}
				AI = ai.ZOMBIE AI_STATES = getter.flags{ FEAR_LEVEL=0 }
				CHATABLE=function(name) dialogs.FANGORN_QUEST_FIRES_OF_ISENGARD() end
			}
		},
		-- Huorns when they comes to the player's rescue in the fires of Isengard quest
		{
			define_as = "RACE_HUORN_QUEST_FIRES_OF_ISENGARD"
			name = "Huorn of fangorn"
			level = 40 rarity = 3
			desc =
			{
				"A very strong near-sentient tree, which has become hostile to other living things."
			}
			color = color.YELLOW_GREEN
			speed = 30 life = {20,100} ac = 200
			aaf = 30 sleep = 15
			exp = 1550
			weight = 5000
			blows =
			{
				{"CRUSH","CRUSH",{7,11}},
				{"CRUSH","CRUSH",{7,11}},
				{"CRUSH","CRUSH",{7,11}},
				{"CRUSH","CRUSH",{7,11}},
			}
			flags =
			{
				RESIST=getter.resists{FIRE=-66 COLD=50 CRUSH=60 SLASH=40 PIERCE=50}
				FORCE_MAXHP=true SPECIAL_GENE=true
				FORCE_SLEEP=true UNIQUE=true
				MOVE_BODY=true DROP_CORPSE=true SMART=true
				TAKE_ITEM=true BASH_DOOR=true NO_SLEEP=true GOOD=true
				NO_CUT=true FACTION=FACTION_ENT
				PASS_WALL=getter.flags{PASS_TREES=1}
				KILL_WALL=getter.flags{WALL=true}
				SPEEDS=getter.array{[SPEED_DIG]=100}
				AI = ai.ZOMBIE AI_STATES = getter.flags{ FEAR_LEVEL=0 }
			}
		},
	}
}

----------------- The Watcher quest, Khazad-dum West gate (QUEST_WATCHER) ---------------
new_monster_races
{
	['~'] =	{
		defaults = {}
		;
		{
			define_as = "RACE_WATCHER_IN_THE_WATER"
			name = "the watcher in the water"
			level = 30 rarity = 3
			desc = 
			{
			 	"Tentacle demon! Eeek!",
			}
			color = color.YELLOW_GREEN
			speed = 20 life = {20,20} ac = 20
			aaf = 30 sleep = 10
			exp = 750
			body =
			{
				INVEN=35 MAINHAND=4 BODY=0 OUTER=0 LITE=0 OFFHAND=4
				HANDS=0 TOOL=0 RING=8 HEAD=0 NECK=0 FEET=0 BACKPACK=1
			}
			weight = 20000
			blows =
			{
				{"CRUSH","CRUSH",{4,5}},
				{"CRUSH","CRUSH",{4,5}},
				{"CRUSH","CRUSH",{4,5}},
				{"CRUSH","CRUSH",{4,5}},
			}
			flags =
			{
				RESIST=getter.resists
				{
					CRUSH=30 FIRE=20 COLD=20 DARK=100 POIS=50
					CONFUSION=100 FEAR=100 DARK_FIRE=66
				}
				FACTION=FACTION_SAURON
				PASS_WALL=getter.flags{PASS_TREES=100} 
				WEIRD_MIND=true POWERFUL=true FORCE_MAXHP=true
				OPEN_DOOR=true BASH_DOOR=true
				EMPTY_MIND=true COLD_BLOOD=true
				NO_SLEEP=true RES_TELE=true 
				AI=ai.ZOMBIE AI_STATES=getter.flags{FEAR_LEVEL=0}
				DROP_NUMBER=getter.random(2,2) DROP_THEME=getter.flags{THEME_COMBAT=100}
				DROP_CORPSE=true DROP_GOOD=true 
				UNIQUE=true SPECIAL_GENE=true
				ON_DEATH = function(monst)
					-- Player may kill the Watcher before the quest is issued
					if quest(QUEST_WATCHER).status == QUEST_STATUS_TAKEN then
						change_quest_status(QUEST_WATCHER, QUEST_STATUS_COMPLETED)
					else
						quest(QUEST_WATCHER).status = QUEST_STATUS_FINISHED
					end
				end
			}
		}
	}
}

----------------- Durin's Bane, the Mines of Moria (QUEST_DURINS_BANE) ---------------
new_monster_races
{
	['U'] =	{
		defaults = {}
		;

		{
			define_as = "RACE_DURINS_BANE"
			name = "Durin's Bane"
			level = 50 rarity = 3
			desc =
			{
				"Groowwrrr! I am a demon! Describe me or I shall gnaw on your elbows!",
			}
			color = color.VIOLET
			speed = 20 life = {50,50} ac = 100
			aaf = 40 sleep = 40
			exp = 1300
			weight = 7000
			blows =
			{
				{"WEAPON","DARK_FIRE",{11,11}},
				{"CLAW_OFFHAND","SLASH",{10,10}},
			}
			flags =
			{
				RESIST=getter.resists{FIRE=100 POIS=80 DARK_FIRE=66 DARK=100 FEAR=100 CONFUSION=100}
				DROP_THEME=getter.flags{THEME_DEMON=70 THEME_MAGIC=30}
				DROP_NUMBER=getter.random(2,2) DROP_GREAT=true DROP_GOOD=true
				UNIQUE=true SPECIAL_GENE=true POWERFUL=true
				SMART=true NO_SLEEP=true
				FACTION=FACTION_SAURON BLOW_RESPONSE=getter.array{[AURA_DARK_FIRE]={10,10}}
				EVIL=true CORRUPTED=true REFLECTING=30 AERIAL=true CAN_FLY=20 MANA_CURR=500
				MANA_MAX=500 DEMON=true
				MANA_REGENERATE=1000 AI=ai.SPELL_AND_MOVE AI_STATES=getter.flags{
					CASTING_AI=ai.SMART_CAST
					MOVE_AI=ai.ZOMBIE
					FEAR_LEVEL=0}
			}
			spells=
			{
				frequency=3
				["Demon Madness"] = {level=50 chance=85}
				-- ["Summon Demon"] = {level=79 chance=80}
				["Demon Field"] = {level=49 chance=70}
				["Demonic Howl"] = {level=59 chance=70}
				["Darkness"] = { level=100 chance=99 }
				["Manathrust"] = { level=40 chance=99 }
				["Drain Mana"] = { level=10 chance=99 }
				["Blind"] = { level=100 chance=98 }
				["Confuse"] = { level=100 chance=97 }
				["Scare"] = { level=100 chance=99 }
				["Fireflash"] = { level=50 chance=99 }
			}
		}
	}
}