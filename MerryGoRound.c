/******************************************************************
*
* RotatingCube.c
*
* Description: This example demonstrates a colored, rotating
* cube in shader-based OpenGL. The use of transformation
* matrices, perspective projection, and indexed triangle sets 
* are shown.
*
* Computer Graphics Proseminar SS 2015
* 
* Interactive Graphics and Simulation Group
* Institute of Computer Science
* University of Innsbruck
*
*******************************************************************/

/*
	If you have a Intel graphicscard under Linux with the Mesa Driver,
	set this to 1, else
	you should set this to 0.
*/
#define USES_MESA_DRIVER 1

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
GLuint BOX1_VBO;

/* Define handle to a color buffer object */
GLuint BOX1_CBO; 

/* Define handle to an index buffer object */
GLuint BOX1_IBO;

/* Define handle to a vertex buffer object */
GLuint PILLAR_VBO;

/* Define handle to a color buffer object */
GLuint PILLAR_CBO; 

/* Define handle to an index buffer object */
GLuint PILLAR_IBO;


/* Indices to vertex attributes; in this case positon and color */ 
enum DataID {vPosition = 0, vColor = 1}; 

/* Strings for loading and storing shader code */
static const char* VertexShaderString;
static const char* FragmentShaderString;

GLuint ShaderProgram;

float ProjectionMatrix[16]; /* Perspective projection matrix */
float ViewMatrix[16]; /* Camera view matrix */ 
float ModelMatrixGround[16]; /* Model matrix for the ground*/
float ModelMatrixRoof[16]; /* Model matrix for the roof */
float ModelMatrixPillar[16]; /* Model matrix for the pillar */
float ModelMatrixBox1[16]; /*Model matrix for box1*/
float ModelMatrixBox2[16]; /*Model matrix for box2*/
float ModelMatrixBox3[16]; /*Model matrix for box3*/
float ModelMatrixBox4[16]; /*Model matrix for box4*/



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

    glEnableVertexAttribArray(vColor);
    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glVertexAttribPointer(vColor, 3, GL_FLOAT,GL_FALSE, 0, 0);   

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

    /* Issue draw command, using indexed triangle list */
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

    
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
    DrawObject(BOX1_VBO, BOX1_CBO, BOX1_IBO, ModelMatrixBox1);
    DrawObject(BOX1_VBO, BOX1_CBO, BOX1_IBO, ModelMatrixBox2);
    DrawObject(BOX1_VBO, BOX1_CBO, BOX1_IBO, ModelMatrixBox3);
    DrawObject(BOX1_VBO, BOX1_CBO, BOX1_IBO, ModelMatrixBox4);


    /* Swap between front and back buffer */ 
    glutSwapBuffers();
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
    
	/* SetUp Rotation matrices */
    float angle = (glutGet(GLUT_ELAPSED_TIME) / 1000.0) * (180.0/M_PI); 
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
    glGenBuffers(1, &BOX1_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, BOX1_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(box1_vertex_buffer_data), box1_vertex_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &BOX1_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BOX1_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(box1_index_buffer_data), box1_index_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &BOX1_CBO);
    glBindBuffer(GL_ARRAY_BUFFER, BOX1_CBO);
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
    VertexShaderString = LoadShader("vertexshader.vs");
    FragmentShaderString = LoadShader("fragmentshader.fs");

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
    /* Set background (clear) color to dark blue */ 
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
    SetIdentityMatrix(ModelMatrixBox1);
    SetIdentityMatrix(ModelMatrixBox2);
    SetIdentityMatrix(ModelMatrixBox3);
    SetIdentityMatrix(ModelMatrixBox4);

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
    float camera_disp = -15.0;
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
    glutMainLoop();

    /* ISO C requires main to return int */
    return 0;
}