#include <variant>
#include <nlohmann/json.hpp>


namespace Tank
{
	using json = nlohmann::json;


	typedef std::variant<int, bool, int, float, char[]> primitive_t;
}