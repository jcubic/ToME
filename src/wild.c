/* File: generate.c */

/* Purpose: Wilderness & Town related things */

/*
 * Copyright (c) 2001 James E. Wilson, Robert A. Koeneke, DarkGod
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"



/*
 * Various defines for the wilderness
 */
#define DUN_WILD_VAULT          50      /* Chance of finding a wilderness vault. */

/*
 * Various defines for the towns
 */
#define TOWN_BORDER             90


/*
 * Helper for plasma generation.
 */
static void perturb_point_mid(s32b x1, s32b x2, s32b x3, s32b x4,
                              s32b xmid, s32b ymid, s32b rough, s32b depth_max)
{
	/*
	 * Average the four corners & perturb it a bit.
	 * tmp is a random s32b +/- rough
	 */
	s32b tmp2 = rough * 2 + 1;
	s32b tmp = randint(tmp2) - (rough + 1);

	s32b avg = ((x1 + x2 + x3 + x4) / 4) + tmp;

	/* Division always rounds down, so we round up again */
	if (((x1 + x2 + x3 + x4) % 4) > 1) avg++;

	/* Normalize */
	if (avg < 0) avg = 0;
	if (avg > depth_max) avg = depth_max;

	/* Set the new value. */
	cave[ymid][xmid].feat = avg;
}


static void perturb_point_end(s32b x1, s32b x2, s32b x3,
                              s32b xmid, s32b ymid, s32b rough, s32b depth_max)
{
	/*
	 * Average the three corners & perturb it a bit.
	 * tmp is a random s32b +/- rough
	 */
	s32b tmp2 = rough * 2 + 1;
	s32b tmp = randint(tmp2) - (rough + 1);

	s32b avg = ((x1 + x2 + x3) / 3) + tmp;

	/* Division always rounds down, so we round up again */
	if ((x1 + x2 + x3) % 3) avg++;

	/* Normalize */
	if (avg < 0) avg = 0;
	if (avg > depth_max) avg = depth_max;

	/* Set the new value. */
	cave[ymid][xmid].feat = avg;
}


/*
 * A generic function to generate the plasma fractal.
 * Note that it uses ``cave_feat'' as temporary storage.
 * The values in ``cave_feat'' after this function
 * are NOT actual features; They are raw heights which
 * need to be converted to features.
 *
 * So we shouldn't call cave_set_feat in the helper functions
 * above.
 */
static void plasma_recursive(s32b x1, s32b y1, s32b x2, s32b y2,
                             s32b depth_max, s32b rough)
{
	/* Find middle */
	s32b xmid = (x2 - x1) / 2 + x1;
	s32b ymid = (y2 - y1) / 2 + y1;

	/* Are we done? */
	if (x1 + 1 == x2) return;

	perturb_point_mid(cave[y1][x1].feat, cave[y2][x1].feat, cave[y1][x2].feat,
	                  cave[y2][x2].feat, xmid, ymid, rough, depth_max);

	perturb_point_end(cave[y1][x1].feat, cave[y1][x2].feat, cave[ymid][xmid].feat,
	                  xmid, y1, rough, depth_max);

	perturb_point_end(cave[y1][x2].feat, cave[y2][x2].feat, cave[ymid][xmid].feat,
	                  x2, ymid, rough, depth_max);

	perturb_point_end(cave[y2][x2].feat, cave[y2][x1].feat, cave[ymid][xmid].feat,
	                  xmid, y2, rough, depth_max);

	perturb_point_end(cave[y2][x1].feat, cave[y1][x1].feat, cave[ymid][xmid].feat,
	                  x1, ymid, rough, depth_max);


	/* Recurse the four quadrants */
	plasma_recursive(x1, y1, xmid, ymid, depth_max, rough);
	plasma_recursive(xmid, y1, x2, ymid, depth_max, rough);
	plasma_recursive(x1, ymid, xmid, y2, depth_max, rough);
	plasma_recursive(xmid, ymid, x2, y2, depth_max, rough);
}


/*
 * Load a town or generate a terrain level using "plasma" fractals.
 *
 * x and y are the coordinates of the area in the wilderness.
 * Border and corner are optimization flags to speed up the
 * generation of the fractal terrain.
 * If border is set then only the border of the terrain should
 * be generated (for initializing the border structure).
 * If corner is set then only the corners of the area are needed.
 *
 * Return the number of floor grids
 */
