
.org 0x100 ; comment

mvi a, 4
stc

label:
  mvi c, 9
  lxi d, msg
  call 0x0005
  dcr  a
  cpi 0
  jnz    label

Mov C, B
call 0005h

msg:
  db "Hello, world", 10, 12, "$", 22

