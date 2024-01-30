#version 400
in vec3 vertex_position;
in vec3 vertex_normal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 normal;
out vec3 f_pos;
void main ()
{
	//�������������� ��������� �������� � ������ ������� ������
	normal = mat3(transpose(inverse(model))) * vertex_normal;
	//�������������� ��������� ��������� � ������ ������� ������
	f_pos = vec3(model * vec4(vertex_position, 1.0));
	//������� ���������� ��������� ���������
	gl_Position = projection * view * model * vec4 (vertex_position, 1.0);
}