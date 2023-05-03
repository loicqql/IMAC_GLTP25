#version 330 core

struct Sun {
    vec3 color;
    vec3 position;
};

struct Spot {
    vec3 color;
    vec3 position;
    vec3 direction;
    float strength;
    float cutOff;
    float outerCutOff;
};

uniform Sun sun;
uniform Spot spotBoat;

in vec3 vColor;

in vec2 vTexCoord;
flat in vec2 vTexId;

in vec3 Normal;

in vec3 crntPos;

in vec4 LightSpacePos;

out vec4 fFragColor;

uniform sampler2D small;
uniform sampler2D gShadowMap;

uniform vec3 camPos;

vec4 sunLight(vec4 textureColor) {

	vec3 lightVec = sun.position - vec3(0); // lightDirection sun

	// ambient lighting
	float ambient = 0.45f;

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


	return (textureColor * (diffuse + ambient) + 0.5 * specular) * vec4(sun.color,1.0);
}

vec4 spotLight(vec4 textureColor) {

    vec3 lightDirection = normalize(spotBoat.position - crntPos);

    float theta = dot(lightDirection, normalize(spotBoat.direction));
    float epsilon = spotBoat.cutOff - spotBoat.outerCutOff;
    float intensity = clamp((theta - spotBoat.outerCutOff) / epsilon, 0.0, 1.0); 

    return textureColor + ((vec4(spotBoat.color,1.0) / 3.0 ) * intensity) * spotBoat.strength;
	
}


void main() {
    fFragColor = texture(small, vTexCoord);
    // fFragColor = vec4(vColor, 1.0);

    vec3 ProjCoords = LightSpacePos.xyz / LightSpacePos.w;
    vec2 UVCoords;
    UVCoords.x = 0.5 * ProjCoords.x + 0.5;
    UVCoords.y = 0.5 * ProjCoords.y + 0.5;
    float z = 0.5 * ProjCoords.z + 0.5;
    float Depth = texture(gShadowMap, UVCoords).x;
    
    float bias = 0.0003;
    float visibility = 0.0;
    vec2 poissonDisk[4] = vec2[](
        vec2( -0.09420163, -0.03990622 ),
        vec2( 0.094558609, -0.07689073 ),
        vec2( -0.009418411, -0.0929389 ),
        vec2( 0.03449594, 0.02938777 )
    );

    for (int i=0;i<2;i++){
        if (texture(gShadowMap, UVCoords + poissonDisk[i]/700.0 ).x  + bias < z) {
            visibility += 0.05;
        }
    }

    fFragColor = sunLight(fFragColor);
    fFragColor = spotLight(fFragColor);
    fFragColor = fFragColor - vec4(1) * visibility;
}