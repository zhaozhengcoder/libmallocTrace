APP_EXE1=libmTrace.so
APP_EXE2=libmTrace2.so

lib2:
	g++ -Wall -fPIC -shared -o $(APP_EXE2) mallocTrace2.cpp -ldl

all:
	g++ -Wall -fPIC -shared -o $(APP_EXE1) mallocTrace.cpp -ldl

clean:
	rm *.so