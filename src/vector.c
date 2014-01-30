 /* File: vector.c */

/* Purpose: dynamically resizable s32b vectors(actually a simple interface to hashtables) */

/*
 * Copyright (c) 203 DarkGod
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"

void vector_push(vector_type *v, s32b i)
{
	u32b last = flag_max_key(v);
	flag_set(v, last + 1, i);
}

s32b vector_pop(vector_type *v)
{
	u32b last = flag_max_key(v);
	s32b val;
	
	val = flag_get(v, last);
	flag_remove(v, last);
	return val;
}

s32b vector_peek(vector_type *v)
{
	u32b last = flag_max_key(v);
	s32b val;
	
	val = flag_get(v, last);
	return val;
}

u32b vector_size(vector_type *v)
{
	return flag_max_key(v);
}

bool vector_is_empty(vector_type *v)
{
	return (vector_size(v) == 0) ? TRUE : FALSE;
}
