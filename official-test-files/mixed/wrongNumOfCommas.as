;this file will include errors- wrong number of commas
;consecutive commas at mov
mov O ,  ,, r3
;consecutive commas at add
add #-5,,M[r14]
;consecutive commas at lea
lea L ,,  , , , O 
;illegal comma before first operand at cmp
cmp , #-5, #2
;illegal comma before first operand at bne
bne ,L
;illegal comma before first operand at red
red ,         r10
M: .data 9,6, -33    ,55
;illegal appearence of comma at stop
stop , 
;illegal appearence of comma after the last parameter at sub
sub N[r14]     , O,
;illegal appearence of comma after the last parameter at jmp
jmp M    ,  
;illegal appearence of comma after the last parameter at not
not        r4      ,   
;missing comma at add
add #2    r7
;missing comma at cmp
cmp        #3       N
;illegal appearence of comma after the last parameter + illegal comma before first operand at prn
prn     ,#-88,
;illegal appearence of comma after the last parameter + illegal comma before first operand at cmp
cmp , r2,  O[r13] ,
;illegal appearence of comma after the last parameter + illegal comma before first operand at red
red , N,
;illegal appearence of comma after the last parameter + consecutive commas at mov
mov #3,,,,,,r7,
;illegal appearence of comma after the last parameter + consecutive commas at add
add r2 ,     ,    r8  ,
.extern L
;illegal comma before first operand + illegal appearence of comma after the last parameter at sub
sub ,N,O[r12],
O: .extern N
;illegal comma before first operand + illegal appearence of comma after the last parameter at lea
  lea , M   ,r1    ,
O: .string "fnuu "  67*(+S4r)  ;/, e H B"
;illegal appearence of comma after the last parameter + illegal 
;comma before first operand + consecutive commas at cmp
cmp  ,#4   ,  , ,,L    , 