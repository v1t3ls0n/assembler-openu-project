;this file will not include errors

LABEL1: add D, r3
jsr X
cmp #47, r9
X: .data -2, 4, 36, 19
dec STR[r13]
.entry D
prn #99
.extern LABEL1
STR: .string "AVgghT"ד