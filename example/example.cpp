#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <execinfo.h>
#include <unistd.h>
#include <time.h>
#include <libgen.h>
#include <string.h>

void leak(int num)
{
    printf("%s [malloc] allocating %d bytes...\n", __PRETTY_FUNCTION__, num);
    void * p1 = malloc(num);
    memset(p1, 0, num);

    printf("%s [new] allocating %d bytes...\n", __PRETTY_FUNCTION__, num);
    char * p2 = new char[num];
    memset(p2, 0, num);
    return;
}

void notleak(int num)
{
    printf("%s [malloc] allocating %d bytes...\n", __PRETTY_FUNCTION__, num);
    void * p1 = malloc(num);
    memset(p1, 0, num);
    free(p1);

    printf("%s [new] allocating %d bytes...\n", __PRETTY_FUNCTION__, num);
    char * p2 = new char[num];
    memset(p2, 0, num);
    delete p2;
    return;
}

int main(int argc, char** argv)
{
    srand((unsigned)time(NULL));
    printf("%s: PID: %d\n", __PRETTY_FUNCTION__, getpid());

    while (1) 
    {
        leak(rand() % 1024);
        notleak(rand() % 1024);
        sleep(rand() % 10);
    }
    return 0;
}