#pragma once
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include "Logger.h"

namespace DataFrame
{
	template<typename T>
	using List = std::vector<T>;
	using df = std::unordered_map<std::string, List<std::string>>;
	namespace fs = std::filesystem;
	using Path = fs::path;

	class Table
	{
	public:
		// Variable: Used to hold data for the table.
		df data_frame{};

		// Method: Used to create an empty Table
		Table();
		// Method: Used to create an instance of Table.
		Table(const List<std::string>& column_vector);

		// Method: Adds columns to an empty Table
		void add_columns(const List<std::string>& column_vector);
		// Method: Adds row to the Table
		bool add_row(const List<std::string>& row_values); // Must check if the number of values in input argument match the number of columns.
		// Method: Exports the Table to a CSV file
		bool export_to_csv(const Path& file_path);
		// Method: Gives you the current count of rows in Table
		size_t get_row_count() const;
		// Method: Get list of all columns in the Table
		List<std::string> get_column_names() const;
		// Method: Clears data in the Table
		void clear();

		// Method: Destructor method.
		~Table();
	private:
		Logger logger{ "SAGE_Log.txt" }; // Varaible: Logger object used to log errors
		size_t max_row_count{}; // Variable: This is to ensure all columns have equal rows, including the ones being added.
		List<std::string> unordered_column_names{}; // Variable: This holds list of column names
		// Method: Adds list of columns based on list of column names. Don't use it more than once.
		bool initialize_columns(const List<std::string>& column_vector); // Initialize value of each column with empty vector
	};
}
