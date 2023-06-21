#!/bin/bash
set -euo pipefail

# --- begin runfiles.bash initialization v3 ---
# Copy-pasted from the Bazel Bash runfiles library v3.
set -uo pipefail; set +e; f=bazel_tools/tools/bash/runfiles/runfiles.bash
source "${RUNFILES_DIR:-/dev/null}/$f" 2>/dev/null || \
source "$(grep -sm1 "^$f " "${RUNFILES_MANIFEST_FILE:-/dev/null}" | cut -f2- -d' ')" 2>/dev/null || \
source "$0.runfiles/$f" 2>/dev/null || \
source "$(grep -sm1 "^$f " "$0.runfiles_manifest" | cut -f2- -d' ')" 2>/dev/null || \
source "$(grep -sm1 "^$f " "$0.exe.runfiles_manifest" | cut -f2- -d' ')" 2>/dev/null || \
{ echo>&2 "ERROR: cannot find $f"; exit 1; }; f=; set -e
# --- end runfiles.bash initialization v3 ---

# This is the most basic test that verifies we can do an identify function
# between the two commands. This is a good sanity check that the commands
# produce a similar output.
# Note: We say similar since the produced ELF may be different in layout or
# sections but will functionally be the same.

source ./tools/bash/unittest.bash || exit 1

readonly ELF2SQL="$(rlocation __main__/src/cli/elf2sql)"
readonly SQL2ELF="$(rlocation __main__/src/cli/sql2elf)"
readonly EXAMPLES_DIR="$(dirname "$(rlocation __main__/src/examples/teensy)")"

# TODO(fzakaria): This is shared from //tools/build_defs/test/test_return_code.sh
# and it should be shared in a common library.
function assert_command_outcome() {
  binary="$1"
  expected_outcome="$2"
  ($binary && result_code=$?) || result_code=$?
  if [ "$result_code" -ne 42 ]; then
    fail "Expected return code ${expected_outcome}, but got $?."
  fi
}

# This test assumes all the examples return 42 as their status
# it does no functionality test aside from the return code
# TODO(fzakaria) find a way to create multiple functions for each test example
function test_examples_directory_simple() {
  for example_path in "$EXAMPLES_DIR"/*; do
    echo "Testing example: $example_path"
    ${ELF2SQL} "$example_path" || fail "elf2sql failed"

    sqlite_db="$(basename "$example_path").db"
    elf_file="$(basename "$example_path").elf"

    ${SQL2ELF} "$sqlite_db"|| fail "sql2elf failed"
    assert_command_outcome "$elf_file" 42
  done
}

run_suite "Test suite for identity tests"