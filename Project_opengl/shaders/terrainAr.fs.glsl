#version 330 core

in vec3 vColor;

in vec2 vTexCoord;
flat in vec2 vTexId;

in vec3 Normal;

in vec3 crntPos;

in vec4 LightSpacePos;

out vec4 fFragColor;

uniform sampler2D ar1;
uniform sampler2D ar2;
uniform sampler2D ar3;
uniform sampler2D gShadowMap;

uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 camPos;

vec4 direcLight(vec4 textureColor) {

	vec3 lightVec = lightPosition - vec3(0); // lightDirection sun

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


	return (textureColor * (diffuse + ambient) + 0.5 * specular) * vec4(lightColor,1.0);
}

void main() {
    
    fFragColor = texture(ar1, vTexCoord);
    fFragColor = mix(fFragColor, texture(ar2, vTexCoord), vTexId.x);
    fFragColor = mix(fFragColor, texture(ar3, vTexCoord), vTexId.y);

    

    vec3 ProjCoords = LightSpacePos.xyz / LightSpacePos.w;
    vec2 UVCoords;
    UVCoords.x = 0.5 * ProjCoords.x + 0.5;
    UVCoords.y = 0.5 * ProjCoords.y + 0.5;
    float z = 0.5 * ProjCoords.z + 0.5;

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

    fFragColor = direcLight(fFragColor);
    fFragColor = fFragColor - vec4(1) * visibility;
}