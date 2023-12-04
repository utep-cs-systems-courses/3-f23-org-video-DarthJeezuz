#include <msp430.h>
#include <stdlib.h>
#include <time.h>
#include "deck.h"


void initDeck() {
  // Initialize the deck with values and suits
  char values[] = {'2', '3', '4', '5', '6', '7', '8', '9', '10', 'J', 'Q', 'K', 'A'};
  char suits[] = {'h', 'd', 's', 'c'};
  int index = 0;
  for (int v = 0; v < 13; v++) {
    for (int s = 0; s < 4; s++) {
      deck[index].value = values[v];
      deck[index].suit = suits[s];
      index++;
    }
  }
  // Shuffle the deck
  srand(time(NULL));
  for (int i = 0; i < 52; i++) {
    int j = i + rand() % (52 - i)
    struct Card temp = deck[i];
    deck[i] = deck[j];
    deck[j] = temp;
  }
}

struct Card drawCard() {
// Draw a card from the deck and update deck state
struct Card drawnCard = deck[player_hand_size + dealer_hand_size];
player_hand_size++;
return drawnCard;
}


