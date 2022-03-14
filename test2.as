;this file will include errors

;undifined label (LOOP)
LOOP: mov #27, r11
;not enough operands
bne
prn HELLO
;using a wrong register in the index addresing method
red MAIN[r2]
HELLO: .string "hello"
sub r9, w
;not enough commas
START: lea HELLO VAR
.entry w
MAIN: .data -7,2
;too much operands
clr w[14],r3
.extern VAR
;too many commas
inc ,r1
;argument from a wrong addresing method
not #6
.entry LOOP
;a string variable without opening "
Q: .string Addd"
;a variable which is both entry and extern
.extern LOOP
;undifined instruction
grh HELLO[r11]
; a data variable with a letter in it (only numbers are allowed)
w: .data -9,3, s, 4
