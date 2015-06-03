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


#define WALL_SIZE 6.0
#define WALL_HEIGHT 0.1

#define MAX_ROTATION_SPEED 1.0
#define START_ROTATION_SPEED 0.1

#define GROUND_SIZE 3.0
#define GROUND_HEIGHT 0.2

#define BOX1_SIZE 0.3
#define BOX1_HEIGHT 1.0

    
#define PILLAR_SIZE 0.5
#define PILLAR_HEIGHT 2


#define GLM_FORCE_RADIANS  /* Use radians in all GLM functions */
/* Standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* GLM includes - adjust path as required for local installation */
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" /* Provides glm::translate, glm::rotate, 
                                         * glm::scale, glm::perspective */
#include "glm/gtc/type_ptr.hpp"         /* Vector/matrix handling */

/* OpenGL includes */
#include <GL/glew.h>
#include <GL/freeglut.h>


/* Local includes */
/* Local includes */

    #include "LoadShader.h"   /* Provides loading function for shader code */

#include "Matrix.h"
#include "OBJParser.h"


/*----------------------------------------------------------------*/
/* Define handle to a vertex array object (only for MESA USE) */
GLuint VAO;


/* Define handle to a vertex buffer object */
GLuint GROUND_VBO;

/* Define handle to a color buffer object */
GLuint GROUND_CBO; 

/* Define handle to an index buffer object */
GLuint GROUND_IBO;



/* Define handle to a vertex buffer object */
GLuint HORSEBOX_VBO;

/* Define handle to a color buffer object */
GLuint HORSEBOX_CBO; 

/* Define handle to an index buffer object */
GLuint HORSEBOX_IBO;



/* Define handle to a vertex buffer object */
GLuint PILLAR_VBO;

/* Define handle to a color buffer object */
GLuint PILLAR_CBO; 

/* Define handle to an index buffer object */
GLuint PILLAR_IBO;




/* Define handle to a vertex buffer object */
GLuint WALL_VBO;

/* Define handle to a color buffer object */
GLuint WALL_CBO; 

/* Define handle to an index buffer object */
GLuint WALL_IBO;



/* for the loaded OBJ */

/* Arrays for holding vertex data of the model */
GLfloat *vertex_buffer_suzanne;

/* Arrays for holding indices of the model */
GLuint *index_buffer_suzanne;

/* Structures for loading of OBJ data */
obj_scene_data suzanne_data;

/* Define handles to two vertex buffer objects */
GLuint SUZANNE_VBO;

/* Define handles to two index buffer objects */
GLuint SUZANNE_IBO;





/* Indices to vertex attributes; in this case positon and color */ 
enum DataID {vPosition = 0, vColor = 1}; 



/* Strings for loading and storing shader code */
static const char* VertexShaderString;
static const char* FragmentShaderString;

GLuint ShaderProgram;



glm::mat4 ProjectionMatrix; /* Perspective projection matrix */
glm::mat4 ViewMatrix; /* Camera view matrix */ 
glm::mat4 PVMMatrix;        /* Final combined transformation */
float camera_disp = -25.0;
float camera_aproach = 10.0;

GLboolean anim = GL_TRUE;

const int CAMERA_FREE_MOVE = 1;
const int CAMERA_FIXED_MOVE = 0;
int cameraMode = 0;

GLboolean camMoveForward = GL_FALSE;
GLboolean camMoveBack = GL_FALSE;
GLboolean camMoveLeft = GL_FALSE;
GLboolean camMoveRight = GL_FALSE;



glm::mat4 ModelMatrixGround;	/* Model matrix for the ground layer */
glm::mat4 ModelMatrixRoof;	/* Model matrix for the roof (top layer) */
glm::mat4 ModelMatrixPillar;	/* Model matrix for the pillar */

