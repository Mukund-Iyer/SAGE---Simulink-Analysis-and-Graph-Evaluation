#pragma once
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <iostream>

class Logger {
public:
	// Variable: To hold list of all logs written under current instance
	std::vector<std::string> log_list{};
	// Variable: To hold the file path of the log for display or information to user
	std::string log_file_path{};
	// Method: Class constructor used to create an instance of logger that logs events or messages.
	Logger(const std::string& filename);
	// Method: Class constructor used to create instance of logger using std::filesystem::path data type
	Logger(const std::filesystem::path& filename);
	// Method: Actual command used to log events or messages.
	void log(const std::string message, const std::string level);
	// Method: Class Destructor used for garbage collection.
	~Logger();
private:
	// Variable: To hold file for writing log
	std::ofstream logFile; 
	// Method: To get current time for timestamp
	std::string currentTimestamp();
};