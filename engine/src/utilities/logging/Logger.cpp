#include "Logger.hpp"

namespace ChronicleEngine::Utils::Logging
{
void Logger::Init(const std::filesystem::path& filePath)
{
	if (m_Initialised) return;
	if (m_LogFile.is_open()) m_LogFile.close();

	if (!std::filesystem::exists(filePath))
	{
		std::filesystem::create_directories(filePath.parent_path());
		m_LogFile.open(filePath, std::ios_base::app);
	}
	else m_LogFile.open(filePath, std::ios_base::app);

	if (!m_LogFile.is_open())
		Log(LoggerMsgType::Error, "Logger failed to open log file! "
			"Logging to files will be disabled! [\"{}\"]",
			std::source_location::current(), filePath.string());

	m_Initialised = true;
}

void Logger::Shutdown()
{
	m_Initialised = false;
	m_LogFile.flush();
	if (m_LogFile.is_open()) m_LogFile.close();
}


void Logger::LogInternal(LoggerMsgType level, std::string& time, std::string& file,
						 std::string& line, std::string& function, std::string& msg)
{
	std::lock_guard locker{m_LogMutex};

	const Colour kTimestampColour{"#369DF6"};
	const Colour kNumberColour{"#5CD2FA"};
	const Colour kFunctionColour{"#FFBF67"};

	// Colourise parts of the message.
	time = ColouriseNumbers(time, kTimestampColour);
	line = ColouriseNumbers(line, kNumberColour);
	function = ColouriseString(function, kFunctionColour);
	msg = ColouriseNumbers(msg, kNumberColour);

	// Construct the final message based on the severity level.
	Colour levelColour{};
	std::string finalMessage{};
	switch (level)
	{
	case LoggerMsgType::Info:
		finalMessage = std::format("{} [{}] \"{}\"",
								   time,
								   ColouriseString(ToString(level), levelColour),
								   msg);
		break;
	case LoggerMsgType::Debug:
		levelColour = Colour::FromHex("#B257FF");
		finalMessage = std::format("{} [{}] <{}:{}> \"{}\"",
								   time,
								   ColouriseString(ToString(level), levelColour),
								   file,
								   line,
								   msg);
		break;
	case LoggerMsgType::Warning:
		levelColour = Colour::FromHex("#ECA64C");
		finalMessage = std::format("{} [{}] <{}:{} {}()> \"{}\"",
								   time,
								   ColouriseString(ToString(level), levelColour),
								   file,
								   line,
								   function,
								   msg);
		break;
	case LoggerMsgType::Error:
		levelColour = Colour::FromHex("#FF3F4A");
		finalMessage = std::format("{} [{}] <{}:{} {}()> \"{}\"",
								   time,
								   ColouriseString(ToString(level), levelColour),
								   file,
								   line,
								   function,
								   msg);
		break;
	case LoggerMsgType::Fatal:
		levelColour = Colour::FromHex("#A7A7A7");
		finalMessage = std::format("{} [{}] <{}:{} {}()> \"{}\"",
								   time,
								   ColouriseString(ToString(level), levelColour),
								   file,
								   line,
								   function,
								   msg);
		break;
	}


	// Output to both the terminal and the log file.
	std::cout << finalMessage << '\n';

	if (!m_LogFile.is_open()) return;
	m_LogFile << StripAnsi(finalMessage) << "\n";
}


std::string Logger::ExtractFileName(const char* file)
{
	if (!file) return {};

	// Normalise slashes for cross-platform safety.
	std::string name = file;
	std::ranges::replace(name, '\\', '/');

	// Take only last path segment.
	int lastSlash = static_cast<int>(name.rfind('/'));
	if (lastSlash != -1) name = name.substr(lastSlash + 1);

	return RemoveWhiteSpaces(name);
}

std::string Logger::ExtractFunctionName(const char* function)
{
	if (!function) return {};

	// Remove parameter list.
	std::string name = function;
	auto bracket = name.find('(');
	if (bracket != std::string::npos) name = name.substr(0, bracket);

	// Take only last scope segment (after last ::).
	auto scope = name.rfind("::");
	if (scope != std::string::npos) name = name.substr(scope + 2);

	return RemoveWhiteSpaces(name);
}


std::string Logger::AnsiColour(const Colour& colour)
{
	return std::format("\033[38;2;{};{};{}m", colour.R, colour.G, colour.B);
}

std::string Logger::StripAnsi(const std::string& text)
{
	std::string result;
	result.reserve(text.size());

	bool inEscape = false;
	for (char c : text)
	{
		if (c == '\x1B') inEscape = true;
		if (!inEscape) result += c;
		if (inEscape && c >= 'A' && c <= 'z') inEscape = false;
	}

	return result;
}

std::string Logger::ColouriseNumbers(const std::string& text, const Colour& colour)
{
	std::string result{};
	result.reserve(text.size() * 2);

	bool inNumber = false;
	for (const char& character : text)
	{
		if (std::isdigit(static_cast<unsigned char>(character)))
		{
			if (!inNumber)
			{
				result += AnsiColour(colour);
				inNumber = true;
			}
			result += character;
		}
		else
		{
			if (inNumber)
			{
				result += AnsiReset();
				inNumber = false;
			}
			result += character;
		}
	}

	if (inNumber) result += AnsiReset();
	return result;
}

std::string Logger::ColouriseString(const std::string& text, const Colour& colour)
{
	return AnsiColour(colour) + text + AnsiReset();
}
}
