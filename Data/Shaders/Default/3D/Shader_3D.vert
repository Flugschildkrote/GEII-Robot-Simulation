#version 330 core

in vec3 in_Vertex;
in vec2 in_TexCoord0;
in vec3 in_Normals;

uniform mat4 modelViewProjection;
uniform mat4 modelView;
uniform mat4 normalsMatrix;
uniform mat4 modelMatrix;

out vec2 coordTexture;
smooth out vec3 vertexNormal;
out vec3 worldPos;
out vec4 eyeSpacePos;

void main()
{
    gl_Position = modelViewProjection* vec4(in_Vertex,1.0);

    coordTexture = in_TexCoord0;
	vec4 normal = normalize(normalsMatrix*vec4(in_Normals,0.0));
	vec4 pos = modelMatrix*vec4(in_Vertex,1.0);
	
	//vec4 normal = vec4(in_Normals,0.0);
	vertexNormal = normal.xyz;
	worldPos = pos.xyz;
	eyeSpacePos = modelView*vec4(in_Vertex, 1.0);
	//vertexNormal = in_Normals;
}
