#include "data.h"
static Item *symbols[HASHSIZE] = {0};
static Item *macros[HASHSIZE] = {0};
static unsigned entriesCount = 0;
static unsigned externalCount = 0;

static ExtListItem *extListHead = NULL;
extern unsigned getICF();
extern Bool verifyLabelNaming(char *s);

void findAllExternals();
void addExtListItem(char *name);
void resetExtList();
ExtListItem *findExtOpListItem(char *name);
void updateExtPositionData(char *name, unsigned base, unsigned offset);
void freeTableItem(Item *item);
void freeTablesMemory();
ExtListItem *findExtOpListItem(char *name)
{
    extern ExtListItem *extListHead;
    ExtListItem *p = extListHead;
    while (p != NULL)
    {
        if (p->name)
        {
            if (strcmp(name, p->name) == 0)
                return p;
        }
        p = p->next;
    }
    return NULL;
}

void initTables()
{
    extern unsigned externalCount, entriesCount;
    int i = 0;
    if (extListHead != NULL)
        /*  resetExtList(); */

        externalCount = entriesCount = 0;
    while (i < HASHSIZE)
    {
        symbols[i] = NULL;
        macros[i] = NULL;
        i++;
    }
}
void resetExtList()
{
    ExtListItem *np = extListHead, *next = NULL;
    ExtPositionData *pos, *nextPos;
    externalCount = 0;

    while (next != NULL)
    {
        next = np->next;
        nextPos = np->value.next;
        while (nextPos != NULL)
        {
            pos = nextPos;
            nextPos = nextPos->next;
            /*free(pos);*/
        }
        /*free(np);*/
    }

    extListHead = NULL;
}

void updateExtPositionData(char *name, unsigned base, unsigned offset)
{

    ExtListItem *np = findExtOpListItem(name);
    /*     printf("line 62 in table inside update ext position data\nname:%s\nbase:%u\noffset:%u\nnp->name:%s\nnp->value.base:%d\n", name, base, offset, np->name, np->value.base);
     */
    if (np->value.base)
    {
        ExtPositionData *new = (ExtPositionData *)malloc(sizeof(ExtPositionData));
        new->base = base;
        new->offset = offset;
        new->next = np->value.next;
        np->value.next = new;
    }
    else
    {
        np->value.base = base;
        np->value.offset = offset;
    }

    externalCount++;
}

void addExtListItem(char *name)
{

    ExtListItem *next;
    next = (ExtListItem *)calloc(1, sizeof(ExtListItem));
    strncpy(next->name, name, strlen(name));

    if (extListHead != NULL)
    {
        next->next = extListHead->next;
        extListHead->next = next;
    }
    else
    {
        next->next = NULL;
        extListHead = next;
    }
}

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
    int i = hash(s);
    for (np = (type == Symbol ? symbols[i] : macros[i]); np != NULL; np = np->next)
        if (!strcmp(s, np->name))
            return np;

    return NULL;
}

Item *install(char *name, ItemType type)
{
    Item *np;
    unsigned hashval;

    if ((np = lookup(name, type == Symbol ? Symbol : Macro)) == NULL)
    {
        np = (Item *)malloc(sizeof(Item));
        if (np == NULL || (np->name = cloneString(name)) == NULL)
        {
            yieldError(memoryAllocationFailure);
            return NULL;
        }

        hashval = hash(name);
        np->next = (type == Symbol ? symbols[hashval] : macros[hashval]);
        if (type == Symbol)
            symbols[hashval] = np;
        else
            macros[hashval] = np;
    }
    else
    {
        printf("line 155\n");
        /*free((Item *)np->next);*/
    }

    if ((np->name = cloneString(name)) == NULL)
        return NULL;

    return np;
}

