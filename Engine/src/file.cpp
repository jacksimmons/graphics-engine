#include "file.hpp"


namespace Tank
{
	namespace File
	{
		bool exists(std::filesystem::path fp)
		{
			return std::filesystem::exists(fp);
		}


		bool readLines(std::filesystem::path fp, std::string &outStr)
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
				outStr = contents;
				return true;
			}
			catch (std::exception const)
			{
				return false;
			}
		}


		bool writeLines(const std::filesystem::path &fp, const std::string &inStr)
		{
			try
			{
				std::ofstream out(fp);
				out.write(inStr.c_str(), inStr.size());
				return true;
			}
			catch (std::exception const)
			{
				return false;
			}
		}
	}
}