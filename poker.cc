#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define MAKE_CARD(v, s) (((s)<<4) + (v))
#define CV(c) ((c)&15)
#define CS(c) ((c)>>4)

int next_values(uint8_t *hand, int len) {
    int i;
    for(i=0; i<len && ++hand[i] >= 15;  i++)
        hand[i] = hand[i+1]+1;
    return i!=len;
}

int main() {
    uint8_t hand[6] = { 2, 2, 2, 2, 2, 14 };
    int i, j, c, N=5;

    do {
        for(i=0; i<N; i++)
            printf("%d ", hand[i]);
        printf("\n");
    } while(next_values(hand, N));

    return 0;
}
