#pragma once
#include "Parser.h"
#include "Comparer.h"
#include <filesystem>

namespace SAGE
{
	namespace filesys = std::filesystem;
	using Path = filesys::path;
	using ParsingAgent = ParserNameSpace::Parser;
	using ComparingAgent = ComparerNameSpace::Comparer;
	template<typename T>
	using List = std::vector<T>;
	using Model = ParserNameSpace::Model;

	class SAGE_Engine
	{
	public:
		// Method: Constructor for the SAGE_Engine class object
		SAGE_Engine();
		// Method: Parses the given .slx file into a specific format for comparison.
		Model parse_slx(const std::string& slx_file_path);
		// Method: Used to compare two .slx files
		std::pair<bool, std::string> compare_slx(const std::string& slx_file_1, const std::string& slx_file_2);
		// Method: Destructor for the SAGE_Engine class object
		~SAGE_Engine();
	private:
		// Variable: Holds the list of extracted_files as a map
		std::unordered_map<std::string, Path> extracted_files{};
		// Variable: Object based on Logger class to invoke logging actions to store errors or messages
		Logger logger{ std::filesystem::temp_directory_path() / "SAGE" / "SAGE_Log.txt" };
		// Variable: Object based on Parser class to parse given .slx file
		ParsingAgent parsing_agent{};
		// Variable: Object based on Comparer class to compare two .slx files
		ComparingAgent comparing_agent{};
	};
}