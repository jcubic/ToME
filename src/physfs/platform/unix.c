/*
 * Unix support routines for PhysicsFS.
 *
 * Please see the file LICENSE in the source's root directory.
 *
 *  This file written by Ryan C. Gordon.
 */

#if HAVE_CONFIG_H
#  include <config.h>
#endif

/* BeOS uses beos.cpp and posix.c ... Cygwin and such use win32.c ... */
#if ((!defined __BEOS__) && (!defined WIN32))

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <dirent.h>
#include <time.h>
#include <errno.h>
#include <sys/mount.h>

#ifndef PHYSFS_DARWIN
#  if defined(__MACH__) && defined(__APPLE__)
#    define PHYSFS_DARWIN 1
#    include <CoreFoundation/CoreFoundation.h>
#    include <CoreServices/CoreServices.h>
#    include <IOKit/IOKitLib.h>
#    include <IOKit/storage/IOMedia.h>
#    include <IOKit/storage/IOCDMedia.h>
#    include <IOKit/storage/IODVDMedia.h>
#  endif
#endif

#if (!defined PHYSFS_NO_PTHREADS_SUPPORT)
#include <pthread.h>
#endif

#ifdef PHYSFS_HAVE_SYS_UCRED_H
#  ifdef PHYSFS_HAVE_MNTENT_H
#    undef PHYSFS_HAVE_MNTENT_H /* don't do both... */
#  endif
#  include <sys/ucred.h>
#endif

#ifdef PHYSFS_HAVE_MNTENT_H
#include <mntent.h>
#endif

#define __PHYSICSFS_INTERNAL__
#include "physfs_internal.h"


const char *__PHYSFS_platformDirSeparator = "/";


int __PHYSFS_platformInit(void)
{
    return(1);  /* always succeed. */
} /* __PHYSFS_platformInit */


int __PHYSFS_platformDeinit(void)
{
    return(1);  /* always succeed. */
} /* __PHYSFS_platformDeinit */


#ifdef PHYSFS_NO_CDROM_SUPPORT

/* Stub version for platforms without CD-ROM support. */
char **__PHYSFS_platformDetectAvailableCDs(void)
{
    char **retval = (char **) malloc(sizeof (char *));
    BAIL_IF_MACRO(retval == NULL, ERR_OUT_OF_MEMORY, NULL);
    *retval = NULL;
    return(retval);
} /* __PHYSFS_platformDetectAvailableCDs */


#elif (defined PHYSFS_DARWIN)  /* "Big Nasty." */
/*
 * Code based on sample from Apple Developer Connection:
 *  http://developer.apple.com/samplecode/Sample_Code/Devices_and_Hardware/Disks/VolumeToBSDNode/VolumeToBSDNode.c.htm
 */

static int darwinIsWholeMedia(io_service_t service)
{
    int retval = 0;
    CFTypeRef wholeMedia;

    if (!IOObjectConformsTo(service, kIOMediaClass))
        return(0);
        
    wholeMedia = IORegistryEntryCreateCFProperty(service,
                                                 CFSTR(kIOMediaWholeKey),
                                                 kCFAllocatorDefault, 0);
    if (wholeMedia == NULL)
        return(0);

    retval = CFBooleanGetValue(wholeMedia);
    CFRelease(wholeMedia);

    return retval;
} /* darwinIsWholeMedia */


static int darwinIsMountedDisc(char *bsdName, mach_port_t masterPort)
{
    int retval = 0;
    CFMutableDictionaryRef matchingDict;
    kern_return_t rc;
    io_iterator_t iter;
    io_service_t service;

    if ((matchingDict = IOBSDNameMatching(masterPort, 0, bsdName)) == NULL)
        return(0);

    rc = IOServiceGetMatchingServices(masterPort, matchingDict, &iter);
    if ((rc != KERN_SUCCESS) || (!iter))
        return(0);

    service = IOIteratorNext(iter);
    IOObjectRelease(iter);
    if (!service)
        return(0);

    rc = IORegistryEntryCreateIterator(service, kIOServicePlane,
             kIORegistryIterateRecursively | kIORegistryIterateParents, &iter);
    
    if (!iter)
        return(0);

    if (rc != KERN_SUCCESS)
    {
        IOObjectRelease(iter);
        return(0);
    } /* if */

    IOObjectRetain(service);  /* add an extra object reference... */

    do
    {
        if (darwinIsWholeMedia(service))
        {
            if ( (IOObjectConformsTo(service, kIOCDMediaClass)) ||
                 (IOObjectConformsTo(service, kIODVDMediaClass)) )
            {
                retval = 1;
            } /* if */
        } /* if */
        IOObjectRelease(service);
    } while ((service = IOIteratorNext(iter)) && (!retval));
                
    IOObjectRelease(iter);
    IOObjectRelease(service);

    return(retval);
} /* darwinIsMountedDisc */


