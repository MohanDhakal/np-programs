

//implementation of stack

#include <stdio.h>
#include <stdlib.h>

#define SIZE 10

int stack[SIZE];

int top = -1;

void push(int data)
{
    if (top < SIZE)
    {
        stack[++top] = data;
    }
    else
    {
        printf("Stack Overflow");
    }
}

int pop()
{
    int data;
    if (top >= 0)
    {
        data = stack[top];
        top--;
        return data;
    }
    else
    {
        printf("stack underflow:\t");
    }
    return 0;
}
int main()
{
    push(12);
    push(22);
    push(42);
    push(52);
    push(52);
    push(52);
    push(52);
    push(12);
    printf("%d\n", pop());
    printf("%d\n", pop());
    printf("%d\n", pop());
    printf("%d\n", pop());
    printf("%d\n", pop());
}
