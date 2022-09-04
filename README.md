
# Maman 14: Final Project (system programming lab Openu CS course) Done by 
## Guy Vitelson && Shahar Stav Torati For Danny Calfon (course instructor)
### Date: 28.03.2022
#### University explicit specification for the project:
https://github.com/v1t3ls0n/assembler-openu-project/files/8794964/university_project_specs_doc.pdf


##### Task Summery: Create an assembler for fictional assembly language defined by openu course.
##### Implementation Overview and general approach:

#### Objectives (Goals): ### 
1) Efficiency in memory: It was important to us allocate memory dynmically and make its size fit exact as required for each specific memory image output of the program.

2) Seperation of concern: we tried to split the program to indepent components that will be responsible for well defined tasks and that the sharing of data between files will be through some kind of encapsolution method such as state getters/setters and scoping global variable to be static so that they will be scoped to the single file that they belongs to.

3) Absctration of data structures and variable use to fit our needs by definning different typedef structs types enums and constants.

#### Our implimentation Overview in short: 
#### Stages:

#### 1) Macros stage:
Parsing the original source (.as) file and replacing macros if existing using hash table to store the starting and ending 
index of each macro in file. if we found an error in the syntax of the macros we yield relevant error message
and will not continue to the first run.

#### 2) First Run:
In the first run the assembler counts the size of the data image and the instruction image (without allocating any memory), 
it adds all the symbols to the symbol table, and last but not least the assembler will verify that there are no errors in code before 
it moves on  to step 3. If the assembler encounter any error in code it outputs error/warning message to stderr 
and error.log (file which will be created in the root folder if any errors/warnings occurs). 

In order to report on each of any error in the source code in the first run, the assember parses all the content of the source code regardless of any occurrence of an error. If any error (warnings do not count as errors) occurred, the assembler will not continue to stage 3 but it will go straight on to the *Next* stage.
 
#### 3) In between first and second run:
If first run ended successfully (else: Next) the assembller will update the address of each data symbol in the symbol table, 
then it will allocate the exact amount of memory it needs to write for the target image (binary of compiled code)
then it will reset the counters (IC/DC) and continue to the second run. 

#### 4) Second run:
In the second run our assembler writes the words of each line of code in binary format, we inserting the words to
the memory image in the correct position depending on the type of word (data/instruction) and we add each external
operand appearence to the linked list of all the positions that external operands appeared. if we encouter label
operand that is not within the symbol table and is not external we yield error message and continue checking the
rest of the code in order to discover all errors of this type if this case happens we will finish second run but
will not export any files.


 #### 5) Export (generate files):
 If second run finished without any error (else: Next) we will generate all of the required outputs (.ob, .ext .ent files).

##### Next:
The assembler then moves on to handle the next source file (means that it will repeate over these 5 steps for each source file passed to it) untill the last one then it ends program.
