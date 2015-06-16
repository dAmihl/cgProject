#version 330 core

const int numberLightSources = 4;

// Interpolated values from the vertex shaders
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace[numberLightSources];
in vec4 vColor;
in vec2 UVcoords;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform mat4 MV;
uniform vec3 LightPosition_worldspace[numberLightSources];
uniform vec3 LightColor[numberLightSources];
uniform float LightIntensity[numberLightSources];
uniform sampler2D textureSampler;
uniform sampler2D normalMapSampler;

void main(){
 
        vec3 Normal_bumpmap = (2*texture2D(normalMapSampler, UVcoords).rgb)-1;

	// Normal of the computed fragment, in camera space
	//vec3 n = normalize( Normal_bumpmap );
	vec3 n = normalize( Normal_cameraspace );

        

	// Eye vector (towards the camera)
	vec3 E = normalize(EyeDirection_cameraspace);

        vec3 DiffuseColor = texture2D( textureSampler, UVcoords ).rgb;
        //vec3 DiffuseColor = vec3(vColor.x + 0.1, vColor.y + 0.1, vColor.z + 0.1);
        vec3 AmbientColor = vec3(0.1, 0.1, 0.1) * DiffuseColor;
        vec3 SpecularColor = vec3(0.3,0.3,0.3);
        vec3 tmpColor = vec3(0,0,0);

        for (int i = 0; i < numberLightSources; i++){
            vec3 lightCol = LightColor[i];
            float intensity = LightIntensity[i];
            float distanceLight = length( LightPosition_worldspace[i] - Position_worldspace );
            vec3 l = normalize( LightDirection_cameraspace[i] );
            float cosThetaLight = clamp( dot( n,l ), 0,1 );
            vec3 R_light = reflect(-l,n);
            float cosAlphaLight = clamp( dot( E,R_light ), 0,1 );
            tmpColor = tmpColor + 
                (DiffuseColor * lightCol * intensity * cosThetaLight / (distanceLight*distanceLight) +
		SpecularColor * lightCol * intensity * pow(cosAlphaLight,5) / (distanceLight*distanceLight));
        }

        tmpColor = tmpColor + AmbientColor;

        color = vec4(tmpColor.x,tmpColor.y, tmpColor.z, 0);
}
