#include <imgui.h>


namespace Tank
{
	class Colour
	{
	public:
		static constexpr ImColor NORMAL = ImColor(255, 255, 255);
		static constexpr ImColor TITLE = ImColor(0, 100, 255);
		static constexpr ImColor DISABLED = ImColor(128, 128, 128);
	};
}