
#include "Model.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

Model& Model::operator=(const Model& other)
{
	vao = other.vao;
	vertecesCount = other.vertecesCount;
	firstIndexOffset = other.firstIndexOffset;
	indicesCount = other.indicesCount;

	coordsVbo = other.coordsVbo;
	vao = other.vao;
	tex_vbo = other.tex_vbo;

	return *this;
}


void Model::render()
{
	//render(projectionMatrix, modelMatrix, viewMatrix, renderMode);
}
void Model::render(GLuint mode)
{
	//Самый простой вариант - просто рисует, ничего в шейдер не передаёт.
	//Выбираем шейдерную программу
	glUseProgram(shaderProgram);
	//Активируем массив вершин
	glBindVertexArray(vao);
	//Если индексы есть - рисуем по ним
	if (indicesCount > 0)
		glDrawElements(mode, indicesCount, GL_UNSIGNED_INT, (void*)firstIndexOffset);
	else
		glDrawArrays(mode, 0, vertecesCount);
}
void Model::render(mat4 projection, mat4 model, mat4 view, GLuint mode, vec3 campos, LightsInfo lights)
{
	//Более сложный вариант - с использованием матрицы преобразований

	//Сначала всегда выбираем программу
	glUseProgram(shaderProgram);

	SetUniformVars(projection, model, view, campos, lights);

	if (texID != 0)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texID);
	}
	

	//А дальше всё так же как и в простом случае
	glBindVertexArray(vao);
	if (indicesCount > 0)
		glDrawElements(mode, indicesCount, GL_UNSIGNED_INT, (void*)firstIndexOffset);
	else
		glDrawArrays(mode, 0, vertecesCount);
}

void Model::render(mat4 projection, mat4 view, vec3 campos, LightsInfo lights)
{
	render(projection, modelMatrix, view, renderMode, campos, lights);
}

void Model::SetUniformVars(mat4 projection, mat4 model, mat4 view, vec3 campos, LightsInfo lights)
{
	GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

	GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);

	GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

	SetUniformVec3("campos", campos);

	SetUniformMaterial();

	SetUniformLights(lights);
}

void Model::SetUniformMaterial()
{
	SetUniformVec3("material.ambient", material.ambient);
	SetUniformVec3("material.diffuse", material.diffuse);
	SetUniformVec3("material.specular", material.specular);

	SetUniformFloat("material.shininess", material.shininess);
}

void Model::SetUniformLights(LightsInfo lights)
{
	SetUniformVec3("lights.dirLight.direction", lights.dirLight.direction);

	SetUniformVec3("lights.dirLight.ambient", lights.dirLight.ambient);
	SetUniformVec3("lights.dirLight.diffuse", lights.dirLight.diffuse);
	SetUniformVec3("lights.dirLight.specular", lights.dirLight.specular);


	GLuint numPLights = glGetUniformLocation(shaderProgram, "lights.numPLights");
	glUniform1i(numPLights, lights.pointLights.size());
	GLuint numSLights = glGetUniformLocation(shaderProgram, "lights.numSLights");
	glUniform1i(numSLights, lights.spotLights.size());

	
	for (int i = 0; i < lights.pointLights.size(); i++)
	{
		// fucking c++ moment
		char name[80];
		sprintf(name, "lights.pointLights[%d].ambient", i);
		SetUniformVec3(name, lights.pointLights[i].ambient);

		sprintf(name, "lights.pointLights[%d].diffuse", i);
		SetUniformVec3(name, lights.pointLights[i].diffuse);

		sprintf(name, "lights.pointLights[%d].specular", i);
		SetUniformVec3(name, lights.pointLights[i].specular);


		sprintf(name, "lights.pointLights[%d].position", i);
		SetUniformVec3(name, lights.pointLights[i].position);

		sprintf(name, "lights.pointLights[%d].constant", i);
		SetUniformFloat(name, lights.pointLights[i].constant);

		sprintf(name, "lights.pointLights[%d].linear", i);
		SetUniformFloat(name, lights.pointLights[i].linear);

		sprintf(name, "lights.pointLights[%d].quadratic", i);
		SetUniformFloat(name, lights.pointLights[i].quadratic);
	}

	for (int i = 0; i < lights.spotLights.size(); i++)
	{
		// fucking c++ moment
		char name[80];
		sprintf(name, "lights.spotLights[%d].ambient", i);
		SetUniformVec3(name, lights.spotLights[i].ambient);

		sprintf(name, "lights.spotLights[%d].diffuse", i);
		SetUniformVec3(name, lights.spotLights[i].diffuse);

		sprintf(name, "lights.spotLights[%d].specular", i);
		SetUniformVec3(name, lights.spotLights[i].specular);


		sprintf(name, "lights.spotLights[%d].position", i);
		SetUniformVec3(name, lights.spotLights[i].position);

		sprintf(name, "lights.spotLights[%d].direction", i);
		SetUniformVec3(name, lights.spotLights[i].direction);

		sprintf(name, "lights.spotLights[%d].constant", i);
		SetUniformFloat(name, lights.spotLights[i].constant);

		sprintf(name, "lights.spotLights[%d].linear", i);
		SetUniformFloat(name, lights.spotLights[i].linear);

		sprintf(name, "lights.spotLights[%d].quadratic", i);
		SetUniformFloat(name, lights.spotLights[i].quadratic);

		sprintf(name, "lights.spotLights[%d].cutOff", i);
		SetUniformFloat(name, lights.spotLights[i].cutOff);

		sprintf(name, "lights.spotLights[%d].outerCutOff", i);
		SetUniformFloat(name, lights.spotLights[i].outerCutOff);
	}
}

