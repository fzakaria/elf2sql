#include "common.h"

#include <cerrno>
#include <fstream>
#include <streambuf>
#include <string>

#include "absl/strings/str_format.h"
#include "tools/cpp/runfiles/runfiles.h"

using bazel::tools::cpp::runfiles::Runfiles;

const std::vector<std::string> SQL_FILES = {
    "elf_header.sql",
};

void CreateDatabaseSchema(SQLite::Database& db) {
  const char* prog_name = program_invocation_name;
  std::string error;
  const std::unique_ptr<Runfiles> runfiles(Runfiles::Create(prog_name, &error));
  if (runfiles == nullptr) {
    throw std::runtime_error(
        absl::StrFormat("Failed to create runfiles: %s", error));
  }
  for (const std::string& sql_file : SQL_FILES) {
    const std::string sql_path =
        runfiles->Rlocation("__main__/src/sql/" + sql_file);
    std::ifstream sql_stream(sql_path);
    if (!sql_stream.is_open()) {
      throw std::runtime_error(
          absl::StrFormat("Failed to open %s: %s", sql_path, strerror(errno)));
    }
    const std::string sql((std::istreambuf_iterator<char>(sql_stream)),
                          std::istreambuf_iterator<char>());
    db.exec(sql);
  }
}