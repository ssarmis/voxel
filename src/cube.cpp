#include "cube.h"

// TODO maybe put just points not vertexies
static Vertex cubePoints[] = {
    // front face
    vertexCreateVertex(-1, 1, -1, 0, 0, 0), // 0
    vertexCreateVertex(1, 1, -1, 1, 0, 0), // 1
    vertexCreateVertex(1, -1, -1, 0, 1, 0), // 2
    vertexCreateVertex(-1, -1, -1, 0, 0, 1), // 3

    // left face
    // 1 goes here
    vertexCreateVertex(1, 1, 1, 1, 0, 1), // 4
    vertexCreateVertex(1, -1, 1, 1, 1, 0), // 5
    // 2 goes here

    // right face
    // 0 goes here
    vertexCreateVertex(-1, 1, 1, 0, 1, 1), // 6
    vertexCreateVertex(-1, -1, 1, 1, 1, 1), // 7
    // 3 goes here        

    // back face
    // 6 goes here
    // 4 goes here
    // 5 goes here
    // 7 goes here

    // top face
    // 6 goes here
    // 4 goes here
    // 1 goes here
    // 0 goes here

    // bottom face
    // 7 goes here
    // 5 goes here
    // 2 goes here
    // 3 goes here
};

static u16 cubeIndices[] = {
    0, 1, 2, // front face
    0, 2, 3,

    1, 4, 5, // left face
    1, 5, 2,

    0, 6, 7, // right face
    0, 7, 3,    

    6, 4, 5, // back face
    6, 5, 7,

    6, 4, 1, // top face
    6, 1, 0,

    7, 5, 2, // bottom face
    7, 2, 3
};

CubeDetails cubeCreate(glm::vec3 position, u32 offset){
    CubeDetails result;

    ARRAY_COPY(cubePoints, result.points, ARRAY_SIZE(cubePoints));
    ARRAY_COPY(cubeIndices, result.indices, ARRAY_SIZE(cubeIndices));

    // TODO use vectors, so I don't write 3 line for something so trivial
    for(int i = 0; i < ARRAY_SIZE(result.points); ++i){
        result.points[i].x += position.x;
        result.points[i].y += position.y;
        result.points[i].z += position.z;
    }

    for(int i = 0; i < ARRAY_SIZE(result.indices); ++i){
        result.indices[i] += offset;
    }

    return result;
}