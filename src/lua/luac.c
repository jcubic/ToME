/*
 * Takes care of compiling lua to bytecode for faster loading
 * Most code from the lua4 implementation
 * Modified to work in ToME by DarkGod
 */
#include "angband.h"

#include "lparser.h"
#include "lstate.h"
#include "lzio.h"
#include "ldebug.h"
#include "lfunc.h"
#include "lmem.h"
#include "lobject.h"
#include "lopcodes.h"
#include "lstring.h"
#include "ltable.h"
#include "lundump.h"

#define Sizeof(x)	((int)sizeof(x))

static lua_State* compile_lua_state = NULL;

bool tome_compile_lua_stripping = TRUE;		/* strip debug information? */
bool tome_compile_lua_optimizing = TRUE;	/* optimize generated code? */

/*
 * Bytecode optimization
 */
static int MapConstant(Hash* t, int j, const TObject* key)
{
	const TObject* o=luaH_get(compile_lua_state,t,key);
	if (ttype(o)==LUA_TNUMBER)
		return (int) nvalue(o);
	else
	{
		TObject val;
		ttype(&val)=LUA_TNUMBER;
		nvalue(&val)=j;
		*luaH_set(compile_lua_state,t,key)=val;
		LUA_ASSERT(j>=0,"MapConstant returns negative!");
		return j;
	}
}

static int MapConstants(Proto* tf, Hash* map)
{
	int i,j,k,n,m=0;
	TObject o;
	j=0; n=tf->nknum; ttype(&o)=LUA_TNUMBER;
	for (i=0; i<n; i++)
	{
		nvalue(&o)=tf->knum[i];
		k=MapConstant(map,j,&o);
		if (k==j) j++;
	}
	m=j;
	j=0; n=tf->nkstr; ttype(&o)=LUA_TSTRING;
	for (i=0; i<n; i++)
	{
		tsvalue(&o)=tf->kstr[i];
		k=MapConstant(map,j,&o);
		if (k==j) j++;
	}
	return m+j;
}

static void PackConstants(Proto* tf, Hash* map)
{
	int i,j,k,n;
	TObject o;

	j=0; n=tf->nknum; ttype(&o)=LUA_TNUMBER;
	for (i=0; i<n; i++)
	{
		nvalue(&o)=tf->knum[i];
		k=MapConstant(map,-1,&o);
		if (k==j) tf->knum[j++]=tf->knum[i];
	}
	tf->nknum=j;
	j=0; n=tf->nkstr; ttype(&o)=LUA_TSTRING;
	for (i=0; i<n; i++)
	{
		tsvalue(&o)=tf->kstr[i];
		k=MapConstant(map,-1,&o);
		if (k==j) tf->kstr[j++]=tf->kstr[i];
	}
	tf->nkstr=j;
}

static void OptConstants(Proto* tf)
{
	Instruction* p;
	int n=tf->nknum+tf->nkstr;
	Hash* map=luaH_new(compile_lua_state,n);
	int m=MapConstants(tf,map);

	if (m==n) return;
	for (p=tf->code;; p++)
	{
		Instruction i=*p;
		int op=GET_OPCODE(i);
		switch (op)
		{
			TObject o;
			int j,k;
		case OP_PUSHNUM: case OP_PUSHNEGNUM:
			j=GETARG_U(i);
			ttype(&o)=LUA_TNUMBER; nvalue(&o)=tf->knum[j];
			k=MapConstant(map,-1,&o);
			if (k!=j) *p=CREATE_U(op,k);
			break;
		case OP_PUSHSTRING: case OP_GETGLOBAL: case OP_GETDOTTED:
		case OP_PUSHSELF:   case OP_SETGLOBAL:
			j=GETARG_U(i);
			ttype(&o)=LUA_TSTRING; tsvalue(&o)=tf->kstr[j];
			k=MapConstant(map,-1,&o);
			if (k!=j) *p=CREATE_U(op,k);
			break;
		case OP_END:
			PackConstants(tf,map);
			luaH_free(compile_lua_state,map);
			return;
		default:
			break;
		}
	}
}

#define OptFunction luaU_optchunk

static void OptFunction(Proto* tf)
{
	int i,n=tf->nkproto;
	OptConstants(tf);
	for (i=0; i<n; i++) OptFunction(tf->kproto[i]);
}


/*
 * Bytecode dumping
 */

