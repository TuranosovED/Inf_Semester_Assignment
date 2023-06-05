#include "lines.h"
#include "student.h"
#include "books.h"
#include "users.h"
#include "borrowers.h"
#include "filesworking.h"
#include "dateworking.h"
#include "hashfunction.h"
#include "defines.h"

//Строки для лога
char booksMenuStr[][50] = {{"Read books csv file"}, {"Add book in books base"} ,{"Find book by ISBN"}, {"Delete book from books base"}, {"Print books base"} ,{"Make books base backup"} ,{"restore books base backup"} ,{"View books borrowed by students"} ,{"Lend a book"}, {"Print all borrowers"}, {"Come back"}};
char studentMenuStr[][50] = {{"Read students csv file"}, {"Add student in student base"}, {"Find student by second name"}, {"Delete student from student base"}, {"Print student base"}, {"Make student base backup"}, {"Restore student base backup"}, {"View student books"}, {"Сome back"}};

int DeleteStudent(StudentsDataBase **HashTable,BorrowersDataBase **BorrowersHashTable, int capacity)//Функция удаления студента из базы по ID
{
    StudentsDataBase *prev = NULL;
    char value[50];
    puts("Enter delete id");
    scanf("%s", value);
    int key = GetHash(value, capacity);
    for (StudentsDataBase *head = HashTable[key]; head != NULL;)
    {
        if (CompareStr(value, head->student.id))
        {
            if(!IsStudentBorrow(BorrowersHashTable,capacity,head->student.id))
            {
                if (prev == NULL)
                HashTable[key] = head->next;
                else
                    prev->next = head->next;
                free(head);
                puts("Complete!");
                return 1;
            }
            else
            {
                puts("Decline! This student has a book");
                return 0;
            }
        }
        prev = head;
        head = head->next;
    }
    puts("Element not founded!");
    return 0;
}

int DeleteBook(BooksDataBase **HashTable,BorrowersDataBase **BorrowersHashTable, int capacity)// Функция удаления книги по ISBN
{
    BooksDataBase *prev = NULL;
    char value[50];
    puts("Enter delete ISBN");
    scanf("%s", value);
    int key = GetHash(value, capacity);
    for (BooksDataBase *head = HashTable[key]; head != NULL;)
    {
        if(!IsBookBorrow(BorrowersHashTable,capacity,head->book.ISBN))
        {
            if (CompareStr(value, head->book.ISBN))
            {
                if (prev == NULL)
                    HashTable[key] = head->next;
                else
                    prev->next = head->next;
                free(head);
                puts("Complete!");
                return 1;
            }
            prev = head;
            head = head->next;
        }
        else
        {
            puts("Decline! This book is busy");
            return 0;
        }
        
    }
    puts("Element not founded!");
    return 0;
}

int ViewStudentBooks(BorrowersDataBase **BorrowersHashTable,BooksDataBase** BooksHashTable,char *value,int capacity) //Вывод в консоль книг оформленых на студента
{
    int trigger = 0;
    for(int i =0;i<capacity;i++)
    {
        for(BorrowersDataBase *head = BorrowersHashTable[i];head != NULL;head = head->next)
        {
            if(CompareStr(value,head->borrower.id))
            {
                trigger = 1;
                FindByISBN(BooksHashTable,capacity,head->borrower.ISBN);
                printf("Date of issue: %s\n",head->borrower.date);
            }
        }
    }
    return trigger;
}

int ViewBookStudents(BorrowersDataBase **BorrowersHashTable,StudentsDataBase** StudentsHashTable,char *value,int capacity) //Вывод в консоль всех студентов оформленых на определенную книгу
{
    int trigger = 0;
    for(int i =0;i<capacity;i++)
    {
        for(BorrowersDataBase *head = BorrowersHashTable[i];head != NULL;head = head->next)
        {
            if(CompareStr(value,head->borrower.ISBN))
            {
                trigger = 1;
                FindById(StudentsHashTable,capacity,head->borrower.id);
                printf("Date of issue: %s\n",head->borrower.date);
            }
        }
    }
    return trigger;
}

