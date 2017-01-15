/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author: 
 * Maintainer: Michael Ledesma <mledesma> and Russell Wang <ruswang>
 * Created: Thu Jan 10 11:23:43 2013
 * Last-Updated: 03/10/2014
 *           By: Michael Ledesma <mledesma>, Russell Wang <ruswang>
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
 * 
 * 
 */
/* Code: */

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_lcd_sd.h>
#include <f3d_delay.h>
#include <f3d_led.h>
#include <f3d_gyro.h>
#include <stdio.h>

void drawgraph(float *fData, char current);
void display_data(float *fData, char current);
void line(void);

//functions from lab7
char button_select(char current);
void light_leds(float *DATA, char current);

int main(void) { 
  f3d_led_init(); //initialization for led lights.
  f3d_lcd_init();
  f3d_gyro_init(); //calling gyro initialization.
  
  //initializes user input button
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  float fData[3];//float array for gyro data
  uint8_t button_state; //stores the status of user input
  char current = 'x';  //starting axis
  //  char s[40];
  while (1)
    {
      button_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
      if (button_state == 1)
	{
	  current = button_select(current);
	}
      
      //gyro display
      f3d_gyro_getdata(fData);
      //refresh screen.
      f3d_lcd_fillScreen(0x0000);
      //display coordinates.
      line();      
      //display_data(fData, current);//causes a lot of lag
      f3d_lcd_drawChar(3, 35, '+', 0xFFFF, 0x0000); //acceleration mark
      f3d_lcd_drawChar(3, 105, '-', 0xFFFF, 0x0000); //deceleration mark
      f3d_lcd_drawChar(20, 145, 'x' ,0xFFFF, 0x0000); // x value
      f3d_lcd_drawChar(65, 145, 'y' ,0xFFFF, 0x0000); // y value
      f3d_lcd_drawChar(110, 145, 'z' ,0xFFFF, 0x0000); // z value
      //highlight the current axis.
      //highlight(current);
      drawgraph(fData, 'x'); //visual graph for x value
      drawgraph(fData, 'y'); //visual graph for y value
      drawgraph(fData, 'z'); //visual graph for z value
      light_leds(fData, current);
    }
}

//draws a line in the middle of the screen to differentiate positive and
//negative values.
void line(void)
{
  int i;
  for(i = 0; i < 126; i++)
    {
      f3d_lcd_drawPixel(i, 70, 0xFFFF);
    }
}

//displays gyro data at the top of screen for a given axis. 
void display_data(float *fData, char current) {
  char s[30];
  switch (current) {
  case 'x':
    sprintf(s, "x = %5f", fData[0]);//this is an expensive call
    f3d_lcd_drawString(20,3,s,0xFFFF,0x0000);
    delay(25);
    break;
  case 'y':
    sprintf(s, "y = %5f", fData[1]);
    f3d_lcd_drawString(20,3,s,0xFFFF,0x0000);
    delay(25);
    break;
  case 'z':
    sprintf(s, "z = %5f", fData[2]);
    f3d_lcd_drawString(20,3,s,0xFFFF,0x0000);
    delay(25);
    break;
  }
}




