#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 vertex_normal;


uniform mat4 mvp;
uniform mat4 model;
uniform mat3 normalMatrix;
out vec3 normals;

out vec2 TextCoord;
out vec3 FragPos;

void main()
{
    gl_Position = mvp * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    normals = normalMatrix * vertex_normal;
    TextCoord = uv;

    FragPos = vec3(model * vec4(aPos, 1.0));
}