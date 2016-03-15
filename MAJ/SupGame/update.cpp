#include "update.hpp"


void update_orientation_hasard(Anim *anim)
{
	switch(anim->orientation)
    {
    case BAS :
        anim->animCouranteY = BAS;
        break;
    case HAUT :
        anim->animCouranteY = HAUT;
        break;
    case DROITE :
        anim->animCouranteY = DROITE;
        break;
    case GAUCHE :
        anim->animCouranteY = GAUCHE;
        break;
    case AUCUNE_DIRECTION :
        anim->animCouranteY = AUCUNE_DIRECTION;
        break;
    case AUCUNE_DIRECTION1:
        anim->animCouranteY = AUCUNE_DIRECTION;
        break;
    case AUCUNE_DIRECTION2:
        anim->animCouranteY = AUCUNE_DIRECTION;
        break;

    default :
        break;
    }
    if(anim->animCouranteY != AUCUNE_DIRECTION && anim->animCouranteY != AUCUNE_DIRECTION1 && anim->animCouranteY != AUCUNE_DIRECTION2)
        anim->src.y = anim->animCouranteY * anim->dest.h;
}

void update_dest_anim(JeuComplet *jeucomplet , Anim *anim , int *vx , int *vy)
{
    int vitesse = 1;
    *vx = 0;
    *vy = 0;
    if(anim->animCouranteY == BAS)
        *vy += vitesse;
    if(anim->animCouranteY == DROITE)
        *vx += vitesse;
    if(anim->animCouranteY == HAUT)
        *vy -= vitesse;
    if(anim->animCouranteY == GAUCHE)
        *vx -= vitesse;
}



void test_deplace(JeuComplet *jeucomplet , Anim *anim , int vx , int vy)
{
    update_deplacement(jeucomplet, anim , vx , vy);
}

void update_deplacement(JeuComplet *jeucomplet , Anim *anim , int vx , int vy)
{
    int i;
	
    SDL_Rect test;
    test = anim->dest;/*Grace à une variable de test on lui ajoute les deux variables*/
    test.x += vx;
    test.y += vy;
    if(anim->classe == HEROS)
    {
        if((jeucomplet->jeu.perso[_PERSO_LOCAL_].dest.y + DIMENSION_PERSO_Y) + vy >= jeucomplet->jeu.decor.scroll.hauteurScroll)
            return;
        if((jeucomplet->jeu.perso[_PERSO_LOCAL_].dest.x + DIMENSION_PERSO_X) + vx >= jeucomplet->jeu.decor.scroll.largeurScroll)
            return;
        if(jeucomplet->jeu.perso[_PERSO_LOCAL_].dest.y + vy <= 0)
            return;
        if(jeucomplet->jeu.perso[_PERSO_LOCAL_].dest.x + vx <= 0)
            return;
    }

    if(anim->classe == ISPNJ)
    {
        if(intersects(test , jeucomplet->jeu.perso[_PERSO_LOCAL_].dest))
            return ;
    }
	
    if (collision_decor(jeucomplet , anim , &test) == 0)
	{	
        anim->dest = test;
        if(anim->classe == HEROS)
        {
            jeucomplet->jeu.perso[_PERSO_LOCAL_].dest = anim->dest;
        }
    }
}

void non_sortir_terrain(JeuComplet *jeucomplet , Anim *anim)
{
    if(anim->animCouranteY == BAS)
    {
        if(anim->dest.y + anim->dest.h > jeucomplet->jeu.decor.scroll.hauteurScroll)
            anim->dest.y = jeucomplet->jeu.decor.scroll.hauteurScroll - anim->dest.h;
    }
    if(anim->animCouranteY == DROITE)
    {
        if(anim->dest.x + anim->dest.w > jeucomplet->jeu.decor.scroll.largeurScroll)
            anim->dest.x = jeucomplet->jeu.decor.scroll.largeurScroll - anim->dest.w;
    }
    if(anim->animCouranteY == HAUT)
    {
        if(anim->dest.y <= 0)
            anim->dest.y = 0;
    }
    if(anim->animCouranteY == GAUCHE)
    {
        if(anim->dest.x < 0)
            anim->dest.x = 0;
    }
}

void update_frame_anim(Anim *anim , int dimensionX)
{
    anim->compteurTemporaire--;/*On baisse le compteur de l'anim envoyé en paramètre*/
    if(anim->compteurTemporaire <= 0)/*Si le compteur est égale à 0*/
    {
        anim->compteurTemporaire = anim->compteurDefinitife;/*On remet le compteur au compteur normale de l'anim envoyé en paramètre*/
        anim->animCouranteX++;/*On augmente l'anim comme ça le personnage peut marcher*/
        if(anim->animCouranteX >= anim->nbFrame)/*Si on arrive au nombre maximum d'animation (l'animation de l'attaque et de la marche n'en font que 4)*/
            anim->animCouranteX = 0;/*On la remet au début -> et on refait l'animation */
    }
    anim->src.x = anim->animCouranteX * dimensionX;/*On régle le découpage pour savoir à quelle image de l'anim on en est*/
}

void update_frame(Anim *anim)
{
    /*Dans cette fonction on vérifie si l'anim est un pnj ou un pnj animés du décor ou encore un monstre ... */
    if(anim->classe == ISPNJ && anim->animCouranteY != AUCUNE_DIRECTION
            && anim->animCouranteY != AUCUNE_DIRECTION1 &&
            anim->animCouranteY != AUCUNE_DIRECTION2)
        update_frame_anim(anim , anim->src.w);/*On met à jour la frame d'un pnj quelquonque, on avance dans son animation*/
    if(anim->classe == MONSTRE && anim->animCouranteY != AUCUNE_DIRECTION
            && anim->animCouranteY != AUCUNE_DIRECTION1 &&
            anim->animCouranteY != AUCUNE_DIRECTION2)
        update_frame_anim(anim , DIMENSION_MONSTRE1_X);/*Pareil mais cette fois uniquement si c'est un monstre*/
}

void update_monstres(JeuComplet *jeucomplet)
{
    int i , vx , vy;
    for(i = 0 ; i < jeucomplet->jeu.nbMonstre ; i++)
    {
        //if(jeucomplet->jeu.monstre[i].animMonstre.classe == MONSTRE)
       // {
            //jeucomplet->jeu.monstre[i].compteurMonstreBouge--;
           // if(jeucomplet->jeu.monstre[i].compteurMonstreBouge <= 0)
           // {
                update_orientation_hasard(&jeucomplet->jeu.monstre[i].animMonstre);/*HAUT-BAS-DROITE-GAUCHE*/
               // jeucomplet->jeu.monstre[i].compteurMonstreBouge = rand()%100;
          //  }
           // update_dest_anim(jeucomplet , &jeucomplet->jeu.monstre[i].animMonstre , &vx , &vy);/*On bouge les monstres*/
           // test_deplace(jeucomplet , &jeucomplet->jeu.monstre[i].animMonstre , vx , vy);
           // non_sortir_terrain(jeucomplet , &jeucomplet->jeu.monstre[i].animMonstre);
            update_frame(&jeucomplet->jeu.monstre[i].animMonstre);/*On gére l'animation*/
      //  }
    }
}