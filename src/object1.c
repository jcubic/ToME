/* File: object1.c */
/* File: object1.c */

/* Purpose: Object code, part 1 */

/*
 * Copyright (c) 1989 James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"
#include "lua.h"
#include "tolua.h"

extern lua_State *L;


/*
 * Hack -- note that "TERM_MULTI" is now just "TERM_VIOLET".
 * We will have to find a cleaner method for "MULTI_HUED" later.
 * There were only two multi-hued "flavors" (one potion, one food).
 * Plus five multi-hued "base-objects" (3 dragon scales, one blade
 * of chaos, and one something else).  See the SHIMMER_OBJECTS code
 * in "dungeon.c" and the object color extractor in "cave.c".
 */
#define TERM_MULTI      TERM_VIOLET


/*
 * Reset the "visual" lists
 *
 * This involves resetting various things to their "default" state.
 *
 * If the "prefs" flag is TRUE, then we will also load the appropriate
 * "user pref file" based on the current setting of the "use_graphics"
 * flag.  This is useful for switching "graphics" on/off.
 *
 * The features, objects, and monsters, should all be encoded in the
 * relevant "font.pref" and/or "graf.prf" files.  XXX XXX XXX
 *
 * The "prefs" parameter is no longer meaningful.  XXX XXX XXX
 */
void reset_visuals(void)
{
	s32b i;

	/* Extract some info about terrain features */
	for (i = 0; i < max_f_idx; i++)
	{
		feature_type *f_ptr = &f_info[i];

		/* Assume we will use the underlying values */
		f_ptr->x_attr = f_ptr->d_attr;
		f_ptr->x_char = f_ptr->d_char;
	}

	/* Extract default attr/char code for stores */
	for (i = 0; i < max_st_idx; i++)
	{
		store_info_type *st_ptr = &st_info[i];

		/* Default attr/char */
		st_ptr->x_attr = st_ptr->d_attr;
		st_ptr->x_char = st_ptr->d_char;
	}

	/* Extract default attr/char code for objects */
	for (i = 0; i < max_k_idx; i++)
	{
		object_kind *k_ptr = &k_info[i];

		/* Default attr/char */
		k_ptr->x_attr = k_ptr->d_attr;
		k_ptr->x_char = k_ptr->d_char;
	}

	/* Extract default attr/char code for monsters */
	for (i = 0; i < max_r_idx; i++)
	{
		monster_race *r_ptr = &r_info[i];

		/* Default attr/char */
		r_ptr->x_attr = r_ptr->d_attr;
		r_ptr->x_char = r_ptr->d_char;
	}

	/* Reset attr/char code for ego monster overlay graphics */
	for (i = 0; i < max_re_idx; i++)
	{
		monster_ego *re_ptr = &re_info[i];

		/* Default attr/char */
		re_ptr->g_attr = 0;
		re_ptr->g_char = 0;
	}

	/* Reset attr/char code for race modifier overlay graphics */
#if 0 // DGDGDGDGDG
	for (i = 0; i < max_rmp_idx; i++)
	{
		player_race_mod *rmp_ptr = &race_mod_info[i];

		/* Default attr/char */
		rmp_ptr->g_attr = 0;
		rmp_ptr->g_char = 0;
	}
#endif

	if (use_graphics)
	{
		/* Process "graf.prf" */
		process_pref_file("graf.prf");

		/*
		 * Hack -- remember graphics mode as an integer value,
		 * for faster processing of map_info()
		 */

		/* IBM-PC pseudo-graphics -- not maintained, but the code is there */
		if (streq(ANGBAND_SYS, "ibm"))
		{
			graphics_mode = GRAPHICS_IBM;
		}

		/*
		 * Isometric view. Also assumes all the attributes of the "new"
		 * graphics.
		 */
		else if (streq(TOME_GRAPHICS, "iso"))
		{
			graphics_mode = GRAPHICS_ISO;
		}

		/*
		 * "32x32" graphics -- supports graphics overlay for traps, ego monsters
		 * and player subraces, and has tiles for lighting effects (row + 1
		 * and row + 2 for "darker" versions of terrain features)
		 */
		else if (streq(TOME_GRAPHICS, "32"))
		{
			graphics_mode = GRAPHICS_32X32;
		}

		/*
		 * "16x16" graphics -- supports graphics overlay for traps, ego monsters
		 * and player subraces, and has tiles for lighting effects (row + 1
		 * and row + 2 for "darker" versions of terrain features)
		 */
		else if (streq(TOME_GRAPHICS, "16"))
		{
			graphics_mode = GRAPHICS_16X16;
		}

		/*
		 * "8x8" graphics -- doesn't support graphics overlay and lighting
		 * effects
		 */
		else if (streq(TOME_GRAPHICS, "8"))
		{
			graphics_mode = GRAPHICS_8X8;
		}

		/* ??? */
		else
		{
			graphics_mode = GRAPHICS_UNKNOWN;
		}
	}

	/* Normal symbols */
	else
	{
		/* Process "font.prf" */
		process_pref_file("font.prf");

		graphics_mode = GRAPHICS_NONE;
	}
}

/* Return object granted power */
s32b object_power(object_type *o_ptr)
{
	object_kind *k_ptr = &k_info[o_ptr->k_idx];
	s32b power = -1;
	s32b i;

	/* Base object */
	power = k_ptr->power;

	/* Ego-item */
	for (i = 0; i < MAX_EGO_PER_OBJ; i++)
	{
		if (o_ptr->ego_id[i])
		{
			ego_item_type *e_ptr = &e_info[o_ptr->ego_id[i]];

			if (power == -1) power = e_ptr->power;
		}
	}

	/* Artifact */
	if (o_ptr->artifact_id)
	{
		artifact_type *a_ptr = &a_info[o_ptr->artifact_id];

		if (power == -1) power = a_ptr->power;
	}

	return (power);
}




/*
 * Creates a description of the item "o_ptr", and stores it in "out_val".
 *
 * One can choose the "verbosity" of the description, including whether
 * or not the "number" of items should be described, and how much detail
 * should be used when describing the item.
 *
 * The given "buf" must be 80 chars long to hold the longest possible
 * description, which can get pretty long, including incriptions, such as:
 * "no more Maces of Disruption (Defender) (+10,+10) [+5] (+3 to stealth)".
 * Note that the inscription will be clipped to keep the total description
 * under 79 chars (plus a terminator).
 *
 * Note the use of "object_desc_num()" and "object_desc_int()" as hyper-efficient,
 * portable, versions of some common "sprintf()" commands.
 *
 * Note that all ego-items (when known) append an "Ego-Item Name", unless
 * the item is also an artifact, which should NEVER happen.
 *
 * Note that all artifacts (when known) append an "Artifact Name", so we
 * have special processing for "Specials" (artifact Lites, Rings, Amulets).
 * The "Specials" never use "modifiers" if they are "known", since they
 * have special "descriptions", such as "The Necklace of the Dwarves".
 *
 * Special Lite's use the "k_info" base-name (Phial, Star, or Arkenstone),
 * plus the artifact name, just like any other artifact, if known.
 *
 * Special Ring's and Amulet's, if not "aware", use the same code as normal
 * rings and amulets, and if "aware", use the "k_info" base-name (Ring or
 * Amulet or Necklace).  They will NEVER "append" the "k_info" name.  But,
 * they will append the artifact name, just like any artifact, if known.
 *
 * None of the Special Rings/Amulets are "EASY_KNOW", though they could be,
 * at least, those which have no "pluses", such as the three artifact lites.
 *
 * Hack -- Display "The One Ring" as "a Plain Gold Ring" until aware.
 *
 * If "pref" then a "numeric" prefix will be pre-pended.
 *
 * Mode:
 *   0 -- The Cloak of Death
 *   1 -- The Cloak of Death [1,+3]
 *   2 -- The Cloak of Death [1,+3] (+2 to Stealth)
 *   3 -- The Cloak of Death [1,+3] (+2 to Stealth) {nifty}
 */
