 /* File: flags.c */

/* Purpose: hashtable flags, dynamic flags */

/*
 * Copyright (c) 2003 DarkGod
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"
#include "lua/tolua.h"

#define MAX_INT 32000
#define MINPOWER2       4       /* minimum size for "growing" vectors */

static u32b next_flag = 1;
static flag_registry *flags_registry = NULL;

/************************* Flag names and registration *******************/

flag_registry *find_flag(u32b index)
{
	flag_registry *i = flags_registry;

	while (i != NULL)
	{
		if (i->index == index)
			return i;
		i = i->next;
	}
	return NULL;
}

flag_registry *find_flag_by_name(cptr name)
{
	flag_registry *i = flags_registry;

	while (i != NULL)
	{
		if (!strcmp(i->name, name))
			return i;
		i = i->next;
	}
	return NULL;
}

cptr get_flag_name(u32b index)
{
	flag_registry *i = find_flag(index);

	return (i)?i->name:"???";
}

u32b get_flag_index(cptr name)
{
	flag_registry *i = find_flag_by_name(name);

	return (i)?i->index:0;
}

u32b register_flag(cptr name)
{
	flag_registry *n;

	if (find_flag_by_name(name))
		quit(format("Flag redefined!!!! : %s\n", name));

	MAKE(n, flag_registry);

	n->index = next_flag++;
	n->name = strdup(name);
	n->next = flags_registry;
	flags_registry = n;
	return n->index;
}


/******************************* Flagsets *******************************/

/*
 * EXPLANATION: A flagset (flags_type structure) is basically a
 * hash-table, with the index of a flag being the key used for that
 * flag's data.  The hash-table consists of a dynamic array of nodes
 * which can also be chained together into linked lists; each node is
 * marked as being used or unused (among other things).  All keys
 * which have the same ideal position (main position) within the array
 * are in a single linked list, with the head of the linked list being
 * the node which is in the main position.  Thus, to find the node for
 * a key, you go to it's main position in the array and start from
 * there in iterating over the linked list, comparing the key you're
 * looking for to the key of each node in the list.  If a match is
 * found, the data in the node is what you're looking for.  If none of
 * the nodes in the list match the key you're looking for, or if the
 * node for the key's main position isn't being used, then the flag
 * you're looking for is not contained within the flagset.
 *
 * To add a new flag to the flagset, you check to see if the flag is
 * already present.  If it is, then the flag's node has its data
 * overwritten by the new data.  If it isn't, then the flag and its
 * data must be put into an unused node.  If the main position of the
 * key (the flag's index) is unused, then that node is used, and it
 * becomes the head of the linked list for nodes sharing that main
 * position.  If the main position is in use, then the node which is
 * already there is called the colliding node.  The main position of
 * the new flag is compared to the main position of the linked list
 * the colliding node is a part of.  If the main positions are the
 * same, then the new flag should be in that linked list, so an unused
 * node is insert into the list and the key plus data is copied onto
 * it.  If the main positions aren't the same, then there is no linked
 * list for the new flag's main position, and the new flag plus data
 * needs to become the head of the linked list for its main position.
 * However, its main position is already occupied by the colliding
 * node.  So an unused node needs to be spliced into the colliding
 * node's linked list, the colliding node's information copied there,
 * and the new flag + data copied onto the now free node where the
 * colliding node used to be.
 */

#define check_init(f) \
	if ((f)->node == NULL) flag_init(f, 2)


/*
** returns the `main' position of an element in a table (that is, the index
** of its hash value)
*/
static flag_node *mainposition(const flags_type *t, u32b key)
{
	u32b h;

	h = key;

/*	printf("mainpos(): key: %ld ; t->size-1: %d := %d\n", key, t->size-1, (h & (t->size-1)));*/

	return &t->node[h &(t->size - 1)];
}

#ifdef DEBUG_FLAGS
/* MATT */
static void print_insane(cptr msg, cptr func_name, s32b key, s32b trigger_key)
{
	if (trigger_key > 0)
	{
		printf(msg, get_flag_name(key));
		printf(" (key=%s): %s\n", get_flag_name(trigger_key), func_name);
	}
	else
	{
		printf(msg, get_flag_name(key));
		printf(": %s\n", func_name);
	}
}