s32b generate_area(s32b y, s32b x, bool border, bool corner, bool refresh)
{
	s32b road, entrance;
	s32b x1, y1;
	s32b hack_floor = 0;

	/* Number of the town (if any) */
	p_ptr->town_num = wf_info[wild_map[y][x].feat].entrance;
	if (!p_ptr->town_num) p_ptr->town_num = wild_map[y][x].entrance;

	if (process_hooks(HOOK_WILD_LARGE_CUSTOM_GENERATOR, "(d,d,b,b,b)", y, x, border, corner, refresh)) return (hack_floor);

	{
		s32b roughness = 1;  /* The roughness of the level. */
		s32b terrain[3][3];  /* The terrain around the current area */
		s32b ym, xm, yp, xp;

		/* Place the player at the center */
		if (!p_ptr->oldpx) p_ptr->oldpx = cur_wid / 2;
		if (!p_ptr->oldpy) p_ptr->oldpy = cur_hgt / 2;

		/* Initialize the terrain array */
		ym = ((y - 1) < 0) ? 0 : (y - 1);
		xm = ((x - 1) < 0) ? 0 : (x - 1);
		yp = ((y + 1) >= max_wild_y) ? (max_wild_y - 1) : (y + 1);
		xp = ((x + 1) >= max_wild_x) ? (max_wild_x - 1) : (x + 1);
		terrain[0][0] = wild_map[ym][xm].feat;
		terrain[0][1] = wild_map[ym][x].feat;
		terrain[0][2] = wild_map[ym][xp].feat;
		terrain[1][0] = wild_map[y][xm].feat;
		terrain[1][1] = wild_map[y][x].feat;
		terrain[1][2] = wild_map[y][xp].feat;
		terrain[2][0] = wild_map[yp][xm].feat;
		terrain[2][1] = wild_map[yp][x].feat;
		terrain[2][2] = wild_map[yp][xp].feat;

		/* Hack -- Use the "simple" RNG */
		Rand_quick = TRUE;

		/* Hack -- Induce consistant town layout */
		Rand_value = wild_map[y][x].seed;

		/* DG: This seems to crash, it is needed even if onyl getting corners, as the next loop doesnt care if (!corner) */
		{
			/* Create level background */
			for (y1 = 0; y1 < MAX_HGT; y1++)
			{
				for (x1 = 0; x1 < MAX_WID; x1++)
				{
					cave_set_feat(y1, x1, MAX_WILD_TERRAIN / 2);
				}
			}
		}

		/*
		 * Initialize the four corners
		 * ToDo: calculate the medium height of the adjacent
		 * terrains for every corner.
		 */
		cave_set_feat(((wild_see_through) ? 1 : 0), ((wild_see_through) ? 1 : 0), (byte)rand_int(MAX_WILD_TERRAIN));
		cave_set_feat(cur_hgt - 1 - ((wild_see_through) ? 1 : 0), ((wild_see_through) ? 1 : 0), (byte)rand_int(MAX_WILD_TERRAIN));
		cave_set_feat(((wild_see_through) ? 1 : 0), cur_wid - 1 - ((wild_see_through) ? 1 : 0), (byte)rand_int(MAX_WILD_TERRAIN));
		cave_set_feat(cur_hgt - 1 - ((wild_see_through) ? 1 : 0), cur_wid - 1 - ((wild_see_through) ? 1 : 0), (byte)rand_int(MAX_WILD_TERRAIN));

		if (!corner)
		{
			/* x1, y1, x2, y2, num_depths, roughness */
			plasma_recursive(((wild_see_through) ? 1 : 0), ((wild_see_through) ? 1 : 0), cur_wid - 1 - ((wild_see_through) ? 1 : 0), cur_hgt - 1 - ((wild_see_through) ? 1 : 0), MAX_WILD_TERRAIN - 1, roughness);
		}

		/* Use the complex RNG */
		Rand_quick = FALSE;

		for (y1 = ((wild_see_through) ? 1 : 0); y1 < cur_hgt - ((wild_see_through) ? 1 : 0); y1++)
		{
			for (x1 = ((wild_see_through) ? 1 : 0); x1 < cur_wid - ((wild_see_through) ? 1 : 0); x1++)
			{
				cave_set_feat(y1, x1, wf_info[terrain[1][1]].terrain[cave[y1][x1].feat]);
			}
		}

	}

	/* Should we create a town ? */
	if ((p_ptr->town_num > 0) && (p_ptr->town_num < 1000))
	{
		call_lua("load_map", "(s,d,d,b)", "", "towns/towns.lua", 0, 0, border || corner);
	}
	else
	{
		/* Reset the town flag */
		p_ptr->town_num = 0;
	}

	if (!corner)
	{
		/*
		 * Place roads in the wilderness
		 * ToDo: make the road a bit more interresting
		 */
		road = wf_info[wild_map[y][x].feat].road;

		if (road & ROAD_NORTH)
		{
			/* North road */
			for (y1 = ((wild_see_through) ? 1 : 0); y1 < cur_hgt / 2; y1++)
			{
				x1 = cur_wid / 2;
				place_floor(y1, x1);
			}
		}

		if (road & ROAD_SOUTH)
		{
			/* North road */
			for (y1 = cur_hgt / 2; y1 < cur_hgt - ((wild_see_through) ? 1 : 0); y1++)
			{
				x1 = cur_wid / 2;
				place_floor(y1, x1);
			}
		}

		if (road & ROAD_EAST)
		{
			/* East road */
			for (x1 = cur_wid / 2; x1 < cur_wid - ((wild_see_through) ? 1 : 0); x1++)
			{
				y1 = cur_hgt / 2;
				place_floor(y1, x1);
			}
		}

		if (road & ROAD_WEST)
		{
			/* West road */
			for (x1 = ((wild_see_through) ? 1 : 0); x1 < cur_wid / 2; x1++)
			{
				y1 = cur_hgt / 2;
				place_floor(y1, x1);
			}
		}
	}

	/* Hack -- Use the "simple" RNG */
	Rand_quick = TRUE;

	/* Hack -- Induce consistant town layout */
	Rand_value = wild_map[y][x].seed;

	entrance = wf_info[wild_map[y][x].feat].entrance;
	if (!entrance) entrance = wild_map[y][x].entrance;

	/* Create the dungeon if requested on the map */
	if (entrance >= 1000)
	{
		s32b dy, dx;

		dy = rand_range(6, cur_hgt - 6);
		dx = rand_range(6, cur_wid - 6);

		cave_set_feat(dy, dx, FEAT_MORE);
		set_flag(&cave[dy][dx], FLAG_DUNGEON, entrance - 1000);
		cave[dy][dx].info |= (CAVE_GLOW | CAVE_MARK);
	}

	/* Use the complex RNG */
	Rand_quick = FALSE;

	/* MEGA HACK -- set at least one floor grid */
	for (y1 = ((wild_see_through) ? 1 : 0); y1 < cur_hgt - ((wild_see_through) ? 1 : 0); y1++)
	{
		for (x1 = ((wild_see_through) ? 1 : 0); x1 < cur_wid - ((wild_see_through) ? 1 : 0); x1++)
		{
			if (cave_floor_bold(y1, x1)) hack_floor++;
		}
	}

	/* NO floor ? put one */
	if (!hack_floor)
	{
		/* Look for the first floor type we can find and use it */
		s32b i;
		for (i = 0; i < max_f_idx; i++) if (has_flag(&f_info[i], FLAG_FLOOR)) break;

		cave_set_feat(cur_hgt / 2, cur_wid / 2, i);
		hack_floor = 1;
	}

	/* Set the monster generation level to the wilderness level */
	monster_level = wf_info[wild_map[y][x].feat].level;

	/* Set the object generation level to the wilderness level */
	object_level = wf_info[wild_map[y][x].feat].level;

	process_hooks(HOOK_WILD_LARGE_TWEAK_GENERATOR, "(d,d,b,b,b)", y, x, border, corner, refresh);

	return hack_floor;
}

