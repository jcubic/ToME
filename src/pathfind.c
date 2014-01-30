/* pathfind.c - simple path finder - public domain -- by Pekka Nurminen */
#include    "angband.h"

#define MAP_WIDTH MAX_WID
#define MAP_HEIGHT MAX_HGT
#define MAP_SIZE MAP_WIDTH*MAP_HEIGHT

#define ASTAR
//#define MAP_VARIYING_COST

/* States of path elements */
enum path_element_state
{
	path_element_empty,
	path_element_open,
	path_element_closed
};

/* Structure for path elements */
struct path_element
{
	s32b x_pos_;
	s32b y_pos_;
	s32b cost_;
	s32b estimate_;
	enum path_element_state state_;
	struct path_element* parent_;
};

/* Nodes - one per cell in map */
static struct path_element path_nodes[ MAP_SIZE ];

/* Open nodes */
static struct path_element* path_open[ MAP_SIZE ];

/* Top index */
static s32b path_open_top;

/* Calculates distance between two points */
static s32b path_distance( s32b x0, s32b y0, s32b x1, s32b y1 );
/* Checks if position is blocked */
static s32b path_blocked( s32b x, s32b y );
/* Initialises path structures */
static void path_init( void );
/* Maps coordinate to one dimensional array */
static s32b path_map( s32b x, s32b y );
/* Returns pointer to path element, or NULL if position is invalid */
static struct path_element* path_element_map( s32b x, s32b y );
/* Calculates cost from element to goal. */
static s32b path_cost( struct path_element* element, s32b goalx, s32b goaly );
/* Pushes element to open stack */
static void path_push_open( struct path_element* element, s32b goalx, s32b goaly );
/* Pops element from open stack */
static struct path_element* path_pop_open( void );
/* Removes element from stack */
static void path_remove( struct path_element* element );
/* Checks if there are elements in open stack */
static s32b path_open_not_empty( void );
/* Pushes position to open stack when needed */
static void path_check( struct path_element* parent,
	s32b posx, s32b posy, s32b goalx, s32b goaly );
/* Finds path from start towards goal */
static s32b path_find( s32b startx, s32b starty, s32b goalx, s32b goaly );
/* Gets next step towards goal. */
static s32b path_get_next( s32b* nextx, s32b* nexty );
/* Updates cost to pos which is adjacent to parent */
static void path_update_cost( struct path_element* parent, struct path_element*	pos );

static monster_type *__m_ptr = NULL;
static s32b cur_x, cur_y;

