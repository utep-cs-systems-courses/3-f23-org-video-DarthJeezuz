/** \file lcddemo.c
 *  \brief A simple demo that draws a string and square
 */

#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

/** Initializes everything, clears the screen, draws "hello" and a square */
int
main()
{
  configureClocks();
  lcd_init();
  u_char width = screenWidth, height = screenHeight;

  clearScreen(COLOR_GREEN);

  drawString5x7(20,20, "Place your bet!", COLOR_WHITE, COLOR_GREEN);
  drawString5x7(1,150, "Inc  Dec  Bet  Decks", COLOR_WHITE, COLOR_GREEN);
  drawCard(20,80,50,60, COLOR_WHITE, '{', 'K'); 
  //fillRectangle(30,30, 60, 60, COLOR_ORANGE);
  
}
