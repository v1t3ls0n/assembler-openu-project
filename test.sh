clear 
rm -f compiler 
rm -f *.log *.am *.ob *.ent *.ext *.warnings *.errors 
make 
./compiler test-from-book test wrongAddressingMethodTest wrongNumOfOperands wrongUseOfInstructions wrongAddressingMethodTest
rm compiler.exe