void object_desc(char *buf, object_type *o_ptr, s32b pref, s32b mode)
{
	cptr desc;

	call_lua("object_desc", "(O,d,d)", "s", o_ptr, pref, mode, &desc);
	strnfmt(buf, 80, "%s", desc);
}


/*
 * Hack -- describe an item currently in a store's inventory
 * This allows an item to *look* like the player is "aware" of it
 */
void object_desc_store(char *buf, object_type *o_ptr, s32b pref, s32b mode)
{
	/* Save the "aware" flag */
	bool hack_aware = object_aware_p(o_ptr);

	/* Save the "known" flag */
	bool hack_known = (o_ptr->ident & (IDENT_KNOWN)) ? TRUE : FALSE;


	/* Set the "known" flag */
	o_ptr->ident |= (IDENT_KNOWN);

	/* Force "aware" for description */
	object_aware(o_ptr);

	/* Describe the object */
	object_desc(buf, o_ptr, pref, mode);


	/* Restore "aware" flag */
	if (!hack_aware) object_unaware(o_ptr);

	/* Clear the known flag */
	if (!hack_known) o_ptr->ident &= ~(IDENT_KNOWN);
}



/* Grab the tval desc */
bool grab_tval_desc(s32b tval)
{
	cptr desc = NULL;

	call_lua("__k_info.get_tval_info", "(s,d)", "s", "desc", tval, &desc);

	if (desc == NULL) return FALSE;

	text_out_c(TERM_L_BLUE, desc);
	text_out("\n");

	return TRUE;
}

#define CHECK_FIRST(txt, first) \
if ((first)) { (first) = FALSE; text_out((txt)); } else text_out(", ");

/*
 * Display the damage done with a multiplier
 */
void output_dam(object_type *o_ptr, s32b mult, s32b mult2, cptr against, cptr against2, bool *first)
{
	s32b dam;

	dam = (o_ptr->dd + (o_ptr->dd * o_ptr->ds)) * 5 * mult;
	dam += (o_ptr->to_d + p_ptr->to_d) * 10;
	dam *= p_ptr->num_blow;
	CHECK_FIRST("", *first);
	if (dam > 0)
	{
		if (dam % 10)
			text_out_c(TERM_L_GREEN, format("%d.%d", dam / 10, dam % 10));
		else
			text_out_c(TERM_L_GREEN, format("%d", dam / 10));
	}
	else
		text_out_c(TERM_L_RED, "0");
	text_out(format(" against %s", against));

	if (mult2)
	{
		dam = (o_ptr->dd + (o_ptr->dd * o_ptr->ds)) * 5 * mult2;
		dam += (o_ptr->to_d + p_ptr->to_d) * 10;
		dam *= p_ptr->num_blow;
		CHECK_FIRST("", *first);
		if (dam > 0)
		{
			if (dam % 10)
				text_out_c(TERM_L_GREEN, format("%d.%d", dam / 10, dam % 10));
			else
				text_out_c(TERM_L_GREEN, format("%d", dam / 10));
		}
		else
			text_out_c(TERM_L_RED, "0");
		text_out(format(" against %s", against2));
	}
}

/*
 * Outputs the damage we do/would do with the weapon
 */
void display_weapon_damage(object_type *o_ptr)
{
#if 0 // DGDGDGDG
	object_type *old_ptr;
	bool first = TRUE;
	bool full = o_ptr->ident & (IDENT_MENTAL);

	/* Ok now the hackish stuff, we replace the current weapon with this one */
	old_ptr = p_ptr->inventory[INVEN_WIELD];
	p_ptr->inventory[INVEN_WIELD] = o_ptr;
	calc_bonuses(TRUE);

	text_out("\nUsing it you would have ");
	text_out_c(TERM_L_GREEN, format("%d ", p_ptr->num_blow));
	text_out(format("blow%s and do an average damage per turn of ", (p_ptr->num_blow) ? "s" : ""));

	if (full && has_flag(o_ptr, FLAG_SLAY_ANIMAL)) output_dam(o_ptr, get_flag(o_ptr, FLAG_SLAY_ANIMAL), 0, "animals", NULL, &first);
	if (full && has_flag(o_ptr, FLAG_SLAY_EVIL)) output_dam(o_ptr, get_flag(o_ptr, FLAG_SLAY_EVIL), 0, "evil creatures", NULL, &first);
	if (full && has_flag(o_ptr, FLAG_SLAY_ORC)) output_dam(o_ptr, get_flag(o_ptr, FLAG_SLAY_ORC), 0, "orcs", NULL, &first);
	if (full && has_flag(o_ptr, FLAG_SLAY_TROLL)) output_dam(o_ptr, get_flag(o_ptr, FLAG_SLAY_TROLL), 0, "trolls", NULL, &first);
	if (full && has_flag(o_ptr, FLAG_SLAY_GIANT)) output_dam(o_ptr, get_flag(o_ptr, FLAG_SLAY_GIANT), 0, "giants", NULL, &first);
	if (full && has_flag(o_ptr, FLAG_SLAY_DRAGON)) output_dam(o_ptr, get_flag(o_ptr, FLAG_SLAY_DRAGON), 0, "dragons", NULL, &first);
	if (full && has_flag(o_ptr, FLAG_SLAY_UNDEAD)) output_dam(o_ptr, get_flag(o_ptr, FLAG_SLAY_UNDEAD), 0, "undead", NULL, &first);
	if (full && has_flag(o_ptr, FLAG_SLAY_DEMON)) output_dam(o_ptr, get_flag(o_ptr, FLAG_SLAY_DEMON), 0, "demons", NULL, &first);

	if (full && has_flag(o_ptr, FLAG_BRAND_FIRE)) output_dam(o_ptr, get_flag(o_ptr, FLAG_BRAND_FIRE), 2 * get_flag(o_ptr, FLAG_BRAND_FIRE), "non fire resistant creatures", "fire susceptible creatures", &first);
	if (full && has_flag(o_ptr, FLAG_BRAND_COLD)) output_dam(o_ptr, get_flag(o_ptr, FLAG_BRAND_COLD), 2 * get_flag(o_ptr, FLAG_BRAND_COLD), "non cold resistant creatures", "cold susceptible creatures", &first);
	if (full && has_flag(o_ptr, FLAG_BRAND_ELEC)) output_dam(o_ptr, get_flag(o_ptr, FLAG_BRAND_ELEC), 2 * get_flag(o_ptr, FLAG_BRAND_ELEC), "non lightning resistant creatures", "lightning susceptible creatures", &first);
	if (full && has_flag(o_ptr, FLAG_BRAND_ACID)) output_dam(o_ptr, get_flag(o_ptr, FLAG_BRAND_ACID), 2 * get_flag(o_ptr, FLAG_BRAND_ACID), "non acid resistant creatures", "acid susceptible creatures", &first);
	if (full && has_flag(o_ptr, FLAG_BRAND_POIS)) output_dam(o_ptr, get_flag(o_ptr, FLAG_BRAND_POIS), 2 * get_flag(o_ptr, FLAG_BRAND_POIS), "non poison resistant creatures", "poison susceptible creatures", &first);

	output_dam(o_ptr, 1, 0, (first) ? "all monsters" : "other monsters", NULL, &first);

	text_out(".");

	/* get our weapon back */
	p_ptr->inventory[INVEN_WIELD] = old_ptr;
	calc_bonuses(TRUE);
#endif
}

