#pragma once

#include "general.h"
#include "vertex.h"
#include "cube.h"
#include "opengl_layer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define CHUNK_X 32
#define CHUNK_Y 16
#define CHUNK_Z 32
#define CHUNK_SIZE (CHUNK_X * CHUNK_Y * CHUNK_Z)
#define CHUNK_POINTS (CUBE_POINTS * CHUNK_SIZE)
#define CHUNK_INDICES (CUBE_INDICES * CHUNK_SIZE)

typedef struct Chunk {
    glm::vec3 position;
    // Cube cubes; to be added, when this updates, we change the chunk
    u32 numberOfIndices;
    RenderingObject mesh;
} Chunk;

Chunk chunkCreate(glm::vec3 position);