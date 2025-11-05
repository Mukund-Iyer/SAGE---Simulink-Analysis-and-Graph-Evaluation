#include "Archiver.h"

using namespace Arch;

// Process: Default constructor method for "Archiver" class.
Archiver::Archiver() = default;

// Method: Used to extract an archive. Archive path to be given as input.
std::unordered_map<std::string, Path> Archiver::extract(const Path& archivePath)
{
	std::unordered_map<std::string, Path> extractedFiles{}; // Variable: To store list of files extracted from .slx
	mz_zip_archive zip{}; // Variable: To use for performing extraction of .slx file

	if (!mz_zip_reader_init_file(&zip, archivePath.string().c_str(), 0)) 
	{
		// Process: Log that the extraction failed as it was unable to open the .slx file
		logger.log("Failed to open archive: " + archivePath.string(), "[ERROR]");
		// Process: Return "extractedFiles" which is empty.
		return extractedFiles;
	}
	else
	{
		int fileCount = static_cast<int>(mz_zip_reader_get_num_files(&zip)); // Variable: To store the number of files extracted.
		Path tempDir = fs::temp_directory_path() / "SAGE" / "Model_Data"; // Variable: To store temporary file path where the .slx is extracted.
		// Process: To create directories including nested ones to ease extraction process
		fs::create_directories(tempDir);
		// Process: Loop through each file within the .slx and extract them out
		for (int i = 0; i < fileCount; ++i) {
			mz_zip_archive_file_stat fileStat; // Variable: Used to store detailed entry about an archive entry
			// Process: If archive entry was found for an instance of file in the archive,...
			if (mz_zip_reader_file_stat(&zip, i, &fileStat)) {
				Path outputPath = tempDir / fileStat.m_filename; // Variable: To store output path for the file in an archive
				// Process: Create nested directories, if not created, to ensure extraction.
				fs::create_directories(outputPath.parent_path());
				// Process: Extract the file in the archive to the said output file path.
				if (mz_zip_reader_extract_to_file(&zip, i, outputPath.string().c_str(), 0)) {
					// Process: Add the path of the file extracted to the "extractedFiles" variable.
					extractedFiles[Path(fileStat.m_filename).filename().string()] = outputPath;
				}
				else {
					// Process: If the extraction of that particular file failed, log it.
					logger.log("Failed to extract: " + (std::string) fileStat.m_filename, "[ERROR]");
				}
			}
		}
		// Process: Close the zip reader
		mz_zip_reader_end(&zip);
		// Process: Return the list of extracted files
		return extractedFiles;
	}
}

// Method: Used to create an archive based on a list of files given as input.
Path Archiver::archive(const std::vector<Path>& listOfFiles)
{
	mz_zip_archive zip; // Variable: To use for performing extraction of .slx file
	// Process: Zero-initializing "zip" to avoid undefined behaviour. 
	memset(&zip, 0, sizeof(zip));
	Path archivePath = fs::temp_directory_path() / "sage_archive.zip"; // Variable: To store temporary path to where the archive might be stored/created.
	if (!mz_zip_writer_init_file(&zip, archivePath.string().c_str(), 0)) {
		// Process: Throw error because we were not able to create an archive out of given files??
		std::cerr << "Failed to create archive: " << archivePath << std::endl;
		// Process: Return empty string as that is the return data type for this method.
		return "";
	}
	// Process: Loop through the list of files selected for archiving.
	for (const auto& filePath : listOfFiles) {
		// Process: If the file instance in the list does not exist...
		if (!fs::exists(filePath)) {
			// Process: Throw error stating file was not found.
			std::cerr << "File not found: " << filePath << std::endl;
			// Process: Continue with next iteration in the loop
			continue;
		}

		std::ifstream file(filePath, std::ios::binary);
		std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		std::string filename = filePath.filename().string(); // Variable: To store the file name of the archive to be created.
		// Process: If the file is written to the archive, it return boolean value.
		if (!mz_zip_writer_add_mem(&zip, filename.c_str(), buffer.data(), buffer.size(), MZ_BEST_COMPRESSION)) {
			// Process: Throw error if file was not added to the archive.
			std::cerr << "Failed to add file to archive: " << filename << std::endl;
		}
	}
	// Process: Process the roster of the archive to actually generate the zip file.
	mz_zip_writer_finalize_archive(&zip);
	// Process: Close the zip writer.
	mz_zip_writer_end(&zip);
	// Process: Return the file path of the archive that was generated.
	return archivePath;
}

// Process: Default destructor method for "Archiver" class. 
Archiver::~Archiver() = default;