#define STB_IMAGE_IMPLEMENTATION

#include <string>
#include <cmath>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "init.hpp"
#include "shader.hpp"
#include "uniform.hpp"
#include "texture.hpp"
#include "camera.hpp"


float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
unsigned int indices[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};


static void handleKeyInput(KeyInput *input, GLFWwindow *win, Camera *cam)
{
	if (input->getKeyState(GLFW_KEY_ESCAPE) == KeyState::Pressed)
		glfwSetWindowShouldClose(win, GL_TRUE);

	if (input->getKeyState(GLFW_KEY_0) == KeyState::Pressed)
		input->cycleRenderMode();


	if (input->getKeyState(GLFW_KEY_W) == KeyState::Held)
		cam->move(glm::vec3(0.0f, -0.01f, 0.0f));

	if (input->getKeyState(GLFW_KEY_A) == KeyState::Held)
		cam->move(glm::vec3(0.01f, 0.0f, 0.0f));

	if (input->getKeyState(GLFW_KEY_S) == KeyState::Held)
		cam->move(glm::vec3(0.0f, 0.01f, 0.0f));

	if (input->getKeyState(GLFW_KEY_D) == KeyState::Held)
		cam->move(glm::vec3(-0.01f, 0.0f, 0.0f));

	if (input->getKeyState(GLFW_KEY_Q) == KeyState::Held)
		cam->move(glm::vec3(0.0f, 0.0f, 0.01f));

	if (input->getKeyState(GLFW_KEY_E) == KeyState::Held)
		cam->move(glm::vec3(0.0f, 0.0f, -0.01f));


	if (input->getKeyState(GLFW_KEY_J) == KeyState::Held)
		cam->rotate(glm::vec3(-0.01f, 0.0f, 0.0f));

	if (input->getKeyState(GLFW_KEY_L) == KeyState::Held)
		cam->rotate(glm::vec3(0.01f, 0.0f, 0.0f));

	if (input->getKeyState(GLFW_KEY_I) == KeyState::Held)
		cam->rotate(glm::vec3(0.0f, 0.01f, 0.0f));

	if (input->getKeyState(GLFW_KEY_K) == KeyState::Held)
		cam->rotate(glm::vec3(0.0f, -0.01f, 0.0f));

	if (input->getKeyState(GLFW_KEY_U) == KeyState::Held)
		cam->rotate(glm::vec3(0.0f, 0.0f, 0.01f));

	if (input->getKeyState(GLFW_KEY_O) == KeyState::Held)
		cam->rotate(glm::vec3(0.0f, 0.0f, -0.01f));
}


int main()
{
	int width = 800; int height = 600;


	// Create GLFW window
	GLFWwindow *window = initGLFW(width, height, (char*)"Renderer");
	if (!window)
	{
		printf("Failed to init GLFW");
		return -1;
	}

	KeyInput *input = new KeyInput({
		GLFW_KEY_ESCAPE,
		GLFW_KEY_0,

		GLFW_KEY_W,
		GLFW_KEY_A,
		GLFW_KEY_S,
		GLFW_KEY_D,
		GLFW_KEY_Q,
		GLFW_KEY_E,

		GLFW_KEY_I,
		GLFW_KEY_J,
		GLFW_KEY_K,
		GLFW_KEY_L,
		GLFW_KEY_U,
		GLFW_KEY_O,
	});
	initGLAD();


	// ===== VIEWPORT =====
	glViewport(0, 0, width, height);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	Shader *shader;
	try
	{
		shader = new Shader("shader.vert", "shader.frag");
	}
	catch (ShaderCompileError const &e)
	{
		std::cout << e.what() << std::endl;
		return -1;
	}


	// ===== INIT VBO/VAO =====
	unsigned int vao;
	glGenVertexArrays(1, &vao);

	unsigned int vbo;
	glGenBuffers(1, &vbo);

	unsigned int ebo;
	glGenBuffers(1, &ebo);
	
	glBindVertexArray(vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	//// Colour
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	// Texture coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// ===== TEXTURES =====
	float borderColour[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColour);

	// Flip loaded textures on the y-axis.
	stbi_set_flip_vertically_on_load(GL_TRUE);

	Texture tex0 = textureFromFile("wall.jpg", GL_TEXTURE0, GL_RGB);
	Texture tex1 = textureFromFile("awesomeface.png", GL_TEXTURE1, GL_RGBA);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Draw
	shader->use();
	Uniform *uTex0 = new Uniform(shader, "tex0");
	uTex0->setInt(0);
	Uniform *uTex1 = new Uniform(shader, "tex1");
	uTex1->setInt(1);

	Uniform *P = new Uniform(shader, "proj");
	Uniform *V = new Uniform(shader, "view");
	Uniform *M = new Uniform(shader, "model");

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	M->setMat4(model);

	glm::mat4 proj;
	proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	P->setMat4(proj);

	Camera *cam = new Camera(glm::vec3(0, 0, 3),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)
	);

	glEnable(GL_DEPTH_TEST);

	// ===== MAINLOOP =====
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex1);

		model = glm::rotate(model, 0.001f, glm::vec3(0.1f, 0.5f, 1.0f));

		M->setMat4(model);
		V->setMat4(cam->getView());

		glBindVertexArray(vao);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		handleKeyInput(input, window, cam);

		// Input must be updated after input is handled.
		input->update();
		cam->update();

		// Double buffering
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup
	delete shader, uTex0, uTex1, P, V, M, cam;
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glfwTerminate();
	return 0;
}