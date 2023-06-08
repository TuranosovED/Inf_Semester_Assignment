#include "users.h"

void FillStructUsers(Users *user,  char *Field, char *s) //Заполнение определенного поля структуры
{
    int structId;
    if(CompareStr(Field,"UserName"))
        structId = 0;
    else if(CompareStr(Field,"Password"))
        structId = 1;
    else if(CompareStr(Field,"StudentBaseAccess"))
        structId = 2;
    else if(CompareStr(Field,"BooksBaseAccess"))
        structId = 3;

    switch (structId)
    {
    case 0:
        user->UserName = s;
        break;
    case 1:
        user->Password = s;
        break;
    case 2:
        user->StudentBaseAccess = atoi(s);
        break;
    case 3:
        user->BooksBaseAccess = atoi(s);
        break;
    default:
        break;
    }
}

void PrintUser(Users Field) //Вывод структуры на экран
{
    printf("UserName - %s; ", Field.UserName);
    printf("Password - %s; ", Field.Password);
    printf("StudentBaseAccess - %d; ", Field.StudentBaseAccess);
    printf("BooksBaseAccess - %d;\n", Field.BooksBaseAccess);
}

UsersDataBase **InitDataBaseUsers(int capacity) //Инициализация таблицы
{
    UsersDataBase **HashTable = (UsersDataBase **)malloc(capacity * sizeof(UsersDataBase *));
    for (int i = 0; i < capacity; i++)
        HashTable[i] = NULL;
    return HashTable;
}


int PushHashTableUsers(UsersDataBase **HashTable, int capacity, Users value) //Добавление элемента в таблицу
{
    int key = GetHash(value.UserName, capacity);
    UsersDataBase *newValue = (UsersDataBase *)malloc(sizeof(UsersDataBase));
    newValue->user.UserName = value.UserName;
    newValue->user.Password = value.Password;
    newValue->user.BooksBaseAccess = value.BooksBaseAccess;
    newValue->user.StudentBaseAccess = value.StudentBaseAccess;
    for (UsersDataBase *head = HashTable[key]; head != NULL; head = head->next)
        if (CompareStr(head->user.UserName, value.UserName))
        {
            puts("This UserName is busy");
            return 0;
        }
    newValue->next = HashTable[key];
    HashTable[key] = newValue;
    return 1;
}

void PrintHashTableUsers(UsersDataBase **HashTable, int capacity) //Вывод таблицы в консоль
{ 
    for (int i = 0; i < capacity; i++)
    {
        //printf("%d:\n", i);
        for (UsersDataBase *head = HashTable[i]; head != NULL; head = head->next)
        {
            PrintUser(head->user);
        }
        //putchar('\n');
    }
}

int ReadCsvUsers(UsersDataBase **HashTable, int capacity,char*FileName) //Считывание csv
{
    int size = 0;
    int structCounter = 0;
    char sign;
    char* str = (char*)calloc(1, sizeof(char));
    int trigger = 0;
    char prev;
    FILE* f;
    if (FileName == NULL)
        f = fopen("users.csv", "r");
    else
        f = fopen(FileName, "r");

    if (f == NULL)
    {
        puts("File is not founded!");
        return 0;
    }
    char **Fields = MakeTableFieldsMass(f,&size);

    Users bufferUser;
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
                FillStructUsers(&bufferUser, Fields[structCounter++], str);
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
            PushHashTableUsers(HashTable, capacity, bufferUser);
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
                PushHashTableUsers(HashTable, capacity, bufferUser);
            break;
        }
    }
    FreeFieldMas(&Fields,size);
    fclose(f);
    return 1;
}

int AccessUser(UsersDataBase** HashTable,int capacity,Users *CurrentUser) //Проверка доступа юзера
{
    char *username = (char*)calloc(50,sizeof(char));
    char *password = (char*)calloc(50,sizeof(char));
    puts("Enter UserName:");
    scanf("%s",username);
    puts("Enter Password:");
    scanf("%s",password);

    int trigger = 0;
    if(HashTable == NULL)
    {
        puts("Base not initialized");
        return 0;
    }
    for(int i =0;i<capacity;i++)
    {
        for(UsersDataBase*head = HashTable[i];head!=NULL;head = head->next)
        {
            if(CompareStr(head->user.UserName,username) && CompareStr(head->user.Password,password))
            {
                trigger=1;
                *CurrentUser = head->user;
            }
                
        }
    }

    free(username);
    free(password);
    if(!trigger)
        puts("Access denied! Wrong Password or Username");
    else
        puts("\nAccess!");
    system("pause");
    return trigger;
}