<<<<<<< HEAD
;this file will include errors
.entry B 
;empty data instruction
A: .data
;missing space after data instruction
A: .data5 , 6 ,3
;consecutive commas
A: 6, 7, ,, 8
;missing comma
A: .data 4  8  
;missing commas (twice)
A: .data 4,5 6 , 7, 8   3
;illegal appearence of comma before the first parameter
A:   .data ,5 , 7 ,1
;missing space between label and instruction
A:.data 5,6,2,3,-1
;illegal appearence of comma after the last parameter
A: .data 8, 
;undefined character (not an int) for data
A: .data   43 , 6.5 , 8, 1
;undefined character (not an int) for data (twice)
A: .data 5 , 7 ,a ,8 ,b
;illegal appearence of comma before the first parameter + illegal appearence of comma after the last parameter
A: .data   , 6 , 7 ,8,
;missing comma + consecutive comma
A: .data 4   6    , 8,9,, , 2
;illegal appearence of comma before the first parameter + missing commas
A: .data ,4  5
;empty string instruction
A: .string
;label defined as both entry and extern
.extern B
;string instruction without closing "
A: .string "juth45  
;string instruction without opening "
A: .string     gfbgrbr"
;string instruction without " (opening and closing)
A: .string uhuf  
=======
;this file will include errors 
.entry B  
;empty data instruction 
A: .data 
;missing space after data instruction 
B: .data5 , 6 ,3 
;consecutive commas 
C: 6, 7, ,, 8 
;missing comma 
D: .data 4  8   
;missing commas (twice) 
E: .data 4,5 6 , 7, 8   3 
;illegal appearence of comma before the first parameter 
F:   .data ,5 , 7 ,1 
;missing space between label and instruction 
G:.data 5,6,2,3,-1 
;illegal appearence of comma after the last parameter 
H: .data 8,  
;undefined character (not an int) for data 
I: .data   43 , 6.5 , 8, 1 
;undefined character (not an int) for data (twice) 
J: .data 5 , 7 ,a ,8 ,b 
;illegal appearence of comma before the first parameter + illegal appearence of comma after the last parameter 
K: .data   , 6 , 7 ,8, 
;missing comma + consecutive comma 
L: .data 4   6    , 8,9,, , 2 
;illegal appearence of comma before the first parameter + missing commas 
M: .data ,4  5 
;empty string instruction 
N: .string 
;label defined as both entry and extern 
.extern B 
;string instruction without closing " 
O: .string "juth45   
;string instruction without opening " 
P: .string     gfbgrbr" 
;string instruction without " (opening and closing) 
Q: .string uhuf   
>>>>>>> 6a084274a8a6fd72d47f483c9ad30e63bf2556a8
