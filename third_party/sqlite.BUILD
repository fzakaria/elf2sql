cc_library(
    name = "sqlite3",
    srcs = ["sqlite3.c"],
    hdrs = [
        "sqlite3.h",
        "sqlite3ext.h",
    ],
    defines = ["SQLITE_OMIT_DEPRECATED"],
    visibility = ["//visibility:public"],
    local_defines = [
      "SQLITE_DQS=0",
      "SQLITE_OMIT_DEPRECATED"
    ]
)

cc_binary(
  name = "shell",
  srcs = [ "shell.c" ],
  deps = [
    ":sqlite3"
  ]
 )