// partie.cpp

#include "partie.hpp"

extern char MSG_[_MAXLEN_];


void partie::test_clic_monstre(JeuComplet *jeucomplet)
{
	int stop =0;

	if(jeucomplet->touche.clic)
	{

		jeucomplet->jeu.num_mob_clic=0;

		for(int i = 0; i < jeucomplet->jeu.nbMonstre;i++)
		{
			if(stop==0)
			{
				jeucomplet->jeu.num_mob_clic=i;
				cout<<i<<"num_mob_clic:"<<jeucomplet->jeu.num_mob_clic<<endl;

				SDL_Rect dest , dest2;
				dest.x = jeucomplet->touche.posSouris.x;
				dest.y = jeucomplet->touche.posSouris.y;
				dest.w = 1;
				dest.h = 1;

				dest2.x = jeucomplet->jeu.monstre[i].animMonstre.dest.x -jeucomplet->jeu.decor.scroll.xscroll;
				dest2.y = jeucomplet->jeu.monstre[i].animMonstre.dest.y -jeucomplet->jeu.decor.scroll.yscroll;
				dest2.w = DIMENSION_MONSTRE1_X;
				dest2.h = DIMENSION_MONSTRE1_Y;


				if(intersects(dest , dest2)){
					cout<<"intersects detect un clic sur le mob "<<i<<endl;
					jeucomplet->jeu.clic_activer=1;
					stop=1;
				}else{
					stop=0;
					jeucomplet->jeu.clic_activer=0;
				}
			}//stop
		} // boucle monstre
	}// on clic ?


	if(jeucomplet->jeu.clic_activer==1)
	{
		SDL_Color foregroundColor;
		foregroundColor.r = 0;
		foregroundColor.g = 255;
		foregroundColor.b = 0;

		cout<<"on a clic sur un mob on affiche les stat etc ^^"<<jeucomplet->jeu.num_mob_clic<<endl;

		char s[12];// ca done  999 999 999 999 bcp trop ?

		sprintf(s, "%d", jeucomplet->jeu.monstre[jeucomplet->jeu.num_mob_clic].hp); // Conversion de l'entier


		drawString2(s, 100, 180, font[4],foregroundColor);

		drawString2(jeucomplet->jeu.monstre[jeucomplet->jeu.num_mob_clic].nom_monstre, 120, 200, font[4],foregroundColor);
		
	}
}


