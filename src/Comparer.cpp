#include "Comparer.h"

using namespace ComparerNameSpace;

// Process: Default constructor method for "Comparer" class.
Comparer::Comparer() = default;

// Method: Used to compare two unparsed Simulink models. Inputs are strings that are paths to the 2 models.
std::pair<bool, std::string> Comparer::compare(std::string model_1_path, std::string model_2_path)
{
	Path structural = fs::temp_directory_path() / "SAGE" / "Reports" / "Structural_Changes.csv";
	Path parametric = fs::temp_directory_path() / "SAGE" / "Reports" / "Parametric_Changes.csv";
	fs::remove(structural); fs::remove(parametric);
	// Variable: Used to store data after parsing one Simulink model
	model_1 = parsing_agent.parse(model_1_path);
	// Variable: Used to store data after parsing another Simulink model
	model_2 = parsing_agent.parse(model_2_path);
	// Process: If there are no logs logged...
	if (parsing_agent.logger.log_list.size() <= 0)
	{
		// Process: Compare the two parsed models
		compare_models(model_1, model_2);
		// Process: If there are any structural changes, export it to a CSV file.
		if (structural_changes.get_row_count() > 0)
		{
			structural_changes.export_to_csv(structural);
		}
		// Process: If there are any parametric changes, export it to a CSV file.
		if (parametric_changes.get_row_count() > 0)
		{
			parametric_changes.export_to_csv(parametric);
		}
		if (fs::exists(structural) || fs::exists(parametric))
		{
			return std::pair<bool, std::string>{true, structural.parent_path().string()};
		}
		else
		{
			return std::pair<bool, std::string>{true,""};
		}
	}
	else
	{
		// Process: If there are logs, we simply do not proceed to compare the parsed models.
		logging_agent.log("Comparison terminated due to error in parsing one or more Simulink models.", "[ERROR]");
		return std::pair<bool, std::string>{false, logging_agent.log_file_path};
	}
}

// Method: Used to compare two completely parsed Simulink models. Inputs are of user-defined struct "Model"
void Comparer::compare_models(parser::Model& model_1, parser::Model& model_2)
{
	// Process: Loop through each block in model_1 and search/compare with model_2
	for (auto& system : model_1.systems)
	{
		if (model_2.systems.find(system.first) != model_2.systems.end())
		{
			// Note: We find a block in another model using hash value made from name, block type and parent name.
			// Note: The hash value is calculated in the Parser module. Check Parser.cpp for implementation.
			are_equal(system.second, model_2.systems[system.first]);
		}
		else
		{
			// Process: System was removed or modified significantly. Add to output table.
			List<std::string> row_val{ system.second["SID"], system.second["Name"], system.second["Path"] , "Deleted"};
			structural_changes.add_row(row_val);
		}
	}
	// Process: Loop through each block in model_2 and check if there are any blocks not in model_1
	for (auto& system : model_2.systems)
	{
		if (model_1.systems.find(system.first) == model_1.systems.end())
		{
			// Process: Block's hash value was not found i.e. the block is newly added or heavily modified.
			List<std::string> row_val{ system.second["SID"], system.second["Name"], system.second["Path"] , "Added" };
			structural_changes.add_row(row_val);
		}
	}
}

// Method: Used to compare two items of type "block". A block is basically an unordered_map of string-string. Usually used to store block properties.
bool Comparer::are_equal(parser::block before_block, parser::block after_block)
{
	List<std::string> row_val{ before_block["SID"], before_block["Name"], before_block["Path"]}; // Variable: To store a value for entire row.
	// Process: Loop through each block property and check between the two models.
	for (const auto&[prop, val] : before_block)
	{
		if (after_block.find(prop) == after_block.end())
		{
			// Process: If the parameter is not found, we add it to "row_val" variable.
			row_val.push_back(prop);
			row_val.push_back(val);
			row_val.push_back("N/A");
		}
		else if(after_block[prop] != val)
		{
			// Process: If the parameter is found, but the values are mismatching, we add it to "row_val" variable.
			row_val.push_back(prop);
			row_val.push_back(val);
			row_val.push_back(after_block[prop]);
		}
	}
	if (row_val.size() > 3)
	{
		// Process: If there was a change detected, we add the details to the changes.
		parametric_changes.add_row(row_val);
		return false;
	}
	else
	{
		// Process: If there was no change detected, we return true because the blocks are equal.
		return true;
	}
}

// Process: Default destructor method for "Archiver" class. 
Comparer::~Comparer() = default;