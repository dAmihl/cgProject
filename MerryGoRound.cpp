/******************************************************************
*
* MerryGoRound.c
*
*
* Computer Graphics Proseminar SS 2015
* First project assignment
* 
*
*	Created by
*	Egger Michael, Benjamin Gaugler, Gerald Kofler
*
*
*******************************************************************/

/*
	If you have a Intel graphicscard under Linux with the Mesa Driver,
	set this to 1, else
	you should set this to 0.
*/
#define USES_MESA_DRIVER 1

/*
 *Prints number of vertices, indices and normals of the loaded obj mesh 
 * when set to 1
 */
#define MESH_DEBUG 1

#define MAX_ROTATION_SPEED 0.5
#define START_ROTATION_SPEED 0.05

#define GROUND_SIZE 3.0
#define GROUND_HEIGHT 0.2

#define GLM_FORCE_RADIANS  /* Use radians in all GLM functions */
/* Standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <cstdlib>

/* GLM includes - adjust path as required for local installation */
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" /* Provides glm::translate, glm::rotate, 
                                         * glm::scale, glm::perspective */
#include "glm/gtc/type_ptr.hpp"         /* Vector/matrix handling */

/*
 Include TinyObjLoader
 * Source: look at README
 */
#include "tinyobjloader/tiny_obj_loader.cc"

/* OpenGL includes */
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "LoadShader.h"   /* Provides loading function for shader code */
#include "LoadTexture.h"

/*----------------------------------------------------------------*/
/* Define handle to a vertex array object (only for MESA USE) */
GLuint VAO;

/* Define handle to a vertex buffer object */
GLuint HORSEBOX_VBO;

/* Define handle to a color buffer object */
GLuint HORSEBOX_CBO; 

/* Define handle to an index buffer object */
GLuint HORSEBOX_IBO;


/* Define handle to a vertex buffer object */
GLuint WALL_VBO;

/* Define handle to a color buffer object */
GLuint WALL_CBO; 

/* Define handle to an index buffer object */
GLuint WALL_IBO;


/* Variables for texture handling */
GLuint TextureUniform;

GLuint TextureRobotID;
TextureDataPtr TextureRobot;

GLuint TexturePavillonID;
TextureDataPtr TexturePavillon;

GLuint TextureFloorID;
TextureDataPtr TextureFloor;


/* for the loaded OBJ */

/* Arrays for holding vertex data of the model */
std::vector<GLfloat> vertex_buffer_suzanne;

/* Arrays for holding indices of the model */
std::vector<GLuint> index_buffer_suzanne;

std::vector<GLfloat> normal_buffer_suzanne;

std::vector<GLfloat> uv_buffer_suzanne;


/* for the loaded Pavillon obj*/

/* Arrays for holding vertex data of the model */
std::vector<GLfloat> vertex_buffer_pavillon;

/* Arrays for holding indices of the model */
std::vector<GLuint> index_buffer_pavillon;

std::vector<GLfloat> normal_buffer_pavillon;

std::vector<GLfloat> uv_buffer_pavillon;

/* for the loaded Floor obj*/

/* Arrays for holding vertex data of the model */
std::vector<GLfloat> vertex_buffer_floor;

/* Arrays for holding indices of the model */
std::vector<GLuint> index_buffer_floor;

std::vector<GLfloat> normal_buffer_floor;

std::vector<GLfloat> uv_buffer_floor;

/*
 *Define VertexBuffer, IndexBuffer and NormalsBuffer for the Merry Object
 */
GLuint SUZANNE_VBO;
GLuint SUZANNE_IBO;
GLuint SUZANNE_NBO;
GLuint SUZANNE_UVBO;

/*
 *Define VertexBuffer, IndexBuffer and NormalsBuffer for the MerryGoRound Pavillon
 */
GLuint PAVILLON_VBO;
GLuint PAVILLON_IBO;
GLuint PAVILLON_NBO;
GLuint PAVILLON_UVBO;

/*
 *Define VertexBuffer, IndexBuffer and NormalsBuffer for the Floor object
 */
GLuint FLOOR_VBO;
GLuint FLOOR_IBO;
GLuint FLOOR_NBO;
GLuint FLOOR_UVBO;


/* Indices to vertex attributes; in this case positon and color */ 
enum DataID {vPosition = 0, vUV = 1, vColor = 3, vNormals = 2}; 



/* Strings for loading and storing shader code */
static const char* VertexShaderString;
static const char* FragmentShaderString;

GLuint ShaderProgram;

//glm::vec3 lightPos = glm::vec3(2.0f, 2.0f, 0.0f);

 GLuint PVMMatrixID;
 GLuint ViewMatrixID;
 GLuint ModelMatrixID;
 GLuint LightSourcesID;
 GLuint LightColorsID;
 GLuint LightIntensitiesID;
 
 /*
  * Light sources and Configurations
  */
 const int numberLightSources = 4;

 /*
  Positions of the lightsources
  */
 std::vector<GLfloat> lightSources = {
     0.0f, 10.0f, 0.0f,
     -2.0f, 2.0f, -2.0f,
     -2.0f, 2.0f, 2.0f,
     2.0f, 2.0f, -2.0f
 };
 
 /*
  * Color vector of each lightsource
  */
  std::vector<GLfloat> lightColors = {
     1, 1, 1,
     1, 0, 0,
     0, 1, 0,
     0, 0, 1
 };
  
  /*
   * Intensity of each lightsource
   */
  std::vector<GLfloat> lightIntensities = {
      60, 30, 30, 30
  };

  
glm::mat4 ProjectionMatrix; /* Perspective projection matrix */
glm::mat4 ViewMatrix; /* Camera view matrix */ 
glm::mat4 PVMMatrix;        /* Final combined transformation */

