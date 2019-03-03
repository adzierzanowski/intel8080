# Intel 8080 emulator

I decided to write an Intel 8080 emulator as my pet project.

At first I tried to write it in Python3 but quickly moved to C++.

The thing you see is a quick draft that took two days of coding. I hope I'll find some more time to make it pretty and functional. Right now it's a mess and crashes when I load downloaded binaries. At least it prints out messages well.

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

* Make an assembler
* Interpreter mode
* Test coverage
* Make test ROMs work
* More syscalls
* Nicer setFlags
