#!/bin/bash
# Simple test to verify that the testing library works as expected
source ./tools/bash/unittest.bash || exit 1

function test_simple() {
  assert_equals 1 1
}

run_suite "unittest test suite"