float camera_disp = -25.0;
float camera_aproach = 10.0;

GLboolean animCamera = GL_TRUE; // if the camera is animated
GLboolean animMerryGoRound = GL_TRUE; // if the merry go round is animated

const int CAMERA_FREE_MOVE = 1;
const int CAMERA_FIXED_MOVE = 0;
int cameraMode = 0; // current camera mode

/*
 Handles for the WASD Movement
 */
GLboolean camMoveForward = GL_FALSE;
GLboolean camMoveBack = GL_FALSE;
GLboolean camMoveLeft = GL_FALSE;
GLboolean camMoveRight = GL_FALSE;


glm::mat4 ModelMatrixFloor; /* Model matrix for the floor entity*/
glm::mat4 InitialTransformFloor;
/*
 Model matrix for each MerryGoRound object
 */
glm::mat4 SuzanneMatrix1;
glm::mat4 SuzanneMatrix2;
glm::mat4 SuzanneMatrix3;
glm::mat4 SuzanneMatrix4;

/*
 Matrices for the MerryGoRound pavillon
 */
glm::mat4 PavillonModelMatrix;
glm::mat4 InitialTransformPavillon;

/*
 Transformation Matrices for the MerryGoRound objects
 */
glm::mat4 TranslateOriginBox1;
glm::mat4 TranslateDownBox1;
glm::mat4 RotateXBox1;
glm::mat4 RotateZBox1;
glm::mat4 InitialTransformBox1;
glm::mat4 UpDownTranslationBox1;

glm::mat4 TranslateOriginBox2;
glm::mat4 TranslateDownBox2;
glm::mat4 RotateXBox2;
glm::mat4 RotateZBox2;
glm::mat4 InitialTransformBox2;
glm::mat4 UpDownTranslationBox2;

glm::mat4 TranslateOriginBox3;
glm::mat4 TranslateDownBox3;
glm::mat4 RotateXBox3;
glm::mat4 RotateZBox3;
glm::mat4 InitialTransformBox3;
glm::mat4 UpDownTranslationBox3;

glm::mat4 TranslateOriginBox4;
glm::mat4 TranslateDownBox4;
glm::mat4 RotateXBox4;
glm::mat4 RotateZBox4;
glm::mat4 InitialTransformBox4;
glm::mat4 UpDownTranslationBox4;


/*
 Handles for the MerryGoRound objects animation
 * UpDown movement
 */
const float BOX1_START_POSITION_Y = 1.0;
const float BOX2_START_POSITION_Y = 1.0;
const float BOX3_START_POSITION_Y = 1.0;
const float BOX4_START_POSITION_Y = 1.0;

float BOX1_CURRENT_POSITION_Y;
float BOX2_CURRENT_POSITION_Y;
float BOX3_CURRENT_POSITION_Y;
float BOX4_CURRENT_POSITION_Y;

int BOX1_CURRENT_UPDOWN_DIRECTION = 1;
int BOX2_CURRENT_UPDOWN_DIRECTION = 1;
int BOX3_CURRENT_UPDOWN_DIRECTION = -1;
int BOX4_CURRENT_UPDOWN_DIRECTION = -1;


/*
 Handles for the Mouse Input
 */
int MOUSE_OLD_X_POS = 0;
int MOUSE_OLD_Y_POS = 0;

int mouseDeltaX = 0;
int mouseDeltaY = 0;


/*
 Attributes of the MerryGoRound animation 
 */
float rotation_speed_factor = START_ROTATION_SPEED;
int rotation_direction = 1;
float updown_speed_factor = 1.0f;
float zoom = 1.0;


/* Transformation matrices for camera rotation */
glm::mat4 TranslationMatrixCameraX;
glm::mat4 TranslationMatrixCameraY;
glm::mat4 TranslationMatrixCameraZ;
glm::mat4 TranslationMatrixCamera;

glm::mat4 RotationMatrixCameraX;
glm::mat4 RotationMatrixCameraY;
glm::mat4 RotationMatrixCameraZ;
glm::mat4 RotationMatrixCamera;

/* Variables for storing current rotation angles */
double angleX, angleY, angleZ = 0.0f; 

/* Indices to active rotation axes */
enum {Xaxis=0, Yaxis=1, Zaxis=2};
int axis = Yaxis;

/* Indices to active triangle mesh */
enum {Model1=0, Model2=1};
int model = Model1; 


/* variables for computing elapsed time since last render */
int deltaTime = 0;
int oldTimeSinceStart = 0;

void computeDeltaTime();




/******************************************************************
*
* Display
*
* This function is called when the content of the window needs to be
* drawn/redrawn. It has been specified through 'glutDisplayFunc()';
* Enable vertex attributes, create binding between C program and 
* attribute name in shader
*
*******************************************************************/


