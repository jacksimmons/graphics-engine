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
			glm::mat4 M = m_inspectedNode->getModelMatrix();
			Mat4Decomp decomp = Matrix::decompose(M);

			// Name
			ImGui::TextColored(TITLE, "Name");
			ImGui::Text(m_inspectedNode->getName().c_str());

			// Snap To
			if (ImGui::Button("<Snap To>"))
			{
				auto cam = Tank::Scene::getActiveScene()->getActiveCamera();
				cam->setPosition(decomp.translation);
				cam->setRotation(glm::vec3());
			}


			// Allow user to set Model Matrix directly...
			// Model Matrix
			ImGui::TextColored(TITLE, "Model Matrix");

			// Transpose the model matrix so each column is displayed as a row in the UI.
			// Each InputFloat4 now corresponds to a column in the model matrix.
			glm::mat4x4 displayMatrix = glm::transpose(M);
			for (int i = 0; i < 4; i++)
			{
				float col[] = { displayMatrix[i].x, displayMatrix[i].y, displayMatrix[i].z, displayMatrix[i].w };
				std::string id = "##ModelMatrixCol" + std::to_string(i);
				if (ImGui::InputFloat4(id.c_str(), col))
				{
					displayMatrix[i] = glm::vec4(col[0], col[1], col[2], col[3]);
				}
			}
			// Transpose it back and update the model matrix.
			M = glm::transpose(displayMatrix);


			// ...Or configure "Transform" properties.
			// Position
			ImGui::TextColored(TITLE, "Position");
			float pos[] = { M[3].x, M[3].y, M[3].z };
			if (ImGui::InputFloat3("##ModelMatrixPosition", pos))
			{
				M[3] = glm::vec4(pos[0], pos[1], pos[2], 1);
			}

			// Scale
			ImGui::TextColored(TITLE, "Scale");
			float scale[] = { M[0].x, M[1].y, M[2].z };
			if (ImGui::InputFloat3("##ModelMatrixScale", scale))
			{
				M[0].x = scale[0];
				M[1].y = scale[1];
				M[2].z = scale[2];
			}

			// Rotation
			ImGui::TextColored(TITLE, "Rotation");

			//glm::vec3 rot = glm::eulerAngles(decomp.orientation);
			//float rotation[] = { rot.x, rot.y, rot.z };
			//if (ImGui::InputFloat3("##ModelMatrixRotation", scale))
			//{
			//	// Rotate M by the difference in each component, about said component's axis.
			//	M = glm::rotate(M, rotation[0] - rot.x, glm::vec3(1, 0, 0));
			//	M = glm::rotate(M, rotation[1] - rot.y, glm::vec3(0, 1, 0));
			//	M = glm::rotate(M, rotation[2] - rot.z, glm::vec3(0, 0, 1));
			//}

			m_inspectedNode->setModelMatrix(M);

			
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