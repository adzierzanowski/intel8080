.org 0x100

MVI c, 9
lxi d, msg
syscall

msg: db "Hello, world", 10, "$"

rst 0