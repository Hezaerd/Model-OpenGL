#version 330 core

in vec3 normals;

out vec4 fragment_color;

void main()
{       
    fragment_color = vec4(normals, 1.0 );
}