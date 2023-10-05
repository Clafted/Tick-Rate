#version 330 core

in vec2 textureCoord;
out vec4 FragColor;

uniform sampler2D txtr;

void main()
{
	FragColor = texture(txtr, textureCoord);
}