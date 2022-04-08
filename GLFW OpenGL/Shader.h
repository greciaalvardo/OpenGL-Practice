#pragma once

#include <GL/glew.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>


class Shader
{
public:
    Shader();
    
    void CreateFromString(const char* vertexCode, const char* fragmentCode);
    
    GLuint GetProjectionLocation();
    GLuint GetModelLocation();
    
    void UseShader();
    void ClearShader();
    
    ~Shader();
    
private:
    GLuint shaderProgram, uniformProjection, uniformModel;
    
    void CompileShader(const char* vertexShader, const char* fragmentShader);
    void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};
