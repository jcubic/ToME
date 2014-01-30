/* File: types.h */

/* Purpose: global type declarations */

/*
 * Copyright (c) 1989 James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */


/*
 * This file should ONLY be included by "angband.h"
 */


/*
 * Note that "char" may or may not be signed, and that "signed char"
 * may or may not work on all machines.  So always use "s16b" or "s32b"
 * for signed values.  Also, note that unsigned values cause math problems
 * in many cases, so try to only use "u16b" and "u32b" for "bit flags",
 * unless you really need the extra bit of information, or you really
 * need to restrict yourself to a single byte for storage reasons.
 *
 * Also, if possible, attempt to restrict yourself to sub-fields of
 * known size (use "s16b" or "s32b" instead of "s32b", and "byte" instead
 * of "bool"), and attempt to align all fields along four-byte words, to
 * optimize storage issues on 32-bit machines.  Also, avoid "bit flags"
 * since these increase the code size and slow down execution.  When
 * you need to store bit flags, use one byte per flag, or, where space
 * is an issue, use a "byte" or "u16b" or "u32b", and add special code
 * to access the various bit flags.
 *
 * Many of these structures were developed to reduce the number of global
 * variables, facilitate structured program design, allow the use of ascii
 * template files, simplify access to indexed data, or facilitate efficient
 * clearing of many variables at once.
 *
 * Certain data is saved in multiple places for efficient access, currently,
 * this includes the tval/sval/weight fields in "object_type", various fields
 * in "header_type", and the "m_idx" and "o_idx" fields in "cave_type".  All
 * of these could be removed, but this would, in general, slow down the game
 * and increase the complexity of the code.
 */



/*
 * Flags
 */
typedef struct flag_node
{
	u32b key;
	union
	{
		struct
		{
			s32b val;
			s32b val2;
		} vals;
		struct flags_type *flags;
		struct
		{
			byte type;
			void *ptr;
		} ptr;
	} vals;
	byte flags;
	struct flag_node *next;  /* for chaining */
} flag_node;

typedef struct flags_type
{
	flag_node *node;

	s32b size;

	flag_node *firstfree;  /* this position is free; all positions after it are full */
} flags_type;

typedef flags_type vector_type;

typedef void *(*flag_pointer_alloc)();
typedef void *(*flag_pointer_dup)(void *ptr);
typedef void (*flag_pointer_unalloc)(void *ptr);
typedef void (*flag_pointer_loadsave)(byte type, void **ptr, s32b flg);
typedef struct flags_pointer_type
{
	cptr name;
	flag_pointer_alloc alloc;
	flag_pointer_dup dup;
	flag_pointer_loadsave loadsave;
	flag_pointer_unalloc unalloc;
} flags_pointer_type;

/* This easy the use of lone flagsets */
typedef struct flags_holder_type
{
	flags_type flags;
} flags_holder_type;

typedef struct flag_registry
{
	u32b index;
	cptr name;

        struct flag_registry *next;
} flag_registry;


/*
 * Template file header information (see "init.c").  16 bytes.
 *
 * Note that the sizes of many of the "arrays" are between 32768 and
 * 65535, and so we must use "unsigned" values to hold the "sizes" of
 * these arrays below.  Normally, I try to avoid using unsigned values,
 * since they can cause all sorts of bizarre problems, but I have no
 * choice here, at least, until the "race" array is split into "normal"
 * and "unique" monsters, which may or may not actually help.
 *
 * Note that, on some machines, for example, the Macintosh, the standard
 * "read()" and "write()" functions cannot handle more than 32767 bytes
 * at one time, so we need replacement functions, see "util.c" for details.
 *
 * Note that, on some machines, for example, the Macintosh, the standard
 * "malloc()" function cannot handle more than 32767 bytes at one time,
 * but we may assume that the "ralloc()" function can handle up to 65535
 * butes at one time.  We should not, however, assume that the "ralloc()"
 * function can handle more than 65536 bytes at a time, since this might
 * result in segmentation problems on certain older machines, and in fact,
 * we should not assume that it can handle exactly 65536 bytes at a time,
 * since the internal functions may use an unsigned short to specify size.
 *
 * In general, these problems occur only on machines (such as most personal
 * computers) which use 2 byte "s32b" values, and which use "s32b" for the
 * arguments to the relevent functions.
 */

typedef struct header header;

struct header
{
	byte	v_major;		/* Version -- major */
	byte	v_minor;		/* Version -- minor */
	byte	v_patch;		/* Version -- patch */
	byte	v_extra;		/* Version -- extra */


	u16b	info_num;		/* Number of "info" records */

	u16b	info_len;		/* Size of each "info" record */


	u32b	head_size;		/* Size of the "header" in bytes */

	u32b	info_size;		/* Size of the "info" array in bytes */

	u32b	name_size;		/* Size of the "name" array in bytes */

	u32b	text_size;		/* Size of the "text" array in bytes */
};


/*
 * Information about terrain "features"
 */

typedef struct feature_type feature_type;

struct feature_type
{
	cptr name;              /* Name (offset) */
	cptr text;              /* Text (offset) */
	cptr block;             /* Text for blocking */

	s32b can_enter;         /* A lua function that allows or no passage */

	s16b mimic;             /* Feature to mimic */

	flags_type flags;       /* flags */

	byte extra;             /* Extra byte (unused) */

	s16b unused;		/* Extra bytes (unused) */

	byte d_attr;		/* Default feature attribute */
	char d_char;		/* Default feature character */


	byte x_attr;		/* Desired feature attribute */
	char x_char;		/* Desired feature character */

	byte shimmer[7];        /* Shimmer colors */

	s16b d_dice[4];                  /* Number of dices */
	s16b d_side[4];                  /* Number of sides */
	s16b d_frequency[4];             /* Frequency of damage (1 is the minimum) */
	s16b d_type[4];                  /* Type of damage */

	s16b priority;          /* Drawing priority */

	cptr load_script;
};


/*
 * Information about object "kinds", including player knowledge.
 *
 * Only "aware" and "tried" are saved in the savefile
 */

typedef struct object_kind object_kind;

struct object_kind
{
	cptr name;			/* Name (offset) */
	cptr text;			/* Text (offset) */

	byte tval;			/* Object type */
	byte sval;			/* Object sub type */

	s16b to_h;			/* Bonus to hit */
	s16b to_d;			/* Bonus to damage */
	s16b to_a;			/* Bonus to armor */

	s16b ac;			/* Base armor */

	byte dd, ds;		/* Damage dice/sides */

	s32b weight;            /* Weight */

	s32b cost;			/* Object "base cost" */

	flags_type flags;       /* flags */

	byte locale[4];		/* Allocation level(s) */
	byte chance[4];		/* Allocation chance(s) */

	byte level;			/* Level */
	byte extra;			/* Something */


