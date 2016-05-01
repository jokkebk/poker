#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

#include "poker.h"

pair<int,int> simulate(vector<int> & deck, int dealt, int opps, int N) {
    int win = 0, tie = 0;

    for(int i=0; i<N; i++) {
        // deal remaining cards for table and opponents
        shuffle_deck(deck, 5-dealt+opps*2, 2+dealt);

        int myrank = rank_hand7(deck), opprank = 0;
        for(int opp=0; opp<opps; opp++) {
            opprank = max(opprank, rank_hand7(deck[2], deck[3],
                        deck[4], deck[5], deck[6],
                        deck[7+opp*2], deck[8+opp*2]));
            if(opprank > myrank) break;
        }

        if(myrank > opprank) win++;
        else if(myrank == opprank) tie++;
    }

    return make_pair(win, tie);
}

int main(int argc, char *argv[]) {
    int N=1000000;

    int opps = (argc > 1) ? argv[1][0]-'0' : 5;

    init_rank();

    while(true) {
        vector<int> deck = make_deck();

        cout << "Running simulations with " << opps << " opponents" << endl;
        cout << "Input first 2 player cards (e.g. 'kh 9s'), then community cards: ";

        for(int i=0; i<7; i++) {
            string vs;
            cin >> vs;
            iter_swap(deck.begin()+i, find(deck.begin(), deck.end(), make_card(vs)));

            // don't simulate on 1st card and community cards 1-2
            if(i<4 && i!=1) continue;

            cout << "Simulating " << N << " times..." << endl;
            pair<int,int> wintie = simulate(deck, i-1, opps, N);
            cout << "Win rate " << (100.0*wintie.first/N) << " %, tie " << (100.0*wintie.second/N) << " %" << endl;
        }
    }

    return 0;
}
