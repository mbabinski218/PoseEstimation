#version 440

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_texcoord;
layout(location = 3) in vec3 vertex_color;
layout(location = 4) in ivec4 bone_ids; 
layout(location = 5) in vec4 weights;

out vec3 vs_position;
out vec3 vs_color;
out vec2 vs_texcoord;
out vec3 vs_normal;

const int MAX_BONES = 100;

uniform int MaxBoneInfluence;
uniform mat4 FinalBonesMatrices[MAX_BONES];
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
	vs_position = vec4(ModelMatrix * vec4(vertex_position, 1.f)).xyz;
	vs_color = vertex_color;
	vs_texcoord = vec2(vertex_texcoord.x, vertex_texcoord.y * -1.f);
	vs_normal = mat3(ModelMatrix) * vertex_normal;

	vec4 totalPosition = vec4(0.0f);
    for(int i = 0 ; i < MaxBoneInfluence ; i++)
    {
        if(bone_ids[i] == -1) 
            continue;
        if(bone_ids[i] >= MAX_BONES) 
        {
            totalPosition = vec4(vertex_position,1.0f);
            break;
        }
        vec4 localPosition = FinalBonesMatrices[bone_ids[i]] * vec4(vertex_position, 1.0f);
        totalPosition += localPosition * weights[i];
        vec3 localNormal = mat3(FinalBonesMatrices[bone_ids[i]]) * vertex_normal;
   }
	
    gl_Position =  ProjectionMatrix * ViewMatrix * ModelMatrix * totalPosition;
}