	byte d_attr;		/* Default object attribute */
	char d_char;		/* Default object character */


	byte x_attr;		/* Desired object attribute */
	char x_char;		/* Desired object character */


	byte flavor;			/* Special object flavor (or zero) */

	bool easy_know;		/* This object is always known (if aware) */


	bool aware;			/* The player is "aware" of the item's effects */

	bool tried;			/* The player has "tried" one of the items */

	bool know;                      /* extractable flag for the alchemist */

	byte btval;                     /* Become Object type */
	byte bsval;                     /* Become Object sub type */
	bool artifact;                  /* Is it a normal artifact(already generated) */

	s16b power;                     /* Power granted(if any) */

	cptr load_script;               /* Script to execute upon loading from raw file */
};



/*
 * Information about "artifacts".
 *
 * Note that the save-file only writes "cur_num" to the savefile.
 *
 * Note that "max_num" is always "1" (if that artifact "exists")
 */

typedef struct artifact_type artifact_type;

struct artifact_type
{
	cptr name;			/* Name (offset) */
	cptr unknown_name;		/* Name displayed when unaware */
	cptr text;			/* Text (offset) */

	byte tval;			/* Artifact type */
	byte sval;			/* Artifact sub type */

	s16b d_attr;        /* Default attribute (-1 for same as base object) */
	s16b x_attr;        /* Desired attribute (-1 for same as base object) */

	s16b to_h;			/* Bonus to hit */
	s16b to_d;			/* Bonus to damage */
	s16b to_a;			/* Bonus to armor */

	s16b ac;			/* Base armor */

	byte dd, ds;		/* Damage when hits */

	s16b weight;		/* Weight */

	s32b cost;			/* Artifact "cost" */

	flags_type flags;       /* flags */

	byte level;			/* Artifact level */
	byte rarity;		/* Artifact rarity */

	s16b cur_num;		/* Number created (0 or 1) */
	s16b max_num;		/* Unused (should be "1") */
	bool known;         /* Artifact has been identified */

	s16b power;                     /* Power granted(if any) */

	s16b set;               /* Does it belongs to a set ?*/
	s16b set_pos;           /* Does it belongs to a set, if so in which "position" ? */

	cptr load_script;
};


/*
 * Information about "ego-items".
 */

typedef struct ego_item_type ego_item_type;

struct ego_item_type
{
	cptr name;			/* Name (offset) */
	cptr text;			/* Text (offset) */

	bool before;                    /* Before or after the object name ? */
	cptr ego_group;			/* Ego group */

	s16b tval[MAX_TVALS_PER_EGO];
	s16b min_sval[MAX_TVALS_PER_EGO];
	s16b max_sval[MAX_TVALS_PER_EGO];
        flags_type item_kinds_ok;

	byte rating;		/* Rating boost */

	byte level;		/* Minimum level */
	s16b allocation;        /* Allocation chance */

	s16b max_to_h;          /* Maximum to-hit bonus */
	s16b max_to_d;          /* Maximum to-dam bonus */
	s16b max_to_a;          /* Maximum to-ac bonus */

	u32b base_value_pct;	/* Item's base value is X% after ego applied */
	u32b value_add;			/* Value ego adds to the item  */

	byte rar[MAX_EGO_FLAG_GROUPS];
	flags_type flags[MAX_EGO_FLAG_GROUPS];       /* flags */

	flags_type need_flags;       /* flags */
	flags_type forbid_flags;       /* flags */

	s16b power;                     /* Power granted(if any) */

	cptr load_script;
};


/*
 * Information about "random artifacts parts".
 */
typedef struct randart_part_type randart_part_type;
struct randart_part_type
{
	byte tval[20];
	byte min_sval[20];
	byte max_sval[20];

	byte level;             /* Minimum level */
	byte rarity;            /* Object rarity */
	byte mrarity;           /* Object rarity */

	s16b max_to_h;          /* Maximum to-hit bonus */
	s16b max_to_d;          /* Maximum to-dam bonus */
	s16b max_to_a;          /* Maximum to-ac bonus */

	s32b value;             /* power value */
	s16b max;               /* Number of time it can appear on a single item */

	flags_type flags;       /* flags */

	flags_type aflags;       /* flags */

	s16b power;             /* Power granted(if any) */
};

typedef struct randart_gen_type randart_gen_type;
struct randart_gen_type
{
	s32b chance;             /* Chance to have that number of powers */
	s32b dd;
	s32b ds;
	s32b plus;               /* xdy+plus power */
};


/*
 * Monster blow structure
 *
 *	- Method (RBM_*)
 *	- Effect (RBE_*)
 *	- Damage Dice
 *	- Damage Sides
 */

typedef struct monster_blow monster_blow;

struct monster_blow
{
	byte method;
	byte effect;
	byte d_dice;
	byte d_side;
};



/*
 * Monster "race" information, including racial memories
 *
 * Note that "d_attr" and "d_char" are used for MORE than "visual" stuff.
 *
 * Note that "x_attr" and "x_char" are used ONLY for "visual" stuff.
 *
 * Note that "cur_num" (and "max_num") represent the number of monsters
 * of the given race currently on (and allowed on) the current level.
 * This information yields the "dead" flag for Unique monsters.
 *
 * Note that "max_num" is reset when a new player is created.
 * Note that "cur_num" is reset when a new level is created.
 *
 * Note that several of these fields, related to "recall", can be
 * scrapped if space becomes an issue, resulting in less "complete"
 * monster recall (no knowledge of spells, etc).  All of the "recall"
 * fields have a special prefix to aid in searching for them.
 */


typedef struct monster_race monster_race;

struct monster_race
{
	cptr name;				/* Name (offset) */
	cptr text;                              /* Text (offset) */

	u16b hdice;				/* Creatures hit dice count */
	u16b hside;				/* Creatures hit dice sides */

	s16b ac;				/* Armour Class */

	s16b sleep;				/* Inactive counter (base) */
	byte aaf;				/* Area affect radius (1-100) */
	byte speed;				/* Speed (normally 110) */

	s32b mexp;				/* Exp value for kill */

	s32b weight;                            /* Weight of the monster */

	byte freq_inate;		/* Inate spell frequency */
	byte freq_spell;		/* Other spell frequency */

	flags_type flags;       /* flags */
	flags_type spells;       /* flags */

	flags_type blow;           /* blows per round */

	flags_type body_parts;      /* To help to decide what to use when body changing */

	byte level;                     /* Level of creature */
	byte rarity;			/* Rarity of creature */


	byte d_attr;			/* Default monster attribute */
	char d_char;			/* Default monster character */

	s16b sval;                      /* Monsters are uniquely identified by their char/sval */
					/* Just as obejcts are identified by tval/sval */

