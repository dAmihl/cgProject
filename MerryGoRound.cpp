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

#include "DeltaTime.h"
#include "LoadShader.h"   /* Provides loading function for shader code */
#include "LoadTexture.h"
#include "Objects.h"
#include "CameraMovement.h"

#include "SOIL.h"

/*----------------------------------------------------------------*/
/* Define handle to a vertex array object (only for MESA USE) */
GLuint VAO;


/* Variables for texture handling */
GLuint TextureUniform;
GLuint NormalMapUniform;


WorldObject Robot;
WorldObject Pavillon;
WorldObject Floor;

BillboardObject Billboard;

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;
glm::mat4 PVMMatrix;

/* Indices to vertex attributes; in this case positon and color */ 
enum DataID {vPosition = 0, vUV = 1, vColor = 3, vNormals = 2, vTangent = 4}; 

/* Strings for loading and storing shader code */
static const char* VertexShaderString;
static const char* FragmentShaderString;

GLuint ShaderProgramStandard;
GLuint ShaderProgramBillboard;

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

GLboolean animCamera = GL_TRUE; // if the camera is animated
GLboolean animMerryGoRound = GL_TRUE; // if the merry go round is animated

glm::mat4 ModelMatrixFloor; /* Model matrix for the floor entity*/
glm::mat4 InitialTransformFloor;
/*
 Model matrix for each MerryGoRound object
 */
glm::mat4 RobotMatrix1;
glm::mat4 RobotMatrix2;
glm::mat4 RobotMatrix3;
glm::mat4 RobotMatrix4;

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

    TextureDataPtr TextureFloorNormalMap;
    GLuint TextureFloorNormalMapID;
    
    TextureDataPtr TextureRobotNormalMap;
    GLuint TextureRobotNormalMapID;
    
    TextureDataPtr TexturePavillonNormalMap;
    GLuint TexturePavillonNormalMapID;


/*
 Attributes of the MerryGoRound animation 
 */
float rotation_speed_factor = START_ROTATION_SPEED;
int rotation_direction = 1;
float updown_speed_factor = 1.0f;


/*
 * Computes the tangent for a world object
 * Source: http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/#Computing_the_tangents_and_bitangents
 */

void computeTangentForObject(WorldObject* obj){

    for ( uint i=0; i<obj->vertex_buffer.size(); i+=3){
 
        // Shortcuts for vertices
        glm::vec3 & v0 = obj->vertex_buffer[i+0];
        glm::vec3 & v1 = obj->vertex_buffer[i+1];
        glm::vec3 & v2 = obj->vertex_buffer[i+2];

        // Shortcuts for UVs
        glm::vec2 & uv0 = obj->uv_buffer[i+0];
        glm::vec2 & uv1 = obj->uv_buffer[i+1];
        glm::vec2 & uv2 = obj->uv_buffer[i+2];

        // Edges of the triangle : postion delta
        glm::vec3 deltaPos1 = v1-v0;
        glm::vec3 deltaPos2 = v2-v0;

        // UV delta
        glm::vec2 deltaUV1 = uv1-uv0;
        glm::vec2 deltaUV2 = uv2-uv0;
        
        float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
        glm::vec3 tangent = (deltaPos1 * deltaUV2.y   - deltaPos2 * deltaUV1.y)*r;
        //glm::vec3 bitangent = (deltaPos2 * deltaUV1.x   - deltaPos1 * deltaUV2.x)*r;
        
        obj->tangent_buffer.push_back(tangent);
        obj->tangent_buffer.push_back(tangent);
        obj->tangent_buffer.push_back(tangent);

    }
}

void computeNormalsForObject(WorldObject* obj){
 
    for ( uint i=0; i<obj->vertex_buffer.size(); i+=3){
 
        // Shortcuts for vertices
        glm::vec3 & v1 = obj->vertex_buffer[i+0];
        glm::vec3 & v2 = obj->vertex_buffer[i+1];
        glm::vec3 & v3 = obj->vertex_buffer[i+2];
    
        glm::vec3 normal = glm::normalize(glm::cross(v3 - v2, v1 - v2));
        obj->normal_buffer.push_back(normal);
        obj->normal_buffer.push_back(normal);
        obj->normal_buffer.push_back(normal);
    }
}



/*
  Sets up the vertices and uv coordinates
  * for a standard billboard
  */
 void SetupStandardBillboard(BillboardObject* billboard){
      
    billboard->vertex_buffer = {
        -1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 0.0f
     };
 
    billboard->uv_buffer = {
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
    };
 }


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


