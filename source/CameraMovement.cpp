#define GLM_FORCE_RADIANS  /* Use radians in all GLM functions */

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"

/* OpenGL includes */
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "CameraMovement.h"
#include "DeltaTime.h"


    int CAMERA_FREE_MOVE = 1;
    int CAMERA_FIXED_MOVE = 0;
    int cameraMode = 0; // current camera mode

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

/****************************************************************
 
 Camera Movement
 * Handles the user input and moves the camera
 
 ****************************************************************/

glm::mat4 CameraFreeMove(glm::mat4 ViewMatrix){
        glm::mat4 RotationMatrixAnimMouseX;
        glm::mat4 RotationMatrixAnimMouseY;
	glm::mat4 TranslationMatrixMouse;
	
        RotationMatrixAnimMouseX = glm::rotate(glm::mat4(1.0f), (float) mouseDeltaY/deltaTime, glm::vec3(1.0f, 0.0f, 0.0f));
        RotationMatrixAnimMouseY = glm::rotate(glm::mat4(1.0f), (float) mouseDeltaX/deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 RotationMatrixAnimMouse = RotationMatrixAnimMouseX * RotationMatrixAnimMouseY;
        ViewMatrix = RotationMatrixAnimMouse * ViewMatrix;
        
        mouseDeltaY = 0;
        mouseDeltaX = 0;
        
        /* WASD Movement*/
        float camMoveX = 0;
        float camMoveY = 0;
        float camMoveZ = 0;
        
        float camMoveSpeed = 0.5f;
        
        if (camMoveForward == GL_TRUE){
            camMoveZ = camMoveSpeed;
        }
        
        if (camMoveBack == GL_TRUE){
            camMoveZ = -camMoveSpeed;
        }
        
        if (camMoveLeft == GL_TRUE){
            camMoveX = camMoveSpeed;
        }
        
        if (camMoveRight == GL_TRUE){
            camMoveX = -camMoveSpeed;
        }
        
        TranslationMatrixMouse = glm::translate(glm::mat4(1.0f), glm::vec3(camMoveX, camMoveY, camMoveZ));
       
        ViewMatrix = TranslationMatrixMouse * ViewMatrix;
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
