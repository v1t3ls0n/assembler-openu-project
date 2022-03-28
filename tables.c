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

ExtListItem *findExtOpListItem(char *name)
{
    ExtListItem *p = extListHead;
    while (p != NULL)
    {
        if (strcmp(name, p->name) == 0)
            return p;
        p = p->next;
    }
    return NULL;
}

void resetExtList()
{
    ExtListItem *np = extListHead, *next = NULL;
    ExtPositionData *pos, *nextPos;
    externalCount = 0;
    entriesCount = 0;
    while (next != NULL)
    {
        next = np->next;
        free(np->name);
        nextPos = np->value;
        while (nextPos != NULL)
        {
            pos = nextPos;
            nextPos = nextPos->next;
            free(pos);
        }
        free(np);
    }
}

void updateExtPositionData(char *name, unsigned base, unsigned offset)
{

    ExtListItem *np = findExtOpListItem(name);
    ExtPositionData *new = (ExtPositionData *)malloc(sizeof(ExtPositionData *));
    new->base = base;
    new->offset = offset;
    new->next = NULL;
    new->next = np->value;
    np->value = new;
}

void addExtListItem(char *name)
{
    ExtListItem *next;
    next = (ExtListItem *)malloc(sizeof(ExtListItem *));
    next->name = name;
    next->value = (ExtPositionData *)malloc(sizeof(ExtPositionData *));
    next->value->base = 0;
    next->value->offset = 0;
    next->value->next = NULL;
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
    unsigned hashval;
    Item *np;
    np = (Item *)malloc(sizeof(Item));
    np->name = (char *)calloc(strlen(name) + 1, sizeof(char *));
    if (np == NULL || np->name == NULL)
    {
        yieldError(memoryAllocationFailure);
        return NULL;
    }
    else
    {
        memcpy(np->name, name, strlen(name));
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
        else if (type == Macro)
        {
            np->val.m.start = -1;
            np->val.m.end = -1;
        }

        hashval = hash(name);

        np->next = (type == Symbol ? symbols[hashval] : macros[hashval]);
        if (type == Symbol)
            symbols[hashval] = np;
        else
            macros[hashval] = np;
    }

    return np;
}

Bool addSymbol(char *name, unsigned value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal)
{
    unsigned base;
    unsigned offset;
    Item *p;

    if (name[strlen(name) - 1] == ':')
        name[strlen(name) - 1] = '\0';

    if (!verifyLabelNamingAndPrintErrors(name))
        return False;
    p = lookup(name, Symbol);
    if (p != NULL)
        return updateSymbol(p, value, isCode, isData, isEntry, isExternal);
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
    Item *macro = lookup(name, Macro);

    if (macro != NULL)
    {
        yieldError(illegalMacroNameAlreadyInUse);
        return NULL;
    }
    else
    {
        macro = install(name, Macro);

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
    {
        externalCount++;
        printf("item->name:%s\n", item->name);
        addExtListItem(cloneString(item->name));
    }

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
        writeSingleExternal(fp, p->name, p->value);
        p = p->next;
    }
}

void writeSingleExternal(FILE *fp, char *name, ExtPositionData *value)
{
    fprintf(fp, "%s BASE %u\n", name, value->base);
    fprintf(fp, "%s OFFSET %u\n", name, value->offset);
    if (value->next != NULL)
        writeSingleExternal(fp, name, value->next);
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

void initTables()
{
    int i = 0;

    if (externalCount > 0 && extListHead != NULL)
        resetExtList();

    while (i < HASHSIZE)
    {
        symbols[i] = NULL;
        macros[i] = NULL;
        i++;
    }
}

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
