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

const float PI = 3.14159265359;
#define TAU 6.28318530718

uniform sampler2D u_EnvMap;
uniform float phi;
uniform float theta;
uniform float a;
uniform float b;

vec2 DirToRectilinear(vec3 dir)
{
    float x = atan(dir.z,dir.x) / TAU + 0.5; // 0-1
    //float y = dir.y * 0.5 + 0.5;
    float y = asin(dir.y)* 0.5 + 0.5;
    return vec2(x,y);
}

vec3 getNormal(vec2 uv)
{
    float phi=(uv.x-0.5)*3.1415*2;
    float theta=(uv.y-0.5)*2;
        
    return vec3(cos(phi)*cos(theta),sin(theta),sin(phi)*cos(theta));
}

// vec3 getNormal(vec2 uv)
// {
//     float x = cos(uv.x * 2 * PI);
//     float z = sin(uv.x * 2 * PI); 
//     float y = (uv.y - 0.5)*2;
//     return vec3(x,y,z)*TAU-0.5;
// }

void main()
{
// The world vector acts as the normal of a tangent surface
    // from the origin, aligned to WorldPos. Given this normal, calculate all
    // incoming radiance of the environment. The result of this radiance
    // is the radiance of light coming from -Normal direction, which is what
    // we use in the PBR shader to sample irradiance.
    vec3 N = normalize(getNormal(v_TexCoord));

    vec3 irradiance = vec3(0.0);   
    
    // tangent space calculation from origin point
    vec3 up    = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(up, N));
    up         = normalize(cross(N, right));
       
    float sampleDelta = 0.1;
    float nrSamples = 0.0;
    for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
    {
        for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
        {
            // spherical to cartesian (in tangent space)
            vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
            // tangent space to world
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N; 

            irradiance += texture(u_EnvMap, DirToRectilinear(sampleVec)).rgb * cos(theta) * sin(theta);
            nrSamples++;
        }
    }
    irradiance = PI * irradiance * (1.0 / float(nrSamples));
    color = vec4(irradiance,1.0);
    // color = texture(u_EnvMap, DirToRectilinear(N));
    
}



