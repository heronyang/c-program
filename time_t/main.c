#include <time.h>
#include <stdio.h>

int main() {

    time_t now;
    time(&now);

    int int_now = now;
    long long_now = now;
    printf("time = %ld\nint_now = %d\nlong_now = %ld\n",
            now, int_now, long_now);

    printf("size of time_t = %zu, int = %zu, long = %zu\n",
            sizeof(time_t), sizeof(int), sizeof(long));


    char buff[20];
    strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
    printf("Time (time_t): %s\n", buff);

    // ERROR while compiling
    //strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&int_now));
    //printf("Time (int): %s\n", buff);

    strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&long_now));
    printf("Time (long): %s\n", buff);

    return 0;

}
