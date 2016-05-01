#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

#include "poker.h"

int main() {
    clock_t start, end;

    start = clock();
    init_rank();
    end = clock();

    cout << "Init took " << (double)(end-start)/CLOCKS_PER_SEC << " seconds" << endl;

    vector<int> deck = make_deck();

    vector<int> cnt(10);
    start = clock();
    int N = 1000000;
    for(int i = 0; i < N; i++) {
        shuffle_deck(deck, 5);
        cnt[rank_hand(deck)>>24]++;
    }
    end = clock();
    cout << (double)(end-start)/CLOCKS_PER_SEC << " seconds" << endl;

    for(int i = 0; i < 10; i++) {
        cout << 100.0*cnt[i]/N << endl;
    }

    return 0;
}
