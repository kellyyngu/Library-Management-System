// include header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include <conio.h>

#define STUDENT_SIZE 200
#define STUDENTID_SIZE 10
#define NAME_SIZE 50
#define PASSWORD_SIZE 50

#define BOOK_SIZE 200
#define BOOKTITLE_SIZE 50
#define ISBN_SIZE 10
#define YEAR_OF_PUBLICATION_SIZE 10

#define DATE_SIZE 15
#define LINE_SIZE 1000

// structure to represent book information
typedef struct Book
{
    char title[BOOKTITLE_SIZE];
    char author[NAME_SIZE];
    char ISBN[ISBN_SIZE];
    char yearOfPublication[YEAR_OF_PUBLICATION_SIZE];
    char copiesAvailable[BOOK_SIZE];
    char totalCopies[BOOK_SIZE];
    char borrowedCopies[BOOK_SIZE];

    char borrowDate[DATE_SIZE];
    char dueDate[DATE_SIZE];
    char returnDate[DATE_SIZE];

    char student[50];
} Book;

// structure to represent student information
typedef struct Student
{
    char studentId[STUDENTID_SIZE];
    char name[NAME_SIZE];
    char category[20];
    char password[PASSWORD_SIZE];
    double totalFine;
} Student;

// function prototypes
int readBooks(Book books[]);
int readStudents(Student students[]);
void librarianMenu(Book books[], int totalBooks, Student students[], int totalStudents);
void studentMenu(Book books[], int totalBooks, Student students[], int totalStudents);
bool validateStudentLogin(Student students[], int totalStudents, char *studentId, char *password);
void searchBook(Book books[], int totalBooks, char *searchISBN);
void bubbleSort(Book books[], int totalBooks);
void displayBookList(Book books[], int totalBooks);
double calculatePenalty(const char *dueDate, const char *returnDate);
void penaltyManagementforLibrarian(Book books[], int totalBooks, Student students[], int totalStudents);
void penaltyManagementforStudent(Book books[], int totalBooks, Student students[], int totalStudents);
int validateStudentID(Student students[], int totalStudents, const char studentId[]);
int validateISBN(Book books[], int totalBooks, const char ISBN[]);
int isValidDateFormat(const char *date);
int compareDates(const char *date1, const char *date2);
void borrowBook(Book books[], int totalBooks, Student students[], int totalStudents);
void returnBook(Book books[], int totalBooks, Student students[], int totalStudents);
void borrowORreturn(Book books[], int totalBooks, Student students[], int totalStudents);
struct tm convertToDate(const char *dateString);
void generateMonthlyReport(Book books[], int totalBooks, Student students[], int totalStudents);

int main()
{
    // read book information from the "books.txt" file
    Book books[BOOK_SIZE]; // books is the variable name
    int totalBooks = readBooks(books);

    // read student information from the "students.txt" file
    Student students[STUDENT_SIZE];
    int totalStudents = readStudents(students);

    // Call the appropriate menu based on the user type
    printf("\n\n\t\t\t\t\t\t|===========================================================|\n");
    printf("\t\t\t\t\t\t|                                                           |\n");
    printf("\t\t\t\t\t\t|                                                           |\n");
    printf("\t\t\t\t\t\t|                     WELCOME TO KELLY'S                    |\n");
    printf("\t\t\t\t\t\t|                  LIBRARY MANAGEMENT SYSTEM                |\n");
    printf("\t\t\t\t\t\t|                                                           |\n");
    printf("\t\t\t\t\t\t|                                                           |\n");
    printf("\t\t\t\t\t\t|===========================================================|\n");
    printf("\t\t\t\t\t\t|                                                           |\n");
    printf("\t\t\t\t\t\t|   Login as:                                               |\n");
    printf("\t\t\t\t\t\t|   1. Librarian                                            |\n");
    printf("\t\t\t\t\t\t|   2. Student                                              |\n");
    printf("\t\t\t\t\t\t|                                                           |\n");
    printf("\t\t\t\t\t\t|===========================================================|\n\n");

    // get and read the choice from user
    int user;
    printf("\t\t\t\t\t\tEnter Choice: ");
    scanf("%d", &user);

    // function call (user type) according to choice of user
    switch (user)
    {
    case 1:
        librarianMenu(books, totalBooks, students, totalStudents);
        break;
    case 2:
        studentMenu(books, totalBooks, students, totalStudents);
        break;
    default:
        printf("\t\t\t\t\t\tInvalid choice. Please run the program again and enter either 1 or 2.\n\n");
        break;
    }

    return 0;
}

// read contents from books.txt file and return total number of books
int readBooks(Book books[])
{
    // open books.txt file in read mode
    FILE *file = fopen("books.txt", "r");
    if (file == NULL)
    {
        puts("books.txt file could not be opened."); // display error message if books.txt file cannot be opened
    }

    int totalBooks = 0; // initialise number of books to 0
    // loop to continue reading lines from the file until fscanf fails to read all 7 items, indicating that there are no more lines to read or an error occurred
    while (fscanf(file, "%30s %30s %4s %4s %3s %3s %3s", // read a string of up to 30 characters
                  books[totalBooks].title, books[totalBooks].author, books[totalBooks].ISBN, books[totalBooks].yearOfPublication,
                  books[totalBooks].copiesAvailable, books[totalBooks].totalCopies, books[totalBooks].borrowedCopies) == 7)
    {
        totalBooks++; // increase number of books only if the line has 7 items in total
    }
    fclose(file); // close the file

    return totalBooks; // return total number of books
}

// read contents from students.txt file and return total number of books
int readStudents(Student students[])
{
    // open students.txt file in read mode
    FILE *file = fopen("students.txt", "r");
    if (file == NULL)
    {
        puts("students.txt file could not be opened."); // display error message if students.txt file cannot be opened
    }

    int totalStudents = 0; // initialise total students to 0
    // loop to continue reading lines from the file until fscanf fails to read all 4 items, indicating that there are no more lines to read or an error occurred
    while (fscanf(file, "%6s,%49[^,],%19[^,],%19s", students[totalStudents].studentId, students[totalStudents].name, students[totalStudents].category, students[totalStudents].password) == 4)
    {
        totalStudents++; // increase number of students only if the line has 7 items in total
    }
    fclose(file); // close the file

    return totalStudents; // return total number of students
}

