/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author: Russell Wang <ruswang>, Michael Ledesma <mledesma> 
 * Maintainer: 
 * Created: Thu Jan 10 11:23:43 2014
 * Last-Updated: Feb, 13 2014
 *           By: Michael Ledesma <mledesma> 
 *     Update #: 4
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
 * 02-10-2014 debugging pause L52
 * 
 */
/* Code: */

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers

// Simple looping delay function
void delay(void) {
  int i = 4000000; //modified delay
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}

int main(void)
{ 
  int index = 0; //initializes index for the led light positions
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_StructInit(&GPIO_InitStructure);
  //initializes all pins (LED)
  f3d_led_init();
  //initializes pin 0 (User input button)
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //toggles LED based on user input
  uint8_t button_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
  while (1) //loops indefinitely
    {
      while (button_state == 1) //when user input button is pressed
	{
	  button_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0); //termination case	
	  if (index == 8) {
	    f3d_led_all_on();
	  }
	  else {
	    f3d_led_on(index);
	  }
	  
	}

      //resets the sequence
      if (index == 8) {
	button_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0); 
	f3d_led_all_on();
	delay();
	f3d_led_all_off();
	index = 0; //conjecture: resetting the index here means we can't pause pin 9
      }

      //sequence flow
	f3d_led_on(index);	
	button_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0); 
	delay();
	f3d_led_off(index);
	button_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0); 
	index++;
      



      
      
      
     

      
      
      
          	    
	
      /*
	else {
	for(index = 0; index < 8; index++)
	  {
	    f3d_led_on(index);
	    delay();
	    f3d_led_off(index);
	  }
      }
    }
      */
    }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
}
#endif

/* main.c ends here */
