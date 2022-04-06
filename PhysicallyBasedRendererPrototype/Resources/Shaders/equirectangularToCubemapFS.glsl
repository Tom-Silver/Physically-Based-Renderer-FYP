#version 460 core

in VSOutput
{
	vec3 worldPos;
} fInput;

out vec4 fragColour;

uniform sampler2D equirectangularMap;

const vec2 invAtan = vec2(0.1591, 0.3183);

vec2 SampleSphericalMap(vec3 dir)
{
	vec2 uv = vec2(atan(dir.z, dir.x), asin(dir.y));
	uv *= invAtan;
	uv += 0.5;

	return uv;
}

void main()
{
	vec2 texCoords = SampleSphericalMap(normalize(worldPos));
	vec3 colour = texture(equirectangularMap, texCoords).rgb;

	fragColour = vec4(colour, 1.0);
}