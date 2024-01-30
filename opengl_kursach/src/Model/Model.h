#pragma once


#include "../func.h"
#include "../Globals.h"
#include "../Light/Lights.h"
#include "../Animation/Animation.h"
using namespace std;


struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;

	Material(vec3 amb, vec3 dif, vec3 spec, float shine)
	{
		ambient = amb; diffuse = dif; specular = spec; shininess = shine;
	}
};

//amb:	0.0215		0.1745		0.0215 
//dif:	0.07568		0.61424		0.07568 
//spec:	0.633		0.727811		0.633
//shine:	0.6
const Material emerald = Material(
	vec3(0.0215f, 0.1745f, 0.0215f),
	vec3(0.07568f, 0.61424f, 0.07568f),
	vec3(0.633f, 0.727811, 0.633f),
	0.6f);

class Model
{
public:
	/// <summary>
	/// Простой конструктор - создаёт массив вершин, пока пустой. И подключает окно.
	/// </summary>
	/// <param name="w"></param>
	Model(GLFWwindow* w, GLuint mode = GL_TRIANGLES) 
	{
		glGenVertexArrays(1, &vao);
		window = w;
		renderMode = mode;

		modelMatrix = glm::mat4(1.f);
	};

	//Деструктор. Так как динамической памяти нет, то он ничего не делает.
	~Model() {};

	Model& operator=(const Model& other); // Оператор присваивания

	//Метод для отображения модели.

	/// <summary>
	/// Рендер почти без параметров - просто отображает массив вершин.
	/// </summary>
	/// <param name="mode">Единственный параметр - режим рисования.</param>
	void virtual render(GLuint mode);
	void virtual render();
	void virtual render(mat4 projection, mat4 model, mat4 view, GLuint mode, vec3 campos, LightsInfo lights);
	void virtual render(mat4 projection, mat4 view, vec3 campos, LightsInfo lights);

	/// <summary>
	/// Метод для загрузки координат вершин.
	/// </summary>
	/// <param name="verteces">Массив с координатами.</param>
	/// <param name="count">Размер массива.</param>
	void load_coords(glm::vec3* verteces, size_t count);

	/// <summary>
	/// Метод для загрузки цветов вершин.
	/// </summary>
	/// <param name="colors">Массив цветов.</param>
	/// <param name="count">Размер массива.</param>
	void load_colors(glm::vec3* colors, size_t count);

	/// <summary>
	/// Метод для загрузки массива индексов.
	/// </summary>
	/// <param name="indices">Массив индексов.</param>
	/// <param name="count">Размер массива.</param>
	void load_indices(GLuint* indices, size_t count);

	// Загрузка матрицы с преобразованиями
	void load_modelMatrix(glm::mat4 mat);

	void load_material(Material mater);

	/// <summary>
	/// Метод для загрузки шейдеров. С целью примера загружает только вершинный и фрагментный шейдеры
	/// В результате выполнения будет скомпонована шейдерная программа.
	/// </summary>
	/// <param name="vect">Путь к вершинному шейдеру</param>
	/// <param name="frag">Путь к фрагментному шейдеру</param>
	void load_shaders(const char* vect, const char* frag);

	void load_normals(vec3* normals, size_t count);

	// Uses Tex shaders or newer
	void virtual load_texture(const char* path, int type = 0);
	void virtual load_texture(const char names[][50]);

	void load_texCoords(vec2* texCoords, size_t count);

	void set_index_range(size_t first_byteOffset, size_t count);

	Animation* animation = nullptr;

	int animation_step = 0;
	
	string name = "";

	glm::mat4 modelMatrix;

protected:
	// Throw the MVP matrix and over properties to the shader
	void SetUniformVars(mat4 projection, mat4 model, mat4 view, vec3 campos, LightsInfo lights);
	void SetUniformMaterial();
	void SetUniformLights(LightsInfo lights);
	void SetUniformVec3(const char* nameInShader, vec3 vec);
	void SetUniformFloat(const char* nameInShader, float val);

	// Режим рисования по-умолчанию для этой модели
	GLuint renderMode;

	Material material = emerald;

	/// <summary>
	/// ID массива вершин
	/// </summary>
	GLuint vao = -1;

	/// <summary>
	/// Количество вершин
	/// </summary>
	size_t vertecesCount = 0;
	GLuint coordsVbo = 0;


	/// <summary>
	/// Количество индексов
	/// </summary>
	size_t indicesCount = 0;

	size_t firstIndexOffset = 0;

	/// <summary>
	/// ID шейдерной программы
	/// </summary>
	GLuint shaderProgram = -1;

	size_t normalsCount = 0;

	GLuint tex_vbo = 0;

	GLuint texID = 0;

	/// <summary>
	/// Указатель на окно
	/// </summary>
	GLFWwindow * window;
};

