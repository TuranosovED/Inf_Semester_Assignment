#include "student.h"

void FillStructStudent(Students *student,  char *Field, char *s) //Заполнение определенного поля структуры 
{
    int structId;
    if(CompareStr(Field,"ID"))
        structId = 0;
    else if(CompareStr(Field,"SecondName"))
        structId = 1;
    else if(CompareStr(Field,"FirstName"))
        structId = 2;
    else if(CompareStr(Field,"ThirdName"))
        structId = 3;
    else if(CompareStr(Field,"Faculty"))
        structId = 4;
    else if(CompareStr(Field,"Speciality"))
        structId = 5;

    switch (structId)
    {
    case 0:
        student->id = s;
        break;
    case 1:
        student->secondName = s;
        break;
    case 2:
        student->firstName = s;
        break;
    case 3:
        student->thirdName = s;
        break;
    case 4:
        student->faculty = s;
        break;
    case 5:
        student->speciality = s;
        break;
    default:
        break;
    }
}

void PrintStudent(Students Field) //Вывод структуры в консоль
{
    printf("id - %s; ", Field.id);
    printf("secondName - %s; ", Field.secondName);
    printf("firstName - %s; ", Field.firstName);
    printf("thirdName - %s; ", Field.thirdName);
    printf("faculty - %s; ", Field.faculty);
    printf("speciality - %s;\n", Field.speciality);
}

StudentsDataBase **InitDataBaseStudent(int capacity) //Инициализация таблицы
{
    StudentsDataBase **HashTable = (StudentsDataBase **)malloc(capacity * sizeof(StudentsDataBase *));
    for (int i = 0; i < capacity; i++)
        HashTable[i] = NULL;
    return HashTable;
}

int  PushHashTableStudent(StudentsDataBase **HashTable, int capacity, Students value) //Добавление элемента в таблицу
{
    int key = GetHash(value.id, capacity);
    StudentsDataBase *newValue = (StudentsDataBase *)malloc(sizeof(StudentsDataBase));
    newValue->student.faculty = value.faculty;
    newValue->student.firstName = value.firstName;
    newValue->student.id = value.id;
    newValue->student.secondName = value.secondName;
    newValue->student.speciality = value.speciality;
    newValue->student.thirdName = value.thirdName;
    for (StudentsDataBase *head = HashTable[key]; head != NULL; head = head->next)
        if (CompareStr(head->student.id, value.id))
        {
            head->student = newValue->student;
            puts("The field has been updated");
            system("pause");
            return 0;
        }
    newValue->next = HashTable[key];
    HashTable[key] = newValue;
    return 1;
}

void PrintHashTableStudent(StudentsDataBase **HashTable, int capacity) //Вывод таблицы в консоль
{ 
    for (int i = 0; i < capacity; i++)
    {
        //printf("%d:\n", i);
        for (StudentsDataBase *head = HashTable[i]; head != NULL; head = head->next)
        {
            PrintStudent(head->student);
        }
        //putchar('\n');
    }
}

int ReadCsvStudent(StudentsDataBase **HashTable, int capacity,char*FileName) //Считывание csv
{
    int size = 0;
    int structCounter = 0;
    char sign;
    char* str = (char*)calloc(1, sizeof(char));
    int trigger = 0;
    char prev;
    FILE* f;
    if (FileName == NULL)
        f = fopen("students.csv", "r");
    else
        f = fopen(FileName, "r");

    if (f == NULL)
    {
        puts("File is not founded!");
        return 0;
    }
    char **Fields = MakeTableFieldsMass(f,&size);
    Students bufferStudent;
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
                FillStructStudent(&bufferStudent, Fields[structCounter++], str);
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
            FillStructStudent(&bufferStudent, Fields[structCounter++], str);
            PushHashTableStudent(HashTable, capacity, bufferStudent);
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
                FillStructStudent(&bufferStudent, Fields[structCounter++], str);
                PushHashTableStudent(HashTable, capacity, bufferStudent);
            }
            break;
        }
    }
    FreeFieldMas(&Fields,size);
    fclose(f);
    return 1;
}

Students MakeStudent() //Создание объекта
{
    Clearbuffer();
    Students student;
    puts("enter faculty:");
    student.faculty = ReadLine();
    puts("enter firstName:");
    student.firstName = ReadLine();
    puts("enter id:");
    student.id = ReadLine();
    puts("enter secondName:");
    student.secondName = ReadLine();
    puts("enter speciality:");
    student.speciality = ReadLine();
    puts("enter thirdName:");
    student.thirdName = ReadLine();
    return student;
}

int BackupStudent(StudentsDataBase ** HashTable,int capacity) //Создание бэкапа таблицы
{
    char *s = MakeBackupFileName(STUDENT);
    FILE *f = fopen(s,"w");
    if(f == NULL)
    {
        puts("File is not created!");
        return 0;
    }
    for(int i =0;i<capacity;i++)
    {
        for(StudentsDataBase *head = HashTable[i];head!=NULL;head = head->next)
        {
            fprintf(f,"%s;%s;%s;%s;%s;%s\n",head->student.id,head->student.secondName,head->student.firstName,head->student.thirdName,head->student.faculty,head->student.speciality);
        }
    }
    fclose(f);
    return 1;
}

int FindBySecondName(StudentsDataBase** HashTable,int capacity,char *value) //Поиск в таблице по фамилии
{
    int trigger = 0;
    if(HashTable == NULL)
    {
        puts("Base not initialized");
        return 0;
    }
    for(int i =0;i<capacity;i++)
    {
        for(StudentsDataBase*head = HashTable[i];head!=NULL;head = head->next)
        {
            if(CompareStr(head->student.secondName,value))
            {
                PrintStudent(head->student);
                trigger=1;
            }
        }
    }
    return trigger;
}

int FindById(StudentsDataBase** HashTable,int capacity,char *value) //Поиск в таблице по ID
{
    int trigger = 0;
    if(HashTable == NULL)
    {
        puts("Base not initialized");
        return 0;
    }
    for(int i =0;i<capacity;i++)
    {
        for(StudentsDataBase*head = HashTable[i];head!=NULL;head = head->next)
        {
            if(CompareStr(head->student.id,value))
            {
                PrintStudent(head->student);
                trigger=1;
            }
        }
    }
    return trigger;
}

void RefreshHashtableStudent(StudentsDataBase *** HashTable,int capacity) //Переинициализация таблицы
{
    StudentsDataBase *prev;
    for(int i =0;i<capacity;i++)
    {
        for(StudentsDataBase *head = (*HashTable)[i];head !=NULL;head = prev)
        {
            prev = head->next;
            free(head);
        }
    }
    free(*HashTable);
    *HashTable = InitDataBaseStudent(capacity);
}