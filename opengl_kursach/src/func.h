#pragma once

#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;
using namespace glm;

/// <summary>
/// �������� ������� �� �����
/// </summary>
/// <param name="filename">���� � �����.</param>
/// <returns>������ � ����� �������.</returns>
string LoadShader(const char* filename);

/// <summary>
/// ������� ������������� ����� � �������� ����.
/// </summary>
/// <param name="w">������ ����.</param>
/// <param name="h">������ ����.</param>
/// <param name="Fullscreen">����� ������� ������. </param>
/// <returns>��������� �� ��������� ����.</returns>
GLFWwindow* InitAll(int w = 640, int h = 480, bool Fullscreen = false);

/// <summary>
/// ������� ��� ����������� ���������� ���������.
/// </summary>
void EndAll();

/// <summary>
/// ���������� ������� ������.
/// </summary>
/// <param name="window">��������� �� ����.</param>
/// <param name="key">��� �������.</param>
/// <param name="scancode">��� �������.</param>
/// <param name="action">��������.</param>
/// <param name="mods">��������� ������������� �������.</param>
void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods);

bool loadOBJ(
	const char* path, //���� � �����
	std::vector<glm::vec3>& outVertices, //�������� ������ ���������
	std::vector<glm::vec2>& outTextures, //�������� ������ ���������� ���������
	std::vector<glm::vec3>& outNormals //�������� ������ ��������
);