extern void print_lua_stack(void);
static flags_type* prev_insane_flag = NULL;
static void flag_check_sanity(flags_type *t, cptr func_name, s32b trigger_key)
{
	int  i = 0;
	bool problem  = FALSE;

	/*
	 * If this is flagset which was recently found to be not be sane,
	 * it's probably still not sane, so don't bother checking.
	 */
	if (prev_insane_flag == t)
	{
		if (trigger_key > 0)
			printf("Previous insane flagset, not checking again (key=%s): %s\n",
				   get_flag_name(trigger_key), func_name);
		else
			printf("Previous insane flagset, not checking again: %s\n",
				   func_name);

		return;
	}

	if (t->firstfree->flags & FLAG_FLAG_USED)
	{
		print_insane("Node %s is t->firstfree, but is actually USED",
					 func_name, t->firstfree->key, trigger_key);
		problem = TRUE;
	}
	else
		for (i = t->size - 1; i >= 0; i--)
		{
			flag_node* node  = &t->node[i];

			if ((node->flags & FLAG_FLAG_USED))
				continue;

			if (node != t->firstfree)
			{
				print_insane("Node %s is first free node, but != t->firstfree",
							 func_name, node->key, trigger_key);
				problem = TRUE;
			}
			break;
		}

	for (i = 0; i < t->size; i++)
	{
		flag_node* node  = &t->node[i];
		s32b       key   = node->key;
		flag_node* mp    = mainposition(t, key);
		flag_node* n;
		bool       found  = FALSE;
		int        j, prevs;
		int        types     = 0;
		bool       rand_type = FALSE;

		if (!(node->flags & FLAG_FLAG_USED))
			continue;

		if (node->flags & (FLAG_FLAG_RAND | FLAG_FLAG_MBONUS |
						   FLAG_FLAG_RAND_MBONUS))
			rand_type = TRUE;

		if (node->flags & FLAG_FLAG_BOTH)
			types++;

		if (node->flags & FLAG_FLAG_FLAGS)
			types++;

		if (node->flags & FLAG_FLAG_PTR)
			types++;

		if (rand_type && types >= 1)
		{
			print_insane("Node %s is both a random and a non-random type",
						 func_name, key, trigger_key);
			problem = TRUE;
		}

		if (types > 1)
		{
			print_insane("Node %s has multiple, conflicting types",
						 func_name, key, trigger_key);
			problem = TRUE;
		}

		if ((node->flags & FLAG_FLAG_FLAGS) && node->vals.flags == NULL)
		{
			print_insane("Node %s is flags type, but flags == NULL",
						 func_name, key, trigger_key);
			problem = TRUE;
		}
		else if ((node->flags & FLAG_FLAG_PTR) && node->vals.ptr.ptr == NULL)
		{
			print_insane("Node %s is PTR type, but ptr.ptr == NULL",
						 func_name, key, trigger_key);
			problem = TRUE;
		}

		prevs = 0;
		for (j = 0; j < t->size; j++)
			if (t->node[j].next == node)
				prevs++;

		if (prevs > 1)
		{
			print_insane("Node %s has more than one previous node",
						 func_name, key, trigger_key);
			problem = TRUE;
		}

		for (n = mp; n != NULL; n = n->next)
		{
			if (!(n->flags & FLAG_FLAG_USED))
				continue;

			if (mainposition(t, n->key) != mp)
			{
				print_insane("Node %s is in wrong chain",
							 func_name, n->key, trigger_key);
				problem = TRUE;
			}

			if (n->key == key)
				found = TRUE;
		} /* for (n = mp; n != NULL; n = n->next) */

		if (!found)
		{
			print_insane("Flag %s is not reachable",
						 func_name, key, trigger_key);

			if (prevs > 0)
				printf("Yet it still has a previous node...\n");

			problem = TRUE;
		}
	} /* for (i = 0; i < t->size; i++) */

	if (problem)
	{
		print_lua_stack();
		prev_insane_flag = t;
	}
}

static void clear_prev_insane_flag(flags_type* t)
{
	if (prev_insane_flag == t)
		prev_insane_flag = NULL;
}

static bool is_power_two(s32b num)
{
	if (num < 2)
		return FALSE;

	while (num > 2)
	{
		if (num % 2 != 0)
			return FALSE;

		num >>= 1;
	}

	return (num == 2);
}

#else /* #ifdef DEBUG_FLAGS */
#	define flag_check_sanity(A, B, C) ;
#	define clear_prev_insane_flag(A) ;
#endif /* #ifdef DEBUG_FLAGS */


s32b flag_used(flags_type *t)
{
	flag_node *v = t->node;
	s32b size = t->size;
	s32b realuse = 0;
	s32b i;
	/* Initialize if it isnt */
	check_init(t);
	for (i = 0; i < size; i++) {
		if (v[i].flags & FLAG_FLAG_USED)
			realuse++;
	}
	return realuse;
}

/* Grab the higher key */
u32b flag_max_key(flags_type *t)
{
	flag_node *v = t->node;
	s32b size = t->size;
	u32b max = 0;
	s32b i;
	/* Initialize if it isnt */
	check_init(t);
	for (i = 0; i < size; i++) {
		if ((v[i].flags & FLAG_FLAG_USED) && (max < v[i].key))
			max = v[i].key;
	}
	return max;
}

/* Grab the first free key higher or equal to min */
u32b flag_first_free_key(flags_type *t, u32b min)
{
	u32b i;
	u32b max = flag_max_key(t);
	/* Initialize if it isnt */
	check_init(t);
	for (i = min; i <= max; i++)
		if (!flag_exists(t, i))
			return i;
	return max + 1;
}

static void setnodevector (flags_type *t, s32b size)
{
	s32b i;

/*	tome_assert(size <= MAX_INT, "table overflow");*/

	C_MAKE(t->node, size, flag_node);

	for (i = 0; i < size; i++)
	{
		t->node[i].flags = 0;
		t->node[i].next = NULL;
	}
	t->size = size;
/*	printf("setnodevector():size = %d\n", size);*/
	t->firstfree = &t->node[size-1];  /* first free position to be used */
}

/*
 * Mega-kludge: If firstfree's "next" field points to a node, and is
 * the head of a chain for a used node (i.e., the node's main position),
 * then things can get weird when firstfree is used and it's "next"
 * field is overwritten.  Thus, if firstree's "next" *does* point to
 * a node, copy that node over onto firstree, free up the node,
 * recalculate firstree, and repeat until firstree's "next" is NULL.
 */
static void fix_firstfree(flags_type *t)
{
	while (t->firstfree->next != NULL)
	{
		flag_node *n = t->firstfree;
		bool fixed = FALSE;

		for (; n != NULL; n = n->next)
		{
			if (n->flags & FLAG_FLAG_USED)
			{
				flag_node *mp = mainposition(t, n->key);
				/*
				 * Firstfree isn't actually the node's main position,
				 * attempt to chain the node in right after it's
				 * main position.
				 */
				if (mp != t->firstfree)
				{
					flag_node *end      = n;
					flag_node* old_next = mp->next;
				
					while (end->next != NULL)
						end = end->next;

					mp->next  = t->firstfree;
					end->next = old_next;
				}

				*(t->firstfree) = *n;
				n->next = NULL;
				n->flags &= ~FLAG_FLAG_USED;

				/* firstfree _must_ be the highest free slot in node[] */
				if (n > t->firstfree)
					t->firstfree = n;
				else
				{
					t->firstfree = &t->node[t->size - 1];
					while (t->firstfree->flags & FLAG_FLAG_USED)
						(t->firstfree)--;
				}

				fixed = TRUE;
				break;
			} /* if (n->flags & FLAG_FLAG_USED) */
		} /* for (; n != NULL; n = n->next) */
		if (!fixed)
			cmsg_format(TERM_L_RED,
						"WARNING: fix_firstfree() failed!");
	} /* while (t->firstfree->next != NULL) */
}

