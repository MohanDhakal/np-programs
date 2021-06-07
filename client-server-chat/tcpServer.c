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

typedef struct sockaddr SA;
#define LISTENQ 10
#define MAXLINE 1024

int main(int argc, char **argv)
{
    char buff[MAXLINE];
    char msgToSend[MAXLINE];

    //making a socket end point
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (sockfd <= 0)
    {
        printf("error creating socket endpoint");
    }

    struct sockaddr_in servaddr, cliaddr;

    bzero(&servaddr, sizeof(servaddr));

    //socket address structure configuration
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));

    //binding the socket address
    if (bind(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("error binding address");
        exit(0);
    }

    //making a listenting socket that stores backlogs
    printf("Server is waiting connection at port %d\t\n", atoi(argv[2]));

    if (listen(sockfd, LISTENQ) < 0)
    {
        printf("error listeting to provided socket descriptor");
    }

    socklen_t len = sizeof(cliaddr);

    //creates a connected socket or accept and incoming connection

    int connfd = accept(sockfd, (SA *)&cliaddr, &len);

    if (connfd == -1)
    {
        printf("error creating a connected socket");
    }
    while (1)
    {
        if (read(connfd, buff, sizeof(buff)) < 0)
        {
            printf("read error");
        }
        printf("client message:\n");
        fputs(buff, stdout);

        int result = strcmp(buff, "quit\n");

        if (result == 0)
        {
            printf("exiting...");
            exit(0);
        }

        printf("Enter Server Response:\n");
        fgets(msgToSend, MAXLINE, stdin);

        //sending data to clients
        if (write(connfd, msgToSend, MAXLINE) < 0)
        {
            printf("write error");
        }
    }
}