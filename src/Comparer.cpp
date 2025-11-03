#include "Comparer.h"

using namespace ComparerNameSpace;

Comparer::Comparer() = default;

void Comparer::compare(std::string model_1_path, std::string model_2_path)
{
	model_1 = parsing_agent.parse(model_1_path);
	model_2 = parsing_agent.parse(model_2_path);
	if (parsing_agent.logger.log_list.size() <= 0)
	{
		compare_models(model_1, model_2);
		if (structural_changes.get_row_count() > 0)
		{
			structural_changes.export_to_csv(Path("D:\\VS_Projects\\SAGE_v2\\Compa_struct.csv"));
		}
		if (parametric_changes.get_row_count() > 0)
		{
			parametric_changes.export_to_csv(Path("D:\\VS_Projects\\SAGE_v2\\Compa_para.csv"));
		}
	}
	else
	{
		logging_agent.log("Comparison terminated due to error in parsing one or more Simulink models.", "[ERROR]");
	}
}

void Comparer::compare_models(parser::Model& model_1, parser::Model& model_2)
{
	for (auto& system : model_1.systems)
	{
		if (model_2.systems.find(system.first) != model_2.systems.end())
		{
			// Found system by hash value made from name, block type and parent name
			are_equal(system.second, model_2.systems[system.first]);
		}
		else
		{
			// System was removed or modified significantly
			List<std::string> row_val{ system.second["SID"], system.second["Name"], system.second["Path"] , "Deleted"};
			structural_changes.add_row(row_val);
		}
	}
	for (auto& system : model_2.systems)
	{
		if (model_1.systems.find(system.first) == model_1.systems.end())
		{
			List<std::string> row_val{ system.second["SID"], system.second["Name"], system.second["Path"] , "Added" };
			structural_changes.add_row(row_val);
		}
	}
}

bool Comparer::are_equal(parser::block before_block, parser::block after_block)
{
	// Check properties between the blocks
	List<std::string> row_val{ before_block["SID"], before_block["Name"], before_block["Path"]};
	for (const auto&[prop, val] : before_block)
	{
		if (after_block.find(prop) == after_block.end())
		{
			// Missing parameter
			row_val.push_back(prop);
			row_val.push_back(val);
			row_val.push_back("N/A");
		}
		else if(after_block[prop] != val)
		{
			// Parameteric changes detected
			row_val.push_back(prop);
			row_val.push_back(val);
			row_val.push_back(after_block[prop]);
		}
	}
	if (row_val.size() > 3)
	{
		parametric_changes.add_row(row_val);
		return false;
	}
	else
	{
		return true;
	}
}

Comparer::~Comparer() = default;