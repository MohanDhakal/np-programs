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

// a simple program to  handle multiple clients
//using select statement
//cmd line arg format :
// {{./server -p pornumber}}

typedef struct sockaddr SA;
#define LISTENQ 10
#define MAXLINE 1024
#define NAMELENGTH 20
int clients_count = 0;

typedef struct
{
    char name[NAMELENGTH];
    int client_fd;

} CLIENTS;
CLIENTS users[10];

void remove_client(int clientposition)
{
    //if the client is the last client in the user list
    if (clients_count == 1 || clientposition == clients_count - 1)
    {
        bzero(users[clientposition].name, NAMELENGTH);
        sprintf(users[clientposition].name, "");
    }
    else
    {
        //if the client posititon is in between the array not the last one
        //make the client detail empty in the position and shift every detail
        //one step back
        for (int i = clientposition; i < clients_count - 1; i++)
        {
            bzero(users[i].name, NAMELENGTH);
            users[i].client_fd = users[i + 1].client_fd;
            strcpy(users[i].name, users[i + 1].name);
        }
        bzero(users[clients_count - 1].name, 10);
        sprintf(users[clients_count - 1].name, "");
    }
    clients_count = clients_count - 1;
}
void addUser(int connfd, char *name)
{
    users[clients_count].client_fd = connfd;
    strcpy(users[clients_count].name, name);
    clients_count = clients_count + 1;
    printf("welcome to group chat %s\n", name);
}
void send_msg_to_all_except_one(char *message, int connfd)

{
    for (int i = 0; i < clients_count; i++)
    {
        if (users[i].client_fd != connfd)
        {
            int n = send(users[i].client_fd, message, strlen(message),0);
            if (n < 0)
            {
                printf("error sending message to all except the requester");
            }
        }
    }
}
int main(int argc, char **argv)
{

    char data_buffer[MAXLINE];
    //file descriptor sets
    fd_set reads, sockets_in_use;
    //internet specific socket address structure
    struct sockaddr_in servaddr, cliaddr;

    //chekcing for valid command line arguments
    if (argc != 3)
    {
        perror("Usage: ./server -p <port>");
        exit(0);
    }

    //creating a master socket endpoint
    int listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (listenfd <= 0)
    {
        printf("error creating socket endpoint");
    }

    bzero(&servaddr, sizeof(servaddr));

    //socket address structure configuration
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));

    if (bind(listenfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("error binding address");
        exit(0);
    }
    printf("Server is waiting connection at port %d\t\n", atoi(argv[2]));

    if (listen(listenfd, LISTENQ) < 0)
    {
        printf("error listeting to provided socket descriptor");
    }

    socklen_t len = sizeof(cliaddr);

    //setting active sockets to 0 initially

    FD_ZERO(&sockets_in_use);

    //currently listenfd socket is active so add it to sockets_in_use fd_set
    FD_SET(listenfd, &sockets_in_use);

    while (1)
    {
        reads = sockets_in_use;
        int s = select(FD_SETSIZE, &reads, 0, 0, NULL);
        if (s < 0)
        {
            printf("select error");
            exit(0);
        }

        else
        {
            //listenfd is ready if something happened inthe master socket
            if (FD_ISSET(listenfd, &reads))
            {
                //read io condition is ready, accpet the inbound connection
                int connfd = accept(listenfd, (SA *)&cliaddr, &len);
                if (connfd > 0)
                {
                    bzero(data_buffer, MAXLINE);
                    if (read(connfd, data_buffer, MAXLINE) <= 0)
                    {
                        printf("read error!!");
                    }
                }
                else
                {
                    printf("accept error!!");
                }

                //adding new connection to the active connection set
                FD_SET(connfd, &sockets_in_use);
                //adding users in the client list
                addUser(connfd, data_buffer);
                //printing in the server that the client in the connection
                //has joined the quiz
                sprintf(data_buffer, "%s joined the chat.\n", data_buffer);
                send_msg_to_all_except_one(data_buffer, connfd);
                // bzero(data_buffer, MAXLINE);
                // sprintf(data_buffer, "0Welcome to Quiz!!\n%s", active_ques);
                // send(connfd, rbuffer, strlen(rbuffer), 0);
            }
            else
            {
                //if the connection is of existing user connection

                int client_counter = 0;

                while (client_counter < clients_count)
                {
                    //if the particular client fd in active in reads fd
                    if (FD_ISSET(users[client_counter].client_fd, &reads))
                    {
                        bzero(data_buffer, MAXLINE);

                        int n = recv(users[client_counter].client_fd, data_buffer, MAXLINE, 0);

                        if (n <= 0)
                        {
                            printf("%s left the Chat.\n", users[client_counter].name);
                            //removing the user from sockets in use
                            FD_CLR(users[client_counter].client_fd, &sockets_in_use);
                            remove_client(client_counter);

                            client_counter = client_counter - 1;
                        }
                        else
                        {
                            printf("Received Message from %s \n", users[client_counter].name);
                            puts(data_buffer);
                        }
                    }
                    client_counter = client_counter + 1; // increase number of checked client.
                }
            }
        }
    }
}