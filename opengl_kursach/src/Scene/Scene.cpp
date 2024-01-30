#include "Scene.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "../tiny_obj_loader.h"

Scene::Scene(GLFWwindow* win)
{
	window = win;

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	eyePoint = vec3(0, 1.f, 3.f);
	LookAt(vec3(0, 0, -1));

	RecalculateProjMatrix();
}

void Scene::RecalculateProjMatrix()
{
	SetProjection(glm::perspective(
		glm::radians(90.0f), // угол обзора в радианах обычно берЄтс€ в диапазоне 30 - 120 градусов
		(float)WinWidth / (float)WinHeight, // соотношение сторон экрана
		0.01f, // рассто€ние от наблюдател€ до ближней плоскости отсечени€
		500.0f //рассто€ние от наблюдател€ до дальней плоскости отсечени€
	));
}

void Scene::LoadSkyBox(const char filenames[][50])
{
	skybox = new Skybox(window, GL_TRIANGLES);
	skybox->load_shaders("src/Shaders/vertSkybox.glsl", "src/Shaders/fragSkybox.glsl");
	skybox->load_texture(filenames); 

	glm::vec3 skybox_verticies[] = {
		{-1.0f,  1.0f, -1.0f},
		{-1.0f, -1.0f, -1.0f},
		{ 1.0f, -1.0f, -1.0f},
		{ 1.0f, -1.0f, -1.0f},
		{ 1.0f,  1.0f, -1.0f},
		{-1.0f,  1.0f, -1.0f},

		{-1.0f, -1.0f,  1.0f},
		{-1.0f, -1.0f, -1.0f},
		{-1.0f,  1.0f, -1.0f},
		{-1.0f,  1.0f, -1.0f},
		{-1.0f,  1.0f,  1.0f},
		{-1.0f, -1.0f,  1.0f},

		{ 1.0f, -1.0f, -1.0f},
		{ 1.0f, -1.0f,  1.0f},
		{ 1.0f,  1.0f,  1.0f},
		{ 1.0f,  1.0f,  1.0f},
		{ 1.0f,  1.0f, -1.0f},
		{ 1.0f, -1.0f, -1.0f},

		{-1.0f, -1.0f,  1.0f},
		{-1.0f,  1.0f,  1.0f},
		{ 1.0f,  1.0f,  1.0f},
		{ 1.0f,  1.0f,  1.0f},
		{ 1.0f, -1.0f,  1.0f},
		{-1.0f, -1.0f,  1.0f},

		{-1.0f,  1.0f, -1.0f},
		{ 1.0f,  1.0f, -1.0f},
		{ 1.0f,  1.0f,  1.0f},
		{ 1.0f,  1.0f,  1.0f},
		{-1.0f,  1.0f,  1.0f},
		{-1.0f,  1.0f, -1.0f},

		{-1.0f, -1.0f, -1.0f},
		{-1.0f, -1.0f,  1.0f},
		{ 1.0f, -1.0f, -1.0f},
		{ 1.0f, -1.0f, -1.0f},
		{-1.0f, -1.0f,  1.0f},
		{ 1.0f, -1.0f,  1.0f}
	};
	skybox->load_coords(skybox_verticies, sizeof(skybox_verticies) / sizeof(glm::vec3));
}

void Scene::LookAt(vec3 point)
{
	centerOfView = point;
	cameraMat = lookAt(eyePoint, point, vectorUp);
}

void Scene::LookAtDir(vec3 dir)
{
	centerOfView = dir - eyePoint;
	LookAt(centerOfView);
}


void Scene::SetCameraRotationSpeed(float speed)
{
	cameraRotateSpeed = speed;
}

void Scene::MoveViewTo(vec3 vecToAdd)
{
	centerOfView += vecToAdd * cameraRotateSpeed;
	LookAt(centerOfView);
}

void Scene::MoveCameraTo(vec3 inputVec)
{
	vec3 dir = glm::normalize(centerOfView - eyePoint);
	
	vec3 moveVec = vec3();
	if (inputVec.x != 0)
	{
		moveVec.x = dir.z * -inputVec.x;
		moveVec.z = dir.x * inputVec.x;
	}
	else if (inputVec.z != 0)
	{
		moveVec = dir * -inputVec.z;
	}
	else if (inputVec.y != 0)
	{
		if (inputVec.y > 0)
			moveVec = vec3(0, 1.f, 0);
		if (inputVec.y < 0)
			moveVec = vec3(0, -1.f, 0);
	}

	centerOfView += moveVec * cameraMoveSpeed;
	eyePoint += moveVec * cameraMoveSpeed;
}

