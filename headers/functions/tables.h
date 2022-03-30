
/*
##########################################################################################################
##########################################################################################################
#################################### tables.c General Overview: ##########################################
##########################################################################################################
##########################################################################################################

The file tables.c is responsible of all the tables, lists and data structures in the code.
It handles the macros table, the symbols table and the external operands nested singly linked list
Both the symbols and the macros able are using the same hash table data structure and main function\interface.
The hashtable main functions are lookup hash and install for finding \ adding \ hashing items in the symbol table.
Each type of table (symbol\macro\external operand) have additional helpers, getters and setters functions that are
using the 3 hash table main functions (install\lookup\hash).

##########################################################################################################
##########################################################################################################
##########################################################################################################
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
/* @ Function: Item *lookup(char *s, ItemType type)
   @ Arguments: char *s (label name) ItemType type (Macro/Symbol)
   @ Description: Function finds the item with the name and of the type
   in the correspoding hash table (macros or symbols hash table)
   it returns a pointer to a typedef struct Item;
*/
Item *lookup(char *s, ItemType type);

/* @ Item *install(char *name, ItemType type)
   @ Arguments:char *s (label name) ItemType type (Macro/Symbol)
   @ Description: Function adds the label name to the corresponding hash table
   it uses hash function and if the index of the hash table is already in use
   thne the it connects the new entry to be the *next of the item under the common index.
   (symbols/macros tables acts as singly linked list within the hash table in those fcases)
   it returns a pointer to a typedef struct Item for the new Item added to the table;
*/
Item *install(char *name, ItemType type);

/* void initTables();
void freeHashTable(ItemType type);
void freeTableItem(Item *item); */

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
void updateFinalValueOfSingleItem(Item *item);
Bool areEntriesExist();
Bool areExternalsExist();
void printSymbolTable();
int printSymbolItem(Item *item);
void writeEntriesToFile(FILE *fp);
int writeSingleEntry(Item *item, FILE *fp, int count);

/*#####################################################################*/
/*#####################################################################*/
/*########################### Macro functions ###########################*/
/*#####################################################################*/
/*#####################################################################*/

Item *addMacro(char *name, int start, int end);
Item *getMacro(char *s);
Item *updateMacro(char *name, int start, int end);
void printMacroTable();
int printMacroItem(Item *item);

/*#####################################################################*/
/*#####################################################################*/
/*############ External Operand Table (Nested singly linked list) ######*/
/*#####################################################################*/
/*#####################################################################*/

ExtListItem *findExtOpListItem(char *name);
void resetExtList();
void updateExtPositionData(char *name, unsigned base, unsigned offset);
void addExtListItem(char *name);
void writeExternalsToFile(FILE *fp);
void writeSingleExternal(FILE *fp, char *name, unsigned base, unsigned offset, ExtPositionData *next);