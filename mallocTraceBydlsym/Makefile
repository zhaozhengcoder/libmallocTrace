APP_EXE=libmTraceBydlsym.so

all:
	gcc -Wall -fPIC -shared -o $(APP_EXE) mallocTraceBydlsym.c -ldl

example:
	g++ -Wall -g example.cpp -o example -rdynamic 

clean:
	rm *.so