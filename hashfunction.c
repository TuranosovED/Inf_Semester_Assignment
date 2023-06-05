#include "hashfunction.h"

int GetHash(char *s, int capacity) //Хэш функция
{
    int sum = 0;
    for (int i = 0; i < s[i] != '\0'; i++)
        sum += s[i];

    return sum % capacity;
}