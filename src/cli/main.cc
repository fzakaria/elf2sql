#include <iostream>
#include <memory>
#include <string>

#include "LIEF/ELF.hpp"
#include "absl/debugging/failure_signal_handler.h"
#include "absl/debugging/symbolize.h"
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"
#include "absl/log/initialize.h"
#include "absl/log/log.h"
#include "absl/strings/str_format.h"
#include "sqlite3.h"

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
  sqlite3* db = nullptr;
  int rc = sqlite3_open(sqliteFile.c_str(), &db);
  if (rc != SQLITE_OK) {
    std::cerr << "Failed to open SQLite database: " << sqlite3_errmsg(db)
              << std::endl;
    return;
  }

  char* errMsg = nullptr;
  const char* createTableSQL =
      "CREATE TABLE IF NOT EXISTS sections (id INTEGER PRIMARY KEY "
      "AUTOINCREMENT, name TEXT, size INTEGER);";
  rc = sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errMsg);
  if (rc != SQLITE_OK) {
    std::cerr << "Failed to create table: " << errMsg << std::endl;
    sqlite3_free(errMsg);
    sqlite3_close(db);
    return;
  }

  // Extract information from the ELF file and insert it into the table
  const auto& sections = binary->sections();
  for (const auto& section : sections) {
    std::string name = section.name();
    uint64_t size = section.size();

    std::string sql = "INSERT INTO sections (name, size) VALUES ('" + name +
                      "', " + std::to_string(size) + ");";
    rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
      std::cerr << "Failed to insert data: " << errMsg << std::endl;
      sqlite3_free(errMsg);
      sqlite3_close(db);
      return;
    }
  }

  // Close the database connection
  sqlite3_close(db);
}

int main(int argc, char** argv) {
  InitGoogle(&argc, &argv);

  if (argc != 2) {
    LOG(ERROR) << "You must include exactly one filename.";
    return EXIT_FAILURE;
  }

  const std::string filename = argv[1];
  if (!LIEF::ELF::is_elf(filename)) {
    LOG(ERROR) << "The file you provided is not an ELF file.";
    return EXIT_FAILURE;
  }

  //TODO(fzakaria): figure out a better way for this
  // this allows bazel run to create files where you invoke it.
  const char* bazelWorkingDirectory = std::getenv("BUILD_WORKING_DIRECTORY");
  std::string database = absl::StrFormat("%s/test.db", bazelWorkingDirectory);

  convertElfToSQLite(filename, database);

  return 0;
}