// function to handle librarian menu
void librarianMenu(Book books[], int totalBooks, Student students[], int totalStudents)
{
    // declaration
    int loginAttempts = 0;
    char librarianPassword[PASSWORD_SIZE];

    do
    {
        // get and read password from the user without showing it on the screen
        printf("\t\t\t\t\t\tEnter Librarian Password: ");
        int i = 0;
        char ch;
        
        while (1)
        {
            ch = _getch();  // use _getch() for hiding password input on the screen
            if (ch == '\r') // enter key
            {
                break;
            }
            else if (ch == '\b' && i > 0) // backspace key
            {
                i--;
                printf("\b \b"); // move the cursor back and erase the character
            }
            else if (ch != '\b') // ignore backspace
            {
                librarianPassword[i++] = ch;
                printf("*"); // print asterisk (*) instead of the actual character
            }
        }
        librarianPassword[i] = '\0'; // null-terminate the password

        // strcmp to check if the password is correct, execute 'if' block if the password is correct
        if (strcmp(librarianPassword, "librarian") == 0) // librarian password is set to "librarian"
        {
            int choice;
            int cont;
            int hasExited = 0; // flag to track whether the user has already chosen to exit

            printf("\n\n\n\t\t\t\t\t\tLOGIN SUCCESSFUL !!");

            do
            {
                // display librarian's menu
                printf("\n\n\t\t\t\t\t\t|===============  LIBRARIAN MENU  ===============|\n");
                printf("\t\t\t\t\t\t|                                                |\n");
                printf("\t\t\t\t\t\t|  1. Search Book                                |\n");
                printf("\t\t\t\t\t\t|  2. Borrow and Return Book                     |\n");
                printf("\t\t\t\t\t\t|  3. Display Book List                          |\n");
                printf("\t\t\t\t\t\t|  4. Penalty Management for Late Return         |\n");
                printf("\t\t\t\t\t\t|  5. Generate Monthly Report                    |\n");
                printf("\t\t\t\t\t\t|  0. Exit                                       |\n");
                printf("\t\t\t\t\t\t|                                                |\n");
                printf("\t\t\t\t\t\t|================================================|\n");
                printf("\n\t\t\t\t\t\tEnter choice: ");
                scanf("%d", &choice);

                // function call according to the choice of the user using a switch statement
                switch (choice)
                {
                case 1:
                {
                    // display the header for search book
                    printf("\n\n|==========================================================|\n");
                    printf("|                                                          |\n");
                    printf("|                       SEARCH BOOK                        |\n");
                    printf("|                                                          |\n");
                    printf("|==========================================================|\n\n");

                    char searchISBN[ISBN_SIZE];
                    printf("\nEnter ISBN to search for a book: ");
                    scanf("%s", searchISBN);

                    // call function if the librarian chooses to search for a book
                    searchBook(books, totalBooks, searchISBN);
                }
                break;
                case 2:
                    // call function if the librarian chooses to access borrow or return books function
                    borrowORreturn(books, totalBooks, students, totalStudents);
                    break;
                case 3:
                    // call function if the librarian chooses to display the book list
                    displayBookList(books, totalBooks);
                    break;
                case 4:
                    // call function if the librarian chooses to find the total fine for each student
                    penaltyManagementforLibrarian(books, totalBooks, students, totalStudents);
                    break;
                case 5:
                    // call function for generating a monthly report
                    generateMonthlyReport(books, totalBooks, students, totalStudents);
                    break;
                case 0:
                    printf("\n\n\t\t\t\t\tEXITING....\n");
                    printf("\n\t\t\t\t\t|==========================================================|\n");
                    printf("\t\t\t\t\t|                                                          |\n");
                    printf("\t\t\t\t\t|                                                          |\n");
                    printf("\t\t\t\t\t|   THANK YOU FOR USING OUR LIBRARY MANAGEMENT SYSTEM      |\n");
                    printf("\t\t\t\t\t|                                                          |\n");
                    printf("\t\t\t\t\t|                                                          |\n");
                    printf("\t\t\t\t\t|==========================================================|\n\n");

                    hasExited = 1; // set the flag to indicate that the exit message has been displayed
                    break;
                default:
                    printf("\n\t\t\t\t\t\tInvalid choice. ");
                }

                // break out of the loop immediately if the user chooses to exit
                if (choice == 0)
                    break;

                // ask the librarian if they want to continue
                do
                {
                    printf("\n\nDo you want to return back to the librarian main menu? (1 for Yes / 0 for No): ");
                    scanf("%d", &cont);

                    // execute if the continue choice is not equal to 0 or 1
                    if (cont != 0 && cont != 1)
                    {
                        printf("Invalid choice.");
                    }
                } while (cont != 0 && cont != 1);

            } while (cont == 1);

            // thank the librarian when the librarian chooses not to continue OR when the continue choice is not equal to 0 or 1
            if (cont == 0 || (cont != 0 && cont != 1))
            {
                if (!hasExited)
                {
                    printf("\n\n\t\t\t\t\tEXITING....\n");
                    printf("\n\t\t\t\t\t|==========================================================|\n");
                    printf("\t\t\t\t\t|                                                          |\n");
                    printf("\t\t\t\t\t|                                                          |\n");
                    printf("\t\t\t\t\t|   THANK YOU FOR USING OUR LIBRARY MANAGEMENT SYSTEM      |\n");
                    printf("\t\t\t\t\t|                                                          |\n");
                    printf("\t\t\t\t\t|                                                          |\n");
                    printf("\t\t\t\t\t|==========================================================|\n\n");
                }
            }

            break; // exits the if statement
        }

        else
        {
            // give the user 3 chances to enter the password, decrease 1 attempt each time when the password is incorrect
            loginAttempts++;
            printf("\n\n\t\t\t\t\t\tInvalid password. %d attempts remaining.\n", 3 - loginAttempts);

            // execute if the user enters the wrong password three times
            if (loginAttempts == 3)
            {
                printf("\t\t\t\t\t\tMaximum login attempts reached. Exiting...\n\n");
                return;
            }
        }

    } while (1);
}

// function to handle student menu
void studentMenu(Book books[], int totalBooks, Student students[], int totalStudents)
{
    char studentId[STUDENTID_SIZE];
    char password[PASSWORD_SIZE];
    int loginAttempts = 0;
    int hasExited = 0; // flag to track whether the user has already chosen to exit

    do
    {
        // get and read information from user
        printf("\t\t\t\t\t\tEnter Student ID: ");
        scanf("%s", studentId);

        // get and read password from the user without showing it on the screen
        printf("\t\t\t\t\t\tEnter Password: ");
        int i = 0;
        char ch;
        while (1)
        {
            ch = _getch();  // use _getch() for hiding password input on the screen
            if (ch == '\r') // enter key
            {
                break;
            }
            else if (ch == '\b' && i > 0) // backspace key
            {
                i--;
                printf("\b \b"); // move the cursor back and erase the character
            }
            else if (ch != '\b') // ignore backspace
            {
                password[i++] = ch;
                printf("*"); // print asterisk (*) instead of the actual character
            }
        }
        password[i] = '\0'; // null-terminate the password

        // to check if the student credentials are correct
        // execute if block if student credentials (studentId and password) are correct/match
        if (validateStudentLogin(students, totalStudents, studentId, password))
        {
            printf("\n\n\n\t\t\t\t\t\tLOGIN SUCCESSFUL !!");

            // find the student's name based on entered student ID
            char studentName[NAME_SIZE];
            for (int i = 0; i < totalStudents; i++)
            {
                if (strcmp(students[i].studentId, studentId) == 0)
                {
                    strcpy(studentName, students[i].name);
                    break;
                }
            }

            // welcome user by displaying their name
            printf("\n\t\t\t\t\t\tWelcome, %s, to the Library Management System!", studentName);

            int choice;
            int cont;
            do
            {
                // display student's menu
                printf("\n\n \t\t\t\t\t\t|===============  STUDENT MENU  =================|\n");
                printf("\t\t\t\t\t\t|                                                |\n");
                printf("\t\t\t\t\t\t|  1. Search Book                                |\n");
                printf("\t\t\t\t\t\t|  2. Display Book List                          |\n");
                printf("\t\t\t\t\t\t|  3. Penalty Management for Late Return         |\n");
                printf("\t\t\t\t\t\t|  0. Exit                                       |\n");
                printf("\t\t\t\t\t\t|                                                |\n");
                printf("\t\t\t\t\t\t|================================================|\n");

                printf("\n\t\t\t\t\t\tEnter choice: ");
                scanf("%d", &choice);

                // function call according to choice of user using switch statement
                switch (choice)
                {
                case 1:
                {
                    // display header for search book
                    printf("\n\n|==========================================================|\n");
                    printf("|                                                          |\n");
                    printf("|                       SEARCH BOOK                        |\n");
                    printf("|                                                          |\n");
                    printf("|==========================================================|\n\n");

                    char searchISBN[ISBN_SIZE];
                    printf("\nEnter ISBN to search for a book: ");
                    scanf("%s", searchISBN);

                    // call function if student choose to search a book
                    searchBook(books, totalBooks, searchISBN);
                }
                break;

                case 2:
                    // call function to display book list
                    displayBookList(books, totalBooks);
                    break;
                case 3:
                    // call function to find total fine for each student
                    penaltyManagementforStudent(books, totalBooks, students, totalStudents);
                    break;

                case 0:
                    printf("\n\n\t\t\t\t\tEXITING....\n");
                    printf("\n\t\t\t\t\t|==========================================================|\n");
                    printf("\t\t\t\t\t|                                                          |\n");
                    printf("\t\t\t\t\t|                                                          |\n");
                    printf("\t\t\t\t\t|   THANK YOU FOR USING OUR LIBRARY MANAGEMENT SYSTEM      |\n");
                    printf("\t\t\t\t\t|                                                          |\n");
                    printf("\t\t\t\t\t|                                                          |\n");
                    printf("\t\t\t\t\t|==========================================================|\n\n");

                    hasExited = 1; // Set the flag to indicate that the exit message has been displayed
                    break;
                default:
                    printf("\n\t\t\t\t\t\tInvalid choice. ");
                }

                // break out of the loop immediately if the user chooses to exit
                if (choice == 0)
                    break;

                // ask the student if they want to continue
                do
                {
                    printf("\n\nDo you want to return back to the student main menu? (1 for Yes / 0 for No): ");
                    scanf("%d", &cont);

                    // execute if the continue choice is not equal to 0 or 1
                    if (cont != 0 && cont != 1)
                    {
                        printf("Invalid choice.");
                    }
                } while (cont != 0 && cont != 1);

            } while (cont == 1);

            // thank the student when the student chooses not to continue OR when the continue choice is not equal to 0 or 1
            if (cont == 0 || (cont != 0 && cont != 1))
            {
                if (!hasExited)
                {
                    printf("\n\n\t\t\t\t\tEXITING....\n");
                    printf("\n\t\t\t\t\t|==========================================================|\n");
                    printf("\t\t\t\t\t|                                                          |\n");
                    printf("\t\t\t\t\t|                                                          |\n");
                    printf("\t\t\t\t\t|   THANK YOU FOR USING OUR LIBRARY MANAGEMENT SYSTEM      |\n");
                    printf("\t\t\t\t\t|                                                          |\n");
                    printf("\t\t\t\t\t|                                                          |\n");
                    printf("\t\t\t\t\t|==========================================================|\n\n");
                }
            }

            break; // exits the if statement
        }

        else
        {
            // give the user 3 chances to enter the password, decrease 1 attempt each time when the password is incorrect
            loginAttempts++;
            printf("\n\n\t\t\t\t\t\tInvalid password. %d attempts remaining.\n", 3 - loginAttempts);

            // execute if the user enters the wrong password three times
            if (loginAttempts == 3)
            {
                printf("\t\t\t\t\t\tMaximum login attempts reached. Exiting...\n\n");
                return;
            }
        }
    } while (1);
}

