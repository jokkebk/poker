CC=g++
CFLAGS=--std=c++11 -O3 -D__NO_INLINE__

all: odds

run:
	./odds

odds: odds.o poker.o
	${CC} ${CFLAGS} $^ -s -o $@
 	
%.o: %.cc
	$(CC) $(CFLAGS) -c $< -o $@

poker.o: poker.h
# %.flash: %.hex
# 	$(DUDE) $(DUDEFLAGS) -U flash:w:$*.hex
# 	
# %.hex: %.elf
# 	$(OBJCOPY) $(OBJFLAGS) $< $@
# 	
# %.elf: %.o
# 	$(CC) $(CFLAGS) $< -o $@
# 	
# %.o: %.S	
# 	$(CC) $(CFLAGS) -c $< -o $@
# 
# poker.o: pokerlib.c arrays.h
# 	${CC} -c ${CFLAGS} pokerlib.c -o pokerlib.o
