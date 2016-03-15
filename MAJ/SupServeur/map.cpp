#include "map.hpp"


void load_scroll(JeuComplet *jeucomplet , int largeurscroll , int hauteurscroll)
{
    /*Charge les différentes variables du scroll*/
    jeucomplet->jeu.decor.scroll.hauteurScroll = hauteurscroll * 32;
    jeucomplet->jeu.decor.scroll.largeurScroll = largeurscroll * 32;
    jeucomplet->jeu.decor.scroll.xscroll = jeucomplet->jeu.decor.scroll.yscroll = 0;
}

void load_valeur_scrool(JeuComplet *jeucomplet)
{
    jeucomplet->jeu.decor.scroll.minx = jeucomplet->jeu.decor.scroll.xscroll / TILE_LG;
    jeucomplet->jeu.decor.scroll.miny = jeucomplet->jeu.decor.scroll.yscroll / TILE_HT;
    jeucomplet->jeu.decor.scroll.maxx = (jeucomplet->jeu.decor.scroll.xscroll + jeucomplet->jeu.decor.scroll.largeurScroll) / TILE_LG;
    jeucomplet->jeu.decor.scroll.maxy = (jeucomplet->jeu.decor.scroll.yscroll + jeucomplet->jeu.decor.scroll.hauteurScroll) / TILE_HT;
}

void load_tileprops(JeuComplet *jeucomplet)
{
    FILE *fp = NULL;
    char isTraversable[MAX_CHAINE];

    jeucomplet->jeu.decor.tileprops = (Tileprops*)calloc(NB_TILE_TILESET , sizeof(Tileprops));/*On met tous les propriétés à 0 grace a calloc ce qui fait que par défault tout est vide*/


    fp = fopen("decor/tileprops.txt" , "r");/*Si c'est le cas on charge le tileprops contenant toutes les propriétés des tiles(vide , mur)*/


	    /*On remplis les propriÃ©tÃ©s de chaque tiles (mur ou vide)*/
    int i , nb;
    for(i = 0 ; i < NB_TILE_TILESET ; i++)
    {
        fscanf(fp , "%d %s" , &nb, isTraversable);
			
        if(!strcmp(isTraversable , "mur"))
            jeucomplet->jeu.decor.tileprops[nb].num = MUR;
        if(!strcmp(isTraversable , "teleportation"))
            jeucomplet->jeu.decor.tileprops[nb].num = TELEPORTATION;
        if(!strcmp(isTraversable , "haut"))
            jeucomplet->jeu.decor.tileprops[nb].num = HAUT_CHANGE;
        if(!strcmp(isTraversable , "bas"))
            jeucomplet->jeu.decor.tileprops[nb].num = BAS_CHANGE;
        if(!strcmp(isTraversable , "droite"))
            jeucomplet->jeu.decor.tileprops[nb].num = DROITE_CHANGE;
        if(!strcmp(isTraversable , "gauche"))
            jeucomplet->jeu.decor.tileprops[nb].num = GAUCHE_CHANGE;
        if(!strcmp(isTraversable , "action_vide"))
            jeucomplet->jeu.decor.tileprops[nb].num = ACTION_VIDE;
        if(!strcmp(isTraversable , "nage"))
            jeucomplet->jeu.decor.tileprops[nb].num = NAGE_PROPS;
	}

    fclose(fp);/*On ferme le fichier car on en a plus besoin*/
}



void load_map(JeuComplet *jeucomplet , char file[MAX_CHAINE])
{
	FILE *fp = NULL;/*On crée le pointeur FILE * pour y contenir le fichier .txt*/
	int j , i , c , hauteurscroll , largeurscroll;
	fp = fopen(file , "r");/*On lit le fichier en mode 'r' pour pouvoir prendre les fichiers du .txt et les mettres dans le tableau*/

	if(fp == NULL)
		cout<<"erreur avec load_map"<<endl;

	fscanf(fp ,"%d\n" ,&largeurscroll);/*On charge la hauteur et la largeur de la map qui dépend à cause du scrolling*/
	fscanf(fp ,"%d\n" ,&hauteurscroll);
	

	load_scroll(jeucomplet , largeurscroll , hauteurscroll);/*Se situe dans scroll.c sert à charger les variables qui servent au scrolling*/
	
	fscanf(fp , "%d\n" , &jeucomplet->jeu.quelMap);
	

	/// ajouter tt les map genre [m][c][j][i]
	for(c = 0 ; c < NB_COUCHES ; c++)
	{
		for(j = 0 ; j < hauteurscroll ; j++)/*Boucle pour remplir le tableau à 3 dimensions du décor*/
		{
			for(i = 0 ; i < largeurscroll ; i++)
			{
				fscanf(fp , "%d ", &jeucomplet->jeu.decor.mapDecor[c][j][i]);/*On prend dans le fichier .txt le nombre pointé par le curseur et on le met dans le tableau à 3dimensions*/
			}
		}
	}

	fclose(fp);/*On n'utilise plus le fichier*/
	
		cout<<"load_map ok"<<endl;
}