Bool addSymbol(char *name, unsigned value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal)
{
    unsigned base = 0;
    unsigned offset = 0;
    Item *p;
    if (name[strlen(name) - 1] == ':')
        name[strlen(name) - 1] = '\0';
    if (!verifyLabelNamingAndPrintErrors(name))
        return False;

    p = install(name, Symbol);
    if (p == NULL)
        return False;

    offset = value % 16;
    base = value - offset;
    p->val.s.value = value;
    p->val.s.base = base;
    p->val.s.offset = offset;
    p->val.s.attrs.code = isCode;
    p->val.s.attrs.entry = isEntry;
    p->val.s.attrs.external = isExternal;
    p->val.s.attrs.data = isData;
    return True;
}

Bool updateSymbol(Item *p, unsigned value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal)
{
    /*     printf("inside updateSymbol\n");
        printf("name:%s value:%d isCode:%u isData:%u isEntry:%u isExternal:%u\n", p->name, value, isCode, isData, isEntry, isExternal);
     */
    /*     printf("inside updateSymbol\n");
     */
    if (((p->val.s.attrs.external) && (value || isData || isEntry || isCode)))
        return yieldError(illegalOverrideOfExternalSymbol);

    else if ((p->val.s.attrs.code || p->val.s.attrs.data || p->val.s.attrs.entry) && isExternal)
        return yieldError(illegalOverrideOfLocalSymbolWithExternalSymbol);

    else
    {
        if ((isData && isCode) || (isCode && p->val.s.attrs.data) || (isData && p->val.s.attrs.code))
            return yieldError(illegalSymbolNameAlreadyInUse);

        if (value)
        {
            unsigned base = 0;
            unsigned offset = 0;
            offset = value % 16;
            base = value - offset;
            p->val.s.value = value;
            p->val.s.base = base;
            p->val.s.offset = offset;
        }

        if (isEntry)
            p->val.s.attrs.entry = 1;
        if (isCode)
            p->val.s.attrs.code = 1;
        if (isData)
            p->val.s.attrs.data = 1;
    }

    return True;
}

Item *getSymbol(char *name)
{
    return lookup(name, Symbol);
}

int getSymbolBaseAddress(char *name)
{
    Item *p = lookup(name, Symbol);
    if (p == NULL)
        return -1;

    return p->val.s.base;
}

int getSymbolOffset(char *name)
{
    Item *p = lookup(name, Symbol);
    if (p == NULL)
        return -1;

    return p->val.s.offset;
}

Bool isSymbolExist(char *name)
{
    return lookup(name, Symbol) != NULL ? True : False;
}
Bool isExternal(char *name)
{
    Item *p = lookup(name, Symbol);
    if (p == NULL)
        return False;
    return p->val.s.attrs.external;
}
Bool isEntry(char *name)
{
    Item *p = lookup(name, Symbol);
    if (p == NULL)
        return False;

    return p->val.s.attrs.entry == 1 ? True : False;
}

Bool isNonEmptyExternal(char *name)
{
    Item *p = lookup(name, Symbol);
    if (p == NULL)
        return False;

    return (p->val.s.attrs.code || p->val.s.attrs.data) ? True : False;
}

Bool isNonEmptyEntry(char *name)
{
    Item *p = lookup(name, Symbol);
    if (p == NULL)
        return False;
    return (p->val.s.attrs.code || p->val.s.attrs.data) ? True : False;
}

Bool isLabelNameAlreadyTaken(char *name, ItemType type)
{
    Item *p = lookup(name, type);

    if (name[strlen(name) - 1] == ':')
        name[strlen(name) - 1] = '\0';

    if (p != NULL)
    {
        if (type == Symbol)
        {
            if (p->val.s.attrs.data || p->val.s.attrs.code)
                return True;
            if (p->val.s.attrs.entry)
                return (!p->val.s.attrs.data && !p->val.s.attrs.code && !p->val.s.attrs.external) ? False : True;
            if (p->val.s.attrs.external)
                return (!p->val.s.attrs.data && !p->val.s.attrs.code && !p->val.s.attrs.entry) ? False : True;
        }

        else if (type == Macro)
            return p->val.m.start != -1 ? False : True;
    }

    return False;
}

Item *updateSymbolAddressValue(char *name, int newValue)
{
    Item *p = getSymbol(name);
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
        yieldError(symbolDoesNotExist);

    return p;
}

Item *getMacro(char *s)
{
    return lookup(s, Macro);
}

