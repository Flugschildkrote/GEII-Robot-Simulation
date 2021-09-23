#version 330 core

layout (location = 0) in vec3 in_Vertex;
layout (location = 2) in vec2 in_TexCoord0;

uniform mat4 matrix;

out vec2 coordTexture;

void main()
{
    gl_Position = matrix*vec4(in_Vertex , 1.0);
    coordTexture = in_TexCoord0;
}
