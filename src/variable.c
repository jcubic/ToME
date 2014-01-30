/* File: variable.c */

/* Purpose: Angband variables */

/*
 * Copyright (c) 1989 James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"


/*
 * Hack -- Link a copyright message into the executable
 */
cptr copyright[6] =
{
	"Copyright (c) 1989 James E. Wilson, Robert A. Keoneke",
	"Copyright (c) 1998 DarkGod",
	"",
	"This software may be copied and distributed for educational, research,",
	"and not for profit purposes provided that this copyright and statement",
	"are included in all such copies."
};

s32b max_macrotrigger = 0;
char *macro_template = NULL;
char *macro_modifier_chr;
char *macro_modifier_name[MAX_MACRO_MOD];
char *macro_trigger_name[MAX_MACRO_TRIG];
char *macro_trigger_keycode[2][MAX_MACRO_TRIG];

/*
 * Executable version
 */
byte version_major;
byte version_minor;
byte version_patch;
byte version_extra = VERSION_EXTRA;

/*
 * Savefile version
 */
byte sf_major; 			/* Savefile's "version_major" */
byte sf_minor; 			/* Savefile's "version_minor" */
byte sf_patch; 			/* Savefile's "version_patch" */
byte sf_extra; 			/* Savefile's "version_extra" */
u32b vernum;

/*
 * Savefile information
 */
u32b sf_xtra; 			/* Operating system info */
u32b sf_when; 			/* Time when savefile created */
u16b sf_lives; 			/* Number of past "lives" with this file */
u16b sf_saves; 			/* Number of "saves" during this life */

/*
 * Run-time arguments
 */
bool arg_fiddle; 			/* Command arg -- Request fiddle mode */
bool arg_wizard; 			/* Command arg -- Request wizard mode */
bool arg_sound; 				/* Command arg -- Request special sounds */
bool arg_graphics; 			/* Command arg -- Request graphics mode */
bool arg_force_original; 	/* Command arg -- Request original keyset */
bool arg_force_roguelike; 	/* Command arg -- Request roguelike keyset */
bool arg_bigtile = FALSE; 	/* Command arg -- Request big tile mode */

/*
 * Various things
 */

bool character_generated = FALSE; 	/* The character exists */
bool character_dungeon; 		/* The character has a dungeon */
bool character_loaded; 		/* The character was loaded from a savefile */
bool character_saved; 		/* The character was just saved to a savefile */

bool character_icky; 		/* The game is in an icky full screen mode */
bool character_xtra; 		/* The game is in an icky startup mode */

bool ignore_numlock;		/* Flag to ignore the numlock key when necessary */

u32b seed_flavor; 		/* Hack -- consistent object colors */
u32b seed_town; 			/* Hack -- consistent town layout */
u32b seed_dungeon;               /* Simulate persisten dungeons */

s32b command_cmd; 		/* Current "Angband Command" */

s16b command_arg; 		/* Gives argument of current command */
s16b command_rep; 		/* Gives repetition of current command */
s16b command_dir; 		/* Gives direction of current command */

s16b command_see; 		/* See "cmd1.c" */
s16b command_wrk; 		/* See "cmd1.c" */

s16b command_new; 		/* Command chaining from inven/equip view */
bool command_new_do_keymap = FALSE;


s32b energy_use;                 /* Energy use this turn */

bool create_up_stair; 	/* Auto-create "up stairs" */
bool create_down_stair; 	/* Auto-create "down stairs" */

bool create_up_shaft;  /* Auto-create "up shaft" */
bool create_down_shaft;        /* Auto-create "down shaft" */

bool msg_flag; 			/* Used in msg_print() for "buffering" */

bool alive; 				/* True if game is running */

bool death; 				/* True if player has died */

s16b running; 			/* Current counter for running, if any */

s16b cur_hgt; 			/* Current dungeon height */
s16b cur_wid; 			/* Current dungeon width */
s16b dun_level; 			/* Current dungeon level */
s16b old_dun_level;              /* Old dungeon level */
s16b num_repro; 			/* Current reproducer count */
s16b object_level; 		/* Current object creation level */
s16b monster_level; 		/* Current monster creation level */

s32b turn; 				/* Current game turn */
s32b old_turn; 			/* Turn when level began (feelings) */

bool wizard; 			/* Is the player currently in Wizard mode? */

bool use_sound; 			/* The "sound" mode is enabled */
bool use_graphics; 		/* The "graphics" mode is enabled */
bool use_bigtile = FALSE;
byte graphics_mode; 		/* Current graphics mode */

u16b total_winner; 		/* Semi-Hack -- Game has been won */
u16b has_won;               /* Semi-Hack -- Game has been won */

u16b panic_save; 		/* Track some special "conditions" */
u16b noscore; 			/* Track various "cheating" conditions */

s16b signal_count; 		/* Hack -- Count interupts */

