#include <memory>
#include <string>

#include "LIEF/ELF.hpp"
#include "SQLiteCpp/Database.h"
#include "SQLiteCpp/Statement.h"
#include "absl/log/log.h"
#include "absl/strings/str_format.h"
#include "src/cli/common.h"
#include "src/utils/path.h"

void convertElfToSQLite(const std::string& elfFile,
                        const std::string& sqliteFile) {
  // Load the ELF file
  std::unique_ptr<LIEF::ELF::Binary> const binary =
      LIEF::ELF::Parser::parse(elfFile);

  // Create a SQLite database and table
  SQLite::Database db(sqliteFile, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

  // Creat the simplest sqlite database to hold the contents of the binary
  db.exec(R"(
    CREATE TABLE IF NOT EXISTS binary (id INTEGER PRIMARY KEY AUTOINCREMENT,
    content BLOB);
      )");

  // Insert the binary contents into the database raw
  // This is the most basic conversion possible
  SQLite::Statement insert(db, "INSERT INTO binary (content) VALUES (?)");
  const int lengthInBytes =
      static_cast<int>(sizeof(uint8_t) * binary->raw().size());
  insert.bind(1, binary->raw().data(), lengthInBytes);
  const int rowsCreated = insert.exec();
  if (rowsCreated == 0) {
    throw std::runtime_error("Failed to insert binary into database");
  }
}

int main(int argc, char** argv) {
  InitGoogle(&argc, &argv, "Convert ELF to SQLite databases");

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

  try {
    convertElfToSQLite(elf_filename, database_filename);
  } catch (const std::exception& e) {
    LOG(ERROR) << "Failed to convert ELF to SQLite: " << e.what();
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