// function to validate student login credentials (studentId and password from user input)
bool validateStudentLogin(Student students[], int totalStudents, char *studentId, char *password)
{
    for (int i = 0; i < totalStudents; ++i)
    {
        // use strcmp to check if both studentId and password match are equal to the studentId and password of the i-th student in the students array
        // if both conditions are true, it means the tudentId and password from user input match the credentials of a student in the array
        if (strcmp(students[i].studentId, studentId) == 0 && strcmp(students[i].password, password) == 0)
        {
            return true; // return true if match
        }
    }
    return false;
}

// function to search for a book by ISBN
void searchBook(Book books[], int totalBooks, char *searchISBN)
{
    int contSearch = 0; // variable to track whether the user wants to continue searching

    do
    {
        int found = 0; // initialize found variable to keep track of whether the book is found

        // iterate through the array of books
        for (int i = 0; i < totalBooks; ++i)
        {
            // check if the ISBN of the current book matches with the ISBN from the user's input
            if (strcmp(books[i].ISBN, searchISBN) == 0)
            {
                printf("\n\nDISPLAYING INFORMATION OF THE BOOK WITH ISBN %s", searchISBN);

                // print table header
                printf("\n\n|---------------------|---------------------|-----------|--------------------------|---------------------|---------------------|---------------------|\n");
                printf("| %-20s| %-20s| %-10s| %-25s| %-20s| %-20s| %-20s|\n", "Title", "Author", "ISBN", "Year of Publication", "Available Copies", "Total Copies", "Borrowed Copies");
                printf("|---------------------|---------------------|-----------|--------------------------|---------------------|---------------------|---------------------|\n");

                // display information of the searched book in tabular format with explicit lines
                printf("| %-20s| %-20s| %-10s| %-25s| %-20s| %-20s| %-20s|\n",
                       books[i].title, books[i].author, books[i].ISBN, books[i].yearOfPublication, books[i].copiesAvailable, books[i].totalCopies, books[i].borrowedCopies);
                printf("|---------------------|---------------------|-----------|--------------------------|---------------------|---------------------|---------------------|\n\n");

                found = 1; // set the found variable to 1 to indicate the book is found
                // break;     // stop searching if the book is found
            }
        }

        // check if the book was not found and display a message
        if (!found)
        {
            printf("\nBook not found with ISBN: %s\n", searchISBN);
        }

        // prompt the user to re-enter the ISBN and ask if they want to continue searching
        printf("Do you want to continue searching? (1 for Yes / 0 for No): ");
        scanf("%d", &contSearch);

        if (contSearch)
        {
            // if user wants to continue searching, prompt user for the ISBN again
            printf("\nEnter ISBN again: ");
            scanf("%s", searchISBN);
        }

    } while (contSearch);
}

// to sort an array based on books' titles in ascending order
void bubbleSort(Book books[], int totalBooks)
{
    // outer loop, iterates over each book in the array
    for (int i = 1; i < totalBooks; i++)
    {
        //  inner loop, compare adjacent books in the array
        for (int j = 0; j < totalBooks - i - 1; j++) // gradually move the larger items to the end of the array
        {
            // strcmp will return 0, negative value and positive value
            // compare adjacent books' titles and swap if they are in the wrong order
            if (strcmp(books[j].title, books[j + 1].title) > 0)
            {
                struct Book temp = books[j]; // temporary variable used to hold the values of the current book temporarily
                books[j] = books[j + 1];
                books[j + 1] = temp;
            }
        }
    }
}

// function to display the list of books
void displayBookList(Book books[], int totalBooks)
{
    // display the header for book list
    printf("\n\n\n\t\t|==================================================================================================================|\n");
    printf("\t\t|                                                                                                                  |\n");
    printf("\t\t|                    DISPLAYING LIST OF BOOKS IN ASCENDING ORDER ACCORDING TO BOOKS' TITLES ...                    |\n");
    printf("\t\t|                                                                                                                  |\n");
    printf("\t\t|==================================================================================================================|\n\n\n");

    // perform bubble sort on the array of books based on books' titles
    bubbleSort(books, totalBooks);

    // display the list of all books in ascending order according to books' titles in tabular format
    // printf("\n\nDISPLAYING LIST OF BOOKS IN ASCENDING ORDER ACCORDING TO BOOKS' TITLES ... \n\n");
    printf("|---------------------|---------------------|-----------|--------------------------|---------------------|---------------------|---------------------|\n");
    printf("| %-20s| %-20s| %-10s| %-25s| %-20s| %-20s| %-20s|\n", "Title", "Author", "ISBN", "Year of Publication", "Available Copies", "Total Copies", "Borrowed Copies");
    printf("|---------------------|---------------------|-----------|--------------------------|---------------------|---------------------|---------------------|\n");

    // print the contents of the books line by line
    for (int i = 0; i < totalBooks; i++)
    {
        printf("| %-20s| %-20s| %-10s| %-25s| %-20s| %-20s| %-20s|\n", books[i].title, books[i].author, books[i].ISBN,
               books[i].yearOfPublication, books[i].copiesAvailable, books[i].totalCopies, books[i].borrowedCopies);
        printf("|---------------------|---------------------|-----------|--------------------------|---------------------|---------------------|---------------------|\n");
    }
}

