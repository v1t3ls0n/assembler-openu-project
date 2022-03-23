clear 
rm -f compiler 
rm -f *.log *.am *.ob *.ent *.ext *.warnings *.errors 
make 
./compiler test  
rm compiler.exe
