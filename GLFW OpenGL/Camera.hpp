#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera{
    
public:
    Camera(glm::vec3 startPosition, glm::vec3 startup, GLfloat startYaw, GLfloat startPitch, GLfloat startMOveSpeed, GLfloatstartTurnSpeed){
        position = startPosition;
        up = startup;
        yaw = startYaw;
        pitch = startPitch;
        front = glm::vec3(0.0f, 0.0f, -1.0f);
        
        moveSpeed = startMoveSpeed;
        turnSpeed = startTurnSpeed;
    }
    ~Camera();
    
private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldup; //birds eye view, frame of reference
    
    GLfloat yaw;
    GLfloat pitch;
    
};
