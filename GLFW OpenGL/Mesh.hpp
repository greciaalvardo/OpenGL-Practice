#pragma once
#include <GL/glew.h>
//#include <Mesh/Mesh.cpp>

class Mesh
{
public:
    Mesh();
    
    void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numVertices, unsigned int numIndices); //create mesh
    void RenderMesh(); //draw mesh
    void ClearMesh(); //delete mesh from graphics card
    ~Mesh();
    
private:
    //vao -- vertex array object
    //vbo -- vertex buffer object
    //ibo -- index buffer object
    // buffer -- portion of memory
    GLuint VAO, VBO, IBO;
    GLsizei indexCount;
};
