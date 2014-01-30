/* File: generate.c */

/* Purpose: Dungeon generation */

/*
 * Copyright (c) 1989 James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"
#include <math.h>
#define SAFE_MAX_ATTEMPTS 5000
/*#define EMPTY_FEAT -1*/

/* FIXME This value is buggy but for some reason it prevents crashes. */
#define EMPTY_FEAT 65536

/*
 * Note that Level generation is *not* an important bottleneck,
 * though it can be annoyingly slow on older machines...  Thus
 * we emphasize "simplicity" and "correctness" over "speed".
 *
 * This entire file is only needed for generating levels.
 * This may allow smart compilers to only load it when needed.
 *
 * Consider the "v_info.txt" file for vault generation.
 *
 * In this file, we use the "special" granite and perma-wall sub-types,
 * where "basic" is normal, "inner" is inside a room, "outer" is the
 * outer wall of a room, and "solid" is the outer wall of the dungeon
 * or any walls that may not be pierced by corridors.  Thus the only
 * wall type that may be pierced by a corridor is the "outer granite"
 * type.  The "basic granite" type yields the "actual" corridors.
 *
 * Note that we use the special "solid" granite wall type to prevent
 * multiple corridors from piercing a wall in two adjacent locations,
 * which would be messy, and we use the special "outer" granite wall
 * to indicate which walls "surround" rooms, and may thus be "pierced"
 * by corridors entering or leaving the room.
 *
 * Note that a tunnel which attempts to leave a room near the "edge"
 * of the dungeon in a direction toward that edge will cause "silly"
 * wall piercings, but will have no permanently incorrect effects,
 * as long as the tunnel can *eventually* exit from another side.
 * And note that the wall may not come back into the room by the
 * hole it left through, so it must bend to the left or right and
 * then optionally re-enter the room (at least 2 grids away).  This
 * is not a problem since every room that is large enough to block
 * the passage of tunnels is also large enough to allow the tunnel
 * to pierce the room itself several times.
 *
 * Note that no two corridors may enter a room through adjacent grids,
 * they must either share an entryway or else use entryways at least
 * two grids apart.  This prevents "large" (or "silly") doorways.
 *
 * To create rooms in the dungeon, we first divide the dungeon up
 * into "blocks" of 11x11 grids each, and require that all rooms
 * occupy a rectangular group of blocks.  As long as each room type
 * reserves a sufficient number of blocks, the room building routines
 * will not need to check bounds.  Note that most of the normal rooms
 * actually only use 23x11 grids, and so reserve 33x11 grids.
 *
 * Note that the use of 11x11 blocks (instead of the old 33x11 blocks)
 * allows more variability in the horizontal placement of rooms, and
 * at the same time has the disadvantage that some rooms (two thirds
 * of the normal rooms) may be "split" by panel boundaries.  This can
 * induce a situation where a player is in a room and part of the room
 * is off the screen.  It may be annoying enough to go back to 33x11
 * blocks to prevent this visual situation.
 *
 * Note that the dungeon generation routines are much different (2.7.5)
 * and perhaps "DUN_ROOMS" should be less than 50.
 *
 * XXX XXX XXX Note that it is possible to create a room which is only
 * connected to itself, because the "tunnel generation" code allows a
 * tunnel to leave a room, wander around, and then re-enter the room.
 *
 * XXX XXX XXX Note that it is possible to create a set of rooms which
 * are only connected to other rooms in that set, since there is nothing
 * explicit in the code to prevent this from happening.  But this is less
 * likely than the "isolated room" problem, because each room attempts to
 * connect to another room, in a giant cycle, thus requiring at least two
 * bizarre occurances to create an isolated section of the dungeon.
 *
 * Note that (2.7.9) monster pits have been split into monster "nests"
 * and monster "pits".  The "nests" have a collection of monsters of a
 * given type strewn randomly around the room (jelly, animal, or undead),
 * while the "pits" have a collection of monsters of a given type placed
 * around the room in an organized manner (orc, troll, giant, dragon, or
 * demon).  Note that both "nests" and "pits" are now "level dependant",
 * and both make 16 "expensive" calls to the "get_mon_num()" function.
 *
 * Note that the cave grid flags changed in a rather drastic manner
 * for Angband 2.8.0 (and 2.7.9+), in particular, dungeon terrain
 * features, such as doors and stairs and traps and rubble and walls,
 * are all handled as a set of 64 possible "terrain features", and
 * not as "fake" objects (440-479) as in pre-2.8.0 versions.
 *
 * The 64 new "dungeon features" will also be used for "visual display"
 * but we must be careful not to allow, for example, the user to display
 * hidden traps in a different way from floors, or secret doors in a way
 * different from granite walls, or even permanent granite in a different
 * way from granite.  XXX XXX XXX
 */


/*
 * Dungeon generation values
 */
#define DUN_ROOMS      50	/* Number of rooms to attempt */
#define DUN_UNUSUAL   194	/* Level/chance of unusual room (was 200) */
#define DUN_DEST       18	/* 1/chance of having a destroyed level */
#define SMALL_LEVEL     6	/* 1/chance of smaller size (3->6) */
#define EMPTY_LEVEL    15	/* 1/chance of being 'empty' (15)*/
#define DARK_EMPTY      5	/* 1/chance of arena level NOT being lit (2)*/
#define XTRA_MAGIC     10	/* 1/chance of having a level with more magic (10)*/
#define DUN_WILD_VAULT 50	/* Chance of finding a wilderness vault. */
#define DUN_WAT_RNG     2	/* Width of rivers */
#define DUN_WAT_CHG    50	/* 1 in 50 chance of junction in river */
#define DUN_CAVERN     30	/* 1/chance of having a cavern level */

/*
 * Dungeon tunnel generation values
 */
#define DUN_TUN_RND    10	/* Chance of random direction */
#define DUN_TUN_CHG    30	/* Chance of changing direction */
#define DUN_TUN_CON    15	/* Chance of extra tunneling */
#define DUN_TUN_PEN    25	/* Chance of doors at room entrances */
#define DUN_TUN_JCT    90	/* Chance of doors at tunnel junctions */

/*
 * Dungeon streamer generation values
 */
#define DUN_STR_DEN     5	/* Density of streamers */
#define DUN_STR_RNG     2	/* Width of streamers */
#define DUN_STR_MAG     3	/* Number of magma streamers */
#define DUN_STR_MC     90	/* 1/chance of treasure per magma */
#define DUN_STR_QUA	    2	/* Number of quartz streamers */
#define DUN_STR_QC     40	/* 1/chance of treasure per quartz */
#define DUN_STR_SAN     1	/* Number of sand streamers */
#define DUN_STR_SC     10	/* 1/chance of treasure per sandwall */
#define DUN_STR_WLW     1	/* Width of lava & water streamers -KMW- */
#define DUN_STR_DWLW    8	/* Density of water & lava streams -KMW- */


/*
 * Dungeon treausre allocation values
 */
#define DUN_AMT_ROOM    9	/* Amount of objects for rooms */
#define DUN_AMT_ITEM    3	/* Amount of objects for rooms/corridors */
#define DUN_AMT_GOLD    3	/* Amount of treasure for rooms/corridors */

/*
 * Hack -- Dungeon allocation "types"
 */
#define ALLOC_TYP_RUBBLE	1	/* Rubble */
#define ALLOC_TYP_TRAP		3	/* Trap */
#define ALLOC_TYP_GOLD		4	/* Gold */
#define ALLOC_TYP_OBJECT	5	/* Object */


/*
 * The "size" of a "generation block" in grids
 */
#define BLOCK_HGT	11
#define BLOCK_WID	11

/*
 * Maximum numbers of rooms along each axis (currently 6x6)
 */
#define MAX_ROOMS_ROW	(MAX_HGT / BLOCK_HGT)
#define MAX_ROOMS_COL	(MAX_WID / BLOCK_WID)


/*
 * Bounds on some arrays used in the "dun_data" structure.
 * These bounds are checked, though usually this is a formality.
 */
#define CENT_MAX	100
#define DOOR_MAX	200
#define WALL_MAX	500
#define TUNN_MAX	900


/*
 * Maximal number of room types
 */
#define ROOM_MAX		12

/*
 * Simple structure to hold a map location
 */


typedef struct coord coord;

struct coord
{
	byte y;
	byte x;
};


/*
 * Room type information
 */

typedef struct room_data room_data;

struct room_data
{
	/* Required size in blocks */
	s16b dy1, dy2, dx1, dx2;

	/* Hack -- minimum level */
	s16b level;
};


/*
 * Structure to hold all "dungeon generation" data
 */

typedef struct dun_data dun_data;

struct dun_data
{
	/* Array of centers of rooms */
	s32b cent_n;
	coord cent[CENT_MAX];

	/* Array of possible door locations */
	s32b door_n;
	coord door[DOOR_MAX];

	/* Array of wall piercing locations */
	s32b wall_n;
	coord wall[WALL_MAX];

	/* Array of tunnel grids */
	s32b tunn_n;
	coord tunn[TUNN_MAX];

	/* Number of blocks along each axis */
	s32b row_rooms;
	s32b col_rooms;

	/* Array of which blocks are used */
	bool room_map[MAX_ROOMS_ROW][MAX_ROOMS_COL];

	/* Hack -- there is a pit/nest on this level */
	bool crowded;
};

/*
 * Level generator type
 */

typedef struct level_generator_type level_generator_type;
struct level_generator_type
{
	cptr name;
	bool (*generator)(cptr);

	bool default_stairs;
	bool default_monsters;
	bool default_objects;
	bool default_miscs;

	struct level_generator_type *next;
};

static level_generator_type *level_generators = NULL;

/*
 * Add a new generator
 */
void add_level_generator(cptr name, bool (*generator)(cptr name), bool stairs, bool monsters, bool objects, bool miscs)
{
	level_generator_type *g;

	MAKE(g, level_generator_type);
	g->name = string_make(name);
	g->generator = generator;

	g->default_stairs = stairs;
	g->default_monsters = monsters;
	g->default_objects = objects;
	g->default_miscs = miscs;

	g->next = level_generators;
	level_generators = g;
}


/*
 * Dungeon generation data -- see "cave_gen()"
 */
static dun_data *dun;


/*
 * Array of room types depths
 */
static s16b roomdep[] =
{
	0, 	/* 0 = Nothing */
	1, 	/* 1 = Simple (33x11) */
	1, 	/* 2 = Overlapping (33x11) */
	3, 	/* 3 = Crossed (33x11) */
	3, 	/* 4 = Large (33x11) */
	5, 	/* 5 = Monster nest (33x11) */
	5, 	/* 6 = Monster pit (33x11) */
	1, 	/* 7 = Lesser vault (33x22) */
	1, 	/* 8 = Greater vault (66x44) */
	1, 	/* 9 = Circular rooms (22x22) */
	3, 	/* 10 = Fractal cave (42x24) */
	10, 	/* 11 = Random vault (44x22) */
	10, 	/* 12 = Crypts (22x22) */
};


/*
 * Always picks a correct direction
 */
static void correct_dir(s32b *rdir, s32b *cdir, s32b y1, s32b x1, s32b y2, s32b x2)
{
	/* Extract vertical and horizontal directions */
	*rdir = (y1 == y2) ? 0 : (y1 < y2) ? 1 : -1;
	*cdir = (x1 == x2) ? 0 : (x1 < x2) ? 1 : -1;

	/* Never move diagonally */
	if (*rdir && *cdir)
	{
		if (rand_int(100) < 50)
		{
			*rdir = 0;
		}
		else
		{
			*cdir = 0;
		}
	}
}


/*
 * Pick a random direction
 */
static void rand_dir(s32b *rdir, s32b *cdir)
{
	/* Pick a random direction */
	s32b i = rand_int(4);

	/* Extract the dy/dx components */
	*rdir = ddy_ddd[i];
	*cdir = ddx_ddd[i];
}


/*
 * Convert existing terrain type to "up stairs"
 */
static void place_up_stairs(s32b y, s32b x)
{
	/* Create up stairs */
	if ((rand_int(3) != 0) || has_flag(dungeon_flags, FLAG_NO_SHAFT))
	{
		cave_set_feat(y, x, FEAT_LESS);
	}
	else
	{
		cave_set_feat(y, x, FEAT_SHAFT_UP);
	}
}


/*
 * Convert existing terrain type to "down stairs" with dungeon changing.
 */
static void place_magical_stairs(s32b y, s32b x, byte next)
{
	/* Create up stairs */
	cave_set_feat(y, x, FEAT_MORE);
}


/*
 * Convert existing terrain type to "down stairs"
 */
static void place_down_stairs(s32b y, s32b x)
{
	/*
	 * Create down stairs
	 * All thoses tests are necesary because a shaft can jump up to 4 levels
	 */
	if ((dun_level + 4 > d_info[dungeon_type].maxdepth) ||
	    (rand_int(3) != 0) || has_flag(dungeon_flags, FLAG_NO_SHAFT))
	{
		cave_set_feat(y, x, FEAT_MORE);
	}
	else
	{
		cave_set_feat(y, x, FEAT_SHAFT_DOWN);
	}

}


/*
 * Helper function for place_new_way. Determine if y, x is one of
 * floor features of the current dungeon
 */
static bool is_safe_floor(s32b y, s32b x)
{
	dungeon_info_type *d_ptr = &d_info[dungeon_type];
	byte feat = cave[y][x].feat;

	/* One of the legal floor types */
	if (feat == d_ptr->floor1) return (TRUE);
	if (feat == d_ptr->floor2) return (TRUE);
	if (feat == d_ptr->floor3) return (TRUE);

	/* Assume non-floor */
	return (FALSE);
}


/*
 * Place a way to next / previoous level on flat places
 */
void place_new_way(s32b *y, s32b *x)
{
	s32b xx, yy;
	s32b x0, x1, x2;
	s32b y0, y1, y2;
	cave_type *c_ptr;
	bool ok;
	s32b  tries;
	s32b i, way_n = 0;
	byte way_x[MAX_WID], way_y[MAX_WID];

	ok    = FALSE;
	tries = 0;

	/* Find valid location XXX XXX XXX */
	while (tries < 3000)
	{
		tries++;
		/* A way on vertical edge */
		if (rand_int(cur_hgt + cur_wid) < cur_hgt)
		{
			/* Pick a random grid */
			yy = *y = rand_int(cur_hgt - 2) + 1;
			xx = *x = 1 + (rand_int(2) * (cur_wid - 3));

			/* Pick a direction */
			if (xx == 1)
			{
				/* Left */
				x0 = + 1;
				y0 = 0;

				/* Sides */
				x1 = 0;
				y1 = -1;
				x2 = 0;
				y2 = + 1;
			}
			else
			{
				/* Right */
				x0 = -1;
				y0 = 0;

				/* Sides */
				x1 = 0;
				y1 = -1;
				x2 = 0;
				y2 = + 1;
			}
		}

		/* A way on horizontal edge */
		else
		{
			/* Pick a random grid */
			xx = *x = rand_int(cur_wid - 2) + 1;
			yy = *y = 1 + (rand_int(2) * (cur_hgt - 3));

			/* Pick a direction */
			if (yy == 1)
			{
				/* Down */
				x0 = 0;
				y0 = + 1;

				/* Sides */
				x1 = -1;
				y1 = 0;
				x2 = + 1;
				y2 = 0;
			}
			else
			{
				/* Up */
				x0 = 0;
				y0 = -1;

				/* Sides */
				x1 = -1;
				y1 = 0;
				x2 = + 1;
				y2 = 0;
			}
		}


		/* Look at the starting location */
		c_ptr = &cave[yy][xx];

		/* Reject locations inside vaults */
		if (c_ptr->info & (CAVE_ICKY)) continue;

		/* Reject permanent features */
		if ((has_flag(&f_info[c_ptr->feat], FLAG_PERMANENT)) &&
		                (has_flag(&f_info[c_ptr->feat], FLAG_FLOOR))) continue;

		/* Reject room walls */
		if ((c_ptr->info & (CAVE_ROOM)) &&
		                (c_ptr->feat == feat_wall_outer)) continue;

		/* Look at a neighbouring edge */
		c_ptr = &cave[yy + y1][xx + x1];

		/* Reject two adjacent ways */
		if ((c_ptr->feat == FEAT_WAY_MORE) ||
		                (c_ptr->feat == FEAT_WAY_LESS)) continue;

		/* Look at the other neighbouring edge */
		c_ptr = &cave[yy + y2][xx + x2];

		/* Reject two adjacent ways */
		if ((c_ptr->feat == FEAT_WAY_MORE) ||
		                (c_ptr->feat == FEAT_WAY_LESS)) continue;

		/* Look ahead */
		c_ptr = &cave[yy + y0][xx + x0];

		/* Reject two adjacent ways -- relatively rare, but this can happen */
		if ((c_ptr->feat == FEAT_WAY_MORE) ||
		                (c_ptr->feat == FEAT_WAY_LESS)) continue;


		/* Reset counter */
		way_n = 0;

		/* Assume bad location */
		ok = FALSE;

		/* Check if it connects to current dungeon */
		while (in_bounds(yy, xx))
		{
#if 1

			/* Check grids ahead */
			if (is_safe_floor(yy + y0, xx + x0)) ok = TRUE;

			/* Check side grids */
			if (is_safe_floor(yy + y1, xx + x1)) ok = TRUE;
			if (is_safe_floor(yy + y2, xx + x2)) ok = TRUE;

#else

			/*
			* This can create unconnected sections if it bumps into a
			* non-penetrating streamer
			*/
			/* Check grids ahead */
			if (cave_floor_bold(yy + y0, xx + x0)) ok = TRUE;

			/* Check side grids */
			if (cave_floor_bold(yy + y1, xx + x1)) ok = TRUE;
			if (cave_floor_bold(yy + y2, xx + x2)) ok = TRUE;

#endif

			/* Connected */
			if (ok) break;

			/* Access grid (ahead) */
			c_ptr = &cave[yy + y0][xx + x0];

			/* Paranoia */
			if (cave_feat_is(c_ptr, FLAG_PERMANENT)) break;

			/*
			 * Hack -- Avoid digging room corner
			 *
			 * CAVEAT: Can't handle situations like this:
			 *
			 *     .....########
			 *     .....########
			 *     ######.....>#
			 *     #############
			 *     .....#
			 *     .....#
			 */
			if (c_ptr->info & (CAVE_ROOM))
			{
				cave_type *c1_ptr = &cave[yy + y0 + y1][xx + x0 + x1];
				cave_type *c2_ptr = &cave[yy + y0 + y2][xx + x0 + x2];

				/* Bend the way */
				if ((c1_ptr->info & (CAVE_ROOM)) &&
				                !(c2_ptr->info & (CAVE_ROOM)))
				{
					way_x[way_n] = xx + x1;
					way_y[way_n] = yy + y1;
					way_n++;
					way_x[way_n] = xx + x1 + x0;
					way_y[way_n] = yy + y1 + y0;
					way_n++;
				}

				/* Bend the way -- the other direction */
				else if ((c2_ptr->info & (CAVE_ROOM)) &&
				                !(c1_ptr->info & (CAVE_ROOM)))
				{
					way_x[way_n] = xx + x2;
					way_y[way_n] = yy + y2;
					way_n++;
					way_x[way_n] = xx + x2 + x0;
					way_y[way_n] = yy + y2 + y0;
					way_n++;
				}

				else
				{
					way_x[way_n] = xx + x0;
					way_y[way_n] = yy + y0;
					way_n++;
				}

				ok = TRUE;
				break;
			}

			/* Remember the location */
			way_x[way_n] = xx + x0;
			way_y[way_n] = yy + y0;
			way_n++;

			/* Advance */
			xx += x0;
			yy += y0;
		}

		/* Accept connected corridor */
		if (ok) break;

		/* Try again, until valid location is found */
	}

	if (!ok) {
		cmsg_print(TERM_VIOLET, "ERROR: Couldn't place stairs!");
		return;
	}

	/* Actually dig a corridor connecting the way to dungeon */
	for (i = 0; i < way_n; i++)
	{
		/* Dig */
		place_floor(way_y[i], way_x[i]);
	}
} /* place_new_way() */


/*
 * Returns random co-ordinates for player/monster/object
 */
bool new_player_spot(s32b branch)
{
	s32b	y, x;
	s32b max_attempts = 5000;

	/* Place the player */
	if ((has_flag(dungeon_flags, FLAG_FLAT)))
	{
		place_new_way(&y, &x);
	}
	else
	{
		while (max_attempts--)
		{
			/* Pick a legal spot */
			y = rand_range(1, cur_hgt - 2);
			x = rand_range(1, cur_wid - 2);

			/* Must be a "naked" floor grid */
			if (!cave_naked_bold(y, x)) continue;

			/* Refuse to start on anti-teleport grids */
			if (cave[y][x].info & (CAVE_ICKY)) continue;

			/* Done */
			break;

		}
	}

	/* Should be -1, actually if we failed... */
	if (max_attempts < 1) return (FALSE);


	/* Save the new player grid */
	p_ptr->py = y;
	p_ptr->px = x;

	/* XXX XXX XXX */
	if (dungeon_stair && !((has_flag(dungeon_flags, FLAG_NO_STAIR))) && dun_level &&
	                (!is_quest(dun_level) || (old_dun_level < dun_level)) && !branch)
	{
		if (old_dun_level < dun_level)
		{
			place_up_stairs(p_ptr->py , p_ptr->px);
			if ((has_flag(dungeon_flags, FLAG_FLAT)))
			{
				cave_set_feat(p_ptr->py, p_ptr->px, FEAT_WAY_LESS);
			}
		}
		else
		{
			place_down_stairs(p_ptr->py , p_ptr->px);
			if ((has_flag(dungeon_flags, FLAG_FLAT)))
			{
				cave_set_feat(p_ptr->py, p_ptr->px, FEAT_WAY_MORE);
			}
		}
	}

	return (TRUE);
}



/*
 * Count the number of walls adjacent to the given grid.
 *
 * Note -- Assumes "in_bounds(y, x)"
 *
 * We count only granite walls and permanent walls.
 */
static s32b next_to_walls(s32b y, s32b x)
{
	s32b	k = 0;

	if (has_flag(&f_info[cave[y + 1][x].feat], FLAG_WALL)) k++;
	if (has_flag(&f_info[cave[y - 1][x].feat], FLAG_WALL)) k++;
	if (has_flag(&f_info[cave[y][x + 1].feat], FLAG_WALL)) k++;
	if (has_flag(&f_info[cave[y][x - 1].feat], FLAG_WALL)) k++;

	return (k);
}



/*
 * Convert existing terrain type to rubble
 */
static void place_rubble(s32b y, s32b x)
{
	/* Create rubble */
	if (FEAT_RUBBLE != -1) cave_set_feat(y, x, FEAT_RUBBLE);
}

/*
 * Place an up/down staircase at given location
 */
static void place_random_stairs(s32b y, s32b x)
{
	/* Paranoia */
	if (!cave_clean_bold(y, x)) return;

	/* Choose a staircase */
	if (!dun_level)
	{
		place_down_stairs(y, x);
	}
	else if (is_quest(dun_level) && (dun_level > 1))
	{
		place_up_stairs(y, x);
	}
	else if (dun_level >= d_info[dungeon_type].maxdepth)
	{
		if (d_info[dungeon_type].next)
		{
			place_magical_stairs(y, x, d_info[dungeon_type].next);
		}
		else
		{
			place_up_stairs(y, x);
		}
	}
	else if (rand_int(100) < 50)
	{
		place_down_stairs(y, x);
	}
	else
	{
		place_up_stairs(y, x);
	}
}

/*
 * Place a normal door at the given location
 */
static void place_closed_door(s32b y, s32b x)
{
	/* Create locked door */
	cave_set_feat(y, x, FEAT_DOOR);
}

/*
 * Place a locked door at the given location
 */
static void place_locked_door(s32b y, s32b x)
{
	/* Create locked door */
	if ((FEAT_DOOR_LOCKED_HEAD != -1) && (FEAT_DOOR_LOCKED_TAIL != -1)) cave_set_feat(y, x, rand_range(FEAT_DOOR_LOCKED_HEAD, FEAT_DOOR_LOCKED_TAIL));
	else place_closed_door(y, x);
}

/*
 * Place a jammed door at the given location
 */
static void place_jammed_door(s32b y, s32b x)
{
	/* Create jammed door */
	if ((FEAT_DOOR_JAMMED_HEAD != -1) && (FEAT_DOOR_JAMMED_TAIL != -1)) cave_set_feat(y, x, rand_range(FEAT_DOOR_JAMMED_HEAD, FEAT_DOOR_JAMMED_TAIL));
	else place_closed_door(y, x);
}


/*
 * Place a secret door at the given location
 */
static void place_secret_door(s32b y, s32b x)
{
	cave_type *c_ptr = &cave[y][x];

	if (FEAT_SECRET == -1)
	{
		place_closed_door(y, x);
		return;
	}

	/* Vaults */
	if (c_ptr->info & CAVE_ICKY)
	{
		c_ptr->mimic = FEAT_WALL_INNER;
	}

	/* Ordinary room -- use current outer or inner wall */
	else if (c_ptr->info & CAVE_ROOM)
	{
		/* Determine if it's inner or outer XXX XXX XXX */
		if ((cave[y - 1][x].info & CAVE_ROOM) &&
		                (cave[y + 1][x].info & CAVE_ROOM) &&
		                (cave[y][x - 1].info & CAVE_ROOM) &&
		                (cave[y][x + 1].info & CAVE_ROOM))
		{
			c_ptr->mimic = feat_wall_inner;
		}
		else
		{
			c_ptr->mimic = feat_wall_outer;
		}
	}
	else
	{
		c_ptr->mimic = fill_type[rand_int(100)];
	}

	/* Create secret door */
	cave_set_feat(y, x, FEAT_SECRET);
}


/*
 * Place a random type of door at the given location
 */
static void place_random_door(s32b y, s32b x)
{
	s32b tmp;

	/* Choose an object */
	tmp = rand_int(1000);

	/* Open doors (300/1000) */
	if (tmp < 300)
	{
		/* Find the feat corresponding to a closed door */
		s32b open = get_flag(&f_info[FEAT_DOOR], FLAG_OPEN_DOOR);

		/* Create open door */
		cave_set_feat(y, x, open);
	}

	/* Broken doors (100/1000) */
	else if (tmp < 400)
	{
		/* Create broken door */
		if (FEAT_BROKEN != -1) cave_set_feat(y, x, FEAT_BROKEN);
		else place_closed_door(y, x);
	}

	/* Secret doors (200/1000) */
	else if (tmp < 600)
	{
		/* Create secret door */
		place_secret_door(y, x);
	}

	/* Closed doors (300/1000) */
	else if (tmp < 900)
	{
		/* Create closed door */
		cave_set_feat(y, x, FEAT_DOOR);
	}

	/* Locked doors (99/1000) */
	else if (tmp < 999)
	{
		/* Create locked door */
		place_locked_door(y, x);
	}

	/* Stuck doors (1/1000) */
	else
	{
		/* Create jammed door */
		place_jammed_door(y, x);
	}
}



/*
 * Places some staircases near walls
 */
static void alloc_stairs(s32b feat, s32b num, s32b walls, s32b branch)
{
	s32b y, x, i, j, flag;
	cave_type *c_ptr;

	/* Place "num" stairs */
	for (i = 0; i < num; i++)
	{
		/* Place some stairs */
		for (flag = FALSE; !flag; )
		{
			/* Try several times, then decrease "walls" */
			for (j = 0; !flag && j <= 3000; j++)
			{
				if ((has_flag(dungeon_flags, FLAG_FLAT)))
				{
					place_new_way(&y, &x);
				}
				else
				{
					/* Pick a random grid */
					y = rand_int(cur_hgt);
					x = rand_int(cur_wid);

					/* Require "naked" floor grid */
					if (!cave_naked_bold(y, x)) continue;

					/* Require a certain number of adjacent walls */
					if (next_to_walls(y, x) < walls) continue;
				}

				/* Access the grid */
				c_ptr = &cave[y][x];

				/* Town -- must go down */
				if (!dun_level)
				{
					/* Clear previous contents, add down stairs */
					if ((has_flag(dungeon_flags, FLAG_FLAT)))
					{
						cave_set_feat(y, x, FEAT_WAY_MORE);
					}
					else if ((rand_int(3) == 0) && (!((has_flag(dungeon_flags, FLAG_NO_SHAFT)))))
					{
						cave_set_feat(y, x, FEAT_SHAFT_DOWN);
					}
					else
					{
						cave_set_feat(y, x, FEAT_MORE);
					}
				}

				/* Quest -- must go up */
				else if ((is_quest(dun_level) && (dun_level >= 1)) ||
				                ((dun_level >= d_info[dungeon_type].maxdepth) &&
				                 (!(has_flag(dungeon_flags, FLAG_FORCE_DOWN)))))
				{
					/* Clear previous contents, add up stairs */
					if ((has_flag(dungeon_flags, FLAG_FLAT)))
					{
						cave_set_feat(y, x, FEAT_WAY_LESS);
					}
					else if ((rand_int(3) == 0) && (!((has_flag(dungeon_flags, FLAG_NO_SHAFT)))))
					{
						cave_set_feat(y, x, FEAT_SHAFT_UP);
					}
					else
					{
						cave_set_feat(y, x, FEAT_LESS);
					}
				}

				/* Requested type */
				else
				{
					/* Clear previous contents, add stairs */
					cave_set_feat(y, x, feat);
				}

				if (branch != 0)
					set_flag(c_ptr, FLAG_LEVEL_CHANGER, branch);

				/* All done */
				flag = TRUE;
			}

			/* Require fewer walls */
			if (walls) walls--;
		}
	}
}

