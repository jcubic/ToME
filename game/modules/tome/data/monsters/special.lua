-- Spell monsters, used for spells, quests, ...
new_monster_races
{
	---------------------- golems -----------------------
	['g'] =
	{
		defaults = {
			body = default_body.humanoid NONLIVING=true
		} -- defaults
		[1] =
		{
			define_as = "MONSTER_FIRE_GOLEM"
			name = "fire golem"
			level = 0 rarity = 3
			desc = "A sentient mass of pure fire."
			color = color.RED
			speed = 5 life = {3,20} ac = 50
			aaf = 50 sleep = 10
			exp = 0
			weight = 100
			blows =
			{
				{"HIT","FIRE",{2,6}},
				{"HIT","FIRE",{2,6}},
				{"HIT","FIRE",{3,6}},
				{"HIT","FIRE",{3,6}},
			}
			flags =
			{
				RESIST=getter.resists{FIRE=100}
				BASH_DOOR=true HAS_LITE=true SPECIAL_GENE=true
				NO_SLEEP=true NONLIVING=true NO_STUN=true MORTAL=true
				NO_CUT=true AI_PLAYER=true
				KILLED_VERB="destroyed" SOUND_KILL=sound.N_KILL
				BLOW_RESPONSE=getter.array{[AURA_FIRE]={2,4}}
			}
			spells =
			{
				frequency = 10
				["Breathe Fire"] = { level=100 chance=100 }
			}
		}
	}
}

new_monster_races
{
	['@'] =
	{
		defaults =
		{
			body = default_body.blob
			flags = {
				SPECIAL_GENE=true
			}
		}
		[1] =
		{
			name = "fire immune monster"
			level = 10 rarity = 4
			desc = "For brand testing."
			color = color.GREEN
			speed = -10 life = {100,100} ac = -100
			aaf = 5 sleep = 5
			exp = 10
			weight = 50
			flags =
			{
				ALLOW_IN_FOREST=true

				NEVER_MOVE=true
				RESIST=getter.resists{FIRE=100}
				FORCE_SLEEP=true STUPID=true EMPTY_MIND=true
			}
		}

		[2] =
		{
			name = "slash immune monster"
			level = 10 rarity = 4
			desc = "For brand testing."
			color = color.GREEN
			speed = -10 life = {100,100} ac = -100
			aaf = 5 sleep = 5
			exp = 10
			weight = 50
			flags =
			{
				ALLOW_IN_FOREST=true

				NEVER_MOVE=true
				RESIST=getter.resists{SLASH=100}
				FORCE_SLEEP=true STUPID=true EMPTY_MIND=true
			}
		}

		[3] =
		{
			define_as = "RACE_TESTDG"
			name = "darkgod's personnal test-o-saur"
			level = 90 rarity = 2
			desc =
			{
				"It is a belligerent test-o-saur with a destructive magical arsenal, armed",
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
				SMART=true
				RES_TELE=true BASH_DOOR=true
				DROP_CORPSE=true EVIL=true FORCE_SLEEP=true FORCE_MAXHP=true
				MORTAL=true
				REGENERATE=10500
				ONLY_ITEM=true DROP_NUMBER=getter.random(5,10) DROP_GOOD=true DROP_GREAT=true

				LIMIT_SPELLS = getter.limit_spells{
					Manaflow = 1
				}

				-- Example AI definition, please change me
				MANA_CURR = 200 MANA_MAX = 200 MANA_REGENERATE=5200
				AI = ai.SMART_CAST
				AI_STATES = getter.flags
				{
					MIN_RANGE=2
					BEST_RANGE=4
					CASTING_AI=ai.SMART_CAST
					MOVE_AI=ai.GHOUL
				}
--				SPEEDS = getter.speeds{ WALK=10 CAST=-30 }
			}
			spells =
			{
				frequency = 1
				["Manathrust"] = { level=1 chance=100 }
--				["Fireflash"] = { level=20 chance=85 }
--				["Strike"] = { level=18 chance=95 }
--				["Healing"] = { level=18 chance=95 }
--				["Phase Door"] = { level=5 chance=100 }
--				["Tidal Wave"] = { level=35 chance=100 }
--				["Manaflow"] = { level=35 chance=90 }
--				["Summon Animal"] = { level=15 chance=100 }
			}
		}

		[4] =
		{
			define_as = "RACE_TESTT"
			name = "darkgod's personnal townspeople"
			level = 50 rarity = 2
			desc =
			{
				"It is a belligerent townspeople with a destructive magical arsenal, armed",
				"with a hammer. It bounds towards you, snorting spells.",
			}
			color = color.SLATE
			speed = 20 life = {250,20} ac = 45
			aaf = 13 sleep = 10
			exp = 4500
			weight = 40000
			blows =
			{
			}
			flags =
			{
				FACTION=FACTION_TOWN
				MOVE_BODY = true
				ONLY_ITEM=true DROP_NUMBER=getter.random(1,2)

				-- Example AI definition, please change me
				AI = ai.TOWNSPEOPLE
			}
		}
		[5] =
		{
			name = "multi-armed test monster"
			level = 50 rarity = 2
			desc =
			{
				"Lots of arms, can attack with lots of weapons."
			}
			body = { INVEN=23 MAINHAND=4 BODY=1 OUTER=1 LITE=1 OFFHAND=4 HANDS=4 TOOL=1 RING=2 HEAD=1 NECK=1 FEET=1 BACKPACK=1 }
			color = color.SLATE
			speed = 20 life = {250,20} ac = 45
			aaf = 13 sleep = 10
			exp = 4500
			weight = 40000
			blows =
			{
				{"WEAPON","SLASH",{3,6}},
				{"WEAPON","SLASH",{3,6}},
				{"WEAPON","SLASH",{3,6}},
				{"WEAPON","SLASH",{3,6}},
				{"WEAPON_OFFHAND","SLASH",{3,6}},
				{"WEAPON_OFFHAND","SLASH",{3,6}},
				{"WEAPON_OFFHAND","SLASH",{3,6}},
				{"WEAPON_OFFHAND","SLASH",{3,6}},
			}
			flags =
			{
				FACTION=FACTION_PLAYER
				AI = ai.NEVER_MOVE
			}
		} -- [5] "multi-armed test monster"
	}
}