/*
 * Border of the wilderness area
 */
static border_type border;

/*
 * Build the wilderness area outside of the town.
 * -KMW-
 */
void wilderness_gen(s32b refresh)
{
	s32b i, y, x, hack_floor;
	bool daytime;
	cave_type *c_ptr;

	/* Init the wilderness */
	call_lua("load_map", "(s)", "", "wild/world.map");

	x = p_ptr->wilderness_x;
	y = p_ptr->wilderness_y;

	/* Set the correct monster hook */
	set_mon_num_hook();

	/* Prepare allocation table */
	get_mon_num_prep();

	if (wild_see_through)
	{
		/* North border */
		generate_area(y - 1, x, TRUE, FALSE, refresh);

		for (i = 1; i < cur_wid - 1; i++)
		{
			border.north[i] = cave[cur_hgt - 2][i].feat;
		}

		/* South border */
		generate_area(y + 1, x, TRUE, FALSE, refresh);

		for (i = 1; i < cur_wid - 1; i++)
		{
			border.south[i] = cave[1][i].feat;
		}

		/* West border */
		generate_area(y, x - 1, TRUE, FALSE, refresh);

		for (i = 1; i < cur_hgt - 1; i++)
		{
			border.west[i] = cave[i][cur_wid - 2].feat;
		}

		/* East border */
		generate_area(y, x + 1, TRUE, FALSE, refresh);

		for (i = 1; i < cur_hgt - 1; i++)
		{
			border.east[i] = cave[i][1].feat;
		}

		/* North west corner */
		generate_area(y - 1, x - 1, FALSE, TRUE, refresh);
		border.north_west = cave[cur_hgt - 2][cur_wid - 2].feat;

		/* North east corner */
		generate_area(y - 1, x + 1, FALSE, TRUE, refresh);
		border.north_east = cave[cur_hgt - 2][1].feat;

		/* South west corner */
		generate_area(y + 1, x - 1, FALSE, TRUE, refresh);
		border.south_west = cave[1][cur_wid - 2].feat;

		/* South east corner */
		generate_area(y + 1, x + 1, FALSE, TRUE, refresh);
		border.south_east = cave[1][1].feat;
	}

	/* Create terrain of the current area */
	hack_floor = generate_area(y, x, FALSE, FALSE, refresh);


	/* Special boundary walls -- North */
	for (i = 0; i < cur_wid; i++)
	{
		cave[0][i].mimic = (wild_see_through) ? border.north[i] : cave[0][i].feat;
		cave_set_feat(0, i, FEAT_PERM_SOLID);
	}

	/* Special boundary walls -- South */
	for (i = 0; i < cur_wid; i++)
	{
		cave[cur_hgt - 1][i].mimic = (wild_see_through) ? border.south[i] : cave[cur_hgt - 1][i].feat;
		cave_set_feat(cur_hgt - 1, i, FEAT_PERM_SOLID);
	}

	/* Special boundary walls -- West */
	for (i = 1; i < cur_hgt - 1; i++)
	{
		cave[i][0].mimic = (wild_see_through) ? border.west[i] : cave[i][0].feat;
		cave_set_feat(i, 0, FEAT_PERM_SOLID);
	}

	/* Special boundary walls -- East */
	for (i = 1; i < cur_hgt - 1; i++)
	{
		cave[i][cur_wid - 1].mimic = (wild_see_through) ? border.east[i] : cave[i][cur_wid - 1].feat;
		cave_set_feat(i, cur_wid - 1, FEAT_PERM_SOLID);
	}

	/* North west corner */
	cave[0][0].mimic = (wild_see_through) ? border.north_west : cave[0][0].mimic;

	/* Make sure it has correct CAVE_WALL flag set */
	cave_set_feat(0, 0, cave[0][0].feat);

	/* North east corner */
	cave[0][cur_wid - 1].mimic = (wild_see_through) ? border.north_east : cave[0][cur_wid - 1].mimic;

	/* Make sure it has correct CAVE_WALL flag set */
	cave_set_feat(0, cur_wid - 1, cave[0][cur_wid - 1].feat);

	/* South west corner */
	cave[cur_hgt - 1][0].mimic = (wild_see_through) ? border.south_west : cave[cur_hgt - 1][0].mimic;

	/* Make sure it has correct CAVE_WALL flag set */
	cave_set_feat(cur_hgt - 1, 0, cave[cur_hgt - 1][0].feat);

	/* South east corner */
	cave[cur_hgt - 1][cur_wid - 1].mimic = (wild_see_through) ? border.south_east : cave[cur_hgt - 1][cur_wid - 1].mimic;

	/* Make sure it has correct CAVE_WALL flag set */
	cave_set_feat(cur_hgt - 1, cur_wid - 1, cave[cur_hgt - 1][cur_wid - 1].feat);

	/* Day time */
	daytime = is_daylight(turn);

	/* Light up or darken the area */
	for (y = 0; y < cur_hgt; y++)
	{
		for (x = 0; x < cur_wid; x++)
		{
			/* Get the cave grid */
			c_ptr = &cave[y][x];

			if (daytime)
			{
				/* Assume lit */
				c_ptr->info |= (CAVE_GLOW);

				/* Hack -- Memorize lit grids if allowed */
				if (view_perma_grids) c_ptr->info |= (CAVE_MARK);
			}
			else
			{
				/* Darken "boring" features */
				if (!has_flag(&f_info[c_ptr->feat], FLAG_REMEMBER))
				{
					/* Forget the grid */
					c_ptr->info &= ~(CAVE_GLOW | CAVE_MARK);
				}
			}
		}
	}

	player_place(p_ptr->oldpy, p_ptr->oldpx);

	if (!refresh && !process_hooks(HOOK_WILD_GEN_MON, "(d)", hack_floor))
	{
		s32b lim = d_info[0].min_m_alloc_level;

		/*
		 * Can't have more monsters than floor grids -1(for the player,
		 * not needed but safer
		 */
		if (lim > hack_floor - 1) lim = hack_floor - 1;

		/* Make some residents */
		for (i = 0; i < lim; i++)
		{
			/* Make a resident */
			(void)alloc_monster(3, TRUE);
		}
	}

	process_hooks(HOOK_WILD_GEN, "(d)", FALSE);
}

