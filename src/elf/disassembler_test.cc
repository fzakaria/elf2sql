#include "disassembler.h"

#include <vector>

#include "gtest/gtest.h"

TEST(DisassemblerTest, BasicDisassembly) {
  Disassembler disassembler(Disassembler::Architecture::x86_64);

  const char* code = "\x55\x48\x8b\x05\xb8\x13\x00\x00";
  const size_t size = 8;
  const std::vector<uint8_t> bytes(code, code + size);

  /*
   * Expected (from https://www.capstone-engine.org/lang_c.html):
   * push rbp
   * mov rax, qword ptr [rip + 0x13b8]
   */
  const std::vector<Disassembler::Instruction> instructions =
      disassembler.disassemble(0, bytes);
  ASSERT_EQ(instructions.size(), 2);
  ASSERT_EQ(instructions[0].mnemonic, "push");
  ASSERT_EQ(instructions[1].mnemonic, "mov");
}
