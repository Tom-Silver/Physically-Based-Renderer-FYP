#version 430 core

in VSOutput
{
	vec3 worldPos;
} fInput;

out vec4 fragColour;

uniform samplerCube environmentMap;

void main()
{
	vec3 colour = textureLod(environmentMap, fInput.worldPos, 0.0).rgb;

	// HDR tonemap/gamma correction
	colour = colour / (colour + vec3(1.0));
	colour = pow(colour, vec3(1.0 / 2.2));

	fragColour = vec4(colour, 1.0);
}