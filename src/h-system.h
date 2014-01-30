/* File: h-system.h */

#ifndef INCLUDED_H_SYSTEM_H
#define INCLUDED_H_SYSTEM_H

/*
 * Include the basic "system" files.
 *
 * Make sure all "system" constants/macros are defined.
 * Make sure all "system" functions have "extern" declarations.
 *
 * This file is a big hack to make other files less of a hack.
 * This file has been rebuilt -- it may need a little more work.
 */

#include <ctype.h>
#include <errno.h>
#include <limits.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <fcntl.h>


#if defined(WINDOWS)
# include <io.h>
#endif


#ifdef SET_UID

# include <pwd.h>
# include <unistd.h>
# include <sys/stat.h>

# include <sys/types.h>
# include <sys/time.h>
# include <sys/timeb.h>
# include <sys/param.h>
# include <sys/file.h>

# ifdef linux
#  include <sys/file.h>
# endif

#endif

#endif