glm::mat4 ModelMatrixFloor; /* Model matrix for the floor entity*/
glm::mat4 ModelMatrixWall1; /* Model matrix for the wall entity*/
glm::mat4 ModelMatrixWall2; /* Model matrix for the wall entity*/
glm::mat4 ModelMatrixWall3; /* Model matrix for the wall entity*/


glm::mat4 SuzanneMatrix1;
glm::mat4 SuzanneMatrix2;
glm::mat4 SuzanneMatrix3;
glm::mat4 SuzanneMatrix4;

glm::mat4 PVMMatrixGround;
glm::mat4 PVMMatrixRoof;
glm::mat4 PVMMatrixPillar;

glm::mat4 PVMMatrixFloor;
glm::mat4 PVMMatrixWall1;
glm::mat4 PVMMatrixWall2;
glm::mat4 PVMMatrixWall3;

glm::mat4 PVMMatrixSuzanne1;
glm::mat4 PVMMatrixSuzanne2;
glm::mat4 PVMMatrixSuzanne3;
glm::mat4 PVMMatrixSuzanne4;



/* Transformation matrices for initial position */

glm::mat4 TranslateOriginRoof;
glm::mat4 TranslateDownRoof;
glm::mat4 RotateXRoof;
glm::mat4 RotateZRoof;
glm::mat4 InitialTransformRoof;

glm::mat4 TranslateOriginPillar;
glm::mat4 TranslateDownPillar;
glm::mat4 RotateXPillar;
glm::mat4 RotateZPillar;
glm::mat4 InitialTransformPillar;

glm::mat4 TranslateOriginGround;
glm::mat4 TranslateDownGround;
glm::mat4 RotateXGround;
glm::mat4 RotateZGround;
glm::mat4 InitialTransformGround;

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

/* Walls and Ground */

glm::mat4 TranslateOriginWall1;
glm::mat4 TranslateDownWall1;
glm::mat4 RotateXWall1;
glm::mat4 RotateYWall1;
glm::mat4 RotateZWall1;
glm::mat4 InitialTransformWall1;
glm::mat4 UpDownTranslationWall1;

glm::mat4 TranslateOriginWall2;
glm::mat4 TranslateDownWall2;
glm::mat4 RotateXWall2;
glm::mat4 RotateZWall2;
glm::mat4 RotateYWall2;
glm::mat4 InitialTransformWall2;
glm::mat4 UpDownTranslationWall2;

glm::mat4 TranslateOriginWall3;
glm::mat4 TranslateDownWall3;
glm::mat4 RotateXWall3;
glm::mat4 RotateZWall3;
glm::mat4 RotateYWall3;
glm::mat4 InitialTransformWall3;
glm::mat4 UpDownTranslationWall3;

glm::mat4 TranslateOriginFloor;
glm::mat4 TranslateDownFloor;
glm::mat4 RotateXFloor;
glm::mat4 RotateZFloor;
glm::mat4 InitialTransformFloor;
glm::mat4 UpDownTranslationFloor;


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



int MOUSE_OLD_X_POS = 0;
int MOUSE_OLD_Y_POS = 0;

int mouseDeltaX = 0;
int mouseDeltaY = 0;


/* ------------------------------------ */
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
float angleX, angleY, angleZ = 0.0f; 

/* Indices to active rotation axes */
enum {Xaxis=0, Yaxis=1, Zaxis=2};
int axis = Yaxis;

/* Indices to active triangle mesh */
enum {Model1=0, Model2=1};
int model = Model1; 


/* ------------------------------------ */



/* variables for computing elapsed time since last render */
int deltaTime = 0;
int oldTimeSinceStart = 0;

void computeDeltaTime();