/*
 * Build the wilderness area.
 * -DG-
 */
void wilderness_gen_small()
{
	s32b i, j, entrance;

	/* To prevent stupid things */
	for (i = 0; i < cur_wid; i++)
	{
		for (j = 0; j < cur_hgt; j++)
		{
			cave_set_feat(j, i, FEAT_WORLD_BORDER);
		}
	}

	/* Init the wilderness */
	call_lua("load_map", "(s)", "", "wild/world.map");

	/* Fill the map */
	for (i = 0; i < max_wild_x; i++)
	{
		for (j = 0; j < max_wild_y; j++)
		{
			entrance = wf_info[wild_map[j][i].feat].entrance;
			if (!entrance) entrance = wild_map[j][i].entrance;

			if (wild_map[j][i].entrance)
			{
				cave_set_feat(j, i, FEAT_MORE);
				if (wild_map[j][i].mimic) cave[j][i].mimic = wild_map[j][i].mimic;
			}
			else
			{
				cave_set_feat(j, i, wf_info[wild_map[j][i].feat].feat);
			}

			if ((cave[j][i].feat == FEAT_MORE) && (entrance >= 1000))
			{
				/* The flag will later get copied over to cave[j][i] */
				set_flag(&wild_map[j][i], FLAG_DUNGEON, entrance - 1000);
			}

			/* Show it if we know it */
			if (wild_map[j][i].known)
			{
				cave[j][i].info |= (CAVE_GLOW | CAVE_MARK);
			}
		}
	}

	/* Place the player */
	p_ptr->px = p_ptr->wilderness_x;
	p_ptr->py = p_ptr->wilderness_y;

	process_hooks(HOOK_WILD_GEN, "(d)", TRUE);
}

