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

${ELF2SQL} /usr/bin/ls
${SQL2ELF} ls.db

ls.elf --help