//draws a bar graph representation for a given axis.
void drawgraph(float *fData, char current) {
  int i;
  if (current == 'x') {
    if (fData[0] >= 5) {
      for (i = 14; i >= 0; i--) {
	f3d_lcd_drawPixel(18, 70 - i, 0xF800);
	f3d_lcd_drawPixel(19, 70 - i, 0xF800);
	f3d_lcd_drawPixel(20, 70 - i, 0xF800);
	f3d_lcd_drawPixel(21, 70 - i, 0xF800);
	f3d_lcd_drawPixel(22, 70 - i, 0xF800);
      }
    }
    if (fData[0] >= 55) {
      for (i = 28; i >= 0; i--) {
	f3d_lcd_drawPixel(18, 70 - i, 0xF800);
	f3d_lcd_drawPixel(19, 70 - i, 0xF800);
	f3d_lcd_drawPixel(20, 70 - i, 0xF800);
	f3d_lcd_drawPixel(21, 70 - i, 0xF800);
	f3d_lcd_drawPixel(22, 70 - i, 0xF800);
      }
    }
    if (fData[0] >= 105) {
      for (i = 37; i >= 0; i--) {
	f3d_lcd_drawPixel(18, 70 - i, 0xF800);
	f3d_lcd_drawPixel(19, 70 - i, 0xF800);
	f3d_lcd_drawPixel(20, 70 - i, 0xF800);
	f3d_lcd_drawPixel(21, 70 - i, 0xF800);
	f3d_lcd_drawPixel(22, 70 - i, 0xF800);
      }
    }
    if (fData[0] >= 155) {
      for (i = 56; i >= 0; i--) {
	f3d_lcd_drawPixel(18, 70 - i, 0xF800);
	f3d_lcd_drawPixel(19, 70 - i, 0xF800);
	f3d_lcd_drawPixel(20, 70 - i, 0xF800);
	f3d_lcd_drawPixel(21, 70 - i, 0xF800);
	f3d_lcd_drawPixel(22, 70 - i, 0xF800);
      }
    }
    if (fData[0] >= 205) {
      for (i = 70; i >= 0; i--) {
	f3d_lcd_drawPixel(18, 70 - i, 0xF800);
	f3d_lcd_drawPixel(19, 70 - i, 0xF800);
	f3d_lcd_drawPixel(20, 70 - i, 0xF800);
	f3d_lcd_drawPixel(21, 70 - i, 0xF800);
	f3d_lcd_drawPixel(22, 70 - i, 0xF800);
      }
    }
    if (fData[0] <= -5) {
      for (i = 14; i >= 0; i--) {
	f3d_lcd_drawPixel(18, 70 + i, 0xF800);
	f3d_lcd_drawPixel(19, 70 + i, 0xF800);
	f3d_lcd_drawPixel(20, 70 + i, 0xF800);
	f3d_lcd_drawPixel(21, 70 + i, 0xF800);
	f3d_lcd_drawPixel(22, 70 + i, 0xF800);
      }
    }
    if (fData[0] <= -55) {
      for (i = 28; i >= 0; i--) {
	f3d_lcd_drawPixel(18, 70 + i, 0xF800);
	f3d_lcd_drawPixel(19, 70 + i, 0xF800);
	f3d_lcd_drawPixel(20, 70 + i, 0xF800);
	f3d_lcd_drawPixel(21, 70 + i, 0xF800);
	f3d_lcd_drawPixel(22, 70 + i, 0xF800);
      }
    }
    if (fData[0] <= -105) {
      for (i = 37; i >= 0; i--) {
	f3d_lcd_drawPixel(18, 70 + i, 0xF800);
	f3d_lcd_drawPixel(19, 70 + i, 0xF800);
	f3d_lcd_drawPixel(20, 70 + i, 0xF800);
	f3d_lcd_drawPixel(21, 70 + i, 0xF800);
	f3d_lcd_drawPixel(22, 70 + i, 0xF800);
      }
    }
    if (fData[0] <= -155) {
      for (i = 56; i >= 0; i--) {
	f3d_lcd_drawPixel(18, 70 + i, 0xF800);
	f3d_lcd_drawPixel(19, 70 + i, 0xF800);
	f3d_lcd_drawPixel(20, 70 + i, 0xF800);
	f3d_lcd_drawPixel(21, 70 + i, 0xF800);
	f3d_lcd_drawPixel(22, 70 + i, 0xF800);
      }
    }
    if (fData[0] <= -205) {
      for (i = 70; i >= 0; i--) {
	f3d_lcd_drawPixel(18, 70 + i, 0xF800);
	f3d_lcd_drawPixel(19, 70 + i, 0xF800);
	f3d_lcd_drawPixel(20, 70 + i, 0xF800);
	f3d_lcd_drawPixel(21, 70 + i, 0xF800);
	f3d_lcd_drawPixel(22, 70 + i, 0xF800);
      }
    }
  }
  if (current == 'y') {
    if (fData[1] >= 5) {
      for (i = 14; i >= 0; i--) {
	f3d_lcd_drawPixel(63, 70 - i, 0x07E0);
	f3d_lcd_drawPixel(64, 70 - i, 0x07E0);
	f3d_lcd_drawPixel(65, 70 - i, 0x07E0);
	f3d_lcd_drawPixel(66, 70 - i, 0x07E0);
	f3d_lcd_drawPixel(67, 70 - i, 0x07E0);
      }
    }
    if (fData[1] >= 55) {
      for (i = 28; i >= 0; i--) {
	f3d_lcd_drawPixel(63, 70 - i, 0x07E0);
	f3d_lcd_drawPixel(64, 70 - i, 0x07E0);
	f3d_lcd_drawPixel(65, 70 - i, 0x07E0);
	f3d_lcd_drawPixel(66, 70 - i, 0x07E0);
	f3d_lcd_drawPixel(67, 70 - i, 0x07E0);
      }
    }
    if (fData[1] >= 105) {
      for (i = 37; i >= 0; i--) {
	f3d_lcd_drawPixel(63, 70 - i, 0x07E0);
	f3d_lcd_drawPixel(64, 70 - i, 0x07E0);
	f3d_lcd_drawPixel(65, 70 - i, 0x07E0);
	f3d_lcd_drawPixel(66, 70 - i, 0x07E0);
	f3d_lcd_drawPixel(67, 70 - i, 0x07E0);
      }
    }
    if (fData[1] >= 155) {
      for (i = 56; i >= 0; i--) {
	f3d_lcd_drawPixel(63, 70 - i, 0x07E0);
	f3d_lcd_drawPixel(64, 70 - i, 0x07E0);
	f3d_lcd_drawPixel(65, 70 - i, 0x07E0);
	f3d_lcd_drawPixel(66, 70 - i, 0x07E0);
	f3d_lcd_drawPixel(67, 70 - i, 0x07E0);
      }
    }
    if (fData[1] >= 205) {
      for (i = 70; i >= 0; i--) {
	f3d_lcd_drawPixel(63, 70 - i, 0x07E0);
	f3d_lcd_drawPixel(64, 70 - i, 0x07E0);
	f3d_lcd_drawPixel(65, 70 - i, 0x07E0);
	f3d_lcd_drawPixel(66, 70 - i, 0x07E0);
	f3d_lcd_drawPixel(67, 70 - i, 0x07E0);
      }
    }
    if (fData[1] <= -5) {
      for (i = 14; i >= 0; i--) {
	f3d_lcd_drawPixel(63, 70 + i, 0x07E0);
	f3d_lcd_drawPixel(64, 70 + i, 0x07E0);
	f3d_lcd_drawPixel(65, 70 + i, 0x07E0);
	f3d_lcd_drawPixel(66, 70 + i, 0x07E0);
	f3d_lcd_drawPixel(67, 70 + i, 0x07E0);
      }
    }
    if (fData[1] <= -55) {
      for (i = 28; i >= 0; i--) {
	f3d_lcd_drawPixel(63, 70 + i, 0x07E0);
	f3d_lcd_drawPixel(64, 70 + i, 0x07E0);
	f3d_lcd_drawPixel(65, 70 + i, 0x07E0);
	f3d_lcd_drawPixel(66, 70 + i, 0x07E0);
	f3d_lcd_drawPixel(67, 70 + i, 0x07E0);
      }
    }
    if (fData[1] <= -105) {
      for (i = 37; i >= 0; i--) {
	f3d_lcd_drawPixel(63, 70 + i, 0x07E0);
	f3d_lcd_drawPixel(64, 70 + i, 0x07E0);
	f3d_lcd_drawPixel(65, 70 + i, 0x07E0);
	f3d_lcd_drawPixel(66, 70 + i, 0x07E0);
	f3d_lcd_drawPixel(67, 70 + i, 0x07E0);
      }
    }
    if (fData[1] <= -155) {
      for (i = 56; i >= 0; i--) {
	f3d_lcd_drawPixel(63, 70 + i, 0x07E0);
	f3d_lcd_drawPixel(64, 70 + i, 0x07E0);
	f3d_lcd_drawPixel(65, 70 + i, 0x07E0);
	f3d_lcd_drawPixel(66, 70 + i, 0x07E0);
	f3d_lcd_drawPixel(67, 70 + i, 0x07E0);
      }
    }
    if (fData[1] <= -205) {
      for (i = 70; i >= 0; i--) {
	f3d_lcd_drawPixel(63, 70 + i, 0x07E0);
	f3d_lcd_drawPixel(64, 70 + i, 0x07E0);
	f3d_lcd_drawPixel(65, 70 + i, 0x07E0);
	f3d_lcd_drawPixel(66, 70 + i, 0x07E0);
	f3d_lcd_drawPixel(67, 70 + i, 0x07E0);
      } 
    }
  }
  if (current == 'z') {
    if (fData[2] >= 5) {
      for (i = 14; i >= 0; i--) {
	f3d_lcd_drawPixel(118, 70 - i, 0x001F);
	f3d_lcd_drawPixel(119, 70 - i, 0x001F);
	f3d_lcd_drawPixel(120, 70 - i, 0x001F);
	f3d_lcd_drawPixel(121, 70 - i, 0x001F);
	f3d_lcd_drawPixel(122, 70 - i, 0x001F);
      }
    }
    if (fData[2] >= 55) {
      for (i = 28; i >= 0; i--) {
	f3d_lcd_drawPixel(118, 70 - i, 0x001F);
	f3d_lcd_drawPixel(119, 70 - i, 0x001F);
	f3d_lcd_drawPixel(120, 70 - i, 0x001F);
	f3d_lcd_drawPixel(121, 70 - i, 0x001F);
	f3d_lcd_drawPixel(122, 70 - i, 0x001F);
      }
    }
    if (fData[2] >= 105) {
      for (i = 37; i >= 0; i--) {
	f3d_lcd_drawPixel(118, 70 - i, 0x001F);
	f3d_lcd_drawPixel(119, 70 - i, 0x001F);
	f3d_lcd_drawPixel(120, 70 - i, 0x001F);
	f3d_lcd_drawPixel(121, 70 - i, 0x001F);
	f3d_lcd_drawPixel(122, 70 - i, 0x001F);
      }
    }
    if (fData[2] >= 155) {
      for (i = 56; i >= 0; i--) {
	f3d_lcd_drawPixel(118, 70 - i, 0x001F);
	f3d_lcd_drawPixel(119, 70 - i, 0x001F);
	f3d_lcd_drawPixel(120, 70 - i, 0x001F);
	f3d_lcd_drawPixel(121, 70 - i, 0x001F);
	f3d_lcd_drawPixel(122, 70 - i, 0x001F);
      }
    }
    if (fData[2] >= 205) {
      for (i = 70; i >= 0; i--) {
	f3d_lcd_drawPixel(118, 70 - i, 0x001F);
	f3d_lcd_drawPixel(119, 70 - i, 0x001F);
	f3d_lcd_drawPixel(120, 70 - i, 0x001F);
	f3d_lcd_drawPixel(121, 70 - i, 0x001F);
	f3d_lcd_drawPixel(122, 70 - i, 0x001F);
      }
    }
    if (fData[2] <= -5) {
      for (i = 14; i >= 0; i--) {
	f3d_lcd_drawPixel(118, 70 + i, 0x001F);
	f3d_lcd_drawPixel(119, 70 + i, 0x001F);
	f3d_lcd_drawPixel(120, 70 + i, 0x001F);
	f3d_lcd_drawPixel(121, 70 + i, 0x001F);
	f3d_lcd_drawPixel(122, 70 + i, 0x001F);
      }
    }
    if (fData[2] <= -55) {
      for (i = 28; i >= 0; i--) {
	f3d_lcd_drawPixel(118, 70 + i, 0x001F);
	f3d_lcd_drawPixel(119, 70 + i, 0x001F);
	f3d_lcd_drawPixel(120, 70 + i, 0x001F);
	f3d_lcd_drawPixel(121, 70 + i, 0x001F);
	f3d_lcd_drawPixel(122, 70 + i, 0x001F);
      }
    }
    if (fData[2] <= -105) {
      for (i = 37; i >= 0; i--) {
	f3d_lcd_drawPixel(118, 70 + i, 0x001F);
	f3d_lcd_drawPixel(119, 70 + i, 0x001F);
	f3d_lcd_drawPixel(120, 70 + i, 0x001F);
	f3d_lcd_drawPixel(121, 70 + i, 0x001F);
	f3d_lcd_drawPixel(122, 70 + i, 0x001F);
      }
    }
    if (fData[2] <= -155) {
      for (i = 56; i >= 0; i--) {
	f3d_lcd_drawPixel(118, 70 + i, 0x001F);
	f3d_lcd_drawPixel(119, 70 + i, 0x001F);
	f3d_lcd_drawPixel(120, 70 + i, 0x001F);
	f3d_lcd_drawPixel(121, 70 + i, 0x001F);
	f3d_lcd_drawPixel(122, 70 + i, 0x001F);
      }
    }
    if (fData[2] <= -205) {
      for (i = 70; i >= 0; i--) {
	f3d_lcd_drawPixel(118, 70 + i, 0x001F);
	f3d_lcd_drawPixel(119, 70 + i, 0x001F);
	f3d_lcd_drawPixel(120, 70 + i, 0x001F);
	f3d_lcd_drawPixel(121, 70 + i, 0x001F);
	f3d_lcd_drawPixel(122, 70 + i, 0x001F);
      }
    }
  }
}