void partie::choix_perso(JeuComplet *jeucomplet)
{

	cout<<"DEBUGGG choix_perso  acces->"<<jeucomplet->jeu.perso[_PERSO_LOCAL_].acces<<endl;
	cout<<"DEBUGGG choix_perso  acces_suite->"<<jeucomplet->jeu.perso[_PERSO_LOCAL_].acces_suite<<endl;
	cout<<"DEBUGGG choix_perso  acc_compte->"<<jeucomplet->jeu.perso[_PERSO_LOCAL_].acc_compte<<endl;



	memset(&jeucomplet->touche , 0 , sizeof(Touche));/*Toute les variable de la structure Touche sont miss à 0*/

	int go =1;
	int afficher =0;
	int text=0;
	string guide="0";

	SDL_Surface *box_choix_perso = NULL,*bouton = NULL,*bouton2 = NULL, *image_perso1 = NULL, *image_perso2 = NULL, *image_perso3 = NULL;

	box_choix_perso = load_image("img/box-choix-perso.png");// img box du perso

	bouton = load_image("img/Image_432.png"); // img si pas sur l img
	bouton2 = load_image("img/Image_436.png"); // img si sur l img

	// on charge tt les image des race osef si on use pas tte  a opti ^^
	image_perso1 = load_image("img/jeu/heros/Magriavs/perso_marche1.png");
	image_perso2 = load_image("img/jeu/heros/Roukrses/perso_marche1.png");
	image_perso3 = load_image("img/jeu/heros/Skrills/perso_marche1.png");


	int afficher_perso=0;

	string traitement_perso = jeucomplet->jeu.perso[_PERSO_LOCAL_].acces_suite;

	// traitement de acces_suite ^^
	//string calculateur_perso=jeucomplet->jeu.perso[_PERSO_LOCAL_].acces_suite;
	int p=0;


	int nb_perso = atoi( traitement_perso.substr(0,3).c_str() ) ;

	cout<<"_MESSAGE_CONNECTION_  tous--> '"<<traitement_perso<<"'"<<endl;
	p = traitement_perso.find_first_of("@");
	traitement_perso = traitement_perso.substr(p+1, traitement_perso.length());		

	//string nom_perso = jeucomplet->jeu.perso[_PERSO_LOCAL_].acces_suite.substr(0,3);
	cout<<"calculateur_perso NB perso --> '"<<nb_perso<<"'"<<endl;
	cout<<"REEEEEEEEEEEEEEEEEEEE traitement_perso --> '"<<traitement_perso<<"'"<<endl;
	string nom_perso1, nom_perso2, nom_perso3;
	int guide_perso1, guide_perso2, guide_perso3;

	p = traitement_perso.find_first_of("@");// acc du compte
	int acc = atoi( traitement_perso.substr(0,3).c_str() ) ;
	traitement_perso = traitement_perso.substr(p+1, traitement_perso.length());
	cout<<"on decouper il reste --> '"<<traitement_perso<<"'"<<endl;
	cout<<"acc du compte --> '"<<acc<<"'"<<endl;
	jeucomplet->jeu.perso[_PERSO_LOCAL_].acc_compte=acc;
	if (nb_perso==1)
	{
		cout<<"tous--> '"<<traitement_perso<<"'"<<endl;
		p = traitement_perso.find_first_of("@");// 1er ! le nom du perso
		nom_perso1 = traitement_perso.substr(0,p);
		traitement_perso = traitement_perso.substr(p+1, traitement_perso.length());
		cout<<"on decouper il reste --> '"<<traitement_perso<<"'"<<endl;
		cout<<"nom_perso1 --> '"<<nom_perso1<<"'"<<endl;

		//	cout<<"_NOUVEAU_CLIENT_suite --> '"<<msg<<"'"<<endl;
		p = traitement_perso.find_first_of("@");// 2eme ! les info des perso
		guide_perso1 = atoi( traitement_perso.substr(0,3).c_str() ) ;
		traitement_perso = traitement_perso.substr(p+1, traitement_perso.length());
		cout<<"on decouper il reste --> '"<<traitement_perso<<"'"<<endl;
		cout<<"guide_perso1 --> '"<<guide_perso1<<"'"<<endl;



		p = traitement_perso.find_first_of("@");// 2eme ! les info des perso
		int race_perso1 = atoi( traitement_perso.substr(0,3).c_str() ) ;
		traitement_perso = traitement_perso.substr(p+1, traitement_perso.length());
		cout<<"on decouper il reste --> '"<<traitement_perso<<"'"<<endl;
		cout<<"race_perso1 --> '"<<race_perso1<<"'"<<endl;

	}else if (nb_perso==2)
	{// !nom! guid! race
		////////////////////////////// 1 er perso
		cout<<"tous--> '"<<traitement_perso<<"'"<<endl;
		p = traitement_perso.find_first_of("@");// 1er ! le nom du perso
		nom_perso1 = traitement_perso.substr(0,p);
		traitement_perso = traitement_perso.substr(p+1, traitement_perso.length());
		cout<<"on decouper il reste --> '"<<traitement_perso<<"'"<<endl;
		cout<<"nom_perso1 --> '"<<nom_perso1<<"'"<<endl;

		//	cout<<"_NOUVEAU_CLIENT_suite --> '"<<msg<<"'"<<endl;
		p = traitement_perso.find_first_of("@");// 2eme ! les info des perso
		guide_perso1 = atoi( traitement_perso.substr(0,3).c_str() ) ;
		traitement_perso = traitement_perso.substr(p+1, traitement_perso.length());
		cout<<"on decouper il reste --> '"<<traitement_perso<<"'"<<endl;
		cout<<"guide_perso1 --> '"<<guide_perso1<<"'"<<endl;



		p = traitement_perso.find_first_of("@");// 2eme ! les info des perso
		int race_perso1 = atoi( traitement_perso.substr(0,3).c_str() ) ;
		traitement_perso = traitement_perso.substr(p+1, traitement_perso.length());
		cout<<"on decouper il reste --> '"<<traitement_perso<<"'"<<endl;
		cout<<"race_perso1 --> '"<<race_perso1<<"'"<<endl;


		/////////////////////////////// 2 eme perso

		cout<<"tous--> '"<<traitement_perso<<"'"<<endl;
		p = traitement_perso.find_first_of("@");// 1er ! le nom du perso
		nom_perso2 = traitement_perso.substr(0,p);
		traitement_perso = traitement_perso.substr(p+1, traitement_perso.length());
		cout<<"on decouper il reste --> '"<<traitement_perso<<"'"<<endl;
		cout<<"nom_perso2 --> '"<<nom_perso2<<"'"<<endl;

		//	cout<<"_NOUVEAU_CLIENT_suite --> '"<<msg<<"'"<<endl;
		p = traitement_perso.find_first_of("@");// 2eme ! les info des perso
		guide_perso2 = atoi( traitement_perso.substr(0,3).c_str() ) ;
		traitement_perso = traitement_perso.substr(p+1, traitement_perso.length());
		cout<<"on decouper il reste --> '"<<traitement_perso<<"'"<<endl;
		cout<<"guide_perso2 --> '"<<guide_perso2<<"'"<<endl;



		p = traitement_perso.find_first_of("@");// 2eme ! les info des perso
		int race_perso2 = atoi( traitement_perso.substr(0,3).c_str() ) ;
		traitement_perso = traitement_perso.substr(p+1, traitement_perso.length());
		cout<<"on decouper il reste --> '"<<traitement_perso<<"'"<<endl;
		cout<<"race_perso2 --> '"<<race_perso2<<"'"<<endl;

	}else if (nb_perso==3)
	{
		////////////////////////////// 1 er perso
		cout<<"tous--> '"<<traitement_perso<<"'"<<endl;
		p = traitement_perso.find_first_of("@");// 1er ! le nom du perso
		nom_perso1 = traitement_perso.substr(0,p);
		traitement_perso = traitement_perso.substr(p+1, traitement_perso.length());
		cout<<"on decouper il reste --> '"<<traitement_perso<<"'"<<endl;
		cout<<"nom_perso1 --> '"<<nom_perso1<<"'"<<endl;

		//	cout<<"_NOUVEAU_CLIENT_suite --> '"<<msg<<"'"<<endl;
		p = traitement_perso.find_first_of("@");// 2eme ! les info des perso
		guide_perso1 = atoi( traitement_perso.substr(0,3).c_str() ) ;
		traitement_perso = traitement_perso.substr(p+1, traitement_perso.length());
		cout<<"on decouper il reste --> '"<<traitement_perso<<"'"<<endl;
		cout<<"guide_perso1 --> '"<<guide_perso1<<"'"<<endl;



		p = traitement_perso.find_first_of("@");// 2eme ! les info des perso
		int race_perso1 = atoi( traitement_perso.substr(0,3).c_str() ) ;
		traitement_perso = traitement_perso.substr(p+1, traitement_perso.length());
		cout<<"on decouper il reste --> '"<<traitement_perso<<"'"<<endl;
		cout<<"race_perso1 --> '"<<race_perso1<<"'"<<endl;

		/////////////////////////////// 2 eme perso

		cout<<"tous--> '"<<traitement_perso<<"'"<<endl;
		p = traitement_perso.find_first_of("@");// 1er ! le nom du perso
		nom_perso2 = traitement_perso.substr(0,p);
		traitement_perso = traitement_perso.substr(p+1, traitement_perso.length());
		cout<<"on decouper il reste --> '"<<traitement_perso<<"'"<<endl;
		cout<<"nom_perso2 --> '"<<nom_perso2<<"'"<<endl;

		//	cout<<"_NOUVEAU_CLIENT_suite --> '"<<msg<<"'"<<endl;
		p = traitement_perso.find_first_of("@");// 2eme ! les info des perso
		guide_perso2 = atoi( traitement_perso.substr(0,3).c_str() ) ;
		traitement_perso = traitement_perso.substr(p+1, traitement_perso.length());
		cout<<"on decouper il reste --> '"<<traitement_perso<<"'"<<endl;
		cout<<"guide_perso2 --> '"<<guide_perso2<<"'"<<endl;



		p = traitement_perso.find_first_of("@");// 2eme ! les info des perso
		int race_perso2 = atoi( traitement_perso.substr(0,3).c_str() ) ;
		traitement_perso = traitement_perso.substr(p+1, traitement_perso.length());
		cout<<"on decouper il reste --> '"<<traitement_perso<<"'"<<endl;
		cout<<"race_perso2 --> '"<<race_perso2<<"'"<<endl;
		/////////////////////////////// 3 eme perso

		cout<<"tous--> '"<<traitement_perso<<"'"<<endl;
		p = traitement_perso.find_first_of("@");// 1er ! le nom du perso
		nom_perso3 = traitement_perso.substr(0,p);
		traitement_perso = traitement_perso.substr(p+1, traitement_perso.length());
		cout<<"on decouper il reste --> '"<<traitement_perso<<"'"<<endl;
		cout<<"nom_perso3 --> '"<<nom_perso3<<"'"<<endl;

		//	cout<<"_NOUVEAU_CLIENT_suite --> '"<<msg<<"'"<<endl;
		p = traitement_perso.find_first_of("@");// 2eme ! les info des perso
		guide_perso3 = atoi( traitement_perso.substr(0,3).c_str() ) ;
		traitement_perso = traitement_perso.substr(p+1, traitement_perso.length());
		cout<<"on decouper il reste --> '"<<traitement_perso<<"'"<<endl;
		cout<<"guide_perso3 --> '"<<guide_perso3<<"'"<<endl;



		p = traitement_perso.find_first_of("@");// 2eme ! les info des perso
		int race_perso3 = atoi( traitement_perso.substr(0,3).c_str() ) ;
		traitement_perso = traitement_perso.substr(p+1, traitement_perso.length());
		cout<<"on decouper il reste --> '"<<traitement_perso<<"'"<<endl;
		cout<<"race_perso3 --> '"<<race_perso3<<"'"<<endl;
	}


	while (go == 1)
	{

		SDL_FillRect(jeu.screen , NULL , SDL_MapRGB(jeu.screen->format, 255, 255, 255));/*On rempli l'écran de noir*/
		SDL_FillRect(jeu.tmps , NULL , SDL_MapRGB(jeu.tmps->format, 255, 255, 255));/*On rempli l'écran de noir*/
		//cout<<"touche.posSouris.x:"<<touche.posSouris.x<<" touche.posSouris.y:"<<touche.posSouris.y<<endl;
		test_touche(jeucomplet);

		//affiche();
		SDL_Color foregroundColor;

		foregroundColor.r = 0;
		foregroundColor.g = 0;
		foregroundColor.b = 0;


		////////////////1

		drawString("Thé Fan Oc", 250, 0, font[3],foregroundColor);


		drawString("Choix de perso :", 50, 90, font[4],foregroundColor);
		drawString("1: choisi un perso clic decu ", 50, 120, font[5],foregroundColor);
		drawString("2: puis clique sur le bouton valider pour choisir le perso", 50, 140, font[5],foregroundColor);




		if(nb_perso==1 || nb_perso==2 || nb_perso==3)
		{
			guide=guide_perso1;
			drawString(strdup(nom_perso1.c_str()), 100, 180, font[4],foregroundColor);
			SDL_Rect dest1;
			dest1.x = 80;
			dest1.y = 200;

			SDL_BlitSurface(box_choix_perso , NULL , jeu.tmps , &dest1);	


			SDL_Rect desti2;
			desti2.x = 130;
			desti2.y = 240;

			SDL_Rect source2;
			source2.w = 32;
			source2.h = 48;
			source2.x = 32;
			source2.y = 48;
			SDL_BlitSurface(image_perso1 , &source2 , jeu.tmps , &desti2);	// le perso 1

			// 1 box
			AABB AB;
			AB.w = dest1.w;
			AB.h = dest1.h;
			AB.x = dest1.x;
			AB.y = dest1.y;

			if(AABBCollision(jeucomplet->touche.posSouris.x , jeucomplet->touche.posSouris.y , AB))/*On teste si on appuis sur le bouton */
			{
				if(jeucomplet->touche.clic)/*Si on clic avec le clic gauche de la souris*/
				{
					text=1;
				}

			}
		}
		//////////////:fin 1
		if(nb_perso==2 || nb_perso==3)
		{
			drawString(strdup(nom_perso2.c_str()), 360, 180, font[4],foregroundColor);
			SDL_Rect dest3;
			dest3.x = 340;
			dest3.y = 200;
			SDL_BlitSurface(box_choix_perso , NULL , jeu.tmps , &dest3);

			SDL_Rect desti4;
			desti4.x = 390;
			desti4.y = 240;

			SDL_Rect source4;
			source4.w = 32;
			source4.h = 48;
			source4.x = 32;
			source4.y = 48;
			SDL_BlitSurface(image_perso2 , &source4 , jeu.tmps , &desti4);		// le perso 2	
			//////////////:fin 2


			// 2 box
			AABB AB;
			AB.w = dest3.w;
			AB.h = dest3.h;
			AB.x = dest3.x;
			AB.y = dest3.y;

			if(AABBCollision(jeucomplet->touche.posSouris.x , jeucomplet->touche.posSouris.y , AB))/*On teste si on appuis sur le bouton */
			{			
				if(jeucomplet->touche.clic)/*Si on clic avec le clic gauche de la souris*/
				{
					text=2;
				}
			}
		}

		if(nb_perso==3)
		{
			drawString(strdup(nom_perso3.c_str()), 620, 180, font[4],foregroundColor);

			SDL_Rect dest5;
			dest5.x = 600;
			dest5.y = 200;
			SDL_BlitSurface(box_choix_perso , NULL , jeu.tmps , &dest5);	


			SDL_Rect desti6;
			desti6.x = 650;
			desti6.y = 240;


			SDL_Rect source6;
			source6.w = 32;
			source6.h = 48;
			source6.x = 32;
			source6.y = 48;
			SDL_BlitSurface(image_perso3 , &source6 , jeu.tmps , &desti6);// le perso 2


			// 3 box
			AABB AB;
			AB.w = dest5.w;
			AB.h = dest5.h;
			AB.x = dest5.x;
			AB.y = dest5.y;

			if(AABBCollision(jeucomplet->touche.posSouris.x , jeucomplet->touche.posSouris.y , AB))/*On teste si on appuis sur le bouton */
			{
				if(jeucomplet->touche.clic)/*Si on clic avec le clic gauche de la souris*/
				{
					text=3;
				}
			}

		}


		// if clic ^^





		if(text==1)
		{
			text==1;
			guide=outils::itos(guide_perso1);
			drawString("Magriavs", 150, 350, font[4],foregroundColor);
			drawString("Info: Magriavs", 350, 380, font[5],foregroundColor);
		}
		else if(text==2)
		{
			text==1;
			guide=outils::itos(guide_perso2);
			drawString("Roukrses", 150, 350, font[4],foregroundColor);
			drawString("Info: Roukrses", 350, 380, font[5],foregroundColor);
		}
		else if(text==3)
		{
			text==3;
			guide=outils::itos(guide_perso3);
			drawString("Skrills", 150, 350, font[4],foregroundColor);
			drawString("Info: Skrills", 350, 380, font[5],foregroundColor);
		}else if(text==4)
		{
			text==4;
			guide="0";
			drawString("CONNECTION ENVOYER", 150, 350, font[4],foregroundColor);
			drawString("Connection en cour", 350, 380, font[5],foregroundColor);
		}else
		{
			text==0;
			drawString("Clic sur une classe pour afficher les detail", 100, 350, font[6],foregroundColor);
		}

		// creation
		SDL_Rect destB2;
		destB2.x = 320;
		destB2.y = 500;
		SDL_BlitSurface(bouton2 , NULL , jeu.screen , &destB2);
		drawString("Nouveau", 350, 510, font[6],foregroundColor);


		// 2 box
		AABB AB;
		AB.w = destB2.w;
		AB.h = destB2.h;
		AB.x = destB2.x;
		AB.y = destB2.y;

		if(AABBCollision(jeucomplet->touche.posSouris.x , jeucomplet->touche.posSouris.y , AB))/*On teste si on appuis sur le bouton */
		{			

			if(jeucomplet->touche.clic)/*Si on clic avec le clic gauche de la souris*/
			{
				crer_perso(jeucomplet);
			}
		}else{
			///////////// bouton valider
			SDL_Rect destB2;
			destB2.x = 320;
			destB2.y = 500;
			SDL_BlitSurface(bouton , NULL , jeu.screen , &destB2);
			drawString("Nouveau", 350, 510, font[6],foregroundColor);
		}

		//connecter
		if(text==1||text==2||text==3)
		{
			SDL_Rect destB2;
			destB2.x = 320;
			destB2.y = 550;
			SDL_BlitSurface(bouton2 , NULL , jeu.screen , &destB2);
			drawString("Valider", 350, 560, font[6],foregroundColor);


			// 2 box
			AABB AB;
			AB.w = destB2.w;
			AB.h = destB2.h;
			AB.x = destB2.x;
			AB.y = destB2.y;

			if(AABBCollision(jeucomplet->touche.posSouris.x , jeucomplet->touche.posSouris.y , AB))/*On teste si on appuis sur le bouton */
			{			
				if(jeucomplet->touche.clic)/*Si on clic avec le clic gauche de la souris*/
				{
					text=4;
					connecte_perso(guide,jeucomplet);
					traitement_complet_message(jeucomplet);/////////////////////////////


					jeucomplet->jeu.perso[_PERSO_LOCAL_].dest.x = 50;/*Position du perso en x*/
					jeucomplet->jeu.perso[_PERSO_LOCAL_].dest.y = 50;/*Position du perso en y*/
					jeucomplet->jeu.perso[_PERSO_LOCAL_].map = 1;/*map */
					jeucomplet->jeu.perso[_PERSO_LOCAL_].guid = 112;/*Position du perso en y*/
					jeucomplet->jeu.perso[_PERSO_LOCAL_].nom = "";/*Position du perso en y*/

					jouer(jeu.screen, false, jeu.tmps,jeucomplet);
				}
			}

		}else{
			///////////// bouton valider
			SDL_Rect destB2;
			destB2.x = 320;
			destB2.y = 550;
			SDL_BlitSurface(bouton , NULL , jeu.screen , &destB2);
			drawString("Valider", 350, 560, font[6],foregroundColor);
		}

		if(SDL_BlitSurface(jeu.tmps, NULL, jeu.screen, NULL) < 0)
			fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());

		SDL_Flip(jeu.screen);
		//pause();

	}
}

