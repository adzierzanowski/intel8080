.org 0x100

mvi c, 9
lxi d, msg
call 0x0005

mvi c, 0
call 0x0005

msg:
  db "Hello, world", 10, "$"
