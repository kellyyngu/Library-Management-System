#include <stdio.h>

int sum(int *myInt);
int factorial(int n);

int main()
{
    int myInt[3];
    int total = 0;
    int factorialResult;

    printf("Enter 3 integers: \n");
    scanf("%d%d%d", &myInt[0], &myInt[1], &myInt[2]);

    total = sum(&myInt);
    printf("Sum is : %d\n", total);
    factorialResult = factorial(total);
    printf("Factorial result is : %d\n", factorialResult);
}

int sum(int *myInt)
{
    int total = 0;

    while (*myInt != '\0')
    {
        total = total + *myInt;
        ++myInt;
    }

    return total;
}

int factorial(int n)
{
    if (n == 0 || n == 1)
    {
        return 1;
    }
    else
    {
        return n * factorial(n - 1);
    }
}
