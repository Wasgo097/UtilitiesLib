#pragma once
#include <optional>
#include <string>
#include <unordered_map>

class ArgumentParser
{
public:
	ArgumentParser(int argc, char* argv[]);
	std::optional<std::string> getValue(const std::string& key) const;
private:
	virtual std::unordered_map<std::string, std::string> getAllOptionsWithDesc() const;
	std::unordered_map<std::string, std::string> _options;
};