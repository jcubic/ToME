/* File: tables.c */

/* Purpose: Angband Tables */

/*
 * Copyright (c) 1989 James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"




/*
 * Global array for looping through the "keypad directions"
 */
s16b ddd[9] =
	{ 2, 8, 6, 4, 3, 1, 9, 7, 5 };

/*
 * Global arrays for converting "keypad direction" into offsets
 */
s16b ddx[10] =
	{ 0, -1, 0, 1, -1, 0, 1, -1, 0, 1 };

s16b ddy[10] =
	{ 0, 1, 1, 1, 0, 0, 0, -1, -1, -1 };

/*
 * Global arrays for optimizing "ddx[ddd[i]]" and "ddy[ddd[i]]"
 */
s16b ddx_ddd[9] =
	{ 0, 0, 1, -1, 1, -1, 1, -1, 0 };

s16b ddy_ddd[9] =
	{ 1, -1, 0, 0, 1, 1, -1, -1, 0 };

/*
 * Global array for turning a set of offsets(-1 <-> 1) into a direction
 * Note this must be accessed as dddir[oy + 1][ox + 1]
 */
s16b dddir[3][3] =
{
	{  7,  8,  9 },
	{  4,  5,  6 },
	{  1,  2,  3 },
};


/*
* Global array for converting numbers to uppercase hexadecimal digit
 * This array can also be used to convert a number to an octal digit
 */
char hexsym[16] =
	{
		'0', '1', '2', '3', '4', '5', '6', '7',
		'8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
	};

/*
 * This table allows quick conversion from "speed" to "energy"
 * The basic function WAS ((S>=110) ? (S-110) : (100 / (120-S)))
 * Note that table access is *much* quicker than computation.
 *
 * Note that the table has been changed at high speeds.  From
 * "Slow (-40)" to "Fast (+30)" is pretty much unchanged, but
 * at speeds above "Fast (+30)", one approaches an asymptotic
 * effective limit of 50 energy per turn.  This means that it
 * is relatively easy to reach "Fast (+30)" and get about 40
 * energy per turn, but then speed becomes very "expensive",
 * and you must get all the way to "Fast (+50)" to reach the
 * point of getting 45 energy per turn.  After that point,
 * furthur increases in speed are more or less pointless,
 * except to balance out heavy inventory.
 */

/* Hack - a short macro to convert values based on a turn energy of 100
 * to ones based on TURN_ENERGY */
#define TE *TURN_ENERGY/100

#define EE(A,B,C,D,E,F,G,H,I,J) \
	A TE, B TE, C TE, D TE, E TE, F TE, G TE, H TE, I TE, J TE,

/*
 * This table has been inverted for Cthangband. The new values
 * are 1000/x where x is the old value. This gives the same spread
 * but subtracting extract_energy[n] each move and always adding
 * 10 per turn, rather than adding extract_energy[n] each turn and
 * always subtracting 100.
 *
 * This has been done to allow the seperating out of movement speed
 * and attack speed.
 */
