//window management and opengl function wrangler
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//standard
#include <string>
#include <iostream>
#include <fstream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stack>
//GL math
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
//project files
#include "Utils.h"
#include "primitives.h"
#include "Shader.h"
#include  "Camera.h"
//dear imgui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

using namespace std;
//using namespace glm;

#define numVAOs 1
#define numVBOs 1
glm::vec3 cubePos = glm::vec3(0.0f);
glm::vec3 sunDirection = glm::vec3(0.0f, -1.0f, 0.0f);
glm::vec3 sunColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 ambientColor = glm::vec3(0.03f, 0.02f, 0.09f);
GLuint vao[numVAOs];
GLuint vbo[numVBOs];
GLuint mainTex;

Shader defaultShader;
Material defaultMaterial{
	0,
	1,
	32.0f
};

Camera mainCamera;

// allocate variables used in display() function, so that they won’t need to be allocated during rendering
GLuint mLoc, vLoc, projLoc, sunPosLoc, sunColorLoc, nMatLoc;
int width, height;
float aspect;
glm::mat4 pMat, mMat, mvMat, tMat, rMat;
glm::mat3 nMat;
float timeFactor;

void setupVertices(void) { // 36 vertices, 12 triangles, makes 2x2x2 cube placed at origin
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);//send positions to shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));//send normals to shader
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));//send uvs to shader
	glEnableVertexAttribArray(2);
}

void WindowResizeCallback(GLFWwindow* window, int newWidth, int newHeight)
{
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);  // 1.0472 radians = 60 degrees
	glViewport(0, 0, width, height);
}

double mouseX = 0.0;
double mouseY = -0.7;
float radius = 10.0f;
float lastX = 0;
float lastY = 0;
float cursorSensitivity = 1.0f / 250.0f;
bool dragCamera = false;

void cursor_callback(GLFWwindow* window, double xposIn, double yposIn) {
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	float xDelta = xpos - lastX;
	float yDelta = ypos - lastY;
	lastX = xpos;
	lastY = ypos;

	if (dragCamera) {
		mouseX -= xDelta * cursorSensitivity;
		mouseY -= yDelta * cursorSensitivity;
		mouseY = std::min(std::max(mouseY, -M_PI), -0.000001);
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	radius += yoffset;
	radius = std::min(radius, 0.0f);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_LEFT_ALT && action == GLFW_PRESS)
		dragCamera = true;
	if (key == GLFW_KEY_LEFT_ALT && action == GLFW_RELEASE)
		dragCamera = false;
}


void init(GLFWwindow* window) {
	glClearColor(0,0,0, 1.0f);
	setupVertices();

	//setup dear imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 430");

	defaultMaterial.diffuseTex = loadTexture("textures/container2.png");
	defaultMaterial.specularTex = loadTexture("textures/container2_specular.png");

	defaultShader.Compile("vertShader.glsl", "fragShader.glsl");
	defaultShader.Use();
	defaultShader.SetFloat3("albedo", glm::vec3(1.0f, 1.0f, 1.0f));
	defaultShader.SetMaterial(defaultMaterial);
	defaultShader.SetInt("material.diffuse", defaultMaterial.diffuseTex);

	//calculate the perspective matrix
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);  // 1.0472 radians = 60 degrees
}

void ImGuiDisplay() {
	ImGui::Begin("Properties");
	ImGuiColorEditFlags flags = 0;
	flags |= ImGuiColorEditFlags_NoInputs;
	ImGui::Text("Environment");
	ImGui::ColorEdit3("Ambient color", (float*)&ambientColor, flags);
	ImGui::ColorEdit3("Sun color", (float*)&sunColor, flags);
	ImGui::DragFloat3("Sun direction", (float*)&sunDirection,0.1f,-1.0f,1.0f);

	ImGui::Text("Cube material");
	ImGui::DragFloat("shininess", (float*)&defaultMaterial.shininess, 0.1f, 1.0f, 128.0f);
	//defaultShader.SetMaterial(defaultMaterial);
	
	ImGui::Text("Diffuse texture");
	ImGui::Image((void*)(intptr_t)defaultMaterial.diffuseTex,ImVec2(256,256));
	ImGui::Text("Specular texture");
	ImGui::Image((void*)(intptr_t)defaultMaterial.specularTex, ImVec2(256, 256));
	ImGui::End();
}

void display(GLFWwindow* window, double currentTime) {
	//imgui
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGuiDisplay();
	ImGui::Render();

	//options
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);

	//camera orbit

	mainCamera.position.x = sin(mouseX) * sin(mouseY) * radius;
	mainCamera.position.y = cos(mouseY) * radius;
	mainCamera.position.z = sin(mouseY) * cos(mouseX) * radius;	
	mainCamera.view_matrix = glm::lookAt(mainCamera.position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//cube
	defaultShader.Use();

	//uniforms
	defaultShader.SetMatrix4f("proj_matrix", pMat);
	defaultShader.SetMatrix4f("v_matrix", mainCamera.view_matrix);
	defaultShader.SetFloat3("sunDir", glm::normalize(sunDirection));
	defaultShader.SetFloat3("viewPos", mainCamera.position);
	defaultShader.SetFloat3("sunColor", sunColor);
	defaultShader.SetFloat3("ambientColor", ambientColor);

	defaultShader.SetMaterial(defaultMaterial);

	//transforms
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	//mMat = glm::rotate(mMat, (float)currentTime, glm::vec3(0.7f, 1.0f, 0.0f));//apply cube rotation

	defaultShader.SetMatrix4f("m_matrix", mMat);	
	nMat = glm::mat3(glm::transpose(glm::inverse(mMat)));
	defaultShader.SetMatrix3f("n_matrix", nMat);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, defaultMaterial.diffuseTex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, defaultMaterial.specularTex);
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);//draw cube

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main(void) {    // main() is unchanged from before
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(960, 800, "Webgl sketchbook", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);
	//callbacks
	glfwSetWindowSizeCallback(window, WindowResizeCallback);
	glfwSetCursorPosCallback(window, cursor_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);

	init(window);
	bool firstFrame = true;

	while (!glfwWindowShouldClose(window)) {

		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glClearColor(ambientColor.x * 0.8f, ambientColor.y * 0.8f, ambientColor.z * 0.8f, 1.0f);
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}