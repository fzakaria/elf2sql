-- typedef struct {
--         unsigned char   e_ident[EI_NIDENT];
--         Elf64_Half      e_type;
--         Elf64_Half      e_machine;
--         Elf64_Word      e_version;
--         Elf64_Addr      e_entry;
--         Elf64_Off       e_phoff;
--         Elf64_Off       e_shoff;
--         Elf64_Word      e_flags;
--         Elf64_Half      e_ehsize;
--         Elf64_Half      e_phentsize;
--         Elf64_Half      e_phnum;
--         Elf64_Half      e_shentsize;
--         Elf64_Half      e_shnum;
--         Elf64_Half      e_shstrndx;
-- } Elf64_Ehdr;
CREATE TABLE Header
(
    id        INTEGER PRIMARY KEY,
    e_type    INTEGER, -- Object file type (ET_REL, ET_EXEC, etc.)
    e_machine INTEGER, -- Architecture (EM_386, EM_X86_64, etc.)
    e_version INTEGER, -- Object file version
    -- Only relevant in a binary file
    -- e_entry   INTEGER, -- Entry point virtual address
    -- Only relevant in a binary file
    -- e_phoff      INTEGER, -- Program header table file offset
    -- Only relevant in a binary file
    --  e_shoff    INTEGER, -- Section header table file offset
    e_flags   INTEGER  -- Processor-specific flags
    -- Only relevant in a binary file
    -- Only relevant in a binary file
    -- e_phentsize INTEGER, -- Program header table entry size
    -- e_phnum     INTEGER, -- Program header table entry count
    -- e_shentsize INTEGER, -- Section header table entry size
    -- e_shnum     INTEGER, -- Section header table entry count
    -- e_shstrndx INTEGER, -- Section header string table index
);