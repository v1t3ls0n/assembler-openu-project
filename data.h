#define M_SIZE 8192
#define W_SIZE 20
#define ON(x, n) ((x) != 1 << (n))
#define ABSOLUTE

typedef enum
{
    Absolute,
    Relocateable,
    External
} ALLOCATION_METHOD;

typedef enum
{
    r0,
    r1,
    r2,
    r3,
    r4,
    r5,
    r6,
    r7,
    r8,
    r9,
    r10,
    r11,
    r12,
    r13,
    r14,
    r15
} REG_ID;

/*
typedef struct
{
    Alloc are;
    Opcode opcode;
} firstWord;

typedef struct
{
    Alloc are;
    unsigned int funct;
    unsigned int srcReg;
    unsigned int srcAddr;
    unsigned int desReg;
    unsigned int desAddr;
} secondWord;



union struct
{
    firstWord,
        secondWord,
        extraWord

} word;
typedef struct
{
unsigned int r0 : 1;
unsigned int r1 : 1;
unsigned int r2 : 1;
unsigned int r3 : 1;
unsigned int r4 : 1;
unsigned int r5 : 1;
unsigned int r6 : 1;
unsigned int r7 : 1;
unsigned int r8 : 1;
unsigned int r9 : 1;
unsigned int r10 : 1;
unsigned int r11 : 1;
unsigned int r12 : 1;
unsigned int r13 : 1;
unsigned int r14 : 1;
unsigned int r15 : 1;

} REG;
*/
