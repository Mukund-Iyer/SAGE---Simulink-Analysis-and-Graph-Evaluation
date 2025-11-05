#include "CLI.h"


std::vector<std::string> CLI::split_regex(const std::string& input, const std::string& delimiter_pattern) {
	std::regex re(delimiter_pattern);
	std::sregex_token_iterator it(input.begin(), input.end(), re, -1);
	std::sregex_token_iterator end;
	return { it, end };
}

std::pair<bool, std::string> CLI::parse(std::string input_command)
{
	auto split_command = split_regex(input_command, " ");
	std::string command = split_command[0];

	if (command == "exit")
	{
		return std::pair<bool, std::string> {false, "Goodbye!"};
	}
	else if (command == "about")
	{
		return std::pair<bool, std::string> {true, about()};
	}
	else if (command == "license")
	{
		return std::pair<bool, std::string> {true, license()};
	}
	else if (command == "help")
	{
		return std::pair<bool, std::string> {true, help()};
	}
	else if (command == "clear")
	{
		return std::pair<bool, std::string> {true, clear()};
	}
	else if (command == "compare" && split_command.size() == 3)
	{
		SAGE::SAGE_Engine engine{};
		return engine.compare_slx(split_command[1], split_command[2]);
	}
	else
	{
		return std::pair<bool, std::string> {true, "Invalid command! Check help section for right command structure."};
	}
}

void CLI::welcome_message()
{
	std::string message = R"(----------------------------------------------------------------------------------------
                           Welcome to SAGE CLI Tool
                Simulink Analysis and Graph Evaluation
----------------------------------------------------------------------------------------)";
	std::cout << message << "\n";
	std::cout << "Type 'help' for more details...\n";
}

std::string CLI::about()
{
	std::string test = R"(Simulink Analysis and Graph Evaluation (SAGE) is a lightweight, 
standalone C++17 tool for parsing and diffing Simulink .slx models without 
relying on MATLAB or Simulink. It is designed for developers and engineers 
working in model-based development environments who need fast, scriptable 
access to model structure and changes)";
	return test;
}

std::string CLI::license()
{
	std::string value = R"(
----------------------------------------------------------------------------------------
MIT License

Copyright(c) 2025 Mukund Iyer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
----------------------------------------------------------------------------------------)";
	return value;
}

std::string CLI::clear()
{
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
	welcome_message();
	std::cout << ">>";
	return "";
}

std::string CLI::help()
{
	std::string value = R"(Following commands are mapped for your usage:
about   -> Displays the information about this tool.
license -> Displays the license information for this tool.
exit    -> Closes the tool.
clear   -> Clears all the contents in the Command Line Interface.
compare -> Requires two inputs delimited by space. First one is old version. Second one is new version.)";
	return value;
}

std::pair<bool, std::string> CLI::compare(std::string command_input)
{
	auto split_command = split_regex(command_input," ");
	std::string command = split_command[0];
	return std::pair<bool, std::string>{};
}

CLI::CLI()
{
	bool loop_continue = true;
	welcome_message();
	std::cout << ">>";
	while (loop_continue)
	{
		std::string command{};
		std::getline(std::cin, command);
		command_list.push_back(command);
		std::pair<bool, std::string> output = parse(command);
		loop_continue = output.first;
		if (output.second != "" && loop_continue)
		{
			std::cout << output.second;
			std::cout << "\n\n";
			std::cout << ">>";
		}
	}
}

CLI::~CLI() = default;