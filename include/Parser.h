#pragma once
#include "Archiver.h"

namespace ParserNameSpace
{
	template<typename T>
	using List = std::vector<T>;
	namespace fs = std::filesystem;
	using Path = fs::path;
	using connections = std::unordered_map<std::string, List<std::string>>;
	using block = std::unordered_map<std::string, std::string>;
	using blocks = std::unordered_map<std::string, block>;
	

	// Structure: Represents either a specific layer within a Simulink model or the entire model, depending on context.
	struct Model
	{
		// Structure member: Stores all the source to destinations connections
		connections lines{};
		// Structure member: Stores all the blocks as key-value pair where key is SID and value is a map of properties
		blocks systems{};
	};

	class Parser
	{
	public:
		// Variable: An object based on Logger class used to log events or messages.
		Logger logger{ "SAGE_Log.txt" };
		// Method: Constructor for Parser class object
		Parser();
		// Method: This parses the given SLX File
		Model parse(const std::string slx_file_path);
		// Method: Removes the SAGE directory from Temp folder.
		~Parser();
	private:
		// Variable: Holds the entire XML document.
		pugi::xml_document xml_content{};
		// Variable: An object based on Archiver class used to extract or archive files.
		Arch::Archiver archiving_interface{};
		// Method: Extracts connection maps in a map
		connections map_connections(pugi::xml_document& xml_content);
		// Method: Extracts Simulink blocks from model into a specialized map
		blocks collect_blocks(pugi::xml_document& xml_content);
		Model linearize_data(std::unordered_map<std::string, Model> parse_results);
		std::string get_block_path(block blk, blocks blks);
		std::unordered_map<std::string, std::string> read_node(pugi::xml_node& input_block_node);
		std::string get_hash_value(block input_block, block parent);
	};
}