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

#define GROUND_SIZE 3.0
#define GROUND_HEIGHT 0.2

#define BOX1_SIZE 0.3
#define BOX1_HEIGHT 1.0
    
#define PILLAR_SIZE 0.5
#define PILLAR_HEIGHT 2


/* Standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* OpenGL includes */
#include <GL/glew.h>
#include <GL/freeglut.h>


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
GLushort *index_buffer_suzanne;

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



float ProjectionMatrix[16]; /* Perspective projection matrix */
float ViewMatrix[16]; /* Camera view matrix */ 
float camera_disp = -25.0;
float camera_aproach = 10.0;

const int CAMERA_FREE_MOVE = 1;
const int CAMERA_FIXED_MOVE = 0;
int cameraMode = 0;



float ModelMatrixGround[16];	/* Model matrix for the ground layer */
float ModelMatrixRoof[16];	/* Model matrix for the roof (top layer) */
float ModelMatrixPillar[16];	/* Model matrix for the pillar */

float ModelMatrixFloor[16]; /* Model matrix for the floor entity*/
float ModelMatrixWall1[16]; /* Model matrix for the wall entity*/
float ModelMatrixWall2[16]; /* Model matrix for the wall entity*/
float ModelMatrixWall3[16]; /* Model matrix for the wall entity*/


float ModelMatrixBox1[16]; /*Model matrix for box1*/
float ModelMatrixBox2[16]; /*Model matrix for box2*/
float ModelMatrixBox3[16]; /*Model matrix for box3*/
float ModelMatrixBox4[16]; /*Model matrix for box4*/

float SuzanneMatrix1[16];
float SuzanneMatrix2[16];
float SuzanneMatrix3[16];
float SuzanneMatrix4[16];



/* Transformation matrices for initial position */

float TranslateOriginRoof[16];
float TranslateDownRoof[16];
float RotateXRoof[16];
float RotateZRoof[16];
float InitialTransformRoof[16];

float TranslateOriginPillar[16];
float TranslateDownPillar[16];
float RotateXPillar[16];
float RotateZPillar[16];
float InitialTransformPillar[16];

float TranslateOriginGround[16];
float TranslateDownGround[16];
float RotateXGround[16];
float RotateZGround[16];
float InitialTransformGround[16];

float TranslateOriginBox1[16];
float TranslateDownBox1[16];
float RotateXBox1[16];
float RotateZBox1[16];
float InitialTransformBox1[16];
float UpDownTranslationBox1[16];

float TranslateOriginBox2[16];
float TranslateDownBox2[16];
float RotateXBox2[16];
float RotateZBox2[16];
float InitialTransformBox2[16];
float UpDownTranslationBox2[16];

float TranslateOriginBox3[16];
float TranslateDownBox3[16];
float RotateXBox3[16];
float RotateZBox3[16];
float InitialTransformBox3[16];
float UpDownTranslationBox3[16];

float TranslateOriginBox4[16];
float TranslateDownBox4[16];
float RotateXBox4[16];
float RotateZBox4[16];
float InitialTransformBox4[16];
float UpDownTranslationBox4[16];

/* Walls and Ground */

float TranslateOriginWall1[16];
float TranslateDownWall1[16];
float RotateXWall1[16];
float RotateYWall1[16];
float RotateZWall1[16];
float InitialTransformWall1[16];
float UpDownTranslationWall1[16];

float TranslateOriginWall2[16];
float TranslateDownWall2[16];
float RotateXWall2[16];
float RotateZWall2[16];
float RotateYWall2[16];
float InitialTransformWall2[16];
float UpDownTranslationWall2[16];

float TranslateOriginWall3[16];
float TranslateDownWall3[16];
float RotateXWall3[16];
float RotateZWall3[16];
float RotateYWall3[16];
float InitialTransformWall3[16];
float UpDownTranslationWall3[16];

