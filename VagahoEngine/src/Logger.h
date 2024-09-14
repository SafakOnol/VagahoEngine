#pragma once

#define LOG_INFO(msg) Logger::Log(msg)
#define LOG_WARNING(msg) Logger::Wrn(msg)
#define LOG_ERROR(msg) Logger::Err(msg, __FILE__, __LINE__)

#include <string>

class Logger {
private:
	static std::string GetCurrentDateTime();
	static std::string severity;
	static std::string timestamp;
public:
	static void Log(const std::string& message);
	static void Wrn(const std::string& message);
	static void Err(const std::string& message, const char* file, int line);
};

