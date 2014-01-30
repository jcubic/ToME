/* File: files.c */

/* Purpose: code dealing with files (and death) */

/*
 * Copyright (c) 1989 James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"
#include "ldebug.h"
extern lua_State* L;



/*
 * Extract the first few "tokens" from a buffer
 *
 * This function uses "colon" and "slash" and delim arg as the delimeter characters.
 *
 * We never extract more than "num" tokens.  The "last" token may include
 * "delimeter" characters, allowing the buffer to include a "string" token.
 *
 * We save pointers to the tokens in "tokens", and return the number found.
 *
 * Hack -- Attempt to handle the 'c' character formalism
 *
 * Hack -- An empty buffer, or a final delimeter, yields an "empty" token.
 *
 * Hack -- We will always extract at least one token
 */
s16b tokenize(char *buf, s16b num, char **tokens, char delim1, char delim2)
{
	s32b i = 0;

	char *s = buf;


	/* Process */
	while (i < num - 1)
	{
		char *t;

		/* Scan the string */
		for (t = s; *t; t++)
		{
			/* Found a delimiter */
			if ((*t == delim1) || (*t == delim2)) break;

			/* Handle single quotes */
			if (*t == '\'')
			{
				/* Advance */
				t++;

				/* Handle backslash */
				if (*t == '\\') t++;

				/* Require a character */
				if (!*t) break;

				/* Advance */
				t++;

				/* Hack -- Require a close quote */
				if (*t != '\'') *t = '\'';
			}

			/* Handle back-slash */
			if (*t == '\\') t++;
		}

		/* Nothing left */
		if (!*t) break;

		/* Nuke and advance */
		*t++ = '\0';

		/* Save the token */
		tokens[i++] = s;

		/* Advance */
		s = t;
	}

	/* Save the token */
	tokens[i++] = s;

	/* Number found */
	return (i);
}

#define NUM_KEY_DIRS     16
#define MAX_DIR_KEYCODES NUM_KEY_DIRS

static char* dir_keycodes[NUM_KEY_DIRS];
static s32b   max_dir_keycode_idx = 0;

static void remember_dir_keycodes(char* trigger, char* keycode)
{
	s32b   i;
	char* dirs[NUM_KEY_DIRS] = {"Left",
								"Up",
								"Right",
								"Down",
								"Page_Up",
								"Page_Down",
								"Home",
								"End",
								"KP_Left",
								"KP_Up",
								"KP_Right",
								"KP_Down",
								"KP_Page_Up",
								"KP_Page_Down",
								"KP_Home",
								"KP_End"};

	for (i = 0; i < NUM_KEY_DIRS; i++)
		if (strcmp(trigger, dirs[i]) == 0) {
			if (max_dir_keycode_idx >= MAX_DIR_KEYCODES)
				/* XXX: Seeing the same trigger/keycode twice??? */
				return;

			dir_keycodes[max_dir_keycode_idx++] = strdup(keycode);
			return;
		}
} /* remember_dir_keycodes() */

bool keycode_is_directional(char* keycode)
{
	s32b i;

	for (i = 0; i < max_dir_keycode_idx; i++)
		if (strcmp(keycode, dir_keycodes[i]))
			return TRUE;

	return FALSE;
} /* keycode_is_directional() */

/*
 * Parse a sub-file of the "extra info" (format shown below)
 *
 * Each "action" line has an "action symbol" in the first column,
 * followed by a colon, followed by some command specific info,
 * usually in the form of "tokens" separated by colons or slashes.
 *
 * Blank lines, lines starting with white space, and lines starting
 * with pound signs ("#") are ignored (as comments).
 *
 * Note the use of "tokenize()" to allow the use of both colons and
 * slashes as delimeters, while still allowing final tokens which
 * may contain any characters including "delimiters".
 *
 * Note the use of "strtol()" to allow all "integers" to be encoded
 * in decimal, hexidecimal, or octal form.
 *
 * Note that "monster zero" is used for the "player" attr/char, "object
 * zero" will be used for the "stack" attr/char, and "feature zero" is
 * used for the "nothing" attr/char.
 *
 * Parse another file recursively, see below for details
 *   %:<filename>
 *
 * Specify the attr/char values for "monsters" by race index
 *   R:<num>:<a>:<c>
 *
 * Specify the attr/char values for "objects" by kind index
 *   K:<num>:<a>:<c>
 *
 * Specify the attr/char values for "features" by feature index
 *   F:<num>:<a>:<c>
 *
 * Specify the attr/char values for "stores" by store index
 *   B:<num>:<a>:<c>
 *
 * Specify the attr/char values for unaware "objects" by kind tval
 *   U:<tv>:<a>:<c>
 *
 * Specify the attr/char values for inventory "objects" by kind tval
 *   E:<tv>:<a>:<c>
 *
 * Define a macro action, given an encoded macro action
 *   A:<str>
 *
 * Create a normal macro, given an encoded macro trigger
 *   P:<str>
 *
 * Create a command macro, given an encoded macro trigger
 *   C:<str>
 *
 * Create a keyset mapping
 *   S:<key>:<key>:<dir>
 *
 * Turn an option off, given its name
 *   X:<str>
 *
 * Turn an option on, given its name
 *   Y:<str>
 *
 * Specify visual information, given an index, and some data
 *   V:<num>:<kv>:<rv>:<gv>:<bv>
 *
 * Specify squelch settings
 *   Q:<num>:<squelch>
 */
errr process_pref_file_aux(char *buf)
{
	s32b i, j, n1, n2;

	char *zz[16];


	/* Skip "empty" lines */
	if (!buf[0]) return (0);

	/* Skip "blank" lines */
	if (isspace(buf[0])) return (0);

	/* Skip comments */
	if (buf[0] == '#') return (0);

	/* Require "?:*" format */
	if (buf[1] != ':') return (1);


	/* Process "%:<fname>" */
	if (buf[0] == '%')
	{
		/* Attempt to Process the given file */
		return (process_pref_file(buf + 2));
	}


	/* Process "R:<name>:<a>/<c>" -- attr/char for monster races */
	if (buf[0] == 'R')
	{
		if (tokenize(buf + 2, 3, zz, ':', '/') == 3)
		{
			monster_race *r_ptr;
			for (i = 0; i < max_r_idx; i++)
				if (strcasecmp(r_info[i].name, zz[0]) == 0)
				{
					r_ptr = &r_info[i];
					n1 = strtol(zz[1], NULL, 0);
					n2 = strtol(zz[2], NULL, 0);
					if (n1) r_ptr->x_attr = n1;
					if (n2)	r_ptr->x_char = n2;
				}
			return (0);
		}
	}


	/* Process "G:<type>:<name>:<a>/<c>" -- attr/char for overlay graphics */
	if (buf[0] == 'G')
	{
		/* Process "G:M:<name>:<a>/<c>" -- attr/char for ego monsters */
		if (buf[2] == 'M')
		{
			if (tokenize(buf + 4, 3, zz, ':', '/') == 3)
			{
				monster_ego *re_ptr;
				for (i = 1; i < max_re_idx; i++)
					if (strcasecmp(re_info[i].name, zz[0]) == 0)
					{
						re_ptr = &re_info[i];
						n1 = strtol(zz[1], NULL, 0);
						n2 = strtol(zz[2], NULL, 0);
						if (n1) re_ptr->g_attr = n1;
						if (n2)	re_ptr->g_char = n2;
					}
				return (0);
			}
		}

		/* Process "G:P:<name>:<a>/<c>" -- attr/char for race modifiers */
		if (buf[2] == 'P')
		{
			if (tokenize(buf + 4, 3, zz, ':', '/') == 3)
			{
#if 0 // DGDGDGDGDG
				player_race_mod *rmp_ptr;
				for (i = 0; i < max_rmp_idx; i++)
					if (strcasecmp(race_mod_info[i].name, zz[0]) == 0)
					{
						rmp_ptr = &race_mod_info[i];
						n1 = strtol(zz[1], NULL, 0);
						n2 = strtol(zz[2], NULL, 0);
						if (n1) rmp_ptr->g_attr = n1;
						if (n2) rmp_ptr->g_char = n2;
					}
#endif
				return (0);
			}
		}
		/* Process "G:T:<name>:<a>/<c>" -- attr/char for traps */
		if (buf[2] == 'T')
		{
			if (tokenize(buf + 4, 3, zz, ':', '/') == 3)
			{
#if 0 //DGDGDGDG
				trap_type *t_ptr;
				for (i = 0; i < max_t_idx; i++)
					if (strcasecmp(t_info[i].name, zz[0]) == 0)
					{
						t_ptr = &t_info[i];
						n1 = strtol(zz[1], NULL, 0);
						n2 = strtol(zz[2], NULL, 0);
						if (n1) t_ptr->g_attr = n1;
						if (n2)	t_ptr->g_char = n2;
					}
#endif
				return (0);
			}
		}
	}


	/* Process "K:<name>:<a>/<c>"  -- attr/char for object kinds */
	else if (buf[0] == 'K')
	{
		if (tokenize(buf + 2, 3, zz, ':', '/') == 3)
		{
			object_kind *k_ptr;
			for (i = 1; i < max_k_idx; i++)
				if (strcasecmp(k_info[i].name, zz[0]) == 0)
				{
					k_ptr = &k_info[i];
					n1 = strtol(zz[1], NULL, 0);
					n2 = strtol(zz[2], NULL, 0);
					if (n1) k_ptr->x_attr = n1;
					if (n2)	k_ptr->x_char = n2;
				}
			return (0);
		}
	}


	/* Process "F:<name>:<a>/<c>" -- attr/char for terrain features */
	else if (buf[0] == 'F')
	{
		if (tokenize(buf + 2, 3, zz, ':', '/') == 3)
		{
			feature_type *f_ptr;
			for (i = 0; i < max_f_idx; i++)
				if (strcasecmp(f_info[i].name, zz[0]) == 0)
				{
					f_ptr = &f_info[i];
					n1 = strtol(zz[1], NULL, 0);
					n2 = strtol(zz[2], NULL, 0);
					if (n1) f_ptr->x_attr = n1;
					if (n2)	f_ptr->x_char = n2;
				}
			return (0);
		}
	}

	/* Process "B:<name>:<a>/<c>" -- attr/char for stores */
	else if (buf[0] == 'B')
	{
		if (tokenize(buf + 2, 3, zz, ':', '/') == 3)
		{
			store_info_type *st_ptr;
			for (i = 0; i < max_st_idx; i++)
				if (strcasecmp(st_info[i].name, zz[0]) == 0)
				{
					st_ptr = &st_info[i];
					n1 = strtol(zz[1], NULL, 0);
					n2 = strtol(zz[2], NULL, 0);
					if (n1) st_ptr->x_attr = n1;
					if (n2) st_ptr->x_char = n2;
				}
			return (0);
		}
	}

	/* Process "U:<tv>:<a>/<c>" -- attr/char for unaware items */
	else if (buf[0] == 'U')
	{
		if (tokenize(buf + 2, 3, zz, ':', '/') == 3)
		{
			j = strtol(zz[0], NULL, 0);
			n1 = strtol(zz[1], NULL, 0);
			n2 = strtol(zz[2], NULL, 0);
			for (i = 1; i < max_k_idx; i++)
			{
				object_kind *k_ptr = &k_info[i];
				if (k_ptr->tval == j)
				{
					if (n1) k_ptr->d_attr = n1;
					if (n2) k_ptr->d_char = n2;
				}
			}
			return (0);
		}
	}


	/* Process "E:<tv>:<a>" -- attribute for inventory objects */
	else if (buf[0] == 'E')
	{
		if (tokenize(buf + 2, 2, zz, ':', '/') == 2)
		{
			j = (byte)strtol(zz[0], NULL, 0) % 128;
			n1 = strtol(zz[1], NULL, 0);
			if (n1) tval_to_attr[j] = n1;
			return (0);
		}
	}


	/* Process "A:<str>" -- save an "action" for later */
	else if (buf[0] == 'A')
	{
		text_to_ascii(macro__buf, buf + 2);
		return (0);
	}

	/* Process "P:<str>" -- normal macro */
	else if (buf[0] == 'P')
	{
		char tmp[1024];
		text_to_ascii(tmp, buf + 2);
		macro_add(tmp, macro__buf);
		return (0);
	}

	/* Process "L:<num>:<trigger>:<descr> -- extended command macro */
	else if (buf[0] == 'L')
	{
		switch (tokenize(buf + 2, 3, zz, ':', 0))
		{
		case 3:
			cli_add(zz[0], zz[1], zz[2]);
			return 0;
		case 2:
			cli_add(zz[0], zz[1], 0);
			return 0;
		default:
			return 1;
		}
	}

	/* Process "C:<str>" -- create keymap */
	else if (buf[0] == 'C')
	{
		s32b mode;

		char tmp[1024];

		if (tokenize(buf + 2, 2, zz, ':', '/') != 2) return (1);

		mode = strtol(zz[0], NULL, 0);
		if ((mode < 0) || (mode >= KEYMAP_MODES)) return (1);

		text_to_ascii(tmp, zz[1]);
		if (!tmp[0] || tmp[1]) return (1);
		i = (byte)(tmp[0]);

		string_free(keymap_act[mode][i]);

		keymap_act[mode][i] = string_make(macro__buf);

		return (0);
	}


	/* Process "V:<num>:<kv>:<rv>:<gv>:<bv>" -- visual info */
	else if (buf[0] == 'V')
	{
		if (tokenize(buf + 2, 5, zz, ':', '/') == 5)
		{
			i = (byte)strtol(zz[0], NULL, 0);
			angband_color_table[i][0] = (byte)strtol(zz[1], NULL, 0);
			angband_color_table[i][1] = (byte)strtol(zz[2], NULL, 0);
			angband_color_table[i][2] = (byte)strtol(zz[3], NULL, 0);
			angband_color_table[i][3] = (byte)strtol(zz[4], NULL, 0);
			return (0);
		}
	}
	/* set macro trigger names and a template */
	/* Process "T:<trigger>:<keycode>:<shift-keycode>" */
	/* Process "T:<template>:<modifier chr>:<modifier name>:..." */
	else if (buf[0] == 'T')
	{
		s32b len, tok;
		tok = tokenize(buf + 2, 2 + MAX_MACRO_MOD, zz, ':', '/');
		if (tok >= 4)
		{
			s32b i;
			s32b num;

			if (macro_template != NULL)
			{
				free(macro_template);
				macro_template = NULL;
				for (i = 0; i < max_macrotrigger; i++)
					free(macro_trigger_name[i]);
				max_macrotrigger = 0;
			}

			if (*zz[0] == '\0') return 0;  /* clear template */
			num = strlen(zz[1]);
			if (2 + num != tok) return 1;  /* error */

			len = strlen(zz[0]) + 1 + num + 1;
			for (i = 0; i < num; i++)
				len += strlen(zz[2 + i]) + 1;
			macro_template = malloc(len);

			strcpy(macro_template, zz[0]);
			macro_modifier_chr =
			        macro_template + strlen(macro_template) + 1;
			strcpy(macro_modifier_chr, zz[1]);
			macro_modifier_name[0] =
			        macro_modifier_chr + strlen(macro_modifier_chr) + 1;
			for (i = 0; i < num; i++)
			{
				strcpy(macro_modifier_name[i], zz[2 + i]);
				macro_modifier_name[i + 1] = macro_modifier_name[i] +
				                             strlen(macro_modifier_name[i]) + 1;
			}
		}
		else if (tok >= 2)
		{
			s32b m;
			char *t, *s;
			if (max_macrotrigger >= MAX_MACRO_TRIG)
			{
				msg_print("Too many macro triggers!");
				return 1;
			}

			remember_dir_keycodes(zz[0], zz[1]);

			m = max_macrotrigger;
			max_macrotrigger++;

			len = strlen(zz[0]) + 1 + strlen(zz[1]) + 1;
			if (tok == 3)
				len += strlen(zz[2]) + 1;
			macro_trigger_name[m] = malloc(len);

			t = macro_trigger_name[m];
			s = zz[0];
			while (*s)
			{
				if ('\\' == *s) s++;
				*t++ = *s++;
			}
			*t = '\0';

			macro_trigger_keycode[0][m] = macro_trigger_name[m] +
			                              strlen(macro_trigger_name[m]) + 1;
			strcpy(macro_trigger_keycode[0][m], zz[1]);
			if (tok == 3)
			{
				macro_trigger_keycode[1][m] = macro_trigger_keycode[0][m] +
				                              strlen(macro_trigger_keycode[0][m]) + 1;
				strcpy(macro_trigger_keycode[1][m], zz[2]);
			}
			else
			{
				macro_trigger_keycode[1][m] = macro_trigger_keycode[0][m];
			}
		}
		return 0;
	}
#if 0 // DGDGDGDG
	/* Process "X:<str>" -- turn option off */
	else if (buf[0] == 'X')
	{
		for (i = 0; option_info[i].o_desc; i++)
		{
			if (option_info[i].o_var &&
			                option_info[i].o_text &&
			                streq(option_info[i].o_text, buf + 2))
			{
				(*option_info[i].o_var) = FALSE;
				return (0);
			}
		}
	}

	/* Process "Y:<str>" -- turn option on */
	else if (buf[0] == 'Y')
	{
		for (i = 0; option_info[i].o_desc; i++)
		{
			if (option_info[i].o_var &&
			                option_info[i].o_text &&
			                streq(option_info[i].o_text, buf + 2))
			{
				(*option_info[i].o_var) = TRUE;
				return (0);
			}
		}
	}
#endif

	/*  Process "Q:<num>:<squelch>" -- item squelch flags */
	else if (buf[0] == 'Q')
	{
		/* This option isn't used anymore */
		return (0);
	}
	/* Failure */
	return (1);
}


