#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 2) in vec3 vertexNormal_modelspace;
layout (location = 3) in vec3 Color;

const int numberLightSources = 4;

// Output data ; will be interpolated for each fragment.
out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace[numberLightSources];
out vec4 vColor;

uniform mat4 ProjectionViewModelMatrix;
uniform mat4 V;
uniform mat4 M;
uniform vec3 LightPosition_worldspace[numberLightSources];

void main(){

	gl_Position =  ProjectionViewModelMatrix * vec4(vertexPosition_modelspace,1);

	Position_worldspace = (M * vec4(vertexPosition_modelspace,1)).xyz;

	vec3 vertexPosition_cameraspace = ( V * M * vec4(vertexPosition_modelspace,1)).xyz;
	EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

        for (int i = 0; i < numberLightSources; i++){
            vec3 LightPosition_cameraspace = ( V * vec4(LightPosition_worldspace[i],1)).xyz;
            LightDirection_cameraspace[i] = LightPosition_cameraspace + EyeDirection_cameraspace;
        }
	
	Normal_cameraspace = ( V * M * vec4(vertexNormal_modelspace,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.

        vColor = vec4(Color.r, Color.g, Color.b, 1.0);

}

