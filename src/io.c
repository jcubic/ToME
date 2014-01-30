/* File: io.c */

/* Purpose: some easy wrappers around physfs functions */

/*
 * Copyright (c) 2003 DarkGod
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "z-form.h"
#include "z-util.h"
#include "physfs.h"

extern unsigned char module_initialized;
extern unsigned char call_lua(const char *function, const char *args, const char *ret, ...);

#ifdef SET_UID

/*
* Extract a "parsed" path from an initial filename
* Normally, we simply copy the filename into the buffer
* But leading tilde symbols must be handled in a special way
* Replace "~user/" by the home directory of the user named "user"
* Replace "~/" by the home directory of the current user
*/
errr path_parse(char *buf, s32b max, cptr file)
{
	cptr	u, s;
	struct passwd	*pw;
	char	user[128];


	/* Assume no result */
	buf[0] = '\0';

	/* No file? */
	if (!file) return ( -1);

	/* File needs no parsing */
	if (file[0] != '~')
	{
		strcpy(buf, file);
		return (0);
	}

	/* Point at the user */
	u = file + 1;

	/* Look for non-user portion of the file */
	s = strstr(u, PATH_SEP);

	/* Hack -- no long user names */
	if (s && (s >= u + sizeof(user))) return (1);

	/* Extract a user name */
	if (s)
	{
		s32b i;
		for (i = 0; u < s; ++i) user[i] = *u++;
		user[i] = '\0';
		u = user;
	}

	/* Look up the "current" user */
	if (u[0] == '\0') u = getlogin();

	/* Look up a user (or "current" user) */
	if (u) pw = getpwnam(u);
	else pw = getpwuid(getuid());

	/* Nothing found? */
	if (!pw) return (1);

	/* Make use of the info */
	(void)strcpy(buf, pw->pw_dir);

	/* Append the rest of the filename, if any */
	if (s) (void)strcat(buf, s);

	/* Success */
	return (0);
}


#else /* SET_UID */


/*
* Extract a "parsed" path from an initial filename
*
* This requires no special processing on simple machines,
* except for verifying the size of the filename.
*/
errr path_parse(char *buf, s32b max, cptr file)
{
	/* Accept the filename */
	strnfmt(buf, max, "%s", file);

	/* Success */
	return (0);
}
#endif /* SET_UID */

/*
 * Hack -- replacement for "fopen()"
 */
PHYSFS_file *my_fopen(const char *file, const char *mode)
{
	char buf[1024];
	PHYSFS_file *s = NULL;
	s32b compress = -1;

	/* is that location rehooked to somewhere else? */
	if (module_initialized)
	{
		const char *new_file;

		call_lua("get_real_file_path", "(s)", "s", file, &new_file);
		file = new_file;
	}

	/* Hack -- Try to parse the path */
	if (path_parse(buf, 1024, file)) return (NULL);

	if (strchr(mode, 'z'))
	{
		compress = 4;
		if (strchr(mode, '0')) compress = 0;
		else if (strchr(mode, '1')) compress = 1;
		else if (strchr(mode, '2')) compress = 2;
		else if (strchr(mode, '3')) compress = 3;
		else if (strchr(mode, '4')) compress = 4;
		else if (strchr(mode, '5')) compress = 5;
		else if (strchr(mode, '6')) compress = 6;
		else if (strchr(mode, '7')) compress = 7;
		else if (strchr(mode, '8')) compress = 8;
		else if (strchr(mode, '9')) compress = 9;
	}

	if (strchr(mode, 'w'))
		s = PHYSFS_openWrite(file, compress);
	else if (strchr(mode, 'r'))
		s = PHYSFS_openRead(file, compress);
	else if (strchr(mode, 'a'))
		s = PHYSFS_openAppend(file, compress);
	else
		quit(format("Unknown file open mode '%s'\n", mode));

	/* Buffer mode on if file opened succesfully */
	if ((s != NULL) && (compress == -1)) PHYSFS_setBuffer(s, 8182);

	/* Attempt to fopen the file anyway */
	return (s);
}


/*
* Hack -- replacement for "fclose()"
*/
errr my_fclose(PHYSFS_file *fff)
{
	/* Require a file */
	if (!fff) return ( -1);

	/* Close, check for error */
	if (PHYSFS_close(fff) == 0) return (1);

	/* Success */
	return (0);
}


s32b my_fprintf(PHYSFS_file *stream, const char *format, ...)
{
	/* DGDGDGDG -- HACK ATTACK ! */
	char buf[4096];

	va_list ap;

	va_start(ap, format);
	vstrnfmt(buf, 4096, format, ap);
	va_end(ap);
	return PHYSFS_write(stream, buf, 1, strlen(buf));
}

s32b my_physfs_fputc(s32b c, PHYSFS_file *stream)
{
	char cc = c;
	return (PHYSFS_write(stream, &cc, 1, 1) > 0) ? c : EOF;
}

s32b my_physfs_fgetc(PHYSFS_file *stream)
{
	char cc;
	return (PHYSFS_read(stream, &cc, 1, 1) > 0) ? cc : EOF;
}
