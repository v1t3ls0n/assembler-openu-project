LIST1: .data 6, -9
LIST2: .data 6   , -9
LIST3: .data 6,-9
LIST4: .data 6   ,-9
LIST5: .data ,6   ,-9
LIST6: .data 6 -9
LIST7: .data 6,, -9
LIST8: .data 6, -9,
LIST9: .data 6,a, -9,
LIST9: .data 6, b, -9

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
cmp val1, #-6
bne END[r15]
dec K
.entry MAIN
sub LOOP[r10] ,r14
END: stop
STR: .string "abcd"
LIST: .data 6, -9
.data -100
.data 5.766349
.data -9.3, 5.56
.data -1.3,2.56
.entry K
K: .data 31
.extern val1