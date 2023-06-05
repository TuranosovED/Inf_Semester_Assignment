#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <direct.h>

#include "defines.h"
#include "dateworking.h"

typedef struct FileDataArray
{
    WIN32_FIND_DATA *Array;
    int size;
}FileDataArray;

char * MakeBackupFileName(int mode);
void PushFileData(FileDataArray **Data,WIN32_FIND_DATA value);
FileDataArray* MakeBackupCatalog(int mode);
char* ChooseFileFromCatalog(FileDataArray *Data,int mode);
void RefreshFileData(FileDataArray** Data,int mode);
void MakeDirs();