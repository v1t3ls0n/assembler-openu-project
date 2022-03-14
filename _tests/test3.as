.entry LIST
.extern W
MAIN: add r3, LIST
LOOP: prn #48
      macro m1
        inc r6
        move r3,W
      endm
lea STR, r6
inc r6
mov r3, W
sub r1, r4
      macro m2
        inc r2
        sub r2,r6
        sub r1,r9
      endm
bne END
cmp val1, #-6
bne END[r15]
dec K
      macro m3
        inc r2
        sub r2,r6