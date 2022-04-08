#include "Shader.h"

Shader::Shader()
{
    shaderProgram = 0;
    uniformModel = 0;
    uniformProjection = 0;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode){
    CompileShader(vertexCode, fragmentCode);
}

void Shader::CompileShader(const char* vertexShader, const char* fragmentShader){
    
    //glCreateProgram creates program
    //@return ID reference to the newly created program object
    shaderProgram = glCreateProgram();
    
    AddShader(shaderProgram, vertexShader, GL_VERTEX_SHADER);
    AddShader(shaderProgram, vertexShader, GL_FRAGMENT_SHADER);
    
    //next step -- attach the shaders to the program object
    //glAttachShader(shaderProgram, vertexShader ); //...attaching vertex shader
    
    //glAttachShader( shaderProgram, fragmentShader ); //...attaching fragment shader
    
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    
    //link it all together!! :)
    glLinkProgram(shaderProgram);
    // Check above for linking errors and get error log if failed
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog );
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    //@param program
    //@param name -- name of the uniform variable
    uniformModel = glGetUniformLocation(shaderProgram, "model");
    uniformProjection = glGetUniformLocation(shaderProgram, "projection");
}

GLuint Shader::GetProjectionLocation(){
    return uniformProjection;
}
GLuint Shader::GetModelLocation(){
    return uniformModel;
}

void Shader::UseShader(){
    glUseProgram(shaderProgram);
}

void Shader::ClearShader(){
    //delete off of graphics card
    if(shaderProgram != 0){
        glDeleteProgram(shaderProgram);
        shaderProgram =0;
    }
}

void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType){
    
    //GLuint shaderProgram = glCreateProgram( );
    
    const GLchar* theCode[1];
    theCode[0] = shaderCode;
    // Build and compile our shader program
    // Vertex shader
    //glCreateShader takes the type of shader you want to create
    GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
    //glShaderSource attaches shader source code to shaderobject
    //@param shaderobject -- object we're compiling
    //@param #ofstrings -- how many strings we're passing as source code
    //@param actual source code of vertex shader -- the shader with the version #
    //@ last param -- not too sure :/
    glShaderSource( vertexShader, 1, theCode, NULL );
    glCompileShader( vertexShader );
    
    GLint success;
    GLchar infoLog[512];
    
    glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &success );
    if ( !success )
    {
        glGetShaderInfoLog( vertexShader, 512, NULL, infoLog );
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // Fragment shader
    GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource(fragmentShader, 1, theCode, NULL );
    glCompileShader(fragmentShader );
    
    // Check for compile time errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success );
    
    if ( !success )
    {
        glGetShaderInfoLog( fragmentShader, 512, NULL, infoLog );
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

Shader::~Shader(){
    ClearShader();
}
