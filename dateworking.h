#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "lines.h"
#include "defines.h"

typedef struct Date
{
    int day;
    int mounth;
    int year;
}Date;

char *MakeTimeStr();
Date ParceDate(char *date);
int CompareDates(Date date1,Date date2);