/* Dump to a file */
static void DumpByte_file(int c, void *D)
{
	my_physfs_fputc(c, (PHYSFS_file*)D);
}
static void DumpSig_file(cptr sig, void *D)
{
	my_fprintf((PHYSFS_file*)D, "%s", sig);
}
static void DumpBlock_file(void *buf, size_t size, void *D)
{
	PHYSFS_write((PHYSFS_file*)D, buf, size, 1);
}
static void DumpVector_file(void *buf, size_t nb, size_t size, void *D)
{
	PHYSFS_write((PHYSFS_file*)D, buf, size, nb);
}

/* Do not dump, just calc size */
static u32b __lua_dumper_size = 0;
static void DumpByte_calc_size(int c, void *D)
{
	__lua_dumper_size++;
}
static void DumpSig_calc_size(cptr sig, void *D)
{
	__lua_dumper_size += strlen(sig);
}
static void DumpBlock_calc_size(void *buf, size_t size, void *D)
{
	__lua_dumper_size += size;
}
static void DumpVector_calc_size(void *buf, size_t nb, size_t size, void *D)
{
	__lua_dumper_size += size * nb;
}

/* Dump to a buffer */
static void DumpByte_buffer(int c, void *D)
{
	((char*)D)[__lua_dumper_size++] = (char)c;
}
static void DumpSig_buffer(cptr sig, void *D)
{
	size_t len = strlen(sig);

	memcpy(&((char*)D)[__lua_dumper_size], sig, len);
	__lua_dumper_size += len;
}
static void DumpBlock_buffer(void *buf, size_t size, void *D)
{
	memcpy(&((char*)D)[__lua_dumper_size], buf, size);
	__lua_dumper_size += size;
}
static void DumpVector_buffer(void *buf, size_t nb, size_t size, void *D)
{
	memcpy(&((char*)D)[__lua_dumper_size], buf, size * nb);
	__lua_dumper_size += size * nb;
}

static void (*DumpByte)(int c, void *D);
static void (*DumpSig)(cptr sig, void *D);
static void (*DumpBlock)(void *buf, size_t size, void *D);
static void (*DumpVector)(void *buf, size_t nb, size_t size, void *D);

static void DumpInt(int x, void* D)
{
	DumpBlock(&x,sizeof(x),D);
}

static void DumpSize(size_t x, void* D)
{
	DumpBlock(&x,sizeof(x),D);
}

static void DumpNumber(Number x, void* D)
{
	DumpBlock(&x,sizeof(x),D);
}

static void DumpString(const TString* s, void* D)
{
	if (s==NULL || s->str==NULL)
		DumpSize(0,D);
	else
	{
		size_t size=s->len+1;			/* include trailing '\0' */
		DumpSize(size,D);
		DumpBlock(s->str,size,D);
	}
}

static void DumpCode(const Proto* tf, void* D)
{
	DumpInt(tf->ncode,D);
	DumpVector(tf->code,tf->ncode,sizeof(*tf->code),D);
}

static void DumpLocals(const Proto* tf, void* D)
{
	int i,n=tf->nlocvars;
	DumpInt(n,D);
	for (i=0; i<n; i++)
	{
		DumpString(tf->locvars[i].varname,D);
		DumpInt(tf->locvars[i].startpc,D);
		DumpInt(tf->locvars[i].endpc,D);
	}
}

static void DumpLines(const Proto* tf, void* D)
{
	DumpInt(tf->nlineinfo,D);
	DumpVector(tf->lineinfo,tf->nlineinfo,sizeof(*tf->lineinfo),D);
}

static void DumpFunction(const Proto* tf, void* D);

static void DumpConstants(const Proto* tf, void* D)
{
	int i,n;
	DumpInt(n=tf->nkstr,D);
	for (i=0; i<n; i++)
		DumpString(tf->kstr[i],D);
	DumpInt(tf->nknum,D);
	DumpVector(tf->knum,tf->nknum,sizeof(*tf->knum),D);
	DumpInt(n=tf->nkproto,D);
	for (i=0; i<n; i++)
		DumpFunction(tf->kproto[i],D);
}

static void DumpFunction(const Proto* tf, void* D)
{
	DumpString(tf->source,D);
	DumpInt(tf->lineDefined,D);
	DumpInt(tf->numparams,D);
	DumpByte(tf->is_vararg,D);
	DumpInt(tf->maxstacksize,D);
	DumpLocals(tf,D);
	DumpLines(tf,D);
	DumpConstants(tf,D);
	DumpCode(tf,D);
}

