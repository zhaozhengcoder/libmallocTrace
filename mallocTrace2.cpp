#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <malloc.h>
#include <execinfo.h>
#include <libgen.h>
#include <string.h>
#include <errno.h>

void Logout(const char* pszLog)
{
    if (NULL == pszLog) {return;}

    char path[128];
    snprintf(path, sizeof(path), "pid_%d_trace.log", getpid());
    FILE* pfile = fopen(path, "ab+");
    if (pfile)
    {
        fprintf(pfile, pszLog);
        fprintf(pfile, "\n");
        fclose(pfile);
    }
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
    Logout("\n");
}


// g++ -Wall -fPIC -shared -o libmTrace2.so mallocTrace2.cpp -ldl