/* File: externs.h */

/* Purpose: extern declarations (variables and functions) */

/*
 * Note that some files have their own header files
 * (z-virt.h, z-util.h, z-form.h, term.h, random.h)
 */


/*
 * Automatically generated "variable" declarations
 */

extern s32b max_macrotrigger;
extern char *macro_template;
extern char *macro_modifier_chr;
extern char *macro_modifier_name[MAX_MACRO_MOD];
extern char *macro_trigger_name[MAX_MACRO_TRIG];
extern char *macro_trigger_keycode[2][MAX_MACRO_TRIG];

/* tables.c */
extern s16b ddd[9];
extern s16b ddx[10];
extern s16b ddy[10];
extern s16b ddx_ddd[9];
extern s16b ddy_ddd[9];
extern s16b dddir[3][3];
extern char hexsym[16];
extern u16b extract_energy[300];
extern s32b *player_exp;
extern cptr color_names[256];
extern cptr stat_names[6];
extern cptr stat_names_reduced[6];
extern magic_power mindcraft_powers[MAX_MINDCRAFT_POWERS];
extern magic_power necro_powers[MAX_NECRO_POWERS];
extern magic_power mimic_powers[MAX_MIMIC_POWERS];
extern magic_power symbiotic_powers[MAX_SYMBIOTIC_POWERS];
extern cptr deity_rarity[2];
extern cptr deity_niceness[10];
extern cptr deity_standing[11];
extern move_info_type move_info[9];
extern tactic_info_type tactic_info[9];
extern activation activation_info[MAX_T_ACT];
extern music music_info[MAX_MUSICS];
extern cptr sense_desc[];
extern flags_group flags_groups[MAX_FLAG_GROUP];
extern power_type powers_type_init[POWER_MAX_INIT];
extern quest_type quest_info[MAX_Q_IDX_INIT];
extern cptr artifact_names_list;
extern monster_power monster_powers[96];
extern between_exit between_exits[MAX_BETWEEN_EXITS];
extern cli_comm *cli_info;
extern s32b cli_total;
extern quest_type quest_init_tome[MAX_Q_IDX_INIT];
extern flags_pointer_type flags_pointer_types[];
extern inventory_slot_type inventory_slot_names_init[];
extern inventory_slot_type *inventory_slot_names;
extern s16b INVEN_PACK;
extern s16b INVEN_TOTAL;

/* variable.c */
extern cptr copyright[6];
extern byte version_major;
extern byte version_minor;
extern byte version_patch;
extern byte version_extra;
extern byte sf_major;
extern byte sf_minor;
extern byte sf_patch;
extern byte sf_extra;
extern u32b sf_xtra;
extern u32b sf_when;
extern u16b sf_lives;
extern u16b sf_saves;
extern u32b vernum; /* Version flag */
extern bool arg_fiddle;
extern bool arg_wizard;
extern bool arg_sound;
extern bool arg_graphics;
extern bool arg_force_original;
extern bool arg_force_roguelike;
extern bool arg_bigtile;
extern bool character_generated;
extern bool character_dungeon;
extern bool character_loaded;
extern bool character_saved;
extern bool character_icky;
extern bool ignore_numlock;
extern bool keypad_autorun;
extern bool character_xtra;
extern u32b seed_flavor;
extern u32b seed_town;
extern u32b seed_dungeon;
extern s32b command_cmd;
extern s16b command_arg;
extern s16b command_rep;
extern s16b command_dir;
extern s16b command_see;
extern s16b command_wrk;
extern s16b command_new;
extern bool command_new_do_keymap;
extern s32b energy_use;
extern s16b choose_default;
extern bool create_up_stair;
extern bool create_down_stair;
extern bool create_up_shaft;
extern bool create_down_shaft;
extern bool msg_flag;
extern bool alive;
extern bool death;
extern s16b running;
extern s16b cur_hgt;
extern s16b cur_wid;
extern s16b dun_level;
extern s16b old_dun_level;
extern s16b num_repro;
extern s16b object_level;
extern s16b monster_level;
extern s32b turn;
extern s32b old_turn;
extern bool wizard;
extern bool use_sound;
extern bool use_graphics;
extern bool use_bigtile;
extern byte graphics_mode;
extern u16b total_winner;
extern u16b has_won;
extern u16b panic_save;
extern u16b noscore;
extern s16b signal_count;
extern bool inkey_base;
extern bool inkey_xtra;
extern bool inkey_scan;
extern bool inkey_flag;
extern bool inkey_dir;
extern s16b coin_type;
extern bool opening_chest;
extern bool shimmer_monsters;
extern bool shimmer_objects;
extern bool repair_monsters;
extern bool repair_objects;
extern s16b inven_nxt;
extern s16b inven_cnt;
extern s16b equip_cnt;
extern s16b m_max;
extern s16b m_cnt;
extern s16b hack_m_idx;
extern s16b hack_m_idx_ii;
extern s32b leaving_quest;
extern bool multi_rew;
extern char summon_kin_type;
extern bool hack_mind;
extern bool hack_corruption;
extern bool is_autosave;
extern s32b artifact_bias;
extern PHYSFS_file *text_out_file;
extern void (*text_out_hook)(byte a, cptr str);
extern s32b text_out_wrap;
extern s32b text_out_indent;
extern PHYSFS_file *highscore_fd;
extern bool show_inven_graph;
extern bool show_store_graph;
extern bool show_equip_graph;
extern bool rogue_like_commands;
extern bool quick_messages;
extern bool other_query_flag;
extern bool carry_query_flag;
extern bool always_pickup;
extern bool prompt_pickup_heavy;
extern bool always_repeat;
extern bool use_old_target;
extern bool depth_in_feet;
extern bool use_color;
extern bool can_redefine_color;
extern bool compress_savefile;
extern bool hilite_player;
extern bool ring_bell;
extern bool find_ignore_stairs;
extern bool find_ignore_doors;
extern bool find_cut;
extern bool find_examine;
extern bool disturb_near;
extern bool disturb_move;
extern bool disturb_panel;
extern bool disturb_detect;
extern bool disturb_state;
extern bool disturb_minor;
extern bool disturb_other;
extern bool avoid_abort;
extern bool avoid_shimmer;
extern bool avoid_other;
extern bool flush_disturb;
extern bool flush_failure;
extern bool flush_command;
extern bool fresh_before;
extern bool fresh_after;
extern bool fresh_message;
extern bool alert_hitpoint;
extern bool alert_failure;
extern bool view_lited_grid;
extern bool plain_descriptions;
extern bool stupid_monsters;
extern bool auto_destroy;
extern bool wear_confirm;
extern bool confirm_stairs;
extern bool disturb_pets;
extern bool view_perma_grids;
extern bool view_torch_grids;
extern bool monster_lite;
extern bool track_follow;
extern bool track_target;
extern bool stack_allow_items;
extern bool stack_allow_wands;
extern bool stack_force_notes;
extern bool stack_force_costs;
extern bool view_reduce_lite;
extern bool view_reduce_view;
extern bool auto_haggle;
extern bool auto_scum;
extern bool expand_look;
extern bool expand_list;
extern bool dungeon_align;
extern bool dungeon_stair;
extern bool smart_learn;
extern bool smart_cheat;
extern bool show_labels;
extern bool show_weights;
extern bool show_choices;
extern bool show_details;
extern bool cheat_peek;
extern bool cheat_hear;
extern bool cheat_room;
extern bool cheat_xtra;
extern bool cheat_know;
extern bool cheat_live;
extern bool last_words;
extern bool speak_unique;
extern bool small_levels;
extern bool empty_levels;
extern bool always_small_level;
#if 0 /* It's controlled by insanity -- pelpel */
extern bool flavored_attacks;
#endif
extern bool player_symbols;
extern byte hitpoint_warn;
extern byte delay_factor;
extern s16b autosave_freq;
extern bool autosave_t;
extern bool autosave_l;
extern s16b feeling;
extern s16b rating;
extern bool good_item_flag;
extern bool closing_flag;
extern s16b max_panel_rows, max_panel_cols;
extern s16b panel_row_min, panel_row_max;
extern s16b panel_col_min, panel_col_max;
extern s16b panel_col_prt, panel_row_prt;
extern s16b feat_wall_outer;
extern s16b feat_wall_inner;
extern s16b floor_type[100];
extern s16b fill_type[100];
extern s16b py;
extern s16b px;
extern s16b target_who;
extern s16b target_col;
extern s16b target_row;
extern s16b health_who;
extern s16b monster_race_idx;
extern s16b monster_ego_idx;
extern object_type *tracked_object;
extern s32b player_uid;
extern s32b player_euid;
extern s32b player_egid;
extern char player_name[128];
extern char player_base[128];
extern char died_from[80];
extern char history[4][60];
extern char savefile[1024];
extern s16b lite_n;
extern s16b lite_y[LITE_MAX];
extern s16b lite_x[LITE_MAX];
extern s16b view_n;
extern byte view_y[VIEW_MAX];
extern byte view_x[VIEW_MAX];
extern byte view_d[VIEW_MAX];
extern s16b temp_n;
extern byte temp_y[TEMP_MAX];
extern byte temp_x[TEMP_MAX];
extern s16b macro__num;
extern cptr *macro__pat;
extern cptr *macro__act;
extern bool *macro__cmd;
extern char *macro__buf;
extern s16b quark__num;
extern cptr *quark__str;
extern u16b message__next;
extern u16b message__last;
extern u16b message__head;
extern u16b message__tail;
extern u16b *message__ptr;
extern byte *message__color;
extern byte *message__type;
extern u16b *message__count;
extern char *message__buf;
extern u32b option_flag[8];
extern u32b option_mask[8];
extern flags_type window_flag[ANGBAND_TERM_MAX];
extern term *angband_term[ANGBAND_TERM_MAX];
extern char angband_term_name[ANGBAND_TERM_MAX][80];
extern byte angband_color_table[256][4];
extern char angband_sound_name[SOUND_MAX][16];
extern s16b SCREEN_HGT;
extern s16b SCREEN_WID;
extern cave_type *cave[MAX_HGT];
extern flags_type monst_list;
extern flags_type keep_monst_list;
extern u16b max_real_towns;
extern u16b max_towns;
extern town_type *town_info;
extern s16b alloc_kind_size;
extern alloc_entry *alloc_kind_table;
extern bool alloc_kind_table_valid;
extern s16b alloc_race_size;
extern alloc_entry *alloc_race_table;
extern byte tval_to_attr[128];
extern char tval_to_char[128];
extern cptr keymap_act[KEYMAP_MODES][256];
extern player_type p_body;
extern player_type *p_ptr;
extern player_descriptor *dp_ptr;
extern s16b *player_hp;
extern ability_type *ab_info;
extern header *s_head;
extern skill_type *s_info;
extern char *s_name;
extern char *s_text;
extern vault_type *v_info;
extern feature_type *f_info;
extern object_kind *k_info;
extern artifact_type *a_info;
extern ego_item_type *e_info;
extern header *ra_head;
extern randart_part_type *ra_info;
extern randart_gen_type ra_gen[30];
extern monster_race *r_info;
extern monster_ego *re_info;
extern dungeon_info_type *d_info;