/*
 * Display the ammo damage done with a multiplier
 */
void output_ammo_dam(object_type *o_ptr, s32b mult, s32b mult2, cptr against, cptr against2, bool *first)
{
#if 0 // DGDGDGDGDG tss lua me !
	s32b dam;
	object_type *b_ptr = get_object_split(INVEN_BOW, 0);
	s32b is_boomerang;
	s32b tmul;

	if (!b_ptr) return;

	tmul = get_flag(b_ptr, get_flag_index("MULTIPLIER")) + intrinsic(XTRA_MIGHT);


	dam = (o_ptr->dd + (o_ptr->dd * o_ptr->ds)) * 5;
	dam += o_ptr->to_d * 10;
	if (!is_boomerang) dam += b_ptr->to_d * 10;
	dam *= tmul;
	if (!is_boomerang) dam += (p_ptr->to_d_ranged) * 10;
	dam *= mult;
	CHECK_FIRST("", *first);
	if (dam > 0)
	{
		if (dam % 10)
			text_out_c(TERM_L_GREEN, format("%d.%d", dam / 10, dam % 10));
		else
			text_out_c(TERM_L_GREEN, format("%d", dam / 10));
	}
	else
		text_out_c(TERM_L_RED, "0");
	text_out(format(" against %s", against));

	if (mult2)
	{
		dam = (o_ptr->dd + (o_ptr->dd * o_ptr->ds)) * 5;
		dam += o_ptr->to_d * 10;
		if (!is_boomerang) dam += b_ptr->to_d * 10;
		dam *= tmul;
		if (!is_boomerang) dam += (p_ptr->to_d_ranged) * 10;
		dam *= mult2;
		CHECK_FIRST("", *first);
		if (dam > 0)
		{
			if (dam % 10)
				text_out_c(TERM_L_GREEN, format("%d.%d", dam / 10, dam % 10));
			else
				text_out_c(TERM_L_GREEN, format("%d", dam / 10));
		}
		else
			text_out_c(TERM_L_RED, "0");
		text_out(format(" against %s", against2));
	}
#endif
}

/*
 * Outputs the damage we do/would do with the current bow and this ammo
 */
void display_ammo_damage(object_type *o_ptr)
{
#if 0 // DGDGDGDG -- tss lua me
	bool first = TRUE;
	bool full = o_ptr->ident & (IDENT_MENTAL);

	if (o_ptr->tval == TV_BOOMERANG)
		text_out("\nUsing it you would do an average damage per throw of ");
	else
		text_out("\nUsing it with your current shooter you would do an average damage per shot of ");

	if (full && has_flag(o_ptr, FLAG_SLAY_ANIMAL)) output_ammo_dam(o_ptr, get_flag(o_ptr, FLAG_SLAY_ANIMAL), 0, "animals", NULL, &first);
	if (full && has_flag(o_ptr, FLAG_SLAY_EVIL)) output_ammo_dam(o_ptr, get_flag(o_ptr, FLAG_SLAY_EVIL), 0, "evil creatures", NULL, &first);
	if (full && has_flag(o_ptr, FLAG_SLAY_ORC)) output_ammo_dam(o_ptr, get_flag(o_ptr, FLAG_SLAY_ORC), 0, "orcs", NULL, &first);
	if (full && has_flag(o_ptr, FLAG_SLAY_TROLL)) output_ammo_dam(o_ptr, get_flag(o_ptr, FLAG_SLAY_TROLL), 0, "trolls", NULL, &first);
	if (full && has_flag(o_ptr, FLAG_SLAY_GIANT)) output_ammo_dam(o_ptr, get_flag(o_ptr, FLAG_SLAY_GIANT), 0, "giants", NULL, &first);
	if (full && has_flag(o_ptr, FLAG_SLAY_DRAGON)) output_ammo_dam(o_ptr, get_flag(o_ptr, FLAG_SLAY_DRAGON), 0, "dragons", NULL, &first);
	if (full && has_flag(o_ptr, FLAG_SLAY_UNDEAD)) output_ammo_dam(o_ptr, get_flag(o_ptr, FLAG_SLAY_UNDEAD), 0, "undead", NULL, &first);
	if (full && has_flag(o_ptr, FLAG_SLAY_DEMON)) output_ammo_dam(o_ptr, get_flag(o_ptr, FLAG_SLAY_DEMON), 0, "demons", NULL, &first);

	if (full && has_flag(o_ptr, FLAG_BRAND_FIRE)) output_ammo_dam(o_ptr, get_flag(o_ptr, FLAG_BRAND_FIRE), 2 * get_flag(o_ptr, FLAG_BRAND_FIRE), "non fire resistant creatures", "fire susceptible creatures", &first);
	if (full && has_flag(o_ptr, FLAG_BRAND_COLD)) output_ammo_dam(o_ptr, get_flag(o_ptr, FLAG_BRAND_COLD), 2 * get_flag(o_ptr, FLAG_BRAND_COLD), "non cold resistant creatures", "cold susceptible creatures", &first);
	if (full && has_flag(o_ptr, FLAG_BRAND_ELEC)) output_ammo_dam(o_ptr, get_flag(o_ptr, FLAG_BRAND_ELEC), 2 * get_flag(o_ptr, FLAG_BRAND_ELEC), "non lightning resistant creatures", "lightning susceptible creatures", &first);
	if (full && has_flag(o_ptr, FLAG_BRAND_ACID)) output_ammo_dam(o_ptr, get_flag(o_ptr, FLAG_BRAND_ACID), 2 * get_flag(o_ptr, FLAG_BRAND_ACID), "non acid resistant creatures", "acid susceptible creatures", &first);
	if (full && has_flag(o_ptr, FLAG_BRAND_POIS)) output_ammo_dam(o_ptr, get_flag(o_ptr, FLAG_BRAND_POIS), 2 * get_flag(o_ptr, FLAG_BRAND_POIS), "non poison resistant creatures", "poison susceptible creatures", &first);

	output_ammo_dam(o_ptr, 1, 0, (first) ? "all monsters" : "other monsters", NULL, &first);
	text_out(". ");

	if (has_flag(o_ptr, FLAG_EXPLODE))
	{
		cptr name;

		call_lua("get_dam_type_info", "(d,s)", "s", get_flag(o_ptr, FLAG_EXPLODE), "desc", &name);

		text_out("The explosion will be ");
		text_out_c(TERM_L_GREEN, name);
		text_out(".");
	}
#endif
}

/*
 * Describe a magic stick powers
 */
void describe_device(object_type *o_ptr)
{
#if 0 // DGDGDGDGDG -- tss lua me
	/* Wands/... of shcool spell */
	if (((o_ptr->tval == TV_WAND) || (o_ptr->tval == TV_STAFF)) && object_known_p(o_ptr))
	{
		s32b spell;
		call_lua("spell_x", "(O,d)", "d", o_ptr, 0, &spell);

		/* Enter device mode  */
		set_stick_mode(o_ptr);

		text_out("\nSpell description:");
		exec_lua(format("print_device_desc(%d)", spell));

		text_out("\nSpell level: ");
		text_out_c(TERM_L_BLUE, string_exec_lua(format("return tostring(get_level(%d, 50, 0))", spell)));

		text_out("\nMinimum Magic Device level to increase spell level: ");
		text_out_c(TERM_L_BLUE, format("%d", school_spells[spell].skill_level));

		text_out("\nSpell fail: ");
		text_out_c(TERM_GREEN, string_exec_lua(format("return tostring(spell_chance(%d))", spell)));

		text_out("\nSpell info: ");
		text_out_c(TERM_YELLOW, string_exec_lua(format("return __spell_info[%d]()", spell)));

		/* Leave device mode  */
		unset_stick_mode();

		text_out("\n");
	}
#endif
}


