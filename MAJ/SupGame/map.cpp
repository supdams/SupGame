#include "map.hpp"

void load_tileprops(JeuComplet *jeucomplet)
{
	FILE *fp = NULL;
	char isTraversable[MAX_CHAINE];

	jeucomplet->jeu.decor.tileprops = (Tileprops*)calloc(NB_TILE_TILESET , sizeof(Tileprops));/*On met tous les propriétés à 0 grace a calloc ce qui fait que par défault tout est vide*/
	if(jeucomplet->jeu.decor.tileprops == NULL)/*On vérifie si ça à été bien alloué*/
		cout<<"erreur avec le chargement du tileprops (calloc)"<<endl;

	fp = fopen("decor/tileprops.txt" , "r");/*Si c'est le cas on charge le tileprops contenant toutes les propriétés des tiles(vide , mur)*/

	if(fp == NULL)
		cout<<"decor/tileprops.txt introuvable"<<endl;

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

void load_tileset(JeuComplet *jeucomplet)
{
	jeucomplet->jeu.decor.imageTileset[TILESET_1] = load_image("img/jeu/decor/Tileset.png");/*On charge le tileset (décor)*/
	jeucomplet->jeu.decor.imageTileset[TILESET_2] = load_image("img/jeu/decor/Tileset1.png");/*On charge le tileset 1 qui sert a l'aimation*/
	jeucomplet->jeu.decor.tilesetActuelle = 0;
	jeucomplet->jeu.decor.compteurTileset = COMPTEUR_CHANGE_TILESET;
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
	scroll Scroll;
	Scroll.load_scroll(jeucomplet , largeurscroll , hauteurscroll);/*Se situe dans scroll.c sert à charger les variables qui servent au scrolling*/
	fscanf(fp , "%d\n" , &jeucomplet->jeu.quelMap);

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
}

void draw_couche1(JeuComplet *jeucomplet)
{
	int j, i, nTile;
	SDL_Rect dest, src;
	src.w = TILE_LG;
	src.h = TILE_HT;

	for(j = 0; j < (jeucomplet->jeu.decor.scroll.hauteurScroll / TILE_HT) + 1; j++)
	{
		for(i = 0 ; i < (jeucomplet->jeu.decor.scroll.largeurScroll / TILE_LG) + 1; i++)
		{
			dest.x = i * TILE_LG - (jeucomplet->jeu.decor.scroll.xscroll & (TILE_LG - 1));
			dest.y = j * TILE_HT - (jeucomplet->jeu.decor.scroll.yscroll & (TILE_HT - 1));
			nTile = jeucomplet->jeu.decor.mapDecor[0]
			[j + (jeucomplet->jeu.decor.scroll.yscroll / TILE_HT)]
			[i + (jeucomplet->jeu.decor.scroll.xscroll / TILE_LG)];
			src.x = (nTile % TILE_X) * TILE_LG;
			src.y = (nTile / TILE_Y) * TILE_HT;
			SDL_BlitSurface(jeucomplet->jeu.decor.imageTileset[jeucomplet->jeu.decor.tilesetActuelle], &src, jeu.tmps, &dest);
		}
	}
}

void draw_couche2(JeuComplet *jeucomplet)
{
	int j, i, nTile;
	SDL_Rect dest, src;
	src.w = TILE_LG;
	src.h = TILE_HT;

	for(j = 0; j < (jeucomplet->jeu.decor.scroll.hauteurScroll / TILE_HT) + 1; j++)
	{
		for(i = 0 ; i < (jeucomplet->jeu.decor.scroll.largeurScroll / TILE_LG) + 1; i++)
		{
			dest.x = i * TILE_LG - (jeucomplet->jeu.decor.scroll.xscroll & (TILE_LG - 1));
			dest.y = j * TILE_HT - (jeucomplet->jeu.decor.scroll.yscroll & (TILE_HT - 1));
			nTile = jeucomplet->jeu.decor.mapDecor[1]
			[j + (jeucomplet->jeu.decor.scroll.yscroll / TILE_HT)]
			[i + (jeucomplet->jeu.decor.scroll.xscroll / TILE_LG)];
			src.x = (nTile % TILE_X) * TILE_LG;
			src.y = (nTile / TILE_Y) * TILE_HT;
			if((nTile < 4846 || nTile > 4862) && (nTile != 4864))/*Protection contre les tiles qu'on ne blitteras pas*/
			{
				if(nTile != 2664 && nTile != 2665 && nTile != 2666 && nTile != 2667)
					SDL_BlitSurface(jeucomplet->jeu.decor.imageTileset[jeucomplet->jeu.decor.tilesetActuelle], &src, jeu.tmps, &dest);
			}
		}
	}

}
void draw_couche3(JeuComplet *jeucomplet)
{
	int j, i, nTile;
	SDL_Rect dest, src;
	src.w = TILE_LG;
	src.h = TILE_HT;

	for(j = 0; j < (jeucomplet->jeu.decor.scroll.hauteurScroll / TILE_HT) + 1; j++)
	{
		for(i = 0 ; i < (jeucomplet->jeu.decor.scroll.largeurScroll / TILE_LG) + 1; i++)
		{
			dest.x = i * TILE_LG - (jeucomplet->jeu.decor.scroll.xscroll & (TILE_LG - 1));
			dest.y = j * TILE_HT - (jeucomplet->jeu.decor.scroll.yscroll & (TILE_HT - 1));
			nTile = jeucomplet->jeu.decor.mapDecor[2]
			[j + (jeucomplet->jeu.decor.scroll.yscroll / TILE_HT)]
			[i + (jeucomplet->jeu.decor.scroll.xscroll / TILE_LG)];
			src.x = (nTile % TILE_X) * TILE_LG;
			src.y = (nTile / TILE_Y) * TILE_HT;
			if(nTile != 2664 && nTile != 2665 && nTile != 2666 && nTile != 2667)
				SDL_BlitSurface(jeucomplet->jeu.decor.imageTileset[jeucomplet->jeu.decor.tilesetActuelle], &src, jeu.tmps, &dest);
		}
	}
}

void load_monstres(JeuComplet *jeucomplet)
{
	cout<<"load_monstres"<<endl;

	cout<<"jeucomplet->jeu.perso[_PERSO_LOCAL_].data_creatures ->"<<jeucomplet->jeu.perso[_PERSO_LOCAL_].data_creatures<<endl;	
	int i;
	int p=0;

	string MSG = jeucomplet->jeu.perso[_PERSO_LOCAL_].data_creatures;

	jeucomplet->jeu.nbMonstre = atoi( MSG.substr(0,3).c_str() ) ;// on prend l int qui defini le nb de monstre
	cout<<"jeucomplet->jeu.nbMonstre ->"<<jeucomplet->jeu.nbMonstre<<endl;



	//jeucomplet->jeu.quelMap = atoi( MSG.substr(0,3).c_str() ) ; // on prent int du num de la map
	//cout<<"jeucomplet->jeu.quelMap ->"<<jeucomplet->jeu.quelMap<<endl;
	//MSG = MSG.substr(p, MSG.length());// on vire pour qu il ne reste que les info des creature @ ...


	jeucomplet->jeu.monstre = (Monstre*)malloc(jeucomplet->jeu.nbMonstre * sizeof(Monstre));
	if(jeucomplet->jeu.monstre == NULL)
		cout<<"jeucomplet->jeu.monstre == NULL"<<endl;

	int nbMonstre = 0;

	for(i = 0; i < jeucomplet->jeu.nbMonstre ; i++)
	{
		int id;
		int map;
		int modelid;
		string nom;
		SDL_Rect destScrollMonstre;
		destScrollMonstre.x;
		destScrollMonstre.y;
		int CompteurRepop;
		int hp;
		int mindmg;
		int maxdmg;
		int vivant;


	p = MSG.find_first_of("@"); // oncherche
	MSG = MSG.substr(p+1, MSG.length());// on vire ce qui a avan le !


		cout<<"DEBUT MSG ->"<<MSG<<endl;
		cout<<"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^monstre num->"<<nbMonstre<<endl;


		p = MSG.find_first_of("!"); // oncherche
		id = atoi( MSG.substr(0,3).c_str() ) ;
		MSG = MSG.substr(p+1, MSG.length());
		jeucomplet->jeu.monstre[nbMonstre].id = id;
		cout<<"jeucomplet->jeu.monstre[nbMonstre].id ->"<<jeucomplet->jeu.monstre[nbMonstre].id<<endl;

		p = MSG.find_first_of("!"); // oncherche
		map = atoi( MSG.substr(0,3).c_str() ) ;
		MSG = MSG.substr(p+1, MSG.length());
		jeucomplet->jeu.monstre[nbMonstre].map = map;
		cout<<"jeucomplet->jeu.monstre[nbMonstre].map ->"<<jeucomplet->jeu.monstre[nbMonstre].map<<endl;

		p = MSG.find_first_of("!"); // oncherche
		modelid = atoi( MSG.substr(0,3).c_str() ) ;
		MSG = MSG.substr(p+1, MSG.length());
		jeucomplet->jeu.monstre[nbMonstre].modelid = modelid;
		cout<<"jeucomplet->jeu.monstre[nbMonstre].modelid ->"<<jeucomplet->jeu.monstre[nbMonstre].modelid<<endl;
			

		p = MSG.find_first_of("!");// 1er ! le nom du monstre qui bug  recup mal l info
		nom = MSG.substr(0,p);
		MSG = MSG.substr(p+1, MSG.length());
		// arrive pas a utiliser un string ...
		jeucomplet->jeu.monstre[nbMonstre].nom_monstre =(char *) malloc(255);
		strcpy(jeucomplet->jeu.monstre[nbMonstre].nom_monstre, nom.c_str());

		cout<<"jeucomplet->jeu.monstre[nbMonstre].son_monstre ->"<<jeucomplet->jeu.monstre[nbMonstre].nom_monstre<<endl;

		p = MSG.find_first_of("!");
		destScrollMonstre.x = atoi( MSG.substr(0,3).c_str() ) ;
		MSG = MSG.substr(p+1, MSG.length());
		jeucomplet->jeu.monstre[nbMonstre].destScrollMonstre.x = destScrollMonstre.x;
		//jeucomplet->jeu.monstre[nbMonstre].animMonstre.dest.x=destScrollMonstre.x;//test
		cout<<"jeucomplet->jeu.monstre[nbMonstre].destScrollMonstre.x ->"<<jeucomplet->jeu.monstre[nbMonstre].destScrollMonstre.x<<endl;





		p = MSG.find_first_of("!");
		destScrollMonstre.y = atoi( MSG.substr(0,3).c_str() ) ;
		MSG = MSG.substr(p+1, MSG.length());
		jeucomplet->jeu.monstre[nbMonstre].destScrollMonstre.y = destScrollMonstre.y;
		//jeucomplet->jeu.monstre[nbMonstre].animMonstre.dest.y=destScrollMonstre.y;//test
		cout<<"jeucomplet->jeu.monstre[nbMonstre].destScrollMonstre.y ->"<<jeucomplet->jeu.monstre[nbMonstre].destScrollMonstre.y<<endl;

		p = MSG.find_first_of("!");
		CompteurRepop = atoi( MSG.substr(0,3).c_str() ) ;
		MSG = MSG.substr(p+1, MSG.length());
		jeucomplet->jeu.monstre[nbMonstre].CompteurRepop = CompteurRepop;
		cout<<"jeucomplet->jeu.monstre[nbMonstre].CompteurRepop ->"<<jeucomplet->jeu.monstre[nbMonstre].CompteurRepop<<endl;


		p = MSG.find_first_of("!");
		hp = atoi( MSG.substr(0,3).c_str() ) ;
		MSG = MSG.substr(p+1, MSG.length());
		jeucomplet->jeu.monstre[nbMonstre].hp = hp;
		cout<<"jeucomplet->jeu.monstre[nbMonstre].hp ->"<<jeucomplet->jeu.monstre[nbMonstre].hp<<endl;


		p = MSG.find_first_of("!");
		mindmg = atoi( MSG.substr(0,3).c_str() ) ;
		MSG = MSG.substr(p+1, MSG.length());
		jeucomplet->jeu.monstre[nbMonstre].mindmg = mindmg;
		cout<<"jeucomplet->jeu.monstre[nbMonstre].mindmg ->"<<jeucomplet->jeu.monstre[nbMonstre].mindmg<<endl;


		p = MSG.find_first_of("!");
		maxdmg = atoi( MSG.substr(0,3).c_str() ) ;
		MSG = MSG.substr(p+1, MSG.length());
		jeucomplet->jeu.monstre[nbMonstre].maxdmg = maxdmg;
		cout<<"jeucomplet->jeu.monstre[nbMonstre].maxdmg ->"<<jeucomplet->jeu.monstre[nbMonstre].maxdmg<<endl;

		p = MSG.find_first_of("!");
		vivant = atoi( MSG.substr(0,3).c_str() ) ;
		jeucomplet->jeu.monstre[nbMonstre].vivant = vivant;
		cout<<"jeucomplet->jeu.monstre[nbMonstre].vivant ->"<<jeucomplet->jeu.monstre[nbMonstre].vivant<<endl;


		MSG = MSG.substr(+1, MSG.length());// TIRE LE DERNIERE CHARACTER QUI PEUX ETRE 0 OU 1 ATTENTION LAISSER EN DERNIERE POSSITION


		cout<<"reste du MSG ->"<<MSG<<endl;
			put_anim_monstre(nbMonstre,jeucomplet->jeu.monstre[jeucomplet->jeu.nbMonstre].destScrollMonstre.x , jeucomplet->jeu.monstre[jeucomplet->jeu.nbMonstre].destScrollMonstre.y , jeucomplet , "img/jeu/monstre/monstre1.png");

		//put_anim_monstre(destScrollMonstre.x , destScrollMonstre.y , jeucomplet , "img/jeu/monstre/monstre1.png");
		nbMonstre++;
	}
	/*
	
	 int boucle =jeucomplet->jeu.nbMonstre;
	 
		jeucomplet->jeu.nbMonstre=0;
	for(i = 0; i < boucle ; i++)
	{
	put_anim_monstre(jeucomplet->jeu.monstre[jeucomplet->jeu.nbMonstre].destScrollMonstre.x , jeucomplet->jeu.monstre[jeucomplet->jeu.nbMonstre].destScrollMonstre.y , jeucomplet , "img/jeu/monstre/monstre1.png");
		jeucomplet->jeu.nbMonstre++;

	}
	*/
}

void put_anim_monstre(int index, int i , int j , JeuComplet *jeucomplet , char file[MAX_CHAINE])
{
    SDL_Rect src;
    src.x = DIMENSION_MONSTRE1_X;
    src.y = DIMENSION_MONSTRE1_Y;
    src.w = DIMENSION_MONSTRE1_X;
    src.h = DIMENSION_MONSTRE1_Y;

    jeucomplet->jeu.monstre[index].vivant = VIVANT;
    jeucomplet->jeu.monstre[index].compteurMonstreBouge = COMPTEUR_MONSTRE_BOUGE1;
	jeucomplet->jeu.monstre[index].CompteurRepop = 1000;
    load_anim(&jeucomplet->jeu.monstre[index].animMonstre , file , src , i , j  , COMPTEUR_MONSTRE1 , MONSTRE , FRAME_MONSTRE1);

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

void draw_monstres(JeuComplet *jeucomplet)
{
    int i;

    for(i = 0 ; i < jeucomplet->jeu.nbMonstre ; i++)
    {
        if(jeucomplet->jeu.monstre[i].vivant == VIVANT){
           SDL_BlitSurface(jeucomplet->jeu.monstre[i].animMonstre.image , &jeucomplet->jeu.monstre[i].animMonstre.src , jeu.tmps , &jeucomplet->jeu.monstre[i].destScrollMonstre);
		   }else{
			   
			   jeucomplet->jeu.monstre[i].CompteurRepop =jeucomplet->jeu.monstre[i].CompteurRepop--;
			   
			   if(jeucomplet->jeu.monstre[i].CompteurRepop ==0)
			   {
			     jeucomplet->jeu.monstre[i].CompteurRepop =1000;
				 jeucomplet->jeu.monstre[i].vivant = VIVANT;
			   }
		}
    }
}

void test_collision_monstre(JeuComplet *jeucomplet)
{
    int i = 0;
    int calculT = 0;
//	int calculT = calculT++;
    SDL_Rect dest , dest2;


    dest.x = jeucomplet->jeu.perso[_PERSO_LOCAL_].dest.x;
    dest.y = jeucomplet->jeu.perso[_PERSO_LOCAL_].dest.y;
    dest.w = jeucomplet->jeu.perso[_PERSO_LOCAL_].animPerso[2].src.w;
    dest.h = jeucomplet->jeu.perso[_PERSO_LOCAL_].animPerso[2].src.h;
    if(jeucomplet->touche.espace == ON && jeucomplet->jeu.perso[_PERSO_LOCAL_].animActuelleObjet == 3)
    {
        for(i = 0 ; i < jeucomplet->jeu.nbMonstre; i++)
        {
			dest2.x = jeucomplet->jeu.monstre[i].animMonstre.dest.x;
            dest2.y = jeucomplet->jeu.monstre[i].animMonstre.dest.y;
            dest2.w = DIMENSION_MONSTRE1_X;
            dest2.h = DIMENSION_MONSTRE1_Y;
            if(intersects(dest , dest2))
               jeucomplet->jeu.monstre[i].vivant = MORT;
			

        }
    }else if(jeucomplet->touche.espace != ON)
    {
		        for(i = 0 ; i < jeucomplet->jeu.nbMonstre; i++)
        {
            dest2.x = jeucomplet->jeu.monstre[i].animMonstre.dest.x;
            dest2.y = jeucomplet->jeu.monstre[i].animMonstre.dest.y;
            dest2.w = DIMENSION_MONSTRE1_X;
            dest2.h = DIMENSION_MONSTRE1_Y;
	          if(intersects(dest , dest2))
               jeucomplet->jeu.perso[_PERSO_LOCAL_].hp = jeucomplet->jeu.perso[_PERSO_LOCAL_].hp -5;
	}
	}
}

void load_anim(Anim *anim , char fileImage[MAX_CHAINE] , SDL_Rect src , int destX , int destY , int compteur , int classe , int frame)
{
    if(classe != ISPNJ || classe != ISPNJNONBOUGE)
       anim->image = load_image(fileImage);/*On charge l'image de l'anim*/
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
