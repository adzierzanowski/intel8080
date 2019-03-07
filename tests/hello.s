.org 100h

mvi c, 9
lxi d, msg
syscall

rst 0

msg:
  db "Hello, world", 10, "$"