/*
 * Helper for object_out_desc
 *
 * Print the level something was found on
 *
 */
static cptr object_out_desc_where_found(s16b level, s16b dungeon)
{
	static char str[80];

	char name[80];

	flags_type *flags;
	cptr       fmt;
	cptr       name_pos, level_pos;
	s32b       mindepth = 0;

	if (dungeon == DUNGEON_WILDERNESS)
	{
		flags = &wf_info[level].flags;
		fmt   = "%s";

		/* Taking care of older objects */
		if (level == 0)
			sprintf(name, "in the wilderness or in a town");
		else if (wf_info[level].terrain_idx == TERRAIN_TOWN)
			sprintf(name, "in the town of %s", wf_info[level].name);
		else
			sprintf(name, "in %s", wf_info[level].text);
	}
	else
	{
		flags     = &d_info[dungeon].flags;
		mindepth = d_info[dungeon].mindepth;

		sprintf(name, "%s", d_info[dungeon].name);
		fmt = "on level %d of %s";
	}

	if (dungeon != DUNGEON_WILDERNESS)
		level = level - mindepth + 1;
	if (flag_exists(flags, FLAG_TOWER))
		level = -level;

	if (flag_exists(flags, FLAG_OBJ_FOUND))
		fmt = flag_get_string(flags, FLAG_OBJ_FOUND);

	name_pos  = strstr(fmt, "%s");
	level_pos = strstr(fmt, "%d");

	if (name_pos == NULL && level_pos == NULL)
		strcpy(str, fmt);
	else if (name_pos != NULL && level_pos == NULL)
		sprintf(str, fmt, name);
	else if (name_pos == NULL && level_pos != NULL)
		sprintf(str, fmt, level);
	else
	{
		if (name_pos < level_pos)
			sprintf(str, fmt, name, level);
		else
			sprintf(str, fmt, level, name);
	}

	return str;
}

#define display_pvaled_flag(o_ptr, flag, name) \
	if (has_flag(o_ptr, flag)) { vv[vn] = get_flag(o_ptr, flag); vp[vn++] = name; }

/*
 * Describe an item
 */
bool object_out_desc(object_type *q_ptr, PHYSFS_file *fff, bool trim_down, bool wait_for_it)
{
	object_type *o_ptr = object_dup(q_ptr);
	bool require_id_message = TRUE;

	if (fff)
	{
		/* Set up stuff for text_out */
		text_out_file = fff;
		text_out_hook = text_out_to_file;
	}
	else
	{
		/* Save the screen */
		character_icky++;
		Term_save();

		/* Set up stuff for text_out */
		text_out_hook = text_out_to_screen;
		text_out("\n");
	}

	/* No need to dump that */
	if (!fff)
	{
		if (!trim_down) grab_tval_desc(o_ptr->tval);
	}

	if (object_known_p(o_ptr))
	{
		if (o_ptr->k_idx && (!trim_down))
		{
			object_kind *k_ptr = &k_info[o_ptr->k_idx];

			text_out_c(TERM_ORANGE, k_ptr->text);
			text_out("\n");
		}

		if (o_ptr->artifact_id && (!trim_down))
		{
			artifact_type *a_ptr = &a_info[o_ptr->artifact_id];

			text_out_c(TERM_YELLOW, a_ptr->text);
			text_out("\n");

			if (a_ptr->set != -1)
			{
				set_type *set_ptr = &set_info[a_ptr->set];

				text_out_c(TERM_GREEN, set_ptr->desc);
				text_out("\n");
			}
		}
	}

	process_hooks(HOOK_OBJECT_DESC, "(O,d,d)", o_ptr, (fff != NULL) ? TRUE : FALSE, trim_down);
	get_lua_var("object_desc_configuration.details_require_id_message", 'b', &require_id_message);

	if (require_id_message && (!trim_down && !fff))
	{
		if (!object_known_p(o_ptr))
			text_out("\nYou might need to identify the item to know some more about it...");
		else if (!(o_ptr->ident & (IDENT_MENTAL)))
			text_out("\nYou might need to *identify* the item to know more about it...");
	}

	/* Copying how others seem to do it. -- neil */
	if (!trim_down || (ego_item_p(o_ptr)) || (artifact_p(o_ptr)))
	{
		/* Where did we found it ? */
		if (has_flag(o_ptr, FLAG_OBJ_FOUND))
		{
			text_out(format("\n%s",
							flag_get_string(&o_ptr->flags, FLAG_OBJ_FOUND)));
		}
		else if (o_ptr->found == OBJ_FOUND_MONSTER)
		{
			char m_name[80];

			monster_race_desc(m_name, o_ptr->found_aux1, o_ptr->found_aux2);
			text_out(format("\nYou found it in the remains of %s %s.",
			                m_name, object_out_desc_where_found(o_ptr->found_aux4, o_ptr->found_aux3)));
		}
		else if (o_ptr->found == OBJ_FOUND_FLOOR)
		{
			text_out(format("\nYou found it lying on the ground %s.",
			                object_out_desc_where_found(o_ptr->found_aux2, o_ptr->found_aux1)));
		}
		else if (o_ptr->found == OBJ_FOUND_VAULT)
		{
			text_out(format("\nYou found it lying in a vault %s.",
			                object_out_desc_where_found(o_ptr->found_aux2, o_ptr->found_aux1)));
		}
		else if (o_ptr->found == OBJ_FOUND_SPECIAL)
		{
			text_out("\nYou found it lying on the floor of a special level.");
		}
		else if (o_ptr->found == OBJ_FOUND_RUBBLE)
		{
			text_out("\nYou found it while digging a rubble.");
		}
		else if (o_ptr->found == OBJ_FOUND_REWARD)
		{
			text_out("\nIt was given to you as a reward.");
		}
		else if (o_ptr->found == OBJ_FOUND_STORE)
		{
			cptr name = st_info[o_ptr->found_aux1].name;

			/*
			 * If the store is named "The Store", say "You bought it from
			 * The Store", not "You bought it from the The Store."
			 */
			if (strstr(name, "the ") == name ||
				strstr(name, "The ") == name)
				text_out(format("\nYou bought it from %s.",
								st_info[o_ptr->found_aux1].name ));
			else
				text_out(format("\nYou bought it from the %s.",
								st_info[o_ptr->found_aux1].name ));
		}
		else if (o_ptr->found == OBJ_FOUND_STOLEN)
		{
			text_out(format("\nYou stole it from the %s.",
			                st_info[o_ptr->found_aux1].name ));
		}
		else if (o_ptr->found == OBJ_FOUND_SELFMADE)
		{
			text_out("\nYou made it yourself.");
		}
		else if (o_ptr->found == OBJ_FOUND_WISH)
		{
			text_out(format("\nYou wished for it %s.",
			                object_out_desc_where_found(o_ptr->found_aux2,
														o_ptr->found_aux1)));
		}
		else if (o_ptr->found == OBJ_FOUND_DEBUG)
		{
			text_out("\nYou created it using a debug command.");
		}
		else if (o_ptr->found == OBJ_FOUND_START)
		{
			text_out("\nYou started the game with it.");
		}

		/* useful for debugging
		else
	{
			text_out("\nYou ordered it from a catalog in the Town.");
	}*/
	}

	if (fff)
	{
		/* Flush the line position. */
		text_out("\n");
		text_out_file = NULL;
	}
	else
	{
		if (wait_for_it)
		{
			/* Wait for it */
			inkey();

			/* Restore the screen */
			Term_load();
		}
		character_icky--;

	}

	/* Reset stuff for text_out */
	text_out_hook = text_out_to_screen;

	delete_object(o_ptr);

	/* Gave knowledge */
	return (TRUE);
}



