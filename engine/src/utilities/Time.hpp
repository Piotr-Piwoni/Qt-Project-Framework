#pragma once
#include <chrono>
#include <string>

namespace ChronicleEngine::Utils
{
inline std::string GetTime()
{
	auto now = std::chrono::system_clock::now();
	auto seconds = std::chrono::floor<std::chrono::seconds>(now);
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - seconds);
	return std::format("{:%Y-%m-%d %H:%M:%S}.{:03}", seconds, ms.count());
}
}
