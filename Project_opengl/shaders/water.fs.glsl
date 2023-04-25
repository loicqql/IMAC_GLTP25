#version 330 core

in vec4 clipSpace;
in vec2 textureCoords;

out vec4 fFragColor;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvMap;
uniform sampler2D normalMap;

uniform float moveWater;
uniform vec3 lightColor;

in vec3 toCameraVector;
in vec3 fromLightVector;

const float waveStrength = 0.008;
const float shineDamper = 30.0;
const float reflectivity = 0.5;

void main()
{
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
    vec3 viewVector = normalize(toCameraVector);
    float fresnelEffect = dot(viewVector, vec3(0.0, 1.0, 0.0));
    fresnelEffect = clamp(fresnelEffect, 0.5, 1.0);

    //Normal map
    vec4 normalMapColor = texture(normalMap, distortedTexCoords);
    vec3 normal = vec3(normalMapColor.r * 2.0 - 1.0, normalMapColor.b * 3.0, normalMapColor.g * 2.0 - 1.0);
    normal = normalize(normal);

    //Lights
    vec3 reflectedLight = reflect(normalize(fromLightVector), normal);
	float specular = max(dot(reflectedLight, viewVector), 0.0);
	specular = pow(specular, shineDamper);
	vec3 specularHighlights = lightColor * specular * reflectivity;

    fFragColor = mix(reflectColor, refractColor, fresnelEffect) + vec4(specularHighlights, 0.0);
    // fFragColor = mix(fFragColor, vec4(0.0, 0.3, 0.5, 1.0), 0.1); // add a blue color
}