u16b extract_energy[300] =
{
	/* Slow */ EE(1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000)
	/* Slow */ EE(1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000)
	/* Slow */ EE(1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000)
	/* Slow */ EE(1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000)
	/* Slow */ EE(1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000)
	/* Slow */ EE(1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000)
	/* Slow */ EE(1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000)
	/* S-40 */ EE( 500,  500,  500,  500,  500,  500,  500,  500,  500,  500)
	/* S-30 */ EE( 500,  500,  500,  500,  500,  500,  500,  333,  333,  333)
	/* S-20 */ EE( 333,  333,  333,  333,  333,  250,  250,  250,  250,  250)
	/* S-10 */ EE( 200,  200,  200,  200,  167,  167,  143,  143,  125,  111)
	/* Norm */ EE( 100,   91,   83,   77,   71,   67,   63,   59,   56,   53)
	/* F+10 */ EE(  50,   48,   45,   43,   42,   40,   38,   37,   36,   34)
	/* F+20 */ EE(  33,   32,   31,   30,   29,   29,   28,   28,   27,   27)
	/* F+30 */ EE(  26,   26,   26,   26,   25,   25,   25,   24,   24,   24)
	/* F+40 */ EE(  24,   24,   24,   23,   23,   23,   23,   23,   23,   23)
	/* F+50 */ EE(  22,   22,   22,   22,   22,   22,   22,   22,   22,   22)
	/* F+60 */ EE(  21,   21,   21,   21,   21,   21,   21,   21,   21,   21)
	/* F+70 */ EE(  20,   20,   20,   20,   20,   20,   20,   20,   20,   20)
	/* Fast */ EE(  20,   20,   20,   20,   20,   20,   20,   20,   20,   20)
	/* Virt */ EE(  20,   20,   20,   20,   20,   20,   20,   20,   20,   20)
	/* Virt */ EE(  20,   20,   20,   20,   20,   20,   20,   20,   20,   20)
	/* Virt */ EE(  20,   20,   20,   20,   20,   20,   20,   20,   20,   20)
	/* Virt */ EE(  20,   20,   20,   20,   20,   20,   20,   20,   20,   20)
	/* Virt */ EE(  20,   20,   20,   20,   20,   20,   20,   20,   20,   20)
	/* Virt */ EE(  20,   20,   20,   20,   20,   20,   20,   20,   20,   20)
	/* Virt */ EE(  20,   20,   20,   20,   20,   20,   20,   20,   20,   20)
	/* Virt */ EE(  20,   20,   20,   20,   20,   20,   20,   20,   20,   20)
	/* Virt */ EE(  20,   20,   20,   20,   20,   20,   20,   20,   20,   20)
	/* Virt */ EE(  20,   20,   20,   20,   20,   20,   20,   20,   20,   20)
};


/*
 * Hack -- the "basic" color names (see "TERM_xxx")
 */
cptr color_names[256] =
{
	"Dark",
	"White",
	"Slate",
	"Orange",
	"Red",
	"Green",
	"Blue",
	"Umber",
	"Light Dark",
	"Light Slate",
	"Violet",
	"Yellow",
	"Light Red",
	"Light Green",
	"Light Blue",
	"Light Umber",
};


/*
 * Abbreviations of healthy stats
 */
cptr stat_names[6] =
{
	"STR", "INT", "WIS", "DEX", "CON", "CHR"
};

/*
 * Abbreviations of damaged stats
 */
cptr stat_names_reduced[6] =
{
	"Str", "Int", "Wis", "Dex", "Con", "Chr"
};


