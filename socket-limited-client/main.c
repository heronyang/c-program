#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>

// Socket
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

// Thread
#include <pthread.h>

#define PORT 8000
#define LISTEN_BACKLOG 1024

/******************** Socket ********************/

typedef struct sockaddr SA;

/*
 * Listen on port, return the socket if succeed
 */
int listen_on(int port) {

    int listenfd, optval = -1;
    struct sockaddr_in server_addr;

    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket construct");
        return -1;
    }

    // avoid address in use error
    if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
                (const void *)&optval, sizeof(int)) < 0) {
        perror("setsocketopt");
        return -1;
    }

    size_t server_addr_len = sizeof(server_addr);
    bzero((char *) &server_addr, server_addr_len);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons((unsigned short)port);

    if(bind(listenfd, (SA *)&server_addr, server_addr_len) < 0) {
        perror("socket bind");
        return -1;
    }

    if(listen(listenfd, LISTEN_BACKLOG) < 0) {
        perror("socket listen");
        return -1;
    }

    return listenfd;

}

/*
 * Wrapper for listen_on
 */
int listen_on_w(int port) {
    int r;
    if((r = listen_on(port)) < 0) {
        exit(EXIT_FAILURE);
    }
    return r;
}

/*
 * Wrapper for accept
 */
int accept_w(int s, struct sockaddr *addr, socklen_t *addr_len) {
    int r;
    if((r = accept(s, addr, addr_len)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    return r;
}

/******************** Thread ********************/

/*
 * Wrapper for pthread_create
 */
int pthread_create_w(pthread_t *thread, const pthread_attr_t *attr,
            void *(*start_routine)(void*), void *arg) {
    int r;
    if((r = pthread_create(thread, attr, start_routine, arg)) != 0) {
        perror("pthread create");
        exit(EXIT_FAILURE);
    }
    return r;
}

/*
 * Wrapper for pthread_detach
 */
int pthread_detach_w(pthread_t thread) {
    int r;
    if((r = pthread_detach(thread)) != 0) {
        perror("pthread detach");
        exit(EXIT_FAILURE);
    }
    return r;
}

/******************** Other ********************/

/*
 * Wrapper for malloc
 */
void *malloc_w(size_t size) {
    void *p;
    if((p = malloc(size)) == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    return p;
}

/******************** Worker ********************/

/*
 * Entry function run by each worker
 */
void *worker(void *connfd_p) {
    int connfd = *((int *) connfd_p);
    pthread_detach_w(pthread_self());   // no pthread_join
    free(connfd_p);

    printf("one connection pinged and left, connfd = %d\n", connfd);

    close(connfd);
    return NULL;
}

/******************** Main ********************/

int main(int argc, char **argv) {

    int listenfd, *connfd_p;

    struct sockaddr_storage client_addr;
    socklen_t client_addr_len;

    pthread_t thread;

    listenfd = listen_on_w(PORT);
    while(true) {
        client_addr_len = sizeof(client_addr);
        connfd_p = malloc_w(sizeof(int));
        *connfd_p = accept_w(listenfd,
                (SA *) &client_addr,
                &client_addr_len);
        pthread_create_w(&thread, NULL, worker, connfd_p);
    }

    return EXIT_SUCCESS;

}