bool find_naked_bold(s32b set, s32b* y, s32b* x)
{
	s32b tries = 0;

	/* Pick a "legal" spot */
	while (tries++ < SAFE_MAX_ATTEMPTS)
	{
		bool room;
		s32b yy, xx;

		/* Location */
		yy = rand_int(cur_hgt);
		xx = rand_int(cur_wid);

		/* Require "naked" floor grid */
		if (!cave_naked_bold(yy, xx)) continue;

		/* Check for "room" */
		room = (cave[yy][xx].info & (CAVE_ROOM)) ? TRUE : FALSE;

		/* Require corridor? */
		if ((set == ALLOC_SET_CORR) && room) continue;

		/* Require room? */
		if ((set == ALLOC_SET_ROOM) && !room) continue;

		/* Accept it */
		*y = yy;
		*x = xx;
		return TRUE;
	}

	/* None found */
	*y = -1;
	*x = -1;
	return FALSE;
}

/*
 * Allocates some objects (using "place" and "type")
 */
static void alloc_object(s32b set, s32b typ, s32b num)
{
	s32b y = 1, x = 1, k;

	/* Place some objects */
	for (k = 0; k < num; k++)
	{
		if (!find_naked_bold(set, &y, &x))
		{
			if (cheat_room)
			{
				msg_format("Warning! Could not place object, type : %d!", typ);
			}

			return;
		}


		/* Place something */
		switch (typ)
		{
		case ALLOC_TYP_RUBBLE:
			{
				place_rubble(y, x);
				break;
			}

		case ALLOC_TYP_TRAP:
			{
				place_trap(y, x, FACTION_DUNGEON);
				break;
			}

		case ALLOC_TYP_GOLD:
			{
				place_gold(y, x);
				break;
			}

		case ALLOC_TYP_OBJECT:
			{
				place_object(y, x, FALSE, FALSE, OBJ_FOUND_FLOOR);
				break;
			}
		}
	}
}


/*
 * The following functions create a rectangle (e.g. outer wall of rooms)
 */
void build_rectangle(s32b y1, s32b x1, s32b y2, s32b x2, s32b feat, s32b info)
{
	s32b y, x;

	/* Top and bottom boundaries */
	for (x = x1; x <= x2; x++)
	{
		cave_set_feat(y1, x, feat);
		cave[y1][x].info |= (info);

		cave_set_feat(y2, x, feat);
		cave[y2][x].info |= (info);
	}

	/* Top and bottom boundaries */
	for (y = y1; y <= y2; y++)
	{
		cave_set_feat(y, x1, feat);
		cave[y][x1].info |= (info);

		cave_set_feat(y, x2, feat);
		cave[y][x2].info |= (info);
	}
}


/*
 * Place water through the dungeon using recursive fractal algorithm
 *
 * Why do those good at math and/or algorithms tend *not* to
 * place any spaces around binary operators? I've been always
 * wondering. This seems almost a unversal phenomenon...
 * Tried to make those conform to the rule, but there may still
 * some left untouched...
 */
void recursive_river(s32b x1, s32b y1, s32b x2, s32b y2, s32b feat1, s32b feat2, s32b width)
{
	s32b dx, dy, length, l, x, y;
	s32b changex, changey;
	s32b ty, tx;


	length = distance(x1, y1, x2, y2);

	if (length > 4)
	{
		/*
		 * Divide path in half and call routine twice.
		 * There is a small chance of splitting the river
		 */
		dx = (x2 - x1) / 2;
		dy = (y2 - y1) / 2;

		if (dy != 0)
		{
			/* perturbation perpendicular to path */
			changex = randint(abs(dy)) * 2 - abs(dy);
		}
		else
		{
			changex = 0;
		}

		if (dx != 0)
		{
			/* perturbation perpendicular to path */
			changey = randint(abs(dx)) * 2 - abs(dx);
		}
		else
		{
			changey = 0;
		}



		/* construct river out of two smaller ones */
		recursive_river(x1, y1, x1 + dx + changex, y1 + dy + changey,
		                feat1, feat2, width);
		recursive_river(x1 + dx + changex, y1 + dy + changey, x2, y2,
		                feat1, feat2, width);

		/* Split the river some of the time -junctions look cool */
		if ((width > 0) && (rand_int(DUN_WAT_CHG) == 0))
		{
			recursive_river(x1 + dx + changex, y1 + dy + changey,
			                x1 + 8 * (dx + changex), y1 + 8 * (dy + changey),
			                feat1, feat2, width - 1);
		}
	}

	/* Actually build the river */
	else
	{
		for (l = 0; l < length; l++)
		{
			x = x1 + l * (x2 - x1) / length;
			y = y1 + l * (y2 - y1) / length;

			for (ty = y - width - 1; ty <= y + width + 1; ty++)
			{
				for (tx = x - width - 1; tx <= x + width + 1; tx++)
				{
					if (!in_bounds(ty, tx)) continue;

					if (cave[ty][tx].feat == feat1) continue;
					if (cave[ty][tx].feat == feat2) continue;

					if (distance(ty, tx, y, x) > rand_spread(width, 1)) continue;

					/* Do not convert permanent features */
					if (cave_perma_bold(ty, tx)) continue;

					/*
					 * Clear previous contents, add feature
					 * The border mainly gets feat2, while the center
					 * gets feat1
					 */
					if (distance(ty, tx, y, x) > width)
					{
						cave_set_feat(ty, tx, feat2);
					}
					else
					{
						cave_set_feat(ty, tx, feat1);
					}

					/* Lava terrain glows */
#if 0 //DGDGDGDDGD
					if ((feat1 == FEAT_DEEP_LAVA) ||
					                (feat1 == FEAT_SHAL_LAVA))
					{
						cave[ty][tx].info |= CAVE_GLOW;
					}
#endif

					/* Hack -- don't teleport here */
					cave[ty][tx].info |= CAVE_ICKY;
				}
			}
		}
	}
}


/*
 * Places water through dungeon.
 */
static void add_river(s32b feat1, s32b feat2)
{
	s32b y2, x2;
	s32b y1 = 0, x1 = 0, wid;


	/* Hack -- Choose starting point */
	y2 = randint(cur_hgt / 2 - 2) + cur_hgt / 2;
	x2 = randint(cur_wid / 2 - 2) + cur_wid / 2;

	/* Hack -- Choose ending point somewhere on boundary */
	switch (randint(4))
	{
	case 1:
		{
			/* top boundary */
			x1 = randint(cur_wid - 2) + 1;
			y1 = 1;
			break;
		}
	case 2:
		{
			/* left boundary */
			x1 = 1;
			y1 = randint(cur_hgt - 2) + 1;
			break;
		}
	case 3:
		{
			/* right boundary */
			x1 = cur_wid - 1;
			y1 = randint(cur_hgt - 2) + 1;
			break;
		}
	case 4:
		{
			/* bottom boundary */
			x1 = randint(cur_wid - 2) + 1;
			y1 = cur_hgt - 1;
			break;
		}
	}
	wid = randint(DUN_WAT_RNG);
	recursive_river(x1, y1, x2, y2, feat1, feat2, wid);
}


/*
 * Places "streamers" of rock through dungeon
 *
 * Note that their are actually six different terrain features used
 * to represent streamers.  Three each of magma and quartz, one for
 * basic vein, one with hidden gold, and one with known gold.  The
 * hidden gold types are currently unused.
 */
static void build_streamer(s32b feat, s32b chance)
{
	s32b i, tx, ty;
	s32b y, x, dir;
	s32b dummy = 0;
	cave_type *c_ptr;


	/* Hack -- Choose starting point */
	y = rand_spread(cur_hgt / 2, 10);
	x = rand_spread(cur_wid / 2, 15);

	/* Choose a random compass direction */
	dir = ddd[rand_int(8)];

	/* Place streamer into dungeon */
	while (dummy < SAFE_MAX_ATTEMPTS)
	{
		dummy++;

		/* One grid per density */
		for (i = 0; i < DUN_STR_DEN; i++)
		{
			s32b d = DUN_STR_RNG;

			/* Pick a nearby grid */
			while (1)
			{
				ty = rand_spread(y, d);
				tx = rand_spread(x, d);
				if (!in_bounds2(ty, tx)) continue;
				break;
			}

			/* Access the grid */
			c_ptr = &cave[ty][tx];

			/* Only convert "granite" walls */
			if ((c_ptr->feat != feat_wall_inner) &&
			                (c_ptr->feat != feat_wall_outer) &&
			                (c_ptr->feat != d_info[dungeon_type].fill_type1) &&
			                (c_ptr->feat != d_info[dungeon_type].fill_type2) &&
			                (c_ptr->feat != d_info[dungeon_type].fill_type3)) continue;

			/* Clear mimic feature to avoid nasty consequences */
			c_ptr->mimic = 0;

			/* Clear previous contents, add proper vein type */
			cave_set_feat(ty, tx, feat);

			/* Hack -- Add some (known) treasure */
			if (rand_int(chance) == 0)
			{
				cave_set_feat(ty, tx, c_ptr->feat + 0x04);
			}
		}

		if (dummy >= SAFE_MAX_ATTEMPTS)
		{
			if (cheat_room)
			{
				msg_print("Warning! Could not place streamer!");
			}
			return;
		}


		/* Advance the streamer */
		y += ddy[dir];
		x += ddx[dir];

		/* Quit before leaving the dungeon */
		if (!in_bounds(y, x)) break;
	}
}



/*
 * Place streams of water, lava, & trees -KMW-
 * This routine varies the placement based on dungeon level
 * otherwise is similar to build_streamer
 */
static void build_streamer2(s32b feat, s32b killwall)
{
	s32b i, tx, ty;
	s32b y, x, dir;
	s32b poolchance;
	cave_type *c_ptr;

	poolchance = randint(10);

	/* Hack -- Choose starting point */
	y = rand_spread(cur_hgt / 2, 10);
	x = rand_spread(cur_wid / 2, 15);

	/* Choose a random compass direction */
	dir = ddd[rand_int(8)];

	/* Place streamer into dungeon */
	if (poolchance > 2)
	{
		while (TRUE)
		{
			/* One grid per density */
			for (i = 0; i < (DUN_STR_DWLW + 1); i++)
			{
				s32b d = DUN_STR_WLW;

				/* Pick a nearby grid */
				while (1)
				{
					ty = rand_spread(y, d);
					tx = rand_spread(x, d);
					if (in_bounds(ty, tx)) break;
				}

				/* Access grid */
				c_ptr = &cave[ty][tx];

				/* Never convert vaults */
				if (c_ptr->info & (CAVE_ICKY)) continue;

				/* Reject permanent features */
				if ((has_flag(&f_info[c_ptr->feat], FLAG_PERMANENT)) &&
				                (has_flag(&f_info[c_ptr->feat], FLAG_FLOOR))) continue;

				/* Avoid converting walls when told so */
				if (killwall == 0)
				{
					if (has_flag(&f_info[c_ptr->feat], FLAG_WALL)) continue;
				}

				/* Clear mimic feature to avoid nasty consequences */
				c_ptr->mimic = 0;

				/* Clear previous contents, add proper vein type */
				cave_set_feat(ty, tx, feat);
			}

			/* Advance the streamer */
			y += ddy[dir];
			x += ddx[dir];

			/* Change direction */
			if (rand_int(20) == 0) dir = ddd[rand_int(8)];

			/* Stop at dungeon edge */
			if (!in_bounds(y, x)) break;
		}
	}

#if 0 //DGDGDGDGDG
	/* Create pool */
	else if ((feat == FEAT_DEEP_WATER) || (feat == FEAT_DEEP_LAVA))
	{
		poolsize = 5 + randint(10);
		mid = poolsize / 2;

		/* One grid per density */
		for (i = 0; i < poolsize; i++)
		{
			for (j = 0; j < poolsize; j++)
			{
				tx = x + j;
				ty = y + i;

				if (!in_bounds(ty, tx)) continue;

				if (i < mid)
				{
					if (j < mid)
					{
						if ((i + j + 1) < mid)
							continue;
					}
					else if (j > (mid + i))
						continue;
				}
				else if (j < mid)
				{
					if (i > (mid + j))
						continue;
				}
				else if ((i + j) > ((mid * 3)-1))
					continue;

				/* Only convert non-permanent features */
				if (has_flag(&f_info[cave[ty][tx].feat], FLAG_PERMANENT)) continue;

				/* Clear mimic feature to avoid nasty consequences */
				cave[ty][tx].mimic = 0;

				/* Clear previous contents, add proper vein type */
				cave_set_feat(ty, tx, feat);
			}
		}
	}
#endif
}



/*
 * Build a destroyed level
 */
static void destroy_level(void)
{
	s32b y1, x1, y, x, k, t, n;

	cave_type *c_ptr;

	/* Note destroyed levels */
	if ((cheat_room) || (p_ptr->precognition)) msg_print("Destroyed Level");

	/* Drop a few epi-centers (usually about two) */
	for (n = 0; n < randint(5); n++)
	{
		/* Pick an epi-center */
		x1 = rand_range(5, cur_wid - 1 - 5);
		y1 = rand_range(5, cur_hgt - 1 - 5);

		/* Big area of affect */
		for (y = (y1 - 15); y <= (y1 + 15); y++)
		{
			for (x = (x1 - 15); x <= (x1 + 15); x++)
			{
				/* Skip illegal grids */
				if (!in_bounds(y, x)) continue;

				/* Extract the distance */
				k = distance(y1, x1, y, x);

				/* Stay in the circle of death */
				if (k >= 16) continue;

				/* Delete the monster (if any) */
				delete_monster(y, x);

				/* Destroy valid grids */
				if (cave_valid_bold(y, x))
				{
					/* Delete objects */
					delete_object_location(y, x);

					/* Access the grid */
					c_ptr = &cave[y][x];

					/* Wall (or floor) type */
					t = rand_int(200);

					/* Granite */
					if (t < 20)
					{
						/* Create granite wall */
						cave_set_feat(y, x, FEAT_WALL_EXTRA);
					}
#if 0//DGDGDGDG
					/* Quartz */
					else if (t < 60)
					{
						/* Create quartz vein */
						cave_set_feat(y, x, FEAT_QUARTZ);
					}

					/* Magma */
					else if (t < 90)
					{
						/* Create magma vein */
						cave_set_feat(y, x, FEAT_MAGMA);
					}

					/* Sand */
					else if (t < 110)
					{
						/* Create sand vein */
						cave_set_feat(y, x, FEAT_SANDWALL);
					}
#endif
					/* Floor */
					else
					{
						/* Create floor */
						place_floor(y, x);
					}

					/* No longer part of a room or vault */
					c_ptr->info &= ~(CAVE_ROOM | CAVE_ICKY);

					/* No longer illuminated or known */
					c_ptr->info &= ~(CAVE_MARK | CAVE_GLOW);
				}
			}
		}
	}
}


/*
 * Function that sees if a square is a floor (Includes range checking)
 */
static bool get_is_floor(s32b x, s32b y)
{
	/* Out of bounds */
	if (!in_bounds(y, x)) return (FALSE);

	/* Do the real check: */
	if (has_flag(&f_info[cave[y][x].feat], FLAG_FLOOR)) return (TRUE);

	return (FALSE);
}


/*
 * Tunnel around a room if it will cut off part of a cave system
 */
static void check_room_boundary(s32b x1, s32b y1, s32b x2, s32b y2)
{
	s32b count, x, y;
	bool old_is_floor, new_is_floor;

	/* Avoid doing this in irrelevant places -- pelpel */
	if (!((has_flag(dungeon_flags, FLAG_CAVERN)))) return;

	/* Initialize */
	count = 0;

	old_is_floor = get_is_floor(x1 - 1, y1);

	/*
	 * Count the number of floor-wall boundaries around the room
	 * Note: diagonal squares are ignored since the player can move diagonally
	 * to bypass these if needed.
	 */

	/* Above the top boundary */
	for (x = x1; x <= x2; x++)
	{
		new_is_floor = get_is_floor(x, y1 - 1);

		/* increment counter if they are different */
		if (new_is_floor != old_is_floor) count++;

		old_is_floor = new_is_floor;
	}

	/* Right boundary */
	for (y = y1; y <= y2; y++)
	{
		new_is_floor = get_is_floor(x2 + 1, y);

		/* increment counter if they are different */
		if (new_is_floor != old_is_floor) count++;

		old_is_floor = new_is_floor;
	}

	/* Bottom boundary*/
	for (x = x2; x >= x1; x--)
	{
		new_is_floor = get_is_floor(x, y2 + 1);

		/* Increment counter if they are different */
		if (new_is_floor != old_is_floor) count++;

		old_is_floor = new_is_floor;
	}

	/* Left boundary */
	for (y = y2; y >= y1; y--)
	{
		new_is_floor = get_is_floor(x1 - 1, y);

		/* Increment counter if they are different */
		if (new_is_floor != old_is_floor) count++;

		old_is_floor = new_is_floor;
	}


	/* If all the same, or only one connection exit */
	if ((count == 0) || (count == 2)) return;


	/* Tunnel around the room so to prevent problems with caves */
	for (y = y1; y <= y2; y++)
	{
		for (x = x1; x <= x2; x++)
		{
			if (in_bounds(y, x)) place_floor(y, x);
		}
	}
}


/*
 * Create up to "num" objects near the given coordinates
 * Only really called by some of the "vault" routines.
 */
static void vault_objects(s32b y, s32b x, s32b num)
{
	s32b dummy = 0;
	s32b i = 0, j = y, k = x;


	/* Attempt to place 'num' objects */
	for (; num > 0; --num)
	{
		/* Try up to 11 spots looking for empty space */
		for (i = 0; i < 11; ++i)
		{
			/* Pick a random location */
			while (dummy < SAFE_MAX_ATTEMPTS)
			{
				j = rand_spread(y, 2);
				k = rand_spread(x, 3);
				dummy++;
				if (in_bounds(j, k)) break;
			}


			if (dummy >= SAFE_MAX_ATTEMPTS)
			{
				if (cheat_room)
				{
					msg_print("Warning! Could not place vault object!");
				}
			}


			/* Require "clean" floor space */
			if (!cave_clean_bold(j, k)) continue;

			/* Place an item */
			if (rand_int(100) < 75)
			{
				place_object(j, k, FALSE, FALSE, OBJ_FOUND_FLOOR);
			}

			/* Place gold */
			else
			{
				place_gold(j, k);
			}

			/* Placement accomplished */
			break;
		}
	}
}


/*
 * Place a trap with a given displacement of point
 */
static void vault_trap_aux(s32b y, s32b x, s32b yd, s32b xd)
{
	s32b count = 0, y1 = y, x1 = x;
	s32b dummy = 0;

	/* Place traps */
	for (count = 0; count <= 5; count++)
	{
		/* Get a location */
		while (dummy < SAFE_MAX_ATTEMPTS)
		{
			y1 = rand_spread(y, yd);
			x1 = rand_spread(x, xd);
			dummy++;
			if (in_bounds(y1, x1)) break;
		}

		if (dummy >= SAFE_MAX_ATTEMPTS)
		{
			if (cheat_room)
			{
				msg_print("Warning! Could not place vault trap!");
			}
		}


		/* Require "naked" floor grids */
		if (!cave_naked_bold(y1, x1)) continue;

		/* Place the trap */
		place_trap(y1, x1, FACTION_DUNGEON);

		/* Done */
		break;
	}
}


/*
 * Place some traps with a given displacement of given location
 */
static void vault_traps(s32b y, s32b x, s32b yd, s32b xd, s32b num)
{
	s32b i;

	for (i = 0; i < num; i++)
	{
		vault_trap_aux(y, x, yd, xd);
	}
}


/*
 * Hack -- Place some sleeping monsters near the given location
 */
static void vault_monsters(s32b y1, s32b x1, s32b num)
{
	s32b k, i, y, x;

	/* Try to summon "num" monsters "near" the given location */
	for (k = 0; k < num; k++)
	{
		/* Try nine locations */
		for (i = 0; i < 9; i++)
		{
			s32b d = 1;

			/* Pick a nearby location */
			scatter(&y, &x, y1, x1, d, 0);

			/* Require "empty" floor grids */
			if (!cave_empty_bold(y, x)) continue;

			/* Place the monster (allow groups) */
			monster_level = dun_level + 2;
			(void)place_monster(y, x, TRUE, TRUE);
			monster_level = dun_level;
		}
	}
}


/*
 * Allocate the space needed by a room in the room_map array.
 *
 * x, y represent the size of the room (0...x-1) by (0...y-1).
 * crowded is used to denote a monset nest.
 * by0, bx0 are the positions in the room_map array given to the build_type'x'
 * function.
 * xx, yy are the returned center of the allocated room in coordinates for
 * cave.feat and cave.info etc.
 */
bool room_alloc(s32b x, s32b y, bool crowded, s32b by0, s32b bx0, s32b *xx, s32b *yy)
{
	s32b temp, bx1, bx2, by1, by2, by, bx;


	/* Calculate number of room_map squares to allocate */

	/* Total number along width */
	temp = ((x - 1) / BLOCK_WID) + 1;

	/* Ending block */
	bx2 = temp / 2 + bx0;

	/* Starting block (Note: rounding taken care of here) */
	bx1 = bx2 + 1 - temp;


	/* Total number along height */
	temp = ((y - 1) / BLOCK_HGT) + 1;

	/* Ending block */
	by2 = temp / 2 + by0;

	/* Starting block */
	by1 = by2 + 1 - temp;


	/* Never run off the screen */
	if ((by1 < 0) || (by2 >= dun->row_rooms)) return (FALSE);
	if ((bx1 < 0) || (bx2 >= dun->col_rooms)) return (FALSE);

	/* Verify open space */
	for (by = by1; by <= by2; by++)
	{
		for (bx = bx1; bx <= bx2; bx++)
		{
			if (dun->room_map[by][bx]) return (FALSE);
		}
	}

	/*
	 * It is *extremely* important that the following calculation
	 * be *exactly* correct to prevent memory errors XXX XXX XXX
	 */

	/* Acquire the location of the room */
	*yy = ((by1 + by2 + 1) * BLOCK_HGT) / 2;
	*xx = ((bx1 + bx2 + 1) * BLOCK_WID) / 2;

	/*
	 * Paranoia -- I still can't figure out why why some rooms don't have
	 * full walls when they stick to the dungeon boundary...
	 * I don't think this function itself is a part of original... --  pelpel
	 */
	if (!in_bounds2(*yy, *xx)) return (FALSE);
	if (!in_bounds2(*yy + y, *xx + x)) return (FALSE);


	/* Save the room location */
	if (dun->cent_n < CENT_MAX)
	{
		dun->cent[dun->cent_n].y = *yy;
		dun->cent[dun->cent_n].x = *xx;
		dun->cent_n++;
	}

	/* Reserve some blocks */
	for (by = by1; by <= by2; by++)
	{
		for (bx = bx1; bx <= bx2; bx++)
		{
			dun->room_map[by][bx] = TRUE;
		}
	}

	/* Count "crowded" rooms */
	if (crowded) dun->crowded = TRUE;

	/*
	 * Hack -- See if room will cut off a cavern.
	 * If so, fix by tunneling outside the room in such a way as
	 * to conect the caves.
	 */
	check_room_boundary(*xx - x / 2 - 1, *yy - y / 2 - 1,
	                    *xx + x / 2 + 1, *yy + y / 2 + 1);

	/* Success */
	return (TRUE);
}


/*
 * Room building routines.
 *
 * Room types:
 *   1 -- normal
 *   2 -- overlapping
 *   3 -- cross shaped
 *   4 -- large room with features
 *   5 -- monster nests
 *   6 -- monster pits
 *   7 -- simple vaults
 *   8 -- greater vaults
 *   9 -- circular rooms
 */

/*
 * Type 1 -- normal rectangular rooms
 */
static void build_type1(s32b by0, s32b bx0)
{
	s32b y, x = 1, y2, x2, yval, xval;
	s32b y1, x1, xsize, ysize;

	bool light;
	s32b info;

	cave_type *c_ptr;


	/* Ugly */
	if (process_hooks(HOOK_BUILD_ROOM1, "(d,d)", by0, bx0)) return;

	/* Choose lite or dark */
	light = (dun_level <= randint(25));

	/* Pick a room size */
	y1 = randint(4);
	x1 = randint(11);
	y2 = randint(3);
	x2 = randint(11);

	xsize = x1 + x2 + 1;
	ysize = y1 + y2 + 1;

	/* Try to allocate space for room.  If fails, exit */
	if (!room_alloc(xsize + 2, ysize + 2, FALSE, by0, bx0, &xval, &yval)) return;

	/* Get corner values */
	y1 = yval - ysize / 2;
	x1 = xval - xsize / 2;
	y2 = yval + (ysize + 1) / 2;
	x2 = xval + (xsize + 1) / 2;

	/* Place a full floor under the room */
	for (y = y1 - 1; y <= y2 + 1; y++)
	{
		for (x = x1 - 1; x <= x2 + 1; x++)
		{
			if (!in_bounds(y, x)) continue;

			c_ptr = &cave[y][x];
			place_floor(y, x);
			c_ptr->info |= (CAVE_ROOM);
			if (light) c_ptr->info |= (CAVE_GLOW);
		}
	}

	/* Walls around the room */
	if (light) info = (CAVE_ROOM | CAVE_GLOW);
	else info = CAVE_ROOM;
	build_rectangle(y1 - 1, x1 - 1, y2 + 1, x2 + 1, feat_wall_outer, info);


	/* Hack -- Occasional pillar room */
	if (rand_int(20) == 0)
	{
		for (y = y1; y <= y2; y += 2)
		{
			for (x = x1; x <= x2; x += 2)
			{
				if (!in_bounds(y, x)) continue;
				c_ptr = &cave[y][x];
				cave_set_feat(y, x, feat_wall_inner);
			}
		}
	}

	/* Hack -- Occasional ragged-edge room */
	else if (rand_int(50) == 0)
	{
		for (y = y1 + 2; y <= y2 - 2; y += 2)
		{
			if (!in_bounds(y, x)) continue;
			c_ptr = &cave[y][x1];
			cave_set_feat(y, x1, feat_wall_inner);
			c_ptr = &cave[y][x2];
			cave_set_feat(y, x2, feat_wall_inner);
		}
		for (x = x1 + 2; x <= x2 - 2; x += 2)
		{
			c_ptr = &cave[y1][x];
			cave_set_feat(y1, x, feat_wall_inner);
			c_ptr = &cave[y2][x];
			cave_set_feat(y2, x, feat_wall_inner);
		}
	}
}


/*
 * Type 2 -- Overlapping rectangular rooms
 */
static void build_type2(s32b by0, s32b bx0)
{
	s32b y, x, yval, xval;
	s32b y1a, x1a, y2a, x2a;
	s32b y1b, x1b, y2b, x2b;

	bool light;
	s32b info;

	cave_type *c_ptr;


	/* Try to allocate space for room.  If fails, exit */
	if (!room_alloc(25, 11, FALSE, by0, bx0, &xval, &yval)) return;

	/* Choose lite or dark */
	light = (dun_level <= randint(25));


	/* Determine extents of the first room */
	y1a = yval - randint(4);
	y2a = yval + randint(3);
	x1a = xval - randint(11);
	x2a = xval + randint(10);

	/* Determine extents of the second room */
	y1b = yval - randint(3);
	y2b = yval + randint(4);
	x1b = xval - randint(10);
	x2b = xval + randint(11);


	/* Place a full floor for room "a" */
	for (y = y1a - 1; y <= y2a + 1; y++)
	{
		for (x = x1a - 1; x <= x2a + 1; x++)
		{
			if (!in_bounds(y, x)) continue;
			c_ptr = &cave[y][x];
			place_floor(y, x);
			c_ptr->info |= (CAVE_ROOM);
			if (light) c_ptr->info |= (CAVE_GLOW);
		}
	}

	/* Place a full floor for room "b" */
	for (y = y1b - 1; y <= y2b + 1; y++)
	{
		for (x = x1b - 1; x <= x2b + 1; x++)
		{
			if (!in_bounds(y, x)) continue;
			c_ptr = &cave[y][x];
			place_floor(y, x);
			c_ptr->info |= (CAVE_ROOM);
			if (light) c_ptr->info |= (CAVE_GLOW);
		}
	}


	/* Wall attributes */
	if (light) info = (CAVE_ROOM | CAVE_GLOW);
	else info = CAVE_ROOM;

	/* Place the walls around room "a" */
	build_rectangle(y1a - 1, x1a - 1, y2a + 1, x2a + 1, feat_wall_outer, info);

	/* Place the walls around room "a" */
	build_rectangle(y1b - 1, x1b - 1, y2b + 1, x2b + 1, feat_wall_outer, info);


	/* Replace the floor for room "a" */
	for (y = y1a; y <= y2a; y++)
	{
		for (x = x1a; x <= x2a; x++)
		{
			if (!in_bounds(y, x)) continue;
			c_ptr = &cave[y][x];
			place_floor(y, x);
		}
	}

	/* Replace the floor for room "b" */
	for (y = y1b; y <= y2b; y++)
	{
		for (x = x1b; x <= x2b; x++)
		{
			if (!in_bounds(y, x)) continue;
			c_ptr = &cave[y][x];
			place_floor(y, x);
		}
	}
}



