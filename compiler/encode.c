#include "data.h"

/*
Errors:
C:\Users\guyvi\dev\msys64\mingw64\bin\gcc.exe -fdiagnostics-color=always -g C:\Users\guyvi\dev\msys64\home\guyvi\compiler-openu\compiler\encode.c -o C:\Users\guyvi\dev\msys64\home\guyvi\compiler-openu\compiler\encode.exe
C:/Users/guyvi/dev/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/11.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:\Users\guyvi\AppData\Local\Temp\cccYPwLb.o: in function `generateFirstWordEncodedToBinary':
C:/Users/guyvi/dev/msys64/home/guyvi/compiler-openu/compiler/encode.c:6: undefined reference to `decToHex'
C:/Users/guyvi/dev/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/11.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:/Users/guyvi/dev/msys64/home/guyvi/compiler-openu/compiler/encode.c:6: undefined reference to `hexToBin'
C:/Users/guyvi/dev/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/11.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:/Users/guyvi/dev/msys64/home/guyvi/compiler-openu/compiler/encode.c:6: undefined reference to `decToHex'
C:/Users/guyvi/dev/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/11.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:/Users/guyvi/dev/msys64/home/guyvi/compiler-openu/compiler/encode.c:6: undefined reference to `hexToBin'
C:/Users/guyvi/dev/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/11.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:\Users\guyvi\AppData\Local\Temp\cccYPwLb.o: in function `encodeIntNum':
C:/Users/guyvi/dev/msys64/home/guyvi/compiler-openu/compiler/encode.c:27: undefined reference to `decToHex'
C:/Users/guyvi/dev/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/11.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:/Users/guyvi/dev/msys64/home/guyvi/compiler-openu/compiler/encode.c:28: undefined reference to `hex2int'
C:/Users/guyvi/dev/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/11.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:/Users/guyvi/dev/msys64/home/guyvi/compiler-openu/compiler/encode.c:29: undefined reference to `hex2int'
C:/Users/guyvi/dev/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/11.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:/Users/guyvi/dev/msys64/home/guyvi/compiler-openu/compiler/encode.c:30: undefined reference to `hex2int'
C:/Users/guyvi/dev/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/11.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:/Users/guyvi/dev/msys64/home/guyvi/compiler-openu/compiler/encode.c:31: undefined reference to `hex2int'
C:/Users/guyvi/dev/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/11.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:/Users/guyvi/dev/msys64/home/guyvi/compiler-openu/compiler/encode.c:32: undefined reference to `hex2int'
C:/Users/guyvi/dev/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/11.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:\Users\guyvi\AppData\Local\Temp\cccYPwLb.o: in function `printBinaryFile':
C:/Users/guyvi/dev/msys64/home/guyvi/compiler-openu/compiler/encode.c:51: undefined reference to `decToHex'
C:/Users/guyvi/dev/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/11.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:/Users/guyvi/dev/msys64/home/guyvi/compiler-openu/compiler/encode.c:51: undefined reference to `hexToBin'
C:/Users/guyvi/dev/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/11.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:/Users/guyvi/dev/msys64/home/guyvi/compiler-openu/compiler/encode.c:52: undefined reference to `decToHex'
C:/Users/guyvi/dev/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/11.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:/Users/guyvi/dev/msys64/home/guyvi/compiler-openu/compiler/encode.c:52: undefined reference to `hexToBin'
C:/Users/guyvi/dev/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/11.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:/Users/guyvi/dev/msys64/home/guyvi/compiler-openu/compiler/encode.c:53: undefined reference to `decToHex'
C:/Users/guyvi/dev/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/11.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:/Users/guyvi/dev/msys64/home/guyvi/compiler-openu/compiler/encode.c:53: undefined reference to `hexToBin'
C:/Users/guyvi/dev/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/11.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:/Users/guyvi/dev/msys64/home/guyvi/compiler-openu/compiler/encode.c:54: undefined reference to `decToHex'
C:/Users/guyvi/dev/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/11.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:/Users/guyvi/dev/msys64/home/guyvi/compiler-openu/compiler/encode.c:54: undefined reference to `hexToBin'
C:/Users/guyvi/dev/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/11.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:/Users/guyvi/dev/msys64/home/guyvi/compiler-openu/compiler/encode.c:55: undefined reference to `decToHex'
C:/Users/guyvi/dev/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/11.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:/Users/guyvi/dev/msys64/home/guyvi/compiler-openu/compiler/encode.c:55: undefined reference to `hexToBin'
collect2.exe: error: ld returned 1 exit status

*/
void main() { printf("\n\n\n\n\n\nTest somefile.c (in fake main function)\n\n\n\n\n\n"); }

char *generateFirstWordEncodedToBinary(Command *cmd)
{
    return strcat(hexToBin(decToHex(A)), hexToBin(decToHex(cmd->op)));
}

EncodedWord *generateFirstWordEncodedHex(Command *cmd)
{
    char *buf = (char *)calloc(6, sizeof(char));
    EncodedWord *newWord = (EncodedWord *)malloc(1 * sizeof(EncodedWord *));
    sprintf(buf, "%x", (A | cmd->op));
    newWord->_A = buf[0];
    newWord->_B = buf[1];
    newWord->_C = buf[2];
    newWord->_D = buf[3];
    newWord->_E = buf[4];
    free(buf);
    return newWord;
}
EncodedWord *encodeIntNum(int num)
{
    char *buf;
    EncodedWord *newWord;
    newWord = (EncodedWord *)malloc(1 * sizeof(EncodedWord *));
    buf = decToHex(num);
    newWord->_A = buf[0] ? hex2int(buf[0]) : '0';
    newWord->_B = buf[1] ? hex2int(buf[1]) : '0';
    newWord->_C = buf[2] ? hex2int(buf[2]) : '0';
    newWord->_D = buf[3] ? hex2int(buf[3]) : '0';
    newWord->_E = buf[4] ? hex2int(buf[4]) : '0';
    free(buf);
    return newWord;
}

void printObjectFile(EncodedWord *words[], unsigned int ICF, unsigned int DCF)
{
    int i;
    printf("              %d   %d\n", ICF, DCF);
    for (i = 0; i < (DCF + ICF) - 1; i++) /*  */
        printf("%04d A%x-B%x-C%x-D%x-E%x\n", 100 + i, words[i]->_A, words[i]->_B, words[i]->_C, words[i]->_D, words[i]->_E);
}

void printBinaryFile(EncodedWord *words[], unsigned int ICF, unsigned int DCF)
{
    int i;
    for (i = 0; i < (DCF + ICF); i++)
    {
        printf("%04d ", 100 + i);
        printf("%s", hexToBin(decToHex(words[i]->_A)));
        printf("%s", hexToBin(decToHex(words[i]->_B)));
        printf("%s", hexToBin(decToHex(words[i]->_C)));
        printf("%s", hexToBin(decToHex(words[i]->_D)));
        printf("%s\n", hexToBin(decToHex(words[i]->_E)));
    }
}