/*
 * Convert an inventory index into a one character label
 * Note that the label does NOT distinguish inven/equip.
 */
char index_to_label(s32b item)
{
	s32b inven = item_slot_to_inven(item);
	s32b idx = item_slot_to_item(item);
	s32b i = 0;

	/* Indexes only for inventory, easy! */
	if (inven == INVEN_INVEN)
	{
		return (I2A(idx - 1));
	}

	/* Indexes for "equip" are a tad harder */
	for_inventory(p_ptr, o_ptr, INVEN_PACK, INVEN_TOTAL);
	{
		/* Found it ? Stop counting */
		if (i == item_slot)
			break;
		i++;
	}
	end_inventory();

	return (I2A(i - 1));
}


/*
 * Convert a label into the index of an item in the "inven"
 * Return "-1" if the label does not indicate a real item
 */
s16b label_to_inven(s32b c)
{
	s32b i;

	/* Convert */
	i = (islower(c) ? A2I(c) : -1);

	/* Verify the index */
	if ((i < 0) || (i >= INVEN_MAX_SIZE)) return ( -1);

	/* Empty slots can never be chosen */
	if (!get_object_split(INVEN_INVEN, i)) return ( -1);

	/* Return the index */
	return (i);
}


/*
 * Convert a label into the index of a item in the "equip"
 * Return "-1" if the label does not indicate a real item
 */
s16b label_to_equip(s32b c)
{
	s32b i;

	/* Convert */
	i = ((islower(c) || (c > 'z')) ? A2I(c) : -1);

	/* Verify the index */
	if (i < 0) return ( -1);

	for_inventory(p_ptr, o_ptr, INVEN_PACK, INVEN_TOTAL);
	{
		if (i == 0) return (item_slot);
	}
	end_inventory();

	/* Return the index */
	return (-1);
}

/*
 * Returns the next free slot of the given "type", return the first
 * if all are used
 */
s32b get_slot(s32b slot)
{
	s32b i
	;

	/* If there are at least one body part corretsonding, the find the free one */
	if (inventory_limit(slot))
	{
		/* Find a free body part */
		i = 1;
		while ((i < INVEN_MAX_SIZE) && (i <= inventory_limit(slot)))
		{
			/* Free ? return the slot */
			if (!get_object_split(slot, i)) return compute_slot(slot, i);


			i++;
		}
		/* Found nothing ? return the first one */
		return compute_slot(slot, 1);
	}
	/* No body parts ? return -1 */
	else return (-1);
}

/*
 * Determine which equipment slot (if any) an item likes, ignoring the player's
 * current body and stuff if ideal == TRUE
 */
s16b wield_slot_ideal(object_type *o_ptr, bool ideal)
{
	/* Try for a script first */
	if (process_hooks_ret(HOOK_WIELD_SLOT, "d", "(O,b)", o_ptr, ideal))
		return process_hooks_return[0].num;

	/* No slot available */
	return ( -1);
}

/*
 * Determine which equipment slot (if any) an item likes for the player's
 * current body and stuff
 */
s16b wield_slot(object_type *o_ptr)
{
	return wield_slot_ideal(o_ptr, FALSE);
}

/*
 * Return a string mentioning how a given item is carried
 */
cptr mention_use(s32b i)
{
	/* Return the result */
	return (inventory_slot_names[item_slot_to_inven(i)].name);
}


/*
 * Return a string describing how a given item is being worn.
 * Currently, only used for items in the equipment, not inventory.
 */
cptr describe_use(s32b i)
{
	/* Return the result */
	return (inventory_slot_names[item_slot_to_inven(i)].describe);
}

/*
 * Check an item against the item tester info
 */
bool item_tester_okay(object_type *o_ptr, s32b item)
{
	/* Hack -- allow listing empty slots */
	if (item_tester_full) return (TRUE);

	/* Require an item */
	if ((!o_ptr) || (!o_ptr->k_idx)) return (FALSE);

	/* Hack -- ignore "gold" */
	if (has_flag(o_ptr, FLAG_GOLD_VALUE)) return (FALSE);

	/* Check the tval */
	if (item_tester_tval)
	{
		if (!(item_tester_tval == o_ptr->tval)) return (FALSE);
	}

	/* Check the hook */
	if (item_tester_hook)
	{
		if (!(*item_tester_hook)(o_ptr, item)) return (FALSE);
	}

	/* Assume okay */
	return (TRUE);
}




void show_equip_aux(bool mirror, bool everything);
void show_inven_aux(bool mirror, bool everything);

/*
 * Choice window "shadow" of the "show_inven()" function
 */
void display_inven(void)
{
	show_inven_aux(TRUE, inventory_no_move);
}



/*
 * Choice window "shadow" of the "show_equip()" function
 */
void display_equip(void)
{
	show_equip_aux(TRUE, inventory_no_move);
}



/* Get the color of the letter idx */
byte get_item_letter_color(object_type *o_ptr)
{
	byte color = TERM_WHITE;

	/* Must have knowlegde */
	if (!object_known_p(o_ptr)) return (TERM_SLATE);

	if (ego_item_p(o_ptr)) color = TERM_L_BLUE;
	if (artifact_p(o_ptr)) color = TERM_YELLOW;
	if (o_ptr->artifact_id && ( -1 != a_info[o_ptr->artifact_id].set)) color = TERM_GREEN;
	if (o_ptr->artifact_id && has_flag(&a_info[o_ptr->artifact_id], FLAG_ULTIMATE) && (o_ptr->ident & (IDENT_MENTAL))) color = TERM_VIOLET;

	return (color);
}


/*
 * Display the inventory.
 *
 * Hack -- do not display "trailing" empty slots
 */
