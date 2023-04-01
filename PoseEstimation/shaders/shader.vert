//#version 440
//
//layout(location = 0) in vec3 aPosition;
//layout(location = 1) in vec3 aNormal;
//
//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;
//uniform vec4 color;
//
//out vec3 WorldPos;
//out vec3 Normal;
//out vec4 Color;
//
//void main()
//{
//  Color = color;
//  WorldPos = vec3(model * vec4(aPosition, 1.0));
//  Normal = aNormal;
//
//  gl_Position = projection * view * model * vec4(aPosition, 1.0f);
//
//}

#version 440

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_texcoord;
layout(location = 3) in vec3 vertex_color;

out vec3 vs_position;
out vec3 vs_color;
out vec2 vs_texcoord;
out vec3 vs_normal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
	vs_position = vec4(ModelMatrix * vec4(vertex_position, 1.f)).xyz;
	vs_color = vertex_color;
	vs_texcoord = vec2(vertex_texcoord.x, vertex_texcoord.y * -1.f);
	vs_normal = mat3(ModelMatrix) * vertex_normal;

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vertex_position, 1.f);
}