/*
 * Type 3 -- Cross shaped rooms
 *
 * Builds a room at a row, column coordinate
 *
 * Room "a" runs north/south, and Room "b" runs east/east
 * So the "central pillar" runs from x1a,y1b to x2a,y2b.
 *
 * Note that currently, the "center" is always 3x3, but I think that
 * the code below will work (with "bounds checking") for 5x5, or even
 * for unsymetric values like 4x3 or 5x3 or 3x4 or 3x5, or even larger.
 */
static void build_type3(s32b by0, s32b bx0)
{
	s32b y, x, dy, dx, wy, wx;
	s32b y1a, x1a, y2a, x2a;
	s32b y1b, x1b, y2b, x2b;
	s32b yval, xval;

	bool light;
	s32b info;

	cave_type *c_ptr;


	/* Try to allocate space for room.  If fails, exit */
	if (!room_alloc(25, 11, FALSE, by0, bx0, &xval, &yval)) return;


	/* Choose lite or dark */
	light = (dun_level <= randint(25));


	/* For now, always 3x3 */
	wx = wy = 1;

	/* Pick max vertical size (at most 4) */
	dy = rand_range(3, 4);

	/* Pick max horizontal size (at most 15) */
	dx = rand_range(3, 11);


	/* Determine extents of the north/south room */
	y1a = yval - dy;
	y2a = yval + dy;
	x1a = xval - wx;
	x2a = xval + wx;

	/* Determine extents of the east/west room */
	y1b = yval - wy;
	y2b = yval + wy;
	x1b = xval - dx;
	x2b = xval + dx;


	/* Place a full floor for room "a" */
	for (y = y1a - 1; y <= y2a + 1; y++)
	{
		for (x = x1a - 1; x <= x2a + 1; x++)
		{
			if (!in_bounds(y, x)) continue;
			c_ptr = &cave[y][x];
			place_floor(y, x);
			c_ptr->info |= (CAVE_ROOM);
			if (light) c_ptr->info |= (CAVE_GLOW);
		}
	}

	/* Place a full floor for room "b" */
	for (y = y1b - 1; y <= y2b + 1; y++)
	{
		for (x = x1b - 1; x <= x2b + 1; x++)
		{
			if (!in_bounds(y, x)) continue;
			c_ptr = &cave[y][x];
			place_floor(y, x);
			c_ptr->info |= (CAVE_ROOM);
			if (light) c_ptr->info |= (CAVE_GLOW);
		}
	}


	/* Wall attributes */
	if (light) info = (CAVE_ROOM | CAVE_GLOW);
	else info = CAVE_ROOM;

	/* Place the walls around room "a" */
	build_rectangle(y1a - 1, x1a - 1, y2a + 1, x2a + 1, feat_wall_outer, info);

	/* Place the walls around room "a" */
	build_rectangle(y1b - 1, x1b - 1, y2b + 1, x2b + 1, feat_wall_outer, info);


	/* Replace the floor for room "a" */
	for (y = y1a; y <= y2a; y++)
	{
		for (x = x1a; x <= x2a; x++)
		{
			if (!in_bounds(y, x)) continue;
			place_floor(y, x);
		}
	}

	/* Replace the floor for room "b" */
	for (y = y1b; y <= y2b; y++)
	{
		for (x = x1b; x <= x2b; x++)
		{
			if (!in_bounds(y, x)) continue;
			place_floor(y, x);
		}
	}



	/* Special features (3/4) */
	switch (rand_int(4))
	{
		/* Large solid middle pillar */
	case 1:
		{
			for (y = y1b; y <= y2b; y++)
			{
				for (x = x1a; x <= x2a; x++)
				{
					if (!in_bounds(y, x)) continue;
					c_ptr = &cave[y][x];
					cave_set_feat(y, x, feat_wall_inner);
				}
			}
			break;
		}

		/* Inner treasure vault */
	case 2:
		{
			/* Build the vault */
			build_rectangle(y1b, x1a, y2b, x2a, feat_wall_inner, info);

			/* Place a secret door on the inner room */
			switch (rand_int(4))
			{
			case 0:
				place_secret_door(y1b, xval);
				break;
			case 1:
				place_secret_door(y2b, xval);
				break;
			case 2:
				place_secret_door(yval, x1a);
				break;
			case 3:
				place_secret_door(yval, x2a);
				break;
			}

			/* Place a treasure in the vault */
			place_object(yval, xval, FALSE, FALSE, OBJ_FOUND_FLOOR);

			/* Let's guard the treasure well */
			vault_monsters(yval, xval, rand_int(2) + 3);

			/* Traps naturally */
			vault_traps(yval, xval, 4, 4, rand_int(3) + 2);

			break;
		}

		/* Something else */
	case 3:
		{
			/* Occasionally pinch the center shut */
			if (rand_int(3) == 0)
			{
				/* Pinch the east/west sides */
				for (y = y1b; y <= y2b; y++)
				{
					if (!in_bounds(y, x1a - 1)) continue;
					if (y == yval) continue;
					c_ptr = &cave[y][x1a - 1];
					cave_set_feat(y, x1a - 1, feat_wall_inner);
					if (!in_bounds(y, x2a + 1)) continue;
					c_ptr = &cave[y][x2a + 1];
					cave_set_feat(y, x2a + 1, feat_wall_inner);
				}

				/* Pinch the north/south sides */
				for (x = x1a; x <= x2a; x++)
				{
					if (!in_bounds(y1b - 1, x)) continue;
					if (x == xval) continue;
					c_ptr = &cave[y1b - 1][x];
					cave_set_feat(y1b - 1, x, feat_wall_inner);
					if (!in_bounds(y2b + 1, x)) continue;
					c_ptr = &cave[y2b + 1][x];
					cave_set_feat(y2b + 1, x, feat_wall_inner);
				}

				/* Sometimes shut using secret doors */
				if (rand_int(3) == 0)
				{
					place_secret_door(yval, x1a - 1);
					place_secret_door(yval, x2a + 1);
					place_secret_door(y1b - 1, xval);
					place_secret_door(y2b + 1, xval);
				}
			}

			/* Occasionally put a "plus" in the center */
			else if (rand_int(3) == 0)
			{
				c_ptr = &cave[yval][xval];
				cave_set_feat(yval, xval, feat_wall_inner);
				c_ptr = &cave[y1b][xval];
				cave_set_feat(y1b, xval, feat_wall_inner);
				c_ptr = &cave[y2b][xval];
				cave_set_feat(y2b, xval, feat_wall_inner);
				c_ptr = &cave[yval][x1a];
				cave_set_feat(yval, x1a, feat_wall_inner);
				c_ptr = &cave[yval][x2a];
				cave_set_feat(yval, x2a, feat_wall_inner);
			}

			/* Occasionally put a pillar in the center */
			else if (rand_int(3) == 0)
			{
				c_ptr = &cave[yval][xval];
				cave_set_feat(yval, xval, feat_wall_inner);
			}

			break;
		}
	}
}


/*
 * Type 4 -- Large room with inner features
 *
 * Possible sub-types:
 *	1 - Just an inner room with one door
 *	2 - An inner room within an inner room
 *	3 - An inner room with pillar(s)
 *	4 - Inner room has a maze
 *	5 - A set of four inner rooms
 */
static void build_type4(s32b by0, s32b bx0)
{
	s32b y, x, y1, x1;
	s32b y2, x2, tmp, yval, xval;

	bool light;
	s32b info;

	cave_type *c_ptr;


	/* Try to allocate space for room.  If fails, exit */
	if (!room_alloc(25, 11, FALSE, by0, bx0, &xval, &yval)) return;

	/* Choose lite or dark */
	light = (dun_level <= randint(25));

	/* Large room */
	y1 = yval - 4;
	y2 = yval + 4;
	x1 = xval - 11;
	x2 = xval + 11;

	/* Place a full floor under the room */
	for (y = y1 - 1; y <= y2 + 1; y++)
	{
		for (x = x1 - 1; x <= x2 + 1; x++)
		{
			if (!in_bounds(y, x)) continue;
			place_floor(y, x);
			c_ptr = &cave[y][x];
			c_ptr->info |= (CAVE_ROOM);
			if (light) c_ptr->info |= (CAVE_GLOW);
		}
	}

	/* Wall attributes */
	if (light) info = (CAVE_ROOM | CAVE_GLOW);
	else info = CAVE_ROOM;

	/* Outer Walls */
	build_rectangle(y1 - 1, x1 - 1, y2 + 1, x2 + 1, feat_wall_outer, info);


	/* The inner room */
	y1 = y1 + 2;
	y2 = y2 - 2;
	x1 = x1 + 2;
	x2 = x2 - 2;

	/* The inner walls */
	build_rectangle(y1 - 1, x1 - 1, y2 + 1, x2 + 1, feat_wall_inner, info);


	/* Inner room variations */
	switch (randint(5))
	{
		/* Just an inner room with a monster */
	case 1:
		{
			/* Place a secret door */
			switch (randint(4))
			{
			case 1:
				place_secret_door(y1 - 1, xval);
				break;
			case 2:
				place_secret_door(y2 + 1, xval);
				break;
			case 3:
				place_secret_door(yval, x1 - 1);
				break;
			case 4:
				place_secret_door(yval, x2 + 1);
				break;
			}

			/* Place a monster in the room */
			vault_monsters(yval, xval, 1);

			break;
		}

		/* Treasure Vault (with a door) */
	case 2:
		{
			/* Place a secret door */
			switch (randint(4))
			{
			case 1:
				place_secret_door(y1 - 1, xval);
				break;
			case 2:
				place_secret_door(y2 + 1, xval);
				break;
			case 3:
				place_secret_door(yval, x1 - 1);
				break;
			case 4:
				place_secret_door(yval, x2 + 1);
				break;
			}

			/* Place another inner room */
			build_rectangle(yval - 1, xval - 1, yval + 1, xval + 1,
			                feat_wall_inner, info);

			/* Place a locked door on the inner room */
			switch (randint(4))
			{
			case 1:
				place_locked_door(yval - 1, xval);
				break;
			case 2:
				place_locked_door(yval + 1, xval);
				break;
			case 3:
				place_locked_door(yval, xval - 1);
				break;
			case 4:
				place_locked_door(yval, xval + 1);
				break;
			}

			/* Monsters to guard the "treasure" */
			vault_monsters(yval, xval, randint(3) + 2);

			/* Object (80%) */
			if (rand_int(100) < 80)
			{
				place_object(yval, xval, FALSE, FALSE, OBJ_FOUND_FLOOR);
			}

			/* Stairs (20%) */
			else
			{
				place_random_stairs(yval, xval);
			}

			/* Traps to protect the treasure */
			vault_traps(yval, xval, 4, 10, 2 + randint(3));

			break;
		}

		/* Inner pillar(s). */
	case 3:
		{
			/* Place a secret door */
			switch (randint(4))
			{
			case 1:
				place_secret_door(y1 - 1, xval);
				break;
			case 2:
				place_secret_door(y2 + 1, xval);
				break;
			case 3:
				place_secret_door(yval, x1 - 1);
				break;
			case 4:
				place_secret_door(yval, x2 + 1);
				break;
			}

			/* Large Inner Pillar */
			for (y = yval - 1; y <= yval + 1; y++)
			{
				for (x = xval - 1; x <= xval + 1; x++)
				{
					if (!in_bounds(y, x)) continue;
					c_ptr = &cave[y][x];
					cave_set_feat(y, x, feat_wall_inner);
				}
			}

			/* Occasionally, two more Large Inner Pillars */
			if (rand_int(2) == 0)
			{
				tmp = randint(2);
				for (y = yval - 1; y <= yval + 1; y++)
				{
					for (x = xval - 5 - tmp; x <= xval - 3 - tmp; x++)
					{
						if (!in_bounds(y, x)) continue;
						c_ptr = &cave[y][x];
						cave_set_feat(y, x, feat_wall_inner);
					}
					for (x = xval + 3 + tmp; x <= xval + 5 + tmp; x++)
					{
						if (!in_bounds(y, x)) continue;
						c_ptr = &cave[y][x];
						cave_set_feat(y, x, feat_wall_inner);
					}
				}
			}

			/* Occasionally, some Inner rooms */
			if (rand_int(3) == 0)
			{
				/* Long horizontal walls */
				for (x = xval - 5; x <= xval + 5; x++)
				{
					if (!in_bounds(yval - 1, x)) continue;
					c_ptr = &cave[yval - 1][x];
					cave_set_feat(yval - 1, x, feat_wall_inner);
					if (!in_bounds(yval + 1, x)) continue;
					c_ptr = &cave[yval + 1][x];
					cave_set_feat(yval + 1, x, feat_wall_inner);
				}

				/* Close off the left/right edges */
				c_ptr = &cave[yval][xval - 5];
				cave_set_feat(yval, xval - 5, feat_wall_inner);
				c_ptr = &cave[yval][xval + 5];
				cave_set_feat(yval, xval + 5, feat_wall_inner);

				/* Secret doors (random top/bottom) */
				place_secret_door(yval - 3 + (randint(2) * 2), xval - 3);
				place_secret_door(yval - 3 + (randint(2) * 2), xval + 3);

				/* Monsters */
				vault_monsters(yval, xval - 2, randint(2));
				vault_monsters(yval, xval + 2, randint(2));

				/* Objects */
				if (rand_int(3) == 0) place_object(yval, xval - 2, FALSE, FALSE, OBJ_FOUND_FLOOR);
				if (rand_int(3) == 0) place_object(yval, xval + 2, FALSE, FALSE, OBJ_FOUND_FLOOR);
			}

			break;
		}

		/* Maze inside. */
	case 4:
		{
			/* Place a secret door */
			switch (randint(4))
			{
			case 1:
				place_secret_door(y1 - 1, xval);
				break;
			case 2:
				place_secret_door(y2 + 1, xval);
				break;
			case 3:
				place_secret_door(yval, x1 - 1);
				break;
			case 4:
				place_secret_door(yval, x2 + 1);
				break;
			}

			/* Maze (really a checkerboard) */
			for (y = y1; y <= y2; y++)
			{
				for (x = x1; x <= x2; x++)
				{
					if (0x1 & (x + y))
					{
						if (!in_bounds(y, x)) continue;
						c_ptr = &cave[y][x];
						cave_set_feat(y, x, feat_wall_inner);
					}
				}
			}

			/* Monsters just love mazes. */
			vault_monsters(yval, xval - 5, randint(3));
			vault_monsters(yval, xval + 5, randint(3));

			/* Traps make them entertaining. */
			vault_traps(yval, xval - 3, 2, 8, randint(3));
			vault_traps(yval, xval + 3, 2, 8, randint(3));

			/* Mazes should have some treasure too. */
			vault_objects(yval, xval, 3);

			break;
		}

		/* Four small rooms. */
	case 5:
		{
			/* Inner "cross" */
			for (y = y1; y <= y2; y++)
			{
				c_ptr = &cave[y][xval];
				cave_set_feat(y, xval, feat_wall_inner);
			}

			for (x = x1; x <= x2; x++)
			{
				c_ptr = &cave[yval][x];
				cave_set_feat(yval, x, feat_wall_inner);
			}

			/* Doors into the rooms */
			if (rand_int(100) < 50)
			{
				s32b i = randint(10);
				place_secret_door(y1 - 1, xval - i);
				place_secret_door(y1 - 1, xval + i);
				place_secret_door(y2 + 1, xval - i);
				place_secret_door(y2 + 1, xval + i);
			}
			else
			{
				s32b i = randint(3);
				place_secret_door(yval + i, x1 - 1);
				place_secret_door(yval - i, x1 - 1);
				place_secret_door(yval + i, x2 + 1);
				place_secret_door(yval - i, x2 + 1);
			}

			/* Treasure, centered at the center of the cross */
			vault_objects(yval, xval, 2 + randint(2));

			/* Gotta have some monsters. */
			vault_monsters(yval + 1, xval - 4, randint(4));
			vault_monsters(yval + 1, xval + 4, randint(4));
			vault_monsters(yval - 1, xval - 4, randint(4));
			vault_monsters(yval - 1, xval + 4, randint(4));

			break;
		}
	}
}



/*
 * Type 5 -- Monster nests
 *
 * A monster nest is a "big" room, with an "inner" room, containing
 * a "collection" of monsters of a given type strewn about the room.
 *
 * The monsters are chosen from a set of 64 randomly selected monster
 * races, to allow the nest creation to fail instead of having "holes".
 *
 * Note the use of the "get_mon_num_prep()" function, and the special
 * "get_mon_num_hook()" restriction function, to prepare the "monster
 * allocation table" in such a way as to optimize the selection of
 * "appropriate" non-unique monsters for the nest.
 *
 * Currently, a monster nest is one of
 *   a nest of "jelly" monsters   (Dungeon level 5 and deeper)
 *   a nest of "animal" monsters  (Dungeon level 30 and deeper)
 *   a nest of "undead" monsters  (Dungeon level 50 and deeper)
 *
 * Note that the "get_mon_num()" function may (rarely) fail, in which
 * case the nest will be empty, and will not affect the level rating.
 *
 * Note that "monster nests" will never contain "unique" monsters.
 */
static void build_type5(s32b by0, s32b bx0)
{
	s32b y, x, y1, x1, y2, x2, xval, yval;

	s32b i;

	s16b what[64];

	cave_type *c_ptr;

	cptr name = "";

	bool empty = FALSE;


	/* Try to allocate space for room.  If fails, exit */
	if (!room_alloc(25, 11, TRUE, by0, bx0, &xval, &yval)) return;

	/* Large room */
	y1 = yval - 4;
	y2 = yval + 4;
	x1 = xval - 11;
	x2 = xval + 11;

	if (seed_dungeon) Rand_quick = FALSE;

	/* Choose a nest type */
	if (process_hooks_ret(HOOK_NEW_NEST, "s", name))
	{
		name = process_hooks_return[0].str;
	}
	else
	{
		if (seed_dungeon)
		{
			Rand_quick = TRUE;
		}
		return;
	}

	/* Place the floor area */
	for (y = y1 - 1; y <= y2 + 1; y++)
	{
		for (x = x1 - 1; x <= x2 + 1; x++)
		{
			if (!in_bounds(y, x)) continue;
			place_floor(y, x);
			c_ptr = &cave[y][x];
			c_ptr->info |= (CAVE_ROOM);
		}
	}

	/* Place the outer walls */
	build_rectangle(y1 - 1, x1 - 1, y2 + 1, x2 + 1, feat_wall_outer, CAVE_ROOM);


	/* Advance to the center room */
	y1 = y1 + 2;
	y2 = y2 - 2;
	x1 = x1 + 2;
	x2 = x2 - 2;

	/* The inner walls */
	build_rectangle(y1 - 1, x1 - 1, y2 + 1, x2 + 1, feat_wall_inner, CAVE_ROOM);


	/* Place a secret door */
	switch (randint(4))
	{
	case 1:
		place_secret_door(y1 - 1, xval);
		break;
	case 2:
		place_secret_door(y2 + 1, xval);
		break;
	case 3:
		place_secret_door(yval, x1 - 1);
		break;
	case 4:
		place_secret_door(yval, x2 + 1);
		break;
	}


	/* Prepare allocation table */
	get_mon_num_prep();


	/* Pick some monster types */
	for (i = 0; i < 64; i++)
	{
		/* Get a (hard) monster type */
		what[i] = get_mon_num(dun_level + 10);

		/* Notice failure */
		if (!what[i]) empty = TRUE;
	}


	/* Remove restriction */
	process_hooks(HOOK_NEST_PIT_POST, "()");

	/* Prepare allocation table */
	get_mon_num_prep();


	/* Oops */
	if (empty) return;


	/* Describe */
	if (cheat_room || p_ptr->precognition)
	{
		/* Room type */
		msg_format("Monster nest (%s)", name);
	}


	/* Increase the level rating */
	rating += 10;

	/* (Sometimes) Cause a "special feeling" (for "Monster Nests") */
	if ((dun_level <= 40) && (randint(dun_level * dun_level + 50) < 300))
	{
		good_item_flag = TRUE;
	}


	/* Place some monsters */
	for (y = yval - 2; y <= yval + 2; y++)
	{
		for (x = xval - 9; x <= xval + 9; x++)
		{
			s32b r_idx = what[rand_int(64)];

			/* Place that "random" monster (no groups) */
			(void)place_monster_aux(y, x, r_idx, FALSE, FALSE, FACTION_DUNGEON);
		}
	}

	if (seed_dungeon) Rand_quick = TRUE;
}



/*
 * Type 6 -- Monster pits
 *
 * A monster pit is a "big" room, with an "inner" room, containing
 * a "collection" of monsters of a given type organized in the room.
 *
 * Monster types in the pit  (list out of date...)
 *   orc pit	(Dungeon Level 5 and deeper)
 *   troll pit	(Dungeon Level 20 and deeper)
 *   giant pit	(Dungeon Level 40 and deeper)
 *   dragon pit	(Dungeon Level 60 and deeper)
 *   demon pit	(Dungeon Level 80 and deeper)
 *
 * The inside room in a monster pit appears as shown below, where the
 * actual monsters in each location depend on the type of the pit
 *
 *   #####################
 *   #0000000000000000000#
 *   #0112233455543322110#
 *   #0112233467643322110#
 *   #0112233455543322110#
 *   #0000000000000000000#
 *   #####################
 *
 * Note that the monsters in the pit are now chosen by using "get_mon_num()"
 * to request 16 "appropriate" monsters, sorting them by level, and using
 * the "even" entries in this sorted list for the contents of the pit.
 *
 * Hack -- all of the "dragons" in a "dragon" pit must be the same "color",
 * which is handled by requiring a specific "breath" attack for all of the
 * dragons.  This may include "multi-hued" breath.  Note that "wyrms" may
 * be present in many of the dragon pits, if they have the proper breath.
 *
 * Note the use of the "get_mon_num_prep()" function, and the special
 * "get_mon_num_hook()" restriction function, to prepare the "monster
 * allocation table" in such a way as to optimize the selection of
 * "appropriate" non-unique monsters for the pit.
 *
 * Note that the "get_mon_num()" function may (rarely) fail, in which case
 * the pit will be empty, and will not effect the level rating.
 *
 * Note that "monster pits" will never contain "unique" monsters.
 */
static void build_type6(s32b by0, s32b bx0)
{
	s32b what[16];
	s32b i, j, y, x, y1, x1, y2, x2, xval, yval;
	bool empty = FALSE;
	cave_type *c_ptr;
	cptr name = "";

	/* Try to allocate space for room.  If fails, exit */
	if (!room_alloc(25, 11, TRUE, by0, bx0, &xval, &yval)) return;

	/* Large room */
	y1 = yval - 4;
	y2 = yval + 4;
	x1 = xval - 11;
	x2 = xval + 11;

	if (seed_dungeon) Rand_quick = FALSE;

	/* Choose a pit type */
	if (process_hooks_ret(HOOK_NEW_PIT, "s", name))
	{
		name = process_hooks_return[0].str;
	}
	else
	{
		if (seed_dungeon)
		{
			Rand_quick = TRUE;
		}
		return;
	}

	/* Place the floor area */
	for (y = y1 - 1; y <= y2 + 1; y++)
	{
		for (x = x1 - 1; x <= x2 + 1; x++)
		{
			place_floor(y, x);
			c_ptr = &cave[y][x];
			c_ptr->info |= (CAVE_ROOM);
		}
	}

	/* Place the outer walls */
	build_rectangle(y1 - 1, x1 - 1, y2 + 1, x2 + 1, feat_wall_outer, CAVE_ROOM);


	/* Advance to the center room */
	y1 = y1 + 2;
	y2 = y2 - 2;
	x1 = x1 + 2;
	x2 = x2 - 2;

	/* The inner walls */
	build_rectangle(y1 - 1, x1 - 1, y2 + 1, x2 + 1, feat_wall_outer, CAVE_ROOM);


	/* Place a secret door */
	switch (randint(4))
	{
	case 1:
		place_secret_door(y1 - 1, xval);
		break;
	case 2:
		place_secret_door(y2 + 1, xval);
		break;
	case 3:
		place_secret_door(yval, x1 - 1);
		break;
	case 4:
		place_secret_door(yval, x2 + 1);
		break;
	}


	/* Prepare allocation table */
	get_mon_num_prep();


	/* Pick some monster types */
	for (i = 0; i < 16; i++)
	{
		/* Get a (hard) monster type */
		what[i] = get_mon_num(dun_level + 10);

		/* Notice failure */
		if (!what[i]) empty = TRUE;
	}


	/* Remove restriction */
	process_hooks(HOOK_NEST_PIT_POST, "");

	/* Prepare allocation table */
	get_mon_num_prep();


	/* Oops */
	if (empty) return;


	/* XXX XXX XXX */
	/* Sort the entries */
	for (i = 0; i < 16 - 1; i++)
	{
		/* Sort the entries */
		for (j = 0; j < 16 - 1; j++)
		{
			s32b i1 = j;
			s32b i2 = j + 1;

			s32b p1 = r_info[what[i1]].level;
			s32b p2 = r_info[what[i2]].level;

			/* Bubble */
			if (p1 > p2)
			{
				s32b tmp = what[i1];
				what[i1] = what[i2];
				what[i2] = tmp;
			}
		}
	}

	/* Select the entries */
	for (i = 0; i < 8; i++)
	{
		/* Every other entry */
		what[i] = what[i * 2];
	}


	/* Message */
	if (cheat_room || p_ptr->precognition)
	{
		/* Room type */
		msg_format("Monster pit (%s)", name);

		if (cheat_hear || p_ptr->precognition)
		{
			/* Contents */
			for (i = 0; i < 8; i++)
			{
				/* Message */
				msg_print(r_info[what[i]].name);
			}
		}
	}


	/* Increase the level rating */
	rating += 10;

	/* (Sometimes) Cause a "special feeling" (for "Monster Pits") */
	if ((dun_level <= 40) && (randint(dun_level * dun_level + 50) < 300))
	{
		good_item_flag = TRUE;
	}


	/* Top and bottom rows */
	for (x = xval - 9; x <= xval + 9; x++)
	{
		place_monster_aux(yval - 2, x, what[0], FALSE, FALSE, FACTION_DUNGEON);
		place_monster_aux(yval + 2, x, what[0], FALSE, FALSE, FACTION_DUNGEON);
	}

	/* Middle columns */
	for (y = yval - 1; y <= yval + 1; y++)
	{
		place_monster_aux(y, xval - 9, what[0], FALSE, FALSE, FACTION_DUNGEON);
		place_monster_aux(y, xval + 9, what[0], FALSE, FALSE, FACTION_DUNGEON);

		place_monster_aux(y, xval - 8, what[1], FALSE, FALSE, FACTION_DUNGEON);
		place_monster_aux(y, xval + 8, what[1], FALSE, FALSE, FACTION_DUNGEON);

		place_monster_aux(y, xval - 7, what[1], FALSE, FALSE, FACTION_DUNGEON);
		place_monster_aux(y, xval + 7, what[1], FALSE, FALSE, FACTION_DUNGEON);

		place_monster_aux(y, xval - 6, what[2], FALSE, FALSE, FACTION_DUNGEON);
		place_monster_aux(y, xval + 6, what[2], FALSE, FALSE, FACTION_DUNGEON);

		place_monster_aux(y, xval - 5, what[2], FALSE, FALSE, FACTION_DUNGEON);
		place_monster_aux(y, xval + 5, what[2], FALSE, FALSE, FACTION_DUNGEON);

		place_monster_aux(y, xval - 4, what[3], FALSE, FALSE, FACTION_DUNGEON);
		place_monster_aux(y, xval + 4, what[3], FALSE, FALSE, FACTION_DUNGEON);

		place_monster_aux(y, xval - 3, what[3], FALSE, FALSE, FACTION_DUNGEON);
		place_monster_aux(y, xval + 3, what[3], FALSE, FALSE, FACTION_DUNGEON);

		place_monster_aux(y, xval - 2, what[4], FALSE, FALSE, FACTION_DUNGEON);
		place_monster_aux(y, xval + 2, what[4], FALSE, FALSE, FACTION_DUNGEON);
	}

	/* Above/Below the center monster */
	for (x = xval - 1; x <= xval + 1; x++)
	{
		place_monster_aux(yval + 1, x, what[5], FALSE, FALSE, FACTION_DUNGEON);
		place_monster_aux(yval - 1, x, what[5], FALSE, FALSE, FACTION_DUNGEON);
	}

	/* Next to the center monster */
	place_monster_aux(yval, xval + 1, what[6], FALSE, FALSE, FACTION_DUNGEON);
	place_monster_aux(yval, xval - 1, what[6], FALSE, FALSE, FACTION_DUNGEON);

	/* Center monster */
	place_monster_aux(yval, xval, what[7], FALSE, FALSE, FACTION_DUNGEON);

	if (seed_dungeon)
	{
		Rand_quick = TRUE;
	}
}



/*
 * Hack -- fill in "vault" rooms
 */