/*
 * Helper function for "process_pref_file()"
 *
 * Input:
 *   v: output buffer array
 *   f: final character
 *
 * Output:
 *   result
 */
static cptr process_pref_file_expr(char **sp, char *fp)
{
	cptr v;

	char *b;
	char *s;

	char b1 = '[';
	char b2 = ']';

	char f = ' ';

	/* Initial */
	s = (*sp);

	/* Skip spaces */
	while (isspace(*s)) s++;

	/* Save start */
	b = s;

	/* Default */
	v = "?o?o?";

	/* Analyze */
	if (*s == b1)
	{
		const char *p;
		const char *t;

		/* Skip b1 */
		s++;

		/* First */
		t = process_pref_file_expr(&s, &f);

		/* Oops */
		if (!*t)
		{
			/* Nothing */
		}

		/* Function: IOR */
		else if (streq(t, "IOR"))
		{
			v = "0";
			while (*s && (f != b2))
			{
				t = process_pref_file_expr(&s, &f);
				if (*t && !streq(t, "0")) v = "1";
			}
		}

		/* Function: AND */
		else if (streq(t, "AND"))
		{
			v = "1";
			while (*s && (f != b2))
			{
				t = process_pref_file_expr(&s, &f);
				if (*t && streq(t, "0")) v = "0";
			}
		}

		/* Function: NOT */
		else if (streq(t, "NOT"))
		{
			v = "1";
			while (*s && (f != b2))
			{
				t = process_pref_file_expr(&s, &f);
				if (*t && !streq(t, "0")) v = "0";
			}
		}

		/* Function: EQU */
		else if (streq(t, "EQU"))
		{
			v = "1";
			if (*s && (f != b2))
			{
				t = process_pref_file_expr(&s, &f);
			}
			while (*s && (f != b2))
			{
				p = t;
				t = process_pref_file_expr(&s, &f);
				if (*t && !streq(p, t)) v = "0";
			}
		}

		/* Function: LEQ */
		else if (streq(t, "LEQ"))
		{
			v = "1";
			if (*s && (f != b2))
			{
				t = process_pref_file_expr(&s, &f);
			}
			while (*s && (f != b2))
			{
				p = t;
				t = process_pref_file_expr(&s, &f);
				if (*t && (strcmp(p, t) > 0)) v = "0";
			}
		}

		/* Function: GEQ */
		else if (streq(t, "GEQ"))
		{
			v = "1";
			if (*s && (f != b2))
			{
				t = process_pref_file_expr(&s, &f);
			}
			while (*s && (f != b2))
			{
				p = t;
				t = process_pref_file_expr(&s, &f);
				if (*t && (strcmp(p, t) < 0)) v = "0";
			}
		}

		/* Function: LEQN */
		else if (streq(t, "LEQN"))
		{
			s32b n = 0;
			v = "1";
			if (*s && (f != b2))
			{
				t = process_pref_file_expr(&s, &f);
				n = atoi(t);
			}
			while (*s && (f != b2))
			{
				p = t;
				t = process_pref_file_expr(&s, &f);
				if (*t && (atoi(t) < n)) v = "0";
			}
		}

		/* Function: GEQN */
		else if (streq(t, "GEQN"))
		{
			s32b n = 0;
			v = "1";
			if (*s && (f != b2))
			{
				t = process_pref_file_expr(&s, &f);
				n = atoi(t);
			}
			while (*s && (f != b2))
			{
				p = t;
				t = process_pref_file_expr(&s, &f);
				if (*t && (atoi(t) > n)) v = "0";
			}
		}

		/* Function SKILL */
		else if (streq(t, "SKILL"))
		{
			static char skill_val[4*sizeof(s32b) + 1];
			s16b skill = -1;
			v = "0";
			while (*s && (f != b2))
			{
				t = process_pref_file_expr(&s, &f);
				if (*t) skill = find_skill_i(t);
			}
			if (skill > 0)
			{
				sprintf(skill_val, "%ld", get_skill(skill));
				v = skill_val;
			}
		}

		/* Oops */
		else
		{
			while (*s && (f != b2))
			{
				t = process_pref_file_expr(&s, &f);
			}
		}

		/* Verify ending */
		if (f != b2) v = "?x?x?";

		/* Extract final and Terminate */
		if ((f = *s) != '\0') * s++ = '\0';
	}

	/* Other */
	else
	{
		/* Accept all printables except spaces and brackets */
		while (isprint(*s) && !strchr(" []", *s)) ++s;

		/* Extract final and Terminate */
		if ((f = *s) != '\0') * s++ = '\0';

		/* Variable */
		if (*b == '$')
		{
			/* System */
			if (streq(b + 1, "SYS"))
			{
				v = ANGBAND_SYS;
			}

			else if (streq(b + 1, "KEYBOARD"))
			{
				v = ANGBAND_KEYBOARD;
			}

			/* Graphics */
			if (streq(b + 1, "GRAF"))
			{
				v = TOME_GRAPHICS;
			}

#if 0 // DGDGDGDG
			/* Race */
			else if (streq(b + 1, "RACE"))
			{
				v = rp_ptr->title;
			}

			/* Race */
			else if (streq(b + 1, "RACEMOD"))
			{
				v = rmp_ptr->title;
			}

			/* Class */
			else if (streq(b + 1, "CLASS"))
			{
				v = spp_ptr->title;
			}
#endif
			/* Player */
			else if (streq(b + 1, "PLAYER"))
			{
				v = player_base;
			}
		}

		/* Constant */
		else
		{
			v = b;
		}
	}

	/* Save */
	(*fp) = f;

	/* Save */
	(*sp) = s;

	/* Result */
	return (v);
}




/*
 * Process the "user pref file" with the given name
 *
 * See the function above for a list of legal "commands".
 *
 * We also accept the special "?" and "%" directives, which
 * allow conditional evaluation and filename inclusion.
 */
errr process_pref_file(cptr name)
{
	PHYSFS_file *fp;

	char buf[1024];

	s32b num = -1;

	errr err = 0;

	bool bypass = FALSE;

	/* Build the filename -- Allow users to override system pref files */
	path_build(buf, 1024, TENGINE_DIR_USER, name);

	/* Open the file */
	fp = my_fopen(buf, "r");

	/* No such file -- Try system pref file */
	if (!fp)
	{
		/* Build the pathname, this time using the system pref directory */
		path_build(buf, 1024, TENGINE_DIR_PREF, name);

		/* Open the file */
		fp = my_fopen(buf, "r");

		/* Failed again */
		if (!fp) return ( -1);
	}


	/* Process the file */
	while (0 == my_fgets(fp, buf, 1024))
	{
		/* Count lines */
		num++;


		/* Skip "empty" lines */
		if (!buf[0]) continue;

		/* Skip "blank" lines */
		if (isspace(buf[0])) continue;

		/* Skip comments */
		if (buf[0] == '#') continue;


		/* Process "?:<expr>" */
		if ((buf[0] == '?') && (buf[1] == ':'))
		{
			char f;
			cptr v;
			char *s;

			/* Start */
			s = buf + 2;

			/* Parse the expr */
			v = process_pref_file_expr(&s, &f);

			/* Set flag */
			bypass = (streq(v, "0") ? TRUE : FALSE);

			/* Continue */
			continue;
		}

		/* Apply conditionals */
		if (bypass) continue;


		/* Process "%:<file>" */
		if (buf[0] == '%')
		{
			/* Process that file if allowed */
			(void)process_pref_file(buf + 2);

			/* Continue */
			continue;
		}

		/* Process the line */
		err = process_pref_file_aux(buf);

		/* Oops */
		if (err) break;
	}


	/* Error */
	if (err)
	{
		/* Useful error message */
		msg_format("Error %d in line %d of file '%s'.", err, num, name);
		msg_format("Parsing '%s'", buf);
	}

	/* Close the file */
	my_fclose(fp);

	/* Result */
	return (err);
}







#ifdef CHECK_TIME

/*
 * Operating hours for ANGBAND (defaults to non-work hours)
 */
static char days[7][29] =
	{
		"SUN:XXXXXXXXXXXXXXXXXXXXXXXX",
		"MON:XXXXXXXX.........XXXXXXX",
		"TUE:XXXXXXXX.........XXXXXXX",
		"WED:XXXXXXXX.........XXXXXXX",
		"THU:XXXXXXXX.........XXXXXXX",
		"FRI:XXXXXXXX.........XXXXXXX",
		"SAT:XXXXXXXXXXXXXXXXXXXXXXXX"
	};

/*
 * Restict usage (defaults to no restrictions)
 */
static bool check_time_flag = FALSE;

#endif


/*
 * Handle CHECK_TIME
 */
errr check_time(void)
{

#ifdef CHECK_TIME

	time_t c;

	struct tm *tp;


	/* No restrictions */
	if (!check_time_flag) return (0);

	/* Check for time violation */
	c = time((time_t *)0);
	tp = localtime(&c);

	/* Violation */
	if (days[tp->tm_wday][tp->tm_hour + 4] != 'X') return (1);

#endif

	/* Success */
	return (0);
}



/*
 * !Ran under the game's permission!
 *
 * Initialize CHECK_TIME
 */
errr check_time_init(void)
{

#ifdef CHECK_TIME

	PHYSFS_file *fp;

	char buf[1024];


	/* Build the filename */
	path_build(buf, 1024, TENGINE_DIR_FILE, "time.txt");

	/*
	 * XXX No need to grab permission here because this function is called
	 * only once before the game drops "game" permission
	 */

	/* Open the file */
	fp = my_fopen(buf, "r");

	/* No file, no restrictions */
	if (!fp) return (0);

	/* Assume restrictions */
	check_time_flag = TRUE;

	/* Parse the file */
	while (0 == my_fgets(fp, buf, 80))
	{
		/* Skip comments and blank lines */
		if (!buf[0] || (buf[0] == '#')) continue;

		/* Chop the buffer */
		buf[29] = '\0';

		/* Extract the info */
		if (prefix(buf, "SUN:")) strcpy(days[0], buf);
		if (prefix(buf, "MON:")) strcpy(days[1], buf);
		if (prefix(buf, "TUE:")) strcpy(days[2], buf);
		if (prefix(buf, "WED:")) strcpy(days[3], buf);
		if (prefix(buf, "THU:")) strcpy(days[4], buf);
		if (prefix(buf, "FRI:")) strcpy(days[5], buf);
		if (prefix(buf, "SAT:")) strcpy(days[6], buf);
	}

	/* Close it */
	my_fclose(fp);

#endif

	/* Success */
	return (0);
}



#ifdef CHECK_LOAD

#ifndef MAXHOSTNAMELEN
# define MAXHOSTNAMELEN 64
#endif

typedef struct statstime statstime;

struct statstime
{
	s32b cp_time[4];
	s32b dk_xfer[4];
	u32b v_pgpgin;
	u32b v_pgpgout;
	u32b v_pswpin;
	u32b v_pswpout;
	u32b v_intr;
	s32b if_ipackets;
	s32b if_ierrors;
	s32b if_opackets;
	s32b if_oerrors;
	s32b if_collisions;
	u32b v_swtch;
	long avenrun[3];
	struct timeval boottime;
	struct timeval curtime;
};

/*
 * Maximal load (if any).
 */
static s32b check_load_value = 0;

#endif


/*
 * Handle CHECK_LOAD
 */
errr check_load(void)
{

#ifdef CHECK_LOAD

	struct statstime st;


	/* Success if not checking */
	if (!check_load_value) return (0);

	/* Check the load */
	if (0 == rstat("localhost", &st))
	{
		long val1 = (long)(st.avenrun[2]);
		long val2 = (long)(check_load_value) * FSCALE;

		/* Check for violation */
		if (val1 >= val2) return (1);
	}

#endif

	/* Success */
	return (0);
}


/*
 * !Ran under the game's permission!
 *
 * Initialize CHECK_LOAD
 */
errr check_load_init(void)
{

#ifdef CHECK_LOAD

	PHYSFS_file *fp;

	char buf[1024];

	char temphost[MAXHOSTNAMELEN + 1];
	char thishost[MAXHOSTNAMELEN + 1];


	/* Build the filename */
	path_build(buf, 1024, TENGINE_DIR_FILE, "load.txt");

	/*
	 * XXX No need to grab permission here because this function is called
	 * only once before the game drops "game" permission
	 */

	/* Open the "load" file */
	fp = my_fopen(buf, "r");

	/* No file, no restrictions */
	if (!fp) return (0);

	/* Default load */
	check_load_value = 100;

	/* Get the host name */
	(void)gethostname(thishost, (sizeof thishost) - 1);

	/* Parse it */
	while (0 == my_fgets(fp, buf, 1024))
	{
		s32b value;

		/* Skip comments and blank lines */
		if (!buf[0] || (buf[0] == '#')) continue;

		/* Parse, or ignore */
		if (sscanf(buf, "%s%d", temphost, &value) != 2) continue;

		/* Skip other hosts */
		if (!streq(temphost, thishost) &&
		                !streq(temphost, "localhost")) continue;

		/* Use that value */
		check_load_value = value;

		/* Done */
		break;
	}

	/* Close the file */
	my_fclose(fp);

#endif

	/* Success */
	return (0);
}

#if 0 /* MATT */
/*
 * Print long number with header at given row, column
 * Use the color for the number, not the header
 */
static void prt_lnum(cptr header, s32b num, s32b row, s32b col, byte color)
{
	s32b len = strlen(header);
	char out_val[32];

	put_str(header, row, col);
	(void)sprintf(out_val, "%9ld", (long)num);
	c_put_str(color, out_val, row, col + len);
}

/*
 * Print str with header at given row, column
 */
static void prt_str(cptr header, cptr str, s32b row, s32b col, byte color)
{
	s32b len = strlen(header);
	char out_val[32];

	put_str(header, row, col);
	put_str("   ", row, col + len);
	(void)sprintf(out_val, "%6s", str);
	c_put_str(color, out_val, row, col + len + 3);
}
#endif /* MATT */

/*
 * Print number with header at given row, column
 */
static void prt_num(cptr header, s32b num, s32b row, s32b col, byte color,
                    char *space)
{
	s32b len = strlen(header);
	char out_val[32];

	put_str(header, row, col);
	put_str(space, row, col + len);
	(void)sprintf(out_val, "%6ld", (long)num);
	c_put_str(color, out_val, row, col + len + strlen(space));
}

/*
 * Prints the following information on the screen.
 *
 * For this to look right, the following should be spaced the
 * same as in the prt_lnum code... -CFT
 */
static void display_player_middle(void)
{
#if 0 // DGDGDGDG
	s32b show_tohit = p_ptr->dis_to_h;
	s32b show_todam = p_ptr->dis_to_d;

	object_type *o_ptr = get_object_split(INVEN_WIELD, 0);

	/* Hack -- add in weapon info if known */
	if (o_ptr)
	{
		if (object_known_p(o_ptr)) show_tohit = p_ptr->dis_to_h + p_ptr->to_h_melee + o_ptr->to_h;
		else show_tohit = p_ptr->dis_to_h + p_ptr->to_h_melee;
		if (object_known_p(o_ptr)) show_todam = p_ptr->dis_to_d + p_ptr->to_d_melee + o_ptr->to_d;
		else show_todam = p_ptr->dis_to_d + p_ptr->to_d_melee;
	}

	/* Dump the bonuses to hit/dam */
	prt_num("+ To Melee Hit   ", show_tohit, 9, 1, TERM_L_BLUE, "   ");
	prt_num("+ To Melee Damage", show_todam, 10, 1, TERM_L_BLUE, "   ");

	o_ptr = get_object_split(INVEN_BOW, 0);

	/* Hack -- add in weapon info if known */
	if (o_ptr)
	{
		if (object_known_p(o_ptr)) show_tohit = p_ptr->dis_to_h + p_ptr->to_h_ranged + o_ptr->to_h;
		else show_tohit = p_ptr->dis_to_h + p_ptr->to_h_ranged;
		if (object_known_p(o_ptr)) show_todam = p_ptr->to_d_ranged + o_ptr->to_d;
		else show_todam = p_ptr->to_d_ranged;
	}

	prt_num("+ To Ranged Hit   ", show_tohit, 11, 1, TERM_L_BLUE, "  ");
	prt_num("+ To Ranged Damage", show_todam, 12, 1, TERM_L_BLUE, "  ");

	/* Dump the total armor class */
	prt_str("  AC             ", format("%d+%d", p_ptr->ac, p_ptr->dis_to_a), 13, 1, TERM_L_BLUE);

	prt_num("Level      ", p_ptr->lev, 9, 28, TERM_L_GREEN, "   ");

	if (p_ptr->exp >= p_ptr->max_exp)
	{
		prt_lnum("Experience ", p_ptr->exp, 10, 28, TERM_L_GREEN);
	}
	else
	{
		prt_lnum("Experience ", p_ptr->exp, 10, 28, TERM_YELLOW);
	}

	prt_lnum("Max Exp    ", p_ptr->max_exp, 11, 28, TERM_L_GREEN);

	if ((p_ptr->lev >= PY_MAX_LEVEL) || (p_ptr->lev >= max_plev))
	{
		put_str("Exp to Adv.", 12, 28);
		c_put_str(TERM_L_GREEN, "    *****", 12, 28 + 11);
	}
	else
	{
		prt_lnum("Exp to Adv.",
		         (player_exp[p_ptr->lev - 1] * p_ptr->expfact / 100L),
		         12, 28, TERM_L_GREEN);
	}

	prt_lnum("Gold       ", p_ptr->au, 13, 28, TERM_L_GREEN);


	if (p_ptr->pgod != GOD_NONE)
		prt_num("Piety          ", p_ptr->grace, 13, 52, TERM_L_GREEN, "   ");
#endif
}

