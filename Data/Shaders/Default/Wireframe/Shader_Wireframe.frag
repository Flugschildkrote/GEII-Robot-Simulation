#version 330 core

out vec4 out_Color;

uniform vec3 linesColor;

void main()
{
	out_Color = vec4(linesColor,1.0);
}