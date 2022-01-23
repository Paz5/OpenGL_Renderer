#pragma once
#include <iostream>
#include <ostream>
#include <string>
#include <GL/glew.h>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Utils.h"
#include "Material.h"

using namespace std;

class Shader
{
private:
	GLuint shaderProgram;
	string ReadShaderSource(const char*);
	void PrintShaderLog(GLuint shader);
	void PrintProgramLog(int prog);
public:
	void Compile(const char* vertexSourcePath, const char* fragmentSourcePath);
	void Use();

	void SetInt(const char* uniformName, int value);
	void SetFloat(const char* uniformName, float value);
	void SetFloat3(const char* uniformName, glm::vec3 value);
	void SetMatrix4f(const char* uniformName, glm::mat4 value);
	void SetMatrix3f(const char* uniformName, glm::mat3 value);
	void SetMaterial(Material value);
};

