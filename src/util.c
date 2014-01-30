/* File: util.c */

/* Purpose: Angband utilities -BEN- */


#include "angband.h"




#ifndef HAS_STRICMP

/*
* For those systems that don't have "stricmp()"
*
* Compare the two strings "a" and "b" ala "strcmp()" ignoring case.
*/
s32b stricmp(cptr a, cptr b)
{
	cptr s1, s2;
	char z1, z2;

	/* Scan the strings */
	for (s1 = a, s2 = b; TRUE; s1++, s2++)
	{
		z1 = toupper((unsigned char) *s1);
		z2 = toupper((unsigned char) *s2);
		if (z1 < z2) return ( -1);
		if (z1 > z2) return (1);
		if (!z1) return (0);
	}
}

#endif


#ifdef SET_UID


/*
* Hack -- External functions
*/
extern struct passwd *getpwuid();
extern struct passwd *getpwnam();
#endif /* SET_UID */

/*
* Find a default user name from the system.
*/
void user_name(char *buf, s32b id)
{
#if 0
#ifdef SET_UID
	struct passwd *pw;

	/* Look up the user name */
	if ((pw = getpwuid(id)))
	{
		(void)strcpy(buf, pw->pw_name);
		buf[16] = '\0';

#ifdef CAPITALIZE_USER_NAME
		/* Hack -- capitalize the user name */
		if (islower(buf[0])) buf[0] = toupper(buf[0]);
#endif /* CAPITALIZE_USER_NAME */

		return;
	}
#endif /* SET_UID */
#endif
	/* Oops.  Hack -- default to "PLAYER" */
	strcpy(buf, "PLAYER");
}




/*
* The concept of the "file" routines below (and elsewhere) is that all
* file handling should be done using as few routines as possible, since
* every machine is slightly different, but these routines always have the
* same semantics.
*
* In fact, perhaps we should use the "path_parse()" routine below to convert
* from "canonical" filenames (optional leading tilde's, internal wildcards,
* slash as the path seperator, etc) to "system" filenames (no special symbols,
* system-specific path seperator, etc).  This would allow the program itself
* to assume that all filenames are "Unix" filenames, and explicitly "extract"
* such filenames if needed (by "path_parse()", or perhaps "path_canon()").
*
* Note that "path_temp" should probably return a "canonical" filename.
*
* Note that "my_fopen()" and "my_open()" and "my_make()" and "my_kill()"
* and "my_move()" and "my_copy()" should all take "canonical" filenames.
*
* Note that "canonical" filenames use a leading "slash" to indicate an absolute
* path, and a leading "tilde" to indicate a special directory, and default to a
* relative path, but MSDOS uses a leading "drivename plus colon" to indicate the
* use of a "special drive", and then the rest of the path is parsed "normally".
*
* We should probably parse a leading "~~/" as referring to "TENGINE_DIR". (?)
*/


/*
* Hack -- acquire a "temporary" file name if possible
*
* This filename is always in "system-specific" form.
*/
errr path_temp(char *buf, s32b max)
{
	static u32b tmp_id = 0;

	/* Format to length */
	strnfmt(buf, max, "/tmp/tmp-%ld", tmp_id++);

	/* Success */
	return (0);
}


/*
* Create a new path by appending a file (or directory) to a path
*
* This requires no special processing on simple machines, except
* for verifying the size of the filename, but note the ability to
* bypass the given "path" with certain special file-names.
*
* Note that the "file" may actually be a "sub-path", including
* a path and a file.
*
* Note that this function yields a path which must be "parsed"
* using the "parse" function above.
*/
errr path_build(char *buf, s32b max, cptr path, cptr file)
{
	/* Special file */
	if (file[0] == '~')
	{
		/* Use the file itself */
		strnfmt(buf, max, "%s", file);
	}

	/* Absolute file, on "normal" systems */
	else if (prefix(file, PATH_SEP) && !streq(PATH_SEP, ""))
	{
		/* Use the file itself */
		strnfmt(buf, max, "%s", file);
	}

	/* No path given */
	else if (!path[0])
	{
		/* Use the file itself */
		strnfmt(buf, max, "%s", file);
	}

	/* Path and File */
	else
	{
		/* Build the new path */
		if (path[strlen(path) - 1] == PATH_SEP_CHAR)
			strnfmt(buf, max, "%s%s", path, file);
		else
			strnfmt(buf, max, "%s%s%s", path, PATH_SEP, file);
	}

	/* Success */
	return (0);
}


/*
* Like "fgets()" but for strings
*
* Process tabs, strip internal non-printables
*/
errr my_str_fgets(cptr full_text, char *buf, size_t n)
{
	static size_t last_read = 0;
	static size_t full_size;
	size_t i = 0;

	char *s;

	char tmp[1024];

	/* Initialize */
	if (!buf)
	{
		last_read = 0;
		full_size = strlen(full_text);
		return 0;
	}

	/* The end!  */
	if (last_read >= full_size)
	{
		return 1;
	}

	while (last_read < full_size)
	{
		char c = full_text[last_read++];

		tmp[i++] = c;

		/* Dont overflow */
		if (i >= 1024)
		{
			/* Nothing */
			buf[0] = '\0';

			/* Failure */
			return (1);
		}

		if ((c == '\n') || (c == '\r'))
		{
			break;
		}
	}

	tmp[i++] = '\n';
	tmp[i++] = '\0';

	/* We need it again */
	i = 0;

	/* Convert weirdness */
	for (s = tmp; *s; s++)
	{
		/* Handle newline */
		if (*s == '\n')
		{
			/* Terminate */
			buf[i] = '\0';

			/* Success */
			return (0);
		}

		/* Handle tabs */
		else if (*s == '\t')
		{
			/* Hack -- require room */
			if (i + 8 >= n) break;

			/* Append a space */
			buf[i++] = ' ';

			/* Append some more spaces */
			while (!(i % 8)) buf[i++] = ' ';
		}

		/* Handle printables */
		else if (isprint(*s))
		{
			/* Copy */
			buf[i++] = *s;

			/* Check length */
			if (i >= n) break;
		}
	}

	/* Nothing */
	buf[0] = '\0';

	/* Failure */
	return (1);
}

char *my_physfs_fgets(char *buf, size_t n, PHYSFS_file *fff)
{
	size_t i;

	if (PHYSFS_eof(fff))
		return NULL;

	for (i = 0; i < n - 2; i++)
	{
		s32b c = fgetc(fff);

		if ((c == '\n') || (c == '\r') || (c == EOF))
			break;

		buf[i] = c;
	}
	buf[i] = '\n';
	buf[i + 1] = '\0';

	return buf;
}

/*
* Hack -- replacement for "fgets()"
*
* Read a string, without a newline, to a file
*
* Process tabs, strip internal non-printables
*/
errr my_fgets(PHYSFS_file *fff, char *buf, size_t n)
{
	size_t i = 0;

	char *s;

	char tmp[1024];

	/* Read a line */
	if (fgets(tmp, 1024, fff))
	{
		/* Convert weirdness */
		for (s = tmp; *s; s++)
		{
			/* Handle newline */
			if (*s == '\n')
			{
				/* Terminate */
				buf[i] = '\0';

				/* Success */
				return (0);
			}

			/* Handle tabs */
			else if (*s == '\t')
			{
				/* Hack -- require room */
				if (i + 8 >= n) break;

				/* Append a space */
				buf[i++] = ' ';

				/* Append some more spaces */
				while (!(i % 8)) buf[i++] = ' ';
			}

			/* Handle printables */
			else if (isprint(*s))
			{
				/* Copy */
				buf[i++] = *s;

				/* Check length */
				if (i >= n) break;
			}
		}
	}

	/* Nothing */
	buf[0] = '\0';

	/* Failure */
	return (1);
}

/*
* Hack -- replacement for "fputs()"
*
* Dump a string, plus a newline, to a file
*
* XXX XXX XXX Process internal weirdness?
*/
errr my_fputs(PHYSFS_file *fff, cptr buf)
{
	/* Dump, ignore errors */
	(void)fprintf(fff, "%s\n", buf);

	/* Success */
	return (0);
}


/*
* Several systems have no "O_BINARY" flag
*/
#ifndef O_BINARY
# define O_BINARY 0
#endif /* O_BINARY */


bool convert_virtual_path(char *buf, s32b size)
{
	/* is that location rehooked to somewhere else? */
	if (module_initialized)
	{
		cptr new_file;

		call_lua("get_real_file_path", "(s)", "s", buf, &new_file);
		strncpy(buf, new_file, size);
		return TRUE;
	}
	return FALSE;
}

/*
* Hack -- attempt to delete a file
*/
errr fd_kill(cptr file)
{
	char buf[1024];

	/* Hack -- Try to parse the path */
	if (path_parse(buf, 1024, file)) return ( -1);
	convert_virtual_path(buf, 1024);

	/* Remove */
	(void)PHYSFS_delete(buf);

	/* XXX XXX XXX */
	return (0);
}

/* Kills a file, either as "name" or "name.gz" */
void fd_kill_compress(cptr name)
{
	char gz[1024];

	sprintf(gz, "%s.gz", name);

	fd_kill(name);
	fd_kill(gz);
}



/*
* XXX XXX XXX Important note about "colors" XXX XXX XXX
*
* The "TERM_*" color definitions list the "composition" of each
* "Angband color" in terms of "quarters" of each of the three color
* components (Red, Green, Blue), for example, TERM_UMBER is defined
* as 2/4 Red, 1/4 Green, 0/4 Blue.
*
* The following info is from "Torbjorn Lindgren" (see "main-xaw.c").
*
* These values are NOT gamma-corrected.  On most machines (with the
* Macintosh being an important exception), you must "gamma-correct"
* the given values, that is, "correct for the intrinsic non-linearity
* of the phosphor", by converting the given intensity levels based
* on the "gamma" of the target screen, which is usually 1.7 (or 1.5).
*
* The actual formula for conversion is unknown to me at this time,
* but you can use the table below for the most common gamma values.
*
* So, on most machines, simply convert the values based on the "gamma"
* of the target screen, which is usually in the range 1.5 to 1.7, and
* usually is closest to 1.7.  The converted value for each of the five
* different "quarter" values is given below:
*
*  Given     Gamma 1.0       Gamma 1.5       Gamma 1.7     Hex 1.7
*  -----       ----            ----            ----          ---
*   0/4        0.00            0.00            0.00          #00
*   1/4        0.25            0.27            0.28          #47
*   2/4        0.50            0.55            0.56          #8f
*   3/4        0.75            0.82            0.84          #d7
*   4/4        1.00            1.00            1.00          #ff
*
* Note that some machines (i.e. most IBM machines) are limited to a
* hard-coded set of colors, and so the information above is useless.
*
* Also, some machines are limited to a pre-determined set of colors,
* for example, the IBM can only display 16 colors, and only 14 of
* those colors resemble colors used by Angband, and then only when
* you ignore the fact that "Slate" and "cyan" are not really matches,
* so on the IBM, we use "orange" for both "Umber", and "Light Umber"
* in addition to the obvious "Orange", since by combining all of the
* "indeterminate" colors into a single color, the rest of the colors
* are left with "meaningful" values.
*/


/*
* Move the cursor
*/
void move_cursor(s32b row, s32b col)
{
	Term_gotoxy(col, row);
}



/*
* Convert a decimal to a single digit octal number
*/
static char octify(int i)
{
	return (hexsym[i % 8]);
}

/*
* Convert a decimal to a single digit hex number
*/
static char hexify(int i)
{
	return (hexsym[i % 16]);
}


/*
* Convert a octal-digit into a decimal
*/
static s32b deoct(char c)
{
	if (isdigit(c)) return (D2I(c));
	return (0);
}

/*
* Convert a hexidecimal-digit into a decimal
*/
static s32b dehex(char c)
{
	if (isdigit(c)) return (D2I(c));
	if (islower(c)) return (A2I(c) + 10);
	if (isupper(c)) return (A2I(tolower(c)) + 10);
	return (0);
}


static s32b my_stricmp(cptr a, cptr b)
{
	cptr s1, s2;
	char z1, z2;

	/* Scan the strings */
	for (s1 = a, s2 = b; TRUE; s1++, s2++)
	{
		z1 = toupper((unsigned char) *s1);
		z2 = toupper((unsigned char) *s2);
		if (z1 < z2) return ( -1);
		if (z1 > z2) return (1);
		if (!z1) return (0);
	}
}

static s32b my_strnicmp(cptr a, cptr b, s32b n)
{
	cptr s1, s2;
	char z1, z2;

	/* Scan the strings */
	for (s1 = a, s2 = b; n > 0; s1++, s2++, n--)
	{
		z1 = toupper((unsigned char) *s1);
		z2 = toupper((unsigned char) *s2);
		if (z1 < z2) return ( -1);
		if (z1 > z2) return (1);
		if (!z1) return (0);
	}
	return 0;
}