int TakeNearestBook(BorrowersDataBase **HashTableBorrowers,int capacity,char *value) //Выводит ближайщую дату возврата книги
{
    int count = 0;
    for(int i =0;i<capacity;i++)
        for(BorrowersDataBase *head = HashTableBorrowers[i];head != NULL; head = head->next)
            count++;
    Borrowers *bufferArray = (Borrowers *)malloc(sizeof(Borrowers) * count);

    count = 0;

    for(int i =0;i<capacity;i++)
            for(BorrowersDataBase *head = HashTableBorrowers[i];head != NULL; head = head->next)
                bufferArray[count++] = head->borrower;
    
    for(int i =0;i<count;i++)
    {
        for(int j =i+1;j<count;j++)
        {
            if(CompareDates(ParceDate(bufferArray[i].date),ParceDate(bufferArray[j].date)))
            {
                swap(Borrowers,bufferArray[i],bufferArray[j]);
            }
        }
    }

    for(int i =0;i<count;i++)
    {
        if(CompareStr(value,bufferArray[i].ISBN))
        {
            printf("There are currently no available books! The next one will be delivered: %s\n",bufferArray[i].date);
            return 1;
        }
    }
    return 0;
}

int LendBook(BooksDataBase **HashTableBooks,BorrowersDataBase **HashTableBorrowers,int capacity,char* value) //Функция оформления книги на студента
{
    for(int i =0;i<capacity;i++)
    {
        for(BooksDataBase *head = HashTableBooks[i];head != NULL;head = head->next)
        {
            if(CompareStr(value,head->book.ISBN))
            {
                if(head->book.FreeBooks > 0)
                {
                    head->book.FreeBooks--;
                    return 1;
                }
                else
                {
                    if(!TakeNearestBook(HashTableBorrowers,capacity,value))
                        puts("There is no such book in the library\n");
                    return 0;
                }
            }
        }
    }
    return 0;
}

void Logging(Users CurrentUser,int table,int command) // Вход в систему
{
    FILE *f = fopen("logs\\log.txt","a+");
    if(table == BOOKS)
        fprintf(f,"%s;%s;%s\n",MakeTimeStr(),CurrentUser.UserName,booksMenuStr[command-1]);
    else if(table == STUDENT)
        fprintf(f,"%s;%s;%s\n",MakeTimeStr(),CurrentUser.UserName,studentMenuStr[command-1]);
    fclose(f);
}

int StudentsBaseMenu(StudentsDataBase ***HashTableStudents,BorrowersDataBase ***HashTableBorrowers,BooksDataBase ***HashTableBooks,FileDataArray** Data,Users CurrentUser,int capacity) // Меню базы студентов
{
    RefreshFileData(Data,STUDENT);
    char *value = (char*)calloc(50,sizeof(char));
    int command;
    getchar();
    while (1)
    {
        system("cls");
        puts("1 - Read students csv file\n2 - Add student in student base\n3 - Find student by second name\n4 - Delete student from student base\n5 - Print student base\n6 - Make student base backup\n7 - Restore student base backup\n8 - View student books\n9 - Сome back");
        scanf("%d",&command);
        
        switch (command)
        {
        case 1: // считать файл базы данных
            RefreshHashtableStudent(HashTableStudents,capacity);
            ReadCsvStudent(*HashTableStudents, capacity,NULL);
            break;
        case 2: // добавить студента в базу
            PushHashTableStudent(*HashTableStudents,capacity,MakeStudent());
            break;
        case 3:
            puts("Enter second name:");
            scanf("%s",value);
            if(FindBySecondName(*HashTableStudents,capacity,value))
                puts("\nComplete!");
            else
                puts("There is no such entry in the database");
            free(value);
            RefreshStr(&value);
            break;
        case 4: // удалить студента из базы по ключу(id)
            PrintHashTableStudent(*HashTableStudents, capacity);
            DeleteStudent(*HashTableStudents,*HashTableBorrowers,capacity);
            break;
        case 5: // вывести всю базу данных
            PrintHashTableStudent(*HashTableStudents, capacity);
            break;
        case 6: //Сделать бэкап базы
            if(BackupStudent(*HashTableStudents,capacity))
                puts("Complete!");
            else
                puts("Error!");
            RefreshFileData(Data,STUDENT);
            break;
        case 7: //Загрузить базу из бэкапа
            RefreshHashtableStudent(HashTableStudents,capacity);
            if(ReadCsvStudent(*HashTableStudents,capacity,ChooseFileFromCatalog(*Data,STUDENT)))
                puts("Complete!");
            else
                puts("File is not founded!");
            break;
        case 8:
            puts("Enter ID:");
            scanf("%s",value);
            if(ViewStudentBooks(*HashTableBorrowers,*HashTableBooks,value,capacity))
                puts("Complete!");
            else
                puts("Student did not take books!");
            free(value);
            RefreshStr(&value);
            break;
        case 9:
            Logging(CurrentUser,STUDENT,command);
            return 0;
            break;
        default:
            puts("Unknown command!");
            break;
        }
        Logging(CurrentUser,STUDENT,command);
        system("pause");
    }
}