void DrawObject(GLuint VBO, GLuint IBO, GLuint NBO, GLuint UVBO,  glm::mat4 ModelMatrix, GLuint TextureID){
    
    glm::mat4 mView = ViewMatrix;
    glm::mat4 mProjection = ProjectionMatrix;
    glm::mat4 PVM = mProjection * mView * ModelMatrix;

    glEnableVertexAttribArray(vPosition);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
 
    
    glEnableVertexAttribArray(vNormals);
    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glVertexAttribPointer(vNormals,3,GL_FLOAT,GL_FALSE,0,(void*)0);
    
    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TextureID);
    TextureUniform  = glGetUniformLocation(ShaderProgram, "textureSampler");
    glUniform1i(TextureUniform, 0);
    
    glEnableVertexAttribArray(vUV);
    glBindBuffer(GL_ARRAY_BUFFER, UVBO);
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(vUV,2,GL_FLOAT,GL_FALSE,0,(void*)0);
                
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    
    GLint size; 
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    
 
 /* Associate program with shader matrices */
 
    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
    glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, glm::value_ptr(mView));
    glUniformMatrix4fv(PVMMatrixID, 1, GL_FALSE, glm::value_ptr(PVM));  
    
    /*
     Bind Light sources
     */
    
    glUniform3fv(LightSourcesID, numberLightSources, &lightSources[0] );
    glUniform3fv(LightColorsID, numberLightSources, &lightColors[0] );
    glUniform1fv(LightIntensitiesID, numberLightSources, &lightIntensities[0] );
    
    /* Issue draw command, using indexed triangle list */
    glDrawElements(GL_TRIANGLES, size/sizeof(GLuint), GL_UNSIGNED_INT, 0);

    /*	--------------------------------------------------------------------------- */
    
    /* Disable attributes */
    glDisableVertexAttribArray(vPosition);
    glDisableVertexAttribArray(vNormals);   
    glDisableVertexAttribArray(vUV);
}

/******************************************************************
*
* SetupTexture
*
* This function is called to load the texture and initialize
* texturing parameters
*
*******************************************************************/

void SetupTexture(GLuint* TextureID, TextureDataPtr Texture, const char* filepath)
{	
    /* Allocate texture container */
    Texture = (TextureDataPtr) malloc(sizeof(TextureDataPtr));

    int success = LoadTexture(filepath, Texture);
    if (!success)
    {
        printf("Error loading texture. Exiting.\n");
	exit(-1);
    }

    /* Create texture name and store in handle */
    glGenTextures(1, TextureID);
	
    /* Bind texture */
    glBindTexture(GL_TEXTURE_2D, *TextureID);

    /* Load texture image into memory */
    glTexImage2D(GL_TEXTURE_2D,     /* Target texture */
		 0,                 /* Base level */
		 GL_RGB,            /* Each element is RGB triple */ 
		 Texture->width,    /* Texture dimensions */ 
                 Texture->height, 
		 0,                 /* Border should be zero */
		 GL_BGR,            /* Data storage format for BMP file */
		 GL_UNSIGNED_BYTE,  /* Type of pixel data, one byte per channel */
		 Texture->data);    /* Pointer to image data  */
 
    /* Next set up texturing parameters */

    /* Repeat texture on edges when tiling */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    /* Linear interpolation for magnification */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* Trilinear MIP mapping for minification */ 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
    glGenerateMipmap(GL_TEXTURE_2D); 

}


/*
 Computes the time elapsed since last Display()
 */
void computeDeltaTime(){
    
    int newTime = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = newTime - oldTimeSinceStart;
    oldTimeSinceStart = newTime; 
}


void Display()
{
    /* Clear window; color specified in 'Initialize()' */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Walls and Floor*/
    DrawObject(FLOOR_VBO,  FLOOR_IBO, FLOOR_NBO,FLOOR_UVBO, ModelMatrixFloor, TextureFloorID);

    DrawObject(SUZANNE_VBO,  SUZANNE_IBO,SUZANNE_NBO, SUZANNE_UVBO, SuzanneMatrix1, TextureRobotID);
    DrawObject(SUZANNE_VBO,  SUZANNE_IBO,SUZANNE_NBO, SUZANNE_UVBO,SuzanneMatrix2, TextureRobotID);
    DrawObject(SUZANNE_VBO,  SUZANNE_IBO,SUZANNE_NBO, SUZANNE_UVBO,SuzanneMatrix3, TextureRobotID);
    DrawObject(SUZANNE_VBO,  SUZANNE_IBO,SUZANNE_NBO, SUZANNE_UVBO,SuzanneMatrix4, TextureRobotID);
    
    DrawObject(PAVILLON_VBO,  PAVILLON_IBO,PAVILLON_NBO, PAVILLON_UVBO,PavillonModelMatrix, TexturePavillonID);

    /* Swap between front and back buffer */ 
    glutSwapBuffers();
}


/******************************************************************
*
* Mouse
*
* Function is called on mouse button press; has been seta
* with glutMouseFunc(), x and y specify mouse coordinates,
* but are not used here.
*
*******************************************************************/

void Mouse(int button, int state, int x, int y) 
{
    // float correction_factor = 1 / 10;
    
    mouseDeltaX = x - MOUSE_OLD_X_POS;
    mouseDeltaY = y - MOUSE_OLD_Y_POS;

    
    if(state == GLUT_DOWN) 
    {
      /* Depending on button pressed, set rotation axis,
       * turn on animation */
        switch(button) 
	{
	    case GLUT_LEFT_BUTTON:    
	        axis = Xaxis;
		break;

	    case GLUT_MIDDLE_BUTTON:  
  	        axis = Yaxis;
	        break;
		
	    case GLUT_RIGHT_BUTTON: 
	        axis = Zaxis;
		zoom = 1.0;
		break;
	}
    }
      
    MOUSE_OLD_X_POS = x;
    MOUSE_OLD_Y_POS = y;
}


/******************************************************************
*
* MouseMove
*
* Function is called on mouse moving in active window; has been seta
* with glutPassiveMotionFunc(), x and y specify mouse coordinates,
* 
*
*******************************************************************/

void MouseMove(int x, int y) 
{    
    mouseDeltaX = x - MOUSE_OLD_X_POS;
    mouseDeltaY = y - MOUSE_OLD_Y_POS;
    
    MOUSE_OLD_X_POS = x;
    MOUSE_OLD_Y_POS = y;
}

/************************************************
 * Helper functions to change the light attributes
 * to user input
 ***********************************************/

void increaseLightIntensity(){
    lightIntensities[0] += 10;
    lightIntensities[0] = glm::clamp(lightIntensities[0], 0.f, 500.f);
}