static void trigger_text_to_ascii(char **bufptr, cptr *strptr)
{
	char *s = *bufptr;
	cptr str = *strptr;
	bool mod_status[MAX_MACRO_MOD];

	s32b i, len = 0;
	s32b shiftstatus = 0;
	cptr key_code;

	if (macro_template == NULL)
		return;

	for (i = 0; macro_modifier_chr[i]; i++)
		mod_status[i] = FALSE;
	str++;

	/* Examine modifier keys */
	while (1)
	{
		for (i = 0; macro_modifier_chr[i]; i++)
		{
			len = strlen(macro_modifier_name[i]);

			if (!my_strnicmp(str, macro_modifier_name[i], len))
				break;
		}
		if (!macro_modifier_chr[i]) break;
		str += len;
		mod_status[i] = TRUE;
		if ('S' == macro_modifier_chr[i])
			shiftstatus = 1;
	}
	for (i = 0; i < max_macrotrigger; i++)
	{
		len = strlen(macro_trigger_name[i]);
		if (!my_strnicmp(str, macro_trigger_name[i], len) && ']' == str[len])
		{
			/* a trigger name found */
			break;
		}
	}

	/* Invalid trigger name? */
	if (i == max_macrotrigger)
	{
		str = strchr(str, ']');
		if (str)
		{
			*s++ = (char)31;
			*s++ = (char)13;
			*bufptr = s;
			*strptr = str;  /* where **strptr == ']' */
		}
		return;
	}

	key_code = macro_trigger_keycode[shiftstatus][i];
	str += len;

	*s++ = (char)31;
	for (i = 0; macro_template[i]; i++)
	{
		char ch = macro_template[i];
		s32b j;

		switch (ch)
		{
		case '&':
			for (j = 0; macro_modifier_chr[j]; j++)
			{
				if (mod_status[j])
					*s++ = macro_modifier_chr[j];
			}
			break;
		case '#':
			strcpy(s, key_code);
			s += strlen(key_code);
			break;
		default:
			*s++ = ch;
			break;
		}
	}
	*s++ = (char)13;

	*bufptr = s;
	*strptr = str;  /* where **strptr == ']' */
	return;
}


/*
* Hack -- convert a printable string into real ascii
*
* I have no clue if this function correctly handles, for example,
* parsing "\xFF" into a (signed) char.  Whoever thought of making
* the "sign" of a "char" undefined is a complete moron.  Oh well.
*/
void text_to_ascii(char *buf, cptr str)
{
	char *s = buf;

	/* Analyze the "ascii" string */
	while (*str)
	{
		/* Backslash codes */
		if (*str == '\\')
		{
			/* Skip the backslash */
			str++;

			/* Macro Trigger */
			if (*str == '[')
			{
				trigger_text_to_ascii(&s, &str);
			}

			/* Hex-mode XXX */
			else if (*str == 'x')
			{
				*s = 16 * dehex(*++str);
				*s++ += dehex(*++str);
			}

			/* Hack -- simple way to specify "backslash" */
			else if (*str == '\\')
			{
				*s++ = '\\';
			}

			/* Hack -- simple way to specify "caret" */
			else if (*str == '^')
			{
				*s++ = '^';
			}

			/* Hack -- simple way to specify "space" */
			else if (*str == 's')
			{
				*s++ = ' ';
			}

			/* Hack -- simple way to specify Escape */
			else if (*str == 'e')
			{
				*s++ = ESCAPE;
			}

			/* Backspace */
			else if (*str == 'b')
			{
				*s++ = '\b';
			}

			/* Newline */
			else if (*str == 'n')
			{
				*s++ = '\n';
			}

			/* Return */
			else if (*str == 'r')
			{
				*s++ = '\r';
			}

			/* Tab */
			else if (*str == 't')
			{
				*s++ = '\t';
			}

			/* Octal-mode */
			else if (*str == '0')
			{
				*s = 8 * deoct(*++str);
				*s++ += deoct(*++str);
			}

			/* Octal-mode */
			else if (*str == '1')
			{
				*s = 64 + 8 * deoct(*++str);
				*s++ += deoct(*++str);
			}

			/* Octal-mode */
			else if (*str == '2')
			{
				*s = 64 * 2 + 8 * deoct(*++str);
				*s++ += deoct(*++str);
			}

			/* Octal-mode */
			else if (*str == '3')
			{
				*s = 64 * 3 + 8 * deoct(*++str);
				*s++ += deoct(*++str);
			}

			/* Skip the final char */
			str++;
		}

		/* Normal Control codes */
		else if (*str == '^')
		{
			str++;
			*s++ = (*str++ & 037);
		}

		/* Normal chars */
		else
		{
			*s++ = *str++;
		}
	}

	/* Terminate */
	*s = '\0';
}


bool trigger_ascii_to_text(char **bufptr, cptr *strptr)
{
	char *s = *bufptr;
	cptr str = *strptr;
	char key_code[100];
	s32b i;
	cptr tmp;

	if (macro_template == NULL)
		return FALSE;

	*s++ = '\\';
	*s++ = '[';

	for (i = 0; macro_template[i]; i++)
	{
		s32b j;
		char ch = macro_template[i];

		switch (ch)
		{
		case '&':
			while ((tmp = strchr(macro_modifier_chr, *str)))
			{
				j = (s32b)(tmp - macro_modifier_chr);
				tmp = macro_modifier_name[j];
				while (*tmp) *s++ = *tmp++;
				str++;
			}
			break;
		case '#':
			for (j = 0; *str && *str != (char)13; j++)
				key_code[j] = *str++;
			key_code[j] = '\0';
			break;
		default:
			if (ch != *str) return FALSE;
			str++;
		}
	}
	if (*str++ != (char)13) return FALSE;

	for (i = 0; i < max_macrotrigger; i++)
	{
		if (!my_stricmp(key_code, macro_trigger_keycode[0][i])
		                || !my_stricmp(key_code, macro_trigger_keycode[1][i]))
			break;
	}
	if (i == max_macrotrigger)
		return FALSE;

	tmp = macro_trigger_name[i];
	while (*tmp) *s++ = *tmp++;

	*s++ = ']';

	*bufptr = s;
	*strptr = str;
	return TRUE;
}

static bool trigger_ascii_to_num(s32b *numptr, cptr str)
{
	char key_code[100];
	s32b  i, mult;
	cptr tmp;

	if (macro_template == NULL)
		return FALSE;

	mult = 0;
	for (i = 0; macro_template[i]; i++)
	{
		s32b j, k;
		char ch = macro_template[i];

		switch (ch)
		{
		case '&':
			while ((tmp = strchr(macro_modifier_chr, *str)))
			{
				s32b place_val = 1;
				j = (s32b)(tmp - macro_modifier_chr);
				for (k = 0; k < j; k++)
					place_val *= 2;
				mult += place_val;
				str++;
			}
			break;
		case '#':
			for (j = 0; *str && *str != (char)13; j++)
				key_code[j] = *str++;
			key_code[j] = '\0';
			break;
		default:
			if (ch != *str) return FALSE;
			str++;
		}
	}
	if (*str++ != (char)13) return FALSE;

	for (i = 0; i < max_macrotrigger; i++)
	{
		if (!my_stricmp(key_code, macro_trigger_keycode[0][i])
		                || !my_stricmp(key_code, macro_trigger_keycode[1][i]))
			break;
	}
	if (i == max_macrotrigger)
		return FALSE;

	*numptr = i + (mult * max_macrotrigger);

	return TRUE;
}


/*
* Hack -- convert a string into a printable form
*/
void ascii_to_text(char *buf, cptr str)
{
	char *s = buf;

	/* Analyze the "ascii" string */
	while (*str)
	{
		byte i = (byte)(*str++);

		/* Macro Trigger */
		if (i == 31)
		{
			if (!trigger_ascii_to_text(&s, &str))
			{
				*s++ = '^';
				*s++ = '_';
			}
		}
		else if (i == ESCAPE)
		{
			*s++ = '\\';
			*s++ = 'e';
		}
		else if (i == ' ')
		{
			*s++ = '\\';
			*s++ = 's';
		}
		else if (i == '\b')
		{
			*s++ = '\\';
			*s++ = 'b';
		}
		else if (i == '\t')
		{
			*s++ = '\\';
			*s++ = 't';
		}
		else if (i == '\n')
		{
			*s++ = '\\';
			*s++ = 'n';
		}
		else if (i == '\r')
		{
			*s++ = '\\';
			*s++ = 'r';
		}
		else if (i == '^')
		{
			*s++ = '\\';
			*s++ = '^';
		}
		else if (i == '\\')
		{
			*s++ = '\\';
			*s++ = '\\';
		}
		else if (i < 32)
		{
			*s++ = '^';
			*s++ = i + 64;
		}
		else if (i < 127)
		{
			*s++ = i;
		}
		else if (i < 64)
		{
			*s++ = '\\';
			*s++ = '0';
			*s++ = octify(i / 8);
			*s++ = octify(i % 8);
		}
		else
		{
			*s++ = '\\';
			*s++ = 'x';
			*s++ = hexify(i / 16);
			*s++ = hexify(i % 16);
		}
	}

	/* Terminate */
	*s = '\0';
}

/*
 * Mirror the macro trigger string being processed, so that if it
 * isn't bound to any macros we can inform the hook about it.
 */
static char trigger__buf[1024];
static s32b  trigger__pos = -1;

/*
* The "macro" package
*
* Functions are provided to manipulate a collection of macros, each
* of which has a trigger pattern string and a resulting action string
* and a small set of flags.
*/



/*
* Determine if any macros have ever started with a given character.
*/
static bool macro__use[256];


/*
* Find the macro (if any) which exactly matches the given pattern
*/
s32b macro_find_exact(cptr pat)
{
	s32b i;

	/* Nothing possible */
	if (!macro__use[(byte)(pat[0])])
	{
		return ( -1);
	}

	/* Scan the macros */
	for (i = 0; i < macro__num; ++i)
	{
		/* Skip macros which do not match the pattern */
		if (!streq(macro__pat[i], pat)) continue;

		/* Found one */
		return (i);
	}

	/* No matches */
	return ( -1);
}


/*
* Find the first macro (if any) which contains the given pattern
*/
static s32b macro_find_check(cptr pat)
{
	s32b i;

	/* Nothing possible */
	if (!macro__use[(byte)(pat[0])])
	{
		return ( -1);
	}

	/* Scan the macros */
	for (i = 0; i < macro__num; ++i)
	{
		/* Skip macros which do not contain the pattern */
		if (!prefix(macro__pat[i], pat)) continue;

		/* Found one */
		return (i);
	}

	/* Nothing */
	return ( -1);
}


/*
* Find the first macro (if any) which contains the given pattern and more
*/
static s32b macro_find_maybe(cptr pat)
{
	s32b i;

	/* Nothing possible */
	if (!macro__use[(byte)(pat[0])])
	{
		return ( -1);
	}

	/* Scan the macros */
	for (i = 0; i < macro__num; ++i)
	{
		/* Skip macros which do not contain the pattern */
		if (!prefix(macro__pat[i], pat)) continue;

		/* Skip macros which exactly match the pattern XXX XXX */
		if (streq(macro__pat[i], pat)) continue;

		/* Found one */
		return (i);
	}

	/* Nothing */
	return ( -1);
}


/*
* Find the longest macro (if any) which starts with the given pattern
*/
static s32b macro_find_ready(cptr pat)
{
	s32b i, t, n = -1, s = -1;

	/* Nothing possible */
	if (!macro__use[(byte)(pat[0])])
	{
		return ( -1);
	}

	/* Scan the macros */
	for (i = 0; i < macro__num; ++i)
	{
		/* Skip macros which are not contained by the pattern */
		if (!prefix(pat, macro__pat[i])) continue;

		/* Obtain the length of this macro */
		t = strlen(macro__pat[i]);

		/* Only track the "longest" pattern */
		if ((n >= 0) && (s > t)) continue;

		/* Track the entry */
		n = i;
		s = t;
	}

	/* Result */
	return (n);
}


/*
* Add a macro definition (or redefinition).
*
* We should use "act == NULL" to "remove" a macro, but this might make it
* impossible to save the "removal" of a macro definition.  XXX XXX XXX
*
* We should consider refusing to allow macros which contain existing macros,
* or which are contained in existing macros, because this would simplify the
* macro analysis code.  XXX XXX XXX
*
* We should consider removing the "command macro" crap, and replacing it
* with some kind of "powerful keymap" ability, but this might make it hard
* to change the "roguelike" option from inside the game.  XXX XXX XXX
*/
errr macro_add(cptr pat, cptr act)
{
	s32b n;


	/* Paranoia -- require data */
	if (!pat || !act) return ( -1);


	/* Look for any existing macro */
	n = macro_find_exact(pat);

	/* Replace existing macro */
	if (n >= 0)
	{
		/* Free the old macro action */
		string_free(macro__act[n]);
	}

	/* Create a new macro */
	else
	{
		/* Acquire a new index */
		n = macro__num++;

		/* Save the pattern */
		macro__pat[n] = string_make(pat);
	}

	/* Save the action */
	macro__act[n] = string_make(act);

	/* Efficiency */
	macro__use[(byte)(pat[0])] = TRUE;

	/* Success */
	return (0);
}



/*
* Initialize the "macro" package
*/
errr macro_init(void)
{
	/* Macro patterns */
	C_MAKE(macro__pat, MACRO_MAX, cptr);

	/* Macro actions */
	C_MAKE(macro__act, MACRO_MAX, cptr);

	/* Success */
	return (0);
}


/*
* Local "need flush" variable
*/
static bool flush_later = FALSE;


/*
* Local variable -- we are inside a "macro action"
*
* Do not match any macros until "ascii 30" is found.
*/
static bool parse_macro = FALSE;

/*
* Local variable -- we are inside a "macro trigger"
*
* Strip all keypresses until a low ascii value is found.
*/
static bool parse_under = FALSE;


/*
* Flush all input chars.  Actually, remember the flush,
* and do a "special flush" before the next "inkey()".
*
* This is not only more efficient, but also necessary to make sure
* that various "inkey()" codes are not "lost" along the way.
*/
void flush(void)
{
	/* Do it later */
	flush_later = TRUE;
}


/*
* Flush the screen, make a noise
*/
void bell(void)
{
	/* Mega-Hack -- Flush the output */
	Term_fresh();

	/* Make a bell noise (if allowed) */
	if (ring_bell) Term_xtra(TERM_XTRA_NOISE, 0);

	/* Flush the input (later!) */
	flush();
}


/*
* Hack -- Make a (relevant?) sound
*/
void sound(s32b val)
{
	/* No sound */
	if (!use_sound) return;

	/* Make a sound (if allowed) */
	Term_xtra(TERM_XTRA_SOUND, val);
}

