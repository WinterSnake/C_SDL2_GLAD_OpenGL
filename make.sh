mkdir -p bin
gcc src/main.c src/glad.c -o bin/sdl2 -Iinclude/ -lSDL2
gcc -shared -fPIC src/glad.c -o bin/glad.so -Iinclude/
