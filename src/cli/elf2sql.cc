#include <memory>

#include "SQLiteCpp/Database.h"
#include "SQLiteCpp/Statement.h"
#include "absl/log/log.h"
#include "absl/strings/str_format.h"
#include "src/cli/common.h"
#include "src/sql/manager.h"
#include "src/utils/path.h"
// This has to be included at the end because it undefs
// some macros that are defined in lib/include/elf.h
// clang-format off
#include "LIEF/ELF.hpp"
// clang-format on

void convertElfToSQLite(const std::string& elfFile,
                        const std::string& sqliteFile) {
  // Load the ELF file
  std::unique_ptr<LIEF::ELF::Binary> const binary =
      LIEF::ELF::Parser::parse(elfFile);

  // Create a SQLite database and table
  SQLite::Database db(sqliteFile, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

  DatabaseManager manager(db);
  // Create the schema for the database
  manager.createDatabaseSchema();

  // Creat the simplest sqlite database to hold the contents of the binary
  db.exec(R"(
    CREATE TABLE IF NOT EXISTS binary (id INTEGER PRIMARY KEY AUTOINCREMENT,
    content BLOB);
      )");

  // Insert the binary contents into the database raw
  // This is the most basic conversion possible
  SQLite::Statement insert(db, "INSERT INTO binary (content) VALUES (?)");
  // https://github.com/SRombauts/SQLiteCpp/issues/430
  const int lengthInBytes =
      static_cast<int>(sizeof(uint8_t) * binary->raw().size());
  insert.bind(1, binary->raw().data(), lengthInBytes);
  const int rowsCreated = insert.exec();
  if (rowsCreated == 0) {
    throw std::runtime_error("Failed to insert binary into database");
  }

  // Insert the ELF header into the database
  const InsertElfHeaderRequest request{
      .e_type = static_cast<int>(binary->header().file_type()),
      .e_machine = static_cast<int>(binary->header().machine_type()),
      .e_flags = static_cast<int>(binary->header().processor_flag()),
  };
  manager.insertHeader(request);

  const auto& it =
      std::find_if(binary->segments().begin(), binary->segments().end(),
                   [](const auto& segment) {
                     return segment.has(LIEF::ELF::ELF_SEGMENT_FLAGS::PF_X);
                   });
  if (it == it.end()) {
    throw std::runtime_error(
        "Expected exactly one loadable executable segment");
  }

  const LIEF::ELF::Segment& segment = *it;
  auto span = segment.content();
  // some executables like teensy include the ELF header itself as part
  // of the executable segment. Let's remove that header from the offset
  const uint64_t offset = binary->entrypoint() - segment.virtual_address();
  manager.insertInstructions(span.subspan(offset));
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

  std::string database_filename = utils::path::ReplaceFileExtension(
      utils::path::Basename(elf_filename), "db");
  // TODO(fzakaria): figure out a better way for this
  //  this allows bazel run to create files where you invoke it.
  const char* bazelWorkingDirectory = std::getenv("BUILD_WORKING_DIRECTORY");
  if (bazelWorkingDirectory != nullptr) {
    database_filename =
        absl::StrFormat("%s/%s", bazelWorkingDirectory, database_filename);
  }

  try {
    convertElfToSQLite(elf_filename, database_filename);
  } catch (const std::exception& e) {
    LOG(ERROR) << "Failed to convert ELF to SQLite: " << e.what();
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
