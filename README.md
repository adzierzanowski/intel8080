# Intel 8080 emulator

I decided to write an Intel 8080 emulator as my pet project.

# Usage

This project consists of two main parts:
  * `i8080asm` which is an assembler written in Python
  * `i8080emu` which is an emulator written in C++.

Assembling a program:
    $ i8080asm program.s

Execute it:
    $ i8080emu program.bin

To suppress any output not coming from the assembled program itself, use `-q` switch.
To print all register values upon every instruction executed, use `-v` switch.

# Building

To build, just call

    $ make

It may be bugged but works for me.

# Resources

https://en.wikipedia.org/wiki/Intel_8080

http://pastraiser.com/cpu/i8080/i8080_opcodes.html

http://www.classiccmp.org/dunfield/r/8080.txt

http://altairclone.com/downloads/manuals/8080%20Programmers%20Manual.pdf

https://svofski.github.io/pretty-8080-assembler/

https://www.seasip.info/Cpm/bdos.html

https://svofski.github.io/pretty-8080-assembler/

http://www.emulator101.com

# Todo

* Interpreter mode
* Test coverage
* Make test ROMs work
* Nicer setFlags