int BooksBaseMenu(BooksDataBase ***HashTableBooks,BorrowersDataBase ***HashTableBorrowers,StudentsDataBase ***HashTableStudents,FileDataArray** Data,Users CurrentUser,int capacity) // Меню базы книг
{
    RefreshFileData(Data,BOOKS);
    char *value = (char*)calloc(50,sizeof(char));
    Borrowers bufferBorrower;
    int command;
    getchar();
    while(1)
    {
        system("cls");
        puts("1 - Read books csv file\n2 - Add book in Books base\n3 - Find book by ISBN\n4 - Delete book from books base\n5 - Print books base\n6 - Make books base backup\n7 - Restore books base backup\n8 - View books borrowed by students\n9 - Lend a book\n10 - Print all borrowers\n11 - Come back");
        scanf("%d",&command);
        switch (command)
        {
        case 1:
            RefreshHashtableBook(HashTableBooks,capacity);
            ReadCsvBook(*HashTableBooks,capacity,NULL);
            break;
        case 2:
            PushHashTableBooks(*HashTableBooks,capacity,MakeBook());
            break;
        case 3:
            puts("Enter ISBN:");
            scanf("%s",value);
            if(FindByISBN(*HashTableBooks,capacity,value))
                puts("\nComplete!");
            else
                puts("There is no such entry in the database");
            free(value);
            RefreshStr(&value);
            break;
        case 4:
            SortHashTableBookByISBN(*HashTableBooks,capacity);
            DeleteBook(*HashTableBooks,*HashTableBorrowers,capacity);
            break;
        case 5:
            SortHashTableBookByISBN(*HashTableBooks,capacity);
            break;
        case 6:
            if(BackupBook(*HashTableBooks,capacity))
                puts("Complete!");
            else
                puts("Error!");
            RefreshFileData(Data,BOOKS);
            break;
        case 7:
            RefreshHashtableBook(HashTableBooks,capacity);
            if(ReadCsvBook(*HashTableBooks,capacity,ChooseFileFromCatalog(*Data,BOOKS)))
                puts("Complete!");
            else
                puts("File is not founded!");
            break;
        case 8:
            puts("Enter ISBN:");
            scanf("%s",value);
            if(ViewBookStudents(*HashTableBorrowers,*HashTableStudents,value,capacity))
                puts("Complete!");
            else
                puts("Nobody took the book!");
            free(value);
            RefreshStr(&value);
            break;
        case 9:
            bufferBorrower = MakeBorrower();
            if(LendBook(*HashTableBooks,*HashTableBorrowers,capacity,bufferBorrower.ISBN))
            {
                PushHashTableBorrower(*HashTableBorrowers,capacity,bufferBorrower);
                puts("Complete!");
            }
            else
                puts("Error!");
            break;
        case 10:
            PrintHashTableBorrowers(*HashTableBorrowers,capacity);
            break;
        case 11:
            Logging(CurrentUser,BOOKS,command);
            return 0;
            break;
        default:
            puts("Unknown command!");
            break;
        }
        Logging(CurrentUser,BOOKS,command);
        system("pause");
    }
}

