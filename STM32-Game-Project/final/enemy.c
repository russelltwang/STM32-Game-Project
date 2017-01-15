/*Filename: enemy.c
 *Part of:C335 final project
 *Created By:Michael Ledesma<mledesma> and Russell Wang <ruswang>
 *On:04/20/2014
 *Last Modified by:Michael Ledesma
 *On:04/30/2014
 */
#include "enemy.h"



uint8_t esprite[10][6] = {
  {1,1,1,1,1,1},
  {1,1,0,0,1,1},
  {1,1,0,0,1,1},
  {0,0,1,1,0,0},
  {1,1,1,1,1,1},
  {1,1,1,1,1,1},
  {1,1,1,1,1,1},
  {0,0,1,1,0,0},
  {0,0,1,1,0,0},
  {0,1,1,1,1,0}
};

//moves enemy sprite in a specified direction.
void move_enemy(enemy *e, char direction) {
  int y,x;
  for (y = 0; y<=9;y++) {
    for (x = 0; x<=5;x++) {
      if (esprite[y][x]) {
	f3d_lcd_drawPixel(x+e->xcoord,y+e->ycoord,BLACK);
      }
    }
  }
  switch (direction) {
  case 'r':
    e->xcoord+= 4;
    e->steps++;
    break;
  case 'l':
    e->xcoord-= 4;
    e->steps++;
    break;
  case 'u':
    e->ycoord-= 4;
    e->steps++;
    break;
  case 'd':
    e->ycoord+= 4;
    e->steps++;
    break;
  }
  for (y = 0; y<=9;y++) {
    for (x = 0; x<=5;x++) {
      if (esprite[y][x]) {
	f3d_lcd_drawPixel(x+e->xcoord,y+e->ycoord,RED);
      }
    }
  }
 }

//moves enemy in a psueudorandom direction.
void move_enemy_random(enemy *e) {
  int y,x,index;
  char directions[4] = {'r','l','u','d'};
  char direction;
  for (y = 0; y<=9;y++) {
    for (x = 0; x<=5;x++) {
      if (esprite[y][x]) {
	f3d_lcd_drawPixel(x+e->xcoord,y+e->ycoord,BLACK);
      }
    }
  }
  //random number is generated in the range 0-3.
  index = rand();
  index %= 4;
  direction = directions[index];
  switch (direction) {
  case 'r':
    e->xcoord+= 4;
    e->steps++;
    break;
  case 'l':
    e->xcoord-= 4;
    e->steps++;
    break;
  case 'u':
    e->ycoord-= 4;
    e->steps++;
    break;
  case 'd':
    e->ycoord+= 4;
    e->steps++;
    break;
  }
  for (y = 0; y<=9;y++) {
    for (x = 0; x<=5;x++) {
      if (esprite[y][x]) {
	f3d_lcd_drawPixel(x+e->xcoord,y+e->ycoord,RED);
      }
    }
  }
 }


//clears enemy from the screen
void remove_enemy(enemy *e) {
  int x,y;
    for (y = 0; y<=9;y++) {
    for (x = 0; x<=5;x++) {
      if (esprite[y][x]) {
	f3d_lcd_drawPixel(x+e->xcoord,y+e->ycoord,BLACK);
      }
    }
  }
}




int enemy_collision(enemy *e, player *p) {
  int xdist = abs (e->xcoord - p->xcoord);
  int ydist = abs (e->ycoord - p->ycoord);
  if (xdist < 6 && ydist < 15) {
    return 1;
  } 
  else {
    return 0;
  }
}
