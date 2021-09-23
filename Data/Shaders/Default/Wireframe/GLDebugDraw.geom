#version 330 core

layout(lines) in;
layout(line_strip, max_vertices = 2) out;

uniform mat4 modelViewProjection;

in vec3 geom_colors[2];
out vec3 frag_color;

void main()
{
	for(int i = 0; i < 2; i++)
	{
		frag_color = geom_colors[i];
		gl_Position = gl_in[i].gl_Position; 
		EmitVertex();
	}
	EndPrimitive();
}