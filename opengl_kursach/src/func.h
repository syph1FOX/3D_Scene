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
/// Загрузка шейдера из файла
/// </summary>
/// <param name="filename">Путь к файлу.</param>
/// <returns>Строка с кодом шейдера.</returns>
string LoadShader(const char* filename);

/// <summary>
/// Функция инициализации всего и создания окна.
/// </summary>
/// <param name="w">Ширина окна.</param>
/// <param name="h">Высота окна.</param>
/// <param name="Fullscreen">Режим полного экрана. </param>
/// <returns>Указатель на созданное окно.</returns>
GLFWwindow* InitAll(int w = 640, int h = 480, bool Fullscreen = false);

/// <summary>
/// Функция для корректного завершения программы.
/// </summary>
void EndAll();

/// <summary>
/// Обработчик нажатия клавиш.
/// </summary>
/// <param name="window">Указатель на окно.</param>
/// <param name="key">Код клавиши.</param>
/// <param name="scancode">Код символа.</param>
/// <param name="action">Действие.</param>
/// <param name="mods">Состояние модификаторов нажатия.</param>
void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods);

bool loadOBJ(
	const char* path, //Путь к файлу
	std::vector<glm::vec3>& outVertices, //Выходной массив координат
	std::vector<glm::vec2>& outTextures, //Выходной массив текстурных координат
	std::vector<glm::vec3>& outNormals //Выходной массив нормалей
);
