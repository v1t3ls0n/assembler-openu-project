Bool yieldError(Error err);
void yieldErrorIntoFile(Error err);
Bool yieldWarning(Warning err);
void yieldWarningIntoFile(Warning err);
void fileOpeningFailure(char *fileName);
void fileCreationFailure(char *fileName);
void closeOpenLogFiles();
