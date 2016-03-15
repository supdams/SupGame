#ifndef _COLLISION_HPP_
#define _COLLISION_HPP_

#include "structs.hpp"



#include "collision.hpp"

int collision_decor(JeuComplet *jeucomplet , Anim *anim , SDL_Rect *t);
int test_tile_collision(JeuComplet *jeucomplet , int indicetile , SDL_Rect *t , Anim *anim);

int intersects(SDL_Rect box1 , SDL_Rect box2);

#endif