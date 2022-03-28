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