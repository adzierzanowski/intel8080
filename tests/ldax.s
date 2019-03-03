.org 0x100

lxi b, bdata
lxi d, ddata

ldax b
mov b, a
ldax d

bdata:
  db 0xbb
ddata:
  db 0xaa
