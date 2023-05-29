cc_library(
    name = "SQLiteCpp",
    srcs = glob([
        "src/*.cpp",
    ]),
    hdrs = glob([
        "include/SQLiteCpp/*.h",
    ]),
    copts = [
        "-Iexternal/sqlite3",
    ],
    strip_include_prefix = "include",
    deps = [
        "@sqlite3",
    ],
    visibility = ["//visibility:public"],
)
