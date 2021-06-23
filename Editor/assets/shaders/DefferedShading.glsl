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

#define MAX_LIGHTS 16
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

uniform PointLight u_PointLights[MAX_LIGHTS];
uniform int u_num_point_light;

struct GBuffer
{
    sampler2D Position;
    sampler2D Normal;
    sampler2D Albedo;
};

uniform GBuffer gBuffer;

vec3 CalculatePointLight(PointLight Light, vec3 Normal, vec3 Position, vec3 ViewDir,vec3 Albedo,float specular, float SpecularPower)
{
    vec3 LightDir = normalize((u_World2View * vec4(Light.position,1.0)).xyz - Position);
    vec3 HalfwayDir = normalize(LightDir + ViewDir);

    vec3 Diffuse = vec3(max(dot(Normal, LightDir), 0.0));
    Diffuse *= Albedo;

    // Specular component
    vec3 RelcetionDir = reflect(-LightDir, Normal);
    vec3 Specular = vec3(pow(max(dot(Normal, HalfwayDir), 0.0), SpecularPower));
    Specular *= specular;

    // atenuation
    float Distance = length((u_World2View * vec4(Light.position,1.0)).xyz - Position);
    float Attenuation = 1.0 / (Distance * Distance);

    return (Attenuation * (Diffuse + Specular).xyz * Light.color.xyz);
}


void main()
{
	// LIGHTING
        // properties
    vec4 gNormalVS = texture(gBuffer.Normal, v_TexCoord);
    // vec3 gNormalWS = ( vec4(gNormalVS.xyz,0.0) * transpose(u_View2World)  ).xyz;
    vec4 gPosition = texture(gBuffer.Position, v_TexCoord);
    vec4 gAlbedo = texture(gBuffer.Albedo, v_TexCoord);


    vec3 viewDir = normalize( - gPosition.xyz);
    vec3 Lighting = vec3(0.00);
    for(int i = 0; i < u_num_point_light; i++)
            Lighting += CalculatePointLight(u_PointLights[i], gNormalVS.xyz, gPosition.xyz, viewDir, gAlbedo.xyz,gAlbedo.w, gNormalVS.w);

    color = vec4(Lighting,1.0);
    //color = gAlbedo;
}



