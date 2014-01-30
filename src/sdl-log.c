/* Copyright (C) 2006 Eric Stevens <sdltome@gmail.com>
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
* THE AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
* AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name(s) of the author(s) shall not be
* used in advertising or otherwise to promote the sale, use or other dealings
* in this Software without prior written authorization from the author(s).
*
*******************************************************************************
*/

#ifdef USE_SDL

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>

#include "sdl-types.h"
#include "sdl-defines.h"
#include "sdl-extern.h"

#ifdef WIN32
#define snprintf	_snprintf
#endif

/*
 * prototypes
 */

void sdl_log_write(Uint32 level, char* fmt, ...);


void sdl_log_info(char* fmt, ...)
{
	va_list	args;
	char		logstr[MAX_PATH_LEN];

	if (TST_CFLAG(LOG_ALL))
	{
		 va_start(args, fmt);
		 vsprintf(logstr, fmt, args);
		 sdl_log_write(ERRLVL_NO_ERR, "%s", logstr);
		 va_end(args);
	}
}

void sdl_log_warn(char* fmt, ...)
{
	va_list	args;
	char		logstr[MAX_PATH_LEN];

	if (TST_CFLAG(LOG_ALL))
	{
		 va_start(args, fmt);
		 vsprintf(logstr, fmt, args);
		 sdl_log_write(ERRLVL_WARNING, "%s", logstr);
		 va_end(args);
	}
}

void sdl_log_error(char* fmt, ...)
{
	va_list	args;
	char	logstr[MAX_PATH_LEN];

	va_start(args, fmt);
	vsprintf(logstr, fmt, args);
	sdl_log_write(ERRLVL_CRITICAL, "%s", logstr);
	va_end(args);

	/* end it - errors mean the application can't continue */
	quit(logstr);
}

void sdl_log_start(char* fmt, ...)
{
	va_list	args;
	char		logstr[MAX_PATH_LEN];

	va_start(args, fmt);
	vsprintf(logstr, fmt, args);
	sdl_log_write(LOG_START, "%s", logstr);
	va_end(args);
}

void sdl_log_stop(char* fmt, ...)
{
	va_list	args;
	char		logstr[MAX_PATH_LEN];

	va_start(args, fmt);
	vsprintf(logstr, fmt, args);
	sdl_log_write(LOG_STOP, "%s", logstr);
	va_end(args);
}

void sdl_log_write(Uint32 level, char* fmt, ...)
{
	PHYSFS_file* h_file;
	va_list	args;
	char	timestring[255];
	char	logstr[MAX_PATH_LEN];
	char	logline[MAX_PATH_LEN];
	char	typestr[32];
	cptr    prev_write_dir;

	errlvl = level;

	// label the log informational level correctly
	switch (errlvl)
	{
		case ERRLVL_CRITICAL:
			sprintf(typestr, "ERROR");
			break;

		case ERRLVL_WARNING:
			sprintf(typestr, "WARNING");
			break;

		case ERRLVL_NO_ERR:
			sprintf(typestr, "INFO");
			break;

		case LOG_START:
			sprintf(typestr, "START");
			break;

		case LOG_STOP:
			sprintf(typestr, "STOP");
			break;

		default:
			/* we don't like undefined behaviour */
			errlvl = ERRLVL_CRITICAL;
			sprintf(typestr, "UNDEFINED");
			break;
	}

	sdl_utl_get_datetime(timestring);

	if (timestring[0])
	{
		// move the the time-stamped log entry to a temporary buffer
		va_start(args, fmt);
		vsprintf(logstr, fmt, args);
		va_end(args);

		// formulate the log text
		snprintf(logline, MAX_PATH_LEN, "%s [%s] %s\n", timestring, typestr, logstr);

		// push the current write path, if any, just in case and set our new one
		prev_write_dir = string_make(PHYSFS_getWriteDir());	
		PHYSFS_setWriteDir(SDL_LOG_DIR);				
		
		// open the log file and write the entry
		h_file = my_fopen(SDL_LOG_FILE, "a");
		
		if (h_file)
		{
			my_fprintf(h_file, "%s", logline);
			my_fclose(h_file);
		}
		else
		{
			printf("Log error - open file: %s (%s)\n", PHYSFS_getLastError(), SDL_LOG_FILE);
		}		 
		errlvl &= ~(LOG_START | LOG_STOP);

		// pop the previous write path
		if (!PHYSFS_setWriteDir(prev_write_dir))
		{
			printf("Log error - restore write dir: %s\n", PHYSFS_getLastError());
		}
	}
}

#endif /* USE_SDL */