static void rehash (flags_type *t)
{
	s32b oldsize = t->size;
	flag_node *nold = t->node;
	s32b nelems = flag_used(t);
	s32b i;

/*        printf("rehashing(flag_used %d)\n", nelems);*/
/*	tome_assert(nelems <= oldsize, "wrong count in hashtable:rehash()");*/

	if (nelems >= oldsize-oldsize/4)  /* using more than 3/4? */
		setnodevector(t, oldsize*2);
#if 0 /* frelling smegging thing */
	else if (nelems <= oldsize/4 &&  /* less than 1/4? */
		oldsize > MINPOWER2)
		setnodevector(t, oldsize/2);
#endif
	else
	{
/*		printf("no rehashing to do\n");*/
                return;
//		setnodevector(t, oldsize);
	}

#ifdef DEBUG_FLAGS
	if (!is_power_two(t->size))
		printf("WARNING: flag size not power of two in rehash()\n");
#endif

	for (i=0; i<oldsize; i++)
	{
		flag_node *old = nold+i;
		if (old->flags & FLAG_FLAG_USED)
		{
/*			flag_registry *flag = find_flag(old->key);
 printf("injecting: %s: %d[%s] = %d\n", (old->used)?"USE":"NOT", old->key, (flag)?flag->name:"???", old->val);*/
			if (old->flags & FLAG_FLAG_PTR)
				flag_set_full(t, old->key, old->flags, old->vals.ptr.type, 0, old->vals.ptr.ptr);
			else
				flag_set_full(t, old->key, old->flags, old->vals.vals.val, old->vals.vals.val2, old->vals.flags);
//			flags_print(t);
		}
	}
	C_FREE(nold, oldsize, flag_node);  /* free old array */
	fix_firstfree(t);
	flag_check_sanity(t, "rehash()", -1);
/*        printf("done rehashing\n");*/
}


/*
 ** inserts a key into a hash table; first, check whether key is
 ** already present; if not, check whether key's main position is free;
 ** if not, check whether colliding node is in its main position or not;
 ** if it is not, move colliding node to an empty place and put new key
 ** in its main position; otherwise (colliding node is in its main position),
 ** new key goes to an empty position.
 */
void flag_set_full(flags_type *t, u32b key, byte flags, s32b value, s32b value2, flags_type *value_flags)
{
	flag_node *mp;
	flag_node *n;

        /* Initialize if it isnt */
	check_init(t);
	mp = mainposition(t, key);
	n = mp;

/*	tome_assert(mp != NULL, "hash index is nil");*/

	do {  /* check whether `key' is somewhere in the chain */
		if ((n->flags & FLAG_FLAG_USED) && (key == n->key))
		{
			/* If already a flag we remove it */
			if (n->flags & FLAG_FLAG_FLAGS)
				flag_free(n->vals.flags, TRUE);
			else if (n->flags & FLAG_FLAG_PTR)
				flags_pointer_types[n->vals.ptr.type].unalloc(n->vals.ptr.ptr);

			if (flags & FLAG_FLAG_PTR)
			{
				n->vals.ptr.type = value;
				n->vals.ptr.ptr = value_flags;
			}
			else if (flags & FLAG_FLAG_FLAGS)
			{
				n->vals.flags = value_flags;
			}
			else
			{
				n->vals.vals.val = value;  /* that's all */
				n->vals.vals.val2 = value2;  /* that's all */
			}
			n->flags = flags;  /* that's all */
			n->flags |= FLAG_FLAG_USED;
			return;
		}
		else n = n->next;
	} while (n);

	rehash(t);  /* test no more free places? */

	/* Ok need to recalc the hash position */
	mp = mainposition(t, key);
	n = mp;

	/* `key' not found; must insert it */
	if (mp->flags & FLAG_FLAG_USED) /* main position is not free? */
	{
		flag_node *othern;  /* main position of colliding node */
		n = t->firstfree;  /* get a free place */

/*
		printf("mp for %s not free\n", get_flag_name(key));
*/

		/* is colliding node out of its main position? (can only happens if
		 its position is after "firstfree") */
		if (/*mp > n && */(othern = mainposition(t, mp->key)) != mp) {
			/*
			 * Find first unused node in chain before colliding node, or
			 * the node immediatly before the colliding node if all of
			 * them are being used.
			 */
/*
			printf("%s coliding out of its position for %s\n",
				   get_flag_name(mp->key),
				   get_flag_name(key));
*/

			while ((othern->flags & FLAG_FLAG_USED) && othern->next &&
				   othern->next != mp)
				othern = othern->next;

			if (!othern->next)
			{
				/* Reached end of chain? */
				cmsg_format(TERM_L_RED,
							"WARNING: othern->next == NULL!!!");

				othern->next = n;
				*n = *mp;
			}
			else if (othern->next == mp)
			{
				/*
				 * All nodes in chain before the colliding node are
				 * in use, insert first free node into chain right
				 * before colliding node, then copy the colliding
				 * node there.
				 */
				if (othern == n)
					cmsg_format(TERM_L_RED,
								"WARNING: othern->next == NULL!!!");

				othern->next = n;
				*n = *mp;
			}
			else
			{
				/*
				 * Node isn't being used, just copy the colliding node
				 * there, but don't alter the ordering of the chain
				 * any.
				 */
				flag_node *next = othern->next;

				if (!next)
					cmsg_format(TERM_L_RED,
						"WARNING: reached end of flag-node chain!");

				if (othern->next == othern)
					cmsg_format(TERM_L_RED,
						"WARNING: loop forming in flag-node chain!");

/*
				printf("othern free, othern->next = %s\n",
					   get_flag_name(next->key));
*/

				*othern = *mp;
				othern->next = next;
			}


			mp->next = NULL;  /* now `mp' is free */
		}
		else {  /* colliding node is in its own main position */
			/* new node will go into free position */

/*
			printf("%s *NOT* coliding out of its position for %s\n",
				   get_flag_name(mp->key),
				   get_flag_name(key));
*/

/*			printf(" n : %s ; next=%s\n", get_flag_name(n->key), (mp->next)?get_flag_name(mp->next->key):"nonext");*/

			n->next = mp->next;  /* chain new position */
			if (n == mp->next)
				cmsg_format(TERM_L_RED,
							"BLOODY! Warning!!! n == mp->next, "
							"size %ld; %ld : %ld\n", t->size,
							(long int)(n - t->node),
							(long int)(mp - t->node));
			mp->next = n;
			if (mp == n)
				cmsg_format(TERM_L_RED,
							"Warning!!! mp == n"); 
			mp = n;
		}
	}

	mp->key = key;

	if (flags & FLAG_FLAG_PTR)
	{
		mp->vals.ptr.type = value;
		mp->vals.ptr.ptr = value_flags;
	}
	else if (flags & FLAG_FLAG_FLAGS)
	{
		mp->vals.flags = value_flags;
	}
	else
	{
		mp->vals.vals.val = value;
		mp->vals.vals.val2 = value2;
	}
	mp->flags = flags;
	mp->flags |= FLAG_FLAG_USED;

	/* firstfree _must_ be the highest free slot in node[] */
	/* rehash() guarantees free slots so never infinite loop */
	while (t->firstfree->flags & FLAG_FLAG_USED)
		(t->firstfree)--;

	fix_firstfree(t);
	flag_check_sanity(t, "flag_set_full()", key);
}