bool inkey_base; 		/* See the "inkey()" function */
bool inkey_xtra; 		/* See the "inkey()" function */
bool inkey_scan; 		/* See the "inkey()" function */
bool inkey_flag; 		/* See the "inkey()" function */
bool inkey_dir; 		/* See the "inkey_aux()" function */

s16b coin_type; 			/* Hack -- force coin type */

bool opening_chest; 		/* Hack -- prevent chest generation */

bool shimmer_monsters;           /* Hack -- optimize multi-hued monsters */
bool shimmer_objects;            /* Hack -- optimize multi-hued objects */

bool repair_monsters; 	/* Hack -- optimize detect monsters */
bool repair_objects; 	/* Hack -- optimize detect objects */

s16b inven_nxt; 			/* Hack -- unused */
bool hack_mind;
s32b artifact_bias;
bool is_autosave = FALSE;

s16b inven_cnt; 			/* Number of items in inventory */
s16b equip_cnt; 			/* Number of items in equipment */

s16b m_cnt = 0; 			/* Number of live monsters */

s16b hack_m_idx = 0; 	/* Hack -- see "process_monsters()" */
s16b hack_m_idx_ii = 0;
bool multi_rew = FALSE;
char summon_kin_type;    /* Hack, by Julian Lighton: summon 'relatives' */

s32b leaving_quest = 0;



/*
 * Hack - the destination file for text_out_to_file.
 */
PHYSFS_file *text_out_file = NULL;


/*
 * Hack -- function hook to output (colored) text to the
 * screen or to a file.
 */
void (*text_out_hook)(byte a, cptr str) = text_out_to_screen;


/*
 * Hack -- Where to wrap the text when using text_out().  Use the default
 * value (for example the screen width) when 'text_out_wrap' is 0.
 */
s32b text_out_wrap = 0;


/*
 * Hack -- Indentation for the text when using text_out().
 */
s32b text_out_indent = 0;


/*
 * The "highscore" file descriptor, if available.
 */
PHYSFS_file *highscore_fd = NULL;


/*
 * Software options (set via the '=' command).  See "tables.c"
 */


/* Option Set 1 -- User Interface */

bool rogue_like_commands; 	/* Rogue-like commands */
bool quick_messages; 		/* Activate quick messages */
bool other_query_flag; 		/* Prompt for various information */
bool carry_query_flag; 		/* Prompt before picking things up */
bool use_old_target; 		/* Use old target by default */
bool always_pickup;              /* Pick things up by default */
bool prompt_pickup_heavy;        /* Don't pick up the corpses */
bool always_repeat;              /* Repeat obvious commands */
bool depth_in_feet;              /* Show dungeon level in feet */

bool stack_force_notes; 		/* Merge inscriptions when stacking */
bool stack_force_costs; 		/* Merge discounts when stacking */

bool show_labels; 			/* Show labels in object listings */
bool show_weights; 			/* Show weights in object listings */
bool show_choices; 			/* Show choices in certain sub-windows */
bool show_details; 			/* Show details in certain sub-windows */

bool ring_bell; 				/* Ring the bell (on errors, etc) */
bool use_color; 				/* Use color if possible (slow) */
bool can_redefine_color = TRUE;			/* Terminal can redefine colors */

bool show_inven_graph; 		/* Show graphics in inventory */
bool show_equip_graph; 		/* Show graphics in equip list */
bool show_store_graph; 		/* Show graphics in store */
bool keypad_autorun;		/* Flag to set keypad to auto-run */



/* Option Set 2 -- Disturbance */

bool find_ignore_stairs; 	/* Run past stairs */
bool find_ignore_doors; 		/* Run through open doors */
bool find_cut; 				/* Run past known corners */
bool find_examine; 			/* Run into potential corners */

bool disturb_move; 			/* Disturb whenever any monster moves */
bool disturb_near; 			/* Disturb whenever viewable monster moves */
bool disturb_panel; 			/* Disturb whenever map panel changes */
bool disturb_detect; 		/* Disturb whenever leaving trap-detected area */
bool disturb_state; 			/* Disturn whenever player state changes */
bool disturb_minor; 			/* Disturb whenever boring things happen */
bool disturb_other; 			/* Disturb whenever various things happen */

bool alert_hitpoint; 		/* Alert user to critical hitpoints */
bool alert_failure; 		/* Alert user to various failures */
bool last_words; 		/* Get last words upon dying */
bool speak_unique; 		/* Speaking uniques + shopkeepers */
bool small_levels; 		/* Allow unusually small dungeon levels */
bool empty_levels; 		/* Allow empty 'arena' levels */
bool always_small_level;         /* Small levels */
#if 0 /* It's controlled by insanity -- pelpel */
bool flavored_attacks;           /* Show silly messages when fighting */
#endif
bool player_symbols; 		/* Use varying symbols for the player char */
bool plain_descriptions; 	/* Plain object descriptions */
bool stupid_monsters; 		/* Monsters use old AI */
bool auto_destroy; 		/* Known worthless items are destroyed without confirmation */
bool confirm_stairs; 		/* Prompt before staircases... */
bool wear_confirm; 		/* Confirm before putting on known cursed items */
bool disturb_pets; 		/* Pets moving nearby disturb us */


