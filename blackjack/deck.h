#include <msp430.h>
#include <stdlib.h>

struct Card{
  char value;
  char suit;
}
struct Card deck[52];
struct Card drawCard();
void initDeck();
