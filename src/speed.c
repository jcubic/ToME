/* File: speed.c */

/* Purpose: multiple speeds processing */

/*
 * Copyright (c) 203 DarkGod
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"
#include "lua/tolua.h"

/* Speed registration */
typedef struct speed_registry
{
	u32b index;
	cptr name;
	cptr desc;

        struct speed_registry *next;
} speed_registry;

static u32b next_speed = 1;
static speed_registry *speeds_registry = NULL;

static speed_registry *find_speed(u32b index)
{
	speed_registry *i = speeds_registry;

	while (i != NULL)
	{
		if (i->index == index)
			return i;
		i = i->next;
	}
	return NULL;
}

static speed_registry *find_speed_by_name(cptr name)
{
	speed_registry *i = speeds_registry;

	while (i != NULL)
	{
		if (!strcmp(i->name, name))
			return i;
		i = i->next;
	}
	return NULL;
}

cptr get_speed_name(u32b index)
{
	speed_registry *i = find_speed(index);

	return (i)?i->name:"???";
}

cptr get_speed_desc(u32b index)
{
	speed_registry *i = find_speed(index);

	return (i)?i->desc:"???";
}

u32b get_speed_index(cptr name)
{
	speed_registry *i = find_speed_by_name(name);

	return (i)?i->index:0;
}

u32b register_speed(cptr name, cptr desc)
{
	speed_registry *n;

	if (find_speed_by_name(name))
		quit(format("Speed redefined!!!! : %s\n", name));

	MAKE(n, speed_registry);

	n->index = next_speed++;
	n->name = strdup(name);
	n->desc = strdup(desc);
	n->next = speeds_registry;
	speeds_registry = n;
	return n->index;
}


/*
 * Get the energy use for the appropriate speed counter
 * skill represents the counter, if skill == -1 then it returns
 * only natural speed
 */
u16b get_player_energy(s32b speed_id)
{
	u16b energy;
	s16b speed = p_ptr->pspeed;

	/* If we need a specific speed, get it */
	if (speed_id != SPEED_GLOBAL)
		speed += flag_get(&p_ptr->speeds, speed_id);

	/* Cap speed */
	if (speed > (MAX_SPEED + 110)) speed = MAX_SPEED + 110;
	else if (speed < (MIN_SPEED + 110)) speed = MIN_SPEED + 110;

	/* And finnaly extract the energy */
	energy = extract_energy[speed];

	return energy;
}

/*
 * Get the energy use for the appropriate speed counter
 * skill represents the counter, if skill == -1 then it returns
 * only natural speed
 */
u16b get_monster_energy(monster_type *m_ptr, s32b speed_id)
{
	u16b energy;
	s16b speed = m_ptr->mspeed;

	/* If we need a specific speed, get it */
	if ((speed_id != SPEED_GLOBAL) && has_flag(m_ptr, FLAG_SPEEDS))
		speed += flag_get(flag_get_flags(&m_ptr->flags, FLAG_SPEEDS), speed_id);

	/* Cap speed */
	if (speed > (MAX_SPEED + 110)) speed = MAX_SPEED + 110;
	else if (speed < (MIN_SPEED + 110)) speed = MIN_SPEED + 110;

	/* And finnaly extract the energy */
	energy = extract_energy[speed];

	return energy;
}

/* We declare them */
#define new_speed(n,d) \
	u32b SPEED_##n
#include "speed.h"
#undef new_speed

/* We init them */
void setup_speeds()
{
#define new_speed(n,d) \
	SPEED_##n = register_speed(#n, d)
#include "speed.h"
#undef new_speed
};

static void declare_speed(lua_State* tolua_S, cptr name, u32b val)
{
	char buf[150] = "SPEED_";

	strcat(buf, name);
	tolua_constant(tolua_S, NULL, buf, val);
}


/* Load speeds function */
s32b tolua_speeds_open (lua_State* tolua_S)
{
	tolua_open(tolua_S);
#define new_speed(n,d) \
	declare_speed(tolua_S, #n, SPEED_##n)
#include "speed.h"
#undef new_speed
	return 1;
}
