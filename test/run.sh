#!/bin/bash

cd "$(dirname "$0")"
here="$(pwd)"

if [ "$#" -lt 1 ]; then
  echo "Usage: run.sh /path/to/hap [test-name]" >&2
  exit 1
fi

HAP="$1"
shift

function run_test {

  set +e +E

  test_file="$1"
  test_name="$(basename "$test_file" ".hap")"
  actual_out="$here/$test_name.out.actual"
  expect_out="$here/$test_name.out.expect"
  actual_err="$here/$test_name.err.actual"
  expect_err="$here/$test_name.err.expect"

  if [ ! -e "$expect_err" ]; then
    expect_err=/dev/null
  fi

  "$HAP" "$test_file" > "$actual_out" 2> "$actual_err"

  if [ ! -e "$expect_out" ]; then
    echo "Test '$test_name' BROKEN." >&2
    echo "Expected positive test output ($expect_out) not found." >&2
    exit 1
  fi

  diff -u "$expect_err" "$actual_err"
  if [ "$?" != 0 ]; then
    echo "Test '$test_name' FAILED." >&2
    echo "Negative test output does not match expected." >&2
    echo
    echo "Expected:" >&2
    cat "$expect_err" >&2
    echo
    echo "Actual:" >&2
    cat "$actual_err" >&2
    echo
    exit 1
  fi

  diff -u "$expect_out" "$actual_out"
  if [ "$?" != 0 ]; then
    echo "Test '$test_name' FAILED." >&2
    echo "Positive test output does not match expected." >&2
    exit 1
  fi

  set -e -E

}

if [ ! -e "$HAP" ]; then
  echo "Unable to run tests; missing 'hap' executable." >&2
  exit 1
fi

if [ $# -gt 0 ]; then
  for test in "$@"; do
    run_test "./$test.hap"
  done
else
  find . -maxdepth 1 -name '*.hap' | while read test_file; do
    run_test "$test_file"
  done
fi
