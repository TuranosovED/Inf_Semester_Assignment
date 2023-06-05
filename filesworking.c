#include "filesworking.h"

char * MakeBackupFileName(int mode) //Создание строки определенного вида для бэкапа
{

    char *FileName = (char*)calloc(50,sizeof(char));
    
    if(mode == STUDENT)
        strcat(FileName,"Backup\\students\\students_");
    else if(mode == BOOKS)
        strcat(FileName,"Backup\\books\\books_");
    
    strcat(FileName,MakeTimeStr());
    strcat(FileName,".csv");
    return FileName;
}

void PushFileData(FileDataArray **Data,WIN32_FIND_DATA value) //Добавление информации о файле(бэкапа) в таблицу
{
    WIN32_FIND_DATA *buffer = (WIN32_FIND_DATA*)malloc(sizeof(WIN32_FIND_DATA) * ((*Data)->size + 1));
    for(int i =0;i<(*Data)->size;i++)
    {
        buffer[i] = (*Data)->Array[i];
    }   
    buffer[(*Data)->size] = value;
    free((*Data)->Array);
    (*Data)->Array = buffer;
    (*Data)->size++;
}

FileDataArray* MakeBackupCatalog(int mode) //Создание таблицы содержащей список файлов в папке с бэкапом
{
    WIN32_FIND_DATA FileDataBuffer; 
    FileDataArray *Data = (FileDataArray*)malloc(sizeof(FileDataArray));
    HANDLE fileHandle;
    Data->size = 0;
    Data->Array = NULL;
    if(mode == BOOKS)
        fileHandle = FindFirstFile("Backup\\books\\*",&FileDataBuffer);
    else if(mode == STUDENT)
        fileHandle = FindFirstFile("Backup\\students\\*",&FileDataBuffer);
    if(FileDataBuffer.cFileName[0] != '.')
        PushFileData(&Data,FileDataBuffer);
    while(FindNextFile(fileHandle,&FileDataBuffer))
    {
        if(FileDataBuffer.cFileName[0] != '.')
            PushFileData(&Data,FileDataBuffer);
        
    }
    
    return Data;
}

char* ChooseFileFromCatalog(FileDataArray *Data,int mode) //Выбор определенного файла(бэкапа) из списка
{
    int command;
    char *FileName = (char*)calloc(256,sizeof(char));
    if(mode == BOOKS)
        strcat(FileName,"Backup\\books\\");
    else if(mode == STUDENT)
        strcat(FileName,"Backup\\students\\");
    for(int i =0;i<Data->size;i++)
    {
        printf("%d: %s\n",i,Data->Array[i].cFileName);
    }
    puts("Select file number:");
    Clearbuffer();
    scanf("%d",&command);
    while(command >= Data->size)
    {
        puts("try again please");
        scanf("%d", &command);
    }
    strcat(FileName,Data->Array[command].cFileName);
    return FileName;
}

void RefreshFileData(FileDataArray** Data,int mode) //Переинициализация Таблицы
{
    if(*Data != NULL)
        free(*Data);
    *Data = MakeBackupCatalog(mode);
}

void MakeDirs() //Создание папок для функций
{
    mkdir("logs");
    mkdir("Backup");
    mkdir("Backup\\students");
    mkdir("Backup\\books");
}