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


struct gwLight
{
	vec3 attenuation;
	vec3 color;
	vec3 dir;
	float intensity;
	vec3 pos;
	int type;
};

uniform Material material;

uniform gwLight ambiant;
uniform gwLight light[MAX_LIGHTS];
uniform int lightsCount;
uniform mat4 modelMatrix;
uniform vec3 camPos;
uniform vec3 camDir;

out vec4 out_Color;


void main()
{


	int GW_LIGHT_DIRECTIONAL = 0;
	int GW_LIGHT_POINT = 1;
	
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
	
	if(material.lightSensitive == 0)
	{
		out_Color = meshColor;
		return;
	}
	
	
	vec3 sceneAmbiantColor = ambiant.color*ambiant.intensity;
	vec3 sunColor = vec3(0,0,0);
	vec3 pointColor = vec3(0,0,0);
	vec3 specularColor = vec3(0,0,0);
	
	/*vec4 vResult = vec4(0.0, 0.0, 0.0, 0.0);
   
   vec3 vReflectedVector = normalize(reflect(dLight.vDirection, vNormal));
   vec3 vVertexToEyeVector = normalize(vEyePos-vWorldPos);
   float fSpecularFactor = dot(vVertexToEyeVector, vReflectedVector);
   
   if (fSpecularFactor > 0)
   {
      fSpecularFactor = pow(fSpecularFactor, mat.fSpecularPower);
      vResult = vec4(dLight.vColor, 1.0) * mat.fSpecularIntensity * fSpecularFactor;
   }*/
   
	//return vResult; 

	for(int i = 0; i < lightsCount; i++)
	{
		vec3 lightDir;
		float attenuation;
		float dotIntensity;
		if(light[i].type == GW_LIGHT_DIRECTIONAL)
		{
			lightDir = -light[i].dir;
			dotIntensity = max(0.0, dot(normalize(vertexNormal), lightDir));
			sunColor += vec3(light[i].color*light[i].intensity*dotIntensity);
		}
		else if(light[i].type == GW_LIGHT_POINT)
		{
			vec3 fragmentToLight = light[i].pos-vertexWorldPos;
			lightDir = normalize(fragmentToLight);		
			float dist = length(fragmentToLight);
			dotIntensity = max(0.0, dot(normalize(vertexNormal), lightDir));
		    attenuation = 1.0/(light[i].attenuation.x + light[i].attenuation.y*dist + (light[i].attenuation.z*pow(dist,2)));
			pointColor += vec3(light[i].color*light[i].intensity*dotIntensity*attenuation);
		}
		if(dotIntensity > 0)
		{
			vec3 reflectVector = normalize(reflect(-lightDir, vertexNormal));
			vec3 fragToEyeVector = normalize(camPos-vertexWorldPos);
			float specularIntensity = dot(fragToEyeVector, reflectVector);
			
			if(specularIntensity > 0)
			{
				specularIntensity = pow(specularIntensity, material.reflectPower);
				specularColor += light[i].color*specularIntensity* material.reflectFactor*attenuation;
			}
		}
	}
	
	vec4 finalColor = vec4( (sceneAmbiantColor*meshColor.rgb) +  (meshColor.rgb*sunColor) + (meshColor.rgb*pointColor) + vec3(meshColor.rgb*specularColor), meshColor.a);
	out_Color = finalColor;
}