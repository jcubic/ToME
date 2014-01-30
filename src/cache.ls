/*
 * This file tells the gel parser the various fields
 * of the various structures to generate caching and lua loading
 * functions for.
 * SAVE: lines have the following format:
 * SAVE:array_name[array_max_index]=>file
 */

struct ability_type
{
	cptr name;                              /* Name */
	cptr desc;                              /* Description */
	cptr action_desc[MAX_SKILL_ACTION];     /* Action Description */

	s16b action_mkey[MAX_SKILL_ACTION];     /* Action do to */

	s16b cost;                              /* Skill points cost */

	/* Prereqs */
	s16b skills[10];                	/* List of prereq skills(10 max) */
	s16b skill_levels[10];                  /* List of prereq skills(10 max) */
	s16b stat[6];                		/* List of prereq stats */
	s16b need_abilities[10];              	/* List of prereq abilities(10 max) */
	s16b forbid_abilities[10];		/* List of forbidden abilities(10 max) */
	s32b allow;

	cptr load_script;
};
SAVE:ab_info[max_ab_idx]=>player/abilities

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

	byte dd;
	byte ds;		/* Damage dice/sides */

	s32b weight;            /* Weight */

	s32b cost;			/* Object "base cost" */

	flags_type flags;       /* flags */

	byte locale[4];		/* Allocation level(s) */
	byte chance[4];		/* Allocation chance(s) */

	byte level;			/* Level */


	byte d_attr;		/* Default object attribute */
	char d_char;		/* Default object character */

	byte btval;                     /* Become Object type */
	byte bsval;                     /* Become Object sub type */

	s16b power;                     /* Power granted(if any) */

	cptr load_script;               /* Script to execute upon loading from raw file */
};
SAVE:k_info[max_k_idx]=>items/items

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
SAVE:ow_info[max_ow_idx]=>stores/owners

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
SAVE:ba_info[max_ba_idx]=>stores/actions

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
SAVE:st_info[max_st_idx]=>stores/stores

struct skill_type
{
	cptr name;                              /* Name */
	cptr desc;                              /* Description */
	cptr action_desc[MAX_SKILL_ACTION];     /* Action Description */

	s16b action_mkey[MAX_SKILL_ACTION];     /* Action do to */

	flags_type action;                      /* List of actions against other skills */

	s16b father;                            /* Father in the skill tree */
	s16b order;                             /* Order in the tree */

	flags_type flags;       /* flags */

	cptr load_script;
};
SAVE:s_info[max_s_idx]=>player/skills
FILE_LOAD_SCRIPT

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

	cptr load_script;
};
SAVE:r_info[max_r_idx]=>monsters/races

struct rule_type
{
	byte mode;                      /* Mode of combination of the monster flags */
	byte percent;                   /* Percent of monsters affected by the rule */

	flags_type flags;       /* flags */
	flags_type spells;       /* flags */

	char r_char[5];                 /* Monster race allowed */
};
SUBSTRUCT

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

	byte min_plev;                  /* Minimal plev needed to enter -- it's an anti-cheating mesure */

	s16b min_m_alloc_level;          /* Minimal number of monsters per level */
	s16b max_m_alloc_chance;         /* There is a 1/max_m_alloc_chance chance per round of creating a new monster */

	flags_type flags;       /* flags */

	s16b size_x;
	s16b size_y;             /* Desired numers of panels */

	rule_type rules[5];             /* Monster generation rules */
	byte rule_percents[100];        /* Flat rule percents */

	s16b ix;
	s16b iy;
	s16b ox;
	s16b oy;             /* Wilderness coordinates of the entrance/output of the dungeon */

	s16b d_dice[4];                  /* Number of dices */
	s16b d_side[4];                  /* Number of sides */
	s16b d_frequency[4];             /* Frequency of damage (1 is the minimum) */
	s16b d_type[4];                  /* Type of damage */

	cptr load_script;               /* Script to execute upon loading from raw file */
};
SAVE:d_info[max_d_idx]=>dungeon/dungeons

struct feature_type
{
	cptr name;              /* Name (offset) */
	cptr text;              /* Text (offset) */
	cptr block;             /* Text for blocking */

	s32b can_enter;         /* A lua function that allows or no passage */

	s16b mimic;             /* Feature to mimic */

	flags_type flags;       /* flags */

	byte d_attr;		/* Default feature attribute */
	char d_char;		/* Default feature character */

