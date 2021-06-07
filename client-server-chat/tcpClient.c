#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <sys/select.h>

#define MAXLINE 1024
typedef struct sockaddr SA;

int main(int argc, char **argv)
{
    char buff[MAXLINE];
    //creating socket endpoint
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (sockfd < 0)
    {
        printf("sockted endpoint error");
    }
    struct sockaddr_in servaddr;

    int port = atoi(argv[4]);

    //filling in socket address structure with the data
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);

    if ((inet_pton(AF_INET, argv[2], &servaddr.sin_addr) <= 0))
    {
        printf("error converting IP form p to n");
    }

    //connecting to the server
    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("error connecting to server");
    }
    printf("Write Your Message:\n ");

    bzero(buff, MAXLINE);
    fgets(buff, MAXLINE, stdin);
    write(sockfd, buff, sizeof(buff));
    bzero(buff, MAXLINE);

    while ((read(sockfd, buff, MAXLINE)) > 0)
    {

        printf("Server Message\n ");

        if (fputs(buff, stdout) == EOF)
            printf("fputs error");

        printf("Write Your Message:\n ");

        bzero(buff, MAXLINE);
        fgets(buff, MAXLINE, stdin);
        write(sockfd, buff, sizeof(buff));

        int result = strcmp(buff, "quit\n");

        if (result == 0)
        {
            printf("exiting...");
            exit(0);
        }

        bzero(buff, MAXLINE);
    }
}