#pragma once

#include "stb_image.h"
#if WIN32
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#else
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#endif
#include <spdlog/spdlog.h>

struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 speculer;
	float shininess;
};

struct Light {
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 speculer;

	float constant;
	float linear;
	float quadratic;
};
