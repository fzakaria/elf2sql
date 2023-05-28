#!/bin/bash

set -euo pipefail

readonly BINARY="$1"
readonly RETURN_CODE="$2"

#######################################
# A helper function for getting the return code of a command that may fail.
# Temporarily disables error safety and stores return value in TEST_STATUS.
# https://github.com/google/tink/blob/9bc2667963e20eb42611b7581e570f0dddf65a2b/cc/examples/mac/mac_cli_test.sh#LL32C1-L46C2
# Globals:
#   TEST_STATUS
# Arguments:
#   Command to execute.
#######################################
test_command() {
  set +e
  "$@"
  TEST_STATUS=$?
  set -e
}

#######################################
# Asserts that the outcome of the latest test command was the expected one.
#
# If not, it terminates the test execution.
# https://github.com/google/tink/blob/9bc2667963e20eb42611b7581e570f0dddf65a2b/cc/examples/mac/mac_cli_test.sh#L60
# Arguments:
#   expected_outcome: The expected outcome.
#######################################
assert_command_outcome() {
  expected_outcome="$1"
  if (( TEST_STATUS != expected_outcome )); then
      echo "[ FAILED ] Expected return code ${expected_outcome}, but got ${TEST_STATUS}."
      exit 1
  fi
}

test_command "${BINARY}"
assert_command_outcome "${RETURN_CODE}"