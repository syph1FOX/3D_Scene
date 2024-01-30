#version 420 core
in vec3 vertex_position;

out vec3 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = vertex_position;
    gl_Position = projection * mat4(mat3(view)) * vec4(vertex_position, 1.0);
}  