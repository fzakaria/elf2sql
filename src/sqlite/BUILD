package(
    default_visibility = [
        "//visibility:public",
    ],
)

cc_library(
    name = "database",
    srcs = [
        "database.cc",
    ],
    hdrs = [
        "database.h",
    ],
    deps = [
        "@com_google_absl//absl/strings:str_format",
        "@sqlite3",
    ],
)