char **__PHYSFS_platformDetectAvailableCDs(void)
{
    const char *devPrefix = "/dev/";
    int prefixLen = strlen(devPrefix);
    mach_port_t masterPort = 0;
    char **retval = (char **) malloc(sizeof (char *));
    int cd_count = 1;  /* We count the NULL entry. */
    struct statfs *mntbufp;
    int i, mounts;

    retval[0] = NULL;

    if (IOMasterPort(MACH_PORT_NULL, &masterPort) != KERN_SUCCESS)
        return(retval);

    mounts = getmntinfo(&mntbufp, MNT_WAIT);  /* NOT THREAD SAFE! */
    for (i = 0; i < mounts; i++)
    {
        char *dev = mntbufp[i].f_mntfromname;
        char *mnt = mntbufp[i].f_mntonname;
        if (strncmp(dev, devPrefix, prefixLen) != 0)  /* a virtual device? */
            continue;

        dev += prefixLen;
        if (darwinIsMountedDisc(dev, masterPort))
        {
            char **tmp = realloc(retval, sizeof (char *) * (cd_count + 1));
            if (tmp)
            {
                retval = tmp;
                retval[cd_count - 1] = (char *) malloc(strlen(mnt) + 1);
                if (retval[cd_count - 1])
                {
                    strcpy(retval[cd_count - 1], mnt);
                    cd_count++;
                } /* if */
            } /* if */
        } /* if */
    } /* for */

    retval[cd_count - 1] = NULL;
    return(retval);
} /* __PHYSFS_platformDetectAvailableCDs */

#elif (defined PHYSFS_HAVE_SYS_UCRED_H)

char **__PHYSFS_platformDetectAvailableCDs(void)
{
    char **retval = (char **) malloc(sizeof (char *));
    int cd_count = 1;  /* We count the NULL entry. */
    struct statfs *mntbufp = NULL;
    int mounts;
    int i;

    BAIL_IF_MACRO(retval == NULL, ERR_OUT_OF_MEMORY, NULL);

    mounts = getmntinfo(&mntbufp, MNT_WAIT);

    for (i = 0; i < mounts; i++)
    {
        int add_it = 0;

        if (strcmp(mntbufp[i].f_fstypename, "iso9660") == 0)
            add_it = 1;
        else if (strcmp( mntbufp[i].f_fstypename, "cd9660") == 0)
            add_it = 1;

        /* add other mount types here */

        if (add_it)
        {
            char **tmp = realloc(retval, sizeof (char *) * (cd_count + 1));
            if (tmp)
            {
                retval = tmp;
                retval[cd_count - 1] = (char *)
                                malloc(strlen(mntbufp[i].f_mntonname) + 1);
                if (retval[cd_count - 1])
                {
                    strcpy(retval[cd_count - 1], mntbufp[i].f_mntonname);
                    cd_count++;
                } /* if */
            } /* if */
        } /* if */
    } /* for */

    retval[cd_count - 1] = NULL;
    return(retval);
} /* __PHYSFS_platformDetectAvailableCDs */

#elif (defined PHYSFS_HAVE_MNTENT_H)

char **__PHYSFS_platformDetectAvailableCDs(void)
{
    char **retval = (char **) malloc(sizeof (char *));
    int cd_count = 1;  /* We count the NULL entry. */
    FILE *mounts = NULL;
    struct mntent *ent = NULL;

    BAIL_IF_MACRO(retval == NULL, ERR_OUT_OF_MEMORY, NULL);

    *retval = NULL;
    mounts = setmntent("/etc/mtab", "r");
    BAIL_IF_MACRO(mounts == NULL, ERR_IO_ERROR, retval);

    while ( (ent = getmntent(mounts)) != NULL )
    {
        int add_it = 0;
        if (strcmp(ent->mnt_type, "iso9660") == 0)
            add_it = 1;

        /* add other mount types here */

        if (add_it)
        {
            char **tmp = realloc(retval, sizeof (char *) * (cd_count + 1));
            if (tmp)
            {
                retval = tmp;
                retval[cd_count-1] = (char *) malloc(strlen(ent->mnt_dir) + 1);
                if (retval[cd_count - 1])
                {
                    strcpy(retval[cd_count - 1], ent->mnt_dir);
                    cd_count++;
                } /* if */
            } /* if */
        } /* if */
    } /* while */

    endmntent(mounts);

    retval[cd_count - 1] = NULL;
    return(retval);
} /* __PHYSFS_platformDetectAvailableCDs */

#endif


/* this is in posix.c ... */
extern char *__PHYSFS_platformCopyEnvironmentVariable(const char *varname);


