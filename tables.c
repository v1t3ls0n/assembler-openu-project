#include "data.h"
/* Shared global State variables*/
extern State globalState;

extern Item *symbols[HASHSIZE];
extern Item *macros[HASHSIZE];
/* Complex Struct Constant Variables: */
extern Operation operations[OP_SIZE];

unsigned hash(char *s)
{
    unsigned hashval = 1;

    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;

    return hashval % HASHSIZE;
}

Item *lookup(char *s, ItemType type)
{

    Item *np;
    for (np = (type == Symbol ? symbols[hash(s)] : macros[hash(s)]); np != NULL; np = np->next)
        if (strcmp(s, np->name) == 0)
            return np;
    return NULL;
}

Item *install(char *name, ItemType type)
{
    unsigned hashval;
    Item *np;
    int nameLength = strlen(name);

    if ((np = lookup(name, (type == Symbol ? Symbol : Macro))) == NULL)
    {
        np = (Item *)malloc(sizeof(Item *));
        np->name = (char *)malloc(nameLength * sizeof(char));
        if (np == NULL || np->name == NULL)
        {
            yieldError(memoryAllocationFailure);
            return NULL;
        }
        memcpy(np->name, name, nameLength);
        np->name[nameLength] = '\0';

        if (type == Symbol)
        {
            np->val.s.attrs.code = 0;
            np->val.s.attrs.entry = 0;
            np->val.s.attrs.external = 0;
            np->val.s.attrs.data = 0;
            np->val.s.base = 0;
            np->val.s.value = 0;
            np->val.s.offset = 0;
        }

        np->next = (Item *)malloc(sizeof(Item *));
        hashval = hash(name);
        np->next = (type == Symbol ? symbols[hashval] : macros[hashval]);
        if (type == Symbol)
            symbols[hashval] = np;
        else
            macros[hashval] = np;
    }
    /* Key name already exist inside table, yield Error of duplicate values in symbol/macro table,
    if it is an entry or an external do not yield error
    */
    else if (type == Symbol && (np->val.s.attrs.entry || np->val.s.attrs.external))
        return np;
    else
    {
        yieldError(type == Macro ? illegalMacroNameAlreadyInUse : illegalSymbolNameAlreadyInUse);
        return NULL;
    }

    return np;
}

void printSymbolTable()
{
    int i = 0;

    /*     printf("inside  printSymbolTable()\n");
     */

    printf("\n\t\t ~ SYMBOL TABLE ~ \n");
    printf("name\tvalue\tbase\toffset\tattributes");
    while (i < HASHSIZE)
    {
        if (symbols[i] != NULL)
            printSymbolItem(symbols[i]);
        i++;
    }
    printf("\n\n");
}

void printSymbolItem(Item *item)
{
    if (!item)
        return;

    printf("\n%s\t%u\t%u\t%u\t", item->name, item->val.s.value, item->val.s.base, item->val.s.offset);
    if (!item->val.s.attrs.code && !item->val.s.attrs.data && !item->val.s.attrs.entry && !item->val.s.attrs.external)
        printf("   ");

    else
    {
        if ((item->val.s.attrs.code || item->val.s.attrs.data) && (item->val.s.attrs.entry || item->val.s.attrs.external))
        {
            if (item->val.s.attrs.code)
                printf("code,");
            else
                printf("data,");

            if (item->val.s.attrs.entry)
                printf("entry");
            else
                printf("external");
        }
        else
        {
            if (item->val.s.attrs.code)
                printf("code");
            else if (item->val.s.attrs.data)
                printf("data");
            else if (item->val.s.attrs.entry)
                printf("entry");
            else
                printf("external");
        }
    }

    if (item->next != NULL)
        printSymbolItem(item->next);
}

Item *addSymbol(char *name, int value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal)
{
    unsigned base;
    unsigned offset;
    Item *p;
    if (!verifyLabelNaming(name))
        return NULL;
    printf("name:%s value:%d isCode:%u isData:%u isEntry:%u isExternal:%u\n", name, value, isCode, isData, isEntry, isExternal);
    p = lookup(name, Symbol);
    if (p != NULL)
    {

        if (p->val.s.attrs.external && (value || isCode || isData || isEntry))
            yieldError(illegalOverrideOfExternalSymbol);
        else if (p->val.s.attrs.entry && (isCode || isExternal))
            yieldError(symbolCannotBeBothCurrentTypeAndRequestedType);
        else
            yieldError(illegalSymbolNameAlreadyInUse);
        return NULL;
    }
    else
    {
        p = install(name, Symbol);
        offset = value % 16;
        base = value - offset;
        p->val.s.value = value;
        p->val.s.base = base;
        p->val.s.offset = offset;
        p->val.s.attrs.code = isCode ? 1 : 0;
        p->val.s.attrs.entry = isEntry ? 1 : 0;
        p->val.s.attrs.external = isExternal ? 1 : 0;
        p->val.s.attrs.data = isData ? 1 : 0;
    }

    /*    if (!isEntry || !isExternal)
           printf("added the name \"%s\" successfully to the symbol table!:)\n", name);
       else
           printf("updated exisiting  entry/external label with the name \"%s\" successfully to the symbol table!:)\n", name);
    */
    /* printSymbolTable();
     */

    return p;
}

