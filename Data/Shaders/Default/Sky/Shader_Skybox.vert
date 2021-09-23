#version 330 core

layout (location = 0) in vec3 in_Vertex;
layout (location = 2) in vec2 in_TexCoord0;
layout (location = 4) in float in_TextureID;

uniform mat4 MVP;

out vec2 coordTexture;
flat out float textureID;

void main()
{
	gl_Position = MVP* vec4(in_Vertex,1.0);
	textureID = in_TextureID;
	coordTexture = in_TexCoord0;
	
}