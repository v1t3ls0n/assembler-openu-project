# clear
# cd ./test_files/valid  
# sh delOutput.sh 

# cd ..
# cd ..
# cd  ./test_files/mixed/
# sh delOutput.sh 
# cd ..
# cd ..
# cd ./test_files/errors/
# sh delOutput.sh 

# cd ..
# cd ..

# sh delOutput.sh


# rm -rf  *.log ./_tests/good_code/code/*.log ./_tests/good_code/code/*.am ./_tests/good_code/code/*.ob ./_tests/good_code/code/*.ent ./_tests/good_code/code/*.ext ./_tests/good_code/code/*.warnings ./_tests/good_code/code/*.warnings.warnings ./_tests/good_code/code/*.errors ./_tests/good_code/code/*.errors.errors  
# rm -rf  *.log *.am *.ob *.ent *.ext *.warnings *.warnings *.errors *.errors.errors  
# # cd ..
# cd ..
make 
./compiler  ./test_files/valid/valid  ./test_files/valid/valid_with_macros ./test_files/mixed/wrongAddressingMethodTest ./test_files/mixed/wrongNumOfCommas ./test_files/mixed/wrongNumOfOperands ./test_files/mixed/wrongUseOfInstructions ./test_files/mixed/shahar_1
rm -f compiler 