/*
 * Simplier set flag
 */
void flag_set(flags_type *t, u32b key, s32b value)
{
	flag_set_full(t, key, FLAG_FLAG_USED, value, 0, NULL);
}

void flag_set_ptr(flags_type *t, u32b key, byte type, void *ptr)
{
	flag_set_full(t, key, FLAG_FLAG_USED | FLAG_FLAG_PTR, type, 0, (flags_type*)ptr);
}

void flag_remove(flags_type *t, u32b key)
{
	flag_node *n;
	s32b i;

	/* Initialize if it isnt */
	check_init(t);

	n = &t->node[key & (t->size - 1)];

	do
	{
		if ((n->flags & FLAG_FLAG_USED) && (n->key == key))
		{
			if (n->flags & FLAG_FLAG_PTR)
			{
				flags_pointer_types[n->vals.ptr.type].unalloc(n->vals.ptr.ptr);
			}
			else if (n->flags & FLAG_FLAG_FLAGS)
			{
				flag_free(n->vals.flags, TRUE);
			}
			n->flags &= ~FLAG_FLAG_USED;
			t->firstfree = MAX(t->firstfree,n);

			/* Rework the chain to skip it */
			for (i = 0; i < t->size; i++)
			{
				if (t->node[i].next == n)
				{
					/*printf("Removing old next %p\n",n->next);*/
					t->node[i].next = n->next;
				}
			}
			fix_firstfree(t);
			flag_check_sanity(t, "flag_remove()", key);
			return;
		}
		n = n->next;
	} while (n);

	flag_check_sanity(t, "flag_remove()", key);

	return;  /* key not found */
}

/* Saem as remove except it does *NOT* free any memory, beware */
void flag_remove_zap(flags_type *t, u32b key)
{
	flag_node *n;
	s32b i;

	/* Initialize if it isnt */
	check_init(t);

	n = &t->node[key & (t->size - 1)];

	do
	{
		if ((n->flags & FLAG_FLAG_USED) && (n->key == key))
		{
			n->flags &= ~FLAG_FLAG_USED;
                        t->firstfree = MAX(t->firstfree,n);

			/* Rework the chain to skip it */
			for (i = 0; i < t->size; i++)
			{
				if (t->node[i].next == n)
				{
/*					printf("Removing old next %d\n",n->next); */
					t->node[i].next = n->next;
				}
			}
			fix_firstfree(t);
			flag_check_sanity(t, "flag_remove_zap()", key);
			return;
		}
		n = n->next;
	} while (n);

	flag_check_sanity(t, "flag_remove_zap()", key);
	return;  /* key not found */
}

void flag_remove_rand(flags_type *t)
{
	s32b i;

	/* Initialize if it isnt */
	check_init(t);
	for (i = 0; i < t->size; i++)
	{
		if (!(t->node[i].flags & FLAG_FLAG_USED)) continue;

		if (t->node[i].flags & FLAG_FLAG_FLAGS)
			flag_remove_rand(t->node[i].vals.flags);
		else if (t->node[i].flags & (FLAG_FLAG_RAND | FLAG_FLAG_MBONUS |
									 FLAG_FLAG_RAND_MBONUS))
			flag_remove(t, t->node[i].key);
	}
	flag_check_sanity(t, "flag_remove_rand()", -1);
}

void flag_remove_rand_zap(flags_type *t)
{
	s32b i;

	/* Initialize if it isnt */
	check_init(t);
	for (i = 0; i < t->size; i++)
	{
		if (!(t->node[i].flags & FLAG_FLAG_USED)) continue;

		if (t->node[i].flags & FLAG_FLAG_FLAGS)
			flag_remove_rand_zap(t->node[i].vals.flags);
		else if (t->node[i].flags & (FLAG_FLAG_RAND | FLAG_FLAG_MBONUS |
									 FLAG_FLAG_RAND_MBONUS))
			flag_remove_zap(t, t->node[i].key);
	}
	flag_check_sanity(t, "flag_remove_rand_zap()", -1);
}

s32b flag_get(flags_type *t, u32b key)
{
	flag_node *n;

	/* Initialize if it isnt */
	check_init(t);

	n = &t->node[key & (t->size - 1)];

	do
	{
/*                printf("test %ld == %ld\n", n->key, key);*/
		if ((n->flags & FLAG_FLAG_USED) && (n->key == key))
			return n->vals.vals.val;
		n = n->next;
	} while (n);

	return 0;  /* key not found */
}