/*
* Helper function called only from "inkey()"
*
* This function does almost all of the "macro" processing.
*
* We use the "Term_key_push()" function to handle "failed" macros, as well
* as "extra" keys read in while choosing the proper macro, and also to hold
* the action for the macro, plus a special "ascii 30" character indicating
* that any macro action in progress is complete.  Embedded macros are thus
* illegal, unless a macro action includes an explicit "ascii 30" character,
* which would probably be a massive hack, and might break things.
*
* Only 500 (0+1+2+...+29+30) milliseconds may elapse between each key in
* the macro trigger sequence.  If a key sequence forms the "prefix" of a
* macro trigger, 500 milliseconds must pass before the key sequence is
* known not to be that macro trigger.  XXX XXX XXX
*/
static s32b inkey_aux(void)
{
	s32b k = 0, n, p = 0, w = 0;

	char tmp;

	s32b ch;

	cptr pat, act;

	char buf[1024];


	/* Wait for a keypress */
	(void)(Term_inkey(&tmp, TRUE, TRUE));

	if (parse_under)
	{
		trigger__buf[trigger__pos++] = tmp;
		trigger__buf[trigger__pos]   = '\0';
	}

	ch = (s32b) tmp;

	if(!parse_macro)
		inkey_dir = FALSE;

	/* End "macro action" */
	if (ch == 30) parse_macro = FALSE;

	/* Inside "macro action" */
	if (ch == 30) return (ch);

	/* Inside "macro action" */
	if (parse_macro) return (ch);

	/* Inside "macro trigger" */
	if (parse_under) return (ch);

	if ((ch < 0) || (ch > 255))
		return (ch);

	/* Save the first key, advance */
	buf[p++] = tmp;
	buf[p] = '\0';


	/* Check for possible macro */
	k = macro_find_check(buf);

	/* No macro pending */
	if (k < 0) return (ch);

	/* Start of (potential) new trigger, reset trigger mirroring buffer. */
	trigger__buf[0] = '\0';
	trigger__pos    = 0;

	/* Wait for a macro, or a timeout */
	while (TRUE)
	{
		/* Check for pending macro */
		k = macro_find_maybe(buf);

		/* No macro pending */
		if (k < 0) break;

		/* Check for (and remove) a pending key */
		if (0 == Term_inkey(&tmp, FALSE, TRUE))
		{
			/* Append the key */
			buf[p++] = tmp;
			buf[p] = '\0';

			trigger__buf[trigger__pos++] = tmp;
			trigger__buf[trigger__pos]   = '\0';

			/* Restart wait */
			w = 0;
		}

		/* No key ready */
		else
		{
			/* Increase "wait" */
			w += 10;

			/* Excessive delay */
			if (w >= 100) break;

			/* Delay */
			Term_xtra(TERM_XTRA_DELAY, w);
		}
	}


	/* Check for available macro */
	k = macro_find_ready(buf);

	/* No macro available */
	if (k < 0)
	{
		/* Not a trigger, reset trigger mirroring buffer. */
		trigger__buf[0] = '\0';
		trigger__pos    = 0;

		/* Push all the keys back on the queue */
		while (p > 0)
		{
			/* Push the key, notice over-flow */
			if (Term_key_push(buf[--p])) return (0);
		}

		/* Wait for (and remove) a pending key */
		(void)Term_inkey(&tmp, TRUE, TRUE);

		/* Return the key */
		return (ch);
	}


	/* Get the pattern */
	pat = macro__pat[k];

	if(keycode_is_directional((char*)pat))
		inkey_dir = TRUE;

	/* Get the length of the pattern */
	n = strlen(pat);

	/* Push the "extra" keys back on the queue */
	while (p > n)
	{
		/* Push the key, notice over-flow */
		if (Term_key_push(buf[--p])) {
			inkey_dir = FALSE;
			return (0);
		}
	}

	/* Begin "macro action" */
	parse_macro = TRUE;

	/* Push the "end of macro action" key */
	if (Term_key_push(30)) return (0);


	/* Access the macro action */
	act = macro__act[k];

	/* Get the length of the action */
	n = strlen(act);

	/* Push the macro "action" onto the key queue */
	while (n > 0)
	{
		/* Push the key, notice over-flow */
		if (Term_key_push(act[--n])) return (0);
	}


	/* Hack -- Force "inkey()" to call us again */
	return (0);
}


/*
* Mega-Hack -- special "inkey_next" pointer.  XXX XXX XXX
*
* This special pointer allows a sequence of keys to be "inserted" into
* the stream of keys returned by "inkey()".  This key sequence will not
* trigger any macros, and cannot be bypassed by the Borg.  It is used
* in Angband to handle "keymaps".
*/
static cptr inkey_next = NULL;


#ifdef ALLOW_BORG

/*
* Mega-Hack -- special "inkey_hack" hook.  XXX XXX XXX
*
* This special function hook allows the "Borg" (see elsewhere) to take
* control of the "inkey()" function, and substitute in fake keypresses.
*/
char (*inkey_hack)(s32b flush_first) = NULL;

#endif /* ALLOW_BORG */



/*
* Get a keypress from the user.
*
* This function recognizes a few "global parameters".  These are variables
* which, if set to TRUE before calling this function, will have an effect
* on this function, and which are always reset to FALSE by this function
* before this function returns.  Thus they function just like normal
* parameters, except that most calls to this function can ignore them.
*
* If "inkey_xtra" is TRUE, then all pending keypresses will be flushed,
* and any macro processing in progress will be aborted.  This flag is
* set by the "flush()" function, which does not actually flush anything
* itself, but rather, triggers delayed input flushing via "inkey_xtra".
*
* If "inkey_scan" is TRUE, then we will immediately return "zero" if no
* keypress is available, instead of waiting for a keypress.
*
* If "inkey_base" is TRUE, then all macro processing will be bypassed.
* If "inkey_base" and "inkey_scan" are both TRUE, then this function will
* not return immediately, but will wait for a keypress for as long as the
* normal macro matching code would, allowing the direct entry of macro
* triggers.  The "inkey_base" flag is extremely dangerous!
*
* If "inkey_flag" is TRUE, then we will assume that we are waiting for a
* normal command, and we will only show the cursor if "hilite_player" is
* TRUE (or if the player is in a store), instead of always showing the
* cursor.  The various "main-xxx.c" files should avoid saving the game
* in response to a "menu item" request unless "inkey_flag" is TRUE, to
* prevent savefile corruption.
*
* If we are waiting for a keypress, and no keypress is ready, then we will
* refresh (once) the window which was active when this function was called.
*
* Note that "back-quote" is automatically converted into "escape" for
* convenience on machines with no "escape" key.  This is done after the
* macro matching, so the user can still make a macro for "backquote".
*
* Note the special handling of "ascii 30" (ctrl-caret, aka ctrl-shift-six)
* and "ascii 31" (ctrl-underscore, aka ctrl-shift-minus), which are used to
* provide support for simple keyboard "macros".  These keys are so strange
* that their loss as normal keys will probably be noticed by nobody.  The
* "ascii 30" key is used to indicate the "end" of a macro action, which
* allows recursive macros to be avoided.  The "ascii 31" key is used by
* some of the "main-xxx.c" files to introduce macro trigger sequences.
*
* Hack -- we use "ascii 29" (ctrl-right-bracket) as a special "magic" key,
* which can be used to give a variety of "sub-commands" which can be used
* any time.  These sub-commands could include commands to take a picture of
* the current screen, to start/stop recording a macro action, etc.
*
* If "angband_term[0]" is not active, we will make it active during this
* function, so that the various "main-xxx.c" files can assume that input
* is only requested (via "Term_inkey()") when "angband_term[0]" is active.
*
* Mega-Hack -- This function is used as the entry point for clearing the
* "signal_count" variable, and of the "character_saved" variable.
*
* Hack -- Note the use of "inkey_next" to allow "keymaps" to be processed.
*
* Mega-Hack -- Note the use of "inkey_hack" to allow the "Borg" to steal
* control of the keyboard from the user.
*/
s32b inkey(void)
{
	s32b v;

	s32b kk;

	s32b ch = 0;

	bool done = FALSE;

	term *old = Term;

	/* Hack -- Use the "inkey_next" pointer */
	if (inkey_next && *inkey_next && !inkey_xtra)
	{
		/* Get next character, and advance */
		ch = (s32b) *inkey_next++;

		/* Cancel the various "global parameters" */
		inkey_base = inkey_xtra = inkey_flag = inkey_scan = FALSE;

		/* Accept result */
		macro_recorder_add(ch);
		return (ch);
	}

	/* Forget pointer */
	inkey_next = NULL;


#ifdef ALLOW_BORG

	/* Mega-Hack -- Use the special hook */
	if (inkey_hack && ((ch = (*inkey_hack)(inkey_xtra)) != 0))
	{
		/* Cancel the various "global parameters" */
		inkey_base = inkey_xtra = inkey_flag = inkey_scan = FALSE;

		/* Accept result */
		macro_recorder_add(ch);
		return (ch);
	}

#endif /* ALLOW_BORG */


	/* Hack -- handle delayed "flush()" */
	if (inkey_xtra)
	{
		/* End "macro action" */
		parse_macro = FALSE;

		/* End "macro trigger" */
		parse_under = FALSE;

		/* Reset trigger mirroring buffer. */
		trigger__buf[0] = '\0';
		trigger__pos    = 0;

		/* Forget old keypresses */
		Term_flush();
	}


	/* Access cursor state */
	(void)Term_get_cursor(&v);

	/* Show the cursor if waiting, except sometimes in "command" mode */
	if (!inkey_scan && (!inkey_flag || hilite_player || character_icky))
	{
		/* Show the cursor */
		(void)Term_set_cursor(1);
	}


	/* Hack -- Activate main screen */
	Term_activate(angband_term[0]);


	/* Get a key */
	while (!ch)
	{
		char tmp;
		/* Hack -- Handle "inkey_scan" */
		if (!inkey_base && inkey_scan &&
		                (0 != Term_inkey(&tmp, FALSE, FALSE)))
		{
			break;
		}


		/* Hack -- Flush output once when no key ready */
		if (!done && (0 != Term_inkey(&tmp, FALSE, FALSE)))
		{
			/* Hack -- activate proper term */
			Term_activate(old);

			/* Flush output */
			Term_fresh();

			/* Hack -- activate main screen */
			Term_activate(angband_term[0]);

			/* Mega-Hack -- reset saved flag */
			character_saved = FALSE;

			/* Mega-Hack -- reset signal counter */
			signal_count = 0;

			/* Only once */
			done = TRUE;
		}
		kk = (s32b) tmp;

		/* Hack -- Handle "inkey_base" */
		if (inkey_base)
		{
			s32b w = 0;

			/* Wait forever */
			if (!inkey_scan)
			{
				/* Wait for (and remove) a pending key */
				if (0 == Term_inkey(&tmp, TRUE, TRUE))
				{
					ch = (s32b) tmp;
					/* Done */
					break;
				}

				/* Oops */
				break;
			}

			/* Wait */
			while (TRUE)
			{
				/* Check for (and remove) a pending key */
				if (0 == Term_inkey(&tmp, FALSE, TRUE))
				{
					ch = (s32b) tmp;
					/* Done */
					break;
				}

				/* No key ready */
				else
				{
					/* Increase "wait" */
					w += 10;

					/* Excessive delay */
					if (w >= 100) break;

					/* Delay */
					Term_xtra(TERM_XTRA_DELAY, w);
				}
			}

			/* Done */
			break;
		}


		/* Get a key (see above) */
		ch = inkey_aux();


		/* Handle "control-right-bracket" */
		if ((ch == 29) || ((!rogue_like_commands) && (ch == KTRL('D'))))
		{
			/* Strip this key */
			ch = 0;

			if (!do_movies)
				/* Do an html dump */
				do_cmd_html_dump();
			else
				/* Do a text box in the cmovie */
				do_cmovie_insert();


			/* Continue */
			continue;
		}


		/* Treat back-quote as escape */
		if (ch == '`') ch = ESCAPE;


		/* End "macro trigger" */
		if (parse_under && (ch <= 32))
		{
			/* Strip this key */
			ch = 0;

			/* End "macro trigger" */
			parse_under = FALSE;

			/* Does the macro trigger have no matching macro? */
			if (macro_find_ready(trigger__buf) < 0)
			{
				s32b num;
				if (trigger_ascii_to_num(&num, trigger__buf))
					ch = -num;
			}

			/* Reset trigger mirroring buffer. */
			trigger__buf[0] = '\0';
			trigger__pos    = 0;
		}


		/* Handle "control-caret" */
		if (ch == 30)
		{
			/* Strip this key */
			ch = 0;
		}

		/* Handle "control-underscore" */
		else if (ch == 31)
		{
			/* Strip this key */
			ch = 0;

			/* Begin "macro trigger" */
			parse_under = TRUE;

			/* Reset trigger mirroring buffer. */
			trigger__buf[0] = '\0';
			trigger__pos    = 0;
		}

		/* Inside "macro trigger" */
		else if (parse_under)
		{
			/* Strip this key */
			ch = 0;
		}
	}


	/* Hack -- restore the term */
	Term_activate(old);


	/* Restore the cursor */
	Term_set_cursor(v);


	/* Cancel the various "global parameters" */
	inkey_base = inkey_xtra = inkey_flag = inkey_scan = FALSE;


	/* Return the keypress */
	macro_recorder_add(ch);
	return (ch);
}




/*
* We use a global array for all inscriptions to reduce the memory
* spent maintaining inscriptions.  Of course, it is still possible
* to run out of inscription memory, especially if too many different
* inscriptions are used, but hopefully this will be rare.
*
* We use dynamic string allocation because otherwise it is necessary
* to pre-guess the amount of quark activity.  We limit the total
* number of quarks, but this is much easier to "expand" as needed.
*
* Any two items with the same inscription will have the same "quark"
* index, which should greatly reduce the need for inscription space.
*
* Note that "quark zero" is NULL and should not be "dereferenced".
*/

