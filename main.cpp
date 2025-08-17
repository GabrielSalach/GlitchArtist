#include <iostream>
// #include "imgui.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "src/Application.h"

int main(int, char **)
{
	// Setup window
	if (!glfwInit())
		return 1;

		// Decide GL+GLSL versions
#if __APPLE__
	// GL 3.2 + GLSL 150
	const char *glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);		   // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char *glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	// Create window with graphics context
	GLFWwindow *window = glfwCreateWindow(1280, 720, "GlitchArtist", NULL, NULL);
	if (window == nullptr)
		return 1;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
		return 1;

	int screen_width, screen_height;
	glfwGetWindowSize(window, &screen_width, &screen_height);
	glViewport(0, 0, screen_width, screen_height);


	GlitchArtist::Application app;
	app.Init(window, glsl_version);
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0,0,0,1);
		glClear(GL_COLOR_BUFFER_BIT);
		app.NewFrame();
		app.Update();
		app.Render();
		glfwSwapBuffers(window);
	}

	app.Shutdown();

	return 0;
}