#version 330 core

in vec3 frag_color;
out vec4 out_Color;

uniform vec3 linesColor;

void main()
{
	//out_Color = vec4(1.0,0.0,1.0,1.0);
	out_Color = vec4(frag_color,1.0);

}