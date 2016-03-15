#ifndef MAP_HPP
#define MAP_HPP

#include "structs.hpp"
#include "load.hpp"
#include "scroll.hpp"
#include "collision.hpp"
#include "animation.hpp"
#include "update.hpp"

 extern Gestion jeu;

void load_tileprops(JeuComplet *jeucomplet);
void load_tileset(JeuComplet *jeucomplet);
void load_map(JeuComplet *jeucomplet , char file[MAX_CHAINE]);

void draw_couche1(JeuComplet *jeucomplet);
void draw_couche2(JeuComplet *jeucomplet);
void draw_couche3(JeuComplet *jeucomplet);

void load_monstres(JeuComplet *jeucomplet);
void draw_monstres(JeuComplet *jeucomplet);

void load_anim(Anim *anim , char fileImage[MAX_CHAINE] , SDL_Rect src , int destX , int destY , int compteur , int classe , int frame);
void put_anim_monstre(int index, int i , int j , JeuComplet *jeucomplet , char file[MAX_CHAINE]);

void update_dest_scroll_monstre(JeuComplet *jeucomplet);

# endif