extern player_descriptor *descriptor_info;

extern wilderness_type_info *wf_info;
extern s32b wildc2i[256];
extern store_info_type *st_info;
extern store_action_type *ba_info;
extern owner_type *ow_info;
extern set_type *set_info;
extern cptr ANGBAND_SYS;
extern cptr ANGBAND_KEYBOARD;
extern cptr TOME_GRAPHICS;
extern cptr ENGINE_DIR;
extern cptr ENGINE_DIR_DEP;
extern cptr ENGINE_VERSION;
extern cptr TENGINE_DIR;
extern cptr TENGINE_DIR_SCORES;
extern cptr TENGINE_DIR_BONE;
extern cptr TENGINE_DIR_CMOV;
extern cptr TENGINE_DIR_DATA;
extern cptr TENGINE_DIR_DNGN;
extern cptr TENGINE_DIR_ENGINE;
extern cptr TENGINE_DIR_FILE;
extern cptr TENGINE_DIR_FONT;
extern cptr TENGINE_DIR_GRAF;
extern cptr TENGINE_DIR_HELP;
extern cptr TENGINE_DIR_INFO;
extern cptr TENGINE_DIR_MODULES;
extern cptr TENGINE_DIR_NOTE;
extern cptr TENGINE_DIR_PATCH;
extern cptr TENGINE_DIR_PREF;
extern cptr TENGINE_DIR_RAW;
extern cptr TENGINE_DIR_SAVE;
extern cptr TENGINE_DIR_SCPT;
extern cptr TENGINE_DIR_USER;
extern cptr TENGINE_DIR_XTRA;
extern char pref_tmp_value[8];
extern bool item_tester_full;
extern byte item_tester_tval;
extern bool (*item_tester_hook)(object_type *o_ptr, s32b item);
extern bool (*ang_sort_comp)(vptr u, vptr v, s32b a, s32b b);
extern void (*ang_sort_swap)(vptr u, vptr v, s32b a, s32b b);
extern bool (*get_mon_num_hook)(s32b r_idx);
extern bool (*get_mon_num2_hook)(s32b r_idx);
extern bool (*get_obj_num_hook)(s32b k_idx);
extern u16b max_wild_x;
extern u16b max_wild_y;
extern wilderness_map **wild_map;
extern u16b old_max_s_idx;
extern u16b max_ab_idx;
extern u16b max_s_idx;
extern u16b max_al_idx;
extern u16b max_r_idx;
extern u16b max_re_idx;
extern u16b max_k_idx;
extern u16b max_v_idx;
extern u16b max_f_idx;
extern u16b max_a_idx;
extern u16b max_e_idx;
extern u16b max_ps_idx;
extern u16b max_ra_idx;
extern u16b max_d_idx;
extern u16b max_m_idx;
extern u16b max_t_idx;
extern u16b max_dp_idx;
extern u16b max_st_idx;
extern u16b max_ba_idx;
extern u16b max_ow_idx;
extern u16b max_wf_idx;
extern s16b max_set_idx;
extern s32b init_flags;
extern u16b no_breeds;
extern bool carried_monster_hit;
extern random_artifact random_artifacts[MAX_RANDARTS];
extern s32b RANDART_WEAPON;
extern s32b RANDART_ARMOR;
extern s32b RANDART_JEWEL;
extern byte vanilla_town;
extern byte dungeon_type;
extern byte old_dungeon_type;
extern s16b *max_dlv;
extern s16b doppleganger;
extern bool permanent_levels;
extern bool autoroll;
extern bool point_based;
extern bool maximize, preserve, special_lvls, ironman_rooms;
extern bool inventory_no_move;
extern bool take_notes, auto_notes;
extern bool *m_allow_special;
extern bool *k_allow_special;
extern bool *a_allow_special;
extern bool rand_birth;
extern bool fast_autoroller;
extern bool joke_monsters;
extern bool munchkin_multipliers;
extern bool center_player;
#ifdef SAFER_PANICS
extern bool panicload;
#endif
extern s16b plots[MAX_PLOTS];
extern random_quest random_quests[MAX_RANDOM_QUEST];
extern bool exp_need;
extern bool autoload_old_colors;
extern bool *special_lvl[MAX_DUNGEON_DEPTH];
extern bool generate_special_feeling;
extern bool auto_more;
extern flags_holder_type *dungeon_flags;
extern birther previous_char;
extern hist_type *bg;
extern s32b max_bg_idx;
extern power_type *powers_type;
extern s16b power_max;
extern s32b extra_savefile_parts;
extern s16b max_q_idx;
extern quest_type *quest;
extern bool player_char_health;
extern s32b project_range;
extern s32b project_time;
extern s16b project_time_speed;
extern s32b project_time_effect;
extern effect_type effects[MAX_EFFECTS];
extern bool linear_stats;
extern s16b STATS_MAX;
extern s16b MIN_STAT_VALUE;
extern s16b MAX_STAT_VALUE;
extern s16b MAX_STAT_NATURAL;
extern s32b max_bact;
extern s16b max_corruptions;
extern bool option_ingame_help;
extern bool automatizer_enabled;
extern s16b last_teleportation_y;
extern s16b last_teleportation_x;
extern cptr game_module;
extern s32b VERSION_MAJOR;
extern s32b VERSION_MINOR;
extern s32b VERSION_PATCH;
extern s32b max_plev;
extern timer_type *gl_timers;
extern s32b spell_caster;
extern s16b ROW_MAP;
extern s16b COL_MAP;
extern s16b ROW_MAP_HGT;
extern s16b COL_MAP_WID;
extern bool done_loading_data_files;
extern s16b FEAT_WAY_LESS;
extern s16b FEAT_WAY_MORE;
extern s16b FEAT_SHAFT_DOWN;
extern s16b FEAT_SHAFT_UP;
extern s16b FEAT_LESS;
extern s16b FEAT_MORE;
extern s16b FEAT_DOOR;			/* (and coresponding open door) */
extern s16b FEAT_OPEN;
extern s16b FEAT_WALL_INNER;
extern s16b FEAT_WALL_OUTER;
extern s16b FEAT_WALL_EXTRA;
extern s16b FEAT_WALL_SOLID;
extern s16b FEAT_PERM_INNER;
extern s16b FEAT_PERM_OUTER;
extern s16b FEAT_PERM_SOLID;
extern s16b FEAT_WORLD_BORDER;

extern s16b FEAT_RUBBLE;
extern s16b FEAT_SECRET;
extern s16b FEAT_BROKEN;
extern s16b FEAT_DOOR_LOCKED_HEAD;
extern s16b FEAT_DOOR_LOCKED_TAIL;
extern s16b FEAT_DOOR_JAMMED_HEAD;
extern s16b FEAT_DOOR_JAMMED_TAIL;
extern s16b FEAT_SHOP;
#define init_skill(s, name) \
	extern s16b s;
#define init_ability(ab, name) \
	extern s16b ab;
#include "init.h"
#undef init_skill
#undef init_ability
extern PHYSFS_file *movfile;
extern s16b savefile_compress;
extern flags_type factions;
extern flags_type faction_names;
extern s32b tome_write_raw_files;
extern s32b SKILL_MAX;
extern s32b SKILL_STEP;
extern u32b DAY;                   /* Number of turns per day */
extern u32b YEAR;             /* Number of turns per year */
extern u32b HOUR;              /* Number of turns per hour */
extern u32b MINUTE;             /* Number of turns per minute */
extern u32b DAY_START;              /* Sunrise */
extern bool wild_see_through;
extern s32b PY_MAX_EXP;		/* Maximum exp */
extern s32b PY_MAX_GOLD;		/* Maximum gold */
extern s32b PY_MAX_LEVEL;			/* Maximum level */
extern bool force_new_game;
extern u32b realtime_freq;
extern u32b realtime_run_factor;
extern bool use_corpse_decay;

/* plots.c */
extern hooks_chain *hooks_heads[MAX_HOOKS];
extern PHYSFS_file *hook_file;
extern bool check_hook(s32b h_idx);
extern void wipe_hooks(void);
extern void dump_hooks(s32b h_idx);
extern void init_hooks(void);
extern hooks_chain* add_hook(s32b h_idx, hook_type hook, cptr name);
extern void add_hook_script(s32b h_idx, char *script, cptr name);
extern void del_hook(s32b h_idx, hook_type hook);
extern void del_hook_name(s32b h_idx, cptr name);
extern s32b get_next_arg(char *fmt);
extern s32b process_hooks_restart;
extern hook_return process_hooks_return[20];
extern bool process_hooks_ret(s32b h_idx, char *ret, char *fmt, ...);
extern bool process_hooks(s32b h_idx, char *fmt, ...);

/* help.c */
extern void ingame_help(bool enable);

/* birth.c */
extern void print_desc_aux(cptr txt, s32b y, s32b x);
extern void save_savefile_names(void);
extern bool no_begin_screen;
extern bool begin_screen(bool *quickstart);
extern errr init_randart(void);
extern void get_height_weight(void);
extern void player_birth_skill();
extern void player_birth(bool new_game);

/* cave.c */
extern s16b cave_feat_get(cave_type *c_ptr, s32b flag);
extern s16b cave_mimic_get(cave_type *c_ptr, s32b flag);
extern bool cave_feat_is(cave_type *c_ptr, s32b flag);
extern bool cave_mimic_is(cave_type *c_ptr, s32b flag);
extern s32b distance(s32b y1, s32b x1, s32b y2, s32b x2);
extern bool los(s32b y1, s32b x1, s32b y2, s32b x2);
extern bool wallless_line(s32b y1, s32b x1, s32b y2, s32b x2,
						bool ignore_passable);
