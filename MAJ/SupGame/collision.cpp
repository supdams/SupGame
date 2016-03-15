#include "collision.hpp"

int collision_decor(JeuComplet *jeucomplet , Anim *anim , SDL_Rect *t)
{
    /*Fonction de collision*/
    int xmin , xmax , ymin , ymax , i , j , c, indicetile;

    xmin = t->x / TILE_LG ;/*Ici on prend toute la box du perso*/
    ymin = (t->y + anim->src.h / 2) / TILE_HT;
    xmax = (t->x + anim->src.w - 5) / TILE_LG;
    ymax = (t->y + anim->src.h) / TILE_HT;
    for(c = 0 ; c < NB_COUCHES ; c++)
    {
        for (j = ymin ; j <= ymax ; j++)
        {
            for (i = xmin ; i <= xmax ; i++)
            {
                indicetile = jeucomplet->jeu.decor.mapDecor[c][j][i];/*On prend chaque chiffre du fichier .txt*/
                if(test_tile_collision(jeucomplet , indicetile , t , anim) != 0)
                    return 1;
            }
        }
    }
    return 0;/*Sinon on peux bouger si il n'y as pas de mur */
}

int test_tile_collision(JeuComplet *jeucomplet , int indicetile , SDL_Rect *t , Anim *anim)
{
    if (indicetile != VIDE && jeucomplet->jeu.decor.tileprops[indicetile].num == MUR)/*Si c'est un mur on retourne 1 afin de dire de ne pas bouger*/
        return 1;
    if(anim->classe == HEROS)
    {
        if (indicetile != VIDE && jeucomplet->jeu.decor.tileprops[indicetile].num == TELEPORTATION
            && jeucomplet->jeu.quelMap != 16)
        {
            jeucomplet->jeu.nBteleportation++;
           // teleportation_map(jeucomplet , t);
            return 1;
        }
       // test_tile_collision_change_map(jeucomplet , indicetile , t);
    }

    return 0;
}

int intersects(SDL_Rect box1 , SDL_Rect box2)/*Fonction qui teste la collision entre deux SDL_Rect*/
{
    if ((box2.x >= box1.x + box1.w)     /* trop à droite*/
            || (box2.x + box2.w <= box1.x) /*trop à gauche*/
            || (box2.y >= box1.y + box1.h) /* trop en bas*/
            || (box2.y + box2.h <= box1.y))  /* trop en haut*/
        return 0;
    else
        return 1;
}