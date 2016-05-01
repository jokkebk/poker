#ifndef __POKER_H
#define __POKER_H

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

int rank_hand(const std::vector<int> & h);
void init_rank();
std::vector<int> make_deck();
int shuffle_deck(std::vector<int> & deck, int n);

#endif
