#pragma once
#include "../func.h"

struct DirLight
{
	vec3 direction;
	//��������� ��������� �����
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;
	//��������� ���������
	float constant = 1.f;
	float linear = 0.7f;
	float quadratic = 1.8f;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	//��������� ����
	float cutOff;
	float outerCutOff;
	//��������� ���������
	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct LightsInfo
{
	DirLight dirLight;
	vector<PointLight> pointLights;
	vector<SpotLight> spotLights;
	//int numPLights;
	//int numSLights;
};
