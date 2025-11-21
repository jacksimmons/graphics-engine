#pragma once
#include "engine.hpp"


namespace Tank
{
	class Node;
	class Scene;
	class KeyInput;

	class Player : public Application
	{
	private:
		std::unique_ptr<Node> m_root;
		Node *m_snake;
		glm::ivec2 m_moveDir = { 0, 0 };
		float m_moveFrequency = 0.5f;
		float m_timeSinceLastMove = 0;
	public:
		Player();
		~Player();
		void loadScene();
	protected:
		virtual void step() override;
		virtual void handleKeyInput() override;
	};
}