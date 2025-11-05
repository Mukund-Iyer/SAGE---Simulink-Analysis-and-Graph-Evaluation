#include "SAGE_Engine.h"

using namespace SAGE;

SAGE_Engine::SAGE_Engine() = default;

Model SAGE_Engine::parse_slx(const std::string& slx_file_path)
{
	return parsing_agent.parse(slx_file_path);
}

std::pair<bool, std::string> SAGE_Engine::compare_slx(const std::string& slx_file_1, const std::string& slx_file_2)
{
	std::pair<bool,std::string> val = comparing_agent.compare(slx_file_1, slx_file_2);
	if (val.first && !val.second.empty())
	{
		return std::pair<bool, std::string> {true, "Differences were found! They are stored in " + val.second};
	}
	else if (!val.first)
	{
		return std::pair<bool, std::string> {val.first, "Some error occured during comparison. Refer -> " + val.second};
	}
}

SAGE_Engine::~SAGE_Engine() = default;