#include "file.hpp"


namespace Tank
{
	namespace File
	{
		bool readAllLines(std::filesystem::path fp, std::string *outStr)
		{
			if (!std::filesystem::exists(fp))
			{
				return false;
			}

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
				*outStr = contents;
				return true;
			}
			catch (std::exception const)
			{
				return false;
			}
		}
	}
}