/*
* Add a new "quark" to the set of quarks.
*/
s16b quark_add(cptr str)
{
	s32b i;

	/* Look for an existing quark */
	for (i = 1; i < quark__num; i++)
	{
		/* Check for equality */
		if (streq(quark__str[i], str)) return (i);
	}

	/* Paranoia -- Require room */
	if (quark__num == QUARK_MAX) return (0);

	/* New maximal quark */
	quark__num = i + 1;

	/* Add a new quark */
	quark__str[i] = string_make(str);

	/* Return the index */
	return (i);
}


/*
* This function looks up a quark
*/
cptr quark_str(s16b i)
{
	cptr q;

	/* Verify */
	if ((i < 0) || (i >= quark__num)) i = 0;

	/* Access the quark */
	q = quark__str[i];

	/* Return the quark */
	return (q);
}




/*
* Second try for the "message" handling routines.
*
* Each call to "message_add(s)" will add a new "most recent" message
* to the "message recall list", using the contents of the string "s".
*
* The messages will be stored in such a way as to maximize "efficiency",
* that is, we attempt to maximize the number of sequential messages that
* can be retrieved, given a limited amount of storage space.
*
* We keep a buffer of chars to hold the "text" of the messages, not
* necessarily in "order", and an array of offsets into that buffer,
* representing the actual messages.  This is made more complicated
* by the fact that both the array of indexes, and the buffer itself,
* are both treated as "circular arrays" for efficiency purposes, but
* the strings may not be "broken" across the ends of the array.
*
* The "message_add()" function is rather "complex", because it must be
* extremely efficient, both in space and time, for use with the Borg.
*/



/*
* How many messages are "available"?
*/
s16b message_num(void)
{
	s32b last, next, n;

	/* Extract the indexes */
	last = message__last;
	next = message__next;

	/* Handle "wrap" */
	if (next < last) next += MESSAGE_MAX;

	/* Extract the space */
	n = (next - last);

	/* Return the result */
	return (n);
}



/*
* Recall the "text" of a saved message
*/
cptr message_str(s32b age)
{
	static char buf[1024];
	s16b x;
	s16b o;
	cptr s;

	/* Forgotten messages have no text */
	if ((age < 0) || (age >= message_num())) return ("");

	/* Acquire the "logical" index */
	x = (message__next + MESSAGE_MAX - (age + 1)) % MESSAGE_MAX;

	/* Get the "offset" for the message */
	o = message__ptr[x];

	/* Access the message text */
	s = &message__buf[o];

	/* Hack -- Handle repeated messages */
	if (message__count[x] > 1)
	{
		strnfmt(buf, 1024, "%s <%dx>", s, message__count[x]);
		s = buf;
	}

	/* Return the message text */
	return (s);
}

/*
* Recall the color of a saved message
*/
byte message_color(s32b age)
{
	s16b x;
	byte color = TERM_WHITE;

	/* Forgotten messages have no text */
	if ((age < 0) || (age >= message_num())) return (TERM_WHITE);

	/* Acquire the "logical" index */
	x = (message__next + MESSAGE_MAX - (age + 1)) % MESSAGE_MAX;

	/* Get the "offset" for the message */
	color = message__color[x];

	/* Return the message text */
	return (color);
}

/*
 * Recall the type of a saved message
 */
byte message_type(s32b age)
{
	s16b x;
	byte type;

	/* Forgotten messages have no text */
	if ((age < 0) || (age >= message_num())) return (MESSAGE_NONE);

	/* Acquire the "logical" index */
	x = (message__next + MESSAGE_MAX - (age + 1)) % MESSAGE_MAX;

	/* Get the "offset" for the message */
	type = message__type[x];

	/* Return the message text */
	return (type);
}



/*
* Add a new message, with great efficiency
*/
void message_add(byte type, cptr str, byte color)
{
	s32b i, k, x, n;
	cptr s;


	/*** Step 1 -- Analyze the message ***/

	/* Hack -- Ignore "non-messages" */
	if (!str) return;

	/* Message length */
	n = strlen(str);

	/* Important Hack -- Ignore "long" messages */
	if (n >= MESSAGE_BUF / 4) return;


	/*** Step 2 -- Handle repeated messages ***/

	/* Acquire the "logical" last index */
	x = (message__next + MESSAGE_MAX - 1) % MESSAGE_MAX;

	/* Get the last message text */
	s = &message__buf[message__ptr[x]];

	/* Last message repeated? */
	if (streq(str, s))
	{
		/* Increase the message count */
		message__count[x]++;

		/* Success */
		return;
	}


	/*** Step 3 -- Attempt to optimize ***/

	/* Limit number of messages to check */
	k = message_num() / 4;

	/* Limit number of messages to check */
	if (k > MESSAGE_MAX / 32) k = MESSAGE_MAX / 32;

	/* Check the last few messages (if any to count) */
	for (i = message__next; k; k--)
	{
		u16b q;

		cptr old;

		/* Back up and wrap if needed */
		if (i-- == 0) i = MESSAGE_MAX - 1;

		/* Stop before oldest message */
		if (i == message__last) break;

		/* Extract "distance" from "head" */
		q = (message__head + MESSAGE_BUF - message__ptr[i]) % MESSAGE_BUF;

		/* Do not optimize over large distance */
		if (q > MESSAGE_BUF / 2) continue;

		/* Access the old string */
		old = &message__buf[message__ptr[i]];

		/* Compare */
		if (!streq(old, str)) continue;

		/* Get the next message index, advance */
		x = message__next++;

		/* Handle wrap */
		if (message__next == MESSAGE_MAX) message__next = 0;

		/* Kill last message if needed */
		if (message__next == message__last) message__last++;

		/* Handle wrap */
		if (message__last == MESSAGE_MAX) message__last = 0;

		/* Assign the starting address */
		message__ptr[x] = message__ptr[i];
		message__color[x] = color;
		message__type[x] = type;
		message__count[x] = 1;

		/* Success */
		return;
	}


	/*** Step 4 -- Ensure space before end of buffer ***/

	/* Kill messages and Wrap if needed */
	if (message__head + n + 1 >= MESSAGE_BUF)
	{
		/* Kill all "dead" messages */
		for (i = message__last; TRUE; i++)
		{
			/* Wrap if needed */
			if (i == MESSAGE_MAX) i = 0;

			/* Stop before the new message */
			if (i == message__next) break;

			/* Kill "dead" messages */
			if (message__ptr[i] >= message__head)
			{
				/* Track oldest message */
				message__last = i + 1;
			}
		}

		/* Wrap "tail" if needed */
		if (message__tail >= message__head) message__tail = 0;

		/* Start over */
		message__head = 0;
	}


	/*** Step 5 -- Ensure space before next message ***/

	/* Kill messages if needed */
	if (message__head + n + 1 > message__tail)
	{
		/* Grab new "tail" */
		message__tail = message__head + n + 1;

		/* Advance tail while possible past first "nul" */
		while (message__buf[message__tail - 1]) message__tail++;

		/* Kill all "dead" messages */
		for (i = message__last; TRUE; i++)
		{
			/* Wrap if needed */
			if (i == MESSAGE_MAX) i = 0;

			/* Stop before the new message */
			if (i == message__next) break;

			/* Kill "dead" messages */
			if ((message__ptr[i] >= message__head) &&
			                (message__ptr[i] < message__tail))
			{
				/* Track oldest message */
				message__last = i + 1;
			}
		}
	}


	/*** Step 6 -- Grab a new message index ***/

	/* Get the next message index, advance */
	x = message__next++;

	/* Handle wrap */
	if (message__next == MESSAGE_MAX) message__next = 0;

	/* Kill last message if needed */
	if (message__next == message__last) message__last++;

	/* Handle wrap */
	if (message__last == MESSAGE_MAX) message__last = 0;



	/*** Step 7 -- Insert the message text ***/

	/* Assign the starting address */
	message__ptr[x] = message__head;
	message__color[x] = color;
	message__type[x] = type;
	message__count[x] = 1;

	/* Append the new part of the message */
	for (i = 0; i < n; i++)
	{
		/* Copy the message */
		message__buf[message__head + i] = str[i];
	}

	/* Terminate */
	message__buf[message__head + i] = '\0';

	/* Advance the "head" pointer */
	message__head += n + 1;
}



/*
* Hack -- flush
*/
static void msg_flush(s32b x)
{
	byte a = TERM_L_BLUE;

	/* Hack -- fake monochrome */
	if (!use_color) a = TERM_WHITE;

	/* Pause for response */
	Term_putstr(x + message_col, message_row, -1, a, "-more-");

	/* Get an acceptable keypress */
	while (1)
	{
		s32b cmd = inkey();
		if (quick_messages) break;
		if ((cmd == ESCAPE) || (cmd == ' ')) break;
		if ((cmd == '\n') || (cmd == '\r')) break;
		bell();
	}

	/* Clear the line */
	Term_erase(message_col, message_row, message_col_max);
}

/* Display a message */
static s16b message_row_max = 19;

s16b message_row = 0;
s16b message_col = 0;
s16b message_col_max = 79;
void display_message(s32b x, s32b y, s32b split, byte color, cptr t)
{
	s32b i = 0, j = 0;

	while (i < split)
	{
		if (t[i] == '#')
		{
			if (t[i + 1] == '#')
			{
/* Should not be neccesary, is it ? 				if (x + j < message_col_max) */
					Term_putstr(x + j + message_col, y + message_row, 1, color, "#");
				i += 2;
				j++;
			}
			else if (t[i + 1] == '0' && t[i + 2] == 'x')
			{
				char hex[3];
				int val;

				i += 3;

				hex[0] = t[i++];
				hex[1] = t[i++];
				hex[2] = '\0';

				if (sscanf(hex, "%x", &val) == 1 )
					color = (s32b)val;
				else
				{
					Term_putstr(x + j + message_col, y + message_row, 1,
								color, "#");
					i -= 4;
					j++;
				}
			}
			else
			{
				color = color_char_to_attr(t[i + 1]);
				i += 2;
			}
		}
		else
		{
/* Should not be neccesary, is it ?			if (x + j < message_col_max) */
				Term_putstr(x + j + message_col, y + message_row, 1, color, t + i);
			i++;
			j++;
		}
	}
}

static char last_msg_str[1024];
static  byte last_msg_color;

/*
* Output a message to the top line of the screen.
*
* Break long messages into multiple pieces (40-72 chars).
*
* Allow multiple short messages to "share" the top line.
*
* Prompt the user to make sure he has a chance to read them.
*
* These messages are memorized for later reference (see above).
*
* We could do "Term_fresh()" to provide "flicker" if needed.
*
* The global "msg_flag" variable can be cleared to tell us to
* "erase" any "pending" messages still on the screen.
*
* XXX XXX XXX Note that we must be very careful about using the
* "msg_print()" functions without explicitly calling the special
* "msg_print(NULL)" function, since this may result in the loss
* of information if the screen is cleared, or if anything is
* displayed on the top line.
*
* XXX XXX XXX Note that "msg_print(NULL)" will clear the top line
* even if no messages are pending.  This is probably a hack.
*/
void cmsg_print(byte color, cptr msg)
{
	static s32b p = 0;

	s32b       n, message_row_old;
	char     *t;
	char      buf[1024];
	s32b       lim = message_col + message_col_max - 8;
	float     lines;
	bool      whole_screen;
	term_win *save;

	/* Hack -- Reset */
	if (!msg_flag) p = 0;

	/* Message Length */
	n = (msg ? strlen(msg) : 0);

	/* Hack -- flush when requested or needed */
	if (p && (!msg || (n == 0) || ((p + n) > lim)))
	{
		/* Flush */
		msg_flush(p - message_col);

		/* Forget it */
		msg_flag = FALSE;

		/* Reset */
		p = 0;
	}


	/* No message or empty message. */
	if (!msg || n == 0) return;

	/* Paranoia */
	if (n > 1000) return;



	/* Handle "auto_more" */
	if (auto_more)
	{
		/* Memorize the message */
		if (character_generated)
			message_add(MESSAGE_MSG, msg, color);

		/*
		 * Hack: Remember the last message whose printing was
		 * attempted, so we can *actually* print it later.  If we
		 * don't do this, the user doesn't get any messages at all
		 * when "auto_more" is set.
		 */
		strcpy(last_msg_str, msg);
		last_msg_color = color;

		/* Window stuff */
		flag_bool(&p_ptr->window, FLAG_PW_MESSAGE);

		/* Force window update */
		window_stuff();

		/* Done */
		return;
	}

	lines = ((float)strlen(msg) / (float)lim);

	if ((lines > 2.0) && (((s32b)lines) < message_row_max))
	{
		save         = Term_save_to();
		Term_clear();
		whole_screen = TRUE;
	}
	else
	{
		save         = NULL;
		whole_screen = FALSE;
	}

	/* Copy it */
	strcpy(buf, msg);

	/* Analyze the buffer */
	t = buf;

	message_row_old = message_row;
	/* Split message */
	while (n > lim)
	{
		char oops;

		s32b check, split;

		/* Default split */
		split = lim;

		/* Find the "best" split point */
		for (check = 40; check < lim; check++)
		{
			/* Found a valid split point */
			if (t[check] == ' ') split = check;
		}

		/* Save the split character */
		oops = t[split];

		/* Split the message */
		t[split]      = '\0';

		/* Display part of the message */
		display_message(0, 0, split, color, t);

		/* Memorize the message */
		if (character_generated)
			message_add(MESSAGE_MSG, t, color);

		if (whole_screen)
			message_row++;
		else
			/* Flush it */
			msg_flush(split + 1 - message_col);

		/* Memorize the piece */
		/* if (character_generated) message_add(t); */

		/* Restore the split character */
		t[split] = oops;

		split++;

		/* Prepare to recurse on the rest of "buf" */
		t += split;
		n -= split;
	}

	/* Memorize the message */
	if (character_generated)
		message_add(MESSAGE_MSG, t, color);

	if (whole_screen)
	{
		/* Display the tail of the message */
		display_message(p, 0, n, color, t);
		display_message(0, 1, 40, TERM_L_BLUE,
						"[Press space, enter, or ESC to continue]");

		/* Get an acceptable keypress */
		while (TRUE)
		{
			s32b cmd = inkey();
			if ((cmd == ESCAPE) || (cmd == ' ')) break;
			if ((cmd == '\n') || (cmd == '\r')) break;
			bell();
		}
		message_row = message_row_old;
		Term_load_from(save, TRUE);
		p = -1;
		n = 0;
	}
	else
		/* Display the tail of the message */
		display_message(p, 0, n, color, t);

	/* Memorize the tail */
	/* if (character_generated) message_add(t); */

	/* Window stuff */
	flag_bool(&p_ptr->window, FLAG_PW_MESSAGE);

	/* Remember the message */
	msg_flag = TRUE;

	/* Remember the position */
	p += n + 1;

	/* Optional refresh */
	if (fresh_message) Term_fresh();
}