static void build_vault(s32b yval, s32b xval, s32b ymax, s32b xmax, cptr data)
{
	s32b dx, dy, x, y;

	cptr t;

	cave_type *c_ptr;

	/* Vaults are different even in persistent dungeons. */
	if (seed_dungeon)
	{
		Rand_quick = FALSE;
	}

	/* Place dungeon features and objects */
	for (t = data, dy = 0; dy < ymax; dy++)
	{
		for (dx = 0; dx < xmax; dx++, t++)
		{
			/* Extract the location */
			x = xval - (xmax / 2) + dx;
			y = yval - (ymax / 2) + dy;

			/* Hack -- skip "non-grids" */
			if (*t == ' ') continue;

			/* Access the grid */
			c_ptr = &cave[y][x];

			/* Lay down a floor */
			place_floor(y, x);

			/* Part of a vault */
			c_ptr->info |= (CAVE_ROOM | CAVE_ICKY);

			/* Analyze the grid */
			switch (*t)
			{
				/* Granite wall (outer) */
			case '%':
				{
					cave_set_feat(y, x, d_info[dungeon_type].outer_wall);
					break;
				}

				/* Granite wall (inner) */
			case '#':
				{
					cave_set_feat(y, x, d_info[dungeon_type].inner_wall);
					break;
				}

				/* Permanent wall (inner) */
			case 'X':
				{
					cave_set_feat(y, x, d_info[dungeon_type].inner_wall);
					break;
				}

				/* Treasure/trap */
			case '*':
				{
					if (rand_int(100) < 75)
					{
						place_object(y, x, FALSE, FALSE, OBJ_FOUND_VAULT);
					}
					else
					{
						place_trap(y, x, FACTION_DUNGEON);
					}
					break;
				}

				/* Secret doors */
			case '+':
				{
					place_secret_door(y, x);
					break;
				}

				/* Trap */
			case '^':
				{
					place_trap(y, x, FACTION_DUNGEON);
					break;
				}
			}
		}
	}


	/* Place dungeon monsters and objects */
	for (t = data, dy = 0; dy < ymax; dy++)
	{
		for (dx = 0; dx < xmax; dx++, t++)
		{
			/* Extract the grid */
			x = xval - (xmax / 2) + dx;
			y = yval - (ymax / 2) + dy;

			/* Hack -- skip "non-grids" */
			if (*t == ' ') continue;

			/* Access the grid */
			c_ptr = &cave[y][x];

			/* Analyze the symbol */
			switch (*t)
			{
				/* Monster */
			case '&':
				{
					monster_level = dun_level + 5;
					place_monster(y, x, TRUE, TRUE);
					monster_level = dun_level;
					break;
				}

				/* Meaner monster */
			case '@':
				{
					monster_level = dun_level + 11;
					place_monster(y, x, TRUE, TRUE);
					monster_level = dun_level;
					break;
				}

				/* Meaner monster, plus treasure */
			case '9':
				{
					monster_level = dun_level + 9;
					place_monster(y, x, TRUE, TRUE);
					monster_level = dun_level;
					object_level = dun_level + 7;
					place_object(y, x, TRUE, FALSE, OBJ_FOUND_VAULT);
					object_level = dun_level;
					break;
				}

				/* Nasty monster and treasure */
			case '8':
				{
					monster_level = dun_level + 40;
					place_monster(y, x, TRUE, TRUE);
					monster_level = dun_level;
					object_level = dun_level + 20;
					place_object(y, x, TRUE, TRUE, OBJ_FOUND_VAULT);
					object_level = dun_level;
					break;
				}

				/* Monster and/or object */
			case ',':
				{
					if (rand_int(100) < 50)
					{
						monster_level = dun_level + 3;
						place_monster(y, x, TRUE, TRUE);
						monster_level = dun_level;
					}
					if (rand_int(100) < 50)
					{
						object_level = dun_level + 7;
						place_object(y, x, FALSE, FALSE, OBJ_FOUND_VAULT);
						object_level = dun_level;
					}
					break;
				}

			case 'A':
				{
					object_level = dun_level + 12;
					place_object(y, x, TRUE, FALSE, OBJ_FOUND_VAULT);
					object_level = dun_level;
					break;
				}
			}
		}
	}

	if (seed_dungeon)
	{
		Rand_quick = TRUE;
	}
}


/*
 * Type 7 -- simple vaults (see "v_info.txt")
 */
static void build_type7(s32b by0, s32b bx0)
{
	vault_type *v_ptr = NULL;
	s32b xval, yval;
	s32b i;
	vector_type *allowed = vector_new();

	for (i = 0; i < max_v_idx; i++)
	{
		v_ptr = &v_info[i];

		if ((v_ptr->typ == 7) && restrict_to_dungeon(&v_ptr->flags, -1) &&
			(ironman_rooms || v_ptr->lvl <= dun_level))
		{
			if (cheat_room) msg_format("LVAULT: Allowing '%s'.", v_ptr->name);
			vector_push(allowed, i);
		}
	}

	/* Pick a lesser vault */
	if (!vector_is_empty(allowed))
	{
		/* The vector_type put their first element at index=1 */
		i = flag_get(allowed,1+rand_int(vector_size(allowed)));
		v_ptr = &v_info[i];
	}
	else
		v_ptr = NULL;

	vector_free(allowed);

	/* Try to allocate space for room.  If fails, exit */
	if ((v_ptr == NULL) || !room_alloc(v_ptr->wid, v_ptr->hgt, FALSE, by0, bx0, &xval, &yval))
	{
		if (cheat_room) msg_print("Could not allocate this vault here");
		return;
	}

#ifdef FORCE_V_IDX
	v_ptr = &v_info[FORCE_V_IDX];
#endif

	/* Message */
	if (cheat_room || p_ptr->precognition) msg_print("Lesser Vault");

	/* Boost the rating */
	rating += v_ptr->rat;

	/* (Sometimes) Cause a special feeling */
	if ((dun_level <= 50) ||
	                (randint((dun_level - 40) * (dun_level - 40) + 50) < 400))
	{
		good_item_flag = TRUE;
	}

	/* Build the vault */
	if (v_ptr->dynamic)
	{
		call_lua("load_map", "(s,d,d,n,b)", "", format("dungeon/vaults/%s", v_ptr->text), yval, xval, TRUE);
	}
	else
	{
		build_vault(yval, xval, v_ptr->hgt, v_ptr->wid, v_ptr->text);
	}
}



/*
 * Type 8 -- greater vaults (see "v_info.txt")
 */
static void build_type8(s32b by0, s32b bx0)
{
	vault_type *v_ptr = NULL;
	s32b dummy = 0, xval, yval;
	bool ret;
	s32b i;
	vector_type *allowed = vector_new();

	for (i = 0; i < max_v_idx; i++)
	{
		v_ptr = &v_info[i];

		if ((v_ptr->typ == 8) && restrict_to_dungeon(&v_ptr->flags, -1) &&
			(ironman_rooms || v_ptr->lvl <= dun_level))
		{
			if (cheat_room) msg_format("GVAULT: Allowing '%s'.", v_ptr->name);
			vector_push(allowed, i);
		}
	}

	/* Pick a greater vault */
	if (!vector_is_empty(allowed))
	{
		/* The vector_type put their first element at index=1 */
		i = flag_get(allowed,1+rand_int(vector_size(allowed)));
		v_ptr = &v_info[i];
	}
	else
		v_ptr = NULL;

	vector_free(allowed);

	/* Try to allocate space for room.  If fails, exit */
	if ((v_ptr == NULL) || !(ret = room_alloc(v_ptr->wid, v_ptr->hgt, FALSE, by0, bx0, &xval, &yval)))
	{
		if (cheat_room) msg_print("Could not allocate this vault here");
		return;
	}

	if (dummy >= SAFE_MAX_ATTEMPTS)
	{
		if (cheat_room)
		{
			msg_print("Warning! Could not place greater vault!");
		}
		return;
	}


#ifdef FORCE_V_IDX
	v_ptr = &v_info[FORCE_V_IDX];
#endif

	/* Message */
	if (cheat_room || p_ptr->precognition) msg_print("Greater Vault");

	/* Boost the rating */
	rating += v_ptr->rat;

	/* (Sometimes) Cause a special feeling */
	if ((dun_level <= 50) ||
	                (randint((dun_level - 40) * (dun_level - 40) + 50) < 400))
	{
		good_item_flag = TRUE;
	}

	/* Build the vault */
	if (v_ptr->dynamic)
	{
		call_lua("load_map", "(s,d,d,n,b)", "", format("dungeon/vaults/%s", v_ptr->text), yval, xval, TRUE);
	}
	else
	{
		build_vault(yval, xval, v_ptr->hgt, v_ptr->wid, v_ptr->text);
	}
}


/*
 * DAG:
 * Build an vertical oval room.
 * For every grid in the possible square, check the distance.
 * If it's less than or == than the radius, make it a room square.
 * If its less, make it a normal grid. If it's == make it an outer
 * wall.
 */
static void build_type9(s32b by0, s32b bx0)
{
	s32b rad, x, y, x0, y0;

	s32b light = FALSE;

	/* Occasional light */
	if (randint(dun_level) <= 5) light = TRUE;

	rad = 2 + rand_int(8);

	/* Try to allocate space for room.  If fails, exit */
	if (!room_alloc(rad*2 + 1, rad*2 + 1, FALSE, by0, bx0, &x0, &y0)) return;

	for (x = x0 - rad; x <= x0 + rad; x++)
	{
		for (y = y0 - rad; y <= y0 + rad; y++)
		{
			if (distance(y0, x0, y, x) == rad)
			{
				cave[y][x].info |= (CAVE_ROOM);
				if (light) cave[y][x].info |= (CAVE_GLOW);

				cave_set_feat(y, x, feat_wall_outer);
			}

			if (distance(y0, x0, y, x) < rad)
			{
				cave[y][x].info |= (CAVE_ROOM);
				if (light) cave[y][x].info |= (CAVE_GLOW);

				place_floor(y, x);
			}
		}
	}
}


/*
 * Store routine for the fractal cave generator
 * this routine probably should be an inline function or a macro
 */
static void store_height(s32b x, s32b y, s32b x0, s32b y0, byte val,
                         s32b xhsize, s32b yhsize, s32b cutoff)
{
	/* Only write to points that are "blank" */
	if (cave[y + y0 - yhsize][x + x0 - xhsize].feat != EMPTY_FEAT) return;

	/* If on boundary set val > cutoff so walls are not as square */
	if (((x == 0) || (y == 0) || (x == xhsize * 2) || (y == yhsize * 2)) &&
	                (val <= cutoff)) val = cutoff + 1;

	/* Store the value in height-map format */
	/* Meant to be temporary, hence no cave_set_feat */
	cave[y + y0 - yhsize][x + x0 - xhsize].feat = val;

	return;
}



/*
 * Explanation of the plasma fractal algorithm:
 *
 * A grid of points is created with the properties of a 'height-map'
 * This is done by making the corners of the grid have a random value.
 * The grid is then subdivided into one with twice the resolution.
 * The new points midway between two 'known' points can be calculated
 * by taking the average value of the 'known' ones and randomly adding
 * or subtracting an amount proportional to the distance between those
 * points.  The final 'middle' points of the grid are then calculated
 * by averaging all four of the originally 'known' corner points.  An
 * random amount is added or subtracted from this to get a value of the
 * height at that point.  The scaling factor here is adjusted to the
 * slightly larger distance diagonally as compared to orthogonally.
 *
 * This is then repeated recursively to fill an entire 'height-map'
 * A rectangular map is done the same way, except there are different
 * scaling factors along the x and y directions.
 *
 * A hack to change the amount of correlation between points is done using
 * the grd variable.  If the current step size is greater than grd then
 * the point will be random, otherwise it will be calculated by the
 * above algorithm.  This makes a maximum distance at which two points on
 * the height map can affect each other.
 *
 * How fractal caves are made:
 *
 * When the map is complete, a cut-off value is used to create a cave.
 * Heights below this value are "floor", and heights above are "wall".
 * This also can be used to create lakes, by adding more height levels
 * representing shallow and deep water/ lava etc.
 *
 * The grd variable affects the width of passages.
 * The roug variable affects the roughness of those passages
 *
 * The tricky part is making sure the created cave is connected.  This
 * is done by 'filling' from the inside and only keeping the 'filled'
 * floor.  Walls bounding the 'filled' floor are also kept.  Everything
 * else is converted to the normal granite FEAT_WALL_EXTRA.
 */


/*
 * Note that this uses the cave.feat array in a very hackish way
 * the values are first set to zero, and then each array location
 * is used as a "heightmap"
 * The heightmap then needs to be converted back into the "feat" format.
 *
 * grd=level at which fractal turns on.  smaller gives more mazelike caves
 * roug=roughness level.  16=normal.  higher values make things more
 * convoluted small values are good for smooth walls.
 * size=length of the side of the square cave system.
 */

void generate_hmap(s32b y0, s32b x0, s32b xsiz, s32b ysiz, s32b grd,
                   s32b roug, s32b cutoff)
{
	s32b xhsize, yhsize, xsize, ysize, maxsize;

	/*
	 * fixed point variables- these are stored as 256 x normal value
	 * this gives 8 binary places of fractional part + 8 places of normal part
	 */
	u16b xstep, xhstep, ystep, yhstep, i, j, diagsize, xxsize, yysize;

	/* Redefine size so can change the value if out of range */
	xsize = xsiz;
	ysize = ysiz;

	/* Paranoia about size of the system of caves*/
	if (xsize > 254) xsize = 254;
	if (xsize < 4) xsize = 4;
	if (ysize > 254) ysize = 254;
	if (ysize < 4) ysize = 4;

	/* Get offsets to middle of array */
	xhsize = xsize / 2;
	yhsize = ysize / 2;

	/* Fix rounding problem */
	xsize = xhsize * 2;
	ysize = yhsize * 2;

	/*
	 * Scale factor for middle points:
	 * About sqrt(2)*256 - correct for a square lattice
	 * approximately correct for everything else.
	 */
	diagsize = 362;

	/* Maximum of xsize and ysize */
	maxsize = (xsize > ysize) ? xsize : ysize;

	/* Clear the section */
	for (i = 0; i <= xsize; i++)
	{
		for (j = 0; j <= ysize; j++)
		{
			cave_type *c_ptr;

			/* Access the grid */
			c_ptr = &cave[j + y0 - yhsize][i + x0 - xhsize];

			/* EMPTY_FEAT is code for "not done yet" */
			c_ptr->feat = EMPTY_FEAT;

			/* Clear icky flag because may be redoing the cave */
			c_ptr->info &= ~(CAVE_ICKY);
		}
	}
	/* Set the corner values just in case grd>size. */
	store_height(0, 0, x0, y0, maxsize, xhsize, yhsize, cutoff);
	store_height(0, ysize, x0, y0, maxsize, xhsize, yhsize, cutoff);
	store_height(xsize, 0, x0, y0, maxsize, xhsize, yhsize, cutoff);
	store_height(xsize, ysize, x0, y0, maxsize, xhsize, yhsize, cutoff);

	/* Set the middle square to be an open area. */
	store_height(xhsize, yhsize, x0, y0, 0, xhsize, yhsize, cutoff);


	/* Initialise the step sizes */
	xstep = xhstep = xsize * 256;
	ystep = yhstep = ysize * 256;
	xxsize = xsize * 256;
	yysize = ysize * 256;

	/*
	 * Fill in the rectangle with fractal height data - like the
	 * 'plasma fractal' in fractint
	 */
	while ((xstep / 256 > 1) || (ystep / 256 > 1))
	{
		/* Halve the step sizes */
		xstep = xhstep;
		xhstep /= 2;
		ystep = yhstep;
		yhstep /= 2;

		/* Middle top to bottom */
		for (i = xhstep; i <= xxsize - xhstep; i += xstep)
		{
			for (j = 0; j <= yysize; j += ystep)
			{
				/* If greater than 'grid' level then is random */
				if (xhstep / 256 > grd)
				{
					store_height(i / 256, j / 256, x0, y0, randint(maxsize),
					             xhsize, yhsize, cutoff);
				}
				else
				{
					cave_type *l, *r;
					s16b val;

					/* Left point */
					l = &cave[j / 256 + y0 - yhsize][(i - xhstep) / 256 + x0 - xhsize];

					/* Right point */
					r = &cave[j / 256 + y0 - yhsize][(i + xhstep) / 256 + x0 - xhsize];

					/* Average of left and right points + random bit */
					val = (l->feat + r->feat) / 2 +
					      (randint(xstep / 256) - xhstep / 256) * roug / 16;

					store_height(i / 256, j / 256, x0, y0, val,
					             xhsize, yhsize, cutoff);
				}
			}
		}


		/* Middle left to right */
		for (j = yhstep; j <= yysize - yhstep; j += ystep)
		{
			for (i = 0; i <= xxsize; i += xstep)
			{
				/* If greater than 'grid' level then is random */
				if (xhstep / 256 > grd)
				{
					store_height(i / 256, j / 256, x0, y0, randint(maxsize),
					             xhsize, yhsize, cutoff);
				}
				else
				{
					cave_type *u, *d;
					s16b val;

					/* Up point */
					u = &cave[(j - yhstep) / 256 + y0 - yhsize][i / 256 + x0 - xhsize];

					/* Down point */
					d = &cave[(j + yhstep) / 256 + y0 - yhsize][i / 256 + x0 - xhsize];

					/* Average of up and down points + random bit */
					val = (u->feat + d->feat) / 2 +
					      (randint(ystep / 256) - yhstep / 256) * roug / 16;

					store_height(i / 256, j / 256, x0, y0, val,
					             xhsize, yhsize, cutoff);
				}
			}
		}

		/* Center */
		for (i = xhstep; i <= xxsize - xhstep; i += xstep)
		{
			for (j = yhstep; j <= yysize - yhstep; j += ystep)
			{
				/* If greater than 'grid' level then is random */
				if (xhstep / 256 > grd)
				{
					store_height(i / 256, j / 256, x0, y0, randint(maxsize),
					             xhsize, yhsize, cutoff);
				}
				else
				{
					cave_type *ul, *dl, *ur, *dr;
					s16b val;

					/* Up-left point */
					ul = &cave[(j - yhstep) / 256 + y0 - yhsize][(i - xhstep) / 256 + x0 - xhsize];

					/* Down-left point */
					dl = &cave[(j + yhstep) / 256 + y0 - yhsize][(i - xhstep) / 256 + x0 - xhsize];

					/* Up-right point */
					ur = &cave[(j - yhstep) / 256 + y0 - yhsize][(i + xhstep) / 256 + x0 - xhsize];

					/* Down-right point */
					dr = &cave[(j + yhstep) / 256 + y0 - yhsize][(i + xhstep) / 256 + x0 - xhsize];

					/*
					 * average over all four corners + scale by diagsize to
					 * reduce the effect of the square grid on the shape
					 * of the fractal
					 */
					val = (ul->feat + dl->feat + ur->feat + dr->feat) / 4 +
					      (randint(xstep / 256) - xhstep / 256) *
					      (diagsize / 16) / 256 * roug;

					store_height(i / 256, j / 256, x0, y0, val,
					             xhsize, yhsize , cutoff);
				}
			}
		}
	}
}


/*
 * Convert from height-map back to the normal Angband cave format
 */
static bool hack_isnt_wall(s32b y, s32b x, s32b cutoff)
{
	/* Already done */
	if (cave[y][x].info & CAVE_ICKY)
	{
		return (FALSE);
	}

	else
	{
		/* Show that have looked at this square */
		cave[y][x].info |= (CAVE_ICKY);

		/* If less than cutoff then is a floor */
		if (cave[y][x].feat <= cutoff)
		{
			place_floor(y, x);
			return (TRUE);
		}

		/* If greater than cutoff then is a wall */
		else
		{
			cave_set_feat(y, x, feat_wall_outer);
			return (FALSE);
		}
	}
}


/*
 * Quick and nasty fill routine used to find the connected region
 * of floor in the middle of the cave
 */
static void fill_hack(s32b y0, s32b x0, s32b y, s32b x, s32b xsize, s32b ysize,
                      s32b cutoff, s32b *amount)
{
	s32b i, j;

	/* check 8 neighbours +self (self is caught in the isnt_wall function) */
	for (i = -1; i <= 1; i++)
	{
		for (j = -1; j <= 1; j++)
		{
			/* If within bounds */
			if ((x + i > 0) && (x + i < xsize) &&
			                (y + j > 0) && (y + j < ysize))
			{
				/* If not a wall or floor done before */
				if (hack_isnt_wall(y + j + y0 - ysize / 2,
				                   x + i + x0 - xsize / 2, cutoff))
				{
					/* then fill from the new point*/
					fill_hack(y0, x0, y + j, x + i, xsize, ysize,
					          cutoff, amount);

					/* keep tally of size of cave system */
					(*amount)++;
				}
			}

			/* Affect boundary */
			else
			{
				cave[y0 + y + j - ysize / 2][x0 + x + i - xsize / 2].info |= (CAVE_ICKY);
			}
		}
	}
}

bool generate_fracave(s32b y0, s32b x0, s32b xsize, s32b ysize,
                      s32b cutoff, bool light, bool room)
{
	s32b x, y, i, amount, xhsize, yhsize;
	cave_type *c_ptr;

	/* Offsets to middle from corner */
	xhsize = xsize / 2;
	yhsize = ysize / 2;

	/* Reset tally */
	amount = 0;

	/*
	 * Select region connected to center of cave system
	 * this gets rid of alot of isolated one-sqaures that
	 * can make teleport traps instadeaths...
	 */
	fill_hack(y0, x0, yhsize, xhsize, xsize, ysize, cutoff, &amount);

	/* If tally too small, try again */
	if (amount < 10)
	{
		/* Too small -- clear area and try again later */
		for (x = 0; x <= xsize; ++x)
		{
			for (y = 0; y < ysize; ++y)
			{
				place_filler(y0 + y - yhsize, x0 + x - xhsize);
				cave[y0 + y - yhsize][x0 + x - xhsize].info &= ~(CAVE_ICKY | CAVE_ROOM);
			}
		}
		return FALSE;
	}


	/*
	 * Do boundaries -- check to see if they are next to a filled region
	 * If not then they are set to normal granite
	 * If so then they are marked as room walls
	 */
	for (i = 0; i <= xsize; ++i)
	{
		/* Access top boundary grid */
		c_ptr = &cave[0 + y0 - yhsize][i + x0 - xhsize];

		/* Next to a 'filled' region? -- set to be room walls */
		if (c_ptr->info & CAVE_ICKY)
		{
			cave_set_feat(0 + y0 - yhsize, i + x0 - xhsize, feat_wall_outer);

			if (light) c_ptr->info |= (CAVE_GLOW);
			if (room)
			{
				c_ptr->info |= (CAVE_ROOM);
			}
			else
			{
				place_filler(0 + y0 - yhsize, i + x0 - xhsize);
			}
		}

		/* Outside of the room -- set to be normal granite */
		else
		{
			place_filler(0 + y0 - yhsize, i + x0 - xhsize);
		}

		/* Clear the icky flag -- don't need it any more */
		c_ptr->info &= ~(CAVE_ICKY);


		/* Access bottom boundary grid */
		c_ptr = &cave[ysize + y0 - yhsize][i + x0 - xhsize];

		/* Next to a 'filled' region? -- set to be room walls */
		if (c_ptr->info & CAVE_ICKY)
		{
			cave_set_feat(ysize + y0 - yhsize, i + x0 - xhsize, feat_wall_outer);
			if (light) c_ptr->info |= (CAVE_GLOW);
			if (room)
			{
				c_ptr->info |= (CAVE_ROOM);
			}
			else
			{
				place_filler(ysize + y0 - yhsize, i + x0 - xhsize);
			}
		}

		/* Outside of the room -- set to be normal granite */
		else
		{
			place_filler(ysize + y0 - yhsize, i + x0 - xhsize);
		}

		/* Clear the icky flag -- don't need it any more */
		c_ptr->info &= ~(CAVE_ICKY);
	}


	/* Do the left and right boundaries minus the corners (done above) */
	for (i = 1; i < ysize; ++i)
	{
		/* Access left boundary grid */
		c_ptr = &cave[i + y0 - yhsize][0 + x0 - xhsize];

		/* Next to a 'filled' region? -- set to be room walls */
		if (c_ptr->info & CAVE_ICKY)
		{
			cave_set_feat(i + y0 - yhsize, 0 + x0 - xhsize, feat_wall_outer);
			if (light) c_ptr->info |= (CAVE_GLOW);
			if (room)
			{
				c_ptr->info |= (CAVE_ROOM);
			}
			else
			{
				place_filler(i + y0 - yhsize, 0 + x0 - xhsize);
			}
		}

		/* Outside of the room -- set to be normal granite */
		else
		{
			place_filler(i + y0 - yhsize, 0 + x0 - xhsize);
		}

		/* Clear the icky flag -- don't need it any more */
		c_ptr->info &= ~(CAVE_ICKY);


		/* Access left boundary grid */
		c_ptr = &cave[i + y0 - yhsize][xsize + x0 - xhsize];

		/* Next to a 'filled' region? -- set to be room walls */
		if (c_ptr->info & CAVE_ICKY)
		{
			cave_set_feat(i + y0 - yhsize, xsize + x0 - xhsize, feat_wall_outer);
			if (light) c_ptr->info |= (CAVE_GLOW);
			if (room)
			{
				c_ptr->info |= (CAVE_ROOM);
			}
			else
			{
				place_filler(i + y0 - yhsize, xsize + x0 - xhsize);
			}
		}

		/* Outside of the room -- set to be normal granite */
		else
		{
			place_filler(i + y0 - yhsize, xsize + x0 - xhsize);
		}

		/* Clear the icky flag -- don't need it any more */
		c_ptr->info &= ~(CAVE_ICKY);
	}


	/*
	 * Do the rest: convert back to the normal format
	 * In other variants, may want to check to see if cave.feat< some value
	 * if so, set to be water:- this will make interesting pools etc.
	 * (I don't do this for standard Angband.)
	 */
	for (x = 1; x < xsize; ++x)
	{
		for (y = 1; y < ysize; ++y)
		{
			/* Access the grid */
			c_ptr = &cave[y + y0 - yhsize][x + x0 - xhsize];

			/* A floor grid to be converted */
			if (has_flag(&f_info[c_ptr->feat], FLAG_FLOOR) && (c_ptr->info & CAVE_ICKY))

			{
				/* Clear the icky flag in the filled region */
				c_ptr->info &= ~(CAVE_ICKY);

				/* Set appropriate flags */
				if (light) c_ptr->info |= (CAVE_GLOW);
				if (room) c_ptr->info |= (CAVE_ROOM);
			}

			/* A wall grid to be convereted */
			else if ((c_ptr->feat == feat_wall_outer) &&
			                (c_ptr->info & CAVE_ICKY))
			{
				/* Clear the icky flag in the filled region */
				c_ptr->info &= ~(CAVE_ICKY);

				/* Set appropriate flags */
				if (light) c_ptr->info |= (CAVE_GLOW);
				if (room)
				{
					c_ptr->info |= (CAVE_ROOM);
				}
				else
				{
					place_filler(y + y0 - yhsize, x + x0 - xhsize);
				}
			}

			/* None of the above -- clear the unconnected regions */
			else
			{
				place_filler(y + y0 - yhsize, x + x0 - xhsize);
				c_ptr->info &= ~(CAVE_ICKY | CAVE_ROOM);
			}
		}
	}

	/*
	 * XXX XXX XXX There is a slight problem when tunnels pierce the caves:
	 * Extra doors appear inside the system.  (Its not very noticeable though.)
	 * This can be removed by "filling" from the outside in.  This allows
	 * a separation from FEAT_WALL_OUTER with FEAT_WALL_INNER.  (Internal
	 * walls are  F.W.OUTER instead.)
	 * The extra effort for what seems to be only a minor thing (even
	 * non-existant if you think of the caves not as normal rooms, but as
	 * holes in the dungeon), doesn't seem worth it.
	 */

	return (TRUE);
}


/*
 * Makes a cave system in the center of the dungeon
 */
static void build_cavern(void)
{
	s32b grd, roug, cutoff, xsize, ysize, x0, y0;
	bool done, light, room;

	light = done = room = FALSE;
	if (dun_level <= randint(25)) light = TRUE;

	/* Make a cave the size of the dungeon */
	xsize = cur_wid - 1;
	ysize = cur_hgt - 1;
	x0 = xsize / 2;
	y0 = ysize / 2;

	/* Paranoia: make size even */
	xsize = x0 * 2;
	ysize = y0 * 2;

	while (!done)
	{
		/* Testing values for these parameters: feel free to adjust */
		grd = 2 ^ (randint(4) + 4);

		/* Want average of about 16 */
		roug = randint(8) * randint(4);

		/* About size/2 */
		cutoff = xsize / 2;

		/* Make it */
		generate_hmap(y0, x0, xsize, ysize, grd, roug, cutoff);

		/* Convert to normal format+ clean up*/
		done = generate_fracave(y0, x0, xsize, ysize, cutoff, light, room);
	}
}


/*
 * Driver routine to create fractal cave system
 */
static void build_type10(s32b by0, s32b bx0)
{
	s32b grd, roug, cutoff, xsize, ysize, y0, x0;

	bool done, light, room;

	/* Get size: note 'Evenness'*/
	xsize = randint(22) * 2 + 6;
	ysize = randint(15) * 2 + 6;

	/* Try to allocate space for room.  If fails, exit */
	if (!room_alloc(xsize + 1, ysize + 1, FALSE, by0, bx0, &x0, &y0)) return;

	light = done = FALSE;
	room = TRUE;

	if (dun_level <= randint(25)) light = TRUE;

	while (!done)
	{
		/*
		 * Note: size must be even or there are rounding problems
		 * This causes the tunnels not to connect properly to the room
		 */

		/* Testing values for these parameters feel free to adjust */
		grd = 2 ^ (randint(4));

		/* Want average of about 16 */
		roug = randint(8) * randint(4);

		/* About size/2 */
		cutoff = randint(xsize / 4) + randint(ysize / 4) +
		         randint(xsize / 4) + randint(ysize / 4);

		/* Make it */
		generate_hmap(y0, x0, xsize, ysize, grd, roug, cutoff);

		/* Convert to normal format + clean up*/
		done = generate_fracave(y0, x0, xsize, ysize, cutoff, light, room);
	}
}


