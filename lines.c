#include "lines.h"

void Clearbuffer() //Очистка буфера консоли
{
    char c;
    while((c = getchar())!='\n');
}

void PushLine(char **s, char value) //Добавление символа в строку
{
    int len = strlen(*s);
    char *buffer = (char *)malloc((len + 2) * sizeof(char));
    for (int i = 0; i < len; i++)
    {
        buffer[i] = (*s)[i];
    }
    buffer[len] = value;
    buffer[len + 1] = '\0';
    free(*s);
    *s = buffer;
}

void PopLine(char **s) //Удаление символа из строки
{
    int len = strlen(*s);
    char *buffer = (char*)malloc(len * sizeof(char));
    for(int i =0;i<len-1;i++)
        buffer[i] = (*s)[i];
    buffer[len-1] = '\0';
    free(*s);
    *s = buffer;
}

void RefreshStr(char **s) //Переинициализация строки
{
    *s = (char *)calloc(1,sizeof(char));
}

// *s = (char *)malloc(sizeof(char));
// (*s)[0] = '\0';

int CompareStr(char *s1, char *s2) //Сравнение строк
{
    if(strlen(s1) != strlen(s2))
        return 0;
    for (int i = 0; (s1[i] != '\0') && (s2[i] != '\0'); i++)
        if (s1[i] != s2[i])
            return 0;
    return 1;
}

char * ReadLine() //Считывание строки произвольной длины из консоли
{
    char c;
    char *str = (char*)malloc(sizeof(char));
    str[0] = '\0';
    while((c = getchar()) != '\n')
        PushLine(&str,c);
    return str;
}