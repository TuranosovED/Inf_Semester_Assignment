#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "hashfunction.h"
#include "lines.h"
#include "Fields.h"
#include "defines.h"

typedef struct Borrowers
{
    char *ISBN;
    char *id;
    char *date;
}Borrowers;

typedef struct BorrowersDataBase
{
    struct Borrowers borrower;
    struct BorrowersDataBase *next;
}BorrowersDataBase;

void FillStructBorrower(Borrowers *borrower,  char *Field, char *s);
BorrowersDataBase **InitDataBaseBorrowers(int capacity);
int PushHashTableBorrower(BorrowersDataBase **HashTable, int capacity, Borrowers value);
void PrintHashTableBorrowers(BorrowersDataBase **HashTable, int capacity);
int IsBookBorrow(BorrowersDataBase **HashTable,int capacity,char *value);
int ReadCsvBorrowers(BorrowersDataBase **HashTable, int capacity,char*FileName);
Borrowers MakeBorrower();
int IsStudentBorrow(BorrowersDataBase **HashTable,int capacity,char *value);
void PrintBorrower(Borrowers Field);