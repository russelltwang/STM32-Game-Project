/*
  ChibiOS - Copyright (C) 2006-2014 Giovanni Di Sirio

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

/*
  I. "Ike" Kottlowski
  ikottlow
  Russell "The Kahn"  Wang
  ruswang
*/

#include "ch.h"
#include "hal.h"
#include "test.h"
#include "chprintf.h"
#include <chstreams.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#define UNUSED(x) (void)(x)

void allLedOn(void){
  palSetPad(GPIOE, GPIOE_LED10_RED); 
  palSetPad(GPIOE, GPIOE_LED3_RED); 
  palSetPad(GPIOE, GPIOE_LED9_BLUE); 
  palSetPad(GPIOE, GPIOE_LED4_BLUE); 
  palSetPad(GPIOE, GPIOE_LED7_GREEN); 
  palSetPad(GPIOE, GPIOE_LED6_GREEN); 
  palSetPad(GPIOE, GPIOE_LED5_ORANGE); 
  palSetPad(GPIOE, GPIOE_LED8_ORANGE);
}

void allLedOff(void){
  palClearPad(GPIOE, GPIOE_LED10_RED); 
  palClearPad(GPIOE, GPIOE_LED3_RED); 
  palClearPad(GPIOE, GPIOE_LED9_BLUE); 
  palClearPad(GPIOE, GPIOE_LED4_BLUE); 
  palClearPad(GPIOE, GPIOE_LED7_GREEN); 
  palClearPad(GPIOE, GPIOE_LED6_GREEN); 
  palClearPad(GPIOE, GPIOE_LED5_ORANGE);
  palClearPad(GPIOE, GPIOE_LED8_ORANGE);
}

int FLAG;
char time[5];
int time_value = -1;
/* Thread that blinks North LED */
static THD_WORKING_AREA(waCounterThread,128);
static THD_FUNCTION(counterThread,arg) {
  UNUSED(arg);
  while (TRUE) {
    if(FLAG == 1){
      allLedOn();
    } else {
      palSetPad(GPIOE, GPIOE_LED3_RED);
      chThdSleepMilliseconds(100);
      palClearPad(GPIOE, GPIOE_LED3_RED);
      chThdSleepMilliseconds(100);
    }
    chThdSleepMilliseconds(10);
  }    
  return 0;
}


/* Thread that write the status of the user button to the south LED */
static THD_WORKING_AREA(waButtonThread,128);
static THD_FUNCTION(buttonThread,arg) {
  UNUSED(arg);
  while (TRUE) {
    if (palReadPad(GPIOA, GPIOA_BUTTON)) { 
      allLedOff();
      time_value = -1;
      FLAG = 0;
    }
    chThdSleepMilliseconds(10);
  }
  return 0;
}

/*
Thread that echos characters received from the console
static THD_WORKING_AREA(waEchoThread,128);
static THD_FUNCTION(echoThread,arg) {
  UNUSED(arg);
  uint8_t ch;
  while (TRUE) {
    chnRead((BaseSequentialStream*)&SD1,&ch,1);
    chnWrite((BaseSequentialStream*)&SD1,&ch,1);
  }
  return 0;
}
*/

/* Egg Timer Thread */
static THD_WORKING_AREA(waEggTimerThread, 128);
static THD_FUNCTION(EggTimerThread, arg){
  UNUSED(arg);
  uint8_t ch;
  int i = 0;
  while(TRUE){
    chnRead((BaseSequentialStream*)&SD1, &ch, 1);
    if(ch == 's'){
      FLAG = 0;
      time_value = -1;
      chprintf((BaseSequentialStream *)&SD1, "Set the timer between 1-10000: ");
      while(TRUE){
        chnRead((BaseSequentialStream*)&SD1, &ch, 1);
        chnWrite((BaseSequentialStream*)&SD1, &ch, 1);
        time[i] = ch;
        if (ch == 's'){
          chprintf((BaseSequentialStream *)&SD1, "\nPress s to set the time.\n");
          time_value = -1;
          i = 0;
          break;
        } else if (ch == 10){
          time_value = atoi(time);
          i = 0;
          time_value = time_value - (time_value % 10);
          break;
        } else {;
          i++;
        }
      }
      if((time_value > 10000) || (time_value < 0)){
        time_value = -1;
        i = 0;
        chprintf((BaseSequentialStream *)&SD1, "Invalid entry, press s to restart.\n");
      }
    }
    chThdSleepMilliseconds(10);
  }
  return 0;
}

static THD_WORKING_AREA(waTimerThread, 128);
static THD_FUNCTION(TimerThread, arg){
  UNUSED(arg);
  while(TRUE){
    if ((FLAG == 0) && (time_value >= 10)) {
      time_value -= 10;
    } else if( (FLAG == 0) && (time_value < 10) && (time_value > 0)) {
      time_value--;
    } else if (time_value == 0){
      time_value = -1;
      FLAG = 1;
    }
    chThdSleepMilliseconds(10);
  }
  return 0;
}


/*
 * Application entry point.
 */
int main(void) {
  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /*
   * Activates the serial driver 1 using the driver default configuration.
   * PC4(RX) and PC5(TX). The default baud rate is 9600.
   */
  sdStart(&SD1, NULL);
  palSetPadMode(GPIOC, 4, PAL_MODE_ALTERNATE(7));
  palSetPadMode(GPIOC, 5, PAL_MODE_ALTERNATE(7));
  chprintf((BaseSequentialStream*)&SD1, "\n\rUp and Running\n\r");

  /*
   * Creates the threads.
   */
  chThdCreateStatic(waCounterThread, sizeof(waCounterThread), NORMALPRIO+1, counterThread, NULL);
  chThdCreateStatic(waButtonThread, sizeof(waButtonThread), NORMALPRIO+1, buttonThread, NULL);
  //chThdCreateStatic(waEchoThread, sizeof(waEchoThread), NORMALPRIO+1, echoThread, NULL);
  chThdCreateStatic(waEggTimerThread, sizeof(waEggTimerThread), NORMALPRIO+1, EggTimerThread, NULL);
  chThdCreateStatic(waTimerThread, sizeof(waTimerThread), NORMALPRIO+1, TimerThread, NULL);

  /*
    Main spins here while the threads do all of the work. 
  */ 
  while (TRUE);
}
