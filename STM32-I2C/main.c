/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author: 
 * Maintainer: Michael Ledesma <mledesma> and Russell Wang <ruswang> 
 * Created: Thu Jan 10 11:23:43 2013
 * Last-Updated: 03/27/2014
 *           By: Michael Ledesma
 *     Update #: 
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
#include <stdio.h>
#include <stm32f30x.h>  // Pull in include files for F30x standard drivers
#include <f3d_gyro.h>
#include <f3d_delay.h> 
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_nunchuk.h>
#include <f3d_mag.h>
#include <f3d_lcd_sd.h>
#include <math.h>

int menu_Select(nunchuk_t data);
void display_Gyro(float gyrodata[]);
void compass_Heading(float adata[], float mdata[]);
void dot_location(int xpos, int ypos, uint16_t color);
void onboard_Tilt(float adata[]);
void nunchuk_Tilt(nunchuk_t data);
void draw_Graph_Gyro(float data[], char current);
void draw_Graph_f3acc(float data[], char current);
int main(void) { 
  nunchuk_t ndata; //struct defined in f3d_nunchuk.h
  //i2c peripheral initialization
  f3d_i2c1_init();
  delay(10);
  f3d_accel_init();
  delay(10);
  f3d_mag_init();
  delay(10);
  f3d_nunchuk_init();
  delay(10);

  //spi peripheral intitialization
  f3d_gyro_init();
  f3d_lcd_init();
  
  //fill screen (black)
  f3d_lcd_fillScreen(0x0000);

  //for application selection
  uint8_t choice;
  
  //arrays for gyroscope, accelerometer, and magnetometer data collection
  float gdata[3];
  float accel_data[3];
  float mag_data[3];
 
  while (1)
    {     
      //read nunchuk data
      f3d_nunchuk_read(&ndata);
      //menu selection
      choice = menu_Select(ndata);
      while (ndata.c != 1 || ndata.z != 1)
	{
	  switch(choice) {
	  case 0:
	    display_Gyro(gdata);
	    break;
	  case 1:
	    compass_Heading(accel_data, mag_data);
	    break;
	  case 2:
	    onboard_Tilt(accel_data);
	    break;
	  case 3:
	    nunchuk_Tilt(ndata);
	    break;
	  }
	  f3d_nunchuk_read(&ndata);
	}
      
    } 
}

//menu select interface (default screen)

int menu_Select(nunchuk_t data) {
  //refreshes the screen upon return
  f3d_lcd_fillScreen(0x0000);
  
  //  char selectiondisp[15];
  //will cycle through each selection 4mod(selection)
  int selection = 0;
  //initial y position for the cursor
  int ycoord = 25;
  //menu display
  f3d_lcd_drawString(25,5,"Lab 9 Menu",0xFFFF,0x0000);
  f3d_lcd_drawString(15,25,"1.gyro data",0xFFFF,0x0000);
  f3d_lcd_drawString(15,45,"2.compass heading",0xFFFF,0x0000);
  f3d_lcd_drawString(15,65,"3.onboard tilt",0xFFFF,0x0000);
  f3d_lcd_drawString(15,85,"4.nunchuk tilt",0xFFFF,0x0000);
  f3d_lcd_drawChar(5, ycoord,'>',0xFFFF,0x0000);
  //c selects
  while (data.c != 1) {
  f3d_nunchuk_read(&data);
  if (data.jy >= 245) {
    selection--;
  }
  if (data.jy <= 10) {
    selection++;
  }

  delay(100);
  selection = selection % 4;
  f3d_lcd_drawString(15,105,"c button selects",0xFFFF,0x0000);
  f3d_lcd_drawString(20,115,"z + c returns",0xFFFF,0x0000);
  switch (selection) {
  case 0:
    f3d_lcd_drawChar(5, ycoord,' ',0xFFFF,0x0000);
    ycoord = 25;
    f3d_lcd_drawChar(5, ycoord,'>',0xFFFF,0x0000);
    break;
  case 1:
    f3d_lcd_drawChar(5, ycoord,' ',0xFFFF,0x0000);
    ycoord = 45;
    f3d_lcd_drawChar(5, ycoord,'>',0xFFFF,0x0000);
    break;
  case 2:
    f3d_lcd_drawChar(5, ycoord,' ',0xFFFF,0x0000);
    ycoord = 65;
    f3d_lcd_drawChar(5, ycoord,'>',0xFFFF,0x0000);
    break;
  case 3:
    f3d_lcd_drawChar(5, ycoord,' ',0xFFFF,0x0000);
    ycoord = 85;
    f3d_lcd_drawChar(5, ycoord,'>',0xFFFF,0x0000);
    break;
  }
  }
  f3d_lcd_fillScreen(0x0000);
  return selection % 4;
}

