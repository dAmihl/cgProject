#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 2) in vec3 vertexNormal_modelspace;
layout (location = 3) in vec3 Color;


// Output data ; will be interpolated for each fragment.
out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 Light1Direction_cameraspace;
out vec3 Light2Direction_cameraspace;
out vec3 Light3Direction_cameraspace;
out vec3 Light4Direction_cameraspace;
out vec4 vColor;

// Values that stay constant for the whole mesh.
uniform mat4 ProjectionViewModelMatrix;
uniform mat4 V;
uniform mat4 M;
uniform vec3 Light1Position_worldspace;
uniform vec3 Light2Position_worldspace;
uniform vec3 Light3Position_worldspace;
uniform vec3 Light4Position_worldspace;

void main(){

	// Output position of the vertex, in clip space : ProjectionViewModelMatrix * position
	gl_Position =  ProjectionViewModelMatrix * vec4(vertexPosition_modelspace,1);
	
	// Position of the vertex, in worldspace : M * position
	Position_worldspace = (M * vec4(vertexPosition_modelspace,1)).xyz;
	
	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 vertexPosition_cameraspace = ( V * M * vec4(vertexPosition_modelspace,1)).xyz;
	EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
        // Light source 1
	vec3 Light1Position_cameraspace = ( V * vec4(Light1Position_worldspace,1)).xyz;
	Light1Direction_cameraspace = Light1Position_cameraspace + EyeDirection_cameraspace;
        // Light source 2
        vec3 Light2Position_cameraspace = ( V * vec4(Light2Position_worldspace,1)).xyz;
	Light2Direction_cameraspace = Light2Position_cameraspace + EyeDirection_cameraspace;
        // Light source 3 
	vec3 Light3Position_cameraspace = ( V * vec4(Light3Position_worldspace,1)).xyz;
	Light3Direction_cameraspace = Light3Position_cameraspace + EyeDirection_cameraspace;
        // Light source 4
	vec3 Light4Position_cameraspace = ( V * vec4(Light4Position_worldspace,1)).xyz;
	Light4Direction_cameraspace = Light4Position_cameraspace + EyeDirection_cameraspace;
	
	// Normal of the the vertex, in camera space
	Normal_cameraspace = ( V * M * vec4(vertexNormal_modelspace,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.

        vColor = vec4(Color.r, Color.g, Color.b, 1.0);

}

