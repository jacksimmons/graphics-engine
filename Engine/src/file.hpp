#pragma once
#include <fstream>
#include <string>
#include <exception>
#include <filesystem>
#include "core.hpp"


namespace Tank
{
	namespace File
	{
		TANK_API bool readLines(std::filesystem::path fp, std::string &outStr);
		TANK_API bool writeLines(const std::filesystem::path &fp, const std::string &inStr);
	}
}