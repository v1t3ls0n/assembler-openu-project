; file ps.am
.entry LIST
.extern W
<<<<<<< HEAD:test.as
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
=======
MAIN: add r3, LIST
LOOP: prn #48
macro m1
 inc r6
 mov r3, W
endm
lea STR, r6
inc r6
mov r3, W
>>>>>>> 929df632150df0414fa979bd3da3ead37245514c:_tests/good_code/code/test-with-macros3.as
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

