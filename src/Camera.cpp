#include "../include/Camera.h"

Camera::Camera(glm::vec3 startPosition) {
	cameraPositon = startPosition;
	movementSpeed = 2.0f;
}

glm::mat4 Camera::GetViewMatrix() {
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

	// Matches your original WASD logic exactly
	if (direction == FORWARD)  cameraPositon.z -= velocity;
	if (direction == BACKWARD) cameraPositon.z += velocity;
	if (direction == LEFT)     cameraPositon.x -= velocity;
	if (direction == RIGHT)    cameraPositon.x += velocity;
}