void Scene::SetProjection(mat4 proj)
{
	projectionMat = proj;
}
//void Scene::MoveCar(vec3 dir) {
//	if(car)
//		car->modelMatrix = glm::translate(car->modelMatrix, dir);
//}

void Scene::UpdateAnimations(double timeLeft)
{
	if (!doAnimation)
		return;
	
	for (Model& model : models)
	{
		
		if (model.animation == nullptr)
			continue;
		if (model.animation->vecToMove != vec3(0.f))
			model.modelMatrix = translate(model.modelMatrix, model.animation->vecToMove);
		if (model.animation->vecToRotate != vec3(0.f))
		{
			//float r = 10;
				if (model.animation_step == 0)
				{
					model.modelMatrix = glm::translate(model.modelMatrix, vec3(0, 23.53f, -21.05f));
					model.modelMatrix = glm::rotate(model.modelMatrix, glm::radians(model.animation->angle), model.animation->vecToRotate);
					//model.modelMatrix = glm::translate(model.modelMatrix, vec3(0, r * sin(glm::radians((180 - model.animation->angle) / 2)), r * cos(glm::radians(180 - model.animation->angle) / 2)));
				}
				else if (model.animation_step == 1) {
					model.modelMatrix = glm::translate(model.modelMatrix, vec3(0, -9.85f, 30.f)); 
					model.modelMatrix = glm::rotate(model.modelMatrix, glm::radians(-model.animation->angle), model.animation->vecToRotate);
					//model.modelMatrix = glm::translate(model.modelMatrix, -vec3(0, r * sin(glm::radians((180 - model.animation->angle) / 2)), r * cos(glm::radians(180 - model.animation->angle) / 2)));
				}
				else if (model.animation_step == 2) {
					model.modelMatrix = glm::translate(model.modelMatrix, vec3(0, -9.85f, 30.f)); 
					model.modelMatrix = glm::rotate(model.modelMatrix, glm::radians(-model.animation->angle), model.animation->vecToRotate);
					//model.modelMatrix = glm::translate(model.modelMatrix, -vec3(0, r * sin(glm::radians((180 - model.animation->angle) / 2)), r * cos(glm::radians(180 - model.animation->angle) / 2)));
				}
				else if (model.animation_step == 3) {
					model.modelMatrix = glm::translate(model.modelMatrix, vec3(0, 23.53f, -21.05f)); 
					model.modelMatrix = glm::rotate(model.modelMatrix, glm::radians(model.animation->angle), model.animation->vecToRotate);
					//model.modelMatrix = glm::translate(model.modelMatrix, vec3(0, r * sin(glm::radians((180 - model.animation->angle) / 2)), r * cos(glm::radians(180 - model.animation->angle) / 2)));
				}
				if (++model.animation_step > 3)
					model.animation_step = 0;
			
		}
			
		if (model.animation->vecToScale != vec3(1.f))
			model.modelMatrix = scale(model.modelMatrix, model.animation->vecToScale);
		model.animation->duration -= timeLeft;
		if (model.animation->duration <= 0.)
			model.animation = nullptr;
	}
	for (Model& model : kacheli)
	{

		if (model.animation == nullptr)
			continue;
		if (model.animation->vecToMove != vec3(0.f))
			model.modelMatrix = translate(model.modelMatrix, model.animation->vecToMove);
		if (model.animation->vecToRotate != vec3(0.f))
		{
			//float r = 10;
			if (model.animation_step == 0)
			{
				model.modelMatrix = glm::translate(model.modelMatrix, vec3(0, 23.53f, -21.05f));
				model.modelMatrix = glm::rotate(model.modelMatrix, glm::radians(model.animation->angle), model.animation->vecToRotate);
				//model.modelMatrix = glm::translate(model.modelMatrix, vec3(0, r * sin(glm::radians((180 - model.animation->angle) / 2)), r * cos(glm::radians(180 - model.animation->angle) / 2)));
			}
			else if (model.animation_step == 1) {
				model.modelMatrix = glm::translate(model.modelMatrix, vec3(0, -9.85f, 30.f));
				model.modelMatrix = glm::rotate(model.modelMatrix, glm::radians(-model.animation->angle), model.animation->vecToRotate);
				//model.modelMatrix = glm::translate(model.modelMatrix, -vec3(0, r * sin(glm::radians((180 - model.animation->angle) / 2)), r * cos(glm::radians(180 - model.animation->angle) / 2)));
			}
			else if (model.animation_step == 2) {
				model.modelMatrix = glm::translate(model.modelMatrix, vec3(0, -9.85f, 30.f));
				model.modelMatrix = glm::rotate(model.modelMatrix, glm::radians(-model.animation->angle), model.animation->vecToRotate);
				//model.modelMatrix = glm::translate(model.modelMatrix, -vec3(0, r * sin(glm::radians((180 - model.animation->angle) / 2)), r * cos(glm::radians(180 - model.animation->angle) / 2)));
			}
			else if (model.animation_step == 3) {
				model.modelMatrix = glm::translate(model.modelMatrix, vec3(0, 23.53f, -21.05f));
				model.modelMatrix = glm::rotate(model.modelMatrix, glm::radians(model.animation->angle), model.animation->vecToRotate);
				//model.modelMatrix = glm::translate(model.modelMatrix, vec3(0, r * sin(glm::radians((180 - model.animation->angle) / 2)), r * cos(glm::radians(180 - model.animation->angle) / 2)));
			}
			if (++model.animation_step > 3)
				model.animation_step = 0;

		}

		if (model.animation->vecToScale != vec3(1.f))
			model.modelMatrix = scale(model.modelMatrix, model.animation->vecToScale);
		model.animation->duration -= timeLeft;
		if (model.animation->duration <= 0.)
			model.animation = nullptr;
	}
}

