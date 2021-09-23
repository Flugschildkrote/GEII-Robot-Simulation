#version 330 core

in vec3 in_Vertex;
in vec3 in_Colors;

uniform mat4 modelViewProjection;

out vec3 geom_colors;

void main()
{
	geom_colors = in_Colors;
	gl_Position = modelViewProjection*vec4(in_Vertex,1.0);
	//gl_Position = vec4(0.0,1.0,0.0,1.0);
}