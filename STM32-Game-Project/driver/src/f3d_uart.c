/* f3d_uart.c --- 
 * 
 * Filename: f3d_uart.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: Michael Ledesma<mledesma>  and Russell Wang <ruswang>
 * Created: Thu Sep 26 07:05:26 2013
 * Last-Updated: 04/10/2014
 *           By: Michael Ledesma  <mledesma> 
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
 * 02/14 -configured UART1 I/O per lab 5 specifications
 *       -added getchar() and putchar() per lab 5 specifications
 * 02/17 -added comments
 * 04/08 -implemented interrupt driven uart 
 */

/* Copyright (c) 2004-2007 The Trustees of Indiana University and 
 * Indiana University Research and Technology Corporation.  
 * 
 * All rights reserved. 
 * 
 * Additional copyrights may follow 
 */

/* Code: */
#include "stm32f30x_misc.h"
#include <stm32f30x.h>
#include <f3d_uart.h>
#include <queue.h>


queue_t txbuf;
queue_t rxbuf;



void f3d_uart_init(void) {
  // Initialization routines related to UART1
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  USART_InitTypeDef USART_InitStructure;
//Initializes Clock for GPIOC
RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
//initializes TX
GPIO_StructInit(&GPIO_InitStructure);
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
GPIO_Init(GPIOC,&GPIO_InitStructure);
//Initializes RX
GPIO_StructInit(&GPIO_InitStructure);
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
GPIO_Init(GPIOC , &GPIO_InitStructure);
//initializes PC4 and PC5
GPIO_PinAFConfig(GPIOC,4,GPIO_AF_7);
GPIO_PinAFConfig(GPIOC,5,GPIO_AF_7);
//UART1 module config

RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
USART_StructInit(&USART_InitStructure);
USART_InitStructure.USART_BaudRate = 9600;
USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
USART_Init(USART1 ,&USART_InitStructure);
USART_Cmd(USART1 , ENABLE);

init_queue(&rxbuf);
init_queue(&txbuf);

// Setup the NVIC priority and subpriority
 NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x08;
 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x08;
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 NVIC_Init(&NVIC_InitStructure);
 
 // Enable the RX interrupt 
 USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
}

void flush_uart(void) {
  USART_ITConfig(USART1,USART_IT_TXE,ENABLE);
}



void USART1_IRQHandler(void) {
  int ch;

  if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE)) {
    ch = USART_ReceiveData(USART1);
    while (!enqueue(&rxbuf,ch));//if the ! is removed, will print
    //chars at 100hz and 10hz...
  }
  if (USART_GetFlagStatus(USART1,USART_FLAG_TXE)) {
    ch = dequeue(&txbuf);
    if (ch) {
      USART_SendData(USART1,ch);
    }
    else {
      USART_ITConfig(USART1,USART_IT_TXE,DISABLE);
    }
  }
}




int putchar(int c) {
  enqueue(&txbuf,c);
      return c;
} 

int getchar(void) {
  //int c;
  //if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == (uint16_t)SET) {
    int c = dequeue(&rxbuf);
    //}
    return c;
  
}

/*sends a string of characters across the terminal*/
void putstring(char *s) {
  while (*s) {
    putchar(*s);
    s++;
  }
}







/* f3d_uart.c ends here */
