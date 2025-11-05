// SAGE.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "CLI.h" 
#include <chrono>

void parsing_speed_test()
{
	auto start = std::chrono::high_resolution_clock::now();
	SAGE::SAGE_Engine engine{};
	auto x = engine.parse_slx("D:/VS_Projects/SAGE_v2/SimulinkModels/TEM.slx");
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end - start;
	std::cout << "Execution time: " << duration.count() << " seconds\n";
}

void comparison_speed_test()
{
	auto start = std::chrono::high_resolution_clock::now();
	SAGE::SAGE_Engine engine{};
	engine.compare_slx("D:/VS_Projects/SAGE_v2/SimulinkModels/TEM.slx", "D:/VS_Projects/SAGE_v2/SimulinkModels/TEM_New.slx");
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end - start;
	std::cout << "Execution time: " << duration.count() << " seconds\n";
}

int main()
{
	bool loop_continue = true;
	CL_Interface::welcome_message();
	std::cout << ">>";
	while (loop_continue)
	{
		std::string command{};
		std::getline(std::cin,command);
		std::pair<bool, std::string> output = CL_Interface::parse(command);
		loop_continue = output.first;
		if (output.second != "" && loop_continue)
		{
			std::cout << output.second;
			std::cout << "\n\n";
			std::cout << ">>";
		}
	}
}