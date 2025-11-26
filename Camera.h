#pragma once
#include "header.h"
class Camera
{
public:
	float yaw = -90.0f, pitch = 0.0f;
	float fov = 95.0f;
	float cameraspeed = 0.0f;
	glm::vec3 camerapos;
	glm::vec3 camerafront;
	glm::vec3 cameraup;
	glm::vec3 cameradirection;
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	Camera() : camerapos(glm::vec3(0.0f, 1.0f, 0.0f)), camerafront(glm::vec3(0.0f, 0.0f, -1.0f)), cameraup(glm::vec3(0.0f, 1.0f, 0.0f)), cameradirection(glm::vec3(1.0f, 0.0f, 1.0f)) {}

	void moveCamera(GLFWwindow* window, double& deltatime);

	void setCameraView();
};

