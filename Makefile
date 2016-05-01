CC=g++
CFLAGS=--std=c++11 -O3 -D__NO_INLINE__

all: odds hand

run: hand
	./$<

hand: hand.o poker.o
	${CC} ${CFLAGS} $^ -s -o $@
 	
%.o: %.cc
	$(CC) $(CFLAGS) -c $< -o $@

odds: odds.o poker.o
	${CC} ${CFLAGS} $^ -s -o $@
 	
%.o: %.cc
	$(CC) $(CFLAGS) -c $< -o $@

poker.o: poker.h
