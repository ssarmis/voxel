#pragma once

#include "general.h"

typedef struct Vertex {
    r32 x;
    r32 y;
    r32 z;

    r32 r;
    r32 g;
    r32 b;
} Vertex;

static inline Vertex vertexCreateVertex(r32 x, r32 y, r32 z, r32 r, r32 g, r32 b){
    return (Vertex){x, y, z, r, g, b};
}