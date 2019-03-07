#!/usr/bin/env sh

TEST_SOURCES=$(ls -1 tests/*.s)


for test in $TEST_SOURCES; do
  echo $test;
  assembler/i8080asm $test -o $(echo $test | rev | cut -c 3- | rev)".bin" 
done