/*
Vertex buffer for the ground box
*/
GLfloat ground_vertex_buffer_data[] = { /* 8 cube vertices XYZ */
    -GROUND_SIZE, -GROUND_HEIGHT,  GROUND_SIZE,
     GROUND_SIZE, -GROUND_HEIGHT,  GROUND_SIZE,
     GROUND_SIZE,  GROUND_HEIGHT,  GROUND_SIZE,
    -GROUND_SIZE,  GROUND_HEIGHT,  GROUND_SIZE,
    -GROUND_SIZE, -GROUND_HEIGHT, -GROUND_SIZE,
     GROUND_SIZE, -GROUND_HEIGHT, -GROUND_SIZE,
     GROUND_SIZE,  GROUND_HEIGHT, -GROUND_SIZE,
    -GROUND_SIZE,  GROUND_HEIGHT, -GROUND_SIZE,
};   

GLfloat ground_color_buffer_data[] = { /* RGB color values for 8 vertices */
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
}; 

GLushort ground_index_buffer_data[] = { /* Indices of 6*2 triangles (6 sides) */
    0, 1, 2,
    2, 3, 0,
    1, 5, 6,
    6, 2, 1,
    7, 6, 5,
    5, 4, 7,
    4, 0, 3,
    3, 7, 4,
    4, 5, 1,
    1, 0, 4,
    3, 2, 6,
    6, 7, 3,
};



/*Horse Box*/

GLfloat box1_vertex_buffer_data[] = { /* 8 cube vertices XYZ */
    -BOX1_SIZE, -BOX1_HEIGHT,  BOX1_SIZE,
     BOX1_SIZE, -BOX1_HEIGHT,  BOX1_SIZE,
     BOX1_SIZE,  BOX1_HEIGHT,  BOX1_SIZE,
    -BOX1_SIZE,  BOX1_HEIGHT,  BOX1_SIZE,
    -BOX1_SIZE, -BOX1_HEIGHT, -BOX1_SIZE,
     BOX1_SIZE, -BOX1_HEIGHT, -BOX1_SIZE,
     BOX1_SIZE,  BOX1_HEIGHT, -BOX1_SIZE,
    -BOX1_SIZE,  BOX1_HEIGHT, -BOX1_SIZE,
};   

GLfloat box1_color_buffer_data[] = { /* RGB color values for 8 vertices */
    0.0, 0.3, 0.0,
    0.0, 0.3, 0.0,
    0.0, 0.3, 0.0,
    0.0, 0.3, 0.0,
    0.0, 0.3, 0.0,
    0.0, 0.3, 0.0,
    0.0, 0.3, 0.0,
    0.0, 0.3, 0.0,
}; 

GLushort box1_index_buffer_data[] = { /* Indices of 6*2 triangles (6 sides) */
    0, 1, 2,
    2, 3, 0,
    1, 5, 6,
    6, 2, 1,
    7, 6, 5,
    5, 4, 7,
    4, 0, 3,
    3, 7, 4,
    4, 5, 1,
    1, 0, 4,
    3, 2, 6,
    6, 7, 3,
};

/*PILLAR*/

GLfloat pillar_vertex_buffer_data[] = { /* 8 cube vertices XYZ */
    -PILLAR_SIZE, -PILLAR_HEIGHT,  PILLAR_SIZE,
     PILLAR_SIZE, -PILLAR_HEIGHT,  PILLAR_SIZE,
     PILLAR_SIZE,  PILLAR_HEIGHT,  PILLAR_SIZE,
    -PILLAR_SIZE,  PILLAR_HEIGHT,  PILLAR_SIZE,
    -PILLAR_SIZE, -PILLAR_HEIGHT, -PILLAR_SIZE,
     PILLAR_SIZE, -PILLAR_HEIGHT, -PILLAR_SIZE,
     PILLAR_SIZE,  PILLAR_HEIGHT, -PILLAR_SIZE,
    -PILLAR_SIZE,  PILLAR_HEIGHT, -PILLAR_SIZE,
};   