s32b flag_get2(flags_type *t, u32b key)
{
	flag_node *n;

	/* Initialize if it isnt */
	check_init(t);

	n = &t->node[key & (t->size - 1)];

	do
	{
/*                printf("test %ld == %ld\n", n->key, key);*/
		if ((n->flags & FLAG_FLAG_USED) && (n->key == key))
			return n->vals.vals.val2;
		n = n->next;
	} while (n);

	return 0;  /* key not found */
}

flags_type *flag_get_flags(flags_type *t, u32b key)
{
	flag_node *n;

	/* Initialize if it isnt */
	check_init(t);

	n = &t->node[key & (t->size - 1)];

	do
	{
/*                printf("test %ld == %ld\n", n->key, key);*/
		if ((n->flags & FLAG_FLAG_USED) && (n->key == key))
			return n->vals.flags;
		n = n->next;
	} while (n);

	return NULL;  /* key not found */
}

void *flag_get_ptr(flags_type *t, u32b key, byte type)
{
	flag_node *n;

	/* Initialize if it isnt */
	check_init(t);

	n = &t->node[key & (t->size - 1)];

	do
	{
		if ((n->flags & FLAG_FLAG_USED) && (n->key == key) && (n->vals.ptr.type == type))
		{
			return n->vals.ptr.ptr;
		}
		n = n->next;
	} while (n);

	return NULL;  /* key not found */
}

flag_node *flag_get_node(flags_type *t, u32b key)
{
	flag_node *n;

	/* Initialize if it isnt */
	check_init(t);

	n = &t->node[key & (t->size - 1)];

	do
	{
/*                printf("test %ld == %ld\n", n->key, key);*/
		if ((n->flags & FLAG_FLAG_USED) && (n->key == key))
			return n;
		n = n->next;
	} while (n);

	return NULL;  /* key not found */
}

bool flag_exists(flags_type *t, u32b key)
{
	flag_node *n;

	/* Initialize if it isnt */
	check_init(t);

	n = &t->node[key & (t->size - 1)];

	do
	{
		if ((n->flags & FLAG_FLAG_USED) && (n->key == key))
			return TRUE;
		n = n->next;
	} while (n);

	return FALSE;  /* key not found */
}

/* Learn about this flag */
void flag_learn(flags_type *t, u32b key, bool learn)
{
	flag_node *n;

	/* Initialize if it isnt */
	check_init(t);

	n = &t->node[key & (t->size - 1)];

	do
	{
		if ((n->flags & FLAG_FLAG_USED) && (n->key == key))
		{
			if (learn)
				n->flags |= FLAG_FLAG_KNOWN;
			else
				n->flags &= ~FLAG_FLAG_KNOWN;
			return;
		}
		n = n->next;
	} while (n);
}

void flag_learn_all(flags_type *t)
{
	s32b i;

	/* Initialize if it isnt */
	check_init(t);

	for (i = 0; i < t->size; i++)
	{
		if (!(t->node[i].flags & FLAG_FLAG_USED)) continue;

		if (t->node[i].flags & FLAG_FLAG_FLAGS)
		{
			flag_learn_all(t->node[i].vals.flags);
		}
		t->node[i].flags |= FLAG_FLAG_KNOWN;
	}
}

/* Is this flag known */
bool flag_is_known(flags_type *t, u32b key)
{
	flag_node *n;

	/* Initialize if it isnt */
	check_init(t);

	n = &t->node[key & (t->size - 1)];

	do
	{
		if ((n->flags & FLAG_FLAG_USED) && (n->key == key))
		{
			return (n->flags & FLAG_FLAG_KNOWN) ? TRUE : FALSE;
		}
		n = n->next;
	} while (n);

	return FALSE;
}

flags_type *flag_new(s32b size)
{
	flags_type *t;

	MAKE(t, flags_type);

	if (size < 2)
		size = 2;

#ifdef DEBUG_FLAGS
	if (!is_power_two(size))
		printf("WARNING: flag size not power of two in flag_new()\n");
#endif

	t->size = 0;
	t->node = NULL;
	setnodevector(t, (size * size));
	return t;
}

void flag_init(flags_type *t, s32b size)
{
/*        if (size < 2) size = 2; */

	t->size = 0;
	t->node = NULL;
	setnodevector(t, (size * size));
}

void flag_empty(flags_type *t)
{
	s32b i;

	/* Initialize if it isnt */
	check_init(t);
	for (i = 0; i < t->size; i++)
	{
		if (!(t->node[i].flags & FLAG_FLAG_USED)) continue;

		if (t->node[i].flags & FLAG_FLAG_PTR)
		{
			flags_pointer_types[t->node[i].vals.ptr.type].unalloc(t->node[i].vals.ptr.ptr);
		}
		else if (t->node[i].flags & FLAG_FLAG_FLAGS)
		{
			flag_free(t->node[i].vals.flags, TRUE);
		}
		t->node[i].flags = 0;
		t->node[i].next = NULL;
	}

	/* Reset firstfree */
	t->firstfree = &t->node[t->size-1];

	clear_prev_insane_flag(t);
}

void flag_empty_zap(flags_type *t)
{
	s32b i;

	/* Initialize if it isnt */
	check_init(t);
	for (i = 0; i < t->size; i++)
	{
		if (!(t->node[i].flags & FLAG_FLAG_USED)) continue;

		t->node[i].flags = 0;
		t->node[i].next = NULL;
	}

	/* Reset firstfree */
	t->firstfree = &t->node[t->size-1];
}

void flag_free(flags_type *t, bool full)
{
	if (t->node)
	{
		/* This is needed so that pairs containing flagsets get correctly deallocated */
		flag_empty(t);

		/* Now get rid of it */
		C_FREE(t->node, t->size, flag_node);
		if (full) FREE(t, flags_type);
	}
}

