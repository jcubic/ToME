-- Yuck it's .. ugly!
mimic.add_shape
{
	define = 	"ABOMINATION"
	name =		"Abomination"
	desc =		"Abominations are failed experiments of powerful wizards."
	realm =		nil
	level = 	1
	rarity =	101
	duration =	{20, 100}
	calc =		function()
		apply_flags
		{
			AGGRAVATE=true
			SPEED=-15
		}
	end
}

-- For Beornings
mimic.add_shape
{
	define = 	"GREAT_BEAR"
	name =		"Great Bear"
	desc =		"A fierce, terrible bear."
	realm =		nil
	level =		1
	rarity =	101
	duration =	{50, 200}
	limit =		true
	calc_end =	function()
		player.pspeed = player.pspeed - 5 + (player.mimic_level / 5)

		-- Some armor deflection
		player.to_a = player.to_a + 5 + ((player.mimic_level * 2) / 3)
		player.dis_to_a = player.dis_to_a + 5 + ((player.mimic_level * 2) / 3)

		-- And some damage reduction(ADDITIVE)
		player.absorbs[dam.CRUSH] = (player.absorbs[dam.CRUSH] or 0) + 1 + (player.mimic_level / 8)
		player.absorbs[dam.PIERCE] = (player.absorbs[dam.PIERCE] or 0) + 1 + (player.mimic_level / 8)
		player.absorbs[dam.SLASH] = (player.absorbs[dam.SLASH] or 0) + 1 + (player.mimic_level / 8)

		player.modify_stat(A_STR,   1 + player.mimic_level / 11)
		player.modify_stat(A_INT, - 1 - player.mimic_level / 11)
		player.modify_stat(A_WIS, - 1 - player.mimic_level / 11)
		player.modify_stat(A_DEX,  -1)
		player.modify_stat(A_CON,   1 + player.mimic_level / 11)
		player.modify_stat(A_CHR, -10)

		-- Increase health regen(7HP/s at level 50)
		player.inc_intrinsic(FLAG_REGEN_LIFE, player.mimic_level * 140)
		-- Reduce mana regen(-3.5MP/s at level 50)
		player.inc_intrinsic(FLAG_REGEN_MANA, -player.mimic_level * 70)
	end
	barehand_combat =
	{
		{
			desc = 'You claw %s.',
			level = 1,
			chance = 0,
			dice = { 3, 4 },
			type = dam.SLASH_STUN
		},
		{
			desc = 'You swat %s.',
			level = 4,
			chance = 0,
			dice = { 4, 4 },
			type = dam.SLASH_WOUND
		},
		{
			desc = 'You bite %s.',
			level = 9,
			chance = 2,
			dice = { 4, 4 },
			type = dam.SLASH_WOUND
		},
		{
			desc = 'You hug %s.',
			level = 15,
			chance = 5,
			dice = { 6, 4 },
			type = dam.SLASH_SLOW
		},
		{
			desc = 'You swat and rake %s.',
			level = 25,
			chance = 10,
			dice = { 6, 5 },
			type = dam.SLASH_STUN_WOUND
		},
		{
			desc = 'You hug and claw %s.',
			level = 30,
			chance = 15,
			dice = { 6, 6 },
			type = dam.SLASH_SLOW_WOUND
		},
		{
			desc = 'You double swat %s.',
			level = 35,
			chance = 20,
			dice = { 9, 7 },
			type = dam.SLASH_STUN_WOUND
		},
		{
			desc = 'You double swat and rake %s.',
			level = 40,
			chance = 25,
			dice = { 10, 10 },
			type = dam.SLASH_STUN_WOUND
		},
	}
}

-- Special high magic shapes
mimic.add_shape
{
	define = 	"FIRE_FORM"
	name =		"Fire Form"
	desc =		"A sentient fire."
	realm =		nil
	level = 	40
	rarity =	101
	duration =	{20, 100}
	calc_end =	function()
		player.resists[dam.FIRE] = 100 - (((100 - (player.resists[dam.FIRE] or 0)) * (100 - player.mimic_level)) / 100)
		player.absorbs[dam.FIRE] = (player.absorbs[dam.FIRE] or 0) + player.mimic_level - 30
		player.intrinsic_flags[FLAG_PROTECT_FROM][dam.FIRE] = true
	end
}
mimic.add_shape
{
	define = 	"WIND_FORM"
	name =		"Wind Form"
	desc =		"A sentient cloud of swirling air."
	realm =		nil
	level = 	40
	rarity =	101
	duration =	{20, 100}
	calc_end =	function()
		player.resists[dam.ELEC] = 100 - (((100 - (player.resists[dam.ELEC] or 0)) * (100 - player.mimic_level)) / 100)
		player.absorbs[dam.ELEC] = (player.absorbs[dam.ELEC] or 0) + player.mimic_level - 30
		player.intrinsic_flags[FLAG_PROTECT_FROM][dam.ELEC] = true

		player.set_intrinsic(FLAG_MAGIC_BREATH)
		player.inc_intrinsic(FLAG_FLY, 2)
	end
}
mimic.add_shape
{
	define = 	"WATER_FORM"
	name =		"Water Form"
	desc =		"A sentient globe of water."
	realm =		nil
	level = 	40
	rarity =	101
	duration =	{20, 100}
	calc_end =	function()
		player.resists[dam.COLD] = 100 - (((100 - (player.resists[dam.COLD] or 0)) * (100 - player.mimic_level)) / 100)
		player.absorbs[dam.COLD] = (player.absorbs[dam.COLD] or 0) + player.mimic_level - 30
		player.intrinsic_flags[FLAG_PROTECT_FROM][dam.COLD] = true

		player.set_intrinsic(FLAG_WATER_BREATH)
	end
}
mimic.add_shape
{
	define = 	"STONE_FORM"
	name =		"Stone Form"
	desc =		"A sentient piece of stone."
	realm =		nil
	level = 	40
	rarity =	101
	duration =	{20, 100}
	calc_end =	function()
		player.resists[dam.ACID] = 100 - (((100 - (player.resists[dam.ACID] or 0)) * (100 - player.mimic_level)) / 100)
		player.absorbs[dam.ACID] = (player.absorbs[dam.ACID] or 0) + player.mimic_level - 30
		player.intrinsic_flags[FLAG_PROTECT_FROM][dam.ACID] = true + player.mimic_level - 30

		-- And some damage reduction(ADDITIVE)
		player.absorbs[dam.CRUSH] = (player.absorbs[dam.CRUSH] or 0) + 1 + ((player.mimic_level - 20) / 8)
		player.absorbs[dam.PIERCE] = (player.absorbs[dam.PIERCE] or 0) + 1 + ((player.mimic_level - 20) / 8)
		player.absorbs[dam.SLASH] = (player.absorbs[dam.SLASH] or 0) + 1 + ((player.mimic_level - 20) / 8)
	end
	barehand_combat =
	{
		{
			desc = 'You hit %s.',
			level = 1,
			chance = 0,
			dice = { 3, 4 },
			type = dam.SLASH_STUN
		},
		{
			desc = 'You crush %s.',
			level = 30,
			chance = 5,
			dice = { 6, 6 },
			type = dam.CRUSH
		},
		{
			desc = 'You double crush %s.',
			level = 35,
			chance = 10,
			dice = { 9, 7 },
			type = dam.CRUSH
		},
		{
			desc = 'You punch %s.',
			level = 40,
			chance = 25,
			dice = { 10, 7 },
			type = dam.CRUSH_FORCE
		},
	}
}
