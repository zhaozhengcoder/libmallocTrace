#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <malloc.h>
#include <execinfo.h>
#include <libgen.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>
#include <limits.h>

static int global_init_done = 0;
static __thread int hook_recursive = 0;
#define MAX_OUTPUT_BUFF_SIZE 4096
char logBuff[MAX_OUTPUT_BUFF_SIZE];

static void * (*glibc_malloc)(size_t) = NULL;
static void * (*glibc_calloc)(size_t, size_t)  = NULL;
static void * (*glibc_realloc)(void *, size_t) = NULL;
static void   (*glibc_free)(void *) = NULL;

static void __init_mtrace()
{
    if (global_init_done)
        return;

    glibc_malloc  = dlsym(RTLD_NEXT, "malloc");
    glibc_calloc  = dlsym(RTLD_NEXT, "calloc");
    glibc_realloc = dlsym(RTLD_NEXT, "realloc");
    glibc_free    = dlsym(RTLD_NEXT, "free");
    global_init_done = 1;
    return;
}

__attribute__((constructor)) static void init_so()
{
    __init_mtrace();
    return;
}

void Logout(const char *fmt, ...)
{
    char path[128];
    snprintf(path, sizeof(path), "pid_%d_trace.log", getpid());
    FILE* pfile = fopen(path, "ab+");
    if (pfile == NULL) {return;}

    va_list ap;
    va_start(ap, fmt);
    fprintf(pfile, fmt, ap);
    va_end(ap);
    fclose(pfile);
}

void printBacktrace()
{
    Logout("[backtrace]");
    void *array[16];

    size_t size = backtrace(array, 16);
    char **strings = backtrace_symbols(array, (int)size);
    if (strings == NULL)
    {
        Logout("backtrace symbols null");
        return ;
    }

    size_t i = 0;
    for (i = 0; i<size; ++i)
    {
        Logout(strings[i]);
    }
    free(strings);
}

void *malloc(size_t size)
{
    void *res;

    if (hook_recursive == 1)
        return glibc_malloc(size);

    hook_recursive = 1;
    if (!global_init_done)
        __init_mtrace();

    res = glibc_malloc(size);

    Logout("\n{\"Addr\" : \"%p\", \"type\" : \"alloc\", \"size\" : %u, \"backtrace\": \""
        , res, (unsigned int) size);
    printBacktrace();
    Logout("\"} \n");

    hook_recursive = 0;
    return res;
}

void free(void *ptr)
{
    if (hook_recursive == 1)
        return glibc_free(ptr);

    hook_recursive = 1;

    if (!global_init_done)
        __init_mtrace();
    
    Logout("\n{\"Addr\" : \"%p\", \"type\" : \"free\", \"backtrace\": \"", ptr);

    glibc_free(ptr);
    printBacktrace();
    Logout("\"} \n");
    hook_recursive = 0;
    return;
}