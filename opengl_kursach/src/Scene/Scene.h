#pragma once

#include "../func.h"
#include "../Globals.h"
#include "../Model/Model.h"
#include "../Model/Skybox.h"

class Scene
{
public:
	Scene(GLFWwindow* win);

	void Run();

	void AddModel(Model& model);

	void RenderModels(bool flag);

	void loadOBJtoScene(const char* filename, const char* mtl_directory, const char* texture_directory, Animation* anim = nullptr, glm::vec3 scaleVec = vec3(1.f));

	void RecalculateProjMatrix();

	void LoadSkyBox(const char filenames[][50]);

	void LookAt(vec3 point);
	void LookAtDir(vec3 dir);

	void GenerateFloor(float diameter);

	void SetCameraRotationSpeed(float speed);

	// Based on camera rotation speed variable
	void MoveViewTo(vec3 vecToAdd);

	void SetProjection(mat4 proj);

	void MoveCameraTo(vec3 vecToAdd);

	void UpdateAnimations(double timeLeft);

	void MoveCar(vec3 dir);

	LightsInfo lights;

	bool doAnimation = true;

private:
	vec3 eyePoint;
	vec3 centerOfView;
	vec3 deltaView = vec3(0.f, 0.f, 0.f);
	mat4 cameraMat;
	float cameraRotateSpeed = 0.3f;

	float cameraMoveSpeed = 0.1f;

	mat4 projectionMat;

	const vec3 vectorUp = vec3(0.f, 1.f, 0.f);

	GLFWwindow* window;

	Skybox* skybox;

	vector<Model> models;

	vector<Model> kacheli;

	int k_ind = 0, k_count = 0;

	GLuint skyboxHandler;

	Model* car;
};

