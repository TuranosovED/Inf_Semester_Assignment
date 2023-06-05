#pragma once
#include <stdio.h>
#include <stdlib.h>

#include "hashfunction.h"

#include "lines.h"
#include "filesworking.h"
#include "defines.h"

typedef struct Students
{
    char *id;         // 0  - struct id
    char *secondName; // 1
    char *firstName;  // 2
    char *thirdName;  // 3
    char *faculty;    // 4
    char *speciality; // 5
} Students;

typedef struct StudentsDataBase
{
    struct Students student;
    struct StudentsDataBase *next;
}StudentsDataBase;

void FillStructStudent(Students *student, int structId, char *s);
void PrintStudent(Students Field);
StudentsDataBase **InitDataBaseStudent(int capacity);
int  PushHashTableStudent(StudentsDataBase **HashTable, int capacity, Students value);
void PrintHashTableStudent(StudentsDataBase **HashTable, int capacity);
int ReadCsvStudent(StudentsDataBase **HashTable, int capacity,char*FileName);
Students MakeStudent();
int BackupStudent(StudentsDataBase ** HashTable,int capacity);
int FindBySecondName(StudentsDataBase** HashTable,int capacity,char *value);
int FindById(StudentsDataBase** HashTable,int capacity,char *value);
void RefreshHashtableStudent(StudentsDataBase *** HashTable,int capacity);