GLfloat pillar_color_buffer_data[] = { /* RGB color values for 8 vertices */
    0.5, 0.0, 0.0,
    0.5, 0.0, 0.0,
    0.5, 0.0, 0.0,
    0.5, 0.0, 0.0,
    0.5, 0.0, 0.0,
    0.5, 0.0, 0.0,
    0.5, 0.0, 0.0,
    0.5, 0.0, 0.0,
}; 

GLushort pillar_index_buffer_data[] = { /* Indices of 6*2 triangles (6 sides) */
    0, 1, 2,
    2, 3, 0,
    1, 5, 6,
    6, 2, 1,
    7, 6, 5,
    5, 4, 7,
    4, 0, 3,
    3, 7, 4,
    4, 5, 1,
    1, 0, 4,
    3, 2, 6,
    6, 7, 3,
};


/*
Vertex buffer for the wall box
*/
GLfloat wall_vertex_buffer_data[] = { /* 8 cube vertices XYZ */
    -WALL_SIZE, -WALL_HEIGHT,  WALL_SIZE,
     WALL_SIZE, -WALL_HEIGHT,  WALL_SIZE,
     WALL_SIZE,  WALL_HEIGHT,  WALL_SIZE,
    -WALL_SIZE,  WALL_HEIGHT,  WALL_SIZE,
    -WALL_SIZE, -WALL_HEIGHT, -WALL_SIZE,
     WALL_SIZE, -WALL_HEIGHT, -WALL_SIZE,
     WALL_SIZE,  WALL_HEIGHT, -WALL_SIZE,
    -WALL_SIZE,  WALL_HEIGHT, -WALL_SIZE,
};   

GLfloat wall_color_buffer_data[] = { /* RGB color values for 8 vertices */
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
}; 

GLushort wall_index_buffer_data[] = { /* Indices of 6*2 triangles (6 sides) */
    0, 1, 2,
    2, 3, 0,
    1, 5, 6,
    6, 2, 1,
    7, 6, 5,
    5, 4, 7,
    4, 0, 3,
    3, 7, 4,
    4, 5, 1,
    1, 0, 4,
    3, 2, 6,
    6, 7, 3,
};


/*----------------------------------------------------------------*/


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


