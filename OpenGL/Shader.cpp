#include "Shader.h"
#include "Material.h"

string Shader::ReadShaderSource(const char* filePath) {
	string content;
	ifstream fileStream(filePath, ios::in);
	string line = "";
	while (!fileStream.eof()) {
		getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();
	return content;
}

void Shader::PrintShaderLog(GLuint shader) {
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		log = (char*)malloc(len);
		glGetShaderInfoLog(shader, len, &chWrittn, log);
		cout << "Shader Info Log: " << log << endl;
		free(log);
	}
}

void Shader::PrintProgramLog(int prog) {
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		log = (char*)malloc(len);
		glGetProgramInfoLog(prog, len, &chWrittn, log);
		cout << "Program Info Log: " << log << endl;
		free(log);
	}
}


GLuint shaderProgram;

void Shader::Compile(const char* vertexSourcePath, const char* fragmentSourcePath) {
	//progress flags
	GLint vertCompiled;
	GLint fragCompiled;
	GLint linked;
	//shader sources
	string vertShaderStr = ReadShaderSource(vertexSourcePath);
	string fragShaderStr = ReadShaderSource(fragmentSourcePath);
	const char* vertShaderSrc = vertShaderStr.c_str();
	const char* fragShaderSrc = fragShaderStr.c_str();
	//creating shaders
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	//compilating shaders
	glShaderSource(vShader, 1, &vertShaderSrc, NULL);
	glShaderSource(fShader, 1, &fragShaderSrc, NULL);
	glCompileShader(vShader);
	CheckOpenGLError();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
	if (vertCompiled != 1) {
		cout << "vertex compilation failed" << endl;
		PrintShaderLog(vShader);
	}
	else
		cout << "vertex compilation succeeded" << endl;

	glCompileShader(fShader);
	CheckOpenGLError();
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
	if (fragCompiled != 1) {
		cout << "fragment compilation failed" << endl;
		PrintShaderLog(fShader);
	}
	else
		cout << "fragment compilation succeeded" << endl;
	//attaching shaders to a shader program
	GLuint vfProgram = glCreateProgram();
	glAttachShader(vfProgram, vShader);
	glAttachShader(vfProgram, fShader);
	glLinkProgram(vfProgram);
	CheckOpenGLError();
	glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
	if (linked != 1) {
		cout << "linking failed" << endl;
		PrintProgramLog(vfProgram);
	}
	else
		cout << "linking succeeded" << endl;
	shaderProgram = vfProgram;
}

void Shader::Use() {
	glUseProgram(shaderProgram);
}

void Shader::SetInt(const char* uniformName, int value) {
	GLuint loc = glGetUniformLocation(shaderProgram, uniformName);
	glUniform1i(loc, value);
}

void Shader::SetFloat(const char* uniformName, float value) {
	GLuint loc = glGetUniformLocation(shaderProgram, uniformName);
	glUniform1f(loc, value);
}

void Shader::SetFloat3(const char* uniformName, glm::vec3 value) {
	GLuint loc = glGetUniformLocation(shaderProgram, uniformName);
	glUniform3fv(loc, 1, glm::value_ptr(value));
}

void Shader::SetMatrix4f(const char* uniformName, glm::mat4 value) {
	GLuint loc = glGetUniformLocation(shaderProgram, uniformName);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetMatrix3f(const char* uniformName, glm::mat3 value) {
	GLuint loc = glGetUniformLocation(shaderProgram, uniformName);
	glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetMaterial(Material value) {
	SetFloat("material.shininess", value.shininess);
}