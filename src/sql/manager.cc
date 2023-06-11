#include "manager.h"

#include <cerrno>
#include <fstream>
#include <string>

#include "SQLiteCpp/Transaction.h"
#include "absl/log/log.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_format.h"
#include "src/elf/disassembler.h"

const std::vector<std::string> SCHEMA_SQL_FILES = {
    "schema/create_elf_header.sql", "schema/create_instructions.sql"};

void DatabaseManager::createDatabaseSchema() {
  for (const std::string& sql_file : SCHEMA_SQL_FILES) {
    const std::string sql = loadSqlFile(sql_file);
    db.exec(sql);
  }
}

void DatabaseManager::insertHeader(const InsertElfHeaderRequest& request) {
  const std::string sql = loadSqlFile("insert/insert_elf_header.sql");
  SQLite::Statement insert{db, sql};
  insert.bind(1, request.e_type);
  insert.bind(2, request.e_machine);
  insert.bind(3, request.e_version);
  insert.bind(4, request.e_flags);
  const int rowsCreated = insert.exec();
  if (rowsCreated == 0) {
    throw std::runtime_error("Failed to insert header into database");
  }
}

void DatabaseManager::insertInstructions(absl::Span<const uint8_t> bytes) {
  // TODO(fzakaria): Only support x86_64 for now
  const uint64_t address = 0x400000;
  Disassembler disassembler(Disassembler::Architecture::x86_64);
  const std::vector<Disassembler::Instruction> instructions =
      disassembler.disassemble(address, bytes);

  // Begin transaction
  SQLite::Transaction transaction(db);

  uint rows = 0;
  const std::string sql = loadSqlFile("insert/insert_instruction.sql");
  for (const auto& instruction : instructions) {
    SQLite::Statement insert{db, sql};
    insert.bind(1, instruction.mnemonic);
    insert.bind(2, instruction.op_str);
    // https://github.com/SRombauts/SQLiteCpp/issues/430
    const int lengthInBytes =
        static_cast<int>(sizeof(uint8_t) * instruction.raw.size());
    insert.bind(3, instruction.raw.data(), lengthInBytes);
    rows += insert.exec();
    insert.reset();
  }

  // Commit transaction
  transaction.commit();

  // Verify all the instructions were inserted
  if (rows != instructions.size()) {
    throw std::runtime_error(absl::StrFormat(
        "Failed to insert all instructions into database: %d/%d", rows,
        instructions.size()));
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
bool DatabaseManager::IsSQLiteDatabase(const std::string& filename) {
  {
    try {
      const SQLite::Database db(filename, SQLite::OPEN_READONLY);
      return true;
    } catch (const std::exception& e) {
      return false;
    }
  }
}
