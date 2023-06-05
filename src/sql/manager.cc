#include "manager.h"

#include <cerrno>
#include <fstream>
#include <string>

#include "absl/strings/str_cat.h"
#include "absl/strings/str_format.h"

const std::vector<std::string> SCHEMA_SQL_FILES = {
    "schema/create_elf_header.sql",
};

void DatabaseManager::createDatabaseSchema() {
  for (const std::string& sql_file : SCHEMA_SQL_FILES) {
    const std::string sql = loadSqlFile(sql_file);
    db.exec(sql);
  }
}

void DatabaseManager::insertHeader(const InsertElfHeaderRequest& request) {
  SQLite::Statement insert(db,
                           "INSERT INTO elf_header (e_type, e_machine, "
                           "e_version, e_flags) VALUES (?, ?, ?, ?)");
  insert.bind(1, request.e_type);
  insert.bind(2, request.e_machine);
  insert.bind(3, request.e_version);
  insert.bind(4, request.e_flags);
  const int rowsCreated = insert.exec();
  if (rowsCreated == 0) {
    throw std::runtime_error("Failed to insert header into database");
  }
}

DatabaseManager::DatabaseManager(SQLite::Database& db) : db(db) {
  const char* prog_name = program_invocation_name;
  std::string error;
  runfiles =
      std::unique_ptr<const Runfiles>(Runfiles::Create(prog_name, &error));
  if (runfiles == nullptr) {
    throw std::runtime_error(
        absl::StrFormat("Failed to create runfiles: %s", error));
  }
}

std::string DatabaseManager::loadSqlFile(absl::string_view sql_file) const {
  const std::string sql_path =
      runfiles->Rlocation(absl::StrCat("__main__/src/sql/", sql_file));
  std::ifstream sql_stream(sql_path);
  if (!sql_stream.is_open()) {
    throw std::runtime_error(
        absl::StrFormat("Failed to open %s: %s", sql_path, strerror(errno)));
  }
  std::string sql((std::istreambuf_iterator<char>(sql_stream)),
                  std::istreambuf_iterator<char>());
  return sql;
}