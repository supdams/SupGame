#ifndef UPDATE_H
#define UPDATE_H

#include "structs.hpp"
#include "collision.hpp"


void update_orientation_hasard(Anim *anim);
void update_dest_anim(JeuComplet *jeucomplet , Anim *anim , int *vx , int *vy);
void test_deplace(JeuComplet *jeucomplet , Anim *anim , int vx , int vy);
void update_deplacement(JeuComplet *jeucomplet , Anim *anim , int vx , int vy);
void non_sortir_terrain(JeuComplet *jeucomplet , Anim *anim);
void update_frame_anim(Anim *anim , int dimensionX);
void update_frame(Anim *anim);
void update_monstres(JeuComplet *jeucomplet);

#endif
