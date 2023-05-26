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
  absl::SetProgramUsageMessage("Convert SQLite database back to to ELF");
  // Initialize the symbolizer to get a human-readable stack trace
  absl::InitializeSymbolizer(*argv[0]);
  absl::ParseCommandLine(*argc, *argv);
  absl::InstallFailureSignalHandler(absl::FailureSignalHandlerOptions{});
}

void convertSQLiteToElf(const std::string& elfFile,
                        const std::string& sqliteFile) {}

bool isSQLiteDatabase(const std::string& filename) {
  sqlite3* db;
  int rc =
      sqlite3_open_v2(filename.c_str(), &db, SQLITE_OPEN_READONLY, nullptr);
  if (rc == SQLITE_OK) {
    sqlite3_close(db);
    return true;
  } else {
    return false;
  }
}

int main(int argc, char** argv) {
  InitGoogle(&argc, &argv);

  if (argc != 2) {
    LOG(ERROR) << "You must include exactly one filename.";
    return EXIT_FAILURE;
  }

  const std::string filename = argv[1];
  if (!isSQLiteDatabase(filename)) {
    LOG(ERROR) << "The file you provided is not an ELF file.";
    return EXIT_FAILURE;
  }

  // TODO(fzakaria): figure out a better way for this
  //  this allows bazel run to create files where you invoke it.
  const char* bazelWorkingDirectory = std::getenv("BUILD_WORKING_DIRECTORY");
  std::string database = absl::StrFormat("%s/test.db", bazelWorkingDirectory);

  convertSQLiteToElf(filename, database);

  return 0;
}