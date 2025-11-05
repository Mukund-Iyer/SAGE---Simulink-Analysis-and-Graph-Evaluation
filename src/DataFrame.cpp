#include "DataFrame.h"

using namespace DataFrame;

Table::Table() = default;

Table::Table(const List<std::string>& column_vector) {
	unordered_column_names = column_vector;
	initialize_columns(column_vector);
}

Table::~Table() = default;

void Table::add_columns(const List<std::string>& column_vector)
{
	if (data_frame.size() <= 0)
	{
		initialize_columns(column_vector);
	}
}

bool Table::initialize_columns(const List<std::string>& column_vector) {
	if (column_vector.size() <= 0) {
		logger.log("No column names provided for add_columns.", "[ERROR]");
		return false;
	}
	for (const std::string column_name : column_vector) {
		if (data_frame.find(column_name) == data_frame.end()) {
			data_frame[column_name] = List<std::string>{};
		}
		else
		{
			logger.log("One or more parametric_columns with same name cannot be added to DataFrame.", "[ERROR]");
			return false;
		}
	}
	return true;
}

bool Table::add_row(const List<std::string>& row_values) {
	size_t input_column_count = row_values.size();
	if (input_column_count == unordered_column_names.size()) {
		const auto column_names = get_column_names();
		for (size_t index = 0; index < column_names.size(); index++) {
			auto test = row_values[index];
			std::replace(test.begin(), test.end(), ',', ' ');
			data_frame[column_names[index]].push_back(test);
		}
		max_row_count = data_frame[column_names[column_names.size() - 1]].size();
		return true;
	}
	else
	{
		logger.log("Mismatch between number of parametric_columns in the DataFrame and given data.", "[ERROR]");
		return false;
	}
}

bool Table::export_to_csv(const Path& file_path) {
	fs::create_directories(file_path.parent_path());
	std::ofstream file(file_path); // Local variable to store open file streaming service to write values.
	if (!file.is_open()) return false;
	const List<std::string> header_list = unordered_column_names; // Local variable to store list of all DataFrame headers
	for (size_t iterator = 0; iterator < header_list.size(); iterator++) {
		file << header_list[iterator];
		if (iterator < header_list.size() - 1) {
			file << ",";
		}
	}
	file << "\n";
	// Add rows
	size_t row_count = get_row_count(); // Local variable to store the number of rows in DataFrame

	for (size_t iterator_1 = 0; iterator_1 < row_count; iterator_1++) {
		for (size_t iterator_2 = 0; iterator_2 < header_list.size(); iterator_2++) {
			file << data_frame[header_list[iterator_2]][iterator_1];
			if (iterator_2 < header_list.size() - 1) {
				file << ",";
			}
		}
		file << "\n";
	}
	file.close();
	return true;
}

size_t Table::get_row_count() const {
	size_t return_value{};
	for (const auto& pair : data_frame) {
		return_value = pair.second.size();
		break;
	}
	return return_value;
}

List<std::string> Table::get_column_names() const {
	return unordered_column_names;
}

void Table::clear() {
	max_row_count = size_t{};
	data_frame = df{};
	unordered_column_names = List<std::string>{};
}