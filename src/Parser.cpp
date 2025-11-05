#include "Parser.h"
using namespace ParserNameSpace;

Parser::Parser() = default;

Model Parser::parse(const std::string slx_file_path)
{
	// Variable: To store block data for each XML file
	std::unordered_map<std::string,Model> output{};
	// Variable: To store string-based path to std::filesystem::path type
	Path file_path = Path(slx_file_path);
	// Map of extracted files with name and std::filesystem::path type values
	std::unordered_map<std::string, Path> extracted_list = archiving_interface.extract(file_path);
	if (extracted_list.find("system_root.xml") == extracted_list.end())
	{
		logger.log("Provided SLX file " + file_path.string() + " is corrupted. Missing system_root.xml", "[ERROR]");
	}
	else
	{
		for (const auto&[name, path_val] : extracted_list)
		{
			if (name.find("system_") == 0 && name.find(".xml") == name.size()-4)
			{
				// Variable: To store XML Parse results for further specialized parsing.
				pugi::xml_parse_result parse_result = xml_content.load_file(path_val.string().c_str(), 0);
				if (parse_result)
				{
					// Variable: To store temporarily the model components in a structured manner.
					Model current_model{ map_connections(xml_content),collect_blocks(xml_content) };
					output[name] = current_model;
				}
			}
		}
	}
	return linearize_data(output);
}

connections Parser::map_connections(pugi::xml_document& xml_content)
{
	std::unordered_map<std::string, List<std::string>> output{};
	pugi::xpath_node_set line_nodes = xml_content.select_nodes("//Line");
	for (const auto& line_node : line_nodes)
	{
		pugi::xml_node input_node = line_node.node();
		pugi::xpath_node_set parameters = input_node.select_nodes(".//P");
		pugi::xpath_node_set branches = input_node.select_nodes(".//Branch");
		std::string src{};
		List<std::string> dst{};
		for (const auto& parameter_node : parameters) {
			pugi::xml_node node = parameter_node.node();
			for (const auto& attr : node.attributes()) {
				std::string attribute_name = std::string(attr.name());
				std::string attribute_value = std::string(attr.value());
				if (attribute_name == "Name" && attribute_value == "Src") {
					src = node.text().as_string();
				}
				else if (attribute_name == "Name" && attribute_value == "Dst") {
					dst.push_back(node.text().as_string());
				}
			}
		}
		if (src != "" && output.find(src) != output.end() && !dst.empty())
		{
			output[src].insert(output[src].end(), dst.begin(), dst.end());
		}
		else if (src != "" && !dst.empty())
		{
			output[src] = dst;
		}

		src = std::string{};
		dst = List<std::string>{};
		for (const auto& branch_node : branches) {
			auto const& branch = branch_node.node();
			auto const& params = branch.select_nodes(".//P");
			for (auto const& param_node : params) {
				for (const auto& attr : param_node.node().attributes()) {
					if (attr.name() == "Name" && attr.value() == "Src") {
						src = param_node.node().text().as_string();
					}
					else if (attr.name() == "Name" && attr.value() == "Dst") {
						dst.push_back(param_node.node().text().as_string());
					}
				}
			}
			if (src != "" && output.find(src) != output.end() && !dst.empty())
			{
				output[src].insert(output[src].end(), dst.begin(), dst.end());
			}
			else if (src != "" && !dst.empty())
			{
				output[src] = dst;
			}
		}
	}
	return output;
}

blocks Parser::collect_blocks(pugi::xml_document& xml_content)
{
	blocks output{};
	pugi::xpath_node_set block_nodes = xml_content.select_nodes("//Block");
	for (const auto& block_node : block_nodes)
	{
		pugi::xml_node block_node_actual = block_node.node();
		std::unordered_map<std::string, std::string> properties = read_node(block_node_actual);
		if (output.find(properties["SID"]) == output.end())
		{
			output[properties["SID"]] = properties;
		}
	}
	return output;
}

