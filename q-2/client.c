#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv)
{
    int a = atoi(argv[1]);
    a = (a % 11);
    printf("%d\n", a);
}