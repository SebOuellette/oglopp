#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glad/gl.h>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define WORLD_UP glm::vec3(0.0f, 1.0f, 0.0f)

class Camera {
private:
	// Positions
	glm::vec3 _pos;
	glm::vec3 _target;

	// Directions
	glm::vec3 _backward; // The tutorial says this is the backward direction
	glm::vec3 _right;

	// Camera stuff
	glm::mat4 _view;

	Camera& _updateRight();

public:
	Camera(glm::vec3 pos);

	// Positions
	glm::vec3 getPos();
	glm::vec3 getTarget();
	// Native directions
	glm::vec3 getBack();
	glm::vec3 getRight();
	glm::vec3 getUp();

	// Alternative for all the crap above?
	Camera& lookAt(glm::vec3 target);

	Camera& setPos(glm::vec3 newPos = glm::vec3(0.0f, 0.0f, 3.0f));
	Camera& setTarget(glm::vec3 newPos = glm::vec3(0.0f, 0.0f, 0.0f));
};

#endif