	byte x_attr;			/* Desired monster attribute */
	char x_char;			/* Desired monster character */


	s16b max_num;                   /* Maximum population allowed per level */

	s16b cur_num;			/* Monster population on current level */


	s16b r_sights;			/* Count sightings of this monster */
	s16b r_deaths;			/* Count deaths from this monster */

	s16b r_pkills;			/* Count monsters killed in this life */
	s16b r_tkills;			/* Count monsters killed in all lives */

	byte r_wake;			/* Number of times woken up (?) */
	byte r_ignore;			/* Number of times ignored (?) */

	byte r_xtra1;			/* Something (unused) */
	byte r_xtra2;			/* Something (unused) */

	byte r_drop_gold;		/* Max number of gold dropped at once */
	byte r_drop_item;		/* Max number of item dropped at once */

	byte r_cast_inate;		/* Max number of inate spells seen */
	byte r_cast_spell;		/* Max number of other spells seen */

	byte r_blows[4];		/* Number of times each blow type was seen */

	flags_type r_flags;       /* flags */

	byte total_visible;             /* Amount of this race that are visible */

	cptr load_script;
};


typedef struct monster_ego monster_ego;

struct monster_ego
{
	cptr name;				/* Name (offset) */
	bool before;                            /* Display ego before or after */

	flags_type blow;           		/* blows per round */
	flags_type blowm_dice;
	flags_type blowm_side;

	s16b hdice;                             /* Creatures hit dice count */
	s16b hside;                             /* Creatures hit dice sides */

	s16b ac;				/* Armour Class */

	s16b sleep;				/* Inactive counter (base) */
	s16b aaf;                               /* Area affect radius (1-100) */
	s16b speed;                             /* Speed (normally 110) */

	s32b mexp;				/* Exp value for kill */

	s32b weight;                            /* Weight of the monster */

	byte freq_inate;                /* Inate spell frequency */
	byte freq_spell;		/* Other spell frequency */

	/* Ego flags */
	flags_type flags;       /* flags */
	flags_type hflags;       /* flags */

	/* Monster flags */
	flags_type mflags;       /* flags */

	/* Negative Flags, to be removed from the monster flags */
	flags_type nflags;       /* flags */

	/* Added Spells flags */
	flags_type msflags;       /* flags */

	/* Removed Spells  Flags */
	flags_type nsflags;       /* flags */
	bool remove_all_spells;

	s16b level;                     /* Level of creature */
	s16b rarity;                    /* Rarity of creature */


	byte d_attr;			/* Default monster attribute */
	char d_char;			/* Default monster character */

	byte g_attr;                    /* Overlay graphic attribute */
	char g_char;                    /* Overlay graphic character */

	char r_char[5];                 /* Monster race allowed */
	char nr_char[5];                /* Monster race not allowed */

	cptr load_script;
};



/*
 * Information about "vault generation"
 */

typedef struct vault_type vault_type;

struct vault_type
{
	cptr name;			/* Name (offset) */
	cptr text;			/* Text (offset) */
	bool dynamic;			/* This means the layout is not an hardcoded layout but rather points to a maps file in /data/dungeons/vaults/ */

	byte typ;			/* Vault type */

	byte rat;			/* Vault rating */

	byte hgt;			/* Vault height */
	byte wid;			/* Vault width */

	s16b lvl;                       /* level of special (if any) */
	byte dun_type;                  /* Dungeon type where the level will show up */

	s16b mon[10];                   /* special monster */
	s16b item[3];                   /* number of item (usually artifact) */

	flags_type flags;

	cptr load_script;
};

/*
 * A single "grid" in a Cave
 *
 * Note that several aspects of the code restrict the actual cave
 * to a max size of 256 by 256.  In partcular, locations are often
 * saved as bytes, limiting each coordinate to the 0-255 range.
 *
 * The "o_idx" and "m_idx" fields are very interesting.  There are
 * many places in the code where we need quick access to the actual
 * monster or object(s) in a given cave grid.  The easiest way to
 * do this is to simply keep the index of the monster and object
 * (if any) with the grid, but this takes 198*66*4 bytes of memory.
 * Several other methods come to mind, which require only half this
 * amound of memory, but they all seem rather complicated, and would
 * probably add enough code that the savings would be lost.  So for
 * these reasons, we simply store an index into the "o_list" and
 * "m_list" arrays, using "zero" when no monster/object is present.
 *
 * Note that "o_idx" is the index of the top object in a stack of
 * objects, using the "next_o_idx" field of objects (see below) to
 * create the singly linked list of objects.  If "o_idx" is zero
 * then there are no objects in the grid.
 */

typedef struct cave_type cave_type;

struct cave_type
{
	u16b info;		/* Hack -- cave flags */

	s16b feat;		/* Hack -- feature type */

	flags_type inventory;	/* Object in this grid */

	s16b m_idx;		/* Monster in this grid */

	flags_type flags;

	s16b mana;              /* Magical energy of the grid */

	s16b mimic;		/* Feature to mimic */

	s16b effect;            /* The lasting effects */

	flags_type activations; /* The various effects that can happen. This is a flagset of flagsets */

	u32b los_turn;		/* The turn this case was in LOS */
	byte los_dist;		/* The distance it was from the player when it was last in LOS */
};

/* Lasting spell effects(clouds, ..) */
typedef struct effect_type effect_type;
struct effect_type
{
	s16b    who;           	/* Who did it */
	s16b    time;           /* For how long */
	s16b    dam;            /* How much damage */
	s16b    type;           /* Of which type */
	s16b    cy;             /* Center of the cast*/
	s16b    cx;             /* Center of the cast*/
	s16b    rad;            /* Radius -- if needed */
	s16b    speed;          /* Speed */
	u32b    flags;          /* Flags */

	s32b    energy;         /* Current energy(for speed) */
};

/*
 * Object information, for a specific object.
 *
 * Note that a "discount" on an item is permanent and never goes away.
 *
 * Note that inscriptions are now handled via the "quark_str()" function
 * applied to the "note" field, which will return NULL if "note" is zero.
 *
 * Note that "object" records are "copied" on a fairly regular basis,
 * and care must be taken when handling such objects.
 *
 * Note that "object flags" must now be derived from the object kind,
 * the artifact and ego-item indexes, and the two "xtra" fields.
 *
 * Each cave grid points to one (or zero) objects via the "o_idx"
 * field (above).  Each object then points to one (or zero) objects
 * via the "next_o_idx" field, forming a singly linked list, which
 * in game terms, represents a "stack" of objects in the same grid.
 *
 * Each monster points to one (or zero) objects via the "hold_o_idx"
 * field (below).  Each object then points to one (or zero) objects
 * via the "next_o_idx" field, forming a singly linked list, which
 * in game terms, represents a pile of objects held by the monster.
 *
 * The "held_m_idx" field is used to indicate which monster, if any,
 * is holding the object.  Objects being held have "ix=0" and "iy=0".
 */

