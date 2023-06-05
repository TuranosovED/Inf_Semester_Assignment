#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Clearbuffer();
void PushLine(char **s, char value);
void RefreshStr(char **s);
int CompareStr(char *s1, char *s2);
char * ReadLine();
void PopLine(char **s);