#pragma once
#include "SAGE_Engine.h"

namespace CL_Interface
{
	std::pair<bool, std::string> parse(std::string input_command);
	void welcome_message();
	std::string about();
	std::string license();
	std::string help();
	std::string clear();
}