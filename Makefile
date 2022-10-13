CC?=gcc
CFLAGS?=-g -pedantic
INCLUDE_PATHS?=${PATH_FLECS_INCLUDE} ${PATH_CILENT_INCLUDE} ${PATH_SOLOUD_INCLUDE} ${PATH_FPSLOOP_INCLUDE} ${PATH_INPUT_INCLUDE} ${PATH_SPRENDER_INCLUDE} ${PATH_FNA3D_INCLUDE} ${PATH_LUA_INCLUDE} ${PATH_SDL2_INCLUDE}
LIBRARY_PATHS?=${PATH_FLECS_LIB} ${PATH_CILENT_LIB} ${PATH_SOLOUD_LIB} ${PATH_FPSLOOP_LIB} ${PATH_INPUT_LIB} ${PATH_SPRENDER_LIB} ${PATH_FNA3D_LIB} ${PATH_LUA_LIB} ${PATH_SDL2_LIB}

.PHONY=build
build: lib
	${CC} ${CFLAGS} -c src/main.c -o src/main.o ${INCLUDE_PATHS}
	${CC} ${CFLAGS} src/main.o -o bin/${PLATFORM}/${BUILD}/main -lflecs -lcilent -lsoloud -lfpsloop -lstginput -lsprender -lFNA3D -llua54 -lm ${LIBRARY_PATHS} -Wl,-rpath=./libs/

lib:
	${CC} ${CFLAGS} -c src/Cilent/Cilent.c -o src/Cilent/Cilent.o ${INCLUDE_PATHS} -fPIC
	${CC} ${CFLAGS} -c src/Cilent/Assets/AssetManager.c -o src/Cilent/Assets/AssetManager.o ${INCLUDE_PATHS} -fPIC
	${CC} ${CFLAGS} -c src/Cilent/Config/Config.c -o src/Cilent/Config/Config.o ${INCLUDE_PATHS} -fPIC
	${CC} ${CFLAGS} -c src/Cilent/Fonts/FontStash.c -o src/Cilent/Fonts/FontStash.o ${INCLUDE_PATHS} -fPIC
	${CC} ${CFLAGS} -c src/Cilent/Lang/Lang.c -o src/Cilent/Lang/Lang.o ${INCLUDE_PATHS} -fPIC
	${CC} ${CFLAGS} -c src/Cilent/Misc/File.c -o src/Cilent/Misc/File.o ${INCLUDE_PATHS} -fPIC
	${CC} ${CFLAGS} -c src/Cilent/Misc/Log.c -o src/Cilent/Misc/Log.o ${INCLUDE_PATHS} -fPIC
	${CC} ${CFLAGS} -c src/Cilent/Mods/Mod.c -o src/Cilent/Mods/Mod.o ${INCLUDE_PATHS} -fPIC
	${CC} ${CFLAGS} -c src/Cilent/Mods/ModState.c -o src/Cilent/Mods/ModState.o ${INCLUDE_PATHS} -fPIC
	${CC} ${CFLAGS} -c src/Cilent/Flecs/Maps.c -o src/Cilent/Flecs/Maps.o ${INCLUDE_PATHS} -fPIC
	${CC} ${CFLAGS} -c src/vendor/ini-master/src/ini.c -o src/vendor/ini-master/src/ini.o -fPIC
	${CC} ${CFLAGS} \
		src/Cilent/Cilent.o \
		src/Cilent/Assets/AssetManager.o \
		src/Cilent/Config/Config.o \
		src/Cilent/Fonts/FontStash.o \
		src/Cilent/Lang/Lang.o \
		src/Cilent/Misc/File.o \
		src/Cilent/Misc/Log.o \
		src/Cilent/Mods/Mod.o \
		src/Cilent/Mods/ModState.o \
		src/Cilent/Flecs/Maps.o \
		src/vendor/ini-master/src/ini.o \
		-shared -o libcilent.${EXT} \
	${LIBRARY_PATHS}
	cp libcilent.${EXT} bin/${PLATFORM}/${BUILD}/libs/libcilent.${EXT}

.PHONY=clean
clean:
	find . -type f -name '*.o' -delete

# TODO: this doesn't work anymore
.PHONY=valgrind
valgrind:
	valgrind --leak-check=full ./bin/linux-gcc-64/Debug/main 2> valgrind.txt

# TODO: this doesn't work anymore
.PHONY=apitrace
apitrace:
	apitrace trace ./main
	qapitrace main.trace
	rm main.trace

.PHONY=shaders
shaders:
	./build-shaders.sh
