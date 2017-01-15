/*Filename: enemy.h
 *Part of:C335 final project
 *Created By:Michael Ledesma<mledesma> and Russell Wang <ruswang>
 *On:04/20/2014
 *Last Modified by:Michael Ledesma
 *On:04/30/2014
 */


#include <player.h>
#include <math.h>


void move_enemy(enemy *e, char direction);
int enemy_collision(enemy *e, player *p);
void remove_enemy(enemy *e);
void move_enemy_random(enemy *e); 