extern bool cave_valid_bold(s32b y, s32b x);
extern bool no_lite(void);
#ifdef USE_TRANSPARENCY
#ifdef USE_EGO_GRAPHICS
extern void map_info(s32b y, s32b x, byte *ap, char *cp, byte *tap, char *tcp, byte *eap, char *ecp);
#else /* USE_EGO_GRAPHICS */
extern void map_info(s32b y, s32b x, byte *ap, char *cp, byte *tap, char *tcp);
#endif /* USE_EGO_GRAPHICS */
#else /* USE_TRANSPARENCY */
extern void map_info(s32b y, s32b x, byte *ap, char *cp);
#endif /* USE_TRANSPARENCY */
extern void map_info_default(s32b y, s32b x, byte *ap, char *cp);
extern void move_cursor_relative(s32b row, s32b col);
extern void print_rel(char c, byte a, s32b y, s32b x);
extern void note_spot(s32b y, s32b x);
extern void lite_spot(s32b y, s32b x);
extern void prt_map(void);
extern void display_map(s32b *cy, s32b *cx);
extern void do_cmd_view_map(void);
extern errr vinfo_init(void);
extern void forget_los_tracks(void);
extern void forget_view(void);
extern void update_view(void);
extern void forget_mon_lite(void);
extern void update_mon_lite(void);
extern void forget_flow(void);
extern void update_flow(void);
extern void map_area(s32b cy, s32b cx, s32b radius);
extern void lite_level(bool mark);
extern void wiz_lite(bool mark);
extern void wiz_lite_extra(void);
extern void forget_level(void);
extern void darken_level(bool all);
extern void wiz_dark(void);
extern void cave_set_feat(s32b y, s32b x, s32b feat);
extern void place_floor(s32b y, s32b x);
extern void place_filler(s32b y, s32b x);
extern void mmove2(s32b *y, s32b *x, s32b y1, s32b x1, s32b y2, s32b x2);
extern bool projectable(s32b y1, s32b x1, s32b y2, s32b x2);
extern void scatter(s32b *yp, s32b *xp, s32b y, s32b x, s32b d, s32b m);
extern void health_track(s32b m_idx);
extern void monster_race_track(s32b r_idx, s32b ego);
extern void object_track(object_type *o_ptr);
extern void disturb(s32b major, s32b flush_output);
extern s32b is_quest(s32b level);
extern s32b random_quest_number(void);
extern s32b new_effect(s32b who, s32b type, s32b dam, s32b time, s32b cy, s32b cx, s32b rad, s32b flags, s16b speed);

/* cmovie.c */
extern void cmovie_init_second(void);
extern s16b do_play_cmovie(cptr cmov_file, bool show_info);
extern void do_record_cmovie(cptr cmovie);
extern void do_stop_cmovie(void);
extern void do_start_cmovie(void);
extern void cmovie_clean_line(s32b y, char *abuf, char *cbuf);
extern void cmovie_record_line(s32b y);
extern void do_cmovie_insert(void);

/* cmd1.c */
extern void carry(s32b pickup);
extern bool py_attack(s32b y, s32b x, s32b max_blow, s32b dir);
extern bool player_can_enter(byte feature);
extern void move_player(s32b dir, s32b do_pickup);
extern void move_player_aux(s32b dir, s32b do_pickup, s32b run, bool disarm);
extern void run_step(s32b dir);
extern void step_effects(s32b y, s32b x, s32b do_pickup);
extern void do_cmd_pet(void);
extern bool execute_inscription(byte i, byte y, byte x);
extern void do_cmd_engrave(void);
extern void do_spin(void);

/* cmd2.c */
extern bool do_cmd_open_obj(s32b y, s32b x, s16b o_idx);
extern bool do_cmd_open_aux(s32b y, s32b x, s32b dir);
extern byte show_monster_inven(s32b m_idx, s32b *monst_list);
extern s32b breakage_chance(object_type *o_ptr);
extern void do_cmd_go_up_down(bool updown);
extern void do_cmd_open(void);
extern void do_cmd_close(void);
extern void do_cmd_chat(void);
extern void do_cmd_give(void);
extern bool do_cmd_tunnel_aux(s32b y, s32b x, s32b dir);
extern void do_cmd_tunnel(void);
extern void do_cmd_disarm(void);
extern void do_cmd_disarm(void);
extern void do_cmd_bash(void);
extern void do_cmd_alter(void);
extern void do_cmd_walk(s32b pickup);
extern void do_cmd_stay(s32b pickup);
extern void do_cmd_run(void);
extern void do_cmd_rest(void);
extern void do_cmd_fire(void);
extern void do_cmd_throw(void);
extern void do_cmd_unwalk(void);
extern void do_cmd_immovable_special(void);
extern void fetch(s32b dir, s32b wgt, bool require_los);
extern void do_cmd_sacrifice(void);
extern void do_cmd_create_artifact(object_type *q_ptr);
extern void do_cmd_steal(void);
extern void do_cmd_racial_power(void);

/* cmd3.c */
extern void do_cmd_html_dump(void);
extern void cli_add(cptr active, cptr trigger, cptr descr);
extern void do_cmd_cli(void);
extern void do_cmd_cli_help(void);
extern void do_cmd_inven(void);
extern void do_cmd_equip(void);
extern void do_cmd_wield(void);
extern void do_cmd_takeoff(void);
extern void do_cmd_drop(void);
extern void do_cmd_destroy(void);
extern void do_cmd_observe(void);
extern void do_cmd_uninscribe(void);
extern void do_cmd_inscribe(void);
extern void do_cmd_target(void);
extern void do_cmd_look(void);
extern void do_cmd_locate(void);
extern void do_cmd_query_symbol(void);
extern bool do_cmd_sense_grid_mana(void);
extern bool research_mon(void);
extern s32b portable_hole_weight(void);
extern void set_portable_hole_weight(void);
extern void do_cmd_portable_hole(void);

/* cmd4.c */
extern void do_cmd_options_win(void);
extern void macro_recorder_start(void);
extern void macro_recorder_add(s32b c);
extern void macro_recorder_stop(void);
extern void do_cmd_macro_recorder(void);
extern void do_cmd_redraw(void);
extern void do_cmd_change_name(void);
extern void do_cmd_message_one(void);
extern void do_cmd_messages(void);
extern void do_cmd_options(char *what);
extern void do_cmd_pref(void);
extern void do_cmd_macros(void);
extern void do_cmd_visuals(void);
extern void do_cmd_colors(void);
extern void do_cmd_note(void);
extern void do_cmd_version(void);
extern void do_cmd_feeling(void);
extern void do_cmd_load_screen(void);
extern void do_cmd_save_screen(void);
extern void plural_aux(char * Name);
extern void do_cmd_change_tactic(s32b i);
extern void do_cmd_change_movement(s32b i);
extern void do_cmd_time(void);
extern void do_cmd_options_aux(s32b page, cptr info, bool read_only);


/* cmd5.c */
extern void calc_magic_bonus(void);
extern void do_cmd_browse_aux(object_type *o_ptr);
extern void do_cmd_browse(void);
extern void do_cmd_cast(void);
extern void do_cmd_rerate(void);
extern void corrupt_player(void);
extern bool item_tester_hook_armour(object_type *o_ptr);
extern void fetch(s32b dir, s32b wgt, bool require_los);
extern void do_poly_self(void);
extern void brand_weapon(s32b brand_type);
extern cptr symbiote_name(bool capitalize);
extern s32b get_school_spell(cptr do_what, cptr check_fct, object_type *force_book, bool browse);
extern void cast_school_spell(void);
extern void browse_school_spell(object_type *o_ptr);
extern s32b find_spell(char *name);
extern bool is_school_book(object_type *o_ptr);

/* cmd6.c */
extern void set_stick_mode(object_type *o_ptr);
extern void unset_stick_mode(void);
extern const char *activation_aux(object_type *o_ptr, bool desc, s32b item);
extern void do_cmd_activate(void);
extern void do_cmd_rerate(void);
extern cptr get_item_hook_find_obj_what;
extern bool get_item_hook_find_obj(s32b *item);

/* cmd7.c */
extern void do_cmd_pray(void);
extern bool get_magic_power(s32b *sn, magic_power *powers, s32b max_powers, void (*power_info)(char *p, s32b power), s32b plev, s32b cast_stat);

/* dungeon.c */
extern byte value_check_aux1(object_type *o_ptr);
extern byte value_check_aux1_magic(object_type *o_ptr);
extern byte value_check_aux2(object_type *o_ptr);
extern byte value_check_aux2_magic(object_type *o_ptr);
extern byte select_sense(object_type *o_ptr, bool ok_combat, bool ok_magic);
extern void play_game(bool new_game);
extern void realtime_mode(u32b freq);

/* files.c */
extern void print_kill_count(PHYSFS_file *fff);
extern void html_screenshot(cptr name);
extern void help_file_screenshot(cptr name);
extern void player_flags(u32b* f1, u32b* f2, u32b* f3, u32b* f4, u32b* f5, u32b* esp);
extern void wipe_saved(void);
extern s16b tokenize(char *buf, s16b num, char **tokens, char delim1, char delim2);
extern void display_player(s32b mode);
extern cptr describe_player_location(void);
extern errr file_character(cptr name);
extern errr process_pref_file_aux(char *buf);
extern errr process_pref_file(cptr name);
extern errr check_time_init(void);
extern errr check_load_init(void);
extern errr check_time(void);
extern errr check_load(void);
extern void read_times(void);
extern bool txt_to_html(cptr head, cptr food, cptr base, cptr ext, bool force, bool recur);
extern bool show_file(cptr name, cptr what, s32b line, s32b mode);
extern void do_cmd_help(void);
extern void process_player_base(void);
extern void process_player_name(bool sf);
extern void get_name(void);
extern void do_cmd_suicide(void);
extern void do_cmd_save_game(void);
extern long total_points(void);
extern void display_scores(s32b from, s32b to);
extern errr predict_score(void);
extern void close_game(void);
extern void exit_game_panic(void);
extern void signals_ignore_tstp(void);
extern void signals_handle_tstp(void);
extern void signals_init(void);
extern errr get_rnd_line(char * file_name, char * output);
extern char *get_line(char* fname, cptr fdir, char *linbuf, s32b line);
extern void do_cmd_knowledge_corruptions(void);
extern void race_legends(void);
extern void show_highclass(s32b building);
extern errr get_xtra_line(char * file_name, monster_type *m_ptr, char * output);

/* gen_maze.c */
extern bool level_generate_maze(cptr name);

/* gen_life.c */
extern bool level_generate_life(cptr name);
extern void evolve_level(bool noise);

/* generate.c */
extern bool find_naked_bold(s32b set, s32b* x, s32b* y);
extern void dungeon_set_level_size(s32b hgt, s32b wid);
extern bool room_build(s32b y, s32b x, s32b typ);
extern void recursive_river(s32b x1, s32b y1, s32b x2, s32b y2, s32b feat1, s32b feat2, s32b width);
extern bool new_player_spot(s32b branch);
extern void add_level_generator(cptr name, bool (*generator)(cptr), bool stairs, bool monsters, bool objects, bool miscs);
extern bool level_generate_dungeon(cptr name);
extern bool generate_fracave(s32b y0, s32b x0,s32b xsize,s32b ysize,s32b cutoff,bool light,bool room);
extern void generate_hmap(s32b y0, s32b x0,s32b xsiz,s32b ysiz,s32b grd,s32b roug,s32b cutoff);
extern bool room_alloc(s32b x,s32b y,bool crowded,s32b by0,s32b bx0,s32b *xx,s32b *yy);
extern void generate_grid_mana(void);
extern byte calc_dungeon_type(void);
extern void generate_cave(void);
extern void build_rectangle(s32b y1, s32b x1, s32b y2, s32b x2, s32b feat, s32b info);