/*
 * Random vault generation from Z 2.5.1
 */

/*
 * Make a very small room centred at (x0, y0)
 *
 * This is used in crypts, and random elemental vaults.
 *
 * Note - this should be used only on allocated regions
 * within another room.
 */
static void build_small_room(s32b x0, s32b y0)
{
	build_rectangle(y0 - 1, x0 - 1, y0 + 1, x0 + 1, feat_wall_inner, CAVE_ROOM);

	/* Place a secret door on one side */
	switch (rand_int(4))
	{
	case 0:
		{
			place_secret_door(y0, x0 - 1);
			break;
		}

	case 1:
		{
			place_secret_door(y0, x0 + 1);
			break;
		}

	case 2:
		{
			place_secret_door(y0 - 1, x0);
			break;
		}

	case 3:
		{
			place_secret_door(y0 + 1, x0);
			break;
		}
	}

	/* Add inner open space */
	place_floor(y0, x0);
}


/*
 * Add a door to a location in a random vault
 *
 * Note that range checking has to be done in the calling routine.
 *
 * The doors must be INSIDE the allocated region.
 */
static void add_door(s32b x, s32b y)
{
	/* Need to have a wall in the center square */
	if (cave[y][x].feat != feat_wall_outer) return;

	/*
	 * Look at:
	 *  x#x
	 *  .#.
	 *  x#x
	 *
	 *  where x=don't care
	 *  .=floor, #=wall
	 */

	if (get_is_floor(x, y - 1) && get_is_floor(x, y + 1) &&
	                (cave[y][x - 1].feat == feat_wall_outer) &&
	                (cave[y][x + 1].feat == feat_wall_outer))
	{
		/* secret door */
		place_secret_door(y, x);

		/* set boundarys so don't get wide doors */
		place_filler(y, x - 1);
		place_filler(y, x + 1);
	}


	/*
	 * Look at:
	 *  x#x
	 *  .#.
	 *  x#x
	 *
	 *  where x = don't care
	 *  .=floor, #=wall
	 */
	if ((cave[y - 1][x].feat == feat_wall_outer) &&
	                (cave[y + 1][x].feat == feat_wall_outer) &&
	                get_is_floor(x - 1, y) && get_is_floor(x + 1, y))
	{
		/* secret door */
		place_secret_door(y, x);

		/* set boundarys so don't get wide doors */
		place_filler(y - 1, x);
		place_filler(y + 1, x);
	}
}


/*
 * Fill the empty areas of a room with treasure and monsters.
 */
static void fill_treasure(s32b x1, s32b x2, s32b y1, s32b y2, s32b difficulty)
{
	s32b x, y, cx, cy, size;
	s32b value;

	/* center of room:*/
	cx = (x1 + x2) / 2;
	cy = (y1 + y2) / 2;

	/* Rough measure of size of vault= sum of lengths of sides */
	size = abs(x2 - x1) + abs(y2 - y1);

	for (x = x1; x <= x2; x++)
	{
		for (y = y1; y <= y2; y++)
		{
			/*
			 * Thing added based on distance to center of vault
			 * Difficulty is 1-easy to 10-hard
			 */
			value = (((s32b)distance(cx, cy, x, y) * 100) / size) +
			        randint(10) - difficulty;

			/* Hack -- Empty square part of the time */
			if ((randint(100) - difficulty * 3) > 50) value = 20;

			/* If floor, shallow water or lava */
			if (get_is_floor(x, y))
			{
				/* The smaller 'value' is, the better the stuff */
				if (value < 0)
				{
					/* Meanest monster + treasure */
					monster_level = dun_level + 40;
					place_monster(y, x, TRUE, TRUE);
					monster_level = dun_level;
					object_level = dun_level + 20;
					place_object(y, x, TRUE, FALSE, OBJ_FOUND_FLOOR);
					object_level = dun_level;
				}
				else if (value < 5)
				{
					/* Mean monster +treasure */
					monster_level = dun_level + 20;
					place_monster(y, x, TRUE, TRUE);
					monster_level = dun_level;
					object_level = dun_level + 10;
					place_object(y, x, TRUE, FALSE, OBJ_FOUND_FLOOR);
					object_level = dun_level;
				}
				else if (value < 10)
				{
					/* Monster */
					monster_level = dun_level + 9;
					place_monster(y, x, TRUE, TRUE);
					monster_level = dun_level;
				}
				else if (value < 17)
				{
					/* Intentional Blank space */

					/*
					 * (Want some of the vault to be empty
					 * so have room for group monsters.
					 * This is used in the hack above to lower
					 * the density of stuff in the vault.)
					 */
				}
				else if (value < 23)
				{
					/* Object or trap */
					if (rand_int(100) < 25)
					{
						place_object(y, x, FALSE, FALSE, OBJ_FOUND_FLOOR);
					}
					else
					{
						place_trap(y, x, FACTION_DUNGEON);
					}
				}
				else if (value < 30)
				{
					/* Monster and trap */
					monster_level = dun_level + 5;
					place_monster(y, x, TRUE, TRUE);
					monster_level = dun_level;
					place_trap(y, x, FACTION_DUNGEON);
				}
				else if (value < 40)
				{
					/* Monster or object */
					if (rand_int(100) < 50)
					{
						monster_level = dun_level + 3;
						place_monster(y, x, TRUE, TRUE);
						monster_level = dun_level;
					}
					if (rand_int(100) < 50)
					{
						object_level = dun_level + 7;
						place_object(y, x, FALSE, FALSE, OBJ_FOUND_FLOOR);
						object_level = dun_level;
					}
				}
				else if (value < 50)
				{
					/* Trap */
					place_trap(y, x, FACTION_DUNGEON);
				}
				else
				{
					/* Various Stuff */

					/* 20% monster, 40% trap, 20% object, 20% blank space */
					if (rand_int(100) < 20)
					{
						place_monster(y, x, TRUE, TRUE);
					}
					else if (rand_int(100) < 50)
					{
						place_trap(y, x, FACTION_DUNGEON);
					}
					else if (rand_int(100) < 50)
					{
						place_object(y, x, FALSE, FALSE, OBJ_FOUND_FLOOR);
					}
				}

			}
		}
	}
}


/*
 * Creates a random vault that looks like a collection of bubbles
 *
 * It works by getting a set of coordinates that represent the center of
 * each bubble.  The entire room is made by seeing which bubble center is
 * closest. If two centers are equidistant then the square is a wall,
 * otherwise it is a floor. The only exception is for squares really
 * near a center, these are always floor.
 * (It looks better than without this check.)
 *
 * Note: If two centers are on the same point then this algorithm will create a
 * blank bubble filled with walls. - This is prevented from happening.
 */

#define BUBBLENUM 10 /* number of bubbles */

static void build_bubble_vault(s32b x0, s32b y0, s32b xsize, s32b ysize)
{
	/* array of center points of bubbles */
	coord center[BUBBLENUM];

	s32b i, j, k, x = 0, y = 0;
	u16b min1, min2, temp;
	bool done;

	/* Offset from center to top left hand corner */
	s32b xhsize = xsize / 2;
	s32b yhsize = ysize / 2;

	if (cheat_room) msg_print("Bubble Vault");

	/* Allocate center of bubbles */
	center[0].x = randint(xsize - 3) + 1;
	center[0].y = randint(ysize - 3) + 1;

	for (i = 1; i < BUBBLENUM; i++)
	{
		done = FALSE;

		/* Get center and check to see if it is unique */
		for (k = 0; !done && (k < 2000); k++)
		{
			done = TRUE;

			x = randint(xsize - 3) + 1;
			y = randint(ysize - 3) + 1;

			for (j = 0; j < i; j++)
			{
				/* Rough test to see if there is an overlap */
				if ((x == center[j].x) || (y == center[j].y)) done = FALSE;
			}
		}

		/* Too many failures */
		if (k >= 2000) return;

		center[i].x = x;
		center[i].y = y;
	}

	build_rectangle(y0 - yhsize, x0 - xhsize,
	                y0 - yhsize + ysize - 1, x0 - xhsize + xsize - 1,
	                feat_wall_outer, CAVE_ROOM | CAVE_ICKY);

	/* Fill in middle with bubbles */
	for (x = 1; x < xsize - 1; x++)
	{
		for (y = 1; y < ysize - 1; y++)
		{
			cave_type *c_ptr;

			/* Get distances to two closest centers */

			/* Initialise */
			min1 = distance(x, y, center[0].x, center[0].y);
			min2 = distance(x, y, center[1].x, center[1].y);

			if (min1 > min2)
			{
				/* Swap if in wrong order */
				temp = min1;
				min1 = min2;
				min2 = temp;
			}

			/* Scan the rest */
			for (i = 2; i < BUBBLENUM; i++)
			{
				temp = distance(x, y, center[i].x, center[i].y);

				if (temp < min1)
				{
					/* Smallest */
					min2 = min1;
					min1 = temp;
				}
				else if (temp < min2)
				{
					/* Second smallest */
					min2 = temp;
				}
			}

			/* Access the grid */
			c_ptr = &cave[y + y0 - yhsize][x + x0 - xhsize];

			/*
			 * Boundary at midpoint+ not at inner region of bubble
			 *
			 * SCSCSC: was feat_wall_outer
			 */
			if (((min2 - min1) <= 2) && (!(min1 < 3)))
			{
				place_filler(y + y0 - yhsize, x + x0 - xhsize);
			}

			/* Middle of a bubble */
			else
			{
				place_floor(y + y0 - yhsize, x + x0 - xhsize);
			}

			/* Clean up rest of flags */
			c_ptr->info |= (CAVE_ROOM | CAVE_ICKY);
		}
	}

	/* Try to add some random doors */
	for (i = 0; i < 500; i++)
	{
		x = randint(xsize - 3) - xhsize + x0 + 1;
		y = randint(ysize - 3) - yhsize + y0 + 1;
		add_door(x, y);
	}

	/* Fill with monsters and treasure, low difficulty */
	fill_treasure(x0 - xhsize + 1, x0 - xhsize + xsize - 2,
	              y0 - yhsize + 1, y0 - yhsize + ysize - 2, randint(5));
}


/*
 * Convert FEAT_WALL_EXTRA (used by random vaults) to normal dungeon wall
 */
static void convert_extra(s32b y1, s32b x1, s32b y2, s32b x2)
{
	s32b x, y;

	for (x = x1; x <= x2; x++)
	{
		for (y = y1; y <= y2; y++)
		{
			if (cave[y][x].feat == FEAT_WALL_OUTER)
			{
				place_filler(y, x);
			}
		}
	}
}


/*
 * Overlay a rectangular room given its bounds
 *
 * This routine is used by build_room_vault (hence FEAT_WALL_OUTER)
 * The area inside the walls is not touched: only granite is removed
 * and normal walls stay
 */
static void build_room(s32b x1, s32b x2, s32b y1, s32b y2)
{
	s32b x, y, xsize, ysize, temp;

	/* Check if rectangle has no width */
	if ((x1 == x2) || (y1 == y2)) return;

	/* initialize */
	if (x1 > x2)
	{
		/* Swap boundaries if in wrong order */
		temp = x1;
		x1 = x2;
		x2 = temp;
	}

	if (y1 > y2)
	{
		/* Swap boundaries if in wrong order */
		temp = y1;
		y1 = y2;
		y2 = temp;
	}

	/* Get total widths */
	xsize = x2 - x1;
	ysize = y2 - y1;

	build_rectangle(y1, x1, y2, x2, feat_wall_outer, CAVE_ROOM | CAVE_ICKY);

	/* Middle */
	for (x = 1; x < xsize; x++)
	{
		for (y = 1; y < ysize; y++)
		{
			if (cave[y1 + y][x1 + x].feat == FEAT_WALL_OUTER)
			{
				/* Clear the untouched region */
				place_floor(y1 + y, x1 + x);
				cave[y1 + y][x1 + x].info |= (CAVE_ROOM | CAVE_ICKY);
			}
			else
			{
				/* Make it a room -- but don't touch */
				cave[y1 + y][x1 + x].info |= (CAVE_ROOM | CAVE_ICKY);
			}
		}
	}
}


/*
 * Create a random vault that looks like a collection of overlapping rooms
 */
static void build_room_vault(s32b x0, s32b y0, s32b xsize, s32b ysize)
{
	s32b i, x1, x2, y1, y2, xhsize, yhsize;

	/* Get offset from center */
	xhsize = xsize / 2;
	yhsize = ysize / 2;

	if (cheat_room) msg_print("Room Vault");

	/* Fill area so don't get problems with arena levels */
	for (x1 = 0; x1 <= xsize; x1++)
	{
		s32b x = x0 - xhsize + x1;

		for (y1 = 0; y1 <= ysize; y1++)
		{
			s32b y = y0 - yhsize + y1;

			cave_set_feat(y, x, FEAT_WALL_OUTER);
			cave[y][x].info &= ~(CAVE_ICKY);
		}
	}

	/* Add ten random rooms */
	for (i = 0; i < 10; i++)
	{
		x1 = randint(xhsize) * 2 + x0 - xhsize;
		x2 = randint(xhsize) * 2 + x0 - xhsize;
		y1 = randint(yhsize) * 2 + y0 - yhsize;
		y2 = randint(yhsize) * 2 + y0 - yhsize;

		build_room(x1, x2, y1, y2);
	}

	convert_extra(y0 - yhsize, x0 - xhsize, y0 - yhsize + ysize,
	              x0 - xhsize + xsize);

	/* Add some random doors */
	for (i = 0; i < 500; i++)
	{
		x1 = randint(xsize - 2) - xhsize + x0 + 1;
		y1 = randint(ysize - 2) - yhsize + y0 + 1;
		add_door(x1, y1);
	}

	/* Fill with monsters and treasure, high difficulty */
	fill_treasure(x0 - xhsize + 1, x0 - xhsize + xsize - 1,
	              y0 - yhsize + 1, y0 - yhsize + ysize - 1, randint(5) + 5);
}


/*
 * Create a random vault out of a fractal cave
 */
static void build_cave_vault(s32b x0, s32b y0, s32b xsiz, s32b ysiz)
{
	s32b grd, roug, cutoff, xhsize, yhsize, xsize, ysize, x, y;
	bool done, light, room;

	/* Round to make sizes even */
	xhsize = xsiz / 2;
	yhsize = ysiz / 2;
	xsize = xhsize * 2;
	ysize = yhsize * 2;

	if (cheat_room) msg_print("Cave Vault");

	light = done = FALSE;
	room = TRUE;

	while (!done)
	{
		/* Testing values for these parameters feel free to adjust */
		grd = 2 ^ rand_int(4);

		/* Want average of about 16 */
		roug = randint(8) * randint(4);

		/* About size/2 */
		cutoff = randint(xsize / 4) + randint(ysize / 4) +
		         randint(xsize / 4) + randint(ysize / 4);

		/* Make it */
		generate_hmap(y0, x0, xsize, ysize, grd, roug, cutoff);

		/* Convert to normal format + clean up */
		done = generate_fracave(y0, x0, xsize, ysize, cutoff, light, room);
	}

	/* Set icky flag because is a vault */
	for (x = 0; x <= xsize; x++)
	{
		for (y = 0; y <= ysize; y++)
		{
			cave[y0 - yhsize + y][x0 - xhsize + x].info |= CAVE_ICKY;
		}
	}

	/* Fill with monsters and treasure, low difficulty */
	fill_treasure(x0 - xhsize + 1, x0 - xhsize + xsize - 1,
	              y0 - yhsize + 1, y0 - yhsize + ysize - 1, randint(5));
}


/*
 * Maze vault -- rectangular labyrinthine rooms
 *
 * maze vault uses two routines:
 *    r_visit - a recursive routine that builds the labyrinth
 *    build_maze_vault - a driver routine that calls r_visit and adds
 *                   monsters, traps and treasure
 *
 * The labyrinth is built by creating a spanning tree of a graph.
 * The graph vertices are at
 *    (x, y) = (2j + x1, 2k + y1)   j = 0,...,m-1    k = 0,...,n-1
 * and the edges are the vertical and horizontal nearest neighbors.
 *
 * The spanning tree is created by performing a suitably randomized
 * depth-first traversal of the graph. The only adjustable parameter
 * is the rand_int(3) below; it governs the relative density of
 * twists and turns in the labyrinth: smaller number, more twists.
 */
static void r_visit(s32b y1, s32b x1, s32b y2, s32b x2,
                    s32b node, s32b dir, s32b *visited)
{
	s32b i, j, m, n, temp, x, y, adj[4];

	/* Dimensions of vertex array */
	m = (x2 - x1) / 2 + 1;
	n = (y2 - y1) / 2 + 1;

	/* Mark node visited and set it to a floor */
	visited[node] = 1;
	x = 2 * (node % m) + x1;
	y = 2 * (node / m) + y1;
	place_floor(y, x);

	/* Setup order of adjacent node visits */
	if (rand_int(3) == 0)
	{
		/* Pick a random ordering */
		for (i = 0; i < 4; i++)
		{
			adj[i] = i;
		}
		for (i = 0; i < 4; i++)
		{
			j = rand_int(4);
			temp = adj[i];
			adj[i] = adj[j];
			adj[j] = temp;
		}
		dir = adj[0];
	}
	else
	{
		/* Pick a random ordering with dir first */
		adj[0] = dir;
		for (i = 1; i < 4; i++)
		{
			adj[i] = i;
		}
		for (i = 1; i < 4; i++)
		{
			j = 1 + rand_int(3);
			temp = adj[i];
			adj[i] = adj[j];
			adj[j] = temp;
		}
	}

	for (i = 0; i < 4; i++)
	{
		switch (adj[i])
		{
			/* (0,+) - check for bottom boundary */
		case 0:
			{
				if ((node / m < n - 1) && (visited[node + m] == 0))
				{
					place_floor(y + 1, x);
					r_visit(y1, x1, y2, x2, node + m, dir, visited);
				}
				break;
			}

			/* (0,-) - check for top boundary */
		case 1:
			{
				if ((node / m > 0) && (visited[node - m] == 0))
				{
					place_floor(y - 1, x);
					r_visit(y1, x1, y2, x2, node - m, dir, visited);
				}
				break;
			}

			/* (+,0) - check for right boundary */
		case 2:
			{
				if ((node % m < m - 1) && (visited[node + 1] == 0))
				{
					place_floor(y, x + 1);
					r_visit(y1, x1, y2, x2, node + 1, dir, visited);
				}
				break;
			}

			/* (-,0) - check for left boundary */
		case 3:
			{
				if ((node % m > 0) && (visited[node - 1] == 0))
				{
					place_floor(y, x - 1);
					r_visit(y1, x1, y2, x2, node - 1, dir, visited);
				}
				break;
			}
		}
	}
}


static void build_maze_vault(s32b x0, s32b y0, s32b xsize, s32b ysize)
{
	s32b y, x, dy, dx;
	s32b y1, x1, y2, x2;
	s32b i, m, n, num_vertices, *visited;
	bool light;
	cave_type *c_ptr;


	if (cheat_room) msg_print("Maze Vault");

	/* Choose lite or dark */
	light = (dun_level <= randint(25));

	/* Pick a random room size - randomized by calling routine */
	dy = ysize / 2 - 1;
	dx = xsize / 2 - 1;

	y1 = y0 - dy;
	x1 = x0 - dx;
	y2 = y0 + dy;
	x2 = x0 + dx;

	/* Generate the room */
	for (y = y1 - 1; y <= y2 + 1; y++)
	{
		for (x = x1 - 1; x <= x2 + 1; x++)
		{
			c_ptr = &cave[y][x];

			c_ptr->info |= (CAVE_ROOM | CAVE_ICKY);

			if ((x == x1 - 1) || (x == x2 + 1) ||
			                (y == y1 - 1) || (y == y2 + 1))
			{
				cave_set_feat(y, x, feat_wall_outer);
			}
			else
			{
				cave_set_feat(y, x, feat_wall_inner);
			}
			if (light) c_ptr->info |= (CAVE_GLOW);
		}
	}

	/* Dimensions of vertex array */
	m = dx + 1;
	n = dy + 1;
	num_vertices = m * n;

	/* Allocate an array for visited vertices */
	C_MAKE(visited, num_vertices, s32b);

	/* Initialise array of visited vertices */
	for (i = 0; i < num_vertices; i++)
	{
		visited[i] = 0;
	}

	/* Traverse the graph to create a spaning tree, pick a random root */
	r_visit(y1, x1, y2, x2, rand_int(num_vertices), 0, visited);

	/* Fill with monsters and treasure, low difficulty */
	fill_treasure(x1, x2, y1, y2, randint(5));

	/* Free the array for visited vertices */
	C_FREE(visited, num_vertices, s32b);
}


/*
 * Build a "mini" checkerboard vault
 *
 * This is done by making a permanent wall maze and setting
 * the diagonal sqaures of the checker board to be granite.
 * The vault has two entrances on opposite sides to guarantee
 * a way to get in even if the vault abuts a side of the dungeon.
 */
static void build_mini_c_vault(s32b x0, s32b y0, s32b xsize, s32b ysize)
{
	s32b dy, dx;
	s32b y1, x1, y2, x2, y, x, total;
	s32b i, m, n, num_vertices;
	s32b *visited;

	if (cheat_room) msg_print("Mini Checker Board Vault");

	/* Pick a random room size */
	dy = ysize / 2 - 1;
	dx = xsize / 2 - 1;

	y1 = y0 - dy;
	x1 = x0 - dx;
	y2 = y0 + dy;
	x2 = x0 + dx;


	/* Generate the room */
	for (y = y1 - 1; y <= y2 + 1; y++)
	{
		for (x = x1 - 1; x <= x2 + 1; x++)
		{
			cave[y][x].info |= (CAVE_ROOM | CAVE_ICKY);

			/* Permanent walls */
			cave_set_feat(y, x, FEAT_PERM_INNER);
		}
	}


	/* Dimensions of vertex array */
	m = dx + 1;
	n = dy + 1;
	num_vertices = m * n;

	/* Allocate an array for visited vertices */
	C_MAKE(visited, num_vertices, s32b);

	/* Initialise array of visited vertices */
	for (i = 0; i < num_vertices; i++)
	{
		visited[i] = 0;
	}

	/* Traverse the graph to create a spannng tree, pick a random root */
	r_visit(y1, x1, y2, x2, rand_int(num_vertices), 0, visited);

	/* Make it look like a checker board vault */
	for (x = x1; x <= x2; x++)
	{
		for (y = y1; y <= y2; y++)
		{
			total = x - x1 + y - y1;

			/* If total is odd and is a floor, then make a wall */
			if ((total % 2 == 1) && get_is_floor(x, y))
			{
				cave_set_feat(y, x, feat_wall_inner);
			}
		}
	}

	/* Make a couple of entrances */
	if (rand_int(2) == 0)
	{
		/* Left and right */
		y = randint(dy) + dy / 2;
		cave_set_feat(y1 + y, x1 - 1, feat_wall_outer);
		cave_set_feat(y1 + y, x2 + 1, feat_wall_outer);
	}
	else
	{
		/* Top and bottom */
		x = randint(dx) + dx / 2;
		cave_set_feat(y1 - 1, x1 + x, feat_wall_outer);
		cave_set_feat(y2 + 1, x1 + x, feat_wall_outer);
	}

	/* Fill with monsters and treasure, highest difficulty */
	fill_treasure(x1, x2, y1, y2, 10);

	/* Free the array for visited vertices */
	C_FREE(visited, num_vertices, s32b);
}


/*
 * Build a town/ castle by using a recursive algorithm.
 * Basically divide each region in a probalistic way to create
 * smaller regions.  When the regions get too small stop.
 *
 * The power variable is a measure of how well defended a region is.
 * This alters the possible choices.
 */
static void build_recursive_room(s32b x1, s32b y1, s32b x2, s32b y2, s32b power)
{
	s32b xsize, ysize;
	s32b x, y;
	s32b choice;

	/* Temp variables */
	s32b t1, t2, t3, t4;

	xsize = x2 - x1;
	ysize = y2 - y1;

	if ((power < 3) && (xsize > 12) && (ysize > 12))
	{
		/* Need outside wall +keep */
		choice = 1;
	}
	else
	{
		if (power < 10)
		{
			/* Make rooms + subdivide */
			if ((randint(10) > 2) && (xsize < 8) && (ysize < 8))
			{
				choice = 4;
			}
			else
			{
				choice = randint(2) + 1;
			}
		}
		else
		{
			/* Mostly subdivide */
			choice = randint(3) + 1;
		}
	}

	/* Based on the choice made above, do something */
	switch (choice)
	{
		/* Outer walls */
	case 1:
		{
			/* Top and bottom */
			for (x = x1; x <= x2; x++)
			{
				cave_set_feat(y1, x, feat_wall_outer);
				cave_set_feat(y2, x, feat_wall_outer);
			}

			/* Left and right */
			for (y = y1 + 1; y < y2; y++)
			{
				cave_set_feat(y, x1, feat_wall_outer);
				cave_set_feat(y, x2, feat_wall_outer);
			}

			/* Make a couple of entrances */
			if (rand_int(2) == 0)
			{
				/* Left and right */
				y = randint(ysize) + y1;
				place_floor(y, x1);
				place_floor(y, x2);
			}
			else
			{
				/* Top and bottom */
				x = randint(xsize) + x1;
				place_floor(y1, x);
				place_floor(y2, x);
			}

			/* Select size of keep */
			t1 = randint(ysize / 3) + y1;
			t2 = y2 - randint(ysize / 3);
			t3 = randint(xsize / 3) + x1;
			t4 = x2 - randint(xsize / 3);

			/* Do outside areas */

			/* Above and below keep */
			build_recursive_room(x1 + 1, y1 + 1, x2 - 1, t1, power + 1);
			build_recursive_room(x1 + 1, t2, x2 - 1, y2, power + 1);

			/* Left and right of keep */
			build_recursive_room(x1 + 1, t1 + 1, t3, t2 - 1, power + 3);
			build_recursive_room(t4, t1 + 1, x2 - 1, t2 - 1, power + 3);

			/* Make the keep itself: */
			x1 = t3;
			x2 = t4;
			y1 = t1;
			y2 = t2;
			xsize = x2 - x1;
			ysize = y2 - y1;
			power += 2;

			/* Fall through */
		}

		/* Try to build a room */
	case 4:
		{
			if ((xsize < 3) || (ysize < 3))
			{
				for (y = y1; y < y2; y++)
				{
					for (x = x1; x < x2; x++)
					{
						cave_set_feat(y, x, feat_wall_inner);
					}
				}

				/* Too small */
				return;
			}

			/* Make outside walls */

			/* Top and bottom */
			for (x = x1 + 1; x <= x2 - 1; x++)
			{
				cave_set_feat(y1 + 1, x, feat_wall_inner);
				cave_set_feat(y2 - 1, x, feat_wall_inner);
			}

			/* Left and right */
			for (y = y1 + 1; y <= y2 - 1; y++)
			{
				cave_set_feat(y, x1 + 1, feat_wall_inner);
				cave_set_feat(y, x2 - 1, feat_wall_inner);
			}

			/* Make a door */
			y = randint(ysize - 3) + y1 + 1;

			if (rand_int(2) == 0)
			{
				/* Left */
				place_floor(y, x1 + 1);
			}
			else
			{
				/* Right */
				place_floor(y, x2 - 1);
			}

			/* Build the room */
			build_recursive_room(x1 + 2, y1 + 2, x2 - 2, y2 - 2, power + 3);

			break;
		}

		/* Try and divide vertically */
	case 2:
		{
			if (xsize < 3)
			{
				/* Too small */
				for (y = y1; y < y2; y++)
				{
					for (x = x1; x < x2; x++)
					{
						cave_set_feat(y, x, feat_wall_inner);
					}
				}
				return;
			}

			t1 = randint(xsize - 2) + x1 + 1;
			build_recursive_room(x1, y1, t1, y2, power - 2);
			build_recursive_room(t1 + 1, y1, x2, y2, power - 2);

			break;
		}

		/* Try and divide horizontally */
	case 3:
		{
			if (ysize < 3)
			{
				/* Too small */
				for (y = y1; y < y2; y++)
				{
					for (x = x1; x < x2; x++)
					{
						cave_set_feat(y, x, feat_wall_inner);
					}
				}
				return;
			}

			t1 = randint(ysize - 2) + y1 + 1;
			build_recursive_room(x1, y1, x2, t1, power - 2);
			build_recursive_room(x1, t1 + 1, x2, y2, power - 2);

			break;
		}
	}
}


/*
 * Build a castle
 *
 * Clear the region and call the recursive room routine.
 *
 * This makes a vault that looks like a castle or city in the dungeon.
 */
static void build_castle_vault(s32b x0, s32b y0, s32b xsize, s32b ysize)
{
	s32b dy, dx;
	s32b y1, x1, y2, x2;
	s32b y, x;

	/* Pick a random room size */
	dy = ysize / 2 - 1;
	dx = xsize / 2 - 1;

	y1 = y0 - dy;
	x1 = x0 - dx;
	y2 = y0 + dy;
	x2 = x0 + dx;

	if (cheat_room) msg_print("Castle Vault");

	/* Generate the room */
	for (y = y1 - 1; y <= y2 + 1; y++)
	{
		for (x = x1 - 1; x <= x2 + 1; x++)
		{
			cave[y][x].info |= (CAVE_ROOM | CAVE_ICKY);

			/* Make everything a floor */
			place_floor(y, x);
		}
	}

	/* Make the castle */
	build_recursive_room(x1, y1, x2, y2, randint(5));

	/* Fill with monsters and treasure, low difficulty */
	fill_treasure(x1, x2, y1, y2, randint(3));
}