void show_inven_aux(bool mirror, bool everything)
{
#if 1
	call_lua("__core_objects.display_inventory", "(n,d,d,d,d)", "", INVEN_INVEN, INVEN_INVEN, mirror, everything);
#else
	s32b i, j, k, l;
	s32b row, col, len, lim;
	s32b wid, hgt;
	object_type *o_ptr;
	char o_name[80];
	char tmp_val[80];
	s32b out_index[INVEN_MAX_SIZE];
	byte out_color[INVEN_MAX_SIZE];
	char out_desc[INVEN_MAX_SIZE][80];

	/* Retrive current screen size */
	Term_get_size(&wid, &hgt);

	/* Starting row */
	row = mirror ? 0 : 1;

	/* Starting column */
	col = mirror ? 0 : 50;

	/* Default "max-length" */
	len = 79 - col;

	/* Maximum space allowed for descriptions */
	lim = 79 - 3;

	/* Require space for weight (if needed) */
	if (show_weights) lim -= 9;

	/* Require space for icon */
	if (show_inven_graph) lim -= 2;

	/* Display the inventory */
	for_inventory(p_ptr, o_ptr, INVEN_INVEN, INVEN_PACK);
	{
		/* Is this item acceptable? */
		if (!item_tester_okay(o_ptr, item_slot))
		{
			if (!everything)
				continue;
			out_index[k] = -1;
		}
		else
		{
			/* Save the object index */
			out_index[k] = item_slot;
		}

		/* Describe the object */
		object_desc(o_name, o_ptr, TRUE, 3);

		/* Hack -- enforce max length */
		o_name[lim] = '\0';

		/* Save the object color, and description */
		out_color[k] = tval_to_attr[o_ptr->tval % 128];
		(void)strcpy(out_desc[k], o_name);

		/* Find the predicted "line length" */
		l = strlen(out_desc[k]) + 5;

		/* Be sure to account for the weight */
		if (show_weights) l += 9;

		/* Account for icon if displayed */
		if (show_inven_graph) l += 2;

		/* Maintain the maximum length */
		if (l > len) len = l;

		/* Advance to next "line" */
		k++;
	}
	end_inventory();

	/* Find the column to start in */
	if (mirror) col = 0;
	else col = (len > wid - 4) ? 0 : (wid - len - 1);

	/* Output each entry */
	for (j = 0; j < k; j++)
	{
		/* Get the index */
		i = out_index[j];

		/* Get the item */
		if (i >= 0)
		{
			o_ptr = get_object(i);

			/* Clear the line */
			prt("", row + j, col ? col - 2 : col);

			/* Prepare an index --(-- */
			sprintf(tmp_val, "%c)", index_to_label(i - 1));

			/* Clear the line with the (possibly indented) index */
			c_put_str(get_item_letter_color(o_ptr), tmp_val, row + j, col);

			/* Display graphics for object, if desired */
			if (show_inven_graph)
			{
				byte a = object_attr(o_ptr);
				char c = object_char(o_ptr);

#ifdef AMIGA
				if (a & 0x80) a |= 0x40;
#endif
				if (!o_ptr->k_idx) c = ' ';

				Term_draw(col + 3, row + j, a, c);
			}


			/* Display the entry itself */
			c_put_str(out_color[j], out_desc[j], row + j,
				show_inven_graph ? (col + 5) : (col + 3));

			/* Display the weight if needed */
			if (show_weights)
			{
				s32b wgt = o_ptr->weight * o_ptr->number;
				(void)sprintf(tmp_val, "%3d.%1d lb", wgt / 10, wgt % 10);
				put_str(tmp_val, row + j, wid - 9);
			}
		}
		else
			/* Display the black entry */
			c_put_str(TERM_DARK, "", row + j, show_inven_graph ? (col + 5) : (col + 3));
	}

	/* Shadow windows */
	if (mirror)
	{
		/* Erase the rest of the window */
		for (j = row + k; j < Term->hgt; j++)
		{
			/* Erase the line */
			Term_erase(0, j, 255);
		}
	}

	/* Main window */
	else
	{
		/* Make a "shadow" below the list (only if needed) */
		if (j && (j < 23)) prt("", row + j, col ? col - 2 : col);
	}
#endif
}


void show_inven(bool mirror)
{
	show_inven_aux(mirror, FALSE);
}

void show_equip(bool mirror)
{
	show_equip_aux(mirror, FALSE);
}

/*
 * Display the equipment.
 */
void show_equip_aux(bool mirror, bool everything)
{
#if 1
	call_lua("__core_objects.display_inventory", "(n,d,d,d,d)", "", INVEN_PACK, INVEN_TOTAL-1, mirror, everything);
#else
	s32b i, j, k, l;
	s32b row, col, len, lim, idx;
	s32b wid, hgt;
	object_type *o_ptr;
	char tmp_val[80];
	char o_name[80];
	s32b out_index[INVEN_MAX_SIZE];
	s32b out_rindex[INVEN_MAX_SIZE];
	byte out_color[INVEN_MAX_SIZE];
	char out_desc[INVEN_MAX_SIZE][80];


	/* Retrive current screen size */
	Term_get_size(&wid, &hgt);

	/* Starting row */
	row = mirror ? 0 : 1;

	/* Starting column */
	col = mirror ? 0 : 50;

	/* Maximal length */
	len = 79 - col;

	/* Maximum space allowed for descriptions */
	lim = 79 - 3;

	/* Require space for labels (if needed) */
	if (show_labels) lim -= (14 + 2);

	/* Require space for weight (if needed) */
	if (show_weights) lim -= 9;

	if (show_equip_graph) lim -= 2;

	/* Scan the equipment list */
	idx = 0;
	for_inventory(p_ptr, o_ptr, INVEN_WIELD, INVEN_TOTAL);
	{
		/* Is there actualy a body part here ? */
		if (!inventory_limit(inven_nb)) continue;

		/* Mega Hack -- don't show symbiote slot if we can't use it */
		if ((i == INVEN_CARRY) && (!get_skill(SKILL_SYMBIOTIC))) continue;
#if 0 // DGDGDGDG
		/* Note that there are two-handed bows now, too -- Neil */
		/* Inform the player that he/she can't use a shield */
		if ((p_ptr->body_parts[i - INVEN_WIELD] == INVEN_ARM) &&
		                !o_ptr->k_idx &&
		                p_ptr->inventory[i - INVEN_ARM + INVEN_WIELD]->k_idx)
		{
			object_type *q_ptr = p_ptr->inventory[i - INVEN_ARM + INVEN_WIELD];
			char q_name[80];

			/* Description */
			object_desc(q_name, q_ptr, TRUE, 3);

			if (has_flag(o_ptr, FLAG_MUST2H))
			{
				sprintf(o_name, "(two handed) %s", q_name);

				/* Truncate the description */
				o_name[lim] = 0;

				/* Save the index */
				out_index[k] = idx;
				out_rindex[k] = i;
				idx++;

				/* Save the color */
				out_color[k] = TERM_L_RED;
				(void)strcpy(out_desc[k], o_name);
				continue;
			}
		}

		if ((p_ptr->body_parts[i - INVEN_WIELD] == INVEN_WIELD) &&
		                !o_ptr->k_idx)
		{
			cptr name;
			call_lua("get_combat_skill_name", "()", "s", &name);
			sprintf(o_name, "(%s)", name);

			/* Truncate the description */
			o_name[lim] = 0;

			/* Save the index */
			out_index[k] = idx;
			out_rindex[k] = i;
			idx++;

			/* Save the color */
			out_color[k] = TERM_L_BLUE;
			(void)strcpy(out_desc[k], o_name);
		}
		else
#endif
		{
			idx++;

			/* Description */
			object_desc(o_name, o_ptr, TRUE, 3);

			/* Truncate the description */
			o_name[lim] = 0;
			/* Is this item acceptable? */
			if (!item_tester_okay(o_ptr, item_slot))
			{
				if (!everything) continue;
				out_index[k] = -1;
			}
			else
			{
				/* Save the index */
				out_index[k] = idx;
			}
			out_rindex[k] = i;

			/* Save the color */
			out_color[k] = tval_to_attr[o_ptr->tval % 128];
			(void)strcpy(out_desc[k], o_name);
		}

		/* Extract the maximal length (see below) */
		l = strlen(out_desc[k]) + (2 + 3);

		/* Increase length for labels (if needed) */
		if (show_labels) l += (14 + 2);

		/* Increase length for weight (if needed) */
		if (show_weights) l += 9;

		if (show_equip_graph) l += 2;

		/* Maintain the max-length */
		if (l > len) len = l;

		/* Advance the entry */
		k++;
	}
	end_inventory();

	/* Hack -- Find a column to start in */
	if (mirror) col = 0;
	else col = (len > wid - 4) ? 0 : (wid - len - 1);

	/* Output each entry */
	for (j = 0; j < k; j++)
	{
		if (j > 20) break;

		/* Get the index */
		i = out_index[j];

		/* Get the item */
		o_ptr = get_object(out_rindex[j]);

		/* Clear the line */
		prt("", row + j, col ? col - 2 : col);

		/* Prepare an index --(-- */
		if (out_index[j] >= 0 )
			sprintf(tmp_val, "%c)", index_to_label(out_rindex[j]));
		else
			sprintf(tmp_val, "  ");

		/* Clear the line with the (possibly indented) index */
		c_put_str(get_item_letter_color(o_ptr), tmp_val, row + j, col);

		if (show_equip_graph)
		{
			byte a = object_attr(o_ptr);
			char c = object_char(o_ptr);

#ifdef AMIGA
			if (a & 0x80) a |= 0x40;
#endif
			if (!o_ptr->k_idx) c = ' ';

			Term_draw(col + 3, row + j, a, c);
		}

		/* Use labels */
		if (show_labels)
		{
			/* Mention the use */
			(void)sprintf(tmp_val, "%-14s: ", mention_use(out_rindex[j]));
			put_str(tmp_val, row + j, show_equip_graph ? col + 5 : col + 3);

			/* Display the entry itself */
			c_put_str(out_color[j], out_desc[j], row + j, show_equip_graph ? col + 21 : col + 19);
		}

		/* No labels */
		else
		{
			/* Display the entry itself */
			c_put_str(out_color[j], out_desc[j], row + j, show_equip_graph ? col + 5 : col + 3);
		}

		/* Display the weight if needed */
		if (show_weights)
		{
			s32b wgt = o_ptr->weight * o_ptr->number;
			(void)sprintf(tmp_val, "%3d.%d lb", wgt / 10, wgt % 10);
			put_str(tmp_val, row + j, wid - 9);
		}
	}


	/* Shadow windows */
	if (mirror)
	{
		/* Erase the rest of the window */
		for (j = row + k; j < Term->hgt; j++)
		{
			/* Erase the line */
			Term_erase(0, j, 255);
		}
	}

	/* Main window */
	else
	{
		/* Make a "shadow" below the list (only if needed) */
		if (j && (j < 23)) prt("", row + j, col ? col - 2 : col);
	}
#endif
}