/* wild.c */
extern s32b generate_area(s32b y, s32b x, bool border, bool corner, bool refresh);
extern void wilderness_gen(s32b refresh);
extern void wilderness_gen_small(void);
extern void reveal_wilderness_around_player(s32b y, s32b x, s32b h, s32b w);
extern void town_gen(s32b t_idx);

/* init1.c */
extern void esettings_set_str(cptr key, cptr val);
extern void esettings_set_int(cptr key, s32b val);
extern cptr esettings_get_str(cptr key, cptr default_val);
extern s32b esettings_get_int(cptr key, s32b default_val);
extern void esettings_load();
extern void esettings_save();

/* init2.c */
extern void init_tome_color(byte id, cptr name, char chr, s32b r, s32b g, s32b b);
extern void init_corruptions(s16b new_size);
extern void init_spells(s16b new_size);
extern void init_schools(s16b new_size);
extern void reinit_inventory_slot_names(s16b new_size);
extern void reinit_quests(s16b new_size);
extern void reinit_powers_type(s16b new_size);
extern void create_stores_stock(s32b t);
extern errr init_v_info(void);
extern void init_file_paths(char *argv0);
extern void init_angband(cptr force_module);
extern void cleanup_angband(void);
extern errr init_buildings(void);
#ifdef ALLOW_TEMPLATES
extern s16b error_idx;
extern s16b error_line;
extern u32b fake_name_size;
extern u32b fake_text_size;
#endif /* ALLOW_TEMPLATES */

/* cache.c */
extern errr init_race_mod_info(void);
extern errr init_wf_info(void);
extern errr init_s_info(void);
extern errr init_class_info(void);
extern errr init_d_info(void);
extern errr init_v_info(void);
extern errr init_sex_info(void);
extern errr init_a_info(void);
extern errr init_e_info(void);
extern errr init_ta_info(void);
extern errr init_ba_info(void);
extern errr init_st_info(void);
extern errr init_set_info(void);
extern errr init_re_info(void);
extern errr init_meta_class_info(void);
extern errr init_race_info(void);
extern errr init_r_info(void);
extern errr init_ra_info(void);
extern errr init_ab_info(void);
extern errr init_k_info(void);
extern errr init_ow_info(void);
extern errr init_f_info(void);
extern errr load_functions_registry(void);
extern errr save_functions_registry(void);
extern void load_cache_wrappers(void);
extern errr init_descriptor_info(void);

/* loadsave.c */
extern bool save_player_quickstart(void);
extern void loadsave_do_item(byte type, object_type **o_ptr, s32b flag);
extern void loadsave_do_monster(byte type, monster_type **m_ptr, s32b flag);
extern void loadsave_do_store(byte type, store_type **str, s32b flag);
extern void do_store(byte type, store_type *str, s32b flag);
extern void do_monster(byte type, monster_type *m_ptr, s32b flag);
extern void do_item(byte type, object_type *o_ptr, s32b flag);
extern bool check_savefile_dir(cptr name);
extern void delete_savefile_dir(cptr name);
extern void set_loadsave_stream(PHYSFS_file *f);
extern void do_char(char *, s32b);
extern void do_bool(bool *, s32b);
extern void do_byte(byte *, s32b);
extern void do_u16b(u16b *, s32b);
extern void do_s16b(s16b *, s32b);
extern void do_u32b(u32b *, s32b);
extern void do_s32b(s32b *, s32b);
extern void loadsave_do_string_nolen(byte type, char **str, s32b flag);
extern void do_string_nolen(char **, s32b);
extern void do_flags(flags_type *f, s32b flag);
extern void register_savefile(s32b num);
extern bool file_exist(cptr buf);
extern s16b rd_variable(void);
extern void wr_variable(s16b *var);
extern void wr_scripts(void);
extern bool load_dungeon(s32b dungeon, s32b level, s32b *saved_turn);
extern void save_dungeon(s32b saved_turn);
extern bool save_player(void);
extern bool load_player(bool quickstart);
extern errr rd_savefile_new(void);
extern byte load_key(char *key);
extern void save_nil_key(char *key);
extern void save_number_key(char *key, s32b val);
extern void save_string_key(char *key, cptr val);

/* melee1.c */
/* melee2.c */
extern bool find_safety(monster_type *m_ptr, s32b *yp, s32b *xp);
extern bool find_hiding(monster_type *m_ptr, s32b *yp, s32b *xp);
extern s32b monst_spell_monst_spell;
extern bool mon_take_hit_mon(s32b s_idx, s32b m_idx, s32b dam, bool *fear, cptr note);
extern bool carried_make_attack_normal(s32b r_idx);
extern bool make_attack_normal(s32b m_idx, byte divis);
extern bool make_attack_spell(s32b m_idx);
extern void process_monsters(void);
extern void curse_equipment(s32b chance, s32b heavy_chance);
extern void curse_equipment_dg(s32b chance, s32b heavy_chance);
extern bool can_pass_aux(flags_type *dest_flags, flags_type *passer_flags,
                         u32b *how);
extern bool monst_can_pass_square(monster_type *m_ptr, s32b y, s32b x,
                                   u32b *how);
extern bool monst_can_enter_square(monster_type *m_ptr, s32b y, s32b x,
                                   u32b *how);

/* monster1.c */
extern void screen_roff(s32b r_idx, s32b ego, s32b remember);
extern void display_roff(s32b r_idx, s32b ego);

extern void monstmem_add(s32b class, s32b type, monster_type *m_ptr, s32b val);
extern void monstmem_add2(s32b class, s32b type1, s32b type2,
                          monster_type *m_ptr, s32b val);
extern s32b mon_killwall(s32b y, s32b x, s32b m_idx);

/* monster2.c */
extern bool restrict_to_dungeon(flags_type *flags, s32b chr);
extern void monster_set_level(s32b m_idx, s32b level);
extern s32b modify_aux(s32b a, s32b b, char mod);
extern void monster_msg(cptr fmt, ...);
extern void cmonster_msg(char a, cptr fmt, ...);
extern bool mego_ok(s32b r_idx, s32b ego);
extern void monster_check_experience(s32b m_idx, bool silent);
extern void monster_gain_exp(s32b m_idx, u32b exp, bool silent);
extern monster_race* race_info_idx(s32b r_idx, s32b ego);
extern s32b get_wilderness_flag(void);
extern void delete_monster_type(monster_type *m_ptr, bool del);
extern void delete_monster_idx(s32b i);
extern void delete_monster(s32b y, s32b x);
extern void compact_monsters(s32b size);
extern void wipe_m_list(void);
extern s16b m_pop(void);
extern errr get_mon_num_prep(void);
extern s16b get_mon_num(s32b level);
extern void monster_desc(char *desc, monster_type *m_ptr, s32b mode);
extern void monster_race_desc(char *desc, s32b r_idx, s32b ego);
extern void lore_do_probe(s32b m_idx);
extern void lore_treasure(s32b m_idx, s32b num_item, s32b num_gold);
extern void update_mon(s32b m_idx, bool full);
extern void update_monsters(bool full);
extern void monster_carry(monster_type *m_ptr, s32b m_idx, object_type *q_ptr);
extern bool monster_carry_okay(monster_type *m_ptr, object_type *q_ptr);
extern bool bypass_r_ptr_max_num ;
extern bool place_monster_aux(s32b y, s32b x, s32b r_idx, bool slp, bool grp, s32b status);
extern s16b place_monster_aux_ego(s32b y, s32b x, s32b r_idx, s32b ego,
                                  bool slp, bool grp, s32b status);
extern bool place_monster(s32b y, s32b x, bool slp, bool grp);
extern bool alloc_horde(s32b y, s32b x);
extern bool alloc_monster(s32b dis, bool slp);
extern s32b summon_specific_level;
extern s32b place_monster_result;
extern bool summon_hack;
extern bool summon_random(s32b y1, s32b x1, s32b lev, s32b faction);
extern s32b get_rand_r_idx(s32b lev, s32b faction, s32b min_friend, s32b max_friend,
				   flags_type* flags, flags_type* flags_forbid,
				   cptr chars, cptr chars_forbid,
				   cptr name, cptr name_forbid);
extern bool summon_specific(s32b y1, s32b x1, s32b lev, s32b faction,
					 s32b min_friend, s32b max_friend, bool group_ok,
					 flags_type* flags, flags_type* flags_forbid,
					 cptr chars, cptr chars_forbid,
					 cptr name, cptr name_forbid);
extern void monster_swap(s32b y1, s32b x1, s32b y2, s32b x2);
extern bool multiply_monster(s32b m_idx, bool charm, bool clone);
extern void update_smart_learn(s32b m_idx, s32b what);
extern bool place_monster_one_no_drop;
extern monster_race *place_monster_one_race;
extern s16b place_monster_type(s32b y, s32b x, monster_type *m_ptr);
extern void monster_prep(monster_type* m_ptr, s32b r_idx, s32b ego);
extern monster_race *place_monster_one_race;
extern monster_type *placed_monsters[PLACED_MONSTERS_MAX];
extern s32b          placed_monster_ids[PLACED_MONSTERS_MAX];
extern s32b          placed_monsters_num;
extern s32b          placed_monster_id;
extern s16b place_monster_one(s32b y, s32b x, s32b r_idx, s32b ego, bool slp, s32b status);
extern s16b player_place(s32b y, s32b x);
extern void monster_drop_carried_objects(monster_type *m_ptr);
extern bool monster_dungeon(s32b r_idx);
extern bool monster_quest(s32b r_idx);
extern bool monster_ocean(s32b r_idx);
extern bool monster_shore(s32b r_idx);
extern bool monster_town(s32b r_idx);
extern bool monster_wood(s32b r_idx);
extern bool monster_volcano(s32b r_idx);
extern bool monster_mountain(s32b r_idx);
extern bool monster_grass(s32b r_idx);
extern bool monster_deep_water(s32b r_idx);
extern bool monster_shallow_water(s32b r_idx);
extern bool monster_lava(s32b r_idx);
extern void set_mon_num_hook(void);
extern void set_mon_num2_hook(s32b y, s32b x);
extern bool monster_can_cross_terrain(byte feat, flags_type *flags);
extern void corrupt_corrupted(void);
extern bool is_obj_mimic_at(s32b y, s32b x);
extern object_type* get_obj_mimic_obj_at(s32b y, s32b x);
extern bool is_feat_mimic_at(s32b y, s32b x);
extern s16b get_feat_mimic_feat_at(s32b y, s32b x);

