#include "Logger.h"

Logger::Logger(const std::string& filename) 
{
	logFile.open(filename, std::ios::app); // Append mode
	log_file_path = filename;
}

Logger::Logger(const std::filesystem::path& filename)
{
	logFile.open(filename.string(), std::ios::app); // Append mode
	log_file_path = filename.string();
}

Logger::~Logger() 
{
	if (logFile.is_open()) 
	{
		logFile.close();
	}
}

void Logger::log(const std::string message, const std::string level) {
	if (logFile.is_open()) {
		std::string message = currentTimestamp() + " - " + level + " - " + message;
		logFile << message << std::endl;
		log_list.push_back(message);
	}
}

std::string Logger::currentTimestamp() {
	auto now = std::chrono::system_clock::now();
	std::time_t timeT = std::chrono::system_clock::to_time_t(now);

	std::tm timeinfo{};
	if (localtime_s(&timeinfo, &timeT) != 0) {
		return "Error getting time";
	}

	std::ostringstream oss;
	oss << std::put_time(&timeinfo, "%Y-%m-%d %H:%M:%S");
	return oss.str();
}