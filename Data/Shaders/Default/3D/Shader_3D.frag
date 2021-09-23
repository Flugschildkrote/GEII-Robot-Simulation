#version 330 core

in vec2 coordTexture;
smooth in vec3 vertexNormal;
in vec3 worldPos;
out vec4 out_Color;

struct AmbiantLight
{
	vec3 color;
	float intensity;
	vec3 mask;
};

struct DirectionnalLight
{
	float intensity;
	vec3 direction;
	vec3 color;
	vec3 mask;
};

struct PointLight
{
	float intensity;
	vec3 position;
	vec3 color;
	vec3 mask;
};

struct SphereLight
{
	float intensity;
	float radius;
	vec3 position;
	vec3 color;
	vec3 points[26];
	vec3 mask;
};

struct FogParameters
{
   vec3 color; // Fog color
   float start; // This is only for linear fog
   float end; // This is only for linear fog
   float density; // For exp and exp2 equation
   bool enabled;
   int mode; // 0 = linear, 1 = exp, 2 = exp2
};

uniform sampler2D tex;
uniform sampler2D lightTexture;
uniform bool useLightText;

uniform AmbiantLight ambiantLight;
uniform DirectionnalLight dirLight[10];
uniform PointLight pointLight[20];
uniform SphereLight sphLight[20];

uniform float modelIntensity;
uniform float specularIntensity;
uniform float specularPower;

uniform vec3 cameraPos;
uniform int nbDirLights;
uniform int nbPointLights;
uniform int nbSphereLights;
in vec4 eyeSpacePos;

uniform FogParameters sceneFog;

float getFogFactor(FogParameters params, float fFogCoord)
{
   float fResult = 0.0;
   if(params.mode == 0)
      fResult = (params.end-fFogCoord)/(params.end-params.start);
   else if(params.mode == 1)
      fResult = exp(-params.density*fFogCoord);
   else if(params.mode == 2)
      fResult = exp(-pow(params.density*fFogCoord, 2.0));
      
   fResult = 1.0- clamp(fResult, 0.0, 1.0);
   
   return fResult;
}

