cc_library(
    name = "elfio",
    hdrs = ["elfio/elfio.hpp"],
    # We remove all the other headers
    # because they are all includes in the elfio.hpp file
    # and therefore are textual in that sense.
    textual_hdrs = glob(
        ["elfio/*.hpp"],
        exclude = ["elfio/elfio.hpp"],
    ),
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "elfio_test_bin",
    srcs = glob(["tests/*.cpp"]),
    deps = [
        ":elfio",
        "@com_google_googletest//:gtest_main",
    ],
    copts = [
        "-Iexternal/elfio",
    ],
    testonly = True,
)

sh_test(
    name = "elfio_test",
    srcs = ["@//third_party/elfio:run_elfio_test.sh"],
    args = [
        "$(rootpath :elfio_test_bin)",
        # Pick any file in the elf_examples to pass the data directory
        "$(rootpath tests/elf_examples/asm)",
    ],
    data = [":elfio_test_bin"] + glob(["tests/elf_examples/**"]),
)