/*
 * Verify the choice of an item.
 *
 * The item can be negative to mean "item on floor".
 */
bool verify(cptr prompt, s32b item)
{
	char o_name[80];

	char out_val[160];

	object_type *o_ptr;

	o_ptr = get_object(item);

	/* Describe */
	object_desc(o_name, o_ptr, TRUE, 3);

	/* Prompt */
	(void)sprintf(out_val, "%s %s? ", prompt, o_name);

	/* Query */
	return (get_check(out_val));
}


/*
 * Hack -- allow user to "prevent" certain choices
 *
 * The item can be negative to mean "item on floor".
 */
bool get_item_allow(s32b item)
{
	cptr s;

	object_type *o_ptr;

	o_ptr = get_object(item);

	/* No inscription */
	if (!o_ptr || !o_ptr->note) return (TRUE);

	/* Find a '!' */
	s = strchr(quark_str(o_ptr->note), '!');

	/* Process preventions */
	while (s)
	{
		/* Check the "restriction" */
		if ((s[1] == command_cmd) || (s[1] == '*'))
		{
			/* Verify the choice */
			if (!verify("Really try", item)) return (FALSE);
		}

		/* Find another '!' */
		s = strchr(s + 1, '!');
	}

	/* Allow it */
	return (TRUE);
}

/*
 * Find the "first" inventory object with the given "tag".
 *
 * A "tag" is a char "n" appearing as "@n" anywhere in the
 * inscription of an object.
 *
 * Also, the tag "@xn" will work as well, where "n" is a tag-char,
 * and "x" is the "current" command_cmd code.
 */
s32b get_tag(s32b *cp, char tag)
{
	cptr s;

	/* Check every object */
	for_inventory(p_ptr, o_ptr, INVEN_INVEN, INVEN_TOTAL);
	{
		/* Skip empty inscriptions */
		if (!o_ptr->note) continue;

		/* Find a '@' */
		s = strchr(quark_str(o_ptr->note), '@');

		/* Process all tags */
		while (s)
		{
			/* Check the normal tags */
			if (s[1] == tag)
			{
				/* Save the actual inventory ID */
				*cp = item_slot;

				/* Success */
				return (TRUE);
			}

			/* Check the special tags */
			if ((s[1] == command_cmd) && (s[2] == tag))
			{
				/* Save the actual inventory ID */
				*cp = item_slot;

				/* Success */
				return (TRUE);
			}

			/* Find another '@' */
			s = strchr(s + 1, '@');
		}
	}
	end_inventory();

	/* No such tag */
	return (FALSE);
}


/*
 * Let the user select an item, save its "index"
 *
 * Return TRUE only if an acceptable item was chosen by the user.
 *
 * The selected item must satisfy the "item_tester_hook()" function,
 * if that hook is set, and the "item_tester_tval", if that value is set.
 *
 * All "item_tester" restrictions are cleared before this function returns.
 *
 * The user is allowed to choose acceptable items from the equipment,
 * inventory, or floor, respectively, if the proper flag was given,
 * and there are any acceptable items in that location.
 *
 * The equipment or inventory are displayed (even if no acceptable
 * items are in that location) if the proper flag was given.
 *
 * If there are no acceptable items available anywhere, and "str" is
 * not NULL, then it will be used as the text of a warning message
 * before the function returns.
 *
 * Note that the user must press "-" to specify the item on the floor,
 * and there is no way to "examine" the item on the floor, while the
 * use of "capital" letters will "examine" an inventory/equipment item,
 * and prompt for its use.
 *
 * If a legal item is selected from the inventory, we save it in "cp"
 * directly (0 to 35), and return TRUE.
 *
 * If a legal item is selected from the floor, we save it in "cp" as
 * a negative (-1 to -511), and return TRUE.
 *
 * If no item is available, we do nothing to "cp", and we display a
 * warning message, using "str" if available, and return FALSE.
 *
 * If no item is selected, we do nothing to "cp", and return FALSE.
 *
 * Global "p_ptr->command_new" is used when viewing the inventory or equipment
 * to allow the user to enter a command while viewing those screens, and
 * also to induce "auto-enter" of stores, and other such stuff.
 *
 * Global "p_ptr->command_see" may be set before calling this function to start
 * out in "browse" mode.  It is cleared before this function returns.
 *
 * Global "p_ptr->command_wrk" is used to choose between equip/inven listings.
 * If it is TRUE then we are viewing inventory, else equipment.
 *
 * We always erase the prompt when we are done, leaving a blank line,
 * or a warning message, if appropriate, if no items are available.
 */
bool (*get_item_extra_hook)(s32b *cp);
bool get_item(s32b *cp, cptr pmt, cptr str, s32b mode)
{
	s32b oldtop;

	automatizer_create = FALSE;

	/* Not using call_lua because we need to tets if the return was nil */
	oldtop = lua_gettop(L);

	/* Push the function */
	lua_getglobal(L, "__get_item");

	/* Push the args */
	tolua_pushstring(L, pmt);
	tolua_pushstring(L, str);
	tolua_pushnumber(L, mode);
	tolua_pushnumber(L, command_cmd);
	lua_pushnil(L);
	lua_pushnil(L);

	/* Call the function */
	if (lua_call(L, 6, 1))
	{
		cmsg_format(TERM_VIOLET, "ERROR in lua_call while calling '__get_item'.");
		lua_settop(L, oldtop);
		return FALSE;
	}

	/* Did it return TRUE */
	if (lua_isnumber(L, -(lua_gettop(L) - oldtop)))
	{
		*cp = tolua_getnumber(L, -(lua_gettop(L) - oldtop), 0);
		return TRUE;
	}
	lua_settop(L, oldtop);

	return FALSE;
}