void display_Gyro(float gyrodata[]) {
  int i;
  char xd[10];
  char yd[10];
  char zd[10];
  for (i = 0; i < 126; i++) {
    f3d_lcd_drawPixel(i,70,0xFFFF);
  }
  f3d_lcd_drawString(5,5,"Gyroscope Data:",0xFFFF,0x0000);
  f3d_gyro_getdata(gyrodata);
  sprintf(xd, "x = %5f", gyrodata[0]);
  f3d_lcd_drawString(20,133,xd,0xF800,0x0000);
  sprintf(yd, "y = %5f", gyrodata[1]);
  f3d_lcd_drawString(20,143,yd,0x07E0,0x0000);
  sprintf(zd, "z = %5f", gyrodata[2]);
  f3d_lcd_drawString(20,153,zd,0x001F,0x0000);
  draw_Graph_Gyro(gyrodata, 'x');
  draw_Graph_Gyro(gyrodata, 'y');
  draw_Graph_Gyro(gyrodata, 'z');
      
}



void compass_Heading(float adata[],float mdata[]) {
  char pitch[40];
  char yaw[40];
  char roll[40];
  char cdisp[40];

  float board_pitch;
  float board_roll;
  float board_yaw;
  float xh, yh, heading;

  f3d_lcd_drawChar(64,0, 'N',0xFFFF,0x0000);
  f3d_lcd_drawChar(64,123,'S',0xFFFF,0x0000);
  f3d_lcd_drawChar(123,64,'E',0xFFFF,0x0000);
  f3d_lcd_drawChar(0,64,'W',0xFFFF,0x0000);
  int i;
  for (i = 10; i <= 118; i++) {
    f3d_lcd_drawPixel(i,(128/118)*i,0xFFFF);
    f3d_lcd_drawPixel(i,(-128/118)*i+128,0xFFFF);
  }
  for (i = 10; i <= 118; i++) {
    f3d_lcd_drawPixel(i,64,0xFFFF);
    f3d_lcd_drawPixel(64,i,0xFFFF);
  }
      
  //read F3 board data
  f3d_accel_read(adata);
  f3d_mag_read(mdata);
  
  //F3 board tilt values
  board_pitch = asin(adata[0]/9.81);      
 
  board_roll = asin(adata[1]/9.81);
 
  board_yaw = asin(adata[2]/9.81);
 
  
  //F3 heading calculation
  xh = mdata[0]*cos(board_pitch) + mdata[2]*sin(board_pitch);
  yh = mdata[0]*sin(board_roll)*sin(board_pitch)+ mdata[1]*cos(board_roll);
  heading = (atan2(yh, xh) * 180) / M_PI;
  f3d_lcd_drawString(15, 143, "Compass Heading", 0xFFFF, 0x0000);
  sprintf(cdisp, "%5f", heading); //converted to degrees
  f3d_lcd_drawString(35, 153, cdisp, 0xFFFF, 0x0000);

  
  //north
  if (heading >= 0 && heading <= 10 ||
      heading >= -10 && heading <= 0 )
    {
      dot_location(63, 10, 0xF800);
    }
  //northeast
  if (heading >= 45  && heading <= 55)
    {
      dot_location(112,10, 0xF800);
    }
  //east
  if (heading >= 90 && heading <= 100)
    {
      dot_location(112,63, 0xF800);
    }
  //southeast
  if (heading >= 135 && heading <= 145)
    {
      dot_location(122,122, 0xF800);
    }
  //south
  if (heading >= 180 && heading <= 190 ||
      heading >= -180 && heading <= -170)
  {
    dot_location(63,112, 0xF800);
  }
//southwest
  if (heading >= -145 && heading <= -135)      
    {
      dot_location(5,112, 0xF800);
    }
  //west
  if (heading >= -100 && heading <= -90)
      
    {
      dot_location(10,63, 0xF800);
    }
  //northwest
  if (heading >= -55 && heading <= -45)      
    {
      dot_location(5,5, 0xF800);
    }
}