new_monster_races
{
	['m'] =
	{
		defaults =
		{
			body = default_body.blob
			flags = {
				SPECIAL_GENE=true
			}
		}
		[1] =
		{
			name = "symbiote test monster"
			level = 1 rarity = 4
			desc = "For symbiosis testing."
			color = color.GREEN
			speed = -10 life = {100,100} ac = 10000
			aaf = 5 sleep = 5
			exp = 10
			weight = 50

            spells = {
                frequency = 6
				["Phase Door"] = { level=20 chance=100 }
                ["Manathrust"] = { level=30 chance=100 }
            }

			flags =	{
				SPECIAL_GENE=true NEVER_MOVE=true FACTION=FACTION_DUNGEON
				STUPID=true EMPTY_MIND=true

				MANA_CURR=200 MANA_MAX=200

				HYPNOTIZE_PRE =
					function(monst)
						local ret, key =
                            get_com("Allow hypnosis? (y/n)")

                        if not ret then
							message("Hypnosis prevented.")
                            return true, false
                        end

                        local key = strlower(strchar(key))

						if key ~= "y" then
							message("Hypnosis prevented.")
							return true, false
						end

						message("Hypnosis allowed.")
					end

				HYPNOTIZE_POST =
					function(monst)
						message("HYPNOTIZE_POST called.")
					end

				WIELD_PRE =
					function(obj)
						local ret, key =
                            get_com("Allow symbiosis (y/n)")

                        if not ret then
							message("Symbiosis prevented.")
                            return true, false
                        end

                        local key = strlower(strchar(key))

						if key ~= "y" then
							message("Symbiosis prevented.")
							return true, false
						end

						message("Symbiosis allowed.")
					end

				WIELD_POST =
					function(obj)
						message("Symbiote's WIELD_POST called.")
					end

				TAKEOFF_PRE =
					function(obj)
						local ret, key =
                            get_com("Allow removal? (y/n)")

                        if not ret then
							message("Removal prevented.")
                            return true, false
                        end

                        local key = strlower(strchar(key))

						if key ~= "y" then
							message("Removal prevented.")
							return true, false
						end

						message("Removal allowed.")
					end

				TAKEOFF_POST =
					function(obj)
						message("Symbiote's TAKEOFF_POST called.")
					end

				SYMB_NO_DEFAULT = true

				RESIST=getter.resists{CRUSH=100 PIERCE=100 SLASH=100}

				SYMB_BONUS = getter.flags {
					RESIST=getter.resists{CRUSH=42 PIERCE=42 SLASH=42}
				}

				SYMB_BONUS_SCALED = getter.flags {
					RESIST=getter.resists{POIS=100}
				}

				SYMB_BONUS_BY_LEVEL = getter.array {
					[2] = getter.flags {
						SPEED=1
					},
					[3] = getter.flags {
						SPEED=1
					},
					[4] = getter.flags {
						SPEED=1
					},
					[5] = getter.flags {
						SPEED=1
					},
					[6] = getter.flags {
						SPEED=1
					},
				} -- SYMB_BONUS_BY_LEVEL

				SYMB_REBEL =
					function(symb, monst)
						message("SYMB_REBEL invoked")
					end

				SYMB_ATTACK =
					function(symb, monst)
						message("SYMB_ATTACK invoked")
					end
			} -- flags
		} -- [1]
	} -- ['m']
}
