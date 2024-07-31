#include "ArgumentParser.h"
#include <sstream>
ArgumentParser::ArgumentParser(int argc, char* argv[])
{
	for (int i = 1; i = i + 2; i + 1 <= argc)
	{
		std::pair pair(argv[i], argv[i + 1]);
		_options.insert(std::move(pair));
	}
}

std::optional<std::string> ArgumentParser::getValue(const std::string& key) const
{
	if (key == "help")
	{
		auto availableOptions = getAllOptionsWithDesc();
		std::stringstream ss;
		ss << "All available options with descriptions:\n";
		for (const auto& [key, desc] : availableOptions)
			ss << key << " ,desc: " << desc << "\n";
		return ss.str();
	}
	std::optional<std::string> result;
	if (_options.contains(key))
		result = _options.at(key);
	return result;
}

std::unordered_map<std::string, std::string> ArgumentParser::getAllOptionsWithDesc() const
{
	return {};
}
