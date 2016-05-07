#include "poker.h"

using namespace std;

#define MAKE_CARD(v, s) (((s)<<4) + (v))
#define CV(c) ((c)&15)
#define CS(c) ((c)>>4)
#define MAKE_RANK(t,k,v) (((t)<<24) + ((k)<<20) + (v))

vector<int> value_rank(0xF0000);

// Transform 5-card hand into offset
int get_offset(const vector<int> & h) {
    int offset = 0;
    for(int j=0; j<5; j++)
        offset += (h[j]&15) << (j*4);
    return offset;
}

int slow_rank(const vector<int> & h, bool debug=false) {
    int base = get_offset(h);
    map<int,int> count;
    for(int v : h) count[v]++;
    vector<pair<int,int>> fq;
    for(auto p : count) fq.push_back(make_pair(p.second, p.first));
    sort(fq.rbegin(), fq.rend()); // Frequencies in decreasing order

    // Check if straight and where does it begin
    bool straight = false;
    int begin = 0;
    if(fq[0].first == 1) {
        straight = true;
        if(h[4]==h[0]+4) begin = h[0];
        else if(h[3]==5 && h[4]==14) begin = 1;
        else straight = false;
    }

    if(debug) {
        cout << "h: ";
        for(int v : h) cout << v << " "; cout << endl;
        for(auto p : fq) {
            cout << p.second << " x " << p.first << endl;
        }
    }

    // Categorize the hand
    if(fq[0].first == 5) // Five of a kind
        return 0; // no five of a kind
    else if(fq[0].first == 4) // Four of a kind
        return MAKE_RANK(7, fq[0].second, base);
    else if(fq[0].first == 3 && fq[1].first == 2) // Full house
        return MAKE_RANK(6, fq[0].second, base); // pair in base
    else if(straight) // Straight
        return MAKE_RANK(4, begin, base);
    else if(fq[0].first == 3) // Three of a kind
        return MAKE_RANK(3, fq[0].second, base);
    else if(fq[0].first == 2) { // A pair or two
        if(fq[1].first == 2) // Two pairs
            return MAKE_RANK(2, max(fq[0].second, fq[1].second), min(fq[0].second, fq[1].second)*16 + fq[2].second);
        else // One pair
            return MAKE_RANK(1, fq[0].second, base);
    } else
        return MAKE_RANK(0, 0, base);
}

void init_rank() {
    vector<int> h(5);

    //for(int i=0x22222; i<0xEEEEE; i++) {
    for(int i=0xEEEEE; i>0x22222; i--) {
        bool skip = false;
        for(int j=0; j<5; j++) {
            h[j] = (i >> (j*4)) & 15;
            if(h[j] < 2 || h[j] > 14) skip=true;
        }
        if(skip) continue; // avoid illegal cards

        int offset = get_offset(h);

        sort(h.begin(), h.end());

        int base = get_offset(h);

        if(base == offset) // ordered, first hand
            value_rank[offset] = slow_rank(h); // create
        else // an unordered permutation (they come later)
            value_rank[offset] = value_rank[base]; // copy
    }
}

int rank_hand(const vector<int> & h) {
    int offset = (h[0]&15) + ((h[1]&15)<<4) +
        ((h[2]&15)<<8) + ((h[3]&15)<<12) + ((h[4]&15)<<16);
    int val = value_rank[offset];
    return (h[0]&h[1]&h[2]&h[3]&h[4]&0xF0) ? MAKE_RANK(5, 0, val) : val;
}

int rank_hand(int c1, int c2, int c3, int c4, int c5) {
    int val = value_rank[(c1&15) + ((c2&15)<<4) +
        ((c3&15)<<8) + ((c4&15)<<12) + ((c5&15)<<16)];
    //cout << "rank_hand(" << c1 << ", " << c2 << ", " << c3 << ", " << c4 << ", " << c5 << ") = " << val << endl;
    return (c1&c2&c3&c4&c5&0xF0) ? MAKE_RANK(5, 0, val) : val;
}

