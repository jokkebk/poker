#ifndef __POKER_H
#define __POKER_H

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <cctype>

int rank_hand(const std::vector<int> & h);
int rank_hand(int c1, int c2, int c3, int c4, int c5);

int rank_hand7(const std::vector<int> & h);
int rank_hand7(int c1, int c2, int c3, int c4, int c5, int c6, int c7);

void init_rank();
std::vector<int> make_deck();
int shuffle_deck(std::vector<int> & deck, int n, int off=0);

int make_card(std::string vs);

std::pair<int,int> simulate(std::vector<int> & deck, int dealt, int opps, int N);

#endif
