#include "Mesh.hpp"

Mesh::Mesh(){
    VAO = 0;
    VBO = 0;
    IBO = 0;
    indexCount = 0;
}

void Mesh::CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numVertices, unsigned int numIndices){
    indexCount = numIndices;
    
    glGenVertexArrays( 1, &VAO );
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray( VAO );
    
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0])*numIndices, indices, GL_STATIC_DRAW);
    
    glGenBuffers( 1, &VBO );
    //copy vertices array to buffer for opengl to use
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof(vertices[0])*numVertices, vertices, GL_STATIC_DRAW );
    
    //set vertex attribute pointers
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ), ( GLvoid * ) 0 );
    glEnableVertexAttribArray( 0 );
    
    //copies our array to a buffer that opengl can use
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    
    glBindVertexArray( 0 ); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
}

void Mesh::RenderMesh(){
    glBindVertexArray( VAO );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindVertexArray( 0 );
}

void Mesh::ClearMesh(){
    if(IBO != 0){
        glDeleteBuffers(1, &IBO);
        IBO = 0;
    }
    
    if(VBO != 0){
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }
    
    if(VAO != 0){
        glDeleteBuffers(1, &VAO);
        VAO = 0;
    }
    
    indexCount = 0;
}

Mesh::~Mesh(){
    ClearMesh();
}