magic_power mindcraft_powers[MAX_MINDCRAFT_POWERS] =
{
	/* Level gained,  cost,  %fail,  name,  desc */
	{
		/* Det. monsters/traps */
		1, 1, 15,
		"Precognition",
		"Detect monsters, traps and level layout and lights up at higher levels."
	},
	{
		/* ~MM */
		2, 1, 20,
		"Neural Blast",
		"Blast the minds of your foes."
	},
	{
		/* Phase/Between gate */
		3, 2, 25,
		"Minor Displacement",
		"Short distance teleportation"
	},
	{
		/* Tele. Self / All */
		7, 6, 35,
		"Major Displacement",
		"Teleport you and others at high levels."
	},
	{
		9, 7, 50,
		"Domination",
		"Charm monsters"
	},
	{
		/* Telekinetic "bolt" */
		11, 7, 30,
		"Pulverise",
		"Fires a bolt of pure sound."
	},
	{
		/* Psychic/physical defenses */
		13, 12, 50,
		"Character Armour",
		"Sets up physical/elemental shield."
	},
	{
		15, 12, 60,
		"Psychometry",
		"Senses/identifies objects."
	},
	{
		/* Ball -> LOS */
		18, 10, 45,
		"Mind Wave",
		"Projects psi waves to crush the minds of your foes."
	},
	{
		23, 15, 50,
		"Adrenaline Channeling",
		"Heals you, cures you and speeds you."
	},
	{
		/* Convert enemy HP to mana */
		25, 10, 40,
		"Psychic Drain",
		"Drain your foes' life into your mana reserves"
	},
	{
		/* Ball -> LOS */
		28, 20, 45,
		"Telekinetic Wave",
		"Powerful wave of pure telekinetic forces."
	},
		};

	magic_power necro_powers[MAX_NECRO_POWERS] =
		{
			/* Level gained,  cost,  %fail,  name,  desc */
			{
				/* Bolt/beam/ball/LOS of stun/scare */
				1, 2, 10,
				"Horrify",
				"Calls upon the darkness to stun and scare your foes."
			},
	{
		/* Ball */
		5, 6, 20,
		"Raise Dead",
		"Brings back your foes in the form of various undead.  Also, can heal monsters."
	},
	{
		/* Summons weapon */
		12, 20, 25,
		"Necromantic Teeth",
		"Conjures a temporary vampiric weapon."
	},
	{
		/* Heals when killing a monster */
		20, 10, 25,
		"Absorb Soul",
		"Gives back some life for each kill."
	},
	{
		/* Bolt */
		30, 15, 20,
		"Vampirism",
		"Drain the life of your foes into your own."
	},
	{
		/* The Death word, always bolt put your HP to 1 */
		35, 100, 25,
		"Death",
		"Instantly kills your opponent and you, turning yourself into an undead."
	},
};

magic_power mimic_powers[MAX_MIMIC_POWERS] =
{
	/* Level gained,  cost,  %fail,  name */
	{
		/* Use a book of lore */
		1, 2, 0,
		"Mimic",
		"Lets you use the powers of a Cloak of Mimicry."
	},
	{
		/* Invisibility */
		10, 6, 20,
		"Invisibility",
		"Hides you from the sight of mortals."
	},
	{
		/* +1 pair of legs */
		25, 20, 25,
		"Legs Mimicry",
		"Temporarily provides a new pair of legs."
	},
	{
		/* wall form */
		30, 40, 30,
		"Wall Mimicry",
		"Temporarily lets you walk in walls, and ONLY in walls."
	},
	{
		/* +1 pair of arms, +1 weapon */
		35, 100, 40,
		"Arms Mimicry",
		"Temporarily provides a new pair of arms."
	},
};

magic_power symbiotic_powers[MAX_SYMBIOTIC_POWERS] =
{
	/* Level gained,  cost,  %fail,  name */
	{
		1, 1, 0,
		"Hypnotise",
		"Hypnotise a non-moving pet to allow you to enter symbiosis(wear) with it."
	},
	{
		1, 1, 0,
		"Release",
		"Release an hypnotised pet."
	},
	{
		3, 2, 10,
		"Charm Never-Moving",
		"Tries to charm a never-moving monster."
	},
	{
		5, 5, 20,
		"Life Share",
		"Evens out your life with your symbiote."
	},
	{
		10, 10, 20,
		"Use Minor Powers",
		"Allows you to use some of the powers of your symbiote."
	},
	{
		15, 14, 25,
		"Heal Symbiote",
		"Heals your symbiotic monster."
	},
	{
		25, 30, 40,
		"Use major powers",
		"Allows you to use all the powers of your symbiote."
	},
	{
		30, 35, 40,
		"Summon Never-Moving Pet",
		"Summons a never-moving pet."
	},
	{
		40, 60, 70,
		"Force Symbiosis",
		"Allows you to use all the powers of a monster in your line of sight."
	},
};



