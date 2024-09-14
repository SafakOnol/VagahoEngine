#include "Logger.h"

#include <iostream>
#include <chrono>	// current time
#include <iomanip>	// time formatting
#include <ctime>	// std::localtime
#include <sstream>	// std::ostringstream
#include <filesystem> // extract file name (C++17)

// Initialize variables
std::vector<LogEntry> Logger::messages;

std::string Logger::GetCurrentDateTime(){
	auto now = std::chrono::system_clock::now();
	std::time_t now_time = std::chrono::system_clock::to_time_t(now);
	std::tm local_time;
	localtime_s(&local_time, &now_time);

	std::ostringstream oss;
	oss << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S");
	return oss.str();
}

void Logger::Log(const std::string& message) {
	LogEntry logEntry;
	logEntry.type = INFO;
	logEntry.message = "[INFO   ][" + GetCurrentDateTime() + "]: " + message;
	
	std::cout << "\033[92m" << logEntry.message << "\033[0m" << std::endl;

	messages.push_back(logEntry);
	return;
}

void Logger::Wrn(const std::string& message) {
	LogEntry logEntry;
	logEntry.type = WARNING;
	logEntry.message = "[WARNING][" + GetCurrentDateTime() + "]: " + message;

	std::cout	<< "\033[33m" << logEntry.message << "\033[0m" << std::endl;
	
	messages.push_back(logEntry);
	return;
}

void Logger::Err(const std::string& message, const char* file, int line) {
	std::string filename = std::filesystem::path(file).filename().string();
	std::string line_str = std::to_string(line);

	LogEntry logEntry;
	logEntry.type = ERROR;
	logEntry.message =	"[ERROR  ][" + GetCurrentDateTime() + "]:" + "\033[90m" + 
						"[File: " + filename + ", Line: " + line_str + "]: "
						+ "\033[31m" + message;

	std::cout	<< "\033[31m" << logEntry.message << "\033[0m" << std::endl;			
	
	messages.push_back(logEntry);
	return;
}
