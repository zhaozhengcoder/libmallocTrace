# libmallocTrace
A tool for tracking memory allocation based ld-preload

* how to build
    ```
    make
    cd example && make
    ```

* how to use

    a simple way is to execute something like below : 

    LD_PRELOAD=/you_path/libmTrace.so /you_path/application

    such as:
    ```
    cd example && make
    LD_PRELOAD=../libmTrace.so ./example
    ```

    Run example generate trace log. You can see appliaction invoke malloc/free/new/delete backtrace.
    ```
    ... 

    malloc (247) returned @0x418590
    [backtrace]
    ../libmTrace.so(_Z14printBacktracev+0x2b) [0x7ffff7fd9370]
    ../libmTrace.so(+0x146c) [0x7ffff7fd946c]
    ./example(_Z7notleaki+0x31) [0x401287]
    ./example(main+0x7f) [0x401382]
    /lib64/libc.so.6(__libc_start_main+0xf3) [0x7ffff71066a3]
    ./example(_start+0x2e) [0x40110e]


    freed pointer @0x418590
    [backtrace]
    ../libmTrace.so(_Z14printBacktracev+0x2b) [0x7ffff7fd9370]
    ../libmTrace.so(+0x14d8) [0x7ffff7fd94d8]
    ./example(_Z7notleaki+0x58) [0x4012ae]
    ./example(main+0x7f) [0x401382]
    /lib64/libc.so.6(__libc_start_main+0xf3) [0x7ffff71066a3]
    ./example(_start+0x2e) [0x40110e]

    ...

    ```