Item *addMacro(char *name, int start, int end)
{
    Item *macro = install(name, Macro);

    if (macro == NULL)
    {
        yieldError(illegalMacroNameAlreadyInUse);
        return NULL;
    }
    else
    {
        if (start != -1)
            macro->val.m.start = start;
        if (end != -1)
            macro->val.m.end = end;
    }

    return macro;
}
Item *updateMacro(char *name, int start, int end)
{
    Item *macro = getMacro(name);
    if (!macro)
        return NULL;
    if (start != -1)
        macro->val.m.start = start;
    if (end != -1)
        macro->val.m.end = end;

    return macro;
}

void updateFinalSymbolTableValues()
{
    int i = 0;
    while (i < HASHSIZE)
    {
        if (symbols[i] != NULL)
            updateFinalValueOfSingleItem(symbols[i]);
        i++;
    }
}

void updateFinalValueOfSingleItem(Item *item)
{
    if (item->val.s.attrs.entry)
        entriesCount++;
    if (item->val.s.attrs.external)
        addExtListItem(item->name);

    if (item->val.s.attrs.data)
    {
        unsigned base = 0, offset = 0, newValue = item->val.s.value + getICF();
        offset = newValue % 16;
        base = newValue - offset;
        item->val.s.offset = offset;
        item->val.s.base = base;
        item->val.s.value = newValue;
    }

    if (item->next != NULL)
        updateFinalValueOfSingleItem(item->next);
}

Bool areEntriesExist()
{
    return entriesCount > 0 ? True : False;
}
Bool areExternalsExist()
{
    return externalCount > 0 ? True : False;
}

void writeExternalsToFile(FILE *fp)
{
    ExtListItem *p = extListHead;
    while (p != NULL)
    {
        if (p->value.base)
            writeSingleExternal(fp, p->name, p->value.base, p->value.offset, p->value.next);
        p = p->next;
    }
}

void writeSingleExternal(FILE *fp, char *name, unsigned base, unsigned offset, ExtPositionData *next)
{

    fprintf(fp, "%s BASE %u\n", name, base);
    fprintf(fp, "%s OFFSET %u\n", name, offset);
    if (next != NULL)
        writeSingleExternal(fp, name, next->base, next->offset, next->next);
}

void writeEntriesToFile(FILE *fp)
{
    int i = 0;
    int totalCount = 0;
    while (i < HASHSIZE)
    {
        if (symbols[i] != NULL && totalCount <= entriesCount)
            totalCount += writeSingleEntry(symbols[i], fp, 0);
        i++;
    }
}

int writeSingleEntry(Item *item, FILE *fp, int count)
{
    if (item->val.s.attrs.entry)
    {
        fprintf(fp, "%s,%d,%d\n", item->name, item->val.s.base, item->val.s.offset);
        count++;
    }
    if (item->next != NULL)
        writeSingleEntry(item->next, fp, count);

    return count;
}

/* void freeHashTable(ItemType type)
{

    int i = 0;
    while (i < HASHSIZE)
    {

        if (type == Symbol && symbols[i] != NULL)
            freeTableItem(symbols[i]);

        else if (type == Macro && macros[i] != NULL)
            freeTableItem(macros[i]);

        i++;
    }
} */

/* void freeTableItem(Item *item)
{

    if (item->next != NULL)
        freeTableItem(item->next);
    free(item->next);
    return;
} */

void printMacroTable()
{
    int i = 0;
    printf("\n\t ~ MACRO TABLE ~ \n");
    printf("\tname\tstart\tend");
    while (i < HASHSIZE)
    {
        if (macros[i] != NULL)
            printMacroItem(macros[i]);
        i++;
    }
    printf("\n\n");
}

int printMacroItem(Item *item)
{

    printf("\n\t%s\t %5d\t%6d", item->name, item->val.m.start, item->val.m.end);
    if (item->next != NULL)
        printMacroItem(item->next);
    return 0;
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

int printSymbolItem(Item *item)
{
    /*  printf("line 94, inside printSymbolItem \n");
     */

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
    return 0;
}
