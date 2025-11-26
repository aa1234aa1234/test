#include "Camera.h"

void Camera::moveCamera(GLFWwindow* window, double& deltatime) {
	cameraspeed = 2.0f * deltatime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camerapos += cameraspeed * cameradirection;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camerapos -= cameraspeed * cameradirection;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camerapos -= glm::normalize(glm::cross(camerafront, cameraup)) * cameraspeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camerapos += glm::normalize(glm::cross(camerafront, cameraup)) * cameraspeed;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camerapos += cameraspeed * glm::vec3(0.0f, 1.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) camerapos += cameraspeed * glm::vec3(0.0f, -1.0f, 0.0f);
}

void Camera::setCameraView() {
	view = glm::lookAt(camerapos, camerapos + camerafront, cameraup);
}
