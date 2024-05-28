#include <string>
#include <cmath>
#include <iostream>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "key_input.hpp"
#include "scene.hpp"
#include "shader.hpp"
#include "uniform.hpp"
#include "texture.hpp"
#include "editor.hpp"
#include "nodes/node.hpp"
#include "nodes/model.hpp"
#include "nodes/hierarchy.hpp"


static void handleKeyInput(std::shared_ptr<KeyInput> input, GLFWwindow *win, Tank::Camera &cam)
{
	if (input->getKeyState(GLFW_KEY_ESCAPE) == KeyState::Pressed)
		glfwSetWindowShouldClose(win, GL_TRUE);

	if (input->getKeyState(GLFW_KEY_0) == KeyState::Pressed)
		input->cycleRenderMode();


	if (input->getKeyState(GLFW_KEY_W) == KeyState::Held)
		cam.translate(glm::vec3(0.0f, -0.01f, 0.0f));

	if (input->getKeyState(GLFW_KEY_A) == KeyState::Held)
		cam.translate(glm::vec3(0.01f, 0.0f, 0.0f));

	if (input->getKeyState(GLFW_KEY_S) == KeyState::Held)
		cam.translate(glm::vec3(0.0f, 0.01f, 0.0f));

	if (input->getKeyState(GLFW_KEY_D) == KeyState::Held)
		cam.translate(glm::vec3(-0.01f, 0.0f, 0.0f));

	if (input->getKeyState(GLFW_KEY_Q) == KeyState::Held)
		cam.translate(glm::vec3(0.0f, 0.0f, 0.01f));

	if (input->getKeyState(GLFW_KEY_E) == KeyState::Held)
		cam.translate(glm::vec3(0.0f, 0.0f, -0.01f));


	if (input->getKeyState(GLFW_KEY_J) == KeyState::Held)
		cam.rotate(glm::vec3(-0.01f, 0.0f, 0.0f));

	if (input->getKeyState(GLFW_KEY_L) == KeyState::Held)
		cam.rotate(glm::vec3(0.01f, 0.0f, 0.0f));

	if (input->getKeyState(GLFW_KEY_I) == KeyState::Held)
		cam.rotate(glm::vec3(0.0f, 0.01f, 0.0f));

	if (input->getKeyState(GLFW_KEY_K) == KeyState::Held)
		cam.rotate(glm::vec3(0.0f, -0.01f, 0.0f));

	if (input->getKeyState(GLFW_KEY_U) == KeyState::Held)
		cam.rotate(glm::vec3(0.0f, 0.0f, 0.01f));

	if (input->getKeyState(GLFW_KEY_O) == KeyState::Held)
		cam.rotate(glm::vec3(0.0f, 0.0f, -0.01f));
}


Editor::Editor()
{
	initGL();
	initImGui();

	// Create scene
	auto root = std::make_shared<Tank::Node>("Root");
	std::shared_ptr<Tank::Camera> cam = std::make_shared<Tank::Camera>(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), "Camera");
	std::shared_ptr<Tank::Scene> scene = std::make_shared<Tank::Scene>(root, cam);

	auto user = std::make_shared<Tank::Node>("User");
	Tank::Node::addChild(user, std::make_shared<Tank::Model>("Cube"));
	Tank::Node::addChild(root, user);

	auto system = std::make_shared<Tank::Node>("System");
	Tank::Node::addChild(system, std::shared_ptr<Hierarchy>(new Hierarchy(scene, "Hierarchy")));
	Tank::Node::addChild(root, system);

	Tank::Scene::setActiveScene(scene);

	glEnable(GL_DEPTH_TEST);
}

Editor::~Editor()
{
	cleanup();
}

void Editor::initGL()
{
	int width = 800, height = 600;

	if (!glfwInit())
	{
		TE_CORE_CRITICAL("GLFW failed to initialise.");
		glfwTerminate();
	}

	// Set GL version hint
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(width, height, (char *)"TankEngine", nullptr, nullptr);
	if (m_window == nullptr)
	{
		TE_CORE_CRITICAL("GLFW failed to create window.");
		glfwTerminate();
	}
	glfwMakeContextCurrent(m_window);

	// Initialise callbacks
	KeyInput::setupKeyInputs(m_window);
	glfwSetFramebufferSizeCallback(m_window, Editor::onFramebufferSizeChange);

	// Initialise GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		TE_CORE_CRITICAL("GLAD failed to initialise.");
	}

	// Create viewport
	glViewport(0, 0, width, height);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void Editor::initImGui()
{
	// Initialise ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &imIO = ImGui::GetIO();
	imIO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::StyleColorsDark();
}

void Editor::run()
{
	std::shared_ptr<KeyInput> keyInput = std::make_shared<KeyInput>(std::vector<int>(
		{
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
		GLFW_KEY_O
		}));

	// ===== MAINLOOP =====
	while (!glfwWindowShouldClose(m_window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw UI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Draw scene
		// Scene must be updated after input is handled.
		keyInput->update();
		auto scene = Tank::Scene::getActiveScene();
		scene->update();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		handleKeyInput(keyInput, m_window, *scene->getActiveCamera());

		// Double buffering
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	cleanup();
}

void Editor::cleanup()
{
	glfwDestroyWindow(m_window);
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}


std::unique_ptr<Tank::Application> Tank::createApplication()
{
	return std::move(std::make_unique<Editor>());
}