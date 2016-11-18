#include <stdio.h>
#include "wrapper.h"

sem_t sem;

/******************** Worker ********************/

/*
 * Dummy, print current time
 * Reference: http://stackoverflow.com/questions/5141960/get-the-current-time-in-c
 */
void print_current_time() {

    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    printf ( "Current local time and date: %s", asctime (timeinfo) );

}

/*
 * Dummy, hold time
 */
void limited_worker_can_come_in() {
    sleep(3);
    print_current_time();
}

/*
 * Entry function run by each worker
 */
void *worker(void *connfd_p) {

    // parse parameters and release resources
    int connfd = *((int *) connfd_p);
    pthread_detach_w(pthread_self());   // no pthread_join
    free(connfd_p);

    // run job
    sem_wait_w(&sem);
    limited_worker_can_come_in();
    sem_post_w(&sem);

    // close
    close(connfd);
    return NULL;
}

/******************** Main ********************/

void init() {
    sem_init_w(&sem, 0, MAX_CONNECTION);
}

void deinit() {
    sem_destroy(&sem);
}

int main(int argc, char **argv) {

    int listenfd, *connfd_p;
    struct sockaddr_storage client_addr;
    socklen_t client_addr_len;
    pthread_t thread;

    init();

    listenfd = listen_on_w(PORT);
    while(true) {
        client_addr_len = sizeof(client_addr);
        connfd_p = malloc_w(sizeof(int));
        *connfd_p = accept_w(listenfd,
                (SA *) &client_addr,
                &client_addr_len);
        pthread_create_w(&thread, NULL, worker, connfd_p);
    }

    deinit();
    return EXIT_SUCCESS;

}