/*
 * Add outer wall to a floored region
 *
 * Note: no range checking is done so must be inside dungeon
 * This routine also stomps on doors
 */
static void add_outer_wall(s32b x, s32b y, s32b light, s32b x1, s32b y1,
                           s32b x2, s32b y2)
{
	s32b i, j;

	if (!in_bounds(y, x)) return;

	/*
	 * Hack -- Check to see if square has been visited before
	 * if so, then exit (use room flag to do this)
	 */
	if (cave[y][x].info & CAVE_ROOM) return;

	/* Set room flag */
	cave[y][x].info |= (CAVE_ROOM);

	if (get_is_floor(x, y))
	{
		for (i = -1; i <= 1; i++)
		{
			for (j = -1; j <= 1; j++)
			{
				if ((x + i >= x1) && (x + i <= x2) &&
				                (y + j >= y1) && (y + j <= y2))
				{
					add_outer_wall(x + i, y + j, light, x1, y1, x2, y2);
					if (light) cave[y][x].info |= CAVE_GLOW;
				}
			}
		}
	}

	/* Set bounding walls */
	else if (cave[y][x].feat == FEAT_WALL_EXTRA)
	{
		cave[y][x].feat = feat_wall_outer;
		if (light == TRUE) cave[y][x].info |= CAVE_GLOW;
	}

	/* Set bounding walls */
	else if (cave[y][x].feat == FEAT_PERM_OUTER)
	{
		if (light == TRUE) cave[y][x].info |= CAVE_GLOW;
	}
}


/*
 * Hacked distance formula - gives the 'wrong' answer
 *
 * Used to build crypts
 */
static s32b dist2(s32b x1, s32b y1, s32b x2, s32b y2,
                 s32b h1, s32b h2, s32b h3, s32b h4)
{
	s32b dx, dy;
	dx = abs(x2 - x1);
	dy = abs(y2 - y1);

	/*
	 * Basically this works by taking the normal pythagorean formula
	 * and using an expansion to express this in a way without the
	 * square root.  This approximate formula is then perturbed to give
	 * the distorted results.  (I found this by making a mistake when I was
	 * trying to fix the circular rooms.)
	 */

	/* h1-h4 are constants that describe the metric */
	if (dx >= 2 * dy) return (dx + (dy * h1) / h2);
	if (dy >= 2 * dx) return (dy + (dx * h1) / h2);

	/* 128/181 is approx. 1/sqrt(2) */
	return (((dx + dy) * 128) / 181 +
	        (dx * dx / (dy * h3) + dy * dy / (dx * h3)) * h4);
}


/*
 * Build target vault
 *
 * This is made by two concentric "crypts" with perpendicular
 * walls creating the cross-hairs.
 */
static void build_target_vault(s32b x0, s32b y0, s32b xsize, s32b ysize)
{
	s32b rad, x, y;

	s32b h1, h2, h3, h4;


	/* Make a random metric */
	h1 = randint(32) - 16;
	h2 = randint(16);
	h3 = randint(32);
	h4 = randint(32) - 16;

	if (cheat_room) msg_print("Target Vault");

	/* Work out outer radius */
	if (xsize > ysize)
	{
		rad = ysize / 2;
	}
	else
	{
		rad = xsize / 2;
	}

	/* Make floor */
	for (x = x0 - rad; x <= x0 + rad; x++)
	{
		for (y = y0 - rad; y <= y0 + rad; y++)
		{
			cave_type *c_ptr;

			/* Access the grid */
			c_ptr = &cave[y][x];

			/* Clear room flag */
			c_ptr->info &= ~(CAVE_ROOM);

			/* Grids in vaults are required to be "icky" */
			c_ptr->info |= (CAVE_ICKY);

			/* Inside -- floor */
			if (dist2(y0, x0, y, x, h1, h2, h3, h4) <= rad - 1)
			{
				place_floor(y, x);
			}

			/* Outside -- make it granite so that arena works */
			else
			{
				c_ptr->feat = FEAT_WALL_EXTRA;
			}

			/* Proper boundary for arena */
			if (((y + rad) == y0) || ((y - rad) == y0) ||
			                ((x + rad) == x0) || ((x - rad) == x0))
			{
				cave_set_feat(y, x, feat_wall_outer);
			}
		}
	}

	/* Find visible outer walls and set to be FEAT_OUTER */
	add_outer_wall(x0, y0, FALSE, x0 - rad - 1, y0 - rad - 1,
	               x0 + rad + 1, y0 + rad + 1);

	/* Add inner wall */
	for (x = x0 - rad / 2; x <= x0 + rad / 2; x++)
	{
		for (y = y0 - rad / 2; y <= y0 + rad / 2; y++)
		{
			if (dist2(y0, x0, y, x, h1, h2, h3, h4) == rad / 2)
			{
				/* Make an internal wall */
				cave_set_feat(y, x, feat_wall_inner);
			}
		}
	}

	/* Add perpendicular walls */
	for (x = x0 - rad; x <= x0 + rad; x++)
	{
		cave_set_feat(y0, x, feat_wall_inner);
	}

	for (y = y0 - rad; y <= y0 + rad; y++)
	{
		cave_set_feat(y, x0, feat_wall_inner);
	}

	/* Make inner vault */
	for (y = y0 - 1; y <= y0 + 1; y++)
	{
		cave_set_feat(y, x0 - 1, feat_wall_inner);
		cave_set_feat(y, x0 + 1, feat_wall_inner);
	}
	for (x = x0 - 1; x <= x0 + 1; x++)
	{
		cave_set_feat(y0 - 1, x, feat_wall_inner);
		cave_set_feat(y0 + 1, x, feat_wall_inner);
	}

	place_floor(y0, x0);


	/*
	 * Add doors to vault
	 *
	 * Get two distances so can place doors relative to centre
	 */
	x = (rad - 2) / 4 + 1;
	y = rad / 2 + x;

	add_door(x0 + x, y0);
	add_door(x0 + y, y0);
	add_door(x0 - x, y0);
	add_door(x0 - y, y0);
	add_door(x0, y0 + x);
	add_door(x0, y0 + y);
	add_door(x0, y0 - x);
	add_door(x0, y0 - y);

	/* Fill with stuff - medium difficulty */
	fill_treasure(x0 - rad, x0 + rad, y0 - rad, y0 + rad, randint(3) + 3);
}


/*
 * Random vaults
 */
static void build_type11(s32b by0, s32b bx0)
{
	s32b y0, x0, xsize, ysize, vtype;

	/* Get size -- gig enough to look good, small enough to be fairly common */
	xsize = randint(22) + 22;
	ysize = randint(11) + 11;

	/* Allocate in room_map.  If will not fit, exit */
	if (!room_alloc(xsize + 1, ysize + 1, FALSE, by0, bx0, &x0, &y0)) return;

	/*
	 * Boost the rating -- Higher than lesser vaults and lower than
	 * greater vaults
	 */
	rating += 10;

	/* (Sometimes) Cause a special feeling */
	if ((dun_level <= 50) ||
	                (randint((dun_level - 40) * (dun_level - 40) + 1) < 400))
	{
		good_item_flag = TRUE;
	}

	/* Select type of vault */
	vtype = randint(8);

	switch (vtype)
	{
		/* Build an appropriate room */
	case 1:
		{
			build_bubble_vault(x0, y0, xsize, ysize);
			break;
		}

	case 2:
		{
			build_room_vault(x0, y0, xsize, ysize);
			break;
		}

	case 3:
		{
			build_cave_vault(x0, y0, xsize, ysize);
			break;
		}

	case 4:
		{
			build_maze_vault(x0, y0, xsize, ysize);
			break;
		}

	case 5:
		{
			build_mini_c_vault(x0, y0, xsize, ysize);
			break;
		}

	case 6:
		{
			build_castle_vault(x0, y0, xsize, ysize);
			break;
		}

	case 7:
		{
			build_target_vault(x0, y0, xsize, ysize);
			break;
		}

		/* I know how to add a few more... give me some time. */

		/* Paranoia */
	default:
		{
			return;
		}
	}
}


/*
 * Crypt room generation from Z 2.5.1
 */

/*
 * Build crypt room.
 * For every grid in the possible square, check the (fake) distance.
 * If it's less than the radius, make it a room square.
 *
 * When done fill from the inside to find the walls,
 */
static void build_type12(s32b by0, s32b bx0)
{
	s32b rad, x, y, x0, y0;
	s32b light = FALSE;
	bool emptyflag = TRUE;
	s32b h1, h2, h3, h4;

	/* Make a random metric */
	h1 = randint(32) - 16;
	h2 = randint(16);
	h3 = randint(32);
	h4 = randint(32) - 16;

	/* Occasional light */
	if (randint(dun_level) <= 5) light = TRUE;

	rad = randint(9);

	/* Allocate in room_map.  If will not fit, exit */
	if (!room_alloc(rad * 2 + 3, rad * 2 + 3, FALSE, by0, bx0, &x0, &y0)) return;

	/* Make floor */
	for (x = x0 - rad; x <= x0 + rad; x++)
	{
		for (y = y0 - rad; y <= y0 + rad; y++)
		{
			/* Clear room flag */
			cave[y][x].info &= ~(CAVE_ROOM);

			/* Inside -- floor */
			if (dist2(y0, x0, y, x, h1, h2, h3, h4) <= rad - 1)
			{
				place_floor(y, x);
			}
			else if (distance(y0, x0, y, x) < 3)
			{
				place_floor(y, x);
			}

			/* Outside -- make it granite so that arena works */
			else
			{
				cave_set_feat(y, x, feat_wall_outer);
			}

			/* Proper boundary for arena */
			if (((y + rad) == y0) || ((y - rad) == y0) ||
			                ((x + rad) == x0) || ((x - rad) == x0))
			{
				cave_set_feat(y, x, feat_wall_outer);
			}
		}
	}

	/* Find visible outer walls and set to be FEAT_OUTER */
	add_outer_wall(x0, y0, light, x0 - rad - 1, y0 - rad - 1,
	               x0 + rad + 1, y0 + rad + 1);

	/* Check to see if there is room for an inner vault */
	for (x = x0 - 2; x <= x0 + 2; x++)
	{
		for (y = y0 - 2; y <= y0 + 2; y++)
		{
			if (!get_is_floor(x, y))
			{
				/* Wall in the way */
				emptyflag = FALSE;
			}
		}
	}

	if (emptyflag && (rand_int(2) == 0))
	{
		/* Build the vault */
		build_small_room(x0, y0);

		/* Place a treasure in the vault */
		place_object(y0, x0, FALSE, FALSE, OBJ_FOUND_FLOOR);

		/* Let's guard the treasure well */
		vault_monsters(y0, x0, rand_int(2) + 3);

		/* Traps naturally */
		vault_traps(y0, x0, 4, 4, rand_int(3) + 2);
	}
}


/*
 * Constructs a tunnel between two points
 *
 * This function must be called BEFORE any streamers are created,
 * since we use the special "granite wall" sub-types to keep track
 * of legal places for corridors to pierce rooms.
 *
 * We use "door_flag" to prevent excessive construction of doors
 * along overlapping corridors.
 *
 * We queue the tunnel grids to prevent door creation along a corridor
 * which intersects itself.
 *
 * We queue the wall piercing grids to prevent a corridor from leaving
 * a room and then coming back in through the same entrance.
 *
 * We "pierce" grids which are "outer" walls of rooms, and when we
 * do so, we change all adjacent "outer" walls of rooms into "solid"
 * walls so that no two corridors may use adjacent grids for exits.
 *
 * The "solid" wall check prevents corridors from "chopping" the
 * corners of rooms off, as well as "silly" door placement, and
 * "excessively wide" room entrances.
 *
 * Useful "feat" values:
 *   FEAT_WALL_EXTRA -- granite walls
 *   FEAT_WALL_INNER -- inner room walls
 *   FEAT_WALL_OUTER -- outer room walls
 *   FEAT_WALL_SOLID -- solid room walls
 *   FEAT_PERM_EXTRA -- shop walls (perma)
 *   FEAT_PERM_INNER -- inner room walls (perma)
 *   FEAT_PERM_OUTER -- outer room walls (perma)
 *   FEAT_PERM_SOLID -- dungeon border (perma)
 */
static void build_tunnel(s32b row1, s32b col1, s32b row2, s32b col2)
{
	s32b i, y, x;
	s32b tmp_row, tmp_col;
	s32b row_dir, col_dir;
	s32b start_row, start_col;
	s32b main_loop_count = 0;

	bool door_flag = FALSE;

	cave_type *c_ptr;


	/* Reset the arrays */
	dun->tunn_n = 0;
	dun->wall_n = 0;

	/* Save the starting location */
	start_row = row1;
	start_col = col1;

	/* Start out in the correct direction */
	correct_dir(&row_dir, &col_dir, row1, col1, row2, col2);

	/* Keep going until done (or bored) */
	while ((row1 != row2) || (col1 != col2))
	{
		/* Mega-Hack -- Paranoia -- prevent infinite loops */
		if (main_loop_count++ > 2000) break;

		/* Allow bends in the tunnel */
		if (rand_int(100) < DUN_TUN_CHG)
		{
			/* Acquire the correct direction */
			correct_dir(&row_dir, &col_dir, row1, col1, row2, col2);

			/* Random direction */
			if (rand_int(100) < DUN_TUN_RND)
			{
				rand_dir(&row_dir, &col_dir);
			}
		}

		/* Get the next location */
		tmp_row = row1 + row_dir;
		tmp_col = col1 + col_dir;


		/* Extremely Important -- do not leave the dungeon */
		while (!in_bounds(tmp_row, tmp_col))
		{
			/* Acquire the correct direction */
			correct_dir(&row_dir, &col_dir, row1, col1, row2, col2);

			/* Random direction */
			if (rand_int(100) < DUN_TUN_RND)
			{
				rand_dir(&row_dir, &col_dir);
			}

			/* Get the next location */
			tmp_row = row1 + row_dir;
			tmp_col = col1 + col_dir;
		}


		/* Access the location */
		c_ptr = &cave[tmp_row][tmp_col];


		/* Avoid the edge of the dungeon */
		if (c_ptr->feat == FEAT_PERM_SOLID) continue;

		/* Avoid the edge of vaults */
		if (c_ptr->feat == FEAT_PERM_OUTER) continue;

		/* Avoid "solid" granite walls */
		if (c_ptr->feat == FEAT_WALL_SOLID) continue;

		/*
		 * Pierce "outer" walls of rooms
		 * Cannot trust feat code any longer...
		 */
		if ((c_ptr->feat == feat_wall_outer) &&
		                (c_ptr->info & CAVE_ROOM))
		{
			/* Acquire the "next" location */
			y = tmp_row + row_dir;
			x = tmp_col + col_dir;

			/* Hack -- Avoid outer/solid permanent walls */
			if (cave[y][x].feat == FEAT_PERM_SOLID) continue;
			if (cave[y][x].feat == FEAT_PERM_OUTER) continue;

			/* Hack -- Avoid outer/solid granite walls */
			if ((cave[y][x].feat == feat_wall_outer) &&
			                (cave[y][x].info & CAVE_ROOM)) continue;
			if (cave[y][x].feat == FEAT_WALL_SOLID) continue;

			/* Accept this location */
			row1 = tmp_row;
			col1 = tmp_col;

			/* Save the wall location */
			if (dun->wall_n < WALL_MAX)
			{
				dun->wall[dun->wall_n].y = row1;
				dun->wall[dun->wall_n].x = col1;
				dun->wall_n++;
			}

			/* Forbid re-entry near this piercing */
			for (y = row1 - 1; y <= row1 + 1; y++)
			{
				for (x = col1 - 1; x <= col1 + 1; x++)
				{
					/* Convert adjacent "outer" walls as "solid" walls */
					if ((cave[y][x].feat == feat_wall_outer) &&
					                (cave[y][x].info & CAVE_ROOM))
					{
						/* Change the wall to a "solid" wall */
						/* Mega-Hack -- to be brought back later... */
						cave_set_feat(y, x, FEAT_WALL_SOLID);
					}
				}
			}
		}

		/* Travel quickly through rooms */
		else if (c_ptr->info & (CAVE_ROOM))
		{
			/* Accept the location */
			row1 = tmp_row;
			col1 = tmp_col;
		}

		/* Tunnel through all other walls */
		else if ((c_ptr->feat == d_info[dungeon_type].fill_type1) ||
		                (c_ptr->feat == d_info[dungeon_type].fill_type2) ||
		                (c_ptr->feat == d_info[dungeon_type].fill_type3))
		{
			/* Accept this location */
			row1 = tmp_row;
			col1 = tmp_col;

			/* Save the tunnel location */
			if (dun->tunn_n < TUNN_MAX)
			{
				dun->tunn[dun->tunn_n].y = row1;
				dun->tunn[dun->tunn_n].x = col1;
				dun->tunn_n++;
			}

			/* Allow door in next grid */
			door_flag = FALSE;
		}

		/* Handle corridor intersections or overlaps */
		else
		{
			/* Accept the location */
			row1 = tmp_row;
			col1 = tmp_col;

			/* Collect legal door locations */
			if (!door_flag)
			{
				/* Save the door location */
				if (dun->door_n < DOOR_MAX)
				{
					dun->door[dun->door_n].y = row1;
					dun->door[dun->door_n].x = col1;
					dun->door_n++;
				}

				/* No door in next grid */
				door_flag = TRUE;
			}

			/* Hack -- allow pre-emptive tunnel termination */
			if (rand_int(100) >= DUN_TUN_CON)
			{
				/* Distance between row1 and start_row */
				tmp_row = row1 - start_row;
				if (tmp_row < 0) tmp_row = ( -tmp_row);

				/* Distance between col1 and start_col */
				tmp_col = col1 - start_col;
				if (tmp_col < 0) tmp_col = ( -tmp_col);

				/* Terminate the tunnel */
				if ((tmp_row > 10) || (tmp_col > 10)) break;
			}
		}
	}


	/* Turn the tunnel into corridor */
	for (i = 0; i < dun->tunn_n; i++)
	{
		/* Access the grid */
		y = dun->tunn[i].y;
		x = dun->tunn[i].x;

		/* Access the grid */
		c_ptr = &cave[y][x];

		/* Clear previous contents, add a floor */
		place_floor(y, x);
	}


	/* Apply the piercings that we found */
	for (i = 0; i < dun->wall_n; i++)
	{
		/* Access the grid */
		y = dun->wall[i].y;
		x = dun->wall[i].x;

		/* Access the grid */
		c_ptr = &cave[y][x];

		/* Clear previous contents, add up floor */
		place_floor(y, x);

		/* Occasional doorway */
		if (!((has_flag(dungeon_flags, FLAG_NO_DOORS))) &&
		                (rand_int(100) < DUN_TUN_PEN))
		{
			/* Place a random door */
			place_random_door(y, x);
		}
	}
}




/*
 * Count the number of "corridor" grids adjacent to the given grid.
 *
 * Note -- Assumes "in_bounds(y1, x1)"
 *
 * XXX XXX This routine currently only counts actual "empty floor"
 * grids which are not in rooms.  We might want to also count stairs,
 * open doors, closed doors, etc.
 */
static s32b next_to_corr(s32b y1, s32b x1)
{
	s32b i, y, x, k = 0;

	cave_type *c_ptr;

	/* Scan adjacent grids */
	for (i = 0; i < 4; i++)
	{
		/* Extract the location */
		y = y1 + ddy_ddd[i];
		x = x1 + ddx_ddd[i];

		/* Skip non floors */
		if (!cave_floor_bold(y, x)) continue;

		/* Access the grid */
		c_ptr = &cave[y][x];

		/* Skip non "empty floor" grids */
		if ((c_ptr->feat != d_info[dungeon_type].floor1) &&
		                (c_ptr->feat != d_info[dungeon_type].floor2) &&
		                (c_ptr->feat != d_info[dungeon_type].floor3))
		{
			continue;
		}

		/* Skip grids inside rooms */
		if (c_ptr->info & (CAVE_ROOM)) continue;

		/* Count these grids */
		k++;
	}

	/* Return the number of corridors */
	return (k);
}


/*
 * Determine if the given location is "between" two walls,
 * and "next to" two corridor spaces.  XXX XXX XXX
 *
 * Assumes "in_bounds(y,x)"
 */
static bool possible_doorway(s32b y, s32b x)
{
	/* Count the adjacent corridors */
	if (next_to_corr(y, x) >= 2)
	{
		/* Check Vertical */
		if (has_flag(&f_info[cave[y - 1][x].feat], FLAG_WALL) &&
		                has_flag(&f_info[cave[y + 1][x].feat], FLAG_WALL))
		{
			return (TRUE);
		}

		/* Check Horizontal */
		if (has_flag(&f_info[cave[y][x - 1].feat], FLAG_WALL) &&
		                has_flag(&f_info[cave[y][x + 1].feat], FLAG_WALL))
		{
			return (TRUE);
		}
	}

	/* No doorway */
	return (FALSE);
}


#if 0

/*
 * Places door at y, x position if at least 2 walls found
 */
static void try_door(s32b y, s32b x)
{
	/* Paranoia */
	if (!in_bounds(y, x)) return;

	/* Some dungeons don't have doors at all */
	if (has_flag(dungeon_flags, FLAG_NO_DOORS)) return;

	/* Ignore walls */
	if (has_flag(&f_info[cave[y][x].feat], FLAG_WALL)) return;

	/* Ignore room grids */
	if (cave[y][x].info & (CAVE_ROOM)) return;

	/* Occasional door (if allowed) */
	if (possible_doorway(y, x) && (rand_int(100) < DUN_TUN_JCT))
	{
		/* Place a door */
		place_random_door(y, x);
	}
}

#endif /* 0 */


/*
 * Places doors around y, x position
 */
static void try_doors(s32b y, s32b x)
{
	bool dir_ok[4];
	s32b i, k, n;
	s32b yy, xx;

	/* Paranoia */
	/* if (!in_bounds(y, x)) return; */

	/* Some dungeons don't have doors at all */
	if (has_flag(dungeon_flags, FLAG_NO_DOORS)) return;

	/* Reset tally */
	n = 0;

	/* Look four cardinal directions */
	for (i = 0; i < 4; i++)
	{
		/* Assume NG */
		dir_ok[i] = FALSE;

		/* Access location */
		yy = y + ddy_ddd[i];
		xx = x + ddx_ddd[i];

		/* Out of level boundary */
		if (!in_bounds(yy, xx)) continue;

		/* Ignore walls */
		if (has_flag(&f_info[cave[yy][xx].feat], FLAG_WALL)) continue;

		/* Ignore room grids */
		if (cave[yy][xx].info & (CAVE_ROOM)) continue;

		/* Not a doorway */
		if (!possible_doorway(yy, xx)) continue;

		/* Accept the direction */
		dir_ok[i] = TRUE;

		/* Count good spots */
		n++;
	}

	/* Use the traditional method 75% of time */
	if (rand_int(100) < 75)
	{
		for (i = 0; i < 4; i++)
		{
			/* Bad locations */
			if (!dir_ok[i]) continue;

			/* Place one of various kinds of doors */
			if (rand_int(100) < DUN_TUN_JCT)
			{
				/* Access location */
				yy = y + ddy_ddd[i];
				xx = x + ddx_ddd[i];

				/* Place a door */
				place_random_door(yy, xx);
			}
		}
	}

	/* Use alternative method */
	else
	{
		/* A crossroad */
		if (n == 4)
		{
			/* Clear OK flags XXX */
			for (i = 0; i < 4; i++) dir_ok[i] = FALSE;

			/* Put one or two secret doors */
			dir_ok[rand_int(4)] = TRUE;
			dir_ok[rand_int(4)] = TRUE;
		}

		/* A T-shaped intersection or two possible doorways */
		else if ((n == 3) || (n == 2))
		{
			/* Pick one random location from the list */
			k = rand_int(n);

			for (i = 0; i < 4; i++)
			{
				/* Reject all but k'th OK direction */
				if (dir_ok[i] && (k-- != 0)) dir_ok[i] = FALSE;
			}
		}

		/* Place secret door(s) */
		for (i = 0; i < 4; i++)
		{
			/* Bad location */
			if (!dir_ok[i]) continue;

			/* Access location */
			yy = y + ddy_ddd[i];
			xx = x + ddx_ddd[i];

			/* Place a secret door */
			place_secret_door(yy, xx);
		}
	}
}


/*
 * Attempt to build a room of the given type at the given block
 *
 * Note that we restrict the number of "crowded" rooms to reduce
 * the chance of overflowing the monster list during level creation.
 */
bool room_build(s32b y, s32b x, s32b typ)
{
	/* Restrict level */
	if ((dun_level < roomdep[typ]) && !ironman_rooms) return (FALSE);

	/* Restrict "crowded" rooms */
	if (dun->crowded && ((typ == 5) || (typ == 6))) return (FALSE);

	/* Build a room */
	switch (typ)
	{
		/* Build an appropriate room */
	case 12:
		build_type12(y, x);
		break;
	case 11:
		build_type11(y, x);
		break;
	case 10:
		build_type10(y, x);
		break;
	case 9:
		build_type9 (y, x);
		break;
	case 8:
		build_type8 (y, x);
		break;
	case 7:
		build_type7 (y, x);
		break;
	case 6:
		build_type6 (y, x);
		break;
	case 5:
		build_type5 (y, x);
		break;
	case 4:
		build_type4 (y, x);
		break;
	case 3:
		build_type3 (y, x);
		break;
	case 2:
		build_type2 (y, x);
		break;
	case 1:
		build_type1 (y, x);
		break;

		/* Paranoia */
	default:
		return (FALSE);
	}

	/* Success */
	return (TRUE);
}

/*
 * Set level boundaries
 */
void set_bounders(bool empty_level)
{
	s32b y, x;

	/* Special boundary walls -- Top */
	for (x = 0; x < cur_wid; x++)
	{
		/* XXX XXX */
		if (empty_level) cave[0][x].mimic = fill_type[rand_int(100)];
		else cave[0][x].mimic = cave[0][x].feat;

		/* Clear previous contents, add "solid" perma-wall */
		cave_set_feat(0, x, FEAT_PERM_SOLID);
	}

	/* Special boundary walls -- Bottom */
	for (x = 0; x < cur_wid; x++)
	{
		/* XXX XXX */
		if (empty_level) cave[cur_hgt - 1][x].mimic = fill_type[rand_int(100)];
		else cave[cur_hgt - 1][x].mimic = cave[cur_hgt - 1][x].feat;

		/* Clear previous contents, add "solid" perma-wall */
		cave_set_feat(cur_hgt - 1, x, FEAT_PERM_SOLID);
	}

	/* Special boundary walls --  Left */
	for (y = 1; y < cur_hgt - 1; y++)
	{
		/* XXX XXX */
		if (empty_level) cave[y][0].mimic = fill_type[rand_int(100)];
		else cave[y][0].mimic = cave[y][0].feat;

		/* Clear previous contents, add "solid" perma-wall */
		cave_set_feat(y, 0, FEAT_PERM_SOLID);
	}

	/* Special boundary walls --  Right */
	for (y = 1; y < cur_hgt - 1; y++)
	{
		/* XXX XXX */
		if (empty_level) cave[y][cur_wid - 1].mimic = fill_type[rand_int(100)];
		else cave[y][cur_wid - 1].mimic = cave[y][cur_wid - 1].feat;

		/* Clear previous contents, add "solid" perma-wall */
		cave_set_feat(y, cur_wid - 1, FEAT_PERM_SOLID);
	}
}

/*
 * Generate a normal dungeon level
 */
