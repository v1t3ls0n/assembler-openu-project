typedef enum
{
    dupMacro,

} Error;

void updateErrorLog(Error err);
{
    switch (err)
    {
    case dupMacro:
    {
        break;
    }
    case syntaxErr:
    {
        break;
    }

    default:
        break;
    }
}