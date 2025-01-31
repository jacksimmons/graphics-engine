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
#include <chrono>

#define STB_IMAGE_IMPLEMENTATION
#include "editor.hpp"
#include "key_input.hpp"
#include "scene.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "framebuffer.hpp"
#include "transform.hpp"
#include "script.hpp"
#include "nodes/node.hpp"
#include "nodes/model.hpp"
#include "nodes/hierarchy.hpp"
#include "nodes/scene_view.hpp"
#include "nodes/inspector.hpp"
#include "nodes/light.hpp"
#include "nodes/models/cube.hpp"
#include "nodes/models/primitive.hpp"
#include "nodes/cube_map.hpp"
#include "static/time.hpp"


// Enable debug output
static void GLAPIENTRY msgCallback(GLenum source,
	GLenum type, GLenum id, GLenum severity, GLsizei length, const GLchar* message,
	const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message
	);
}


Editor::Editor()
{
	m_settings = std::make_unique<WindowSettings>();
	m_settings->configFlags = ImGuiConfigFlags_DockingEnable;
	m_settings->mainWinFlags = ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_MenuBar
		| ImGuiWindowFlags_NoBringToFrontOnFocus;
	m_settings->windowSize = glm::ivec2(800, 600);

	initGL();
	initImGui();

	// Flip loaded textures on the y-axis.
	stbi_set_flip_vertically_on_load(GL_TRUE);
	initSystem();
	loadScene();
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
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	m_window = glfwCreateWindow(m_settings->windowSize.x, m_settings->windowSize.y, (char *)"TankEngine", nullptr, nullptr);
	if (m_window == nullptr)
	{
		TE_CORE_CRITICAL("GLFW failed to create window.");
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(m_window);

	// Initialise callbacks
	Tank::KeyInput::setupKeyInputs(m_window);
	glfwSetFramebufferSizeCallback(m_window, Editor::onWindowSizeChange);

	// Initialise GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		TE_CORE_CRITICAL("GLAD failed to initialise.");
		return;
	}

	// Create viewport
	glViewport(0, 0, m_settings->windowSize.x, m_settings->windowSize.y);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// Enable depth buffer
	glEnable(GL_DEPTH_TEST);

	// Enable debug output
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(msgCallback, nullptr);

	// Re-set title to add graphics info
	std::string graphicalDeviceInfo = (const char*)glGetString(GL_VENDOR)
		+ std::string(": ") + (const char*)glGetString(GL_RENDERER);
	std::string title = "TankEngine [" + graphicalDeviceInfo + "]";
	glfwSetWindowTitle(m_window, title.c_str());
}


void Editor::initImGui()
{
	// Initialise ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	io.ConfigFlags = m_settings->configFlags;

	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");
	ImGui::StyleColorsDark();
}


void Editor::initSystem()
{
	m_system = std::make_unique<Tank::Node>("Editor");
	{
		m_system->addChild(std::make_unique<Tank::Inspector>("Inspector"));
		m_system->addChild(std::make_unique<Tank::Hierarchy>("Hierarchy"));
	}
}


void Editor::loadScene()
{
	// Create nodes
	{
		auto scene = std::make_unique<Tank::Scene>();
		scene->addChild(std::make_unique<Tank::Camera>("Camera"));
		scene->addChild(std::make_unique<Tank::Skybox>("Skybox"));
		scene->setActiveCamera(dynamic_cast<Tank::Camera *>(scene->getChild("Camera")));

		auto cube = std::make_unique<Tank::Cube>("Awesomeface", "shader.vert", "shader.frag");
		scene->addChild(std::move(cube));

		// Set the active scene and m_scene
		Tank::Scene::setActiveScene(scene.get());
		m_scene = std::move(scene);
	}

	// Lights
	glm::vec3 amb{ 0.1f, 0.1f, 0.1f };
	glm::vec3 diff{ 0.5f, 0.5f, 0.5f };
	glm::vec3 spec{ 0.1f, 0.1f, 0.1f };
	glm::vec3 pointLightPositions[]
	{
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	Tank::Node *root = Tank::Scene::getActiveScene();
	for (int i = 0; i < 1; i++)
	{
		std::string name = "PtLight";
		auto lightCube = std::make_unique<Tank::Cube>("PtLightContainer", "lightCubeShader.vert", "lightCubeShader.frag");
		auto light = std::make_unique<Tank::PointLight>(name, amb, diff, spec);
		lightCube->getTransform()->setTranslation(pointLightPositions[1]);
		lightCube->addChild(std::move(light));
		root->addChild(std::move(lightCube));
	}

	// Initialise input. Must be done after scene.
	m_keyInput = std::make_unique<Tank::KeyInput>(std::vector<int>(
	{
		GLFW_KEY_ESCAPE,
		GLFW_KEY_F1,

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

	m_system->addChild(std::make_unique<Tank::SceneView>("SceneView", m_settings->windowSize, m_settings->windowSize, m_keyInput.get()));
}


void Editor::run()
{	
	ImGuiIO &io = ImGui::GetIO();
	auto frameStart = std::clock();
	auto frameEnd = std::clock();
	float lastFrameDelta = 0;

	// ===== MAINLOOP =====
	while (!glfwWindowShouldClose(m_window))
	{
		frameStart = std::clock();

		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw UI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::SetNextWindowPos(ImVec2(0,0));
		ImGui::SetNextWindowSize(io.DisplaySize);
		
		ImGui::Begin("##Main", nullptr, m_settings->mainWinFlags);

		//ImGui::ShowDemoWindow();

		handleKeyInput();
		// Decay input states (comes after handleKeyInput)
		m_keyInput->update();
		m_system->update();
		//m_scene->update();
		//ImGui::ShowDemoWindow();

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

		frameEnd = std::clock();
		Tank::Time::setFrameDelta(frameStart, frameEnd);
	}
}


void Editor::handleKeyInput()
{
	if (m_keyInput->getKeyState(GLFW_KEY_ESCAPE) == Tank::KeyState::Pressed)
		glfwSetWindowShouldClose(m_window, GL_TRUE);

	((Tank::SceneView *)m_system->getChild("SceneView"))->handleKeyInput();
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
	return std::make_unique<Editor>();
}