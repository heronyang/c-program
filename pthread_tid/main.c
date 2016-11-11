#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

typedef struct {
    int tid;
} Thread_Args;

void *run_thread(void *args) {
    Thread_Args *as = args;
    printf("Hi, I'm thread. My tid is: %u\n", as->tid);
    return NULL;
}

int main(int argc, char *argv[]) {

    // don't buffer the stdout
    setbuf(stdout, NULL);

    // init
    int i, n = 5;
    pthread_t *tids = malloc(n * sizeof(pthread_t));

    // run
    for(i=0; i<n; i++) {

        // it's important to create args for each thread instead of sharing
        Thread_Args *wa = malloc(sizeof(Thread_Args));
        wa->tid = i;

        pthread_create(&tids[i], NULL, run_thread, (void *)wa);

    }

    // collect result
    for(i=0; i<n; i++) {
        pthread_join(tids[i], NULL);
        printf("Thread %d: Done\n", i);
    }

    return 0;

}