bool level_generate_dungeon(cptr name)
{
	s32b i, k, y, x, y1, x1, branch = get_branch();
	dungeon_info_type *d_ptr = &d_info[dungeon_type];

	s32b max_vault_ok = 2;

	bool empty_level = FALSE;
	bool cavern = FALSE;
	s16b town_level = 0;

	/* Is it a town level ? */
	for (i = 0; i < TOWN_DUNGEON; i++)
	{
		if (d_ptr->t_level[i] == dun_level) town_level = d_ptr->t_idx[i];
	}

	/* unused */
	name = name;

	/* Check for arena level */
	if ((has_flag(dungeon_flags, FLAG_EMPTY)) ||
	                (empty_levels && (rand_int(EMPTY_LEVEL) == 0)))
	{
		empty_level = TRUE;

		if (cheat_room || p_ptr->precognition)
		{
			msg_print("Arena level.");
		}
	}

#if 0	/* Possible cavern */
	if (((has_flag(dungeon_flags, FLAG_CAVERN))) && (rand_int(dun_level / 2) > DUN_CAVERN))
	{
		cavern = TRUE;

		/* Make a large fractal cave in the middle of the dungeon */
		if (cheat_room)
		{
			msg_print("Cavern on level.");
		}

		build_cavern();
	}
#endif
	/* Actual maximum number of rooms on this level */
	dun->row_rooms = cur_hgt / BLOCK_HGT;
	dun->col_rooms = cur_wid / BLOCK_WID;


	/* Initialize the room table */
	for (y = 0; y < dun->row_rooms; y++)
	{
		for (x = 0; x < dun->col_rooms; x++)
		{
			dun->room_map[y][x] = FALSE;
		}
	}


	/* No "crowded" rooms yet */
	dun->crowded = FALSE;


	/* No rooms yet */
	dun->cent_n = 0;

	/* Build some rooms */
	for (i = 0; i < DUN_ROOMS; i++)
	{
		/* Pick a block for the room */
		y = rand_int(dun->row_rooms);
		x = rand_int(dun->col_rooms);

		/* Align dungeon rooms */
		if (dungeon_align)
		{
			/* Slide some rooms right */
			if ((x % 3) == 0) x++;

			/* Slide some rooms left */
			if ((x % 3) == 2) x--;
		}

		/* If we have no rooms defining function do the classic stuff */
		if (!has_flag(dungeon_flags, FLAG_ROOM_DEFINITION))
		{
			/* Attempt an "unusual" room -- no vaults on town levels */
			if (!town_level && (ironman_rooms || (rand_int(DUN_UNUSUAL) < dun_level)))
			{
				/* Roll for room type */
				k = (ironman_rooms ? 0 : rand_int(100));

				/* Attempt a very unusual room */ /* test hack */
				if (ironman_rooms || (rand_int(DUN_UNUSUAL) < dun_level))
				{
#ifdef FORCE_V_IDX
					if (room_build(y, x, 8)) continue;
#else
					/* Type 8 -- Greater vault (10%) */
					if (k < 10)
					{
						if (max_vault_ok > 1)
						{
							if (room_build(y, x, 8)) continue;
						}
						else
						{
							if (cheat_room) msg_print("Refusing a greater vault.");
						}
					}

					/* Type 7 -- Lesser vault (15%) */
					if (k < 25)
					{
						if (max_vault_ok > 0)
						{
							if (room_build(y, x, 7)) continue;
						}
						else
						{
							if (cheat_room) msg_print("Refusing a lesser vault.");
						}
					}


					/* Type 5 -- Monster nest (15%) */
					if ((k < 40) && room_build(y, x, 5)) continue;

					/* Type 6 -- Monster pit (15%) */
					if ((k < 55) && room_build(y, x, 6)) continue;

					/* Type 11 -- Random vault (5%) */
					if ((k < 60) && room_build(y, x, 11)) continue;
#endif
				}

				/* Type 4 -- Large room (25%) */
				if ((k < 25) && room_build(y, x, 4)) continue;

				/* Type 3 -- Cross room (20%) */
				if ((k < 45) && room_build(y, x, 3)) continue;

				/* Type 2 -- Overlapping (20%) */
				if ((k < 65) && room_build(y, x, 2)) continue;

				/* Type 10 -- Fractal cave (15%) */
				if ((k < 80) && room_build(y, x, 10)) continue;

				/* Type 9 -- Circular (10%) */
				/* Hack - build standard rectangular rooms if needed */
				if (k < 90)
				{
					if (((has_flag(dungeon_flags, FLAG_CIRCULAR_ROOMS))) && room_build(y, x, 1)) continue;
					else if (room_build(y, x, 9)) continue;
				}

				/* Type 12 -- Crypt (10%) */
				if ((k < 100) && room_build(y, x, 12)) continue;
			}
		}
		/* Let the lua code do its magic */
		else
		{
			s32b ret;
			open_lua_functions_registry(get_flag(dungeon_flags,
												 FLAG_ROOM_DEFINITION),
										get_flag2(dungeon_flags,
												  FLAG_ROOM_DEFINITION));
			call_lua(NULL, "(d,d)", "d", y, x, &ret);
			close_lua_functions_registry();
			if (ret) continue;
		}

		/* Attempt a trivial room */
		if ((has_flag(dungeon_flags, FLAG_CAVE)))
		{
			if (room_build(y, x, ROOM_TYPE_FRACTAL))
				continue;
		}
		else
		{
			if (((has_flag(dungeon_flags, FLAG_CIRCULAR_ROOMS))) &&
				room_build(y, x, ROOM_TYPE_CIRCULAR)) continue;
			else if (room_build(y, x, ROOM_TYPE_NORMAL)) continue;
		}

		if (wizard)
			cmsg_print(TERM_L_RED, "WARNING: Could not create a room");
	} /* for (i = 0; i < DUN_ROOMS; i++) */

	/* If no rooms are allocated... */
	while (dun->cent_n == 0)
	{
		/* ...force the creation of a small rectangular room */
		(void)room_build(0, 0, 1);
	}

	/* Hack -- Scramble the room order */
	for (i = 0; i < dun->cent_n; i++)
	{
		s32b pick1 = rand_int(dun->cent_n);
		s32b pick2 = rand_int(dun->cent_n);
		y1 = dun->cent[pick1].y;
		x1 = dun->cent[pick1].x;
		dun->cent[pick1].y = dun->cent[pick2].y;
		dun->cent[pick1].x = dun->cent[pick2].x;
		dun->cent[pick2].y = y1;
		dun->cent[pick2].x = x1;
	}

	/* Start with no tunnel doors */
	dun->door_n = 0;

	/* Hack -- connect the first room to the last room */
	y = dun->cent[dun->cent_n - 1].y;
	x = dun->cent[dun->cent_n - 1].x;

	/* Connect all the rooms together */
	for (i = 0; i < dun->cent_n; i++)
	{
		/* Connect the room to the previous room */
		build_tunnel(dun->cent[i].y, dun->cent[i].x, y, x);

		/* Remember the "previous" room */
		y = dun->cent[i].y;
		x = dun->cent[i].x;
	}

	/* Mega-Hack -- Convert FEAT_WALL_SOLID back into outer walls */
	for (y = 0; y < cur_hgt; y++)
	{
		for (x = 0; x < cur_wid; x++)
		{
			if (cave[y][x].feat == FEAT_WALL_SOLID)
			{
				cave_set_feat(y, x, feat_wall_outer);
			}
		}
	}

	/* Place intersection doors	 */
	for (i = 0; i < dun->door_n; i++)
	{
		/* Extract junction location */
		y = dun->door[i].y;
		x = dun->door[i].x;

		/* Try placing doors */
		try_doors(y, x);
	}

#if 0 //DGDGDGDG
	if (strcmp(game_module, "ToME") == 0)
	{
		/* Hack -- Add some magma streamers */
		if ((dungeon_type == DUNGEON_BARADDUR) || (dungeon_type == DUNGEON_ANGBAND))
			for (i = 0; i < DUN_STR_MAG; i++)
			{
				build_streamer(FEAT_MAGMA, DUN_STR_MC);
			}

		/* Hack -- Add some quartz streamers */
		if ((dungeon_type == DUNGEON_BARADDUR) || (dungeon_type == DUNGEON_ANGBAND))
			for (i = 0; i < DUN_STR_QUA; i++)
			{
				build_streamer(FEAT_QUARTZ, DUN_STR_QC);
			}
	}

	/* Add some sand streamers */
	if (((has_flag(dungeon_flags, FLAG_SAND_VEIN))) && !rand_int(4))
	{
		if ((cheat_room) || (p_ptr->precognition)) msg_print("Sand vein.");
		build_streamer(FEAT_SANDWALL, DUN_STR_SC);
	}
#endif
	/* Create the town if needed */
	if (town_level)
	{
		town_gen(town_level);
	}

#if 0 //DGDGDGDG
	/* Hack -- Add some rivers if requested */
	if (((has_flag(dungeon_flags, FLAG_WATER_RIVER))) && !rand_int(4))
	{
		if (cheat_room || p_ptr->precognition) msg_print("River of water.");
		add_river(FEAT_DEEP_WATER, FEAT_SHAL_WATER);
	}
	if (((has_flag(dungeon_flags, FLAG_LAVA_RIVER))) && !rand_int(4))
	{
		if ((cheat_room) || (p_ptr->precognition)) msg_print("River of lava.");
		add_river(FEAT_DEEP_LAVA, FEAT_SHAL_LAVA);
	}

	if ((has_flag(dungeon_flags, FLAG_WATER_RIVERS)))
	{
		s32b max = 3 + rand_int(2);
		bool said = FALSE;

		for (i = 0; i < max; i++)
		{
			if (rand_int(3) == 0)
			{
				add_river(FEAT_DEEP_WATER, FEAT_SHAL_WATER);
				if (!said && ((cheat_room) || (p_ptr->precognition))) msg_print("Rivers of water.");
				said = TRUE;
			}
		}
	}

	if ((has_flag(dungeon_flags, FLAG_LAVA_RIVERS)))
	{
		s32b max = 2 + rand_int(2);
		bool said = FALSE;

		for (i = 0; i < max; i++)
		{
			if (rand_int(3) == 0)
			{
				add_river(FEAT_DEEP_LAVA, FEAT_SHAL_LAVA);
				if (!said && ((cheat_room) || (p_ptr->precognition))) msg_print("Rivers of lava.");
				said = TRUE;
			}
		}
	}

	/* Add streamers of trees, water, or lava -KMW- */
	if (!((has_flag(dungeon_flags, FLAG_NO_STREAMERS))))
	{
		s32b num;

		/*
		 * Flat levels (was: levels 1--2)
		 *
		 * Small trees (penetrate walls)
		 */
		if (((has_flag(dungeon_flags, FLAG_FLAT))) && (randint(20) > 15))
		{
			num = randint(DUN_STR_QUA);

			for (i = 0; i < num; i++)
			{
				build_streamer2(FEAT_SMALL_TREES, 1);
			}
		}

		/*
		 * Levels 1 -- 33 (was: 1 -- 19)
		 *
		 * Shallow water (preserve walls)
		 * Deep water (penetrate walls)
		 */
		if (!(dun_level <= 33) && (randint(20) > 15))
		{
			num = randint(DUN_STR_QUA - 1);

			for (i = 0; i < num; i++)
			{
				build_streamer2(FEAT_SHAL_WATER, 0);
			}

			if (randint(20) > 15)
			{
				num = randint(DUN_STR_QUA);

				for (i = 0; i < num; i++)
				{
					build_streamer2(FEAT_DEEP_WATER, 1);
				}
			}
		}

		/*
		 * Levels 34 -- (was: 20 --)
		 */
		else if (dun_level > 33)
		{
			/*
			 * Shallow lava (preserve walls)
			 * Deep lava (penetrate walls)
			 */
			if (randint(20) > 15)
			{
				num = randint(DUN_STR_QUA);

				for (i = 0; i < num; i++)
				{
					build_streamer2(FEAT_SHAL_LAVA, 0);
				}

				if (randint(20) > 15)
				{
					num = randint(DUN_STR_QUA - 1);

					for (i = 0; i < num; i++)
					{
						build_streamer2(FEAT_DEEP_LAVA, 1);
					}
				}
			}

			/*
			 * Shallow water (preserve walls)
			 * Deep water (penetrate walls)
			 */
			else if (randint(20) > 15)
			{
				num = randint(DUN_STR_QUA - 1);

				for (i = 0; i < num; i++)
				{
					build_streamer2(FEAT_SHAL_WATER, 0);
				}

				if (randint(20) > 15)
				{
					num = randint(DUN_STR_QUA);

					for (i = 0; i < num; i++)
					{
						build_streamer2(FEAT_DEEP_WATER, 1);
					}
				}
			}
		}
	}
#endif
	/* Hack, seems like once a room overrode the level boundaries, this is BAD */
	set_bounders(empty_level);

	/* Determine the character location */
	if (!new_player_spot(branch))
		return FALSE;

	return TRUE;
}

/*
 * Bring the imprinted pets from the old level
 */
void replace_all_friends()
{
	if (p_ptr->wild_mode) return;

	/* Scan every saved pet */
	for_flags(&keep_monst_list);
	{
		s32b y = p_ptr->py, x = p_ptr->px;
		s32b y1, x1;
		cave_type *c_ptr;
		monster_type *m_ptr;
		s32b nb_tries;

		/* Find a suitable location */
		/* Look for a location */
		for (nb_tries = 0; nb_tries < 600; ++nb_tries)
		{
			/* Pick a distance */
			s32b d = (nb_tries / 15) + 1;

			/* Pick a location */
			scatter(&y1, &x1, y, x, d, 0);

			/* Require "empty" floor grid */
			if (!cave_empty_bold(y1, x1)) continue;

			/* Okay */
			break;
		}

		/* Failure */
		if (nb_tries == 600)
		{
			if (wizard) msg_print("LOSING COMPANION");
			continue;
		}

		c_ptr = &cave[y1][x1];

		/* Get a m_idx to use */
		c_ptr->m_idx = m_pop();
		m_ptr = flag_get_monster(&keep_monst_list, __key);
		flag_remove_zap(&keep_monst_list, __key);

		/* Actualy place the monster */
		flag_set_monster(&monst_list, c_ptr->m_idx, m_ptr);
		m_ptr->fy = y1;
		m_ptr->fx = x1;
	}
	end_for_flags();
	flag_empty(&keep_monst_list);
}

/*
 * Save the imprinted pets from the old level
 */
void save_all_friends()
{
	if (p_ptr->old_wild_mode) return;

	flag_empty(&keep_monst_list);
	for_flags(&monst_list);
	{
		monster_type *m_ptr = get_monster(__key);

		if (m_ptr->r_idx && has_flag(m_ptr, FLAG_PERMANENT))
		{
			/* Remove but do not dealloc */
			flag_remove_zap(&monst_list, __key);
			/* Place in the keeping list */
			flag_set_monster(&keep_monst_list, flag_first_free_key(&keep_monst_list, 1), m_ptr);
		}
	}
	end_for_flags();
}



/*
 * Build probability tables for walls and floors and set feat_wall_outer
 * and feat_wall_inner according to the current information in d_info.txt
 *
 * *hint* *hint* with this made extern, and we no longer have to
 * store fill_type and floor_type in the savefile...
 */
static void init_feat_info(void)
{
	dungeon_info_type *d_ptr = &d_info[dungeon_type];
	s32b i;
	s32b cur_depth, max_depth;
	s32b p1, p2;
	s32b floor_lim1, floor_lim2, floor_lim3;
	s32b fill_lim1, fill_lim2, fill_lim3;


	/* Retrieve dungeon depth info (base 1, to avoid zero divide errors) */
	cur_depth = (dun_level - d_ptr->mindepth) + 1;
	max_depth = (d_ptr->maxdepth - d_ptr->mindepth) + 1;


	/* Set room wall types */
	feat_wall_outer = d_ptr->outer_wall;
	feat_wall_inner = d_ptr->inner_wall;

	/* Setup probability info -- Floors */
	p1 = d_ptr->floor_percent1[0];
	p2 = d_ptr->floor_percent1[1];
	floor_lim1 = p1 + (p2 - p1) * cur_depth / max_depth;

	p1 = d_ptr->floor_percent2[0];
	p2 = d_ptr->floor_percent2[1];
	floor_lim2 = floor_lim1 + p1 + (p2 - p1) * cur_depth / max_depth;

	floor_lim3 = 100;

	/* Setup probability info -- Fillers */
	p1 = d_ptr->fill_percent1[0];
	p2 = d_ptr->fill_percent1[1];
	fill_lim1 = p1 + (p2 - p1) * cur_depth / max_depth;

	p1 = d_ptr->fill_percent2[0];
	p2 = d_ptr->fill_percent2[1];
	fill_lim2 = fill_lim1 + p1 + (p2 - p1) * cur_depth / max_depth;

	fill_lim3 = 100;


	/* Fill the arrays of floors and walls in the good proportions */
	for (i = 0; i < 100; i++)
	{
		if (i < floor_lim1)
		{
			floor_type[i] = d_ptr->floor1;
		}
		else if (i < floor_lim2)
		{
			floor_type[i] = d_ptr->floor2;
		}
		else
		{
			floor_type[i] = d_ptr->floor3;
		}

		if (i < fill_lim1)
		{
			fill_type[i] = d_ptr->fill_type1;
		}
		else if (i < fill_lim2)
		{
			fill_type[i] = d_ptr->fill_type2;
		}
		else
		{
			fill_type[i] = d_ptr->fill_type3;
		}
	}
}

/*
 * Different from the init_feat_info() because wilderness features
 * don't have floor and wall definitions, but a terrain definition.
 */
static void init_feat_info_wild(void)
{
	s32b  i, num_floors, num_walls;
	s32b  y = p_ptr->wilderness_y, x = p_ptr->wilderness_x;
	byte *terrain_feats = wf_info[wild_map[y][x].feat].terrain;
	byte floor_feats[MAX_WILD_TERRAIN], wall_feats[MAX_WILD_TERRAIN];

	num_floors = num_walls = 0;
	for (i = 0; i < MAX_WILD_TERRAIN; i++)
		if (has_flag(&f_info[terrain_feats[i]], FLAG_FLOOR))
			floor_feats[num_floors++] = terrain_feats[i];
		else
			wall_feats[num_walls++] = terrain_feats[i];

	if (num_floors >= 1)
	{
		s32b div = ceil(100.0 / num_floors);
		for (i = 0; i < 100; i++)
			floor_type[i] = floor_feats[i / div];
	}
	else
		for (i = 0; i < 100; i++)
			floor_type[i] = 0;

	if (num_walls >= 1)
	{
		s32b div = ceil(100.0 / num_walls);

		for (i = 0; i < 100; i++)
			fill_type[i] = wall_feats[i / (100 / div)];
	}
	else
		for (i = 0; i < 100; i++)
			fill_type[i] = 0;
} /* init_feat_info_wild() */

/*
 * Fill a level with wall type specified in A: or L: line of d_info.txt
 *
 * 'use_floor', when it is TRUE, tells the function to use floor type
 * terrains (L:) instead of walls (A:).
 *
 * Filling behaviour can be controlled by the second parameter 'smooth',
 * with the following options available:
 *
 * smooth  behaviour
 * ------  ------------------------------------------------------------
 * 0       Fill the entire level with fill_type1 / floor1
 * 1       All the grids are randomly selected (== --P5.1.2)
 * 2       Slightly smoothed -- look like scattered patches
 * 3       More smoothed -- tend to look like caverns / small scale map
 * 4--     Max smoothing -- tend to look like landscape/island/
 *         continent etc.
 *
 * I put it here, because there's another filler generator in
 * wild.c, but it works better there, in fact...
 *
 * CAVEAT: smoothness of 3 or greater doesn't work well with the
 * current secret door implementation. Outer walls also need some
 * rethinking.
 *
 * -- pelpel
 */

/*
 * Thou shalt not invoke the name of thy RNG in vain.
 * The Angband RNG generates 28 bit pseudo-random number, hence
 * 28 / 2 = 14
 */
#define MAX_SHIFTS 14

static void fill_level(bool use_floor, byte smooth)
{
	s32b y, x;
	s32b step;
	s32b shift;


	/* Convert smoothness to initial step */
	if (smooth == 0) step = 0;
	else if (smooth == 1) step = 1;
	else if (smooth == 2) step = 2;
	else if (smooth == 3) step = 4;
	else step = 8;

	/*
	 * Paranoia -- step must be less than or equal to a half of
	 * width or height, whichever shorter
	 */
	if ((cur_hgt < 16) && (step > 4)) step = 4;
	if ((cur_wid < 16) && (step > 4)) step = 4;


	/* Special case -- simple fill */
	if (step == 0)
	{
		byte filler;

		/* Pick a filler XXX XXX XXX */
		if (use_floor) filler = d_info[dungeon_type].floor1;
		else filler = d_info[dungeon_type].fill_type1;

		/* Fill the level with the filler without calling RNG */
		for (y = 0; y < cur_hgt; y++)
		{
			for (x = 0; x < cur_wid; x++)
			{
				cave_set_feat(y, x, filler);
			}
		}

		/* Done */
		return;
	}


	/*
	 * Fill starting positions -- every 'step' grids horizontally and
	 * vertically
	 */
	for (y = 0; y < cur_hgt; y += step)
	{
		for (x = 0; x < cur_wid; x += step)
		{
			/*
			 * Place randomly selected terrain feature using the prebuilt
			 * probability table
			 *
			 * By slightly modifying this, you can build streamers as
			 * well as normal fillers all at once, but this calls for
			 * modifications to the other part of the dungeon generator.
			 */
			if (use_floor) place_floor(y, x);
			else place_filler(y, x);
		}
	}


	/*
	 * Fill spaces between, randomly picking one of their neighbours
	 *
	 * This simple yet powerful algorithm was described by Mike Anderson:
	 *
	 * A   B      A | B      A a B
	 *        ->  --+--  ->  d e b
	 * D   C      D | C      D c C
	 *
	 * a can be either A or B, b B or C, c C or D and d D or A.
	 * e is chosen from A, B, C and D.
	 * Subdivide and repeat the process as many times as you like.
	 *
	 * All the nasty tricks that obscure this simplicity are mine (^ ^;)
	 */

	/* Initialise bit shift counter */
	shift = MAX_SHIFTS;

	/* Repeat subdivision until all the grids are filled in */
	while ((step = step >> 1) > 0)
	{
		bool y_even, x_even;
		s16b y_wrap, x_wrap;
		s16b y_sel, x_sel;
		u32b selector = 0;

		/* Hacklette -- Calculate wrap-around locations */
		y_wrap = ((cur_hgt - 1) / (step * 2)) * (step * 2);
		x_wrap = ((cur_wid - 1) / (step * 2)) * (step * 2);

		/* Initialise vertical phase */
		y_even = 0;

		for (y = 0; y < cur_hgt; y += step)
		{
			/* Flip vertical phase */
			y_even = !y_even;

			/* Initialise horizontal phase */
			x_even = 0;

			for (x = 0; x < cur_wid; x += step)
			{
				/* Flip horizontal phase */
				x_even = !x_even;

				/* Already filled in by previous iterations */
				if (y_even && x_even) continue;

				/*
				 * Retrieve next two bits from pseudo-random bit sequence
				 *
				 * You can do well not caring so much about their randomness.
				 *
				 * This is not really necessary, but I don't like to invoke
				 * relatively expensive RNG when we can do with much smaller
				 * number of calls.
				 */
				if (shift >= MAX_SHIFTS)
				{
					selector = rand_int(0x10000000L);
					shift = 0;
				}
				else
				{
					selector >>= 2;
					shift++;
				}

				/* Vertically in sync */
				if (y_even) y_sel = y;

				/* Bit 1 selects neighbouring y */
				else y_sel = (selector & 2) ? y + step : y - step;

				/* Horizontally in sync */
				if (x_even) x_sel = x;

				/* Bit 0 selects neighbouring x */
				else x_sel = (selector & 1) ? x + step : x - step;

				/* Hacklette -- Fix out of range indices by wrapping around */
				if (y_sel >= cur_hgt) y_sel = 0;
				else if (y_sel < 0) y_sel = y_wrap;
				if (x_sel >= cur_wid) x_sel = 0;
				else if (x_sel < 0) x_sel = x_wrap;

				/*
				 * Fill the grid with terrain feature of the randomly
				 * picked up neighbour
				 */
				cave_set_feat(y, x, cave[y_sel][x_sel].feat);
			}
		}
	}
}


/*
 * Generate a new dungeon level
 *
 * Note that "dun_body" adds about 4000 bytes of memory to the stack.
 */