/* Show a small radius of wilderness around the player */
void reveal_wilderness_around_player(s32b y, s32b x, s32b h, s32b w)
{
	s32b i, j;

	/* Circle or square ? */
	if (h == 0)
	{
		for (i = x - w; i < x + w; i++)
		{
			for (j = y - w; j < y + w; j++)
			{
				/* Bound checking */
				if (!in_bounds(j, i)) continue;

				/* Severe bound checking */
				if ((i < 0) || (i >= max_wild_x) || (j < 0) || (j >= max_wild_y)) continue;

				/* We want a radius, not a "squarus" :) */
				if (distance(y, x, j, i) >= w) continue;

				/* New we know here */
				wild_map[j][i].known = TRUE;

				/* Only if we are in overview */
				if (p_ptr->wild_mode)
				{
					cave[j][i].info |= (CAVE_GLOW | CAVE_MARK);

					/* Show it */
					lite_spot(j, i);
				}
			}
		}
	}
	else
	{
		for (i = x; i < x + w; i++)
		{
			for (j = y; j < y + h; j++)
			{
				/* Bound checking */
				if (!in_bounds(j, i)) continue;

				/* New we know here */
				wild_map[j][i].known = TRUE;

				/* Only if we are in overview */
				if (p_ptr->wild_mode)
				{
					cave[j][i].info |= (CAVE_GLOW | CAVE_MARK);

					/* Show it */
					lite_spot(j, i);
				}
			}
		}
	}
}

/*
 * Builds a store at a given pseudo-location
 *
 * As of 2.8.1 (?) the town is actually centered in the middle of a
 * complete level, and thus the top left corner of the town itself
 * is no longer at (0,0), but rather, at (qy,qx), so the constants
 * in the comments below should be mentally modified accordingly.
 *
 * As of 2.7.4 (?) the stores are placed in a more "user friendly"
 * configuration, such that the four "center" buildings always
 * have at least four grids between them, to allow easy running,
 * and the store doors tend to face the middle of town.
 *
 * The stores now lie inside boxes from 3-9 and 12-18 vertically,
 * and from 7-17, 21-31, 35-45, 49-59.  Note that there are thus
 * always at least 2 open grids between any disconnected walls.
 *
 * Note the use of "town_illuminate()" to handle all "illumination"
 * and "memorization" issues.
 */
static void build_store(s32b qy, s32b qx, s32b n, s32b yy, s32b xx)
{
	s32b y, x, y0, x0, y1, x1, y2, x2, tmp;

	/* Find the "center" of the store */
	y0 = qy + yy * 9 + 6;
	x0 = qx + xx * 14 + 12;

	/* Determine the store boundaries */
	y1 = y0 - randint((yy == 0) ? 3 : 2);
	y2 = y0 + randint((yy == 1) ? 3 : 2);
	x1 = x0 - randint(5);
	x2 = x0 + randint(5);

	/* Build an invulnerable rectangular building */
	for (y = y1; y <= y2; y++)
	{
		for (x = x1; x <= x2; x++)
		{
			/* Create the building */
			cave_set_feat(y, x, FEAT_PERM_SOLID);
		}
	}

	/* Pick a door direction (S,N,E,W) */
	tmp = rand_int(4);

	/* Re-roll "annoying" doors */
	if (((tmp == 0) && (yy == 1)) ||
	                ((tmp == 1) && (yy == 0)) ||
	                ((tmp == 2) && (xx == 3)) ||
	                ((tmp == 3) && (xx == 0)))
	{
		/* Pick a new direction */
		tmp = rand_int(4);
	}

	/* Extract a "door location" */
	switch (tmp)
	{
		/* Bottom side */
	case 0:
		{
			y = y2;
			x = rand_range(x1, x2);
			break;
		}

		/* Top side */
	case 1:
		{
			y = y1;
			x = rand_range(x1, x2);
			break;
		}

		/* Right side */
	case 2:
		{
			y = rand_range(y1, y2);
			x = x2;
			break;
		}

		/* Left side */
	default:
		{
			y = rand_range(y1, y2);
			x = x1;
			break;
		}
	}

	/* Clear previous contents, add a store door */
	cave_set_feat(y, x, FEAT_SHOP);
	set_flag(&cave[y][x], FLAG_CONTAINS_BUILDING, n);
	cave[y][x].info |= CAVE_FREE;
}

