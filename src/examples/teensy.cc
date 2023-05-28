#include <elf.h>
#include <fcntl.h>
#include <unistd.h>

#include <cstdio>

/**
 * This is the program code that will be written to the ELF file.
 * All it does is call the exit system call with the value 42.
 */
const uint8_t programCode[] = {
    0xb8, 0x01, 0x00, 0x00, 0x00,  // mov    eax, 0x1
    0xbb, 0x2a, 0x00, 0x00, 0x00,  // mov    ebx, 0x2a
    0xcd, 0x80};                   // int    0x80
/**
 * This function creates the smallest possible ELF executable file for Linux.
 * It is based on the excellent blog post
 * https://www.muppetlabs.com/~breadbox/software/tiny/teensy.html which
 * accomplishes the same thing except via assembly.
 *
 * Shout-out to GPT4 which also helped write this file.
 */
int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <filename>\n", argv[0]);
    return 1;
  }

  // ELF header
  Elf64_Ehdr elfHeader = {
      .e_ident =
          {
              ELFMAG0, ELFMAG1, ELFMAG2,
              ELFMAG3,  // Magic number identifying ELF file format
              ELFCLASS64, ELFDATA2LSB,
              EV_CURRENT,  // ELF class, data encoding, and version
              ELFOSABI_NONE, 0, 0, 0, 0, 0, 0,
              0  // OS-specific identification (none in this case)
          },
      .e_type = ET_EXEC,        // Executable file type
      .e_machine = EM_X86_64,   // Target machine architecture
      .e_version = EV_CURRENT,  // ELF version
      // x86-64 virtual address entry point is set to 0x400000
      // by ld. Let's use the same value here just to mimic it.
      // We also offset the ELF header and program header table as the machine
      // code will be written to the file after them.
      .e_entry = 0x400000 + sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr),
      .e_phoff = sizeof(Elf64_Ehdr),  // Offset of the program header table
      .e_shoff = 0,  // Offset of the section header table (none in this case)
      .e_flags = 0,  // Processor-specific flags
      .e_ehsize = sizeof(Elf64_Ehdr),     // Size of ELF header
      .e_phentsize = sizeof(Elf64_Phdr),  // Size of program header entry
      .e_phnum = 1,                       // Number of program header entries
      .e_shentsize = 0,  // Size of section header entry (none in this case)
      .e_shnum = 0,      // Number of section header entries (none in this case)
      .e_shstrndx = 0  // Index of section header entry for section name strings
                       // (none in this case)
  };

  // Program header
  Elf64_Phdr programHeader = {
      .p_type = PT_LOAD,  // Type of the program header entry (loadable segment)
      .p_flags = PF_R | PF_X,  // Segment flags (readable and executable)
      .p_offset = 0,           // Offset of the segment in the file
      // Virtual address at which the segment should be loaded in memory
      // We set it to 0x400000 to mimic ld.
      .p_vaddr = 0x400000,
      // Physical address at which the segment should be loaded. We don't
      // care about physical addresses so we set it to 0.
      .p_paddr = 0,
      // This field represents the size of the segment in the file. It indicates
      // the number of bytes occupied by the segment's data within the ELF file
      // itself. The p_filesz field includes any padding or alignment bytes, as
      // well as the actual data of the segment.
      .p_filesz = sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr) + sizeof(programCode),
      // This field represents the size of the segment in memory. It indicates
      // the total memory required by the segment when loaded into memory during
      // program execution. The p_memsz field encompasses the entire space
      // allocated for the segment in memory, including any padding, alignment,
      // or additional memory allocated for uninitialized portions.
      // Typically, p_filesz will be less than or equal to p_memsz, as p_filesz
      // represents the actual occupied size of the segment in the file, while
      // p_memsz represents the size needed in memory to accommodate the
      // segment's data.
      .p_memsz = sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr) + sizeof(programCode),
      //  This field represents the alignment requirements for the segment. It
      //  specifies the minimum alignment, in bytes, that the segment's data
      //  should adhere to when loaded into memory.
      // If you need to indicate that no specific alignment requirement is
      // necessary for a segment, it is common to set p_align to 1. This means
      // that the segment's data does not have any strict alignment requirements
      // and can be loaded at any address.
      .p_align = 1};

  // Create and write the ELF file
  int fd =
      open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IXUSR);
  if (fd == -1) {
    perror("open");
    return 1;
  }

  // Write ELF header to the file
  ssize_t written = write(fd, &elfHeader, sizeof(Elf64_Ehdr));
  if (written == -1) {
    perror("write");
    close(fd);
    return 1;
  }
  // Write program header to the file
  written = write(fd, &programHeader, sizeof(Elf64_Phdr));
  if (written == -1) {
    perror("write");
    close(fd);
    return 1;
  }
  // Write program code to the file
  written = write(fd, programCode, sizeof(programCode));
  if (written == -1) {
    perror("write");
    close(fd);
    return 1;
  }

  close(fd);
  return 0;
}
