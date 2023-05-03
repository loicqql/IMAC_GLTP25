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

in vec4 clipSpace;
in vec2 textureCoords;

out vec4 fFragColor;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvMap;
uniform sampler2D normalMap;

uniform float moveWater;

uniform vec3 camPos;

in vec3 crntPos;


const float waveStrength = 0.008;
const float shineDamper = 30.0;
const float reflectivity = 0.5;

void main()
{

    vec3 fromSunVector = normalize(sun.position - crntPos);
    vec3 fromCamVector = normalize(camPos - crntPos);

    vec3 fromProjectedLight = normalize((spotBoat.position + spotBoat.direction * -3)- crntPos);
    vec3 fromSpotVector = normalize(spotBoat.position - crntPos);
    
    //reflecTexCoords & refracTexCoords
    vec2 ndc = (clipSpace.xy / clipSpace.w)/2.0 + 0.5;
    vec2 refracTexCoords = vec2(ndc.x, ndc.y);
    vec2 reflecTexCoords = vec2(ndc.x, -ndc.y);

    //distortion
    vec2 distortedTexCoords = texture(dudvMap, vec2(textureCoords.x - moveWater, textureCoords.y)).rg*0.1;
	distortedTexCoords = textureCoords + vec2(distortedTexCoords.x, distortedTexCoords.y-moveWater);
	vec2 totalDistortion = (texture(dudvMap, distortedTexCoords).rg * 2.0 - 1.0) * waveStrength;

    refracTexCoords += totalDistortion;
    refracTexCoords = clamp(refracTexCoords, 0.001, 0.999);

    reflecTexCoords += totalDistortion;
    reflecTexCoords.x = clamp(reflecTexCoords.x, 0.001, 0.999);
    reflecTexCoords.y = clamp(reflecTexCoords.y, -0.999, -0.001);

    vec4 reflectColor = texture(reflectionTexture, reflecTexCoords);
    vec4 refractColor = texture(refractionTexture, refracTexCoords);

    //fresnelEffect
    vec3 viewVector = normalize(-fromCamVector);
    float fresnelEffect = dot(viewVector, vec3(0.0, 1.0, 0.0));
    fresnelEffect = clamp(fresnelEffect, 0.5, 1.0);

    //Normal map
    vec4 normalMapColor = texture(normalMap, distortedTexCoords);
    vec3 normal = vec3(normalMapColor.r * 2.0 - 1.0, normalMapColor.b * 3.0, normalMapColor.g * 2.0 - 1.0);
    normal = normalize(normal);

    //Lights
    vec3 reflectedLight = reflect(normalize(fromSunVector), normal);
	float specular = max(dot(reflectedLight, viewVector), 0.0);
	specular = pow(specular, shineDamper);
	vec3 specularHighlights = sun.color * specular * reflectivity;

    //Spot
    vec3 reflectedSpot = reflect(normalize(fromSpotVector), normal);
	float specularSpot = max(dot(reflectedSpot, viewVector), 0.0);
	specularSpot = pow(specularSpot, shineDamper * 3);
	vec3 specularHighlightsSpot = spotBoat.color * specularSpot * (reflectivity * 0.7) * spotBoat.strength;

    //Spot Projected
    vec3 reflectedProjectedSpot = reflect(normalize(-fromSpotVector), normal);
	float specularProjectedSpot = max(dot(reflectedProjectedSpot, normalize(fromProjectedLight)), 0.0);
    float specularProjectedSpotCam = max(dot(reflectedProjectedSpot, viewVector), 0.0); // filter with cam view
	specularProjectedSpot = pow(specularProjectedSpot, 50.0);
	vec3 specularHighlightsProjectedSpot = spotBoat.color * specularProjectedSpot * specularProjectedSpotCam * (reflectivity * 0.8);

    fFragColor = mix(reflectColor, refractColor, fresnelEffect) + vec4(specularHighlights, 0.0);

    //Spot
    fFragColor += vec4(specularHighlightsProjectedSpot, 0.0);

    //spot in cam
    vec3 lightDirection = normalize(spotBoat.position - camPos);
    float theta = dot(lightDirection, normalize(spotBoat.direction));
    float epsilon = spotBoat.cutOff - spotBoat.outerCutOff;
    float intensity = clamp((theta - spotBoat.outerCutOff) / epsilon, 0.0, 1.0);
    fFragColor += vec4(specularHighlightsSpot, 0.0) * intensity;

    fFragColor = mix(fFragColor, vec4(0.0, 0.3, 0.5, 1.0), 0.15); // add a blue color
}