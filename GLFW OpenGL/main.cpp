#include <iostream>
#include <cmath>
#include <vector>
#include "Mesh.hpp"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159265f / 180.0f;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

//move the triangle!
//GLuint uniformXMove, uniformProjection;
bool direction = true;
float triOffset = 0.0f;
//will keep moving until it hits 0.7 and will go other direction
float triMaxoffset = 0.5f;
//speed
float triIncrement = 0.005f;

float currAngle = 0.0f;
bool sizeDirection = true;
float currSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;


// From top to bottom:
// shader version
// layout specifies location of variable it seems -- go directly to glEnableVertexAttribArray(0) -- worry ab this later
const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"                                       \n"
"out vec4 vColor;                       \n" //out here
"                                       \n"
" uniform mat4 xMove;                   \n"
" uniform mat4 projection;              \n"
"void main()                            \n"
"{                                      \n"
//can have 2 param bc "vec3 position" gives 3 values
"   gl_Position = projection * xMove * vec4(position, 1.0);  \n"
//clamp is to correct negatives
"   vColor = vec4(clamp(position, 0.0f, 1.0f), 1.0f); \n"
"}";
const GLchar* fragmentShaderSource = "#version 330 core\n"
" in vec4 vColor;                                       \n" // in here
"out vec4 color;\n"
"void main()\n"
"{                                                      \n"
"   color = vColor;                                         \n"
"}";

void CreateShaders(){
    Shader *shader1 = new Shader();
    shader1->CreateFromString(vertexShaderSource,fragmentShaderSource);
    shaderList.push_back(*shader1);
}

int main()
{
    // Init GLFW
    glfwInit( );
    
    // required stuff for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );

    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    // create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr );
    
    int screenWidth, screenHeight;
    glfwGetFramebufferSize( window, &screenWidth, &screenHeight );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent( window );
    
    // true so glew knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // initialize glew to setup the opengl function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    //need this because the pyramid was kind of redrawing itself
    glEnable(GL_DEPTH_TEST);
    
    // Define the viewport dimensions
    glViewport( 0, 0, screenWidth, screenHeight );
    
    CreateShaders();
    
    GLuint uniformProjection = 0,
    uniformModel = 0;
    
    glm::mat4 projection(1.0f);
    projection = glm::perspective(45.0f, (GLfloat)screenWidth/(GLfloat)screenHeight, 0.1f, 100.0f);
    
    
    // Build and compile our shader program
    // Vertex shader
    //glCreateShader takes the type of shader you want to create
    GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
    //glShaderSource attaches shader source code to shaderobject
    //@param shaderobject -- object we're compiling
    //@param #ofstrings -- how many strings we're passing as source code
    //@param actual source code of vertex shader -- the shader with the version #
    //@ last param -- not too sure :/
    glShaderSource( vertexShader, 1, &vertexShaderSource, NULL );
    glCompileShader( vertexShader );
    
    // Check for compile time errors
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
    glShaderSource( fragmentShader, 1, &fragmentShaderSource, NULL );
    glCompileShader( fragmentShader );
    
    // Check for compile time errors
    glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &success );
    
    if ( !success )
    {
        glGetShaderInfoLog( fragmentShader, 512, NULL, infoLog );
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // To use the compiled shaders you have to link them to a shader program object!!
    //glCreateProgram creates program
    //@return ID reference to the newly created program object
  //  GLuint shaderProgram = glCreateProgram( );
    //next step -- attach the shaders to the program object
   // glAttachShader( shaderProgram, vertexShader ); //...attaching vertex shader
   // glAttachShader( shaderProgram, fragmentShader ); //...attaching fragment shader
    //link it all together!! :)
    //glLinkProgram( shaderProgram );
    
    // Check above for linking errors and get error log if failed
   // glGetProgramiv( shaderProgram, GL_LINK_STATUS, &success );
    
    /**if ( !success )
    {
        glGetProgramInfoLog( shaderProgram, 512, NULL, infoLog );
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    //@param program
    //@param name -- name of the uniform variable
    uniformXMove = glGetUniformLocation(shaderProgram, "xMove");
    uniformProjection = glGetUniformLocation(shaderProgram, "projection"); */
    
    glDeleteShader( vertexShader );
    glDeleteShader( fragmentShader );
    
    
    // Set up vertex data (and buffer(s)) and attribute pointers
    //vertex input is in X,Y,Z format!! :D
    //range is btwn -1.0 & 1.0
    //this input will be sent to vertex shader (which is the first process of the graphics pipeline)
    
    //order to draw
    unsigned int indices[]{
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2,
    };
    GLfloat vertices[] =
    {
        -1.0f, -1.0f, 0.0f, // bottom left
        0.0f, -1.0f, 1.0f, // 3d
        1.0f, -1.0f, 0.0f, // bottom right
        0.0f,  1.0f, 0.0f  // top
    };
    
    Mesh *obj1 = new Mesh();
    obj1 -> CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(obj1);
    
    // Game loop
    while ( !glfwWindowShouldClose( window ) )
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents( );
        
        if(direction){
            triOffset += triIncrement;
        }else{
            triOffset -= triIncrement;
        }
        
        if(abs(triOffset) >= triMaxoffset){
            direction = !direction;
        }
        
        currAngle += 0.001f;
        if(currAngle >= 360){
            //just to avoid overflow
            currAngle -= 360;
        }
        
        if(sizeDirection){
            currSize += 0.05f;
        } else{
            currSize -= 0.05f;
        }
        
        //if size reaches min or max then make it either shrink or get bigger
        if(currSize >= maxSize || currSize <= minSize){
            sizeDirection = !sizeDirection;
        }
        
        // Render
        // Clear the colorbuffer
        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
        //clearing both buffers
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Activate the program!
        //glUseProgram( shaderProgram );
        shaderList[0].UseShader();
        uniformModel = shaderList[0].GetModelLocation();
        uniformProjection = shaderList[0].GetProjectionLocation();
        
        //mat4 -- matrix of 4x4
        glm::mat4 model(1.0f);
        
        // TRANSFORMATIONS --- Order matters!! //
        
        //top right value of matrix will be changed to triOffset
        //vec3(sideways, up & down, z)
        model = glm::translate(model, glm::vec3(0.0f, triOffset,-2.5f));
        // last param is axis of rotation
        //warps a little bit??
       // model = glm::rotate(model, currAngle, glm::vec3(0.0f, 1.0f, 0.0f));
        
        //make vertex shader simpler
        //currSize is changing!
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
        
        //glUniform1f(uniformXMove, triOffset);
       // glUniformMatrix4fv(uniformXMove, 1, GL_FALSE, glm::value_ptr(model));
       // glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        
        meshList[0] -> RenderMesh();
        
        // Swap the screen buffers
        glfwSwapBuffers( window );
    }
    
    // de-allocate resources
    //glDeleteVertexArrays( 1, &VAO );
    //glDeleteBuffers( 1, &VBO );
    
    // terminate glfw and clear resources
    glfwTerminate( );
    
    return EXIT_SUCCESS;
}