void flag_copy(flags_type *dest, flags_type *src)
{
	s32b i;

	/* Initialize if it isnt */
	check_init(dest);
	check_init(src);

	C_FREE(dest->node, dest->size, flag_node);
	setnodevector(dest, src->size);

	for (i = 0; i < src->size; i++)
		if (src->node[i].flags & FLAG_FLAG_USED)
		{
			flags_type *val_flags = NULL;

			/* If needed then we copy the value flags */
			if (src->node[i].flags & FLAG_FLAG_FLAGS)
			{
				val_flags = flag_dup(src->node[i].vals.flags);
				flag_set_full(dest, src->node[i].key, src->node[i].flags, src->node[i].vals.vals.val, src->node[i].vals.vals.val2, val_flags);
			}
			else if (src->node[i].flags & FLAG_FLAG_PTR)
			{
				flag_set_full(dest, src->node[i].key, src->node[i].flags, src->node[i].vals.ptr.type, 0, flags_pointer_types[src->node[i].vals.ptr.type].dup(src->node[i].vals.ptr.ptr));
			}
			else
				flag_set_full(dest, src->node[i].key, src->node[i].flags, src->node[i].vals.vals.val, src->node[i].vals.vals.val2, val_flags);
		}
	flag_check_sanity(dest, "flag_copy()", -1);
}

flags_type *flag_dup(flags_type *src)
{
	flags_type *dest = flag_new(src->size);

	/* Initialize if it isnt */
	check_init(src);

	flag_copy(dest, src);

	flag_check_sanity(dest, "flag_dup()", -1);

	return dest;
}

/* Copy flags but don't copy objects pointed-to or flagsets */
void flag_shallow_copy(flags_type *dest, flags_type *src)
{
	s32b i;

	/* Initialize if it isnt */
	check_init(dest);
	check_init(src);

	C_FREE(dest->node, dest->size, flag_node);
	setnodevector(dest, src->size);

	for (i = 0; i < src->size; i++)
		if (src->node[i].flags & FLAG_FLAG_USED)
		{
			flags_type *val_flags = NULL;

			if (src->node[i].flags & FLAG_FLAG_FLAGS)
				flag_set_full(dest, src->node[i].key, src->node[i].flags, src->node[i].vals.vals.val, src->node[i].vals.vals.val2, src->node[i].vals.flags);
			else if (src->node[i].flags & FLAG_FLAG_PTR)
				flag_set_full(dest, src->node[i].key, src->node[i].flags, src->node[i].vals.ptr.type, 0, src->node[i].vals.ptr.ptr);
			else
				flag_set_full(dest, src->node[i].key, src->node[i].flags, src->node[i].vals.vals.val, src->node[i].vals.vals.val2, val_flags);
		}
	flag_check_sanity(dest, "flag_shallow_copy()", -1);
}

flags_type *flag_shallow_dup(flags_type *src)
{
	flags_type *dest = flag_new(src->size);

	/* Initialize if it isnt */
	check_init(src);

	flag_shallow_copy(dest, src);
	flag_check_sanity(dest, "flag_shallow_copy()", -1);

	return dest;
}

void flag_shallow_free(flags_type *t, bool full)
{
	if (t->node)
	{
		/* Now get rid of it */
		C_FREE(t->node, t->size, flag_node);
		if (full) FREE(t, flags_type);
	}
}


u32b flag_contains_nb(flags_type *f1, flags_type *f2)
{
	s32b i;
	u32b nb = 0;

	/* Initialize if it isnt */
	check_init(f1);
	check_init(f2);

	for (i = 0; i < f2->size; i++)
		if ((f2->node[i].flags & FLAG_FLAG_USED) && flag_exists(f1, f2->node[i].key) && (flag_get(f1, f2->node[i].key) == flag_get(f1, f2->node[i].key)))
			nb++;

	return nb;
}

bool flag_intersects(flags_type *f1, flags_type *f2)
{
	s32b i;

	/* Initialize if it isnt */
	check_init(f1);
	check_init(f2);

	for (i = 0; i < f1->size; i++)
	{
		if ((f1->node[i].flags & FLAG_FLAG_USED) && (flag_exists(f2, f1->node[i].key)))
		{
			return TRUE;
		}
	}
	return FALSE;
}

bool flag_intersects_where(flags_type *f1, flags_type *f2, u32b *where)
{
	s32b i;

	/* Initialize if it isnt */
	check_init(f1);
	check_init(f2);

	for (i = 0; i < f1->size; i++)
	{
		if ((f1->node[i].flags & FLAG_FLAG_USED) && (flag_exists(f2, f1->node[i].key)))
		{
			*where = f1->node[i].key;
			return TRUE;
		}
	}
	return FALSE;
}


bool flag_contains(flags_type *f1, flags_type *f2)
{
	s32b i;

	/* Initialize if it isnt */
	check_init(f1);
	check_init(f2);

	for (i = 0; i < f2->size; i++)
		if ((f2->node[i].flags & FLAG_FLAG_USED) &&
			((!flag_exists(f1, f2->node[i].key)) ||
				(flag_get(f1, f2->node[i].key) != flag_get(f2, f2->node[i].key))))
			return FALSE;

	return TRUE;
}

static bool flag_node_equal(flag_node *n1, flag_node *n2) {
	if (n1->flags != n2->flags) return FALSE;

	if (n1->flags & FLAG_FLAG_FLAGS) {
		if (!flag_equal(n1->vals.flags,n2->vals.flags))
			return FALSE;
	} else if (n1->flags & FLAG_FLAG_PTR) {
		if (n1->vals.ptr.type != n2->vals.ptr.type)
			return FALSE;
		if (n1->vals.ptr.ptr != n2->vals.ptr.ptr)
			return FALSE;
	} else if (n1->flags & FLAG_FLAG_BOTH) {
		if (n1->vals.vals.val != n2->vals.vals.val)
			return FALSE;
		if (n1->vals.vals.val2 != n2->vals.vals.val2)
			return FALSE;
	} else {
		if (n1->vals.vals.val != n2->vals.vals.val)
			return FALSE;
	}
	return TRUE;
}

