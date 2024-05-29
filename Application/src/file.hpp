#pragma once
#include <string>
#include <exception>
#include <filesystem>


class File
{
public:
	static std::string readAllLines(std::filesystem::path fp);
};


class InvalidFileException : public std::exception {};


class InaccessibleFileException : public std::exception {};