/*
 * Print very last message for which printing was attempted.  If
 * this isn't used, then "auto_more" causes *no* messages to be
 * displayed at all.  This is a gret big Hack.
 */
void cmsg_print_last()
{
	bool old_val = character_generated;

	/*
	 * Hack: Don't add this message to the message memory, since it
	 * will already be there.
	 */
	character_generated = FALSE;

	cmsg_print(last_msg_color, last_msg_str);
	last_msg_str[0] = '\0';

	character_generated = old_val;
}

/* Hack -- for compatibility and easy sake */
void msg_print(cptr msg)
{
	cmsg_print(TERM_WHITE, msg);
}


/*
 * Hack -- prevent "accidents" in "screen_save()" or "screen_load()"
 */
static s32b __screen_depth = 0;


/*
 * Save the screen, and increase the "icky" depth.
 *
 * This function must match exactly one call to "screen_load()".
 */
void screen_save(void)
{
	/* Hack -- Flush messages */
	msg_print(NULL);

	/* Save the screen (if legal) */
	if (__screen_depth++ == 0) Term_save();

	/* Increase "icky" depth */
	character_icky++;
}

s32b screen_depth(void)
{
	return __screen_depth;
}

/*
 * Load the screen, and decrease the "icky" depth.
 *
 * This function must match exactly one call to "screen_save()".
 */
void screen_load(void)
{
	/* Hack -- Flush messages */
	msg_print(NULL);

	/* Load the screen (if legal) */
	if (--__screen_depth == 0) Term_load();

	/* Decrease "icky" depth */
	character_icky--;
}


/*
* Display a formatted message, using "vstrnfmt()" and "msg_print()".
*/
void msg_format(cptr fmt, ...)
{
	va_list vp;

	char buf[1024];

	/* Begin the Varargs Stuff */
	va_start(vp, fmt);

	/* Format the args, save the length */
	(void)vstrnfmt(buf, 1024, fmt, vp);

	/* End the Varargs Stuff */
	va_end(vp);

	/* Display */
	cmsg_print(TERM_WHITE, buf);
}

void cmsg_format(byte color, cptr fmt, ...)
{
	va_list vp;

	char buf[1024];

	/* Begin the Varargs Stuff */
	va_start(vp, fmt);

	/* Format the args, save the length */
	(void)vstrnfmt(buf, 1024, fmt, vp);

	/* End the Varargs Stuff */
	va_end(vp);

	/* Display */
	cmsg_print(color, buf);
}



/*
* Display a string on the screen using an attribute.
*
* At the given location, using the given attribute, if allowed,
* add the given string.  Do not clear the line.
*/
void c_put_str(byte attr, cptr str, s32b row, s32b col)
{
	/* Hack -- fake monochrome */
	if (!use_color) attr = TERM_WHITE;

	/* Position cursor, Dump the attr/text */
	Term_putstr(col, row, -1, attr, str);
}

/*
* As above, but in "white"
*/
void put_str(cptr str, s32b row, s32b col)
{
	/* Spawn */
	Term_putstr(col, row, -1, TERM_WHITE, str);
}



/*
* Display a string on the screen using an attribute, and clear
* to the end of the line.
*/
void c_prt(byte attr, cptr str, s32b row, s32b col)
{
	/* Hack -- fake monochrome */
	if (!use_color) attr = TERM_WHITE;

	/* Clear line, position cursor */
	Term_erase(col, row, 255);

	/* Dump the attr/text */
	Term_addstr( -1, attr, str);
}

/*
* As above, but in "white"
*/
void prt(cptr str, s32b row, s32b col)
{
	/* Spawn */
	c_prt(TERM_WHITE, str, row, col);
}



/*
 * Print some (colored) text to the screen at the current cursor position,
 * automatically "wrapping" existing text (at spaces) when necessary to
 * avoid placing any text into the last column, and clearing every line
 * before placing any text in that line.  Also, allow "newline" to force
 * a "wrap" to the next line.  Advance the cursor as needed so sequential
 * calls to this function will work correctly.
 *
 * Once this function has been called, the cursor should not be moved
 * until all the related "text_out()" calls to the window are complete.
 *
 * This function will correctly handle any width up to the maximum legal
 * value of 256, though it works best for a standard 80 character width.
 */
void text_out_to_screen(byte a, cptr str)
{
	s32b x, y;

	s32b wid, h;

	s32b wrap;

	cptr s;


	/* Obtain the size */
	(void)Term_get_size(&wid, &h);

	/* Obtain the cursor */
	(void)Term_locate(&x, &y);

	/* Use special wrapping boundary? */
	if ((text_out_wrap > 0) && (text_out_wrap < wid))
		wrap = text_out_wrap;
	else
		wrap = wid;

	/* Process the string */
	for (s = str; *s; s++)
	{
		char ch;

		/* Force wrap */
		if (*s == '\n')
		{
			/* Wrap */
			x = text_out_indent;
			y++;

			/* Clear line, move cursor */
			Term_erase(x, y, 255);

			continue;
		}

		/* Color code */
		if ((*s == '#') && (*(s + 1) != '\0'))
		{
			s++;
			if (*s != '#')
				a = color_char_to_attr(*s);
			continue;
		}

		/* Clean up the char */
		ch = (isprint((unsigned char) * s) ? *s : ' ');

		/* Wrap words as needed */
		if ((x >= wrap - 1) && (ch != ' '))
		{
			s32b i, n = 0;

			byte av[256];
			char cv[256];

			/* Wrap word */
			if (x < wrap)
			{
				/* Scan existing text */
				for (i = wrap - 2; i >= 0; i--)
				{
					/* Grab existing attr/char */
					Term_what(i, y, &av[i], &cv[i]);

					/* Break on space */
					if (cv[i] == ' ') break;

					/* Track current word */
					n = i;
				}
			}

			/* Special case */
			if (n == 0) n = wrap;

			/* Clear line */
			Term_erase(n, y, 255);

			/* Wrap */
			x = text_out_indent;
			y++;

			/* Clear line, move cursor */
			Term_erase(x, y, 255);

			/* Wrap the word (if any) */
			for (i = n; i < wrap - 1; i++)
			{
				/* Dump */
				Term_addch(av[i], cv[i]);

				/* Advance (no wrap) */
				if (++x > wrap) x = wrap;
			}
		}

		/* Dump */
		Term_addch(a, ch);

		/* Advance */
		if (++x > wrap) x = wrap;
	}
}


/*
 * Write text to the given file and apply line-wrapping.
 *
 * Hook function for text_out(). Make sure that text_out_file points
 * to an open text-file.
 *
 * Long lines will be wrapped at text_out_wrap, or at column 75 if that
 * is not set; or at a newline character.
 *
 * You must be careful to end all file output with a newline character
 * to "flush" the stored line position.
 */
void text_out_to_file(byte a, cptr str)
{
	/* Current position on the line */
	static s32b pos = 0;

	/* Wrap width */
	s32b wrap = (text_out_wrap ? text_out_wrap : 75);

	/* Current location within "str" */
	cptr s = str;

	/* Unused parameter */
	(void)a;

	/* Process the string */
	while (*s)
	{
		char ch;
		s32b n = 0;
		s32b len = wrap - pos;
		s32b l_space = 0;

		/* If we are at the start of the line... */
		if (pos == 0)
		{
			s32b i;

			/* Output the indent */
			for (i = 0; i < text_out_indent; i++)
			{
				fputc(' ', text_out_file);
				pos++;
			}
		}

		/* Find length of line up to next newline or end-of-string */
		while ((n < len) && !((s[n] == '\n') || (s[n] == '\0')))
		{
			/* Mark the most recent space in the string */
			if (s[n] == ' ') l_space = n;

			/* Increment */
			n++;
		}

		/* If we have encountered no spaces */
		if ((l_space == 0) && (n == len))
		{
			/* If we are at the start of a new line */
			if (pos == text_out_indent)
			{
				len = n;
			}
			else
			{
				/* Begin a new line */
				fputc('\n', text_out_file);

				/* Reset */
				pos = 0;

				continue;
			}
		}
		else
		{
			/* Wrap at the newline */
			if ((s[n] == '\n') || (s[n] == '\0')) len = n;

			/* Wrap at the last space */
			else len = l_space;
		}

		/* Write that line to file */
		for (n = 0; n < len; n++)
		{
			/* Ensure the character is printable */
			ch = (isprint(s[n]) ? s[n] : ' ');

			/* Write out the character */
			fputc(ch, text_out_file);

			/* Increment */
			pos++;
		}

		/* Move 's' past the stuff we've written */
		s += len;

		/* If we are at the end of the string, end */
		if (*s == '\0') return;

		/* Skip newlines */
		if (*s == '\n') s++;

		/* Begin a new line */
		fputc('\n', text_out_file);

		/* Reset */
		pos = 0;

		/* Skip whitespace */
		while (*s == ' ') s++;
	}

	/* We are done */
	return;
}


/*
 * Output text to the screen or to a file depending on the selected
 * text_out hook.
 */
void text_out(cptr str)
{
	text_out_c(TERM_WHITE, str);
}


/*
 * Output text to the screen (in color) or to a file depending on the
 * selected hook.
 */
void text_out_c(byte a, cptr str)
{
	text_out_hook(a, str);
}




/*
* Clear part of the screen
*/
void clear_from(s32b row)
{
	s32b y;

	/* Erase requested rows */
	for (y = row; y < Term->hgt; y++)
	{
		/* Erase part of the screen */
		Term_erase(0, y, 255);
	}
}

/*
 * Try to find a matching command completion.
 * Note that this is not so friendly since it doesn't give
 * a list of possible completions.
 *
 * First arg is the string to be completed, second is it's length,
 * third is it's maximum length.
 */
static s32b complete_where = 0;
static char complete_buf[100];
static s32b complete_command(char *buf, s32b clen, s32b mlen)
{
	s32b i, j = 1, max = clen;
	bool gotone = FALSE;

	/* Forget the characters after the end of the string. */
	complete_buf[clen] = '\0';

	for (i = 0; i < cli_total; i++)
	{
		cli_comm *cli_ptr = cli_info + i;

		if (!strncmp(cli_ptr->comm, complete_buf, clen))
		{
			Term_erase(0, j, 80);
			Term_putstr(0, j++, -1, TERM_WHITE, cli_ptr->comm);

			/* For the first match, copy the whole string to buf. */
			if (!gotone)
			{
				sprintf(buf, "%.*s", (int) mlen, cli_ptr->comm);
				gotone = TRUE;
			}
			/* For later matches, simply notice how much of buf it
			 * matches. */
			else
			{
				for (max = clen; max < mlen; max++)
				{
					if (cli_ptr->comm[max] == '\0') break;
					if (cli_ptr->comm[max] != buf[max]) break;
				}
				if (max < mlen) buf[max] = '\0';
			}
		}
	}

	return strlen(buf) + 1;
}


/*
* Get some input at the cursor location.
* Assume the buffer is initialized to a default string.
* Note that this string is often "empty" (see below).
* The default buffer is displayed in yellow until cleared.
* Pressing RETURN right away accepts the default entry.
* Normal chars clear the default and append the char.
* Backspace clears the default or deletes the final char.
* ESCAPE clears the buffer and the window and returns FALSE.
* RETURN accepts the current buffer contents and returns TRUE.
*/
bool askfor_aux_complete = FALSE;
bool askfor_aux(char *buf, s32b len)
{
	s32b y, x;

	s32b i = 0;

	s32b k = 0;

	bool done = FALSE;


	/* Locate the cursor */
	Term_locate(&x, &y);


	/* Paranoia -- check len */
	if (len < 1) len = 1;

	/* Paranoia -- check column */
	if ((x < 0) || (x >= 80)) x = 0;

	/* Restrict the length */
	if (x + len > 80) len = 80 - x;


	/* Paranoia -- Clip the default entry */
	buf[len] = '\0';


	/* Display the default answer */
	Term_erase(x, y, len);
	Term_putstr(x, y, -1, TERM_YELLOW, buf);

	if (askfor_aux_complete)
	{
		screen_save();
		complete_where = 0;
		strncpy(complete_buf, buf, 100);
	}

	/* Process input */
	while (!done)
	{
		/* Place cursor */
		Term_gotoxy(x + k, y);

		/* Get a key */
		i = inkey();

		/* Analyze the key */
		switch (i)
		{
		case ESCAPE:
			k = 0;
			done = TRUE;
			break;

		case '\n':
		case '\r':
			k = strlen(buf);
			done = TRUE;
			break;

		case '\t':
			if (askfor_aux_complete && k)
			{
				screen_load();
				screen_save();
				k = complete_command(buf, k, len);
			}
			else
			{
				bell();
			}

		case 0x7F:
		case '\010':
			if (k > 0) k--;
			strncpy(complete_buf, buf, k);
			break;

		default:
			if ((k < len) && (isprint(i)))
			{
				buf[k++] = i;
				strncpy(complete_buf, buf, k);
			}
			else
			{
				bell();
			}
			break;
		}

		/* Terminate */
		buf[k] = '\0';

		/* Update the entry */
		Term_erase(x, y, len);
		Term_putstr(x, y, -1, TERM_WHITE, buf);
	}

	if (askfor_aux_complete)
	{
		screen_load();
	}

	/* Aborted */
	if (i == ESCAPE) return (FALSE);

	/* Success */
	return (TRUE);
}