void partie::crer_perso(JeuComplet *jeucomplet)
{


	memset(&jeucomplet->touche , 0 , sizeof(Touche));/*Toute les variable de la structure Touche sont miss à 0*/

	jeu.tmps = SDL_ConvertSurface(jeu.screen, jeu.screen->format, jeu.screen->flags);

	SDL_SetColorKey(jeu.tmps, SDL_SRCCOLORKEY, SDL_MapRGB(jeu.tmps->format, 255, 255, 255));

	int go;
	go=1;

	int afficher;
	afficher=0;

	int text;
	text =0;

	SDL_Surface *box_choix_perso = NULL,*bouton = NULL,*bouton2 = NULL, *image_perso1 = NULL, *image_perso2 = NULL, *image_perso3 = NULL;

	box_choix_perso = load_image("img/box-choix-perso.png");

	bouton = load_image("img/Image_432.png");
	bouton2 = load_image("img/Image_436.png");

	image_perso1 = load_image("img/jeu/heros/Magriavs/perso_marche1.png");
	image_perso2 = load_image("img/jeu/heros/Roukrses/perso_marche1.png");
	image_perso3 = load_image("img/jeu/heros/Skrills/perso_marche1.png");

	///boite text
	SDL_Color Couleur_Noir;
	Couleur_Noir.r = 0;
	Couleur_Noir.g = 0;
	Couleur_Noir.b = 0;

	TextEdition pseudo;
	memset(&pseudo, 0, sizeof(TextEdition));
	pseudo.blitStyle = TE_BLITSTYLE_BLENDED;
	pseudo.colorBG = Couleur_Noir;

	SDL_Rect pos;
	pos.x = 305;
	pos.y = 450;
	pos.w = 200;
	pos.h = 30;
	TE_NewTextEdition(&pseudo, 20, pos, font[7], Couleur_Noir, TE_STYLE_NORMAL | TE_STYLE_HSCROLL | TE_STYLE_VSCROLL | TE_STYLE_BLITRGBA);
	TE_SetEditionText(&pseudo, "dams");
	//
	SDL_Event event;
	int clic=0;
	int done = 0;

	SDL_Rect posSouris;
	posSouris.x = 0;
	posSouris.y = 0;


	while (go == 1)
	{
		SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

		SDL_WaitEvent(&event);
		switch (event.type)
		{



		case SDL_MOUSEBUTTONDOWN :
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT :
				clic = 1;
				break;
			default :
				break;
			}
			break;
		case SDL_MOUSEBUTTONUP :
			clic = 0;
			break;

		case SDL_MOUSEMOTION:
			posSouris.x = event.motion.x;
			posSouris.y = event.motion.y;
			break;

		default:
			break;
		}
		SDL_FillRect(jeu.screen , NULL , SDL_MapRGB(jeu.screen->format, 255, 255, 255));/*On rempli l'écran de noir*/
		SDL_FillRect(jeu.tmps , NULL , SDL_MapRGB(jeu.tmps->format, 255, 255, 255));/*On rempli l'écran de noir*/
		//cout<<"touche.posSouris.x:"<<touche.posSouris.x<<" touche.posSouris.y:"<<touche.posSouris.y<<endl;

		if(text==1||text==2||text==3)
		{
			if (!done)
			{
				SDL_FillRect(jeu.screen, &pos, SDL_MapRGB(jeu.screen->format, 255,0,0));
				drawString("Pseudo:", 360, 420, font[5],Couleur_Noir);

				TE_HoldTextEdition(&pseudo, event);
				TE_DisplayTextEdition(&pseudo);

				test_touche(jeucomplet);			
				printf("pseudo: %s\n", pseudo.text);
			}

		}

		//affiche();
		SDL_Color foregroundColor;

		foregroundColor.r = 0;
		foregroundColor.g = 0;
		foregroundColor.b = 0;


		////////////////1

		drawString("Thé Fan Oc", 250, 0, font[3],foregroundColor);


		drawString("Création de perso :", 50, 90, font[4],foregroundColor);
		drawString("1: choisi une classe clic decu ", 50, 120, font[5],foregroundColor);
		drawString("2: puis clique sur le bouton valider pour choisir cette classe", 50, 140, font[5],foregroundColor);



		drawString("Magriavs", 100, 180, font[4],foregroundColor);
		SDL_Rect dest1;
		dest1.x = 80;
		dest1.y = 200;

		SDL_BlitSurface(box_choix_perso , NULL , jeu.tmps , &dest1);	


		SDL_Rect desti2;
		desti2.x = 130;
		desti2.y = 240;

		SDL_Rect source2;
		source2.w = 32;
		source2.h = 48;
		source2.x = 32;
		source2.y = 48;
		SDL_BlitSurface(image_perso1 , &source2 , jeu.tmps , &desti2);	// le perso 1

		//////////////:fin 1

		drawString("Roukrses", 360, 180, font[4],foregroundColor);
		SDL_Rect dest3;
		dest3.x = 340;
		dest3.y = 200;
		SDL_BlitSurface(box_choix_perso , NULL , jeu.tmps , &dest3);

		SDL_Rect desti4;
		desti4.x = 390;
		desti4.y = 240;

		SDL_Rect source4;
		source4.w = 32;
		source4.h = 48;
		source4.x = 32;
		source4.y = 48;
		SDL_BlitSurface(image_perso2 , &source4 , jeu.tmps , &desti4);		// le perso 2	
		//////////////:fin 2

		drawString("Skrills", 620, 180, font[4],foregroundColor);

		SDL_Rect dest5;
		dest5.x = 600;
		dest5.y = 200;
		SDL_BlitSurface(box_choix_perso , NULL , jeu.tmps , &dest5);	


		SDL_Rect desti6;
		desti6.x = 650;
		desti6.y = 240;


		SDL_Rect source6;
		source6.w = 32;
		source6.h = 48;
		source6.x = 32;
		source6.y = 48;
		SDL_BlitSurface(image_perso3 , &source6 , jeu.tmps , &desti6);// le perso 2




		// if clic ^^
		// 1 box
		AABB AB;
		AB.w = dest1.w;
		AB.h = dest1.h;
		AB.x = dest1.x;
		AB.y = dest1.y;

		if(AABBCollision(posSouris.x , posSouris.y , AB))/*On teste si on appuis sur le bouton */
		{
			if(clic)/*Si on clic avec le clic gauche de la souris*/
			{
				text=1;
			}

		}
		// 2 box
		AB.w = dest3.w;
		AB.h = dest3.h;
		AB.x = dest3.x;
		AB.y = dest3.y;

		if(AABBCollision(posSouris.x , posSouris.y , AB))/*On teste si on appuis sur le bouton */
		{			
			if(clic)/*Si on clic avec le clic gauche de la souris*/
			{
				text=2;
			}
		}

		// 3 box
		AB.w = dest5.w;
		AB.h = dest5.h;
		AB.x = dest5.x;
		AB.y = dest5.y;

		if(AABBCollision(posSouris.x , posSouris.y , AB))/*On teste si on appuis sur le bouton */
		{
			if(clic)/*Si on clic avec le clic gauche de la souris*/
			{
				text=3;
			}
		}

		if(text==1)
		{
			drawString("Magriavs", 150, 350, font[4],foregroundColor);
			drawString("Info: Magriavs", 350, 380, font[5],foregroundColor);
		}
		else if(text==2)
		{
			drawString("Roukrses", 150, 350, font[4],foregroundColor);
			drawString("Info: Roukrses", 350, 380, font[5],foregroundColor);
		}
		else if(text==3)
		{
			drawString("Skrills", 150, 350, font[4],foregroundColor);
			drawString("Info: Skrills", 350, 380, font[5],foregroundColor);
		}else if(text==4)
		{
			drawString("CREATIN ENVOYER", 150, 350, font[4],foregroundColor);
			drawString("text 1PPPPPPPPPPPPPPP", 350, 380, font[5],foregroundColor);
		}else
		{
			text==0;
			drawString("Clic sur un perso pour afficher les detail", 150, 350, font[6],foregroundColor);
		}



		if(text==1||text==2||text==3)
		{
			SDL_Rect destB2;
			destB2.x = 320;
			destB2.y = 550;
			SDL_BlitSurface(bouton2 , NULL , jeu.screen , &destB2);
			drawString("Valider", 350, 560, font[6],foregroundColor);


			// 2 box
			AB.w = destB2.w;
			AB.h = destB2.h;
			AB.x = destB2.x;
			AB.y = destB2.y;

			if(AABBCollision(posSouris.x , posSouris.y , AB))/*On teste si on appuis sur le bouton */
			{			
				if(clic)/*Si on clic avec le clic gauche de la souris*/
				{
					text=4;
					string SPseudochaine(pseudo.text);

					creation_perso(SPseudochaine,jeucomplet);
					traitement_complet_message(jeucomplet);/////////////////////////////


					if(creation_test(jeucomplet)==0){


						SDL_Color foregroundColor;
						foregroundColor.r = 255;
						foregroundColor.g = 0;
						foregroundColor.b = 0;

						drawString("pseudo incorrect ", 250, 450, font[4],foregroundColor);
						drawString("recommence dans 5 secondes!!!!", 250, 500, font[4],foregroundColor);

						if(SDL_BlitSurface(jeu.tmps, NULL, jeu.screen, NULL) < 0)
							fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());

						SDL_Flip(jeu.screen);
						SDL_Delay(3000);
						done=0;
					}


				}
			}

		}else{
			///////////// bouton valider
			SDL_Rect destB2;
			destB2.x = 320;
			destB2.y = 550;
			SDL_BlitSurface(bouton , NULL , jeu.screen , &destB2);
			drawString("Valider", 350, 560, font[6],foregroundColor);
		}

		if(SDL_BlitSurface(jeu.tmps, NULL, jeu.screen, NULL) < 0)
			fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());

		SDL_Flip(jeu.screen);
		//pause();

	}

	/////fin new text box




}