void DrawObject(GLuint VBO, GLuint IBO, GLuint NBO, GLuint UVBO, GLuint TBO,  glm::mat4 ModelMatrix, GLuint TextureID, GLuint NormalMapID){
       
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
   GLint TextureSampler;
    
    /*
     If a normal map is given, bind it to the normalMapSampler uniform
     */
    GLuint NormalMapActiveFlag  = glGetUniformLocation(ShaderProgramStandard, "normalMappingActive");
    if (NormalMapID != 0){
             
        glUniform1i(NormalMapActiveFlag, 1);
         
        glEnableVertexAttribArray(vTangent);
        glBindBuffer(GL_ARRAY_BUFFER, TBO);
        glVertexAttribPointer(vTangent,2,GL_FLOAT,GL_FALSE,0,(void*)0);
         
         
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureID);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, NormalMapID);
       
        TextureSampler  = glGetUniformLocation(ShaderProgramStandard, "textureSampler");
        glUniform1i(TextureSampler, 0);
        TextureSampler  = glGetUniformLocation(ShaderProgramStandard, "normalMapSampler");
        glUniform1i(TextureSampler, 1);
        
    }else{
        glUniform1i(NormalMapActiveFlag, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureID);
        TextureSampler  = glGetUniformLocation(ShaderProgramStandard, "textureSampler");
        glUniform1i(TextureSampler, 0);
    }
    
    glEnableVertexAttribArray(vUV);
    glBindBuffer(GL_ARRAY_BUFFER, UVBO);
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
    glDisableVertexAttribArray(vTangent);
}


void DrawBillboard(GLuint VBO, GLuint UVBO, GLuint TextureID, glm::vec3 billboardPos, glm::vec2 billboardSize){
    
   
    
    glm::mat4 mView = ViewMatrix;
    glm::mat4 mProjection = ProjectionMatrix;
    glm::mat4 PV = mProjection * mView ;
    
    
    // Vertex shader
    GLuint CameraRight_worldspace_ID  = glGetUniformLocation(ShaderProgramBillboard, "CameraRight_worldspace");
    GLuint CameraUp_worldspace_ID  = glGetUniformLocation(ShaderProgramBillboard, "CameraUp_worldspace");
    GLuint ViewProjMatrixID = glGetUniformLocation(ShaderProgramBillboard, "VP");
    GLuint BillboardPosID = glGetUniformLocation(ShaderProgramBillboard, "BillboardPos");    
    GLuint BillboardSizeID = glGetUniformLocation(ShaderProgramBillboard, "BillboardSize");
    
       
    /* Associate program with shader matrices */
    glUniform3f(CameraRight_worldspace_ID, ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
    glUniform3f(CameraUp_worldspace_ID   , ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);
		
    glUniform3f(BillboardPosID, billboardPos.x, billboardPos.y, billboardPos.z); // The billboard will be just above the cube
    glUniform2f(BillboardSizeID, billboardSize.x, billboardSize.y);     // and 1m*12cm, because it matches its 256*32 resolution =)
    glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, glm::value_ptr(PV));

    
    glEnableVertexAttribArray(vPosition);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    
    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TextureID);
    TextureUniform  = glGetUniformLocation(ShaderProgramBillboard, "textureSampler");
    glUniform1i(TextureUniform, 0);
    
    
    glEnableVertexAttribArray(vUV);
    glBindBuffer(GL_ARRAY_BUFFER, UVBO);
    glVertexAttribPointer(vUV,2,GL_FLOAT,GL_FALSE,0,(void*)0);
 
    
    GLint size; 
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    
    /* Issue draw command, using indexed triangle list */
    glDrawElements(GL_TRIANGLES, size/sizeof(GLuint), GL_UNSIGNED_INT, 0);
    
    /* Disable attributes */
    glDisableVertexAttribArray(vPosition);
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

    Texture->data = SOIL_load_image(filepath, &Texture->width, &Texture->height, 0, SOIL_LOAD_RGBA);
    //int success = LoadTexture(filepath, Texture);
    if (Texture->data == 0)
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
		 GL_RGBA,            /* Each element is RGB triple */ 
		 Texture->width,    /* Texture dimensions */ 
                 Texture->height, 
		 0,                 /* Border should be zero */
		 GL_RGBA,            /* Data storage format for BMP file */
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



