#include <msp430.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "deck.h"
#include "switches.h"

struct Card player_hand[10] = {'\0'};
struct Card dealer_hand[10] = {'\0'};
int player_hand_size = 0;//# of cards
int dealer_hand_size = 0;
int wallet = 150;
int bet = 0;
int player_hand_value = 0;//total value
int dealer_hand_value = 0;
bool noStart = true;
bool betting = false; 
bool playing = false;
bool second_hand = false; 
bool stats = false;
bool dealers_turn = false;
bool fin = false;
bool draw = false;
bool win = false;
bool loss = false;

void newRound();
void betMenu();
void playMenu();
void splitHand();
bool checkHand();
bool checkTAK();
void displayCards();
void displayValues();
void drawDealersHand();
void dealInitialCards();
int calculateHandValue(struct Card hand[], int size);
void updateDisplay();
void dealerTurn();
void declareWin();
void declareLoss();
void declareDraw();
void playAgain();
void sendCommand(int command);
void bettingStage(int cmd);
void playStage(int cmd);

int main(void) {

  WDTCTL = WDTPW + WDTHOLD; // Stop the watchdog timer
  configureClocks();
  lcd_init();
  switch_init();
  or_sr(0x8);// GIE (enable interrupts)
  wallet = 150;
  newRound();
  clearScreen(COLOR_GREEN);
  drawString5x7(20, 40, "BLACKJACK", COLOR_BLACK, COLOR_GREEN);
  drawString5x7(1, 150, "Press any button", COLOR_BLACK, COLOR_GREEN);
  drawChar5x7(50, 100 , 'K', COLOR_BLACK, COLOR_GREEN);
  drawChar5x7(50, 115 , 'Q', COLOR_BLACK, COLOR_GREEN);
  drawChar5x7(50, 130 , 'J', COLOR_BLACK, COLOR_GREEN);
  or_sr(18);
  // Insert card graphic if there's time
  /* while(noStart){ or_sr(0x10);} */
  /* betMenu(); */
  /* while(1){ */
  /*   or_sr(0x10); */
  /* } */
}
void newRound(){
  initDeck();
  player_hand_size = 0;
  dealer_hand_size = 0;
  bet = 0;
  player_hand_value = 0;
  dealer_hand_value = 0;
  betting = true;
  playing = false;
  second_hand = false;
  stats = false;
  dealers_turn = false;
  fin = false;
  noStart = false;
}
void betMenu(){  
  //clearScreen(COLOR_GREEN);
  //betting = true;
  drawString5x7(20, 30, "Place your bet!", COLOR_BLACK, COLOR_GREEN);
  drawString5x7(1, 150, "Inc  Dec  Stats Bet", COLOR_BLACK, COLOR_GREEN);
  //or_sr(0x18);
}
void playMenu(){
  //clearScreen(COLOR_GREEN);
  drawString5x7(1, 150, "Hit  Stand  Double  Split", COLOR_BLACK, COLOR_GREEN);
  //or_sr(0x18);
}

bool checkWallet(){
  if(wallet < bet){return false;}
  return true;
}

bool checkTAK(){ // verify Two of A Kind (TAK)
  char val1 = player_hand[0].value;
  char val2 = player_hand[1].value;
  if(val1 == val2){return true;}
  return false;
}

void displayCards(){
  char val;
  char suit;
  for(int i = 0; i < player_hand_size; i++){
    val = player_hand[i].value;
    suit = player_hand[i].suit;
    drawChar5x7(90+(10 * i), 100 , val, COLOR_BLACK, COLOR_GREEN);
    drawHand(20+(10 * i),80,50,60, COLOR_WHITE, suit, val);
    // drawHand(2+(10 * i),80,50,60, COLOR_WHITE, player_hand[i].suit, player_hand[i].value);
  }
  drawDealersHand();
  //displayValues(); 
}
void displayValues(){
  int total = calculateHandValue(player_hand, player_hand_size);
  int dlrCrd = calculateHandValue(dealer_hand, dealer_hand_size);
  char handVal[12];
  char wString[15];// = {'\0'};
  char dString[15];// = {'\0'};
  sprintf(wString, "Wallet: %d ", wallet);
  drawString5x7(5, 5, wString, COLOR_BLACK, COLOR_WHITE);
  sprintf(wString, "bet: %d", bet);
  drawString5x7(5, 15, wString, COLOR_BLACK, COLOR_WHITE);
  sprintf(dString, "Dealer: %d", dlrCrd);
  drawString5x7(5, 50, dString, COLOR_BLACK, COLOR_WHITE);
  sprintf(handVal, "Hand: %d", total);
  drawString5x7(5, 70, handVal, COLOR_BLACK, COLOR_WHITE);
}
void displayStatistics(){
}

void drawDealersHand(){
  char val;
  char suit;
  for(int i = 0; i < dealer_hand_size; i++){
    if(i == 0 || dealers_turn){ // only show the first card face up, unless it's dealers turn
      val = dealer_hand[i].value;
      suit = dealer_hand[i].suit;
      drawHand(50, -40, 50, 60, COLOR_WHITE, suit, val);
    }
    else{
      // draw the cards'backside
      drawBOC(50+(10 * i), -40, 50, 60, COLOR_BLACK);
    }
  }
}