void dot_location(int xpos, int ypos, uint16_t color)
{
  int i,j;
  //graphical display
  for(i = xpos; i < xpos + 6; i++)
	{
	  for(j = ypos; j < ypos + 6; j++)
	    {
	      f3d_lcd_drawPixel(i,j,color);
	    }
	}
   delay(100);
  for(i = xpos; i < xpos + 6; i++)
  {
    for(j = ypos; j < ypos + 6; j++)
  	{
  	  f3d_lcd_drawPixel(i,j,0x0000);
  	}
    }
  
}
  

void onboard_Tilt(float adata[]) {
  int i;
  char pitch[40];
  char yaw[40];
  char roll[40];

  float board_pitch;
  float board_roll;
  float board_yaw;
  f3d_lcd_drawString(5,5,"onboard:",0xFFFF,0x0000);
  f3d_accel_read(adata);

   for (i = 0; i < 126; i++) {
    f3d_lcd_drawPixel(i,70,0x07E0);
    f3d_lcd_drawPixel(64,i,0xF800);
  }
 
 //F3 board tilt values
   board_pitch = asin(adata[0]/9.81);      
  sprintf(pitch, "pitch = %f", board_pitch);
  f3d_lcd_drawString(5,130,pitch,0xF800,0x0000);
  board_roll = asin(adata[1]/9.81);
  sprintf(roll, "roll = %f", board_roll);
  f3d_lcd_drawString(5,140,roll,0x07E0,0x0000);
  board_yaw = asin(adata[2]/9.81);
  sprintf(yaw, "yaw = %f", board_yaw);
  f3d_lcd_drawString(5,150,yaw,0x001F,0x0000);

  if ((board_roll >= .001 && board_roll < .04) &&
      (board_pitch >= .001 && board_pitch < .04)) {
    dot_location(64,70, 0xFFFF);
  }
  if (board_roll <= -.04 & board_roll > -.08) {
      dot_location(93,70, 0x07E0);
    }
  if (board_roll <= -.08) {
    dot_location(122,70, 0x07E0);
  }
  if (board_roll >= .04 && board_roll < .08) {
    dot_location(32,70, 0x07E0);
  }
  if (board_roll >= .08) {
    dot_location(0,70, 0x07E0);
  }

  if (board_pitch >= .04 && board_pitch < .08) {
    dot_location(64,35, 0xF800);
  }
  if (board_pitch >= .08) {
    dot_location(64,0, 0xF800);
  }
 if (board_pitch <= -.04 & board_pitch > -.08) {
      dot_location(64,102, 0xF800);
    }
  if (board_pitch <= -.08) {
    dot_location(64,122, 0xF800);
  }
  
}

