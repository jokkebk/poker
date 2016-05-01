#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

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
    int val = value_rank[get_offset(h)];
    int s = h[0]&0xF0;
    for(int i=1; i<5; i++)
        if((h[i]&0xF0) != s) return val;
    return MAKE_RANK(5, 0, val); // Upgrade straight to 9 and normal to 5
}

int shuffle(vector<int> & deck, int n) {
    for(int i=0; i<n; i++)
        swap(deck[i], deck[i+rand()%(52-i)]);
}

int main() {
    vector<int> deck(52);
    clock_t start, end;

    start = clock();
    init_rank();
    end = clock();

    cout << "Init took " << (double)(end-start)/CLOCKS_PER_SEC << " seconds" << endl;

    for(int i=0; i<52; i++)
        deck[i] = (0x10 << (i&3)) + i/4+2;

    vector<int> cnt(10);
    start = clock();
    int N = 100000000;
    for(int i = 0; i < N; i++) {
        shuffle(deck, 5);
        cnt[rank_hand(deck)>>24]++;
    }
    end = clock();
    cout << (double)(end-start)/CLOCKS_PER_SEC << " seconds" << endl;

    for(int i = 0; i < 10; i++) {
        cout << 100.0*cnt[i]/N << endl;
    }

    return 0;

    while(true) {
        vector<int> h(5);
        cout << "Input 5 cards";
        for(int i=0; i<5; i++)
            cin >> hex >> h[i];
        for(int v : h) cout << v << " "; cout << endl;
        cout << hex << rank_hand(h) << endl;
    }

    return 0;
}
