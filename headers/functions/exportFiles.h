

/*
##########################################################################################################
#################################### exportFiles.c General Overview: #####################################
##########################################################################################################
This file is used for generating the required output files of the assembler.
##########################################################################################################
##########################################################################################################
*/

/*  void exportFilesMainHandler()
This function calls other functions that will create the .ob file,
.ent file and .ext file if there are any entry labels, and .ext file
if there are any external labels.*/
void exportFilesMainHandler();

/* void generateObFile()
This function generates the .ob files by writing the memory image,
if the file can't be generate it yields error  */
void generateObFile();

/* void createEntriesFile()
This function generates the .ent files (as long as there are entry labels) by writing
it into the file, if the file can't be generate it yields error */
void createEntriesFile();

/* createExternalsFile()
This function generates the .ext files (as long as there are external labels)
by writing it into the file, if the file can't be generate it yields error */
void createExternalsFile();