void decreaseLightIntensity(){
    lightIntensities[0] -= 10;
    lightIntensities[0] = glm::clamp(lightIntensities[0], 0.0f, 500.0f);
}

void increaseLightColor(){
    srand (time(0));  
    lightColors[0] += ((double) rand() / (RAND_MAX));
    lightColors[1] += ((double) rand() / (RAND_MAX));
    lightColors[2] += ((double) rand() / (RAND_MAX)); 
    lightColors[0] = glm::clamp(lightColors[0], 0.f, 1.f);
    lightColors[1] = glm::clamp(lightColors[1], 0.f, 1.f);
    lightColors[2] = glm::clamp(lightColors[2], 0.f, 1.f);
}

void decreaseLightColor(){
    srand (time(0));  
    lightColors[0] -= ((double) rand() / (RAND_MAX));
    lightColors[1] -= ((double) rand() / (RAND_MAX));
    lightColors[2] -= ((double) rand() / (RAND_MAX)); 
    lightColors[0] = glm::clamp(lightColors[0], 0.f, 1.f);
    lightColors[1] = glm::clamp(lightColors[1], 0.f, 1.f);
    lightColors[2] = glm::clamp(lightColors[2], 0.f, 1.f);
}

/******************************************************************
*
* Keyboard
*
* Function to be called on key press in window; set by
* glutKeyboardFunc(); x and y specify mouse position on keypress;
* not used in this example 
*
*******************************************************************/

void Keyboard(unsigned char key, int x, int y)   
{
    switch( key ) 
    {
	/* --------------------------------------- */
	// keys to manipulate the modle
	// change speed and direction of rotation
	case 'w':
	    
            if (cameraMode == CAMERA_FREE_MOVE) camMoveForward = GL_TRUE;
            else{
                if (rotation_speed_factor < MAX_ROTATION_SPEED) rotation_speed_factor += 0.1;
                if (updown_speed_factor < MAX_ROTATION_SPEED) updown_speed_factor += 0.1;
            }
	    break;
	    
	case 's':
	   
            if (cameraMode == CAMERA_FREE_MOVE) camMoveBack = GL_TRUE;
            else{
                if (rotation_speed_factor > 0) rotation_speed_factor -= 0.1;
                if (updown_speed_factor > 0) updown_speed_factor -= 0.1;
            }
	    break;
	case 'a':
	    
            if (cameraMode == CAMERA_FREE_MOVE) camMoveLeft = GL_TRUE;
            else{
                rotation_direction = 1;
            }
	    break;
	    
	case 'd':
	    
            if (cameraMode == CAMERA_FREE_MOVE) camMoveRight = GL_TRUE;
            else{
                rotation_direction = -1;
            }
	    break;
            
        case 'p':
            animMerryGoRound = !animMerryGoRound;
            break;
            
        case 'i':
            increaseLightIntensity();
            break;
        
        case 'k':
            decreaseLightIntensity();
            break;
            
        case 'u':
            increaseLightColor();
            break;
            
        case 'j':
            decreaseLightColor();
            break;
	/* --------------------------------------- */
	// keys to manipulate the camera
	/* Activate camera mode fixed or free */
	case '1': 
		cameraMode = CAMERA_FIXED_MOVE;
		animCamera = GL_TRUE;
		break;

	case '2':
		cameraMode = CAMERA_FREE_MOVE;
		animCamera = GL_FALSE;
		break;

	/* Reset initial rotation of object */	  
	 case 'o':
    
	  BOX1_CURRENT_POSITION_Y = BOX1_START_POSITION_Y;
	  BOX2_CURRENT_POSITION_Y = BOX2_START_POSITION_Y;
	  BOX3_CURRENT_POSITION_Y = BOX3_START_POSITION_Y;
	  BOX4_CURRENT_POSITION_Y = BOX4_START_POSITION_Y;
	  
	  rotation_speed_factor = START_ROTATION_SPEED;
	  rotation_direction = 1;
	  updown_speed_factor = START_ROTATION_SPEED;
	  
	  break;
	  
	case 'q': case 'Q':  
	    exit(0);    
		break;
    }

    glutPostRedisplay();
}


void KeyboardUp(unsigned char key, int x, int y)   
{
    switch( key ) 
    {
	/* --------------------------------------- */
	// keys to manipulate the modle
	// change speed and direction of rotation
	case 'w':
            if (cameraMode == CAMERA_FREE_MOVE) camMoveForward = GL_FALSE;
	    break;
	    
	case 's':
            if (cameraMode == CAMERA_FREE_MOVE) camMoveBack = GL_FALSE;
	    break;
	case 'a':
            if (cameraMode == CAMERA_FREE_MOVE) camMoveLeft = GL_FALSE;
	    break;
	    
	case 'd':
            if (cameraMode == CAMERA_FREE_MOVE) camMoveRight = GL_FALSE;
	    break;
    }
}






/******************************************************************
*
* OnIdle
*
* 
*
*******************************************************************/

