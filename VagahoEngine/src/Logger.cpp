#include "Logger.h"

#include <iostream>
#include <chrono>	// current time
#include <iomanip>	// time formatting
#include <ctime>	// std::localtime
#include <sstream>	// std::ostringstream
#include <filesystem> // extract file name (C++17)

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
	std::cout	<< "\033[92m" 
				<< "[LOG    ]" << "[" << GetCurrentDateTime() << "]: " 
				<< message << "\033[0m" << std::endl;
	return;
}

void Logger::Wrn(const std::string& message) {
	std::cout	<< "\033[33m" 
				<< "[WARNING]" << "[" << GetCurrentDateTime() << "]: " 
				<< message << "\033[0m" << std::endl;
				
	return;
}

void Logger::Err(const std::string& message, const char* file, int line) {
	std::string filename = std::filesystem::path(file).filename().string();
	std::cout	<< "\033[31m" 
				<< "[ERROR  ]" << "[" << GetCurrentDateTime() << "]:" 
				<< "\033[90m"
				<< "[File: " << filename << ", Line: " << line << "]: "
				<< "\033[31m"
				<< message << "\033[0m" << std::endl;
				
	return;
}
