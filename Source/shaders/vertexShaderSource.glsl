#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 vertex_normal;

uniform mat4 mvp;

out vec3 normals;

void main()
{
	normals = vertex_normal;
	gl_Position = mvp * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}