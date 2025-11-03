#pragma once
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <vector>

class Logger {
public:
	// Variable: To hold list of all logs written under current instance
	std::vector<std::string> log_list{};
	// Method: Class constructor used to create an instance of logger that logs events or messages.
	Logger(const std::string& filename);
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
