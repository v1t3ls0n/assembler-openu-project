clear
rm -rf  *.am *.ob *.ent *.ext *.warnings *.warnings.warnings *.errors *.errors.errors  
make && ./compiler test test2 test3  && rm -f compiler
