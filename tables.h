#include "tables.c"
/*The file tables.c is responsible of all the tables, lists and data structures in the code.
It handles the macros table, the symbols table and the external operands nested singly linked list
Both the symbols and the macros able are using the same hash table data structure and main function\interface.
The hashtable main functions are lookup hash and install for finding \ adding \ hashing items in the symbol table.
Each type of table (symbol\macro\external operand) have additional helpers, getters and setters functions that are
using the 3 hash table main functions (install\lookup\hash).
*/
/*#####################################################################*/
/*#####################################################################*/
/*####################### Hash Table Functions ############################*/
/*#####################################################################*/
/*#####################################################################*/
/* @ Function: unsigned hash(char *s)
   @ Arguments: The function gets char *s- a label name
   @ Description: The function gets a string that is a label name,
   it hashes it with a simple algorithem and
   it returns the index position in the macros/symbols hash table array.
*/

unsigned hash(char *s);
Item *lookup(char *s, ItemType type);
Item *install(char *name, ItemType type);

/*#####################################################################*/
/*#####################################################################*/
/*##########################Symbols functions#############################*/
/*#####################################################################*/
/*#####################################################################*/

Bool addSymbol(char *name, unsigned value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal);
Bool updateSymbol(Item *p, unsigned value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal);
Item *getSymbol(char *name);
int getSymbolBaseAddress(char *name);
int getSymbolOffset(char *name);
Bool isSymbolExist(char *name);
Bool isExternal(char *name);
Bool isEntry(char *name);
Bool isNonEmptyExternal(char *name);
Bool isNonEmptyEntry(char *name);
Bool isLabelNameAlreadyTaken(char *name, ItemType type);
Item *updateSymbolAddressValue(char *name, int newValue);
void updateFinalSymbolTableValues();

/*#####################################################################*/
/*#####################################################################*/
/*###########################Macro functions##########################################*/
/*#####################################################################*/
/*#####################################################################*/

Item *addMacro(char *name, int start, int end);
Item *getMacro(char *s);
Item *updateMacro(char *name, int start, int end);

/*#####################################################################*/
/*#####################################################################*/
/*###################External Operand Table##################################*/
/*#####################################################################*/
/*#####################################################################*/

ExtListItem *findExtOpListItem(char *name);
void resetExtList();
void updateExtPositionData(char *name, unsigned base, unsigned offset);
void addExtListItem(char *name);