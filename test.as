; file ps.am
.entry LIST
.extern W
<<<<<<< HEAD
MAIN: add r3, LIST
LOOP: prn #48

lea STR, r6
inc r6
mov r3, W
=======
MAIN: add r3, W
LOOP: prn #4
lea STR, W
inc val1
mov r3, val1
>>>>>>> e1867acf86bb0394e0dae86cdf23df66996b8f53
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
.entry K
K: .data 31
.extern val1