typedef struct object_type object_type;

struct object_type
{
	s16b k_idx;			/* Kind index (zero if "dead") */

	byte iy;			/* Y-position on map, or zero */
	byte ix;			/* X-position on map, or zero */

	byte tval;			/* Item type (from kind) */
	byte sval;			/* Item sub-type (from kind) */

	byte discount;		/* Discount (if any) */

	byte number;		/* Number of items */

	s32b weight;            /* Item weight */

	byte elevel;            /* Item exp level */
	s32b exp;               /* Item exp */

	byte d_attr;		/* Default object attribute */
	byte x_attr;		/* Desired object attribute */

	s16b artifact_id;	/* Artifact type, if any */
	s16b ego_id[MAX_EGO_PER_OBJ];	/* Ego-Item types, if any */

	byte xtra1;			/* Extra info type */
	s16b xtra2;                     /* Extra info index */

	s16b to_h;			/* Plusses to hit */
	s16b to_d;			/* Plusses to damage */
	s16b to_a;			/* Plusses to AC */

	s16b ac;			/* Normal AC */

	byte dd, ds;		/* Damage dice/sides */

	s16b timeout;		/* Timeout Counter */

	byte ident;			/* Special flags  */

	byte marked;		/* Object is marked */

	u16b note;			/* Inscription index */
	u16b art_name;      /* Artifact name (random artifacts) */

	flags_type flags;

	s16b next_o_idx;	/* Next object in stack (if any) */

	s16b held_m_idx;	/* Monster holding us (if any) */

	byte sense;             /* Pseudo-id status */

	byte found;             /* How did we find it */
	s16b found_aux1;        /* Stores info for found */
	s16b found_aux2;        /* Stores info for found */
	s16b found_aux3;        /* Stores info for found */
	s16b found_aux4;        /* Stores info for found */
};


/*
 * Monster mind, use for skills and such
 */
typedef struct monster_mind monster_mind;
struct monster_mind
{
	/*
	 * Without this, bcc can't compile because it does not
	 * allow empty structure.  Remove this when you add some
	 * variables to this structure.  -- Kusunose
	 */
	byte dummy;
};



/*
 * Monster information, for a specific monster.
 *
 * Note: fy, fx constrain dungeon size to 256x256
 *
 * The "hold_o_idx" field points to the first object of a stack
 * of objects (if any) being carried by the monster (see above).
 */

typedef struct monster_type monster_type;

struct monster_type
{
	s16b r_idx;			/* Monster race index */
	char d_char;
	s16b sval;

	u16b ego;                       /* Ego monster type */

	byte fy;			/* Y location on map */
	byte fx;			/* X location on map */

	s32b hp;			/* Current Hit points */
	s32b maxhp;			/* Max Hit points */

	flags_type blow;           /* blows per round */

	byte speed;                     /* Speed (normally 110) */
	byte level;                     /* Level of creature */
	s16b ac;                        /* Armour Class */
	u32b exp;                       /* Experience */

	s16b csleep;		/* Inactive counter */

	byte mspeed;		/* Monster "speed" */
	s32b energy;		/* Monster "energy" */

	byte stunned;		/* Monster is stunned */
	byte confused;		/* Monster is confused */
	byte monfear;		/* Monster is afraid */

	s16b bleeding;          /* Monster is bleeding */
	s16b poisoned;          /* Monster is poisoned */

	byte cdis;			/* Current dis from player */

	s32b mflag;			/* Extra monster flags */

	byte freq_inate;		/* Inate spell frequency */
	byte freq_spell;		/* Other spell frequency */

	flags_type flags;       /* flags */
	flags_type spells;       /* flags */

	bool ml;			/* Monster is "visible" */

	flags_type inventory;	/* Object being held (if any) */

	u32b ai;			/* Current AI state */
	u16b ai_speed;			/* AI states */
	byte ai_action;			/* AI states */
	s16b ai_move_y;			/* AI states */
	s16b ai_move_x;			/* AI states */
	flags_type ai_states;		/* AI states */

	bool los_flow_last_turn;	/* Did we flow with los algorithm last turn? */

	u32b smart;			/* Field for "smart_learn" */

	u32b faction;			/* What faction does the monster belongs to */

	s16b target;                    /* Monster target */

	s16b possessor;                 /* Is it under the control of a possessor ? */

	monster_race *sr_ptr;           /* Does it have a specific race(not in r_info) */

	monster_mind *mind;             /* Does it have a mind? */
};




/*
 * An entry for the object/monster allocation functions
 *
 * Pass 1 is determined from allocation information
 * Pass 2 is determined from allocation restriction
 * Pass 3 is determined from allocation calculation
 */

typedef struct alloc_entry alloc_entry;

struct alloc_entry
{
	s16b index;		/* The actual index */

	byte level;		/* Base dungeon level */
	byte prob1;		/* Probability, pass 1 */
	byte prob2;		/* Probability, pass 2 */
	byte prob3;		/* Probability, pass 3 */

	u16b total;		/* Unused for now */
};

/*
 * A store owner
 */
typedef struct owner_type owner_type;

struct owner_type
{
	cptr name;                      /* Name (offset) */

	s16b max_cost;                  /* Purse limit */

	byte max_inflate;               /* Inflation (max) */
	byte min_inflate;               /* Inflation (min) */

	byte haggle_per;                /* Haggle unit */

	byte insult_max;                /* Insult limit */

	flags_type races[2];                  /* Liked/hated races */
	flags_type classes[2];                /* Liked/hated classes */

	s16b costs[3];                  /* Costs for liked people */

	cptr load_script;
};




/*
 * A store, with an owner, various state flags, a current stock
 * of items, and a table of items that are often purchased.
 */
typedef struct store_type store_type;

struct store_type
{
	u16b st_idx;

	u16b owner;                     /* Owner index */

	s16b insult_cur;		/* Insult counter */

	s16b good_buy;			/* Number of "good" buys */
	s16b bad_buy;			/* Number of "bad" buys */

	s32b store_open;		/* Closed until this turn */

	s32b last_visit;		/* Last visited on this turn */

	flags_type stock;		/* Stock -- Actual stock items */
};

/*
 * A store/building type
 */
typedef struct store_info_type store_info_type;

struct store_info_type
{
	cptr name;                      /* Name (offset) */

	flags_type table;   		/* Table -- Legal item kinds */
	s16b max_obj;                   /* Number of items this store can hold */

	s32b buy;			/* Lua Function that allows buying */

	cptr store_maker;		/* The store creator to use */
	s16b store_make_num;		/* The number of items to fill the shop with */

	u16b owners[4];                 /* List of owners(refers to ow_info) */

