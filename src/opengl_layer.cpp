#include "opengl_layer.h"

#include <assert.h>

static glm::mat4 camera;
static glm::mat4 transform;

static inline BufferId openglCreateVertexArray(u32 amount){
    assert(amount);

    BufferId result;
    glGenVertexArrays(amount, &result);

    assert(glGetError() == GL_NO_ERROR);

    return result;
}

static inline BufferId openglCreateBuffer(u32 amount){
    assert(amount);

    BufferId result;
    glGenBuffers(amount, &result);

    assert(glGetError() == GL_NO_ERROR);

    return result;
}

void openglUseVertexArray(BufferId bufferId){
    glBindVertexArray(bufferId);

    // assert(glGetError() == GL_NO_ERROR); Curious about this one
}

void openglUnuseVertexArray(){
    glBindVertexArray(0);

    // assert(glGetError() == GL_NO_ERROR);
}

void openglUseBuffer(BufferId bufferId, GLenum type){
    glBindBuffer(type, bufferId);

    // assert(glGetError() == GL_NO_ERROR);
}

void openglUnuseBuffer(GLenum type){
    glBindBuffer(type, 0);

    // assert(glGetError() == GL_NO_ERROR);
}

static inline void openglAllocateBufferSize(BufferId bufferId, u32 size, GLenum type){
    assert(size);

    openglUseBuffer(bufferId, type);
    glBufferData(type, size, NULL, GL_DYNAMIC_DRAW);
    openglUnuseBuffer(type);

    // assert(glGetError() == GL_NO_ERROR);
}

RenderingObject openglCreateRenderingObject(u32 vertexBufferSize, u32 indicesBufferSize){
    RenderingObject result;

    result.vertexBufferArrayId = openglCreateVertexArray(1);
    result.vertexBufferId = openglCreateBuffer(1);
    result.indicesBufferId = openglCreateBuffer(1);

    openglUseVertexArray(result.vertexBufferArrayId);

    openglAllocateBufferSize(result.vertexBufferId, vertexBufferSize, GL_ARRAY_BUFFER);
    openglAllocateBufferSize(result.indicesBufferId, indicesBufferSize, GL_ELEMENT_ARRAY_BUFFER);

    openglUseBuffer(result.vertexBufferId, GL_ARRAY_BUFFER);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (GLvoid*)NULL);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (GLvoid*)(3 * sizeof(r32)));
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    openglUnuseBuffer(GL_ARRAY_BUFFER);

    openglUnuseVertexArray();

    return result;
}

void openglBufferIndicesData(RenderingObject& renderingObject, u32 offset, u16* data, u32 numberOfIndices){
    glNamedBufferSubData(renderingObject.indicesBufferId, offset, numberOfIndices * sizeof(u16), (void*)data);
}

void openglBufferVertexData(RenderingObject& renderingObject, u32 offset, VertexData& data){
    glNamedBufferSubData(renderingObject.vertexBufferId, offset, data.numberOfVertecies * sizeof(Vertex), (void*)data.data);
}

#include <stdio.h> // removeme

RenderingProgram openglCreateRenderProgram(){
    RenderingProgram result;

    result.vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    result.fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    
    char* vertexShader = R"(
        #version 440 core
        layout(location = 0) in vec3 vertexPosition;
        layout(location = 1) in vec3 vertexColor;

        uniform mat4 perspectiveMat;
        uniform mat4 transformMat;
        uniform mat4 viewMat;

        out vec3 outPosition;
        out flat vec3 outColor;

        void main(){
            gl_Position = perspectiveMat * viewMat * transformMat * vec4(vertexPosition, 1.0);
            outPosition = vertexPosition;
            outColor = vertexColor;
        }
    )";

    char *fragmentShader = R"(
        #version 440 core

        in vec3 outPosition;
        in flat vec3 outColor;

        void main(){
            gl_FragColor = vec4(outColor, 1.0);
        }
    )";

    glShaderSource(result.vertexShaderId, 1, &vertexShader, NULL);
    glCompileShader(result.vertexShaderId);

    // TODO make the shader reports nicer

    GLint shaderStatus;
    glGetShaderiv(result.vertexShaderId, GL_COMPILE_STATUS, &shaderStatus);
    if (shaderStatus != GL_TRUE){
        GLsizei logLength = 0;
        GLchar message[1024];
        glGetShaderInfoLog(result.vertexShaderId, 1024, &logLength, message);
        message[logLength] = 0;
        printf("%s\n", message);
    }

    glShaderSource(result.fragmentShaderId, 1, &fragmentShader, NULL);
    glCompileShader(result.fragmentShaderId);

    glGetShaderiv(result.fragmentShaderId, GL_COMPILE_STATUS, &shaderStatus);
    if (shaderStatus != GL_TRUE){
        GLsizei logLength = 0;
        GLchar message[1024];
        glGetShaderInfoLog(result.fragmentShaderId, 1024, &logLength, message);
        message[logLength] = 0;
        printf("%s\n", message);
    }

    result.id = glCreateProgram();

    glAttachShader(result.id, result.vertexShaderId);
    glAttachShader(result.id, result.fragmentShaderId);
    glLinkProgram(result.id);

    GLint programLinked;
    glGetProgramiv(result.id, GL_LINK_STATUS, &programLinked);
    if (programLinked != GL_TRUE){
        GLsizei logLength = 0;
        GLchar message[1024];
        glGetProgramInfoLog(result.id, 1024, &logLength, message);
        printf("%*.s\n", logLength, message);
    }


    // crash at any problem
    result.perspectiveMatrix = glGetUniformLocation(result.id, "perspectiveMat");
    assert(result.perspectiveMatrix != -1);

    result.transformMatrix = glGetUniformLocation(result.id, "transformMat");
    assert(result.transformMatrix != -1);

    result.viewMatrix = glGetUniformLocation(result.id, "viewMat");
    assert(result.viewMatrix != -1);

    return result;
}

