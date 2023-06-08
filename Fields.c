#include "Fields.h"

void PrintBackUpField(char *s,FILE *f)
{
    int trigger = 0;
    for(int i =0;s[i]!='\0';i++)
    {
        switch(s[i])
        {
            case ',':
                trigger = 1;
                break;
            case ';':
                trigger = 1;
                break;
            case '"':
                trigger = 1;
                break;
        }
    }
    system("pause");
    if(trigger)
    {
        fprintf(f,"\"%s\",",s);
    }
    else
    {
        fprintf(f,"%s,",s);
    }
}

char** MakeTableFieldsMass(FILE *f,int *size)
{
    char c;
    char *s = (char*)calloc(1,sizeof(char));
    while((c = fgetc(f))!='\n')
    {
        if(c == ',')
            (*size)++;
        PushLine(&s,c);
    }
    PushLine(&s,c);

    char **Mas = (char**)calloc((*size),sizeof(char*));

    for(int i =0;i<(*size);i++)
        Mas[i] = (char*)calloc(1,sizeof(char));

    int prev = 0;

    for(int i =0;i<(*size);i++)
    {
        for(int j = prev;(s[j] != ',') && (s[j] != '\n');j++)
        { 
            PushLine(&Mas[i],s[j]);
            prev = j;
        }
        prev+=2;
    }
    free(s);
    return Mas;
}

void FreeFieldMas(char ***mas,int size)
{
    for(int i =0;i < size;i++)
        free((*mas)[i]);
    free(*mas);
}