	byte shimmer[7];        /* Shimmer colors */

	s16b d_dice[4];                  /* Number of dices */
	s16b d_side[4];                  /* Number of sides */
	s16b d_frequency[4];             /* Frequency of damage (1 is the minimum) */
	s16b d_type[4];                  /* Type of damage */

	s16b priority;          /* Drawing priority */

	cptr load_script;
};
SAVE:f_info[max_f_idx]=>dungeon/features
LOAD_CORE_DEFAULT:features.lua

/*
 * Player racial info
 */
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

	flags_type oflags[PY_MAX_LEVEL + 1];       /* flags */

	flags_type skill_base;
	flags_type skill_mod;

	flags_type objects;	/* Starting objects */

	flags_type abilities;                /* Abilities to be gained by level(doesnt take prereqs in account) */

	cptr load_script;
};
SAVE:descriptor_info[max_dp_idx]=>player/descriptors

/* Vaults */
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
SAVE:v_info[max_v_idx]=>dungeon/vaults

/* Set items */
struct __set_type_aux
{
	s16b a_idx;                     /* What artifact ? */
	flags_type flags[6];            /* flags */
};
SUBSTRUCT

struct set_type
{
	cptr name;                              /* Name */
	cptr desc;                              /* Desc */

	byte num;                               /* Number of artifacts used */

	__set_type_aux arts[6];				/* the various items */

	cptr load_script;
};
SAVE:set_info[max_set_idx]=>items/sets

/* Monster ego */
struct monster_ego
{
	cptr name;				/* Name (offset) */
	bool before;                            /* Display ego before or after */

	flags_type blow;           /* blows per round */
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

	char r_char[5];                 /* Monster race allowed */
	char nr_char[5];                /* Monster race not allowed */

	cptr load_script;
};
SAVE:re_info[max_re_idx]=>monsters/egos

/* Ego items */
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

	byte level;			/* Minimum level */
	s16b allocation;        /* Allocation chance */

	s16b max_to_h;          /* Maximum to-hit bonus */
	s16b max_to_d;          /* Maximum to-dam bonus */
	s16b max_to_a;          /* Maximum to-ac bonus */

	u32b base_value_pct;    /* Item's base value is X% after ego applied */
	u32b value_add;         /* Value ego adds to the item  */

	byte rar[MAX_EGO_FLAG_GROUPS];
	flags_type flags[MAX_EGO_FLAG_GROUPS];       /* flags */

	flags_type need_flags;       /* flags */
	flags_type forbid_flags;       /* flags */

	s16b power;                     /* Power granted(if any) */

	cptr load_script;
};
SAVE:e_info[max_e_idx]=>items/egos

/* Artifacts */
struct artifact_type
{
	cptr name;			/* Name (offset) */
	cptr unknown_name;	/* Name displayed when unaware */
	cptr text;			/* Text (offset) */

	byte tval;			/* Artifact type */
	byte sval;			/* Artifact sub type */

	s16b d_attr;		/* Default attribute (-1 for same as base object) */
	s16b x_attr;		/* Desired attribute (-1 for same as base object) */

	s16b to_h;			/* Bonus to hit */
	s16b to_d;			/* Bonus to damage */
	s16b to_a;			/* Bonus to armor */

	s16b ac;			/* Base armor */

	byte dd;
	byte ds;		/* Damage when hits */

	s16b weight;		/* Weight */

	s32b cost;			/* Artifact "cost" */

	flags_type flags;       /* flags */

	byte level;			/* Artifact level */
	byte rarity;		/* Artifact rarity */

	s16b power;                     /* Power granted(if any) */

	cptr load_script;
};
SAVE:a_info[max_a_idx]=>items/artifacts

/* Wilderness features */
struct wilderness_type_info
{
	cptr    name;                   /* Name (offset) */
	cptr    text;                   /* Text (offset) */
	u16b    entrance;               /* Which town is there(<1000 i's a town, >=1000 it a dungeon) */
	s32b	wild_x;			/* Map coordinates (backed out while parsing map) */
	s32b	wild_y;
	byte    road;                   /* Flags of road */
	s16b     level;                  /* Difficulty level */
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
SAVE:wf_info[max_wf_idx]=>wild/terrain
AFTER_LOAD_EXEC:init_wf_info_wildc2i

/* Randarts */
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
SAVE:ra_info[max_ra_idx]=>items/randarts

/* DEBUG */
