#include "books.h"

void FillStructBooks(Books *book, int structId, char *s) //Заполнение структуры по номеру поля
{
    switch (structId)
    {
    case 0:
        book->ISBN = s;
        break;
    case 1:
        book->Initials = s;
        break;
    case 2:
        book->BookName = s;
        break;
    case 3:
        book->BookCount = atoi(s);
        break;
    case 4:
        book->FreeBooks = atoi(s);
        break;
    default:
        break;
    }
}

void PrintBook(Books Field) //Вывод структуры
{
    printf("ISBN - %s; ", Field.ISBN);
    printf("Initials - %s; ", Field.Initials);
    printf("BookName - %s; ", Field.BookName);
    printf("BookCount - %d; ", Field.BookCount);
    printf("FreeBooks - %d;\n", Field.FreeBooks);
}

BooksDataBase **InitDataBaseBooks(int capacity) //Инициализация таблицы
{
    BooksDataBase **HashTable = (BooksDataBase **)malloc(capacity * sizeof(BooksDataBase *));
    for (int i = 0; i < capacity; i++)
        HashTable[i] = NULL;
    return HashTable;
}

void RefreshHashtableBook(BooksDataBase *** HashTable,int capacity) //Переинициализация таблицы
{
    free(*HashTable);
    *HashTable = InitDataBaseBooks(capacity);
}

int PushHashTableBooks(BooksDataBase **HashTable, int capacity, Books value) //Добавление элемента в таблицу
{
    int key = GetHash(value.ISBN, capacity);
    BooksDataBase *newValue = (BooksDataBase *)malloc(sizeof(BooksDataBase));
    newValue->book.BookCount = value.BookCount;
    newValue->book.BookName = value.BookName;
    newValue->book.FreeBooks = value.FreeBooks;
    newValue->book.Initials = value.Initials;
    newValue->book.ISBN = value.ISBN;
    for (BooksDataBase *head = HashTable[key]; head != NULL; head = head->next)
        if (CompareStr(head->book.ISBN, value.ISBN))
        {
            head->book = newValue->book;
            puts("The field has been updated!");
            return 0;
        }
    newValue->next = HashTable[key];
    HashTable[key] = newValue;
    return 1;
}

void SortHashTableBookByISBN(BooksDataBase **HashTable, int capacity) //Сортировка по ISBN и вывод в консоль
{
    int count = 0;
    for(int i =0;i<capacity;i++)
        for(BooksDataBase *head = HashTable[i];head!=NULL;head = head ->next)
            count++;
    Books *bufferArray = (Books*)malloc(sizeof(Books) * count);

    count = 0;

    for(int i =0;i<capacity;i++)
        for(BooksDataBase *head = HashTable[i];head!=NULL;head = head ->next)
            bufferArray[count++] = head->book;

    for(int i =0;i<count;i++)
    {
        for(int j = i+1;j<count;j++)
        {
            if(atoll(bufferArray[i].ISBN) > atoll(bufferArray[j].ISBN))
            {
                swap(Books,bufferArray[j],bufferArray[i]);
            }
        }
    }
        
    for(int i =0;i<count;i++)
        PrintBook(bufferArray[i]);
}

void PrintHashTableBook(BooksDataBase **HashTable, int capacity) //Вывод таблицы в консоль
{ 
    for (int i = 0; i < capacity; i++)
    {
        //printf("%d:\n", i);
        for (BooksDataBase *head = HashTable[i]; head != NULL; head = head->next)
        {
            PrintBook(head->book);
        }
        //putchar('\n');
    }
}

int ReadCsvBook(BooksDataBase **HashTable, int capacity,char*FileName) //Считывание csv
{
    int structCounter = 0;
    char sign;
    char* str = (char*)calloc(1, sizeof(char));
    int trigger = 0;
    char prev;
    FILE* f;

    if(FileName == NULL)
    {
        f = fopen("books.csv", "r");
    }
    else 
        f = fopen(FileName,"r");

    if(f == NULL)
    {
        puts("File is not founded!");
        return 0;
    }
    
    Books bufferBook;

    while (1)
    {
        sign = fgetc(f);
        switch (sign)
        {
        case ';':
            if (prev == '"')
            {
                trigger = 0;
                PopLine(&str);
            }
            if (!trigger)
            {
                FillStructBooks(&bufferBook, structCounter++, str);
                RefreshStr(&str);
            }
            else
            {
                PushLine(&str, sign);
            }
            break;
        case '\n':
            if (prev == '"')
            {
                trigger = 0;
                PopLine(&str);
            }
            FillStructBooks(&bufferBook, structCounter++, str);
            PushHashTableBooks(HashTable, capacity, bufferBook);
            RefreshStr(&str);
            structCounter = 0;
            trigger = 0;
            break;
        case '"':
            if(!trigger)
                trigger = 1;
            else
                PushLine(&str, sign);
            break;
        default:
            if (!feof(f))
                PushLine(&str, sign);
            break;
        }
        prev = sign;
        if (feof(f))
        {
            if (str[0] != '\0')
            {
                FillStructBooks(&bufferBook, structCounter++, str);
                PushHashTableBooks(HashTable, capacity, bufferBook);
            }
            break;
        }
    }
    fclose(f);
    return 1;
}

Books MakeBook() //Создание объекта
{
    Clearbuffer();
    Books book;
    puts("enter BookCount:");
    book.BookCount = atoi(ReadLine());
    puts("enter BookName:");
    book.BookName = ReadLine();
    puts("enter FreeBooks count:");
    book.FreeBooks = atoi(ReadLine());
    puts("enter Initials:");
    book.Initials = ReadLine();
    puts("enter ISBN:");
    book.ISBN = ReadLine();
    return book;
}

int BackupBook(BooksDataBase ** HashTable,int capacity) //Создание бэкапа
{
    char *s = MakeBackupFileName(BOOKS);
    FILE *f = fopen(s,"w");
    if(f == NULL)
    {
        puts("File is not created!");
        return 0;
    }
    for(int i =0;i<capacity;i++)
    {
        for(BooksDataBase *head = HashTable[i];head!=NULL;head = head->next)
        {
            fprintf(f,"%s;%s;%s;%d;%d\n",head->book.ISBN,head->book.Initials,head->book.BookName,head->book.BookCount,head->book.FreeBooks);
        }
    }
    fclose(f);
    return 1;
}

int FindByISBN(BooksDataBase** HashTable,int capacity,char *value) //Поиск по ISBN в базе
{
    int trigger = 0;
    if(HashTable == NULL)
    {
        puts("Base not initialized");
        return 0;
    }
    for(int i =0;i<capacity;i++)
    {
        for(BooksDataBase*head = HashTable[i];head!=NULL;head = head->next)
        {
            if(CompareStr(head->book.ISBN,value))
            {
                PrintBook(head->book);
                trigger=1;
            }
        }
    }
    return trigger;
}