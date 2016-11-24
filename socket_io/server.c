#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>

#define SOCKET_BACKLOG  3
#define PORT            8000
#define BUFFER_SIZE     20
#define HEADER_SIZE     4

void print_buffer(int len, char *buffer) {

    int i;

    printf("> %d,\t", len);
    for(i=0; i<len; i++) {
        printf("[%02x]  ", buffer[i]);
    }

    printf("\n");

}

/*
 * First 4 bytes of the data indicates the size
 */
unsigned read_size(int clientfd) {

    char buffer[HEADER_SIZE];
    int n_read = recv(clientfd, buffer, HEADER_SIZE, 0);

    if(n_read != HEADER_SIZE) {
        perror("Error in read");
        return -1;
    }

    int size = 0, i;
    for(i=0; i<4; i++) {
        size += buffer[i];
        if(i != 3) {
            size <<= 8;
        }
        printf("%d, %02x, size = %x\n", i, buffer[i], size);
    }

    return size;
}

/*
 * Read following content with specific size, if failed, return NULL
 */
char *read_content(int connfd, unsigned size) {

    int n_read;
    char buffer[BUFFER_SIZE];
    char *content = malloc(size);
    char *iterator = content;

    while(true) {

        n_read = recv(connfd, buffer, BUFFER_SIZE, 0);

        if(n_read == -1) {
            perror("Error in read");
            return NULL;
        } else if(n_read == 0) {    // EOF
            break;
        }

        memcpy(iterator, buffer, n_read);

        print_buffer(n_read, buffer);
        iterator += n_read;

    }

    if(n_read != 0) {
        perror("Recv failed\n");
        free(content);
        return NULL;
    }

    return content;

}

int main(int argc, char *argv[]) {

    struct sockaddr_in server, client;

    // create
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if(socketfd == -1) {
        perror("Error found in creating a new socket\n");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // bind
    if(bind(socketfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Bind failed");
        return 1;
    }

    // listen
    listen(socketfd, SOCKET_BACKLOG);
    printf("Listing on port %d...\n", PORT);

    // accept
    int sa_size = sizeof(struct sockaddr_in);
    int clientfd = accept(socketfd, (struct sockaddr *)&client, (socklen_t*)&sa_size);
    if(clientfd < 0) {
        perror("Accept failed");
        return 1;
    }
    printf("Client connected\n");

    // recv / write
    // > get size
    unsigned size = read_size(clientfd);
    printf("Size = %x (%d)\n", size, size);

    // > get content
    char *content = read_content(clientfd, size);
    printf("Content = %.*s\n", size, content);

    // close
    close(clientfd);
    close(socketfd);

    return 0;

}