void OnIdle()
{
    
    computeDeltaTime();

    if (cameraMode == CAMERA_FREE_MOVE){
        glm::mat4 RotationMatrixAnimMouseX;
        glm::mat4 RotationMatrixAnimMouseY;
        glm::mat4 RotationMatrixAnimMouseZ;
	glm::mat4 TranslationMatrixMouse;
	
        RotationMatrixAnimMouseX = glm::rotate(glm::mat4(1.0f), (float) mouseDeltaY/deltaTime, glm::vec3(1.0f, 0.0f, 0.0f));
        RotationMatrixAnimMouseY = glm::rotate(glm::mat4(1.0f), (float) mouseDeltaX/deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
        RotationMatrixAnimMouseZ = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
              
        ViewMatrix = RotationMatrixAnimMouseX * ViewMatrix;
        ViewMatrix = RotationMatrixAnimMouseY * ViewMatrix;
        ViewMatrix = RotationMatrixAnimMouseZ * ViewMatrix;
        
        mouseDeltaY = 0;
        mouseDeltaX = 0;
        
        /* WASD Movement*/
        float camMoveX = 0;
        float camMoveY = 0;
        float camMoveZ = 0;
        
        int camMoveSpeed = 1;
        
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
    }// 1 4 5
    // Camera Fixed Move active
    else {
     
    if(animCamera) {
        
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

        }
    }
    
    
    if (animMerryGoRound){
            /* SetUp Rotation matrices */
        float angle = (glutGet(GLUT_ELAPSED_TIME) / 1000.0) * (180.0/M_PI);

        angle *= rotation_speed_factor;
        angle *= rotation_direction;


        glm::mat4 RotationMatrixAnimPavillon;
        glm::mat4 RotationMatrixAnimBox1;
        glm::mat4 RotationMatrixAnimBox2;
        glm::mat4 RotationMatrixAnimBox3;
        glm::mat4 RotationMatrixAnimBox4;



        /* Time dependent rotation */
        RotationMatrixAnimPavillon = glm::rotate(glm::mat4(1.0f), -angle/2, glm::vec3(0.0f, 1.0f, 0.0f));
        RotationMatrixAnimBox1 = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
        RotationMatrixAnimBox2 = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
        RotationMatrixAnimBox3 = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
        RotationMatrixAnimBox4 = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));    

        /*Rotation of the light sources*/
        float angleLight = fmod(0.001 * deltaTime, 360.0);
        glm::mat4 RotationMatrixAnimLight = glm::rotate(glm::mat4(1.0f), angleLight, glm::vec3(0.0f, 1.0f, 0.0f));

        for (int i = 0; i < numberLightSources * 3; i+=3){
            glm::vec4 lightVec = glm::vec4(lightSources[i], lightSources[i+1], lightSources[i+2], 1.0f);
            lightVec = lightVec * RotationMatrixAnimLight;
            lightSources[i] = lightVec.x;
            lightSources[i+1] = lightVec.y;
            lightSources[i+2] = lightVec.z;
        }


            /* compute merry-go-round horse translation */
        BOX1_CURRENT_UPDOWN_DIRECTION = (BOX1_CURRENT_POSITION_Y > 2 ? -1 : (BOX1_CURRENT_POSITION_Y < 0 ? 1 : BOX1_CURRENT_UPDOWN_DIRECTION));
        BOX2_CURRENT_UPDOWN_DIRECTION = (BOX2_CURRENT_POSITION_Y > 2 ? -1 : (BOX2_CURRENT_POSITION_Y < 0 ? 1 : BOX2_CURRENT_UPDOWN_DIRECTION));
        BOX3_CURRENT_UPDOWN_DIRECTION = (BOX3_CURRENT_POSITION_Y > 2 ? -1 : (BOX3_CURRENT_POSITION_Y < 0 ? 1 : BOX3_CURRENT_UPDOWN_DIRECTION));
        BOX4_CURRENT_UPDOWN_DIRECTION = (BOX4_CURRENT_POSITION_Y > 2 ? -1 : (BOX4_CURRENT_POSITION_Y < 0 ? 1 : BOX4_CURRENT_UPDOWN_DIRECTION));

        float updown = 3.f * deltaTime/1000;
        updown *= updown_speed_factor;

        BOX1_CURRENT_POSITION_Y += updown * BOX1_CURRENT_UPDOWN_DIRECTION;
        BOX2_CURRENT_POSITION_Y += updown * BOX2_CURRENT_UPDOWN_DIRECTION;
        BOX3_CURRENT_POSITION_Y += updown * BOX3_CURRENT_UPDOWN_DIRECTION;
        BOX4_CURRENT_POSITION_Y += updown * BOX4_CURRENT_UPDOWN_DIRECTION;

        UpDownTranslationBox1 = glm::translate(glm::mat4(1.0f), glm::vec3(0, BOX1_CURRENT_POSITION_Y, 0));
        UpDownTranslationBox2 = glm::translate(glm::mat4(1.0f), glm::vec3(0, BOX2_CURRENT_POSITION_Y, 0));
        UpDownTranslationBox3 = glm::translate(glm::mat4(1.0f), glm::vec3(0, BOX3_CURRENT_POSITION_Y, 0));
        UpDownTranslationBox4 = glm::translate(glm::mat4(1.0f), glm::vec3(0, BOX4_CURRENT_POSITION_Y, 0));

        PavillonModelMatrix = RotationMatrixAnimPavillon * InitialTransformPavillon;
        ModelMatrixFloor = InitialTransformFloor;
        /* Apply model rotation; finally move cube down */    
        SuzanneMatrix1 = TranslateDownBox1 * UpDownTranslationBox1 * RotationMatrixAnimBox1 * InitialTransformBox1;
        SuzanneMatrix2 = TranslateDownBox2 * UpDownTranslationBox2 * RotationMatrixAnimBox2 * InitialTransformBox2;
        SuzanneMatrix3 = TranslateDownBox3 * UpDownTranslationBox3 * RotationMatrixAnimBox3 * InitialTransformBox3;
        SuzanneMatrix4 = TranslateDownBox4 * UpDownTranslationBox4 * RotationMatrixAnimBox4 * InitialTransformBox4;

    }
    /* ---------------------------------------------------------------------------- */

    /* Request redrawing forof window content */  
    glutPostRedisplay();
}




/******************************************************************
*
* SetupDataBuffers
*
* Create buffer objects and load data into buffers
*
*******************************************************************/

