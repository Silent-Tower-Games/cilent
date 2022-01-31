CC=gcc
CFLAGS=-g -pedantic

.PHONY=build
build:
	make lib
	${CC} ${CFLAGS} -c src/main.c -o src/main.o ${INCLUDE_PATHS}
	${CC} ${CFLAGS} src/main.o -o bin/${BUILD}/main -lflecs -lcilent -lsprender -lFNA3D -llua54 -lm `sdl2-config --libs` ${LIBRARY_PATHS} -Wl,-rpath=./libs/

lib:
	${CC} ${CFLAGS} -c src/Cilent/Cilent.c -o src/Cilent/Cilent.o ${INCLUDE_PATHS} -fPIC
	${CC} ${CFLAGS} \
		src/Cilent/Cilent.o \
		-shared -o libcilent.${EXT} \
	${LIBRARY_PATHS}
	cp libcilent.${EXT} bin/${BUILD}/libs/libcilent.${EXT}

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
