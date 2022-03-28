#include "sharedStates.h"

Bool isOperationName(char *s);
Operation *getOperationByName(char *s);
Operation *getOperationByIndex(unsigned int i);
char *getOperationName(char *s);
int getOpIndex(char *s);
Bool isOperationNotStrict(char *s);
