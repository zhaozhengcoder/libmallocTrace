APP_EXE=libmTrace.so

all:
	g++ -Wall -fPIC -shared -o $(APP_EXE) mallocTrace.cpp -ldl

clean:
	rm $(APP_EXE)