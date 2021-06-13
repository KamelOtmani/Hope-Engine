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

uniform vec3 u_LightPosWS; 
uniform vec3 u_CameraPositionWS; 
uniform vec3 u_LightColor;

void main()
{
	// ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * u_LightColor;
  	
    // diffuse 
    vec3 norm = normalize(WS_Normal);
    vec3 lightDir = normalize(u_LightPosWS - WS_Position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_LightColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(u_CameraPositionWS - WS_Position);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * u_LightColor;  

    vec3 result = (ambient + diffuse + specular) * u_Color.xyz;
    color = vec4(result, 1.0);
}