/* jk - to hit, to dam, to ac, to stealth, to disarm, to saving throw */
/* this concept is taken from Adom, where Thomas Biskup thought it out, */
/* as far as I know. */
tactic_info_type tactic_info[9] =
{
	/*        hit  dam   ac stl  dis  sav */
	{ -10, -10, + 15, + 3, + 15, + 14, "coward"},             /* 4-4 */
	{ -8, -8, + 10, + 2, + 9, + 9, "meek"},               /* 4-3 */
	{ -4, -4, + 5, + 1, + 5, + 5, "wary"},               /* 4-2 */
	{ -2, -2, + 2, + 1, + 2, + 2, "careful"},            /* 4-1 */
	{ 0, 0, 0, 0, 0, 0, "normal"},             /* 4+0 */
	{ 2, 2, -2, -1, -2, -3, "confident"},          /* 4+1 */
	{ 4, 4, -5, -2, -5, -7, "aggressive"},         /* 4+2 */
	{ 6, 6, -10, -3, -11, -12, "furious"},            /* 4+3 */
	{ 8, 12, -25, -5, -18, -18, "berserker"}         /* 4+4 */
};

/*
 * Random artifact activations.
 */
activation activation_info[MAX_T_ACT] =
{
	{ "death", 0, ACT_DEATH },
	{ "ruination", 0, ACT_RUINATION },
	{ "destruction", 1000, ACT_DESTRUC },
	{ "stupidity", 0, ACT_UNINT },
	{ "weakness", 0, ACT_UNSTR },
	{ "unhealth", 0, ACT_UNCON },
	{ "ugliness", 0, ACT_UNCHR },
	{ "clumsiness", 0, ACT_UNDEX },
	{ "naivete", 0, ACT_UNWIS },
	{ "stat loss", 0, ACT_STATLOSS },
	{ "huge stat loss", 0, ACT_HISTATLOSS },
	{ "experience loss", 0, ACT_EXPLOSS },
	{ "huge experience loss", 0, ACT_HIEXPLOSS },
	{ "teleportation", 1000, ACT_TELEPORT },
	{ "monster summoning", 5, ACT_SUMMON_MONST },
	{ "paralyzation", 0, ACT_PARALYZE },
	{ "hallucination", 100, ACT_HALLU },
	{ "poisoning", 0, ACT_POISON },
	{ "hunger", 0, ACT_HUNGER },
	{ "stun", 0, ACT_STUN },
	{ "cuts", 0, ACT_CUTS },
	{ "paranoia", 0, ACT_PARANO },
	{ "confusion", 0, ACT_CONFUSION },
	{ "blindness", 0, ACT_BLIND },
	{ "pet summoning", 1010, ACT_PET_SUMMON },
	{ "cure paralyzation", 5000, ACT_CURE_PARA },
	{ "cure hallucination", 1000, ACT_CURE_HALLU },
	{ "cure poison", 1000, ACT_CURE_POISON },
	{ "cure hunger", 1000, ACT_CURE_HUNGER },
	{ "cure stun", 1000, ACT_CURE_STUN },
	{ "cure cut", 1000, ACT_CURE_CUTS },
	{ "cure fear", 1000, ACT_CURE_FEAR },
	{ "cure confusion", 1000, ACT_CURE_CONF },
	{ "cure blindness", 1000, ACT_CURE_BLIND },
	{ "cure light wounds", 500, ACT_CURE_LW },
	{ "cure serious wounds", 750, ACT_CURE_MW },
	{ "cure critical wounds", 1000, ACT_CURE_700 },
	{ "curing", 1100, ACT_CURING },
	{ "genocide", 5000, ACT_GENOCIDE },
	{ "mass genocide", 10000, ACT_MASS_GENO },
	{ "restoration", 2000, ACT_REST_ALL },
	{ "light", 1000, ACT_LIGHT },
	{ "darkness", 0, ACT_DARKNESS },
	{ "teleportation", 1000, ACT_TELEPORT },
	{ "level teleportation", 500, ACT_LEV_TELE },
	{ "acquirement", 30000, ACT_ACQUIREMENT },
	{ "something weird", 50, ACT_WEIRD },
	{ "aggravation", 0, ACT_AGGRAVATE },
	{ "corruption", 100, ACT_MUT },
	{ "cure insanity", 2000, ACT_CURE_INSANITY },
	{ "light absortion", 800, ACT_LIGHT_ABSORBTION },
#if 0 /* No more for the time being, ehehhe evil I am :> */
	{ "cure corruption", 2000, ACT_CURE_MUT },
#endif
};

