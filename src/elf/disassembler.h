#pragma once

#include <string>
#include <vector>

#include "capstone/capstone.h"
/**
 * A friendly wrapper class around Capstone disassembler.
 * ref:
 * https://github.com/google/silifuzz/blob/main/tracing/capstone_disassembler.h
 */
class Disassembler {
 public:
  enum class Architecture {
    x86_64,
    Unknown,
  };

  class Instruction {
   public:
    std::string mnemonic;
    std::string op_str;
    std::vector<uint8_t> raw;
    std::string fullText() const { return mnemonic + " " + op_str; }

    friend std::ostream& operator<<(std::ostream& os, const Instruction& ins) {
      os << ins.fullText();
      return os;
    }
  };

  explicit Disassembler(Architecture arch);
  virtual ~Disassembler();

  /**
   * Disassemble bytes to instructions
   * @param address The address of the first instruction
   * @param bytes The raw bytes of the single instruction
   */
  std::vector<Instruction> disassemble(uint64_t address,
                                       const std::vector<uint8_t>& bytes);

 private:
  csh cs_handle{};
  // https://www.capstone-engine.org/iteration.html
  cs_insn* capstone_instruction;
  cs_arch arch;
  cs_mode mode;
};
