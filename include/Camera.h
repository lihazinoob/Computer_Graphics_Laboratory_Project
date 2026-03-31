#pragma once

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>


enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

enum Camera_ViewMode {
	GROUND_VIEW,
	TOP_DOWN_VIEW
};


class Camera {
public:
	glm::vec3 cameraPositon;
	float movementSpeed;
	Camera_ViewMode viewMode;

	Camera(glm::vec3 startPosition = glm::vec3(0.0f, 0.0f, 0.0f));

	// Returns the view matrix using glm::lookAt
	glm::mat4 GetViewMatrix();

	// Processes keyboard input to move the camera strictly along world axes
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);

	void SetViewMode(Camera_ViewMode mode);
};