	u16b actions[6];                /* Actions(refers to ba_info) */

	byte d_attr;			/* Default building attribute */
	char d_char;			/* Default building character */

	byte x_attr;			/* Desired building attribute */
	char x_char;			/* Desired building character */

	flags_type flags;       /* flags */

	cptr load_script;
};

/*
 * Stores/buildings actions
 */
typedef struct store_action_type store_action_type;

struct store_action_type
{
	cptr name;                      /* Name (offset) */

	s16b costs[3];                  /* Costs for liked people */
	char letter;                    /* Action letter */
	char letter_aux;                /* Action letter */
	s32b action;                    /* Action code */
	s16b action_restr;              /* Action restriction */

	cptr load_script;               /* Script to execute upon loading from raw file */
};

/*
 * Player racial info
 */

typedef struct player_descriptor player_descriptor;

struct player_descriptor
{
	s16b id;			/* Index of the descriptor */
	cptr type;                      /* Type of the descriptor */
	cptr title;                     /* Name of the descriptor */
	cptr desc;
	byte place;                     /* SUBRACE_PLACE_AFTER = race race modifier, SUBRACE_PLACE_BEFORE = Race modifier race, SUBRACE_PLACE_REPLACE = Race modifier */

	flags_type adj;		/* Stat bonuses */

	char luck;              /* Luck */

	s16b mana;                      /* Mana % */
	s16b blow_num;
	s16b blow_wgt;
	s16b blow_mul;
	s16b extra_blows;

	s16b dis;		/* disarming */
	s16b dev;		/* magic devices */
	s16b sav;		/* saving throw */
	s16b stl;		/* stealth */
	s16b srh;		/* search ability */
	s16b fos;		/* search frequency */
	s16b thn;		/* combat (normal) */
	s16b thb;		/* combat (shooting) */

	byte mhp;		/* Race hit-dice modifier */
	u16b exp;		/* Race experience factor */

	byte b_age;		/* base age */
	byte m_age;		/* mod age */

	byte m_b_ht;		/* base height (males) */
	byte m_m_ht;		/* mod height (males) */
	byte m_b_wt;		/* base weight (males) */
	byte m_m_wt;		/* mod weight (males) */

	byte f_b_ht;		/* base height (females) */
	byte f_m_ht;		/* mod height (females)	  */
	byte f_b_wt;		/* base weight (females) */
	byte f_m_wt;		/* mod weight (females) */

	byte infra;             /* Infra-vision range */

	flags_type choice;            /* Legal otehr descriptors choices */

	s16b powers[4];         /* Powers of the race */

	flags_type body_parts;      /* To help to decide what to use when body changing */

	cptr chart;             /* Chart history */

	flags_type flags;       /* flags */

	flags_type oflags[1000];       /* flags -- DG: WARNING dumb limit detected please make it nicer when not lazy */

	flags_type skill_base;
	flags_type skill_mod;

	flags_type objects;	/* Starting objects */

	flags_type abilities;                /* Abilities to be gained by level(doesnt take prereqs in account) */

	cptr load_script;
};

/* Help type */
typedef struct help_info help_info;
struct help_info
{
	bool enabled;           /* ingame help enabled */

	u32b help1;             /* help flags 1 */
};


/*
 * Most of the "player" information goes here.
 *
 * This stucture gives us a large collection of player variables.
 *
 * This structure contains several "blocks" of information.
 *   (1) the "permanent" info
 *   (2) the "variable" info
 *   (3) the "transient" info
 *
 * All of the "permanent" info, and most of the "variable" info,
 * is saved in the savefile.  The "transient" info is recomputed
 * whenever anything important changes.
 */

typedef struct player_type player_type;

struct player_type
{
	s32b lives;             /* How many times we resurected */

	s16b oldpy;		/* Previous player location -KMW- */
	s16b oldpx;		/* Previous player location -KMW- */
	bool wild_force_side_y_ok;
	bool wild_force_side_x_ok;
	s16b wild_force_side_y;
	s16b wild_force_side_x;

	s16b py;		/* Player location */
	s16b px;		/* Player location */

	player_descriptor descriptor; /* The player descriptor */
	flags_type descriptors;	/* A list of player descriptors */
	byte mimic_form;        /* Actualy transformation */
	s16b mimic_level;       /* Level of the mimic effect */
	byte oops;              /* Unused */

	flags_type inventory_limits;	/* Size limit for all INVEN_foo */
	flags_type inventory; /* Player inventory */

	byte hitdie;		/* Hit dice (sides) */
	u16b expfact;           /* Experience factor */

	byte maximize;		/* Maximize stats */
	byte preserve;		/* Preserve artifacts */
	byte special;           /* Special levels */
	byte allow_one_death;   /* Blood of life */

	s16b age;			/* Characters age */
	s16b ht;			/* Height */
	s16b wt;			/* Weight */
	s16b sc;			/* Social Class */


	s32b au;			/* Current Gold */

	s32b max_exp;		/* Max experience */
	s32b exp;			/* Cur experience */
	u16b exp_frac;		/* Cur exp frac (times 2^16) */

	s16b lev;			/* Level */

	s16b town_num;			/* Current town number */
	s16b arena_number;		/* monster number in arena -KMW- */
	s16b inside_arena;		/* Is character inside arena? */
	s16b inside_quest;		/* Inside quest level */
	bool exit_bldg;			/* Goal obtained in arena? -KMW- */

	s32b wilderness_x;              /* Coordinates in the wilderness */
	s32b wilderness_y;
	s32b old_wilderness_x;              /* Coordinates in the wilderness */
	s32b old_wilderness_y;
	bool wild_mode;                 /* TRUE = Small map, FLASE = Big map */
	bool old_wild_mode;             /* TRUE = Small map, FLASE = Big map */

	s16b hp_mod;                    /* A modificator(permanent) */

	s16b max_plv;                   /* Max Player Level */

	s16b luck_cur;                  /* Current "natural" luck value (range -30 <> 30) */
	s16b luck_max;                  /* Current "maximal base" luck value (range -30 <> 30) */
	s16b luck_base;                 /* Current "base" luck value (range -30 <> 30) */

	s16b recall_dungeon;    /* Recall in which dungeon */
	s16b word_recall;	/* Word of recall counter */

	s32b energy;            /* Current energy */

	s16b new_spells;	/* Number of spells available */

	s16b old_spells;

	s16b xtra_spells;       /* Number of xtra spell learned(via potion) */

	bool old_cumber_armor;
	bool old_cumber_glove;
	bool old_heavy_wield;
	bool old_heavy_shoot;
	bool old_icky_wield;

	s16b old_lite;		/* Old radius of lite (if any) */
	s16b old_view;		/* Old radius of view (if any) */

	s16b old_food_aux;	/* Old value of food */


