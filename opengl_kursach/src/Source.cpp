#include "func.h"
#include "Scene/Scene.h"
#include "Globals.h"

int WinWidth;
int WinHeight;
Scene* scene;
const glm::mat4 WorldMatrix = glm::mat4(1.0f);
bool rotationmode;

#pragma region MouseVars
bool firstMouse = true;
float horizontalAngle = -90;
float verticalAngle = 0;
float lastX = WinWidth / 2.f;
float lastY = WinHeight / 2.f;
float camPitch = 0.f;
float camYaw = -90.0f;
#pragma endregion

#pragma region Input Functions
void OnWindowResize(GLFWwindow* window, int width, int height)
{
	WinWidth = width;
	WinHeight = height;

	scene->RecalculateProjMatrix();
}

void MouseCallback(GLFWwindow* window, double xposIn, double yposIn)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		firstMouse = true;
		return;
	}	

	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		//pitch = 0.f;
		//yaw = -90.f;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.01f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	camYaw += xoffset;
	camPitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (camPitch > 89.0f)
		camPitch = 89.0f;
	if (camPitch < -89.0f)
		camPitch = -89.0f;

	/*glm::vec3 front;
	front.x = cos(glm::radians(camYaw)) * cos(glm::radians(camPitch));
	front.y = sin(glm::radians(camPitch));
	front.z = sin(glm::radians(camYaw)) * cos(glm::radians(camPitch));
	scene->LookAtDir(glm::normalize(front));*/

	horizontalAngle += sensitivity * float(WinWidth / 2 - xpos);
	verticalAngle += sensitivity * float(WinHeight / 2 - ypos);

	glm::vec3 direction(
		cos(glm::radians(verticalAngle)) * sin(glm::radians(horizontalAngle)),
		sin(glm::radians(verticalAngle)),
		cos(glm::radians(verticalAngle)) * cos(glm::radians(horizontalAngle))
	);
	scene->LookAtDir(glm::normalize(direction)); 

	glm::vec3 right = glm::vec3( 
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	); 

	glm::vec3 up = glm::cross(right, direction); 
	//glfwSetCursorPos(window, WinWidth / 2, WinHeight / 2);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		camYaw += 10;
		glm::vec3 front;
		front.x = cos(glm::radians(camYaw));
		front.y = sin(glm::radians(camPitch));
		front.z = sin(glm::radians(camYaw));
		scene->LookAtDir(glm::normalize(front));
	}
	if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		camYaw -= 10;
		glm::vec3 front;
		front.x = cos(glm::radians(camYaw));
		front.y = sin(glm::radians(camPitch));
		front.z = sin(glm::radians(camYaw));
		scene->LookAtDir(glm::normalize(front));
	}
	if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		camPitch += 10;
		glm::vec3 front;
		front.x = cos(glm::radians(camYaw)) * cos(glm::radians(camPitch));
		front.y = sin(glm::radians(camPitch));
		front.z = sin(glm::radians(camYaw)) * cos(glm::radians(camPitch));
		scene->LookAtDir(glm::normalize(front));
	}
	if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		camPitch -= 10;
		glm::vec3 front;
		front.x = cos(glm::radians(camYaw)) * cos(glm::radians(camPitch));
		front.y = sin(glm::radians(camPitch));
		front.z = sin(glm::radians(camYaw)) * cos(glm::radians(camPitch));
		scene->LookAtDir(glm::normalize(front));
	}
	if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		scene->MoveCameraTo(vec3(0, 0, -1.f));
		glm::vec3 front;
		front.x = cos(glm::radians(camYaw)) * cos(glm::radians(camPitch));
		front.y = sin(glm::radians(camPitch));
		front.z = sin(glm::radians(camYaw)) * cos(glm::radians(camPitch));
		scene->LookAtDir(glm::normalize(front));
	}
	if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		scene->MoveCameraTo(vec3(0, 0, 1.f));
		glm::vec3 front;
		front.x = cos(glm::radians(camYaw)) * cos(glm::radians(camPitch));
		front.y = sin(glm::radians(camPitch));
		front.z = sin(glm::radians(camYaw)) * cos(glm::radians(camPitch));
		scene->LookAtDir(glm::normalize(front));
	}
	if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		scene->MoveCameraTo(vec3(-1.f, 0, 0));
		glm::vec3 front;
		front.x = cos(glm::radians(camYaw));
		front.y = sin(glm::radians(camPitch));
		front.z = sin(glm::radians(camYaw));
		scene->LookAtDir(glm::normalize(front));
	}
	if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		scene->MoveCameraTo(vec3(1.f, 0, 0));
		glm::vec3 front;
		front.x = cos(glm::radians(camYaw));
		front.y = sin(glm::radians(camPitch));
		front.z = sin(glm::radians(camYaw));
		scene->LookAtDir(glm::normalize(front));
	}
	if (key == GLFW_KEY_SPACE && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		scene->MoveCameraTo(vec3(0, 1.f, 0));
	}
	if (key == GLFW_KEY_LEFT_CONTROL && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		scene->MoveCameraTo(vec3(0, -1.f, 0));
	}
	if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
	{
		scene->doAnimation = !scene->doAnimation;
	}
	/*if (key == GLFW_KEY_I && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		scene->MoveCar(vec3(0, 1.f, 1000.f));
	}
	if (key == GLFW_KEY_K && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		scene->MoveCar(vec3(0, -1.f, -1000.f));
	}
	if (key == GLFW_KEY_J && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		scene->MoveCar(vec3(1000.f, 1.f, 0));
	}
	if (key == GLFW_KEY_L && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		scene->MoveCar(vec3(-1000.f, -1.f, 0));
	}*/
}
#pragma endregion

