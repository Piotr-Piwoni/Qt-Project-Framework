#pragma once
#include <QDateTime>
#include <QtLogging>

#include "utilities/logging/Logger.hpp"

namespace Chronicle::Utils
{
class QtLoggerAdapter : public ChronicleEngine::Utils::Logging::Logger
{
public:
	static void Init(const std::filesystem::path& filePath)
	{
		Logger::Init(filePath);
		qInstallMessageHandler(QtMessageHandler);
	}


	static void QtMessageHandler(QtMsgType type, const QMessageLogContext& context,
								 const QString& msg)
	{
		thread_local bool inLogger{false};

		std::string message = msg.toStdString();
		if (inLogger)
		{
			std::println("{}", message); //< Fallback.
			return;
		}

		ChronicleEngine::Utils::Logging::LoggerGuard guard(inLogger);
		// Construct message components.
		std::string time = QDateTime::currentDateTime()
						   .toString("yyyy-MM-dd hh:mm:ss.zzz").toStdString();
		std::string file = ExtractFileName(context.file);
		std::string line = std::to_string(context.line);
		std::string function = ExtractFunctionName(context.function);

		LogInternal(ToLoggerMsgType(type), time, file, line, function, message);
	}

	static ChronicleEngine::Utils::Logging::LoggerMsgType ToLoggerMsgType(QtMsgType type)
	{
		using enum ChronicleEngine::Utils::Logging::LoggerMsgType;
		switch (type)
		{
		case QtInfoMsg: return Info;
		case QtDebugMsg: return Debug;
		case QtWarningMsg: return Warning;
		case QtCriticalMsg: return Error;
		case QtFatalMsg: return Fatal;
		}

		// Return "Fatal" on unknown type passed.
		return Fatal;
	}
};
}
