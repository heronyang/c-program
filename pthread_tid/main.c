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

    // print directly
    setbuf(stdout, NULL);

    // init
    int i, n = 5;
    pthread_t *tids = malloc(n * sizeof(pthread_t));

    // run
    for(i=0; i<n; i++) {

        Thread_Args *wa = malloc(sizeof(Thread_Args));
        wa->tid = i;

        pthread_create(&tids[i], NULL, run_thread, (void *)wa);

    }

    // done
    for(i=0; i<n; i++) {
        pthread_join(tids[i], NULL);
        printf("Thread %d: Done\n", i);
    }

    return 0;

}
