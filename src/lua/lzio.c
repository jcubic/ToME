/*
** $Id: lzio.c,v 1.4 2003/09/28 03:48:31 darkgod Exp $
** a generic input stream interface
** See Copyright Notice in lua.h
*/



#include <stdio.h>
#include <string.h>

#include "lua.h"

#include "lzio.h"



/* ----------------------------------------------------- memory buffers --- */

static int zmfilbuf (ZIO* z) {
  (void)z;  /* to avoid warnings */
  return EOZ;
}


ZIO* zmopen (ZIO* z, const char* b, size_t size, const char *name) {
  if (b==NULL) return NULL;
  z->n = size;
  z->p = (const unsigned char *)b;
  z->filbuf = zmfilbuf;
  z->u = NULL;
  z->name = name;
  return z;
}

/* ------------------------------------------------------------ strings --- */

ZIO* zsopen (ZIO* z, const char* s, const char *name) {
  if (s==NULL) return NULL;
  return zmopen(z, s, strlen(s), name);
}

/* -------------------------------------------------------------- FILEs --- */

static int zffilbuf (ZIO* z) {
  size_t n;
  if (PHYSFS_eof((PHYSFS_file *)z->u)) return EOZ;
  n = PHYSFS_read((PHYSFS_file *)z->u, z->buffer, 1, ZBSIZE);
  if (n==0) return EOZ;
  z->n = n-1;
  z->p = z->buffer;
  return *(z->p++);
}


ZIO* zFopen (ZIO* z, PHYSFS_file* f, const char *name) {
  if (f==NULL) return NULL;
  z->n = 0;
  z->p = z->buffer;
  z->filbuf = zffilbuf;
  z->u = f;
  z->name = name;
  return z;
}


/* --------------------------------------------------------------- read --- */
size_t zread (ZIO *z, void *b, size_t n) {
  while (n) {
    size_t m;
    if (z->n == 0) {
      if (z->filbuf(z) == EOZ)
        return n;  /* return number of missing bytes */
      zungetc(z);  /* put result from `filbuf' in the buffer */
    }
    m = (n <= z->n) ? n : z->n;  /* min. between n and z->n */
    memcpy(b, z->p, m);
    z->n -= m;
    z->p += m;
    b = (char *)b + m;
    n -= m;
  }
  return 0;
}
