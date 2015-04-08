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

#define BOX1_SIZE 0.5
#define BOX1_HEIGHT 1.0

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


/* Indices to vertex attributes; in this case positon and color */ 
enum DataID {vPosition = 0, vColor = 1}; 

/* Strings for loading and storing shader code */
static const char* VertexShaderString;
static const char* FragmentShaderString;

GLuint ShaderProgram;

float ProjectionMatrix[16]; /* Perspective projection matrix */
float ViewMatrix[16]; /* Camera view matrix */ 
float ModelMatrixGround[16]; /* Model matrix */
float ModelMatrixBox1[16]; /*Model matrix for box1*/

/* Transformation matrices for initial position */
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


float BOX1_CURRENT_HEIGHT = 0.0;
int BOX1_DIRECTION = 1;


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
    1.0, 0.0, 1.0,
    1.0, 1.0, 1.0,
    0.0, 1.0, 1.0,
    0.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
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



/*BOX 1*/

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
    0.0, 0.0, 1.0,
    1.0, 0.0, 1.0,
    1.0, 1.0, 1.0,
    0.0, 1.0, 1.0,
    0.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
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


void Display()
{
    /* Clear window; color specified in 'Initialize()' */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    DrawObject(GROUND_VBO, GROUND_CBO, GROUND_IBO, ModelMatrixGround);
    DrawObject(BOX1_VBO, BOX1_CBO, BOX1_IBO, ModelMatrixBox1);
    
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
    float angle = (glutGet(GLUT_ELAPSED_TIME) / 1000.0) * (180.0/M_PI); 
    float RotationMatrixAnimGround[16];
    float RotationMatrixAnimBox1[16];
    
    
    if (BOX1_CURRENT_HEIGHT >= 1.5) BOX1_DIRECTION = -1;
    else if(BOX1_CURRENT_HEIGHT <= 0) BOX1_DIRECTION = 1;
    else BOX1_DIRECTION = BOX1_DIRECTION;
    
    float newBox1Height = BOX1_CURRENT_HEIGHT + (BOX1_DIRECTION * 0.05f);
    
    SetTranslation(1, newBox1Height, 1, TranslateDownBox1);

    BOX1_CURRENT_HEIGHT = newBox1Height;
    
    /* Time dependent rotation */
    SetRotationY(angle, RotationMatrixAnimGround);
    SetRotationY(angle, RotationMatrixAnimBox1);


    /* Apply model rotation; finally move cube down */
    MultiplyMatrix(RotationMatrixAnimGround, InitialTransformGround, ModelMatrixGround);
    MultiplyMatrix(TranslateDownGround, ModelMatrixGround, ModelMatrixGround);

    MultiplyMatrix(RotationMatrixAnimBox1, InitialTransformBox1, ModelMatrixBox1);
    MultiplyMatrix(TranslateDownBox1, ModelMatrixBox1, ModelMatrixBox1);

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
    glGenBuffers(1, &GROUND_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, GROUND_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ground_vertex_buffer_data), ground_vertex_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &GROUND_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GROUND_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ground_index_buffer_data), ground_index_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &GROUND_CBO);
    glBindBuffer(GL_ARRAY_BUFFER, GROUND_CBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ground_color_buffer_data), ground_color_buffer_data, GL_STATIC_DRAW);
    
    
    glGenBuffers(1, &BOX1_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, BOX1_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(box1_vertex_buffer_data), box1_vertex_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &BOX1_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BOX1_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(box1_index_buffer_data), box1_index_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &BOX1_CBO);
    glBindBuffer(GL_ARRAY_BUFFER, BOX1_CBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(box1_color_buffer_data), box1_color_buffer_data, GL_STATIC_DRAW);
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
    glClearColor(0.0, 0.0, 0.4, 0.0);

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
    SetIdentityMatrix(ModelMatrixBox1);


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

    
    /* Translate and rotate box1 onto tip */
    SetTranslation(1, 1, 1, TranslateOriginBox1);
    SetRotationX(0, RotateXBox1);
    SetRotationZ(0, RotateZBox1);
    
    /* Translate down */	
    SetTranslation(0, -sqrtf(sqrtf(2.0) * 1.0), 0, TranslateDownGround);
    SetTranslation(0, -sqrtf(sqrtf(2.0) * 1.0), 0, TranslateDownBox1);


    /* Initial transformation matrix */
    MultiplyMatrix(RotateXGround, TranslateOriginGround, InitialTransformGround);
    MultiplyMatrix(RotateZGround, InitialTransformGround, InitialTransformGround);
     
    MultiplyMatrix(RotateXBox1, TranslateOriginBox1, InitialTransformBox1);
    MultiplyMatrix(RotateZBox1, InitialTransformBox1, InitialTransformBox1);
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
    glutCreateWindow("CG Proseminar - Rotating Cube");



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