/*
 * Possible movement type.
 */
move_info_type move_info[9] =
{
	/*        speed, searching, stealth, perception */
	{ -10, 17, 4, 20, "slug-like"},
	{ -8, 12, 4, 16, "very slow"},
	{ -6, 8, 3, 10, "slow"},
	{ -3, 4, 2, 6, "leisurely"},
	{ 0, 0, 0, 0, "normal"},
	{ 1, -4, -1, -4, "brisk"},
	{ 2, -6, -4, -8, "fast"},
	{ 3, -10, -7, -14, "very fast"},
	{ 4, -16, -10, -20, "running"}
};

/*
 * Inscriptions for pseudo-id
 */
cptr sense_desc[] =
{
	"whoops",
	"cursed",
	"average",
	"good",
	"good",
	"excellent",
	"worthless",
	"terrible",
	"special",
	"broken",
	""
};


/* Powers */
power_type powers_type_init[POWER_MAX_INIT] =
{
	{
		"Incarnate",
		"You can incarnate into a body.",
		"You feel the need to get a body.",
		"You no longer feel the need for a new body.",
		0, 0, 0, 0,
	},
};

/*
 * The Quests
 */
quest_type quest_init_tome[MAX_Q_IDX_INIT] =
{
	{
		FALSE,
		FALSE,
		"",
		{
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
		},
		QUEST_STATUS_UNTAKEN,
		0,

		NULL,
		HOOK_TYPE_C,
		quest_null_hook,
		{0, 0},
	},
};

/*
 * Flags pointers types
 */
flags_pointer_type flags_pointer_types[] =
{
	/* Object */
	{
		"object_type",
		(flag_pointer_alloc)new_object,
		(flag_pointer_dup)object_dup,
		(flag_pointer_loadsave)loadsave_do_item,
		(flag_pointer_unalloc)delete_object,
	},
	/* String, unable to save/load */
	{
		"cptr",
		(flag_pointer_alloc)NULL,
		(flag_pointer_dup)string_make,
		(flag_pointer_loadsave)loadsave_do_string_nolen,
		(flag_pointer_unalloc)string_free,
	},
	/* Monster blows */
	{
		"monster_blow",
		(flag_pointer_alloc)monster_blow_new,
		(flag_pointer_dup)monster_blow_dup,
		(flag_pointer_loadsave)monster_blow_loadsave,
		(flag_pointer_unalloc)monster_blow_del,
	},
	/* Monster */
	{
		"monster_type",
		(flag_pointer_alloc)monster_type_new,
		(flag_pointer_dup)monster_type_dup,
		(flag_pointer_loadsave)loadsave_do_monster,
		(flag_pointer_unalloc)monster_type_del,
	},
	/* Data buffers */
	{
		"buffer_type",
		(flag_pointer_alloc)data_buffer_alloc,
		(flag_pointer_dup)data_buffer_dup,
		(flag_pointer_loadsave)data_buffer_loadsave,
		(flag_pointer_unalloc)data_buffer_delete,
	},
	/* Stores */
	{
		"store_type",
		(flag_pointer_alloc)store_type_new,
		(flag_pointer_dup)store_type_dup,
		(flag_pointer_loadsave)loadsave_do_store,
		(flag_pointer_unalloc)store_type_del,
	},
};

/*
 * List of slot names
 */
inventory_slot_type inventory_slot_names_init[] =
{
	{
		"inventory",
		"In pack",
		"carrying in your backpack",
		"You are seeing a bug!",
		"You were carrying in your backpack",
	},
};