	bool cumber_armor;	/* Mana draining armor */
	bool cumber_glove;	/* Mana draining gloves */
	bool heavy_wield;	/* Heavy weapon */
	bool heavy_shoot;	/* Heavy shooter */
	bool icky_wield;	/* Icky weapon */

	s16b cur_lite;		/* Radius of lite (if any) */


	u32b notice;		/* Special Updates (bit flags) */
	u32b update;		/* Pending Updates (bit flags) */

	flags_type redraw;	/* Normal Redraws */

	flags_type window;		/* Window Redraws (bit flags) */

	flags_type stat_max;       /* Current "maximal" stat values */
	flags_type stat_cur;       /* Current "natural" stat values */
	flags_type stat_add;	/* Modifiers to stat values */

	flags_type stat_ind;	/* Current stat values */
	flags_type stat_top;	/* Maximal modified stats */

	flags_type stat_cnt;	/* Counter for temporary drains */
	flags_type stat_los;	/* Amount of temporary drains */

	flags_type intrinsic;  /* All the intrinsic powers as calculated by calc_bonus */

	flags_type resists;   /* Summary of all resistances */
	flags_type absorbs;   /* Summary of all absorbs */

	flags_type skill_bonuses;   /* Bonus to skill levels */

	s16b dis_to_h;		/* Known bonus to hit */
	s16b dis_to_d;		/* Known bonus to dam */
	s16b dis_to_a;		/* Known bonus to ac */

	s16b dis_ac;		/* Known base ac */

	s16b to_l;                      /* Bonus to life */
	s16b to_m;                      /* Bonus to mana */
	s16b to_s;                      /* Bonus to spell */
	s16b to_h;			/* Bonus to hit */
	s16b to_d;			/* Bonus to dam */
	s16b to_a;			/* Bonus to ac */

	s16b ac;			/* Base ac */

	byte antimagic;         /* Power of the anti magic field */
	byte antimagic_dis;     /* Radius of the anti magic field */

	s16b see_infra;		/* Infravision range */

	s16b skill_dis;		/* Skill: Disarming */
	s16b skill_dev;		/* Skill: Magic Devices */
	s16b skill_sav;		/* Skill: Saving throw */
	s16b skill_stl;		/* Skill: Stealth factor */
	s16b skill_thn;		/* Skill: To hit (normal) */
	s16b skill_thb;		/* Skill: To hit (shooting) */
	s16b skill_tht;		/* Skill: To hit (throwing) */

	s16b num_blow;		/* Number of blows */
	s16b num_fire;		/* Number of shots */

	byte throw_mult;	/* Multiplier for throw damage */

	s16b extra_blows;
	s16b extra_shots;

	byte tval_xtra;		/* Correct xtra tval */

	s16b pspeed;		/* Current global speed */
	flags_type speeds;      /* Specific speeds, indexed by skills */

	u32b mimic_extra;       /* Mimicry powers use that */
	u32b antimagic_extra;   /* Antimagic powers */
	u32b druid_extra;       /* Druid powers */
	u32b druid_extra2;      /* Druid powers */
	u32b druid_extra3;      /* Druid powers */
	u32b music_extra;       /* Music songs */
	u32b music_extra2;      /* Music songs */
	u32b necro_extra;       /* Necro powers */
	u32b necro_extra2;       /* Necro powers */

	u32b race_extra1;       /* Variable for race */
	u32b race_extra2;       /* Variable for race */
	u32b race_extra3;       /* Variable for race */
	u32b race_extra4;       /* Variable for race */
	u32b race_extra5;       /* Variable for race */
	u32b race_extra6;       /* Variable for race */
	u32b race_extra7;       /* Variable for race */

	s16b dodge_chance;      /* Dodging chance */

	u32b maintain_sum;      /* Do we have partial summons */

	cptr mimic_name;

	char tactic;                  /* from 128-4 extremely coward to */
	                              /* 128+4 berserker */
	char movement;                /* base movement way */

	s16b companion_killed;  /* Number of companion death */

	bool no_mortal;         /* Fated to never die by the hand of a mortal being */

	bool black_breath;      /* The Tolkien's Black Breath */

	bool precognition;      /* Like the cheat mode */

	/*** Pet commands ***/
	byte pet_follow_distance; /* Length of the imaginary "leash" for pets */
	byte pet_open_doors;      /* flag - allow pets to open doors */
	byte pet_pickup_items;    /* flag - allow pets to pickup items */

	s16b control;                   /* Controlled monster */
	byte control_dir;               /* Controlled monster */

	/*** Body changing variables ***/
	monster_type body_monster; /* In which body is the player */

	flags_type extra_body_parts;      /* Various body modifiers */

	/* Powers */
	flags_type powers;		/* Actual powers */
	flags_type powers_mod;		/* Intrinsinc powers */

	/* Skills */
	s16b skill_points;
	s16b skill_last_level;  /* Prevents gaining skills by losing level and regaining them */
	s16b combat_style;       /* How are  */
	s16b use_piercing_shots; /* for archery */

	/* Help */
	help_info help;

	flags_type flags;       /* flags */

	/*** Temporary fields ***/

	bool did_nothing;               /* True if the last action wasnt a real action */
	bool leaving;                   /* True if player is leaving */
};


/* Powers - used by Mindcrafters and Necromancers */
typedef struct magic_power magic_power;

struct magic_power
{
	s32b     min_lev;
	s32b     mana_cost;
	s32b     fail;
	cptr    name;
	cptr    desc;
};

/* Border */
typedef struct border_type border_type;

struct border_type
{
	byte 	north[MAX_WID];
	byte 	south[MAX_WID];
	byte 	east[MAX_HGT];
	byte 	west[MAX_HGT];
	byte	north_west;
	byte	north_east;
	byte	south_west;
	byte	south_east;
};

/* Define monster generation rules */
typedef struct rule_type rule_type;
struct rule_type
{
	byte mode;                      /* Mode of combination of the monster flags */
	byte percent;                   /* Percent of monsters affected by the rule */

	flags_type flags;       /* flags */
	flags_type spells;       /* flags */

	char r_char[5];                 /* Monster race allowed */
};



/*
 * A structure describing a wilderness area
 * with a terrain, a town or a dungeon entrance
 */
typedef struct wilderness_type_info wilderness_type_info;

struct wilderness_type_info
{
	cptr    name;                   /* Name (offset) */
	cptr    text;                   /* Text (offset) */
	u16b    entrance;               /* Which town is there(<1000 i's a town, >=1000 it a dungeon) */
	s32b	wild_x;			/* Map coordinates (backed out while parsing map) */
	s32b	wild_y;
	byte    road;                   /* Flags of road */
	s16b    level;                  /* Difficulty level */
	flags_type flags;       /* flags */
	byte    feat;                   /* The feature of f_info.txt that is used to allow passing, ... and to get a char/color/graph */
	byte    terrain_idx;            /* Terrain index(defined in defines.h) */

