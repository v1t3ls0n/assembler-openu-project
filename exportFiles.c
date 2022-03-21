#include "data.h"

void generateObFile(char *filename)
{
    FILE *ob;
    ob = fopen(strcat(filename, ".ob"), 'w+');
    if (ob == NULL)
    {
        printf("failed to create new .am file for the source expanded source code\n");
        printf("Press any key to exit...\n");
    }
}

void createEntriesFile(char *filename)
{
}

void createExternalFile(char *filename)
{
}

State createExpandedSourceFile(FILE *source, char *fileName)
{
    FILE *target;
    /*     int c = 0; */
    fileName[strlen(fileName) - 1] = 'm';
    target = fopen(fileName, "w+");
    if (target == NULL)
    {
        fclose(source);
        printf("failed to create new .am file for the source expanded source code\n");
        printf("Press any key to exit...\n");
        exit(1);
    }
    parseAndReplaceMacros(source, target);
    rewind(target);
    fclose(source);
    return target;
}