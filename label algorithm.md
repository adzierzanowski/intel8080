
```s
 .org 100h
 
 main:
   mov a, c ; 0
   lxi d, 0x5555 ; 1 2 3
 
   jnz label ; 4 5 6
 
   mvi c, 9 ; 7 8
   lxi d,msg ; 9 10 11
 
 msg: db "Hello, world"
```

    0 Token(DIRECTIVE, ".org") @ pos(1,4)
    1 Token(NUMBER, "256") @ pos(1,9)
    2 Token(LABEL, "main:") @ pos(3,4)
    3 Token(INSTRUCTION, "mov") @ pos(4,6)
    4 Token(REGISTER, "a") @ pos(4,10)
    5 Token(REGISTER, "c") @ pos(4,13)
    6 Token(COMMENT, "; 0") @ pos(4,15)
    7 Token(INSTRUCTION, "lxi") @ pos(5,6)
    8 Token(REGISTER, "d") @ pos(5,10)
    9 Token(NUMBER, "21845") @ pos(5,13)
    10 Token(COMMENT, "; 1 2 3") @ pos(5,20)
    11 Token(INSTRUCTION, "jnz") @ pos(7,6)
    12 Token(IDENTIFIER, "label") @ pos(7,10)
    13 Token(COMMENT, "; 4 5 6") @ pos(7,16)
    14 Token(INSTRUCTION, "mvi") @ pos(9,6)
    15 Token(REGISTER, "c") @ pos(9,10)
    16 Token(NUMBER, "9") @ pos(9,13)
    17 Token(COMMENT, "; 7 8") @ pos(9,15)
    18 Token(INSTRUCTION, "lxi") @ pos(10,6)
    19 Token(REGISTER, "d") @ pos(10,10)
    20 Token(IDENTIFIER, "msg") @ pos(10,12)
    21 Token(COMMENT, "; 9 10 11") @ pos(10,16)
    22 Token(LABEL, "msg:") @ pos(12,4)
    23 Token(DATA, "db") @ pos(12,9)
    24 Token(STRING, ""Hello, world"") @ pos(12,12)

| tok | label_pos | origin | labels |
|-----|-----------|--------|--------|
| 0   | 0         | 0      |        | Token(DIRECTIVE, ".org") - skip iterator +1
| 2   | 0         | 0      | main   | main:0
|
