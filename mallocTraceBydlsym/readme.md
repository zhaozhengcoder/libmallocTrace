# libmallocTrace

add example by use dlsym to hook malloc

such as :
```
glibc_malloc  = dlsym(RTLD_NEXT, "malloc");
```