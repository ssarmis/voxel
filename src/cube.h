#pragma once

#include "general.h"
#include "vertex.h"

#include "glm/glm.hpp"

#define CUBE_SIZE 2
#define CUBE_POINTS 8
#define CUBE_INDICES 36

typedef struct CubeDetails {
    Vertex points[8];
    u16 indices[36];
} CubeDetails;

CubeDetails cubeCreate(glm::vec3 position, u32 offset);