/* Option Set 3 -- Game-Play */

bool auto_haggle; 			/* Auto-haggle in stores */

bool auto_scum; 				/* Auto-scum for good levels */

bool stack_allow_items; 		/* Allow weapons and armor to stack */
bool stack_allow_wands; 		/* Allow wands/staffs/rods to stack */

bool expand_look; 			/* Expand the power of the look command */
bool expand_list; 			/* Expand the power of the list commands */

bool view_perma_grids; 		/* Map remembers all perma-lit grids */
bool view_torch_grids; 		/* Map remembers all torch-lit grids */

bool monster_lite; 			/* Allow some monsters to carry light */

bool dungeon_align; 			/* Generate dungeons with aligned rooms */
bool dungeon_stair; 			/* Generate dungeons with connected stairs */

bool track_follow; 			/* Monsters follow the player */
bool track_target; 			/* Monsters target the player */

bool smart_learn; 			/* Monsters learn from their mistakes */
bool smart_cheat; 			/* Monsters exploit player weaknesses */


/* Option Set 4 -- Efficiency */

bool view_reduce_lite; 		/* Reduce lite-radius when running */
bool view_reduce_view; 		/* Reduce view-radius in town */

bool avoid_abort; 			/* Avoid checking for user abort */
bool avoid_shimmer; 			/* Avoid processing extra shimmering */
bool avoid_other; 			/* Avoid processing special colors */

bool flush_failure; 			/* Flush input on any failure */
bool flush_disturb; 			/* Flush input on disturbance */
bool flush_command; 			/* Flush input before every command */

bool fresh_before; 			/* Flush output before normal commands */
bool fresh_after; 			/* Flush output after normal commands */
bool fresh_message; 			/* Flush output after all messages */

bool compress_savefile; 		/* Compress messages in savefiles */

bool hilite_player; 			/* Hilite the player with the cursor */

bool view_lited_grid; 		/* Use special colors for torch-lit/dark grids */

/* Option set 5 -- Testing */

/* removed (JMLB) */

/* Cheating options */

bool cheat_peek; 		/* Peek into object creation */
bool cheat_hear; 		/* Peek into monster creation */
bool cheat_room; 		/* Peek into dungeon creation */
bool cheat_xtra; 		/* Peek into something else */
bool cheat_know; 		/* Know complete monster info */
bool cheat_live; 		/* Allow player to avoid death */


/* Special options */

byte hitpoint_warn; 		/* Hitpoint warning (0 to 9) */

byte delay_factor; 		/* Delay factor (0 to 9) */

bool autosave_l;         /* Autosave before entering new levels */
bool autosave_t;         /* Timed autosave */
s16b autosave_freq;      /* Autosave frequency */


/*
 * Dungeon variables
 */

s16b feeling; 			/* Most recent feeling */
s16b rating; 			/* Level's current rating */

bool good_item_flag; 		/* True if "Artifact" on this level */

bool closing_flag; 		/* Dungeon is closing */

/*
 * Dungeon size info
 */

s16b max_panel_rows, max_panel_cols;
s16b panel_row_min, panel_row_max;
s16b panel_col_min, panel_col_max;
s16b panel_col_prt, panel_row_prt;

/*
 * Dungeon graphics info
 * Why the first two are byte and the rest s16b???
 */
s16b feat_wall_outer; 	/* Outer wall of rooms */
s16b feat_wall_inner; 	/* Inner wall of rooms */
s16b floor_type[100]; 	/* Dungeon floor */
s16b fill_type[100]; 	/* Dungeon filler */

/*
 * Targetting variables
 */
s16b target_who;
s16b target_col;
s16b target_row;

/*
 * Health bar variable -DRS-
 */
s16b health_who;

/*
 * Monster race to track
 */
s16b monster_race_idx;
s16b monster_ego_idx;

/*
 * Object to track
 */
object_type *tracked_object;



/*
 * User info
 */
s32b player_uid;
s32b player_euid;
s32b player_egid;

/*
 * Current player's character name
 */
char player_name[128];

/*
 * Stripped version of "player_name"
 */
char player_base[128];

/*
 * What killed the player
 */
char died_from[80];

/*
 * Hack -- Textual "history" for the Player
 */
char history[4][60];

/*
 * Buffer to hold the current savefile name
 */
char savefile[1024];


/*
 * Array of grids lit by player lite (see "cave.c")
 */
s16b lite_n;
s16b lite_y[LITE_MAX];
s16b lite_x[LITE_MAX];

