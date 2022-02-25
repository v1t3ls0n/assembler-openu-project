#include "data.h"
/* Shared global State variables*/
extern State globalState;
extern Error currentError;
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
    name[strlen(name) - 1] = '\0'; /*cleaning the label token from the last : character as we should do before we try to add it to the symbol table.     */

    verifyLabelNaming(name);
    if (globalState == collectErrors)
        return NULL;
    if ((np = lookup(name, (type == Symbol ? Symbol : Macro))) == NULL)
    {
        np = (Item *)malloc(sizeof(Item *));
        np->name = (char *)malloc(nameLength * sizeof(char));
        memcpy(np->name, name, nameLength);
        np->name[nameLength] = '\0';
        if (np == NULL || np->name == NULL)
        {
            globalState = collectErrors;
            currentError = memoryAllocationFailure;
            return NULL;
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
    else
    {
        globalState = collectErrors;
        currentError = type == Macro ? illegalMacroNameAlreadyInUse : illegalSymbolNameAlreadyInUse;
        return NULL;
    }

    return np;
}

void printSymbolTable()
{
    int i = 0;
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
    printf("\n%s\t%u\t%u\t%u\t", item->name, item->val.s.value, item->val.s.base, item->val.s.offset);
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

    if (item->next != NULL)
        printSymbolItem(item->next);
}

Item *addSymbol(char *name, int value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal)
{
    Item *p;
    unsigned base;
    unsigned offset;
    p = install(name, Symbol);
    if (globalState != collectErrors)
    {
        offset = value % 16;
        base = value - offset;
        p->val.s.value = value;
        p->val.s.base = base;
        p->val.s.offset = offset;
        p->val.s.attrs.code = isCode ? 1 : 0;
        p->val.s.attrs.entry = isEntry ? 1 : 0;
        p->val.s.attrs.external = isExternal ? 1 : 0;
        p->val.s.attrs.data = isData ? 1 : 0;
        printf("added the name \"%s\" successfully to the symbol table!:)\n", name);
        printSymbolTable();
        return p;
    }

    return NULL;
}

Item *findOrAddSymbol(char *name, ItemType type)
{
    Item *p = lookup(name, type);
    if (p != NULL)
        return p;
    else
        return install(name, type);
}

Item *findSymbol(char *name, ItemType type)
{
    return lookup(name, type);
}
void updateSymbol(char *name, int newValue)
{
    Item *p = lookup(name, Symbol);
    unsigned base;
    unsigned offset;

    if (p != NULL)
    {
        offset = newValue % 16;
        base = newValue - offset;
        p->val.s.offset = offset;
        p->val.s.base = base;
        p->val.s.value = newValue;
    }
    else
    {
        globalState = collectErrors;
        currentError = symbolDoesNotExist;
    }
}

char *getMacroCodeValue(char *s)
{
    Item *result = lookup(s, Macro);
    if (result != NULL)
        return strdup(result->val.m.code);
    else
    {
        globalState = collectErrors;
        currentError = macroDoesNotExist;
    }

    return NULL;
}

void addMacro(char *name, char *code)
{
    Item *item = install(name, Macro);
    if (globalState != collectErrors)
        item->val.m.code = strdup(code);
}

void verifyLabelNaming(char *s)
{
    int i = 0;
    const char *regs[] = {R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15};
    int labelLength = strlen(s);

    /* if label name does not start with a alphabet letter */
    if (isalpha(s[0]) == 0)
    {
        globalState = collectErrors;
        currentError = illegalLabelNameUseOfCharacters;
        return;
    }

    /* maximum label name length is 31 characters */
    if (strlen(s) > MAX_LABEL_LEN)
    {
        globalState = collectErrors;
        currentError = illegalLabelNameLength;
        return;
    }
    if (globalState != collectErrors)
    {
        if (strchr(s, 'r') && labelLength >= 2 && labelLength <= 3)
        {
            while (i < REGS_SIZE && globalState != collectErrors)
            {
                if ((strcmp(regs[i], s) == 0))
                {
                    currentError = illegalLabelNameUseOfSavedKeywords;
                    globalState = collectErrors;
                }
                i++;
            }
        }

        else if ((labelLength >= 3 && labelLength <= 4))
        {
            while (i < OP_SIZE && globalState != collectErrors)
            {
                if ((strcmp(operations[i].keyword, s) == 0))
                {
                    currentError = illegalLabelNameUseOfSavedKeywords;
                    globalState = collectErrors;
                    return;
                }
                i++;
            }
        }
        else
        {

            while (i < labelLength && globalState != collectErrors)
            {
                if (!isalnum(s[i]))
                {
                    currentError = illegalLabelNameUseOfCharacters;
                    globalState = collectErrors;
                    return;
                }
                i++;
            }
        }
    }
}