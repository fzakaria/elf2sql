"""Capstone is a lightweight multi-platform, multi-architecture disassembly

Original source from https://github.com/google/silifuzz/blob/main/third_party/BUILD.capstone
"""
package(
    default_visibility = ["//visibility:public"],
)

cc_library(
    name = "capstone",
    srcs = [
        "LEB128.h",
        "MCDisassembler.h",
        "MCFixedLenDisassembler.h",
        "MCInst.c",
        "MCInst.h",
        "Mapping.h",
        "Mapping.c",
        "MCInstrDesc.c",
        "MCInstrDesc.h",
        "MCRegisterInfo.c",
        "MCRegisterInfo.h",
        "MathExtras.h",
        "SStream.c",
        "SStream.h",
        "cs.c",
        "cs_priv.h",
        "utils.c",
        "utils.h",
    ] + glob([
        "arch/**/*.c",
        "arch/**/*.h",
        "arch/**/*.inc",
    ]),
    # Affect external dependencies.
    strip_include_prefix = "include",
    hdrs = [":capstone-headers"],
    # https://github.com/capstone-engine/capstone/blob/master/COMPILE_CMAKE.TXT
    copts = [
        # Disable all warnings
        "-w",
        "-DCAPSTONE_HAS_ARM",
        "-DCAPSTONE_HAS_ARM64",
        "-DCAPSTONE_HAS_BPF",
        "-DCAPSTONE_HAS_EVM",
        "-DCAPSTONE_HAS_M680X",
        "-DCAPSTONE_HAS_M68K",
        "-DCAPSTONE_HAS_MIPS",
        "-DCAPSTONE_HAS_MOS65XX",
        "-DCAPSTONE_HAS_POWERPC",
        "-DCAPSTONE_HAS_RISCV",
        "-DCAPSTONE_HAS_SPARC",
        "-DCAPSTONE_HAS_SYSZ",
        "-DCAPSTONE_HAS_TMS320C64X",
        "-DCAPSTONE_HAS_WASM",
        "-DCAPSTONE_HAS_X86",
        "-DCAPSTONE_HAS_XCORE",
        "-DCAPSTONE_USE_SYS_DYN_MEM",
        "-Iexternal/capstone/include",
    ],
)

filegroup(
    name = "capstone-headers",
    srcs = glob(["include/capstone/*.h"]),
)
