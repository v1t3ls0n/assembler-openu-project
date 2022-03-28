LOOP: prn #48
macro mdwafs1
 inc r6
 mov r3, W
endm
dec K
mdwafs1
.entry MAIN
sub LOOP[r10] ,r14
