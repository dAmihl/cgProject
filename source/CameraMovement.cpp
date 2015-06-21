#define GLM_FORCE_RADIANS  /* Use radians in all GLM functions */

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"

/* OpenGL includes */
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "CameraMovement.h"
#include "DeltaTime.h"

#include <stdio.h>


    int CAMERA_FREE_MOVE = 1;
    int CAMERA_FIXED_MOVE = 0;
    int cameraMode = 1; // current camera mode

    /*
     Handles for the Mouse Input
     */
    int MOUSE_OLD_X_POS = 0;
    int MOUSE_OLD_Y_POS = 0;

    int mouseDeltaX = 0;
    int mouseDeltaY = 0;


    /* Variables for storing current rotation angles */
    double angleX, angleY, angleZ = 0.0f; 

    /* Indices to active rotation axes */
    
    int axis = Yaxis;


/*
 Handles for the WASD Movement
 */
GLboolean camMoveForward = GL_FALSE;
GLboolean camMoveBack = GL_FALSE;
GLboolean camMoveLeft = GL_FALSE;
GLboolean camMoveRight = GL_FALSE;

/* Transformation matrices for camera rotation */
glm::mat4 TranslationMatrixCameraX;
glm::mat4 TranslationMatrixCameraY;
glm::mat4 TranslationMatrixCameraZ;
glm::mat4 TranslationMatrixCamera;

glm::mat4 RotationMatrixCameraX;
glm::mat4 RotationMatrixCameraY;
glm::mat4 RotationMatrixCameraZ;
glm::mat4 RotationMatrixCamera;


glm::vec3 getCurrentCameraPosition(glm::mat4 ViewMatrix){
    
    // In order to get the current Camera Position, the bottom row of the inverse of the viewmatrix is needed
    glm::mat4 ViewInverse = glm::inverse(glm::mat4(ViewMatrix));
    glm::vec3 cameraPos = glm::vec3(ViewInverse[3].x, ViewInverse[3].y, ViewInverse[3].z);
    fprintf(stderr, "CurrentCameraPosition: %f %f %f\n", cameraPos.x, cameraPos.y, cameraPos.z);
    return cameraPos;
}

glm::vec3 getCurrentCameraLookAt(glm::mat4 ViewMatrix){
        glm::mat4 tmpView = glm::mat4(ViewMatrix);
    // In order to get the current Camera look at vector, the third row of the inverse of the viewmatrix is needed
    glm::vec3 cameraLookAt = glm::vec3(tmpView[0][2], tmpView[1][2], tmpView[2][2]);
    fprintf(stderr, "CurrentCameraLookat: %f %f %f\n", cameraLookAt.x, cameraLookAt.y, cameraLookAt.z);
    return cameraLookAt;
}


/****************************************************************
 
 Camera Movement
 * Handles the user input and moves the camera
 
 ****************************************************************/

glm::mat4 CameraFreeMove(glm::mat4 ViewMatrix){
	//glm::mat4 TranslationMatrixMouse;
	        
        glm::vec3 CurrentCameraPosition = getCurrentCameraPosition(ViewMatrix);
        glm::vec3 cameraLookAt = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        
        float camMoveSpeed = 0.5f;
        glm::vec3 cameraMovement = glm::vec3(0.0f);
        
        if (camMoveForward == GL_TRUE){
            cameraMovement -= camMoveSpeed * cameraLookAt;
        }
        
        if (camMoveBack == GL_TRUE){
            cameraMovement += camMoveSpeed * cameraLookAt;
        }
        
        if (camMoveLeft == GL_TRUE){
            cameraMovement += glm::normalize(glm::cross(cameraLookAt, cameraUp)) * camMoveSpeed;
        }
        
        if (camMoveRight == GL_TRUE){
            cameraMovement -= glm::normalize(glm::cross(cameraLookAt, cameraUp)) * camMoveSpeed;
        }
        
        glm::mat4 TranslationMatrixMouse = glm::translate(glm::mat4(1.0f), glm::vec3(cameraMovement.x, cameraMovement.y, cameraMovement.z));
        
        /* Set viewing transform */  
        ViewMatrix = glm::lookAt(CurrentCameraPosition,    /* Eye vector */
			     cameraLookAt,     /* Viewing center */
			     cameraUp /* Up vector */
                );  
        
        ViewMatrix = TranslationMatrixMouse * ViewMatrix;
        
        
                
        mouseDeltaY = 0;
        mouseDeltaX = 0;
        return ViewMatrix;
}

glm::mat4 CameraFixedMove(glm::mat4 ViewMatrix){
    float translationCameraX = 0.0f, translationCameraY = 0.0f, translationCameraZ = 0.0f;
        TranslationMatrixCamera = glm::mat4(1.0f);
    /* Increment rotation angles and update matrix */
        if(axis == Xaxis)
	{
  	    angleX = fmod(0.15 * deltaTime, 360.0);
            double angleRad = (2 * M_PI / 360) * angleX;
            translationCameraY =  -cos(angleRad) / 1.5f ;
            RotationMatrixCamera = glm::rotate(glm::mat4(1.0f), (float)angleRad, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	else if(axis == Yaxis)
	{
	    angleY = fmod( 0.15 * deltaTime, 360.0); 
            double angleRad = (2 * M_PI / 360) * angleY;
            translationCameraX =  cos(angleRad) /1.5f;
            RotationMatrixCamera = glm::rotate(glm::mat4(1.0f), (float)angleRad, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else if(axis == Zaxis)
	{			
	    angleZ = fmod(0.15 * deltaTime, 360.0);
            double angleRad = (2 * M_PI / 360) * angleZ;
            RotationMatrixCamera = glm::rotate(glm::mat4(1.0f), (float)angleRad, glm::vec3(0.0f, 0.0f, 1.0f));
	}
      

        TranslationMatrixCamera = glm::translate(glm::mat4(1.0f), glm::vec3(translationCameraX, translationCameraY, translationCameraZ));
        ViewMatrix = TranslationMatrixCamera * RotationMatrixCamera * ViewMatrix;    
        
        return ViewMatrix;
}

void moveForward(GLboolean flag){
    if (cameraMode == CAMERA_FREE_MOVE) camMoveForward = flag;
}

void moveBack(GLboolean flag){
    if (cameraMode == CAMERA_FREE_MOVE) camMoveBack = flag;
}

void moveLeft(GLboolean flag){
    if (cameraMode == CAMERA_FREE_MOVE) camMoveLeft = flag;
}

void moveRight(GLboolean flag){
    if (cameraMode == CAMERA_FREE_MOVE) camMoveRight = flag;
}

void MouseMoveUpdate(int x, int y){
    mouseDeltaX = x - MOUSE_OLD_X_POS;
    mouseDeltaY = y - MOUSE_OLD_Y_POS;
    
    MOUSE_OLD_X_POS = x;
    MOUSE_OLD_Y_POS = y;
}

void setAxis(int turnAxis){
    axis = turnAxis;
}