static void DumpHeader(void* D)
{
	DumpByte(ID_CHUNK,D);
	DumpSig(SIGNATURE, D);
	DumpByte(VERSION,D);
	DumpByte(luaU_endianess(),D);
	DumpByte(sizeof(int),D);
	DumpByte(sizeof(size_t),D);
	DumpByte(sizeof(Instruction),D);
	DumpByte(SIZE_INSTRUCTION,D);
	DumpByte(SIZE_OP,D);
	DumpByte(SIZE_B,D);
	DumpByte(sizeof(Number),D);
	DumpNumber(TEST_NUMBER,D);
}

void luaU_dumpchunk_file(const Proto* Main, PHYSFS_file* D)
{
	cptr err;

	DumpByte = DumpByte_file;
	DumpSig = DumpSig_file;
	DumpBlock = DumpBlock_file;
	DumpVector = DumpVector_file;

	DumpHeader(D);
	DumpFunction(Main,D);

	if ((err = PHYSFS_getLastError()) != NULL)
	{
		quit(format("luac: write error: %s", err));
	}
}

char *luaU_dumpchunk_buffer(const Proto* Main, size_t *len)
{
	char *buf;

	/* First calculate the total size*/
	__lua_dumper_size = 0;
	DumpByte = DumpByte_calc_size;
	DumpSig = DumpSig_calc_size;
	DumpBlock = DumpBlock_calc_size;
	DumpVector = DumpVector_calc_size;

	DumpHeader(NULL);
	DumpFunction(Main, NULL);

	/* Allocate the buffer */
	*len = __lua_dumper_size;
	C_MAKE(buf, __lua_dumper_size, char);

	/* Now dump it for real */
	__lua_dumper_size = 0;
	DumpByte = DumpByte_buffer;
	DumpSig = DumpSig_buffer;
	DumpBlock = DumpBlock_buffer;
	DumpVector = DumpVector_buffer;

	DumpHeader(buf);
	DumpFunction(Main, buf);
	return buf;
}


/*
 * Lua source to bytecode loading
 */
PHYSFS_file* efopen(const char* name, const char* mode)
{
	PHYSFS_file* f = my_fopen(name, mode);
	if (f==NULL)
	{
		quit(format("luac: cannot open %sput file '%s' : %s\n",*mode=='r' ? "in" : "out", name, PHYSFS_getLastError()));
		return NULL;
	}
	return f;
}

static Proto* load(const char* filename)
{
	Proto* tf;
	ZIO z;
	char source[512];
	PHYSFS_file* f;


        /* Open the file */
	f = efopen(filename, "r");

	sprintf(source,"@%.*s",Sizeof(source)-2,filename);

	luaZ_Fopen(&z,f,source);

        /* Parse the lua code */
	tf = luaY_parser(compile_lua_state,&z);

        /* Close the file */
	my_fclose(f);

	return tf;
}

static void strip(Proto* tf)
{
	int i,n=tf->nkproto;

	tf->lineinfo=NULL;
	tf->nlineinfo=0;
	tf->source=luaS_new(compile_lua_state,"=(none)");
	tf->locvars=NULL;
	tf->nlocvars=0;
	for (i=0; i<n; i++) strip(tf->kproto[i]);
}

/*
 * Compiles src lua source to dest lua bytecode
 * Bytecode is quicker to load as it is preparsed and optimized.
 * This function does NOT understands ToME virtual paths they
 * need to be converted first, this is intentionnal
 */
s32b tome_compile_lua(char *src)
{
	char *split;
	char dest[2048];
	Proto *tf;
	PHYSFS_file *dump;

	/* Clear error state */
	PHYSFS_getLastError();

        /* A new empty lua state */
	compile_lua_state = lua_open(0);

	tf = load(src);

        /* If we dump we need to optimize */
	if (tome_compile_lua_optimizing) luaU_optchunk(tf);

	/* Dump to a file */

	if (tome_compile_lua_stripping) strip(tf);

	/* Make up the destination file .lua => .lb */
	strncpy(dest, src, 2048);
	dest[strlen(dest) - 2] = 'b';
	dest[strlen(dest) - 1] = '\0';

	/* Make sure distination exists */
	split = strrchr(dest, '/');
	*split = '\0';
	PHYSFS_mkdir(dest);
	*split = '/';

	dump = efopen(dest, "wb");
	if (dump == NULL)
		quit("Could not compile lua: error writting");
	PHYSFS_getLastError();
	luaU_dumpchunk_file(tf, dump);
	my_fclose(dump);

	/* Clean up */
/* For a strange reasons this totaly panics lua ... 	lua_close(compile_lua_state); */

	return 0;
}
