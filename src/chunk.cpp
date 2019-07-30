#include "chunk.h"

#include <stdlib.h>
#include <time.h> //removeme

Chunk chunkCreate(glm::vec3 position){
    Chunk result;

    result.position = position;

    u32 processedVertexies = 0;
    u32 processedIndices = 0;

    Vertex* points = (Vertex*)malloc(CHUNK_POINTS * sizeof(Vertex));
    Index* indices = (Index*)malloc(CHUNK_INDICES * sizeof(Index));

    CubeDetails cube;
    for(int y = 0; y < CHUNK_Y; ++y){
        for(int x = 0; x < CHUNK_X; ++x){
            for(int z = 0; z < CHUNK_Z; ++z){
                // if(rand() % 10 == 0){
                    cube = cubeCreate(glm::vec3(x * CUBE_SIZE + x * 5 * CUBE_SIZE,
                                                y * CUBE_SIZE + y * 5 * CUBE_SIZE,
                                                z * CUBE_SIZE + z * 5 * CUBE_SIZE), processedVertexies);

                    ARRAY_COPY(cube.points, (points + processedVertexies), ARRAY_SIZE(cube.points));
                    processedVertexies += ARRAY_SIZE(cube.points);

                    ARRAY_COPY(cube.indices, (indices + processedIndices), ARRAY_SIZE(cube.indices));
                    processedIndices += ARRAY_SIZE(cube.indices);
                // }s
            }
        }
    }
    // srand(time(NULL));

    VertexData vertexData = openglCreateVertexData(points, processedVertexies);
    result.mesh = openglCreateRenderingObject(vertexData.numberOfVertecies * sizeof(Vertex),
                                                         processedIndices * sizeof(u16));

    openglBufferVertexData(result.mesh, 0, vertexData);
    openglBufferIndicesData(result.mesh, 0, indices, processedIndices);

    result.numberOfIndices = processedIndices;

    free(points);
    free(indices);

    return result;
}