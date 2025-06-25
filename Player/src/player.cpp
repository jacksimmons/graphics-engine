#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "player.hpp"
#include "key_input.hpp"
#include "scene_serialisation.hpp"
#include "nodes/node.hpp"
#include "nodes/scene.hpp"
#include "nodes/camera.hpp"
#include "nodes/cube_map.hpp"
#include "nodes/light.hpp"
#include "nodes/sprite.hpp"
#include "static/time.hpp"


namespace Tank
{
	Player::Player() : Application(
		{
			GLFW_KEY_F1,
			GLFW_KEY_F2,
			GLFW_KEY_F3,
			GLFW_KEY_F4,
			GLFW_KEY_F5,
			GLFW_KEY_F6,

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
		}
	)
	{
		m_root = std::make_unique<Node>("Root");
		loadScene();
	}


	void Player::loadScene()
	{
		Scene *scene = Serialisation::loadScene("snake.scene");
		m_snake = scene->getChild("Snake");
		Scene::setActiveScene(scene);
		m_root->addChild(std::unique_ptr<Scene>(scene));
	}


	Player::~Player()
	{
		m_root->shutdown();
	}


	void Player::step()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glStencilMask(0x00); // Don't write to stencil by default

		m_root->update();
		m_root->startup();

		if (m_timeSinceLastMove > m_moveFrequency && m_moveDir != glm::ivec2 { 0, 0 })
		{
			m_timeSinceLastMove = 0;
			auto snakeTransform = m_snake->getTransform();
			snakeTransform->setLocalTranslation(
				snakeTransform->getLocalTranslation() +
				glm::vec3{ m_moveDir.x, m_moveDir.y, 0 }
			);
		}

		m_timeSinceLastMove += Time::getFrameDelta();
		TE_CORE_INFO(m_timeSinceLastMove);
	}


	void Player::handleKeyInput()
	{
		if (m_keyInput->getKeyState(GLFW_KEY_W) == KeyState::Pressed)
			m_moveDir = { 0, 1 };
		else if (m_keyInput->getKeyState(GLFW_KEY_A) == KeyState::Pressed)
			m_moveDir = { -1, 0 };
		else if (m_keyInput->getKeyState(GLFW_KEY_S) == KeyState::Pressed)
			m_moveDir = { 0, -1 };
		else if (m_keyInput->getKeyState(GLFW_KEY_D) == KeyState::Pressed)
			m_moveDir = { 1, 0 };

		//float frameDelta = Time::getFrameDelta();
		//float panSpd = 10;
		//float rotSpd = 10;
		//Camera *cam = Scene::getActiveScene()->getActiveCamera();

		//if (m_keyInput->getKeyState(GLFW_KEY_Q) == KeyState::Held)
		//	cam->translate(glm::vec3(0.0f, 0.0f, frameDelta * panSpd));

		//if (m_keyInput->getKeyState(GLFW_KEY_E) == KeyState::Held)
		//	cam->translate(glm::vec3(0.0f, 0.0f, -frameDelta * panSpd));

		//if (m_keyInput->getKeyState(GLFW_KEY_J) == KeyState::Held)
		//	cam->rotate(glm::vec3(-frameDelta * rotSpd, 0.0f, 0.0f));

		//if (m_keyInput->getKeyState(GLFW_KEY_L) == KeyState::Held)
		//	cam->rotate(glm::vec3(frameDelta * rotSpd, 0.0f, 0.0f));

		//if (m_keyInput->getKeyState(GLFW_KEY_I) == KeyState::Held)
		//	cam->rotate(glm::vec3(0.0f, frameDelta * rotSpd, 0.0f));

		//if (m_keyInput->getKeyState(GLFW_KEY_K) == KeyState::Held)
		//	cam->rotate(glm::vec3(0.0f, -frameDelta * rotSpd, 0.0f));

		//if (m_keyInput->getKeyState(GLFW_KEY_U) == KeyState::Held)
		//	cam->rotate(glm::vec3(0.0f, 0.0f, frameDelta * rotSpd));

		//if (m_keyInput->getKeyState(GLFW_KEY_O) == KeyState::Held)
		//	cam->rotate(glm::vec3(0.0f, 0.0f, -frameDelta * rotSpd));

		//if (m_keyInput->getKeyState(GLFW_KEY_ESCAPE) == KeyState::Pressed)
		//	Serialisation::saveScene(Scene::getActiveScene(), "hi.scene");
	}


	std::unique_ptr<Application> createApplication()
	{
		return std::make_unique<Player>();
	}
}