void Display()
{
    /* Clear window; color specified in 'Initialize()' */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glUseProgram(ShaderProgramStandard);
    /* Walls and Floor*/
    DrawObject(Floor.VBO,  Floor.IBO, Floor.NBO,Floor.UVBO, Floor.TBO, ModelMatrixFloor, Floor.TextureID, TextureFloorNormalMapID);

    DrawObject(Robot.VBO,  Robot.IBO,Robot.NBO, Robot.UVBO, Robot.TBO,  RobotMatrix1, Robot.TextureID, TextureRobotNormalMapID);
    DrawObject(Robot.VBO,  Robot.IBO,Robot.NBO, Robot.UVBO, Robot.TBO, RobotMatrix2, Robot.TextureID, TextureRobotNormalMapID);
    DrawObject(Robot.VBO,  Robot.IBO,Robot.NBO, Robot.UVBO, Robot.TBO, RobotMatrix3, Robot.TextureID, TextureRobotNormalMapID);
    DrawObject(Robot.VBO,  Robot.IBO,Robot.NBO, Robot.UVBO, Robot.TBO, RobotMatrix4, Robot.TextureID, TextureRobotNormalMapID);
    
    DrawObject(Pavillon.VBO,  Pavillon.IBO,Pavillon.NBO, Pavillon.UVBO, Pavillon.TBO, PavillonModelMatrix, Pavillon.TextureID, TexturePavillonNormalMapID);

        
    glUseProgram(ShaderProgramBillboard);
    DrawBillboard(Billboard.VBO, Billboard.UVBO, Billboard.TextureID, glm::vec3(0.0f, 5.0f, 50.0f), glm::vec2(5.0f, 5.0f));

    
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

    
    if(state == GLUT_DOWN) 
    {
      /* Depending on button pressed, set rotation axis,
       * turn on animation */
        switch(button) 
	{
	    case GLUT_LEFT_BUTTON:    
	        setAxis(Xaxis);
		break;

	    case GLUT_MIDDLE_BUTTON:  
                setAxis(Yaxis);
	        break;
		
	    case GLUT_RIGHT_BUTTON: 
	        setAxis(Zaxis);
		break;
	}
    }

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
    MouseMoveUpdate(x, y);
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
            if (cameraMode == CAMERA_FREE_MOVE) moveForward(GL_TRUE);
            else{
                if (rotation_speed_factor < MAX_ROTATION_SPEED) rotation_speed_factor += 0.1;
                if (updown_speed_factor < MAX_ROTATION_SPEED) updown_speed_factor += 0.1;
            }
	    break;
	    
	case 's':
	   
            if (cameraMode == CAMERA_FREE_MOVE) moveBack(GL_TRUE);
            else{
                if (rotation_speed_factor > 0) rotation_speed_factor -= 0.1;
                if (updown_speed_factor > 0) updown_speed_factor -= 0.1;
            }
	    break;
	case 'a':
	    
            if (cameraMode == CAMERA_FREE_MOVE) moveLeft(GL_TRUE);
            else{
                rotation_direction = 1;
            }
	    break;
	    
	case 'd':
	    
            if (cameraMode == CAMERA_FREE_MOVE) moveRight(GL_TRUE);
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
            moveForward(GL_FALSE);
	    break;
	    
	case 's':
            moveBack(GL_FALSE);
	    break;
	case 'a':
            moveLeft(GL_FALSE);
	    break;
	    
	case 'd':
            moveRight(GL_FALSE);
	    break;
    }
}




/***********************************************
 
 * Merry Go Round Animation
 * computes the animation of the MerryGoRound
 **********************************************/