/*
 * Prints ratings on certain abilities
 *
 * This code is "imitated" elsewhere to "dump" a character sheet.
 */
static void display_player_various(void)
{
#if 0 // DGDGDGDG
	s32b tmp, dambonus, blows;
	s32b xthn, xthb, xfos, xsrh;
	s32b xdis, xdev, xsav, xstl;
	cptr desc;

	object_type *o_ptr;

	/* Initialize xthn and xthb in case we're naked. */
	xthn = 0;
	xthb = 0;

	/* Fighting Skill (with current weapon) */
	o_ptr = get_object_split(INVEN_WIELD, 0);
	if (o_ptr)
	{
		tmp = p_ptr->to_h + o_ptr->to_h + p_ptr->to_h_melee;
		xthn = p_ptr->skill_thn + (tmp * BTH_PLUS_ADJ);
	}

	/* Shooting Skill (with current bow and normal missile) */
	o_ptr = get_object_split(INVEN_BOW, 0);
	if (o_ptr)
	{
		tmp = p_ptr->to_h + o_ptr->to_h + p_ptr->to_h_ranged;
		xthb = p_ptr->skill_thb + (tmp * BTH_PLUS_ADJ);
	}

	/* variables for all types of melee damage */
	dambonus = p_ptr->dis_to_d;
	blows = p_ptr->num_blow;

	/* Basic abilities */
	xdis = p_ptr->skill_dis;
	xdev = p_ptr->skill_dev;
	xsav = p_ptr->skill_sav;
	xstl = p_ptr->skill_stl;
	xsrh = p_ptr->skill_srh;
	xfos = p_ptr->skill_fos;


	put_str("Fighting    :", 16, 1);
	desc = likert(xthn, 12);
	c_put_str(likert_color, desc, 16, 15);

	put_str("Bows/Throw  :", 17, 1);
	desc = likert(xthb, 12);
	c_put_str(likert_color, desc, 17, 15);

	put_str("Saving Throw:", 18, 1);
	desc = likert(xsav, 6);
	c_put_str(likert_color, desc, 18, 15);

	put_str("Stealth     :", 19, 1);
	desc = likert(xstl, 1);
	c_put_str(likert_color, desc, 19, 15);


	put_str("Perception  :", 16, 28);
	desc = likert(xfos, 6);
	c_put_str(likert_color, desc, 16, 42);

	put_str("Searching   :", 17, 28);
	desc = likert(xsrh, 6);
	c_put_str(likert_color, desc, 17, 42);

	put_str("Disarming   :", 18, 28);
	desc = likert(xdis, 8);
	c_put_str(likert_color, desc, 18, 42);

	put_str("Magic Device:", 19, 28);
	desc = likert(xdev, 6);
	c_put_str(likert_color, desc, 19, 42);


	put_str("Blows/Round:", 16, 55);
	put_str(format("%d", p_ptr->num_blow), 16, 69);

	put_str("Shots/Round:", 17, 55);
	put_str(format("%d", p_ptr->num_fire), 17, 69);

	put_str("Mel.dmg/Rnd:", 18, 55);     /* From PsiAngband */

	call_lua("get_melee_dam_info", "()", "s", &desc);
	put_str(desc, 18, 69);


	put_str("Infra-Vision:", 19, 55);
	put_str(format("%d feet", p_ptr->see_infra * 10), 19, 69);

	/* jk - add tactic */
	put_str("Tactic:", 20, 55);
	c_put_str(TERM_L_BLUE, tactic_info[(byte)p_ptr->tactic].name, 20, 69);

	/* jk - add movement */
	put_str("Explor:", 21, 55);
	c_put_str(TERM_L_BLUE, move_info[(byte)p_ptr->movement].name, 21, 69);
#endif
}



/*
 * Obtain the "flags" for the player as if he was an item
 */
void player_flags(u32b *f1, u32b *f2, u32b *f3, u32b *f4, u32b *f5, u32b *esp)
{
#if 0 // DGDGDGDG
	s32b i;

	/* Clear */
	(*f1) = (*f2) = (*f3) = (*f4) = (*f5) = (*esp) = 0L;

	if (get_skill(SKILL_DAEMON) > 20) (*f2) |= TR2_RES_CONF;
	if (get_skill(SKILL_DAEMON) > 30) (*f2) |= TR2_RES_FEAR;

	/* Classes */
	for (i = 1; i <= p_ptr->lev; i++)
	{
		(*f1) |= cp_ptr->oflags1[i];
		(*f2) |= cp_ptr->oflags2[i];
		(*f3) |= cp_ptr->oflags3[i];
		(*f4) |= cp_ptr->oflags4[i];
		(*f5) |= cp_ptr->oflags5[i];
		(*esp) |= cp_ptr->oesp[i];
	}

	/* Races */
	if ((!p_ptr->mimic_form) && (!p_ptr->body_monster))
	{
		for (i = 1; i <= p_ptr->lev; i++)
		{
			(*f1) |= rp_ptr->oflags1[i];
			(*f2) |= rp_ptr->oflags2[i];
			(*f3) |= rp_ptr->oflags3[i];
			(*f4) |= rp_ptr->oflags4[i];
			(*f5) |= rp_ptr->oflags5[i];
			(*esp) |= rp_ptr->oesp[i];

			(*f1) |= rmp_ptr->oflags1[i];
			(*f2) |= rmp_ptr->oflags2[i];
			(*f3) |= rmp_ptr->oflags3[i];
			(*f4) |= rmp_ptr->oflags4[i];
			(*f5) |= rmp_ptr->oflags5[i];
			(*esp) |= rmp_ptr->oesp[i];
		}
	}
	else
	{
		monster_race *r_ptr = &r_info[p_ptr->body_monster];

		if ((has_flag(r_ptr, FLAG_REFLECTING))) (*f2) |= TR2_REFLECT;
		if ((has_flag(r_ptr, FLAG_REGENERATE))) (*f3) |= TR3_REGEN;
		if ((has_flag(r_ptr, FLAG_AURA_FIRE))) (*f3) |= TR3_SH_FIRE;
		if ((has_flag(r_ptr, FLAG_AURA_ELEC))) (*f3) |= TR3_SH_ELEC;
		if ((has_flag(r_ptr, FLAG_PASS_WALL))) (*f3) |= TR3_WRAITH;
		if ((has_flag(r_ptr, FLAG_SUSCEP_FIRE))) (*f2) |= TR2_SENS_FIRE;
		if ((has_flag(r_ptr, FLAG_IM_ACID))) (*f2) |= TR2_RES_ACID;
		if ((has_flag(r_ptr, FLAG_IM_ELEC))) (*f2) |= TR2_RES_ELEC;
		if ((has_flag(r_ptr, FLAG_IM_FIRE))) (*f2) |= TR2_RES_FIRE;
		if ((has_flag(r_ptr, FLAG_IM_POIS))) (*f2) |= TR2_RES_POIS;
		if ((has_flag(r_ptr, FLAG_IM_COLD))) (*f2) |= TR2_RES_COLD;
		if ((has_flag(r_ptr, FLAG_RES_NETH))) (*f2) |= TR2_RES_NETHER;
		if ((has_flag(r_ptr, FLAG_RES_NEXU))) (*f2) |= TR2_RES_NEXUS;
		if ((has_flag(r_ptr, FLAG_RES_DISE))) (*f2) |= TR2_RES_DISEN;
		if ((has_flag(r_ptr, FLAG_NO_FEAR))) (*f2) |= TR2_RES_FEAR;
		if ((has_flag(r_ptr, FLAG_NO_SLEEP))) (*f2) |= TR2_FREE_ACT;
		if ((has_flag(r_ptr, FLAG_NO_CONF))) (*f2) |= TR2_RES_CONF;
		if ((has_flag(r_ptr, FLAG_CAN_FLY))) (*f3) |= TR3_FEATHER;
	}

	(*f1) |= p_ptr->xtra_f1;
	(*f2) |= p_ptr->xtra_f2;
	(*f3) |= p_ptr->xtra_f3;
	(*f4) |= p_ptr->xtra_f4;
	(*f5) |= p_ptr->xtra_f5;
	(*esp) |= p_ptr->xtra_esp;

	if (p_ptr->black_breath)
	{
		(*f4) |= TR4_BLACK_BREATH;
	}
#endif
}

/*
 * Summarize resistances
 */
static void display_player_ben_one(s32b mode)
{
#if 0 // DGDGDGDG
	s32b i, n, x, y, z, dispx, modetemp, xtemp;

	object_type *o_ptr;

	char dummy[80], c;

	u32b f1, f2, f3, f4, f5, esp;

	u16b b[INVEN_TOTAL - INVEN_WIELD + 1][10];

	s32b d[INVEN_TOTAL - INVEN_WIELD + 1];

	bool got;

	byte a;

	cptr name;

	/* Scan equipment */
	for (i = INVEN_WIELD; i < INVEN_TOTAL; i++)
	{
		/* Index */
		n = (i - INVEN_WIELD);

		/* Object */
		o_ptr = p_ptr->inventory[i];

		/* Incorporate */
		b[n][0] = (u16b)(f1 & 0xFFFF);
		b[n][1] = (u16b)(f1 >> 16);
		b[n][2] = (u16b)(f2 & 0xFFFF);
		b[n][3] = (u16b)(f2 >> 16);
		b[n][4] = (u16b)(f3 & 0xFFFF);
		b[n][5] = (u16b)(f3 >> 16);
		b[n][6] = (u16b)(f4 & 0xFFFF);
		b[n][7] = (u16b)(f4 >> 16);
		b[n][8] = (u16b)(esp & 0xFFFF);
		b[n][9] = (u16b)(esp >> 16);
//DGDGDGDG bah		d[n] = o_ptr->pval;
	}

	/* Index */
	n = INVEN_TOTAL - INVEN_WIELD;

	/* Player flags */
	player_flags(&f1, &f2, &f3, &f4, &f5, &esp);

	/* Incorporate */
	b[n][0] = (u16b)(f1 & 0xFFFF);
	b[n][1] = (u16b)(f1 >> 16);
	b[n][2] = (u16b)(f2 & 0xFFFF);
	b[n][3] = (u16b)(f2 >> 16);
	b[n][4] = (u16b)(f3 & 0xFFFF);
	b[n][5] = (u16b)(f3 >> 16);
	b[n][6] = (u16b)(f4 & 0xFFFF);
	b[n][7] = (u16b)(f4 >> 16);
	b[n][8] = (u16b)(esp & 0xFFFF);
	b[n][9] = (u16b)(esp >> 16);

	/* Generate the equip chars */
	sprintf(dummy, " ");
	for (i = 0; i < INVEN_TOTAL - INVEN_WIELD; i++)
	{
		/* If you have that body part then show it */
		if (p_ptr->body_parts[i])
		{
			strcat(dummy, format("%c", i + 'a'));
		}
	}
	strcat(dummy, "@");

	/* Scan cols */
	for (x = 1; x > -1; x--)
	{
		/* Label */
		Term_putstr(x * 40 + 11, 3, -1, TERM_WHITE, dummy);

		/* Scan rows */
		for (y = 0; y < 16; y++)
		{
			if (mode == 3 && x == 1)
			{
				modetemp = 4;
				xtemp = 0;
			}
			else
			{
				modetemp = mode;
				xtemp = x;
			}

			for (z = mode; z <= modetemp; z++)
			{
				if (mode == 3 && x == 1 && z == modetemp) xtemp = 1;
				name = object_flag_names[32 * modetemp + 16 * xtemp + y];
				got = FALSE;

				/* No name */
				if (!name) continue;

				/* Dump colon */
				if (!(modetemp == 1 && x == 0 && y > 7 && y < 12))
				{
					Term_putch(x * 40 + 10, y + 4, TERM_WHITE, ':');
				}

				/* Check flags */
				dispx = 0;
				for (n = 0; n < INVEN_TOTAL - INVEN_WIELD + 1; n++)
				{
					/* Change colour every two columns */
					bool is_green = (dispx & 0x02);
					a = (is_green ? TERM_GREEN : TERM_SLATE);
					c = '.';

					/* If the body part doesn't exists then skip it :) */
					if ((n < INVEN_TOTAL - INVEN_WIELD) && (!p_ptr->body_parts[n])) continue;

					/* Increment the drawing coordinates */
					dispx++;

					/* Check flag */
					if (b[n][2 * modetemp + xtemp] & (1 << y))
					{
						a = (is_green ? TERM_L_GREEN : TERM_WHITE);
						if (modetemp == 1 && x == 0 && y > 7 && y < 12)
						{
							c = '*';
						}
						else if (modetemp == 0 && x == 0 && y < 14 && (y < 6 || y > 7))
						{
							if (n == INVEN_TOTAL - INVEN_WIELD)
							{
								c = '+';
							}
							else
							{
								c = (d[n] > 9 ? '*' : d[n]);
								if (c < 0)
								{
									c = I2D(c * -1);
									a = TERM_RED;
								}
								else if (c < 10)
								{
									c = I2D(c);
								}
							}
						}
						else
						{
							c = '+';
						}
						got = TRUE;
					}

					/* HACK - Check for nether immunity and
					   apply to Res Neth line */
					if (modetemp == 1 && x == 1 && y == 12)
					{
						if (b[n][7] & (1 << 7))
						{
							a = (is_green ? TERM_L_GREEN : TERM_WHITE);
							c = '*';
							got = TRUE;
						}
					}

					/* Monochrome */
					if (!use_color) a = TERM_WHITE;

					/* Dump flag */
					if (modetemp == 1 && x == 0 && y > 7 && y < 12)
					{
						if (c == '*') Term_putch(40 + 11 + dispx, y - 4, a, c);
					}
					else
					{
						Term_putch(x * 40 + 11 + dispx, y + 4, a, c);
					}
				}

				a = TERM_WHITE;
				if (use_color && got)
				{
					if (modetemp == 1 && x == 0 && y > 7 && y < 12)
					{
						a = TERM_L_GREEN;
					}
					else if (modetemp != 0)
					{
						a = TERM_GREEN;
					}
				}

				/* HACK - Check for nether immunity and change "Res Neth" */
				if (modetemp == 1 && x == 1 && y == 12 && intrinsic(IM_NETHER))
				{
					name = "Imm Neth";
					a = TERM_L_GREEN;
				}

				/* Dump name */
				if (modetemp == 1 && x == 0 && y > 7 && y < 12)
				{
					if (got) Term_putstr(40, y - 4, -1, a, name);
				}
				else
				{
					Term_putstr(x * 40, y + 4, -1, a, name);
				}
			}
		}
	}
#endif
}


/*
 * Display the character on the screen (various modes)
 *
 * The top two and bottom two lines are left blank.
 *
 * Mode 0 = standard display with skills
 * Mode 1 = standard display with history
 * Mode 2 = current flags (part 1)
 * Mode 3 = current flags (part 2)
 * Mode 4 = current flags (part 3)
 * Mode 5 = current flags (part 4)
 * Mode 6 = current flags (part 5 -- esp)
 */
void display_player(s32b mode)
{
	s32b i;

	char buf[80];


	/* Erase screen */
	clear_from(0);

	/* Standard */
	if ((mode == 0) || (mode == 1))
	{
		char         name[80];
		monster_type *body = &p_ptr->body_monster;

		/* Visible monster, ignore insanity, no article */
		monster_desc(buf, body, 0x380);

		/* Name, Sex, Race, Class */
		put_str("Name  :", 2, 1);
		put_str("Sex   :", 3, 1);
		put_str("Race  :", 4, 1);
		put_str("Class :", 5, 1);
		put_str("Body  :", 6, 1);
		put_str("God   :", 7, 1);
		c_put_str(TERM_L_BLUE, player_name, 2, 9);
		if (body->r_idx != 0)
		{
			char tmp[12];

			if (((has_flag(body, FLAG_MALE))) != 0)
				strcpy(tmp, "Male");
			else if (((has_flag(body, FLAG_FEMALE))) != 0)
				strcpy(tmp, "Female");
			else
				strcpy(tmp, "Neuter");
			c_put_str(TERM_L_BLUE, tmp, 3, 9);
		}
#if 0 // DGDGDGDG
		else
			c_put_str(TERM_L_BLUE, sp_ptr->title, 3, 9);
		sprintf(buf, "%s", get_player_race_name(p_ptr->prace, p_ptr->pracem));
#endif
		c_put_str(TERM_L_BLUE, buf, 4, 9);
#if 0 // DGDGDGDG
		c_put_str(TERM_L_BLUE, spp_ptr->title, 5, 9);
#endif
		c_put_str(TERM_L_BLUE, name, 6, 9);

		/* Age, Height, Weight, Social */
		prt_num("Age          ", p_ptr->age + bst(YEAR, turn), 2, 32, TERM_L_BLUE, "   ");
		prt_num("Height       ", p_ptr->ht, 3, 32, TERM_L_BLUE, "   ");
		prt_num("Weight       ", p_ptr->wt, 4, 32, TERM_L_BLUE, "   ");
		prt_num("Social Class ", p_ptr->sc, 5, 32, TERM_L_BLUE, "   ");
#if 0 // DGDGDGDG
		/* Display the stats */
		for (i = 0; i <= STATS_MAX; i++)
		{
			char punctuation = get_stat(i, max) == 18 + 100 ? '!' : ':';
			/* Special treatment of "injured" stats */
			if (get_stat(i, cur) < get_stat(i, max))
			{
				s32b value;
				s32b colour;

				if (get_stat(i, cnt))
					colour = TERM_ORANGE;
				else
					colour = TERM_YELLOW;

				/* Use lowercase stat name */
				put_str(format("%s%c ", stat_names_reduced[i], punctuation), 2 + i, 61);

				/* Get the current stat */
				value = get_stat(i, use);

				/* Obtain the current stat (modified) */
				cnv_stat(value, buf);

				/* Display the current stat (modified) */
				c_put_str(colour, buf, 2 + i, 66);

				/* Acquire the max stat */
				value = get_stat(i, top);

				/* Obtain the maximum stat (modified) */
				cnv_stat(value, buf);

				/* Display the maximum stat (modified) */
				c_put_str(TERM_L_GREEN, buf, 2 + i, 73);
			}

			/* Normal treatment of "normal" stats */
			else
			{
				/* Assume uppercase stat name */
				put_str(format("%s%c ", stat_names[i], punctuation), 2 + i, 61);

				/* Obtain the current stat (modified) */
				cnv_stat(get_stat(i, use), buf);

				/* Display the current stat (modified) */
				c_put_str(TERM_L_GREEN, buf, 2 + i, 66);
			}
		}
#endif
		/* Extra info */
		display_player_middle();

		/* Display "history" info */
		if (mode == 1)
		{
			put_str("(Character Background)", 15, 25);

			for (i = 0; i < 4; i++)
			{
				put_str(history[i], i + 16, 10);
			}
		}

		/* Display "various" info */
		else
		{
			put_str("(Miscellaneous Abilities)", 15, 25);

			display_player_various();
		}
	}

	/* Special */
	else
	{
		display_player_ben_one(mode - 2);
	}
}

