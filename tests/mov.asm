.org 100h

errcntaddr equ 0xffff ; error count address

lxi sp, 0x1000 ; set stack pointer

mvi b, 0x00
mvi c, 0x01
mvi d, 0x02
mvi e, 0x03
mvi h, 0x04
mvi l, 0x05
mvi a, 0x07

mov b, b
mov a, b
cmp b
cnz incerrd
mov b, c
mov a, c
cmp b
cnz incerrd
mov b, d
mov a, d
cmp b
cnz incerrd
mov b, c
mov a, c
cmp b
cnz incerrd
mov b, d
mov a, d
cmp b
cnz incerrd
mov b, e
mov a, e
cmp b
cnz incerrd
mov b, h
mov a, h
cmp b
cnz incerrd
mov b, m
mov a, m
cmp b
cnz incerrd
mov b, a
cmp b
cnz incerrd

mov c, b
mov a, b
cmp c
cnz incerrd
mov c, c
mov a, c
cmp c
cnz incerrd
mov c, d
mov a, d
cmp c
cnz incerrd
mov c, c
mov a, c
cmp c
cnz incerrd
mov c, d
mov a, d
cmp c
cnz incerrd
mov c, e
mov a, e
cmp c
cnz incerrd
mov c, h
mov a, h
cmp c
cnz incerrd
mov c, m
mov a, m
cmp c
cnz incerrd
mov c, a
cmp c
cnz incerrd

mov d, b
mov a, b
cmp d
cnz incerrd
mov d, c
mov a, c
cmp d
cnz incerrd
mov d, d
mov a, d
cmp d
cnz incerrd
mov d, c
mov a, c
cmp d
cnz incerrd
mov d, d
mov a, d
cmp d
cnz incerrd
mov d, e
mov a, e
cmp d
cnz incerrd
mov d, h
mov a, h
cmp d
cnz incerrd
mov d, m
mov a, m
cmp d
cnz incerrd
mov d, a
cmp d
cnz incerrd

mov e, b
mov a, b
cmp e
cnz incerrd
mov e, c
mov a, c
cmp e
cnz incerrd
mov e, d
mov a, d
cmp e
cnz incerrd
mov e, c
mov a, c
cmp e
cnz incerrd
mov e, d
mov a, d
cmp e
cnz incerrd
mov e, e
mov a, e
cmp e
cnz incerrd
mov e, h
mov a, h
cmp e
cnz incerrd
mov e, m
mov a, m
cmp e
cnz incerrd
mov e, a
cmp e
cnz incerrd

mov h, b
mov a, b
cmp h
cnz incerrd
mov h, c
mov a, c
cmp h
cnz incerrd
mov h, d
mov a, d
cmp h
cnz incerrd
mov h, c
mov a, c
cmp h
cnz incerrd
mov h, d
mov a, d
cmp h
cnz incerrd
mov h, e
mov a, e
cmp h
cnz incerrd
mov h, h
mov a, h
cmp h
cnz incerrd
mov h, m
mov a, m
cmp h
cnz incerrd
mov h, a
cmp h
cnz incerrd

mov h, b
mov a, b
cmp h
cnz incerrd
mov h, c
mov a, c
cmp h
cnz incerrd
mov h, d
mov a, d
cmp h
cnz incerrd
mov h, c
mov a, c
cmp h
cnz incerrd
mov h, d
mov a, d
cmp h
cnz incerrd
mov h, e
mov a, e
cmp h
cnz incerrd
mov h, h
mov a, h
cmp h
cnz incerrd
mov h, m
mov a, m
cmp h
cnz incerrd
mov h, a
cmp h
cnz incerrd

mov l, b
mov a, b
cmp l
cnz incerrd
mov l, c
mov a, c
cmp l
cnz incerrd
mov l, d
mov a, d
cmp l
cnz incerrd
mov l, c
mov a, c
cmp l
cnz incerrd
mov l, d
mov a, d
cmp l
cnz incerrd
mov l, e
mov a, e
cmp l
cnz incerrd
mov l, h
mov a, h
cmp l
cnz incerrd
mov l, m
mov a, m
cmp l
cnz incerrd
mov l, a
cmp l
cnz incerrd

mov m, b
mov a, b
cmp m
cnz incerrd
mov m, c
mov a, c
cmp m
cnz incerrd
mov m, d
mov a, d
cmp m
cnz incerrd
mov m, c
mov a, c
cmp m
cnz incerrd
mov m, d
mov a, d
cmp m
cnz incerrd
mov m, e
mov a, e
cmp m
cnz incerrd
mov m, h
mov a, h
cmp m
cnz incerrd
mov m, a
cmp m
cnz incerrd

; terminate
mvi c, 0x00
call 00005h

incerrb: ; increment error count through B:C
        lxi b, errcntaddr
        ldax b
        inr a
        stax b
        ret

incerrd: ; increment error count through D:E
        lxi d, errcntaddr
        ldax d
        inr a
        stax d
        ret