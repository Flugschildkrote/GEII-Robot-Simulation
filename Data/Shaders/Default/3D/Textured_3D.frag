#version 330 core
#define MAX_LIGHTS 100

in vec2 coordTexture;
smooth in vec3 vertexNormal;
in vec3 vertexWorldPos;

struct Material
{
	vec3 color;
	int lightSensitive;
	float pictureIntensity;
	sampler2D picture;
	float reflectFactor;
	float reflectPower;
};

uniform Material material;

uniform int lightsCount;
uniform mat4 modelMatrix;
uniform vec3 camPos;
uniform vec3 camDir;

out vec4 out_Color;


void main()
{

	vec3 materialColor = material.color;
	vec4 meshColor = vec4(materialColor.rgb, 1.0);
	if(material.pictureIntensity != 0) 
	{ 
		vec4 textureColor = texture(material.picture, coordTexture);
		meshColor = vec4(materialColor*(1.0f - material.pictureIntensity) + textureColor.rgb*material.pictureIntensity, meshColor.a);
	}
	else
	{
		meshColor = vec4(materialColor, 1.0);
	}
	out_Color = meshColor;
}