// Function to calculate the penalty for late return of a book based on due date and return date
// dueDate: A string representing the due date in the format "DD-MM-YYYY"
// returnDate: A string representing the return date in the format "DD-MM-YYYY"
// returns the calculated penalty as a double value
double calculatePenalty(const char *dueDate, const char *returnDate)
{
    // Create struct tm objects to represent the due date and return date
    struct tm tmDueDate = {0};
    struct tm tmReturnDate = {0};

    // extract day, month, and year from the due date string and adjust month and year values
    sscanf(dueDate, "%d-%d-%d", &tmDueDate.tm_mday, &tmDueDate.tm_mon, &tmDueDate.tm_year);
    tmDueDate.tm_mon -= 1;     // adjust month to be in the range [0, 11]
    tmDueDate.tm_year -= 1900; // adjust year to be the number of years since 1900

    sscanf(returnDate, "%d-%d-%d", &tmReturnDate.tm_mday, &tmReturnDate.tm_mon, &tmReturnDate.tm_year);
    tmReturnDate.tm_mon -= 1;
    tmReturnDate.tm_year -= 1900;

    // convert struct tm objects to time_t objects
    time_t dueTime = mktime(&tmDueDate);
    time_t returnTime = mktime(&tmReturnDate); // store in number of seconds

    // calculate the difference in days between return date and due date
    // difftime() calculates the difference in seconds between returnTime and dueTime
    double daysDifference = difftime(returnTime, dueTime) / (60 * 60 * 24); // (second,min,hour)

    // calculate the penalty as the maximum of 0 and the rounded up value of days difference
    // the penalty is multiplied by 1.00 to ensure it's a double value
    double penalty = fmax(0, ceil(daysDifference)) * 1.00;

    // return the calculated penalty
    return penalty;
}

// function to show penalty for late return books
void penaltyManagementforLibrarian(Book books[], int totalBooks, Student students[], int totalStudents)
{
    // open the dates.txt file for reading
    FILE *file = fopen("dates.txt", "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening the dates file.\n");
    }

    // declaration of variables to store information from each line in dates.txt
    char ISBN[ISBN_SIZE];
    char student[50];
    char title[BOOKTITLE_SIZE];
    char borrowDate[DATE_SIZE];
    char dueDate[DATE_SIZE];
    char returnDate[DATE_SIZE];

    // array to store book information for penalty calculation
    totalBooks = 0;

    // read each line from dates.txt and store relevant information in the books array
    char line[LINE_SIZE];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        line[strcspn(line, "\n")] = '\0';

        // extract information from the line and store it in the books array
        if (sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%s", ISBN, student, title, borrowDate, dueDate, returnDate) == 6)
        {
            strncpy(books[totalBooks].ISBN, ISBN, sizeof(books[totalBooks].ISBN));
            strncpy(books[totalBooks].student, student, sizeof(books[totalBooks].student));
            strncpy(books[totalBooks].title, title, sizeof(books[totalBooks].title));
            strncpy(books[totalBooks].borrowDate, borrowDate, sizeof(books[totalBooks].borrowDate));
            strncpy(books[totalBooks].dueDate, dueDate, sizeof(books[totalBooks].dueDate));
            strncpy(books[totalBooks].returnDate, returnDate, sizeof(books[totalBooks].returnDate));

            totalBooks++;
        }
    }

    // display penalty record for each book
    printf("\n\n\t\t\t\t\t\t\t\t\t|==========================================================|\n");
    printf("\t\t\t\t\t\t\t\t\t|                                                          |\n");
    printf("\t\t\t\t\t\t\t\t\t|          LIBRARY BORROWING AND PENALTY RECORDS           |\n");
    printf("\t\t\t\t\t\t\t\t\t|                                                          |\n");
    printf("\t\t\t\t\t\t\t\t\t|==========================================================|\n\n");

    // display header for the penalty record table
    printf("\t\t\t\t|----------------------|----------------------|----------------------|-----------------|-----------------|-----------------|-----------------|\n");
    printf("\t\t\t\t| %-20s | %-20s | %-20s | %-15s | %-15s | %-15s | %-15s |\n", "Student", "Book Title", "ISBN", "Borrow Date", "Due Date", "Return Date", "Penalty (RM)");
    printf("\t\t\t\t|----------------------|----------------------|----------------------|-----------------|-----------------|-----------------|-----------------|\n");

    // display contents of the table
    for (int i = 0; i < totalBooks; ++i)
    {
        // check if the book has been returned and has a due date
        if (strlen(books[i].returnDate) > 0 && strlen(books[i].dueDate) > 0)
        {
            // calculate the penalty for late return
            double penalty = calculatePenalty(books[i].dueDate, books[i].returnDate);

            // display information for each book in the penalty record
            printf("\t\t\t\t| %-20s | %-20s | %-20s | %-15s | %-15s | %-15s | %-15.2f |\n",
                   books[i].student, books[i].title, books[i].ISBN,
                   books[i].borrowDate, books[i].dueDate, books[i].returnDate, penalty);
        }
    }
    printf("\t\t\t\t|----------------------|----------------------|----------------------|-----------------|-----------------|-----------------|-----------------|\n");

    // users are given 2 choices to choose from
    // either view total fines for all students or view total fine for a specific student
    int choice;
    do
    {
        printf("\n\nEnter 1 to view the total fine for every student.\n");
        printf("Enter 2 to find the total fine for a specific student using student ID. \n");
        printf("Enter Choice (1 or 2): ");
        scanf("%d", &choice);

        if (choice != 1 && choice != 2)
        {
            printf("\nInvalid choice. Please enter 1 or 2.\n");
        }

    } while (choice != 1 && choice != 2);

    // display total fines for all students if user's choice is 1
    if (choice == 1)
    {
        printf("\n\nDisplaying Total Penalty for All Students... \n");
        printf("|----------------------|--------------------|\n");
        printf("| %-20s | %-15s |\n", "Student", "Total Penalty (RM)");
        printf("|----------------------|--------------------|\n");

        // calculate and display total fines for each student
        for (int i = 0; i < totalStudents; ++i)
        {
            double totalPenalty = 0.0;

            for (int j = 0; j < totalBooks; ++j)
            {
                if (strcmp(books[j].student, students[i].name) == 0 && strlen(books[j].returnDate) > 0 && strlen(books[j].dueDate) > 0)
                {
                    totalPenalty += calculatePenalty(books[j].dueDate, books[j].returnDate);
                }
            }

            printf("| %-20s | %-18.2f |\n", students[i].name, totalPenalty);
        }
        printf("|----------------------|--------------------|\n");
    }

    // display total fines for a specific student using student id if user's choice is 1
    if (choice == 2)
    {
        char studentID[STUDENTID_SIZE];
        int studentIndex = -1;

        // ask the user to enter student id to view that particular student's total fine
        do
        {
            printf("\nEnter Student ID: ");
            scanf("%s", studentID);

            // check if the entered student id exists in the students array
            for (int i = 0; i < totalStudents; ++i)
            {
                if (strcmp(students[i].studentId, studentID) == 0)
                {
                    studentIndex = i;
                    break; // no need to continue checking other students once the student is found
                }
            }

            // if the student id does not exist in the students array, prompt the user to enter a valid student id
            if (studentIndex == -1)
            {
                printf("Invalid student ID. Please enter a valid student ID.\n");
            }

        } while (studentIndex == -1);

        // calculate the total fine for the selected student
        double totalPenalty = 0.0;

        for (int j = 0; j < totalBooks; ++j)
        {
            if (strcmp(books[j].student, students[studentIndex].name) == 0 && strlen(books[j].returnDate) > 0 && strlen(books[j].dueDate) > 0)
            {
                totalPenalty += calculatePenalty(books[j].dueDate, books[j].returnDate);
            }
        }

        // display the total fine for the selected student
        printf("Total Fine for %s is RM %.2f\n\n", students[studentIndex].name, totalPenalty);
        printf("|----------------------|----------------------|----------------------|-----------------|-----------------|-----------------|\n");
        printf("| %-20s | %-20s | %-20s | %-15s | %-15s | %-15s |\n", "Book Returned Late", "ISBN", "Borrow Date", "Due Date", "Return Date", "Penalty (RM)");
        printf("|----------------------|----------------------|----------------------|-----------------|-----------------|-----------------|\n");

        // loop through each book to find late returns for the selected student
        for (int i = 0; i < totalBooks; ++i)
        {
            if (strcmp(books[i].student, students[studentIndex].name) == 0 &&
                strlen(books[i].returnDate) > 0 && strlen(books[i].dueDate) > 0 &&
                calculatePenalty(books[i].dueDate, books[i].returnDate) > 0)
            {
                // display information for each late return
                double penalty = calculatePenalty(books[i].dueDate, books[i].returnDate);
                printf("| %-20s | %-20s | %-20s | %-15s | %-15s | %-15.2f |\n",
                       books[i].title, books[i].ISBN,
                       books[i].borrowDate, books[i].dueDate, books[i].returnDate, penalty);
                printf("|----------------------|----------------------|----------------------|-----------------|-----------------|-----------------|\n");
            }
        }
    }

    // close dates.txt file
    fclose(file);
}

