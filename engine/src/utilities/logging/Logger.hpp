#pragma once
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <mutex>
#include <print>
#include <regex>
#include <source_location>
#include <string>

#include "LoggingTypes.hpp"
#include "utilities/Colour.hpp"
#include "utilities/StringUtils.hpp"
#include "utilities/Time.hpp"

namespace ChronicleEngine::Utils::Logging
{
class Logger
{
public:
	~Logger() { Shutdown(); }

	static void Init(const std::filesystem::path& filePath);
	static void Shutdown();

	template<class... Args>
	static void Log(LoggerMsgType level, const std::string& pattern,
					std::source_location location, Args&&... args);

protected:
	Logger() = default;


	static void LogInternal(LoggerMsgType level, std::string& time,
							std::string& file, std::string& line,
							std::string& function, std::string& msg);

	static std::string ExtractFileName(const char* file);
	static std::string ExtractFunctionName(const char* function);

	static std::string AnsiColour(const Colour& colour);
	static std::string AnsiReset() { return "\033[0m"; }
	static std::string StripAnsi(const std::string& text);
	static std::string ColouriseNumbers(const std::string& text, const Colour& colour);
	static std::string ColouriseString(const std::string& text, const Colour& colour);

protected:
	inline static std::ofstream m_LogFile{};
	inline static std::mutex m_LogMutex{};
	inline static bool m_Initialised{false};
};


template<class... Args>
void Logger::Log(const LoggerMsgType level, const std::string& pattern,
				 const std::source_location location, Args&&... args)
{
	thread_local bool inLogger{false};

	std::string message = std::vformat(pattern, std::make_format_args(args...));
	if (inLogger)
	{
		std::println("{}", message); // Fallback.
		return;
	}

	LoggerGuard guard(inLogger);
	// Construct message components.
	std::string time = GetTime();
	std::string file = ExtractFileName(location.file_name());
	std::string line = std::to_string(location.line());
	std::string function = ExtractFunctionName(location.function_name());

	LogInternal(level, time, file, line, function, message);
}


#define lInfo(pattern, ...) \
ChronicleEngine::Utils::Logging::Logger::Log( \
ChronicleEngine::Utils::Logging::LoggerMsgType::Info, \
pattern, \
std::source_location::current(), \
##__VA_ARGS__)

#define lDebug(pattern, ...) \
ChronicleEngine::Utils::Logging::Logger::Log( \
ChronicleEngine::Utils::Logging::LoggerMsgType::Debug, \
pattern, \
std::source_location::current(), \
##__VA_ARGS__)

#define lWarn(pattern, ...) \
ChronicleEngine::Utils::Logging::Logger::Log( \
ChronicleEngine::Utils::Logging::LoggerMsgType::Warning, \
pattern, \
std::source_location::current(), \
##__VA_ARGS__)

#define lError(pattern, ...) \
ChronicleEngine::Utils::Logging::Logger::Log( \
ChronicleEngine::Utils::Logging::LoggerMsgType::Error, \
pattern, \
std::source_location::current(), \
##__VA_ARGS__)

#define lFatal(pattern, ...) \
ChronicleEngine::Utils::Logging::Logger::Log( \
ChronicleEngine::Utils::Logging::LoggerMsgType::Fatal, \
pattern, \
std::source_location::current(), \
##__VA_ARGS__)
}
