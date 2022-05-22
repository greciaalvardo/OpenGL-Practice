#pragma once

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
    Window();

    Window(GLint windowWidth, GLint windowHeight);

    int Initialise();

    GLint getBufferWidth() { return bufferWidth; }
    GLint getBufferHeight() { return bufferHeight; }

    bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }
    
    bool* getsKeys() {return keys; }
    GLfloat getXChange();
    GLfloat getYChange();

    void swapBuffers() { glfwSwapBuffers(mainWindow); }

    ~Window();

private:
    GLFWwindow* mainWindow;

    GLint width, height;
    GLint bufferWidth, bufferHeight;
    
    //get key and mouse movements
    //1024 covers range of ascii characters -- so we can do is this number pressed
    bool keys[1024];
    
    //compare last corrdinates to current coordinates to compare how much it's changed
    GLfloat lastX;
    GLfloat lastY;
    GLfloat xChange;
    GLfloat yChange;
    //take into account initial movement
    bool mouseFirstMoved;
    
    void createCallbacks();
    static void handleKeys(GLFWwindow* window, int key, int action, int mode);
    static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};