// function to show penalty for late return books
void penaltyManagementforStudent(Book books[], int totalBooks, Student students[], int totalStudents)
{
    // open the dates.txt file for reading
    FILE *file = fopen("dates.txt", "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening the dates file.\n");
    }

    // declaration of variables to store information from each line in dates.txt
    char ISBN[ISBN_SIZE];
    char student[50];
    char title[BOOKTITLE_SIZE];
    char borrowDate[DATE_SIZE];
    char dueDate[DATE_SIZE];
    char returnDate[DATE_SIZE];

    // array to store book information for penalty calculation
    totalBooks = 0;

    // read each line from dates.txt and store relevant information in the books array
    char line[LINE_SIZE];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        line[strcspn(line, "\n")] = '\0';

        // extract information from the line and store it in the books array
        if (sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%s", ISBN, student, title, borrowDate, dueDate, returnDate) == 6)
        {
            strncpy(books[totalBooks].ISBN, ISBN, sizeof(books[totalBooks].ISBN));
            strncpy(books[totalBooks].student, student, sizeof(books[totalBooks].student));
            strncpy(books[totalBooks].title, title, sizeof(books[totalBooks].title));
            strncpy(books[totalBooks].borrowDate, borrowDate, sizeof(books[totalBooks].borrowDate));
            strncpy(books[totalBooks].dueDate, dueDate, sizeof(books[totalBooks].dueDate));
            strncpy(books[totalBooks].returnDate, returnDate, sizeof(books[totalBooks].returnDate));

            totalBooks++;
        }
    }

    // declaration of variables
    char studentID[STUDENTID_SIZE];
    int studentIndex = -1;

    // ask the user to enter student id to view that particular student's total fine
    do
    {
        printf("\nEnter your Student ID to view your total fine: ");
        scanf("%s", studentID);

        // check if the entered student id exists in the students array
        for (int i = 0; i < totalStudents; ++i)
        {
            if (strcmp(students[i].studentId, studentID) == 0)
            {
                studentIndex = i;
                break; // No need to continue checking other students once the student is found
            }
        }

        // if the student id does not exist in the students array, prompt the user to enter a valid student id
        if (studentIndex == -1)
        {
            printf("Invalid student ID. Please enter a valid student ID.\n");
        }

    } while (studentIndex == -1);

    // calculate the total fine for the selected student
    double totalPenalty = 0.0;

    for (int j = 0; j < totalBooks; ++j)
    {
        if (strcmp(books[j].student, students[studentIndex].name) == 0 && strlen(books[j].returnDate) > 0 && strlen(books[j].dueDate) > 0)
        {
            totalPenalty += calculatePenalty(books[j].dueDate, books[j].returnDate);
        }
    }

    // display the total fine for the selected student
    printf("\nTotal Fine for %s is RM %.2f\n\n", students[studentIndex].name, totalPenalty);
    printf("DISPLAYING BOOK(S) THAT ARE RETURNED LATE .... \n");
    printf("|----------------------|----------------------|----------------------|-----------------|-----------------|-----------------|\n");
    printf("| %-20s | %-20s | %-20s | %-15s | %-15s | %-15s |\n", "Book Returned Late", "ISBN", "Borrow Date", "Due Date", "Return Date", "Penalty (RM)");
    printf("|----------------------|----------------------|----------------------|-----------------|-----------------|-----------------|\n");

    // loop through each book to find late returns for the selected student
    for (int i = 0; i < totalBooks; ++i)
    {
        if (strcmp(books[i].student, students[studentIndex].name) == 0 &&
            strlen(books[i].returnDate) > 0 && strlen(books[i].dueDate) > 0 &&
            calculatePenalty(books[i].dueDate, books[i].returnDate) > 0)
        {
            // display information for each late return
            double penalty = calculatePenalty(books[i].dueDate, books[i].returnDate);
            printf("| %-20s | %-20s | %-20s | %-15s | %-15s | %-15.2f |\n",
                   books[i].title, books[i].ISBN,
                   books[i].borrowDate, books[i].dueDate, books[i].returnDate, penalty);
            printf("|----------------------|----------------------|----------------------|-----------------|-----------------|-----------------|\n");
        }
    }

    // remind studenr to pay their fine and how the fine for late return is calculated
    printf("\nPlease pay your fine as soon as possible. Thank you for your cooperation.\n");
    printf("**NOTE THAT RM1.00 PER DAY IS CHARGED FOR LATE RETURN. \n");

    // close dates.txt file
    fclose(file);
}

// function to validate student id (to check whether the entered student id is in the students.txt file)
int validateStudentID(Student students[], int totalStudents, const char studentId[])
{
    for (int i = 0; i < totalStudents; i++)
    {
        if (strcmp(students[i].studentId, studentId) == 0)
        {
            return i; // return student index if found
        }
    }
    return -1; // return -1 if not found
}

// function to validate book ISBN (to check whether the entered book ISBN is in the books.txt file)
int validateISBN(Book books[], int totalBooks, const char ISBN[])
{
    for (int i = 0; i < totalBooks; ++i)
    {
        if (strcmp(books[i].ISBN, ISBN) == 0)
        {
            return i; // return book index if found
        }
    }
    return -1; // return -1 if not found
}

// function to validate date format (DD-MM-YYYY)
int isValidDateFormat(const char *date)
{
    int day, month, year;
    if (sscanf(date, "%2d-%2d-%4d", &day, &month, &year) == 3)
    {
        // check if month and day are within valid ranges and if the year is exactly four digits
        if ((month >= 1 && month <= 12) && (day >= 1 && day <= 31) && (year >= 1800 && year <= 2024))
        {
            return 1; // valid date format
        }
    }
    return 0; // invalid date format
}

// implementation of the compareDates function
int compareDates(const char *date1, const char *date2)
{
    // date format is DD-MM-YYYY
    int day1, month1, year1;
    int day2, month2, year2;

    sscanf(date1, "%d-%d-%d", &day1, &month1, &year1);
    sscanf(date2, "%d-%d-%d", &day2, &month2, &year2);

    if (year1 != year2)
    {
        return year1 - year2;
    }
    if (month1 != month2)
    {
        return month1 - month2;
    }
    return day1 - day2;
}

