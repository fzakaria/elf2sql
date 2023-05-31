cc_library(
    name = "sqlite3",
    srcs = ["sqlite3.c"],
    hdrs = [
        "sqlite3.h",
        "sqlite3ext.h",
    ],
    defines = ["SQLITE_OMIT_DEPRECATED"],
    local_defines = [
        "SQLITE_DQS=0",
        "SQLITE_OMIT_DEPRECATED",
    ],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "shell",
    srcs = ["shell.c"],
    visibility = ["//visibility:public"],
    deps = [
        ":sqlite3",
    ],
    copts = [
        # suppress all warnings
        "-w",
    ],
)
