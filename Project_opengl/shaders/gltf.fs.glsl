#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

// Imports the current position from the Vertex Shader
in vec3 crntPos;
// Imports the normal from the Vertex Shader
in vec3 Normal;
// Imports the color from the Vertex Shader
in vec3 color;
// Imports the texture coordinates from the Vertex Shader
in vec2 texCoord;



// Gets the Texture Units from the main function
uniform sampler2D diffuse0;
uniform sampler2D specular0;
// Gets the color of the light from the main function
uniform vec3 lightColor;
// Gets the position of the light from the main function
uniform vec3 lightPosition;
// Gets the position of the camera from the main function
uniform vec3 camPos;

in vec4 LightSpacePos;

uniform sampler2D gShadowMap;

vec4 sunLight()
{

	vec3 lightVec = lightPosition - vec3(0); // lightDirection sun

	// ambient lighting
	float ambient = 0.25f;

	// intensity of light with respect to distance
	// float dist = length(lightVec);
	// float a = 3.0;
	// float b = 0.7;
	// float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;


	return (texture(diffuse0, texCoord) * (diffuse + ambient) + 1.0 * specular) * vec4(lightColor,1.0);
	// return (texture(diffuse0, texCoord) * (diffuse + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
}

void main() {

	vec3 ProjCoords = LightSpacePos.xyz / LightSpacePos.w;
    vec2 UVCoords;
    UVCoords.x = 0.5 * ProjCoords.x + 0.5;
    UVCoords.y = 0.5 * ProjCoords.y + 0.5;
    float z = 0.5 * ProjCoords.z + 0.5;

    float bias = 0.0003;
    float inshadow = 0.0;
    vec2 poissonDisk[4] = vec2[](
        vec2( -0.09420163, -0.03990622 ),
        vec2( 0.094558609, -0.07689073 ),
        vec2( -0.009418411, -0.0929389 ),
        vec2( 0.03449594, 0.02938777 )
    );


    for (int i=0;i<2;i++){
        if (texture(gShadowMap, UVCoords + poissonDisk[i]/700.0 ).x  + bias < z) {
            inshadow += 0.05;
        }
    }

	// outputs final color
	
	FragColor = sunLight();
	FragColor = FragColor - vec4(1) * inshadow;
}