/* monster3.c */
extern void talk_to_monster(s32b m_idx);
extern bool player_monster_swap(monster_type *m_ptr);
extern void set_monster_ai(monster_type *m_ptr, u32b ai);
extern void set_faction_friendliness(u32b faction, u32b friend_faction, s32b percent);
extern s32b get_faction_friendliness(u32b faction, u32b friend_faction);
extern bool change_monst_faction(monster_type *m_ptr, u32b new_faction);
extern u32b get_hostile_faction(monster_type *m_ptr, u32b hostile_to,
                                u32b default_faction);
extern s16b lookup_race(char d_char, s16b sval);
extern void do_cmd_companion(void);
extern bool do_control_reconnect(void);
extern bool do_control_drop(void);
extern bool do_control_magic(void);
extern bool do_control_pickup(void);
extern bool do_control_inven(void);
extern bool do_control_walk(void);
extern bool can_create_companion(void);
extern void ai_deincarnate(s32b m_idx);
extern bool ai_possessor(s32b m_idx, s32b o_idx);
extern bool ai_multiply(s32b m_idx);
extern bool change_side(monster_type *m_ptr);
extern s32b is_friend(monster_type *m_ptr);
extern bool is_enemy(monster_type *m_ptr, monster_type *t_ptr);
extern monster_blow *monster_blow_new();
extern void monster_blow_del(monster_blow *blow);
extern monster_blow *monster_blow_dup(monster_blow *blow);
extern void monster_blow_loadsave(byte, monster_blow **blow, s32b flg);
extern monster_type *monster_type_new();
extern void monster_type_del(monster_type *m_ptr);
extern monster_type *monster_type_dup(monster_type *m_ptr);
extern void monster_type_copy(monster_type *dest, monster_type *src);

/* object1.c */
/* object2.c */
extern bool get_item_allow(s32b item);
extern s32b get_tag(s32b *cp, char tag);
extern byte get_item_letter_color(object_type *o_ptr);
extern void describe_device(object_type *o_ptr);
extern void object_pickup(s32b this_o_idx);
extern s32b get_slot(s32b slot);
extern bool apply_set(s16b a_idx, s16b set_idx);
extern bool takeoff_set(s16b a_idx, s16b set_idx);
extern bool wield_set(s16b a_idx, s16b set_idx, bool silent);
extern object_type *get_object(s32b item);
extern void set_object(s32b item, object_type *o_ptr);
extern s32b calc_total_weight(void);
extern void add_random_ego_flag(object_type *o_ptr, flags_type *fego, bool *limit_blows);
extern bool info_spell;
extern char spell_txt[50];
extern bool grab_tval_desc(s32b tval);
extern bool verify(cptr prompt, s32b item);
extern void flavor_init(void);
extern void reset_visuals(void);
extern s32b object_power(object_type *o_ptr);
extern void object_desc(char *buf, object_type *o_ptr, s32b pref, s32b mode);
extern void object_desc_store(char *buf, object_type *o_ptr, s32b pref, s32b mode);
extern bool object_out_desc(object_type *o_ptr, PHYSFS_file *fff, bool trim_down, bool wait_for_it);
extern char index_to_label(s32b i);
extern s16b label_to_inven(s32b c);
extern s16b label_to_equip(s32b c);
extern s16b wield_slot_ideal(object_type *o_ptr, bool ideal);
extern s16b wield_slot(object_type *o_ptr);
extern cptr mention_use(s32b i);
extern cptr describe_use(s32b i);
extern void item_charges(s32b item);
extern void item_describe(s32b item);
extern void item_increase(s32b item, s32b num);
extern bool item_optimize(s32b item);
extern bool inven_carry_okay(object_type *o_ptr);
extern s16b inven_carry(object_type *o_ptr, bool final);
extern s16b inven_carry_inven(flags_type *inven, object_type *o_ptr, bool final);
extern s16b inven_takeoff(s32b item, s32b amt, bool force_drop, bool silent);
extern s16b inven_drop(s32b item, s32b amt, s32b dy, s32b dx, bool silent);
extern bool item_tester_okay(object_type *o_ptr, s32b item);
extern void display_inven(void);
extern void display_equip(void);
extern void show_inven(bool mirror);
extern void show_equip(bool mirror);
extern void toggle_inven_equip(void);
extern bool (*get_item_extra_hook)(s32b *cp);
extern bool get_item(s32b *cp, cptr pmt, cptr str, s32b mode);
extern errr get_obj_num_prep(void);
extern s16b get_obj_num(s32b level);
extern void object_known(object_type *o_ptr);
extern void object_aware(object_type *o_ptr);
extern void object_unaware(object_type *o_ptr);
extern bool object_aware_p(object_type *o_ptr);
extern void object_tried(object_type *o_ptr);
extern bool object_tried_p(object_type *o_ptr);
extern s32b object_value(object_type *o_ptr);
extern s32b object_value_real(object_type *o_ptr);
extern bool object_similar(object_type *o_ptr, object_type *j_ptr);
extern void object_absorb(object_type *o_ptr, object_type *j_ptr);
extern void object_unabsorb(object_type *o_ptr, object_type *j_ptr);
extern s16b lookup_kind(s32b tval, s32b sval);
extern s32b hack_apply_magic_power;
extern void apply_magic(object_type *o_ptr, s32b lev, bool okay, bool good, bool great);
extern object_type* make_object(bool good, bool great, flags_type* theme);
extern void place_object(s32b y, s32b x, bool good, bool great, s32b where);
extern bool make_gold(object_type *j_ptr);
extern void place_gold(s32b y, s32b x);
extern void process_objects(void);
extern s16b drop_near(object_type *o_ptr, s32b chance, s32b y, s32b x);
extern void acquirement(s32b y1, s32b x1, s32b num, bool great, bool known);
extern cptr item_activation(object_type *o_ptr,byte num);
extern void combine_pack(void);
extern void reorder_pack(void);
extern void display_koff(s32b k_idx);
extern void random_artifact_resistance (object_type * o_ptr);
extern void random_resistance (object_type * o_ptr, bool is_scroll, s32b specific);
extern s16b floor_carry(s32b y, s32b x, object_type *j_ptr);
extern void pack_decay(s32b item);
extern void floor_decay(object_type *o_ptr);
extern bool scan_floor(s32b *items, s32b *item_num, s32b y, s32b x, s32b mode);
extern void show_floor(s32b y, s32b x);
extern bool get_item_floor(s32b *cp, cptr pmt, cptr str, s32b mode);
extern void py_pickup_floor(s32b pickup);
extern s16b m_bonus(s32b max, s32b level);

/* object3.c */
extern void preserve_artifacts_in_inven(flags_type *inven);
extern void prepare_inventory(flags_type *inventory);
extern object_type *new_object(void);
extern void delete_object(object_type *o_ptr);
extern void delete_object_idx(s32b o_idx);
extern void delete_object_location(s32b y, s32b x);
extern void compact_objects(s32b size);
extern void wipe_o_list(void);
extern s16b o_pop(void);
extern object_type *object_dup(object_type *j_ptr);
extern void object_wipe(object_type *o_ptr);
extern void object_prep(object_type *o_ptr, s32b k_idx);
extern void object_copy(object_type *o_ptr, object_type *j_ptr);
extern void excise_object_idx(s32b o_idx);


/* powers.c */
extern void do_cmd_power(void);

/* traps.c */
extern void activate_trap_door(s32b y, s32b x);
extern void player_activate_door_trap(s16b y, s16b x);
extern void place_trap(s32b y, s32b x, s32b faction);
extern void place_trap_object(object_type *o_ptr);
extern bool mon_hit_trap(s32b m_idx, s32b y, s32b x);
extern bool find_one_trap(flags_type* traps);
extern bool find_one_location_trap(s32b y, s32b x);
extern bool find_one_object_trap(object_type* obj);
extern bool find_all_traps(flags_type* traps);
extern bool find_all_location_traps(s32b y, s32b x);
extern bool find_all_object_traps(object_type* obj);
extern s32b  get_num_traps(flags_type* traps, bool known_traps, s16b faction,
						  bool friendly_traps, bool apparent_traps);
extern s32b  get_num_location_traps(s16b y, s16b x, bool known_traps,
								   s16b faction, bool friendly_traps,
								   bool apparent_traps);
extern s32b  get_num_object_traps(object_type* obj, bool known_traps,
								 s16b faction, bool friendly_traps,
								 bool apparent_traps);
extern bool get_trap(s32b faction_exclude, s32b spell, s16b y, s16b x,
					 s32b* trap_idx, flags_type** trap_out);
extern s32b  disarm_one_location_trap(bool manual, s32b who, s32b y, s32b x);
extern s32b  disarm_one_object_trap(bool manual, s32b who, object_type* o_ptr);
extern bool activate_location_traps(s32b who, s32b y, s32b x,
									bool friendly_traps);
extern bool activate_object_traps(s32b who, s32b y, s32b x, s32b o_idx,
								  bool friendly_traps);

/* spells1.c */
extern byte spell_color(s32b type);
extern s16b poly_r_idx(s32b r_idx);
extern void get_pos_player(s32b dis, s32b *ny, s32b *nx);
extern bool teleport_player_bypass;
extern void teleport_to_player(s32b m_idx);
extern void teleport_player_directed(s32b rad, s32b dir);
extern void teleport_away(s32b m_idx, s32b dis);
extern void teleport_player(s32b dis);
extern void teleport_player_to(s32b ny, s32b nx);
extern void teleport_monster_to(s32b m_idx, s32b ny, s32b nx);
extern void teleport_player_level(void);
extern void recall_player(s32b d, s32b f);
extern void take_hit(s32b damage, cptr kb_str);
extern void acid_dam(s32b dam, cptr kb_str);
extern void elec_dam(s32b dam, cptr kb_str);
extern void fire_dam(s32b dam, cptr kb_str);
extern void cold_dam(s32b dam, cptr kb_str);
extern bool inc_stat(s32b stat);
extern bool dec_stat(s32b stat, s32b amount, s32b mode);
extern bool res_stat(s32b stat, bool full);
extern bool apply_disenchant(s32b mode);
extern bool project_m(s32b who, s32b r, s32b y, s32b x, s32b dam, s32b typ, s32b flg,
					s32b dir);
extern bool project_o(s32b who, s32b r, s32b y, s32b x, s32b dam, s32b typ, s32b flg,
					s32b dir);
extern bool project_f(s32b who, s32b r, s32b y, s32b x, s32b dam, s32b typ, s32b flg,
					s32b dir);
extern bool project_p(s32b who, s32b r, s32b y, s32b x, s32b dam, s32b typ,
					s32b a_rad, s32b flg, s32b dir);
