#include <memory>
#include <string>
#include <filesystem>
#include <imgui.h>
#include <glm/gtx/euler_angles.hpp>
#include "transform.hpp"
#include "camera.hpp"
#include "scene.hpp"
#include "colours.hpp"
#include "file.hpp"
#include "widget.hpp"
#include "nodes/ui.hpp"
#include "nodes/model.hpp"
#include "nodes/inspector.hpp"
#include "nodes/light.hpp"


namespace Tank
{
	Inspector::Inspector(const std::string &name) : Panel(name)
	{
		m_inspectedNode = nullptr;
	}


	void Inspector::drawPanel()
	{
		if (m_inspectedNode)
		{
			ImGui::TextColored(Colour::TITLE, "Type");
			ImGui::Text(typeid(*m_inspectedNode).name());

			drawNodeSection();

			// Draw Node subclass sections
			if (Model *model = dynamic_cast<Model *>(m_inspectedNode))
				drawModelSection(model);

			if (Camera *camera = dynamic_cast<Camera *>(m_inspectedNode))
				drawCameraSection(camera);

			if (Light *light = dynamic_cast<Light *>(m_inspectedNode))
				drawLightSection(light);
		}	
	}

	
	/// <summary>
	/// Draws inspector section that is present for all Nodes.
	/// </summary>
	void Inspector::drawNodeSection() const
	{
		Transform *transform = m_inspectedNode->getTransform();
		const glm::mat4 &modelMatrix = transform->getWorldMatrix();

		bool enabled = m_inspectedNode->getEnabled();
		if (ImGui::Checkbox("Enabled", &enabled))
			m_inspectedNode->setEnabled(enabled);

		bool visible = m_inspectedNode->getVisibility();
		if (ImGui::Checkbox("Visible", &visible))
			m_inspectedNode->setVisibility(visible);

		ImGui::TextColored(Colour::TITLE, "Name");
		{
			const size_t size = 100;
			char buf[size] = "\0";
			if (ImGui::InputTextWithHint("##Inspector_Name", m_inspectedNode->getName().c_str(), buf, size))
			{
				m_inspectedNode->setName(std::string(buf));
			}
		}

		ImGui::TextColored(Colour::TITLE, "Scripts");
		size_t scriptCount = m_inspectedNode->getScriptCount();
		for (int i = 0; i < scriptCount; i++)
		{
			ImGui::Text(typeid(m_inspectedNode->getScript(i)).name());
		}
		if (scriptCount == 0)
		{
			ImGui::Text("None");
		}

		if (ImGui::Button("<Snap To>"))
		{
			auto cam = Tank::Scene::getActiveScene()->getActiveCamera();
			if (cam != nullptr)
			{
				cam->setPosition(transform->getTranslation());
				cam->setRotation(transform->getRotation());
			}
		}

		ImGui::TextColored(Colour::TITLE, "Model Matrix");
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

		ImGui::TextColored(Colour::TITLE, "Translation");
		Widget::vec3Input(
			"##Inspector_Translation",
			transform->getTranslation(),
			[&transform](glm::vec3 newTranslation)
			{
				transform->setTranslation(newTranslation);
			}
		);

		ImGui::TextColored(Colour::TITLE, "Scale");
		Widget::vec3Input(
			"##Inspector_Scale",
			transform->getScale(),
			[&transform](glm::vec3 newScale)
			{
				transform->setScale(newScale);
			}
		);

		ImGui::TextColored(Colour::TITLE, "Rotation (Euler Angles)");
		Widget::vec3Input(
			"##Inspector_Rotation_EulerAngles",
			glm::eulerAngles(transform->getRotation()),
			[&transform](glm::vec3 newRotation)
			{
				transform->setRotation(Quat::fromAngleAxis(newRotation));
			}
		);
	}


