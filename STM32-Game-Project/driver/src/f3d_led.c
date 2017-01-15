/* led.c --- 
 * 
 * Filename: led.c
 * Description: driver for led initialization on STM32
 * Author: Michael Ledesma<mledesma> and Russell Wang<ruswang>
 * Maintainer: 
 * Created: Thu Jan 10 10:53:06 2013
 * Last-Updated: 2/13/14 by Michael Ledesma
 *     Update #: 1
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
 * 02-07-2014 defined methods
 * 02-13-2014 altered mapping of indices to pins, modified comments 
 */

/* Code: */
#include <stm32f30x.h>
#include <f3d_led.h>

/*
  initializes all pins corresponding to the compass leds
 */
void f3d_led_init(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/*
  enables the led specified by int led parameter
 */
void f3d_led_on(int led) {
  switch (led) {
  case 7:
    GPIOE->BSRR = GPIO_Pin_8;
    break;
  case 0:
    GPIOE->BSRR = GPIO_Pin_9;
    break;
  case 1:
    GPIOE->BSRR = GPIO_Pin_10;
    break;
  case 2:
    GPIOE->BSRR = GPIO_Pin_11;
    break;
  case 3:
    GPIOE->BSRR = GPIO_Pin_12;
    break;
  case 4:
    GPIOE->BSRR = GPIO_Pin_13;
    break;
  case 5:
    GPIOE->BSRR = GPIO_Pin_14;
    break;
  case 6:
    GPIOE->BSRR = GPIO_Pin_15;
  default:
    break;
  }
}

/*
 disables led specified by led input
 */ 
void f3d_led_off(int led) {
  switch (led) {
  case 7:
    GPIOE->BRR = GPIO_Pin_8;
    break;
  case 0:
    GPIOE->BRR = GPIO_Pin_9;
    break;
  case 1:
    GPIOE->BRR = GPIO_Pin_10;
    break;
  case 2:
    GPIOE->BRR = GPIO_Pin_11;
    break;
  case 3:
    GPIOE->BRR = GPIO_Pin_12;
    break;
  case 4:
    GPIOE->BRR = GPIO_Pin_13;
    break;
  case 5:
    GPIOE->BRR = GPIO_Pin_14;
    break;
  case 6:
    GPIOE->BRR = GPIO_Pin_15;
  default:
    break;
  }
} 

/*
  turn on all 8 leds 
 */
void f3d_led_all_on(void) {
  
  GPIOE->BSRR = GPIO_Pin_8;
  GPIOE->BSRR = GPIO_Pin_9;
  GPIOE->BSRR = GPIO_Pin_10;
  GPIOE->BSRR = GPIO_Pin_11;
  GPIOE->BSRR = GPIO_Pin_12;
  GPIOE->BSRR = GPIO_Pin_13;
  GPIOE->BSRR = GPIO_Pin_14;
  GPIOE->BSRR = GPIO_Pin_15;
} 
/*
 turn off all 8 leds
*/
void f3d_led_all_off(void) {  
  GPIOE->BRR = GPIO_Pin_8;
  GPIOE->BRR = GPIO_Pin_9;
  GPIOE->BRR = GPIO_Pin_10;
  GPIOE->BRR = GPIO_Pin_11;
  GPIOE->BRR = GPIO_Pin_12;
  GPIOE->BRR = GPIO_Pin_13;
  GPIOE->BRR = GPIO_Pin_14;
  GPIOE->BRR = GPIO_Pin_15;
} 

/* led.c ends here */

