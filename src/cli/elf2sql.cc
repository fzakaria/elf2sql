#include <iostream>
#include <memory>
#include <string>

#include "LIEF/ELF.hpp"
#include "SQLiteCpp/Database.h"
#include "absl/debugging/failure_signal_handler.h"
#include "absl/debugging/symbolize.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"
#include "absl/log/initialize.h"
#include "absl/log/log.h"
#include "absl/strings/str_format.h"
#include "src/sqlite/database.h"

/**
 * For a given string replace the extension with the new one.
 * If no extension is present, simply append the new extension.
 */
std::string replace_file_extension(const std::string& filename,
                                   const std::string& new_extension) {
  const size_t pos = filename.find_last_of('.');
  if (pos == std::string::npos) {
    return filename + "." + new_extension;
  }
  return filename.substr(0, pos) + new_extension;
}

/**
 * Return the basename for a given string.
 */
std::string basename(const std::string& path) {
  size_t const pos = path.find_last_of("/\\");
  if (pos == std::string::npos) {
    return path;
  }
  return path.substr(pos + 1);
}

void InitGoogle(const int* argc, char*** argv) {
  absl::InitializeLog();
  absl::SetProgramUsageMessage("Convert ELF to SQLite databases");
  // Initialize the symbolizer to get a human-readable stack trace
  absl::InitializeSymbolizer(*argv[0]);
  absl::ParseCommandLine(*argc, *argv);
  absl::InstallFailureSignalHandler(absl::FailureSignalHandlerOptions{});
}

void convertElfToSQLite(const std::string& elfFile,
                        const std::string& sqliteFile) {
  // Load the ELF file
  std::unique_ptr<LIEF::ELF::Binary> binary = LIEF::ELF::Parser::parse(elfFile);

  // Create a SQLite database and table
  const sqlite::Database db(sqliteFile,
                            static_cast<sqlite::Database::OpenFlags>(
                                sqlite::Database::OpenFlags::ReadWrite |
                                sqlite::Database::OpenFlags::Create));

  // Creat the simplest sqlite database to hold the contents of the binary
  db.exec(R"(
    CREATE TABLE IF NOT EXISTS binary (id INTEGER PRIMARY KEY AUTOINCREMENT,
    content BLOB);
      )");
}

int main(int argc, char** argv) {
  InitGoogle(&argc, &argv);

  if (argc != 2) {
    LOG(ERROR) << "You must include exactly one filename.";
    return EXIT_FAILURE;
  }

  const std::string elf_filename = argv[1];
  if (!LIEF::ELF::is_elf(elf_filename)) {
    LOG(ERROR) << "The file you provided is not an ELF file.";
    return EXIT_FAILURE;
  }

  // TODO(fzakaria): figure out a better way for this
  //  this allows bazel run to create files where you invoke it.
  const char* bazelWorkingDirectory = std::getenv("BUILD_WORKING_DIRECTORY");
  const std::string database_filename =
      absl::StrFormat("%s/%s", bazelWorkingDirectory,
                      replace_file_extension(basename(elf_filename), "db"));

  convertElfToSQLite(elf_filename, database_filename);

  return 0;
}
