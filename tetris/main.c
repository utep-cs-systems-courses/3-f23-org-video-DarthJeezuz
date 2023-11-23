#include <msp430.h>
//#include "common.h"
#include "msp430-tetris.h"

void main(void){
  WDTCTL = WDTPW | WDTHOLD;

  initLeds();
  configDisplay();
  configKeypad();
  configMusic();
  bool enableMusic = true;
  unsigned char selection = 0;
  beepWelcome();

  while (true){
    //Draw Menu
    clearScreen(COLOR_BLACK);
    drawString11x16(20,20,"TETRIS", COLOR_WHITE, COLOR_BLACK);
    drawString8x12(20,50,"Select Difficulty", COLOR_WHITE, COLOR_BLACK);  
    //Process menu selections
    while (true){
      selection = getKey();
      if (selection == '1' || selection == '2' || selection == '#' || selection == '*')
	break;
    }
    switch (selection){
    case '1':
      tetris(musicEnabled);
      beepWelcome();
      break;
    case '2':
      blackjack(musicEnabled);
      beepWelcome();
      break;
    case '#':
      musicEnabled = !musicEnabled;
      break;
    }
  }
}
