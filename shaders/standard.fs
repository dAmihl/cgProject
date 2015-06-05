#version 330 core

// Interpolated values from the vertex shaders
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 Light1Direction_cameraspace;
in vec3 Light2Direction_cameraspace;
in vec3 Light3Direction_cameraspace;
in vec3 Light4Direction_cameraspace;
in vec4 vColor;


// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform mat4 MV;
uniform vec3 Light1Position_worldspace;
uniform vec3 Light2Position_worldspace;
uniform vec3 Light3Position_worldspace;
uniform vec3 Light4Position_worldspace;


void main(){

        // Light properties
        // Light 1
	vec3 Light1Color = vec3(1,1,1);
	float Light1Power = 50.0f;
        // Light 2
        vec3 Light2Color = vec3(0,1,0);
	float Light2Power = 50.0f;
        // Light 3
        vec3 Light3Color = vec3(1,0,0);
	float Light3Power = 50.0f;
	// Light 4
        vec3 Light4Color = vec3(0,0,1);
	float Light4Power = 50.0f;



	// Material properties
        vec3 MaterialDiffuseColor = vec3(vColor.x + 0.1, vColor.y + 0.1, vColor.z + 0.1);
	vec3 MaterialAmbientColor = vec3(0.1,0.1,0.1) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);

	// Distance to the light
	float distanceLight1 = length( Light1Position_worldspace - Position_worldspace );
	float distanceLight2 = length( Light2Position_worldspace - Position_worldspace );
	float distanceLight3 = length( Light3Position_worldspace - Position_worldspace );
	float distanceLight4 = length( Light4Position_worldspace - Position_worldspace );

	// Normal of the computed fragment, in camera space
	vec3 n = normalize( Normal_cameraspace );

	// Direction of the light (from the fragment to the light)
	vec3 l1 = normalize( Light1Direction_cameraspace );
	vec3 l2 = normalize( Light2Direction_cameraspace );
	vec3 l3 = normalize( Light3Direction_cameraspace );
	vec3 l4 = normalize( Light4Direction_cameraspace );

	// Cosine of the angle between the normal and the light direction, 
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosThetaLight1 = clamp( dot( n,l1 ), 0,1 );
	float cosThetaLight2 = clamp( dot( n,l2 ), 0,1 );
	float cosThetaLight3 = clamp( dot( n,l3 ), 0,1 );
	float cosThetaLight4 = clamp( dot( n,l4 ), 0,1 );

	
	// Eye vector (towards the camera)
	vec3 E = normalize(EyeDirection_cameraspace);
	// Direction in which the triangle reflects the light
	vec3 R_light1 = reflect(-l1,n);
	vec3 R_light2 = reflect(-l2,n);
	vec3 R_light3 = reflect(-l3,n);
	vec3 R_light4 = reflect(-l4,n);

	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlphaLight1 = clamp( dot( E,R_light1 ), 0,1 );
	float cosAlphaLight2 = clamp( dot( E,R_light2 ), 0,1 );
	float cosAlphaLight3 = clamp( dot( E,R_light3 ), 0,1 );
	float cosAlphaLight4 = clamp( dot( E,R_light4 ), 0,1 );

	
	color = 
		// Ambient : simulates indirect lighting
		MaterialAmbientColor +
                // Colors for every light
		// Light 1
		MaterialDiffuseColor * Light1Color * Light1Power * cosThetaLight1 / (distanceLight1*distanceLight1) +
		MaterialSpecularColor * Light1Color * Light1Power * pow(cosAlphaLight1,5) / (distanceLight1*distanceLight1)+
		// Light 2
		MaterialDiffuseColor * Light2Color * Light2Power * cosThetaLight2 / (distanceLight2*distanceLight2) +
		MaterialSpecularColor * Light2Color * Light2Power * pow(cosAlphaLight2,5) / (distanceLight2*distanceLight2)+
                // Light 3
		MaterialDiffuseColor * Light3Color * Light3Power * cosThetaLight3 / (distanceLight3*distanceLight3) +
		MaterialSpecularColor * Light3Color * Light3Power * pow(cosAlphaLight3,5) / (distanceLight3*distanceLight3)+
                // Light 4
		MaterialDiffuseColor * Light4Color * Light4Power * cosThetaLight4 / (distanceLight4*distanceLight4) +
		MaterialSpecularColor * Light4Color * Light4Power * pow(cosAlphaLight4,5) / (distanceLight4*distanceLight4);
}