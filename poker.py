"""Fast poker hand ranking library.

Cards hold value in top hex, suit in bottom hex. Ace is 14 (E).
Hand is a tuple/list, e.g. [0x2E, 0x2D, 0x2C, 0x2B, 0x2A] (royal flush).
"""

from itertools import combinations_with_replacement
from collections import Counter
from array import array

def make_card(s):
    """Make card from 10s, QD kind of string."""
    suit = 'SDHC'.index(s.upper()[-1])
    if len(s) == 3: return suit + int(s[:2])*16
    else: return '__23456789TJQKA'.index(s.upper()[0])*16 + suit

def card_str(c):
    """Return string representation of a card."""
    return '__23456789TJQKA'[c>>4] + 'SDHC'[c&15]

def hand_str(h):
    """Return string representation of a hand."""
    return ' '.join(card_str(c) for c in h)

def hand2value(h):
    """Transform card values in a hand to a base 16 number."""
    return sum((v>>4) << i*4 for i,v in enumerate(sorted(h)))

UNIT = lambda t,v: (t<<24)+(v<<20) # Used in rank(h)
RANK = {}

def init_rank():
    """Precalculate lookup values for rank()."""
    S, H = 0, 0
    for h in combinations_with_replacement(range(2,15), 5):
        multi = Counter(h).most_common(3)
        base = sum(v << i*4 for i,v in enumerate(h))

        if multi[0][1] == 5: continue # No five of a kind

        straight = False
        if multi[0][1] == 1: 
            straight = True
            if h[4]==h[0]+4: begin = h[0]
            elif h[3]==5 and h[4]==14: begin = 1
            else: straight = False

        # Categorize the hand
        if multi[0][1] == 4: # Four of a kind
            RANK[base] = UNIT(7, multi[0][0]) + base
        elif multi[0][1] == 3 and multi[1][1] == 2: # Full house
            RANK[base] = UNIT(6, multi[0][0]) + base # pair in base
        elif straight: # Straight
            RANK[base] = UNIT(4, begin) + base
        elif multi[0][1] == 3: # three of a kind
            RANK[base] = UNIT(3, multi[0][0]) + base
        elif multi[0][1] == 2: # two or three pairs
            if multi[1][1] == 2: # two pairs
                RANK[base] = UNIT(2, max(multi[0][0], multi[1][0])) + min(multi[0][0], multi[1][0])*16 + multi[2][0]
            else: # one pair
                RANK[base] = UNIT(1,multi[0][0]) + base
        else:
            RANK[base] = UNIT(0, 0) + base

def rank(h):
    """Rank a hand. Larger value is better.
    
    Top hex describes hand type:
    0x9xxxxxx: Straight flush (including royal)
    0x7xxxxxx: Four of a kind
    0x6xxxxxx: Full house
    0x5xxxxxx: Flush
    0x4xxxxxx: Straight
    0x3xxxxxx: Three of a kind
    0x2xxxxxx: Two pairs
    0x1xxxxxx: One pair
    0x0xxxxxx: Nothing special
    """
    val = RANK[hand2value(h)]
    for v in h:
        if (v^h[0])&15: return val
    return val + UNIT(5, 0) # Upgrade straight to 9 and normal to 5
