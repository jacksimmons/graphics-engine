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
		bool TANK_API readLines(std::filesystem::path fp, std::string &outStr);
		bool TANK_API writeLines(const std::filesystem::path &fp, const std::string &inStr);
	}
}