clear
rm -rf  *.log ./_tests/good_code/code/*.log ./_tests/good_code/code/*.am ./_tests/good_code/code/*.ob ./_tests/good_code/code/*.ent ./_tests/good_code/code/*.ext ./_tests/good_code/code/*.warnings ./_tests/good_code/code/*.warnings.warnings ./_tests/good_code/code/*.errors ./_tests/good_code/code/*.errors.errors  
rm -rf  *.log *.am *.ob *.ent *.ext *.warnings *.warnings *.errors *.errors.errors  
make 
./compiler ./_tests/test-from-book ./_tests/test2
rm -f compiler 
# wrongNumOfOperands wrongUseOfInstructions wrongAddressingMethodTest
# wrongAddressingMethodTest wrongNumOfOperands wrongUseOfInstructions wrongNumOfCommas
# test wrongAddressingMethodTest wrongNumOfOperands wrongUseOfInstructions wrongAddressingMethodTest