#pragma once

#include "GL/glew.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "general.h"
#include "vertex.h"

typedef GLuint BufferId;
typedef GLuint ShaderId;
typedef GLuint ProgramId;
typedef GLint UniformLocation;
typedef u16 Index;

typedef struct RenderingObject {
    BufferId vertexBufferArrayId;
    BufferId vertexBufferId;
    BufferId indicesBufferId;
} RenderingObject; 

typedef struct VertexData {
    u32 numberOfVertecies;
    Vertex *data;
} VertexData;

typedef struct RenderingProgram {
    ProgramId id;
    ShaderId vertexShaderId;
    ShaderId fragmentShaderId;

    UniformLocation perspectiveMatrix;
    UniformLocation transformMatrix;
    UniformLocation viewMatrix;
} RenderingProgram;

static inline VertexData openglCreateVertexData(Vertex* data, u32 amount){
    return (VertexData){amount, data};
}

// TODO some functions are void because the app will crash if any error occurs in the first place
//      no need to return a status or whatever

// TODO add better error handling, maybe I will output some error message, whatever
void openglUseVertexArray(BufferId bufferId);

void openglUnuseVertexArray();

void openglUseBuffer(BufferId bufferId, GLenum type);

void openglUnuseBuffer(GLenum type);

RenderingObject openglCreateRenderingObject(u32 vertexBufferSize, u32 indicesBufferSize);

void openglBufferIndicesData(RenderingObject& renderingObject, u32 offset, u16* data, u32 numberOfIndices);
void openglBufferVertexData(RenderingObject& renderingObject, u32 offset, VertexData& data);

RenderingProgram openglCreateRenderProgram();

void openglUseRenderProgram(RenderingObject& program);
void openglUnuseRenderProgram();

void openglRenderObject(RenderingObject& object, RenderingProgram& program, u32 numberOfIndices);

// TODO maybe the rendering program should be global, so no need to pass it around
void openglPerspectiveSetup(RenderingProgram& program);

void openglTranslate(RenderingProgram& program, r32 x, r32 y, r32 z);
void openglRotation(RenderingProgram& program, r32 angle, glm::vec3 axis);
void openglClearTransformations(RenderingProgram& program);

void openglCameraSetup(RenderingProgram& program);
void openglCameraTranslate(RenderingProgram& program, glm::vec3 translation);
void openglCameraRotate(RenderingProgram& program, r32 angle, glm::vec3 axis);