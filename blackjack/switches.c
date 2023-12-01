#include <msp430.h>
#include "switches.h"

#define SWITCHES 15

static char switch_update_interrupt_sense()
{
  char p2val = P2IN;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);/* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);/* if switch down, sense up */
  return p2val;
}

void switch_interrupt_handler(){
  char p2val = switch_update_interrupt_sense();
  char sw1 = (p2val & SW1) ? 0 : SW1;
  char sw2 = (p2val & SW2) ? 0 : SW2;
  char sw3 = (p2val & SW3) ? 0 : SW3;
  char sw4 = (p2val & SW4) ? 0 : SW4;
  if(sw1){
  }
  if(sw2){
  }
  if(sw3){
  }
  if(sw4){
  }
}

void __interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;      /* clear pending sw interrupts */
    switch_interrupt_handler();/* single handler for all switches */
  }
}

void switch_init()/* setup switch */
{
  P2REN |= SWITCHES;/* enables resistors for switches */
  P2IE |= SWITCHES;/* enable interrupts from switches */
  P2OUT |= SWITCHES;/* pull-ups for switches */
  P2DIR &= ~SWITCHES;/* set switches' bits for input */
  switch_update_interrupt_sense();
}