extern s32b project_path(u16b *gp, s32b range, s32b y1, s32b x1, s32b y2, s32b x2, s32b flg);
extern bool project(s32b who, s32b rad, s32b y, s32b x, s32b dam, s32b typ, s32b flg, s32b max_range);
extern bool potion_smash_effect(s32b who, s32b y, s32b x, s32b o_sval);
extern void do_poly_self(void);
extern void corrupt_player(void);
extern void describe_attack_fully(s32b type, char* r);
extern s16b do_poly_monster(s32b y, s32b x);


/* spells2.c */
extern bool remove_curse_object(object_type *o_ptr, bool all);
extern void curse_artifact(object_type * o_ptr);
extern void build_prob(cptr learn);
extern void grow_things(s16b type, s32b rad);
extern void grow_grass(s32b rad);
extern void grow_trees(s32b rad);
extern bool hp_player(s32b num);
extern bool do_dec_stat(s32b stat, s32b mode);
extern bool do_res_stat(s32b stat, bool full);
extern bool do_inc_stat(s32b stat);
extern void make_item_fully_identified(object_type *o_ptr);
extern void identify_pack(void);
extern void identify_pack_fully(void);
extern bool hack_message_pain_may_silent;
extern void message_pain(s32b m_idx, s32b dam);
extern bool remove_curse(void);
extern bool remove_all_curse(void);
extern bool restore_level(void);
extern void self_knowledge(PHYSFS_file *fff);
extern bool lose_all_info(void);
extern bool wall_stone(s32b y, s32b x);
extern bool enchant(object_type *o_ptr, s32b n, s32b eflag);
extern bool enchant_spell(s32b num_hit, s32b num_dam, s32b num_ac, s32b num_pval);
extern bool create_artifact(object_type *o_ptr, bool a_scroll, bool get_name);
extern bool artifact_scroll(void);
extern bool ident_spell(void);
extern bool ident_all(void);
extern bool identify_fully(void);
extern bool recharge(s32b num);
extern bool speed_monsters(void);
extern bool slow_monsters(void);
extern bool sleep_monsters(void);
extern bool conf_monsters(void);
extern void aggravate_monsters(s32b who);
extern void wake_monsters(s32b y, s32b x, s32b rad, u32b power);
extern bool genocide_aux(bool player_cast, char typ);
extern bool genocide(bool player_cast);
extern bool mass_genocide(bool player_cast);
extern void do_probe(s32b m_idx);
extern bool probing(void);
extern void change_wild_mode(void);
extern bool banish_evil(s32b dist);
extern bool dispel_evil(s32b dam);
extern bool dispel_good(s32b dam);
extern bool dispel_undead(s32b dam);
extern bool dispel_monsters(s32b dam);
extern bool dispel_living(s32b dam);
extern bool dispel_demons(s32b dam);
extern bool turn_undead(void);
extern void wipe(s32b y1, s32b x1, s32b r);
extern void destroy_area(s32b y1, s32b x1, s32b r, bool full, bool bypass);
extern void earthquake(s32b cy, s32b cx, s32b r);
extern void lite_room(s32b y1, s32b x1);
extern void unlite_room(s32b y1, s32b x1);
extern bool fire_ball_beam(s32b typ, s32b dir, s32b dam, s32b rad, s32b max_range);
extern bool fire_cloud(s32b typ, s32b dir, s32b dam, s32b rad, s32b time, s32b speed, s32b max_range);
extern bool fire_wave(s32b typ, s32b dir, s32b dam, s32b rad, s32b time, s32b speed, s32b eff, s32b max_range);
extern bool fire_wall(s32b typ, s32b dir, s32b dam, s32b time, s32b speed, s32b max_range);
extern bool fire_ball(s32b typ, s32b dir, s32b dam, s32b rad, s32b max_range);
extern bool fire_cone(s32b typ, s32b dir, s32b dam, s32b rad, s32b max_range);
extern bool fire_bolt(s32b typ, s32b dir, s32b dam, s32b max_range);
extern bool fire_beam(s32b typ, s32b dir, s32b dam, s32b max_range);
extern bool fire_druid_ball(s32b typ, s32b dir, s32b dam, s32b rad, s32b max_range);
extern bool fire_druid_bolt(s32b typ, s32b dir, s32b dam, s32b max_range);
extern bool fire_druid_beam(s32b typ, s32b dir, s32b dam, s32b max_range);
extern void call_chaos(void);
extern bool fire_bolt_or_beam(s32b prob, s32b typ, s32b dir, s32b dam, s32b max_range);
extern bool lite_line(s32b dir);
extern bool drain_life(s32b dir, s32b dam);
extern bool death_ray(s32b dir, s32b plev);
extern bool wall_to_mud(s32b dir);
extern bool destroy_door(s32b dir);
extern bool wizard_lock(s32b dir);
extern bool heal_monster(s32b dir);
extern bool speed_monster(s32b dir);
extern bool slow_monster(s32b dir);
extern bool sleep_monster(s32b dir);
extern bool stasis_monster(s32b dir);    /* Like sleep, affects undead as well */
extern bool confuse_monster(s32b dir, s32b plev);
extern bool stun_monster(s32b dir, s32b plev);
extern bool fear_monster(s32b dir, s32b plev);
extern bool scare_monsters(void);
extern bool poly_monster(s32b dir);
extern bool clone_monster(s32b dir);
extern bool teleport_monster(s32b dir);
extern bool door_creation(void);
extern bool trap_creation(void);
extern bool destroy_doors_touch(void);
extern bool destroy_traps_touch(void);
extern bool sleep_monsters_touch(void);
extern bool alchemy(void);
extern void activate_foul_curse(void);
extern void activate_morg_curse(void);
extern void activate_hi_summon(void);
extern void summon_cyber(void);
extern void wall_breaker(void);
extern void bless_weapon(void);
extern bool confuse_monsters(s32b dam);
extern bool charm_monsters(s32b dam);
extern bool charm_animals(s32b dam);
extern bool charm_demons(s32b dam);
extern bool stun_monsters(s32b dam);
extern bool stasis_monsters(s32b dam);
extern bool banish_monsters(s32b dist);
extern bool turn_monsters(s32b dam);
extern bool turn_evil(s32b dam);
extern bool deathray_monsters(void);
extern bool charm_monster(s32b dir, s32b plev);
extern bool star_charm_monster(s32b dir, s32b plev);
extern bool control_one_undead(s32b dir, s32b plev);
extern bool charm_animal(s32b dir, s32b plev);
extern bool mindblast_monsters(s32b dam);
extern void get_table_name(char * out_string);
extern s32b flag_cost(object_type * o_ptr);
extern void alter_reality(void);
extern void report_magics(void);
extern void teleport_swap(s32b dir);
extern void swap_position(s32b lty, s32b ltx);
extern bool item_tester_hook_recharge(object_type *o_ptr);
extern bool fire_explosion(s32b y, s32b x, s32b typ, s32b rad, s32b dam, s32b max_range);
extern bool fire_godly_wrath(s32b y, s32b x, s32b typ, s32b dir, s32b dam, s32b max_range);
extern s32b  get_activation_power(void);
extern bool invoke(s32b dam, s32b typ);
extern bool project_hack(s32b typ, s32b dam);
extern void project_meteor(s32b radius, s32b typ, s32b dam, u32b flg);
extern bool item_tester_hook_artifactable(object_type *o_ptr);
extern bool passwall(s32b dir, bool safe);
extern bool project_hook(s32b typ, s32b dir, s32b dam, s32b flg, s32b max_range);
extern void random_misc (object_type * o_ptr, bool is_scroll);
extern void random_plus(object_type * o_ptr, bool is_scroll);
extern bool reset_recall(bool no_trepas_max_depth);
extern void remove_morg_curse(void);

/* store.c */
extern store_type *store_type_new();
extern void store_type_del(store_type *store);
extern store_type *store_type_dup(store_type *store);

extern void do_cmd_store(void);
/*DGDGDGDGDGextern bool is_blessed(object_type *o_ptr);
extern void store_shuffle(s32b which);
extern void store_maint(s32b town_num, s32b store_num);
extern void store_init(s32b town_num, s32b store_num);
extern void move_to_black_market(object_type * o_ptr);
extern void do_cmd_home_trump(void);
extern void store_sell(void);
extern void store_purchase(void);
extern void store_examine(void);
extern void store_stole(void);
extern void store_prt_gold(void);
extern void store_request_item(void);*/

/* bldg.c -KMW- */
extern bool bldg_process_command(store_type *s_ptr, s32b i);
extern void show_building(store_type *s_ptr);
extern bool is_state(store_type *s_ptr, s32b state);
extern void do_cmd_bldg(void);
extern void enter_quest(void);
extern void select_bounties(void);

/* util.c */
extern void sdl_utl_set_fast_draw_delay(s32b delay);
extern void sdl_utl_fast_draw(bool enable);
extern s32b color_char_to_attr(char c);
extern byte conv_color[16];
extern void darken_screen(byte color);
extern s32b sroot(s32b n);
extern s32b find_power(cptr name);
extern s32b find_player_descriptor(cptr name);
extern void scansubdir(cptr dir);
extern s32b rescale(s32b x, s32b max, s32b new_max);
extern bool input_box(cptr text, s32b y, s32b x, char *buf, s32b max);
extern void draw_box(s32b y, s32b x, s32b h, s32b w);
extern void display_list(s32b y, s32b x, s32b h, s32b w, cptr title,
                         cptr *list, s32b max, s32b begin, s32b sel,
                         byte sel_color, bool show_scrollbar);
