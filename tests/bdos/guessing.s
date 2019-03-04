.org 0x100
.def maxtries 5
.def guessaddr 0x1000

mvi a, maxtries
; assertion is made that register L holds
; a random value (i.e. emulator was compiled using -O3 flag)

ask:
  sbi 1
  jz fail

  mvi c, 9
  lxi d, askmsg
  syscall

  mvi c, 10
  lxi d, guessaddr
  syscall

  call compareguess

  jmp ask

compareguess:
  push psw ; push A:FLAGS

  ; load number to guess to A
  ; and compare with L
  call parsenumber
  cmp l
  jz win ; if this is the number, jump to win
  cp higher
  cm lower

  pop psw
  ret

lower:
  mvi c, 9
  lxi d, lowermsg
  syscall
  ret

higher:
  mvi c, 9
  lxi d, highermsg 
  syscall
  ret

win:
  mvi c, 9
  lxi d, winmsg
  syscall

  rst 0

fail:
  mvi c, 9
  lxi d, failmsg
  syscall

  rst 0

; get number from guessaddr
; and store it in L
parsenumber:
  push psw ; push FLAGS:A

  mvi c, 0 ; digit count | C=0

  ; load last digit to A
  lxi d, guessaddr ; DE=guessaddr
  ldax d ;  A=[guessaddr]
  comparedollar:
    cpi 36 ; $
    jnz more

  ; ok, we've got a $
  ; load previous char to A
  dcx d
  ldax d
  sbi 48 ; char -> int
  push psw ; push it

  ; check if more digits
  mov a, c
  cpi 0
  jz setl ; if not, pop the number and store it in L
  ;else load another

  dcx d
  ldax d
  sbi 48

  mov l, a ; copy A to L
  ; add it ten times
  add l
  add l
  add l
  add l
  add l
  add l
  add l
  add l
  add l
  add l

  mov l, a ; copy A to L
  pop psw ; pop the previous digit
  add l ; add 10 * a1 to a0
  push psw ; push it back

  dcr c
  mov a, c
  cpi 0
  jz setl

  ; more digits? m'kay
  dcx d
  ldax d
  sbi 48

  cpi 2
  jz add200 
  cpi 1
  jz add100
  jmp setl

  add200:
    pop psw
    adi 200
    push psw
    jmp setl

  add100:
    pop psw
    adi 100
    push psw
    jmp setl

  more:
    inr c
    inx d ; increment address
    ldax d
    jmp comparedollar

  setl:
    pop psw
    mov l, a

  pop psw
  ret

askmsg:
  db "Guess the number: $"
failmsg:
  db "You've failed.", 10, "$"
winmsg:
  db "That is a correct number!", 10 "$"
lowermsg:
  db 10, "The number is lower than that", 10, "$"
highermsg:
  db 10, "The number is greater than that", 10, "$"

