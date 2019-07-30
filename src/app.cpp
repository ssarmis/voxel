#include "GL/glew.h"

#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "renderer.h"
#include "opengl_layer.h"
#include "vertex.h"
#include "cube.h"
#include "chunk.h"

#include <stdio.h>
#include <malloc.h>

#define SIZE 4

int main(int argumentCount, char* arguments[]){

    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window *window = SDL_CreateWindow("Voxel",
                                          SDL_WINDOWPOS_UNDEFINED,           
                                          SDL_WINDOWPOS_UNDEFINED,           
                                          1280,                               
                                          768,                               
                                          SDL_WINDOW_OPENGL);

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    int status = SDL_GL_SetSwapInterval(1);
    if(status == -1){
        printf("Could not vsync :(((((\n");
    }
    glewInit();

    bool done = false;

    SDL_Event event;
    glClearColor(0.2, 0, 0.2, 1);

    Chunk chunks[SIZE * SIZE * 2 * SIZE * 2];
    u32 index = 0;
    for(int y = 0; y < SIZE; ++y){
        for(int x = 0; x < SIZE * 2; ++x){
            for(int z = 0; z < SIZE * 2; ++z){
                chunks[index++] = chunkCreate(glm::vec3(x * CHUNK_X * CUBE_SIZE,
                                                        y * CHUNK_Y * CUBE_SIZE,
                                                        z * CHUNK_Z * CUBE_SIZE));
            }
        }
    }

    RenderingProgram program = openglCreateRenderProgram();

    glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);

#if 0
    CubeDetails cube = cubeCreate(glm::vec3(0, 0, 0), 0);
    RenderingObject test;
    VertexData vertexData = openglCreateVertexData(cube.points, 8);
    test = openglCreateRenderingObject(vertexData.numberOfVertecies * sizeof(Vertex),
                                                         36 * sizeof(u16));

    openglBufferVertexData(test, 0, vertexData);
    openglBufferIndicesData(test, 0, cube.indices, 36);
#endif

    openglPerspectiveSetup(program);
    openglCameraSetup(program);

    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    while(!done){
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:{
                        done = !done;
                    }
                    break;
            }
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        openglCameraRotate(program, -0.007, glm::vec3(0, 1, 0));

        for(int i = 0; i < index; ++i){
            openglTranslate(program, chunks[i].position.x - 200 * CUBE_SIZE, chunks[i].position.y - 50 * 1, chunks[i].position.z - 200 * CUBE_SIZE);
            openglRenderObject(chunks[i].mesh, program, chunks[i].numberOfIndices);
            openglClearTransformations(program);
        }

        SDL_GL_SwapWindow(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}