/*
* Get a string from the user
*
* The "prompt" should take the form "Prompt: "
*
* Note that the initial contents of the string is used as
* the default response, so be sure to "clear" it if needed.
*
* We clear the input, and return FALSE, on "ESCAPE".
*/
bool get_string(cptr prompt, char *buf, s32b len)
{
	bool res;

	/* Paranoia XXX XXX XXX */
	msg_print(NULL);

	/* Display prompt */
	prt(prompt, 0, 0);

	/* Ask the user for a string */
	res = askfor_aux(buf, len);

	/* Clear prompt */
	prt("", 0, 0);

	/* Result */
	return (res);
}


/*
 * Verify something with the user
 *
 * The "prompt" should take the form "Query? "
 *
 * Note that "[y/n]" is appended to the prompt.
 */
bool get_check(cptr prompt)
{
	return get_check_aux(prompt, FALSE);
}

/*
 * Like get_check(), but accepts a second argument keep_cursor,
 * which if true will put the cursor back to it's original
 * position before requesting a keypress.
 */
bool get_check_aux(cptr prompt, bool keep_cursor)
{

	s32b x, y;
	s32b i;
	char buf[80];

	Term_locate(&x, &y);

	/* Paranoia XXX XXX XXX */
	msg_print(NULL);

	/* Hack -- Build a "useful" prompt */
	strnfmt(buf, 78, "%.70s[y/n] ", prompt);

	/* Prompt for it */
	prt(buf, 0, 0);

	if (keep_cursor)
		Term_gotoxy(x, y);

	/* Get an acceptable answer */
	while (TRUE)
	{
		i = inkey();
		if (quick_messages) break;
		if (i == ESCAPE) break;
		if (strchr("YyNn", i)) break;
		bell();
	}

	/* Erase the prompt */
	prt("", 0, 0);

	if (keep_cursor)
		Term_gotoxy(x, y);

	/* Normal negation */
	if ((i != 'Y') && (i != 'y')) return (FALSE);

	/* Success */
	return (TRUE);
}

/*
 * Prompts for a keypress
 *
 * The "prompt" should take the form "Command: "
 *
 * Returns TRUE unless the character is "Escape"
 */
bool get_com(cptr prompt, s32b *command)
{
	return get_com_aux(prompt, command, FALSE);
}

/*
 * Like get_com, but has arguemnt keep_cursor, which if true will
 * return the cursor to its fromer position before asking for
 * a key.
 */
bool get_com_aux(cptr prompt, s32b *command, bool keep_cursor)
{
	s32b x, y;

	Term_locate(&x, &y);

	/* Paranoia XXX XXX XXX */
	msg_print(NULL);

	/* Display a prompt */
	prt(prompt, 0, 0);

	if (keep_cursor)
		Term_gotoxy(x, y);

	/* Get a key */
	*command = inkey();

	/* Clear the prompt */
	prt("", 0, 0);

	if (keep_cursor)
		Term_gotoxy(x, y);

	/* Handle "cancel" */
	if (*command == ESCAPE) return (FALSE);

	/* Success */
	return (TRUE);
}


/*
* Request a "quantity" from the user
*
* Hack -- allow "command_arg" to specify a quantity
*/
s32b get_quantity(cptr prompt, s32b max)
{
	s32b amt;
	s32b aamt;

	char tmp[80];

	char buf[80];


	/* Use "command_arg" */
	if (command_arg)
	{
		/* Extract a number */
		amt = command_arg;

		/* Clear "command_arg" */
		command_arg = 0;

		/* Enforce the maximum */
		if (amt > max) amt = max;

		/* Use it */
		return (amt);
	}

#ifdef ALLOW_REPEAT /* TNB */

	/* Get the item index */
	if ((max != 1) && repeat_pull(&aamt))
	{
		amt = aamt;

		/* Enforce the maximum */
		if (amt > max) amt = max;

		/* Enforce the minimum */
		if (amt < 0) amt = 0;

		/* Use it */
		return (amt);
	}

#endif /* ALLOW_REPEAT -- TNB */

	/* Build a prompt if needed */
	if (!prompt)
	{
		/* Build a prompt */
		sprintf(tmp, "Quantity (1-%ld): ", max);

		/* Use that prompt */
		prompt = tmp;
	}


	/* Default to one */
	amt = 1;

	/* Build the default */
	sprintf(buf, "%ld", amt);

	/* Ask for a quantity */
	if (!get_string(prompt, buf, 9)) return (0);

	/* Extract a number */
	amt = atoi(buf);

	/* A letter means "all" */
	if (isalpha(buf[0])) amt = max;

	/* Enforce the maximum */
	if (amt > max) amt = max;

	/* Enforce the minimum */
	if (amt < 0) amt = 0;

#ifdef ALLOW_REPEAT /* TNB */

	if (amt) repeat_push(amt);

#endif /* ALLOW_REPEAT -- TNB */

	/* Return the result */
	return (amt);
}


/*
* Pause for user response XXX XXX XXX
*/
void pause_line(s32b row)
{
	s32b i;
	cptr next = "#R====>>> [Press any key to continue] <<<====";
	prt("", 23, row);
	display_message(row, 23, strlen(next), TERM_WHITE, next);
	i = inkey();
	prt("", 23, row);
}


/*
* Hack -- special buffer to hold the action of the current keymap
*/
static char request_command_buffer[256];



/*
* Request a command from the user.
*
* Sets p_ptr->command_cmd, p_ptr->command_dir, p_ptr->command_rep,
* p_ptr->command_arg.  May modify p_ptr->command_new.
*
* Note that "caret" ("^") is treated specially, and is used to
* allow manual input of control characters.  This can be used
* on many machines to request repeated tunneling (Ctrl-H) and
* on the Macintosh to request "Control-Caret".
*
* Note that "backslash" is treated specially, and is used to bypass any
* keymap entry for the following character.  This is useful for macros.
*
* Note that this command is used both in the dungeon and in
* stores, and must be careful to work in both situations.
*
* Note that "p_ptr->command_new" may not work any more.  XXX XXX XXX
*/
void request_command()
{
	s32b cmd;
	s32b cmd_char;

	s32b mode;

	cptr act;


	/* Roguelike */
	if (rogue_like_commands)
	{
		mode = KEYMAP_MODE_ROGUE;
	}

	/* Original */
	else
	{
		mode = KEYMAP_MODE_ORIG;
	}


	/* No command yet */
	command_cmd = 0;

	/* No "argument" yet */
	command_arg = 0;

	/* No "direction" yet */
	command_dir = 0;


	/* Get command */
	while (1)
	{
		/* Hack -- auto-commands */
		if (command_new)
		{
			/* Flush messages */
			msg_print(NULL);

			/* Use auto-command */
			cmd = command_new;

			/* Forget it */
			command_new = 0;

			/* Hack - bypass keymaps. Does this break inven/equip? */
			if (!inkey_next && !command_new_do_keymap) inkey_next = "";

			command_new_do_keymap = FALSE;
		}

		/* Get a keypress in "command" mode */
		else
		{
			/* Hack -- no flush needed */
			msg_flag = FALSE;

			/* Activate "command mode" */
			inkey_flag = TRUE;

			/* Get a command */
			if (realtime_freq) inkey_scan = TRUE;
			cmd = inkey();
		}

		/* Clear top line */
		prt("", 0, 0);

		if ((cmd < 0) || (cmd > 255))
		{
			/* Not a normal key, don't touch it. */
			command_cmd = cmd;
			break;
		}

		/* Command Count */
		if (cmd == '0')
		{
			s32b old_arg = command_arg;

			/* Reset */
			command_arg = 0;

			/* Begin the input */
			prt("Count: ", 0, 0);

			/* Get a command count */
			while (1)
			{
				/* Get a new keypress */
				cmd = inkey();

				/* Simple editing (delete or backspace) */
				if ((cmd == 0x7F) || (cmd == KTRL('H')))
				{
					/* Delete a digit */
					command_arg = command_arg / 10;

					/* Show current count */
					prt(format("Count: %d", command_arg), 0, 0);
				}

				/* Actual numeric data */
				else if (cmd >= '0' && cmd <= '9')
				{
					/* Stop count at 9999 */
					if (command_arg >= 1000)
					{
						/* Warn */
						bell();

						/* Limit */
						command_arg = 9999;
					}

					/* Increase count */
					else
					{
						/* Incorporate that digit */
						command_arg = command_arg * 10 + D2I(cmd);
					}

					/* Show current count */
					prt(format("Count: %d", command_arg), 0, 0);
				}

				/* Exit on "unusable" input */
				else
				{
					break;
				}
			}

			/* Hack -- Handle "zero" */
			if (command_arg == 0)
			{
				/* Default to 99 */
				command_arg = 99;

				/* Show current count */
				prt(format("Count: %d", command_arg), 0, 0);
			}

			/* Hack -- Handle "old_arg" */
			if (old_arg != 0)
			{
				/* Restore old_arg */
				command_arg = old_arg;

				/* Show current count */
				prt(format("Count: %d", command_arg), 0, 0);
			}

			/* Hack -- white-space means "enter command now" */
			if ((cmd == ' ') || (cmd == '\n') || (cmd == '\r'))
			{
				/* Get a real command */
				bool temp = get_com("Command: ", &cmd_char);
				cmd = cmd_char;

				if (!temp)
				{
					/* Clear count */
					command_arg = 0;

					/* Continue */
					continue;
				}
			}
		}


		/* Allow "keymaps" to be bypassed */
		if (cmd == '\\')
		{
			/* Get a real command */
			(void)get_com("Command: ", &cmd_char);

			cmd = cmd_char;

			/* Hack -- bypass keymaps */
			if (!inkey_next) inkey_next = "";
		}


		/* Allow "control chars" to be entered */
		if (cmd == '^')
		{
			/* Get a new command and controlify it */
			if (get_com("Control: ", &cmd_char)) cmd = KTRL(cmd_char);
			else cmd = 0;
		}


		/* Look up applicable keymap */
		act = keymap_act[mode][(byte)(cmd)];

		/* Apply keymap if not inside a keymap already */
		if (act && !inkey_next)
		{
			/* Install the keymap (limited buffer size) */
			strnfmt(request_command_buffer, 256, "%s", act);

			/* Start using the buffer */
			inkey_next = request_command_buffer;

			/* Continue */
			continue;
		}


		/* Paranoia */
		if (realtime_freq && !cmd) cmd = 's';
		if (!cmd) continue;


		/* Use command */
		command_cmd = cmd;

		/* Done */
		break;
	}

	/* Hack -- Auto-repeat certain commands */
	if (always_repeat && (command_arg <= 0))
	{
		/* Hack -- auto repeat certain commands */
		if ((command_cmd >= 0) && (command_cmd <= 255))
			if (strchr("TBDoc+", command_cmd))
			{
				/* Repeat 99 times */
				command_arg = 99;
			}
	}

#if 0 /* XXX MATT: Moved this into the code for each command; bad idea? */
	/* Hack -- Scan equipment */
	for_inventory(p_ptr, o_ptr, INVEN_PACK, INVEN_TOTAL);
	{
		cptr s;

		/* No inscription */
		if (!o_ptr->note) continue;

		/* Obtain the inscription */
		s = quark_str(o_ptr->note);

		/* Find a '^' */
		s = strchr(s, '^');

		/* Process preventions */
		while (s)
		{
			/* Check the "restriction" character */
			if ((s[1] == command_cmd) || (s[1] == '*'))
			{
				/* Hack -- Verify command */
				if (!get_check("Are you sure? "))
				{
					/* Hack -- Use space */
					command_cmd = ' ';
				}
			}

			/* Find another '^' */
			s = strchr(s + 1, '^');
		}
	}
	end_inventory();
#endif

	/* Hack -- erase the message line. */
	prt("", 0, 0);
}




/*
 * Check a char for "vowel-hood"
 */
bool is_a_vowel(s32b ch)
{
	switch (ch)
	{
	case 'a':
	case 'e':
	case 'i':
	case 'o':
	case 'u':
	case 'A':
	case 'E':
	case 'I':
	case 'O':
	case 'U':
		return (TRUE);
	}

	return (FALSE);
}



#if 0

/*
* Replace the first instance of "target" in "buf" with "insert"
* If "insert" is NULL, just remove the first instance of "target"
* In either case, return TRUE if "target" is found.
*
* XXX Could be made more efficient, especially in the
* case where "insert" is smaller than "target".
*/
static bool insert_str(char *buf, cptr target, cptr insert)
{
	s32b i, len;
	s32b	b_len, t_len, i_len;

	/* Attempt to find the target (modify "buf") */
	buf = strstr(buf, target);

	/* No target found */
	if (!buf) return (FALSE);

	/* Be sure we have an insertion string */
	if (!insert) insert = "";

	/* Extract some lengths */
	t_len = strlen(target);
	i_len = strlen(insert);
	b_len = strlen(buf);

	/* How much "movement" do we need? */
	len = i_len - t_len;

	/* We need less space (for insert) */
	if (len < 0)
	{
		for (i = t_len; i < b_len; ++i) buf[i + len] = buf[i];
	}

	/* We need more space (for insert) */
	else if (len > 0)
	{
		for (i = b_len - 1; i >= t_len; --i) buf[i + len] = buf[i];
	}

	/* If movement occured, we need a new terminator */
	if (len) buf[b_len + len] = '\0';

	/* Now copy the insertion string */
	for (i = 0; i < i_len; ++i) buf[i] = insert[i];

	/* Successful operation */
	return (TRUE);
}