// function for when students want to borrow book
void borrowBook(Book books[], int totalBooks, Student students[], int totalStudents)
{
    // display following header to make it more interaactive
    printf("\n|========================================================|\n");
    printf("|                      Borrow Book                       |\n");
    printf("|========================================================|\n");

    // declaration of variables
    char studentId[STUDENTID_SIZE];
    char ISBN[ISBN_SIZE];
    int studentIndex, bookIndex;

    do
    {
        // VALIDATE STUDENT ID AND FIND CURRENT STUDENT INDEX USING STUDENT ID ENTERED BY USER
        printf("\nEnter Student ID to borrow the book: ");
        scanf("%s", studentId);
        studentIndex = validateStudentID(students, totalStudents, studentId);

        if (studentIndex == -1)
        {
            printf("Invalid student ID. Please try again.\n");
        }

    } while (studentIndex == -1); // continue to ask librarian to enter valid student id until they get it right

    do
    {
        // VALIDATE BOOK ISBN AND FIND CURRENT BOOK INDEX USING ISBN ENTERED BY USER
        printf("Enter ISBN of the book to be borrowed: ");
        scanf("%s", ISBN);

        bookIndex = validateISBN(books, totalBooks, ISBN);

        // display error message if the entered book ISBN is not in books.txt file
        if (bookIndex == -1)
        {
            printf("\nBook not found with ISBN: %s. Please try again.\n", ISBN);
        }

    } while (bookIndex == -1);

    // display contents of the selected book
    printf("\n\nDisplaying Selected Book: \n");
    printf("|---------------------|---------------------|-----------|--------------------------|---------------------|---------------------|---------------------|\n");
    printf("| %-20s| %-20s| %-10s| %-25s| %-20s| %-20s| %-20s|\n", "Title", "Author", "ISBN", "Year of Publication", "Available Copies", "Total Copies", "Borrowed Copies");
    printf("|---------------------|---------------------|-----------|--------------------------|---------------------|---------------------|---------------------|\n");

    printf("| %-20s| %-20s| %-10s| %-25s| %-20s| %-20s| %-20s|\n",
           books[bookIndex].title, books[bookIndex].author, books[bookIndex].ISBN,
           books[bookIndex].yearOfPublication, books[bookIndex].copiesAvailable,
           books[bookIndex].totalCopies, books[bookIndex].borrowedCopies);
    printf("|---------------------|---------------------|-----------|--------------------------|---------------------|---------------------|---------------------|\n\n");

    // INSERT INFORMATION TO DATES.TXT FILE TO UPDATE WHO BORROWS THE BOOK
    if (atoi(books[bookIndex].copiesAvailable) > 0)
    {
        // inform the librarian that the book is available and display the number of available copies
        printf("\nThis book is AVAILABLE! \n");
        printf("Number of copies available: %s\n\n", books[bookIndex].copiesAvailable);

        // get and read input borrow date and due date from user input
        char borrowDate[DATE_SIZE];
        char dueDate[DATE_SIZE];

        // get and validate input for the borrow date
        // do while loop to keep asking the user to enter correct date format/range until they get it right
        do
        {
            printf("Enter Borrow Date (DD-MM-YYYY): ");
            scanf("%s", borrowDate);

            if (!isValidDateFormat(borrowDate))
            {
                printf("Invalid date format or range. Please re-enter valid date in DD-MM-YYYY format.\n\n");
            }
        } while (!isValidDateFormat(borrowDate));

        // get and validate input for the due date
        do
        {
            printf("Enter Due Date (DD-MM-YYYY): ");
            scanf("%s", dueDate);

            if (!isValidDateFormat(dueDate) || compareDates(dueDate, borrowDate) < 0)
            {
                printf("\nInvalid due date. Please enter a valid date later than the borrow date (%s).\n\n", borrowDate);
            }
        } while (!isValidDateFormat(dueDate) || compareDates(dueDate, borrowDate) < 0);

        // write student/borrower details, borrowed book details, borrow date, and due date to dates.txt file
        FILE *datesFile = fopen("dates.txt", "a"); // open dates.txt file in append mode
        if (datesFile == NULL)
        {
            printf("dates.txt file could not be opened"); // display an error message if dates.txt file cannot be opened/not found
            return;
        }

        // print the borrowing information to dates.txt file (including borrow date, due date, borrower name, book ISBN and title)
        fprintf(datesFile, "%s,%s,%s,%s,%s,%s\n",
                ISBN, students[studentIndex].name, books[bookIndex].title, borrowDate, dueDate, NULL); // return date is set to NULL(empty)
        fclose(datesFile);                                                                             // close dates.txt file

        // to update the new values for available book and borrowed book copies
        int newAvail = atoi(books[bookIndex].copiesAvailable) - 1;   // minus available copies by 1 because it is borrowed by student
        int newBorrowed = atoi(books[bookIndex].borrowedCopies) + 1; // add borrowed copies by 1
        sprintf(books[bookIndex].copiesAvailable, "%d", newAvail);   // format integer to character
        sprintf(books[bookIndex].borrowedCopies, "%d", newBorrowed);

        // STORE UPDATED DATA IN A TEMPORARY ARRAY
        FILE *booksFile = fopen("books.txt", "r"); // open books.txt file in read mode
        if (booksFile == NULL)
        {
            puts("books.txt could not be opened\n"); // display an error message if dates.txt file cannot be opened/not found
            return;
        }

        // declaration
        char temp[LINE_SIZE];
        char newContent[LINE_SIZE] = "";

        // read contents from books.txt file and store it in a temp array
        while (fgets(temp, sizeof(temp), booksFile) != NULL)
        {
            char lineISBN[ISBN_SIZE];
            sscanf(temp, "%*s %*s %13s", lineISBN); // extract ISBN from the current line, *s is used to ignore part of the input

            // check if the ISBN from the file matches with ISBN from user input, if true, execute if block
            if (strcmp(lineISBN, ISBN) == 0)
            {
                // format the content with updated book information and store it in respective members/characteristics
                sprintf(temp, "%s %s %s %s %s %s %s\n",
                        books[bookIndex].title, books[bookIndex].author, books[bookIndex].ISBN,
                        books[bookIndex].yearOfPublication, books[bookIndex].copiesAvailable,
                        books[bookIndex].totalCopies, books[bookIndex].borrowedCopies);
            }
            strcat(newContent, temp); // concatenate temp (either updated or unchanged) to newContent which was initialized to empty string
        }
        fclose(booksFile); // close books.txt file

        // UPDATE BOOKS.TXT FILE WITH NEW DATA
        booksFile = fopen("books.txt", "w"); // open books.txt file in write mode to update its content
        fputs(newContent, booksFile);        // write the new modified content to the books.txt file
        fclose(booksFile);                   // close books.xtt file

        // inform the librarian that the book has been borrowed successfully
        printf("\n\n%s (Student ID: %s) has successfully borrowed the book \"%s\"!\n", students[studentIndex].name, studentId, books[bookIndex].title);

        // display the student receipt
        printf("\n|============================================|\n");
        printf("|              STUDENT RECEIPT               |\n");
        printf("|============================================|\n");
        printf("|                                            |\n");
        printf("| Student's Name: %-27s|\n", students[studentIndex].name);
        printf("| Book Borrowed: %-28s|\n", books[bookIndex].title);
        printf("| Author: %-35s|\n", books[bookIndex].author);
        printf("| ISBN: %s                                 |\n", books[bookIndex].ISBN);
        printf("| Year Of Publication: %s                  |\n", books[bookIndex].yearOfPublication);
        printf("| Borrow Date: %s                    |\n", borrowDate);
        printf("| Due Date: %s                       |\n", dueDate);
        printf("|                                            |\n");
        printf("| Happy Reading!!                            |\n");
        printf("|                                            |\n");
        printf("|============================================|\n");

        // display updated book information in a tabular format
        printf("\nUpdated Book Information:\n");
        printf("|---------------------|---------------------|-----------|--------------------------|---------------------|---------------------|---------------------|\n");
        printf("| %-20s| %-20s| %-10s| %-25s| %-20s| %-20s| %-20s|\n", "Title", "Author", "ISBN", "Year of Publication", "Available Copies", "Total Copies", "Borrowed Copies");
        printf("|---------------------|---------------------|-----------|--------------------------|---------------------|---------------------|---------------------|\n");

        printf("| %-20s| %-20s| %-10s| %-25s| %-20s| %-20s| %-20s|\n",
               books[bookIndex].title, books[bookIndex].author, books[bookIndex].ISBN,
               books[bookIndex].yearOfPublication, books[bookIndex].copiesAvailable,
               books[bookIndex].totalCopies, books[bookIndex].borrowedCopies);
        printf("|---------------------|---------------------|-----------|--------------------------|---------------------|---------------------|---------------------|\n");
    }

    // if books[bookIndex].copiesAvailable is not more than 0, execute else block
    else
    {
        printf("\nNo available copies of the book with ISBN: %s. \n", ISBN);
    }
}

