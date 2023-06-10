#include "disassembler.h"

#include "absl/log/check.h"
#include "absl/strings/str_format.h"

Disassembler::~Disassembler() {
  cs_free(capstone_instruction, 1);
  CHECK_EQ(cs_close(&cs_handle), CS_ERR_OK);
}

Disassembler::Disassembler(Disassembler::Architecture arch) {
  switch (arch) {
    case Disassembler::Architecture::x86_64:
      this->arch = CS_ARCH_X86;
      this->mode = CS_MODE_64;
      break;
    default:
      throw std::runtime_error(
          absl::StrFormat("Unsupported architecture: %d", arch));
  }

  CHECK_EQ(cs_open(this->arch, this->mode, &cs_handle), CS_ERR_OK);

  // allocate the memory for a single capstone instruction when streaming
  capstone_instruction = cs_malloc(cs_handle);
}

std::vector<Disassembler::Instruction> Disassembler::disassemble(
    uint64_t address, absl::Span<const uint8_t> bytes) {
  // Create a raw C memory buffer for capstone to read from
  size_t size = bytes.size();
  const uint8_t *raw_memory = bytes.data();

  std::vector<Disassembler::Instruction> instructions;
  while (cs_disasm_iter(cs_handle, &raw_memory, &size, &address,
                        capstone_instruction)) {
    const Disassembler::Instruction instruction = {
        .mnemonic = capstone_instruction->mnemonic,
        .op_str = capstone_instruction->op_str,
        .raw = std::vector<uint8_t>(
            capstone_instruction->bytes,
            capstone_instruction->bytes + capstone_instruction->size),
    };
    instructions.push_back(instruction);
  }

  return instructions;
}
