#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main() {
    
    unsigned u = 2123124124;    // 7E8C4D9C
    uint8_t *s = malloc(4);

    printf("size of char: %zu, size of unsigned: %zu\n", sizeof(char), sizeof(unsigned));

    printf("1:%02x\n", (u >> 24) & 0xFF);
    printf("2:%02x\n", (u >> 16) & 0xFF);
    printf("3:%02x\n", (u >> 8) & 0xFF);
    printf("4:%02x\n", (u >> 0) & 0xFF);

    *(s + 0) = (u >> 24) & 0xFF;
    *(s + 1) = (u >> 16) & 0xFF;
    *(s + 2) = (u >> 8) & 0xFF;
    *(s + 3) = (u >> 0) & 0xFF;

    printf("1:%02x 2:%02x 3:%02x 4:%02x\n", s[0], s[1], s[2], s[3]);

    return 0;
}
