#version 400
in vec3 vertex_position;
in vec3 vertex_color;
uniform mat4 MVP;
out vec3 color;
void main()
{
	color = vertex_color;
	gl_Position = MVP * vec4 (vertex_position, 1.0);
}