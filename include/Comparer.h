#pragma once
#include "Parser.h"
#include "DataFrame.h"

namespace ComparerNameSpace
{
	namespace parser = ParserNameSpace;
	namespace df = DataFrame;
	using Table = df::Table;
	template<typename T>
	using List = std::vector<T>;
	namespace fs = std::filesystem;
	using Path = fs::path;

	class Comparer
	{
	public:
		// Variable: To store parsed model details for a model for comparison
		parser::Model model_1{};
		// Variable: To store parsed model details for another model for comparison
		parser::Model model_2{};
		// Variable: To store structural changes between two Simulink models
		Table structural_changes{ List<std::string> { "SID","Block_Name","Block_Path","Added or Deleted?" } };
		// Variable: To store parametric changes between two Simulink models
		Table parametric_changes{ List<std::string> { "SID","Block_Name","Block_Path","Property","Before_Value","After_Value"  } };
		// Method: Constructor method for object of class "Comparer" 
		Comparer();
		//Method: Used to actually compare two Simulink models
		void compare(std::string model_1_path, std::string model_2_path);
		// Method: Destructor method for object of class "Comparer" 
		~Comparer();
	private:
		// Variable: Class object used to create instance for parsing SLX files
		parser::Parser parsing_agent{};
		// Variable: Logging agent to log events
		Logger logging_agent{ "SAGE_Log.txt" };
		// Method: Compare two models
		void compare_models(parser::Model& model_1, parser::Model& model_2);
		bool are_equal(parser::block a, parser::block b);
	};
}