#include <msp430.h>
#include <stdlib.h>
//#include <time.h>
#include <stdbool.h>
#include "deck.h"

unsigned int customRandSeed = 1;

void initDeck() {
  // Initialize the deck with values and suits
  char values[] = {'2', '3', '4', '5', '6', '7', '8', '9', 10, 'J', 'Q', 'K', 'A'};
  /* Heart = { , Diamond = | , Spade = } , Clover = ~  */
  char suits[] = {'{', '|', '}', '~'};
  int index = 0;
  for (int v = 0; v < 13; v++) {
    for (int s = 0; s < 4; s++) {
      deck[index].value = values[v];
      deck[index].suit = suits[s];
      deck[index].drawn = false;
      index++;
    }
  }
  // Shuffle the deck
  //srand(time(NULL));
  for (int i = 0; i < 52; i++) {
    int j = i + customRand() % (52 - i);
    struct Card temp = deck[i];
    deck[i] = deck[j];
    deck[j] = temp;
  }
}

int customRand(){
  customRandSeed = customRandSeed * 1103515245 + 12345;
  return (customRandSeed / 65536) % 32768;
}

struct Card drawCard() {
  // Draw a card from the deck and update deck state
  //  int index;
  int crd = 0;
  //srand(time(NULL));
  do{
    crd++;
    //index = customRand() % 52;// randomly draw from the deck to improve shuffle
  } while(deck[crd].drawn);  // keep drawing until undrawn card found
  deck[crd].drawn = true;    // mark as drawn
  return deck[crd];
}