extern s32b value_scale(s32b value, s32b vmax, s32b max, s32b min);
extern s32b ask_menu(cptr ask, char **items, s32b max);
extern cptr get_date_string(s32b turn);
extern cptr get_note_date_string(s32b turn);
extern cptr get_long_note_date_string(s32b turn);
extern s32b bst(s32b what, s32b t);
extern bool is_daylight(s32b turn);
extern bool is_dawn(s32b turn);
extern bool is_dusk(s32b turn);
extern errr path_parse(char *buf, s32b max, cptr file);
extern errr path_temp(char *buf, s32b max);
extern errr path_build(char *buf, s32b max, cptr path, cptr file);
extern PHYSFS_file *my_fopen(cptr file, cptr mode);
extern errr my_str_fgets(cptr full_text, char *buf, size_t n);
extern char *my_physfs_fgets(char *buf, size_t n, PHYSFS_file *fff);
extern s32b my_fprintf(PHYSFS_file *stream, const char *format, ...);
extern s32b my_physfs_fputc(s32b c, PHYSFS_file *stream);
extern s32b my_physfs_fgetc(PHYSFS_file *stream);
extern errr my_fgets(PHYSFS_file *fff, char *buf, size_t n);
extern errr my_fputs(PHYSFS_file *fff, cptr buf);
extern errr my_fclose(PHYSFS_file *fff);
extern bool convert_virtual_path(char *buf, s32b size);
extern errr fd_kill(cptr file);
extern void fd_kill_compress(cptr name);
extern void flush(void);
extern void bell(void);
extern void sound(s32b num);
extern void move_cursor(s32b row, s32b col);
extern void text_to_ascii(char *buf, cptr str);
extern void ascii_to_text(char *buf, cptr str);
extern void keymap_init(void);
extern errr macro_add(cptr pat, cptr act);
extern s32b macro_find_exact(cptr pat);
extern s32b inkey(void);
extern bool keycode_is_directional(char* keycode);
extern cptr quark_str(s16b num);
extern s16b quark_add(cptr str);
extern s16b message_num(void);
extern cptr message_str(s32b age);
extern byte message_color(s32b age);
extern byte message_type(s32b age);
extern void message_add(byte type, cptr msg, byte color);
extern s16b message_row;
extern s16b message_col;
extern s16b message_col_max;
extern void display_message(s32b x, s32b y, s32b split, byte color, cptr t);
extern void cmsg_print(byte color, cptr msg);
extern void cmsg_print_last();
extern void msg_print(cptr msg);
extern void cmsg_format(byte color, cptr fmt, ...);
extern void msg_format(cptr fmt, ...);
extern void screen_save(void);
extern void screen_load(void);
extern s32b screen_depth(void);
extern void c_put_str(byte attr, cptr str, s32b row, s32b col);
extern void put_str(cptr str, s32b row, s32b col);
extern void c_prt(byte attr, cptr str, s32b row, s32b col);
extern void prt(cptr str, s32b row, s32b col);
extern void text_out_to_screen(byte a, cptr str);
extern void text_out_to_file(byte a, cptr str);
extern void text_out(cptr str);
extern void text_out_c(byte a, cptr str);
extern void clear_screen(void);
extern void clear_from(s32b row);
extern bool askfor_aux_complete;
extern bool askfor_aux(char *buf, s32b len);
extern bool get_string(cptr prompt, char *buf, s32b len);
extern bool get_check(cptr prompt);
extern bool get_check_aux(cptr prompt, bool keep_cursor);
extern bool get_com(cptr prompt, s32b *command);
extern bool get_com_aux(cptr prompt, s32b *command, bool keep_cursor);
extern s32b get_quantity(cptr prompt, s32b max);
extern void pause_line(s32b row);
extern void request_command();
extern bool is_a_vowel(s32b ch);
extern s32b get_keymap_dir(char ch);
extern s32b line_to_dir(s32b starty, s32b staryx, s32b endy, s32b endx);
extern byte count_bits(u32b array);
extern void strlower(char *buf);
extern s32b test_monster_name(cptr name);
extern s32b test_mego_name(cptr name);
extern s32b test_item_name(cptr name);
extern s32b msg_box(cptr text, s32b y, s32b x);
extern timer_type *new_timer(cptr callback, s32b delay);
extern void del_timer(timer_type *t_ptr);
extern buffer_type *data_buffer_alloc();
extern void data_buffer_delete(buffer_type *buf);
extern buffer_type *data_buffer_dup(buffer_type *buf);
extern void data_buffer_loadsave(byte type, buffer_type **buf, s32b flag);
extern bool check_prevent_cmd(object_type* o_ptr);
extern bool check_prevent_cmd_aux(object_type* o_ptr, char cmd);

/* xtra1.c */
extern void fix_irc_message(void);
extern void fix_message(void);
extern void apply_flags(flags_type *f, s16b tval, s16b to_h, s16b to_d, s16b to_a);
extern s32b luck(s32b min, s32b max);
extern s32b weight_limit(void);
extern bool calc_powers_silent;
extern void cnv_stat(s32b i, char *out_val);
extern s16b modify_stat_value(s32b value, s32b amount);
extern void notice_stuff(void);
extern void update_stuff(void);
extern void redraw_stuff(void);
extern void window_stuff(void);
extern void handle_stuff(void);
extern bool monk_empty_hands(void);
extern bool monk_heavy_armor(void);
extern bool beastmaster_whip(void);
extern void calc_bonuses(bool silent);
extern void calc_sanity(void);
extern bool change_player_body(monster_type* new_body, monster_type* old_body);

/* xtra2.c */
extern s32b resolve_mimic_name(cptr name);
extern void do_rebirth(void);
extern cptr get_subrace_title(s32b racem);
extern void set_subrace_title(s32b racem, cptr name);
extern void switch_subrace(s32b racem, bool copy_old);
extern void switch_class(s32b sclass);
extern void switch_subclass(s32b sclass);
extern void drop_from_wild(void);
extern void clean_wish_name(char *buf, char *name);
extern bool test_object_wish(char *name, object_type *o_ptr, char *what);
extern void check_experience(void);
extern void check_experience_obj(object_type *o_ptr);
extern void gain_exp(s32b amount);
extern void lose_exp(s32b amount);
extern s32b get_coin_type(monster_race *r_ptr);
extern void monster_death(s32b m_idx, s32b who);
extern bool mon_take_hit(s32b m_idx, s32b dam, bool *fear, cptr note);
extern bool change_panel(s32b dy, s32b dx);
extern void verify_panel(void);
extern void panel_bounds(void);
extern void resize_map(void);
extern void resize_window(void);
extern cptr look_mon_desc(s32b m_idx);
extern void ang_sort_aux(vptr u, vptr v, s32b p, s32b q);
extern void ang_sort(vptr u, vptr v, s32b n);
extern bool target_able(s32b m_idx);
extern bool target_okay(void);
extern bool target_set(s32b mode);
extern bool get_aim_dir(s32b *dp);
extern bool get_hack_dir(s32b *dp);
extern bool get_rep_dir(s32b *dp);
extern void gain_level_reward(s32b chosen_reward);
extern bool set_shadow(s32b v);
extern bool set_tim_esp(s32b v);
extern bool tgt_pt(s32b *x, s32b * y);
extern bool tgt_pt_prompt(cptr prompt, s32b *x, s32b *y);
extern bool gain_random_corruption(s32b choose_mut);
extern bool got_corruptions(void);
extern void dump_corruptions(PHYSFS_file *OutFile, bool color);
extern void do_poly_self(void);
extern void do_poly_wounds(void);
extern bool curse_weapon(void);
extern bool curse_armor(void);
extern void random_resistance(object_type * q_ptr, bool is_scroll, s32b specific);
extern bool lose_corruption(s32b choose_mut);
extern bool lose_all_corruptions(void);
extern void great_side_effect(void);
extern void nasty_side_effect(void);
extern void deadly_side_effect(bool god);
extern void godly_wrath_blast(void);
extern s32b interpret_grace(void);
extern s32b interpret_favor(void);
extern void make_wish(void);
extern bool set_sliding(s16b v);
extern void create_between_gate(s32b dist, s32b y, s32b x);

/* levels.c */
extern bool get_dungeon_generator(char *buf);
extern bool get_level_desc(char *buf);
extern void get_level_flags(void);
extern bool get_dungeon_name(char *buf);
extern bool get_dungeon_special(char *buf);
extern s32b get_branch(void);
extern s32b get_fbranch(void);
extern s32b get_flevel(void);

/* ghost.c */
extern s16b place_ghost(void);
extern void make_bones(void);


/* wizard2.c */
extern void do_cmd_wiz_cure_all(void);
extern void do_cmd_wiz_named_friendly(s32b r_idx, bool slp);
extern void do_cmd_debug(void);

/* notes.c */
extern void show_notes_file(void);
extern void output_note(char *final_note);
extern void add_note(char *note, char code);
extern void add_note_type(s32b note_number);

/* squeltch.c */
extern void squeltch_inventory(void);
extern void squeltch_grid(void);
extern void do_cmd_automatizer(void);
extern void automatizer_add_rule(object_type *o_ptr, bool destroy);
extern bool automatizer_create;



/*
 * Hack -- conditional (or "bizarre") externs
 */

/* util.c */
extern void user_name(char *buf, s32b id);

#ifndef HAS_STRICMP
/* util.c */
extern s32b stricmp(cptr a, cptr b);
#endif

#ifdef SUPPORT_GAMMA
/* util.c */
extern void build_gamma_table(s32b gamma);
extern byte gamma_table[256];

/* variable.c */
extern u16b gamma_val;
#endif

#ifdef USE_MACOSX
extern char *macToMEConfigPath;
#endif

#ifdef USE_WINDOWS
/* main-win.c */
/* extern s32b FAR PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, ...); */
#endif

#if !defined(USE_WINDOWS)
/* files.c */
extern bool chg_to_txt(cptr base, cptr newname);
#endif /* !USE_WINDOWS */

#ifdef ALLOW_REPEAT /* TNB */

/* util.c */
extern s32b repeat__idx;
extern s32b repeat__cnt;
extern void repeat_pop();
extern void repeat_push(s32b what);
extern bool repeat_pull(s32b *what);
extern void repeat_check(void);
extern void get_count(s32b number, s32b max);

#endif /* ALLOW_REPEAT -- TNB */

#ifdef ALLOW_EASY_OPEN /* TNB */

/* variable.c */
extern bool easy_open;
extern bool easy_tunnel;

/* cmd2.c */
extern bool easy_open_door(s32b y, s32b x);

#endif /* ALLOW_EASY_OPEN -- TNB */

#ifdef ALLOW_EASY_DISARM /* TNB */

/* variable.c */
extern bool easy_disarm;

extern bool easy_inven;

/* cmd2.c */
extern bool do_cmd_disarm_aux(s32b y, s32b x, s32b dir, s32b do_pickup);

#endif /* ALLOW_EASY_DISARM -- TNB */

extern bool easy_floor;


extern void irc_poll(void);
extern void irc_connect(void);
extern void irc_emote(char *buf);
extern void irc_chat(void);
extern void irc_disconnect(void);
extern void irc_disconnect_aux(char *str, bool message);
extern void irc_quit(char *str);


/* script.c */
extern s32b lua_bytecode_dump(lua_State *L);
extern void save_perm_data(void);
extern s32b tome_compile_lua(char *src);
extern byte is_lua_started(void);
extern void init_lua(void);
extern void init_lua_init(void);
extern s32b exec_lua(cptr file);
extern cptr string_exec_lua(cptr file);
extern bool tome_dofile(char *file, bool bypass_redirection);
extern bool tome_dofile_anywhere(cptr dir, char *file, bool test_exist, bool bypass_redirection);
extern void dump_lua_stack(s32b min, s32b max);
extern void open_lua_functions_registry(s32b domain, s32b ref);
extern void close_lua_functions_registry();
extern s32b get_lua_functions_registry_domain(cptr name);
extern bool call_lua(cptr function, cptr args, cptr ret, ...);
extern bool get_lua_var(cptr name, char type, void *arg);

