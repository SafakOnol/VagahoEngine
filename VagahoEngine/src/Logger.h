#pragma once

#define LOG_INFO(msg) Logger::Log(msg)
#define LOG_WARNING(msg) Logger::Wrn(msg)
#define LOG_ERROR(msg) Logger::Err(msg, __FILE__, __LINE__)

#include <string>
#include <vector>

enum LogType {
	INFO,
	WARNING,
	ERROR
};

struct LogEntry {
	LogType type;
	std::string message;
};

class Logger {
private:
	static std::string GetCurrentDateTime();
public:
	static void Log(const std::string& message);
	static void Wrn(const std::string& message);
	static void Err(const std::string& message, const char* file, int line);

	static std::vector<LogEntry> messages;
};

