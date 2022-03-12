; file ps.am
LOOP: prn #48
bne END[r15]
sub LOOP[r10] ,r14
END: stop