void partie::logger(string SPseudochaine, string SMdpchaine, JeuComplet *jeucomplet)
{
	string msg;
	msg=this->perso_[_PERSO_LOCAL_]->creerMsg_logger(SPseudochaine, SMdpchaine, jeucomplet);
	cout<<"logger : "<<msg<<endl;
	this->client_->envoyer_message(msg);
}

void partie::connecte_perso(string guide, JeuComplet *jeucomplet)
{
	string msg;
	msg=this->perso_[_PERSO_LOCAL_]->creerMsg_connecte_perso(guide, jeucomplet);
	cout<<"connect perso : "<<msg<<endl;
	this->client_->envoyer_message(msg);
}
void partie::creation_perso(string SPseudochaine, JeuComplet *jeucomplet)
{
	string msg;
	msg=this->perso_[_PERSO_LOCAL_]->creerMsg_creation_perso(SPseudochaine, jeucomplet);
	cout<<"creation perso : "<<msg<<endl;
	this->client_->envoyer_message(msg);
}

int partie::logger_test(JeuComplet *jeucomplet)
{
	cout<<"DEBUGGG ACCES logger_test ->"<<jeucomplet->jeu.perso[_PERSO_LOCAL_].acces<<endl;
	int test = 0;

	if(jeucomplet->jeu.perso[_PERSO_LOCAL_].acces == "load_login_reussi")
	{
		//jouer(jeu.screen, false, jeu.tmps,jeucomplet);
		choix_perso(jeucomplet);
		//crer_perso(jeucomplet);
		test =1;
	}
	return test;
}