#endif


/*
 * GH
 * Called from cmd4.c and a few other places. Just extracts
 * a direction from the keymap for ch (the last direction,
 * in fact) byte or char here? I'm thinking that keymaps should
 * generally only apply to single keys, which makes it no more
 * than 128, so a char should suffice... but keymap_act is 256...
 */
s32b get_keymap_dir(char ch)
{
	s32b d = 0;

	s32b mode;

	cptr act;

	cptr s;


	/* Already a direction? */
	if (isdigit(ch))
	{
		d = D2I(ch);
	}
	else
	{
		/* Roguelike */
		if (rogue_like_commands)
		{
			mode = KEYMAP_MODE_ROGUE;
		}

		/* Original */
		else
		{
			mode = KEYMAP_MODE_ORIG;
		}

		/* Extract the action (if any) */
		act = keymap_act[mode][(byte)(ch)];

		/* Analyze */
		if (act)
		{
			/* Convert to a direction */
			for (s = act; *s; ++s)
			{
				/* Use any digits in keymap */
				if (isdigit(*s)) d = D2I(*s);
			}
		}
	}

	/* Paranoia */
	if (d == 5) d = 0;

	/* Return direction */
	return (d);
}

/*
 * Figure out which keypad direction is the best approximation of
 * the direction between two points.
 */
s32b line_to_dir(s32b starty, s32b startx, s32b endy, s32b endx)
{
	float slope;

	if ((starty == endy) && (startx == endx))
		/* No movement */
		return(5);

	if (startx == endx)
		return((endy > starty) ? 2 : 8);

	if (starty == endy)
		return(endx > startx ? 6: 4);

	slope = ((float)(endy - starty)) / ((float) (endx - startx));

	if ((endy > starty) && (endx > startx))
	{
		/* North-east quadrant */
		if (slope > 2.414)
			return 2;
		else if (slope < 0.414)
			return 6;
		else
			return 3;
	}
	else if ((endy < starty) && (endx < startx))
	{
		/* South-west quadrant */
		if (slope > 2.414)
			return 8;
		else if (slope < 0.414)
			return 4;
		else
			return 7;
	}
	else if ((endy > starty) && (endx < startx))
	{
		/* North-west quadrant */
		if (-slope > 2.414)
			return 2;
		else if (-slope < 0.414)
			return 4;
		else
			return 1;
	}
	else
	{
		/* South-east quadrant */
		if (-slope > 2.414)
			return 8;
		else if (-slope < 0.414)
			return 6;
		else
			return 9;
	}
} /* line_to_dir() */

#ifdef ALLOW_REPEAT /* TNB */

#define REPEAT_MAX		20

/* Number of chars saved */
s32b repeat__cnt = 0;

/* Current index */
s32b repeat__idx = 0;

/* Saved "stuff" */
static s32b repeat__key[REPEAT_MAX];

void repeat_pop()
{
	repeat__idx--;
	repeat__cnt--;

	/* Paranoia, shouldnt be needed */
	if (repeat__idx < 0) repeat__idx = 0;
	if (repeat__cnt < 0) repeat__cnt = 0;
}

void repeat_push(s32b what)
{
	/* Too many keys */
	if (repeat__cnt == REPEAT_MAX) return;

	/* Push the "stuff" */
	repeat__key[repeat__cnt++] = what;

	/* Prevents us from pulling keys */
	++repeat__idx;
}


bool repeat_pull(s32b *what)
{
	/* All out of keys */
	if (repeat__idx == repeat__cnt) return (FALSE);

	/* Grab the next key, advance */
	*what = repeat__key[repeat__idx++];

	/* Success */
	return (TRUE);
}

void repeat_check(void)
{
	s32b what;

	/* Ignore some commands */
	if (command_cmd == ESCAPE) return;
	if (command_cmd == ' ') return;
	if (command_cmd == '\r') return;
	if (command_cmd == '\n') return;

	/* Repeat Last Command */
	if (command_cmd == 'n')
	{
		/* Reset */
		repeat__idx = 0;

		/* Get the command */
		if (repeat_pull(&what))
		{
			/* Save the command */
			command_cmd = what;
		}
	}

	/* Start saving new command */
	else
	{
		/* Give a chance to do something with the last command stack */
		if (process_hooks(HOOK_REPEAT_KEY_CHAIN_DESTROY, "()")) return;

		/* Reset */
		repeat__cnt = 0;
		repeat__idx = 0;

		what = command_cmd;

		/* Save this command */
		repeat_push(what);
	}
}

#endif /* ALLOW_REPEAT -- TNB */

/*
 * Read a number at a specific location on the screen
 *
 * Allow numbers of any size and save the last keypress.
 */
u32b get_number(u32b def, u32b max, s32b y, s32b x, char *cmd)
{
	u32b res = def;

	/* Player has not typed anything yet */
	bool no_keys = TRUE;

	/* Begin the input with default */
	prt(format("%lu", def), y, x);

	/* Get a command count */
	while (1)
	{
		/* Get a new keypress */
		s32b key = inkey();

		if ((key < 0) || (key > 255))
			continue;

		*cmd = (char) key;

		/* Simple editing (delete or backspace) */
		if ((*cmd == 0x7F) || (*cmd == KTRL('H')))
		{
			/* Override the default */
			no_keys = FALSE;

			/* Delete a digit */
			res = res / 10;

			prt(format("%lu", res), y, x);
		}

		/* Actual numeric data */
		else if (*cmd >= '0' && *cmd <= '9')
		{
			/* Override the default */
			if (no_keys)
			{
				no_keys = FALSE;
				res = 0;
			}

			/* Don't overflow */
			if (((u32b)(0 - 1) - D2I(*cmd)) / 10 < res)
			{
				/* Warn */
				bell();

				/* Limit */
				res = (max + 1 == 0) ? 0 - 1 : max;
			}

			/* Stop count at maximum */
			else if (res * 10 + D2I(*cmd) > max)
			{
				/* Warn */
				bell();

				/* Limit */
				res = max;
			}

			/* Increase count */
			else
			{
				/* Incorporate that digit */
				res = res * 10 + D2I(*cmd);
			}

			/* Show current count */
			prt(format("%lu", res), y, x);
		}

		/* Escape cancels */
		else if (*cmd == ESCAPE)
		{
			res = 0;
			break;
		}

		/* Exit on "unusable" input */
		else
		{
			break;
		}
	}

	return res;
}

/*
 * Allow the user to select multiple items without pressing '0'
 */
void get_count(s32b number, s32b max)
{
	char cmd;

	/* Use the default */
	command_arg = number;

	/* Hack -- Optional flush */
	if (flush_command) flush();

	/* Clear top line */
	prt("", 0, 0);

	/* Begin the input */
	prt("How many?", 0, 0);

	/* Actually get a number */
	command_arg = get_number(command_arg, max, 0, 10, &cmd);

	prt("", 0, 0);
}

byte count_bits(u32b array)
{
	byte k = 0, i;

	if (array)
		for (i = 0; i < 32; i++)
			if (array & (1 << i)) k++;

	return k;
}

/* Return the lowered string */
void strlower(char *buf)
{
	s32b i = 0;

	while ((buf[i] != 0) && (i < 256))
	{
		if (isupper(buf[i])) buf[i] = tolower(buf[i]);

		i++;
	}
}

/*
 * Given monster name as string, return the index in r_info array. Name
 * must exactly match (look out for commas and the like!), or else 0 is
 * returned. Case doesn't matter. -GSN-
 */

s32b test_monster_name(cptr name)
{
	s32b i;

	/* Scan the monsters */
	for (i = 1; i < max_r_idx; i++)
	{
		monster_race *r_ptr = &r_info[i];
		cptr mon_name = r_ptr->name;

		/* If name matches, give us the number */
		if (stricmp(name, mon_name) == 0) return (i);
	}
	return (0);
}
s32b test_mego_name(cptr name)
{
	s32b i;

	/* Scan the monsters */
	for (i = 1; i < max_re_idx; i++)
	{
		monster_ego *re_ptr = &re_info[i];
		cptr mon_name = re_ptr->name;

		/* If name matches, give us the number */
		if (stricmp(name, mon_name) == 0) return (i);
	}
	return (0);
}

/*
 * Given item name as string, return the index in k_info array. Name
 * must exactly match (look out for commas and the like!), or else 0 is
 * returned. Case doesn't matter. -DG-
 */

s32b test_item_name(cptr name)
{
	s32b i;

	/* Scan the items */
	for (i = 1; i < max_k_idx; i++)
	{
		object_kind *k_ptr = &k_info[i];
		cptr obj_name = k_ptr->name;

		/* If name matches, give us the number */
		if (stricmp(name, obj_name) == 0) return (i);
	}
	return (0);
}

s32b find_player_descriptor(cptr name)
{
	s32b i;

	/* Scan the items */
	for (i = 0; i < max_dp_idx; i++)
	{
		player_descriptor *dp_ptr = &descriptor_info[i];
		cptr dname = dp_ptr->title;

		/* If name matches, give us the number */
		if (stricmp(name, dname) == 0) return (i);
	}
	return (-1);
}

s32b find_power(cptr name)
{
	s32b i;

	/* Find it in the list */
	for (i = 0; i < power_max; i++)
	{
		if (stricmp(name, powers_type[i].name) == 0) return i;
	}
	return -1;
}

/*
 * Break scalar time
 */
s32b bst(s32b what, s32b turns)
{
	if (what == MINUTE) return ((turns / 10 / MINUTE) % 60);
	if (what == HOUR) return (turns / 10 / (HOUR) % 24);
	if (what == DAY) return (turns / 10 / (DAY) % 365);
	if (what == YEAR) return (turns / 10 / (YEAR));
	return (0);
}

cptr get_date_string(s32b turn)
{
	/* If you use a power of two as your static buffer size it looks
	   less like an aribtrarily-chosen number */
	static char buf[512];
	cptr str;

	call_lua("time.get_date_string", "(d)", "s", turn, &str);
	strncpy(buf, str, 511);

	return buf;
}

cptr get_note_date_string(s32b turn)
{
	/* If you use a power of two as your static buffer size it looks
	   less like an arbitrarily-chosen number */
	static char buf[512];
	cptr str;

	call_lua("time.get_note_date_string", "(d)", "s", turn, &str);
	strncpy(buf, str, 511);

	return buf;
}

cptr get_long_note_date_string(s32b turn)
{
	/* If you use a power of two as your static buffer size it looks
	   less like an arbitrarily-chosen number */
	static char buf[512];
	cptr str;

	call_lua("time.get_long_note_date_string", "(d)", "s", turn, &str);
	strncpy(buf, str, 511);

	return buf;
}

bool is_daylight(s32b turn)
{
	s32b i;
	call_lua("time.is_daylight", "(d)", "d", turn, &i);
	return i;
}

bool is_dawn(s32b turn)
{
	s32b i;
	call_lua("time.is_dawn", "(d)", "d", turn, &i);
	return i;
}

bool is_dusk(s32b turn)
{
	s32b i;
	call_lua("time.is_dusk", "(d)", "d", turn, &i);
	return i;
}

#ifdef SUPPORT_GAMMA

/* Table of gamma values */
byte gamma_table[256];

/* Table of ln(x / 256) * 256 for x going from 0 -> 255 */
static const s16b gamma_helper[256] =
{
	0, -1420, -1242, -1138, -1065, -1007, -961, -921, -887, -857, -830,
	-806, -783, -762, -744, -726, -710, -694, -679, -666, -652, -640,
	-628, -617, -606, -596, -586, -576, -567, -577, -549, -541, -532,
	-525, -517, -509, -502, -495, -488, -482, -475, -469, -463, -457,
	-451, -455, -439, -434, -429, -423, -418, -413, -408, -403, -398,
	-394, -389, -385, -380, -376, -371, -367, -363, -359, -355, -351,
	-347, -343, -339, -336, -332, -328, -325, -321, -318, -314, -311,
	-308, -304, -301, -298, -295, -291, -288, -285, -282, -279, -276,
	-273, -271, -268, -265, -262, -259, -257, -254, -251, -248, -246,
	-243, -241, -238, -236, -233, -231, -228, -226, -223, -221, -219,
	-216, -214, -212, -209, -207, -205, -203, -200, -198, -196, -194,
	-192, -190, -188, -186, -184, -182, -180, -178, -176, -174, -172,
	-170, -168, -166, -164, -162, -160, -158, -156, -155, -153, -151,
	-149, -147, -146, -144, -142, -140, -139, -137, -135, -134, -132,
	-130, -128, -127, -125, -124, -122, -120, -119, -117, -116, -114,
	-112, -111, -109, -108, -106, -105, -103, -102, -100, -99, -97, -96,
	-95, -93, -92, -90, -89, -87, -86, -85, -83, -82, -80, -79, -78,
	-76, -75, -74, -72, -71, -70, -68, -67, -66, -65, -63, -62, -61,
	-59, -58, -57, -56, -54, -53, -52, -51, -50, -48, -47, -46, -45,
	-44, -42, -41, -40, -39, -38, -37, -35, -34, -33, -32, -31, -30,
	-29, -27, -26, -25, -24, -23, -22, -21, -20, -19, -18, -17, -16,
	-14, -13, -12, -11, -10, -9, -8, -7, -6, -5, -4, -3, -2, -1
};


/*
 * Build the gamma table so that floating point isn't needed.
 *
 * Note gamma goes from 0->256.  The old value of 100 is now 128.
 */
