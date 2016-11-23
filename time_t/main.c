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

    return 0;

}
