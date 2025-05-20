#pragma once



namespace Tank
{
	class Widget
	{
	public:
		static void textInput(const char *label, const std::string &hint, std::function<void(const std::string&)> onModified, const std::string &contents = "");
		static void floatInput(const char *label, float current, std::function<void(float)> onModified);
		static void vec3Input(const char *label, const glm::vec3 &current, std::function<void(const glm::vec3&)> onModified);
	};
}