	char	d_char;

	bool 	rules_set;
	byte 	rule_percents[100];        /* Flat rule percents */
	rule_type rules[5];             /* Monster generation rules */

	byte    terrain[MAX_WILD_TERRAIN];/* Feature types for the plasma generator */

	cptr load_script;
};

/*
 * A structure describing a wilderness map
 */
typedef struct wilderness_map wilderness_map;

struct wilderness_map
{
	s32b       feat;     /* Wilderness feature */
	s16b       mimic;    /* Wilderness feature */
	u32b       seed;     /* Seed for the RNG */
	u16b       entrance; /* Entrance for dungeons */

	bool       known;    /* Is it seen by the player ? */
	flags_type flags;    /* Persistant per-grid flags */
};

/*
 * A structure describing a town with
 * stores and buildings
 */
typedef struct town_type town_type;
struct town_type
{
	cptr name;
	u32b seed; /* Seed for RNG */
	flags_type stores;      /* The stores [max_st_idx] */
	byte numstores;

	byte flags;             /* Town flags */
	/* Left this for the sake of compatibility */
	bool stocked;           /* Is the town actualy stocked ? */

	bool destroyed;         /* Is the town destroyed? */
};

/*
 A structure for deity information.
 */
typedef struct deity_type deity_type;
struct deity_type
{
	cptr name;
	char desc[10][80];
};

/* A structure for tactics */
typedef struct tactic_info_type tactic_info_type;

struct tactic_info_type
{
	s16b to_hit;
	s16b to_dam;
	s16b to_ac;
	s16b to_stealth;
	s16b to_disarm;
	s16b to_saving;
	cptr name;
};

/* A structure to describe a random artifact. */
typedef struct random_artifact random_artifact;

struct random_artifact
{
	char name_full[80];     /* Full name for the artifact */
	char name_short[80];    /* Un-Id'd name */
	byte level;             /* Level of the artifact */
	byte attr;              /* Color that is used on the screen */
	u32b cost;              /* Object's value */
	byte activation;        /* Activation. */
	s16b timeout;           /* Timeout. */
	byte generated;         /* Does it exist already? */
};

/* A structure to describe an activation. */
typedef struct activation activation;

struct activation
{
	char desc[80];          /* Desc of the activation */
	u32b cost;              /* costs value */
	s16b spell;             /* Spell. */
};

/* A structure to describe a music. */
typedef struct music music;

struct music
{
	char desc[80];          /* Desc of the music */
	s16b music;             /* Music. */
	s16b dur;               /* Duration(if any) */
	s16b init_recharge;     /* Minimal recharge time */
	s16b turn_recharge;     /* Recharge time for each more turn */
	byte min_inst;          /* Minimum instrument for the music */
	byte rarity;            /* Rarity of the music(use 100 to unallow to be randomly generated) */
};

/* A structure to describe the fate of the player */
typedef struct fate fate;

struct fate
{
	byte fate;      /* Which fate */
	byte level;     /* On which level */
	byte serious;   /* Is it sure? */
	s16b o_idx;     /* Object to find */
	s16b e_idx;     /* Ego-Item to find */
	s16b a_idx;     /* Artifact to find */
	s16b v_idx;     /* Vault to find */
	s16b r_idx;     /* Monster to find */
	s16b count;     /* Number of things */
	s16b time;      /* Turn before */
	bool know;      /* Has it been predicted? */
	bool icky;	/* Hackish runtime-only flag */
};

/* A structure for movements */
typedef struct move_info_type move_info_type;

struct move_info_type
{
	s16b to_speed;
	s16b to_search;
	s16b to_stealth;
	s16b to_percep;
	cptr name;
};

/* A structure for the != dungeon types */
typedef struct dungeon_info_type dungeon_info_type;
struct dungeon_info_type
{
	cptr name;                      /* Name */
	cptr text;                      /* Description */
	char short_name[3];             /* Short name */

	cptr generator;                 /* Name of the level generator */

	s16b floor1;                    /* Floor tile 1 */
	byte floor_percent1[2];         /* Chance of type 1 */
	s16b floor2;                    /* Floor tile 2 */
	byte floor_percent2[2];         /* Chance of type 2 */
	s16b floor3;                    /* Floor tile 3 */
	byte floor_percent3[2];         /* Chance of type 3 */
	s16b outer_wall;                /* Outer wall tile */
	s16b inner_wall;                /* Inner wall tile */
	s16b fill_type1;                /* Cave tile 1 */
	byte fill_percent1[2];          /* Chance of type 1 */
	s16b fill_type2;                /* Cave tile 2 */
	byte fill_percent2[2];          /* Chance of type 2 */
	s16b fill_type3;                /* Cave tile 3 */
	byte fill_percent3[2];          /* Chance of type 3 */
	byte fill_method;				/* Smoothing parameter for the above */

	s16b mindepth;                  /* Minimal depth */
	s16b maxdepth;                  /* Maximal depth */

	byte next;                      /* The next part of the main dungeon */
	byte min_plev;                  /* Minimal plev needed to enter -- it's an anti-cheating mesure */

	s16b min_m_alloc_level;          /* Minimal number of monsters per level */
	s16b max_m_alloc_chance;         /* There is a 1/max_m_alloc_chance chance per round of creating a new monster */

	flags_type flags;       /* flags */

	s16b size_x, size_y;             /* Desired numers of panels */

	byte rule_percents[100];        /* Flat rule percents */
	rule_type rules[5];             /* Monster generation rules */

	s16b ix, iy, ox, oy;             /* Wilderness coordinates of the entrance/output of the dungeon */

	s16b d_dice[4];                  /* Number of dices */
	s16b d_side[4];                  /* Number of sides */
	s16b d_frequency[4];             /* Frequency of damage (1 is the minimum) */
	s16b d_type[4];                  /* Type of damage */

	s16b t_idx[TOWN_DUNGEON];       /* The towns */
	s16b t_level[TOWN_DUNGEON];     /* The towns levels */
	s16b t_num;                     /* Number of towns */

	cptr load_script;               /* Script to execute upon loading from raw file */
};

/* For level gaining artifacts, artifact creation, ... */
typedef struct flags_group flags_group;
struct flags_group
{
	char name[30];          /* Name */
	byte color;             /* Color */

	byte price;             /* Price to "buy" it */

	flags_type flags;       /* flags */
};

/* For powers(racial, class, mutation, artifacts, ... */
typedef struct power_type power_type;
struct power_type
{
	char *name;              /* Name */
	char *desc_text;         /* Text describing power */
	char *gain_text;         /* Text displayed on gaining the power */
	char *lose_text;         /* Text displayed on losing the power */

