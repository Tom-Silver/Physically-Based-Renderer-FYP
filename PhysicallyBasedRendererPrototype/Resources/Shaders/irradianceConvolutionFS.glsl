#version 430 core

in VSOutput
{
	vec3 worldPos;
} fInput;

out vec4 fragColour;

uniform samplerCube environmentMap;

const float pi = 3.14159265359;

void main()
{
	vec3 N = normalize(fInput.worldPos);
	vec3 irradiance = vec3(0.0);

	vec3 up = vec3(0.0, 1.0, 0.0);
	vec3 right = normalize(cross(up, N));
	up = normalize(cross(N, right));

	float sampleDelta = 0.025;
	float noSamples = 0.0;
	for (float phi = 0.0; phi < 2.0 * pi; phi += sampleDelta)
	{
		for (float theta = 0.0; theta < 0.5 * pi; theta += sampleDelta)
		{
			vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
			vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;

			irradiance += texture(environmentMap, sampleVec).rgb * cos(theta) * sin(theta);
			noSamples++;
		}
	}

	irradiance = pi * irradiance * (1.0 / float(noSamples));
	
	fragColour = vec4(irradiance, 1.0);
}