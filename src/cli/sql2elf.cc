#include <sys/stat.h>

#include <fstream>
#include <memory>

#include "SQLiteCpp/Database.h"
#include "SQLiteCpp/Statement.h"
#include "absl/log/log.h"
#include "absl/strings/str_format.h"
#include "sqlite3.h"
#include "src/cli/common.h"
#include "src/utils/path.h"

void convertSQLiteToElf(const std::string& sqliteFile,
                        const std::string& elfFile) {
  // Create a SQLite database and table
  SQLite::Database db(sqliteFile, SQLite::OPEN_READONLY);

  // Fetch the binary data from the database
  // This assumes that there is only one entry at the moment
  const SQLite::Column column = db.execAndGet(R"(
    SELECT CONTENT FROM binary LIMIT 1
      )");
  if (!column.isBlob()) {
    throw std::runtime_error("Expected binary blob result.");
  }

  const char* data = reinterpret_cast<const char*>(column.getBlob());
  const int size = column.getBytes();

  std::ofstream file(elfFile, std::ios::binary);
  // Write the data to the file
  file.write(data, size);
  // Close the file
  file.close();

  // TODO(fzakaria): This doesn't only add executable but read + write also
  if (chmod(elfFile.c_str(), S_IRUSR | S_IWUSR | S_IXUSR) != 0) {
    throw std::runtime_error(
        absl::StrFormat("Failed to set executable permissions on %s: %s",
                        elfFile, strerror(errno)));
  }
}

bool isSQLiteDatabase(const std::string& filename) {
  try {
    const SQLite::Database db(filename, SQLite::OPEN_READONLY);
    return true;
  } catch (const std::exception& e) {
    LOG(ERROR) << "Failed to open SQLite database: " << filename << " "
               << e.what();
    return false;
  }
}

int main(int argc, char** argv) {
  InitGoogle(&argc, &argv, "Convert SQLite database back to to ELF");

  if (argc != 2) {
    LOG(ERROR) << "You must include exactly one filename.";
    return EXIT_FAILURE;
  }

  const std::string sqlite_filename = argv[1];
  if (!isSQLiteDatabase(sqlite_filename)) {
    LOG(ERROR) << "The file you provided is not a SQLite file.";
    return EXIT_FAILURE;
  }

  std::string elf_filename =
      replace_file_extension(basename(sqlite_filename), "elf");
  // TODO(fzakaria): figure out a better way for this
  //  this allows bazel run to create files where you invoke it.
  const char* bazelWorkingDirectory = std::getenv("BUILD_WORKING_DIRECTORY");
  if (bazelWorkingDirectory != nullptr) {
    elf_filename =
        absl::StrFormat("%s/%s", bazelWorkingDirectory, elf_filename);
  }

  try {
    convertSQLiteToElf(sqlite_filename, elf_filename);
  } catch (const std::exception& e) {
    LOG(ERROR) << "Failed to convert SQLite database to ELF: " << e.what();
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}