#include "config.h"
#include "Callback.h"
#include "Utilities.h"
#include "Vertex.h"
#include "OpenGLLoader.h"
#include "OpenGLDraw.h"
#include "Input.h"
#include "GLSLShaderLoader.h"
#include "Particle.h"


int main(int argc, char** argv)
{
	glfwSetErrorCallback(glfw_error_callback);
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

	GLFWwindow* window = glfwCreateWindow(800, 600, extract_version(argv[0]), nullptr, nullptr);

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSetWindowCloseCallback(window, glfw_window_close_callback);
	glfwSetFramebufferSizeCallback(window, glfw_framebuffer_size_callback);

	const char* vertshader =
		"#version 450 core                                                        \n"
		"layout(location = 0) in vec3 vertexPosition_modelspace;                  \n"
		"out vec4 ParticleColor;                                                  \n"
		"uniform mat4 projection;                                                 \n"
		"uniform vec2 offset;                                                     \n"
		"uniform vec4 color;                                                      \n"
		//"uniform mat4 modelmatrix;                                              \n"
		"void main(){                                                             \n"
		//"gl_Position = modelmatrix * vec4(vertexPosition_modelspace, 1.0);      \n"
		"float scale = 10.0f;                                                     \n"
		"TexCoords = vertexPosition_modelspace.zw;                                \n"
		"gl_Position = projection * vec4((vertex.xy * scale) + offset, 0.0, 1.0); \n"
		"ParticleColor = color;                                                   \n"
		"gl_Position = vec4(vertexPosition_modelspace, 1.0);                      \n"
		"}";
	const char* fragshader =
		"#version 450 core                                     \n"
		"in vec4 ParticleColor;                                \n"
		"out vec3 color;                                       \n"
		"uniform vec3 ucolor = vec3(.2,.5,.5);                 \n"
		"void main() {                                         \n"
		//"color = ucolor;                                     \n"
		"color = (texture(sprite, TexCoords) * ParticleColor); \n"
		"}";
	unsigned int mainShader = LoadShader(vertshader, fragshader);
	glClearColor(.2f, .2f, .6f, 0.f);
	std::vector<DrawDetails> ourDrawDetails;
	{
		// CREATE OBJECT TO DISPLAY (POINTS) C++
		std::vector<Vertex> obj_pts;
		obj_pts.emplace_back(.5f, -.5f, 0.f);
		obj_pts.emplace_back(-.5f, -.5f, 0.f);
		obj_pts.emplace_back(0.f, .5f, 0.f);

		std::vector<uint32_t> elem = { 0, 1, 2 };

		// UPLOAD DATA TO GRAPHICS CARD
		ourDrawDetails.push_back(UploadMesh(obj_pts, elem));
	}

	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution(0.f, 1.f);

	while (!glfwWindowShouldClose(window))
	{
		// HANDLE KEYPRESS
		ProcessInput(window);
		glClear(GL_COLOR_BUFFER_BIT);
		// RENDER OUR OBJECT
		glUseProgram(mainShader);

		float c1 = distribution(generator);
		float c2 = distribution(generator);
		float c3 = distribution(generator);
		uint32_t var = glGetUniformLocation(mainShader, "ucolor");
		glUniform3f(var, c1, c2, c3);

		Draw(ourDrawDetails);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// UNLOAD DATA FROM GRAPHICS CARD
	UnloadMesh(ourDrawDetails);

	glfwTerminate();
	return 0;
}