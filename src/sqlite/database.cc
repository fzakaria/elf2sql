#include "database.h"

#include "absl/strings/str_format.h"

namespace sqlite {

Database::~Database() = default;

Database::Database(const std::string& filename,
                   const Database::OpenFlags& flags)
    : db(nullptr, destroy) {
  sqlite3* temp_db = nullptr;
  int const rc = sqlite3_open_v2(filename.c_str(), &temp_db, flags, nullptr);
  db = std::unique_ptr<sqlite3, decltype(&destroy)>(temp_db, destroy);
  if (rc != SQLITE_OK) {
    throw std::runtime_error(absl::StrFormat("Failed to open database [%s]: %s",
                                             filename,
                                             sqlite3_errmsg(db.get())));
  }
}

void Database::exec(const std::string& sql) const {
  char* errMsg = nullptr;
  const int rc = sqlite3_exec(db.get(), sql.c_str(), nullptr, nullptr, &errMsg);
  if (rc != SQLITE_OK) {
    const std::string msg =
        absl::StrFormat("Failed to execute SQL: %s", errMsg);
    sqlite3_free(errMsg);
    throw std::runtime_error(msg);
  }
}

void Database::destroy(sqlite3* db) {
  if (db != nullptr) {
    sqlite3_close(db);
  }
}

}  // namespace sqlite