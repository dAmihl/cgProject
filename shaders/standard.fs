#version 330 core

const int numberLightSources = 4;

// Interpolated values from the vertex shaders
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 Normal_modelspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace[numberLightSources];
in vec4 vColor;
in vec2 UVcoords;
in vec3 tangent;

// Ouput data
out vec4 color;

uniform int normalMappingActive;

// Values that stay constant for the whole mesh.
uniform mat4 MV;
uniform vec3 LightPosition_worldspace[numberLightSources];
uniform vec3 LightColor[numberLightSources];
uniform float LightIntensity[numberLightSources];
uniform sampler2D textureSampler;
uniform sampler2D normalMapSampler;

void main(){
 
	vec3 n = normalize( Normal_cameraspace );

        /*
            Normal
            Computes the bitangent by cross product of the tangent (given by code) and the normal in modelspace  
        */
       if (normalMappingActive == 1){
            vec3 normalModelspace = normalize(Normal_modelspace);
            vec3 t = tangent;
            vec3 bt = cross(t, normalModelspace);
            vec3 texNormal = (2.0 * texture(normalMapSampler, UVcoords).rgb - vec3(1.0, 1.0, 1.0));
            mat3 TBN = mat3(t, bt, normalModelspace);
            n = normalize(TBN * texNormal);
        }

	// Eye vector (towards the camera)
	vec3 E = normalize(EyeDirection_cameraspace);

        vec3 DiffuseColor = texture2D( textureSampler, UVcoords ).rgb ;
        //vec3 DiffuseColor = vec3(vColor.x + 0.1, vColor.y + 0.1, vColor.z + 0.1);
        vec3 AmbientColor = 0.4f * DiffuseColor;
        vec3 SpecularColor = vec3(0.5,0.5,0.5);
        vec3 tmpColor = vec3(0,0,0);
        float alpha = texture2D( textureSampler, UVcoords ).a;

        /*
            Compute light color for each light source
        */
        for (int i = 0; i < numberLightSources; i++){
            vec3 lightCol = LightColor[i];
            float intensity = LightIntensity[i];
            float distanceLight = length( LightPosition_worldspace[i] - Position_worldspace );
            vec3 l = normalize( LightDirection_cameraspace[i] );
            float cosThetaLight = clamp( dot( n,l ), 0,1 );
            vec3 R_light = reflect(-l,n);
            float cosAlphaLight = clamp( dot( E,R_light ), 0,1 );
            tmpColor = tmpColor + 
                (DiffuseColor * lightCol * intensity * cosThetaLight / (distanceLight*distanceLight)) +
		(SpecularColor * lightCol * intensity * pow(cosAlphaLight,5) / (distanceLight*distanceLight));
        }

        tmpColor = tmpColor + AmbientColor;
        color = vec4(tmpColor.x,tmpColor.y, tmpColor.z, alpha);
}