/* modules.c */
extern cptr force_module;
extern bool select_module(cptr force_module);
extern bool module_initialized;


/* lua_bind.c */
#ifdef USE_SDL
extern SDL_TimerID rt_timer_add(s32b interval, void *c_callback);
void rt_timer_del(SDL_TimerID id);
#endif
extern s16b add_new_inventory_slot(cptr desc, cptr name, cptr describe, cptr wear, cptr takeoff);
extern magic_power *grab_magic_power(magic_power *m_ptr, s32b num);
extern bool get_magic_power(s32b *sn, magic_power *powers, s32b max_powers, void (*power_info)(char *p, s32b power), s32b plev, s32b cast_stat);
extern magic_power *new_magic_power(s32b num);
extern bool get_magic_power_lua(s32b *sn, magic_power *powers, s32b max_powers, char *info_fct, s32b plev, s32b cast_stat);
extern bool lua_spell_success(magic_power *spell, s32b fail_adj, s32b fail_min, char *oups_fct);

extern object_type *new_object(void);
extern void end_object(object_type *o_ptr);
extern void lua_set_item_tester(s32b tval, char *fct);
extern void lua_set_get_obj_num_hook(char *fct);
extern char *lua_object_desc_store(object_type *o_ptr, s32b pref, s32b mode);

extern s16b    add_new_power(cptr name, cptr desc, cptr gain, cptr lose, byte level, byte cost, byte stat, byte diff);

extern void find_position(s32b y, s32b x, s32b *yy, s32b *xx);
extern bool summon_lua_okay(s32b r_idx);
extern bool lua_summon_monster(s32b y, s32b x, s32b lev, s32b faction,
			bool group_ok, char *fct);

extern s16b add_new_quest(char *name);
extern void quest_set_desc(s32b q_idx, s32b d, char *desc);
extern cptr quest_get_desc(s32b q_idx, s32b line);

extern bool get_com_lua(cptr promtp, s32b *com, bool keep_cursor);

extern s32b lua_get_level(s32b s, s32b lvl, s32b max, s32b min, s32b bonus);
extern s32b lua_spell_chance(s32b chance, s32b level, s32b skill_level, s32b mana, s32b cur_mana, s32b fail_adj, s32b fail_min);
extern s32b lua_spell_device_chance(s32b skill, s32b chance, s32b level, s32b base_level);

extern cave_type *lua_get_cave(s32b y, s32b x);
extern void set_target(s32b y, s32b x);
extern void get_target(s32b dir, s32b *y, s32b *x);

extern bool alloc_room(s32b by0, s32b bx0, s32b ysize, s32b xsize, s32b *y1, s32b *x1, s32b *y2, s32b *x2);

extern void lua_print_hook(cptr str);

extern char *lua_input_box(cptr title, s32b max, cptr def);
extern s32b lua_msg_box(cptr title);

extern list_type *lua_create_list(s32b size);
extern void lua_delete_list(list_type *, s32b size);
extern void lua_add_to_list(list_type *, s32b idx, cptr str);
extern void lua_display_list(s32b y, s32b x, s32b h, s32b w, cptr title,
                             list_type *list, s32b max, s32b begin,
                             s32b sel, byte sel_color, bool show_scrollbar);

extern bool lua_make_temp_file(void);
extern errr lua_write_temp_file(cptr str);
extern void lua_close_temp_file(void);
extern void lua_end_temp_file(void);
extern cptr lua_get_temp_name(void);

extern void add_scripted_generator(cptr name, bool stairs, bool monsters, bool objects, bool miscs);
extern cptr lua_apply_keymap(s32b ch);

/* skills.c */
extern void dump_skills(PHYSFS_file *fff);
extern s16b find_skill(cptr name);
extern s16b find_skill_i(cptr name);
extern s32b get_skill_raw(s32b skill);
extern s32b get_skill(s32b skill);
extern s32b get_skill_scale(s32b skill, s32b scale);
extern void do_cmd_skill(void);
extern void do_cmd_activate_skill(s32b type);
extern s16b melee_skills[MAX_MELEE];
extern char *melee_names[MAX_MELEE];
extern s16b get_combat_skills(void);
extern s16b get_combat_skill(void);
extern void compute_skills(s32b *v, s32b *m, s32b i);
extern void select_default_combat(void);
extern void do_get_new_skill(void);
extern void init_skill(s32b value, s32b mod, s32b i);
extern s16b find_ability(cptr name);
extern void dump_abilities(PHYSFS_file *fff);
extern void do_cmd_ability(void);
extern bool has_ability(s32b ab);
extern void apply_level_abilities(s32b level);
extern void recalc_skills(bool init);

/* flags.c */
extern u32b register_flag(cptr name);
extern flag_registry *find_flag(u32b index);
extern flag_registry *find_flag_by_name(cptr name);
extern u32b get_flag_index(cptr name);
extern cptr get_flag_name(u32b index);
extern void flag_set(flags_type *t, u32b key, s32b value);
extern void flag_set_full(flags_type *t, u32b key, byte flags, s32b value, s32b value2, flags_type *value_flags);
#define flag_set_flags(t, key, value_flags) (flag_set_full(t, key, FLAG_FLAG_FLAGS, 0, 0, value_flags))
extern void flag_remove(flags_type *t, u32b key);
extern void flag_remove_zap(flags_type *t, u32b key);
extern void flag_remove_rand(flags_type *t);
extern void flag_remove_rand_zap(flags_type *t);
extern s32b flag_get(flags_type *t, u32b key);
extern s32b flag_get2(flags_type *t, u32b key);
extern void flag_learn(flags_type *t, u32b key, bool learn);
extern bool flag_is_known(flags_type *t, u32b key);
extern void flag_learn_all(flags_type *t);
extern flags_type *flag_get_flags(flags_type *t, u32b key);
extern flag_node *flag_get_node(flags_type *t, u32b key);
extern bool flag_exists(flags_type *t, u32b key);
extern flags_type *flag_new(s32b size);
extern void flag_init(flags_type *t, s32b size);
extern void flag_free(flags_type *t, bool full);
extern void flags_print(flags_type *t);
extern u32b flag_first_free_key(flags_type *t, u32b min);
extern u32b flag_max_key(flags_type *t);
extern void flag_empty(flags_type *t);
extern void flag_empty_zap(flags_type *t);
extern bool flag_intersects(flags_type *f1, flags_type *f2);
extern bool flag_intersects_where(flags_type *f1, flags_type *f2, u32b *where);
extern flags_type *flag_dup(flags_type *src);
extern void flag_copy(flags_type *dest, flags_type *src);
extern flags_type *flag_shallow_dup(flags_type *src);
extern void flag_shallow_copy(flags_type *dest, flags_type *src);
extern void flag_shallow_free(flags_type *t, bool full);
extern bool flag_contains(flags_type *f1, flags_type *f2);
extern u32b flag_contains_nb(flags_type *f1, flags_type *f2);
extern bool flag_equal(flags_type *dest, flags_type *src);
extern s32b flag_used(flags_type *t);
extern s16b flags_mbonus_level;
extern void flag_add(flags_type *dest, flags_type *src);
extern void flag_add_new(flags_type *dest, flags_type *src);
extern void flag_add_value_from(flags_type *dest, flags_type *src, flags_type *from);
extern void flag_del(flags_type *dest, flags_type *src);
extern u32b flag_get_rand(flags_type *t);
extern void flag_add_increase(flags_type *dest, flags_type *src);
extern void flag_add_higher(flags_type *dest, flags_type *src);
extern void setup_flags(void);
extern void *flag_get_ptr(flags_type *t, u32b key, byte type);
extern void flag_set_ptr(flags_type *t, u32b key, byte type, void *ptr);
#define flag_get_obj(t, key) (object_type*)flag_get_ptr(t, key, FLAG_TYPE_OBJ)
#define flag_set_obj(t, key, o_ptr) flag_set_ptr(t, key, FLAG_TYPE_OBJ, (void*)o_ptr)
#define flag_get_string(t, key) (cptr)flag_get_ptr(t, key, FLAG_TYPE_STRING)
#define flag_set_string(t, key, str) flag_set_ptr(t, key, FLAG_TYPE_STRING, (void*)str)
#define flag_get_monster(t, key) (monster_type*)flag_get_ptr(t, key, FLAG_TYPE_MONSTER)
#define flag_set_monster(t, key, monst) flag_set_ptr(t, key, FLAG_TYPE_MONSTER, (void*)monst)
#define flag_get_blow(t, key) (monster_blow*)flag_get_ptr(t, key, FLAG_TYPE_MONSTER_BLOW)
#define flag_set_blow(t, key, blow) flag_set_ptr(t, key, FLAG_TYPE_MONSTER_BLOW, (void*)blow)
#define flag_get_buffer(t, key) (buffer_type*)flag_get_ptr(t, key, FLAG_TYPE_DATA_BUFFER)
#define flag_set_buffer(t, key, str) flag_set_ptr(t, key, FLAG_TYPE_DATA_BUFFER, (void*)str)
#define flag_get_store(t, key) (store_type*)flag_get_ptr(t, key, FLAG_TYPE_STORE)
#define flag_set_store(t, key, store) flag_set_ptr(t, key, FLAG_TYPE_STORE, (void*)store)

/* vector.c */
extern void vector_push(vector_type *v, s32b i);
extern s32b vector_pop(vector_type *v);
extern s32b vector_peek(vector_type *v);
extern u32b vector_size(vector_type *v);
extern bool vector_is_empty(vector_type *v);
#define vector_get(v, i) flag_get(v, i)
#define vector_new() flag_new(2)
#define vector_free(v) flag_free(v, TRUE)



/* speed.c */
extern cptr get_speed_name(u32b index);
extern cptr get_speed_desc(u32b index);
extern u32b get_speed_index(cptr name);
extern u32b register_speed(cptr name, cptr desc);
extern void setup_speeds();
extern u16b get_player_energy(s32b speed_id);
extern u16b get_monster_energy(monster_type *m_ptr, s32b speed_id);



/* pathfind.c */
extern bool pathfind_get_first(monster_type *m_ptr, s32b start_y, s32b start_x, s32b goal_y, s32b goal_x, s32b *res_y, s32b *res_x, bool *complete);
extern bool pathfind_get_next(s32b *res_y, s32b *res_x);



/* net.c */
extern void init_net_subsystem(void);

/* Export flags */
#define new_flag(n) \
	extern u32b FLAG_##n
#include "flags.h"
#undef new_flag

/* Export speeds */
#define new_speed(n,d) \
	extern u32b SPEED_##n
#include "speed.h"
#undef new_speed
