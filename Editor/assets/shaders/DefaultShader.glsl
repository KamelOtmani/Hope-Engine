#type vertex
#version 330 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec4 a_Color;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform vec4 u_Color;

out vec3 v_Position;
out vec4 v_Color;
out vec3 WS_Position;
out vec3 WS_Normal;

void main()
{
	v_Position = a_Position;
	v_Color = u_Color;
	WS_Position =  vec3(u_Transform * vec4(a_Position, 1.0));
	mat4 tiM = transpose( inverse( u_Transform ) );
	WS_Normal =  normalize((tiM*vec4(a_Normal, 1.0)).xyz);
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
}

#type pixel
#version 330 core
			
layout(location = 0) out vec4 color;

in vec3 v_Position;
in vec4 v_Color;
in vec3 WS_Position;
in vec3 WS_Normal;

uniform vec4 u_Color;

void main()
{
	float l = dot(WS_Normal,vec3(0.0f)-WS_Position);
	color = u_Color*l;
	//color = vec4(WS_Normal,1.0f);
}