; file ps.am
.entry LIST
.extern W
MAIN: add r3, LIST
LOOP: prn #48

macro m1
 inc r1
    inc r2
         inc r3
endm



lea STR, r6
inc r6
mov r3, W
sub r1, r4
        macro m2
inc 4
int 5
int 6
endm

bne END
cmp val1, #-6

m1

bne END[r15]
dec K
.entry MAIN
sub LOOP[r10] ,r14
END: stop
STR: .string "abcd"
LIST: .data 6, -9
.data -100
.entry K
K: .data 31
.extern val1

m2

