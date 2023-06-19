cc_library(
    name = "libdwarf",
    srcs = glob([
        "libdwarf/*.c",
    ]) + [
        "config.h",
    ],
    hdrs = glob([
        "libdwarf/*.h",
    ]),
    copts = [
        "-Ibazel-out/k8-dbg/bin/external/libdwarf ",
        "-DHAVE_CONFIG_H",
    ],
    strip_include_prefix = "libdwarf",
    includes = [
        "libdwarf",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "@elfutils//:elf",
    ],
)

genrule(
    name = "config_h",
    srcs = [
        "configure",
        "install-sh",
        "config.guess",
        "config.sub",
        "configure.ac",
        "ltmain.sh",
        "ar-lib",
        "compile",
        "missing",
    ] + glob(["**/*.in"]),
    outs = ["config.h"],
    cmd = "./$(location configure) " +
          "&& cp config.h $(location config.h)",
)
