.org 0x100

xra a
mvi b, 1
mvi c, 1
mvi d, 1
mvi e, 1
mvi h, 1
mvi l, 1
mvi m, 1

add b
add c
add d
add e
add h
add l
add m

push psw
xra a
mov c, a
pop psw
call 5