/*
 * Array of grids viewable to the player (see "cave.c")
 */
s16b view_n;
byte view_y[VIEW_MAX];
byte view_x[VIEW_MAX];
byte view_d[VIEW_MAX];

/*
 * Array of grids for use by various functions (see "cave.c")
 */
s16b temp_n;
byte temp_y[TEMP_MAX];
byte temp_x[TEMP_MAX];


/*
 * Number of active macros.
 */
s16b macro__num;

/*
 * Array of macro patterns [MACRO_MAX]
 */
cptr *macro__pat;

/*
 * Array of macro actions [MACRO_MAX]
 */
cptr *macro__act;

/*
 * Array of macro types [MACRO_MAX]
 */
bool *macro__cmd;

/*
 * Current macro action [1024]
 */
char *macro__buf;


/*
 * The number of quarks
 */
s16b quark__num;

/*
 * The pointers to the quarks [QUARK_MAX]
 */
cptr *quark__str;


/*
 * The next "free" index to use
 */
u16b message__next;

/*
 * The index of the oldest message (none yet)
 */
u16b message__last;

/*
 * The next "free" offset
 */
u16b message__head;

/*
 * The offset to the oldest used char (none yet)
 */
u16b message__tail;

/*
 * The array of offsets, by index [MESSAGE_MAX]
 */
u16b *message__ptr;

/*
 * The array of colors, by index [MESSAGE_MAX]
 */
byte *message__color;

/*
 * The array of type, by index [MESSAGE_MAX]
 */
byte *message__type;

/*
 * The array of message counts, by index [MESSAGE_MAX]
 */
u16b *message__count;

/*
 * The array of chars, by offset [MESSAGE_BUF]
 */
char *message__buf;


/*
 * The array of normal options
 */
u32b option_flag[8];
u32b option_mask[8];


/*
 * The array of window options
 */
flags_type window_flag[ANGBAND_TERM_MAX];


/*
 * The array of window pointers
 */
term *angband_term[ANGBAND_TERM_MAX];


/*
 * Standard window names
 */
char angband_term_name[ANGBAND_TERM_MAX][80] =
{
	"Main",
	"Term-1",
	"Term-2",
	"Term-3",
	"Term-4",
	"Term-5",
	"Term-6",
	"Term-7"
};


/*
 * Global table of color definitions
 */
byte angband_color_table[256][4] =
{
	{0x00, 0x00, 0x00, 0x00}, 	/* TERM_DARK */
	{0x00, 0xFF, 0xFF, 0xFF}, 	/* TERM_WHITE */
	{0x00, 0x80, 0x80, 0x80}, 	/* TERM_SLATE */
	{0x00, 0xFF, 0x80, 0x00}, 	/* TERM_ORANGE */
	{0x00, 0xC0, 0x00, 0x00}, 	/* TERM_RED */
	{0x00, 0x00, 0x80, 0x40}, 	/* TERM_GREEN */
	{0x00, 0x00, 0x00, 0xFF}, 	/* TERM_BLUE */
	{0x00, 0x80, 0x40, 0x00}, 	/* TERM_UMBER */
	{0x00, 0x40, 0x40, 0x40}, 	/* TERM_L_DARK */
	{0x00, 0xC0, 0xC0, 0xC0}, 	/* TERM_L_WHITE */
	{0x00, 0xFF, 0x00, 0xFF}, 	/* TERM_VIOLET */
	{0x00, 0xFF, 0xFF, 0x00}, 	/* TERM_YELLOW */
	{0x00, 0xFF, 0x00, 0x00}, 	/* TERM_L_RED */
	{0x00, 0x00, 0xFF, 0x00}, 	/* TERM_L_GREEN */
	{0x00, 0x00, 0xFF, 0xFF}, 	/* TERM_L_BLUE */
	{0x00, 0xC0, 0x80, 0x40}	/* TERM_L_UMBER */
};


#ifdef SUPPORT_GAMMA

/*
 * Gamma correction - gamma_val == (s32b)(256 / gamma)
 * The value of 0 means no gamma correction (== 1.0)
 */
u16b gamma_val;

#endif /* SUPPORT_GAMMA */


/*
 * Standard sound names
 */
char angband_sound_name[SOUND_MAX][16] =
{
	"",
	"hit",
	"miss",
	"flee",
	"drop",
	"kill",
	"level",
	"death",
	"study",
	"teleport",
	"shoot",
	"quaff",
	"zap",
	"walk",
	"tpother",
	"hitwall",
	"eat",
	"store1",
	"store2",
	"store3",
	"store4",
	"dig",
	"opendoor",
	"shutdoor",
	"tplevel",
	"scroll",
	"buy",
	"sell",
	"warn",
	"rocket",
	"n_kill",
	"u_kill",
	"quest",
	"heal",
	"x_heal",
	"bite",
	"claw",
	"m_spell",
	"summon",
	"breath",
	"ball",
	"m_heal",
	"atkspell",
	"evil",
	"touch",
	"sting",
	"crush",
	"slime",
	"wail",
	"winner",
	"fire",
	"acid",
	"elec",
	"cold",
	"illegal",
	"fail",
	"wakeup",
	"invuln",
	"fall",
	"pain",
	"destitem",
	"moan",
	"show",
	"unused",
	"explode",
};

