#pragma once
#include "SAGE_Engine.h"
#include <algorithm>
#include <regex>

namespace CL_Interface
{
	std::vector<std::string> split_regex(const std::string& input, const std::string& delimiter_pattern);
	std::pair<bool, std::string> parse(std::string input_command);
	void welcome_message();
	std::string about();
	std::string license();
	std::string help();
	std::string clear();
	std::pair<bool, std::string> compare(std::string command_input);
}