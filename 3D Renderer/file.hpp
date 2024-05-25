#pragma once
#include <string>
#include <exception>


class File
{
public:
	static std::string readAllLines(std::string filepath);
};


class FileReadException : std::exception {};