int partie::creation_test(JeuComplet *jeucomplet)
{
	cout<<"DEBUGGG ACCES creation_reussi ->"<<jeucomplet->jeu.perso[_PERSO_LOCAL_].creation_reussi<<endl;
	int test = 0;

	if(jeucomplet->jeu.perso[_PERSO_LOCAL_].creation_reussi == "!OK")
	{

		//jouer(jeu.screen, false, jeu.tmps,jeucomplet);
		choix_perso(jeucomplet);

		//crer_perso(jeucomplet);
		test =1;
	}
	return test;
}


void update_perso(JeuComplet *jeucomplet,int p)
{
	int i;
	for(i = 0 ; i < NB_ALL_ANIM_PERSO_EVER_OF_THE_WORLD ; i++)
	{
		update_frame_perso(jeucomplet , &jeucomplet->touche , &jeucomplet->jeu.perso[_PERSO_LOCAL_].animPerso[i]);
	}
	for(i = 0 ; i < NB_ALL_ANIM_PERSO_EVER_OF_THE_WORLD ; i++)
	{
		update_orientation(jeucomplet , &jeucomplet->touche , &jeucomplet->jeu.perso[_PERSO_LOCAL_].animPerso[i]);
	}

}

void update_frame_perso(JeuComplet *jeucomplet , Touche *touche , Anim *anim)/*Le héros à une fonction d'avancement de frame, c'est plus gérable*/
{
	if(touche->espace || touche->haut || touche->bas || touche->gauche || touche->droite || touche->lshift)
	{
		anim->compteurTemporaire--;/*On baisse le compteur de l'anim envoyé en paramètre*/
		if(anim->compteurTemporaire <= 0)/*Si le compteur est égale à 0*/
		{
			anim->compteurTemporaire = anim->compteurDefinitife;/*On remet le compteur au compteur normale de l'anim envoyé en paramètre*/
			anim->animCouranteX++;/*On augmente l'anim comme ça le personnage peut marcher*/
			if(anim->animCouranteX >= anim->nbFrame)/*Si on arrive au nombre maximum d'animation (l'animation de l'attaque et de la marche n'en font que 4)*/
				anim->animCouranteX = 0;/*On la remet au début -> et on refait l'animation */
		}
		anim->src.x = anim->animCouranteX * anim->src.w;/*On régle le découpage pour savoir à quelle image de l'anim on en est*/
	}
}


void update_orientation(JeuComplet *jeucomplet , Touche *touche , Anim *anim)
{
	/*Si l'anim est le héros*/
	if(anim->classe == HEROS)/*Pour mettre à jour l'animation de la marche il ne faut pas que le perso attaque*/
		update_orientation_anim(jeucomplet , touche , anim , anim[jeucomplet->jeu.perso[_PERSO_LOCAL_].animActuelleNormale].src.h);

}

void update_orientation_anim(JeuComplet *jeucomplet , Touche *touche , Anim *anim , int dimensionY)
{
	/*On met à jour l'orientation du perso selon la touche appuyé (haut , bas , droite , gauche)*/
	if(touche->droite)
	{
		anim[jeucomplet->jeu.perso[_PERSO_LOCAL_].animActuelleNormale].animCouranteY = DROITE;
		anim[jeucomplet->jeu.perso[_PERSO_LOCAL_].animActuelleNormale].src.y = anim[jeucomplet->jeu.perso[_PERSO_LOCAL_].animActuelleNormale].animCouranteY * dimensionY;
	}
	if(touche->gauche)
	{
		anim[jeucomplet->jeu.perso[_PERSO_LOCAL_].animActuelleNormale].animCouranteY = GAUCHE;
		anim[jeucomplet->jeu.perso[_PERSO_LOCAL_].animActuelleNormale].src.y = anim[jeucomplet->jeu.perso[_PERSO_LOCAL_].animActuelleNormale].animCouranteY * dimensionY;
	}
	if(touche->bas)
	{
		anim[jeucomplet->jeu.perso[_PERSO_LOCAL_].animActuelleNormale].animCouranteY = BAS;
		anim[jeucomplet->jeu.perso[_PERSO_LOCAL_].animActuelleNormale].src.y = anim[jeucomplet->jeu.perso[_PERSO_LOCAL_].animActuelleNormale].animCouranteY * dimensionY;
	}
	if(touche->haut)
	{
		anim[jeucomplet->jeu.perso[_PERSO_LOCAL_].animActuelleNormale].animCouranteY = HAUT;
		anim[jeucomplet->jeu.perso[_PERSO_LOCAL_].animActuelleNormale].src.y = anim[jeucomplet->jeu.perso[_PERSO_LOCAL_].animActuelleNormale].animCouranteY * dimensionY;
	}
}