bool pathfind_get_first(monster_type *m_ptr, s32b start_y, s32b start_x, s32b goal_y, s32b goal_x, s32b *res_y, s32b *res_x, bool *complete)
{
	__m_ptr = m_ptr;
	cur_y = start_y;
	cur_x = start_x;
	if (path_find(cur_x, cur_y, goal_x, goal_y))
	{
		if (!path_get_next(&cur_x, &cur_y)) *complete = TRUE;
		else complete = FALSE;
		*res_y = start_y;
		*res_x = start_x;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

bool pathfind_get_next(s32b *res_y, s32b *res_x)
{
	s32b path = path_get_next(&cur_x, &cur_y);
	*res_y = cur_y;
	*res_x = cur_x;
	return (path == 0) ? TRUE : FALSE;
}

#if 0
void test_path(s32b start_y, s32b start_x, s32b goal_y, s32b goal_x)
{
	s32b path;
	s32b steps;

	/* Goal & start location are Ok, generate path */
	printf( "\tStart = (%d, %d), goal = (%d, %d)\n",
		start_x, start_y, goal_x, goal_y );

	path = path_find( start_x, start_y, goal_x, goal_y );
	if ( path )
	{
		/* Print out steps to goal. */
		steps = 0;
		while ( path )
		{
			s32b prev_x = start_x;
			s32b prev_y = start_y;

			path = path_get_next( &start_x, &start_y );

			if ( !steps )
			{
				printf( "\t(%2d, %2d) -> (%2d, %2d)",
					prev_x, prev_y, start_x, start_y );
			}
			else
			{
				if ( ( steps + 1 ) & 7 )
				{
					printf( " -> (%2d, %2d)", start_x, start_y );
				}
				else
				{
					printf( " ->\n\t(%2d, %2d)", start_x, start_y );
				}
			}

			steps++;
		}

		if ( ( start_x == goal_x ) &&
			( start_y == goal_y ) )
		{
			printf( "\n\tFound target!\n" );
		}
	}
	else
	{
		printf( "\tCould not find path\n" );
	}
}
#endif
/* Calculate distance between two points */
static s32b path_distance( s32b x0, s32b y0, s32b x1, s32b y1 )
{
	return distance(y0, x0, y1, x1);
}

/* Checks if position is blocked */
static s32b path_blocked( s32b x, s32b y )
{
	if (__m_ptr)
	{
		bool ok = TRUE;
		if (cave_feat_is(&cave[y][x],FLAG_DOOR))
		if ((__m_ptr->fy == y) && (__m_ptr->fx == x)) return FALSE;
		if (cave[y][x].m_idx && has_flag(__m_ptr, FLAG_MOVE_BODY))
		{
			monster_type *m2_ptr = get_monster(cave[y][x].m_idx);
			ok = !is_enemy(__m_ptr, m2_ptr);
		}
		if (ok && cave_feat_is(&cave[y][x], FLAG_DOOR) && (has_flag(__m_ptr, FLAG_OPEN_DOOR) || has_flag(__m_ptr, FLAG_BASH_DOOR)))
			return FALSE;
		return !(ok && monst_can_pass_square(__m_ptr, y, x, NULL));
	}
	return !cave_empty_bold(y, x);
}

/* Initialises path structures. */
static void path_init( void )
{
	s32b i, j;
	s32b p;

	/* Everything done here is not absolutely necessary. */
	memset( path_nodes, 0, MAP_SIZE * sizeof( struct path_element ) );
	memset( path_open, 0, MAP_SIZE * sizeof( struct path_element* ) );
	path_open_top = 0;

	p = 0;
	for ( j = 0; j < MAP_HEIGHT; j++ )
	{
		for ( i = 0; i < MAP_WIDTH; i++ )
		{
			path_nodes[ p ].x_pos_ = i;
			path_nodes[ p ].y_pos_ = j;
			p++;
		}
	}
}

/* Maps coordinates to one dimensional array indices. */
static s32b path_map( s32b x, s32b y )
{
	return y * MAP_WIDTH + x;
}

/* Returns pointer to path element structure or NULL if invalid position. */
static struct path_element* path_element_map( s32b x, s32b y )
{
	struct path_element* result = NULL;

	if ( ( x >= 1 && x < cur_wid - 2 ) &&
		( y >= 1 && y < cur_hgt - 2 ) )
	{
		result = &path_nodes[ path_map( x, y ) ];
	}

	return result;
}

/* Returns cost from position defined by element to goal.*/
static s32b path_cost( struct path_element* element, s32b goalx, s32b goaly )
{
#ifdef ASTAR
	if ( !element->estimate_ )
	{
		element->estimate_ =
		path_distance( element->x_pos_, element->y_pos_, goalx, goaly );
	}
#endif

	return element->cost_ + element->estimate_;
}

/* Pushes pointer to one element to the open stack */
static void path_push_open( struct path_element* element, s32b goalx, s32b goaly )
{
	s32b i;

	path_open[ path_open_top ] = element;
	path_open_top++;

	/* Keep elements in ascending order by distance to goal
	 - we want to find one of the shortest paths */
	for ( i = path_open_top - 1; i >= 1; i-- )
	{
		struct path_element* current = path_open[ i ];
		struct path_element* next = path_open[ i - 1 ];

#if 0
		if ( path_distance( next->x_pos_, next->y_pos_, goalx, goaly ) <
			path_distance( current->x_pos_, current->y_pos_, goalx,
				goaly ) )
#else
			s32b next_total = path_cost( next, goalx, goaly );
		s32b current_total = path_cost( current, goalx, goaly );
		if ( next_total < current_total )
		{
#endif
			/* Swap pointers */
			path_open[ i ] = next;
			path_open[ i - 1 ] = current;
		}
	}
}

/* Pops one element from the open stack */
static struct path_element* path_pop_open( void )
{
	path_open_top--;
	struct path_element* result = path_open[ path_open_top ];
	path_open[ path_open_top ] = NULL;
	return result;
}

/* Removes element from stack */
static void path_remove( struct path_element* element )
{
	s32b i;
	for ( i = 0; i < path_open_top; i++ )
	{
		if ( element == path_open[ i ] )
		{
			break;
		}
	}

	if ( i != path_open_top )
	{
		/* Element was found. */
		path_open[ i ] = NULL;
		for ( ; i < ( path_open_top - 1 ); i++ )
		{
			path_open[ i ] = path_open[ i + 1 ];
		}

		path_open_top--;
	}
}

/* Checks that open stack is not empty. */
static s32b path_open_not_empty( void )
{
	return path_open_top;
}

/* Pushes to open stack unless already open, closed or impassable */
static void path_check(
	struct path_element* parent,
	s32b posx, s32b posy, s32b goalx, s32b goaly )
{
	struct path_element* pos = path_element_map( posx, posy );
	if ( pos )
	{
		/* We can ignore blocked positions (consider that cost is
		 infinite).*/
		if ( !path_blocked( posx, posy ) )
		{
			/* If not processed yet, add to open set */
			if ( pos->state_ == path_element_empty )
			{
				pos->state_ = path_element_open;
				pos->parent_ = parent;
				path_update_cost( parent, pos );

				path_push_open( pos, goalx, goaly );
			}
			else
			{
				/* Now element is either in open or closed set. */
				const s32b orig_cost = path_cost( pos, goalx, goaly );
				struct path_element tmp = *pos;
				path_update_cost( parent, &tmp );

				if ( orig_cost > path_cost( &tmp, goalx, goaly ) )
				{
					/* New path is better than old. */
					s32b was_open = ( pos->state_ == path_element_open );

					*pos = tmp;
					pos->parent_ = parent;
					pos->state_ = path_element_open;

					if ( was_open )
					{
						path_remove( pos );
					}

					path_push_open( pos, goalx, goaly );
				}
			}
		}
	}
}

/* Updates startx and starty one step towards (goalx, goaly). */
static s32b path_find( s32b startx, s32b starty, s32b goalx, s32b goaly )
{
	s32b found = 0;

	/* Parent points towards start of search. So if we start from goal,
	 then those parent pointers are automatically ok.

	 Of course, the following assumption is made (reflexivity):
	 if there is path from A to B, then there is path from B to A.
	 (this holds in this application)
	 */
	const s32b real_goal_x = startx;
	const s32b real_goal_y = starty;
	const s32b real_start_x = goalx;
	const s32b real_start_y = goaly;

	path_init();

	struct path_element* pos = path_element_map( real_start_x,
		real_start_y );
	struct path_element* start = path_element_map( real_goal_x,
		real_goal_y );
	if ( pos && start ) /* Both should be acceptable */
	{
		/* Push first element to stack. */
		pos->state_ = path_element_open;
		path_push_open( pos, real_goal_x, real_goal_y );

		while ( path_open_not_empty() )
		{
			struct path_element* current = path_pop_open();

			/* Did we reach target? */
			if ( ( current->x_pos_ == real_goal_x ) &&
				( current->y_pos_ == real_goal_y ) )
			{
				found = 1;
				break;
			}

			/* Current is closed. */
			current->state_ = path_element_closed;

			/* Generate positions reachable from current position. */
			path_check( current,
				current->x_pos_ + 1, current->y_pos_ - 1,
				real_goal_x, real_goal_y );
			path_check( current,
				current->x_pos_ - 1, current->y_pos_ - 1,
				real_goal_x, real_goal_y );
			path_check( current,
				current->x_pos_ - 1, current->y_pos_ + 1,
				real_goal_x, real_goal_y );
			path_check( current,
				current->x_pos_ + 1, current->y_pos_ + 1,
				real_goal_x, real_goal_y );
			path_check( current,
				current->x_pos_ + 1, current->y_pos_,
				real_goal_x, real_goal_y );
			path_check( current,
				current->x_pos_ - 1, current->y_pos_,
				real_goal_x, real_goal_y );
			path_check( current,
				current->x_pos_, current->y_pos_ + 1,
				real_goal_x, real_goal_y );
			path_check( current,
				current->x_pos_, current->y_pos_ - 1,
				real_goal_x, real_goal_y );
		}
	}

	return found;
}

/* Gets next step towards goal - path_find must have been called beforehand.
 Results are erased after next path_find call.
 */
static s32b path_get_next( s32b* nextx, s32b* nexty )
{
	s32b result = 0;
	struct path_element* pos = path_element_map( *nextx, *nexty );
	if ( pos->parent_ )
	{
		*nextx = pos->parent_->x_pos_;
		*nexty = pos->parent_->y_pos_;
		result = ( pos->parent_->parent_ != NULL );
	}

	return result;
}

const static s32b cost_table_a[3][3] =
{ { 1, 1, 1 },
{ 3, 1, 3 },
{ 1, 1, 1 } };

const static s32b cost_table_b[3][3] =
{ { 1, 3, 1 },
{ 1, 1, 1 },
{ 1, 3, 1 } };

/* Updates cost to pos which is adjacent to parent. */
static void path_update_cost( struct path_element* parent, struct path_element*
	pos )
{
#ifndef MAP_VARIYING_COST
	pos->cost_ = parent->cost_ + 1;
#else
	s32b dx = pos->x_pos_ - parent->x_pos_ + 1; /* dx = 0, 1, 2 */
	s32b dy = pos->y_pos_ - parent->y_pos_ + 1; /* dy = 0, 1, 2 */

	if ( pos->x_pos_ & 1 )
	{
		pos->cost_ = parent->cost_ + cost_table_a[dx][dy];
	}
	else
	{
		pos->cost_ = parent->cost_ + cost_table_b[dx][dy];
	}
#endif // MAP_VARIYING_COST
}

/* End of file */
