#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include "external/pugixml.hpp"
#include "external/miniz.h"
#include "Logger.h"
#include <fstream>

namespace Arch
{
	namespace fs = std::filesystem;
	using Path = fs::path;
	class Archiver
	{
	public:
		Archiver();
		std::unordered_map<std::string, Path> extract(const Path& archivePath);
		Path archive(const std::vector<Path>& listOfFiles);
		~Archiver();
	private:
		Logger logger{"SAGE_Log.txt"};
	};
}