static void build_store_circle(s32b qy, s32b qx, s32b n, s32b yy, s32b xx)
{
	s32b tmp, y, x, y0, x0, rad = 2 + rand_int(2);

	/* Find the "center" of the store */
	y0 = qy + yy * 9 + 6;
	x0 = qx + xx * 14 + 12;

	/* Determine the store boundaries */

	/* Build an invulnerable circular building */
	for (y = y0 - rad; y <= y0 + rad; y++)
	{
		for (x = x0 - rad; x <= x0 + rad; x++)
		{
			if (distance(y0, x0, y, x) > rad) continue;

			/* Create the building */
			cave_set_feat(y, x, FEAT_PERM_SOLID);
		}
	}

	/* Pick a door direction (S,N,E,W) */
	tmp = rand_int(4);

	/* Re-roll "annoying" doors */
	if (((tmp == 0) && (yy == 1)) ||
	                ((tmp == 1) && (yy == 0)) ||
	                ((tmp == 2) && (xx == 3)) ||
	                ((tmp == 3) && (xx == 0)))
	{
		/* Pick a new direction */
		tmp = rand_int(4);
	}

	/* Extract a "door location" */
	switch (tmp)
	{
		/* Bottom side */
	case 0:
		{
			for (y = y0; y <= y0 + rad; y++)
				place_floor(y, x0);
			break;
		}

		/* Top side */
	case 1:
		{
			for (y = y0 - rad; y <= y0; y++)
				place_floor(y, x0);
			break;
		}

		/* Right side */
	case 2:
		{
			for (x = x0; x <= x0 + rad; x++)
				place_floor(y0, x);
			break;
		}

		/* Left side */
	default:
		{
			for (x = x0 - rad; x <= x0; x++)
				place_floor(y0, x);
			break;
		}
	}

	/* Clear previous contents, add a store door */
	cave_set_feat(y0, x0, FEAT_SHOP);
	set_flag(&cave[y0][x0], FLAG_CONTAINS_BUILDING, n);
	cave[y0][x0].info |= CAVE_FREE;
}

static void build_store_hidden(s32b n, s32b yy, s32b xx)
{
	/* Clear previous contents, add a store door */
	cave_set_feat(yy, xx, FEAT_SHOP);
	set_flag(&cave[yy][xx], FLAG_CONTAINS_BUILDING, n);
	cave[yy][xx].info |= CAVE_FREE;
}

/* Return a list of stores */
static s32b get_shops(s32b *rooms)
{
	s32b n, num = 0;

	for (n = 0; n < max_st_idx; n++)
	{
		rooms[n] = 0;
	}

	for (n = 0; n < max_st_idx; n++)
	{
		s32b chance = 50;

		if (has_flag(&st_info[n], FLAG_COMMON)) chance += 30;
		if (has_flag(&st_info[n], FLAG_RARE)) chance -= 20;
		if (has_flag(&st_info[n], FLAG_VERY_RARE)) chance -= 30;

		if (!magik(chance)) continue;

		if (has_flag(&st_info[n], FLAG_RANDOM)) rooms[num++] = n;
	}

	return num;
}

/* Generate town borders */
static void set_border(s32b y, s32b x)
{
	cave_type *c_ptr;

	/* Paranoia */
	if (!in_bounds(y, x)) return;

	/* Was a floor */
	if (cave_floor_bold(y, x) ||
	                has_flag(&f_info[cave[y][x].feat], FLAG_DOOR))
	{
		cave_set_feat(y, x, FEAT_DOOR);
	}

	/* Was a wall */
	else
	{
		cave_set_feat(y, x, FEAT_PERM_SOLID);

	}

	/* Access grid */
	c_ptr = &cave[y][x];

	/* Clear special attrs */
	c_ptr->mimic = 0;
	c_ptr->info |= (CAVE_ROOM);
}


static void town_borders(s32b t_idx, s32b qy, s32b qx)
{
	s32b y, x;

	x = qx;
	for (y = qy; y < qy + SCREEN_HGT - 1; y++)
	{
		set_border(y, x);
	}

	x = qx + SCREEN_WID - 1;
	for (y = qy; y < qy + SCREEN_HGT - 1; y++)
	{
		set_border(y, x);
	}

	y = qy;
	for (x = qx; x < qx + SCREEN_WID - 1; x++)
	{
		set_border(y, x);
	}

	y = qy + SCREEN_HGT - 1;
	for (x = qx; x < qx + SCREEN_WID; x++)
	{
		set_border(y, x);
	}
}

