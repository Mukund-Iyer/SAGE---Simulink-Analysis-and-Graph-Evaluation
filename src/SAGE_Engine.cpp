#include "SAGE_Engine.h"

using namespace SAGE;

SAGE_Engine::SAGE_Engine() = default;

Model SAGE_Engine::parse_slx(const std::string& slx_file_path)
{
	return parsing_agent.parse(slx_file_path);
}

void SAGE_Engine::compare_slx(const std::string& slx_file_1, const std::string& slx_file_2)
{
	comparing_agent.compare(slx_file_1, slx_file_2);
}

SAGE_Engine::~SAGE_Engine() = default;