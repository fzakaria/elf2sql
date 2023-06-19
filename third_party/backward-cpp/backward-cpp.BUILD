cc_library(
    name = "backward-cpp",
    include_prefix = "backward",
    srcs = ["@//third_party/backward-cpp:backward.cpp"],
    hdrs = ["backward.hpp"],
    deps = [
        "@//third_party:libdwarf",
        "@elfutils//:elf",
        "@//third_party:libunwind",
    ],
    visibility = ["//visibility:public"],
    # Link any binary that depends (directly or indirectly) on this C++ library
    # in all the object files for the files listed in srcs, even if some contain
    # no symbols referenced by the binary.
    # Without this the backward.cpp file won't be included and then won't
    # initialize
    alwayslink = True,
    defines = [
        # For libdwarf
        "BACKWARD_HAS_DWARF=1",
        # For libunwind
        "BACKWARD_HAS_LIBUNWIND=1",
    ],
)
