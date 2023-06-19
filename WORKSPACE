load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "rules_foreign_cc",
    sha256 = "2a4d07cd64b0719b39a7c12218a3e507672b82a97b98c6a89d38565894cf7c51",
    strip_prefix = "rules_foreign_cc-0.9.0",
    url = "https://github.com/bazelbuild/rules_foreign_cc/archive/refs/tags/0.9.0.tar.gz",
)

load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")

# This sets up some common toolchains for building targets. For more details, please see
# https://github.com/bazelbuild/rules_foreign_cc/tree/main/docs#rules_foreign_cc_dependencies
rules_foreign_cc_dependencies()

# Make all contents of an external repository accessible under a filegroup.  Used for external HTTP
# archives, e.g. cares.
BUILD_ALL_CONTENT = """filegroup(name = "all", srcs = glob(["**"]), visibility = ["//visibility:public"])"""

http_archive(
    name = "lief",
    build_file_content = BUILD_ALL_CONTENT,
    patch_args = ["-p1"],
    patches = ["//third_party/lief:undef_elf.patch"],
    sha256 = "8834e2ccfeefd1003527887357950173fe55e9a712004aa638af67378e28ef55",
    strip_prefix = "LIEF-0.13.0",
    url = "https://github.com/lief-project/LIEF/archive/refs/tags/0.13.0.tar.gz",
)

http_archive(
    name = "com_google_absl",
    sha256 = "51d676b6846440210da48899e4df618a357e6e44ecde7106f1e44ea16ae8adc7",
    strip_prefix = "abseil-cpp-20230125.3",
    urls = ["https://github.com/abseil/abseil-cpp/archive/refs/tags/20230125.3.zip"],
)

# GoogleTest/GoogleMock framework. Used by most unit-tests.
http_archive(
    name = "com_google_googletest",
    sha256 = "ffa17fbc5953900994e2deec164bb8949879ea09b411e07f215bfbb1f87f4632",
    strip_prefix = "googletest-1.13.0",
    urls = ["https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip"],
)

# Google benchmark.
http_archive(
    name = "com_github_google_benchmark",  # 2023-01-10T16:48:17Z
    sha256 = "ede6830512f21490eeea1f238f083702eb178890820c14451c1c3d69fd375b19",
    strip_prefix = "benchmark-a3235d7b69c84e8c9ff8722a22b8ac5e1bc716a6",
    urls = ["https://github.com/google/benchmark/archive/a3235d7b69c84e8c9ff8722a22b8ac5e1bc716a6.zip"],
)

# SQLite
http_archive(
    name = "sqlite3",
    build_file = "//third_party:sqlite.BUILD",
    sha256 = "01df06a84803c1ab4d62c64e995b151b2dbcf5dbc93bbc5eee213cb18225d987",
    strip_prefix = "sqlite-amalgamation-3410200",
    url = "https://www.sqlite.org/2023/sqlite-amalgamation-3410200.zip",
)

# ELFIO
http_archive(
    name = "elfio",
    build_file = "//third_party:elfio/elfio.BUILD",
    sha256 = "c372c1b5d90769c61cf1243fa2e8c1270094d311ee3bf58836ef54b072ba93a9",
    strip_prefix = "ELFIO-d251da09a07dff40af0b63b8f6c8ae71d2d1938d",
    url = "https://github.com/serge1/ELFIO/archive/d251da09a07dff40af0b63b8f6c8ae71d2d1938d.tar.gz",
)

# SQLiteCpp
http_archive(
    name = "SQLiteCpp",
    build_file = "//third_party:SQLiteCpp.BUILD",
    sha256 = "4fa61167484bad7220c4582f2d97475da46f364ff3b1b11078fec18db6bdfe33",
    strip_prefix = "SQLiteCpp-3.3.0",
    url = "https://github.com/SRombauts/SQLiteCpp/archive/refs/tags/3.3.0.zip",
)

# backward-cpp
http_archive(
    name = "backward-cpp",
    build_file = "//third_party/backward-cpp:backward-cpp.BUILD",
    sha256 = "42d9151d383f91813c12d06b0c76fd1caf6c6bfa07d1aa2e7ca8a775ad643bdc",
    strip_prefix = "backward-cpp-2395cfa2422edb71929c9d166a6a614571331db3",
    url = "https://github.com/bombela/backward-cpp/archive/2395cfa2422edb71929c9d166a6a614571331db3.tar.gz",
)

# Capstone
http_archive(
    name = "capstone",
    build_file = "//third_party:capstone.BUILD",
    sha256 = "a1d8557331c51d7bb9fdb23c8b0a7ed7d81a9c66b4cc9f0c2e3a951f63af77dd",
    strip_prefix = "capstone-a026fc4fe493a6f209e50f6fd8fd7aa3b6418c06",
    url = "https://github.com/capstone-engine/capstone/archive/a026fc4fe493a6f209e50f6fd8fd7aa3b6418c06.zip",
)

