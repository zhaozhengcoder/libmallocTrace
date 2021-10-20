APP_EXE1=libmTrace.so

all:
	g++ -Wall -fPIC -shared -o $(APP_EXE1) mallocTrace.cpp -ldl

clean:
	rm *.so