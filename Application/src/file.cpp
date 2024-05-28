#include <fstream>
#include <string>

#include "file.hpp"


std::string File::readAllLines(std::string fp)
{
	try
	{
		std::ifstream in(fp);
		std::string contents;
		std::string line;
		while (std::getline(in, line))
		{
			contents.append(line);
			contents.push_back('\n');
		}
		return contents;
	}
	catch (std::exception const)
	{
		throw FileReadException();
	}
}


//std::string readFileString(char *filepath)
//{
//	
//
//	return contents;
//
//	FILE *file = fopen_s(filepath, "r");
//	char *buffer = nullptr;
//
//	// Seek the file pointer to the end.
//	if (file == nullptr || fseek(file, 0, SEEK_END))
//	{
//		return nullptr;
//	}
//
//	// Get file pointer pos, and therefore file length.
//	// If len < 0, an error occurred.
//	long length = ftell(file);
//	if (length == -1 || length >= SIZE_MAX)
//	{
//		fclose(file);
//		return nullptr;
//	}
//
//	// long -> long long
//	size_t ulength = (size_t)length;
//	buffer = (char *)malloc(ulength + 1);
//
//	// If previous mallocation did not work...
//	if (buffer == nullptr)
//	{
//		fclose(file);
//		return nullptr;
//	}
//
//	// Read the file into buffer. First need to seek the file ptr
//	// back to the start with rewind.
//	rewind(file);
//
//	// If fread read positive number of characters...
//	size_t readChars = fread(buffer, 1, ulength, file);
//	if (readChars > 0)
//	{
//		// Null-termination at the number of chars read.
//		buffer[readChars] = '\0';
//	}
//	else
//	{
//		fclose(file);
//		return nullptr;
//	}
//
//	return buffer;
//}