int rank_hand7(const std::vector<int> & h) {
    int best = rank_hand(h[0], h[1], h[2], h[3], h[4]);
    best = max(best, rank_hand(h[0], h[1], h[2], h[3], h[5]));
    best = max(best, rank_hand(h[0], h[1], h[2], h[3], h[6]));
    best = max(best, rank_hand(h[0], h[1], h[2], h[4], h[5]));
    best = max(best, rank_hand(h[0], h[1], h[2], h[4], h[6]));
    best = max(best, rank_hand(h[0], h[1], h[2], h[5], h[6]));
    best = max(best, rank_hand(h[0], h[1], h[3], h[4], h[5]));
    best = max(best, rank_hand(h[0], h[1], h[3], h[4], h[6]));
    best = max(best, rank_hand(h[0], h[1], h[3], h[5], h[6]));
    best = max(best, rank_hand(h[0], h[1], h[4], h[5], h[6]));
    best = max(best, rank_hand(h[0], h[2], h[3], h[4], h[5]));
    best = max(best, rank_hand(h[0], h[2], h[3], h[4], h[6]));
    best = max(best, rank_hand(h[0], h[2], h[3], h[5], h[6]));
    best = max(best, rank_hand(h[0], h[2], h[4], h[5], h[6]));
    best = max(best, rank_hand(h[0], h[3], h[4], h[5], h[6]));
    best = max(best, rank_hand(h[1], h[2], h[3], h[4], h[5]));
    best = max(best, rank_hand(h[1], h[2], h[3], h[4], h[6]));
    best = max(best, rank_hand(h[1], h[2], h[3], h[5], h[6]));
    best = max(best, rank_hand(h[1], h[2], h[4], h[5], h[6]));
    best = max(best, rank_hand(h[1], h[3], h[4], h[5], h[6]));
    return max(best, rank_hand(h[2], h[3], h[4], h[5], h[6]));
}

int rank_hand7(int c1, int c2, int c3, int c4, int c5, int c6, int c7) {
    int best = rank_hand(c1, c2, c3, c4, c5);
    best = max(best, rank_hand(c1, c2, c3, c4, c6));
    best = max(best, rank_hand(c1, c2, c3, c4, c7));
    best = max(best, rank_hand(c1, c2, c3, c5, c6));
    best = max(best, rank_hand(c1, c2, c3, c5, c7));
    best = max(best, rank_hand(c1, c2, c3, c6, c7));
    best = max(best, rank_hand(c1, c2, c4, c5, c6));
    best = max(best, rank_hand(c1, c2, c4, c5, c7));
    best = max(best, rank_hand(c1, c2, c4, c6, c7));
    best = max(best, rank_hand(c1, c2, c5, c6, c7));
    best = max(best, rank_hand(c1, c3, c4, c5, c6));
    best = max(best, rank_hand(c1, c3, c4, c5, c7));
    best = max(best, rank_hand(c1, c3, c4, c6, c7));
    best = max(best, rank_hand(c1, c3, c5, c6, c7));
    best = max(best, rank_hand(c1, c4, c5, c6, c7));
    best = max(best, rank_hand(c2, c3, c4, c5, c6));
    best = max(best, rank_hand(c2, c3, c4, c5, c7));
    best = max(best, rank_hand(c2, c3, c4, c6, c7));
    best = max(best, rank_hand(c2, c3, c5, c6, c7));
    best = max(best, rank_hand(c2, c4, c5, c6, c7));
    best = max(best, rank_hand(c3, c4, c5, c6, c7));
    //cout << "rank_hand7(" << c1 << ", " << c2 << ", " << c3 << ", " << c4 << ", " << c5 << ", " << c6 << ", " << c7 << ") = " << best << endl;
    return best;
}

vector<int> make_deck() {
    vector<int> deck(52);
    for(int i=0; i<52; i++)
        deck[i] = (0x10 << (i&3)) + i/4+2;
    return deck;
}

int shuffle_deck(vector<int> & deck, int n, int off /*=0*/) {
    for(int i=off; i<off+n; i++)
        swap(deck[i], deck[i+rand()%(52-i)]);
}

int make_card(string vs) {
    static string values = "__23456789TJQKA", suits = "SDHC";
    return (0x10 << suits.find(toupper(vs[1]))) + values.find(toupper(vs[0]));
}

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