void DrawObject(GLuint VBO, GLuint CBO, GLuint IBO, glm::mat4 pvm){
    

    glEnableVertexAttribArray(vPosition);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

    
    glEnableVertexAttribArray(vColor);
    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glVertexAttribPointer(vColor, 3, GL_FLOAT,GL_FALSE, 0, 0);   
    
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    
    GLint size; 
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    
 /* Associate program with shader matrices */
    GLint PVMMatrixID = glGetUniformLocation(ShaderProgram, "ProjectionViewModelMatrix");
    if (PVMMatrixID == -1) 
    {
        fprintf(stderr, "Could not bind uniform ProjectionViewModelMatrix\n");
        exit(-1);
    }
    glUniformMatrix4fv(PVMMatrixID, 1, GL_FALSE, glm::value_ptr(pvm));  
    
    /*	-------------------------------------------------------------------------- */
    /* Set state to only draw wireframe (no lighting used, yet) */
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /* Issue draw command, using indexed triangle list */
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

    /* Issue draw command, using indexed triangle list */
    //glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
    /*	--------------------------------------------------------------------------- */
    
    /* Disable attributes */
    glDisableVertexAttribArray(vPosition);
    glDisableVertexAttribArray(vColor);   
    
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
    
    DrawObject(GROUND_VBO, GROUND_CBO, GROUND_IBO, PVMMatrixGround);
    DrawObject(GROUND_VBO, GROUND_CBO, GROUND_IBO, PVMMatrixRoof);
    DrawObject(PILLAR_VBO, PILLAR_CBO, PILLAR_IBO, PVMMatrixPillar);
    
    /* Walls and Floor*/
    DrawObject(WALL_VBO, WALL_CBO, WALL_IBO, PVMMatrixFloor);
    DrawObject(WALL_VBO, WALL_CBO, WALL_IBO, PVMMatrixWall1);
    DrawObject(WALL_VBO, WALL_CBO, WALL_IBO, PVMMatrixWall2);
    DrawObject(WALL_VBO, WALL_CBO, WALL_IBO, PVMMatrixWall3);

    DrawObject(SUZANNE_VBO, HORSEBOX_CBO, SUZANNE_IBO, PVMMatrixSuzanne1);
    DrawObject(SUZANNE_VBO, HORSEBOX_CBO, SUZANNE_IBO, PVMMatrixSuzanne2);
    DrawObject(SUZANNE_VBO, HORSEBOX_CBO, SUZANNE_IBO, PVMMatrixSuzanne3);
    DrawObject(SUZANNE_VBO, HORSEBOX_CBO, SUZANNE_IBO, PVMMatrixSuzanne4);


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
	// anim = GL_TRUE;
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
	/* --------------------------------------- */
	// keys to manipulate the camera
	/* Activate camera mode fixed or free */
	case '1': 
		cameraMode = CAMERA_FIXED_MOVE;
		anim = GL_TRUE;
		break;

	case '2':
		cameraMode = CAMERA_FREE_MOVE;
		anim = GL_FALSE;
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
     
    if(anim) {
        
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
    
    
	/* SetUp Rotation matrices */
    float angle = (glutGet(GLUT_ELAPSED_TIME) / 1000.0) * (180.0/M_PI);
    
    angle *= rotation_speed_factor;
    angle *= rotation_direction;
   

    glm::mat4 RotationMatrixAnimGround;
    glm::mat4 RotationMatrixAnimPillar;
    glm::mat4 RotationMatrixAnimRoof;
    glm::mat4 RotationMatrixAnimBox1;
    glm::mat4 RotationMatrixAnimBox2;
    glm::mat4 RotationMatrixAnimBox3;
    glm::mat4 RotationMatrixAnimBox4;


    
    /* Time dependent rotation */
    RotationMatrixAnimGround = glm::rotate(glm::mat4(1.0f), -angle/2, glm::vec3(0.0f, 1.0f, 0.0f));
    RotationMatrixAnimRoof = glm::rotate(glm::mat4(1.0f), -angle/2, glm::vec3(0.0f, 1.0f, 0.0f));
    RotationMatrixAnimPillar = glm::rotate(glm::mat4(1.0f), -angle/2, glm::vec3(0.0f, 1.0f, 0.0f));
    RotationMatrixAnimBox1 = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
    RotationMatrixAnimBox2 = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
    RotationMatrixAnimBox3 = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
    RotationMatrixAnimBox4 = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));    

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

    

    ModelMatrixGround = TranslateDownGround * RotationMatrixAnimGround * InitialTransformGround;
    ModelMatrixRoof = TranslateDownRoof * RotationMatrixAnimRoof * InitialTransformRoof;
    ModelMatrixPillar = TranslateDownPillar * RotationMatrixAnimPillar * InitialTransformPillar;
    

    /* Apply model rotation; finally move cube down */    
    SuzanneMatrix1 = TranslateDownBox1 * UpDownTranslationBox1 * RotationMatrixAnimBox1 * InitialTransformBox1;
    SuzanneMatrix2 = TranslateDownBox2 * UpDownTranslationBox2 * RotationMatrixAnimBox2 * InitialTransformBox2;
    SuzanneMatrix3 = TranslateDownBox3 * UpDownTranslationBox3 * RotationMatrixAnimBox3 * InitialTransformBox3;
    SuzanneMatrix4 = TranslateDownBox4 * UpDownTranslationBox4 * RotationMatrixAnimBox4 * InitialTransformBox4;

    
    
     /* Set up single transformation matrix for complete transformation 
       from model to screen space */
    PVMMatrixGround = ProjectionMatrix * ViewMatrix * ModelMatrixGround;
    PVMMatrixRoof = ProjectionMatrix * ViewMatrix * ModelMatrixRoof;
    PVMMatrixPillar = ProjectionMatrix * ViewMatrix * ModelMatrixPillar;
    
        
    PVMMatrixFloor = ProjectionMatrix * ViewMatrix * ModelMatrixFloor;
    PVMMatrixWall1 = ProjectionMatrix * ViewMatrix * ModelMatrixWall1;
    PVMMatrixWall2 = ProjectionMatrix * ViewMatrix * ModelMatrixWall2;
    PVMMatrixWall3 = ProjectionMatrix * ViewMatrix * ModelMatrixWall3;
    
    
    PVMMatrixSuzanne1 = ProjectionMatrix * ViewMatrix * SuzanneMatrix1;
    PVMMatrixSuzanne2 = ProjectionMatrix * ViewMatrix * SuzanneMatrix2;
    PVMMatrixSuzanne3 = ProjectionMatrix * ViewMatrix * SuzanneMatrix3;
    PVMMatrixSuzanne4 = ProjectionMatrix * ViewMatrix * SuzanneMatrix4;


    
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
    
  
    /*Ground Shape */
    glGenBuffers(1, &GROUND_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, GROUND_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ground_vertex_buffer_data), ground_vertex_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &GROUND_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GROUND_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ground_index_buffer_data), ground_index_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &GROUND_CBO);
    glBindBuffer(GL_ARRAY_BUFFER, GROUND_CBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ground_color_buffer_data), ground_color_buffer_data, GL_STATIC_DRAW);
    

    /* Horse shape*/
    /* */
    glGenBuffers(1, &HORSEBOX_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, HORSEBOX_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(box1_vertex_buffer_data), box1_vertex_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &HORSEBOX_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, HORSEBOX_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(box1_index_buffer_data), box1_index_buffer_data, GL_STATIC_DRAW);
    /* */
    glGenBuffers(1, &HORSEBOX_CBO);
    glBindBuffer(GL_ARRAY_BUFFER, HORSEBOX_CBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(box1_color_buffer_data), box1_color_buffer_data, GL_STATIC_DRAW);
      
    
    /* Pillar shape*/
    glGenBuffers(1, &PILLAR_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, PILLAR_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pillar_vertex_buffer_data), pillar_vertex_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &PILLAR_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, PILLAR_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pillar_index_buffer_data), pillar_index_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &PILLAR_CBO);
    glBindBuffer(GL_ARRAY_BUFFER, PILLAR_CBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pillar_color_buffer_data), pillar_color_buffer_data, GL_STATIC_DRAW);
    
    
    /* WALL shape*/
    glGenBuffers(1, &WALL_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, WALL_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wall_vertex_buffer_data), wall_vertex_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &WALL_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, WALL_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(wall_index_buffer_data), wall_index_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &WALL_CBO);
    glBindBuffer(GL_ARRAY_BUFFER, WALL_CBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wall_color_buffer_data), wall_color_buffer_data, GL_STATIC_DRAW);
    
    
 /* -------------------------------------------------------------------------*/
 /*         Added for exercise 2                                             */
   
    glGenBuffers(1, &SUZANNE_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, SUZANNE_VBO);
    glBufferData(GL_ARRAY_BUFFER, suzanne_data.vertex_count*3*sizeof(GLfloat), vertex_buffer_suzanne, GL_STATIC_DRAW);

    
    glGenBuffers(1, &SUZANNE_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SUZANNE_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, suzanne_data.face_count*3*sizeof(GLuint), index_buffer_suzanne, GL_STATIC_DRAW);
 
 /* -------------------------------------------------------------------------*/   
    
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
    VertexShaderString = LoadShader("shaders/vertexshader.vs");
    FragmentShaderString = LoadShader("shaders/fragmentshader.fs");

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
* Initialize
*
* This function is called to initialize rendering elements, setup
* vertex buffer objects, and to setup the vertex and fragment shader
*
*******************************************************************/

