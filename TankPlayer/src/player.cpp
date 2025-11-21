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
#include "../../snake_script.hpp"


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

		// Handle timed movements
		if (m_timeSinceLastMove > m_moveFrequency && m_moveDir != glm::ivec2 { 0, 0 })
		{
			m_timeSinceLastMove = 0;
			auto snakeTransform = m_snake->getTransform();
			snakeTransform->setLocalTranslation(
				snakeTransform->getLocalTranslation() +
				glm::vec3{ m_moveDir.x, m_moveDir.y, 0 }
			);

			TE_CORE_INFO("Snake has moved to " + glm::to_string(m_snake->getTransform()->getLocalTranslation()));
		}

		// Handle fruit spawns (random position in range)	
		if (m_fruits.empty())
		{
			ShaderSources sources;
			sources.vertex.location = "shader.vert";
			sources.fragment.location = "shader.frag";
			Sprite *sprite = new Sprite("Fruit", sources, "fruit.png");
			m_fruits.push_back(sprite);

			// Seed to the current time
			std::srand(std::time({}));
			// Random range x between -4 and 4, y between -3 and 3
			glm::vec3 randomTranslation = glm::vec3{ std::rand() % 8 - 4, std::rand() % 6 - 3, 0 };
			TE_CORE_INFO("Fruit spawned at " + glm::to_string(randomTranslation));
			sprite->getTransform()->setLocalTranslation(randomTranslation);
			m_root->addChild(std::unique_ptr<Sprite>(sprite));
		}

		// Loop over all fruits - if a single fruit overlaps with the snake, destroy it.
		// Exit the loop if any fruit is eaten, to not break iteration.
		for (auto it = m_fruits.begin(); it != m_fruits.end(); ++it)
		{
			if (m_snake->getTransform()->getLocalTranslation() == (*it)->getTransform()->getLocalTranslation())
			{
				(*it)->destroy();
				m_fruits.erase(it);
				m_score++;
				TE_CORE_INFO(std::format("Score has increased to {}", m_score));
				break;
			}
		}

		m_timeSinceLastMove += Time::getFrameDelta();
	}


	void Player::uiStep()
	{
	}


	void Player::handleKeyInput()
	{
		// Update direction based on WASD
		if (m_keyInput->getKeyState(GLFW_KEY_W) == KeyState::Pressed)
			m_moveDir = { 0, 1 };
		else if (m_keyInput->getKeyState(GLFW_KEY_A) == KeyState::Pressed)
			m_moveDir = { -1, 0 };
		else if (m_keyInput->getKeyState(GLFW_KEY_S) == KeyState::Pressed)
			m_moveDir = { 0, -1 };
		else if (m_keyInput->getKeyState(GLFW_KEY_D) == KeyState::Pressed)
			m_moveDir = { 1, 0 };
	}


	std::unique_ptr<Application> createApplication()
	{
		return std::make_unique<Player>();
	}
}
