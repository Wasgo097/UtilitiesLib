#pragma once
#include "json.hpp"
#include <filesystem>
#include <format>
#include <fstream>
#include <optional>
#include <string>
class SettingsParser
{
public:
	SettingsParser(std::string root="");
	template <typename T>
	std::optional<T> getSettings(const std::string& fileName) const
	{
		std::optional<T> result;
		std::string fullPath{ _root + fileName };
		if (!std::filesystem::exists(fullPath))
			throw std::invalid_argument(std::format("File {} doesn't exist",fullPath).c_str());
		std::ifstream file(fullPath);
		try
		{
			nlohmann::json json;
			file >> json;
			T obj;
			obj = json;
			result = obj;
		}
		catch (...)
		{
		}
		file.close();
		return result;
	}
	template <typename T>
	void createSettings(const std::string& fileName, const T& obj) const
	{
		std::string fullPath{ _root + fileName };
		if (!std::filesystem::exists(_root))
			std::filesystem::create_directory(_root);
		std::ofstream file(fullPath);
		try
		{
			nlohmann::json json;
			json = obj;
			file << json.dump(4);
		}
		catch (...)
		{
		}
		file.close();
	}
private:
	std::string _root;
};