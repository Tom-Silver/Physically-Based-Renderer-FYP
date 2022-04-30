#version 460 core

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

	mat4 rotView = mat4(mat3(view));

	vec4 clipPos = projection * rotView * vec4(aPos, 1.0);
	gl_Position = clipPos.xyww;
}