/*
 * See where program (bin) resides in the $PATH specified by (envr).
 *  returns a copy of the first element in envr that contains it, or NULL
 *  if it doesn't exist or there were other problems. PHYSFS_SetError() is
 *  called if we have a problem.
 *
 * (envr) will be scribbled over, and you are expected to free() the
 *  return value when you're done with it.
 */
static char *findBinaryInPath(const char *bin, char *envr)
{
    size_t alloc_size = 0;
    char *exe = NULL;
    char *start = envr;
    char *ptr;

    BAIL_IF_MACRO(bin == NULL, ERR_INVALID_ARGUMENT, NULL);
    BAIL_IF_MACRO(envr == NULL, ERR_INVALID_ARGUMENT, NULL);

    do
    {
        size_t size;
        ptr = strchr(start, ':');  /* find next $PATH separator. */
        if (ptr)
            *ptr = '\0';

        size = strlen(start) + strlen(bin) + 2;
        if (size > alloc_size)
        {
            char *x = (char *) realloc(exe, size);
            if (x == NULL)
            {
                if (exe != NULL)
                    free(exe);
                BAIL_MACRO(ERR_OUT_OF_MEMORY, NULL);
            } /* if */

            alloc_size = size;
            exe = x;
        } /* if */

        /* build full binary path... */
        strcpy(exe, start);
        if ((exe[0] == '\0') || (exe[strlen(exe) - 1] != '/'))
            strcat(exe, "/");
        strcat(exe, bin);

        if (access(exe, X_OK) == 0)  /* Exists as executable? We're done. */
        {
            strcpy(exe, start);  /* i'm lazy. piss off. */
            return(exe);
        } /* if */

        start = ptr + 1;  /* start points to beginning of next element. */
    } while (ptr != NULL);

    if (exe != NULL)
        free(exe);

    return(NULL);  /* doesn't exist in path. */
} /* findBinaryInPath */


char *__PHYSFS_platformCalcBaseDir(const char *argv0)
{
    /* If there isn't a path on argv0, then look through the $PATH for it. */

    char *retval;
    char *envr;

    if (strchr(argv0, '/') != NULL)   /* default behaviour can handle this. */
        return(NULL);

    envr = __PHYSFS_platformCopyEnvironmentVariable("PATH");
    BAIL_IF_MACRO(!envr, NULL, NULL);
    retval = findBinaryInPath(argv0, envr);
    free(envr);
    return(retval);
} /* __PHYSFS_platformCalcBaseDir */


/* Much like my college days, try to sleep for 10 milliseconds at a time... */
void __PHYSFS_platformTimeslice(void)
{
    usleep( 10 * 1000 );           /* don't care if it fails. */
} /* __PHYSFS_platformTimeslice */


#if PHYSFS_DARWIN
/* 
 * This function is only for OSX. The problem is that Apple's applications
 * can actually be directory structures with the actual executable nested
 * several levels down. PhysFS computes the base directory from the Unix
 * executable, but this may not be the correct directory. Apple tries to
 * hide everything from the user, so from Finder, the user never sees the
 * Unix executable, and the directory package (bundle) is considered the
 * "executable". This means that the correct base directory is at the 
 * level where the directory structure starts.
 * A typical bundle seems to look like this:
 * MyApp.app/    <-- top level...this is what the user sees in Finder 
 *   Contents/
 *     MacOS/
 *       MyApp   <-- the actual executable
 *       
 * Since anything below the app folder is considered hidden, most 
 * application files need to be at the top level if you intend to
 * write portable software. Thus if the application resides in:
 * /Applications/MyProgram
 * and the executable is the bundle MyApp.app,
 * PhysFS computes the following as the base directory:
 * /Applications/MyProgram/MyApp.app/Contents/MacOS/
 * We need to strip off the MyApp.app/Contents/MacOS/
 * 
 * However, there are corner cases. OSX applications can be traditional
 * Unix executables without the bundle. Also, it is not entirely clear
 * to me what kinds of permutations bundle structures can have.
 * 
 * For now, this is a temporary hack until a better solution 
 * can be made. This function will try to find a "/Contents/MacOS"
 * inside the path. If it succeeds, then the path will be truncated
 * to correct the directory. If it is not found, the path will be 
 * left alone and will presume it is a traditional Unix execuatable.
 * Most programs also include the .app extention in the top level
 * folder, but it doesn't seem to be a requirement (Acrobat doesn't 
 * have it). MacOS looks like it can also be MacOSClassic. 
 * This function will test for MacOS and hope it captures any
 * other permutations.
 */
