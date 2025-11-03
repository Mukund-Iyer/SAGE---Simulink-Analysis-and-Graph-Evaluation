#pragma once
#include <vector>
#include <unordered_map>

namespace GraphNameSpace
{
	template<typename T>
	using List = std::vector<T>;
	class Graph
	{
		std::unordered_map<std::string, List<std::string>> graph{};
	};
}