void Model::SetUniformVec3(const char* nameInShader, vec3 vec)
{
	GLuint location = glGetUniformLocation(shaderProgram, nameInShader);
	glUniform3fv(location, 1, value_ptr(vec));
}

void Model::SetUniformFloat(const char* nameInShader, float val)
{
	GLuint location = glGetUniformLocation(shaderProgram, nameInShader);
	glUniform1fv(location, 1, &val);
}


void Model::load_coords(glm::vec3* verteces, size_t count)
{
	//Сохранение размера массива
	vertecesCount = count;
	//Создание VBO
	coordsVbo = 0;
	glGenBuffers(1, &coordsVbo);
	glBindBuffer(GL_ARRAY_BUFFER, coordsVbo);
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::vec3), verteces, GL_STATIC_DRAW);
	//Так как VAO уже создан, то можно сразу связать с ним
	glBindVertexArray(vao);
	//Собственно связь
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	//Подключение атрибута
	glEnableVertexAttribArray(0);
}

void Model::load_colors(glm::vec3* colors, size_t count)
{
	//Аналогично координатам
	//Размер сохранять не требуется - важно только число вершин
	GLuint colors_vbo = 0;
	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::vec3), colors, GL_STATIC_DRAW);
	glBindVertexArray(vao);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);
}

void Model::load_indices(GLuint* indices, size_t count)
{
	//Сохранение числа индексов
	indicesCount = count;
	//Дальше всё почти как всегда с поправкой на тип буфера
	glBindVertexArray(vao);
	GLuint elementbuffer;
	glGenBuffers(1, &elementbuffer); // Генерация одного объекта буфера вершин
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer); // Привязка элементного буфера
	//Загрузка индексов в используемый элементный буфер
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), indices, GL_STATIC_DRAW);
}

void Model::load_modelMatrix(glm::mat4 mat)
{
	modelMatrix = mat;
}

void Model::load_material(Material mater)
{
	material = mater;
}

void Model::load_shaders(const char* vect, const char* frag)
{
	// Переменные под результат компиляции программы
	GLint result = GL_FALSE;
	int infoLogLength;

	//Создание шейдерной программы
	shaderProgram = glCreateProgram();

	//Загрузка текстов шейдеров из файлов
	string vstext = LoadShader(vect);
	const char* vertex_shader = vstext.c_str();

	string fstext = LoadShader(frag);
	const char* fragment_shader = fstext.c_str();

	//Создание вершинного шейдера
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);

	//Проверка результата компиляции
	glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &infoLogLength);

	//Вывод сообщения об ошибке если что-то пошло не так
	if (infoLogLength > 0)
	{
		char* errorMessage = new char[infoLogLength + 1];
		glGetShaderInfoLog(vs, infoLogLength, NULL, errorMessage);
		std::cout << errorMessage;
		delete[] errorMessage;
	}

	//Аналогично с фрагментным шейдером
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength > 0)
	{
		char* errorMessage = new char[infoLogLength + 1];
		glGetShaderInfoLog(fs, infoLogLength, NULL, errorMessage);
		std::cout << errorMessage;
		delete[] errorMessage;
	}

	//Сборка программы
	glAttachShader(shaderProgram, vs);
	glAttachShader(shaderProgram, fs);

	//Тонкий момент - тут жёстко связываются названия входных переменных в шейдерах со атрибутами массива вершин
	//По хорошему, это можно вынести в отдельные функции для большей универсальности.
	//Однако так как это пример, то оставлено так
	glBindAttribLocation(shaderProgram, 0, "vertex_position");
	glBindAttribLocation(shaderProgram, 1, "vertex_color");

	//Компоновка шейдерной программы
	glLinkProgram(shaderProgram);
}

// Замещает массив цветов в шейдере
void Model::load_normals(vec3* normals, size_t count)
{
	normalsCount = count;

	GLuint normalsVbo = 0;
	glGenBuffers(1, &normalsVbo);
	glBindBuffer(GL_ARRAY_BUFFER, normalsVbo);
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(vec3), normals, GL_STATIC_DRAW);
	glBindVertexArray(vao);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);
}

void Model::load_texture(const char* path, int type)
{
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, STBI_default);

	texID = 0;
	glActiveTexture(GL_TEXTURE0 + type);
	glGenTextures(1, &texID);	
	glBindTexture(GL_TEXTURE_2D, texID);

	if (nrChannels == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	else if (nrChannels == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);

	char* pathInShader = new char[20];
	sprintf(pathInShader, "material.texture%d", type);
	GLuint mtx = glGetUniformLocation(shaderProgram, pathInShader);
	glUniform1i(mtx, type);
}

void Model::load_texture(const char names[][50])
{
	unsigned char* image;
	int width, height, channels;
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
	for (int i = 0; i < 6; ++i)
	{
		image = stbi_load(names[i], &width, &height, &channels, STBI_default);
		if (image)
		{
			if (channels == 3) // RGB
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			else if (channels == 4) // RGBA
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			//glGenerateMipmap(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
			
			stbi_image_free(image);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}



void Model::load_texCoords(vec2* texCoords, size_t count)
{
	tex_vbo = 0;
	glGenBuffers(1, &tex_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, tex_vbo);
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::vec2), texCoords, GL_STATIC_DRAW);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, tex_vbo);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(2);
}

void Model::set_index_range(size_t first_byteOffset, size_t count)
{
	firstIndexOffset = first_byteOffset;
	indicesCount = count;
}
