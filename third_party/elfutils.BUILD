LIBELF_HDRS = glob(["libelf/elf32_*.c"]) + [
    "lib/crc32.c",
    "lib/next_prime.c",
    "libelf/elf.h",
    "libelf/gelf.h",
    "libelf/libelf.h",
]

cc_library(
    name = "elf",
    srcs = glob([
        "libelf/*.c",
    ]) + [
        "config.h",
        "lib/eu-config.h",
        "lib/fixedsizehash.h",
        "lib/system.h",
        "libelf/abstract.h",
        "libelf/chdr_xlate.h",
        "libelf/common.h",
        "libelf/dl-hash.h",
        "libelf/elf-knowledge.h",
        "libelf/exttypes.h",
        "libelf/gelf_xlate.h",
        "libelf/gnuhash_xlate.h",
        "libelf/libelfP.h",
        "libelf/nlist.h",
        "libelf/note_xlate.h",
        "libelf/version_xlate.h",
    ],
    hdrs = LIBELF_HDRS,
    copts = [
        "-Iexternal/elfutils/libelf",
        "-Iexternal/elfutils/lib",
        "-I$(BINDIR)/external/elfutils",
        "-DHAVE_CONFIG_H",
        # Disable all warnings
        "-w",
    ],
    visibility = ["//visibility:public"],
    # Code seems to just include <libelf.h>
    includes = ["libelf"],
    deps = [
        "@zlib",
    ],
)

genrule(
    name = "config_h",
    srcs = [
        "configure",
        "libelf/libelf.h",
        "libdw/known-dwarf.h",
        "config/config.rpath",
        "config/ar-lib",
        "config/compile",
        "config/config.guess",
        "config/config.sub",
        "config/missing",
        "config/install-sh",
    ] + glob(["**/*.in"]),
    outs = ["config.h"],
    cmd = "./$(location configure) --disable-debuginfod --disable-libdebuginfod" +
          "&& cp config.h $(location config.h)",
)
