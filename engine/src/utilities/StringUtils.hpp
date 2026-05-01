#pragma once
#include <regex>
#include <string>

namespace ChronicleEngine::Utils
{
inline std::string RemoveWhiteSpaces(const std::string& text)
{
	static const std::regex kWhiteSpaceRegex{R"(\s+)"};
	return std::regex_replace(text, kWhiteSpaceRegex, "");
}
}
