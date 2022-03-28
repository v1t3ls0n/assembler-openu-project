; file ps.am
.entry LIST
.extern W

      macro m1
        inc r6
        move r3,W
      endm


      macro m2
        inc r2
        sub r2,r6
        sub r1,r9
      endm
