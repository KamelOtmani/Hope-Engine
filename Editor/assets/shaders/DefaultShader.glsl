#type vertex
#version 330 core

			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec4 a_Color;

uniform mat4 u_View2World;
uniform mat4 u_World2View;
uniform mat4 u_View2Clip;
uniform mat4 u_World2Clip;

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
	mat4 tiM = transpose( inverse( u_World2View * u_Transform ) );
	// mat4 tiM = transpose( inverse( u_Transform ) );
	WS_Normal =  normalize((tiM*vec4(a_Normal, 1.0)).xyz);
	gl_Position = u_World2Clip * u_Transform * vec4(a_Position, 1.0);	
}

#type pixel
#version 330 core

#define MAX_LIGHTS 16
#define CONSTANT 1.0
#define LINEAR 0.0
#define QUAD 2.0
			
layout(location = 0) out vec4 gPosition;
layout(location = 1) out vec4 gNormal;
layout(location = 2) out vec4 gAlbedo;

in vec3 v_Position;
in vec4 v_Color;
in vec2 v_TexCoord;
in vec3 WS_Position;
in vec3 WS_Normal;

uniform vec3 u_CameraPositionWS; 
uniform mat4 u_View2World;
uniform mat4 u_World2View;
uniform mat4 u_View2Clip;
uniform mat4 u_World2Clip;

struct PointLight
{
    vec3 position;
    vec4 color;
};

struct Material
{
    vec4 Color;
    float Roughness;
    float Metalic;
    sampler2D AlbedoTexture;
    sampler2D RoughnessTexture;
    sampler2D MetalicTexture;
    bool hasAlbedoTexture;
    bool hasRoughnessTexture;
    bool hasMetalicTexture;
};

uniform PointLight u_PointLights[MAX_LIGHTS];
uniform Material u_Material;
uniform int u_num_point_light;


void main()
{
    // gPosition = vec4(1.0,0.0,0.0,1.0);

    vec3 Albedo = u_Material.hasAlbedoTexture ? texture(u_Material.AlbedoTexture, v_TexCoord).xyz * u_Material.Color.xyz: u_Material.Color.xyz;
    float Roughness = clamp(u_Material.hasRoughnessTexture ? texture(u_Material.RoughnessTexture, v_TexCoord).x * u_Material.Roughness: u_Material.Roughness,0.050,1.0);
    float Metalic = clamp(u_Material.hasMetalicTexture ? texture(u_Material.MetalicTexture, v_TexCoord).x * u_Material.Metalic: u_Material.Metalic,0.0,1.0);
    gAlbedo = vec4(Albedo, Metalic);
    gNormal = vec4(WS_Normal, Roughness);
    gPosition = u_World2View *vec4( WS_Position,1.0);


}

    //// properties
    //vec3 Normal = normalize(WS_Normal);
    //vec3 viewDir = normalize(u_CameraPositionWS - WS_Position);

    //vec3 Lighting = vec3(0.00);
    //for(int i = 0; i < u_num_point_light; i++)
    //        Lighting += CalculatePointLight(u_PointLights[i], Normal, WS_Position, viewDir);    

    //vec3 result = Lighting * u_Material.Color.xyz;
    // gPosition = u_View * vec4(WS_Position,1.0); // view-space
// vec3 CalculatePointLight(PointLight Light, vec3 NormalWS ,vec3 PositionWS, vec3 ViewDir)
// {
//     vec3 LightDir = normalize(Light.position - PositionWS);
//     vec3 HalfwayDir = normalize(LightDir + ViewDir);

//     vec3 Diffuse = vec3(max(dot(NormalWS, LightDir),0.0));
//     Diffuse *= u_Material.hasAlbedoTexture ? texture(u_Material.AlbedoTexture, v_TexCoord).xyz : vec3(1.0);
    
//     // Specular component
//     vec3 RelcetionDir = reflect(-LightDir,NormalWS);
//     vec3 Specular = vec3(pow(max(dot(NormalWS,HalfwayDir),0.0),u_Material.SpecularPower));
//     Specular *= u_Material.hasSpecularTexture ? texture(u_Material.SpecularTexture, v_TexCoord).xyz : vec3(1.0);

//     // atenuation
//     float Distance = length(Light.position - PositionWS);
//     float Attenuation = 1.0 / (Distance * Distance);

//     return (Attenuation * (Diffuse + Specular).xyz * Light.color.xyz );
// }