void partie::load_anim_perso(JeuComplet *jeucomplet, int p)
{
	FILE *fp = NULL;
	int i , nombreAnim;
	char file[MAX_CHAINE];
	/*
	roukrses
	skrills
	magriavs
	*/

	cout<<"Chargement des definitions des images de la race :"<<jeucomplet->jeu.perso[p].race<<" pour le perso :"<<jeucomplet->jeu.perso[p].nom<<endl;


	if(jeucomplet->jeu.perso[p].race ==1){
		fp = fopen("img/jeu/heros/Roukrses/infos.txt" , "r");
	}else if(jeucomplet->jeu.perso[p].race ==2){
		fp = fopen("img/jeu/heros/Skrills/infos.txt" , "r");
	}else{
		fp = fopen("img/jeu/heros/Magriavs/infos.txt" , "r");
	}

	if(fp == NULL)
		cout<<"erreur load anim_perso :) "<<endl;

	fscanf(fp , "%d" , &nombreAnim);

	jeucomplet->jeu.perso[p].animPerso = (Anim*) malloc(sizeof(Anim) * nombreAnim);
	if(jeucomplet->jeu.perso[p].animPerso == NULL)
		cout<<"erreur avec animPerso !"<<endl;

	for(i = 0; i < nombreAnim ; i ++)
	{
		fscanf(fp , "%s" , file);
		jeucomplet->jeu.perso[p].animPerso[i].image = load_image(file);
		fscanf(fp , "%d" , &jeucomplet->jeu.perso[p].animPerso[i].classe);
		fscanf(fp , "%d" , &jeucomplet->jeu.perso[p].animPerso[i].nbFrame);
		fscanf(fp , "%d" , &jeucomplet->jeu.perso[p].animPerso[i].src.w);
		fscanf(fp , "%d" , &jeucomplet->jeu.perso[p].animPerso[i].src.h);
		fscanf(fp , "%d" , &jeucomplet->jeu.perso[p].animPerso[i].compteurDefinitife);
		jeucomplet->jeu.perso[p].animPerso[i].compteurTemporaire = jeucomplet->jeu.perso[p].animPerso[i].compteurDefinitife;
		jeucomplet->jeu.perso[p].animPerso[i].animCouranteX = 0;
		jeucomplet->jeu.perso[p].animPerso[i].animCouranteY = DROITE;
		jeucomplet->jeu.perso[p].animPerso[i].src.x = jeucomplet->jeu.perso[p].animPerso[i].src.w * jeucomplet->jeu.perso[p].animPerso[i].animCouranteX;
		jeucomplet->jeu.perso[p].animPerso[i].src.y = jeucomplet->jeu.perso[p].animPerso[i].src.h * jeucomplet->jeu.perso[p].animPerso[i].animCouranteY;
	}

	fclose(fp);
}

void update_touche_perso(JeuComplet *jeucomplet)
{
	int vx , vy;
	jeucomplet->jeu.perso[_PERSO_LOCAL_].animPerso[jeucomplet->jeu.perso[_PERSO_LOCAL_].animActuelleNormale].dest.x = jeucomplet->jeu.perso[_PERSO_LOCAL_].dest.x;
	jeucomplet->jeu.perso[_PERSO_LOCAL_].animPerso[jeucomplet->jeu.perso[_PERSO_LOCAL_].animActuelleNormale].dest.y = jeucomplet->jeu.perso[_PERSO_LOCAL_].dest.y;
	jeucomplet->jeu.perso[_PERSO_LOCAL_].animPersoTempo = jeucomplet->jeu.perso[_PERSO_LOCAL_].animPerso[jeucomplet->jeu.perso[_PERSO_LOCAL_].animActuelleNormale];



	if(jeucomplet->touche.haut == ON || jeucomplet->touche.droite == ON || jeucomplet->touche.gauche == ON || jeucomplet->touche.bas == ON)
		jeucomplet->jeu.perso[_PERSO_LOCAL_].animActuelleNormale = 0;/*On reviens à l'anim de la marche !*/
	if(jeucomplet->touche.haut == OFF && jeucomplet->touche.droite == OFF && jeucomplet->touche.gauche == OFF && jeucomplet->touche.bas == OFF)
		jeucomplet->jeu.perso[_PERSO_LOCAL_].animActuelleNormale = AUCUNE_OCCUPATION;/*On ne fait rien alors le perso bougera la tête*/
	if(jeucomplet->touche.c == ON)
	{
		jeucomplet->jeu.perso[_PERSO_LOCAL_].animActuelleObjet++;
		if(jeucomplet->jeu.perso[_PERSO_LOCAL_].animActuelleObjet == NB_ALL_ANIM_PERSO_EVER_OF_THE_WORLD)
			jeucomplet->jeu.perso[_PERSO_LOCAL_].animActuelleObjet = 3;
		jeucomplet->touche.c = OFF;
	}

	update_vector(jeucomplet , &vx , &vy);/*A l'aide de deux variables on voit si quand on bouge le perso il touche un mur si */
	update_deplacement(jeucomplet , &jeucomplet->jeu.perso[_PERSO_LOCAL_].animPersoTempo, vx , vy);/*oui on ne valide pas les variables sur le perso car il touche un mur si non on le bouge normalement*/
}

void update_vector(JeuComplet *jeucomplet , int *vx , int *vy)
{
	int vitesse = VITESSE_PERSO;/*La vitesse du personnage*/
	*vy = 0;
	*vx = 0;



	if( jeucomplet->touche.clic==1 ){ // si on bouge la souris ET qu'on a d'abord cliqué

		if((jeucomplet->jeu.perso[_PERSO_LOCAL_].dest.x - jeucomplet->jeu.decor.scroll.xscroll )>jeucomplet->touche.posSouris.x)//DROITE
			*vx -= vitesse;/*On augmente la variable x de la vitesse et on fait ça pour chaque direction */
		if((jeucomplet->jeu.perso[_PERSO_LOCAL_].dest.x - jeucomplet->jeu.decor.scroll.xscroll )<jeucomplet->touche.posSouris.x)//GAUCHE
			*vx += vitesse;
		if((jeucomplet->jeu.perso[_PERSO_LOCAL_].dest.y  - jeucomplet->jeu.decor.scroll.yscroll)>jeucomplet->touche.posSouris.y)//BAS
			*vy -= vitesse;
		if((jeucomplet->jeu.perso[_PERSO_LOCAL_].dest.y  - jeucomplet->jeu.decor.scroll.yscroll)<jeucomplet->touche.posSouris.y)//HAUT
			*vy += vitesse;		
		cout<<" SOURIE jeucomplet->touche.posSouris.x "<<jeucomplet->touche.posSouris.x<<endl;

	}


	if(jeucomplet->touche.droite)
		*vx += vitesse;/*On augmente la variable x de la vitesse et on fait ça pour chaque direction */
	if(jeucomplet->touche.gauche)
		*vx -= vitesse;
	if(jeucomplet->touche.bas)
		*vy += vitesse;
	if(jeucomplet->touche.haut)
		*vy -= vitesse;

}





partie::partie(JeuComplet *jeucomplet)
{
	for(int i=0;i<_MAXPERSO_;i++)
		this->perso_[i]=NULL;

	this->perso_[_PERSO_LOCAL_] = new perso();
	this->client_ = new client(_PORT_);
	this->client_->connecter(jeucomplet, this->perso_[_PERSO_LOCAL_]->creerMsg(jeucomplet) );

	this->pleinEcran_ = false;
}

partie::~partie(){}


Uint32 time_left(Uint32 next_time)
{
	Uint32 now;

	now = SDL_GetTicks();
	if(next_time <= now)
		return 0;
	else
		return next_time - now;
}

