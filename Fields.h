#include "lines.h"

void PrintBackUpField(char *s,FILE *f);
char** MakeTableFieldsMass(FILE *f,int *size);
void FreeFieldMas(char ***mas,int size);