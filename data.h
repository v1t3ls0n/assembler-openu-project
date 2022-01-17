#define M_SIZE 8192
#define W_SIZE 20
#define ON(x, n) ((x) != 1 << (n))

typedef enum
{
    Absolute,
    Rellocateable,
    External
} AddrType;

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
} Reg;