/*
 * Number of grids used to display the dungeon (vertically).
 * Must be a multiple of 11, probably hard-coded to 22.
 */
s16b SCREEN_HGT = 22;

/*
 * Number of grids used to display the dungeon (horizontally).
 * Must be a multiple of 33, probably hard-coded to 66.
 */
s16b SCREEN_WID = 66;

/*
 * The array of "cave grids" [MAX_WID][MAX_HGT].
 * Not completely allocated, that would be inefficient
 * Not completely hardcoded, that would overflow memory
 */
cave_type *cave[MAX_HGT];

/*
 * The array of dungeon monsters
 */
flags_type monst_list;

/*
 * The array of to keep monsters
 */
flags_type keep_monst_list;


/*
 * Maximum number of towns
 */
u16b max_towns;
u16b max_real_towns;

/*
 * The towns [max_towns]
 */
town_type *town_info;

/*
 * The size of "alloc_race_table" (at most max_r_idx)
 */
s16b alloc_race_size;

/*
 * The entries in the "race allocator table"
 */
alloc_entry *alloc_race_table;


/*
 * Specify attr/char pairs for inventory items (by tval)
 * Be sure to use "index & 0x7F" to avoid illegal access
 */
byte tval_to_attr[128];
char tval_to_char[128];


/*
 * Keymaps for each "mode" associated with each keypress.
 */
cptr keymap_act[KEYMAP_MODES][256];



/*** Player information ***/

/*
 * Static player info record
 */
player_type p_body;

/*
 * Pointer to the player info
 */
player_type *p_ptr = &p_body;

/*
 * Calculated base hp values for player at each level,
 * store them so that drain life + restore life does not
 * affect hit points.  Also prevents shameless use of backup
 * savefiles for hitpoint acquirement.
 */
s16b *player_hp;

/*
 * Base experience levels, may be adjusted up for race and/or class
 */
s32b *player_exp;

/*
 * The vault generation arrays
 */
vault_type *v_info;

/*
 * The terrain feature arrays
 */
feature_type *f_info;

/*
 * The object kind arrays
 */
object_kind *k_info;

/*
 * The artifact arrays
 */
artifact_type *a_info;

/*
 * The item set arrays
 */
set_type *set_info;

/*
 * The ego-item arrays
 */
ego_item_type *e_info;

/*
 * The randart arrays
 */
header *ra_head;
randart_part_type *ra_info;
randart_gen_type ra_gen[30];

/*
 * The monster race arrays
 */
monster_race *r_info;

/*
 * The monster ego race arrays
 */
monster_ego *re_info;

/*
 * The dungeon types arrays
 */
dungeon_info_type *d_info;

/*
 * Player abilities arrays
 */
ability_type *ab_info;

/*
 * Player skills arrays
 */
header *s_head;
skill_type *s_info;
char *s_name;
char *s_text;

/*
 * Player sex array
 */
player_descriptor *descriptor_info;

/*
 * The wilderness features arrays
 */
wilderness_type_info *wf_info;
s32b wildc2i[256];

/*
 * The store/building types arrays
 */
store_info_type *st_info;
/* char *st_text; */

/*
 * The building actions types arrays
 */
store_action_type *ba_info;

/*
 * The owner types arrays
 */
owner_type *ow_info;

/*
 * The dungeon types arrays
 */
header *d_head;
dungeon_info_type *d_info;
char *d_name;
char *d_text;

/*
 * Hack -- The special Angband "System Suffix"
 * This variable is used to choose an appropriate "pref-xxx" file
 */
cptr ANGBAND_SYS = "xxx";

/*
 * Hack -- The special Angband "Keyboard Suffix"
 * This variable is used to choose an appropriate macro-trigger definition
 */
#ifdef JP
cptr ANGBAND_KEYBOARD = "JAPAN";
#else
cptr ANGBAND_KEYBOARD = "0";
#endif

/*
 * Hack -- The special ToME "Graphics Suffix"
 * This variable is used to choose an appropriate "graf-xxx" file
 */
cptr TOME_GRAPHICS = "8";

/*
 * T-Engine config dir
 */
cptr ENGINE_DIR;

/*
 * T-Engine config dir dependent path
 */
cptr ENGINE_DIR_DEP;

/*
 * T-Engine Version
 */
cptr ENGINE_VERSION;

