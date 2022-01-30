CC=gcc
CFLAGS=-g -pedantic

.PHONY=build
build:
	${CC} ${CFLAGS} -c src/main.c -o src/main.o ${INCLUDE_PATHS}
	${CC} ${CFLAGS} src/main.o -o bin/${BUILD}/main -lsprender -lFNA3D -llua54 -lm `sdl2-config --libs` ${LIBRARY_PATHS} -Wl,-rpath=./libs/

.PHONY=clean
clean:
	rm -f src/*.o
	rm -f src/**/*.o

.PHONY=valgrind
valgrind:
	valgrind --leak-check=full ./main 2> valgrind.txt

.PHONY=apitrace
apitrace:
	apitrace trace ./main
	qapitrace main.trace
	rm main.trace
