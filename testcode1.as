   .entry      x
   x:            .data          3        
   MAIN:mov x, r2
   x:            .string          3      
   y:            .data          5, 8,     56        
   add #3, y  
   d:            .data          5, $,     56        
   z:            .data          7      ,  ,   56,        
   rts
   fg:            .data          7      ,  ,   44,$^        
   g:            .data          7      ,  ,   ,,,56,     
   .external Bla
   .external stamm      ddd   
   .external dsafs   
   str:            .string          "abcd"        
   str2:            .string          "AAADDDDDabcd"        
   str3:            .string          \"FFFFFFFFAAADDDDDabcd\"       