Item *findOrAddSymbol(char *name, ItemType type)
{
    Item *p = lookup(name, type);
    if (p != NULL)
        return p;
    else
        return addSymbol(name, 0, 0, 0, 0, 0);
}

Item *findSymbol(char *name, ItemType type)
{
    return lookup(name, type);
}

Item *removeFromTable(char *name, ItemType type)
{
    Item *p = lookup(name, type);
    if (p->next)
        p = p->next;
    else
        p = NULL;

    return p;
}

Item *updateSymbolAddressValue(char *name, int newValue)
{
    Item *p = findSymbol(name, Symbol);
    unsigned base;
    unsigned offset;

    /*     printf("inside updateSymbolAddressValue\n");
     */

    if (p != NULL)
    {
        offset = newValue % 16;
        base = newValue - offset;
        p->val.s.offset = offset;
        p->val.s.base = base;
        p->val.s.value = newValue;
        printf("updated adrress values for \"%s\" successfully to the symbol table!:)\n", name);

        /*
              printSymbolTable();
      */
    }
    else
        yieldError(symbolDoesNotExist);

    return p;
}

Item *updateSymbolAttribute(char *name, int type)
{
    Item *p = findSymbol(name, Symbol);

    if (p != NULL)
    {
        if (((type == _TYPE_DATA || type == _TYPE_STRING) && p->val.s.attrs.code) || (type == _TYPE_CODE && p->val.s.attrs.data) || ((type == _TYPE_ENTRY) && p->val.s.attrs.external) || (type == _TYPE_EXTERNAL && p->val.s.attrs.entry))
        {
            yieldError(symbolCannotBeBothCurrentTypeAndRequestedType);
            return NULL;
        }
        else
        {
            if (type == _TYPE_DATA || type == _TYPE_STRING)
                p->val.s.attrs.data = 1;
            else if (type == _TYPE_ENTRY)
                p->val.s.attrs.entry = 1;
            else if (type == _TYPE_EXTERNAL)
                p->val.s.attrs.external = 1;
            else if (type == _TYPE_CODE)
                p->val.s.attrs.code = 1;

            /*
            printf("updated \"%s\" attributes successfully to the symbol table!:)\n", name);
            printSymbolTable();
            */
        }
    }
    else
        yieldError(symbolDoesNotExist);

    return p;
}

char *getMacroCodeValue(char *s)
{
    Item *result = lookup(s, Macro);
    if (result != NULL)
        return strdup(result->val.m.code);
    else
        yieldError(macroDoesNotExist);

    return NULL;
}

Item *addMacro(char *name, char *code)
{
    Item *item = install(name, Macro);
    if (item != NULL)
        item->val.m.code = strdup(code);

    return item;
}

Bool verifyLabelNaming(char *s)
{
    int i = 0;
    const char *regs[] = {R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15};
    int labelLength = strlen(s);

    /* if label name does not start with a alphabet letter */
    if (isalpha(s[0]) == 0)
        return yieldError(illegalLabelNameUseOfCharacters);

    /* maximum label name length is 31 characters */
    if (strlen(s) > MAX_LABEL_LEN)
        return yieldError(illegalLabelNameLength);

    if (strchr(s, 'r') && labelLength >= 2 && labelLength <= 3)
    {
        while (i < REGS_SIZE)
        {
            if ((strcmp(regs[i], s) == 0))
                return yieldError(illegalLabelNameUseOfSavedKeywords);

            i++;
        }
    }

    else if ((labelLength >= 3 && labelLength <= 4))
    {
        while (i < OP_SIZE)
        {
            if ((strcmp(operations[i].keyword, s) == 0))
                return yieldError(illegalLabelNameUseOfSavedKeywords);

            i++;
        }
    }
    else
    {

        while (i < labelLength)
        {
            if (!isalnum(s[i]))
                return yieldError(illegalLabelNameUseOfCharacters);

            i++;
        }
    }

    return True;
}