void partie::traitement_complet_message(JeuComplet *jeucomplet)
{
	//bug obliger de faire 2 foi la manip ou ca bug probleme avec le truc des login ???
	this->client_->lire_messages();
	traiter_messages(jeucomplet);

	this->client_->lire_messages();
	traiter_messages(jeucomplet);
}
/*
void partie::jouer(SDL_Surface *screen, bool pe, SDL_Surface *tmps); gère le déroulement de 
la boucle de jeu.
*/
void partie::jouer(SDL_Surface *screen, bool pe, SDL_Surface *tmps,JeuComplet *jeucomplet)
{
	string msg;
	Uint32 next_time=0;
	this->pleinEcran_ = pe;


	SDL_Color foregroundColor, foregroundColor2;

	foregroundColor2.r = 255;
	foregroundColor2.g = 255;
	foregroundColor2.b = 255;

	foregroundColor.r = 234;
	foregroundColor.g = 211;
	foregroundColor.b = 69;

	while(this->perso_[_PERSO_LOCAL_]->getId() == " "){
		this->client_->lire_messages();
		traiter_messages(jeucomplet);
	}

	//jeucomplet->jeu.perso[_PERSO_LOCAL_].dest.x = 417;/*Position du perso en x*/
	//jeucomplet->jeu.perso[_PERSO_LOCAL_].dest.y = 112;/*Position du perso en y*/
	jeucomplet->jeu.perso[_PERSO_LOCAL_].animActuelleNormale = 0;/*Anim de la marche, 0 = marche normal*/
	jeucomplet->jeu.perso[_PERSO_LOCAL_].animActuelleObjet = 0;/*Anim des armes, 2 = EPEE*/
	jeucomplet->jeu.perso[_PERSO_LOCAL_].activeNage = OFF;

	jeucomplet->jeu.perso[_PERSO_LOCAL_].persoCamouflage = load_image("img/jeu/heros/perso_camouflage.png");
	jeucomplet->jeu.perso[_PERSO_LOCAL_].activeCamouflage = OFF;
	jeucomplet->jeu.perso[_PERSO_LOCAL_].collisionPnjPerso = OFF;

	jeucomplet->jeu.perso[_PERSO_LOCAL_].hp = 100;

	load_anim_perso(jeucomplet,_PERSO_LOCAL_);// on cre le local  donc 0

	load_tileset(jeucomplet);/*On charge le tileset*/
	load_tileprops(jeucomplet);/*Puis le tileprops qui definit si chaque tile est un mur ou non */

	load_map(jeucomplet , strdup(("map/map" + outils::itos(jeucomplet->jeu.perso[_PERSO_LOCAL_].map) + "/map" + outils::itos(jeucomplet->jeu.perso[_PERSO_LOCAL_].map) + ".txt").c_str()));/*On commence dans la map 1 */

	memset(&jeucomplet->touche , 0 , sizeof(Touche));/*Toute les variable de la structure Touche sont miss à 0*/


	
	jeucomplet->jeu.clic_activer=0;


	while(true){

		SDL_FillRect(jeu.screen , NULL , 0);/*On rempli l'écran de noir*/
		SDL_FillRect(jeu.tmps , NULL , 0);/*On rempli l'écran de noir*/	

		// Création du message contenant les infos à envoyer au serveur pour l'informer
		// notement de notre position dans l'écran (pero->posx_ et perso->posy);
		msg=this->perso_[_PERSO_LOCAL_]->creerMsg(jeucomplet);

		// Envoie du message au serveur
		this->client_->envoyer_message(msg);		

		// On traite les messages lus par le thread créé précédement (c la méthode lire_messages();
		// de la classe client qui tourne dans ce thread).
		// Ce traitement permet notement de mettre à jour les positions des autres joueurs connectés.
		traiter_messages(jeucomplet);
		scroll Scroll;

		Scroll.windows_pos(jeucomplet);
		Scroll.load_valeur_scrool(jeucomplet);

		update_dest_scroll_monstre(jeucomplet);
		// Gestion des touches du clavier pour bouger le perso ou quitter l'appli
		//gerer_touches(jeucomplet);
		test_touche(jeucomplet);
		update_touche_perso(jeucomplet);
		update_perso(jeucomplet,0);
		update_monstres(jeucomplet);
		test_clic_monstre(jeucomplet);

		this->client_->lire_messages();



		draw_couche1(jeucomplet);/*On dessine la couches de blit 1*/
		draw_couche2(jeucomplet);/*On dessine la couches de blit 2*/	

		// Rendu des différentes données à l'écran.
		animer(jeucomplet);		
		draw_monstres(jeucomplet);

		draw_couche3(jeucomplet);/*On dessine la couches de blit 2*/


		jeucomplet->jeu.decor.compteurTileset--;
		if(jeucomplet->jeu.decor.compteurTileset <= 0)
		{
			jeucomplet->jeu.decor.compteurTileset = COMPTEUR_CHANGE_TILESET;
			jeucomplet->jeu.decor.tilesetActuelle++;
			if(jeucomplet->jeu.decor.tilesetActuelle == NB_TILESETS)
				jeucomplet->jeu.decor.tilesetActuelle = 0;
		}


		char text[200];
		memset(&text, 0, sizeof(text));
		sprintf(text, "CARTE ACTUEL :%d", jeucomplet->jeu.quelMap);
		drawString(text, 2, 2, font[0],foregroundColor2); 	


		char hp[200];
		memset(&hp, 0, sizeof(hp));
		sprintf(hp, "HP :%d", jeucomplet->jeu.perso[_PERSO_LOCAL_].hp);
		drawString(hp, 300, 2, font[0],foregroundColor2); 	


		//re activer pour empeche les sacade vu la maj des mob actuel a revoir apres
		//cout<<time_left(next_time)<<endl;
		//SDL_Delay(time_left(next_time));
		//next_time = SDL_GetTicks() + _TICK_INTERVAL_;


		if(SDL_BlitSurface(jeu.tmps, NULL, jeu.screen, NULL) < 0)
			fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());

		SDL_Flip(jeu.screen);		
	}
}

void partie::animer(JeuComplet *jeucomplet)
{
	// On blitte les perso un à un sur la surface temporaire.
	for(int i=0 ; i<_MAXPERSO_ ; i++){
		if(this->perso_[i])
			//cout<<"Nom :"<<jeucomplet->jeu.perso[_PERSO_LOCAL_].nom<<endl;
			this->perso_[i]->afficher(jeucomplet,i);
	}
}

