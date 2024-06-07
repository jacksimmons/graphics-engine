#pragma once
#include <fstream>
#include <string>
#include <exception>
#include <filesystem>


namespace Tank
{
	namespace File
	{
		bool readAllLines(std::filesystem::path fp, std::string *outStr);
	}
}