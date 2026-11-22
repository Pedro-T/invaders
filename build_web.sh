emcc -o bin/web/index.html src/main.c src/common.c src/player.c src/bunker.c src/aliens.c src/title.c src/projectiles.c \
  -Os -Wall \
  ./build/external/raylib-master/src/libraylib.web.a \
  -I./build/external/raylib-master/src \
  -L./build/external/raylib-master/src \
  -s USE_GLFW=3 \
  -s ASYNCIFY \
  --shell-file ./build/external/raylib-master/src/minshell.html \
  --preload-file resources \
  -DPLATFORM_WEB