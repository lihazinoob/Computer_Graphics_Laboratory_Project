#include "../include/Camera.h"

Camera::Camera(glm::vec3 startPosition) {
	cameraPositon = startPosition;
	movementSpeed = 2.0f;
	viewMode = GROUND_VIEW;
}

glm::mat4 Camera::GetViewMatrix() {
	if (viewMode == TOP_DOWN_VIEW) {
		glm::vec3 front = glm::normalize(glm::vec3(0.0f, -0.5f, -1.0f));
		glm::vec3 target = cameraPositon + front;
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		return glm::lookAt(cameraPositon, target, up);
	}

	glm::vec3 front;
	front.x = 0.0f;
	front.y = -sin(glm::radians(10.0f));
	front.z = -cos(glm::radians(10.0f));

	glm::vec3 target = cameraPositon + front;
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	return glm::lookAt(cameraPositon, target, up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
	float velocity = movementSpeed * deltaTime;

	// World-axis movement keeps the controls predictable in both camera modes.
	if (direction == FORWARD)  cameraPositon.z -= velocity;
	if (direction == BACKWARD) cameraPositon.z += velocity;
	if (direction == LEFT)     cameraPositon.x -= velocity;
	if (direction == RIGHT)    cameraPositon.x += velocity;
	if (direction == UP)       cameraPositon.y += velocity;
	if (direction == DOWN)     cameraPositon.y -= velocity;
}

void Camera::SetViewMode(Camera_ViewMode mode) {
	viewMode = mode;

	if (viewMode == TOP_DOWN_VIEW) {
		movementSpeed = 8.0f;
	}
	else {
		movementSpeed = 2.0f;
	}
}