void Initialize(void)
{   
    int i;
    int success;
    
    /* Load suzanne */
    fprintf(stderr, "Before");
    char* filename_suzanne = (char*) "models/suzanne.obj";
    fprintf(stderr, "After1");
    success = parse_obj_scene(&suzanne_data, filename_suzanne);
    fprintf(stderr, "After2");
    if(!success)
        printf("Could not load file 'suzanne'. Exiting. \n");
    
    /*  Copy mesh data from structs into appropriate arrays */ 
    int vert = suzanne_data.vertex_count;
    int indx = suzanne_data.face_count;

    vertex_buffer_suzanne = (GLfloat*) calloc (vert*3, sizeof(GLfloat));
    index_buffer_suzanne = (GLuint*) calloc (indx*3, sizeof(GLuint));
  
    /* Vertices */
    for(i=0; i<vert; i++)
    {
        vertex_buffer_suzanne[i*3] = (GLfloat)(suzanne_data.vertex_list[i])->e[0];
	vertex_buffer_suzanne[i*3+1] = (GLfloat)(suzanne_data.vertex_list[i])->e[1];
	vertex_buffer_suzanne[i*3+2] = (GLfloat)(suzanne_data.vertex_list[i])->e[2];
    }

    /* Indices */
    for(i=0; i<indx; i++)
    {
	index_buffer_suzanne[i*3] = (GLuint)(suzanne_data.face_list[i])->vertex_index[0];
	index_buffer_suzanne[i*3+1] = (GLuint)(suzanne_data.face_list[i])->vertex_index[1];
	index_buffer_suzanne[i*3+2] = (GLuint)(suzanne_data.face_list[i])->vertex_index[2];
    }
    
    /* Set background (clear) color to white */ 
    glClearColor(1.0, 1.0, 1.0, 0.0);

    /* Enable depth testing */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);    

    /*Intel troubleshooting*/
    setupArrayObject();

    /* Setup vertex, color, and index buffer objects */
    SetupDataBuffers();

    /* Setup shaders and shader program */
    CreateShaderProgram();

    /* Initialize matrices */
    

    ModelMatrixGround = glm::mat4(1.0f);
    ModelMatrixPillar = glm::mat4(1.0f);
    ModelMatrixRoof = glm::mat4(1.0f);
   
    ModelMatrixFloor = glm::mat4(1.0f);
    ModelMatrixWall1 = glm::mat4(1.0f);
    ModelMatrixWall2 = glm::mat4(1.0f);
    ModelMatrixWall3 = glm::mat4(1.0f);
    
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


    /* Translate and rotate ground onto tip */
    TranslateOriginGround = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,0));	
    TranslateOriginRoof = glm::translate(glm::mat4(1.0f), glm::vec3(0,4.2,0));
    TranslateOriginPillar = glm::translate(glm::mat4(1.0f), glm::vec3(0,2,0));
    
    /* Walls and Floor*/
    //floor
    int floor_y = -3;
    TranslateOriginFloor = glm::translate(glm::mat4(1.0f), glm::vec3(0,floor_y,0));
    
    // left wall
    TranslateOriginWall1 = glm::translate(glm::mat4(1.0f), glm::vec3(-floor_y, -WALL_SIZE, 0));
    RotateZWall1 = glm::rotate(glm::mat4(1.0f), (float)(90.0f*(2 * M_PI / 360)), glm::vec3(0.0f, 0.0f, 1.0f));
         
    // right wall
    TranslateOriginWall2 = glm::translate(glm::mat4(1.0f), glm::vec3(-floor_y, WALL_SIZE, 0));
    RotateZWall2 = glm::rotate(glm::mat4(1.0f), (float)(90.0f*(2 * M_PI / 360)), glm::vec3(0.0f, 0.0f, 1.0f));
    
    // back wall
    TranslateOriginWall3 = glm::translate(glm::mat4(1.0f), glm::vec3(0, -WALL_SIZE, floor_y));
    RotateXWall3 = glm::rotate(glm::mat4(1.0f), (float)(90.0f*(2 * M_PI / 360)), glm::vec3(1.0f, 0.0f, 0.0f));
        
    
    TranslateOriginBox1 = glm::translate(glm::mat4(1.0f), glm::vec3(2, BOX1_START_POSITION_Y, 2));
    TranslateOriginBox2 = glm::translate(glm::mat4(1.0f), glm::vec3(-2, BOX2_START_POSITION_Y, -2));
    TranslateOriginBox3 = glm::translate(glm::mat4(1.0f), glm::vec3(-2, BOX3_START_POSITION_Y, 2));
    TranslateOriginBox4 = glm::translate(glm::mat4(1.0f), glm::vec3(2, BOX4_START_POSITION_Y, -2));

   
    /* Translate down */
    TranslateDownGround = glm::translate(glm::mat4(1.0f), glm::vec3(0, -sqrtf(sqrtf(2.0) * 1.0), 0 ));
    TranslateDownPillar = glm::translate(glm::mat4(1.0f), glm::vec3(0, -sqrtf(sqrtf(2.0) * 1.0), 0 ));
    TranslateDownRoof = glm::translate(glm::mat4(1.0f), glm::vec3(0, -sqrtf(sqrtf(2.0) * 1.0), 0 ));
    TranslateDownBox1 = glm::translate(glm::mat4(1.0f), glm::vec3(0, -sqrtf(sqrtf(2.0) * 1.0), 0 ));
    TranslateDownBox2 = glm::translate(glm::mat4(1.0f), glm::vec3(0, -sqrtf(sqrtf(2.0) * 1.0), 0 ));
    TranslateDownBox3 = glm::translate(glm::mat4(1.0f), glm::vec3(0, -sqrtf(sqrtf(2.0) * 1.0), 0 ));
    TranslateDownBox4 = glm::translate(glm::mat4(1.0f), glm::vec3(0, -sqrtf(sqrtf(2.0) * 1.0), 0 ));

   

    InitialTransformGround = RotateZGround * RotateXGround * TranslateOriginGround;
    InitialTransformPillar = RotateZPillar * RotateXPillar * TranslateOriginPillar;
    InitialTransformRoof = RotateZRoof * RotateXRoof * TranslateOriginRoof;

    
    ModelMatrixFloor =  RotateZFloor * RotateXFloor  * InitialTransformFloor * TranslateOriginFloor;
    ModelMatrixWall1 = RotateZWall1 * RotateXWall1 * InitialTransformWall1 * TranslateOriginWall1;
    ModelMatrixWall2 = RotateZWall2 * RotateXWall2 * InitialTransformWall2 * TranslateOriginWall2;
    ModelMatrixWall3 = RotateZWall3 * RotateXWall3 * InitialTransformWall3* TranslateOriginWall3 ;

        
    PVMMatrixFloor = ProjectionMatrix * ViewMatrix * ModelMatrixFloor;
    PVMMatrixWall1 = ProjectionMatrix * ViewMatrix * ModelMatrixWall1;
    PVMMatrixWall2 = ProjectionMatrix * ViewMatrix * ModelMatrixWall2;
    PVMMatrixWall3 = ProjectionMatrix * ViewMatrix * ModelMatrixWall3;
    
    /* Walls and Floor*/
    InitialTransformBox1 = RotateZBox1 * RotateXBox1 * TranslateOriginBox1;
    InitialTransformBox2 = RotateZBox2 * RotateXBox2 * TranslateOriginBox2;
    InitialTransformBox3 = RotateZBox3 * RotateXBox3 * TranslateOriginBox3;
    InitialTransformBox4 = RotateZBox4 * RotateXBox4 * TranslateOriginBox4;

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
