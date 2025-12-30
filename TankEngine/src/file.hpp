#pragma once


namespace Tank
{
	namespace File
	{
		enum class ReadError
		{
			NotFile,
			Error
		};

		TANK_API bool exists(const std::filesystem::path &fp);
		TANK_API std::expected<std::string, ReadError> readLines(const std::filesystem::path &fp) noexcept;
		TANK_API bool writeLines(const std::filesystem::path &fp, const std::string &inStr);
		TANK_API char* readBytes(const std::filesystem::path &fp, int *outSize);
	}
}