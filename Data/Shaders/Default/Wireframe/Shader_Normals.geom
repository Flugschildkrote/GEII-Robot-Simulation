#version 330 core

layout(triangles) in;
layout(line_strip, max_vertices = 12) out;

in vec4 normalStart[3];
in vec4 normalEnd[3];

void main()
{
	for(int i = 0; i < 3; i++)
	{
		gl_Position = normalStart[i]; 
		EmitVertex();
		gl_Position = normalEnd[i]; 
		EmitVertex();
		EndPrimitive();

	}
	
	for(int i = 0; i < 3; i++)
	{
		gl_Position = normalStart[i]; 
		EmitVertex();
	}
	gl_Position = normalStart[0]; 
		EmitVertex();
	EndPrimitive();
}