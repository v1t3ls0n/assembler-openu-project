; file ps.am
.entry LIST
.extern W
MAIN: add r3, LIST
LOOP: prn #48
lea STR, r6
inc r6
mov r3, W
sub r1, r4
bne END
cmp val1, W
bne END[r15]
dec K
.entry MAIN
sub LOOP[r10] ,r14
END: stop
STR: .string         "a   \'''' ""                                  bc     d"                                     
LIST: .data 6, -9
.data -100
.entry K
K: .data 31
.extern val1