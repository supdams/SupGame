#ifndef MAP_HPP
#define MAP_HPP

#include "constantes.hpp"

void update_frame_anim(Anim *anim , int dimensionX);
void update_frame(Anim *anim);
void non_sortir_terrain(JeuComplet *jeucomplet , Anim *anim);

void load_tileprops(JeuComplet *jeucomplet);
void load_map(JeuComplet *jeucomplet , char file[MAX_CHAINE]);


void put_anim_monstre(int id, int i , int j , JeuComplet *jeucomplet , char file[MAX_CHAINE]);

void update_monstres(JeuComplet *jeucomplet);
void update_dest_scroll_monstre(JeuComplet *jeucomplet);

void load_valeur_scrool(JeuComplet *jeucomplet);


void load_scroll(JeuComplet *jeucomplet , int largeurscroll , int hauteurscroll);

void test_deplace(JeuComplet *jeucomplet , Anim *anim , int vx , int vy);
void update_deplacement(JeuComplet *jeucomplet , Anim *anim , int vx , int vy);

void update_dest_anim(JeuComplet *jeucomplet , Anim *anim , int *vx , int *vy);
void update_orientation_hasard(Anim *anim);
void load_anim(Anim *anim , char fileImage[MAX_CHAINE] , SDL_Rect src , int destX , int destY , int compteur , int classe , int frame);


int intersects(SDL_Rect box1 , SDL_Rect box2)/*Fonction qui teste la collision entre deux SDL_Rect*/;
int collision_decor(JeuComplet *jeucomplet , Anim *anim , SDL_Rect t);
int test_tile_collision(JeuComplet *jeucomplet , int indicetile , SDL_Rect *t , Anim *anim);


# endif