@mkdir build
@g++ -g -std=c++11 -DGLEW_STATIC -Wwrite-strings^
 -o build/voxel ^
 src/*.cpp ^
 -Ilibs/glm -Ilibs/glew/include -Ilibs/SDL2/i686-w64-mingw32/include ^
 -Llibs/glew/lib/Release/Win32 -Llibs/SDL2/i686-w64-mingw32/lib ^
 -lglew32s -lopengl32 -lmingw32 -lSDL2main -lSDL2