Model Parser::linearize_data(std::unordered_map<std::string, Model> parse_results)
{
	connections conns{};
	blocks blks{};
	blocks temp_blks{};
	// Actual linearization of data
	for (auto&[name, model] : parse_results)
	{
		for (const auto&[src, dst_list] : model.lines)
		{
			if (conns.find(src) == conns.end())
			{
				conns[src] = dst_list;
			}
		}
		for (auto[sid, prop] : model.systems)
		{
			if (blks.find(sid) == blks.end())
			{
				blks[sid] = prop;
			}
		}
	}
	// Setting parent SID for all blocks, except top-level blocks
	for (auto& blk : blks)
	{
		if (blk.second.find("contents") != blk.second.end())
		{
			auto x = parse_results[blk.second["contents"] + ".xml"];
			for (auto& y : x.systems)
			{
				blks[y.first]["Parent_SID"] = blk.second["SID"];
			}
		}
	}
	// Setting parent SID for all top-level blocks
	for (auto& blk : blks)
	{
		if (blk.second.find("Parent_SID") == blk.second.end())
		{
			blk.second["Parent_SID"] = "system_root";
		}
	}
	// Setting block path for all blocks
	for (auto& blk : blks)
	{
		blk.second["Path"] = get_block_path(blks[blk.first], blks);
	}
	for (auto& blk : blks)
	{
		std::string hash_value = get_hash_value(blk.second, blks[blk.second["Parent_SID"]]);
		if (!hash_value.empty())
		{
			temp_blks[get_hash_value(blk.second, blks[blk.second["Parent_SID"]])] = blk.second;
		}
	}
	return Model{ conns, temp_blks };
}

std::string Parser::get_block_path(block blk,blocks blks)
{
	std::vector<std::string> path{};
	std::string output{};
	while (blk.find("Parent_SID") != blk.end() && blk["Parent_SID"] != "system_root")
	{
		path.push_back(blk["Name"]);
		blk = blks[blk["Parent_SID"]];
	}
	if (blk["Parent_SID"] == "system_root")
	{
		path.push_back(blk["Name"]);
		path.push_back("system_root");
	}
	for (int i = path.size()-1; i >= 0; i--)
	{
		if (i > 0)
		{
			output += path[i] + "/";
		}
		else
		{
			output += path[i];
		}
	}
	return output;
}

std::unordered_map<std::string, std::string> Parser::read_node(pugi::xml_node& input_block_node)
{
	std::unordered_map<std::string, std::string> output{};
	std::unordered_map<std::string, std::string> parent_output{};
	for (const auto& attr : input_block_node.attributes()) {
		output[attr.name()] = attr.value();
	}
	pugi::xpath_node_set pnodes = input_block_node.select_nodes(".//P");
	pugi::xpath_node system_node = input_block_node.select_node(".//System");
	if (system_node)
	{
		const auto sys_node = system_node.node();
		for (const auto& sys_node_attr : sys_node.attributes()) {
			output["contents"] = sys_node_attr.value();
		}
	}
	for (const auto& pnode : pnodes) {
		const auto pnode_node = pnode.node();
		for (const auto& pnode_attr : pnode_node.attributes()) {
			output[pnode_attr.value()] = pnode_node.text().as_string();
		}
	}
	return output;
}

std::string Parser::get_hash_value(block input_block, block parent)
{
	std::hash<std::string> h_f;
	if (input_block.find("Name") != input_block.end() && input_block.find("BlockType") != input_block.end() && parent.find("Name") != parent.end())
	{
		std::string value_to_hash = input_block["Name"] + "/" +  input_block["BlockType"] + "/" + parent["Name"];
		std::stringstream ss;
		ss << std::hex << h_f(value_to_hash);
		return ss.str();
	}
	else if (input_block.find("Name") != input_block.end() && input_block.find("BlockType") != input_block.end() && parent.empty())
	{
		std::string value_to_hash = input_block["Name"] + "/" + input_block["BlockType"] + "/" + "system_root";
		std::stringstream ss;
		ss << std::hex << h_f(value_to_hash);
		return ss.str();
	}
	else
	{
		//logger.log("Failed to get hash value for a Simulink block!","[ERROR]");
		return "";
	}
}

Parser::~Parser()
{
	Path tempDir = fs::temp_directory_path() / "SAGE" / "Model_Data";
	fs::remove_all(tempDir);
}