#include <string>
#include <cmath>
#include <iostream>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "editor.hpp"
#include "key_input.hpp"
#include "scene.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "framebuffer.hpp"
#include "nodes/node.hpp"
#include "nodes/model.hpp"
#include "nodes/hierarchy.hpp"
#include "nodes/scene_view.hpp"


Editor::Editor()
{
	m_settings.configFlags = ImGuiConfigFlags_DockingEnable;
	m_settings.mainWinFlags = ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_MenuBar
		| ImGuiWindowFlags_NoBringToFrontOnFocus;
	m_settings.windowSize = glm::ivec2(800, 600);

	initGL();
	initImGui();
	generateSceneThenInitInput();
}

void Editor::initGL()
{
	if (!glfwInit())
	{
		TE_CORE_CRITICAL("GLFW failed to initialise.");
		glfwTerminate();
	}

	// Set GL version hint
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(m_settings.windowSize.x, m_settings.windowSize.y, (char *)"TankEngine", nullptr, nullptr);
	if (m_window == nullptr)
	{
		TE_CORE_CRITICAL("GLFW failed to create window.");
		glfwTerminate();
	}
	glfwMakeContextCurrent(m_window);

	// Initialise callbacks
	KeyInput::setupKeyInputs(m_window);
	glfwSetFramebufferSizeCallback(m_window, Editor::onWindowSizeChange);

	// Initialise GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		TE_CORE_CRITICAL("GLAD failed to initialise.");
	}

	// Create viewport
	glViewport(0, 0, m_settings.windowSize.x, m_settings.windowSize.y);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// Enable depth buffer
	glEnable(GL_DEPTH_TEST);
}

void Editor::initImGui()
{
	// Initialise ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	m_io = std::shared_ptr<ImGuiIO>(&ImGui::GetIO());
	m_io->ConfigFlags = m_settings.configFlags;

	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::StyleColorsDark();
}

void Editor::generateSceneThenInitInput()
{
	// Create scene
	auto root = std::make_shared<Tank::Node>("Root");
	std::shared_ptr<Tank::Camera> cam = std::make_shared<Tank::Camera>(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), "Camera");
	root->addChild(cam);
	root->addChild(std::make_shared<Tank::Model>("Cube"));

	m_uiRoot = std::make_shared<Tank::Node>("System");
	m_uiRoot->addChild(std::make_shared<Tank::Hierarchy>("Hierarchy"));
	m_uiRoot->addChild(std::make_shared<Tank::SceneView>("SceneView", m_settings.windowSize, m_settings.windowSize));

	std::shared_ptr<Tank::Scene> scene = std::make_shared<Tank::Scene>(root, cam);
	Tank::Scene::setActiveScene(scene);

	// Initialise input. Requires scene init first.
	m_keyInput = std::make_unique<KeyInput>(std::vector<int>(
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
}

void Editor::run()
{	
	// ===== MAINLOOP =====
	while (!glfwWindowShouldClose(m_window))
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw UI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::SetNextWindowPos(ImVec2(0,0));
		ImGui::SetNextWindowSize(m_io->DisplaySize);
		
		ImGui::Begin("##Main", nullptr, m_settings.mainWinFlags);

		handleKeyInput();
		// Decay input states (comes after handleKeyInput)
		m_keyInput->update();
		m_uiRoot->update();

		ImGui::End();
		ImGui::Render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//// ! Remove this?
		GLFWwindow *backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
		// ! End

		// Double buffering
		glfwSwapBuffers(m_window);
	}
}

void Editor::handleKeyInput()
{
	auto cam = Tank::Scene::getActiveScene()->getActiveCamera();
	
	if (m_keyInput->getKeyState(GLFW_KEY_ESCAPE) == KeyState::Pressed)
		glfwSetWindowShouldClose(m_window, GL_TRUE);

	if (m_keyInput->getKeyState(GLFW_KEY_0) == KeyState::Pressed)
		m_keyInput->cycleRenderMode();


	if (m_keyInput->getKeyState(GLFW_KEY_W) == KeyState::Held)
		cam->translate(glm::vec3(0.0f, -0.01f, 0.0f));

	if (m_keyInput->getKeyState(GLFW_KEY_A) == KeyState::Held)
		cam->translate(glm::vec3(0.01f, 0.0f, 0.0f));

	if (m_keyInput->getKeyState(GLFW_KEY_S) == KeyState::Held)
		cam->translate(glm::vec3(0.0f, 0.01f, 0.0f));

	if (m_keyInput->getKeyState(GLFW_KEY_D) == KeyState::Held)
		cam->translate(glm::vec3(-0.01f, 0.0f, 0.0f));

	if (m_keyInput->getKeyState(GLFW_KEY_Q) == KeyState::Held)
		cam->translate(glm::vec3(0.0f, 0.0f, 0.01f));

	if (m_keyInput->getKeyState(GLFW_KEY_E) == KeyState::Held)
		cam->translate(glm::vec3(0.0f, 0.0f, -0.01f));


	if (m_keyInput->getKeyState(GLFW_KEY_J) == KeyState::Held)
		cam->rotate(glm::vec3(-0.01f, 0.0f, 0.0f));

	if (m_keyInput->getKeyState(GLFW_KEY_L) == KeyState::Held)
		cam->rotate(glm::vec3(0.01f, 0.0f, 0.0f));

	if (m_keyInput->getKeyState(GLFW_KEY_I) == KeyState::Held)
		cam->rotate(glm::vec3(0.0f, 0.01f, 0.0f));

	if (m_keyInput->getKeyState(GLFW_KEY_K) == KeyState::Held)
		cam->rotate(glm::vec3(0.0f, -0.01f, 0.0f));

	if (m_keyInput->getKeyState(GLFW_KEY_U) == KeyState::Held)
		cam->rotate(glm::vec3(0.0f, 0.0f, 0.01f));

	if (m_keyInput->getKeyState(GLFW_KEY_O) == KeyState::Held)
		cam->rotate(glm::vec3(0.0f, 0.0f, -0.01f));
}

Editor::~Editor()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

std::unique_ptr<Tank::Application> Tank::createApplication()
{
	return std::move(std::make_unique<Editor>());
}