/*
 * Utility function; should probably be in some other file...
 *
 * Describe the player's location -- either by dungeon level, town, or in
 * wilderness with landmark reference.
 */
cptr describe_player_location()
{
	s32b i;
	static char desc[80];
	s32b pwx = (p_ptr->wild_mode ? p_ptr->px : p_ptr->wilderness_x);
	s32b pwy = (p_ptr->wild_mode ? p_ptr->py : p_ptr->wilderness_y);
	s32b feat = wild_map[pwy][pwx].feat;

	if (dungeon_type != DUNGEON_WILDERNESS && dun_level > 0)
		sprintf(desc, "on level %d of %s", dun_level, d_info[dungeon_type].name);
	else if (wf_info[feat].terrain_idx == TERRAIN_TOWN)
		sprintf(desc, "in the town of %s", wf_info[feat].name);
	else if (wf_info[feat].entrance)
		sprintf(desc, "near %s", wf_info[feat].name);
	else
	{
		/*
		 * The complicated case.  Find the nearest known landmark,
		 * and describe our position relative to that.  Note that
		 * we may not even have any known landmarks (for instance,
		 * a Lost Soul character just after escaping the Halls of
		 * Mandos).
		 */
		s32b landmark = 0, lwx = 0, lwy = 0;
		s32b l_dist = -1;
		s32b i;

		for (i = 0; i < max_wf_idx; i++)
		{
			s32b wx = wf_info[i].wild_x;
			s32b wy = wf_info[i].wild_y;
			s32b dist;

			/* Skip if not a landmark */
			if (!wf_info[i].entrance) continue;

			/* Skip if we haven't seen it */
			if (!wild_map[wy][wx].known) continue;

			dist = distance(wy, wx, pwy, pwx);
			if (dist < l_dist || l_dist < 0)
			{
				landmark = i;
				l_dist = dist;
				lwx = wx;
				lwy = wy;
			}
		}

		if (!landmark)
			sprintf(desc, "in %s", wf_info[feat].text);
		else if (pwx == lwx && pwy == lwy)
			/* Paranoia; this should have been caught above */
			sprintf(desc, "near %s", wf_info[feat].name);
		else
		{
			/*
			 * We split the circle into eight equal octants of
			 * size pi/4 radians; the "east" octant, for
			 * instance, is defined as due east plus or minus
			 * pi/8 radians.  Now sin(pi/8) ~= 0.3826 ~= 31/81,
			 * so we check |dx|/|dy| and |dy|/|dx| against that
			 * ratio to determine which octant we're in.
			 */
			s32b dx = pwx - lwx;
			s32b dy = pwy - lwy;
			cptr ns = (dy > 0 ? "south" : "north");
			cptr ew = (dx > 0 ? "east" : "west");

			dx = (dx < 0 ? -dx : dx);
			dy = (dy < 0 ? -dy : dy);
			if (dy * 81 < dx * 31) ns = "";
			if (dx * 81 < dy * 31) ew = "";

			sprintf(desc, "in %s %s%s of %s",
			        wf_info[feat].text, ns, ew,
			        wf_info[landmark].name);
		}
	}

	/* strip trailing whitespace */
	for (i = 0; desc[i]; ++i);
	while (desc[--i] == ' ')
		desc[i] = 0;

	return desc;
}

/*
 * Dump a character description file
 *
 */
errr file_character(cptr name)
{
	call_lua("character_sheet.write_to_file", "(s)", "", name);
	return (0);
}


/*
 * Recursive file perusal.
 *
 * Return FALSE on "ESCAPE", otherwise TRUE.
 *
 * Process various special text in the input file, including
 * the "menu" structures used by the "help file" system.
 *
 * XXX XXX XXX Consider using a temporary file.
 *
 * XXX XXX XXX Allow the user to "save" the current file.
 */

/*
 * A structure to hold (some of == XXX) the hyperlink information.
 * This prevents excessive use of stack.
 */
#define MAX_LINKS 1024
struct hyperlink
{
	/* Path buffer */
	char path[1024];

	/* General buffer */
	char rbuf[1024];

	/* Hold a string to find */
	char finder[81];

	/* Hold a string to show */
	char shower[81];

	/* Describe this thing */
	char caption[128];

	/* Hypertext info */
	char link[MAX_LINKS][32], link_key[MAX_LINKS];
	s32b link_x[MAX_LINKS], link_y[MAX_LINKS], link_line[MAX_LINKS];
};

typedef struct hyperlink hyperlink_type;

bool show_file(cptr name, cptr what, s32b line, s32b mode)
{
	s32b i, k, x;

	byte link_color = TERM_ORANGE, link_color_sel = TERM_YELLOW;

	/* Number of "real" lines passed by */
	s32b next = 0;

	/* Number of "real" lines in the file */
	s32b size = 0;

	/* Backup value for "line" */
	s32b back = 0;

	/* Color of the next line */
	byte color = TERM_WHITE;

	/* This screen has sub-screens */
	bool menu = FALSE;

	/* Current help file */
	PHYSFS_file *fff = NULL;

	/* Find this string (if any) */
	cptr find = NULL;

	/* Char array type of hyperlink info */
	hyperlink_type *h_ptr;

	/* Pointer to general buffer in the above */
	char *buf;

	s32b cur_link = 0, max_link = 0;

	/* Read size of screen for big-screen stuff -pav- */
	s32b wid, hgt;

	/* Allocate hyperlink data */
	MAKE(h_ptr, hyperlink_type);

	/* Setup buffer pointer */
	buf = h_ptr->rbuf;

	/* Wipe the links */
	for (i = 0; i < MAX_LINKS; i++)
	{
		h_ptr->link_x[i] = -1;
	}

	/* Hack XXX XXX XXX */
	if (what)
	{
		/* h_ptr->caption */
		strcpy(h_ptr->caption, what);

		/* Access the "file" */
		strcpy(h_ptr->path, name);

		/* Open */
		fff = my_fopen(h_ptr->path, "r");
	}

	/* Look in "help" */
	if (!fff)
	{
		/* h_ptr->caption */
		sprintf(h_ptr->caption, "Help file '%s'", name);

		/* Build the filename */
		path_build(h_ptr->path, 1024, TENGINE_DIR_HELP, name);

		/* Open the file */
		fff = my_fopen(h_ptr->path, "r");
	}

	/* Look in "info" */
	if (!fff)
	{
		/* h_ptr->caption */
		sprintf(h_ptr->caption, "Info file '%s'", name);

		/* Build the filename */
		path_build(h_ptr->path, 1024, TENGINE_DIR_INFO, name);

		/* Open the file */
		fff = my_fopen(h_ptr->path, "r");
	}

	/* Look in "file" */
	if (!fff)
	{
		/* h_ptr->caption */
		sprintf(h_ptr->caption, "File '%s'", name);

		/* Build the filename */
		path_build(h_ptr->path, 1024, TENGINE_DIR_FILE, name);

		/* Open the file */
		fff = my_fopen(h_ptr->path, "r");
	}

	/* Oops */
	if (!fff)
	{
		/* Message */
		msg_format("Cannot open '%s'.", name);
		msg_print(NULL);

		/* Free hyperlink info */
		KILL(h_ptr, hyperlink_type);

		/* Oops */
		return (TRUE);
	}


	/* Pre-Parse the file */
	while (TRUE)
	{
		/* Read a line or stop */
		if (my_fgets(fff, h_ptr->rbuf, 1024)) break;

		/* Get a color */
		if (prefix(h_ptr->rbuf, "#####"))
		{
			buf = &h_ptr->rbuf[6];
		}
		else buf = h_ptr->rbuf;

		/* Get the link colors */
		if (prefix(buf, "|||||"))
		{
			link_color = color_char_to_attr(buf[5]);
			link_color_sel = color_char_to_attr(buf[6]);
		}

		/* Tag ? */
		if (prefix(buf, "~~~~~"))
		{
			if (line < 0)
			{
				s32b i;
				char old_c;

				for (i = 5; (buf[i] >= '0') && (buf[i] <= '9'); i++)
					;
				old_c = buf[i];
				buf[i] = '\0';

				if (atoi(buf + 5) == -line)
				{
					line = next + 1;
				}
				buf[i] = old_c;
			}
		}

		x = 0;
		while (buf[x])
		{
			/* Hyperlink ? */
			if (prefix(buf + x, "*****"))
			{
				s32b xx = x + 5, stmp, xdeb = x + 5, z;
				char tmp[20];

				for (z = 0; z < 20; z++) tmp[z] = '\0';

				h_ptr->link_x[max_link] = x;
				h_ptr->link_y[max_link] = next;

				if (buf[xx] == '/')
				{
					xx++;
					h_ptr->link_key[max_link] = buf[xx];
					xx++;
					xdeb += 2;
				}
				else
				{
					h_ptr->link_key[max_link] = 0;
				}

				/* Zap the link info */
				while (buf[xx] != '*')
				{
					h_ptr->link[max_link][xx - xdeb] = buf[xx];
					xx++;
				}
				h_ptr->link[max_link][xx - xdeb] = '\0';
				xx++;
				stmp = xx;
				while (buf[xx] != '[')
				{
					tmp[xx - stmp] = buf[xx];
					xx++;
				}
				xx++;
				tmp[xx - stmp] = '\0';
				h_ptr->link_line[max_link] = -atoi(tmp);
				max_link++;
			}
			x++;
		}

		/* Count the "real" lines */
		next++;
	}

	/* Save the number of "real" lines */
	size = next;



	/* Display the file */
	while (TRUE)
	{
		/* Clear screen */
		Term_clear();

		Term_get_size(&wid, &hgt);

		/* Restart when necessary */
		if (line >= size) line = 0;


		/* Re-open the file if needed */
		if (next > line)
		{
			/* Close it */
			my_fclose(fff);

			/* Hack -- Re-Open the file */
			fff = my_fopen(h_ptr->path, "r");


			/* Oops */
			if (!fff)
			{
				/* Free hyperlink info */
				KILL(h_ptr, hyperlink_type);

				return (FALSE);
			}

			/* File has been restarted */
			next = 0;
		}

		/* Skip lines if needed */
		for (; next < line; next++)
		{
			/* Skip a line */
			if (my_fgets(fff, buf, 1024)) break;
		}


		/* Dump the next 20 (or more in bigscreen) lines of the file */
		for (i = 0; i < (hgt - 4); )
		{
			s32b print_x;

			/* Hack -- track the "first" line */
			if (!i) line = next;

			/* Get a line of the file or stop */
			if (my_fgets(fff, h_ptr->rbuf, 1024)) break;

			/* Get a color */
			if (prefix(h_ptr->rbuf, "#####"))
			{
				color = color_char_to_attr(h_ptr->rbuf[5]);
				buf = &h_ptr->rbuf[6];
			}
			else buf = h_ptr->rbuf;

			/* Count the "real" lines */
			next++;

			/* Skip link colors */
			if (prefix(buf, "|||||")) continue;

			/* Skip tags */
			if (prefix(buf, "~~~~~"))
			{
				i++;
				continue;
			}

			/* Hack -- keep searching */
			if (find && !i && !strstr(buf, find)) continue;

			/* Hack -- stop searching */
			find = NULL;

			/* Be sure to get a correct cur_link */
			if (h_ptr->link_y[cur_link] >= line + (hgt - 4))
			{
				while ((cur_link > 0) && (h_ptr->link_y[cur_link] >= line + (hgt - 4)))
				{
					cur_link--;
				}
			}
			if (h_ptr->link_y[cur_link] < line)
			{
				while ((cur_link < max_link) && (h_ptr->link_y[cur_link] < line))
				{
					cur_link++;
				}
			}

			/* Dump the line */
			print_x = 0;
			if (!prefix(buf, "&&&&&"))
			{
				x = 0;
				while (buf[x])
				{
					/* Hyperlink ? */
					if (prefix(buf + x, "*****"))
					{
						s32b xx = x + 5;

						/* Zap the link info */
						while (buf[xx] != '[')
						{
							xx++;
						}
						xx++;
						/* Ok print the link name */
						while (buf[xx] != ']')
						{
							byte color = link_color;

							if ((h_ptr->link_x[cur_link] == x) && (h_ptr->link_y[cur_link] == line + i))
								color = link_color_sel;

							/* Now we treat the next char as printable */
							if (buf[xx] == '\\')
								xx++;

							Term_putch(print_x, i + 2, color, buf[xx]);
							xx++;
							print_x++;
						}
						x = xx;
					}
					/* Color ? */
					else if (prefix(buf + x, "[[[[["))
					{
						s32b xx = x + 6;

						/* Ok print the link name */
						while (buf[xx] != ']')
						{
							/* Now we treat the next char as printable */
							if (buf[xx] == '\\')
								xx++;
							Term_putch(print_x, i + 2, color_char_to_attr(buf[x + 5]), buf[xx]);
							xx++;
							print_x++;
						}
						x = xx;
					}
					/* Remove HTML ? */
					else if (prefix(buf + x, "{{{{{"))
					{
						s32b xx = x + 6;

						/* Ok remove this section */
						while (buf[xx] != '}')
						{
							xx++;
						}
						x = xx;
					}
					else
					{
						Term_putch(print_x, i + 2, color, buf[x]);
						print_x++;
					}

					x++;
				}
			}
			/* Verbatim mode: i.e: acacacac */
			else
			{
				x = 5;
				while (buf[x])
				{
					Term_putch(print_x, i + 2, color_char_to_attr(buf[x]), buf[x + 1]);
					print_x++;
					x += 2;
				}
			}
			color = TERM_WHITE;

			/* Hilite "h_ptr->shower" */
			if (h_ptr->shower[0])
			{
				cptr str = buf;

				/* Display matches */
				while ((str = strstr(str, h_ptr->shower)) != NULL)
				{
					s32b len = strlen(h_ptr->shower);

					/* Display the match */
					Term_putstr(str - buf, i + 2, len, TERM_YELLOW, h_ptr->shower);

					/* Advance */
					str += len;
				}
			}

			/* Count the printed lines */
			i++;
		}

		/* Hack -- failed search */
		if (find)
		{
			bell();
			line = back;
			find = NULL;
			continue;
		}


		/* Show a general "title" */
		prt(format("[%s %ld.%ld.%ld, %s, Line %d/%d]", game_module,
		           VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH,
		           h_ptr->caption, line, size), 0, 0);

		/* Prompt -- menu screen */
		if (menu)
		{
			/* Wait for it */
			prt("[Press a Number, or ESC to exit.]", hgt - 1, 0);
		}

		/* Prompt -- small files */
		else if (size <= (hgt - 4))
		{
			/* Wait for it */
			prt("[Press ESC to exit.]", hgt - 1, 0);
		}

		/* Prompt -- large files */
		else
		{
			/* Wait for it */
			prt("[Press 2, 8, 4, 6, /, =, #, %, backspace, or ESC to exit.]", hgt - 1, 0);
		}

		/* Get a keypress */
		k = inkey();

		/* Hack -- return to last screen */
		if ((k == '?') || (k == 0x7F) || (k == '\010')) break;

		/* Hack -- try showing */
		if (k == '=')
		{
			/* Get "h_ptr->shower" */
			prt("Show: ", hgt - 1, 0);
			(void)askfor_aux(h_ptr->shower, 80);
		}

		/* Hack -- try finding */
		if (k == '/')
		{
			/* Get "h_ptr->finder" */
			prt("Find: ", hgt - 1, 0);
			if (askfor_aux(h_ptr->finder, 80))
			{
				/* Find it */
				find = h_ptr->finder;
				back = line;
				line = line + 1;

				/* Show it */
				strcpy(h_ptr->shower, h_ptr->finder);
			}
		}

		/* Hack -- go to a specific line */
		if (k == '#')
		{
			char tmp[81];
			prt("Goto Line: ", hgt - 1, 0);
			strcpy(tmp, "0");
			if (askfor_aux(tmp, 80))
			{
				line = atoi(tmp);
			}
		}

		/* Hack -- go to a specific file */
		if (k == '%')
		{
			char tmp[81];
			prt("Goto File: ", hgt - 1, 0);
			strcpy(tmp, "help.hlp");
			if (askfor_aux(tmp, 80))
			{
				if (!show_file(tmp, NULL, 0, mode)) k = ESCAPE;
			}
		}

		/* Hack -- Allow backing up */
		if (k == '-')
		{
			line = line - (hgt - 4);
			if (line < 0) line = 0;
		}

		if (k == '8')
		{
			line--;
			if (line < 0) line = 0;
		}

		/* Hack -- Advance a single line */
		if (k == '2')
		{
			line = line + 1;
		}

		/* Advance one page */
		if (k == ' ')
		{
			line = line + (hgt - 4);
		}

		/* Advance one link */
		if ((k == '6') || (k == '\t'))
		{
			cur_link++;
			if (cur_link >= max_link) cur_link = max_link - 1;

			if (h_ptr->link_y[cur_link] < line) line = h_ptr->link_y[cur_link];
			if (h_ptr->link_y[cur_link] >= line + (hgt - 4)) line = h_ptr->link_y[cur_link];
		}
		/* Return one link */
		if (k == '4')
		{
			cur_link--;
			if (cur_link < 0) cur_link = 0;

			if (h_ptr->link_y[cur_link] < line) line = h_ptr->link_y[cur_link];
			if (h_ptr->link_y[cur_link] >= line + (hgt - 4)) line = h_ptr->link_y[cur_link];
		}

		/* Recurse on numbers */
		if (k == '\r')
		{
			if (h_ptr->link_x[cur_link] != -1)
			{
				/* Recurse on that file */
				if (!show_file(h_ptr->link[cur_link], NULL, h_ptr->link_line[cur_link], mode)) k = ESCAPE;
			}
		}

		/* Exit on escape */
		if (k == ESCAPE) break;

		/* No other key ? lets look for a shortcut */
		for (i = 0; i < max_link; i++)
		{
			if (h_ptr->link_key[i] == k)
			{
				/* Recurse on that file */
				if (!show_file(h_ptr->link[i], NULL, h_ptr->link_line[i], mode)) k = ESCAPE;
				break;
			}
		}
	}

	/* Close the file */
	my_fclose(fff);

	/* Free hyperlink buffers */
	KILL(h_ptr, hyperlink_type);

	/* Escape */
	if (k == ESCAPE) return (FALSE);

	/* Normal return */
	return (TRUE);
}

