/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author: 
 * Maintainer: Michael Ledesma <mledesma> and Russell Wang <ruswang> 
 * Created: Thu Jan 10 11:23:43 2013
 * Last-Updated: 03/06/2014
 *           By: Michael Ledesma
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
/* Code: */

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <stdio.h>


#define TIMER 20000
//char axis_state(void);
//void axis_display(void);
char console_select(char current);
char button_select(char current);
void display_data(char current, float *DATA);
void light_leds(char current, float *DATA);
//char axis_select(uint8_t button_state, char current);

//from lab4 main

// Simple looping delay function
void delay(void) {
  int i = 1000000; //modified delay
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}



int main(void) { 

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  //Initializes gyro, led, uart, and user input button
  f3d_gyro_init();
  f3d_led_init();
  f3d_uart_init();

  //initializes user input button
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  printf("Reset\n");
  
  uint8_t button_state; //stores the status of user input
  char current = 'x';  //starting axis
  float DATA[3]; //float array for gyro data

  while (1) { 
    button_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
    if (button_state == 1)
      {
	current = button_select(current);
      }
    else 
      {
	current = console_select(current);
	//because it constantly polls for output,
	//getchar won't allow user input button
      }
    f3d_gyro_getdata(DATA);
    display_data(current, DATA);
    light_leds(current,DATA);
    
  }
}

//returns a char that progresses the sequence based on the current input.  
char button_select(char current) {
     switch(current)
    {
    case 'x':
      printf("switching axis to y...\n");
      current = 'y';
      f3d_led_all_off();
      return current;
      break;
    case 'y':
      printf("switching axis to z...\n");
      current = 'z';
      f3d_led_all_off();
      return current;
      break;
    case 'z':
      printf("switching axis to x...\n");
      current = 'x';
      f3d_led_all_off();
      return current;
      break;
    }
  return current;
}

//returns a character based on keyboard input.
char console_select(char current) {
  char c =  getchar();  
  switch(c)
    {
    case 'x':
      current = 'x';
      printf("current axis: %c\n",current);
      f3d_led_all_off();
      return current;
      break;
    case 'y':
      current = 'y';
      printf("current axis: %c\n",current);
      f3d_led_all_off();
      return current;
      break;
    case 'z':
      current = 'z';
      printf("current axis: %c\n",current);
      f3d_led_all_off();
      return current;
      break;
    default:
      return current;
      break;
    }
}

void display_data(char current, float *DATA)
{
  switch (current)
    {
    case 'x':
      printf("data for axis x: %f\n", DATA[0]);
      break;
    case 'y':
      printf("data for axis y: %f\n", DATA[1]);
      break;
    case 'z':
      printf("data for axis z: %f\n", DATA[2]);
      break;
  }
}
 
void light_leds(char current, float *DATA) {
  int i;
  switch (current)
    {
    case 'x':
      i = 0;
      break;
    case 'y':
      i = 1;
      break;
    case 'z':
      i = 2;
      break;
    }
  if (DATA[i] >= 5) { //light western leds
    f3d_led_all_off();     
    f3d_led_on(0);//north
    if (DATA[i] >= 55) {
      f3d_led_on(7);//nw
      delay();
    }
    if (DATA[i] >= 105) {
      f3d_led_on(6);//w
      delay();
    }
    if (DATA[i] >= 155) {
      f3d_led_on(5);//sw
      delay();
    }
    if (DATA[i] >= 205) {
      f3d_led_on(4);//s
      delay();
    } 
  }
  else if (DATA[i] <= -5) { //light eastern leds
    f3d_led_all_off();     
    f3d_led_on(0);//n
    if (DATA[i] <= -55) {
      f3d_led_on(1);//ne
      delay();
    }
    if (DATA[i] <= -105) {
      f3d_led_on(2);//se
      delay();
    }
    if (DATA[i] <= -155) {
      f3d_led_on(3);//e
      delay();
    }
    if (DATA[i] <= -205) {
      f3d_led_on(4);//s
      delay();
    }
  } 
  
}


#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */
