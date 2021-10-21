#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <malloc.h>
#include <execinfo.h>
#include <libgen.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>

static void myInitHook(void);
static void *myMallocHook(size_t, const void*);
static void  myFreeHook(void*, const void*);

static void *(*old_malloc_hook)(size_t, const void *);
static void  (*old_free_hook)(void*, const void *);

void (*__malloc_initialize_hook) (void) = myInitHook;

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

__attribute__((constructor)) static void init_so()
{
    // Logout("init so ...\n");
    return;
}

static void myInitHook(void)
{
    // Logout("setting up hooks...\n");
    old_malloc_hook = __malloc_hook;
    old_free_hook   = __free_hook;
    __malloc_hook   = myMallocHook;
    __free_hook     = myFreeHook;
}
 
static void restoreOldHooks()
{
    __malloc_hook = old_malloc_hook;
    __free_hook = old_free_hook;
}
 
static void restoreMyHooks()
{
    __malloc_hook = myMallocHook;
    __free_hook = myFreeHook;
}
 
static void saveOldHooks()
{
    old_malloc_hook = __malloc_hook;
    old_free_hook = __free_hook;
}

void printBacktrace()
{
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

static void* myMallocHook(size_t size, const void* caller)
{
    void* res;
    restoreOldHooks();
    res = malloc(size);
    saveOldHooks();
 
    // Do your memory statistics here...
    Logout("\n{\"Addr\" : \"%p\", \"type\" : \"alloc\", \"size\" : %u, \"backtrace\": \""
        , res, (unsigned int) size);
    printBacktrace();
    Logout("\"} \n");

    // Restore our own hooks
    restoreMyHooks();
    return res;
}
 
static void myFreeHook(void* ptr, const void* caller)
{
    restoreOldHooks();
    free(ptr);
    saveOldHooks();
 
    // Do your memory statistics here...
    Logout("\n{\"Addr\" : \"%p\", \"type\" : \"free\", \"backtrace\": \"", ptr);
    printBacktrace();
    Logout("\"} \n");
    // Restore our own hooks
    restoreMyHooks();
}