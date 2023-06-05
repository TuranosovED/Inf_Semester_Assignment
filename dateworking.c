#include "dateworking.h"

char *MakeTimeStr() //Создание строки содержащей текущие дату и время
{
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);//Получение cтруктуры содержащей текущие дату и время

    char *FileName = (char*)calloc(50,sizeof(char));
    char dateBuffer[16];

    int day = local->tm_mday;
    int mounth = local->tm_mon;
    int year = local->tm_year;

    int hours = local->tm_hour;
    int minutes = local->tm_min;
    int seconds = local->tm_sec;

    if(day/10 == 0)
        strcat(FileName,"0");
    itoa(day,dateBuffer,10);
    strcat(FileName,dateBuffer);
    strcat(FileName,".");

    if((mounth+1)/10 == 0)
        strcat(FileName,"0");
    itoa(mounth+1,dateBuffer,10);
    strcat(FileName,dateBuffer);
    strcat(FileName,".");

    itoa(year+1900,dateBuffer,10);
    strcat(FileName,dateBuffer);
    strcat(FileName,"_");

    if(hours/10 == 0)
        strcat(FileName,"0");
    itoa(hours,dateBuffer,10);
    strcat(FileName,dateBuffer);
    strcat(FileName,".");

    if(minutes/10 == 0)
        strcat(FileName,"0");
    itoa(minutes,dateBuffer,10);
    strcat(FileName,dateBuffer);
    strcat(FileName,".");

    if(seconds/10 == 0)
        strcat(FileName,"0");
    itoa(seconds,dateBuffer,10);
    strcat(FileName,dateBuffer);

    return FileName;
}

Date ParceDate(char *date) //Парсинг даты
{
    int counter = 0;
    int day = 0, mounth = 0, year = 0;
    char* buffer = (char*)calloc(1, sizeof(char));
    for (int i = 0; date[i] != '\0'; i++)
    {
        if (date[i] != '.')
            PushLine(&buffer, date[i]);
        else
        {
            switch (counter)
            {
            case 0:
                day = atoi(buffer);
                RefreshStr(&buffer);
                counter++;
                break;
            case 1:
                mounth = atoi(buffer);
                RefreshStr(&buffer);
                break;
            }
        }
    }
    year = atoi(buffer); 
    return (Date){day,mounth,year};
}

int CompareDates(Date date1,Date date2) //return 1, if date1 > date2, return 2 if date 1 = date 2, return - if date1 < date2
{
    if(date1.year >= date2.year)
    {
        if(date1.year == date2.year)
        {
            if(date1.mounth >= date2.mounth)
            {
                if(date1.mounth == date2.mounth)
                {
                    if(date1.day >= date2.day)
                    {
                        if(date1.day == date2.day)
                            return 2;
                        else
                            return 1;
                    }
                    else
                        return 0;
                }
                else 
                    return 1;
            }
            else
                return 0;
        }
        else
            return 1;
    }
    else
        return 0;
}