static void stripAppleBundle(char *path)
{
    char *sub_str = "/contents/macos";
    char *found_ptr = NULL;
    char *tempbuf = NULL;
    int i;
    
    /* Calloc will place the \0 character in the proper place for us */
    /* !!! FIXME: Can we stack-allocate this? --ryan. */
    tempbuf = (char*)calloc( (strlen(path)+1), sizeof(char) );
    /* Unlike other Unix filesystems, HFS is case insensitive
     * It wouldn be nice to use strcasestr, but it doesn't seem
     * to be available in the OSX gcc library right now.
     * So we should make a lower case copy of the path to 
     * compare against 
     */
    for(i=0; i<strlen(path); i++)
    {
        /* convert to lower case */
        tempbuf[i] = tolower(path[i]);
    }
    /* See if we can find "/contents/macos" in the path */
    found_ptr = strstr(tempbuf, sub_str);
    if(NULL == found_ptr)
    {
        /* It doesn't look like a bundle so we can keep the 
         * original path. Just return */
        free(tempbuf);
        return;
    }
    /* We have a bundle, so let's backstep character by character
     * to erase the extra parts of the path. Quit when we hit
     * the preceding '/' character.
     */
    for(i=strlen(path)-strlen(found_ptr)-1; i>=0; i--)
    {
        if('/' == path[i])
        {
            break;
        }
    }
    /* Safety check */
    if(i<1)
    {
        /* This probably shouldn't happen. */
        path[0] = '\0';
    }
    else
    {
        /* Back up one more to remove trailing '/' and set the '\0' */
        path[i] = '\0';
    }
    free(tempbuf);
    return;
}
#endif /* defined __MACH__ && defined __APPLE__ */


char *__PHYSFS_platformRealPath(const char *path)
{
    char resolved_path[MAXPATHLEN];
    char *retval = NULL;

    errno = 0;
    BAIL_IF_MACRO(!realpath(path, resolved_path), strerror(errno), NULL);
    retval = (char *) malloc(strlen(resolved_path) + 1);
    BAIL_IF_MACRO(retval == NULL, ERR_OUT_OF_MEMORY, NULL);
    strcpy(retval, resolved_path);

#if defined(__MACH__) && defined(__APPLE__)
    stripAppleBundle(retval);
#endif /* defined __MACH__ && defined __APPLE__ */
    
    return(retval);
} /* __PHYSFS_platformRealPath */


#if (defined PHYSFS_NO_PTHREADS_SUPPORT)

PHYSFS_uint64 __PHYSFS_platformGetThreadID(void) { return(0x0001); }
void *__PHYSFS_platformCreateMutex(void) { return((void *) 0x0001); }
void __PHYSFS_platformDestroyMutex(void *mutex) {}
int __PHYSFS_platformGrabMutex(void *mutex) { return(1); }
void __PHYSFS_platformReleaseMutex(void *mutex) {}

#else

/* Just in case; this is a panic value. */
#if ((!defined SIZEOF_INT) || (SIZEOF_INT <= 0))
#  define SIZEOF_INT 4
#endif

#if (SIZEOF_INT == 4)
#  define PHTREAD_TO_UI64(thr) ( (PHYSFS_uint64) ((PHYSFS_uint32) (thr)) )
#elif (SIZEOF_INT == 2)
#  define PHTREAD_TO_UI64(thr) ( (PHYSFS_uint64) ((PHYSFS_uint16) (thr)) )
#elif (SIZEOF_INT == 1)
#  define PHTREAD_TO_UI64(thr) ( (PHYSFS_uint64) ((PHYSFS_uint8) (thr)) )
#else
#  define PHTREAD_TO_UI64(thr) ((PHYSFS_uint64) (thr))
#endif

PHYSFS_uint64 __PHYSFS_platformGetThreadID(void)
{
    return(PHTREAD_TO_UI64(pthread_self()));
} /* __PHYSFS_platformGetThreadID */


void *__PHYSFS_platformCreateMutex(void)
{
    int rc;
    pthread_mutex_t *m = (pthread_mutex_t *) malloc(sizeof (pthread_mutex_t));
    BAIL_IF_MACRO(m == NULL, ERR_OUT_OF_MEMORY, NULL);
    rc = pthread_mutex_init(m, NULL);
    if (rc != 0)
    {
        free(m);
        BAIL_MACRO(strerror(rc), NULL);
    } /* if */

    return((void *) m);
} /* __PHYSFS_platformCreateMutex */


void __PHYSFS_platformDestroyMutex(void *mutex)
{
    pthread_mutex_destroy((pthread_mutex_t *) mutex);
    free(mutex);
} /* __PHYSFS_platformDestroyMutex */


int __PHYSFS_platformGrabMutex(void *mutex)
{
    return(pthread_mutex_lock((pthread_mutex_t *) mutex) == 0);    
} /* __PHYSFS_platformGrabMutex */


void __PHYSFS_platformReleaseMutex(void *mutex)
{
    pthread_mutex_unlock((pthread_mutex_t *) mutex);
} /* __PHYSFS_platformReleaseMutex */

#endif /* !PHYSFS_NO_PTHREADS_SUPPORT */


#endif /* !defined __BEOS__ && !defined WIN32 */

/* end of unix.c ... */