static bool cave_gen(void)
{
	s32b i, k, branch;
	dungeon_info_type *d_ptr = &d_info[dungeon_type];

	s32b max_vault_ok = 2;

	bool empty_level = FALSE;
	s16b town_level = 0;

	level_generator_type *generator;

	dun_data dun_body;

	char generator_name[100];

	if (!get_dungeon_generator(generator_name))
		strnfmt(generator_name, 99, "%s", d_ptr->generator);

	/*
	 * We generate a double dungeon. First we should halve the desired
	 * width/height, generate the dungeon normally, then double it
	 * in both directions
 */
	if ((has_flag(dungeon_flags, FLAG_DOUBLE)))
	{
		cur_wid /= 2;
		cur_hgt /= 2;
	}

	/* Is it a town level ? */
	for (i = 0; i < TOWN_DUNGEON; i++)
	{
		if (d_ptr->t_level[i] == dun_level) town_level = d_ptr->t_idx[i];
	}

	/* Set the correct monster hook */
	set_mon_num_hook();

	/* Prepare allocation table */
	get_mon_num_prep();

	/* Global data */
	dun = &dun_body;

	if (!(max_panel_rows)) max_vault_ok--;
	if (!(max_panel_cols)) max_vault_ok--;

	/*
	 * Hack -- Start with fill_type's
	 *
	 * Need a way to know appropriate smoothing factor for the current
	 * dungeon. Maybe we need another d_info flag/value.
	 */
	fill_level(empty_level, d_ptr->fill_method);

	set_bounders(empty_level);

	/*
	 * Call the good level generator
	 */
	generator = level_generators;
	while (generator)
	{
		if (!strcmp(generator->name, generator_name))
		{
			if (!generator->generator(generator->name))
			{
				if (wizard) msg_format("Wrong generator: %s", generator_name);
				return FALSE;
			}
			break;
		}

		generator = generator->next;
	}

	/* Only if requested */
	if (generator->default_stairs)
	{
		/* Is there a dungeon branch ? */
		if ((branch = get_branch()))
		{
			/* Place 5 down stair some walls */
			alloc_stairs(FEAT_MORE, 5, 3, branch);
		}

		/* Is there a father dungeon branch ? */
		if ((branch = get_fbranch()))
		{
			/* Place 1 down stair some walls */
			alloc_stairs(FEAT_LESS, 5, 3, branch);
		}

		if ((dun_level < d_ptr->maxdepth) || ((dun_level == d_ptr->maxdepth) && ((has_flag(dungeon_flags, FLAG_FORCE_DOWN)))))
		{
			/* Place 3 or 4 down stairs near some walls */
			alloc_stairs(((has_flag(dungeon_flags, FLAG_FLAT))) ? FEAT_WAY_MORE : FEAT_MORE, rand_range(3, 4), 3, 0);

			/* Place 1 or 2 down shafts near some walls */
			if (!((has_flag(dungeon_flags, FLAG_NO_SHAFT)))) alloc_stairs(((has_flag(dungeon_flags, FLAG_FLAT))) ? FEAT_WAY_MORE : FEAT_SHAFT_DOWN, rand_range(0, 1), 3, 0);
		}

		if ((dun_level > d_ptr->mindepth) || ((dun_level == d_ptr->mindepth) && (!((has_flag(dungeon_flags, FLAG_NO_UP))))))
		{
			/* Place 1 or 2 up stairs near some walls */
			alloc_stairs(((has_flag(dungeon_flags, FLAG_FLAT))) ? FEAT_WAY_LESS : FEAT_LESS, rand_range(1, 2), 3, 0);

			/* Place 0 or 1 up shafts near some walls */
			if (!((has_flag(dungeon_flags, FLAG_NO_SHAFT)))) alloc_stairs(((has_flag(dungeon_flags, FLAG_FLAT))) ? FEAT_WAY_LESS : FEAT_SHAFT_UP, rand_range(0, 1), 3, 0);
		}
	}

	process_hooks(HOOK_GEN_LEVEL, "(d)", is_quest(dun_level));

	/* Monsters and objects change even in persistent dungeons. */
	if (seed_dungeon) Rand_quick = FALSE;

	/* Basic "amount" */
	k = (dun_level / 3);
	if (k > 10) k = 10;
	if (k < 2) k = 2;

	/* Only if requested */
	if (generator->default_monsters)
	{
		/* Pick a base number of monsters */
		i = d_ptr->min_m_alloc_level + randint(8);

		/* Put some monsters in the dungeon */
		for (i = i + k; i > 0; i--)
		{
			(void)alloc_monster(0, TRUE);
		}
	}

	/* Only if requested */
	if (generator->default_miscs)
	{
		bool has_traps = FALSE, has_rubbles = FALSE;
		s32b traps, rubbles;

		if (has_flag(dungeon_flags, FLAG_GENERATOR_GET_QUANTITY))
		{
			open_lua_functions_registry(get_flag(dungeon_flags, FLAG_GENERATOR_GET_QUANTITY), get_flag2(dungeon_flags, FLAG_GENERATOR_GET_QUANTITY));
			call_lua(NULL, "(s,d)", "bd", "traps", k * 2, &has_traps, &traps);
			close_lua_functions_registry();
			open_lua_functions_registry(get_flag(dungeon_flags, FLAG_GENERATOR_GET_QUANTITY), get_flag2(dungeon_flags, FLAG_GENERATOR_GET_QUANTITY));
			call_lua(NULL, "(s,d)", "bd", "rubbles", k * 2, &has_rubbles, &rubbles);
			close_lua_functions_registry();
		}

		/* Place some traps in the dungeon */
		if (!has_traps || traps) alloc_object(ALLOC_SET_BOTH, ALLOC_TYP_TRAP, has_traps ? traps : randint(k * 2));

		/* Put some rubble in corridors */
		if (!has_rubbles || rubbles) alloc_object(ALLOC_SET_CORR, ALLOC_TYP_RUBBLE, has_rubbles ? rubbles : randint(k));
	}

	/* Only if requested */
	if (generator->default_objects)
	{
		bool has_gold = FALSE, has_objs_room = FALSE, has_objs_dun = FALSE;
		s32b objs_room, objs_dun, gold;

		if (has_flag(dungeon_flags, FLAG_GENERATOR_GET_QUANTITY))
		{
			open_lua_functions_registry(get_flag(dungeon_flags, FLAG_GENERATOR_GET_QUANTITY), get_flag2(dungeon_flags, FLAG_GENERATOR_GET_QUANTITY));
			call_lua(NULL, "(s,d)", "bd", "objects_room", DUN_AMT_ROOM, &has_objs_room, &objs_room);
			close_lua_functions_registry();
			open_lua_functions_registry(get_flag(dungeon_flags, FLAG_GENERATOR_GET_QUANTITY), get_flag2(dungeon_flags, FLAG_GENERATOR_GET_QUANTITY));
			call_lua(NULL, "(s,d)", "bd", "objects_dun", DUN_AMT_ITEM, &has_objs_dun, &objs_dun);
			close_lua_functions_registry();
			open_lua_functions_registry(get_flag(dungeon_flags, FLAG_GENERATOR_GET_QUANTITY), get_flag2(dungeon_flags, FLAG_GENERATOR_GET_QUANTITY));
			call_lua(NULL, "(s,d)", "bd", "gold", DUN_AMT_GOLD, &has_gold, &gold);
			close_lua_functions_registry();
		}

		/* Put some objects in rooms */
		if (!has_objs_room || objs_room) alloc_object(ALLOC_SET_ROOM, ALLOC_TYP_OBJECT, has_objs_room ? objs_room : randnor(DUN_AMT_ROOM, 3));

		/* Put some objects/gold in the dungeon */
		if (!has_objs_dun || objs_dun) alloc_object(ALLOC_SET_BOTH, ALLOC_TYP_OBJECT, has_objs_dun ? objs_dun : randnor(DUN_AMT_ITEM, 3));
		if (!has_gold || gold) alloc_object(ALLOC_SET_BOTH, ALLOC_TYP_GOLD, has_gold ? gold : randnor(DUN_AMT_GOLD, 3));
	}

	/* Only if requested */
	if (generator->default_miscs)
	{
	}

	/* Put an Artifact and Artifact Guardian is requested */
	if (has_flag(d_ptr, FLAG_DUNGEON_GUARDIAN) && (d_ptr->maxdepth == dun_level))
	{
		s32b oy;
		s32b ox;
		s32b m_idx, try = 10000;

		/* Find a good position */
		while (try)
		{
			/* Get a random spot */
			oy = randint(cur_hgt - 4) + 2;
			ox = randint(cur_wid - 4) + 2;

			/* Is it a good spot ? */
			if (cave_empty_bold(oy, ox)) break;

			/* One less try */
			try--;
		}

		/* Place the guardian */
		m_allow_special[get_flag(d_ptr, FLAG_DUNGEON_GUARDIAN)] = TRUE;
		place_monster_one(oy, ox, get_flag(d_ptr, FLAG_DUNGEON_GUARDIAN), 0, FALSE, FACTION_DUNGEON);
		m_allow_special[get_flag(d_ptr, FLAG_DUNGEON_GUARDIAN)] = FALSE;

		m_idx = cave[oy][ox].m_idx;

		if (!m_idx && wizard) cmsg_print(TERM_L_RED, "WARNING: COUlD not place guardian");

		/*
		 * If guardian is successfully created and his/her/its
		 * treasure hasn't been found, let him/her/it own that
		 */
		if (m_idx && has_flag(d_ptr, FLAG_DUNGEON_ARTIFACT) &&
		                (a_info[get_flag(d_ptr, FLAG_DUNGEON_ARTIFACT)].cur_num == 0))
		{
#if 0 // DGDGDGDG
			artifact_type *a_ptr = &a_info[get_flag(d_ptr, FLAG_DUNGEON_ARTIFACT)];
			object_type *q_ptr, *o_ptr;
			s32b I_kind, o_idx;

			/* Get new object */
			o_idx = o_pop();

			/* Proceed only if there's an object slot available */
			if (o_idx)
			{
				a_allow_special[get_flag(d_ptr, FLAG_DUNGEON_ARTIFACT)] = TRUE;

				/* Get local object */
				q_ptr = new_object();

				/* Wipe the object */
				object_wipe(q_ptr);

				/* Acquire the "kind" index */
				I_kind = lookup_kind(a_ptr->tval, a_ptr->sval);

				/* Create the artifact */
				object_prep(q_ptr, I_kind);

				/* Save the name */
				q_ptr->artifact_id = get_flag(d_ptr, FLAG_DUNGEON_ARTIFACT);

				/* Actually create it */
				apply_magic(q_ptr, -1, TRUE, TRUE, TRUE);

				/* Where it is found ? */
				q_ptr->found = OBJ_FOUND_MONSTER;
				q_ptr->found_aux1 = get_flag(d_ptr, FLAG_DUNGEON_GUARDIAN);
				q_ptr->found_aux2 = 0;
				q_ptr->found_aux3 = dungeon_type;
				q_ptr->found_aux4 = level_or_feat(dungeon_type, dun_level);

				a_allow_special[get_flag(d_ptr, FLAG_DUNGEON_ARTIFACT)] = FALSE;

				/* Get the item */
				delete_object(o_list[o_idx]);
				o_list[o_idx] = q_ptr;
				o_ptr = q_ptr;
			}
#endif
		}

		if (m_idx && has_flag(d_ptr, FLAG_DUNGEON_OBJECT) &&
		                (k_info[get_flag(d_ptr, FLAG_DUNGEON_OBJECT)].artifact == FALSE))
		{
#if 0 // DGDGDGDG
			object_type *q_ptr, *o_ptr;
			s32b o_idx;

			/* Get new object */
			o_idx = o_pop();

			/* Proceed only if there's an object slot available */
			if (o_idx)
			{
				/* Get local object */
				q_ptr = new_object();

				k_allow_special[get_flag(d_ptr, FLAG_DUNGEON_OBJECT)] = TRUE;

				/* Wipe the object */
				object_wipe(q_ptr);

				/* Create the final object */
				object_prep(q_ptr, get_flag(d_ptr, FLAG_DUNGEON_OBJECT));
				apply_magic(q_ptr, 1, FALSE, FALSE, FALSE);

				/* Where it is found ? */
				q_ptr->found = OBJ_FOUND_MONSTER;
				q_ptr->found_aux1 = get_flag(d_ptr, FLAG_DUNGEON_GUARDIAN);
				q_ptr->found_aux2 = 0;
				q_ptr->found_aux3 = dungeon_type;
				q_ptr->found_aux4 = level_or_feat(dungeon_type, dun_level);

				k_allow_special[get_flag(d_ptr, FLAG_DUNGEON_OBJECT)] = FALSE;

				k_info[get_flag(d_ptr, FLAG_DUNGEON_OBJECT)].artifact = TRUE;

				/* Get the item */
				delete_object(o_list[o_idx]);
				o_list[o_idx] = q_ptr;
				o_ptr = q_ptr;
			}
#endif
		}
	}

	if (has_flag(dungeon_flags, FLAG_LITE_EMPTY) && (empty_level) && (randint(DARK_EMPTY) != 1 || (randint(100) > dun_level)))
		wiz_lite(TRUE);

	/* Now double the generated dungeon */
	if ((has_flag(dungeon_flags, FLAG_DOUBLE)))
	{
		s32b y, y1, x, x1;

		/* We begin at the bottom-right corner and from there move
		 * up/left (this way we don't need another array for the
		 * dungeon data) */
		/* Note: we double the border permanent walls, too. It is
		 * easier this way and I think it isn't too ugly */
		for (y = cur_hgt - 1, y1 = y * 2; y >= 0; y--, y1 -= 2)
			for (x = cur_wid - 1, x1 = x * 2; x >= 0; x--, x1 -= 2)
			{
				s32b disp[4][2] = {{0, 0}, {0, + 1}, { + 1, 0}, { + 1, + 1}};

				cave_type *cc_ptr    = &cave[y][x];
				s32b       m_idx     = cc_ptr->m_idx;
				s32b       rand_quad = rand_int(4);

				/*
				 * Now we copy the generated data to the
				 * appropriate grids
				 */
				for (i = 0; i < 4; i++)
				{
					cave_type *x_ptr =
						&cave[y1 + disp[i][0]][x1 + disp[i][1]];

					x_ptr->m_idx = 0;
					x_ptr->info  = cc_ptr->info;
					x_ptr->feat  = cc_ptr->feat;
					x_ptr->mimic = cc_ptr->mimic;
					x_ptr->mana  = cc_ptr->mana;

					flag_copy(&x_ptr->flags,       &cc_ptr->flags);
					flag_copy(&x_ptr->activations, &cc_ptr->activations);

					if (i == rand_quad)
					{
						flag_shallow_copy(&x_ptr->inventory,
										  &cc_ptr->inventory);
						flag_empty_zap(&cc_ptr->inventory);

						for_inventory_slot(&x_ptr->inventory, o_ptr);
						{
							o_ptr->iy = y1 + disp[i][0];
							o_ptr->ix = x1 + disp[i][1];
						}
						end_inventory_slot();

						if (m_idx != 0)
						{
							monster_type *m_ptr = get_monster(m_idx);

							x_ptr->m_idx  = m_idx;
							cc_ptr->m_idx = 0;

							m_ptr->fy = y1 + disp[i][0];
							m_ptr->fx = x1 + disp[i][1];
						}
					} /* if (i == rand_quad) */
				} /* for (i = 0; i < 4; i++) */
			}

		/* Set the width/height ... */
		cur_wid *= 2;
		cur_hgt *= 2;

		/* ... and player position to the right place */
		p_ptr->py *= 2;
		p_ptr->px *= 2;
	}
	return TRUE;
}


/*
 * Generate a quest level
 */
static void quest_gen(void)
{
	process_hooks(HOOK_GEN_QUEST, "(d)", is_quest(dun_level));
}

/*
 * Creates a special level
 */

/* Mega-Hack */
#define REGEN_HACK 0x02

bool build_special_level(void)
{
	char buf[80];
	s32b y, x;
	s16b level;

	/* No special levels on the surface */
	if (!dun_level) return FALSE;

	level = dun_level - d_info[dungeon_type].mindepth;
	if ((!has_flag(dungeon_flags, FLAG_SAVE_LEVEL)) && (special_lvl[level][dungeon_type])) return FALSE;
	if (!get_dungeon_special(buf)) return FALSE;

	/* Start with perm walls */
	for (y = 0; y < cur_hgt; y++)
	{
		for (x = 0; x < cur_wid; x++)
		{
			cave_set_feat(y, x, FEAT_PERM_SOLID);
		}
	}
	/* Set the correct monster hook */
	set_mon_num_hook();

	/* Prepare allocation table */
	get_mon_num_prep();

	call_lua("load_map", "(s,d,d)", "", buf, 0, 0);

	special_lvl[level][dungeon_type] = REGEN_HACK;
	generate_special_feeling = TRUE;

	/* Special feeling because it's special */
	good_item_flag = TRUE;

	/*
	 * Hack -- It's better/more dangerous than a greater vault.
	 * Better to have a rating field in special level description.
	 */
	rating += 40;

	return TRUE;
}

/*
 * Prepare regeneration of a special level, which should not happen,
 * but just in case...
 */
static void wipe_special_level(void)
{
	s16b level;
	char buf[80];

	/* No special levels on the surface */
	if (!dun_level) return;

	process_hooks(HOOK_LEVEL_REGEN, "()");

	/* Calculate relative depth */
	level = dun_level - d_info[dungeon_type].mindepth;

	/* No special level at this depth */
	if ((!has_flag(dungeon_flags, FLAG_SAVE_LEVEL)) &&
	                special_lvl[level][dungeon_type]) return;
	if (!get_dungeon_special(buf)) return;

	/* Clear the Mega-Hack flag */
	if (special_lvl[level][dungeon_type] == REGEN_HACK)
		special_lvl[level][dungeon_type] = FALSE;
}

/*
 * Finalise generation of a special level
 */
static void finalise_special_level(void)
{
	s16b level;
	char buf[80];

	process_hooks(HOOK_LEVEL_END_GEN, "()");

	/* No special levels on the surface */
	if (!dun_level) return;

	/* Calculate relative depth */
	level = dun_level - d_info[dungeon_type].mindepth;

	/* No special level at this depth */
	if (!has_flag(dungeon_flags, FLAG_SAVE_LEVEL) &&
		 (special_lvl[level][dungeon_type] == TRUE))
		return;
	if (!get_dungeon_special(buf)) return;

	/* Set the "generated" flag */
	if (special_lvl[level][dungeon_type] == REGEN_HACK)
	{
		/* Only if the level is not persistent */
		if (!has_flag(dungeon_flags, FLAG_SAVE_LEVEL))
			special_lvl[level][dungeon_type] = TRUE;
		else
			special_lvl[level][dungeon_type] = FALSE;
	}
}

/*
 * Give some magical energy to the each grid of the level
 */
void generate_grid_mana()
{
	s32b y, x, mana, mult;
	bool xtra_magic = FALSE;

	if (randint(XTRA_MAGIC) == 1)
	{
		xtra_magic = TRUE;

		if (cheat_room || p_ptr->precognition)
		{
			msg_print("Magical level");
		}
	}

	mult = ((xtra_magic) ? 3 : 2);

	for (y = 0; y < cur_hgt; y++)
	{
		for (x = 0; x < cur_wid; x++)
		{
			cave_type *c_ptr = &cave[y][x];

			/* Calculate the amount of mana in each grid */
			mana = mult * m_bonus(255, dun_level) / 2;
			if (xtra_magic) mana += 10 + rand_int(10);

			/* Never more than 255 or less than 0(parano�a) */
			if (mana < 0) mana = 0;
			if (mana > 255) mana = 255;

			c_ptr->mana = mana;
		}
	}
}

void dungeon_set_level_size(s32b hgt, s32b wid)
{
	/* Requested size level */
	cur_hgt = hgt;
	cur_wid = wid;
	if (cur_hgt > MAX_HGT) cur_hgt = MAX_HGT;
	if (cur_wid > MAX_WID) cur_wid = MAX_WID;

	/* Determine number of panels */
	max_panel_rows = (cur_hgt / SCREEN_HGT) * 2 - 2;
	max_panel_cols = (cur_wid / SCREEN_WID) * 2 - 2;

	/* Assume illegal panel */
	panel_row_min = max_panel_rows * (SCREEN_HGT / 2);
	panel_col_min = max_panel_cols * (SCREEN_WID / 2);
}

/*
 * Remove any already killed uniques present in a saved level.
 *
 * This allow the generation of uniques on multiple saved levels without
 * allowing the player to kill them twice.
 */
void purge_dead_uniques(void)
{
	s32b i;
	monster_type *m_ptr;
	monster_race *r_ptr;

	for_flags(&monst_list);
	{
		i = __key;
		m_ptr = get_monster(i);

		if (!m_ptr->r_idx) continue;
		if (!has_flag(m_ptr, FLAG_UNIQUE)) continue;
		
		r_ptr = race_inf(m_ptr);

		if (r_ptr->cur_num > r_ptr->max_num) {
			if (wizard)
				msg_format("Duplicated unique purged : %d.", m_ptr->r_idx);
			delete_monster_idx(i);
		}
	}
	end_for_flags();
}

/*
 * Count the artifact in a cave or monster inventory after loading a level.
 * If an artifact is found to be a duplicate, delete it.
 *
 * Recurse in any flags sets (including inside monsters and objects) and
 * in monster monster inventories.
 */
void count_loaded_artifacts_inven(flags_type *inven)
{
	s32b i;
	object_type *o_ptr;
	vector_type *to_delete = vector_new();

	/* Note : for_inventory_slot_ro declares item_nb */
	for_inventory_slot_ro(inven, o_ptr);
	{
		if (o_ptr->artifact_id && !object_known_p(o_ptr))
		{
			if (wizard)
				msg_format("Found an artifact (%d)", o_ptr->artifact_id);
			if (a_info[o_ptr->artifact_id].cur_num)
				vector_push(to_delete, item_nb);
			else
				a_info[o_ptr->artifact_id].cur_num = 1;
		}
		if (flag_exists(&(o_ptr->flags), FLAG_NORM_ART) && 
			!object_known_p(o_ptr))
		{
			if (wizard)
				msg_format("Found a NORM_ART (%d)", o_ptr->k_idx);
			if (k_info[o_ptr->k_idx].artifact)
				vector_push(to_delete, item_nb);
			else
				k_info[o_ptr->k_idx].artifact = TRUE;
		}
	}
	end_inventory();

	/* Delete the artifacts backwards to keep valid indices */
	while (!vector_is_empty(to_delete))
	{
		s32b idx = vector_pop(to_delete);
		s32b item;
		s32b max = flag_max_key(inven);

		if (wizard)
			msg_format("Deleting duplicate artifact.");
		flag_remove(inven, idx);

		/* Slide everything down */
		for (i = idx; i < max; i++)
		{
			object_type *j_ptr = flag_get_obj(inven, i + 1);
			flag_remove_zap(inven, i + 1);
			flag_set_obj(inven, i, j_ptr);
		}
	}

	vector_free(to_delete);
}

/*
 * Increment the cur_num field for artifacts present on a loaded level.
 * If cur_num was already 1 before (the player already found/got/sold the
 * artifact), the delete the artifact.
 *
 * This allow the generation of artifacts on multiple saved levels without
 * allowing the player to find them twice.
 */
void count_loaded_artifacts(void)
{
	s32b y, x;

	/* This code isn't needed (and would be harmful) without preserve mode */
	if (!p_ptr->preserve)
		return;

	/* Loop on all cave grids */
	for (y = 0; y < MAX_HGT; y++)
	{
		for (x = 0; x < MAX_WID; x++)
		{
			count_loaded_artifacts_inven(&cave[y][x].inventory);

			if (cave[y][x].m_idx) {
				monster_type *m_ptr = get_monster(cave[y][x].m_idx);
				
				count_loaded_artifacts_inven(&(m_ptr->inventory));
			}
		}
	}
}

/*
 * Generates a random dungeon level			-RAK-
 *
 * Hack -- regenerate any "overflow" levels
 *
 * Hack -- allow auto-scumming via a gameplay option.
 */
void generate_cave(void)
{
	dungeon_info_type *d_ptr = &d_info[dungeon_type];
	s32b tester_1, tester_2;
	s32b y, x, num, i;
	bool loaded = FALSE;
	s32b loaded_turn = 0;
	s16b town_level = 0;
	s32b old_seed_dungeon = seed_dungeon;

	/* The dungeon is not ready */
	character_dungeon = FALSE;
	generate_special_feeling = FALSE;

	/* Blank the feats. */
	/* Yes, this is really needed or otherwise the cavern code will break again. */
	for (y = 0; y < cur_hgt; ++y)
	{
		for (x = 0; x < cur_wid; ++x)
		{
			cave[y][x].feat = EMPTY_FEAT;
		}
	}

	flag_empty(&dungeon_flags->flags);
	/* Initialize the flags with the basic dungeon flags */
	if (!dun_level)
	{
		flag_copy(&dungeon_flags->flags, &d_info[DUNGEON_WILDERNESS].flags);
	}
	else
	{
		flag_copy(&dungeon_flags->flags, &d_ptr->flags);
	}
	get_level_flags();

	/* Fill the arrays of floors and walls in the good proportions */
	if (dun_level == 0)
		init_feat_info_wild();
	else
		init_feat_info();

	/* Is it a town level ? */
	for (i = 0; i < TOWN_DUNGEON; i++)
	{
		if (d_ptr->t_level[i] == dun_level) town_level = d_ptr->t_idx[i];
	}

	/* Save the imprinted monsters */
	save_all_friends();
	wipe_m_list();

	/* Seed the RNG if appropriate */
	if (seed_dungeon)
	{
		Rand_quick = TRUE;
		Rand_value = seed_dungeon + dun_level;
	}

	if (town_level)
	{
		Rand_quick = TRUE;
		seed_dungeon = town_info[town_level].seed;
		Rand_value = seed_dungeon;
	}

	/* Save wilderness per-grid flags */
	if (!p_ptr->wild_mode && p_ptr->old_wild_mode)
	{
		for (y = 0; y < max_wild_y; y++)
			for (x = 0; x < max_wild_x; x++)
				flag_copy(&wild_map[y][x].flags, &cave[y][x].flags);
	}

	process_hooks(HOOK_GEN_LEVEL_BEGIN, "");

	/* Try to load a saved level */
	if (has_flag(dungeon_flags, FLAG_SAVE_LEVEL) && (dun_level || !p_ptr->wild_mode))
	{
		/* No effects */
		for (i = 0; i < MAX_EFFECTS; i++)
		{
			effects[i].time = 0;
		}

		/* Start with a blank cave */
		for (y = 0; y < MAX_HGT; y++)
		{
			for (x = 0; x < MAX_WID; x++)
			{
				/* No flags */
				cave[y][x].info = 0;

				/* No features */
				cave_set_feat(y, x, FEAT_PERM_INNER);

				/* No flags */
				flag_empty(&cave[y][x].flags);

				/* No objects */
				preserve_artifacts_in_inven(&cave[y][x].inventory);
				flag_empty(&cave[y][x].inventory);

				/* Default size */
				flag_set(&cave[y][x].inventory, INVEN_LIMIT_KEY, 23);

				/* No monsters */
				cave[y][x].m_idx = 0;

				/* No traps */
				flag_empty(&cave[y][x].activations);

				/* No mimic */
				cave[y][x].mimic = 0;

				/* No effect */
				cave[y][x].effect = 0;

				/* No LOS info */
			}
		}

		loaded = load_dungeon(dungeon_type, dun_level, &loaded_turn);
		if (loaded) 
		{
			purge_dead_uniques();
			count_loaded_artifacts();
		}
	}

	process_hooks(HOOK_GEN_LEVEL_AFTER_LOAD, "bd", loaded, loaded_turn);

	/* No saved level -- generate new one */
	if (!loaded)
	{
		/* Generate */
		for (num = 0; TRUE; num++)
		{
			bool okay = TRUE;
			cptr why  = NULL;

			/* No effects */
			for (i = 0; i < MAX_EFFECTS; i++)
			{
				effects[i].time = 0;
			}

			/* Start with a blank cave */
			for (y = 0; y < MAX_HGT; y++)
			{
				for (x = 0; x < MAX_WID; x++)
				{
					/* No flags */
					cave[y][x].info = 0;

					/* No features */
					cave_set_feat(y, x, FEAT_PERM_INNER);

					/* No flags */
					flag_empty(&cave[y][x].flags);

					/* No objects */
					preserve_artifacts_in_inven(&cave[y][x].inventory);
					flag_empty(&cave[y][x].inventory);

					/* Default size */
					flag_set(&cave[y][x].inventory, INVEN_LIMIT_KEY, 23);

					/* No monsters */
					cave[y][x].m_idx = 0;

					/* No traps */
					flag_empty(&cave[y][x].activations);

					/* No mimic */
					cave[y][x].mimic = 0;

					/* No effect */
					cave[y][x].effect = 0;

					/* No LOS info */
				}
			}

			/* Mega-Hack -- no player yet */
			p_ptr->px = p_ptr->py = 0;


			/* Mega-Hack -- no panel yet */
			panel_row_min = 0;
			panel_row_max = 0;
			panel_col_min = 0;
			panel_col_max = 0;

			/* Reset the monster generation level */
			monster_level = dun_level;

			/* Reset the object generation level */
			object_level = dun_level;

			/* Nothing special here yet */
			good_item_flag = FALSE;

			/* Nothing good here yet */
			rating = 0;

			/* Requested size level */
			dungeon_set_level_size(d_ptr->size_y * SCREEN_HGT, d_ptr->size_x * SCREEN_WID);

			if (cheat_room)
			{
				msg_format("X:%d, Y:%d.", max_panel_cols, max_panel_rows);
			}

			/* Quest levels -KMW- */
			if (p_ptr->inside_quest)
			{
				quest_gen();
			}

			/* Special levels */
			else if (build_special_level())
			{
				/* nothing */
			}

			/* Build the town */
			else if (!dun_level)
			{
				/* Big wilderness mode */
				if (!p_ptr->wild_mode)
				{
					/* Make the wilderness */
					wilderness_gen(0);

					okay = TRUE;
				}

				/* Small wilderness mode */
				else
				{
					/* Make the wilderness */
					wilderness_gen_small();

					okay = TRUE;
				}
			}

			/* Build a dungeon level */
			else
			{
				/* Small level */
				if (!((has_flag(dungeon_flags, FLAG_BIG))) &&
				                (always_small_level ||
				                 ((has_flag(dungeon_flags, FLAG_SMALL))) ||
				                 (small_levels && rand_int(SMALL_LEVEL) == 0)))
				{
					if (cheat_room)
					{
						msg_print ("A 'small' dungeon level.");
					}

					tester_1 = rand_range(1, (cur_hgt / SCREEN_HGT));
					tester_2 = rand_range(1, (cur_wid / SCREEN_WID) - 1);

					dungeon_set_level_size(tester_1 * SCREEN_HGT, tester_2 * SCREEN_WID);

					if (cheat_room)
					{
						msg_format("X:%d, Y:%d.", max_panel_cols, max_panel_rows);
					}
				}

				/* Generate a level */
				if (!cave_gen())
				{
					why = "could not place player";
					okay = FALSE;
				}
			}

			/* Extract the feeling */
			if (rating > 100) feeling = 2;
			else if (rating > 80) feeling = 3;
			else if (rating > 60) feeling = 4;
			else if (rating > 40) feeling = 5;
			else if (rating > 30) feeling = 6;
			else if (rating > 20) feeling = 7;
			else if (rating > 10) feeling = 8;
			else if (rating > 0) feeling = 9;
			else feeling = 10;

			/* Hack -- Have a special feeling sometimes */
			if (good_item_flag && !p_ptr->preserve) feeling = 1;

			/* It takes 1000 game turns for "feelings" to recharge */
			if ((turn - old_turn) < 1000) feeling = 0;

			/* Hack -- no feeling in the town */
			if (!dun_level) feeling = 0;

			/* Mega-Hack -- "auto-scum" */
			if (auto_scum && (num < 100) && !p_ptr->inside_quest && dun_level)
			{
				/* Require "goodness" */
				if ((feeling > 9) ||
				                ((dun_level >= 5) && (feeling > 8)) ||
				                ((dun_level >= 10) && (feeling > 7)) ||
				                ((dun_level >= 20) && (feeling > 6)) ||
				                ((dun_level >= 40) && (feeling > 5)))
				{
					/* Give message to cheaters */
					if (cheat_room || cheat_hear ||
					    cheat_peek || cheat_xtra ||
					    p_ptr->precognition || wizard)
						/* Message */
						why = "boring level";

					/* Try again */
					okay = FALSE;
				}
			}
			/* Accept */
			if (okay || town_level) break;

			/* Message */
			if (why) msg_format("Generation restarted (%s)", why);

			/* Wipe the monsters */
			wipe_m_list();

			/*
			 * Mega-Hack -- Reset special level flag if necessary
			 * XXX XXX XXX
			 */
			wipe_special_level();
		}

		/* Give some mana to each grid -- DG */
		generate_grid_mana();
	}

	/* Restore wilderness per-grid flags */
	if (p_ptr->wild_mode && !p_ptr->old_wild_mode)
	{
		for (y = 0; y < max_wild_y; y++)
			for (x = 0; x < max_wild_x; x++)
				flag_copy(&cave[y][x].flags, &wild_map[y][x].flags);
	}

	/* Put the kept monsters -- DG */
	if (!p_ptr->wild_mode) replace_all_friends();

	/* Set special level generated flag if applicable */
	finalise_special_level();

	/* While in town/wilderness and not in the overworld mode */
	if (has_flag(&d_info[dungeon_type], FLAG_SURFACE_LITE) &&
		!p_ptr->wild_mode)
	{
		if (is_daylight(turn))
		{
			if (dun_level == 0)
				lite_level(TRUE);
			else
				lite_level(FALSE);
		}
		else
			darken_level(FALSE);
	}

	/* Froce player position? */
	if (!p_ptr->wild_mode && dun_level == 0)
	{
		if (p_ptr->wild_force_side_y_ok)
		{
			if (p_ptr->wild_force_side_y == 1) p_ptr->py = cur_hgt - 2;
			else if (p_ptr->wild_force_side_y == 0) p_ptr->py = cur_hgt / 2;
			else if (p_ptr->wild_force_side_y == -1) p_ptr->py = 1;
		}
		if (p_ptr->wild_force_side_x_ok)
		{
			if (p_ptr->wild_force_side_x == 1) p_ptr->px = cur_wid - 2;
			else if (p_ptr->wild_force_side_x == 0) p_ptr->px = cur_wid / 2;
			else if (p_ptr->wild_force_side_x == -1) p_ptr->px = 1;
		}
	}
	if ((!p_ptr->wild_force_side_x_ok) && (!p_ptr->wild_force_side_y_ok) && (!p_ptr->wild_mode) && (p_ptr->old_wild_mode))
	{
		p_ptr->py = p_ptr->oldpy;
		p_ptr->px = p_ptr->oldpx;
	}
	p_ptr->wild_force_side_y_ok = FALSE;
	p_ptr->wild_force_side_x_ok = FALSE;

	/* No teleporatations yet */
	last_teleportation_y = -1;
	last_teleportation_x = -1;

	/* Mark the dungeon town as found */
	if (town_level)
	{
		/* Set the known flag */
		town_info[town_level].flags |= (TOWN_KNOWN);
	}

	/* The dungeon is ready */
	character_dungeon = TRUE;

	/* Remember when this level was "created" */
	old_turn = turn;

	if (seed_dungeon)
	{
		Rand_quick = FALSE;

		seed_dungeon = old_seed_dungeon;
	}

	/* Player should get the first move upon entering the dungeon */
	p_ptr->energy = 100;

	/* Redraw */
	for (y = 0; y < cur_hgt; y++)
		for (x = 0; x < cur_wid; x++)
			cave[y][x].info &= ~(CAVE_VIEW | CAVE_SEEN | CAVE_PLIT | CAVE_MLIT);
	p_ptr->update |= PU_VIEW | PU_DISTANCE | PU_MONSTERS | PU_MON_LITE;
	verify_panel();
	panel_bounds();
	update_stuff();
	/*
	 * Flush the input buffer on next inkey() so that if the player pressed keys while generating the level
	 * he/she wont die a stupid death
	 */
	inkey_xtra = TRUE;
}