bool txt_to_html(cptr head, cptr foot, cptr base, cptr ext, bool force, bool recur)
{
	s32b i, x;

	/* Number of "real" lines passed by */
	s32b next = 0;

	/* Number of "real" lines in the file */
	s32b size = 0;

	char buf_name[80];

	/* Color of the next line */
	byte color = TERM_WHITE;

	/* Current help file */
	PHYSFS_file *fff = NULL;

	/* Current aux file */
	PHYSFS_file *aux = NULL;

	/* Current html file */
	PHYSFS_file *htm = NULL;

	/* Char array type of hyperlink info */
	hyperlink_type *h_ptr;

	cptr file_ext;
	cptr link_prefix;
	cptr link_suffix;

	/* Pointer to general buffer in the above */
	char *buf;

	/* Allocate hyperlink data */
	MAKE(h_ptr, hyperlink_type);

	/* Setup buffer pointer */
	buf = h_ptr->rbuf;

	/* Wipe the links */
	for (i = 0; i < MAX_LINKS; i++)
	{
		h_ptr->link_x[i] = -1;
	}

	/* Parse it(yeah lua is neat :) */
	tome_dofile_anywhere(TENGINE_DIR_HELP, "def.aux", TRUE, FALSE);

	/* Ok now get the parameters */
	file_ext = string_exec_lua("return file_ext");
	link_prefix = string_exec_lua("return link_prefix");
	link_suffix = string_exec_lua("return link_suffix");

	sprintf(buf_name, "%s.%s", base, file_ext);

	if ((!force) && file_exist(buf_name)) return FALSE;

	/* Build the filename */
	path_build(h_ptr->path, 1024, TENGINE_DIR_HELP, buf_name);

	/* Open the file */
	htm = my_fopen(h_ptr->path, "w");

	sprintf(buf_name, "%s.%s", base, ext);

	/* h_ptr->caption */
	sprintf(h_ptr->caption, "Help file '%s'", buf_name);

	/* Build the filename */
	path_build(h_ptr->path, 1024, TENGINE_DIR_HELP, buf_name);

	/* Open the file */
	fff = my_fopen(h_ptr->path, "r");

	/* Oops */
	if (!fff || !htm)
	{
		/* Free hyperlink info */
		KILL(h_ptr, hyperlink_type);

		my_fclose(fff);
		my_fclose(htm);

		/* Oops */
		return (TRUE);
	}

	/* Save the number of "real" lines */
	size = next;

	/* Build the filename */
	path_build(h_ptr->path, 1024, TENGINE_DIR_HELP, head);

	/* Open the file */
	aux = my_fopen(h_ptr->path, "r");

	/* Copy the header */
	if (aux)
	{
		while (TRUE)
		{
			char *find;

			if (my_fgets(aux, h_ptr->rbuf, 1024)) break;
			find = strstr(h_ptr->rbuf, "%t");
			if (find != NULL)
			{
				*find = '\0';
				find += 2;
				fprintf(htm, "%s", h_ptr->rbuf);
				fprintf(htm, "%s", base);
				fprintf(htm, "%s\n", find);
			}
			else
				fprintf(htm, "%s\n", h_ptr->rbuf);
		}
		my_fclose(aux);
	}

	/* Display the file */
	while (TRUE)
	{
		bool do_color = FALSE;

		/* Skip a line */
		if (my_fgets(fff, h_ptr->rbuf, 1024)) break;

		color = TERM_WHITE;

		{
			s32b print_x;

			/* Get a color */
			if (prefix(h_ptr->rbuf, "#####"))
			{
				color = color_char_to_attr(h_ptr->rbuf[5]);
				do_color = TRUE;
				fprintf(htm, "<FONT COLOR=\"#%02X%02X%02X\">",
				        angband_color_table[color][1],
				        angband_color_table[color][2],
				        angband_color_table[color][3]);
				buf = &h_ptr->rbuf[6];
			}
			else buf = h_ptr->rbuf;

			/* Count the "real" lines */
			next++;

			/* Skip link colors */
			if (prefix(buf, "|||||")) continue;

			/* Skip tags */
			if (prefix(buf, "~~~~~"))
			{
				s32b i;

				for (i = 5; (buf[i] >= '0') && (buf[i] <= '9'); i++)
					;
				buf[i] = '\0';
				fprintf(htm, "<A NAME=\"%d\"></A>", atoi(buf + 5));
				continue;
			}

			/* Dump the line */
			print_x = 0;
			if (!prefix(buf, "&&&&&"))
			{
				x = 0;
				while (buf[x])
				{
					/* Hyperlink ? */
					if (prefix(buf + x, "*****"))
					{
						s32b xx = x + 5, z = 0;
						char buff[80];
						char link_line[80], *s;

						if (buf[xx] == '/') xx += 2;

						/* Zap the link info */
						while (buf[xx] != '*')
						{
							buff[z++] = buf[xx];
							xx++;
						}
						xx++;
						buff[z] = '\0';

						/* Zap the link info */
						z = 0;
						while (buf[xx] != '[')
						{
							link_line[z++] = buf[xx];
							xx++;
						}
						xx++;
						link_line[z] = '\0';

						/* parse it */
						s = buff;
						while (*s != '.') s++;
						*s = '\0';
						s++;
						if (recur) txt_to_html(head, foot, buff, s, FALSE, recur);

						if (atoi(link_line)) fprintf(htm, "<A HREF=\"%s%s.%s%s#%d\">", link_prefix, buff, file_ext, link_suffix, atoi(link_line));
						else fprintf(htm, "<A HREF=\"%s%s.%s%s\">", link_prefix, buff, file_ext, link_suffix);

						/* Ok print the link name */
						while (buf[xx] != ']')
						{
							/* Now we treat the next char as printable */
							if (buf[xx] == '\\')
								xx++;
							fprintf(htm, "%c", buf[xx]);
							xx++;
							print_x++;
						}
						x = xx;

						fprintf(htm, "</A>");
					}
					/* Color ? */
					else if (prefix(buf + x, "[[[[["))
					{
						s32b xx = x + 6;

						color = color_char_to_attr(buf[x + 5]);
						fprintf(htm, "<FONT COLOR=\"#%02X%02X%02X\">",
						        angband_color_table[color][1],
						        angband_color_table[color][2],
						        angband_color_table[color][3]);

						/* Ok print the link name */
						while (buf[xx] != ']')
						{
							/* Now we treat the next char as printable */
							if (buf[xx] == '\\')
								xx++;
							fprintf(htm, "%c", buf[xx]);
							xx++;
							print_x++;
						}
						x++;
						x = xx;

						fprintf(htm, "</FONT>");
					}
					/* Hidden HTML tag? */
					else if (prefix(buf + x, "{{{{{"))
					{
						s32b xx = x + 5;

						/* Ok output the tag inside */
						while (buf[xx] != '}')
						{
							fprintf(htm, "%c", buf[xx]);
							xx++;
						}
						x++;
						x = xx;
					}
					else
					{
						fprintf(htm, "%c", buf[x]);
						print_x++;
					}

					x++;
				}
			}
			/* Verbatim mode: i.e: acacacac */
			else
			{
				byte old_color;

				x = 5;
				old_color = color_char_to_attr(buf[x]);
				fprintf(htm, "<FONT COLOR=\"#%02X%02X%02X\">",
				        angband_color_table[color][1],
				        angband_color_table[color][2],
				        angband_color_table[color][3]);
				while (buf[x])
				{
					color = color_char_to_attr(buf[x]);
					if (color != old_color)
						fprintf(htm, "</FONT><FONT COLOR=\"#%02X%02X%02X\">",
						        angband_color_table[color][1],
						        angband_color_table[color][2],
						        angband_color_table[color][3]);

					fprintf(htm, "%c", buf[x + 1]);
					print_x++;
					x += 2;
				}
				fprintf(htm, "</FONT>");
			}
		}
		if (do_color)
		{
			fprintf(htm, "</FONT>");
		}
		fprintf(htm, "\n");
	}

	/* Build the filename */
	path_build(h_ptr->path, 1024, TENGINE_DIR_HELP, foot);

	/* Open the file */
	aux = my_fopen(h_ptr->path, "r");

	/* Copy the footer */
	if (aux)
	{
		while (TRUE)
		{
			if (my_fgets(aux, h_ptr->rbuf, 1024)) break;
			fprintf(htm, "%s\n", h_ptr->rbuf);
		}
		my_fclose(aux);
	}

	/* Close the file */
	my_fclose(htm);
	my_fclose(fff);

	/* Free hyperlink buffers */
	KILL(h_ptr, hyperlink_type);

	/* Normal return */
	return (TRUE);
}

/* Take an help file screenshot(yes yes I know..) */
void help_file_screenshot(cptr name)
{
	s32b y, x;
	s32b wid, hgt;

	byte a = 0;
	char c = ' ';

	PHYSFS_file *htm;

	char buf[1024];

	/* The terms package supports up to 255x255 screen size */
	char abuf[256];
	char cbuf[256];


	/* Build the filename */
	path_build(buf, 1024, TENGINE_DIR_USER, name);

	/* Append to the file */
	htm = my_fopen(buf, "w");

	/* Oops */
	if (!htm) return;

	/* Retrieve current screen size */
	Term_get_size(&wid, &hgt);

	/* Dump the screen */
	for (y = 0; y < hgt; y++)
	{
		cmovie_clean_line(y, abuf, cbuf);

		/* Dump each row */
		fprintf(htm, "&&&&&");
		for (x = 0; x < wid; x++)
		{
			a = abuf[x];
			c = cbuf[x];

			fprintf(htm, "%c%c", conv_color[a & 0xf], c);
		}

		/* End the row */
		fprintf(htm, "\n");
	}

	/* Close it */
	my_fclose(htm);
}

