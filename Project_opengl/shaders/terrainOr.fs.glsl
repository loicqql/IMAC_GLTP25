#version 330 core

struct Sun {
    vec3 color;
    vec3 position;
};

struct Spot {
    vec3 color;
    vec3 position;
    vec3 direction;
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

uniform sampler2D or1;
uniform sampler2D or2;
uniform sampler2D or3;
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
    if(theta > 0.978147) {
        return textureColor + vec4(spotBoat.color,1.0) / 2.0;
    }else {
        return textureColor;
    }
	
}

void main() {
    fFragColor = texture(or1, vTexCoord);
    fFragColor = mix(fFragColor, texture(or2, vTexCoord), vTexId.x);
    fFragColor = mix(fFragColor, texture(or3, vTexCoord), vTexId.y);

    vec3 ProjCoords = LightSpacePos.xyz / LightSpacePos.w;
    vec2 UVCoords;
    UVCoords.x = 0.5 * ProjCoords.x + 0.5;
    UVCoords.y = 0.5 * ProjCoords.y + 0.5;
    float z = 0.5 * ProjCoords.z + 0.5;
    float Depth = texture(gShadowMap, UVCoords).x;
    
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

    
    fFragColor = sunLight(fFragColor);
    fFragColor = spotLight(fFragColor);
    fFragColor = fFragColor - vec4(1) * inshadow;

    // if(vTexId == 1) {
    //     fFragColor = texture(or1, vTexCoord);
    // }else if(vTexId == 2) {
    //     fFragColor = texture(or2, vTexCoord);
    // }else if(vTexId == 3) {
    //     fFragColor = texture(or3, vTexCoord);
    // }else if(vTexId == 4) {
    //     fFragColor = texture(or4, vTexCoord);
    // }else if(vTexId == 5) {
    //     fFragColor = texture(ar1, vTexCoord);
    // }else if(vTexId == 6) {
    //     fFragColor = texture(ar2, vTexCoord);
    // }else if(vTexId == 7) {
    //     fFragColor = texture(ar3, vTexCoord);
    // }else if(vTexId == 8) {
    //     fFragColor = texture(ar4, vTexCoord);
    // }else if(vTexId == 9) {
    //     fFragColor = texture(small, vTexCoord);
    // }else {
    //     fFragColor = vec4(vColor, 1.0);
    // }
}