void put_anim_monstre(int id, int i , int j , JeuComplet *jeucomplet , char file[MAX_CHAINE])
{
    SDL_Rect src;
    src.x = DIMENSION_MONSTRE1_X;
    src.y = DIMENSION_MONSTRE1_Y;
    src.w = DIMENSION_MONSTRE1_X;
    src.h = DIMENSION_MONSTRE1_Y;

    jeucomplet->jeu.monstre[id].vivant = VIVANT;
    jeucomplet->jeu.monstre[id].compteurMonstreBouge = 10;
	jeucomplet->jeu.monstre[id].CompteurRepop = 1000;
    load_anim(&jeucomplet->jeu.monstre[id].animMonstre , file , src ,
              i * TILE_LG - jeucomplet->jeu.decor.scroll.xscroll , j * TILE_HT - jeucomplet->jeu.decor.scroll.yscroll  , COMPTEUR_MONSTRE1 , MONSTRE , FRAME_MONSTRE1);
}

void update_monstres(JeuComplet *jeucomplet)
{
    int i , vx , vy;

    for(i = 0 ; i < jeucomplet->jeu.nbMonstre ; i++)
    {
		if(jeucomplet->jeu.monstre[i].animMonstre.classe == MONSTRE)
        {
            jeucomplet->jeu.monstre[i].compteurMonstreBouge--;
            if(jeucomplet->jeu.monstre[i].compteurMonstreBouge <= 0)
            {
				update_orientation_hasard(&jeucomplet->jeu.monstre[i].animMonstre);/*HAUT-BAS-DROITE-GAUCHE*/
                jeucomplet->jeu.monstre[i].compteurMonstreBouge = rand()%100;
            }
            update_dest_anim(jeucomplet , &jeucomplet->jeu.monstre[i].animMonstre , &vx , &vy);/*On bouge les monstres*/
            test_deplace(jeucomplet , &jeucomplet->jeu.monstre[i].animMonstre , vx , vy);
            non_sortir_terrain(jeucomplet , &jeucomplet->jeu.monstre[i].animMonstre);
            update_frame(&jeucomplet->jeu.monstre[i].animMonstre);/*On gére l'animation*/
			

        }
    }
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

void update_dest_scroll_monstre(JeuComplet *jeucomplet)
{
    int i;
    for(i = 0 ; i < jeucomplet->jeu.nbMonstre ; i++)
    {
        jeucomplet->jeu.monstre[i].destScrollMonstre.x = jeucomplet->jeu.monstre[i].animMonstre.dest.x - jeucomplet->jeu.decor.scroll.xscroll;
        jeucomplet->jeu.monstre[i].destScrollMonstre.y = jeucomplet->jeu.monstre[i].animMonstre.dest.y - jeucomplet->jeu.decor.scroll.yscroll;
        jeucomplet->jeu.monstre[i].destScrollMonstre.w = DIMENSION_MONSTRE1_X;
        jeucomplet->jeu.monstre[i].destScrollMonstre.h = DIMENSION_MONSTRE1_Y;
    }
}

void test_deplace(JeuComplet *jeucomplet , Anim *anim , int vx , int vy)
{
    update_deplacement(jeucomplet, anim , vx , vy);
}

void update_deplacement(JeuComplet *jeucomplet , Anim *anim , int vx , int vy)
{
	SDL_Rect test;
    test = anim->dest;/*Grace à une variable de test on lui ajoute les deux variables*/
    test.x += vx;
    test.y += vy;

    if (collision_decor(jeucomplet , anim , test) == 0)
	{	
        anim->dest = test;
    }
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

void update_orientation_hasard(Anim *anim)
{
    int orientationHasard;
    orientationHasard = (rand() % (6 - 0 + 1)) + 0;

    switch(orientationHasard)
    {
    case BAS :
        anim->animCouranteY = BAS;
        anim->orientation = BAS;
        break;
    case HAUT :
        anim->animCouranteY = HAUT;
        anim->orientation = HAUT;
        break;
    case DROITE :
        anim->animCouranteY = DROITE;
        anim->orientation = DROITE;
        break;
    case GAUCHE :
        anim->animCouranteY = GAUCHE;
        anim->orientation = GAUCHE;
        break;
    case AUCUNE_DIRECTION :
        anim->animCouranteY = AUCUNE_DIRECTION;
        anim->orientation = AUCUNE_DIRECTION;
        break;
    case AUCUNE_DIRECTION1:
        anim->animCouranteY = AUCUNE_DIRECTION;
        anim->orientation = AUCUNE_DIRECTION;
        break;
    case AUCUNE_DIRECTION2:
        anim->animCouranteY = AUCUNE_DIRECTION;
        anim->orientation = AUCUNE_DIRECTION;
        break;

    default :
        break;
    }
    if(anim->animCouranteY != AUCUNE_DIRECTION && anim->animCouranteY != AUCUNE_DIRECTION1 && anim->animCouranteY != AUCUNE_DIRECTION2)
        anim->src.y = anim->animCouranteY * anim->dest.h;
}

void load_anim(Anim *anim , char fileImage[MAX_CHAINE] , SDL_Rect src , int destX , int destY , int compteur , int classe , int frame)
{
   // if(classe != ISPNJ || classe != ISPNJNONBOUGE)
     //  anim->image = load_image(fileImage);/*On charge l'image de l'anim*/
    anim->dest.x = destX;/*Coordonées en x*/
    anim->dest.y = destY;/*Coordonées en y*/
    anim->animCouranteX = 0;/*On commence en haut à gauche*/
    if(classe != 2)/*Si ce n'est pas un pnj qui ne bouge pas*/
        anim->animCouranteY = 0;
    anim->src.x = src.w * anim->animCouranteX;/*On découpe les frames du coup */
    anim->src.y = src.h * anim->animCouranteY;/*ici quand on blitteras le perso on le verra vers le bas */
    anim->src.w = src.w;/*Dimesions en y et x du personnage*/
    anim->src.h = src.h;
    anim->compteurDefinitife = anim->compteurTemporaire = compteur;/*Temps avant de passer à l'animation suivante*/
    anim->nbFrame = frame;
    anim->classe = classe;/*Le perso n'est pas un pnj à parler ou un pnj pour décorer ...*/
    anim->dest.w = src.w;
    anim->dest.h = src.h;
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

int collision_decor(JeuComplet *jeucomplet , Anim *anim , SDL_Rect t)
{
    /*Fonction de collision*/
    int xmin , xmax , ymin , ymax , i , j , c, indicetile;
	
    xmin = t.x / TILE_LG ;/*Ici on prend toute la box du perso*/
    ymin = (t.y + anim->src.h / 2) / TILE_HT;
    xmax = (t.x + anim->src.w - 1) / TILE_LG;
    ymax = (t.y + anim->src.h -1) / TILE_HT;
    for(c = 0 ; c < NB_COUCHES ; c++)
    {
        for (j = ymin ; j <= ymax ; j++)
        {
            for (i = xmin ; i <= xmax ; i++)
            {
				indicetile = jeucomplet->jeu.decor.mapDecor[c][j][i];
				
				if (indicetile != VIDE && jeucomplet->jeu.decor.tileprops[indicetile].num)
					return 1;
			}
        }
    }
    return 0;/*Sinon on peux bouger si il n'y as pas de mur */
}

int test_tile_collision(JeuComplet *jeucomplet , int indicetile , SDL_Rect *t , Anim *anim)
{
    if (indicetile != VIDE && jeucomplet->jeu.decor.tileprops[indicetile].num == MUR){
	cout<<"indicetile :"<<indicetile<< endl; 
	return 1;
	 }
    if(anim->classe == HEROS)
    {
        if (indicetile != VIDE && jeucomplet->jeu.decor.tileprops[indicetile].num == TELEPORTATION
            && jeucomplet->jeu.quelMap != 16)
        {
            jeucomplet->jeu.nBteleportation++;
//            teleportation_map(jeucomplet , t);
            return 1;
        }
      //  test_tile_collision_change_map(jeucomplet , indicetile , t);
        //test_tile_collision_nage(jeucomplet , indicetile);
    }

    return 0;
}