bool flag_equal(flags_type *f1, flags_type *f2)
{
	s32b i;

	/* Initialize if it isnt */
	check_init(f1);
	check_init(f2);

	for (i = 0; i < f1->size; i++)
		if (f1->node[i].flags & FLAG_FLAG_USED) {
			flag_node *node;

			node = flag_get_node(f2,f1->node[i].key);

			if (!node)
				return FALSE;

			if (!flag_node_equal(&f1->node[i],node))
				return FALSE;
		}

	for (i = 0; i < f2->size; i++)
		if (f2->node[i].flags & FLAG_FLAG_USED) {
			flag_node *node;

			node = flag_get_node(f1,f2->node[i].key);

			if (!node)
				return FALSE;

			if (!flag_node_equal(&f2->node[i],node))
				return FALSE;
		}


	return TRUE;
}

s16b flags_mbonus_level = -1;
void flag_add(flags_type *dest, flags_type *src)
{
	s32b i;

	/* Initialize if it isnt */
	check_init(dest);
	check_init(src);

	for (i = 0; i < src->size; i++)
		if (src->node[i].flags & FLAG_FLAG_USED)
		{
			/* Need to randomize between val & val2 */
			if (src->node[i].flags & FLAG_FLAG_RAND)
			{
				s32b v;

				if (src->node[i].flags & FLAG_FLAG_MBONUS)
				{
					s16b lev = flags_mbonus_level;

					if (flags_mbonus_level == -1) lev = dun_level;
					v = src->node[i].vals.vals.val2 + m_bonus(src->node[i].vals.vals.val, lev);
				}
				else if (src->node[i].flags & FLAG_FLAG_RAND_MBONUS)
				{
					s16b lev = flags_mbonus_level;
					s32b v2;

					if (flags_mbonus_level == -1) lev = dun_level;

					if (src->node[i].vals.vals.val2 > 0) v2 = randint(src->node[i].vals.vals.val2);
					else v2 = -randint(-src->node[i].vals.vals.val2);
					v = v2 + m_bonus(src->node[i].vals.vals.val, lev);
				}
				else
				{
					v = rand_range(src->node[i].vals.vals.val2, src->node[i].vals.vals.val);
				}
				flag_set(dest, src->node[i].key, v);
				flag_learn(dest, src->node[i].key, src->node[i].flags & FLAG_FLAG_KNOWN);
			}
			/* Need to get both */
			else if (src->node[i].flags & FLAG_FLAG_BOTH)
			{
				flag_set_full(dest, src->node[i].key, src->node[i].flags, src->node[i].vals.vals.val, src->node[i].vals.vals.val2, NULL);
			}
			else if (src->node[i].flags & FLAG_FLAG_FLAGS)
			{
				flags_type *flg;

				/* Recursively add them if any exists */
				if (flag_exists(dest, src->node[i].key))
					flg = flag_get_flags(dest, src->node[i].key);
				else
				{
					flg = flag_new(2);
					flag_set_full(dest, src->node[i].key, src->node[i].flags, 0, 0, flg);
				}

				/* Make sure to correctly recuse and handle randoms & such */
				flag_add(flg, src->node[i].vals.flags);
			}
			else if (src->node[i].flags & FLAG_FLAG_PTR)
			{
				flag_set_full(dest, src->node[i].key, src->node[i].flags, src->node[i].vals.ptr.type, 0, flags_pointer_types[src->node[i].vals.ptr.type].dup(src->node[i].vals.ptr.ptr));
			}
			else
			{
				flag_set(dest, src->node[i].key, src->node[i].vals.vals.val);
				flag_learn(dest, src->node[i].key, src->node[i].flags & FLAG_FLAG_KNOWN);
			}
		}
	flag_check_sanity(dest, "flag_add()", -1);
}

void flag_add_new(flags_type *dest, flags_type *src)
{
	s32b i;

	/* Initialize if it isnt */
	check_init(dest);
	check_init(src);

	for (i = 0; i < src->size; i++)
		if ((src->node[i].flags & FLAG_FLAG_USED) && (!flag_exists(dest, src->node[i].key)))
		{
			/* Need to randomize between val & val2 */
			if (src->node[i].flags & FLAG_FLAG_RAND)
			{
				s32b v;

				if (src->node[i].flags & FLAG_FLAG_MBONUS)
				{
					s16b lev = flags_mbonus_level;

					if (flags_mbonus_level == -1) lev = dun_level;
					v = src->node[i].vals.vals.val2 + m_bonus(src->node[i].vals.vals.val, lev);
				}
				else if (src->node[i].flags & FLAG_FLAG_RAND_MBONUS)
				{
					s16b lev = flags_mbonus_level;
					s32b v2;

					if (flags_mbonus_level == -1) lev = dun_level;

					if (src->node[i].vals.vals.val2 > 0) v2 = randint(src->node[i].vals.vals.val2);
					else v2 = -randint(-src->node[i].vals.vals.val2);
					v = v2 + m_bonus(src->node[i].vals.vals.val, lev);
				}
				else
				{
					v = rand_range(src->node[i].vals.vals.val2, src->node[i].vals.vals.val);
				}
				flag_set(dest, src->node[i].key, v);
				flag_learn(dest, src->node[i].key, src->node[i].flags & FLAG_FLAG_KNOWN);
			}
			/* Need to get both */
			else if (src->node[i].flags & FLAG_FLAG_BOTH)
			{
				flag_set_full(dest, src->node[i].key, src->node[i].flags, src->node[i].vals.vals.val, src->node[i].vals.vals.val2, NULL);
			}
			else if (src->node[i].flags & FLAG_FLAG_FLAGS)
			{
				flags_type *flg;

				/* Recursively add them if any exists */
				if (flag_exists(dest, src->node[i].key))
					flg = flag_get_flags(dest, src->node[i].key);
				else
				{
					flg = flag_new(2);
					flag_set_full(dest, src->node[i].key, src->node[i].flags, 0, 0, flg);
				}

				/* Make sure to correctly recuse and handle randoms & such */
				flag_add(flg, src->node[i].vals.flags);
			}
			else if (src->node[i].flags & FLAG_FLAG_PTR)
			{
				flag_set_full(dest, src->node[i].key, src->node[i].flags, src->node[i].vals.ptr.type, 0, flags_pointer_types[src->node[i].vals.ptr.type].dup(src->node[i].vals.ptr.ptr));
			}
			else
			{
				flag_set(dest, src->node[i].key, src->node[i].vals.vals.val);
				flag_learn(dest, src->node[i].key, src->node[i].flags & FLAG_FLAG_KNOWN);
			}
		}
	flag_check_sanity(dest, "flag_add_new()", -1);
}

