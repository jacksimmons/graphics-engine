#include <memory>
#include <string>
#include <filesystem>
#include <imgui.h>
#include <glm/gtx/euler_angles.hpp>
#include "transform.hpp"
#include "camera.hpp"
#include "scene.hpp"
#include "nodes/node.hpp"
#include "nodes/model.hpp"
#include "nodes/inspector.hpp"
#include "colours.hpp"
#include "file.hpp"


namespace Tank
{
	Inspector::Inspector(std::string name) : Node(name)
	{
		m_inspectedNode = nullptr;
	}

	void Inspector::drawInspector() const
	{
		ImGui::Begin("Inspector");
		
		if (m_inspectedNode)
		{
			Transform *transform = m_inspectedNode->getTransform();
			const glm::mat4 &modelMatrix = transform->getModelMatrix();

			ImGui::TextColored(TITLE, "Name");
			ImGui::Text(m_inspectedNode->getName().c_str());

			if (ImGui::Button("<Snap To>"))
			{
				auto cam = Tank::Scene::getActiveScene()->getActiveCamera();
				if (cam != nullptr)
				{
					cam->setPosition(transform->getTranslation());
					cam->setRotation(transform->getRotation());
				}
			}

			ImGui::TextColored(TITLE, "Model Matrix");
			// glm::mat4 indexing is column-major, but ImGui is row-major.
			// Transposing the model means an ImGui row corresponds to a model matrix row.
			glm::mat4 displayMatrix = glm::transpose(modelMatrix);
			for (int i = 0; i < 4; i++)
			{
				glm::vec4 row = displayMatrix[i];
				std::string rowText = std::to_string(row.x) + "\t" +
									std::to_string(row.y) + "\t" +
									std::to_string(row.z) + "\t" +
									std::to_string(row.w);
				ImGui::Text(rowText.c_str());
			}

			ImGui::TextColored(TITLE, "Translation");
			glm::vec3 trans = transform->getTranslation();
			float transInp[] = { trans.x, trans.y, trans.z };
			if (ImGui::InputFloat3("##Transform_Translation", transInp))
			{
				glm::vec3 newTrans = { transInp[0], transInp[1], transInp[2] };
				transform->setTranslation(newTrans);
			}

			ImGui::TextColored(TITLE, "Scale");
			glm::vec3 scale = transform->getScale();
			float scaleInp[] = { scale.x, scale.y, scale.z };
			if (ImGui::InputFloat3("##Transform_Scale", scaleInp))
			{
				glm::vec3 newScale = { scaleInp[0], scaleInp[1], scaleInp[2] };
				transform->setScale(newScale);
			}

			ImGui::TextColored(TITLE, "Rotation (EulerAngles)");
			glm::vec3 rot = glm::eulerAngles(transform->getRotation());
			float rotInp[] = { rot.x, rot.y, rot.z };
			if (ImGui::InputFloat3("##Transform_Rotation", rotInp))
			{
				glm::vec3 newRotEuler = { rotInp[0], rotInp[1], rotInp[2] };
				glm::quat newRot = Quat::fromAngleAxis(newRotEuler);
				transform->setRotation(newRot);
			}

			// Set to nullptr if cast fails (so if statement is not entered).
			// Shader Files (if Node is Model)
			if (Model *model = dynamic_cast<Model*>(m_inspectedNode))
			{
				std::filesystem::path fragPath = model->m_shader->getFragPath();
				std::filesystem::path vertPath = model->m_shader->getVertPath();

				ImGui::TextColored(TITLE, "Vertex Shader");
				std::string vshader;
				if (File::readAllLines("src/shaders" / vertPath, &vshader))
				{
					ImGui::Text(vshader.c_str());
				}

				ImGui::TextColored(TITLE, "Fragment Shader");
				std::string fshader;
				if (File::readAllLines("src/shaders" / fragPath, &fshader))
				{
					ImGui::Text(fshader.c_str());
				}
			}
		}

		ImGui::End();
	}

	void Inspector::draw() const
	{
		drawInspector();
		Node::draw();
	}

	void Inspector::onNodeDeleted(Node *node)
	{
		int childCount = node->getChildCount();
		for (int i = 0; i < childCount; i++)
		{
			onNodeDeleted(node->getChild(i));
		}

		if (node == m_inspectedNode)
		{
			m_inspectedNode = nullptr;
		}
	}
}