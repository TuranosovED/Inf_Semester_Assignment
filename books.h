#pragma once
#include <stdio.h>
#include <stdlib.h>

#include "hashfunction.h"
#include "filesworking.h"
#include "defines.h"

typedef struct Books
{
    char *ISBN;       // 0
    char *Initials;   // 1
    char *BookName;   // 2
    int BookCount;   // 3
    int FreeBooks;   // 4
}Books;

typedef struct BooksDataBase
{
    struct Books book;
    struct BooksDataBase *next;
}BooksDataBase;

void FillStructBooks(Books *book, int structId, char *s);
void PrintBook(Books Field);
BooksDataBase **InitDataBaseBooks(int capacity);
void RefreshHashtableBook(BooksDataBase *** HashTable,int capacity);
int PushHashTableBooks(BooksDataBase **HashTable, int capacity, Books value);
void SortHashTableBookByISBN(BooksDataBase **HashTable, int capacity);
void PrintHashTableBook(BooksDataBase **HashTable, int capacity);
int ReadCsvBook(BooksDataBase **HashTable, int capacity,char*FileName);
Books MakeBook();
int BackupBook(BooksDataBase ** HashTable,int capacity);
int FindByISBN(BooksDataBase** HashTable,int capacity,char *value);