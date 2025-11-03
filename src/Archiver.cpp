#include "Archiver.h"

using namespace Arch;

Archiver::Archiver() = default;

std::unordered_map<std::string, Path> Archiver::extract(const Path& archivePath)
{
	std::unordered_map<std::string, Path> extractedFiles;
	mz_zip_archive zip{};

	if (!mz_zip_reader_init_file(&zip, archivePath.string().c_str(), 0)) 
	{
		logger.log("Failed to open archive: " + archivePath.string(), "[ERROR]");
		return extractedFiles;
	}
	else
	{
		int fileCount = static_cast<int>(mz_zip_reader_get_num_files(&zip));
		Path tempDir = fs::temp_directory_path() / "SAGE";
		fs::create_directories(tempDir);

		for (int i = 0; i < fileCount; ++i) {
			mz_zip_archive_file_stat fileStat;
			if (mz_zip_reader_file_stat(&zip, i, &fileStat)) {
				Path outputPath = tempDir / fileStat.m_filename;
				fs::create_directories(outputPath.parent_path());
				if (mz_zip_reader_extract_to_file(&zip, i, outputPath.string().c_str(), 0)) {
					extractedFiles[Path(fileStat.m_filename).filename().string()] = outputPath;
				}
				else {
					logger.log("Failed to extract: " + (std::string) fileStat.m_filename, "[ERROR]");
				}
			}
		}

		mz_zip_reader_end(&zip);
		return extractedFiles;
	}
}

Path Archiver::archive(const std::vector<Path>& listOfFiles)
{
	mz_zip_archive zip;
	memset(&zip, 0, sizeof(zip));

	Path archivePath = fs::temp_directory_path() / "sage_archive.zip";
	if (!mz_zip_writer_init_file(&zip, archivePath.string().c_str(), 0)) {
		std::cerr << "Failed to create archive: " << archivePath << std::endl;
		return "";
	}

	for (const auto& filePath : listOfFiles) {
		if (!fs::exists(filePath)) {
			std::cerr << "File not found: " << filePath << std::endl;
			continue;
		}

		std::ifstream file(filePath, std::ios::binary);
		std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		std::string filename = filePath.filename().string();

		if (!mz_zip_writer_add_mem(&zip, filename.c_str(), buffer.data(), buffer.size(), MZ_BEST_COMPRESSION)) {
			std::cerr << "Failed to add file to archive: " << filename << std::endl;
		}
	}

	mz_zip_writer_finalize_archive(&zip);
	mz_zip_writer_end(&zip);
	return archivePath;
}

Archiver::~Archiver() = default;