void flag_add_value_from(flags_type *dest, flags_type *src, flags_type *from)
{
	s32b i;

	/* Initialize if it isnt */
	check_init(dest);
	check_init(src);

	for (i = 0; i < src->size; i++)
		if (src->node[i].flags & FLAG_FLAG_USED)
		{
			/* If both src and from have teh flag add it with teh value from "from" */
			if (flag_exists(from, src->node[i].key))
			{
				flag_set(dest, src->node[i].key, flag_get(from, src->node[i].key));
			}
		}
	flag_check_sanity(dest, "flag_add_value_from()", -1);
}

/* Adds the flags, if they existed, add the values */
void flag_add_increase(flags_type *dest, flags_type *src)
{
	s32b i;

	/* Initialize if it isnt */
	check_init(dest);
	check_init(src);

	flag_check_sanity(dest, "flag_add_increase() dest", -1);
	flag_check_sanity(src, "flag_add_increase() src", -1);

	for (i = 0; i < src->size; i++)
		if (src->node[i].flags & FLAG_FLAG_USED)
		{
			if (src->node[i].flags & FLAG_FLAG_FLAGS)
			{
				flags_type *src_sub = (flags_type*) src->node[i].vals.flags;
				flags_type *dest_sub = flag_get_flags(dest, src->node[i].key);

				if (dest_sub == NULL)
				{
					dest_sub = flag_new(2);

					flag_set_flags(dest, src->node[i].key, dest_sub);
				}

				flag_add_increase(dest_sub, src_sub);
			}
			else
				flag_set(dest, src->node[i].key,
						 flag_get(dest, src->node[i].key) +
						 src->node[i].vals.vals.val);
		}
	flag_check_sanity(dest, "flag_add_increase()", -1);
}

/* Adds the flags, if they existed, add the values */
void flag_add_higher(flags_type *dest, flags_type *src)
{
	s32b i;

	/* Initialize if it isnt */
	check_init(dest);
	check_init(src);

	for (i = 0; i < src->size; i++)
		if (src->node[i].flags & FLAG_FLAG_USED)
		{
			if (src->node[i].flags & FLAG_FLAG_FLAGS)
			{
				flags_type *src_sub = (flags_type*) src->node[i].vals.ptr.ptr;
				flags_type *dest_sub = flag_get_flags(dest, src->node[i].key);

				if (dest_sub == NULL)
				{
					dest_sub = flag_new(2);

					flag_set_flags(dest, src->node[i].key, dest_sub);
				}

				flag_add_higher(dest_sub, src_sub);
			}
			else {
				s32b val = flag_get(dest, src->node[i].key);
				if (val < src->node[i].vals.vals.val)
					val = src->node[i].vals.vals.val;
				flag_set(dest, src->node[i].key, val);
			}
		}
	flag_check_sanity(dest, "flag_add_higher()", -1);
}

void flag_del(flags_type *dest, flags_type *src)
{
	s32b i;

	/* Initialize if it isnt */
	check_init(dest);
	check_init(src);

	for (i = 0; i < src->size; i++)
		if (src->node[i].flags & FLAG_FLAG_USED)
			flag_remove(dest, src->node[i].key);
	flag_check_sanity(dest, "flag_del()", -1);
}

/* Gets a random flag */
u32b flag_get_rand(flags_type *t)
{
	s32b realuse = 0;
	s32b i;
	s32b rand = rand_int(flag_used(t));

	for (i = 0; i < t->size; i++)
	{
		flag_node *n = &t->node[i];

		if (n->flags & FLAG_FLAG_USED)
		{
			if (realuse == rand)
				return n->key;
			realuse++;
		}
	}

	/* Should not happen unless the flagset is empty */
	return 0;
}

void flags_print(flags_type *t)
{
	s32b i;

	printf("<==\n");
	for (i = 0; i < t->size; i++)
	{
		flag_node *n = &t->node[i];

		if (n->flags & FLAG_FLAG_USED)
		{
			flag_registry *flag = find_flag(n->key);
			if (n->flags & FLAG_FLAG_PTR)
			{
				if (n->vals.ptr.type == FLAG_TYPE_DATA_BUFFER)
					printf("%s: %ld[%s] = %s\n", (n->flags & FLAG_FLAG_USED)?"USE":"NOT", n->key, (flag)?flag->name:"???", (char*)n->vals.ptr.ptr);
				else
					printf("%s: %ld[%s] = %p\n", (n->flags & FLAG_FLAG_USED)?"USE":"NOT", n->key, (flag)?flag->name:"???", n->vals.ptr.ptr);
			}
		else
			printf("%s: %ld[%s] = %ld\n", (n->flags & FLAG_FLAG_USED)?"USE":"NOT", n->key, (flag)?flag->name:"???", n->vals.vals.val);
		}
	}
	printf("==>\n");
}

/* We declare them */
#define new_flag(n) \
	u32b FLAG_##n
#include "flags.h"
#undef new_flag

/* We init them */
void setup_flags()
{
#define new_flag(n) \
	FLAG_##n = register_flag(#n)
#include "flags.h"
#undef new_flag
}

static void declare_flag(lua_State* tolua_S, cptr name, u32b val)
{
	char buf[150] = "FLAG_";

	strcat(buf, name);
	tolua_constant(tolua_S, NULL, buf, val);
}


/* Load flags function */
s32b tolua_flags_open (lua_State* tolua_S)
{
	tolua_open(tolua_S);
#define new_flag(n) \
	declare_flag(tolua_S, #n, FLAG_##n)
#include "flags.h"
#undef new_flag
	return 1;
}
