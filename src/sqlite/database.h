#pragma once

#include <string>

#include "sqlite3.h"

namespace sqlite {
class Database {
 public:
  enum OpenFlags : uint {
    ReadOnly = SQLITE_OPEN_READONLY,
    ReadWrite = SQLITE_OPEN_READWRITE,
    Create = SQLITE_OPEN_CREATE
  };

  Database(const std::string& filename, const OpenFlags& flags);
  ~Database();

  /**
   * Accept a list of semi-colon separated SQL statements and execute them.
   */
  void exec(const std::string& sql);
  static void destroy(sqlite3* db);

 private:
  std::unique_ptr<sqlite3, decltype(&destroy)> db;
};
}  // namespace sqlite
