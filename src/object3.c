/* File: object3.c */

/* Purpose: Object code, dynamic allocation */

/*
 * Copyright (c) 2003 DarkGod
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"

/*
 * Allocate an object
 * NOTE: this is the only allowed way to create an object
 */
object_type *new_object()
{
	object_type *o_ptr;

	MAKE(o_ptr, object_type);
	flag_init(&o_ptr->flags, 2);

	object_wipe(o_ptr);

	return o_ptr;
}

static void delete_obj_at_location(object_type *o_ptr, byte iy, byte ix);
/*
 * Recurse through an object's flags.  If any objects are encountered,
 * if the item is an artifact its dropped on the ground, otherwise its
 * deleted as well.  Also checks through the inventory of any monster
 * embeded in the item.
 */
static void delete_obj_recurse_on_flags(flags_type *flags, byte iy, byte ix, byte number)
{
	s32b i;

	for (i = 0; i < flags->size; i++)
	{
		flag_node*  n = &flags->node[i];

		/* Not used */
		if (!(n->flags & FLAG_FLAG_USED)) continue;

		if (n->flags & FLAG_FLAG_FLAGS)
			delete_obj_recurse_on_flags(n->vals.flags, iy, ix, number);
		else if (n->flags & FLAG_FLAG_PTR)
		{
			if (n->vals.ptr.type == FLAG_TYPE_OBJ)
			{
				object_type *o_ptr = (object_type*) n->vals.ptr.ptr;

				if (number && artifact_p(o_ptr) && o_ptr->held_m_idx == 0) {
					object_type *copy = new_object();

					object_copy(copy, o_ptr);
					o_ptr->number = number;
					drop_near(copy, -1, iy, ix);
				}
			}
			else if (n->vals.ptr.type == FLAG_TYPE_MONSTER)
			{
				monster_type *m_ptr = (monster_type*) n->vals.ptr.ptr;

				delete_obj_recurse_on_flags(&(m_ptr->inventory), iy, ix, number);
			}
		} /* else if (n->flags & FLAG_FLAG_PTR) */
	} /* for (i = 0; i < flags->size; i++) */
} /* delete_obj_recurse_on_flags() */

static void delete_obj_at_location(object_type *o_ptr, byte iy, byte ix)
{
	delete_obj_recurse_on_flags(&(o_ptr->flags), iy, ix, o_ptr->number);

	flag_free(&o_ptr->flags, FALSE);
	FREE(o_ptr, object_type);
} /* delete_obj_at_location() */

/*
 * Desallocate an object
 * NOTE: This is the only way allowed to delete an item
 */
void delete_object(object_type *o_ptr)
{
	if ((o_ptr->iy != 0) && (o_ptr->ix != 0))
		delete_obj_at_location(o_ptr, o_ptr->iy, o_ptr->ix);
	else
		delete_obj_at_location(o_ptr, p_ptr->py, p_ptr->px);
}

/*
 * Wipe an object clean.
 */
void object_wipe(object_type *o_ptr)
{
	flags_type f = o_ptr->flags;

	/* Wipe the structure */
	(void) WIPE(o_ptr, object_type);
	o_ptr->flags = f;
	flag_free(&o_ptr->flags, FALSE);
	flag_init(&o_ptr->flags, 2);
}


/*
 * Prepare an object based on an existing object
 */
void object_copy(object_type *o_ptr, object_type *j_ptr)
{
	flag_node *f = o_ptr->flags.node;

	/* Copy the structure */
	(void) COPY(o_ptr, j_ptr, object_type);
	o_ptr->flags.node = f;

	flag_copy(&o_ptr->flags, &j_ptr->flags);
}


/*
 * Allocate + copy
 */
object_type *object_dup(object_type *j_ptr)
{
	object_type *o_ptr = new_object();
	object_copy(o_ptr, j_ptr);
	return o_ptr;
}


/*
 * Prepare an object based on an object kind.
 */
void object_prep(object_type *o_ptr, s32b k_idx)
{
	object_kind *k_ptr = &k_info[k_idx];

	/* Clear the record */
	object_wipe(o_ptr);

	/* Save the kind index */
	o_ptr->k_idx = k_idx;

	/* Efficiency -- tval/sval */
	o_ptr->tval = k_ptr->tval;
	o_ptr->sval = k_ptr->sval;

	/* Default number */
	o_ptr->number = 1;

	/* Default weight */
	o_ptr->weight = k_ptr->weight;

	/* Default magic */
	o_ptr->to_h = k_ptr->to_h;
	o_ptr->to_d = k_ptr->to_d;
	o_ptr->to_a = k_ptr->to_a;

	/* Default power */
	o_ptr->ac = k_ptr->ac;
	o_ptr->dd = k_ptr->dd;
	o_ptr->ds = k_ptr->ds;

	/* Default color */
	o_ptr->d_attr = k_ptr->d_attr;
	o_ptr->x_attr = k_ptr->x_attr;

	/* Hack -- cursed items are always "cursed" */
	if (has_flag(k_ptr, FLAG_CURSED)) o_ptr->ident |= (IDENT_CURSED);

	/* Hack give a basic exp/exp level to an object that needs it */
	if (has_flag(k_ptr, FLAG_LEVELS))
	{
		o_ptr->elevel = (k_ptr->level / 10) + 1;
		o_ptr->exp = player_exp[o_ptr->elevel - 1];
	}

        /* Grab the basic flags */
	flag_add(&o_ptr->flags, &k_ptr->flags);
}


/*
 * Delete a dungeon object
 *
 * Handle "stacks" of objects correctly.
 */
void delete_object_idx(s32b o_idx)
{
	/* Any additional tasks */
	call_lua("__core_objects.delete_object_idx_notice", "(d)", "", o_idx);

	/* Excise */
	item_increase(o_idx, -255);
	item_optimize(o_idx);
}


/*
 * Deletes all objects at given location
 */
void delete_object_location(s32b y, s32b x)
{
	cave_type *c_ptr;

	/* Refuse "illegal" locations */
	if (!in_bounds(y, x)) return;

	/* Grid */
	c_ptr = &cave[y][x];

	/* Objects are gone */
	flag_empty(&c_ptr->inventory);

	/* Visual update */
	lite_spot(y, x);
}


void prepare_inventory(flags_type *inventory)
{
	s32b i;

	flag_init(inventory, 2);

	for (i = INVEN_INVEN; i < INVEN_TOTAL; i++)
	{
		flags_type *inven = flag_new(2);

		/* Default size */
		flag_set(inven, INVEN_LIMIT_KEY, 23);

		/* Empty list */
		flag_set_full(inventory, i, FLAG_FLAG_FLAGS, 0, 0, inven);
	}
}

/* Preserve arts if needed */
void preserve_artifacts_in_inven(flags_type *inven)
{
	if (p_ptr->preserve)
	{
		for_inventory_slot_ro(inven, o_ptr);
		{
			/* Hack -- Preserve unknown artifacts */
			if (artifact_p(o_ptr) && !object_known_p(o_ptr))
			{
				/* Mega-Hack -- Preserve the artifact */
				if (has_flag(&k_info[o_ptr->k_idx], FLAG_NORM_ART))
				{
					k_info[o_ptr->k_idx].artifact = FALSE;
				}
				else
				{
					a_info[o_ptr->artifact_id].cur_num = 0;
				}
			}
		}
		end_inventory();
	}
}
