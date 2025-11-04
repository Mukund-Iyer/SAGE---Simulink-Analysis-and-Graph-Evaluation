#include "CLI.h"

std::pair<bool, std::string> CL_Interface::parse(std::string input_command)
{
	if (input_command == "exit")
	{
		return std::pair<bool, std::string> {false, "Goodbye!"};
	}
	else if (input_command == "about")
	{
		return std::pair<bool, std::string> {true, about()};
	}
	else if (input_command == "license")
	{
		return std::pair<bool, std::string> {true, license()};
	}
	else if (input_command == "help")
	{
		return std::pair<bool, std::string> {true, help()};
	}
	else if (input_command == "clear")
	{
		return std::pair<bool, std::string> {true, clear()};
	}
	else
	{
		return std::pair<bool, std::string> {true, "Invalid command!"};
	}
}

void CL_Interface::welcome_message()
{
	std::string message = R"(----------------------------------------------------------------------------------------
                           Welcome to SAGE CLI Tool
                Simulink Analysis and Graph Evaluation
----------------------------------------------------------------------------------------)";
	std::cout << message << "\n";
	std::cout << "Type 'help' for more details...\n";
}

std::string CL_Interface::about()
{
	std::string test = R"(Simulink Analysis and Graph Evaluation (SAGE) is a lightweight, 
standalone C++17 tool for parsing and diffing Simulink .slx models without 
relying on MATLAB or Simulink. It is designed for developers and engineers 
working in model-based development environments who need fast, scriptable 
access to model structure and changes)";
	return test;
}

std::string CL_Interface::license()
{
	std::string value = R"(
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
		SOFTWARE.)";
	return value;
}

std::string CL_Interface::clear()
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

std::string CL_Interface::help()
{
	std::string value = R"(Following commands are mapped for your usage:
about   -> Displays the information about this tool.
license -> Displays the license information for this tool.
exit    -> Closes the tool.
clear   -> Clears all the contents in the Command Line Interface.)";
	return value;
}