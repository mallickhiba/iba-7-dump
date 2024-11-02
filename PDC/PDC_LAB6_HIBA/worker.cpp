#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#define SUCCESS 0
#define ERROR 1
#define END_LINE 0x0
#define SERVER_PORT 1500
#define MAX_MSG 100
int sum(int *a);

void *handle_client(void *arg)
{
    int newSd = *reinterpret_cast<int *>(arg);
    int rcv_msg[MAX_MSG];
    struct sockaddr_in cliAddr;
    int cliLen = sizeof(cliAddr);

    getpeername(newSd, reinterpret_cast<struct sockaddr *>(&cliAddr), reinterpret_cast<socklen_t *>(&cliLen));

    int count = 0;
    int x;
    recv(newSd, &rcv_msg, MAX_MSG, 0);
    for (int i = 0; i < 10; i++)
    {
        std::cout << "received from " << inet_ntoa(cliAddr.sin_addr) << ":TCP" << ntohs(cliAddr.sin_port) << " : " << rcv_msg[i] << std::endl;
    }

    x = sum(rcv_msg);
    int arr1[1];
    arr1[0] = x;
    std::cout << arr1[0];
    send(newSd, &arr1, sizeof(arr1), 0);
    close(newSd);
    pthread_exit(NULL);
}
int sum(int *a)
{
    int product = 0;
    int vector_b[5];
    for (int i = 0; i < 5; i++)
    {
        vector_b[i] = a[i + 5];
    }
    for (int i = 0; i < 5; i++)
    {
        product = product + a[i] * vector_b[i];
    }
    return product;

int main(int argc, char *argv[]){
    int sd, newSd, cliLen;
    struct sockaddr_in cliAddr, servAddr;
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0)
    {
        perror("cannot open socket ");
        return ERROR;
    }
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(SERVER_PORT);
    if (bind(sd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
        perror("cannot bind port ");
        return ERROR;
    }
    listen(sd, 5);
    while (1)
    {
        std::cout << argv[0] << ": waiting for data on port TCP " << SERVER_PORT << std::endl;
        cliLen = sizeof(cliAddr);
        newSd = accept(sd, (struct sockaddr *)&cliAddr, reinterpret_cast<socklen_t *>(&cliLen));
        if (newSd < 0)
        {
            perror("cannot accept connection ");
            return ERROR;
        }

        handle_client(reinterpret_cast<void *>(&newSd));
    }
    close(sd);
    close(newSd);
}
}