static bool create_townpeople_hook(s32b r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (r_ptr->d_char == 't') return TRUE;
	else return FALSE;
}


/*
 * Generate the "consistent" town features, and place the player
 *
 * Hack -- play with the R.N.G. to always yield the same town
 * layout, including the size and shape of the buildings, the
 * locations of the doorways, and the location of the stairs.
 */
static void town_gen_hack(s32b t_idx, s32b qy, s32b qx)
{
	s32b y, x, num = 0;
	bool (*old_get_mon_num_hook)(s32b r_idx);

	s32b *rooms;

	/* Place some floors */
	for (y = qy + 1; y < qy + SCREEN_HGT - 1; y++)
	{
		for (x = qx + 1; x < qx + SCREEN_WID - 1; x++)
		{
			/* Create empty floor */
			cave_set_feat(y, x, floor_type[rand_int(100)]);
			cave[y][x].info |= (CAVE_ROOM | CAVE_FREE);
		}
	}

	/* Prepare an Array of "remaining stores", and count them */
	C_MAKE(rooms, max_st_idx, s32b);
	num = get_shops(rooms);

	/* Place two rows of stores */
	for (y = 0; y < 2; y++)
	{
		/* Place four stores per row */
		for (x = 0; x < 4; x++)
		{
			if(--num > -1)
			{
				/* Build that store at the proper location */
				build_store(qy, qx, rooms[num], y, x);
			}
		}
	}
	C_FREE(rooms, max_st_idx, s32b);

	/* Generates the town's borders */
	if (magik(TOWN_BORDER)) town_borders(t_idx, qy, qx);


	/* Some inhabitants(leveled .. hehe :) */

	/* Backup the old hook */
	old_get_mon_num_hook = get_mon_num_hook;

	/* Require "okay" monsters */
	get_mon_num_hook = create_townpeople_hook;

	/* Prepare allocation table */
	get_mon_num_prep();

	for (x = qx; x < qx + SCREEN_WID; x++)
		for (y = qy; y < qy + SCREEN_HGT; y++)
		{
			s32b m_idx, r_idx;

			/* Only in town */
			if (!in_bounds(y, x)) continue;
			if (!(cave[y][x].info & CAVE_FREE)) continue;
			if (!cave_empty_bold(y, x)) continue;

			if (rand_int(100)) continue;

			r_idx = get_mon_num(0);
			m_allow_special[r_idx] = TRUE;
			m_idx = place_monster_one(y, x, r_idx, 0, TRUE, FACTION_TOWN);
			m_allow_special[r_idx] = FALSE;

			if (m_idx)
			{
				monster_type *m_ptr = get_monster(m_idx);
				if (m_ptr->level < (dun_level / 2))
				{
					m_ptr->exp = MONSTER_EXP(m_ptr->level + (dun_level / 2) + randint(dun_level / 2));
					monster_check_experience(m_idx, TRUE);
				}
			}
		}

	/* Reset restriction */
	get_mon_num_hook = old_get_mon_num_hook;

	/* Prepare allocation table */
	get_mon_num_prep();
}

