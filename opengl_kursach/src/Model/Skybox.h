#pragma once
#include "Model.h"



class Skybox : public Model
{
public:
	Skybox(GLFWwindow* w, GLuint mode) : Model(w, mode) {}

	void virtual render(mat4 projection, mat4 model, mat4 view, GLuint mode, vec3 campos, LightsInfo lights) override;
	void virtual render(mat4 projection, mat4 view, vec3 campos, LightsInfo lights) override;

	//void load_texture(const char names[][50]);
};

