#version 430 core

layout (location = 0) in vec3 aPos;

out VSOutput
{
	vec3 worldPos;
} vOutput;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	vOutput.worldPos = aPos;

	gl_Position = projection * view * vec4(aPos, 1.0);
}