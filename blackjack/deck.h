#ifndef deck_included
#define deck_included
#include <msp430.h>
#include <stdlib.h>

struct Card{
  char value; // numerical value
  char suit;  // shape & color
  bool drawn; // has the card been drawn?
};
struct Card deck[52];
struct Card drawCard();
void initDeck();
int customRand();

#endif
