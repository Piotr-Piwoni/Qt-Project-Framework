#pragma once
#include <cstdint>

namespace ChronicleEngine::Utils::Logging
{
enum class LoggerMsgType : std::uint8_t
{
	Info = 0,
	Debug,
	Warning,
	Error,
	Fatal
};

inline const char* ToString(const LoggerMsgType type)
{
	switch (type)
	{
	case LoggerMsgType::Info: return "INFO";
	case LoggerMsgType::Debug: return "DEBUG";
	case LoggerMsgType::Warning: return "WARNING";
	case LoggerMsgType::Error: return "ERROR";
	case LoggerMsgType::Fatal: return "FATAL";
	default: return "unknown";
	}
}

struct LoggerGuard
{
	bool& flag;
	explicit LoggerGuard(bool& f) : flag(f) { flag = true; }
	~LoggerGuard() { flag = false; }
};
}