void Scene::Run()
{
	glClearColor(.5f, .5f, .5f, .0f);

	double limitFPS = 36 / 165.0;

	double lastTime = glfwGetTime(), timer = lastTime;
	double deltaTime = 0, nowTime = 0;
	int frames = 0, updates = 0;

	clock_t now = clock(); 
	clock_t last_render_time = 0; 
	clock_t elapsed_time = 0; 
	float fps = 30;

	while (!glfwWindowShouldClose(window))
	{
		nowTime = glfwGetTime();
		deltaTime += (nowTime - lastTime) / limitFPS;
		lastTime = nowTime;

		while (deltaTime >= 1.0)
		{
			UpdateAnimations(deltaTime * limitFPS);
			updates++;
			deltaTime--;
		}

		glViewport(0, 0, WinWidth, WinHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		LookAt(centerOfView);

		if (skybox)
		{
			skybox->render(projectionMat, cameraMat, eyePoint, lights);
		}
		

		elapsed_time = now - last_render_time;
		if (elapsed_time > 1000 / fps) {
			RenderModels(true);
			last_render_time = clock();
		}
		else {
			RenderModels(false);
		}
		now = clock();
		frames++;

		if (glfwGetTime() - timer > 1.0)
		{
			timer++;
			updates = 0, frames = 0;
		}

		glfwPollEvents();
		glfwSwapBuffers(window);
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(window, 1);
		}
	}
}

void Scene::AddModel(Model& model)
{
	if (model.name == "kacheli")
	{
		kacheli.emplace_back(model);
		return;
	}
	models.emplace_back(model); 
	if (model.name == "car")
		car = &models.back();
}

void Scene::RenderModels(bool flag = false)
{
	for (Model& model : models)
		model.render(projectionMat, cameraMat, eyePoint, lights);
	
	if (kacheli.size() > 0) {
		kacheli[k_ind].render(projectionMat, cameraMat, eyePoint, lights);
		if (flag)
			k_ind++;
		if (k_ind >= kacheli.size())
			k_ind = 0;
	}
}


inline void hash_combine(std::size_t& seed) { }
template <typename T, typename... Rest>
inline void hash_combine(std::size_t& seed, const T& v, Rest... rest) {
	std::hash<T> hasher;
	seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	hash_combine(seed, rest...);
}