char button_select(char current) {
     switch(current)
    {
    case 'x':
      f3d_lcd_drawChar(65, 145, 'y' ,0xFFFF, 0x07E0); // y value
      delay(100);
      current = 'y';
      f3d_led_all_off();
      return current;
      break;
    case 'y':
      f3d_lcd_drawChar(110, 145, 'z' ,0xFFFF, 0x001F); // z value
      delay(100);
      current = 'z';
      f3d_led_all_off();
      return current;
      break;
    case 'z':
      f3d_lcd_drawChar(20, 145, 'x' ,0xFFFF, 0xF800); // x value
      delay(100);
      current = 'x';
      f3d_led_all_off();
      return current;
      break;
    }
  return current;
}


void light_leds(float *DATA, char current) {
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
      delay(50);
    }
    if (DATA[i] >= 105) {
      f3d_led_on(6);//w
      delay(50);
    }
    if (DATA[i] >= 155) {
      f3d_led_on(5);//sw
      delay(50);
    }
    if (DATA[i] >= 205) {
      f3d_led_on(4);//s
      delay(50);
    } 
  }
  else if (DATA[i] <= -5) { //light eastern leds
    f3d_led_all_off();     
    f3d_led_on(0);//n
    if (DATA[i] <= -55) {
      f3d_led_on(1);//ne
      delay(50);
    }
    if (DATA[i] <= -105) {
      f3d_led_on(2);//se
      delay(50);
    }
    if (DATA[i] <= -155) {
      f3d_led_on(3);//e
      delay(50);
    }
    if (DATA[i] <= -205) {
      f3d_led_on(4);//s
      delay(50);
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