void MerryGoRoundAnimation(){
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
        ModelMatrixFloor = glm::scale(ModelMatrixFloor, glm::vec3(2.0f, 2.0f, 2.0f));
        /* Apply model rotation; finally move cube down */    
        RobotMatrix1 = TranslateDownBox1 * UpDownTranslationBox1 * RotationMatrixAnimBox1 * InitialTransformBox1;
        RobotMatrix2 = TranslateDownBox2 * UpDownTranslationBox2 * RotationMatrixAnimBox2 * InitialTransformBox2;
        RobotMatrix3 = TranslateDownBox3 * UpDownTranslationBox3 * RotationMatrixAnimBox3 * InitialTransformBox3;
        RobotMatrix4 = TranslateDownBox4 * UpDownTranslationBox4 * RotationMatrixAnimBox4 * InitialTransformBox4;

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
        ViewMatrix = CameraFreeMove(ViewMatrix);
        }
    // Camera Fixed Move active
    else {
        if(animCamera) {
           ViewMatrix = CameraFixedMove(ViewMatrix);
        }
    }
       
    // if the animation is not paused
    if (animMerryGoRound){
        MerryGoRoundAnimation();
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

void SetupDataBuffers(WorldObject* object)
{
    
    glGenBuffers(1, &object->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, object->VBO);
    glBufferData(GL_ARRAY_BUFFER, object->vertex_buffer.size()*sizeof(glm::vec3), &object->vertex_buffer[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &object->NBO);
    glBindBuffer(GL_ARRAY_BUFFER, object->NBO);
    glBufferData(GL_ARRAY_BUFFER, object->normal_buffer.size()*sizeof(glm::vec3), &object->normal_buffer[0], GL_STATIC_DRAW);

    
    glGenBuffers(1, &object->IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, object->index_buffer.size()*sizeof(GLuint), &object->index_buffer[0], GL_STATIC_DRAW);
 
    glGenBuffers(1, &object->UVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->UVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, object->uv_buffer.size()*sizeof(glm::vec2), &object->uv_buffer[0], GL_STATIC_DRAW);
 
    glGenBuffers(1, &object->TBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->TBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, object->tangent_buffer.size()*sizeof(glm::vec3), &object->tangent_buffer[0], GL_STATIC_DRAW);
 
}

void SetupDataBuffersBillboards(BillboardObject* billboard){
    
    glGenBuffers(1, &billboard->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, billboard->VBO);
    glBufferData(GL_ARRAY_BUFFER, billboard->vertex_buffer.size()*sizeof(GLfloat), &billboard->vertex_buffer[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &billboard->UVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, billboard->UVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, billboard->uv_buffer.size()*sizeof(GLfloat), &billboard->uv_buffer[0], GL_STATIC_DRAW);
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

void CreateShaderProgram(GLuint* shader, const char* vsPath, const char* fsPath)
{
    /* Allocate shader object */
    *shader = glCreateProgram();

    if (*shader == 0) 
    {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }

    /* Load shader code from file */
    VertexShaderString = LoadShader(vsPath);
    FragmentShaderString = LoadShader(fsPath);

    /* Separately add vertex and fragment shader to program */
    AddShader(*shader, VertexShaderString, GL_VERTEX_SHADER);
    AddShader(*shader, FragmentShaderString, GL_FRAGMENT_SHADER);

    GLint Success = 0;
    GLchar ErrorLog[1024];

    /* Link shader code into executable shader program */
    glLinkProgram(*shader);

    /* Check results of linking step */
    glGetProgramiv(*shader, GL_LINK_STATUS, &Success);

    if (Success == 0) 
    {
        glGetProgramInfoLog(*shader, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    /* Check if shader program can be executed */ 
    glValidateProgram(*shader);
    glGetProgramiv(*shader, GL_VALIDATE_STATUS, &Success);

    if (!Success) 
    {
        glGetProgramInfoLog(*shader, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    

}

void InitializeStandardShaderID(){
        
    PVMMatrixID = glGetUniformLocation(ShaderProgramStandard, "ProjectionViewModelMatrix");
    ViewMatrixID = glGetUniformLocation(ShaderProgramStandard, "V");
    ModelMatrixID = glGetUniformLocation(ShaderProgramStandard, "M");
    LightSourcesID = glGetUniformLocation(ShaderProgramStandard, "LightPosition_worldspace");
    LightColorsID = glGetUniformLocation(ShaderProgramStandard, "LightColor");
    LightIntensitiesID = glGetUniformLocation(ShaderProgramStandard, "LightIntensity");
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


void LoadMesh(WorldObject* object, const char* path){
    std::string inputfile = path;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    
         
    std::string err = tinyobj::LoadObj(shapes, materials, inputfile.c_str());

    if (!err.empty()) {
        fprintf(stderr, "Error loading obj File!");
      exit(1);
    }
    
    if (MESH_DEBUG){
        int numVertices = shapes[0].mesh.positions.size();
        int numIndices = shapes[0].mesh.indices.size();
        int numNormals = shapes[0].mesh.normals.size();
        int numUVs = shapes[0].mesh.texcoords.size();
        

        fprintf(stderr, "Number of vertics: %d \n", numVertices);
        fprintf(stderr, "Number of indices: %d\n", numIndices);
        fprintf(stderr, "Number of normals : %d\n",numNormals);
        fprintf(stderr, "Nuber of uvs: %d\n", numUVs);
    }
    
    for (uint i = 0; i < shapes[0].mesh.positions.size(); i+=3){
        object->vertex_buffer.push_back(glm::vec3(shapes[0].mesh.positions[i+0], shapes[0].mesh.positions[i+1], shapes[0].mesh.positions[i+2]));
    }
    object->index_buffer = shapes[0].mesh.indices;
    for (uint i = 0; i < shapes[0].mesh.normals.size(); i+=3){
        object->normal_buffer.push_back(glm::vec3(shapes[0].mesh.normals[i+0], shapes[0].mesh.normals[i+1], shapes[0].mesh.normals[i+2]));
    }
    for (uint i = 0; i < shapes[0].mesh.texcoords.size(); i+=2){
        object->uv_buffer.push_back(glm::vec2(shapes[0].mesh.texcoords[i+0], shapes[0].mesh.texcoords[i+1]));
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

void InitializeWorldObject(WorldObject* obj){
    obj = (WorldObject*) malloc(sizeof(WorldObject));
}

void InitializeBillboardObject(BillboardObject* obj){
    obj =(BillboardObject*) malloc(sizeof(BillboardObject));
}


void Initialize(void)
{   
    
    InitializeWorldObject(&Robot);
    InitializeWorldObject(&Pavillon);
    InitializeWorldObject(&Floor);
    
    InitializeBillboardObject(&Billboard);

    
    // load the meshes
   //LoadMesh(&Robot, "models/spider01.obj");
    LoadMesh(&Robot, "models/ufo_rusty.obj");
    LoadMesh(&Pavillon, "models/pavillon_uv.obj");
    LoadMesh(&Floor, "models/groundplane.obj");
    SetupStandardBillboard(&Billboard);
    
    glClearColor(0.0f, 0.0f, 0.3f, 0.0);

    /* Enable depth testing */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);    

    /*Intel troubleshooting*/
    setupArrayObject();
/*
    computeNormalsForObject(&Robot);
    computeNormalsForObject(&Floor);
    computeNormalsForObject(&Pavillon);
   */ 
    /* Compute the vertex tangents*/
    computeTangentForObject(&Robot);
    computeTangentForObject(&Floor);
    computeTangentForObject(&Pavillon);
    
    /* Setup vertex, color, and index buffer objects */
    SetupDataBuffers(&Robot);
    SetupDataBuffers(&Floor);
    SetupDataBuffers(&Pavillon);
    SetupDataBuffersBillboards(&Billboard);

    /* Setup Texture*/
    
    SetupTexture(&Robot.TextureID, Robot.TextureData, "textures/rustytexture.bmp");
    SetupTexture(&Floor.TextureID, Floor.TextureData, "textures/grass.bmp");
    SetupTexture(&Pavillon.TextureID, Pavillon.TextureData, "textures/bunker_galvanized.bmp");

    // Normal Maps
    SetupTexture(&TextureFloorNormalMapID, TextureFloorNormalMap, "textures/normalmap_grass.bmp");
    SetupTexture(&TextureRobotNormalMapID, TextureRobotNormalMap, "textures/normalmap_robot.bmp");
    SetupTexture(&TexturePavillonNormalMapID, TexturePavillonNormalMap, "textures/normalmap_robot.bmp");

    
    SetupTexture(&Billboard.TextureID, Billboard.TextureData, "textures/uvtemplate.bmp");

    
    /* Setup shaders and shader program */
    CreateShaderProgram(&ShaderProgramStandard, "shaders/standard.vs", "shaders/standard.fs");
    CreateShaderProgram(&ShaderProgramBillboard, "shaders/billboard.vs", "shaders/billboard.fs");

    InitializeStandardShaderID();

    /* Initialize matrices */
    
    RobotMatrix1 = glm::mat4(1.0f);
    RobotMatrix2 = glm::mat4(1.0f);
    RobotMatrix3 = glm::mat4(1.0f);
    RobotMatrix4 = glm::mat4(1.0f);
    
    ModelMatrixFloor = glm::mat4(1.0f);
    
    PavillonModelMatrix = glm::mat4(1.0f);
    InitialTransformPavillon = glm::mat4(1.0f);
    
    
    glm::mat4 scalePavillon = glm::scale(glm::mat4(1.0f), glm::vec3(1.1f));
    glm::mat4 translatePavillon = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    
    InitialTransformPavillon = scalePavillon * translatePavillon;
    glm::mat4 InitialScaleFloor = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
    glm::mat4 InitialTranslateFloor = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, 0.0f));
    InitialTransformFloor = InitialScaleFloor * InitialTranslateFloor;

    
    
    BOX1_CURRENT_POSITION_Y = BOX1_START_POSITION_Y;
    BOX2_CURRENT_POSITION_Y = BOX2_START_POSITION_Y;
    BOX3_CURRENT_POSITION_Y = BOX3_START_POSITION_Y;
    BOX4_CURRENT_POSITION_Y = BOX4_START_POSITION_Y;

    
    /* Set projection transform */
    float fovy = 45.0;
    float aspect = 1.0; 
    float nearPlane = 1.0; 
    float farPlane = 100.0;
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
    glutInitWindowSize(800, 800);
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
