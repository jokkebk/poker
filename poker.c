#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int next_values(uint8_t *hand, int len) {
    int i;
    for(i=0; i<len && ++hand[i] >= 15;  i++)
        hand[i] = hand[i+1]+1;
    return i!=len;
}

int main() {
    uint8_t hand[6] = { 2, 2, 2, 2, 2, 14 };
    int i, j, c, N=3;

    for(i=0; i<0xF0000; i++) {
        c = 0;
        for(j=0; j<5*4; j+=4) {
            if(((i>>j)&15)<2 || ((i>>j)&15)>14) {
                c = 1;
                break;
            }
            if(((i>>j)&15) < ((i>>(j+4))&15)) {
                c = 1;
                break;
            }
        }
        if(c) continue;
        //printf("%05X\n", i);
    }
    return 0;
    do {
        for(i=0; i<N; i++)
            printf("%d ", hand[i]);
        printf("\n");
    } while(next_values(hand, N));

    return 0;
}
