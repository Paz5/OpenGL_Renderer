//includes
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <iomanip>

//project scope includes
#include "Utils.h"

#define numVAOs 1

using namespace std;

GLuint renderingProgram;
GLuint vao[numVAOs];

string mainWindowTitle = "Hello World";

ImVec4 clearColor = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);

void Init(GLFWwindow* window) {
	renderingProgram = CreateShaderProgram("vertShader.glsl", "fragShader.glsl");
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
}

void InitImGui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.Fonts->AddFontDefault();
	io.Fonts->Build();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
}

void Display(GLFWwindow* window, double currentTime) {
	glUseProgram(renderingProgram);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void inline ImGuiDraw()
{
	ImGui::Begin("Hello, world!");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }

	//init window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(600, 600, "Main Window", NULL, NULL);
	glfwSetWindowTitle(window, mainWindowTitle.c_str());
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	Init(window);
	InitImGui(window);

	//main draw loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glClear(GL_DEPTH_BUFFER_BIT);
		glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		Display(window, glfwGetTime());
		ImGuiDraw();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);

	}
	//clean up
	glfwDestroyWindow(window);
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	exit(EXIT_SUCCESS);
}