void SetupDataBuffers()
{
    glGenBuffers(1, &SUZANNE_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, SUZANNE_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_suzanne.size()*sizeof(GLfloat), &vertex_buffer_suzanne[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &SUZANNE_NBO);
    glBindBuffer(GL_ARRAY_BUFFER, SUZANNE_NBO);
    glBufferData(GL_ARRAY_BUFFER, normal_buffer_suzanne.size()*sizeof(GLfloat), &normal_buffer_suzanne[0], GL_STATIC_DRAW);

    
    glGenBuffers(1, &SUZANNE_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SUZANNE_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_suzanne.size()*sizeof(GLuint), &index_buffer_suzanne[0], GL_STATIC_DRAW);
 
    glGenBuffers(1, &SUZANNE_UVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SUZANNE_UVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, uv_buffer_suzanne.size()*sizeof(GLfloat), &uv_buffer_suzanne[0], GL_STATIC_DRAW);
 
    
    glGenBuffers(1, &PAVILLON_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, PAVILLON_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_pavillon.size()*sizeof(GLfloat), &vertex_buffer_pavillon[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &PAVILLON_NBO);
    glBindBuffer(GL_ARRAY_BUFFER, PAVILLON_NBO);
    glBufferData(GL_ARRAY_BUFFER, normal_buffer_pavillon.size()*sizeof(GLfloat), &normal_buffer_pavillon[0], GL_STATIC_DRAW);

    
    glGenBuffers(1, &PAVILLON_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, PAVILLON_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_pavillon.size()*sizeof(GLuint), &index_buffer_pavillon[0], GL_STATIC_DRAW);
 
    glGenBuffers(1, &PAVILLON_UVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, PAVILLON_UVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, uv_buffer_pavillon.size()*sizeof(GLfloat), &uv_buffer_pavillon[0], GL_STATIC_DRAW);
 
    
    
    glGenBuffers(1, &FLOOR_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, FLOOR_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_floor.size()*sizeof(GLfloat), &vertex_buffer_floor[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &FLOOR_NBO);
    glBindBuffer(GL_ARRAY_BUFFER, FLOOR_NBO);
    glBufferData(GL_ARRAY_BUFFER, normal_buffer_floor.size()*sizeof(GLfloat), &normal_buffer_floor[0], GL_STATIC_DRAW);

    glGenBuffers(1, &FLOOR_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, FLOOR_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_floor.size()*sizeof(GLuint), &index_buffer_floor[0], GL_STATIC_DRAW);
     
    glGenBuffers(1, &FLOOR_UVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, FLOOR_UVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, uv_buffer_floor.size()*sizeof(GLfloat), &uv_buffer_floor[0], GL_STATIC_DRAW);
 
}


/******************************************************************
*
* AddShader
*
* This function creates and adds individual shaders
*
*******************************************************************/

void AddShader(GLuint ShaderProgram, const char* ShaderCode, GLenum ShaderType)
{
    /* Create shader object */
    GLuint ShaderObj = glCreateShader(ShaderType);

    if (ShaderObj == 0) 
    {
        fprintf(stderr, "Error creating shader type %d\n", ShaderType);
        exit(0);
    }

    /* Associate shader source code string with shader object */
    glShaderSource(ShaderObj, 1, &ShaderCode, NULL);

    GLint success = 0;
    GLchar InfoLog[1024];

    /* Compile shader source code */
    glCompileShader(ShaderObj);
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

    if (!success) 
    {
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
        exit(1);
    }

    /* Associate shader with shader program */
    glAttachShader(ShaderProgram, ShaderObj);
 
}


/******************************************************************
*
* CreateShaderProgram
*
* This function creates the shader program; vertex and fragment
* shaders are loaded and linked into program; final shader program
* is put into the rendering pipeline 
*
*******************************************************************/

void CreateShaderProgram()
{
    /* Allocate shader object */
    ShaderProgram = glCreateProgram();

    if (ShaderProgram == 0) 
    {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }

    /* Load shader code from file */
    VertexShaderString = LoadShader("shaders/standard.vs");
    FragmentShaderString = LoadShader("shaders/standard.fs");

    /* Separately add vertex and fragment shader to program */
    AddShader(ShaderProgram, VertexShaderString, GL_VERTEX_SHADER);
    AddShader(ShaderProgram, FragmentShaderString, GL_FRAGMENT_SHADER);

    GLint Success = 0;
    GLchar ErrorLog[1024];

    /* Link shader code into executable shader program */
    glLinkProgram(ShaderProgram);

    /* Check results of linking step */
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

    if (Success == 0) 
    {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    /* Check if shader program can be executed */ 
    glValidateProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);

    if (!Success) 
    {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    /* Put linked shader program into drawing pipeline */
    glUseProgram(ShaderProgram);
    
    
    PVMMatrixID = glGetUniformLocation(ShaderProgram, "ProjectionViewModelMatrix");
    ViewMatrixID = glGetUniformLocation(ShaderProgram, "V");
    ModelMatrixID = glGetUniformLocation(ShaderProgram, "M");
    LightSourcesID = glGetUniformLocation(ShaderProgram, "LightPosition_worldspace");
    LightColorsID = glGetUniformLocation(ShaderProgram, "LightColor");
    LightIntensitiesID = glGetUniformLocation(ShaderProgram, "LightIntensity");
}

/*
	Setup Vertex Array Object for use with MESA driver under Linux (Intel HD graphiccards)
*/

void setupArrayObject(){
    if (USES_MESA_DRIVER){
		glGenVertexArrays(1, &VAO); // Create our Vertex Array Object  
		glBindVertexArray(VAO); // Bind our Vertex Array Object so we can use it 
	}
}

void setupIntelMesaConfiguration(){

	if (USES_MESA_DRIVER){
		/*
		INTEL MESA TROUBLESHOOTING
		*/
		glutInitContextVersion(3,3);
		glutInitContextProfile(GLUT_CORE_PROFILE);
    	glewExperimental = GL_TRUE;
	}
}


/******************************************************************
*
* LoadMesh
*
 * Loads the meshes of the MerryGoRound Object, the Pavillon and the Ground
*
*******************************************************************/


void LoadMesh(){
    std::string inputfileSuzanne = "models/ufo_rusty.obj";
    std::vector<tinyobj::shape_t> shapesSuzanne;
    std::vector<tinyobj::material_t> materialsSuzanne;
    
    std::string inputfilePavillon = "models/pavillon_metal.obj";
    std::vector<tinyobj::shape_t> shapesPavillon;
    std::vector<tinyobj::material_t> materialsPavillon;
    
    std::string inputfileFloor = "models/ground_glyphs.obj";
    std::vector<tinyobj::shape_t> shapesFloor;
    std::vector<tinyobj::material_t> materialsFloor;
    
         
    std::string err = tinyobj::LoadObj(shapesSuzanne, materialsSuzanne, inputfileSuzanne.c_str());

    if (!err.empty()) {
        fprintf(stderr, "Error loading obj File Suzanne!");
      exit(1);
    }
    
    if (MESH_DEBUG){
        int numVertices = shapesSuzanne[0].mesh.positions.size();
        int numIndices = shapesSuzanne[0].mesh.indices.size();
        int numNormals = shapesSuzanne[0].mesh.normals.size();
        int numUVs = shapesSuzanne[0].mesh.texcoords.size();
        

        fprintf(stderr, "Number of vertics Suzanne: %d \n", numVertices);
        fprintf(stderr, "Number of indices Suzanne: %d\n", numIndices);
        fprintf(stderr, "Number of normalsSuzanne : %d\n",numNormals);
        fprintf(stderr, "Nuber of uvs Suzanne: %d\n", numUVs);
    }
    vertex_buffer_suzanne = shapesSuzanne[0].mesh.positions;
    index_buffer_suzanne = shapesSuzanne[0].mesh.indices;
    normal_buffer_suzanne = shapesSuzanne[0].mesh.normals;
    uv_buffer_suzanne = shapesSuzanne[0].mesh.texcoords;

    
    /*
     Load Pavillon
     */
    err = tinyobj::LoadObj(shapesPavillon, materialsPavillon, inputfilePavillon.c_str());

    if (!err.empty()) {
        fprintf(stderr, "Error loading obj File Pavillon!");
      exit(1);
    }
    
    if (MESH_DEBUG){
        int numVertices = shapesPavillon[0].mesh.positions.size();
        int numIndices = shapesPavillon[0].mesh.indices.size();
        int numNormals = shapesPavillon[0].mesh.normals.size();
        int numUVs = shapesPavillon[0].mesh.texcoords.size();


        fprintf(stderr, "Number of vertics Pavillon: %d \n", numVertices);
        fprintf(stderr, "Number of indices Pavillon: %d\n", numIndices);
        fprintf(stderr, "Number of normals Pavillon : %d\n",numNormals);
        fprintf(stderr, "Number of uvs Pavillon: %d\n", numUVs);
    }
    vertex_buffer_pavillon = shapesPavillon[0].mesh.positions;
    index_buffer_pavillon = shapesPavillon[0].mesh.indices;
    normal_buffer_pavillon = shapesPavillon[0].mesh.normals;
    uv_buffer_pavillon = shapesPavillon[0].mesh.texcoords;


    /*
     Load Ground
     */
    err = tinyobj::LoadObj(shapesFloor, materialsFloor, inputfileFloor.c_str());

    if (!err.empty()) {
        fprintf(stderr, "Error loading obj File Pavillon!");
      exit(1);
    }
    
    if (MESH_DEBUG){
        int numVertices = shapesFloor[0].mesh.positions.size();
        int numIndices = shapesFloor[0].mesh.indices.size();
        int numNormals = shapesFloor[0].mesh.normals.size();
        int numUVS = shapesFloor[0].mesh.texcoords.size();
        
        fprintf(stderr, "Number of vertics Floor: %d \n", numVertices);
        fprintf(stderr, "Number of indices Floor: %d\n", numIndices);
        fprintf(stderr, "Number of normals Floor : %d\n",numNormals);
        fprintf(stderr, "Number of uvs Floor: %d\n", numUVS);
    }
    vertex_buffer_floor = shapesFloor[0].mesh.positions;
    index_buffer_floor = shapesFloor[0].mesh.indices;
    normal_buffer_floor = shapesFloor[0].mesh.normals;
    uv_buffer_floor = shapesFloor[0].mesh.texcoords;

}


/******************************************************************
*
* Initialize
*
* This function is called to initialize rendering elements, setup
* vertex buffer objects, and to setup the vertex and fragment shader
*
*******************************************************************/


void Initialize(void)
{   
    // load the meshes
    LoadMesh();
    
    glClearColor(0.0f, 0.0f, 0.3f, 0.0);

    /* Enable depth testing */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);    

    /*Intel troubleshooting*/
    setupArrayObject();

    /* Setup vertex, color, and index buffer objects */
    SetupDataBuffers();

    /* Setup Texture*/
    SetupTexture(&TextureRobotID, TextureRobot, "textures/rustytexture.bmp");
    SetupTexture(&TextureFloorID, TextureFloor, "textures/glyphfloor.bmp");
    //SetupTexture(&TextureFloorID, TextureFloor, "textures/bumpmaptest2.bmp");
    SetupTexture(&TexturePavillonID, TexturePavillon, "textures/metalpavillon.bmp");

    
    /* Setup shaders and shader program */
    CreateShaderProgram();

    /* Initialize matrices */
   
    
    TranslationMatrixCamera = glm::mat4(1.0f);
    TranslationMatrixCameraX = glm::mat4(1.0f);
    TranslationMatrixCameraY = glm::mat4(1.0f);
    TranslationMatrixCameraZ = glm::mat4(1.0f);
    
    RotationMatrixCamera = glm::mat4(1.0f);
    RotationMatrixCameraX = glm::mat4(1.0f);
    RotationMatrixCameraY = glm::mat4(1.0f);
    RotationMatrixCameraZ = glm::mat4(1.0f);
    
    SuzanneMatrix1 = glm::mat4(1.0f);
    SuzanneMatrix2 = glm::mat4(1.0f);
    SuzanneMatrix3 = glm::mat4(1.0f);
    SuzanneMatrix4 = glm::mat4(1.0f);
    
    ModelMatrixFloor = glm::mat4(1.0f);
    
    PavillonModelMatrix = glm::mat4(1.0f);
    InitialTransformPavillon = glm::mat4(1.0f);
    
    glm::mat4 scalePavillon = glm::scale(glm::mat4(1.0f), glm::vec3(1.1f));
    glm::mat4 translatePavillon = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    
    InitialTransformPavillon = scalePavillon * translatePavillon;
    glm::mat4 InitialScaleFloor = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
    glm::mat4 InitialTranslateFloor = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    InitialTransformFloor = InitialScaleFloor * InitialTranslateFloor;

    
    
    BOX1_CURRENT_POSITION_Y = BOX1_START_POSITION_Y;
    BOX2_CURRENT_POSITION_Y = BOX2_START_POSITION_Y;
    BOX3_CURRENT_POSITION_Y = BOX3_START_POSITION_Y;
    BOX4_CURRENT_POSITION_Y = BOX4_START_POSITION_Y;

    
    /* Set projection transform */
    float fovy = 45.0;
    float aspect = 1.0; 
    float nearPlane = 1.0; 
    float farPlane = 50.0;
    ProjectionMatrix = glm::perspective(fovy, aspect, nearPlane, farPlane); 

    /* Set viewing transform */  
    ViewMatrix = glm::lookAt(glm::vec3(0,0,-10),    /* Eye vector */
			     glm::vec3(0,0,0),     /* Viewing center */
			     glm::vec3(0,1,0) );  /* Up vector */
    
    
    TranslateOriginBox1 = glm::translate(glm::mat4(1.0f), glm::vec3(2, BOX1_START_POSITION_Y, 2));
    TranslateOriginBox2 = glm::translate(glm::mat4(1.0f), glm::vec3(-2, BOX2_START_POSITION_Y, -2));
    TranslateOriginBox3 = glm::translate(glm::mat4(1.0f), glm::vec3(-2, BOX3_START_POSITION_Y, 2));
    TranslateOriginBox4 = glm::translate(glm::mat4(1.0f), glm::vec3(2, BOX4_START_POSITION_Y, -2));

    TranslateDownBox1 = glm::translate(glm::mat4(1.0f), glm::vec3(0, -sqrtf(sqrtf(2.0) * 1.0), 0 ));
    TranslateDownBox2 = glm::translate(glm::mat4(1.0f), glm::vec3(0, -sqrtf(sqrtf(2.0) * 1.0), 0 ));
    TranslateDownBox3 = glm::translate(glm::mat4(1.0f), glm::vec3(0, -sqrtf(sqrtf(2.0) * 1.0), 0 ));
    TranslateDownBox4 = glm::translate(glm::mat4(1.0f), glm::vec3(0, -sqrtf(sqrtf(2.0) * 1.0), 0 ));
      
    /* Walls and Floor*/
    InitialTransformBox1 = RotateZBox1 * RotateXBox1 * TranslateOriginBox1;
    InitialTransformBox2 = RotateZBox2 * RotateXBox2 * TranslateOriginBox2;
    InitialTransformBox3 = RotateZBox3 * RotateXBox3 * TranslateOriginBox3;
    InitialTransformBox4 = RotateZBox4 * RotateXBox4 * TranslateOriginBox4;
    
    InitialTransformBox1 = glm::scale(InitialTransformBox1, glm::vec3(0.5f));
    InitialTransformBox2 = glm::scale(InitialTransformBox2, glm::vec3(0.5f));
    InitialTransformBox3 = glm::scale(InitialTransformBox3, glm::vec3(0.5f));
    InitialTransformBox4 = glm::scale(InitialTransformBox4, glm::vec3(0.5f));

}






/******************************************************************
*
* main
*
* Main function to setup GLUT, GLEW, and enter rendering loop
*
*******************************************************************/

int main(int argc, char** argv)
{
    /* Initialize GLUT; set double buffered window and RGBA color model */
    glutInit(&argc, argv);

    setupIntelMesaConfiguration();

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(400, 400);
    glutCreateWindow("Round round round");

    

    /* Initialize GL extension wrangler */
    GLenum res = glewInit();
    if (res != GLEW_OK) 
    {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }

    /* Setup scene and rendering parameters */
    Initialize();


    /* Specify callback functions;enter GLUT event processing loop, 
     * handing control over to GLUT */
    glutIdleFunc(OnIdle);
    glutDisplayFunc(Display);
    glutKeyboardFunc(Keyboard);     
    glutKeyboardUpFunc(KeyboardUp);
    glutMouseFunc(Mouse);
    glutPassiveMotionFunc(MouseMove);
    glutMainLoop();

    /* ISO C requires main to return int */
    return 0;
}