int main()
{
    system("chcp 65001");
    MakeDirs(); //создание директорий используемых функциями
    UsersDataBase **HashTableUsers = InitDataBaseUsers(USERS_DATABASE_CAPACITY);
    ReadCsvUsers(HashTableUsers,USERS_DATABASE_CAPACITY,NULL);
    Users CurrentUser;
    if(!AccessUser(HashTableUsers,USERS_DATABASE_CAPACITY,&CurrentUser))
        return 0;

    int capacity = CAPACITY;
    // puts("enter capacity of hash table: ");
    // scanf("%d", &capacity);
    // while (capacity <= 0)
    // {
    //     system("cls");
    //     puts("try again please");
    //     scanf("%d", &capacity);
    // }
    
    FileDataArray* Data = NULL;
    StudentsDataBase **HashTableStudent = InitDataBaseStudent(capacity);
    BooksDataBase **HashTableBooks = InitDataBaseBooks(capacity);
    BorrowersDataBase **HashTableBorrowers = InitDataBaseBorrowers(capacity);

    ReadCsvStudent(HashTableStudent,capacity,NULL);
    ReadCsvBook(HashTableBooks,capacity,NULL);
    ReadCsvBorrowers(HashTableBorrowers,capacity,NULL);

    //PrintHashTableUsers(HashTableUsers,USERS_DATABASE_CAPACITY);
    //PrintHashTableBorrowers(HashTableBorrowers,capacity);

    int command;
    getchar();
    while(1) // Тело программы
    {
        system("cls");
        if(CurrentUser.BooksBaseAccess && CurrentUser.StudentBaseAccess)
        {
            puts("1 - Books\n2 - Students\n3 - Exit\n");
            scanf("%d",&command);
            switch (command)
            {
                case 1:
                    BooksBaseMenu(&HashTableBooks,&HashTableBorrowers,&HashTableStudent,&Data,CurrentUser,capacity);
                    break;
                case 2:
                    StudentsBaseMenu(&HashTableStudent,&HashTableBorrowers,&HashTableBooks,&Data,CurrentUser,capacity);
                    break;
                case 3:
                    BackupStudent(HashTableStudent,capacity);
                    BackupBook(HashTableBooks,capacity);
                    return 0;
                    break;
                default:
                    puts("Unknown command!");
                    break;
            }
        }
        else if(CurrentUser.BooksBaseAccess)
        {
            puts("1 - Books\n2 - Exit\n");
            scanf("%d",&command);
            switch (command)
            {
                case 1:
                    BooksBaseMenu(&HashTableBooks,&HashTableBorrowers,&HashTableStudent,&Data,CurrentUser,capacity);
                    break;
                case 2:
                    BackupBook(HashTableBooks,capacity);
                    return 0;
                    break;
                default:
                    puts("Unknown command!");
                    break;
            }
        }
        else if(CurrentUser.StudentBaseAccess)
        {
            puts("1 - Students\n2 - Exit\n");
            scanf("%d",&command);
            switch (command)
            {
                case 1:
                    StudentsBaseMenu(&HashTableStudent,&HashTableBorrowers,&HashTableBooks,&Data,CurrentUser,capacity);
                    break;
                case 2:
                    BackupStudent(HashTableStudent,capacity);
                    return 0;
                    break;
                default:
                    puts("Unknown command!");
                    break;
            }
        }
        else
        {
            puts("Access Denied!");
            return 0;
        }
        system("pause");
    }
    return 0;
}