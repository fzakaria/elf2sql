#include <sys/stat.h>

#include <fstream>
#include <memory>

#include "SQLiteCpp/Database.h"
#include "absl/log/log.h"
#include "absl/strings/str_format.h"
#include "elfio/elfio.hpp"
#include "sqlite3.h"
#include "src/cli/common.h"
#include "src/sql/manager.h"
#include "src/utils/path.h"

// https://stackoverflow.com/a/30606613
std::vector<uint8_t> HexToBytes(const std::string& hex) {
  std::vector<uint8_t> bytes;

  for (unsigned int i = 0; i < hex.length(); i += 2) {
    const std::string byteString = hex.substr(i, 2);
    const auto byte =
        static_cast<uint8_t>(strtol(byteString.c_str(), nullptr, 16));
    bytes.push_back(byte);
  }

  return bytes;
}

void convertSQLiteToElf(const std::string& sqliteFile,
                        const std::string& elfFile) {
  // Create a SQLite database and table
  SQLite::Database db(sqliteFile, SQLite::OPEN_READONLY);

  // Fetch the binary data from the database
  // This assumes that there is only one entry at the moment
  const SQLite::Column column = db.execAndGet(R"(
    SELECT CONTENT FROM binary LIMIT 1
      )");
  if (!column.isBlob()) {
    throw std::runtime_error("Expected binary blob result.");
  }

  const char* data = reinterpret_cast<const char*>(column.getBlob());
  const int size = column.getBytes();

  std::ofstream file(elfFile, std::ios::binary);
  // Write the data to the file
  file.write(data, size);
  // Close the file
  file.close();

  const SQLite::Column instructionsColumn =
      db.execAndGet("select group_concat(hex(raw), '') from Instructions;");
  const std::string instructionsAsHex = instructionsColumn.getString();
  const std::vector<uint8_t> bytes = HexToBytes(instructionsAsHex);

  // Beginning of real ELF writer
  ELFIO::elfio writer;

  // You can choose the file class: ELFCLASS32 for 32 bit or ELFCLASS64 for 64
  // bit
  writer.create(ELFCLASS64, ELFDATA2LSB);

  writer.set_os_abi(ELFOSABI_SYSV);
  writer.set_type(ET_EXEC);
  writer.set_machine(EM_X86_64);

  // Create a text section
  ELFIO::section* text_sec = writer.sections.add(".text");
  text_sec->set_type(SHT_PROGBITS);
  text_sec->set_flags(SHF_ALLOC | SHF_EXECINSTR);
  text_sec->set_addr_align(1);
  text_sec->set_data(reinterpret_cast<const char*>(bytes.data()), bytes.size());

  // Create a load segment
  ELFIO::segment* load_seg = writer.segments.add();
  load_seg->set_type(PT_LOAD);
  load_seg->set_virtual_address(0x400000);
  load_seg->set_physical_address(0x400000);
  load_seg->set_flags(PF_X | PF_R);
  load_seg->set_align(1);
  load_seg->add_section_index(text_sec->get_index(),
                              text_sec->get_addr_align());
  // Set the entry point (address where the program starts executing)
  writer.set_entry(load_seg->get_virtual_address());

  // Finally, write it to a file
  writer.save("/tmp/sample.elf");

  // TODO(fzakaria): This doesn't only add executable but read + write also
  if (chmod(elfFile.c_str(), S_IRUSR | S_IWUSR | S_IXUSR) != 0) {
    throw std::runtime_error(
        absl::StrFormat("Failed to set executable permissions on %s: %s",
                        elfFile, strerror(errno)));
  }
}

int main(int argc, char** argv) {
  InitGoogle(&argc, &argv, "Convert SQLite database back to to ELF");

  if (argc != 2) {
    LOG(ERROR) << "You must include exactly one filename.";
    return EXIT_FAILURE;
  }

  const std::string sqlite_filename = argv[1];
  if (!DatabaseManager::IsSQLiteDatabase(sqlite_filename)) {
    LOG(ERROR) << "The file you provided is not a SQLite file.";
    return EXIT_FAILURE;
  }

  std::string elf_filename =
      replace_file_extension(basename(sqlite_filename), "elf");
  // TODO(fzakaria): figure out a better way for this
  //  this allows bazel run to create files where you invoke it.
  const char* bazelWorkingDirectory = std::getenv("BUILD_WORKING_DIRECTORY");
  if (bazelWorkingDirectory != nullptr) {
    elf_filename =
        absl::StrFormat("%s/%s", bazelWorkingDirectory, elf_filename);
  }

  try {
    convertSQLiteToElf(sqlite_filename, elf_filename);
  } catch (const std::exception& e) {
    LOG(ERROR) << "Failed to convert SQLite database to ELF: " << e.what();
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}