/* Take an html screenshot */
void html_screenshot(cptr name)
{
	s32b y, x;
	s32b wid, hgt;

	byte a = 0, oa = TERM_WHITE;
	char c = ' ';

	PHYSFS_file *htm;

	char buf[1024];

	/* The terms package supports up to 255x255 screen size */
	char abuf[256];
	char cbuf[256];


	/* Build the filename */
	path_build(buf, 1024, TENGINE_DIR_USER, name);

	/* Append to the file */
	htm = my_fopen(buf, "w");

	/* Oops */
	if (!htm) return;

	/* Retrieve current screen size */
	Term_get_size(&wid, &hgt);

	fprintf(htm, "<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\n"
	             "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"DTD/xhtml1-strict.dtd\">\n"
	             "<html xmlns=\"http://www.w3.org/1999/xhtml\">\n"
	             "<head>\n");
	fprintf(htm, "<meta name=\"GENERATOR\" content=\"%s %ld.%ld.%ld\"/>\n",
	        game_module, VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
	fprintf(htm, "<title>%s</title>\n", name);
	fprintf(htm, "</head>\n"
	             "<body>\n"
	             "<pre style=\"color: #ffffff; background-color: #000000; font-family: monospace\">\n");
	fprintf(htm, "<span style=\"color: #%02X%02X%02X\">\n",
	        angband_color_table[TERM_WHITE][1],
	        angband_color_table[TERM_WHITE][2],
	        angband_color_table[TERM_WHITE][3]);

	/* Dump the screen */
	for (y = 0; y < hgt; y++)
	{
		cmovie_clean_line(y, abuf, cbuf);

		/* Dump each row */
		for (x = 0; x < wid; x++)
		{
			a = abuf[x];
			c = cbuf[x];

			if (oa != a)
			{
				fprintf(htm, "</span><span style=\"color: #%02X%02X%02X\">", angband_color_table[a][1], angband_color_table[a][2], angband_color_table[a][3]);
				oa = a;
			}
			if (c == '<')
				fprintf(htm, "&lt;");
			else if (c == '>')
				fprintf(htm, "&gt;");
			else if (c == '&')
				fprintf(htm, "&amp;");
			else
				fprintf(htm, "%c", c);
		}

		/* End the row */
		fprintf(htm, "\n");
	}
	fprintf(htm, "</span>\n"
	             "</pre>\n"
	             "</body>\n"
	             "</html>\n");

	/* Close it */
	my_fclose(htm);
}

/*
 * Because this is dead code and hardly anyone but DG needs it.
 * IMHO this should never been included in the game code -- pelpel
 */
#if !defined(USE_WINDOWS)

#define KEY_NUM         9
static s32b keys_tab[KEY_NUM] =
{
	'I', 'G', 'M', 'O', 'P', 'm', 'D', 'B', 'V',
};

static cptr keys_desc[KEY_NUM] =
{
	"Interface changes:",
	"Gameplay changes:",
	"Monster changes:",
	"Object changes:",
	"Player changes:",
	"Misc changes:",
	"Dungeon changes:",
	"Bug fixes:",
	"Version:",
};

static s32b get_key(char c)
{
	s32b i;

	i = 0;
	while (keys_tab[i] != c)
		i++;
	return ((i > KEY_NUM) ? KEY_NUM : i);
}

/*
 * Some ports don't like huge stacks
 */
typedef char chg_type[500][100];

bool chg_to_txt(cptr base, cptr newname)
{
	s32b i, j, key = 0;

	char buf[1024];

	s32b lens[KEY_NUM] = {0, 0, 0, 0, 0, 0, 0, 0};
	chg_type *strs;

	/* Current chg file */
	PHYSFS_file *fff = NULL;

	/* Current txt file */
	PHYSFS_file *txt = NULL;

	/* Open the file */
	fff = my_fopen(base, "r");

	/* Oops */
	if (!fff)
	{
		my_fclose(fff);
		my_fclose(txt);

		/* Oops */
		return (TRUE);
	}

	/* Count the file */
	while (TRUE)
	{
		/* Skip a line */
		if (my_fgets(fff, buf, 1024)) break;

		if ((!(*buf)) ||
		                ((buf[0] >= '0') && (buf[0] <= '9')) || (buf[0] == '#')) continue;

		if (buf[1] != ' ')

			lens[get_key(buf[1])]++;
	}

	/* Beware, beware */
	PHYSFS_setWriteDir(PHYSFS_getBaseDir());

	/* Open the file */
	txt = my_fopen(newname, "w");

	/* Open the file */
	fff = my_fopen(base, "r");

	/* Oops */
	if (!fff || !txt)
	{
		my_fclose(fff);
		my_fclose(txt);

		/* Oops */
		return (TRUE);
	}

	for (i = 0; i < KEY_NUM; i++) lens[i] = 0;

	/* Allocate big amount of temporary storage */
	C_MAKE(strs, KEY_NUM, chg_type);

	fprintf(txt, "%s %ld.%ld.%ld changes\n",
	        game_module, VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);

	/* Display the file */
	while (TRUE)
	{
		/* Skip a line */
		if (my_fgets(fff, buf, 1024)) break;

		if ((!(*buf)) ||
		                ((buf[0] >= '0') && (buf[0] <= '9')) || (buf[0] == '#')) continue;

		if (buf[1] != ' ') key = get_key(buf[1]);

		strcpy(strs[key][lens[key]++], buf + 3);
	}

	for (i = 0; i < KEY_NUM - 1; i++)
	{
		if (lens[i]) fprintf(txt, "\n%s\n", keys_desc[i]);

		for (j = 0; j < lens[i]; j++)
		{
			fprintf(txt, "%s\n", strs[i][j]);
		}
	}

	/* Close the file */
	my_fclose(txt);
	my_fclose(fff);

	/* Free temporary memory */
	C_FREE(strs, KEY_NUM, chg_type);

	/* Normal return */
	return (TRUE);
}

#endif /* !USE_WINDOWS */

/*
 * Peruse the On-Line-Help
 */
void do_cmd_help(void)
{
	/* Save screen */
	screen_save();

	/* Peruse the main help file */
	(void)show_file("help.hlp", NULL, 0, 0);

	/* Load screen */
	screen_load();
}




/*
 * Process the player name.
 * Extract a clean "base name".
 * Build the savefile name if needed.
 */
void process_player_base()
{
	char temp[256];

#if defined(SAVEFILE_USE_UID) && !defined(PRIVATE_USER_PATH)
	/* Rename the savefile, using the player_uid and player_base */
	(void)sprintf(temp, "%d.%s", player_uid, player_base);
#else
	/* Rename the savefile, using the player_base */
	(void)sprintf(temp, "%s", player_base);
#endif

	/* Build the filename */
	path_build(savefile, 1024, TENGINE_DIR_SAVE, temp);
}

void process_player_name(bool sf)
{
	s32b i, k = 0;
	char tmp[128];

	/* Cannot be too long */
	if (strlen(player_base) > 127)
	{
		/* Name too long */
		quit_fmt("The name '%s' is too long!", player_base);
	}

	/* Cannot contain "icky" characters */
	for (i = 0; player_base[i]; i++)
	{
		/* No control characters */
		if (iscntrl(player_base[i]))
		{
			/* Illegal characters */
			quit_fmt("The name '%s' contains control chars!", player_base);
		}
	}

	/* Extract "useful" letters */
	for (i = 0; player_base[i]; i++)
	{
		char c = player_base[i];

		/* Accept some letters */
		if (isalpha(c) || isdigit(c)) tmp[k++] = c;

		/* Convert space, dot, and underscore to underscore */
		else if (strchr("@. _", c)) tmp[k++] = '_';
	}


#if defined(USE_WINDOWS)

	/* Hack -- max length */
/* Tssssk	if (k > 8) k = 8; */

#endif

	/* Terminate */
	tmp[k] = '\0';
	sprintf(player_base, tmp);

	/* Require a "base" name */
	if (!player_base[0]) strcpy(player_base, "PLAYER");


#ifdef SAVEFILE_MUTABLE

	/* Accept */
	sf = TRUE;

#endif

	/* Change the savefile name */
	if (sf)
	{
		process_player_base();
	}
}


/*
 * Gets a name for the character, reacting to name changes.
 *
 * Assumes that "display_player(0)" has just been called
 *
 * Perhaps we should NOT ask for a name (at "birth()") on
 * Unix machines?  XXX XXX
 *
 * What a horrible name for a global function.  XXX XXX XXX
 */
void get_name(void)
{
	char tmp[32];

	/* Clear last line */
	clear_from(22);

	/* Prompt and ask */
	prt("[Enter your player's name above, or hit ESCAPE]", 23, 2);

	/* Ask until happy */
	while (1)
	{
		/* Go to the "name" field */
		move_cursor(2, 9);

		/* Save the player name */
		strcpy(tmp, player_name);

		/* Get an input, ignore "Escape" */
		if (askfor_aux(tmp, 31)) strcpy(player_name, tmp);

		/* Process the player name */
		process_player_name(FALSE);

		/* All done */
		break;
	}

	/* Pad the name (to clear junk) */
	sprintf(tmp, "%-31.31s", player_name);

	/* Re-Draw the name (in light blue) */
	c_put_str(TERM_L_BLUE, tmp, 2, 9);

	/* Erase the prompt, etc */
	clear_from(22);
}



/*
 * Hack -- commit suicide
 */
void do_cmd_suicide(void)
{
	s32b i;

	/* Flush input */
	flush();

	/* Verify Retirement */
	if (total_winner)
	{
		/* Verify */
		if (!get_check("Do you want to retire? ")) return;
	}

	/* Verify Suicide */
	else
	{
		/* Verify */
		if (!get_check("Do you really want to quit? ")) return;

		if (!noscore)
		{
			/* Special Verification for suicide */
			prt("Please verify QUITTING by typing the '@' sign: ", 0, 0);
			flush();
			i = inkey();
			prt("", 0, 0);
			if (i != '@') return;
		}
	}

	/* Stop playing */
	alive = FALSE;

	/* Kill the player */
	death = TRUE;

	/* Leaving */
	p_ptr->leaving = TRUE;

	/* Cause of death */
	(void)strcpy(died_from, "Quitting");
}



/*
 * Save the game
 */
void do_cmd_save_game(void)
{
	panic_save = 0;   /* Fixes an apparently long-lived bug */

	/* Save the current level if in a persistent level */
	if (!p_ptr->wild_mode) save_dungeon(turn);

	/* Autosaves do not disturb */
	if (!is_autosave)
	{
		/* Disturb the player */
		disturb(1, 0);
	}

	/* Clear messages */
	msg_print(NULL);

	/* Handle stuff */
	handle_stuff();

	/* Message */
	prt("Saving game...", 0, 0);

	/* Refresh */
	Term_fresh();

	/* The player is not dead */
	(void)strcpy(died_from, "(saved)");

	/* Forbid suspend */
	signals_ignore_tstp();

	/* Save the player */
	if (save_player())
	{
		prt("Saving game... done.", 0, 0);
	}

	/* Save failed (oops) */
	else
	{
		prt("Saving game... failed!", 0, 0);
	}

	/* Allow suspend again */
	signals_handle_tstp();

	/* Refresh */
	Term_fresh();

	/* Note that the player is not dead */
	(void)strcpy(died_from, "(alive and well)");
}



/*
 * Hack -- Calculates the total number of points earned                -JWT-
 */
long total_points(void)
{
	s16b max_dl = 0, i, k;
	long temp, Total = 0;
	long mult = 100;
	long comp_death = (p_ptr->companion_killed * 2 / 5);

	if (!comp_death) comp_death = 1;

	if (p_ptr->preserve) mult -= 5;  /* Penalize preserve, maximize modes */
	if (p_ptr->maximize) mult -= 5;
	if (auto_scum) mult -= 20;
	if (stupid_monsters) mult -= 50;
	if (small_levels) mult += ((always_small_level) ? 20 : 50);
	if (empty_levels) mult += 10;
	if (smart_learn) mult += 20;
	if (smart_cheat) mult += 20;

	if (mult < 10) mult = 10;  /* At least 10% of the original score */

	for (i = 0; i < max_d_idx; i++)
		if (max_dlv[i] > max_dl)
			max_dl = max_dlv[i];

	temp = p_ptr->lev * p_ptr->lev * p_ptr->lev * p_ptr->lev + (100 * max_dl);

	temp += p_ptr->max_exp / 5;

	temp += p_ptr->au / 5;

	temp = (temp * mult / 100);

	/* Completing quest increase score */
	for (i = 0; i < max_q_idx; i++)
	{
		if (quest[i].status >= QUEST_STATUS_COMPLETED)
		{
			temp += 2000;
			temp += quest[i].level * 100;
		}
	}

	/* Death of a companion is BAD */
	temp /= comp_death;

	/* The know objects increase the score */
	/* Scan the object kinds */
	for (k = 1; k < max_k_idx; k++)
	{
		object_kind *k_ptr = &k_info[k];

		/* Hack -- skip artifacts */
		if (has_flag(k_ptr, FLAG_INSTA_ART)) continue;

		/* List known flavored objects */
		if (k_ptr->flavor && k_ptr->aware)
		{
			object_type *i_ptr;

			/* Get local object */
			i_ptr = new_object();

			/* Create fake object */
			object_prep(i_ptr, k);

			temp += object_value_real(i_ptr);

			delete_object(i_ptr);
		}
	}

	for (k = 1; k < max_r_idx; k++)
	{
		monster_race *r_ptr = &r_info[k];

		if (has_flag(r_ptr, FLAG_UNIQUE))
		{
			bool dead = (r_ptr->max_num == 0);

			if (dead)
			{
				/* Uniques are supposed to be harder */
				Total += 50;
			}
		}
		else
		{
			s16b This = r_ptr->r_pkills;

			if (This > 0)
			{
				Total += This;
			}
		}
	}
	temp += Total * 50;

	if (total_winner) temp += 1000000;



	return (temp);
}



/*
 * Centers a string within a 31 character string                -JWT-
 */
static void center_string(char *buf, cptr str)
{
	int i, j;

	/* Total length */
	i = strlen(str);

	/* Necessary border */
	j = 15 - i / 2;

	/* Mega-Hack */
	(void)sprintf(buf, "%*s%s%*s", j, "", str, 31 - i - j, "");
}


/*
 * Redefinable "print_tombstone" action
 */
bool (*tombstone_aux)(void) = NULL;


/*
 * Display a "tomb-stone"
 */
static void print_tomb(void)
{
	bool done = FALSE;

	/* Do we use a special tombstone ? */
	if (tombstone_aux)
	{
		/* Use tombstone hook */
		done = (*tombstone_aux)();
	}

	/* Print the text-tombstone */
	if (!done)
	{
		cptr p;

		char tmp[160];

		char buf[1024];
		char dummy[80];

		PHYSFS_file *fp;

		time_t ct = time((time_t)0);


		/* Clear screen */
		Term_clear();

		/* Build the filename */
		path_build(buf, 1024, TENGINE_DIR_FILE, "dead.txt");

		/* Open the News file */
		fp = my_fopen(buf, "r");

		/* Dump */
		if (fp)
		{
			s32b i = 0;

			/* Dump the file to the screen */
			while (0 == my_fgets(fp, buf, 1024))
			{
				/* Display and advance */
				display_message(0, i++, strlen(buf), TERM_WHITE, buf);
			}

			/* Close */
			my_fclose(fp);
		}


		/* King or Queen */
		if (total_winner || (p_ptr->lev > PY_MAX_LEVEL))
		{
			p = "Magnificent";
		}

		/* Normal */
		else
		{
#if 0 // DGDGDGDG
			p = cp_ptr->titles[(p_ptr->lev - 1) / 5];
#endif
			/* Must initialize p to *something*.... */
			p = "Bug-finder";
		}

		center_string(buf, player_name);
		put_str(buf, 6, 11);

		center_string(buf, "the");
		put_str(buf, 7, 11);

		center_string(buf, p);
		put_str(buf, 8, 11);


#if 0 // DGDGDGDG
		center_string(buf, spp_ptr->title);
#endif
		put_str(buf, 10, 11);

		(void)sprintf(tmp, "Level: %d", p_ptr->lev);
		center_string(buf, tmp);
		put_str(buf, 11, 11);

		(void)sprintf(tmp, "Exp: %ld", (long)p_ptr->exp);
		center_string(buf, tmp);
		put_str(buf, 12, 11);

		(void)sprintf(tmp, "AU: %ld", (long)p_ptr->au);
		center_string(buf, tmp);
		put_str(buf, 13, 11);

		(void)sprintf(tmp, "Killed on Level %d", dun_level);
		center_string(buf, tmp);
		put_str(buf, 14, 11);


		if (strlen(died_from) > 24)
		{
			strncpy(dummy, died_from, 24);
			dummy[24] = '\0';
			(void)sprintf(tmp, "by %s.", dummy);
		}
		else
			(void)sprintf(tmp, "by %s.", died_from);

		center_string(buf, tmp);
		put_str(buf, 15, 11);


		(void)sprintf(tmp, "%-.24s", ctime(&ct));
		center_string(buf, tmp);
		put_str(buf, 17, 11);
	}
}


/*
 * Display some character info
 */
static void show_info(void)
{
#if 0
	s32b i, j, k;
	object_type *o_ptr;
//DGDGDGDG	store_type *st_ptr;

	/* Hack -- Know everything in the inven/equip */
	for_inventory(p_ptr, o_ptr, 0, INVEN_TOTAL);
	{
		/* Aware and Known */
		object_aware(o_ptr);
		object_known(o_ptr);
	}
	end_inventory();

#if 0 // DGDGDGDG
	for (i = 1; i < max_towns; i++)
	{
		st_ptr = &town_info[i].store[7];

		/* Hack -- Know everything in the home */
		for (j = 0; j < st_ptr->stock_num; j++)
		{
			o_ptr = st_ptr->stock[j];

			/* Skip non-objects */
			if (!o_ptr->k_idx) continue;

			/* Aware and Known */
			object_aware(o_ptr);
			object_known(o_ptr);
		}
	}
#endif

	/* Hack -- Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Handle stuff */
	handle_stuff();

	/* Flush all input keys */
	flush();

	/* Flush messages */
	msg_print(NULL);


	/* Describe options */
	prt("You may now dump a character record to one or more files.", 21, 0);
	prt("Then, hit RETURN to see the character, or ESC to abort.", 22, 0);

	/* Dump character records as requested */
	while (TRUE)
	{
		char out_val[160];

		/* Prompt */
		put_str("Filename(you can post it to http://angband.oook.cz/): ", 23, 0);

		/* Default */
		strcpy(out_val, "");

		/* Ask for filename (or abort) */
		if (!askfor_aux(out_val, 60)) return;

		/* Return means "show on screen" */
		if (!out_val[0]) break;

		/* Save screen */
		character_icky++;
		Term_save();

		/* Dump a character file */
		(void)file_character(out_val);

		/* Load screen */
		Term_load();
		character_icky--;
	}


	/* Display player */
	display_player(0);

	/* Prompt for inventory */
	prt("Hit any key to see more information (ESC to abort): ", 23, 0);

	/* Allow abort at this point */
	if (inkey() == ESCAPE) return;


	/* Show equipment and inventory */

	/* Equipment -- if any */
	if (equip_cnt)
	{
		Term_clear();
		item_tester_full = TRUE;
		show_equip(FALSE);
		prt("You are using: -more-", 0, 0);
		if (inkey() == ESCAPE) return;
	}

	/* Inventory -- if any */
	if (inven_cnt)
	{
		Term_clear();
		item_tester_full = TRUE;
		show_inven(FALSE);
		prt("You are carrying: -more-", 0, 0);
		if (inkey() == ESCAPE) return;
	}

#if 0 // DGDGDGDG
	/* Homes in the different towns */
	for (k = 1; k < max_towns; k++)
	{
		st_ptr = &town_info[k].store[7];

#if 0 /* TODO -- actualy somewhere set the real variable */
		/* Step to the real towns */
		if (!(town_info[k].flags & (TOWN_REAL))) continue;
#endif

		/* Home -- if anything there */
		if (st_ptr->stock_num)
		{
			/* Display contents of the home */
			for (k = 0, i = 0; i < st_ptr->stock_num; k++)
			{
				/* Clear screen */
				Term_clear();

				/* Show 12 items */
				for (j = 0; (j < 12) && (i < st_ptr->stock_num); j++, i++)
				{
					char o_name[80];
					char tmp_val[80];

					/* Acquire item */
					o_ptr = st_ptr->stock[i];

					/* Print header, clear line */
					sprintf(tmp_val, "%c) ", I2A(j));
					prt(tmp_val, j + 2, 4);

					/* Display object description */
					object_desc(o_name, o_ptr, TRUE, 3);
					c_put_str(tval_to_attr[o_ptr->tval], o_name, j + 2, 7);
				}

				/* h_ptr->caption */
				prt(format("Your home contains (page %d): -more-", k + 1), 0, 0);

				/* Wait for it */
				if (inkey() == ESCAPE) return;
			}
		}
	}
#endif
#else
	call_lua("player.on_died_show_info", "()", "");
#endif
}





/*
 * Semi-Portable High Score List Entry (128 bytes) -- BEN
 *
 * All fields listed below are null terminated ascii strings.
 *
 * In addition, the "number" fields are right justified, and
 * space padded, to the full available length (minus the "null").
 *
 * Note that "string comparisons" are thus valid on "pts".
 */

typedef struct high_score high_score;

struct high_score
{
	char what[8];                 /* Version info (string) */

	char pts[10];                 /* Total Score (number) */

	char gold[10];                 /* Total Gold (number) */

	char turns[10];                 /* Turns Taken (number) */

	char day[10];                 /* Time stamp (string) */

	char who[16];                 /* Player Name (string) */

	char uid[8];                 /* Player UID (number) */

	char sex[2];                 /* Player Sex (string) */
	char p_r[3];                 /* Player Race (number) */
	char p_s[3];             /* Player Subrace (number) */
	char p_c[3];                 /* Player Class (number) */
	char p_cs[3];            /* Player Class spec (number) */

	char cur_lev[4];                 /* Current Player Level (number) */
	char cur_dun[4];                 /* Current Dungeon Level (number) */
	char max_lev[4];                 /* Max Player Level (number) */
	char max_dun[4];                 /* Max Dungeon Level (number) */

	char arena_number[4];         /* Arena level attained -KMW- */
	char inside_arena[4];    /* Did the player die in the arena? */
	char inside_quest[4];    /* Did the player die in a quest? */
	char exit_bldg[4];         /* Can the player exit arena? Goal obtained? -KMW- */

	char how[32];                 /* Method of death (string) */
};



/*
 * Seek score 'i' in the highscore file
 */
static s32b highscore_seek(s32b i)
{
	/* Seek for the requested record */
	return (PHYSFS_seek(highscore_fd, (i) * sizeof(high_score)));
}


/*
 * Read one score from the highscore file
 */
static errr highscore_read(high_score *score)
{
	/* Read the record, note failure */
	return (PHYSFS_read(highscore_fd, score, sizeof(high_score), 1));
}

/*
 * Just determine where a new score *would* be placed
 * Return the location (0 is best) or -1 on failure
 */
static s32b highscore_where(high_score *score)
{
	s32b i;

	high_score the_score;

	/* Paranoia -- it may not have opened */
	if (highscore_fd == NULL) return ( -1);

	/* Go to the start of the highscore file */
	if (highscore_seek(0)) return ( -1);

	/* Read until we get to a higher score */
	for (i = 0; i < MAX_HISCORES; i++)
	{
		if (highscore_read(&the_score)) return (i);
		if (strcmp(the_score.pts, score->pts) < 0) return (i);
	}

	/* The "last" entry is always usable */
	return (MAX_HISCORES - 1);
}


/*
 * Display the scores in a given range.
 * Assumes the high score list is already open.
 * Only five entries per line, too much info.
 *
 * Mega-Hack -- allow "fake" entry at the given position.
 */
static void display_scores_aux(s32b from, s32b to, s32b note, high_score *score)
{
	s32b i, j, k, n, place;
	byte attr;
	char out_val[256];
	char tmp_val[160];
	high_score the_score;


	/* Paranoia -- it may not have opened */
	if (highscore_fd == NULL) return;


	/* Assume we will show the first 10 */
	if (from < 0) from = 0;
	if (to < 0) to = 10;
	if (to > MAX_HISCORES) to = MAX_HISCORES;


	/* Seek to the beginning */
	if (highscore_seek(0)) return;

	/* Hack -- Count the high scores */
	for (i = 0; i < MAX_HISCORES; i++)
	{
		if (highscore_read(&the_score)) break;
	}

	/* Hack -- allow "fake" entry to be last */
	if ((note == i) && score) i++;

	/* Forget about the last entries */
	if (i > to) i = to;


	/* Show 5 per page, until "done" */
	for (k = from, place = k + 1; k < i; k += 5)
	{
		/* Clear screen */
		Term_clear();

		/* Title */
		put_str(format("              %s Hall of Fame", game_module), 0, 0);

		/* Indicate non-top scores */
		if (k > 0)
		{
			sprintf(tmp_val, "(from position %ld)", k + 1);
			put_str(tmp_val, 0, 40);
		}

		/* Dump 5 entries */
		for (j = k, n = 0; j < i && n < 5; place++, j++, n++)
		{
			s32b pcs, pr, ps, pc, clev, mlev, cdun, mdun;

			cptr user, gold, when, aged;

			s32b in_arena, in_quest;

			/* Hack -- indicate death in yellow */
			attr = (j == note) ? TERM_YELLOW : TERM_WHITE;


			/* Mega-Hack -- insert a "fake" record */
			if ((note == j) && score)
			{
				the_score = (*score);
				attr = TERM_L_GREEN;
				score = NULL;
				note = -1;
				j--;
			}

			/* Read a normal record */
			else
			{
				/* Read the proper record */
				if (highscore_seek(j)) break;
				if (highscore_read(&the_score)) break;
			}

			/* Extract the race/class */
			pr = atoi(the_score.p_r);
			ps = atoi(the_score.p_s);
			pc = atoi(the_score.p_c);
			pcs = atoi(the_score.p_cs);

			/* Extract the level info */
			clev = atoi(the_score.cur_lev);
			mlev = atoi(the_score.max_lev);
			cdun = atoi(the_score.cur_dun);
			mdun = atoi(the_score.max_dun);

			in_arena = atoi(the_score.inside_arena);
			in_quest = atoi(the_score.inside_quest);

			/* Hack -- extract the gold and such */
			for (user = the_score.uid; isspace(*user); user++) /* loop */;
			for (when = the_score.day; isspace(*when); when++) /* loop */;
			for (gold = the_score.gold; isspace(*gold); gold++) /* loop */;
			for (aged = the_score.turns; isspace(*aged); aged++) /* loop */;

			/* Dump some info */
#if 0 // DGDGDGDG
			sprintf(out_val, "%3d.%9s  %s the %s %s, Level %d",
			        place, the_score.pts, the_score.who,
			        get_player_race_name(pr, ps), class_info[pc].spec[pcs].title,
			        clev);
#endif
			/* Append a "maximum level" */
			if (mlev > clev) strcat(out_val, format(" (Max %d)", mlev));

			/* Dump the first line */
			c_put_str(attr, out_val, n*4 + 2, 0);

			/* Another line of info */
			if (in_arena)
			{
				sprintf(out_val, "               Killed by %s in the Arena",
				        the_score.how);
			}
			else if (in_quest)
			{
				sprintf(out_val, "               Killed by %s while questing",
				        the_score.how);
			}
			/* Hack -- some people die in the town */
			else if (!cdun)
			{
				sprintf(out_val, "               Killed by %s in the Town",
				        the_score.how);
			}
			else
			{
				sprintf(out_val, "               Killed by %s on %s %ld",
				        the_score.how, "Dungeon Level", cdun);
			}

			/* Append a "maximum level" */
			if (mdun > cdun) strcat(out_val, format(" (Max %d)", mdun));

			/* Dump the info */
			c_put_str(attr, out_val, n*4 + 3, 0);

			/* And still another line of info */
			sprintf(out_val,
			        "               (User %s, Date %s, Gold %s, Turn %s).",
			        user, when, gold, aged);
			c_put_str(attr, out_val, n*4 + 4, 0);
		}


		/* Wait for response */
		prt("[Press ESC to quit, any other key to continue.]", 23, 17);
		j = inkey();
		prt("", 23, 0);

		/* Hack -- notice Escape */
		if (j == ESCAPE) break;
	}
}


/*
 * Hack -- Display the scores in a given range and quit.
 *
 * This function is only called from "main.c" when the user asks
 * to see the "high scores".
 */
void display_scores(s32b from, s32b to)
{
	char buf[1024];

	/* Build the filename */
	path_build(buf, 1024, TENGINE_DIR_SCORES, "scores.raw");

	/* Open the binary high score file, for reading */
	highscore_fd = my_fopen(buf, "r");

	/* Paranoia -- No score file */
	if (highscore_fd == NULL) quit("Score file unavailable.");

	/* Clear screen */
	Term_clear();

	/* Display the scores */
	display_scores_aux(from, to, -1, NULL);

	/* Shut the high score file */
	my_fclose(highscore_fd);

	/* Forget the high score fd */
	highscore_fd = NULL;

	/* Quit */
	quit(NULL);
}


/*
 * show_highclass - selectively list highscores based on class
 * -KMW-
 */
void show_highclass(s32b building)
{

	register s32b i = 0, j, m = 0;
	s32b pcs, pr, ps, pc, clev, al;
	high_score the_score;
	char buf[1024], out_val[256];

	switch (building)
	{
	case 1:
		prt("               Busts of Greatest Kings", 5, 0);
		break;
	case 2:
		prt("               Plaque - Greatest Arena Champions", 5, 0);
		break;
	case 10:
		prt("               Plaque - Greatest Fighters", 5, 0);
		break;
	case 11:
		prt("               Spires of the Greatest Magic-Users", 5, 0);
		break;
	case 12:
		prt("               Busts of Greatest Priests", 5, 0);
		break;
	case 13:
		prt("               Wall Inscriptions - Greatest Thieves", 5, 0);
		break;
	case 14:
		prt("               Plaque - Greatest Rangers", 5, 0);
		break;
	case 15:
		prt("               Plaque - Greatest Paladins", 5, 0);
		break;
	case 16:
		prt("               Spires of the Greatest Illusionists", 5, 0);
		break;
	default:
		bell();
		break;
	}

	/* Build the filename */
	path_build(buf, 1024, TENGINE_DIR_SCORES, "scores.raw");

	highscore_fd = my_fopen(buf, "r");

	if (highscore_fd == NULL)
	{
		msg_print("Score file unavailable.");
		msg_print(NULL);
		return;
	}

	if (highscore_seek(0)) return;

	for (i = 0; i < MAX_HISCORES; i++)
		if (highscore_read(&the_score)) break;

	m = 0;
	j = 0;
	clev = 0;

	while ((m < 9) || (j < MAX_HISCORES))
	{
		if (highscore_seek(j)) break;
		if (highscore_read(&the_score)) break;
		pr = atoi(the_score.p_r);
		ps = atoi(the_score.p_s);
		pc = atoi(the_score.p_c);
		pcs = atoi(the_score.p_cs);
		clev = atoi(the_score.cur_lev);
		al = atoi(the_score.arena_number);
		if (((pc == (building - 10)) && (building != 1) && (building != 2)) ||
		                ((building == 1) && (clev >= PY_MAX_LEVEL)) ||
		                ((building == 2) && (al > MAX_ARENA_MONS)))
		{
#if 0 // DGDGDGDG
			sprintf(out_val, "%3d) %s the %s (Level %2d)",
				(m + 1), the_score.who, race_info[pr].title, clev);
#endif
			prt(out_val, (m + 7), 0);
			m++;
		}
		j++;
	}

	/* Now, list the active player if they qualify */
	if ((building == 1) && (p_ptr->lev >= PY_MAX_LEVEL))
	{
#if 0 // DGDGDGDG
		sprintf(out_val, "You) %s the %s (Level %2d)",
			player_name, race_info[p_ptr->prace].title, p_ptr->lev);
#endif
		prt(out_val, (m + 8), 0);
	}
	else if ((building == 2) && (p_ptr->arena_number > MAX_ARENA_MONS))
	{
#if 0 // DGDGDGDG
		sprintf(out_val, "You) %s the %s (Level %2d)",
			player_name, race_info[p_ptr->prace].title, p_ptr->lev);
#endif
		prt(out_val, (m + 8), 0);
	}
	else if ((building != 1) && (building != 2))
	{
#if 0 // DGDGDGDG
		if ((p_ptr->lev > clev) && (p_ptr->pclass == (building - 10)))
		{
			sprintf(out_val, "You) %s the %s (Level %2d)",
				player_name, race_info[p_ptr->prace].title, p_ptr->lev);
			prt(out_val, (m + 8), 0);
		}
#endif
	}

	my_fclose(highscore_fd);
	highscore_fd = NULL;
	msg_print("Hit any key to continue");
	msg_print(NULL);
	for (j = 5; j < 18; j++)
		prt("", j, 0);
}


/*
 * Race Legends
 * -KMW-
 */
void race_score(s32b race_num)
{
	register s32b i = 0, j, m = 0;
	s32b pr, ps, pc, clev, pcs, al, lastlev;
	high_score the_score;
	char buf[1024], out_val[256], tmp_str[80];

	lastlev = 0;

	/* rr9: TODO - pluralize the race */
#if 0 // DGDGDGDG
	sprintf(tmp_str, "The Greatest of all the %s", race_info[race_num].title);
#endif
	prt(tmp_str, 5, 15);

	/* Build the filename */
	path_build(buf, 1024, TENGINE_DIR_SCORES, "scores.raw");

	/* Open the highscore file */
	highscore_fd = my_fopen(buf, "r");

	if (highscore_fd == NULL)
	{
		msg_print("Score file unavailable.");
		msg_print(NULL);
		return;
	}

	if (highscore_seek(0)) return;

	for (i = 0; i < MAX_HISCORES; i++)
	{
		if (highscore_read(&the_score)) break;
	}

	m = 0;
	j = 0;

	while ((m < 10) || (j < MAX_HISCORES))
	{
		if (highscore_seek(j)) break;
		if (highscore_read(&the_score)) break;
		pr = atoi(the_score.p_r);
		ps = atoi(the_score.p_s);
		pc = atoi(the_score.p_c);
		pcs = atoi(the_score.p_cs);
		clev = atoi(the_score.cur_lev);
		al = atoi(the_score.arena_number);
		if (pr == race_num)
		{
#if 0 // DGDGDGDG
			sprintf(out_val, "%3d) %s the %s (Level %3d)",
			        (m + 1), the_score.who,
				race_info[pr].title, clev);
#endif
			prt(out_val, (m + 7), 0);
			m++;
			lastlev = clev;
		}
		j++;
	}

	/* add player if qualified */
#if 0 // DGDGDGDG
	if ((p_ptr->prace == race_num) && (p_ptr->lev >= lastlev))
	{
		sprintf(out_val, "You) %s the %s (Level %3d)",
			player_name, race_info[p_ptr->prace].title, p_ptr->lev);
		prt(out_val, (m + 8), 0);
	}
#endif

	my_fclose(highscore_fd);
	highscore_fd = NULL;
}


/*
 * Race Legends
 * -KMW-
 */
void race_legends(void)
{
#if 0 // DGDGDGDG
	s32b i, j;

	for (i = 0; i < max_rp_idx; i++)
	{
		race_score(i);
		msg_print("Hit any key to continue");
		msg_print(NULL);
		for (j = 5; j < 19; j++)
			prt("", j, 0);
	}
#endif
}

/*
 * Predict the players location, and display it.
 */
errr predict_score(void)
{
	s32b j;

	high_score the_score;


	/* No score file */
	if (highscore_fd == NULL)
	{
		msg_print("Score file unavailable.");
		msg_print(NULL);
		return (0);
	}


	/* Save the version */
	sprintf(the_score.what, "%lu.%lu.%lu",
	        VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);

	/* Calculate and save the points */
	sprintf(the_score.pts, "%9lu", (long)total_points());

	/* Save the current gold */
	sprintf(the_score.gold, "%9lu", (long)p_ptr->au);

	/* Save the current turn */
	sprintf(the_score.turns, "%9lu", (long)turn);

	/* Hack -- no time needed */
	strcpy(the_score.day, "TODAY");

	/* Save the player name (15 chars) */
	sprintf(the_score.who, "%-.15s", player_name);

#if 0 // DGDGDGDG
	/* Save the player info XXX XXX XXX */
	sprintf(the_score.uid, "%7u", player_uid);
	sprintf(the_score.sex, "%c", (p_ptr->psex ? 'm' : 'f'));
	sprintf(the_score.p_r, "%2d", p_ptr->prace);
	sprintf(the_score.p_s, "%2d", p_ptr->pracem);
	sprintf(the_score.p_c, "%2d", p_ptr->pclass);
	sprintf(the_score.p_cs, "%2d", p_ptr->pspec);
#endif
	/* Save the level and such */
	sprintf(the_score.cur_lev, "%3d", p_ptr->lev);
	sprintf(the_score.cur_dun, "%3d", dun_level);
	sprintf(the_score.max_lev, "%3d", p_ptr->max_plv);
	sprintf(the_score.max_dun, "%3d", max_dlv[dungeon_type]);

	sprintf(the_score.arena_number, "%3d", p_ptr->arena_number);  /* -KMW- */
	sprintf(the_score.inside_arena, "%3d", p_ptr->inside_arena);
	sprintf(the_score.inside_quest, "%3d", p_ptr->inside_quest);
	sprintf(the_score.exit_bldg, "%3d", p_ptr->exit_bldg);  /* -KMW- */

	/* Hack -- no cause of death */
	strcpy(the_score.how, "nobody (yet!)");


	/* See where the entry would be placed */
	j = highscore_where(&the_score);


	/* Hack -- Display the top fifteen scores */
	if (j < 10)
	{
		display_scores_aux(0, 15, j, &the_score);
	}

	/* Display some "useful" scores */
	else
	{
		display_scores_aux(0, 5, -1, NULL);
		display_scores_aux(j - 2, j + 7, j, &the_score);
	}


	/* Success */
	return (0);
}



/*
 * Change the player into a King!                        -RAK-
 */
static void kingly(void)
{
	PHYSFS_file *fp;
	char buf[1024];

	/* Hack -- retire in town */
	dun_level = 0;

	/* Fake death */
	(void)strcpy(died_from, "Ripe Old Age");

	/* Restore the experience */
	p_ptr->exp = p_ptr->max_exp;

	/* Restore the level */
	p_ptr->lev = p_ptr->max_plv;

	/* Hack -- Instant Gold */
	p_ptr->au += 10000000L;

	/* Clear screen */
	Term_clear();

	/* Build the filename */
	path_build(buf, 1024, TENGINE_DIR_FILE, "winner.txt");

	/* Open the News file */
	fp = my_fopen(buf, "r");

	/* Dump */
	if (fp)
	{
		s32b i = 0;

		/* Dump the file to the screen */
		while (0 == my_fgets(fp, buf, 1024))
		{
			/* Display and advance */
			display_message(0, i++, strlen(buf), TERM_WHITE, buf);
		}

		/* Close */
		my_fclose(fp);
	}
	else
	{
		/* Display a crown */
		put_str("#", 1, 34);
		put_str("#####", 2, 32);
		put_str("#", 3, 34);
		put_str(",,,  $$$  ,,,", 4, 28);
		put_str(",,=$   \"$$$$$\"   $=,,", 5, 24);
		put_str(",$$        $$$        $$,", 6, 22);
		put_str("*>         <*>         <*", 7, 22);
		put_str("$$         $$$         $$", 8, 22);
		put_str("\"$$        $$$        $$\"", 9, 22);
		put_str("\"$$       $$$       $$\"", 10, 23);
		put_str("*#########*#########*", 11, 24);
		put_str("*#########*#########*", 12, 24);

		/* Display a message */
		put_str("Veni, Vidi, Vici!", 15, 26);
		put_str("I came, I saw, I conquered!", 16, 21);
	}

	/* Flush input */
	flush();

	/* Wait for response */
	pause_line(15);
}


/*
 * Wipe the saved levels
 */
void wipe_saved()
{
	char buf[1024];
	char **rc;
	char **i;

	if (!file_exist(savefile)) return;

	/* Wipe all files and the directory */
	rc = PHYSFS_enumerateFiles(savefile);

	for (i = rc; *i != NULL; i++)
	{
		/* Starts with dun means it is a permanent level file */
		if ((!prefix(*i, "dun")) && (!prefix(*i, "wild"))) continue;

		/* Kill it! */
		path_build(buf, 1024, savefile, *i);
		fd_kill_compress(buf);
	}

	PHYSFS_freeList(rc);
}


/*
 * Close up the current game (player may or may not be dead)
 *
 * This function is called only from "main.c" and "signals.c".
 */
void close_game(void)
{
	/* Handle stuff */
	handle_stuff();

	/* Flush the messages */
	msg_print(NULL);

	/* Flush the input */
	flush();


	/* No suspending now */
	signals_ignore_tstp();

	/* Hack -- Character is now "icky" */
	character_icky++;

	/* Handle death */
	if (death)
	{
		/* Handle retirement */
		if (total_winner)
		{
			/* Write a note, if that option is on */
			if (take_notes)
			{
				add_note_type(NOTE_WINNER);
			}

			kingly();
		}

		/* Wipe the saved levels */
		wipe_saved();

		/* Save memories */
		if (!save_player()) msg_print("death save failed!");

		/* You are dead */
		print_tomb();

		/* Show more info */
		show_info();

		/* Write a note */
		if (take_notes)
		{
			char long_day[30];
			char buf[80];
			time_t ct = time((time_t*)NULL);

			/* Get the date */
			strftime(long_day, 30,
			         "%Y-%m-%d at %H:%M:%S", localtime(&ct));

			/* Create string */
			sprintf(buf, "\n%s was killed by %s on %s\n", player_name,
			        died_from, long_day);

			/* Output to the notes file */
			output_note(buf);
		}

		/* Dump bones file */
//		make_bones();

		/* Handle score, show Top scores */
//		top_twenty();
	}

	/* Still alive */
	else
	{
		is_autosave = FALSE;

		/* Save the game */
		do_cmd_save_game();

		/* If note-taking enabled, write session end to notes file */
		if (take_notes)
		{
			add_note_type(NOTE_SAVE_GAME);
		}

		/* Prompt for scores XXX XXX XXX */
		prt("Press Return (or Escape).", 0, 40);

		/* Predict score (or ESCAPE) */
//		if (inkey() != ESCAPE) predict_score();
	}

	/* Allow suspending now */
	signals_handle_tstp();
}


/*
 * Handle abrupt death of the visual system
 *
 * This routine is called only in very rare situations, and only
 * by certain visual systems, when they experience fatal errors.
 *
 * XXX XXX Hack -- clear the death flag when creating a HANGUP
 * save file so that player can see tombstone when restart.
 */
void exit_game_panic(void)
{
	/* If nothing important has happened, just quit */
	if (!character_generated || character_saved) quit("panic");

	/* Mega-Hack -- see "msg_print()" */
	msg_flag = FALSE;

	/* Clear the top line */
	prt("", 0, 0);

	/* Hack -- turn off some things */
	disturb(1, 0);

	/* Hardcode panic save */
	panic_save = 1;

	/* Forbid suspend */
	signals_ignore_tstp();

	/* Indicate panic save */
	(void)strcpy(died_from, "(panic save)");

	/* Panic save, or get worried */
	if (!save_player()) quit("panic save failed!");

	/* Successful panic save */
	quit("panic save succeeded!");
}


/*
 * Grab a randomly selected line in lib/file/file_name
 */
errr get_rnd_line(char *file_name, char *output)
{
	PHYSFS_file *fp;

	char buf[1024];

	s32b lines = 0;

	s32b line;

	s32b i;


	/* Clear the output buffer */
	strcpy(output, "");

	/* test hack */
	if (wizard && cheat_xtra) msg_print(file_name);

	/* Build the filename */
	path_build(buf, 1024, TENGINE_DIR_FILE, file_name);

	/* Open the file */
	fp = my_fopen(buf, "r");

	/* Failed */
	if (!fp) return ( -1);

	/* Read the first line */
	if (0 != my_fgets(fp, buf, 80))
	{
		my_fclose(fp);
		return ( -1);
	}

	/* Retrieve number of valid lines in the file */
	lines = atoi(buf);

	/* Pick a line in the file */
	line = randint(lines);

	/*
	 * Scan through the file XXX XXX XXX
	 * Seemingly wrong use of the counter is justified by the
	 * stupid 'buffer' lines in the random text files.
	 */
	for (i = 0; i <= line; i++)
	{
		if (0 != my_fgets(fp, buf, 80))
		{
			my_fclose(fp);
			return ( -1);
		}

		/* Found the line */
		if (i == line) break;
	}

	/* Copy the line to the output buffer */
	strcpy(output, buf);

	/* Close the file */
	my_fclose(fp);

	/* Success */
	return (0);
}


/*
 * Read line'th line file the file
 * and return pointer to it, or NULL if it fails.
 *
 * Nuked the static buffer. Caller should provide one. -- pelpel
 *
 * Caution: 'linbuf' should be at least 80 byte long.
 */
char *get_line(char* fname, cptr fdir, char *linbuf, s32b line)
{
	PHYSFS_file* fp;
	s32b i;
	char buf[1024];


	/* Don't count the first line in the file, which is a comment line */
	line++;

	/* Build the filename */
	path_build(buf, 1024, fdir, fname);

	/* Open the file */
	fp = my_fopen(buf, "r");

	/* Failed */
	if (!fp) return (NULL);

	/* Read past specified number of lines */
	for (i = 0; i <= line; i++)
	{
		/* Oops */
		if (my_fgets(fp, linbuf, 80) != 0)
		{
			my_fclose(fp);
			return (NULL);
		}
	}

	my_fclose(fp);

	return (linbuf);
}


/*
 * Return a line for a speaking unique, by Matt G.
 *
 * XXX XXX XXX Opening a file and scanning it through whenever a unique
 * tries to say something? Something like DELAY_LOAD_?_TEXT would be
 * much better -- pelpel
 *
 * XXX XXX XXX I must say the original is an extremely poor and unreliable
 * implementation...  I removed noxious flag -- I'm too stupid to
 * understand such complexities -- and added extra error checkings
 * and made sure fd is always closed -- pelpel
 */
errr get_xtra_line(char *file_name, monster_type *m_ptr, char *output)
{
	PHYSFS_file *fp;
	char buf[1024];
	s32b line;
	s32b num_entries;
	s32b i;
	s32b mnum;


	/* Clear the message buffer */
	strcpy(output, "");

	/* test and DEBUG hack */
	if (wizard && cheat_xtra)
	{
		msg_print(file_name);
	}

	/* Build the filename */
	path_build(buf, 1024, TENGINE_DIR_FILE, file_name);

	/* Open the file */
	fp = my_fopen(buf, "r");

	/* Failed */
	if (!fp) return ( -1);

	/* Monster number we are looking for */
	mnum = m_ptr->r_idx;

	/* Find matching N: line */
	while (1)
	{
		s32b n;

		/* Read a line */
		if (my_fgets(fp, buf, 90) != 0)
		{
			my_fclose(fp);
			return ( -1);
		}

		/* Not a N: line */
		if (buf[0] != 'N') continue;

		/* Skip "N:" and parse off a number */
		sscanf(buf + 2, "%ld", &n);

		/* Match found */
		if (n == mnum) break;
	}

	/* Retrieve number of normal messages */
	while (1)
	{
		/* Read next line */
		if (my_fgets(fp, buf, 90) != 0)
		{
			my_fclose(fp);
			return ( -1);
		}

		/* The first line not beginning with 'N:' holds number of lines */
		if (buf[0] != 'N')
		{
			num_entries = atoi(buf);
			break;
		}
	}

	/* The monster is afraid */
	if (m_ptr->monfear)
	{
		/* Read past normal lines */
		for (line = 0; line < num_entries + 1; line++)
		{
			if (my_fgets(fp, buf, 90))
			{
				my_fclose(fp);
				return ( -1);
			}
		}

		/* Retrieve number of 'afraid' lines */
		num_entries = atoi(buf);
	}


	/* Pick a random line */
	line = rand_int(num_entries);

	/* test and DEBUG hack */
	if (wizard && cheat_xtra)
	{
		sprintf(buf, "Line number %ld", line);
		msg_print(buf);
	}

	/* Find the selected line */
	for (i = 0; i <= line; i++)
	{
		/* Oops */
		if (0 != my_fgets(fp, buf, 90))
		{
			my_fclose(fp);
			return ( -1);
		}

		/* Found it */
		if (i == line) break;
	}

	/* Copy it to the output buffer */
	strcpy(output, buf);

	/* Close the file */
	my_fclose(fp);

	/* Success */
	return (0);
}


#ifdef HANDLE_SIGNALS


#include <signal.h>


/*
 * Handle signals -- suspend
 *
 * Actually suspend the game, and then resume cleanly
 */
static void handle_signal_suspend(int sig)
{
	/* Disable handler */
	(void)signal(sig, SIG_IGN);

#ifdef SIGSTOP

	/* Flush output */
	Term_fresh();

	/* Suspend the "Term" */
	Term_xtra(TERM_XTRA_ALIVE, 0);

	/* Suspend ourself */
	(void)kill(0, SIGSTOP);

	/* Resume the "Term" */
	Term_xtra(TERM_XTRA_ALIVE, 1);

	/* Redraw the term */
	Term_redraw();

	/* Flush the term */
	Term_fresh();

#endif

	/* Restore handler */
	(void)signal(sig, handle_signal_suspend);
}


/*
 * Handle signals -- simple (interrupt and quit)
 *
 * This function was causing a *huge* number of problems, so it has
 * been simplified greatly.  We keep a global variable which counts
 * the number of times the user attempts to kill the process, and
 * we commit suicide if the user does this a certain number of times.
 *
 * We attempt to give "feedback" to the user as he approaches the
 * suicide thresh-hold, but without penalizing accidental keypresses.
 *
 * To prevent messy accidents, we should reset this global variable
 * whenever the user enters a keypress, or something like that.
 */
static void handle_signal_simple(int sig)
{
	/* Disable handler */
	(void)signal(sig, SIG_IGN);


	/* Nothing to save, just quit */
	if (!character_generated || character_saved) quit(NULL);


	/* Count the signals */
	signal_count++;


	/* Terminate dead characters */
	if (death)
	{
		/* Mark the savefile */
		(void)strcpy(died_from, "Abortion");

		/* Close stuff */
		close_game();

		/* Quit */
		quit("interrupt");
	}

	/* Allow suicide (after 5) */
	else if (signal_count >= 5)
	{
		/* Cause of "death" */
		(void)strcpy(died_from, "Interrupting");

		/* Stop playing */
		alive = FALSE;

		/* Suicide */
		death = TRUE;

		/* Leaving */
		p_ptr->leaving = TRUE;

		/* Close stuff */
		close_game();

		/* Quit */
		quit("interrupt");
	}

	/* Give warning (after 4) */
	else if (signal_count >= 4)
	{
		/* Make a noise */
		Term_xtra(TERM_XTRA_NOISE, 0);

		/* Clear the top line */
		Term_erase(0, 0, 255);

		/* Display the cause */
		Term_putstr(0, 0, -1, TERM_WHITE, "Contemplating suicide!");

		/* Flush */
		Term_fresh();
	}

	/* Give warning (after 2) */
	else if (signal_count >= 2)
	{
		/* Make a noise */
		Term_xtra(TERM_XTRA_NOISE, 0);
	}

	/* Restore handler */
	(void)signal(sig, handle_signal_simple);
}


/*
 * Handle signal -- abort, kill, etc
 */
static void handle_signal_abort(int sig)
{
	char buf[32];
	cptr sig_tag[33] = {
		"SIGHUP",
		"SIGINT",
		"SIGQUIT",
		"SIGILL",
		"SIGTRAP",
		"SIGABRT",
		"SIGIOT",
		"SIGBUS",
		"SIGFPE",
		"SIGKILL",
		"SIGUSR1",
		"SIGSEGV",
		"SIGUSR2",
		"SIGPIPE",
		"SIGALRM",
		"SIGTERM",
		"SIGSTKFLT",
		"SIGCHLD",
		"SIGCONT",
		"SIGSTOP",
		"SIGTSTP",
		"SIGTTIN",
		"SIGTTOU",
		"SIGURG",
		"SIGXCPU",
		"SIGXFSZ",
		"SIGVTALRM",
		"SIGPROF",
		"SIGWINCH",
		"SIGIO / SIGPOLL",
		"SIGPWR",
		"SIGSYS",
		"SIGUNUSED" };

	/* Disable handler */
	(void)signal(sig, SIG_IGN);

	/* Emergency lua stacktrace */
	{
		s32b stack_id = 0;
		PHYSFS_file *fff = my_fopen("/engine_errors.log", "a+");
		if (fff) my_fprintf(fff, "Lua emergency stacktrace:\n");
		else printf("Lua emergency stacktrace:\n");
		while (TRUE)
		{
			lua_Debug ar;
			if (!lua_getstack(L, stack_id, &ar)) break;
			lua_getinfo(L, "lnuS", &ar);
			if (fff) my_fprintf(fff, "STACK (%d): name(%s), namewhat(%s), curline(%d), linedef(%d), what(%s), source(%s // %s)\n", stack_id, ar.name, ar.namewhat, ar.currentline, ar.linedefined, ar.what, ar.source, ar.short_src);
			else printf("STACK (%ld): name(%s), namewhat(%s), curline(%d), linedef(%d), what(%s), source(%s // %s)\n", stack_id, ar.name, ar.namewhat, ar.currentline, ar.linedefined, ar.what, ar.source, ar.short_src);
			stack_id++;
		}
		if (fff) my_fprintf(fff, "Lua emergency stacktrace end. Proceeding to die. Arrrrg!\n");
		else fprintf(fff, "Lua emergency stacktrace end. Proceeding to die. Arrrrg!\n");
		if (fff) my_fclose(fff);
	}

	/* Nothing to save, just quit */
	if (!character_generated || character_saved) quit(NULL);


	/* Clear the bottom line */
	Term_erase(0, 23, 255);

	/* Give a warning */
	Term_putstr(0, 23, -1, TERM_RED,
	            "A gruesome software bug LEAPS out at you!");

	/* Message */
	Term_putstr(45, 23, -1, TERM_RED, "Panic save...");

	/* Flush output */
	Term_fresh();

	/* Panic Save */
	panic_save = 1;

	/* Panic save */
	(void)strcpy(died_from, "(panic save)");

	/* Forbid suspend */
	signals_ignore_tstp();

	/* Attempt to save */
	if (save_player())
	{
		Term_putstr(45, 23, -1, TERM_RED, "Panic save succeeded!");
	}

	/* Save failed */
	else
	{
		Term_putstr(45, 23, -1, TERM_RED, "Panic save failed!");
	}

	/* Flush output */
	Term_fresh();

	/* Quit */
	sprintf(buf, sig_tag[sig]);
	quit(format("software bug %d %d (signal %s)", p_ptr->px, p_ptr->py, buf));
}




/*
 * Ignore SIGTSTP signals (keyboard suspend)
 */
void signals_ignore_tstp(void)
{

#ifdef SIGTSTP
	(void)signal(SIGTSTP, SIG_IGN);
#endif

}

/*
 * Handle SIGTSTP signals (keyboard suspend)
 */
void signals_handle_tstp(void)
{

#ifdef SIGTSTP
	(void)signal(SIGTSTP, handle_signal_suspend);
#endif

}


/*
 * Prepare to handle the relevant signals
 */
void signals_init(void)
{

#ifdef SIGHUP
	(void)signal(SIGHUP, SIG_IGN);
#endif


#ifdef SIGTSTP
	(void)signal(SIGTSTP, handle_signal_suspend);
#endif


#ifdef SIGINT
	(void)signal(SIGINT, handle_signal_simple);
#endif

#ifdef SIGQUIT
	(void)signal(SIGQUIT, handle_signal_simple);
#endif


#ifdef SIGFPE
	(void)signal(SIGFPE, handle_signal_abort);
#endif

#ifdef SIGILL
	(void)signal(SIGILL, handle_signal_abort);
#endif

#ifdef SIGTRAP
	(void)signal(SIGTRAP, handle_signal_abort);
#endif

#ifdef SIGIOT
	(void)signal(SIGIOT, handle_signal_abort);
#endif

#ifdef SIGKILL
	(void)signal(SIGKILL, handle_signal_abort);
#endif

#ifdef SIGBUS
	(void)signal(SIGBUS, handle_signal_abort);
#endif

#ifdef SIGSEGV
	(void)signal(SIGSEGV, handle_signal_abort);
#endif

#ifdef SIGTERM
	(void)signal(SIGTERM, handle_signal_abort);
#endif

#ifdef SIGPIPE
	(void)signal(SIGPIPE, handle_signal_abort);
#endif

#ifdef SIGEMT
	(void)signal(SIGEMT, handle_signal_abort);
#endif

#ifdef SIGDANGER
	(void)signal(SIGDANGER, handle_signal_abort);
#endif

#ifdef SIGSYS
	(void)signal(SIGSYS, handle_signal_abort);
#endif

#ifdef SIGXCPU
	(void)signal(SIGXCPU, handle_signal_abort);
#endif

#ifdef SIGPWR
	(void)signal(SIGPWR, handle_signal_abort);
#endif

}


#else        /* HANDLE_SIGNALS */


/*
* Do nothing
*/
void signals_ignore_tstp(void)
{}

/*
* Do nothing
*/
void signals_handle_tstp(void)
{}

/*
* Do nothing
*/
void signals_init(void)
{}


#endif        /* HANDLE_SIGNALS */
