
CPU		= msp430g2553
CFLAGS		= -mmcu=${CPU} -Os -I../h
LDFLAGS		= -L/opt/ti/msp430_gcc/include -L../lib

CC		= msp430-elf-gcc
AS		= msp430-elf-gcc -mmcu=${CPU} -c

all: bj_game.elf

bj_game.elf: ${COMMON_OBJECTS} bj_game.o deck.o switches.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lTimer -lLcd

load: bj_game.elf
	msp430loader.sh $^

clean:
	rm -f *.o *.elf