void nunchuk_Tilt(nunchuk_t data) {
  int i;
  
 //character arrays to display nunchuk data
  char n_pitch[15];
  char n_yaw[15];
  char n_roll[15];
  //floats for nunchuk accelerometer data:
  float chuk_pitch;//rotation around the x axis 
  float chuk_roll;//rotation around the y axis
  float chuk_yaw;//rotation around the z axis

  for (i = 0; i < 126; i++) {
    f3d_lcd_drawPixel(i,70,0xF800);
    f3d_lcd_drawPixel(64,i,0x07E0);
  }
  //pitch,roll, and yaw values for nunchuk (in radians)      
  chuk_pitch = atan (data.ax / sqrt(pow(data.ay, 2) + pow(data.az, 2)));
  sprintf(n_pitch, "pitch = %5f", chuk_pitch);
  f3d_lcd_drawString(5,130,n_pitch,0xF800,0x0000);
  chuk_roll = (data.ay / sqrt(pow(data.ax, 2) + pow(data.az, 2))); 
  sprintf(n_roll, "roll = %5f", chuk_roll);
  f3d_lcd_drawString(5,140,n_roll,0x07E0,0x0000);
  chuk_yaw = atan(data.az / sqrt(pow(data.ax, 2) + pow(data.ay, 2)));
  sprintf(n_yaw, "yaw = %5f", chuk_yaw);
  f3d_lcd_drawString(5,150,n_yaw,0x001F,0x0000);

  if (chuk_pitch >= .3 && chuk_pitch < .5) {
    dot_location(0,70, 0xF800);
  }
  if (chuk_pitch >= .5 && chuk_pitch < .7) {
      dot_location(64,70, 0xF800);
    }
  if (chuk_pitch >= .7) {
    dot_location(122,70, 0xF800);
  }
  
  if (chuk_roll >= .3 && chuk_roll < .5) {
    dot_location(64,122, 0x07E0);
  }
  if (chuk_roll >= .5 && chuk_roll < .7) {
    dot_location(64,70, 0x07E0);
  }
  if (chuk_roll >= .7) {
    dot_location(64,0, 0x07E0);
  }
  
}


