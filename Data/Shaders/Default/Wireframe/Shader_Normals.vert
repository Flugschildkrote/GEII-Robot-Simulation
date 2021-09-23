#version 330 core

in vec3 in_Vertex;
in vec3 in_Normals;

uniform mat4 MVP;
uniform mat4 normalsMatrix;
uniform mat4 modelMatrix;

out vec4 normalStart;
out vec4 normalEnd;

void main()
{
	float lineSize = 0.5;
	normalStart = MVP*vec4(in_Vertex,1.0);
	normalEnd = MVP*vec4(in_Vertex+(in_Normals*lineSize) ,1.0);
	//vertexPos = (MVP*vec4(in_Vertex,1.0));
    //gl_Position = vertexPos;
	
	//vec4 normal = normalize(normalsMatrix*vec4(in_Normals,0.0));
	//vertexNormal = normal.xyz;
}
