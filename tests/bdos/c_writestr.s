.org 0x100

; set string delimiter to a
mvi c, 0x6e
mvi e, 0x61
syscall

; print string
mvi c, 9
lxi d, msg
syscall

mvi c, 0
syscall

msg:
  db "Some string with an a in it"

