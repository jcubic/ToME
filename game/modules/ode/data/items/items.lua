new_item_kinds
{
	[TV_WEAPON] =
	{
		color = color.WHITE
		name = "Weapons"
		desc =
		{
			"Sharp, long, short or blunts, but all deadly.",
		}
		-- Here we define the defaults
		-- The wild_slot flag allows to define in whcih equipment slots the
		-- item will go
		defaults = { flags = { WIELD_SLOT=INVEN_MAINHAND } }

		[SV_DAGGER] = {
			-- & = a/an
			-- ~ = pluralisation(s or not)
			name = "& dagger~"
			display = '|' color = color.LIGHT_WHITE
			desc =
			{
				"It's a bladed weapon.",
				"It's the standard weapon of rogues and thieves.  The blade is",
				"a foot long."
			}
			level = 0 weight = 12 cost = 10
			allocation = { {0,1}, {1,1}, {2,2}, }
			flags = {
				WEAPON    = true
				SHOW_MODS = true
				DAM       = getter.damages{MELEE={1,4}}
			}
		}
		[SV_RAPIER] = {
			name = "& rapier~"
			display = '|' color = color.LIGHT_WHITE
			desc =
			{
				"The rapier's hilt consisted of a pair of oval guards pierced with holes,",
				"recurved quillions, and a knuckle guard.  The guard is very intricate",
				"and very effective as protection."
			}
			level = 2 weight = 40 cost = 42
			allocation = { {2,1} }
			flags = {
				WEAPON = true
				DAM = getter.damages{MELEE = {1,6}}
			}
		}
		[SV_SMALL_SWORD] = {
			name = "& small sword~"
			display = '|' color = color.LIGHT_WHITE
			desc =
			{
				"It's a bladed weapon.",
				"It's the favourite weapon of strong mages and thieves, the blade is",
				"about twenty inches long. It's very easy to handle, although it is a lot less",
				"efficient than the longer and heavier designs."
			}
			level = 3 weight = 75 cost = 48
			allocation = { {3,1} }
			flags = {
				WEAPON = true
				DAM = getter.damages{MELEE={1,8}}
			}
		}
		[SV_CUTLASS] = {
			name = "& cutlass~"
			display = '|' color = color.LIGHT_WHITE
			desc =
			{
				"This is a bladed weapon.",
				"This oriental weapon is a short, thick, curving sword",
				"with a single cutting edge. This simple slashing weapon",
				"is typically carried by buccaneers, pirates, and sailors."
			}
			level = 4 weight = 110 cost = 85
			allocation = { {4,1} }
			flags = {
				WEAPON = true
				DAM = getter.damages{MELEE = {3,9}}
			}
		}
		[SV_BROAD_SWORD] = {
			name = "& broad sword~"
			display = '|' color = color.LIGHT_WHITE
			desc =
			{
				"It's a bladed weapon.",
				"This broader version of the long sword is a standard weapon in the army",
				"of Gondolin."
			}
			level = 5 weight = 150 cost = 255
			allocation = { {5,1}, {15,1} }
			flags = {
				WEAPON = true
				DAM = getter.damages{MELEE = {3,8}}
			}
		}
		[SV_LONG_SWORD] = {
			name = "& long sword~"
			display = '|' color = color.LIGHT_WHITE
			desc =
			{
				"It's a bladed weapon.",
				"A long straight sword, tapering to a pronounced point. Mainly good for",
				"piercing attacks, but it can be used for slashing, too. It is a very",
				"popular design and has become standard issue in many armies."
			}
			level = 6 weight = 130 cost = 300
			allocation = { {6,1} }
			flags = {
				WEAPON = true
				DAM = getter.damages{MELEE={4,12}}
			}
		}
		[SV_TWO_HANDED_SWORD] = {
			name = "& two-handed sword~"
			display = '|' color = color.LIGHT_WHITE
			desc =
			{
				"This is a bladed weapon.",
				"This blade is lot longer, wider and heavier than a long sword. You have to",
				"wield it with two hands. This means that wielding a shield makes fighting",
				"very difficult."
			}
			level = 7 weight = 200 cost = 775
			allocation = { {7,1} }
			flags = {
				WEAPON = true
				DAM = getter.damages{MELEE = {2,24}}
			}
		}
	}
	[TV_ARMOR] =
	{
		color = color.SLATE
		name = "Armor"
		desc =
		{
			"Armors protect your skin. Armors are nice. Love armors.",
		}
		defaults = { flags = { WIELD_SLOT=INVEN_BODY } }

		[SV_FILTHY_RAG] = {
			name = "& filthy rag~"
			display = '(' color = color.LIGHT_DARK
			desc =
			{
				"A piece of discarded cloth, smelly and dirty. Eurgh. You're not going to",
				"wear this, are you?"
			}
			level = 0 weight = 20 cost = 1
			allocation = { {0,1} }
			ac = 1
			to_a = -1
		}
		[SV_ROBE] = {
			name = "& robe~"
			display = '(' color = color.BLUE
			desc =
			{
				"A full-length garment which can be worn on the body. It is not really",
				"armour, but mages often wear them as they are very light and don't hinder",
				"movement much."
			}
			level = 1 weight = 20 cost = 4
			allocation = { {1,1},}
			ac = 1
		}
		[SV_SOFT_LEATHER_ARMOR] = {
			name = "soft leather armour~"
			display = '(' color = color.LIGHT_UMBER
			desc =
			{
				"A leather jerkin, light and unencumbering, but not very protective."
			}
			level = 2 weight = 80 cost = 18
			allocation = { {2,1} }
			ac = 3
		}
		[SV_SOFT_STUDDED_LEATHER] = {
			name = "soft studded leather~"
			display = '(' color = color.LIGHT_UMBER
			desc =
			{
				"A leather jerkin with metal studs in critical places offering slightly better",
				"protection."
			}
			level = 3 weight = 90 cost = 35
			allocation = { {3,1} }
			ac = 4
		}
		[SV_HARD_LEATHER_ARMOR] = {
			name = "hard leather armour~"
			display = '(' color = color.LIGHT_UMBER
			desc =
			{
				"A leather armour covering only the body. It is made of hardened leather to",
				"make it harder to penetrate.  It's also a bit harder to move in, as it is",
				"rather stiff."
			}
			level = 4 weight = 100 cost = 150
			allocation = { {4,1} }
			ac = 5
		}
		[SV_METAL_SCALE_MAIL] = {
			name = "metal scale mail~"
			display = '[' color = color.SLATE
			desc =
			{
				"A suit of overlapping metal scales, sewn onto a leather or cloth jerkin."
			}
			level = 5 weight = 250 cost = 550
			allocation = { {5,1} }
			ac = 8
		}
		[SV_CHAIN_MAIL] = {
			name = "chain mail~"
			display = '[' color = color.SLATE
			desc =
			{
				"A suit of interlinked metal rings, to be worn over a woollen garment."
			}
			level = 6 weight = 220 cost = 750
			allocation = { {6,1} }
			ac = 10
		}
		[SV_DOUBLE_RING_MAIL] = {
			name = "double ring mail~"
			display = '[' color = color.SLATE
			desc =
			{
				"A suit of leather armour with metal rings sewn onto it. In addition, in important parts it is",
				"reinforced with mail."
			}
			level = 7 weight = 230 cost = 700
			allocation = { {7,1} }
			ac = 12
		}
		[SV_AUGMENTED_CHAIN_MAIL] = {
			name = "augmented chain mail~"
			display = '[' color = color.SLATE
			desc =
			{
				"A suit of interlinked metal rings, with additional metal plates or scales",
				"covering vulnerable parts of the wearer."
			}
			level = 8 weight = 270 cost = 900
			allocation = { {8,1} }
			ac = 14
			flags = {
				RESIST=getter.resists{FIRE=5 COLD=5}
				SPEED=-1
			}
		}
		[SV_DOUBLE_CHAIN_MAIL] = {
			name = "double chain mail~"
			display = '[' color = color.SLATE
			desc =
			{
				"A suit of chain mail, with an additional layer of mail in some places."
			}
			level = 8 weight = 250 cost = 850
			allocation = { {8,1} }
			ac = 16
			flags = {
				RESIST=getter.resists{FIRE=10 COLD=10}
				SPEED=-2
			}
		}
	}
	[TV_CORPSE] =
	{
		color = color.LIGHT_UMBER
		name = "Corpse"
		desc =
		{
			"Something has died, leaving this empty husk behind,",
			"and going on to someplace better than this foul world.",
		}
		 -- 001
		[SV_CORPSE_CORPSE] = {
			name = "corpse"
			display = '~' color = color.LIGHT_UMBER
			desc =
			{
				"Whatever happened with this one, it wasn't pretty."
			}
			level = 20 weight = 80 cost = 0
			allocation = { {0,200} }
			flags = {
				DECAY=true
			}
		}
	}

	[TV_POTION] =
	{
		color = color.LIGHT_BLUE
		name = "potions"
		desc =
		{
			"These mystic concoctions have a variety of strange effects",
		}
		-- Quaff and you win !
		[SV_POTION_BLOOD] = {
			name = "& blood~ of life"
			display = '!' color = color.VIOLET
			level = 10 weight = 4 cost = 10000
			allocation = { {10,200} }
			flags = {
				ON_QUAFF=function()
					message(color.VIOLET, "You feel the blood of life running through your veins!")
					message(color.VIOLET, "Your mortal body is transformed by the potion...")
					message(color.VIOLET, "You become immortal!")
					message(color.VIOLET, "Well done, you have won eternal life!")
					winner_state = WINNER_NORMAL
					has_won = WINNER_NORMAL
					return true
				end
				NORM_ART = getter.object(TV_POTION, SV_POTION_RESTORATION)
				FULL_NAME = true
				SPECIAL_GENE = true
			}
		}
		[SV_POTION_INVIS] = {
			name = "& potion~ of invisibility"
			display = '!' color = color.BLUE
			desc =
			{
				"This magical brew will temporarily hide you from sight, and also attunes you to this state so",
				"that your eyes can still perceive your hidden form."
			}
			level = 3 weight = 4 cost = 50
			allocation = { {3,1} }
			flags = {
				ON_QUAFF=function()
					local t = 5 + rng(5)
					timed_effect(timed_effect.INVISIBILITY, t, 50)
					timed_effect(timed_effect.SEE_INVISIBILITY, t, 50)
					return true
				end
			}
		}
		[SV_POTION_SPEED] = {
			name = "& potion~ of speed"
			display = '!' color = color.GREEN
			desc =
			{
				"A magical drink which temporarily allows you to act much faster when imbibed."
			}
			level = 1 weight = 4 cost = 75
			allocation = { {1,1}, {5,1}, }
			flags = {
				ON_QUAFF=function()
					local t = 5 + rng(5)
					timed_effect(timed_effect.SPEED, t, 10)
					return true
				end
			}
		}
		[SV_POTION_CURE_LIGHT] = {
			name = "& potion~ of cure light wounds"
			display = '!' color = color.LIGHT_BLUE
			desc =
			{
				"This healthy drink heals a little damage you have taken."
			}
			level = 1 weight = 4 cost = 15
			allocation = { {1,1}, {2,1}, }
			flags = {
				ON_QUAFF=function()
					hp_player(rng.roll(1, 10))
					return true
				end
			}
		}
		[SV_POTION_CURE_SERIOUS] = {
			name = "& potion~ of cure serious wounds"
			display = '!' color = color.LIGHT_BLUE
			desc =
			{
				"This beneficial potion will cure some wounds and other inhibiting ailments."
			}
			level = 3 weight = 4 cost = 40
			allocation = { {3,1} }
			flags = {
				ON_QUAFF=function()
					hp_player(rng.roll(2, 10))
					return true
				end
			}
		}
		[SV_POTION_CURE_CRITICAL] = {
			name = "& potion~ of cure critical wounds"
			display = '!' color = color.LIGHT_BLUE
			desc =
			{
				"This nice potion will cure a good bit of hurt you have suffered and also allows you to recover",
				"from unhealthy conditions like blood poisoning, confusion or blindness."
			}
			level = 5 weight = 4 cost = 100
			allocation = { {5,1} }
			flags = {
				ON_QUAFF=function()
					hp_player(rng.roll(3, 10))
					return true
				end
			}
		}
		[SV_POTION_HEALING] = {
			name = "& potion~ of healing"
			display = '!' color = color.LIGHT_BLUE
			desc =
			{
				"This blessed potion greatly heals you and also cures many other ailments you might suffer",
				"from."
			}
			level = 7 weight = 4 cost = 300
			allocation = { {7,1}, {8,1}, }
			flags = {
				ON_QUAFF=function()
					hp_player(rng.roll(4, 10))
					return true
				end
			}
		}
		[SV_POTION_RESTORATION] = {
			name = "& potion~ of restoration"
			display = '!' color = color.RED
			desc =
			{
				"This magical potion will bring back your physical power to its full extent, should it be drained."
			}
			level = 1 weight = 4 cost = 300
			allocation = { {1,1},{3,1},{5,1}, }
			flags = {
				ON_QUAFF=function()
					do_res_stat(A_STR, true)
					do_res_stat(A_CON, true)
					return true
				end
			}
		}
		[SV_POTION_INC_STR] = {
			name = "& potion~ of strength"
			display = '!' color = color.RED
			desc =
			{
				"This beneficial potion will magically improve your physical strength."
			}
			level = 6 weight = 4 cost = 8000
			allocation = { {6,6}, }
			flags = {
				ON_QUAFF=function()
					return do_inc_stat(A_STR)
				end
			}
		}
		[SV_POTION_INC_CON] = {
			name = "& potion~ of constitution"
			display = '!' color = color.RED
			desc =
			{
				"This magical concoction greatly improves your health, making you permanently tougher."
			}
			level = 6 weight = 4 cost = 8000
			allocation = { {6,6}, }
			flags = {
				ON_QUAFF=function()
					return do_inc_stat(A_CON)
				end
			}
		}
	}

	---------------------- Lites ----------------------
	[TV_LITE] =
	{
		color = color.YELLOW
		name = "lites"
		desc =
		{
			"These may enable you to see in the depths of the dungeon.",
		}
		symbol = '~'
		defaults =
		{
			flags =
			{
				WIELD_SLOT = INVEN_LITE
			}
		}

		[SV_LANTERN] = {
			name = "& lantern~"
			display = '~' color = color.LIGHT_RED
			level = 1 weight = 50 cost = 2500
			allocation = { {1,1} }
			flags = {
				LITE  = obvious(2)
				EASY_KNOW = true
			}
		}
	}

	[TV_FOOD] =
	{
		color = color.LIGHT_UMBER
		name = "Food"
		desc =
		{
			"Edible items may have mysteries effects. Or maybe",
			"just something to much on when you get hungry",
		}
		 -- 035
		[SV_FOOD_RATION] = {
			name = "& ration~ of food"
			display = ',' color = color.LIGHT_UMBER
			desc =
			{
				"Lightweight and filling. Not an incredible taste experience, but that'd be asking a bit much.",
				"You can 'E'at it."
			}
			level = 0 weight = 10 cost = 3
			allocation = { {0,1}, {5,1}, {10,1} }
			flags = {
			}
		}
	}
	[TV_GOLD] =
	{
		color = color.YELLOW
		name = "Gold"
		desc =
		{
			"All that gliters is not go..mhh..all that gliters is MINE MINE MINE!!",
		}
		 -- 001
		[1] = {
			name = "gold"
			display = '$' color = color.UMBER
			level = 1 weight = 0 cost = 1
			flags = {
				GOLD_VALUE = 10
			}
		}
	}
}
