#type vertex
#version 330 core

			
layout(location = 0) in vec3 a_Position;
layout(location = 2) in vec2 a_TexCoord;


out vec3 v_Position;
out vec2 v_TexCoord;

void main()
{
	v_Position = a_Position;
	v_TexCoord = a_TexCoord;
	gl_Position = vec4(a_Position, 1.0);	
}

#type pixel
#version 330 core
			
layout(location = 0) out vec4 color;

in vec3 v_Position;
in vec2 v_TexCoord;

uniform sampler2D u_MainTexture;

vec3 ACESFilm(vec3 x)
{
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    return clamp((x*(a*x+b))/(x*(c*x+d)+e),0.0,1.0);
}

void main()
{
    vec3 linear = texture(u_MainTexture, v_TexCoord).xyz;
    vec3 TonemappedColor = ACESFilm(linear);
    vec3 GammaCorrected = pow(TonemappedColor,vec3(1/2.2));
    color = vec4(GammaCorrected,1.);
    //color = vec4(v_TexCoord,0.0,1.0);
}



