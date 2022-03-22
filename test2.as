; file ps.am
.entry LIST
.extern W
MAIN: add r3, W
LOOP: prn W
lea STR, W
inc r6
mov r3, W
sub r1, W
bne END
cmp val1, W
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