int main()
{
	GLFWwindow* window = InitAll(1080, 1080, false);
	if (window == nullptr)
	{
		EndAll();
		return 1;
	}

	scene = new Scene(window);
	
	glfwSetWindowSizeCallback(window, OnWindowResize);
	glfwSetKeyCallback(window, KeyCallback);
	//glfwSetCursorPosCallback(window, MouseCallback);
	glLineWidth(2.f);

	Animation* an0 = new Animation;
	an0->duration = 0.f;
	an0->vecToMove = vec3(150.f, 0.f, 50.f);

	Animation* an1 = new Animation;
	an1->duration = 0.f;
	an1->vecToMove = vec3(75.f, 12.f, 25.f); 

	Animation* an2 = new Animation;
	an2->duration = 0.f;
	an2->vecToMove = vec3(-100.f, 0.f, -10.f);

	Animation* an3 = new Animation;
	an3->duration = 0.f;
	an3->vecToMove = vec3(0.f, -100.f, -7500.f);

	Animation* an4 = new Animation;
	an4->duration = 0.f;
	an4->vecToMove = vec3(0.f, -20.f, -50.f);

	Animation* an5 = new Animation;
	an5->duration = 0.f;
	an5->vecToMove = vec3(15.f, 2.f, -15.f);

	Animation* an6 = new Animation;
	an6->duration = 999.f;
	an6->angle = 30.f;
	an6->vecToRotate = vec3(1.f, 0, 0);
	//an6->vecToMove  = vec3(0, -9.5f, 30.f);

	Animation* an = new Animation;
	an->duration = 0.f;
	an->vecToMove = vec3(-34.f, 0.f, 0.f);

	Animation* an7 = new Animation;
	an7->duration = 0.f;
	an7->vecToMove = vec3(-32.f, 0.f, 0.f);
	
	/*char skybox[][50] =
	{
		"resources/skybox/px.jpg",
		"resources/skybox/nx.jpg",
		"resources/skybox/py.jpg",
		"resources/skybox/ny.jpg",
		"resources/skybox/pz.jpg",
		"resources/skybox/nz.jpg"
	};*/
	char skybox[][50] =
	{
		"resources/skybox/skycube_1/skyrender0004.bmp",
		"resources/skybox/skycube_1/skyrender0002.bmp",
		"resources/skybox/skycube_1/skyrender0003.bmp",
		"resources/skybox/skycube_1/skyrender0006.bmp",
		"resources/skybox/skycube_1/skyrender0001.bmp",
		"resources/skybox/skycube_1/skyrender0005.bmp"
	};
	
	scene->LoadSkyBox(skybox);

	scene->loadOBJtoScene("resources/box/cubes.obj", "resources/box/", "resources/box/", an5, vec3(0.1f, 0.1f, 0.1f));

	scene->loadOBJtoScene("resources/Models_E0502A095/BMW_M3_GTR.obj", "resources/Models_E0502A095/", "resources/Models_E0502A095/", an3, vec3(0.0001f, 0.0001f, 0.0001f));

	scene->loadOBJtoScene("resources/bochka/boch.obj", "resources/bochka/", "resources/bochka/", an0, vec3(0.01f, 0.01f, 0.01f));

	scene->loadOBJtoScene("resources/kach_opora/kach_opora.obj", "resources/kach_opora/", "resources/kach_opora/", 0, vec3(0.01f, 0.01f, 0.01f));

	scene->loadOBJtoScene("resources/kach_kach/kach_kach.obj", "resources/kach_kach/", "resources/kach_kach/", an7, vec3(0.01f, 0.01f, 0.01f));

	//scene->loadOBJtoScene("resources/kacheli/kacheli1.obj", "resources/kacheli/", "resources/kacheli/", an, vec3(0.01f, 0.01f, 0.01f));

	string kach_path;
	for (int i = 1; i <= 100; i++) {
		kach_path = "resources/kacheli/kacheli" + to_string(i) + ".obj";
		scene->loadOBJtoScene(kach_path.c_str(), "resources/kach_kach/", "resources/kach_kach/", an, vec3(0.01f, 0.01f, 0.01f));
	}

	//scene->loadOBJtoScene("resources/kach_kach/kach_kach.obj", "resources/kach_kach/", "resources/kach_kach/", an6, vec3(0.01f, 0.01f, 0.01f));

	scene->loadOBJtoScene("resources/zemlya/zemlya.obj", "resources/zemlya/", "resources/zemlya/", 0, vec3(0.01f, 0.01f, 0.01f));

	scene->loadOBJtoScene("resources/lampa/lamp.obj", "resources/lampa/", "resources/lampa/", an1, vec3(0.02f, 0.02f, 0.02f)); 

	scene->loadOBJtoScene("resources/scumeyka/scumeyka.obj", "resources/scumeyka/", "", an2, vec3(0.015f, 0.02f, 0.015f));
	

	PointLight lamp;
	lamp.linear = 0.14f;
	lamp.quadratic = 0.07f;
	lamp.position = vec3(0.f, 0.f, -1.5f);
	lamp.ambient = vec3(0.7f);
	lamp.diffuse = vec3(1.0f);
	lamp.specular = vec3(1.0f);
	scene->lights.pointLights.emplace_back(lamp);

	scene->Run();

	return 0;
}