	byte level;             /* Min level */
	byte cost;              /* Mana/Life cost */
	byte stat;              /* Stat used */
	byte diff;              /* Difficulty */
};

/* Hooks */
typedef bool (*hook_type)(char *fmt);

/*
 * Structure for the "quests"
 */
typedef struct quest_type quest_type;

struct quest_type
{
	bool silent;

	bool dynamic_desc;      /* Do we need to ask a function to get the description ? */

	char name[40];          /* Quest name */

	char desc[10][80];      /* Quest desc */

	s16b status;            /* Is the quest taken, completed, finished? */

	s16b level;             /* Dungeon level */

	s16b *plot;             /* Which plot does it belongs to? */

	byte type;              /* Lua or C ? */

	bool (*init)(s32b q);    /* Function that takes care of generating hardcoded quests */

	s32b data[4];           /* Various datas used by the quests */
};
typedef struct random_quest random_quest;
struct random_quest
{
	byte type;              /* Type/number of monsters to kill(0 = no quest) */
	s16b r_idx;             /* Monsters to crush */
	bool done;              /* Done ? */
};

/* Monster powers for player uses */
typedef struct monster_power monster_power;
struct monster_power
{
	u32b    power;          /* Power RF?_xxx */
	cptr    name;           /* Name of it */
	s32b     mana;           /* Mana needed */
	bool    great;          /* Need the use of great spells */
};

/*
 * Between exit
 */
typedef struct between_exit between_exit;
struct between_exit
{
	s16b corresp;           /* Corresponding between gate */
	bool dungeon;           /* Do we exit in a dungeon or in the wild ? */

	s16b wild_x, wild_y;    /* Wilderness spot to land onto */
	s16b px, py;            /* Location of the map */

	s16b d_idx;             /* Dungeon to land onto */
	s16b level;
};

/*
 * A structure to hold "rolled" information
 */
typedef struct birther birther;
struct birther
{
	s16b sex;
	s16b race;
	s16b rmod;
	s16b pclass;
	s16b spec;

	byte quests;

	byte god;
	s32b grace;
	s32b god_favor;

	s16b age;
	s16b wt;
	s16b ht;
	s16b sc;

	s32b au;

	s16b stat[6];
	s16b luck;

	u32b weapon;

	char history[4][60];

	bool quick_ok;
};

typedef struct hooks_chain hooks_chain;
struct hooks_chain
{
	hook_type hook;
	cptr name;
	cptr script;
	byte type;
	hooks_chain *next;
};

typedef union hook_return hook_return;
union hook_return
{
	s32b num;
	cptr str;
	object_type *o_ptr;
	monster_type *m_ptr;
	monster_race *r_ptr;
	flags_type *flags;
	bool boolean;
};

/*
 * Forward declare
 */
typedef struct hist_type hist_type;

/*
 * Player background information
 */
struct hist_type
{
	s32b info;                              /* Textual History -- uses rp_text */

	byte roll;			        /* Frequency of this entry */
	s16b chart;                             /* Chart index */
	s16b next;                              /* Next chart index */
	byte bonus;			        /* Social Class Bonus + 50 */
};

/*
 * Item sets
 */
typedef struct __set_type_aux __set_type_aux;
struct __set_type_aux
{
	bool present;                   /* Is it actually wore ? */
	s16b a_idx;                     /* What artifact ? */
	flags_type flags[6];            /* flags */
};

typedef struct set_type set_type;
struct set_type
{
	cptr name;                              /* Name */
	cptr desc;                              /* Desc */

	byte num;                               /* Number of artifacts used */
	byte num_use;                           /* Number actually wore */

	__set_type_aux arts[6];				/* the various items */

	cptr load_script;
};

/* A structure for CLI commands. */
typedef struct cli_comm cli_comm;
struct cli_comm
{
	cptr comm;	/* Extended name of the command. */
	cptr descrip;	/* Description of the command. */
	s16b key;	/* Key to convert command to. */
};

/*
 * Skills !
 */
typedef struct skill_type skill_type;
struct skill_type
{
	cptr name;                              /* Name */
	cptr desc;                              /* Description */
	cptr action_desc[MAX_SKILL_ACTION];     /* Action Description */

	s16b action_mkey[MAX_SKILL_ACTION];     /* Action do to */

	s32b i_value;                           /* Actual value */
	s32b i_mod;                             /* Modifier(1 skill point = modifier skill) */

	s32b value;                             /* Actual value */
	s32b mod;                               /* Modifier(1 skill point = modifier skill) */
	s16b rate;                              /* Modifier decreasing rate */

	u32b uses;                             	/* Number of times used */

	flags_type action;                      /* List of actions against other skills */

	s16b father;                            /* Father in the skill tree */
	bool dev;                               /* Is the branch developped ? */
	s16b order;                             /* Order in the tree */
	bool hidden;                            /* Innactive */

	flags_type flags;       /* flags */

	cptr load_script;
};

/*
 * Timers
 */
typedef struct timer_type timer_type;
struct timer_type
{
	timer_type *next;       /* The next timer in the list */

	bool enabled;           /* Is it currently counting? */

	s32b delay;             /* Delay between activations */
	s32b countdown;         /* The current number of turns passed, when it reaches delay it fires */

	cptr callback;          /* The lua function to call upon firing(no C callback yet .. maybe) */
};

/*
 * This is for lua functions that need to pass table to c functions
 */
typedef struct list_type list_type;
struct list_type
{
	cptr *list;
};

/*
 * Abilities
 */
typedef struct ability_type ability_type;
struct ability_type
{
	cptr name;                              /* Name */
	cptr desc;                              /* Description */
	cptr action_desc[MAX_SKILL_ACTION];     /* Action Description */

	s16b action_mkey[MAX_SKILL_ACTION];     /* Action do to */

	s16b cost;                              /* Skill points cost */

	bool hidden;				/* If it actually available to buy ? */
	bool acquired;                          /* Has the player actually got it ? */

	/* Prereqs */
	s16b skills[10];                	/* List of prereq skills(10 max) */
	s16b skill_levels[10];                  /* List of prereq skills(10 max) */
	s16b stat[6];                		/* List of prereq stats */
	s16b need_abilities[10];              	/* List of prereq abilities(10 max) */
	s16b forbid_abilities[10];		/* List of forbidden abilities(10 max) */
	s32b allow;

	cptr load_script;
};

/*
 * Item slots
 */
typedef struct inventory_slot_type inventory_slot_type;
struct inventory_slot_type
{
	cptr desc;
	cptr name;
	cptr describe;
	cptr wear;
	cptr takeoff;
};

/*
 * A generic buffer(can contain lua bytecode for example) that can be
 * put in a flagset
 */
typedef struct buffer_type buffer_type;
struct buffer_type
{
	u32b len;
	char *buf;
};