/*
 * Hook to determine if an object is getable
 */
static bool item_tester_hook_getable(object_type *o_ptr, s32b item)
{
	if (!inven_carry_okay(o_ptr)) return (FALSE);

	/* Assume yes */
	return (TRUE);
}


/*
 * Make the player carry everything in a grid
 *
 * If "pickup" is FALSE then only gold will be picked up
 *
 * This is called by py_pickup() when easy_floor is TRUE.
 */
bool can_carry_heavy(object_type *o_ptr)
{
	/* Query if object is heavy */
	if (prompt_pickup_heavy)
	{
		s32b i, j;
		s32b old_enc = 0;
		s32b new_enc = 0;

		/* Extract the "weight limit" (in tenth pounds) */
		i = weight_limit();

		/* Calculate current encumbarance */
		j = calc_total_weight();

		/* Apply encumbarance from weight */
		if (j > i / 2) old_enc = ((j - (i / 2)) / (i / 10));

		/* Increase the weight, recalculate encumbarance */
		j += (o_ptr->number * o_ptr->weight);

		/* Apply encumbarance from weight */
		if (j > i / 2) new_enc = ((j - (i / 2)) / (i / 10));

		/* Should we query? */
		if (new_enc > old_enc)
		{
			return (FALSE);
		}
	}
	return (TRUE);
}

/* Do the actuall picking up */
void object_pickup(s32b this_o_idx)
{
	s32b slot = 0;
	char o_name[100] = "";
	object_type *o_ptr;

	/* Access the item */
	o_ptr = get_object(this_o_idx);

	/* Describe the object */
	object_desc(o_name, o_ptr, TRUE, 3);

	/* Note that the pack is too full */
	if (!inven_carry_okay(o_ptr))
	{
		msg_format("You have no room for %s.", o_name);
		return;
	}

	/* Pick up object */

	/* Can we pick up? */
	if (process_hooks(HOOK_GET_PRE, "(O,d)", o_ptr, this_o_idx))
		return;

	o_ptr = object_dup(o_ptr);
	slot = inven_carry(o_ptr, TRUE);

	/* Get the item again */
	o_ptr = get_object(slot);

	object_track(o_ptr);

	/* Describe the object */
	object_desc(o_name, o_ptr, TRUE, 3);

	/* Message */
	msg_format("You have %s (%c).", o_name, index_to_label(slot));

	/* Tell the scripts */
	process_hooks(HOOK_GET_POST, "(O,d)", o_ptr, slot);

	/* Delete the object */
	delete_object_idx(this_o_idx);
}


void py_pickup_floor(s32b pickup)
{
	char o_name[80] = "";
	object_type *o_ptr = 0;

	s32b floor_num = 0;
	s32b pickup_idx = 0;

	bool do_pickup = TRUE;

	bool do_ask = TRUE;

	/* using flags_type as a list really */
	flags_type *gold_to_delete = flag_new(1);
	s32b num_gold_to_delete = 0;
	s32b gold_i;

	/* Squeltch the floor */
	squeltch_grid();

	/* Scan the pile of objects */
	for_inventory_slot(&cave[p_ptr->py][p_ptr->px].inventory, o_ptr);
	{
		/* Hack -- disturb */
		disturb(0, 0);

		/* Pick up gold */
		if (has_flag(o_ptr, FLAG_GOLD_VALUE))
		{
			char goldname[80];
			object_desc(goldname, o_ptr, TRUE, 3);
			s32b amt;

			if (has_flag(o_ptr, FLAG_MONEY_PICKUP))
			{
				amt = get_flag(o_ptr, FLAG_GOLD_VALUE);
				open_lua_functions_registry(get_flag(o_ptr, FLAG_MONEY_PICKUP), get_flag2(o_ptr, FLAG_MONEY_PICKUP));
				call_lua(NULL, "(d,s)", "d", amt, goldname, &amt);
				close_lua_functions_registry();
			}
			else
			{
				amt = get_flag(o_ptr, FLAG_GOLD_VALUE);
				/* Message */
				msg_format("You have found %ld gold pieces worth of %s.", amt, goldname);
			}

			/* Collect the gold */
			p_ptr->au += amt;

			/* Redraw gold */
			flag_bool(&p_ptr->redraw, FLAG_PR_GOLD);

			/* Window stuff */
			flag_bool(&p_ptr->window, FLAG_PW_PLAYER);

			/* Mark the gold for later deletion */
			flag_set(gold_to_delete,num_gold_to_delete,item_nb);
			num_gold_to_delete++;

			continue;
		}

		{
			char testdesc[80];

			object_desc(testdesc, o_ptr, TRUE, 3);
			if (0 != strncmp(testdesc, "(nothing)", 80))
			{
				strncpy(o_name, testdesc, 80);
			}
		}

		/* Count non-gold */
		floor_num++;
	}
	end_inventory_slot();

	/* Delete the queued gold objects
	   Do it backwards so the indexes aren't ruined by
	   the 'array' being shifted down */

	for(gold_i = num_gold_to_delete-1; gold_i >= 0; gold_i--) {
		/* Acquire object */
		call_lua("generate_item_floor_index", "(d,d,d)", "d",
			 p_ptr->py, p_ptr->px,
			 flag_get(gold_to_delete,gold_i),
			 &pickup_idx);

		delete_object_idx(pickup_idx);
	}

	flag_free(gold_to_delete,TRUE);

	/* There were no non-gold items */
	if (!floor_num) return;

	/* Mention number of items */
	if (!pickup)
	{
		/* One item */
		if (floor_num == 1)
		{
			/* Acquire object */
			o_ptr = flag_get_obj(&cave[p_ptr->py][p_ptr->px].inventory, 1);

			/* Message */
			msg_format("You see %s.", o_name);
		}

		/* Multiple items */
		else
		{
			/* Message */
			msg_format("You see a pile of %d items.", floor_num);
		}

		/* Done */
		return;
	}

	/* One item */
	if (floor_num == 1)
	{
		/* Acquire object */
		call_lua("generate_item_floor_index", "(d,d,d)", "d", p_ptr->py, p_ptr->px, 1, &pickup_idx);
		o_ptr = get_object(pickup_idx);

		/* Hack -- query every item */
		if (carry_query_flag || (!can_carry_heavy(o_ptr)))
		{
			if (!inven_carry_okay(o_ptr))
			{
				object_desc(o_name, o_ptr, TRUE, 3);
				msg_format("You have no room for %s.", o_name);
				do_pickup = FALSE;
			}
			else
			{
				char out_val[160];
				sprintf(out_val, "Pick up %s? ", o_name);
				do_pickup = get_check(out_val);
			}
		}

		/* Don't ask */
		do_ask = FALSE;
	}

	/* Ask */
	if (do_ask)
	{
		cptr q, s;

		s32b item;

		/* Get an item */

		item_tester_hook = item_tester_hook_getable;

		q = "Get which item? ";
		s = "You have no room in your pack for any of the items here.";
		if (get_item(&item, q, s, (USE_FLOOR)))
		{
			pickup_idx = item;

			if (!can_carry_heavy(get_object(pickup_idx)))
			{
				char out_val[160];

				/* Describe the object */
				object_desc(o_name, get_object(pickup_idx), TRUE, 3);

				sprintf(out_val, "Pick up %s? ", o_name);
				do_pickup = get_check(out_val);
			}
		}
		else
		{
			do_pickup = FALSE;
		}
	}

	/* Pick up the item */
	if (do_pickup)
	{
		object_pickup(pickup_idx);
	}
}
