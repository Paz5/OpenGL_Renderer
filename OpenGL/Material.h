#pragma once
#include <GL/glew.h>

struct Material {
	GLuint diffuseTex;
	GLuint specularTex;
	float shininess;
};