float TranslateOriginFloor[16];
float TranslateDownFloor[16];
float RotateXFloor[16];
float RotateZFloor[16];
float InitialTransformFloor[16];
float UpDownTranslationFloor[16];


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
float rotation_speed_factor = 1.0;
int rotation_direction = 1;

float updown_speed_factor = 1.0;

float zoom = 1.0;



/* Transformation matrices for camera rotation */
float RotationMatrixCameraX[16];
float RotationMatrixCameraY[16];
float RotationMatrixCameraZ[16];
float RotationMatrixCamera[16];

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
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
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


void DrawObject(GLuint VBO, GLuint CBO, GLuint IBO, float ModelMatrix[16]){
    

    glEnableVertexAttribArray(vPosition);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

    /*
    glEnableVertexAttribArray(vColor);
    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glVertexAttribPointer(vColor, 3, GL_FLOAT,GL_FALSE, 0, 0);   
    */
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    
    GLint size; 
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

    /* Associate program with shader matrices */
    GLint projectionUniform = glGetUniformLocation(ShaderProgram, "ProjectionMatrix");
    if (projectionUniform == -1) 
    {
        fprintf(stderr, "Could not bind uniform ProjectionMatrix\n");
	exit(-1);
    }
    glUniformMatrix4fv(projectionUniform, 1, GL_TRUE, ProjectionMatrix);
    
    GLint ViewUniform = glGetUniformLocation(ShaderProgram, "ViewMatrix");
    if (ViewUniform == -1) 
    {
        fprintf(stderr, "Could not bind uniform ViewMatrix\n");
        exit(-1);
    }
    glUniformMatrix4fv(ViewUniform, 1, GL_TRUE, ViewMatrix);
   
    GLint RotationUniform = glGetUniformLocation(ShaderProgram, "ModelMatrix");
    if (RotationUniform == -1) 
    {
        fprintf(stderr, "Could not bind uniform ModelMatrix\n");
        exit(-1);
    }
    glUniformMatrix4fv(RotationUniform, 1, GL_TRUE, ModelMatrix);  
    
    /*	-------------------------------------------------------------------------- */
    /* Set state to only draw wireframe (no lighting used, yet) */
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /* Issue draw command, using indexed triangle list */
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

    /* Issue draw command, using indexed triangle list */
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
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
    
    DrawObject(GROUND_VBO, GROUND_CBO, GROUND_IBO, ModelMatrixGround);
    DrawObject(GROUND_VBO, GROUND_CBO, GROUND_IBO, ModelMatrixRoof);
    DrawObject(PILLAR_VBO, PILLAR_CBO, PILLAR_IBO, ModelMatrixPillar);
    DrawObject(HORSEBOX_VBO, HORSEBOX_CBO, HORSEBOX_IBO, ModelMatrixBox1);
    
    /* Walls and Floor*/
    DrawObject(WALL_VBO, WALL_CBO, WALL_IBO, ModelMatrixFloor);
    DrawObject(WALL_VBO, WALL_CBO, WALL_IBO, ModelMatrixWall1);
    DrawObject(WALL_VBO, WALL_CBO, WALL_IBO, ModelMatrixWall2);
    DrawObject(WALL_VBO, WALL_CBO, WALL_IBO, ModelMatrixWall3);


    
    //DrawObject(BOX1_VBO, BOX1_CBO, BOX1_IBO, SuzanMatrix);
    DrawObject(HORSEBOX_VBO, HORSEBOX_CBO, HORSEBOX_IBO, ModelMatrixBox2);
    DrawObject(HORSEBOX_VBO, HORSEBOX_CBO, HORSEBOX_IBO, ModelMatrixBox3);
    DrawObject(HORSEBOX_VBO, HORSEBOX_CBO, HORSEBOX_IBO, ModelMatrixBox4);
    
    DrawObject(SUZANNE_VBO, HORSEBOX_CBO, SUZANNE_IBO, SuzanneMatrix1);
    DrawObject(SUZANNE_VBO, HORSEBOX_CBO, SUZANNE_IBO, SuzanneMatrix2);
    DrawObject(SUZANNE_VBO, HORSEBOX_CBO, SUZANNE_IBO, SuzanneMatrix3);
    DrawObject(SUZANNE_VBO, HORSEBOX_CBO, SUZANNE_IBO, SuzanneMatrix4);


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
    float correction_factor = 1 / 10;
    
    mouseDeltaX = x - MOUSE_OLD_X_POS;
    mouseDeltaY = y - MOUSE_OLD_Y_POS;
    
    mouseDeltaX *= correction_factor;
    mouseDeltaY *= correction_factor;
    
    //printf("x: %i, y: %i\n",mouseDeltaX, mouseDeltaY );
    
    if(state == GLUT_DOWN) 
    {
      /* Depending on button pressed, set rotation axis,
       * turn on animation */
        switch(button) 
	{
	    case GLUT_LEFT_BUTTON:    
	        //axis = Xaxis;
		break;

	    case GLUT_MIDDLE_BUTTON:  
  	        //axis = Yaxis;
	        break;
		
	    case GLUT_RIGHT_BUTTON: 
	       // axis = Zaxis;
		zoom = 1.0;
		break;
	}
	//anim = GL_TRUE;
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
    float correction_factor = 1 / 10;
    
    mouseDeltaX = x - MOUSE_OLD_X_POS;
    mouseDeltaY = y - MOUSE_OLD_Y_POS;
    
    mouseDeltaX *= correction_factor;
    mouseDeltaY *= correction_factor;
    
   // printf("x: %i, y: %i\n",mouseDeltaX, mouseDeltaY );
    
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
	    if (rotation_speed_factor < 2) rotation_speed_factor += 0.1;
	    if (updown_speed_factor < 2) updown_speed_factor += 0.1;
	    break;
	    
	case 's':
	    if (rotation_speed_factor > 0) rotation_speed_factor -= 0.1;
	    if (updown_speed_factor > 0) updown_speed_factor -= 0.1;
	    break;
	case 'a':
	    rotation_direction = 1;
	    break;
	    
	case 'd':
	    rotation_direction = -1;
	    break;
	/* --------------------------------------- */
	// keys to manipulate the camera
	/* Activate camera mode fixed or free */
	case '1': 
		cameraMode = CAMERA_FIXED_MOVE;
		break;

	case '2':
		cameraMode = CAMERA_FREE_MOVE;	
		break;

	/* Toggle animation */
	case '0':
		/*if (anim)
			anim = GL_FALSE;		
		else
			anim = GL_TRUE;*/
		break;

	/* Reset initial rotation of object */
	/*
	 case 'o':
	    SetIdentityMatrix(RotationMatrixAnimX);
	    SetIdentityMatrix(RotationMatrixAnimY);
	    SetIdentityMatrix(RotationMatrixAnimZ);
	    angleX = 0.0;
	    angleY = 0.0;
	    angleZ = 0.0;
	    break;
	 */   
	 case 'o':
    
	  BOX1_CURRENT_POSITION_Y = BOX1_START_POSITION_Y;
	  BOX2_CURRENT_POSITION_Y = BOX2_START_POSITION_Y;
	  BOX3_CURRENT_POSITION_Y = BOX3_START_POSITION_Y;
	  BOX4_CURRENT_POSITION_Y = BOX4_START_POSITION_Y;
	  
	  rotation_speed_factor = 1.0;
	  rotation_direction = 1;
	  updown_speed_factor = 1.0;
	  
	  break;
	  
	case 'q': case 'Q':  
	    exit(0);    
		break;
    }

    glutPostRedisplay();
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
        float RotationMatrixAnimMouseX[16];
        float RotationMatrixAnimMouseY[16];
        float RotationMatrixAnimMouseZ[16];
	
	
	
	/* ------------------------------------------ */
	 // SetIdentityMatrix(RotationMatrixAnimMouseZ);
	/* ------------------------------------------ */
	float rotatey = mouseDeltaY / 180 * 3.141592654f;
        SetRotationX(-mouseDeltaY, RotationMatrixAnimMouseX);
        SetRotationY(-mouseDeltaX, RotationMatrixAnimMouseY);
        SetRotationZ(0, RotationMatrixAnimMouseZ);
        
        MultiplyMatrix(RotationMatrixAnimMouseX, ViewMatrix, ViewMatrix);
        MultiplyMatrix(RotationMatrixAnimMouseY, ViewMatrix, ViewMatrix);
        MultiplyMatrix(RotationMatrixAnimMouseZ, ViewMatrix, ViewMatrix);
    }// 1 4 5
    else {
     // empty
    }
    
    
	/* SetUp Rotation matrices */
    float angle = (glutGet(GLUT_ELAPSED_TIME) / 1000.0) * (180.0/M_PI);
    
    angle *= rotation_speed_factor;
    angle *= rotation_direction;
   

    float RotationMatrixAnimGround[16];
    float RotationMatrixAnimPillar[16];
    float RotationMatrixAnimRoof[16];
    float RotationMatrixAnimBox1[16];
    float RotationMatrixAnimBox2[16];
    float RotationMatrixAnimBox3[16];
    float RotationMatrixAnimBox4[16];


    
    /* Time dependent rotation */
    SetRotationY(-angle/2, RotationMatrixAnimGround);
    SetRotationY(-angle/2, RotationMatrixAnimRoof);
    SetRotationY(-angle/2, RotationMatrixAnimPillar);
    SetRotationY(angle, RotationMatrixAnimBox1);
    SetRotationY(angle, RotationMatrixAnimBox2);
    SetRotationY(angle, RotationMatrixAnimBox3);
    SetRotationY(angle, RotationMatrixAnimBox4);

    

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
    
    SetTranslation(0, BOX1_CURRENT_POSITION_Y, 0, UpDownTranslationBox1);
    SetTranslation(0, BOX2_CURRENT_POSITION_Y, 0, UpDownTranslationBox2);
    SetTranslation(0, BOX3_CURRENT_POSITION_Y, 0, UpDownTranslationBox3);
    SetTranslation(0, BOX4_CURRENT_POSITION_Y, 0, UpDownTranslationBox4);

    

    

    /* Apply model rotation; finally move cube down */
    MultiplyMatrix(RotationMatrixAnimGround, InitialTransformGround, ModelMatrixGround);
    MultiplyMatrix(TranslateDownGround, ModelMatrixGround, ModelMatrixGround);
    
    MultiplyMatrix(RotationMatrixAnimRoof, InitialTransformRoof, ModelMatrixRoof);
    MultiplyMatrix(TranslateDownRoof, ModelMatrixRoof, ModelMatrixRoof);
    
    MultiplyMatrix(RotationMatrixAnimPillar, InitialTransformPillar, ModelMatrixPillar);
    MultiplyMatrix(TranslateDownPillar, ModelMatrixPillar, ModelMatrixPillar);
    
    

    MultiplyMatrix(RotationMatrixAnimBox1, InitialTransformBox1, ModelMatrixBox1);
    MultiplyMatrix(UpDownTranslationBox1, ModelMatrixBox1, ModelMatrixBox1);
    MultiplyMatrix(TranslateDownBox1, ModelMatrixBox1, ModelMatrixBox1);
    
    MultiplyMatrix(RotationMatrixAnimBox2, InitialTransformBox2, ModelMatrixBox2);
    MultiplyMatrix(UpDownTranslationBox2, ModelMatrixBox2, ModelMatrixBox2);
    MultiplyMatrix(TranslateDownBox2, ModelMatrixBox2, ModelMatrixBox2);
    
    MultiplyMatrix(RotationMatrixAnimBox3, InitialTransformBox3, ModelMatrixBox3);
    MultiplyMatrix(UpDownTranslationBox3, ModelMatrixBox3, ModelMatrixBox3);
    MultiplyMatrix(TranslateDownBox3, ModelMatrixBox3, ModelMatrixBox3);
    
    MultiplyMatrix(RotationMatrixAnimBox4, InitialTransformBox4, ModelMatrixBox4);
    MultiplyMatrix(UpDownTranslationBox4, ModelMatrixBox4, ModelMatrixBox4);
    MultiplyMatrix(TranslateDownBox4, ModelMatrixBox4, ModelMatrixBox4);
    
    
    /* ---------------------------------------------------------------------------- */
    /* 		Added for exercise 2						    */
    MultiplyMatrix(RotationMatrixAnimBox1, InitialTransformBox1, SuzanneMatrix1);
    MultiplyMatrix(UpDownTranslationBox1, SuzanneMatrix1, SuzanneMatrix1);
    MultiplyMatrix(TranslateDownBox1, SuzanneMatrix1, SuzanneMatrix1);
    
    MultiplyMatrix(RotationMatrixAnimBox2, InitialTransformBox2, SuzanneMatrix2);
    MultiplyMatrix(UpDownTranslationBox2, SuzanneMatrix2, SuzanneMatrix2);
    MultiplyMatrix(TranslateDownBox2, SuzanneMatrix2, SuzanneMatrix2);
    
    MultiplyMatrix(RotationMatrixAnimBox3, InitialTransformBox3, SuzanneMatrix3);
    MultiplyMatrix(UpDownTranslationBox3, SuzanneMatrix3, SuzanneMatrix3);
    MultiplyMatrix(TranslateDownBox3, SuzanneMatrix3, SuzanneMatrix3);
    
    MultiplyMatrix(RotationMatrixAnimBox4, InitialTransformBox4, SuzanneMatrix4);
    MultiplyMatrix(UpDownTranslationBox4, SuzanneMatrix4, SuzanneMatrix4);
    MultiplyMatrix(TranslateDownBox4, SuzanneMatrix4, SuzanneMatrix4);
    
    
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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, suzanne_data.face_count*3*sizeof(GLushort), index_buffer_suzanne, GL_STATIC_DRAW);
 
 /*
    glGenBuffers(1, &BOX1_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, suzanne_data.vertex_count*3*sizeof(GLfloat), vertex_buffer_suzanne, GL_STATIC_DRAW);

    
    glGenBuffers(1, &BOX1_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, suzanne_data.face_count*3*sizeof(GLushort), index_buffer_suzanne, GL_STATIC_DRAW);
 */
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
    char* filename_suzanne = "models/suzanne.obj";
    success = parse_obj_scene(&suzanne_data, filename_suzanne);
    
    if(!success)
        printf("Could not load file 'suzanne'. Exiting. \n");
    
    /*  Copy mesh data from structs into appropriate arrays */ 
    int vert = suzanne_data.vertex_count;
    int indx = suzanne_data.face_count;

    vertex_buffer_suzanne = (GLfloat*) calloc (vert*3, sizeof(GLfloat));
    index_buffer_suzanne = (GLushort*) calloc (indx*3, sizeof(GLushort));
  
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
	index_buffer_suzanne[i*3] = (GLushort)(suzanne_data.face_list[i])->vertex_index[0];
	index_buffer_suzanne[i*3+1] = (GLushort)(suzanne_data.face_list[i])->vertex_index[1];
	index_buffer_suzanne[i*3+2] = (GLushort)(suzanne_data.face_list[i])->vertex_index[2];
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
    SetIdentityMatrix(ProjectionMatrix);
    SetIdentityMatrix(ViewMatrix);
    

    SetIdentityMatrix(ModelMatrixGround);
    SetIdentityMatrix(ModelMatrixPillar);
    SetIdentityMatrix(ModelMatrixRoof);
    
    SetIdentityMatrix(ModelMatrixFloor);
    SetIdentityMatrix(ModelMatrixWall1);
    SetIdentityMatrix(ModelMatrixWall2);
    SetIdentityMatrix(ModelMatrixWall3);

    
    
    SetIdentityMatrix(ModelMatrixBox1);
    SetIdentityMatrix(ModelMatrixBox2);
    SetIdentityMatrix(ModelMatrixBox3);
    SetIdentityMatrix(ModelMatrixBox4);
    
    SetIdentityMatrix(SuzanneMatrix1);
    SetIdentityMatrix(SuzanneMatrix2);
    SetIdentityMatrix(SuzanneMatrix3);
    SetIdentityMatrix(SuzanneMatrix4);
    
    SetIdentityMatrix(RotationMatrixCameraX);
    SetIdentityMatrix(RotationMatrixCameraY);
    SetIdentityMatrix(RotationMatrixCameraZ);
    SetIdentityMatrix(RotationMatrixCamera);
    
    BOX1_CURRENT_POSITION_Y = BOX1_START_POSITION_Y;
    BOX2_CURRENT_POSITION_Y = BOX2_START_POSITION_Y;
    BOX3_CURRENT_POSITION_Y = BOX3_START_POSITION_Y;
    BOX4_CURRENT_POSITION_Y = BOX4_START_POSITION_Y;

    
        



    /* Set projection transform */
    float fovy = 45.0;
    float aspect = 1.0; 
    float nearPlane = 1.0; 
    float farPlane = 50.0;
    SetPerspectiveMatrix(fovy, aspect, nearPlane, farPlane, ProjectionMatrix);

    /* Set viewing transform */
    //float camera_disp = -15.0;
    SetTranslation(0.0, 0.0, camera_disp, ViewMatrix);

    
    /* Translate and rotate ground onto tip */
    SetTranslation(0, 0, 0, TranslateOriginGround);
    SetRotationX(0, RotateXGround);
    SetRotationZ(0, RotateZGround);	
    
    SetTranslation(0, 4.2, 0, TranslateOriginRoof);
    SetRotationX(0, RotateXRoof);
    SetRotationZ(0, RotateZRoof);
    
    SetTranslation(0, 2, 0, TranslateOriginPillar);
    SetRotationX(0, RotateXPillar);
    SetRotationZ(0, RotateZPillar);
    
    /* Walls and Floor*/
    //floor
    int floor_y = -3;
    SetTranslation(0, -3, 0, TranslateOriginFloor);
    SetRotationX(0, RotateXFloor);
    SetRotationZ(0, RotateZFloor);
    
    // left wall
    SetTranslation(-floor_y, -WALL_SIZE, 0, TranslateOriginWall1);
    SetRotationX(0, RotateXWall1);
    SetRotationY(0, RotateYWall1);
    SetRotationZ(90, RotateZWall1);
         
    // right wall
    SetTranslation(-floor_y, WALL_SIZE, 0, TranslateOriginWall2);
    SetRotationX(0, RotateXWall2);
    SetRotationY(0, RotateYWall2);
    SetRotationZ(90, RotateZWall2);
    
    // back wall
    SetTranslation(0, -WALL_SIZE, floor_y, TranslateOriginWall3);
    SetRotationX(90, RotateXWall3);
    SetRotationZ(0, RotateZWall3);
    SetRotationY(0, RotateYWall3);
        
    
    /* Translate and rotate box1 onto tip */
    SetTranslation(2, BOX1_START_POSITION_Y, 2, TranslateOriginBox1);
    SetRotationX(0, RotateXBox1);
    SetRotationZ(0, RotateZBox1);
    
    SetTranslation(-2, BOX2_START_POSITION_Y, -2, TranslateOriginBox2);
    SetRotationX(0, RotateXBox2);
    SetRotationZ(0, RotateZBox2);
    
    SetTranslation(-2, BOX3_START_POSITION_Y, 2, TranslateOriginBox3);
    SetRotationX(0, RotateXBox3);
    SetRotationZ(0, RotateZBox3);
    
    SetTranslation( 2, BOX4_START_POSITION_Y, -2, TranslateOriginBox4);
    SetRotationX(0, RotateXBox4);
    SetRotationZ(0, RotateZBox4);
    
    /* Translate down */
    SetTranslation(0, -sqrtf(sqrtf(2.0) * 1.0), 0, TranslateDownGround);
    SetTranslation(0, -sqrtf(sqrtf(2.0) * 1.0), 0, TranslateDownPillar);
    SetTranslation(0, -sqrtf(sqrtf(2.0) * 1.0), 0, TranslateDownRoof);
    SetTranslation(0, -sqrtf(sqrtf(2.0) * 1.0), 0, TranslateDownBox1);
    SetTranslation(0, -sqrtf(sqrtf(2.0) * 1.0), 0, TranslateDownBox2);
    SetTranslation(0, -sqrtf(sqrtf(2.0) * 1.0), 0, TranslateDownBox3);
    SetTranslation(0, -sqrtf(sqrtf(2.0) * 1.0), 0, TranslateDownBox4);




    /* Initial transformation matrix */    
    MultiplyMatrix(RotateXGround, TranslateOriginGround, InitialTransformGround);
    MultiplyMatrix(RotateZGround, InitialTransformGround, InitialTransformGround);
    
    MultiplyMatrix(RotateXPillar, TranslateOriginPillar, InitialTransformPillar);
    MultiplyMatrix(RotateZPillar, InitialTransformPillar, InitialTransformPillar);
    
    MultiplyMatrix(RotateXRoof, TranslateOriginRoof, InitialTransformRoof);
    MultiplyMatrix(RotateZRoof, InitialTransformRoof, InitialTransformRoof);
    
    /* Walls and Floor*/
    MultiplyMatrix(RotateXFloor, TranslateOriginFloor, InitialTransformFloor);
    MultiplyMatrix(RotateZFloor, InitialTransformFloor, InitialTransformFloor);
    MultiplyMatrix(InitialTransformFloor, ModelMatrixFloor, ModelMatrixFloor);
    
    MultiplyMatrix(RotateXWall1, TranslateOriginWall1, InitialTransformWall1);
    MultiplyMatrix(RotateZWall1, InitialTransformWall1, InitialTransformWall1);
    //MultiplyMatrix(RotateYWall1, InitialTransformWall1, InitialTransformWall1);
    MultiplyMatrix(InitialTransformWall1, ModelMatrixWall1, ModelMatrixWall1);

    MultiplyMatrix(RotateXWall2, TranslateOriginWall2, InitialTransformWall2);
    MultiplyMatrix(RotateZWall2, InitialTransformWall2, InitialTransformWall2);
   // MultiplyMatrix(RotateYWall2, InitialTransformWall2, InitialTransformWall2);
    MultiplyMatrix(InitialTransformWall2, ModelMatrixWall2, ModelMatrixWall2);

    MultiplyMatrix(RotateXWall3, TranslateOriginWall3, InitialTransformWall3);
    MultiplyMatrix(RotateZWall3, InitialTransformWall3, InitialTransformWall3);
    //MultiplyMatrix(RotateYWall3, InitialTransformWall3, InitialTransformWall3);
    MultiplyMatrix(InitialTransformWall3, ModelMatrixWall3, ModelMatrixWall3);

    
    MultiplyMatrix(RotateXBox1, TranslateOriginBox1, InitialTransformBox1);
    MultiplyMatrix(RotateZBox1, InitialTransformBox1, InitialTransformBox1);
    
    MultiplyMatrix(RotateXBox2, TranslateOriginBox2, InitialTransformBox2);
    MultiplyMatrix(RotateZBox2, InitialTransformBox2, InitialTransformBox2);
    
    MultiplyMatrix(RotateXBox3, TranslateOriginBox3, InitialTransformBox3);
    MultiplyMatrix(RotateZBox3, InitialTransformBox3, InitialTransformBox3);
    
    MultiplyMatrix(RotateXBox4, TranslateOriginBox4, InitialTransformBox4);
    MultiplyMatrix(RotateZBox4, InitialTransformBox4, InitialTransformBox4);
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
    glutMouseFunc(Mouse);
    glutPassiveMotionFunc(MouseMove);
    glutMainLoop();

    /* ISO C requires main to return int */
    return 0;
}
