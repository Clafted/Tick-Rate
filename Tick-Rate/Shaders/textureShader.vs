#version 330 core

layout (location = 0) in vec3 fragPos;
layout (location = 1) in vec2 texCoord;

uniform mat4 model;
uniform mat4 view;

out vec2 textureCoord;

void main()
{
	gl_Position = view * model * vec4(fragPos, 1.0);
	textureCoord = texCoord;
}