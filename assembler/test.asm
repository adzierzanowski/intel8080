
.org 0x100 ; comment

mvi a, 4

label:
  mvi c, 9
  mvi     e,  msg
  call 0x0005
  dcr  a
  cpi 0
  jnz    label

MVI C, 0
call 0005h

msg: db "Hello, world", 10, '$'

