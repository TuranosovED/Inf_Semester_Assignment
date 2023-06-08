#include "borrowers.h"

void FillStructBorrower(Borrowers *borrower,  char *Field, char *s) //Заполнение структуры по номеру поля
{
    int structId;
    if(CompareStr(Field,"ISBN"))
        structId = 0;
    else if(CompareStr(Field,"ID"))
        structId = 1;
    else if(CompareStr(Field,"DATE"))
        structId = 2;

    switch (structId)
    {
    case 0:
        borrower->ISBN = s;
        break;
    case 1:
        borrower->id = s;
        break;
    case 2:
        borrower->date = s;
        break;
    default:
        break;
    }
}

BorrowersDataBase **InitDataBaseBorrowers(int capacity) //Инициализация таблицы
{
    BorrowersDataBase **HashTable = (BorrowersDataBase **)malloc(capacity * sizeof(BorrowersDataBase *));
    for (int i = 0; i < capacity; i++)
        HashTable[i] = NULL;
    return HashTable;
}

int PushHashTableBorrower(BorrowersDataBase **HashTable, int capacity, Borrowers value) //Добавление в таблицу
{
    int key = GetHash(value.id, capacity);
    BorrowersDataBase *newValue = (BorrowersDataBase *)malloc(sizeof(BorrowersDataBase));
    newValue->borrower.id = value.id;
    newValue->borrower.date = value.date;
    newValue->borrower.ISBN = value.ISBN;
    newValue->next = HashTable[key];
    HashTable[key] = newValue;
    return 1;
}

void PrintHashTableBorrowers(BorrowersDataBase **HashTable, int capacity) //Вывод таблицы
{ 
    for (int i = 0; i < capacity; i++)
    {
        //printf("%d:\n", i);
        for (BorrowersDataBase *head = HashTable[i]; head != NULL; head = head->next)
        {
            PrintBorrower(head->borrower);
        }
        //putchar('\n');
    }
}

int IsBookBorrow(BorrowersDataBase **HashTable,int capacity,char *value) //Проверка книги в списке должников
{
    for(int i = 0;i<capacity;i++)
    {
        for(BorrowersDataBase* head = HashTable[i]; head != NULL;head = head->next)
        {
            if(CompareStr(value,head->borrower.ISBN))
                return 1;
        }
    }
    return 0;
}

int ReadCsvBorrowers(BorrowersDataBase **HashTable, int capacity,char*FileName) //Считывание csv
{
    int size = 0;
    int structCounter = 0;
    char sign;
    char* str = (char*)calloc(1, sizeof(char));
    int trigger = 0;
    char prev;
    FILE* f;
    if (FileName == NULL)
        f = fopen("student_books.csv", "r");
    else
        f = fopen(FileName, "r");

    if (f == NULL)
    {
        puts("File is not founded!");
        return 0;
    }
    char **Fields = MakeTableFieldsMass(f,&size);
    
    Borrowers bufferBorrower;
    while (1)
    {
        sign = fgetc(f);
        switch (sign)
        {
        case ',':
            if (prev == '"')
            {
                trigger = 0;
                PopLine(&str);
            }
            if (!trigger)
            {
                FillStructBorrower(&bufferBorrower, Fields[structCounter++], str);
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
            PushHashTableBorrower(HashTable, capacity, bufferBorrower);
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
            if (str[0] != ',')
                PushHashTableBorrower(HashTable, capacity, bufferBorrower);
            break;
        }
    }
    FreeFieldMas(&Fields,size);
    fclose(f);
    return 1;
}

Borrowers MakeBorrower() //Создание объекта
{
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);

    int day = local->tm_mday;
    int mounth = local->tm_mon;
    int year = local->tm_year;
    char *date = (char*)calloc(20,sizeof(char));
    char dateBuffer[16];

    int command;
    Clearbuffer();
    Borrowers borrower;
    puts("enter ISBN:");
    borrower.ISBN = ReadLine();
    puts("enter ID:");
    borrower.id = ReadLine();
    puts("1 - Use current date\n2 - Enter Date");
    while (1)
    {
        scanf("%d",&command);
        switch (command)
        {
        case 1:
            if(day/10 == 0)
                strcat(date,"0");
            itoa(day,dateBuffer,10);
            strcat(date,dateBuffer);
            strcat(date,".");

            if((mounth+1)/10 == 0)
                strcat(date,"0");
            itoa(mounth+1,dateBuffer,10);
            strcat(date,dateBuffer);
            strcat(date,".");

            itoa(year+1900,dateBuffer,10);
            strcat(date,dateBuffer);
            borrower.date = date;
            return borrower; 
            break;
        case 2:
            puts("Enter date:");
            Clearbuffer();
            borrower.date = ReadLine();
            return borrower;
            break;
        default:
            puts("try again please");
        }
    }
}

int IsStudentBorrow(BorrowersDataBase **HashTable,int capacity,char *value) //Проверка студента в списке должников
{
    for(int i =0;i<capacity;i++)
    {
        for(BorrowersDataBase* head = HashTable[i]; head != NULL;head = head->next)
        {
            if(CompareStr(value,head->borrower.id))
                return 1;
                
        }
    }
    return 0;
}

void PrintBorrower(Borrowers Field) //Вывод структуры
{
    printf("ISBN - %s; ", Field.ISBN);
    printf("id - %s; ", Field.id);
    printf("date - %s;\n", Field.date);
}