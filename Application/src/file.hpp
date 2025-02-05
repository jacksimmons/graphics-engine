#pragma once
#include <fstream>
#include <string>
#include <exception>
#include <filesystem>


namespace Tank
{
	namespace File
	{
		bool readLines(std::filesystem::path fp, std::string &outStr);
		bool writeLines(const std::filesystem::path &fp, const std::string &inStr);
	}
}