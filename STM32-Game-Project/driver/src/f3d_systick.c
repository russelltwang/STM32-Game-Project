/* f3d_systick.c --- 
 * 
 * Filename: f3d_systick.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Thu Nov 14 07:57:37 2013
 * Last-Updated: 
 *           By: 
 *     Update #: 0
 * Keywords: 
 * Compatibility: 
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* Change log:
 * 
 * 
 */

/* Copyright (c) 2004-2007 The Trustees of Indiana University and 
 * Indiana University Research and Technology Corporation.  
 * 
 * All rights reserved. 
 * 
 * Additional copyrights may follow 
 */

/* Code: */
#include "queue.h"
#include <f3d_systick.h>
#include <f3d_led.h> 
#include <f3d_uart.h>


extern queue_t txbuf;
extern queue_t rxbuf;




volatile uint8_t  rate = 100;

volatile uint8_t i = 0;
volatile int systick_flag = 0;

void f3d_systick_init(void) {  
  SysTick_Config(SystemCoreClock/rate);
}

void SysTick_Handler(void) {
  systick_flag ^=1;
  if (systick_flag) {
  f3d_led_on(i);
  
  
  }
  else {
  f3d_led_off(i);
  i++;
  i = i % 8;
  }  
  
  
  if (!queue_empty(&txbuf)) {
    flush_uart();
  }
  // if (button_state) {
  //  rate = 10;
  // }
  //else {
  //  rate = 100;
  // }
}

/* f3d_systick.c ends here */
