#pragma once
#include <stdio.h>
#include <stdlib.h>

#include "hashfunction.h"
#include "lines.h"
#include "defines.h"

typedef struct Users
{
    char *UserName;
    char *Password;
    int StudentBaseAccess;
    int BooksBaseAccess;
}Users;

typedef struct UsersDataBase
{
    Users user;
    struct UsersDataBase* next;
}UsersDataBase;

void FillStructUsers(Users *user, int structId, char *s);
void PrintUser(Users Field);
UsersDataBase **InitDataBaseUsers(int capacity);
int PushHashTableUsers(UsersDataBase **HashTable, int capacity, Users value);
void PrintHashTableUsers(UsersDataBase **HashTable, int capacity);
int ReadCsvUsers(UsersDataBase **HashTable, int capacity,char*FileName);
int AccessUser(UsersDataBase** HashTable,int capacity,Users *CurrentUser);