/*
 * Path name: The main "game" directory
 * This variable is not actually used anywhere in the code
 */
cptr TENGINE_DIR;

/*
 * High score files (binary)
 * These files may be portable between platforms
 */
cptr TENGINE_DIR_SCORES;

/*
 * Bone files for player ghosts (ascii)
 * These files are portable between platforms
 */
cptr TENGINE_DIR_BONE;

/*
 * Cmovie files of entire games (ascii)
 * Apart from possible newline things, likely portable btw platforms
 */

cptr TENGINE_DIR_CMOV;

/*
 * Textual template files for the "*_info" arrays (ascii)
 * These files are portable between platforms
 */
cptr TENGINE_DIR_DATA;

/*
 * Textual dungeon level definition files
 * These files are portable between platforms
 */
cptr TENGINE_DIR_DNGN;

/*
 * Core lua system
 * These files are portable between platforms
 */
cptr TENGINE_DIR_ENGINE;

/*
 * Various extra files (ascii)
 * These files may be portable between platforms
 */
cptr TENGINE_DIR_FILE;

/*
 * Various font files (binary)
 * These files may be portable between platforms
 */
cptr TENGINE_DIR_FONT;

/*
 * Various graphics files (binary)
 * These files may be portable between platforms
 */
cptr TENGINE_DIR_GRAF;

/*
 * Help files (normal) for the online help (ascii)
 * These files are portable between platforms
 */
cptr TENGINE_DIR_HELP;

/*
 * Help files (spoilers) for the online help (ascii)
 * These files are portable between platforms
 */
cptr TENGINE_DIR_INFO;

/*
 * Modules, those subdirectories are half-mirrors of lib/
 */
cptr TENGINE_DIR_MODULES;

/*
 * Textual template files for the plot files (ascii)
 * These files are portable between platforms
 */
cptr TENGINE_DIR_NOTE;

/*
 * Patches, contains one subdir per patch with a patch.lua file
 * in it and a patch_init() function in it
 */
cptr TENGINE_DIR_PATCH;

/*
 * Default "preference" files (ascii)
 * These files are rarely portable between platforms
 */
cptr TENGINE_DIR_PREF;

/*
 * Binary image files for the "*_info" arrays (binary)
 * These files are not portable between platforms
 */
cptr TENGINE_DIR_RAW;

/*
 * Savefiles for current characters (binary)
 * These files are portable between platforms
 */
cptr TENGINE_DIR_SAVE;

/*
 * Scripts.
 * These files are portable between platforms
 */
cptr TENGINE_DIR_SCPT;

/*
 * User "preference" files (ascii)
 * These files are rarely portable between platforms
 */
cptr TENGINE_DIR_USER;

/*
 * Various extra files (binary)
 * These files are rarely portable between platforms
 */
cptr TENGINE_DIR_XTRA;

/*
 * Some variables values are created on the fly XXX XXX
 */

char pref_tmp_value[8];



/*
 * Total Hack -- allow all items to be listed (even empty ones)
 * This is only used by "do_cmd_inven_e()" and is cleared there.
 */
bool item_tester_full;


/*
 * Here is a "pseudo-hook" used during calls to "get_item()" and
 * "show_inven()" and "show_equip()", and the choice window routines.
 */
byte item_tester_tval;


/*
 * Here is a "hook" used during calls to "get_item()" and
 * "show_inven()" and "show_equip()", and the choice window routines.
 */
bool (*item_tester_hook)(object_type*, s32b);



/*
 * Current "comp" function for ang_sort()
 */
bool (*ang_sort_comp)(vptr u, vptr v, s32b a, s32b b);


/*
 * Current "swap" function for ang_sort()
 */
void (*ang_sort_swap)(vptr u, vptr v, s32b a, s32b b);



/*
 * Hack -- function hooks to restrict "get_mon_num_prep()" function
 */
bool (*get_mon_num_hook)(s32b r_idx);
bool (*get_mon_num2_hook)(s32b r_idx);


/*
 * Hack -- function hook to restrict "get_obj_num_prep()" function
 */
bool (*get_obj_num_hook)(s32b k_idx);

#ifdef ALLOW_EASY_OPEN /* TNB */
bool easy_open = TRUE;
#endif /* ALLOW_EASY_OPEN -- TNB */

#ifdef ALLOW_EASY_DISARM /* TNB */
bool easy_disarm = TRUE;
#endif /* ALLOW_EASY_DISARM -- TNB */

bool easy_tunnel = FALSE;

bool easy_inven = TRUE;

/*
 * Maximum size of the wilderness map
 */
u16b max_wild_x;
u16b max_wild_y;

/*
 * Wilderness map
 */
wilderness_map **wild_map;


/*
 * Maximum number of skills in s_info.txt
 */
u16b old_max_s_idx = 0;
u16b max_s_idx;

/*
 * Maximum number of abilities in ab_info.txt
 */