void build_gamma_table(s32b gamma)
{
	s32b i, n;

	/*
	 * value is the current sum.
	 * diff is the new term to add to the series.
	 */
	long value, diff;

	/* Hack - convergence is bad in these cases. */
	gamma_table[0] = 0;
	gamma_table[255] = 255;

	for (i = 1; i < 255; i++)
	{
		/*
		 * Initialise the Taylor series
		 *
		 * value and diff have been scaled by 256
		 */
		n = 1;
		value = 256 * 256;
		diff = ((long)gamma_helper[i]) * (gamma - 256);

		while (diff)
		{
			value += diff;
			n++;

			/*
			 * Use the following identiy to calculate the gamma table.
			 * exp(x) = 1 + x + x^2/2 + x^3/(2*3) + x^4/(2*3*4) +...
			 *
			 * n is the current term number.
			 *
			 * The gamma_helper array contains a table of
			 * ln(x/256) * 256
			 * This is used because a^b = exp(b*ln(a))
			 *
			 * In this case:
			 * a is i / 256
			 * b is gamma.
			 *
			 * Note that everything is scaled by 256 for accuracy,
			 * plus another factor of 256 for the final result to
			 * be from 0-255.  Thus gamma_helper[] * gamma must be
			 * divided by 256*256 each itteration, to get back to
			 * the original power series.
			 */
			diff = (((diff / 256) * gamma_helper[i]) * (gamma - 256)) / (256 * n);
		}

		/*
		 * Store the value in the table so that the
		 * floating point pow function isn't needed.
		 */
		gamma_table[i] = ((long)(value / 256) * i) / 256;
	}
}

#endif /* SUPPORT_GAMMA */

/*
 * Ask to select an item in a list
 */
s32b ask_menu(cptr ask, char **items, s32b max)
{
	s32b ret = -1, i, start = 0;
	char c;
	s32b key;

	/* Enter "icky" mode */
	character_icky++;

	/* Save the screen */
	Term_save();

	while (TRUE)
	{
		/* Display list */
		Term_load();
		Term_save();
		prt(ask, 0, 0);
		for (i = start; (i < max) && (i < start + 20); i++)
		{
			prt(format("%c) %s", I2A(i - start), items[i]), i - start + 1, 0);
		}

		/* Wait for user input */
		key = inkey();

		if ((key < 0) || (key > 255))
			continue;

		c = (char) key;

		/* Leave the screen */
		if (c == ESCAPE) break;

		/* Scroll */
		else if (c == '+')
		{
			if (start + 20 < max)
				start += 20;
			continue;
		}

		/* Scroll */
		else if (c == '-')
		{
			start -= 20;
			if (start < 0) start = 0;
			continue;
		}

		/* Good selection */
		else
		{
			c = tolower(c);
			if (A2I(c) + start >= max)
			{
				bell();
				continue;
			}
			if (A2I(c) + start < 0)
			{
				bell();
				continue;
			}

			ret = A2I(c) + start;
			break;
		}
	}

	/* Load the screen */
	Term_load();

	/* Leave "icky" mode */
	character_icky--;

	return ret;
}

/*
 * Determine if string "t" is a prefix of string "s"
 */
bool prefix(cptr s, cptr t)
{
	/* Paranoia */
	if (!s || !t)
	{
		if (alert_failure) message_add(MESSAGE_MSG, "prefix() called with null argument!", TERM_RED);
		return FALSE;
	}

	/* Scan "t" */
	while (*t)
	{
		/* Compare content and length */
		if (*t++ != *s++) return (FALSE);
	}

	/* Matched, we have a prefix */
	return (TRUE);
}

/*
 * Rescale a value
 */
s32b value_scale(s32b value, s32b vmax, s32b max, s32b min)
{
	s32b full_max = max - min;

	value = (value * full_max) / vmax;
	value += min;

	return value;
}

/*
 * Darken the screen
 */
void darken_screen(byte color)
{
	s32b wid = 0, hgt = 0, i, j;
	Term_get_size(&wid, &hgt);

	for (j = 0; j < hgt; j++)
	{
		for (i = 0; i < wid; i++)
		{
			byte a;
			char c;
			Term_what(i, j, &a, &c);
			Term_putch(i, j, color, c);
		}
	}
}

/*
 * Displays a box
 */
void draw_box(s32b y, s32b x, s32b h, s32b w)
{
	s32b i, j;

	for (i = x + 1; i < x + w; i++)
		for (j = y + 1; j < y + h; j++)
			Term_putch(i, j, TERM_L_BLUE, ' ');

	for (i = x; i < x + w; i++)
	{
		c_put_str(TERM_L_BLUE, "-", y, i);
		c_put_str(TERM_L_BLUE, "-", y + h, i);
	}
	for (i = y; i < y + h; i++)
	{
		c_put_str(TERM_L_BLUE, "|", i, x);
		c_put_str(TERM_L_BLUE, "|", i, x + w);
	}
	Term_putch(x, y, TERM_L_BLUE, '/');
	Term_putch(x + w, y, TERM_L_BLUE, '\\');
	Term_putch(x, y + h, TERM_L_BLUE, '\\');
	Term_putch(x + w, y + h, TERM_L_BLUE, '/');
}


/*
 * Displays a scrollable boxed list with a selected item
 */
void display_list(s32b y, s32b x, s32b h, s32b w, cptr title, cptr *list,
				  s32b max, s32b begin, s32b sel, byte sel_color,
				  bool show_scrollbar)
{
	s32b i, thumb_pos;

	draw_box(y, x, h, w);
	display_message(x + ((w - strlen(title)) / 2), y, strlen(title),
					TERM_L_BLUE, title);

	for (i = 0; i < h - 1; i++)
	{
		byte color = TERM_WHITE;

		if (i + begin >= max) break;

		if (i + begin == sel) color = sel_color;
		display_message(x + 1, y + 1 + i, strlen(list[i + begin]),
						color, list[i + begin]);
	}

	if (!show_scrollbar || max <= h - 2)
		return;

	thumb_pos = (h - 3) * begin / max + 2;

	for (i = 1; i < h; i++)
	{
		cptr c;

		if (i == 1)
			c = "^";
		else if (i == h - 1)
			c = "v";
		else if (i == thumb_pos)
			c = "*";
		else
			c = "|";

		c_put_str(TERM_YELLOW, c, y + i, x + w - 1);
	}
}

/*
 * Creates an input box
 */
bool input_box(cptr text, s32b y, s32b x, char *buf, s32b max)
{
	s32b smax;
	bool ret;
	term_win *save = Term_save_to();

	if (max < strlen(text)) smax = strlen(text) + 1;
	else smax = max + 1;

	darken_screen(TERM_SLATE);
	draw_box(y - 1, x - (smax / 2), 3, smax);
	c_put_str(TERM_WHITE, text, y, x - (strlen(text) / 2));

	Term_gotoxy(x - (smax / 2) + 1, y + 1);
	ret = askfor_aux(buf, max);

	Term_load_from(save, TRUE);

	return ret;
}
/*
 * Creates a msg bbox and ask a question
 */
s32b msg_box(cptr text, s32b y, s32b x)
{
	s32b  i, j;
	s32b  nb_line = 0;
	s32b  len = 0;
	s32b  max_len = 0;
	s32b ret;
	term_win *save = Term_save_to();

	if (x == -1)
	{
		s32b wid = 0, hgt = 0;
		Term_get_size(&wid, &hgt);
		x = wid / 2;
		y = hgt / 2;
	}

	i = 0;
	while (text[i])
	{
		len++;
		i++;
		if ((text[i] == '\0') || (text[i] == '\n'))
		{
			if (len > max_len) max_len = len;
			len = 0;
			nb_line++;
		}
	}

	darken_screen(TERM_SLATE);
	draw_box(y - 1, x - ((max_len + 1) / 2), 1 + nb_line, max_len + 1);
	j = 0;
	i = 0;
	nb_line = 0;
	while (text[i])
	{
		i++;
		if ((text[i] == '\0') || (text[i] == '\n'))
		{
			display_message(x - ((max_len + 1) / 2) + 1, y + nb_line, i - j, TERM_WHITE, text + j);
			nb_line++;
			j = i + 1;
		}
	}
	ret = inkey();

	Term_load_from(save, TRUE);
	return ret;
}

/* Rescale a value */
s32b rescale(s32b x, s32b max, s32b new_max)
{
	return (x * new_max) / max;
}
/*
 * Timers
 */
timer_type *new_timer(cptr callback, s32b delay)
{
	timer_type *t_ptr;

	MAKE(t_ptr, timer_type);
	t_ptr->next = gl_timers;
	gl_timers = t_ptr;

	t_ptr->callback = string_make(callback);
	t_ptr->delay = delay;
	t_ptr->countdown = delay;
	t_ptr->enabled = FALSE;

	return t_ptr;
}

void del_timer(timer_type *t_ptr)
{
	timer_type *i, *old;

	old = NULL;
	for (i = gl_timers; (i != NULL) && (i != t_ptr); old = i, i = i->next)
		;
	if (i)
	{
		if (old == NULL)
			gl_timers = t_ptr->next;
		else
			old->next = t_ptr->next;
		string_free(t_ptr->callback);
		FREE(t_ptr, timer_type);
	}
	else
		cmsg_print(TERM_VIOLET, "Unknown timer!");
}

/*
 * Data buffers
 */
buffer_type *data_buffer_alloc()
{
	buffer_type *buf;

	MAKE(buf, buffer_type);
	buf->len = 0;
	buf->buf = NULL;

	return buf;
}
void data_buffer_delete(buffer_type *buf)
{
	if (buf->len > 0)
		C_FREE(buf->buf, buf->len, char);
	FREE(buf, buffer_type);
}

buffer_type *data_buffer_dup(buffer_type *buf)
{
	buffer_type *buf2 = data_buffer_alloc();

	buf2->len = buf->len;
	if (buf->len > 0)
	{
		C_MAKE(buf2->buf, buf2->len, char);
		memcpy(buf2->buf, buf->buf, buf->len);
	}

	return buf2;
}

void data_buffer_loadsave(byte type, buffer_type **bbuf, s32b flag)
{
	buffer_type *buf = *bbuf;
	u32b i;

	if ((flag == LS_LOAD) && (buf->len > 0))
		C_FREE(buf->buf, buf->len, char);

	do_u32b(&buf->len, flag);

	if (flag == LS_LOAD)
		C_MAKE(buf->buf, buf->len, char);

	for (i = 0; i < buf->len; i++)
		do_byte((byte*) &buf->buf[i], flag);
}

/*
 * math.h(sqrt) is banned of angband so ... :)
 */
s32b sroot(s32b n)
{
	s32b i = n / 2;

	if (n < 2) return (n);

	while (1)
	{
		s32b err = (i - n / (i + 1)) / 2;

		if (!err) break;

		i -= err;
	}

	return ((n / i < i) ? (i - 1) : i);
}

bool check_prevent_cmd(object_type* o_ptr) {
	return check_prevent_cmd_aux(o_ptr, command_cmd);
}

bool check_prevent_cmd_aux(object_type* o_ptr, char cmd) {
	cptr s;

	if (cmd == '\0')
		return(FALSE);

	if (!o_ptr->note)
		return(FALSE);

	/* Obtain the inscription */
	s = quark_str(o_ptr->note);

	/* Find a '!' */
	s = strchr(s, '!');

	/* Process preventions */
	while (s)
	{
		/* Check the "restriction" character */
		if ((s[1] == cmd) || (s[1] == '*'))
		{
			/* Hack -- Verify command */
			if (!get_check("Are you sure? "))
			{
				return(TRUE);
			}
		}

		/* Find another '!' */
		s = strchr(s + 1, '!');
	}
	return(FALSE);
}

/*
 * Convert a "color letter" into an "actual" color
 * The colors are: dwsorgbuDWvyRGBU, as shown below
 */
s32b color_char_to_attr(char c)
{
	switch (c)
	{
	case 'd': /*  0 (0x00) */
		return (TERM_DARK);
	case 'w': /*  1 (0x01) */
		return (TERM_WHITE);
	case 's': /*  2 (0x02) */
		return (TERM_SLATE);
	case 'o': /*  3 (0x03) */
		return (TERM_ORANGE);
	case 'r': /*  4 (0x04) */
		return (TERM_RED);
	case 'g': /*  5 (0x05) */
		return (TERM_GREEN);
	case 'b': /*  6 (0x06) */
		return (TERM_BLUE);
	case 'u': /*  7 (0x07) */
		return (TERM_UMBER);

	case 'D': /*  8 (0x08) */
		return (TERM_L_DARK);
	case 'W': /*  9 (0x09) */
		return (TERM_L_WHITE);
	case 'v': /* 10 (0x0A) */
		return (TERM_VIOLET);
	case 'y': /* 11 (0x0B) */
		return (TERM_YELLOW);
	case 'R': /* 12 (0x0C) */
		return (TERM_L_RED);
	case 'G': /* 13 (0x0D) */
		return (TERM_L_GREEN);
	case 'B': /* 14 (0x0E) */
		return (TERM_L_BLUE);
	case 'U': /* 15 (0x0F) */
		return (TERM_L_UMBER);
	}

	return ( -1);
}

/*
 * Attr value-to-char convertion table
 */
byte conv_color[16] =
{
	'd', /*  0 (0x00) */
	'w', /*  1 (0x01) */
	's', /*  2 (0x02) */
	'o', /*  3 (0x03) */
	'r', /*  4 (0x04) */
	'g', /*  5 (0x05) */
	'b', /*  6 (0x06) */
	'u', /*  7 (0x07) */
	'D', /*  8 (0x08) */
	'W', /*  9 (0x09) */
	'v', /* 10 (0x0A) */
	'y', /* 11 (0x0B) */
	'R', /* 12 (0x0C) */
	'G', /* 13 (0x0D) */
	'B', /* 14 (0x0E) */
	'U', /* 15 (0x0F) */
};

/* Blanket functions */
#ifndef USE_SDL
void sdl_utl_fast_draw(bool enable)
{
}
#endif