void dealInitialCards() {
  drawString5x7(1, 100, "Entered 'dealInitialCards()'", COLOR_BLACK, COLOR_GREEN);
  for(int i=0; i < 2; i++){
    drawString5x7(1, 100, "Entered for loop", COLOR_BLACK, COLOR_GREEN);
    player_hand[i] = drawCard();
    dealer_hand[i] = drawCard();
    dealer_hand_size++;
    player_hand_size++;
  }
  //displayCards();
  player_hand_value = calculateHandValue(player_hand, player_hand_size);
  dealer_hand_value = calculateHandValue(dealer_hand, dealer_hand_size);
  drawString5x7(1, 100, "FINISHED CALCULATING VALUES FOR CARDS", COLOR_BLACK, COLOR_GREEN);
  updateDisplay();
  if(dealer_hand_value == 21 && player_hand_value == 21){ declareDraw();}
  else if(dealer_hand_value == 21 && player_hand_value != 21){ declareLoss();}
  else{ updateDisplay();}
}

int calculateHandValue(struct Card hand[], int size){
  int totalValue = 0;
  int numAces = 0;
  for (int i = 0; i < size; i++) {
    char cardValue = hand[i].value;
    if (cardValue >= '2' && cardValue <= '9') {
      totalValue += (cardValue - '0');  // Convert character to integer
    } else if (cardValue == 'A') {
      numAces++;
    } else {
      // '10', 'J', 'Q', 'K' are all worth 10 points
      totalValue += 10;
    }
  }
  // Add Aces with considering the optimal value (1 or 11)
  for (int i = 0; i < numAces; i++) {
    if (totalValue + 11 <= 21) {
      totalValue += 11;
    } else {
      totalValue += 1;
    }
  }
  return totalValue;
}
void updateDisplay() {
  clearScreen(COLOR_GREEN);
  displayValues();
  if(playing){
    displayCards();
    playMenu();
  }
  else if(betting){ betMenu();}
  else if(draw){ declareDraw();}
  else if(loss){ declareLoss();}
  else if(win){ declareWin();}
  if(stats){ displayStatistics();}
}

void dealerTurn() {
  dealers_turn = true;
  while(dealer_hand_value < 16){
    dealer_hand[dealer_hand_size] = drawCard();
    dealer_hand_size++;
    dealer_hand_value = calculateHandValue(dealer_hand, dealer_hand_size);
    updateDisplay();
  }
  fin = true;
  if(dealer_hand_value > 21 || player_hand_value > dealer_hand_value){ win = true;}
  else{ loss = true;}
  updateDisplay();
}
 
void sendCommand(int command) {
// (inc|dec|bet|stats)
  if(betting){ bettingStage(command);}
// (hit|stand|double|split)
  else if(playing){ playStage(command);}
// (YES           NO)
  else if(fin){   //yes (reset all values, except wallet) 
    if(command == 1){
      newRound();
      betMenu();
    }
    if(command == 4){ main();}//no (wallet resets)
  }
  // starting Interface. press any key to start
  else if(noStart){
    if(command == 1 || command == 2 || command == 3 || command == 4){
      noStart = false;
      betting = true;
      betMenu();
    }
  }
}

void splitHand() {
  second_hand = true;
  
}
void playStage(int cmd){
  switch(cmd){
    case 1:// request an additional card
      player_hand[player_hand_size] = drawCard();
      player_hand_size++;
      updateDisplay();
      break;
    case 2:// proclaim to be done
      dealerTurn();
      break;
    case 3:// double the bet & recieve a final card
      bet *= 2;
      player_hand[player_hand_size] = drawCard();
      updateDisplay();
      dealerTurn();
      break;
    case 4:
      /* IFF the initial pair of cards given, are the same value 
      (i.e. two eights | two kings, etc) they can be split and play
      each one like two separate hands. The original bet will go 
      with one hand and the player is required to place an equal bet
      on the second hand. Each hand has to be played one at a time. 
      Should the player lack the funds necessary to bet on the second
      hand then they'll be unable to 'split' their hand.*/
      if(checkWallet() && checkTAK()){
	splitHand();
      }
      else{
	// send error message: player lacks funds
      }
      break;    
  }
}
void bettingStage(int cmd){
  displayValues();
  switch(cmd){
    case 1://increase bet
      if(wallet <= 0){
	// display error message
	// beep error sound
	break;
      }
      bet += 10;
      wallet -= 10;
      updateDisplay();
      break;
    case 2://decrease bet
      if(bet <= 0){
	// send error message
	break;
      }
      bet -= 10;
      wallet += 10;
      updateDisplay();
      break;
    case 3:// toggle statistics
      if(stats){stats = false;}
      else{stats = true;}
      updateDisplay();
      break;
    case 4:// start round
      betting = false;
      playing = true;
      dealInitialCards();
      break;
    }
}

void declareWin(){
  clearScreen(COLOR_GREEN);
  char win[10] = {'\0'};// string to hold winning amount
  drawString5x7(20, 80, "YOU WIN!", COLOR_GREEN, COLOR_BLACK);
  sprintf(win, "+ %d", bet);
  //drawString5x7(55, 70, totVal, COLOR_BLACK, COLOR_WHITE);
  drawString5x7(20, 90, win, COLOR_BLACK, COLOR_WHITE);
  wallet += (bet*2);
  playAgain();
}
void declareLoss(){
  clearScreen(COLOR_GREEN);
  drawString5x7(20, 80, "YOU LOSE!", COLOR_WHITE, COLOR_BLACK);
  playAgain();
}
void declareDraw(){
  clearScreen(COLOR_GREEN);
  drawString5x7(20, 80, "It's a Draw!", COLOR_WHITE, COLOR_BLACK);
  wallet += bet;
  playAgain();
}
void playAgain(){
  playing = false;
  fin = true;
  drawString5x7(20, 110, "PLAY AGAIN?", COLOR_BLACK, COLOR_GREEN);
  drawString5x7(1, 150, "YES             NO", COLOR_BLACK, COLOR_GREEN);	
}
