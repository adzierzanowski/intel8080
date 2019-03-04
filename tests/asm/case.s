.org 0x100

mvi a, 9
MVI b, 0
lxi d, msg
MOV c, a
mvi c, 9
syscall

mov c, b
syscall

msg: db "TeXt CaSe", 10, 36