// function for when students want to return book
void returnBook(Book books[], int totalBooks, Student students[], int totalStudents)
{
    // display the following header to make it more interactive
    printf("\n|===============================================|\n");
    printf("|                  Return Book                  |\n");
    printf("|===============================================|\n");

    // declaration of variables
    char studentId[STUDENTID_SIZE];
    char ISBN[ISBN_SIZE];
    char returnDate[DATE_SIZE];

    int studentIndex, bookIndex;

    do
    {
        // VALIDATE STUDENT ID AND TO FIND CURRENT STUDENT INDEX USING STUDENT ID ENTERED BY USER
        printf("\nEnter Student ID: ");
        scanf("%s", studentId);
        studentIndex = validateStudentID(students, totalStudents, studentId);
        if (studentIndex == -1)
        {
            printf("Invalid student ID. Please try again.\n");
        }
    } while (studentIndex == -1);

    do
    {
        // VALIDATE BOOK ISBN AND TO FIND CURRENT BOOK INDEX USING ISBN ENTERED BY USER
        printf("Enter ISBN of the book to be returned: ");
        scanf("%s", ISBN);
        bookIndex = validateISBN(books, totalBooks, ISBN);

        // execute if isbn is not found
        if (bookIndex == -1)
        {
            printf("\nBook not found with ISBN: %s. Please try again.\n", ISBN);
        }
        else
        {
            // OPEN DATES.TXT FILE IN READ MODE AND EXTRACT TARGETED ISBN, STUDENT NAME, AND OTHER DETAILS FOR RETURNED BOOK
            FILE *datesFile = fopen("dates.txt", "r");
            if (datesFile == NULL)
            {
                puts("dates.txt file could not be opened\n"); // display error message if dates.txt cannot be opened/found
            }

            // declaration of variables
            char temp[5000];
            char newContent[5000] = ""; // declare and initialize newContent as an empty string
            int returned = 0;           // initialize returned variable to keep track of whether the book is returned

            // read all the contents from dates.txt file and store it in temp array
            while (fgets(temp, sizeof(temp), datesFile) != NULL)
            {
                // extract ISBN, student name, and other details from dates.txt file
                char targetISBN[14], targetStudent[50], targetTitle[50], targetBorrowDate[11], targetDueDate[11], targetReturnDate[11];
                sscanf(temp, "%[^,],%[^,],%[^,],%[^,],%[^,],%s", targetISBN, targetStudent, targetTitle, targetBorrowDate, targetDueDate, targetReturnDate);
                // sscanf is read from given string

                // compare ISBN, student's name and if the return date is NULL to identify the line to modify
                // execute if block if all 3 conditions are true
                if (strcmp(targetISBN, ISBN) == 0 && strcmp(targetStudent, students[studentIndex].name) == 0 && strcmp(targetReturnDate, "(null)") == 0)
                {
                    // get borrow date and due date from dates.txt
                    printf("\n\n|===============================================|");
                    printf("\n|              BOOK TO BE RETURNED              |");
                    printf("\n|===============================================|");
                    printf("\n| Book ISBN:      %-30s|", targetISBN);
                    printf("\n| Book Title:     %-30s|", targetTitle);
                    printf("\n| Borrower Name:  %-30s|", targetStudent);
                    printf("\n| Borrow Date:    %-30s|", targetBorrowDate);
                    printf("\n| Due Date:       %-30s|", targetDueDate);
                    printf("\n|===============================================|\n\n");

                    // if the return date is invalid or is before the borrow date which doesn't make sense
                    do
                    {
                        printf("Enter Return Date (DD-MM-YYYY): ");
                        scanf("%s", returnDate);

                        // execute if the return date is in invalid format
                        if (!isValidDateFormat(returnDate))
                        {
                            printf("Invalid date format or range. Please enter the date in DD-MM-YYYY format.\n\n");
                        }
                        // execute if the return date is before the borrow date (which doesnt make sense)
                        else if (compareDates(returnDate, targetBorrowDate) < 0)
                        {
                            printf("Invalid return date. Please enter a valid date later than or equal to the borrow date (%s).\n\n", targetBorrowDate);
                        }
                    } while (!isValidDateFormat(returnDate) || compareDates(returnDate, targetBorrowDate) < 0); // continue the loop as long as the return date is invalid or before the borrow date

                    // calculate penalty
                    double penalty = calculatePenalty(targetDueDate, returnDate);

                    // display the fine if the return date is later than the due date
                    if (penalty > 0)
                    {
                        // DISPLAY UPDATED BOOK RETURNED RECEIPT
                        printf("\n\n|===============================================|");
                        printf("\n|               BOOK RETURN RECEIPT             |");
                        printf("\n|===============================================|");
                        printf("\n| Borrower Name :%-30s |", targetStudent);
                        printf("\n| Book ISBN     :%-30s |", targetISBN);
                        printf("\n| Book Title    :%-30s |", targetTitle);
                        printf("\n| Borrow Date   :%-30s |", targetBorrowDate);
                        printf("\n| Due Date      :%-30s |", targetDueDate);
                        printf("\n|                                               |");
                        printf("\n| Return Date   :%-30s |", returnDate);
                        printf("\n| Fine          :RM %-26.2f %1s", penalty, " |");
                        printf("\n|===============================================|\n\n");
                        printf("%s has returned the book \"%s\" late.\n", targetStudent, targetTitle);
                    }
                    else
                    {
                        // DISPLAY UPDATED BOOK RETURNED RECEIPT
                        printf("|===============================================|");
                        printf("\n|         UPDATED BOOK RETURN RECEIPT           |");
                        printf("\n|===============================================|");
                        printf("\n| Borrower Name:  %-30s|", targetStudent);
                        printf("\n| Book ISBN:      %-30s|", targetISBN);
                        printf("\n| Book Title:     %-30s|", targetTitle);
                        printf("\n| Borrow Date:    %-30s|", targetBorrowDate);
                        printf("\n| Due Date:       %-30s|", targetDueDate);
                        printf("\n|                                               |");
                        printf("\n| Return Date:    %-30s|", returnDate);
                        printf("\n| Fine:           %-30s|", "RM0.00");
                        printf("\n|===============================================|\n\n");

                        printf("%s has returned the book \"%s\" prior to the due date.\n", targetStudent, targetTitle);
                        printf("No fines will be incurred.\n\n");
                    }

                    // format the content with updated information and store it in temp array
                    sprintf(temp, "%s,%s,%s,%s,%s,%s\n",
                            targetISBN, targetStudent, targetTitle, targetBorrowDate, targetDueDate, returnDate);

                    returned = 1; // set returned to 1 if the book is returned successfully
                }
                strcat(newContent, temp); // concatenate temp (either updated or unchanged) to newContent, which was initialized to an empty string
            }
            fclose(datesFile); // close dates.txt file

            // WRITE MODIFIED CONTENTS TO DATES.TXT FILE
            datesFile = fopen("dates.txt", "w");
            fputs(newContent, datesFile); // write the new modified content to the books.txt file
            fclose(datesFile);

            // if the book is returned successfully, execute 'if' block, if not, execute 'else' block
            if (returned)
            {
                // UPDATE THE UPDATED VALUES FOR COPIES AVAILABLE AND BORROWED COPIES IN BOOKS.TXT FILE
                int newAvail = atoi(books[bookIndex].copiesAvailable) + 1;   // add 1 to available copies for the returned book because the book is returned
                sprintf(books[bookIndex].copiesAvailable, "%d", newAvail);   // format int to char
                int newBorrowed = atoi(books[bookIndex].borrowedCopies) - 1; // minus 1 from borrowed copies for the returned book because the book is returned
                sprintf(books[bookIndex].borrowedCopies, "%d", newBorrowed);

                // open books.txt file in read mode
                FILE *booksFile = fopen("books.txt", "r");
                if (booksFile == NULL)
                {
                    puts("books.txt could not be opened\n"); // display error message if books.txt cannot be opened
                }

                char temp2[LINE_SIZE];
                char newContent2[LINE_SIZE] = "";

                // read all the contents from books.txt file and store it in temp2 array
                while (fgets(temp2, sizeof(temp2), booksFile) != NULL)
                {
                    // extract ISBN from the line
                    char targetISBN[ISBN_SIZE];
                    sscanf(temp2, "%*s %*s %13s", targetISBN);

                    // compare ISBN to identify the line to modify
                    if (strcmp(targetISBN, ISBN) == 0)
                    {
                        // modify the line with the updated information
                        sprintf(temp2, "%s %s %s %s %s %s %s\n",
                                books[bookIndex].title, books[bookIndex].author, books[bookIndex].ISBN,
                                books[bookIndex].yearOfPublication, books[bookIndex].copiesAvailable,
                                books[bookIndex].totalCopies, books[bookIndex].borrowedCopies);
                    }
                    strcat(newContent2, temp2); // append the line to new content
                }
                fclose(booksFile); // close books.txt file

                // open books.txt file in write mode
                booksFile = fopen("books.txt", "w");
                fputs(newContent2, booksFile); // write the new modified content to the books.txt file
                fclose(booksFile);             // close books.txt file
                printf("BOOK RETURNED SUCCESSFULLY!!\n");
            }
            else
            {
                printf("\nNo matching entry found for return. Please double-check the information.\n");
            }
        }
    } while (bookIndex == -1);
}

