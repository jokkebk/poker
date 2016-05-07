#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>

using namespace std;

#include "poker.h"

int main(int argc, char *argv[]) {
    int N=1000000, cards;

    if(argc < 4) {
        cout << "Usage: hand <opponent_num> <mycard1> <mycard2> [<common card1> ...]" << endl;
        cout << "Give cards in 'kh 9s tc' type of 2-character format" << endl;
        return 0;
    }

    int opps = argv[1][0]-'0';

    init_rank();

    vector<int> deck = make_deck();

    for(cards=0; cards+2 < argc; cards++)
        iter_swap(deck.begin()+cards, find(deck.begin(), deck.end(), make_card(argv[cards+2])));

    pair<int,int> wintie = simulate(deck, cards-2, opps, N);
    cout << "Win rate " << (100.0*wintie.first/N) << " %, tie " << (100.0*wintie.second/N) << " %" << endl;

    return 0;
}
