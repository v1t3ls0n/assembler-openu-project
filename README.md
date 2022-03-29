
# Maman 14: Final Project (system programming lab Openu CS course) Done by 
## Guy Vitelson (203379706) && Shahar Stav Torati (215661422) For Danny Calfon (course instructor)
### Date: 28.03.2022

##### Task Summery: Create an assembler for fictional assembly language defined by openu course.
##### Implementation Overview and general approach:

#### Objectives (Goals): ### 
1) Efficiency in memory (It was important to us allocate memory dynmically and make its size ##### fit exact as required for each specific memory image output of the program.

2) Seperation of concern: we tried to split the program to indepent components that will be##### responsible for well defined tasks and that the sharing of data between files will be through ##### some kind of encapsolution method such as state getters/setters and scoping global variable ##### to be static so that they will be scoped to the single file that they belongs to.

3) Absctration of data structures and variable use to fit our needs by definning different typedef structs types enums and constants.

#### Our implimentation Overview in short: 
#### Stages:

1. Macros stage
Parsing the original .as file and replacing macros if existing using hash table to store the starting and ending 
index of each macro in file. if we found an error in the syntax of the macros we yield relevant error message
and will not continue to the first run.

2. First Run
Moving to first run in which we will only count the size of the data image and the instruction image and also
we will add all the symbols to the symbol table and will verify that there are no errors in code. if we found errors we
yield a print messages to stderr and error.log file which will be created in the root folder, we will parse all the content of the source code in first run anyway so if we encounter an error we yield print error message and continue to check and look for other errors.if even 1 error occured in first run we will not enter second run, we will not allocate any memory and will not write any binary data at all. in the first run we do not write or convert any argument to the resulted binary/hex image.
 
3. In between first and second run
If everything was valid in first run we allocate the exact amount of memory we need to write the image result, before
we do that we first updating the address of each data symbol in the symbol table. then we reset the counters (IC/DC) and 
then we allocating the memory for the resulted hex/binary image. we are allocating the exact size and since we counted the size of each operation or instruction on the first run. after allocating memory we move to second run.

4. Second run
In the second run our assembler writes the words of each line of code in binary format, we inserting the words to
the memory image in the correct position depending on the type of word (data/instruction) and we add each external
operand appearence to the linked list of all the positions that external operands appeared. if we encouter label
operand that is not within the symbol table and is not external we yield error message and continue checking the
rest of the code in order to discover all errors of this type if this case happens we will finish second run but
will not export any files.

 5. Exporting files, after second run
 Exporting all the required files (.ob, .ext .ent) and moves to the next file or end program.

 






