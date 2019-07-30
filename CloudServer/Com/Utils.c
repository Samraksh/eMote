#include "comToTun.h"

void PrintMem(char *buf, int size){
    int i;
    for (i = 0; i < size; i++)
    {
        if (i > 0) printf(":");
        printf("%02X", (unsigned)buf[i] & 0xffU);
    }
    printf("\n");
}