#!/bin/bash
set -euo pipefail

# This is the most basic test that verifies we can do an identify function
# between the two commands. This is a good sanity check that the commands
# produce a similar output.
# Note: We say similar since the produced ELF may be different in layout or
# sections but will functionally be the same.

readonly ELF2SQL="$1"
readonly SQL2ELF="$2"

${ELF2SQL} /usr/bin/ls
${SQL2ELF} ls.db

ls.elf --help
