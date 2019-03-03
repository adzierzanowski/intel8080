.org 0x100

mvi c, 9
lxi d, msg
syscall

; c_readstr
mvi c, 0x0a
lxi d, 0x1000
syscall

; print 32 bytes of RAM at [0x1000]
mvi c, 0xfe
lxi d, 0x1000
lxi h, 0x0020
syscall

mvi c, 0
syscall

msg:
  db "Write some words: $"
