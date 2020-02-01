# Intel 8080 emulator

[![Build Status](https://travis-ci.com/adzierzanowski/intel8080.svg?branch=master)](https://travis-ci.com/adzierzanowski/intel8080)
[![codecov](https://codecov.io/gh/adzierzanowski/intel8080/branch/master/graph/badge.svg)](https://codecov.io/gh/adzierzanowski/intel8080)


This is a work in progress.

# Building

```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
```

or you can just use a Makefile wrapper over cmake:

```bash
$ make # make emulator
$ make tests # build tests
```

# TODO

1. emulator
  * complete instruction set
  * parralel data port emulation
2. interpreter
3. assembler
4. C compiler
5. GUI/debugger
