#pragma once

#include "SQLiteCpp/Database.h"
#include "absl/strings/string_view.h"
#include "absl/types/span.h"
#include "tools/cpp/runfiles/runfiles.h"

struct InsertElfHeaderRequest {
  int e_type;
  int e_machine;
  int e_version;
  int e_flags;
};

using bazel::tools::cpp::runfiles::Runfiles;

class DatabaseManager {
 public:
  explicit DatabaseManager(SQLite::Database& db);
  ~DatabaseManager() = default;

  void createDatabaseSchema();

  void insertHeader(const InsertElfHeaderRequest& request);

  void insertInstructions(absl::Span<const uint8_t> bytes);

 private:
  std::string loadSqlFile(absl::string_view sql_file) const;

  SQLite::Database& db;
  std::unique_ptr<const Runfiles> runfiles;
};