// function to allow user to either choose to borrow or return a book
void borrowORreturn(Book books[], int totalBooks, Student students[], int totalStudents)
{
    int choice;

    do
    {
        // get and read input from the user on whether to borrow or return a book
        printf("\nChoose an option:\n");
        printf("1. Borrow a book\n");
        printf("2. Return a book\n");
        printf("Enter your choice (1 or 2): ");
        scanf("%d", &choice);

        // check if the choice is valid
        if (choice != 1 && choice != 2)
        {
            printf("\nInvalid choice. Please enter 1 or 2.\n");
        }
    } while (choice != 1 && choice != 2);

    // function call according to user input using a switch statement
    switch (choice)
    {
    case 1:
        borrowBook(books, totalBooks, students, totalStudents);
        break;
    case 2:
        returnBook(books, totalBooks, students, totalStudents);
        break;
    default:
        // This should not be reached, as the loop ensures a valid choice
        printf("\nInvalid choice. \n");
        break;
    }
}

// function to convert a date string to a struct tm object
// dateString: Input date string in the format "DD-MM-YYYY"
// Returns: a struct tm object representing the converted date
struct tm convertToDate(const char *dateString)
{
    // initialize the struct tm object to store the converted date
    struct tm tmDate = {0};

    // use sscanf to extract day, month, and year from the date string
    sscanf(dateString, "%d-%d-%d", &tmDate.tm_mday, &tmDate.tm_mon, &tmDate.tm_year);

    // adjust month and year values (date string is in the format DD-MM-YYYY)
    tmDate.tm_mon -= 1;     // the month in struct tm is in the range [0, 11], so decrement by 1
    tmDate.tm_year -= 1900; // the year in struct tm is the number of years since 1900, so subtract 1900

    return tmDate; // return the converted struct tm object
}

// function to generate monthly report
void generateMonthlyReport(struct Book books[], int totalBooks, struct Student students[], int totalStudents)
{
    // display the header for monhtly report
    printf("\n\n\t\t\t\t\t\t\t|=====================================================================|\n");
    printf("\t\t\t\t\t\t\t|                                                                     |\n");
    printf("\t\t\t\t\t\t\t|                            MONTHLY REPORT                           |\n");
    printf("\t\t\t\t\t\t\t|                                                                     |\n");
    printf("\t\t\t\t\t\t\t|=====================================================================|\n");

    int cont = 0; // variable to track whether the user wants to continue with another month

    do
    {
        char month[10];

        // prompt user to enter the month for which the report is to be generated
        do
        {
            printf("\n\nMonthly report is available from June until December 2023 ONLY\n");
            printf("Enter the complete name of the month with the first letter capitalized, for instance, (June, July, or August) :  ");
            scanf("%s", month);

            // validate the entered month to be within the allowed range
            if (strcmp(month, "June") != 0 && strcmp(month, "July") != 0 && strcmp(month, "August") != 0 &&
                strcmp(month, "September") != 0 && strcmp(month, "October") != 0 && strcmp(month, "November") != 0 &&
                strcmp(month, "December") != 0)
            {
                printf("Invalid Input. Please try again.\n");
            }
        } while (strcmp(month, "June") != 0 && strcmp(month, "July") != 0 && strcmp(month, "August") != 0 &&
                 strcmp(month, "September") != 0 && strcmp(month, "October") != 0 && strcmp(month, "November") != 0 &&
                 strcmp(month, "December") != 0); // continue the loop if the entered month name is invalid

        // initialize variables to count various activities for the monthly report
        int borrowedCount = 0;
        int returnedCount = 0;
        int toBeReturnedCount = 0;
        double totalFines = 0.0;

        // open dates.txt file for reading
        FILE *datesFile = fopen("dates.txt", "r");
        if (datesFile == NULL)
        {
            puts("dates.txt file could not be opened.");
            return;
        }

        // read each line from dates.txt and analyze borrowing and return activities
        char line[LINE_SIZE];
        while (fgets(line, sizeof(line), datesFile) != NULL)
        {
            line[strcspn(line, "\n")] = '\0';

            // extract borrow date, return date, and due date from the line
            char borrowDate[DATE_SIZE], returnDate[DATE_SIZE], dueDate[DATE_SIZE];
            sscanf(line, "%*[^,],%*[^,],%*[^,],%[^,],%[^,],%s", borrowDate, dueDate, returnDate);

            // convert borrow date to month and check if it matches the specified month
            struct tm tmBorrowDate = convertToDate(borrowDate);
            char borrowMonth[10];
            strftime(borrowMonth, sizeof(borrowMonth), "%B", &tmBorrowDate);

            if (strcmp(borrowMonth, month) == 0)
            {
                borrowedCount++;
            }

            // check if there is a return date, convert it to month, and check if it matches the specified month
            if (strlen(returnDate) > 0)
            {
                struct tm tmReturnDate = convertToDate(returnDate);
                char returnMonth[10];
                strftime(returnMonth, sizeof(returnMonth), "%B", &tmReturnDate);

                if (strcmp(returnMonth, month) == 0)
                {
                    returnedCount++;
                    totalFines += calculatePenalty(dueDate, returnDate);
                }
            }

            // convert due date to month and check if it matches the specified month
            struct tm tmDueDate = convertToDate(dueDate);
            char dueMonth[10];
            strftime(dueMonth, sizeof(dueMonth), "%B", &tmDueDate);
            if (strstr(line, "(null)") != NULL && strcmp(dueMonth, month) == 0)
            {
                toBeReturnedCount++;
            }
        }

        // close dates.txt file
        fclose(datesFile);

        // display the monthly report
        printf("\n\n\t\t\t\t\t\t\t\t\tMonthly Report for %s 2023\n", month);
        printf("\t\t\t\t\t\t\t\t|----------------------------------------------------|\n");
        printf("\t\t\t\t\t\t\t\t| %-31s | %-16s |\n", "Category", "Count");
        printf("\t\t\t\t\t\t\t\t|---------------------------------|------------------|\n");
        printf("\t\t\t\t\t\t\t\t| Total Books Borrowed            | %-16d |\n", borrowedCount);
        printf("\t\t\t\t\t\t\t\t| Total Books Returned            | %-16d |\n", returnedCount);
        printf("\t\t\t\t\t\t\t\t| Total Books to be Returned      | %-16d |\n", toBeReturnedCount);
        printf("\t\t\t\t\t\t\t\t|---------------------------------|------------------|\n");
        printf("\t\t\t\t\t\t\t\t| Total Fines Collected           | RM %-13.2f |\n", totalFines);
        printf("\t\t\t\t\t\t\t\t|----------------------------------------------------|\n");

        // prompt the user if they want to input another month
        printf("\nDo you want to generate a report for another month? (1 for Yes / 0 for No): ");
        scanf("%d", &cont);
    } while (cont == 1);
}
