clear
rm -rf  *.log ./_tests/good_code/code/*.log ./_tests/good_code/code/*.am ./_tests/good_code/code/*.ob ./_tests/good_code/code/*.ent ./_tests/good_code/code/*.ext ./_tests/good_code/code/*.warnings ./_tests/good_code/code/*.warnings.warnings ./_tests/good_code/code/*.errors ./_tests/good_code/code/*.errors.errors  
rm -rf  *.log *.am *.ob *.ent *.ext *.warnings *.warnings *.errors *.errors.errors  
make 
./compiler ./official-test-files/valid/valid ./official-test-files/valid/valid_with_macros
rm -f compiler 


# ./compiler ./official-test-files/valid/valid ./official-test-files/valid/valid_with_macros
# ./compiler ./official-test-files/errors/randomErrors.as  ./official-test-files/mixed/shahar_1