//Bar graph for gyro data
void draw_Graph_Gyro(float data[], char current) {
  int i,c,xstart;
  uint16_t color;
  switch(current) {
  case 'x':
    c = 0;
    xstart = 18;
    color = 0xF800;
    break;
  case 'y':
    c = 1;
    xstart = 63;
    color = 0x07E0;
    break;
  case 'z':
    c = 2;
    xstart = 118;
    color = 0x001F;
    break;
  }

  if (data[c] >= 5) {
      for (i = 14; i >= 0; i--) {
	f3d_lcd_drawPixel(xstart, 70 - i, color);
	f3d_lcd_drawPixel(xstart + 1, 70 - i, color);
	f3d_lcd_drawPixel(xstart + 2, 70 - i, color);
	f3d_lcd_drawPixel(xstart + 3, 70 - i, color);
	f3d_lcd_drawPixel(xstart + 4, 70 - i, color);
      }
      delay(100);
      for (i = 14; i >= 0; i--) {
	f3d_lcd_drawPixel(xstart, 70 - i, 0x0000);
	f3d_lcd_drawPixel(xstart + 1, 70 - i, 0x0000);
	f3d_lcd_drawPixel(xstart + 2, 70 - i, 0x0000);
	f3d_lcd_drawPixel(xstart + 3, 70 - i, 0x0000);
	f3d_lcd_drawPixel(xstart + 4, 70 - i, 0x0000);
      }      
  }

  if (data[c] >= 55) {
    for (i = 28; i >= 0; i--) {
      f3d_lcd_drawPixel(xstart, 70 - i, color);
      f3d_lcd_drawPixel(xstart + 1, 70 - i, color);
	f3d_lcd_drawPixel(xstart + 2, 70 - i, color);
	f3d_lcd_drawPixel(xstart + 3, 70 - i, color);
	f3d_lcd_drawPixel(xstart + 4, 70 - i, color);
    }
    delay(100);
    for (i = 28; i >= 0; i--) {
      f3d_lcd_drawPixel(xstart, 70 - i, 0x0000);
      f3d_lcd_drawPixel(xstart + 1, 70 - i, 0x0000);
      f3d_lcd_drawPixel(xstart + 2, 70 - i, 0x0000);
      f3d_lcd_drawPixel(xstart + 3, 70 - i, 0x0000);
      f3d_lcd_drawPixel(xstart + 4, 70 - i, 0x0000);
    } 
  }
  if (data[c] >= 105) {
    for (i = 37; i >= 0; i--) {
	f3d_lcd_drawPixel(xstart, 70 - i, color);
	f3d_lcd_drawPixel(xstart + 1, 70 - i, color);
	f3d_lcd_drawPixel(xstart + 2, 70 - i, color);
	f3d_lcd_drawPixel(xstart + 3, 70 - i, color);
	f3d_lcd_drawPixel(xstart + 4, 70 - i, color);
      }
      delay(100);
      for (i = 37; i >= 0; i--) {
	f3d_lcd_drawPixel(xstart, 70 - i, 0x0000);
	f3d_lcd_drawPixel(xstart + 1, 70 - i, 0x0000);
	f3d_lcd_drawPixel(xstart + 2, 70 - i, 0x0000);
	f3d_lcd_drawPixel(xstart + 3, 70 - i, 0x0000);
	f3d_lcd_drawPixel(xstart + 4, 70 - i, 0x0000);
      } 
  }

  if (data[c] >= 155) {
      for (i = 56; i >= 0; i--) {
	f3d_lcd_drawPixel(xstart, 70 - i, color);
	f3d_lcd_drawPixel(xstart + 1, 70 - i, color);
	f3d_lcd_drawPixel(xstart + 2, 70 - i, color);
	f3d_lcd_drawPixel(xstart + 3, 70 - i, color);
	f3d_lcd_drawPixel(xstart + 4, 70 - i, color);
      }
      delay(100);
      for (i = 56; i >= 0; i--) {
	f3d_lcd_drawPixel(xstart, 70 - i, 0x0000);
	f3d_lcd_drawPixel(xstart + 1, 70 - i, 0x0000);
	f3d_lcd_drawPixel(xstart + 2, 70 - i, 0x0000);
	f3d_lcd_drawPixel(xstart + 3, 70 - i, 0x0000);
	f3d_lcd_drawPixel(xstart + 4, 70 - i, 0x0000);
      } 
  }
  if (data[c] >= 205) {
      for (i = 70; i >= 0; i--) {
	f3d_lcd_drawPixel(xstart, 70 - i, color);
	f3d_lcd_drawPixel(xstart + 1, 70 - i, color);
	f3d_lcd_drawPixel(xstart + 2, 70 - i, color);
	f3d_lcd_drawPixel(xstart + 3, 70 - i, color);
	f3d_lcd_drawPixel(xstart + 4, 70 - i, color);
      }
      delay(100);
      for (i = 70; i >= 0; i--) {
	f3d_lcd_drawPixel(xstart, 70 - i, 0x0000);
	f3d_lcd_drawPixel(xstart + 1, 70 - i, 0x0000);
	f3d_lcd_drawPixel(xstart + 2, 70 - i, 0x0000);
	f3d_lcd_drawPixel(xstart + 3, 70 - i, 0x0000);
	f3d_lcd_drawPixel(xstart + 4, 70 - i, 0x0000);
      } 
  }
  if (data[c] <= -5) {
    for (i = 14; i >= 0; i--) {
      f3d_lcd_drawPixel(xstart, 70 + i, color);
      f3d_lcd_drawPixel(xstart + 1, 70 + i, color);
      f3d_lcd_drawPixel(xstart + 2, 70 + i, color);
      f3d_lcd_drawPixel(xstart + 3, 70 + i, color);
      f3d_lcd_drawPixel(xstart + 4, 70 + i, color);
    }
    delay(100);
    for (i = 14; i >= 0; i--) {
      f3d_lcd_drawPixel(xstart, 70 + i, 0x0000);
      f3d_lcd_drawPixel(xstart + 1, 70 + i, 0x0000);
      f3d_lcd_drawPixel(xstart + 2, 70 + i, 0x0000);
      f3d_lcd_drawPixel(xstart + 3, 70 + i, 0x0000);
      f3d_lcd_drawPixel(xstart + 4, 70 + i, 0x0000);
    } 
  }
  if (data[c] <= -55) {
    for (i = 28; i >= 0; i--) {
      f3d_lcd_drawPixel(xstart, 70 + i, color);
      f3d_lcd_drawPixel(xstart + 1, 70 + i, color);
      f3d_lcd_drawPixel(xstart + 2, 70 + i, color);
      f3d_lcd_drawPixel(xstart + 3, 70 + i, color);
      f3d_lcd_drawPixel(xstart + 4, 70 + i, color);
    }
    delay(100);
    for (i = 28; i >= 0; i--) {
      f3d_lcd_drawPixel(xstart, 70 + i, 0x0000);
      f3d_lcd_drawPixel(xstart + 1, 70 + i, 0x0000);
      f3d_lcd_drawPixel(xstart + 2, 70 + i, 0x0000);
      f3d_lcd_drawPixel(xstart + 3, 70 + i, 0x0000);
      f3d_lcd_drawPixel(xstart + 4, 70 + i, 0x0000);
    } 
  }
  if (data[c] <= -105) {
    for (i = 37; i >= 0; i--) {
      f3d_lcd_drawPixel(xstart, 70 + i, color);
      f3d_lcd_drawPixel(xstart + 1, 70 + i, color);
      f3d_lcd_drawPixel(xstart + 2, 70 + i, color);
      f3d_lcd_drawPixel(xstart + 3, 70 + i, color);
      f3d_lcd_drawPixel(xstart + 4, 70 + i, color);
    }
    delay(100);
    for (i = 37; i >= 0; i--) {
      f3d_lcd_drawPixel(xstart, 70 + i, 0x0000);
      f3d_lcd_drawPixel(xstart + 1, 70 + i, 0x0000);
      f3d_lcd_drawPixel(xstart + 2, 70 + i, 0x0000);
      f3d_lcd_drawPixel(xstart + 3, 70 + i, 0x0000);
      f3d_lcd_drawPixel(xstart + 4, 70 + i, 0x0000);
    } 
  }
  if (data[c] <= -155) {
    for (i = 56; i >= 0; i--) {
      f3d_lcd_drawPixel(xstart, 70 + i, color);
      f3d_lcd_drawPixel(xstart + 1, 70 + i, color);
      f3d_lcd_drawPixel(xstart + 2, 70 + i, color);
      f3d_lcd_drawPixel(xstart + 3, 70 + i, color);
      f3d_lcd_drawPixel(xstart + 4, 70 + i, color);
    }
    delay(100);
    for (i = 56; i >= 0; i--) {
      f3d_lcd_drawPixel(xstart, 70 + i, 0x0000);
      f3d_lcd_drawPixel(xstart + 1, 70 + i, 0x0000);
      f3d_lcd_drawPixel(xstart + 2, 70 + i, 0x0000);
      f3d_lcd_drawPixel(xstart + 3, 70 + i, 0x0000);
      f3d_lcd_drawPixel(xstart + 4, 70 + i, 0x0000);
    } 
  }
  if (data[c] <= -205) {
    for (i = 70; i >= 0; i--) {
      f3d_lcd_drawPixel(xstart, 70 + i, color);
      f3d_lcd_drawPixel(xstart + 1, 70 + i, color);
      f3d_lcd_drawPixel(xstart + 2, 70 + i, color);
      f3d_lcd_drawPixel(xstart + 3, 70 + i, color);
      f3d_lcd_drawPixel(xstart + 4, 70 + i, color);
    }
    delay(100);
    for (i = 70; i >= 0; i--) {
      f3d_lcd_drawPixel(xstart, 70 + i, 0x0000);
      f3d_lcd_drawPixel(xstart + 1, 70 + i, 0x0000);
      f3d_lcd_drawPixel(xstart + 2, 70 + i, 0x0000);
      f3d_lcd_drawPixel(xstart + 3, 70 + i, 0x0000);
      f3d_lcd_drawPixel(xstart + 4, 70 + i, 0x0000);
    } 
  }
}

