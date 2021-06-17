#type vertex
#version 330 core

			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec4 a_Color;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform vec4 u_Color;

out vec3 v_Position;
out vec4 v_Color;
out vec3 WS_Position;
out vec3 WS_Normal;
out vec2 v_TexCoord;

void main()
{
	v_Position = a_Position;
	v_Color = u_Color;
    v_TexCoord = a_TexCoord;
	WS_Position =  vec3(u_Transform * vec4(a_Position, 1.0));
	mat4 tiM = transpose( inverse( u_Transform ) );
	WS_Normal =  normalize((tiM*vec4(a_Normal, 1.0)).xyz);
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
}

#type pixel
#version 330 core

#define MAX_LIGHTS 16
#define CONSTANT 1.0
#define LINEAR 0.0
#define QUAD 2.0
			
layout(location = 0) out vec4 color;

in vec3 v_Position;
in vec4 v_Color;
in vec2 v_TexCoord;
in vec3 WS_Position;
in vec3 WS_Normal;

uniform vec3 u_CameraPositionWS; 

struct PointLight
{
    vec3 position;
    vec4 color;
};

struct Material
{
    vec4 Color;
    float SpecularPower;
    sampler2D AlbedoTexture;
    sampler2D SpecularTexture;
    bool hasAlbedoTexture;
    bool hasSpecularTexture;
};

uniform PointLight u_PointLights[MAX_LIGHTS];
uniform Material u_Material;
uniform int u_num_point_light;

vec3 CalculatePointLight(PointLight Light, vec3 NormalWS ,vec3 PositionWS, vec3 ViewDir)
{
    vec3 LightDir = normalize(Light.position - PositionWS);

    vec3 Diffuse = vec3(max(dot(NormalWS, LightDir),0.0));
    Diffuse *= u_Material.hasAlbedoTexture ? texture(u_Material.AlbedoTexture, v_TexCoord).xyz : vec3(1.0);
    
    // Specular component
    vec3 RelcetionDir = reflect(-LightDir,NormalWS);
    vec3 Specular = vec3(pow(max(dot(ViewDir,RelcetionDir),0.0),u_Material.SpecularPower));
    Specular *= u_Material.hasSpecularTexture ? texture(u_Material.SpecularTexture, v_TexCoord).xyz : vec3(1.0);

    // atenuation
    float Distance = length(Light.position - PositionWS);
    float Attenuation = 1.0 / (CONSTANT + LINEAR * Distance + QUAD * (Distance * Distance));

    return (Attenuation * (Diffuse + Specular).xyz * Light.color.xyz );
}

void main()
{
    // properties
    vec3 Normal = normalize(WS_Normal);
    vec3 viewDir = normalize(u_CameraPositionWS - WS_Position);

    vec3 Lighting = vec3(0.05);
    for(int i = 0; i < u_num_point_light; i++)
            Lighting += CalculatePointLight(u_PointLights[i], Normal, WS_Position, viewDir);    

    vec3 result = Lighting * u_Material.Color.xyz;
    color = vec4(result,1.0);
}



