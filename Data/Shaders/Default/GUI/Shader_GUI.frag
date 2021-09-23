#version 330 core

in vec2 coordTexture;

uniform sampler2D tex;
uniform vec4 color;

out vec4 out_Color;

void main()
{
	if(color.a == 0){
		out_Color = texture(tex, coordTexture);
	}else{
		out_Color = color;
	}
	if(out_Color.a == 0){
		discard;
	}
}
