#include <string>
#include <cmath>
#include <iostream>
#include <memory>
#include <chrono>
#include <format>

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
#include "shader.hpp"
#include "texture.hpp"
#include "framebuffer.hpp"
#include "transform.hpp"
#include "script.hpp"
#include "log.hpp"
#include "serialisation.hpp"
#include "static/time.hpp"
#include "nodes/node.hpp"
#include "nodes/scene.hpp"
#include "nodes/model.hpp"
#include "nodes/light.hpp"
#include "nodes/cube_map.hpp"
#include "nodes/ui/console.hpp"
#include "nodes/ui/scene_view.hpp"
#include "nodes/ui/hierarchy.hpp"
#include "nodes/ui/inspector.hpp"


// Enable debug output
static void GLAPIENTRY msgCallback(GLenum source,
	GLenum type, GLenum id, GLenum severity, GLsizei length, const GLchar* message,
	const void* userParam)
{
	TE_CORE_ERROR(std::format("GL CALLBACK: {} type = {}, severity = {}, message = {}",
		type == GL_DEBUG_TYPE_ERROR ? "**GL ERROR**" : "",
		type, severity, message));
}


namespace Tank::Editor
{
	EditorApp::EditorApp()
	{
		m_settings = std::make_unique<WindowSettings>();
		m_settings->configFlags = ImGuiConfigFlags_DockingEnable;
		m_settings->mainWinFlags = ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoTitleBar // No blue bar
			| ImGuiWindowFlags_NoBringToFrontOnFocus;
		m_settings->windowSize = glm::ivec2(800, 600);

		initGL();
		initImGui();

		// Flip loaded textures on the y-axis.
		stbi_set_flip_vertically_on_load(GL_TRUE);
	}


	void EditorApp::initGL()
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
		glfwSetFramebufferSizeCallback(m_window, EditorApp::onWindowSizeChange);

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
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(msgCallback, nullptr);

		// Re-set title to add graphics info
		std::string graphicalDeviceInfo = (const char*)glGetString(GL_VENDOR)
			+ std::string(": ") + (const char*)glGetString(GL_RENDERER);
		std::string title = "TankEngine [" + graphicalDeviceInfo + "]";
		glfwSetWindowTitle(m_window, title.c_str());
	}


	void EditorApp::initImGui()
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


	void EditorApp::preSceneSetup()
	{
		m_system = std::make_unique<Tank::Node>("Editor");
		m_system->addChild(std::unique_ptr<_Hierarchy>(new _Hierarchy("Hierarchy")));
		m_system->addChild(std::unique_ptr<_Inspector>(new _Inspector("Inspector")));
	}


	void EditorApp::loadScene(std::unique_ptr<Tank::Scene> scene)
	{
		m_scene = std::move(scene);
		m_scene->setActiveScene(m_scene.get());
	}


	void EditorApp::loadDemoScene()
	{
		// Create nodes
		{
			auto scene = std::make_unique<Tank::Scene>();
			scene->addChild(std::make_unique<Tank::Camera>());
			scene->addChild(std::make_unique<Tank::CubeMap>());
			scene->setActiveCamera(dynamic_cast<Tank::Camera *>(scene->getChild("Camera")));

			auto backpack = std::make_unique<Tank::Model>("Backpack", "shader.vert", "shader.frag", "models/backpack/backpack.obj");
			scene->addChild(std::move(backpack));

			loadScene(std::move(scene));
		}

		// Lights
		{
			std::string name = "DirLight";
			auto light = std::make_unique<Tank::DirLight>(name,
				glm::vec3{ 0.0f, -1.0f, 0.0f },
				glm::vec3{ 0.02f, 0.02f, 0.02f },
				glm::vec3{ 0.2f, 0.2f, 0.2f },
				glm::vec3{ 0.1f, 0.1f, 0.1f }
			);
			m_scene->addChild(std::move(light));
		}
	}


	void EditorApp::postSceneSetup()
	{
		// Initialise input. Must be done after scene load, and before scene view load.
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

		m_system->addChild(std::unique_ptr<_SceneView>(new _SceneView("SceneView", m_settings->windowSize, m_settings->windowSize, m_keyInput.get())));
		m_system->addChild(std::unique_ptr<_Console>(new _Console("Console")));
	}


	void EditorApp::run()
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

			ImGui::SetNextWindowPos(ImVec2(0, 20));
			ImGui::SetNextWindowSize(io.DisplaySize);

			ImGui::Begin("##Main", nullptr, m_settings->mainWinFlags);

			// Draw File, Edit, etc...
			drawMainMenuBar();

			if (m_keyInput)
			{
				handleKeyInput();
				// Decay input states (comes after handleKeyInput)
				m_keyInput->update();
			}
			if (m_system) m_system->update();

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
			Time::setFrameDelta(frameStart, frameEnd);
		}
	}


	void EditorApp::handleKeyInput()
	{
		if (m_keyInput->getKeyState(GLFW_KEY_ESCAPE) == Tank::KeyState::Pressed)
			glfwSetWindowShouldClose(m_window, GL_TRUE);

		((_SceneView*)m_system->getChild("SceneView"))->handleKeyInput();
	}


	void EditorApp::drawMainMenuBar()
	{
		bool newProject = false;
		bool openProject = false;
		bool saveProject = false;
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::MenuItem("New Project", "", &newProject);
				ImGui::MenuItem("Open Project", "", &openProject);
				ImGui::MenuItem("Save Project", "", &saveProject);
				ImGui::EndMenu();
			}
		}
		ImGui::EndMainMenuBar();

		if (newProject)
		{
			preSceneSetup();
			loadDemoScene();
			postSceneSetup();
		}
		if (openProject)
		{
			preSceneSetup();
			loadScene(std::unique_ptr<Tank::Scene>(Tank::Serialisation::loadScene("test.scene")));
			postSceneSetup();
		}
		if (saveProject && m_scene)
		{
			Tank::Serialisation::saveScene(m_scene.get(), "test.scene");
		}
	}


	EditorApp::~EditorApp()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}
}


namespace Tank
{
	std::unique_ptr<Tank::Application> createApplication()
	{
		return std::make_unique<Editor::EditorApp>();
	}
}