u16b max_ab_idx;

/*
 * Maximum number of monsters in r_info.txt
 */
u16b max_r_idx;

/*
 * Maximum number of ego monsters in re_info.txt
 */
u16b max_re_idx;

/*
 * Maximum number of items in k_info.txt
 */
u16b max_k_idx;

/*
 * Maximum number of vaults in v_info.txt
 */
u16b max_v_idx;

/*
 * Maximum number of terrain features in f_info.txt
 */
u16b max_f_idx;

/*
 * Maximum number of alchemist recipies in al_info.txt
 */
u16b max_al_idx;

/*
 * Maximum number of artifacts in a_info.txt
 */
u16b max_a_idx;

/*
 * Maximum number of ego-items in e_info.txt
 */
u16b max_e_idx;

/*
 * Maximum number of randarts in ra_info.txt
 */
u16b max_ra_idx;

/*
 * Maximum number of dungeon types in d_info.txt
 */
u16b max_d_idx;

/*
 * Maximum number of stores types in st_info.txt
 */
u16b max_st_idx;

/*
 * Item sets
 */
s16b max_set_idx = 1;

/*
 * Maximum number of players info in p_info.txt
 */
u16b max_dp_idx;

/*
 * Maximum number of actions types in ba_info.txt
 */
u16b max_ba_idx;

/*
 * Maximum number of owner types in ow_info.txt
 */
u16b max_ow_idx;

/*
 * Maximum number of monsters in the level
 */
u16b max_m_idx;

/*
 * Maximum number of traps in tr_info.txt
 */
u16b max_t_idx;

/*
 * Maximum number of wilderness features in wf_info.txt
 */
u16b max_wf_idx;

/*
 * Flags for initialization
 */
s32b init_flags;

/* No breeders */
u16b no_breeds;

/* Corpse decay (defaults to off) */
bool use_corpse_decay = 0;

/*
 * Random artifacts.
 */
random_artifact random_artifacts[MAX_RANDARTS];
/* These three used to be constants but now are set by modules */
s32b RANDART_WEAPON;
s32b RANDART_ARMOR;
s32b RANDART_JEWEL;

/*
 * Spell description
 */
bool info_spell = FALSE;
char spell_txt[50];


/*
 * Vanilla town.
 */
byte vanilla_town = FALSE;

/*
 * Which dungeon ?
 * (see d_info.txt)
 */
byte dungeon_type;
byte old_dungeon_type;
s16b *max_dlv;

/* The Doppleganger index in m_list */
s16b doppleganger;

/* Permanent dungeons ? */
bool permanent_levels;

/* Autoroler */
bool autoroll;

/* Point based */
bool point_based;

/* Maximize, preserve, special levels, ironman_rooms */
bool maximize, preserve, special_lvls, ironman_rooms;

/* In inventory option window, just erase the letters,
 * rather that displaying the list without the invalid
 * selections */
bool inventory_no_move;

/* Notes patch */
bool take_notes, auto_notes;

/*
 * Such an ugly hack ...
 */
bool *m_allow_special;
bool *k_allow_special;
bool *a_allow_special;

/*
 * Gives a random object to newly created characters
 */
bool rand_birth;

/*
 * Fast autoroller
 */
bool fast_autoroller;

/*
 * Which monsters are allowed ?
 */
bool joke_monsters;

/*
 * Center view
 */
bool center_player = FALSE;

/*
 * Plots
 */
s16b plots[MAX_PLOTS];

/*
 * Random quest
 */
random_quest random_quests[MAX_RANDOM_QUEST];

/*
 * Show exp left
 */
bool exp_need;

/*
 * Auto load old colors;
 */
bool autoload_old_colors;

/*
 * Special levels
 */
bool *special_lvl[MAX_DUNGEON_DEPTH];
bool generate_special_feeling = FALSE;

/*
 * Auto more
 */
bool auto_more;

/*
 * Dungeon flags
 */
flags_holder_type *dungeon_flags;

/*
 * The last character displayed
 */
birther previous_char;

/*
 * Race histories
 */
hist_type *bg;
s32b max_bg_idx;

/*
 * Powers
 */
s16b power_max = POWER_MAX_INIT;
power_type *powers_type;

/*
 * Variable savefile stuff
 */
s32b extra_savefile_parts = 0;

/*
 * Quests
 */
s16b max_q_idx = MAX_Q_IDX_INIT;
quest_type *quest;

/*
 * Display the player as a special symbol when in bad health ?
 */
bool player_char_health;


/*
 * Lasting spell effects
 */
s32b project_time = 0;
s16b project_time_speed = 0;
s32b project_time_effect = 0;
effect_type effects[MAX_EFFECTS];

/*
 * Projection range
 */
s32b project_range = 0;

/*
 * Display stats as linear
 */
bool linear_stats;

/*
 * Table of "cli" macros.
 */