/*
void partie::traiter_messages(); est surement la méthode la plus obscure.
Elle est chargée d'interpréter les messages reçus du serveur (par exemple 
l'arrivée d'un nouveau perso ou l'actualisation des coordonnées d'un perso, ...)
et de faire les actions correspondantes.
*/
void partie::traiter_messages(JeuComplet *jeucomplet)
{
	int i, posx, posy;
	string msg, nom, id, race, acces, acces_suite;
	cout<<"traiter_messages : "<<MSG_<<endl;
	// On switch sur le type de message reçu (pour l'instant seuls 2 type de message :
	//	- nouveau client
	//	- position du client
	// Le type de message est codé par le premier caractère du message suivant un protocole 
	// à la con que g défini : 'N' pour nouveau client et 'P' pour position.
	switch(MSG_[0]){
	case _NOUVEAU_CLIENT_:		// Message indiquant la connexion d'un nouveau perso
	case _BIENVENUE_ID_:
		//cout<<"TRAITER MESSAGE: "<<MSG_[0]<<endl;
		msg = outils::ctos(MSG_);

		if( MSG_[0] == _BIENVENUE_ID_ ){
			msg = outils::extraire_message(msg, MSG_[0], &nom, &id, &posx, &posy, &race, &acces, &acces_suite);
			this->perso_[_PERSO_LOCAL_]->setId(id);				
		}

		// On va rabotter msg à chaque fois qu'on a traité un perso						
		while( msg.length()>3 ){

			for(i=1 ; i<_MAXPERSO_ && this->perso_[i] ; i++);
			if(i==_MAXPERSO_)
				cout<<"Erreur de conception, il y a trop de persos..."<<endl;

			msg = outils::extraire_message(msg, _NOUVEAU_CLIENT_, &nom, &id, &posx, &posy, &race, &acces, &acces_suite);

			this->perso_[i] = new perso(nom, posx, posy, race);
			this->perso_[i]->setId(id);
			this->perso_[i]->setrace(race);
			cout<<"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^Nom du perso: "<<nom<<endl;

			//ici les autre

			jeucomplet->jeu.perso[i].dest.x = 417;/*Position du perso en x*/
			jeucomplet->jeu.perso[i].dest.y = 112;/*Position du perso en y*/
			jeucomplet->jeu.perso[i].animActuelleNormale = 0;/*Anim de la marche, 0 = marche normal*/
			jeucomplet->jeu.perso[i].animActuelleObjet = 0;/*Anim des armes, 2 = EPEE*/
			jeucomplet->jeu.perso[i].activeNage = OFF;

			jeucomplet->jeu.perso[i].persoCamouflage = load_image("img/jeu/heros/perso_camouflage.png");
			jeucomplet->jeu.perso[i].activeCamouflage = OFF;
			jeucomplet->jeu.perso[i].collisionPnjPerso = OFF;
			jeucomplet->jeu.perso[i].hp = 100;
			jeucomplet->jeu.perso[i].race = atoi(race.c_str());
			jeucomplet->jeu.perso[i].nom = nom;
			load_anim_perso(jeucomplet,i);// on cre le local  donc 0

		}

		MSG_[0]=' ';
		break;


	case _MESSAGE_NORMAL_:
		msg = outils::ctos(MSG_);
		while( msg.length()>3 ){

			msg = outils::extraire_message(msg, MSG_[0], &nom, &id, &posx, &posy, &race, &acces, &acces_suite);
			i = this->getIndicePerso(id);
			if(i>0)
				this->perso_[i]->update_position(posx, posy);
			else
				cout<<"BOG ; id non trouve dans la liste des persos..."<<endl;				
		}
		MSG_[0]=' ';
		break;

	case _DECONNEXION_CLIENT_:
		msg = outils::ctos(MSG_);
		msg = outils::extraire_message(msg, MSG_[0], &nom, &id, &posx, &posy, &race, &acces, &acces_suite);
		i = this->getIndicePerso(id);
		delete this->perso_[i];
		this->perso_[i] = NULL;
		break;

	case _MESSAGE_CONNECTION_:

		cout<<"BOG ; _MESSAGE_CONNECTION_debut"<<endl;

		msg = outils::ctos(MSG_);
		msg = outils::extraire_message(msg, _MESSAGE_CONNECTION_, &nom, &id, &posx, &posy, &race, &acces, &acces_suite);
		jeucomplet->jeu.perso[_PERSO_LOCAL_].acces = acces;
		jeucomplet->jeu.perso[_PERSO_LOCAL_].acces_suite = acces_suite;
		cout<<"msg ->"<<msg<<endl;
		cout<<"acces ->"<<acces<<endl;
		cout<<"acces_suite ->"<<acces_suite<<endl;
		cout<<"BOG ; _MESSAGE_CONNECTION_ fin"<<endl;


		// si le bon message comme quoi on es connect  on lance le jeu et load le joeur :)
		break;

	case _MESSAGE_GUID_CONNECT_:

		cout<<"BOG ; __MESSAGE_GUID_CONNECT__debut"<<endl;

		msg = outils::ctos(MSG_);
		msg = outils::extraire_message(msg, _MESSAGE_GUID_CONNECT_, &nom, &id, &posx, &posy, &race, &acces, &acces_suite);
		jeucomplet->jeu.perso[_PERSO_LOCAL_].acces = acces;
		jeucomplet->jeu.perso[_PERSO_LOCAL_].acces_suite = acces_suite;
		cout<<"msg ->"<<msg<<endl;
		cout<<"acces ->"<<acces<<endl;
		cout<<"acces_suite ->"<<acces_suite<<endl;
		cout<<"BOG ; __MESSAGE_GUID_CONNECT__ fin"<<endl;

		break;

	case _MESSAGE_CREATION_PERSO_:

		msg = outils::ctos(MSG_);
		msg = outils::extraire_message(msg, _MESSAGE_CREATION_PERSO_, &nom, &id, &posx, &posy, &race, &acces, &acces_suite);
		jeucomplet->jeu.perso[_PERSO_LOCAL_].creation_reussi = acces;
		jeucomplet->jeu.perso[_PERSO_LOCAL_].acces_suite = acces_suite;
		cout<<"msg ->"<<msg<<endl;
		cout<<"acces ->"<<acces<<endl;
		cout<<"acces_suite ->"<<acces_suite<<endl;
		cout<<"jeucomplet->jeu.perso[_PERSO_LOCAL_].creation_reussi ->"<<jeucomplet->jeu.perso[_PERSO_LOCAL_].creation_reussi<<endl;
		cout<<"BOG ; _MESSAGE_CONNECTION_ fin"<<endl;

		break;

	case _MESSAGE_CREATURE_INFO_:

		cout<<"BOG ; _MESSAGE_CREATURE_INFO_ debut"<<endl;

		msg = outils::ctos(MSG_);
		msg = outils::extraire_message(msg, _MESSAGE_CREATURE_INFO_, &nom, &id, &posx, &posy, &race, &acces, &acces_suite);
		jeucomplet->jeu.perso[_PERSO_LOCAL_].data_creatures = msg;

		load_monstres(jeucomplet);
		liste_montre_ok =1;

		break;

	case _MESSAGE_CREATURE_MAJ_:

		if(liste_montre_ok==1){// on ne peux pas maj si on a pas recu la liste
			int id ,x ,y, o;
			int p;

			string MSG = outils::ctos(MSG_);
			int nbMonstre = atoi( MSG.substr(1,4).c_str() ) ;

			p = MSG.find_first_of("$"); // oncherche
			string bugg =  MSG.substr(0,3) ;
			MSG = MSG.substr(p+1, MSG.length());

			for(i = 0; i < nbMonstre ; i++)
			{
				p = MSG.find_first_of("!"); // oncherche
				string bugg =  MSG.substr(0,3) ;
				MSG = MSG.substr(p+1, MSG.length());

				p = MSG.find_first_of("!"); // oncherche
				id = atoi( MSG.substr(0,3).c_str() ) ;
				MSG = MSG.substr(p+1, MSG.length());

				p = MSG.find_first_of("!"); // oncherche
				x = atoi( MSG.substr(0,3).c_str() ) ;
				MSG = MSG.substr(p+1, MSG.length());

				p = MSG.find_first_of("!"); // oncherche
				y = atoi( MSG.substr(0,3).c_str() ) ;
				MSG = MSG.substr(p+1, MSG.length());

				p = MSG.find_first_of("!"); // oncherche
				o = atoi( MSG.substr(0,3).c_str() ) ;
				MSG = MSG.substr(p+1, MSG.length());

				MSG="!"+MSG;
				jeucomplet->jeu.monstre[id-1].animMonstre.dest.x=x;
				jeucomplet->jeu.monstre[id-1].animMonstre.dest.y=y;
				jeucomplet->jeu.monstre[id-1].animMonstre.orientation=o;
			}
		}else{
			cout<<"erreur liste non recu imposible de maj donc on renvoi une demande de liste ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"<<endl;
			string message = "C" + outils::charToString(_FIN_MESSAGE_);
			this->client_->envoyer_message(message);
		}	
		break;


	default:
		break;
	}
}

void partie::gerer_touches(JeuComplet *jeucomplet)
{
	SDL_Event event;
	Uint8 *keys;

	while(SDL_PollEvent(&event)) {
		switch(event.type){
		case SDL_QUIT:
			exit(1);
			break;
		}
	}
	keys = SDL_GetKeyState(NULL);
	if(keys[SDLK_ESCAPE])
		exit(1);


}

int partie::getIndicePerso(string id)
{
	int i;

	for(i=0 ; i<_MAXPERSO_ ; i++){
		if(this->perso_[i] && (this->perso_[i]->getId()==id))
			break;
	}

	if( i<_MAXPERSO_ )
		return(i);
	else
		return(-1);
}