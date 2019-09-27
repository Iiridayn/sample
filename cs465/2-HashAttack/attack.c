#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include <bsd/stdlib.h>
#include <openssl/sha.h>

#define SHA1SIZE 20
#define DUMP(STATE, SIZE) do { \
    for (int i = 0; i < SIZE; i++) \
        printf("%02x", (STATE)[i]); \
} while (0)

//#define DEBUG_FIND_MATCH
uint32_t find_match(uint8_t *out, uint8_t *outputs, uint8_t size, uint32_t max)
{
    for (int i = 0; i < max; i++) {
#ifdef DEBUG_FIND_MATCH
        printf("%d comparing ", i);
        DUMP(out, size);
        printf(" with ");
        DUMP(&outputs[i * size], size);
        printf("\n");
#endif

        if (0 == memcmp(out, &outputs[i * size], size)) {
#ifdef DEBUG_FIND_MATCH
            printf("matched\n");
#endif
            return i;
        }
    }
#ifdef DEBUG_FIND_MATCH
    printf("no match\n");
#endif
    return UINT32_MAX;
}

#define USAGE() do { \
    printf("USAGE: %s <nibbles to compare> <c|p> (<samples>=50)\n", argv[0]); \
    return 1; \
} while(1)

int main(int argc, char *argv[])
{
    uint8_t check[SHA1SIZE] = {0};

    if (argc < 3 || strlen(argv[2]) > 1)
        USAGE();

    uint8_t nibbles;
    sscanf(argv[1], "%hhu", &nibbles);

    bool collision = false;
    if (argv[2][0] == 'c')
        collision = true;
    else if (argv[2][0] != 'p')
        USAGE();

    uint32_t samples = 50;
    if (argc > 3)
        sscanf(argv[3], "%u", &samples);

    uint8_t guess[SHA1SIZE]; // sizeof sha1 output; don't reduce output space
    uint8_t out[SHA1SIZE];

    uint8_t mask = 0xFF;
    uint8_t size = nibbles >> 1;
    if (nibbles % 2) {
        mask = 0xF0;
        size++;
    }
    uint8_t final = size - 1;

    uint8_t *outputs;
    uint8_t *inputs;
    uint32_t max = 1 << (nibbles * 4); // _expect_ to use 2^(n/2) checks; _permit_ 2^n checks
    if (collision) {
        outputs = malloc(size * max);
        inputs = malloc(SHA1SIZE * max);
    }

    uint32_t idx;
    for (int i = 1; i <= samples; i++) {
        for (int j = 1;; j++) {
            arc4random_buf(guess, sizeof (guess));
            SHA1(guess, sizeof (guess), out);
            out[final] &= mask;

            bool match;
            if (collision) {
                if (j >= max) {
                    printf("Buffer storage exceeded!\n");
                    return -1;
                }

                idx = UINT32_MAX;
                if (j > 1)
                    idx = find_match(out, outputs, size, j - 1);

                match = idx != UINT32_MAX;
                if (!match) {
                    memcpy(&inputs[(j - 1) * SHA1SIZE], guess, SHA1SIZE);
                    memcpy(&outputs[(j - 1) * size], out, size);
                }
            } else {
                match = 0 == memcmp(check, out, size);
            }

            if (match) {
                printf("%d: %d ", i, j);
                DUMP(guess, SHA1SIZE);
                printf(" ");
                if (collision) {
                    DUMP(&inputs[idx * SHA1SIZE], SHA1SIZE);
                    printf(" ");
                    DUMP(out, size);
                } else {
                    printf("%0*d", nibbles, 0);
                }
                printf("\n");
                break;
            }
        }
    }

    if (collision) {
        free(outputs);
        free(inputs);
    }
}
