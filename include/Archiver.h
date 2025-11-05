#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include "external/pugixml.hpp"
#include "external/miniz.h"
#include "Logger.h"
#include <fstream>
#include <algorithm>

namespace Arch
{
	namespace fs = std::filesystem;
	using Path = fs::path;
	/*********************************************************************************
	Class Name: Archiver
	Purpose: A wrapper or interface to use miniz library to extract and archive files.
	*********************************************************************************/
	class Archiver
	{
	public:
		// Method: Construcor class. Used to initialize or create an object of this class.
		Archiver();
		// Method: Used to extract an archive. Archive path to be given as input.
		std::unordered_map<std::string, Path> extract(const Path& archivePath);
		// Method: Used to create an archive based on a list of files given as input.
		Path archive(const std::vector<Path>& listOfFiles);
		// Method: Destructor class. Used for garbage collection. Often called when the object created loses context or is deleted.
		~Archiver();
	private:
		// Variable: Object of class "Logger" used to add logs to the common log file.
		Logger logger{fs::temp_directory_path() / "SAGE" / "SAGE_Log.txt"};
	};
}