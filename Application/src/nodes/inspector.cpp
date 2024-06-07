#include <memory>
#include <string>
#include <filesystem>
#include <imgui.h>
#include "matrix.hpp"
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
		
		if (m_inspectedNode != nullptr)
		{
			// Name
			ImGui::TextColored(TITLE, "Name");
			ImGui::Text(m_inspectedNode->getName().c_str());

			// Snap To
			if (ImGui::Button("<Snap To>"))
			{
				auto cam = Tank::Scene::getActiveScene()->getActiveCamera();
				cam->setPosition(Matrix::getPosition(m_inspectedNode->getModelMatrix()));
				cam->setRotation(glm::vec3());
			}

			// Model Matrix
			ImGui::TextColored(TITLE, "Model Matrix");

			// Transpose the model matrix so each column is displayed as a row in the UI.
			// Each InputFloat4 now corresponds to a column in the model matrix.
			glm::mat4x4 displayMatrix = glm::transpose(m_inspectedNode->getModelMatrix());
			for (int i = 0; i < 4; i++)
			{
				float col[] = {displayMatrix[i].x,displayMatrix[i].y,displayMatrix[i].z,displayMatrix[i].w};
				std::string id = "##ModelMatrixCol" + std::to_string(i);
				if (ImGui::InputFloat4(id.c_str(), col))
				{
					displayMatrix[i] = glm::vec4(col[0], col[1], col[2], col[3]);
				}
			}
			// Transpose it back and update the model matrix.
			m_inspectedNode->setModelMatrix(glm::transpose(displayMatrix));
			
			// Set to nullptr if cast fails (so if statement is not entered).
			// Shader Files (if Node is Model)
			if (std::shared_ptr<Model> model = std::dynamic_pointer_cast<Model>(m_inspectedNode))
			{
				std::filesystem::path fragPath = model->m_shader->getFragPath();
				std::filesystem::path vertPath = model->m_shader->getVertPath();

				ImGui::TextColored(TITLE, "Vertex Shader");
				ImGui::Text(File::readAllLines(vertPath).c_str());

				ImGui::TextColored(TITLE, "Fragment Shader");
				ImGui::Text(File::readAllLines(fragPath).c_str());
			}
		}

		ImGui::End();
	}


	void Inspector::draw() const
	{
		drawInspector();
		Node::draw();
	}
}