void Scene::loadOBJtoScene(const char* filename, const char* mtl_directory, const char* texture_directory, Animation* anim, glm::vec3 scaleVec)
{
	Model model(window, GL_TRIANGLES);

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	if (filename == "resources/Models_E0502A095/BMW_M3_GTR.obj")
		model.name = "car";
	if (filename[10] == 'k' && filename[11] == 'a' && filename[12] == 'c' && filename[13] == 'h' && filename[14] == 'e' && filename[15] == 'l' && filename[16] == 'i') {
		k_count++;
		model.name = "kacheli";
	}
	
	ofstream fout(filename, ios::app);
	if (fout.is_open()) {
		fout << "#eof";
		fout.close();
	}

	std::string err;
	std::string warn;
	// ≈сли в процессе загрузки возникли ошибки - выдадим исключение
	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, &warn, filename, mtl_directory))
		throw std::runtime_error(err);
	if (warn != "")
		cout << warn << endl;

	std::vector<GLuint>    indices;  // индексы модели
	std::vector<glm::vec3> verteces; // вершины
	std::vector<glm::vec3> normals; // нормали
	std::vector<glm::vec2> texCords; // текстурные координаты
	size_t hash; // ƒл€ уникальных вершин
	std::map <int, int> uniqueVerteces; // словарь дл€ уникальных вершин: ключ - хеш, значение - индекс вершины

	int last_material_index = 0; // индекс последнего материала (дл€ группировки моделей)
	int count = 0, offset; // дл€ индексов начала и конца в индексном буфере
	std::vector<int> materials_range; // хранилище индексов
	std::vector<int> materials_ids; // индексы материалов

	materials_range.push_back(count); // «акидываем начало отрезка в индексном буфере
	// ÷икл по считанным модел€м
	for (const auto& shape : shapes)
	{
		offset = count;  // ѕеременна€ дл€ 
		last_material_index = shape.mesh.material_ids[(count - offset) / 3]; // «апоминаем индекс материала

		// ÷икл по индексам модели
		for (const auto& index : shape.mesh.indices)
		{
			hash = 0;
			hash_combine(hash
				, attrib.vertices[3 * index.vertex_index + 0], attrib.vertices[3 * index.vertex_index + 1], attrib.vertices[3 * index.vertex_index + 2]
				, attrib.normals[3 * index.normal_index + 0], attrib.normals[3 * index.normal_index + 1], attrib.normals[3 * index.normal_index + 2]
				, attrib.texcoords[2 * index.texcoord_index + 0], attrib.texcoords[2 * index.texcoord_index + 1]);

			if (!uniqueVerteces.count(hash))
			{
				uniqueVerteces[hash] = verteces.size();

				// группируем вершины в массив на основании индексов
				verteces.push_back({ attrib.vertices[3 * index.vertex_index + 0]
												, attrib.vertices[3 * index.vertex_index + 1]
												, attrib.vertices[3 * index.vertex_index + 2]
					});
				// группируем нормали в массив на основании индексов
				normals.push_back({ attrib.normals[3 * index.normal_index + 0]
												, attrib.normals[3 * index.normal_index + 1]
												, attrib.normals[3 * index.normal_index + 2]
					});
				// группируем текстурные координаты в массив на основании индексов
				texCords.push_back({ attrib.texcoords[2 * index.texcoord_index + 0]
												, 1 - attrib.texcoords[2 * index.texcoord_index + 1]
					});
			}
			// —охран€ем индекс в массив
			indices.push_back(uniqueVerteces[hash]);

			// ≈сли индекс последнего материала изменилс€, то необходимо сохранить его
			if (last_material_index != shape.mesh.material_ids[(count - offset) / 3])
			{
				materials_range.push_back(count); // как конец отрезка
				materials_ids.push_back(last_material_index); // как используемый материал
				last_material_index = shape.mesh.material_ids[(count - offset) / 3];
			}
			count++;
		} // for (const auto& index : shape.mesh.indices) 

		// ≈сли последний материал не загружен - загружаем его
		if (materials_range[materials_range.size() - 1] != count - 1)
		{
			materials_range.push_back(count); // последний конец отрезка
			materials_ids.push_back(last_material_index); // последний используемый материал
		}
	}

	model.load_shaders("src/Shaders/vertTex.glsl", "src/Shaders/fragTex.glsl");
	model.load_modelMatrix(scale(WorldMatrix, scaleVec));
	// «агрузка в буферы
	model.load_coords(&verteces[0], verteces.size());
	model.load_normals(&normals[0], normals.size());
	model.load_texCoords(&texCords[0], texCords.size());
	
	// «агрузка индексного буфера
	model.load_indices(&indices[0], indices.size());

	model.animation = anim;

	for (int i = 0; i < materials_range.size() - 1; i++)
	{
		if (model.name == "kacheli") {
			auto copy = model;
			string path = "resources/kacheli/kach.png";
			//string path = "resources/kacheli/kacheli" + to_string(k_count) + ".mtl";
			copy.load_texture(path.c_str(), i);
			copy.set_index_range(0, 3900);
			AddModel(copy);
			break;
		}
		auto copy = model;
		if (texture_directory != "") {
			std::string path = texture_directory + materials[materials_ids[i]].diffuse_texname;
			copy.load_texture(path.c_str(), i);
		}
		copy.set_index_range(materials_range[i] * sizeof(GLuint), materials_range[i + 1] - materials_range[i]);
		AddModel(copy);
	}
}