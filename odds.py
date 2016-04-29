import random, time
from itertools import product
from poker import *

start = time.clock()
init_rank()
end = time.clock()
print('Init took', '%.3f' % (end-start), 'seconds')

s, N, step = [0]*10, 0, 100000
deck = list(v*16+s for v, s in product(range(2,15), range(4)))
while True:
    for i in range(step): s[rank(random.sample(deck, 5))>>24] += 1
    N += step
    print('%d:'%N, '  '.join('%.4f' % (100*v/N) for v in s))