	/// <summary>
	/// Draws inspector section that is present for all Models.
	/// </summary>
	void Inspector::drawModelSection(Model *model) const
	{
		std::filesystem::path fragPath = model->m_shader->getFragPath();
		std::filesystem::path vertPath = model->m_shader->getVertPath();

		ImGui::TextColored(Colour::TITLE, "Vertex Shader");
		std::string vshader;
		if (File::readAllLines("shaders" / vertPath, &vshader))
		{
			// https://github.com/ocornut/imgui/issues/2429
			ImGui::TextUnformatted(vshader.c_str());
		}

		ImGui::TextColored(Colour::TITLE, "Fragment Shader");
		std::string fshader;
		if (File::readAllLines("shaders" / fragPath, &fshader))
		{
			// https://github.com/ocornut/imgui/issues/2429
			ImGui::TextUnformatted(fshader.c_str());
		}
	}


	/// <summary>
	/// Draws inspector section that is present for all Cameras.
	/// </summary>
	void Inspector::drawCameraSection(Camera *camera) const
	{
		ImGui::TextColored(Colour::TITLE, "Camera Eye");
		glm::vec3 eye = camera->getTransformedEye();
		ImGui::Text(glm::to_string(eye).c_str());

		ImGui::TextColored(Colour::TITLE, "Camera Centre");
		glm::vec3 centre = camera->getTransformedCentre();
		ImGui::Text(glm::to_string(centre).c_str());

		ImGui::TextColored(Colour::TITLE, "Camera Up");
		glm::vec3 up = camera->getTransformedUp();
		ImGui::Text(glm::to_string(up).c_str());

		ImGui::TextColored(Colour::TITLE, "Camera Pan Speed");
		float panSpd = camera->getPanSpeed();
		if (ImGui::InputFloat("##Inspector_Camera_PanSpd", &panSpd))
		{
			camera->setPanSpeed(panSpd);
		}

		ImGui::TextColored(Colour::TITLE, "Camera Rotation Speed");
		float rotSpd = camera->getRotSpeed();
		if (ImGui::InputFloat("##Inspector_Camera_RotSpd", &rotSpd))
		{
			camera->setRotSpeed(rotSpd);
		}
	}


	void Inspector::drawLightSection(Light *light) const
	{
		ImGui::TextColored(Colour::TITLE, "Light Struct");
		std::string lightStruct = light->getLightStruct();
		ImGui::Text(lightStruct.c_str());

		ImGui::TextColored(Colour::TITLE, "Ambient Intensity (RGB)");
		Widget::vec3Input(
			"##Inspector_Light_Ambient",
			light->getAmbient(),
			[&light](glm::vec3 newAmbient)
			{
				light->setAmbient(newAmbient);
			}
		);
		//ImGui::Text(glm::to_string(light->getAmbient()).c_str());

		ImGui::TextColored(Colour::TITLE, "Diffuse Intensity (RGB)");
		Widget::vec3Input(
			"##Inspector_Light_Diffuse",
			light->getDiffuse(),
			[&light](glm::vec3 newDiffuse)
			{
				light->setDiffuse(newDiffuse);
			}
		);
		//ImGui::Text(glm::to_string(light->getDiffuse()).c_str());

		ImGui::TextColored(Colour::TITLE, "Specular Intensity (RGB)");
		Widget::vec3Input(
			"##Inspector_Light_Specular",
			light->getSpecular(),
			[&light](glm::vec3 newSpecular)
			{
				light->setSpecular(newSpecular);
			}
		);
		//ImGui::Text(glm::to_string(light->getSpecular()).c_str());

		// Draw Light subclass sections
		if (DirLight *dir = dynamic_cast<DirLight *>(light))
		{
			drawDirLightSection(dir);
		}
	}


	void Inspector::drawDirLightSection(DirLight *dir) const
	{
		ImGui::TextColored(Colour::TITLE, "Light Direction");
		Widget::vec3Input(
			"##Inspector_DirLight_Direction",
			dir->getDirection(),
			[&dir](glm::vec3 newDirection)
			{
				dir->setDirection(newDirection);
			}
		);
	}


	/// <summary>
	/// Recurse over all descendants of node, and if any match to the inspected
	/// node, set the inspected node to nullptr (to reflect the deletion).
	/// </summary>
	void Inspector::onNodeDeleted(Node *node)
	{
		node->forEachDescendant(
			[this](Node *node)
			{
				if (node == m_inspectedNode)
					m_inspectedNode = nullptr;
			},
			[this]()
			{
				return m_inspectedNode == nullptr;
			}
		);
	}
}