void openglUseRenderProgram(RenderingProgram& program){
    glUseProgram(program.id);
}

void openglUnuseRenderProgram(){
    glUseProgram(0);
}

void openglRenderObject(RenderingObject& object, RenderingProgram& program, u32 numberOfIndices){
    openglUseVertexArray(object.vertexBufferArrayId);
    openglUseBuffer(object.indicesBufferId, GL_ELEMENT_ARRAY_BUFFER);
    openglUseRenderProgram(program);

    glDrawElements(GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_SHORT, NULL);
    
    openglUnuseVertexArray();
    
    openglUnuseRenderProgram();
}

void openglPerspectiveSetup(RenderingProgram& program){
    openglUseRenderProgram(program);

    glm::mat4 perspective = glm::perspective<r32>(glm::radians(80.0), 1.66, 0.01, 100000.0);
    glm::mat4 identity = glm::mat4(1.0);
    
    glUniformMatrix4fv(program.perspectiveMatrix, 1, false, &perspective[0][0]);
    // maybe setup the camera here ?
    glUniformMatrix4fv(program.transformMatrix, 1, false, &identity[0][0]);
    glUniformMatrix4fv(program.viewMatrix, 1, false, &identity[0][0]);
    
    openglUnuseRenderProgram();
}

void openglTranslate(RenderingProgram& program, r32 x, r32 y, r32 z){
    openglUseRenderProgram(program);

    transform = glm::translate<r32>(glm::mat4(1.0), glm::vec3(x, y, z));

    glUniformMatrix4fv(program.transformMatrix, 1, false, &transform[0][0]);
    
    openglUnuseRenderProgram();
}

void openglRotation(RenderingProgram& program, r32 angle, glm::vec3 axis){
    openglUseRenderProgram(program);

    transform = glm::rotate(transform, angle, axis);

    glUniformMatrix4fv(program.transformMatrix, 1, false, &transform[0][0]);
    
    openglUnuseRenderProgram();
}

void openglClearTransformations(RenderingProgram& program){
    openglUseRenderProgram(program);

    transform = glm::mat4(1.0);

    glUniformMatrix4fv(program.transformMatrix, 1, false, &transform[0][0]);
    
    openglUnuseRenderProgram();
}

void openglCameraSetup(RenderingProgram& program){
    openglUseRenderProgram(program);

    camera = glm::lookAt<r32>(glm::vec3(0, 0, -1),
                              glm::vec3(0, 0, 0),
                              glm::vec3(0, 1, 0));

    glUniformMatrix4fv(program.viewMatrix, 1, false, &camera[0][0]);
    
    openglUnuseRenderProgram();
}

void openglCameraRotate(RenderingProgram& program, r32 angle, glm::vec3 axis){
    openglUseRenderProgram(program);

    camera = glm::rotate(camera, angle, axis);

    glUniformMatrix4fv(program.viewMatrix, 1, false, &camera[0][0]);
    
    openglUnuseRenderProgram();
}

void openglCameraTranslate(RenderingProgram& program, glm::vec3 translation){
    openglUseRenderProgram(program);

    camera = glm::translate(camera, translation);

    glUniformMatrix4fv(program.viewMatrix, 1, false, &camera[0][0]);
    
    openglUnuseRenderProgram();
}