void draw_Graph_f3acc(float data[], char current) {
  int i,c,xstart;
  uint16_t color;
  switch(current) {
  case 'x':
    c = 0;
    xstart = 18;
    color = 0xF800;
    break;
  case 'y':
    c = 1;
    xstart = 63;
    color = 0x07E0;
    break;
  case 'z':
    c = 2;
    xstart = 118;
    color = 0x001F;
    break;
  }

  if (data[c] >= .03) {
      for (i = 14; i >= 0; i--) {
	f3d_lcd_drawPixel(xstart, 70 - i, color);
	f3d_lcd_drawPixel(xstart + 1, 70 - i, color);
	f3d_lcd_drawPixel(xstart + 2, 70 - i, color);
	f3d_lcd_drawPixel(xstart + 3, 70 - i, color);
	f3d_lcd_drawPixel(xstart + 4, 70 - i, color);
      }
      delay(100);
      for (i = 14; i >= 0; i--) {
	f3d_lcd_drawPixel(xstart, 70 - i, 0x0000);
	f3d_lcd_drawPixel(xstart + 1, 70 - i, 0x0000);
	f3d_lcd_drawPixel(xstart + 2, 70 - i, 0x0000);
	f3d_lcd_drawPixel(xstart + 3, 70 - i, 0x0000);
	f3d_lcd_drawPixel(xstart + 4, 70 - i, 0x0000);
      }      
  }

  if (data[c] >= .05) {
    for (i = 28; i >= 0; i--) {
      f3d_lcd_drawPixel(xstart, 70 - i, color);
      f3d_lcd_drawPixel(xstart + 1, 70 - i, color);
	f3d_lcd_drawPixel(xstart + 2, 70 - i, color);
	f3d_lcd_drawPixel(xstart + 3, 70 - i, color);
	f3d_lcd_drawPixel(xstart + 4, 70 - i, color);
    }
    delay(100);
    for (i = 28; i >= 0; i--) {
      f3d_lcd_drawPixel(xstart, 70 - i, 0x0000);
      f3d_lcd_drawPixel(xstart + 1, 70 - i, 0x0000);
      f3d_lcd_drawPixel(xstart + 2, 70 - i, 0x0000);
      f3d_lcd_drawPixel(xstart + 3, 70 - i, 0x0000);
      f3d_lcd_drawPixel(xstart + 4, 70 - i, 0x0000);
    } 
  }
  if (data[c] >= .07) {
    for (i = 37; i >= 0; i--) {
	f3d_lcd_drawPixel(xstart, 70 - i, color);
	f3d_lcd_drawPixel(xstart + 1, 70 - i, color);
	f3d_lcd_drawPixel(xstart + 2, 70 - i, color);
	f3d_lcd_drawPixel(xstart + 3, 70 - i, color);
	f3d_lcd_drawPixel(xstart + 4, 70 - i, color);
      }
      delay(100);
      for (i = 37; i >= 0; i--) {
	f3d_lcd_drawPixel(xstart, 70 - i, 0x0000);
	f3d_lcd_drawPixel(xstart + 1, 70 - i, 0x0000);
	f3d_lcd_drawPixel(xstart + 2, 70 - i, 0x0000);
	f3d_lcd_drawPixel(xstart + 3, 70 - i, 0x0000);
	f3d_lcd_drawPixel(xstart + 4, 70 - i, 0x0000);
      } 
  }

  if (data[c] >= .1) {
      for (i = 56; i >= 0; i--) {
	f3d_lcd_drawPixel(xstart, 70 - i, color);
	f3d_lcd_drawPixel(xstart + 1, 70 - i, color);
	f3d_lcd_drawPixel(xstart + 2, 70 - i, color);
	f3d_lcd_drawPixel(xstart + 3, 70 - i, color);
	f3d_lcd_drawPixel(xstart + 4, 70 - i, color);
      }
      delay(100);
      for (i = 56; i >= 0; i--) {
	f3d_lcd_drawPixel(xstart, 70 - i, 0x0000);
	f3d_lcd_drawPixel(xstart + 1, 70 - i, 0x0000);
	f3d_lcd_drawPixel(xstart + 2, 70 - i, 0x0000);
	f3d_lcd_drawPixel(xstart + 3, 70 - i, 0x0000);
	f3d_lcd_drawPixel(xstart + 4, 70 - i, 0x0000);
      } 
  }

  if (data[c] <= -.03) {
    for (i = 14; i >= 0; i--) {
      f3d_lcd_drawPixel(xstart, 70 + i, color);
      f3d_lcd_drawPixel(xstart + 1, 70 + i, color);
      f3d_lcd_drawPixel(xstart + 2, 70 + i, color);
      f3d_lcd_drawPixel(xstart + 3, 70 + i, color);
      f3d_lcd_drawPixel(xstart + 4, 70 + i, color);
    }
    delay(100);
    for (i = 14; i >= 0; i--) {
      f3d_lcd_drawPixel(xstart, 70 + i, 0x0000);
      f3d_lcd_drawPixel(xstart + 1, 70 + i, 0x0000);
      f3d_lcd_drawPixel(xstart + 2, 70 + i, 0x0000);
      f3d_lcd_drawPixel(xstart + 3, 70 + i, 0x0000);
      f3d_lcd_drawPixel(xstart + 4, 70 + i, 0x0000);
    } 
  }
  if (data[c] <= -.05) {
    for (i = 28; i >= 0; i--) {
      f3d_lcd_drawPixel(xstart, 70 + i, color);
      f3d_lcd_drawPixel(xstart + 1, 70 + i, color);
      f3d_lcd_drawPixel(xstart + 2, 70 + i, color);
      f3d_lcd_drawPixel(xstart + 3, 70 + i, color);
      f3d_lcd_drawPixel(xstart + 4, 70 + i, color);
    }
    delay(100);
    for (i = 28; i >= 0; i--) {
      f3d_lcd_drawPixel(xstart, 70 + i, 0x0000);
      f3d_lcd_drawPixel(xstart + 1, 70 + i, 0x0000);
      f3d_lcd_drawPixel(xstart + 2, 70 + i, 0x0000);
      f3d_lcd_drawPixel(xstart + 3, 70 + i, 0x0000);
      f3d_lcd_drawPixel(xstart + 4, 70 + i, 0x0000);
    } 
  }
  if (data[c] <= -.07) {
    for (i = 37; i >= 0; i--) {
      f3d_lcd_drawPixel(xstart, 70 + i, color);
      f3d_lcd_drawPixel(xstart + 1, 70 + i, color);
      f3d_lcd_drawPixel(xstart + 2, 70 + i, color);
      f3d_lcd_drawPixel(xstart + 3, 70 + i, color);
      f3d_lcd_drawPixel(xstart + 4, 70 + i, color);
    }
    delay(100);
    for (i = 37; i >= 0; i--) {
      f3d_lcd_drawPixel(xstart, 70 + i, 0x0000);
      f3d_lcd_drawPixel(xstart + 1, 70 + i, 0x0000);
      f3d_lcd_drawPixel(xstart + 2, 70 + i, 0x0000);
      f3d_lcd_drawPixel(xstart + 3, 70 + i, 0x0000);
      f3d_lcd_drawPixel(xstart + 4, 70 + i, 0x0000);
    } 
  }
  if (data[c] <= -.1) {
    for (i = 56; i >= 0; i--) {
      f3d_lcd_drawPixel(xstart, 70 + i, color);
      f3d_lcd_drawPixel(xstart + 1, 70 + i, color);
      f3d_lcd_drawPixel(xstart + 2, 70 + i, color);
      f3d_lcd_drawPixel(xstart + 3, 70 + i, color);
      f3d_lcd_drawPixel(xstart + 4, 70 + i, color);
    }
    delay(100);
    for (i = 56; i >= 0; i--) {
      f3d_lcd_drawPixel(xstart, 70 + i, 0x0000);
      f3d_lcd_drawPixel(xstart + 1, 70 + i, 0x0000);
      f3d_lcd_drawPixel(xstart + 2, 70 + i, 0x0000);
      f3d_lcd_drawPixel(xstart + 3, 70 + i, 0x0000);
      f3d_lcd_drawPixel(xstart + 4, 70 + i, 0x0000);
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