cli_comm *cli_info;
s32b cli_total = 0;

/*
 * max_bact, only used so that lua scripts can add new bacts without worrying about the numbers
 */
s32b max_bact = 54;

/*
 * Ingame contextual help
 */
bool option_ingame_help = TRUE;

/*
 * Automatizer enabled status
 */
bool automatizer_enabled = FALSE;

/*
 * Location of the last teleportation thath affected the level
 */
s16b last_teleportation_y = -1;
s16b last_teleportation_x = -1;

/*
 * The current game module
 */
cptr game_module = NULL;
s32b VERSION_MAJOR;
s32b VERSION_MINOR;
s32b VERSION_PATCH;

/*
 * Some module info
 */
s32b max_plev = 50;

/*
 * Timers
 */
timer_type *gl_timers = NULL;

/*
 * The current spell cast(player or monster)
 */
s32b spell_caster = 0;

/*
 * Where to draw the map
 */
s16b ROW_MAP = 1;
s16b COL_MAP = 13;
s16b ROW_MAP_HGT = 24;  /* instead of a Term_get_size to fool the map displaying to the corect size*/
s16b COL_MAP_WID = 80;  /* This is a bit cheating with the map functions, those are returns*/

/*
 * Set when loading info data files is done
 */
bool done_loading_data_files = FALSE;

/*
 * Skills
 */
#define init_skill(s, name) \
	s16b s;
#define init_ability(ab, name) \
	s16b ab;
#include "init.h"
#undef init_skill
#undef init_ability

/*
 * Feats, they are filled up when f_info loads
 * Some are absolutly needed if the standart dungeon generator is used
 * Others are merely optional
 */
/* Needed */
s16b FEAT_WAY_LESS = -1;
s16b FEAT_WAY_MORE = -1;
s16b FEAT_SHAFT_DOWN = -1;
s16b FEAT_SHAFT_UP = -1;
s16b FEAT_LESS = -1;
s16b FEAT_MORE = -1;
s16b FEAT_DOOR = -1;			/* (and coresponding open door) */
s16b FEAT_OPEN = -1;
s16b FEAT_WALL_INNER = -1;
s16b FEAT_WALL_OUTER = -1;
s16b FEAT_WALL_EXTRA = -1;
s16b FEAT_WALL_SOLID = -1;
s16b FEAT_PERM_INNER = -1;
s16b FEAT_PERM_OUTER = -1;
s16b FEAT_PERM_SOLID = -1;
s16b FEAT_WORLD_BORDER = -1;

/* Optinnal */
s16b FEAT_RUBBLE = -1;
s16b FEAT_SECRET = -1;
s16b FEAT_BROKEN = -1;
s16b FEAT_DOOR_LOCKED_HEAD = -1;
s16b FEAT_DOOR_LOCKED_TAIL = -1;
s16b FEAT_DOOR_JAMMED_HEAD = -1;
s16b FEAT_DOOR_JAMMED_TAIL = -1;
s16b FEAT_SHOP = -1;

/*
 * Compression level for savefiles from 1 to 9, 0 means no compression
 */
s16b savefile_compress = 0;

/*
 * List of inventories slots
 */
inventory_slot_type *inventory_slot_names;

/*
 * Total number of inventory slots (hard-coded).
 */
s16b INVEN_PACK = 1;
s16b INVEN_TOTAL = 1;

/*
 * Factions
 */
flags_type factions;
flags_type faction_names;

/*
 * Save raw files
 */
s32b tome_write_raw_files = FALSE;

/*
 * Last stat number
 */
s16b STATS_MAX = -1;
s16b MIN_STAT_VALUE = 0;
s16b MAX_STAT_VALUE = 37;
s16b MAX_STAT_NATURAL = 25;

/*
 * Maximun value of skills
 */
s32b SKILL_MAX = 50000;
s32b SKILL_STEP = 1000;

/*
 * Date/time
 */
u32b DAY = 11520;		/* Number of turns per day */
u32b YEAR = 4204800;		/* (DAY * 365); Number of turns per year */
u32b HOUR = 480;		/* (DAY / 24); Number of turns per hour */
u32b MINUTE = 8;		/* (HOUR / 60); Number of turns per minute */
u32b DAY_START = 2880;		/* (HOUR * 6); Sunrise */

/*
 * Use see through wilderness
 */
bool wild_see_through = TRUE;

/*
 * Player exp/au
 */
s32b PY_MAX_EXP = 99999999L;		/* Maximum exp */
s32b PY_MAX_GOLD = 999999999L;		/* Maximum gold */
s32b PY_MAX_LEVEL = 50;			/* Maximum level */

/*
 * Forces a new game to begin
 */
bool force_new_game = FALSE;

/*
 * Do we do realtime ?
 */
u32b realtime_freq = 0;
u32b realtime_run_factor = 1;
