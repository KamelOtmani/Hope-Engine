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

// DEFINES
#define PI 3.14159265359
#define TAU 6.28318530718

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
uniform sampler2D u_EnvMap;

// GGX NDF
float D_GGX(float NoH, float roughness) {
    float a2 = roughness * roughness;
    float f = (NoH * a2 - NoH) * NoH + 1.0;
    return a2 / (PI * f * f);
}

// Schlick Fresnel approx (maybe use a better aprox ?)
vec3 F_Schlick(float u, vec3 f0) {
    return f0 + (vec3(1.0) - f0) * pow(1.0 - u, 5.0);
}

// vec3 F_Schlick(const vec3 f0, float f90, float VoH) {
//     // Schlick 1994, "An Inexpensive BRDF Model for Physically-Based Rendering"
//     return f0 + (f90 - f0) * pow5(1.0 - VoH);
// }

// Shadowing function using GGX Correlated
float V_SmithGGXCorrelated(float NoV, float NoL, float roughness) {
    float a2 = roughness * roughness;
    float GGXL = NoV * sqrt((-NoL * a2 + NoL) * NoL + a2);
    float GGXV = NoL * sqrt((-NoV * a2 + NoV) * NoV + a2);
    return 0.5 / (GGXV + GGXL);
}

float GeometrySchlickGGX(float NdotV, float k)
{
    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return nom / denom;
}
  
float GeometrySmith(float NdotV,float NdotL, float k)
{
    float ggx1 = GeometrySchlickGGX(NdotV, k);
    float ggx2 = GeometrySchlickGGX(NdotL, k);
	
    return ggx1 * ggx2;
}

// Using the lamertian Diffuse term
// TODO use diney lambertian with mutli scattering 
float Fd_Lambert() {
    return 1.0 / PI;
}

// float Fd_Burley(float roughness, float NoV, float NoL, float LoH) {
//     // Burley 2012, "Physically-Based Shading at Disney"
//     float f90 = 0.5 + 2.0 * roughness * LoH * LoH;
//     float lightScatter = F_Schlick(1.0, f90, NoL);
//     float viewScatter  = F_Schlick(1.0, f90, NoV);
//     return lightScatter * viewScatter * (1.0 / PI);
// }

// Cook Torance BRDF
// void BRDF(vec3 h,vec3 l,vec3 n,float roughness,vec3 f0,) {
//     vec3 h = normalize(v + l);

//     float NoV = abs(dot(n, v)) + 1e-5;
//     float NoL = clamp(dot(n, l), 0.0, 1.0);
//     float NoH = clamp(dot(n, h), 0.0, 1.0);
//     float LoH = clamp(dot(l, h), 0.0, 1.0);



//     float D = D_GGX(NoH, a);
//     vec3  F = F_Schlick(LoH, f0);
//     float V = V_SmithGGXCorrelated(NoV, NoL, roughness);

//     // specular BRDF
//     vec3 Fr = (D * V) * F;

//     // diffuse BRDF
//     vec3 Fd = diffuseColor * Fd_Lambert();

//     // apply lighting...
// }

vec2 DirToRectilinear(vec3 dir)
{
    float x = atan(dir.z,dir.x) / TAU + 0.5; // 0-1
    float y = dir.y * 0.5 + 0.5;
    return vec2(x,y);
}

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
    vec3 gNormalWS = ( vec4(gNormalVS.xyz,0.0) * transpose(u_View2World)  ).xyz;
    vec4 gPosition = texture(gBuffer.Position, v_TexCoord);
    vec4 gAlbedo = texture(gBuffer.Albedo, v_TexCoord);

    vec3 N = normalize(gNormalVS.xyz);
    vec3 V = normalize( - gPosition.xyz);

    vec3 Albedo = gAlbedo.rgb;
        // perceptually linear roughness to roughness (see parameterization)
    float Roughness = gNormalVS.w*gNormalVS.w;
    float Metalic = gAlbedo.w;
    vec3 f0 = vec3(0.04);
    f0      = mix(f0, Albedo, Metalic);
    vec3 Lighting = vec3(0.00);
    for(int i = 0; i < u_num_point_light; i++)
    {
        vec3 LightPosVS = (u_World2View * vec4(u_PointLights[i].position,1.0)).xyz;
        vec3 L = normalize(LightPosVS - gPosition.xyz);
        vec3 H = normalize(L + V);

        float distance    = length(LightPosVS- gPosition.xyz);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance     = u_PointLights[i].color.rgb * attenuation; 

        float NoV = abs(dot(N, V)) + 1e-5;
        float NoL = clamp(dot(N, L), 0.0, 1.0);
        float NoH = clamp(dot(N, H), 0.0, 1.0);
        float LoH = clamp(dot(L, H), 0.0, 1.0);

        float D = D_GGX(NoH, Roughness);
        vec3  F = F_Schlick(LoH, f0);
        // this is not working 
        // TODO : Investigate this
        // float V = V_SmithGGXCorrelated(NoV, NoL, 0.3);
        float V = GeometrySmith(NoV,NoL, Roughness);

        // Remapping 
        vec3 diffuseColor = (1.0 - Metalic) * Albedo;

        // Specular BRDF
        vec3 Fr = (D * V) * F;

        // Diffuse BRDF
        vec3 Fd = diffuseColor * Fd_Lambert();
        Lighting += (Fd + Fr)*radiance*NoL;
        // Lighting = vec3(V);
    }
    // vec3 AmbientLighting = textureLod(u_EnvMap,DirToRectilinear(gNormalWS),3).rgb*Albedo;
    vec3 AmbientLighting = vec3(0.25)*Albedo;
    color = vec4(Lighting + AmbientLighting,1.0);
    //color = gAlbedo;
}



