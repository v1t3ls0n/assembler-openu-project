; file ps.am
.entry LIST
.extern W
.extern STR
MAIN: add r3, LIST
LOOP: prn #48
lea STR, r6
inc r6
mov r3, W
.extern W
.entry LIST