void main()
{
	vec3 posToCamVector = normalize(cameraPos-worldPos);
	vec4 textureColor = texture(tex, coordTexture);
	vec4 lightTextureColor;
	if(useLightText == true)
	{
		lightTextureColor = texture(lightTexture,coordTexture);
		//textureColor = texture(lightTexture, coordTexture);
		//textureColor = vec4(1,0,0,1);
	}
		
	vec3 normale = vertexNormal.xyz;
	
	
	vec4 finalDiffuseColor = vec4(0.0,0.0,0.0,1.0);
	vec4 finalSpecularColor = vec4(0.0,0.0,0.0,1.0);
	vec4 finalPointLightColor = vec4(0.0,0.0,0.0,1.0);
	vec4 finalSphereLightColor = vec4(0.0,0.0,0.0,1.0);
	
	for(int i = 0; i < nbDirLights; i++)
	{
		if(useLightText)
		{
			float R = lightTextureColor.x*255;
			float G = lightTextureColor.y*255;
			float B = lightTextureColor.z*255;
			
			if(!(dirLight[i].mask.x < R+2 && dirLight[i].mask.x > R -2 && 
			     dirLight[i].mask.y < G+2 && dirLight[i].mask.y > G -2 &&
			     dirLight[i].mask.z < B+2 && dirLight[i].mask.z > B -2))
			{
				continue;
			}
		}
		float tmpDiffuseIntensity = max(0.0, dot(normalize(normale),- normalize(dirLight[i].direction)));
		vec3 tmpReflectVector = normalize(reflect(dirLight[i].direction,normale));
		float tmpReflectIntensity = max(0.0, dot(posToCamVector,tmpReflectVector));
		tmpReflectIntensity = pow(tmpReflectIntensity,specularPower);
		
		vec4 tmpColor = vec4(dirLight[i].color,1.0f)*dirLight[i].intensity*tmpDiffuseIntensity;
		finalDiffuseColor += tmpColor;
		vec4 tmpSpecularColor = vec4(dirLight[i].color, 1.0f) * specularIntensity * tmpReflectIntensity * dirLight[i].intensity;
		finalSpecularColor += tmpSpecularColor;
	}
	
	for(int i = 0; i < nbPointLights; i++)
	{
		if(useLightText)
		{
			float R = lightTextureColor.x*255;
			float G = lightTextureColor.y*255;
			float B = lightTextureColor.z*255;
			
			if(!(pointLight[i].mask.x < R+2 && pointLight[i].mask.x > R -2 && 
			     pointLight[i].mask.y < G+2 && pointLight[i].mask.y > G -2 &&
			     pointLight[i].mask.z < B+2 && pointLight[i].mask.z > B -2))
			{
				continue;
			}
		}
		
		vec3 meshToPointLightVector = normalize(pointLight[i].position-worldPos);
		float pointLightFactor = max(0.0, dot(normalize(normale),meshToPointLightVector));
		vec3 tmpReflectVector = normalize(reflect(-meshToPointLightVector,normale));
		float tmpReflectIntensity = max(0.0, dot(posToCamVector,tmpReflectVector));
		tmpReflectIntensity = pow(tmpReflectIntensity,specularPower);
		vec3 vect = pointLight[i].position-worldPos;
		float Distance = length(vect);
		float Attenuation = 1 +      0 * Distance +      (0.2 * (Distance/2) * (Distance/2));
		//Attenuation = 1;
		vec4 tmpSpecularColor = vec4(pointLight[i].color, 1.0f) * specularIntensity * tmpReflectIntensity * pointLight[i].intensity /Attenuation;
		finalSpecularColor += tmpSpecularColor;
		vec4 pointLightColor = vec4(pointLight[i].color,1.0f)*pointLight[i].intensity*pointLightFactor/Attenuation;
		finalPointLightColor += pointLightColor;
	}
	
	for(int sphLightIndex = 0; sphLightIndex < nbSphereLights; sphLightIndex++)
	{
		float compteurPoints = 0;
		vec3 finalSphereVector = vec3(0,0,0);
		for(int i = 0; i < 26; i++)
		{
			vec3 tmpVector = sphLight[sphLightIndex].points[i]-worldPos;
			
			if(dot(normalize(normale), normalize(tmpVector)) >= 0.0)
			{
				finalSphereVector += tmpVector;
				compteurPoints += 1;
			}
		}
		
		if(compteurPoints == 0)
			compteurPoints = 1;
		
		finalSphereVector /= compteurPoints;
		float Distance = length(finalSphereVector);
		float Attenuation = 1 +      0 * Distance +      1 * Distance * Distance;
		vec3 pointToSphere = sphLight[sphLightIndex].position-worldPos;
		
		if(length(pointToSphere) > sphLight[nbSphereLights].radius)
		{
			vec3 tmpReflectVector = normalize(reflect(-pointToSphere,normale));
			float tmpReflectIntensity = max(0.0, dot(posToCamVector,tmpReflectVector));
			tmpReflectIntensity = pow(tmpReflectIntensity,specularPower);
			vec4 tmpSpecularColor = vec4(sphLight[sphLightIndex].color, 1.0f) * specularIntensity * tmpReflectIntensity* sphLight[sphLightIndex].intensity/Attenuation;
		    finalSpecularColor += tmpSpecularColor;
		}
		
		float sphereLightIntensity = max(0.0,dot(normalize(normale), normalize(finalSphereVector))); 
		vec4 finalSphereColor = vec4(sphLight[sphLightIndex].color,1.0f)*sphLight[sphLightIndex].intensity*sphereLightIntensity/Attenuation;
		finalSphereLightColor += finalSphereColor;
	}
	
	vec4 totalColors = vec4((ambiantLight.color*ambiantLight.intensity),1.0);
	out_Color = textureColor * vec4(modelIntensity,modelIntensity,modelIntensity,1.0) * (totalColors+finalDiffuseColor+finalSpecularColor+finalPointLightColor+finalSphereLightColor);
	
	vec3 gamma = vec3(1.0/2.2, 1.0/2.2, 1.0/2.2);
	out_Color = vec4(pow(out_Color.x, gamma.x),
                  pow(out_Color.y, gamma.y),
                  pow(out_Color.z, gamma.z),out_Color.a );
	if(out_Color.a == 0)
	{
		discard;
	}
	
	if(sceneFog.enabled)
	{
		float fFogCoord = abs(eyeSpacePos.z/eyeSpacePos.w);
		out_Color = mix(out_Color, vec4(sceneFog.color, 1.0), getFogFactor(sceneFog, fFogCoord));
	}
}