static void town_gen_circle(s32b t_idx, s32b qy, s32b qx)
{
	s32b y, x, cy, cx, rad, num = 0;
	bool (*old_get_mon_num_hook)(s32b r_idx);

	s32b *rooms;

	rad = (SCREEN_HGT / 2);

	y = qy;
	for (x = qx + rad; x < qx + SCREEN_WID - rad; x++)
	{
		set_border(y, x);
	}

	y = qy + SCREEN_HGT - 1;
	for (x = qx + rad; x < qx + SCREEN_WID - rad; x++)
	{
		set_border(y, x);
	}
	/* Place some floors */
	for (y = qy + 1; y < qy + SCREEN_HGT - 1; y++)
	{
		for (x = qx + rad; x < qx + SCREEN_WID - rad; x++)
		{
			/* Create empty floor */
			cave_set_feat(y, x, floor_type[rand_int(100)]);
			cave[y][x].info |= CAVE_ROOM | CAVE_FREE;
		}
	}

	cy = qy + (SCREEN_HGT / 2);

	cx = qx + rad;
	for (y = cy - rad; y < cy + rad; y++)
		for (x = cx - rad; x < cx + 1; x++)
		{
			s32b d = distance(cy, cx, y, x);

			if ((d == rad) || (d == rad - 1)) set_border(y, x);

			if (d < rad - 1)
			{
				cave_set_feat(y, x, floor_type[rand_int(100)]);
				cave[y][x].info |= CAVE_ROOM | CAVE_FREE;
			}
		}

	cx = qx + SCREEN_WID - rad - 1;
	for (y = cy - rad; y < cy + rad; y++)
		for (x = cx; x < cx + rad + 1; x++)
		{
			s32b d = distance(cy, cx, y, x);

			if ((d == rad) || (d == rad - 1)) set_border(y, x);

			if (d < rad - 1)
			{
				cave_set_feat(y, x, floor_type[rand_int(100)]);
				cave[y][x].info |= CAVE_ROOM | CAVE_FREE;
			}
		}

	/* Prepare an Array of "remaining stores", and count them */
	C_MAKE(rooms, max_st_idx, s32b);
	num = get_shops(rooms);

	/* Place two rows of stores */
	for (y = 0; y < 2; y++)
	{
		/* Place four stores per row */
		for (x = 0; x < 4; x++)
		{
			if(--num > -1)
			{
				/* Build that store at the proper location */
				build_store_circle(qy, qx, rooms[num], y, x);
			}
		}
	}
	C_FREE(rooms, max_st_idx, s32b);

	/* Some inhabitants(leveled .. hehe :) */

	/* Backup the old hook */
	old_get_mon_num_hook = get_mon_num_hook;

	/* Require "okay" monsters */
	get_mon_num_hook = create_townpeople_hook;

	/* Prepare allocation table */
	get_mon_num_prep();

	for (x = qx; x < qx + SCREEN_WID; x++)
		for (y = qy; y < qy + SCREEN_HGT; y++)
		{
			s32b m_idx, r_idx;

			/* Only in town */
			if (!in_bounds(y, x)) continue;
			if (!(cave[y][x].info & CAVE_FREE)) continue;
			if (!cave_empty_bold(y, x)) continue;

			if (rand_int(100)) continue;

			r_idx = get_mon_num(0);
			m_allow_special[r_idx] = TRUE;
			m_idx = place_monster_one(y, x, r_idx, 0, TRUE, FACTION_TOWN);
			m_allow_special[r_idx] = FALSE;
			if (m_idx)
			{
				monster_type *m_ptr = get_monster(m_idx);
				if (m_ptr->level < (dun_level / 2))
				{
					m_ptr->exp = MONSTER_EXP(m_ptr->level + (dun_level / 2) + randint(dun_level / 2));
					monster_check_experience(m_idx, TRUE);
				}
			}
		}

	/* Reset restriction */
	get_mon_num_hook = old_get_mon_num_hook;

	/* Prepare allocation table */
	get_mon_num_prep();
}


static void town_gen_hidden(s32b t_idx, s32b qy, s32b qx)
{
	s32b y, x, n, num = 0, i;

	s32b *rooms;

	/* Prepare an Array of "remaining stores", and count them */
	C_MAKE(rooms, max_st_idx, s32b);
	num = get_shops(rooms);

	/* Get a number of stores to place */
	n = rand_int(num / 2) + (num / 2);

	/* Place k stores */
	for (i = 0; i < n; i++)
	{
		/* Find a good spot */
		while (TRUE)
		{
			y = rand_range(1, cur_hgt - 2);
			x = rand_range(1, cur_wid - 2);

			if (cave_empty_bold(y, x)) break;
		}

		if(--num > -1)
		{
			/* Build that store at the proper location */
			build_store_hidden(rooms[num], y, x);
		}
	}
	C_FREE(rooms, max_st_idx, s32b);
}



/*
 * Town logic flow for generation of new town
 *
 * We start with a fully wiped cave of normal floors.
 *
 * Note that town_gen_hack() plays games with the R.N.G.
 *
 * This function does NOT do anything about the owners of the stores,
 * nor the contents thereof.  It only handles the physical layout.
 *
 * We place the player on the stairs at the same time we make them.
 *
 * Hack -- since the player always leaves the dungeon by the stairs,
 * he is always placed on the stairs, even if he left the dungeon via
 * word of recall or teleport level.
 */
void town_gen(s32b t_idx)
{
	s32b qy, qx;

	/* Did not tell us what a shop is, do not attempt anything */
	if (FEAT_SHOP == -1) return;

	/* Level too small to contain a town */
	if (cur_hgt < SCREEN_HGT) return;
	if (cur_wid < SCREEN_WID) return;

	/* Center fo the level */
	qy = (cur_hgt - SCREEN_HGT) / 2;
	qx = (cur_wid - SCREEN_WID) / 2;

	/* Build stuff */
	switch (rand_int(3))
	{
	case 0:
		{
			town_gen_hack(t_idx, qy, qx);
			if (wizard)
			{
				msg_format("Town level(normal) (%d, seed %d)",
				           t_idx, town_info[t_idx].seed);
			}
			break;
		}

	case 1:
		{
			town_gen_circle(t_idx, qy, qx);
			if (wizard)
			{
				msg_format("Town level(circle)(%d, seed %d)",
				           t_idx, town_info[t_idx].seed);
			}
			break;
		}

	case 2:
		{
			town_gen_hidden(t_idx, qy, qx);
			if (wizard)
			{
				msg_format("Town level(hidden)(%d, seed %d)",
				           t_idx, town_info[t_idx].seed);
			}
			break;
		}
	}

	p_ptr->town_num = t_idx;
}
