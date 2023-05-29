#!/bin/bash

set -euo pipefail

# Grab the ELFIO gtest binary.
# We will be changing directory so we grab the absolute path
readonly ELFIOTest=$(readlink -f "$1")
readonly DATA_DIRECTORY=$(dirname "$2")
readonly RUNNING_DIRECTORY=$(dirname "$DATA_DIRECTORY")

# We now need to run the binary wth the data directory relative to it
cd "$RUNNING_DIRECTORY"

# Run the tests
"${ELFIOTest}"