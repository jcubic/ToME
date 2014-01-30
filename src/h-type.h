/* File: h-type.h */

#ifndef INCLUDED_H_TYPE_H
#define INCLUDED_H_TYPE_H

/*
 * Basic "types".
 *
 * Note the attempt to make all basic types have 4 letters.
 * This improves readibility and standardizes the code.
 */


/*** Special 4 letter names for some standard types ***/

/* A standard pointer (to "void" because ANSI C says so) */
typedef void *vptr;


/* A simple pointer (to unmodifiable strings) */
typedef const char *cptr;
typedef char *mcptr;


/* Since float's are silly, hard code real numbers as doubles */
typedef double real;


/* Error codes for function return values */
/* Success = 0, Failure = -N, Problem = +N */
typedef int errr;


/*
 * Hack -- prevent problems with AMIGA
 */
#undef byte
#define byte byte_hack

/*
 * Hack -- prevent problems with C++
 */
#undef bool
#define bool bool_hack



/* Simple true/false type */
typedef char bool;



/*
 * Use guaranteed-size ints where possible
 */
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)

  /* Use guaranteed-size types */
  #include <stdint.h>

  typedef uint8_t byte;

  typedef uint16_t u16b;
  typedef int16_t s16b;

  typedef uint32_t u32b;
  typedef int32_t s32b;

#else /* HAVE_STDINT_H */

  /* Try hacks instead (not guaranteed to work) */
  typedef unsigned char byte;
  typedef signed short s16b;
  typedef unsigned short u16b;

  /* Detect >32-bit longs */
  #if (UINT_MAX == 0xFFFFFFFFUL) && (ULONG_MAX > 0xFFFFFFFFUL)
    typedef signed int s32b;
    typedef unsigned int u32b;
  #else
    typedef signed long s32b;
    typedef unsigned long u32b;
  #endif

#endif /* HAVE_STDINT_H */


/* An unsigned, "standard" integer (often pre-defined) */
#ifndef uint
    typedef unsigned int uint;
#endif


/*
 * A normal string but used for the various foo_name and foo_text when exported to lua
 * This typdef is not exported and thus considered normal userdata(aka pointer) instead
 * of a lua string
 */
typedef char * string_line;


#endif

