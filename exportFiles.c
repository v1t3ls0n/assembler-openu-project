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
