#include <stdio.h>
#include <stdlib.h>

//implementation of queue data structure
#define SIZE 5
int front = -1;
int rear = -1;
int queue[SIZE];

void add(int data)
{
    if (rear >= SIZE - 1)
    {
        printf("No more data can be added\n");
    }
    else
    {
        queue[++rear] = data;
    }

    if (front == -1)
    {
        front = 0;
    }
}

int delete ()
{
    if (front > rear || front < 0)
    {
        printf("nothing to remove\n");
    }
    else
    {
        printf("Removed number is %d\n", queue[front++]);

    }
}

int main()
{
    add(2);
    add(5);
    add(5);
    add(5);
    add(2);
    add(5);
    add(5);
    add(5);

    delete ();
    delete ();
    delete ();
    delete ();
    delete ();
    delete ();

}