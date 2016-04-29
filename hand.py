import sys, random
from itertools import product, combinations
from poker import *

def best_hand(cs):
    return max(rank(list(h)) for h in combinations(cs, 5))

def simulate(hole, common, opps, sims=10000):
    deck = list(v*16+s for v, s in product(range(2,15), range(4)))

    for c in hole+common: deck.remove(c) # take out my cards

    totals = [0,0,0]

    for i in range(sims):
        deal = random.sample(deck, 5-len(common) + 2*opps) + common
        comm = deal[-5:]
        score = best_hand(comm+hole)
        res = 1
        for opp in range(opps):
            oc = deal[opp*2:opp*2+2]
            oppscore = best_hand(comm+oc)
            if oppscore > score:
                res = -1
                break
            elif oppscore == score:
                res = 0
        totals[res+1] += 1
    return totals[2]/sims, totals[1]/sims

sims = int(sys.argv[1]) if len(sys.argv) > 1 else 10000
opps = 5
init_rank()

print('Poker odds calculator using random simulations.')
print('%d simulations, change with command-line argument.' % sims)
print('%d opponents, change with "opp <n>"' % opps)

while True:
    print('Input hand in format "10S KD" (not case sensitive)')
    cmd = input().strip().upper().split()

    if len(cmd) == 0: continue
    if cmd[0] == 'OPP':
        opps = int(cmd[1])
        print('%d opponents' % opps)
        continue
    if cmd[0] in ('Q', 'QUIT', 'X', 'EXIT'): break

    hole = [make_card(c) for c in cmd]
    common = []

    while len(common) < 5:
        print('Input common cards and press ENTER to calculate odds')
        cards = input().strip().upper().split()
        for c in cards: common.append(make_card(c))
        win, tie = simulate(hole, common, opps, sims)
        print('Common', hand_str(common), 'you', hand_str(hole))
        print('Win probability %.2f, tie %.2f' % (100.0*win, 100.0*tie))
