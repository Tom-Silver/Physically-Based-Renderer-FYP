#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VSOutput
{
	vec2 texCoords;
	vec3 worldPos;
	vec3 normal;
} vOutput;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;

void main()
{
	vOutput.texCoords = aTexCoords;
	vOutput.worldPos = vec3(world * vec4(aPos, 1.0));
	vOutput.normal = mat3(transpose(inverse(world))) * aNormal;

	gl_Position = projection * view * world * vec4(aPos, 1.0);
}