# Hedron's Compile Commands Extractor for Bazel
# https://github.com/hedronvision/bazel-compile-commands-extractor
http_archive(
    name = "hedron_compile_commands",
    sha256 = "3cd0e49f0f4a6d406c1d74b53b7616f5e24f5fd319eafc1bf8eee6e14124d115",
    strip_prefix = "bazel-compile-commands-extractor-3dddf205a1f5cde20faf2444c1757abe0564ff4c",
    url = "https://github.com/hedronvision/bazel-compile-commands-extractor/archive/3dddf205a1f5cde20faf2444c1757abe0564ff4c.tar.gz",
)

load("@hedron_compile_commands//:workspace_setup.bzl", "hedron_compile_commands_setup")

hedron_compile_commands_setup()

# NASM
http_archive(
    name = "nasm",
    build_file_content = BUILD_ALL_CONTENT,
    sha256 = "d833bf0f5716e89dbcd345b7f545f25fe348c6e2ef16dbc293e1027bcd22d881",
    strip_prefix = "nasm-2.16.01",
    url = "https://www.nasm.us/pub/nasm/releasebuilds/2.16.01/nasm-2.16.01.tar.gz",
)

# ELFUTILS
http_archive(
    name = "elfutils",
    build_file = "//third_party:elfutils.BUILD",
    sha256 = "39bd8f1a338e2b7cd4abc3ff11a0eddc6e690f69578a57478d8179b4148708c8",
    strip_prefix = "elfutils-0.189",
    url = "https://sourceware.org/elfutils/ftp/0.189/elfutils-0.189.tar.bz2",
)

# ZLIB
http_archive(
    name = "zlib",
    build_file = "//third_party:zlib.BUILD",
    sha256 = "c2856951bbf30e30861ace3765595d86ba13f2cf01279d901f6c62258c57f4ff",
    strip_prefix = "zlib-1.2.13",
    url = "https://github.com/madler/zlib/archive/refs/tags/v1.2.13.zip",
)

# LIBUNWIND
http_archive(
    name = "libunwind",
    build_file_content = BUILD_ALL_CONTENT,
    sha256 = "c24c913d2337d6eff851b6ab32aadfb683a86fee48d28fe1fc9cd56c8e9dfa58",
    strip_prefix = "libunwind-1.70",
    url = "https://github.com/libunwind/libunwind/releases/download/v1.7.0/libunwind-1.70.tar.gz",
)

# LIBDWARF
http_archive(
    name = "libdwarf",
    #build_file = "//third_party:libdwarf.BUILD",
    build_file_content = BUILD_ALL_CONTENT,
    sha256 = "4518de76a92af80919945dc4d37dd88414d3fae9116dc424f07f25e1ecc7d004",
    strip_prefix = "libdwarf-code-db0d0c4de28d3de217d79ed6ce6c8cf34cd017b9",
    urls = ["https://github.com/davea42/libdwarf-code/archive/db0d0c4de28d3de217d79ed6ce6c8cf34cd017b9.tar.gz"],
)

http_archive(
    name = "bazel_skylib",
    sha256 = "b8a1527901774180afc798aeb28c4634bdccf19c4d98e7bdd1ce79d1fe9aaad7",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/bazel-skylib/releases/download/1.4.1/bazel-skylib-1.4.1.tar.gz",
        "https://github.com/bazelbuild/bazel-skylib/releases/download/1.4.1/bazel-skylib-1.4.1.tar.gz",
    ],
)

load("@bazel_skylib//:workspace.bzl", "bazel_skylib_workspace")

bazel_skylib_workspace()

http_archive(
    name = "io_buildbuddy_buildbuddy_toolchain",
    sha256 = "e899f235b36cb901b678bd6f55c1229df23fcbc7921ac7a3585d29bff2bf9cfd",
    strip_prefix = "buildbuddy-toolchain-fd351ca8f152d66fc97f9d98009e0ae000854e8f",
    urls = ["https://github.com/buildbuddy-io/buildbuddy-toolchain/archive/fd351ca8f152d66fc97f9d98009e0ae000854e8f.tar.gz"],
)

load("@io_buildbuddy_buildbuddy_toolchain//:deps.bzl", "buildbuddy_deps")

buildbuddy_deps()

load("@io_buildbuddy_buildbuddy_toolchain//:rules.bzl", "UBUNTU20_04_IMAGE", "buildbuddy")

buildbuddy(
    name = "buildbuddy_toolchain",
    container_image = UBUNTU20_04_IMAGE,
    llvm = True,
)
