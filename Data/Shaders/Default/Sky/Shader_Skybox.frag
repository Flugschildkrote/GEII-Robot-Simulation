#version 330 core 


in vec2 coordTexture;
flat in float textureID;

uniform sampler2D picture[6];

out vec4 out_Color;

void main()
{
	
	int tmp = int (textureID)*1;
	out_Color = texture(picture[tmp], coordTexture);
	//out_Color = vec4(0.5*tmp,1.0,0.0,1.0);
	if(out_Color.a == 0)
	{
		discard;
	}
}