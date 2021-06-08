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
    //creating file descriptorsets
    fd_set reads, scokets_in_use;
    struct sockaddr_in servaddr;
    char *username;
    char dataBuffer[MAXLINE];

    if (argc != 7)
    {
        perror("Usage: ./client -h <server_address> -p <port> -u <username>\n");
        exit(0);
    }
    if (strlen(argv[6]) > 20)
    {
        perror("Please use username less than 10 characters long.\n");
        exit(1);
    }
    int listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    //making the socket address structure ready

    int port = atoi(argv[4]); // ascii to integer conversion

    bzero(&servaddr, sizeof(servaddr)); // fills servaddr with zeros.

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);

    if (inet_pton(AF_INET, argv[2], &servaddr.sin_addr) <= 0)
    {
        // pton = presentatin to network, convert command line argument like 204..... to correct.
        printf("inet_pton error for %s", argv[2]);
    }

    if (connect(listenfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("error connecting to server");
    }
    else
    {
        username = argv[6];
        int n = send(listenfd, username, strlen(username), 0);
        if (n < 0)
        {
            perror("Cannot send username.\n");
            exit(0);
        }
    }
    //add only active socket(lisfenfd) to the fd set
    FD_ZERO(&scokets_in_use);
    //for the input from socket
    FD_SET(listenfd, &scokets_in_use);
    //for the input from keyboard
    FD_SET(0, &scokets_in_use);

    while (1)
    {
        reads = scokets_in_use;
        int s = select(FD_SETSIZE, &reads, 0, 0, NULL);
        if (s < 0)
        {
            printf("select error");
            exit(0);
        }

        //if the descriptor is ready to be read from keyboard

        if (FD_ISSET(0, &reads))
        {
            printf("\nEnter Your Message\n");

            bzero(dataBuffer, MAXLINE);
            fgets(dataBuffer, MAXLINE, stdin);

            //send the data to server
            send(listenfd, dataBuffer, strlen(dataBuffer), 0);

            if (strncmp(dataBuffer, "quit", 4) == 0)
            {
                printf("Quitting....\n");
                exit(1);
            }
        }

        // descriptor is ready for reading something from socket
        else if (FD_ISSET(listenfd, &reads))
        {
            bzero(dataBuffer, MAXLINE);
            int n = recv(listenfd, dataBuffer, MAXLINE, 0);
            dataBuffer[n] = 0;

            if (n <= 0)
            {
                perror("Error reading.");
                exit(0);
            }
            printf("Message from Server:%s\n", dataBuffer);
        }
    }
}