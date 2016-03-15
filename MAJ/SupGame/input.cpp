#include "input.hpp"


void test_touche(JeuComplet *jeucomplet)
{
	if (MODE_DEBUG == ON) {
		cout<<"test_touche"<<endl;
    }

    /*On teste si l'utilisateur appuis sur une touche , si oui on met la variable qui correspon à la touche */
    /*appuyée à 1 si la touche est relachée on met la variable correspondante à 0 . Ce qui sert pour la fonction update*/
    SDL_Event event;

	bool onclick = false;
	      




	jeucomplet->touche.mousebuttons[SDL_BUTTON_WHEELUP] = 0;
	jeucomplet->touche.mousebuttons[SDL_BUTTON_WHEELDOWN] = 0;

    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_QUIT :
			exit(1);
            break;
        case SDL_KEYDOWN :
            switch (event.key.keysym.sym)
            {
            case SDLK_RETURN :
                jeucomplet->touche.entrer = 1;
                break;
            case SDLK_SPACE :
                jeucomplet->touche.espace = 1;
                break;
            case SDLK_RIGHT :
                jeucomplet->touche.droite = 1;
                break;
            case SDLK_LEFT :
                jeucomplet->touche.gauche = 1;
                break;
            case SDLK_UP :
                jeucomplet->touche.haut = 1;
                break;
            case SDLK_DOWN:
                jeucomplet->touche.bas = 1;
                break;
            case SDLK_LSHIFT :
                jeucomplet->touche.lshift = 1;
                break;
             case SDLK_c :
                jeucomplet->touche.c = 1;
                break;
            default :
                ;
            }
            break;
        case SDL_KEYUP :
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE :
                jeucomplet->continuer = 0;
                break;
            case SDLK_RETURN :
                jeucomplet->touche.entrer = 0;
                break;
            case SDLK_RIGHT :
                jeucomplet->touche.droite = 0;
                break;
            case SDLK_LEFT :
                jeucomplet->touche.gauche = 0;
                break;
            case SDLK_UP :
                jeucomplet->touche.haut = 0;
                break;
            case SDLK_DOWN:
                jeucomplet->touche.bas = 0;
                break;
            case SDLK_SPACE :
                jeucomplet->touche.espace = 0;
                break;
            case SDLK_LSHIFT :
                jeucomplet->touche.lshift = 0;
                break;
            case SDLK_c :
                jeucomplet->touche.c = 0;
                break;
            default :
                break;
            }
            break;



        case SDL_MOUSEBUTTONDOWN :
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT :
                jeucomplet->touche.clic = 1;
                break;
            default :
                break;
            }
            break;
        case SDL_MOUSEBUTTONUP :
            jeucomplet->touche.clic = 0;
            break;
        case SDL_MOUSEMOTION:
            jeucomplet->touche.posSouris.x = event.motion.x; 
            jeucomplet->touche.posSouris.y = event.motion.y;
            break;
      
		
		
		
		
		
		
		
		
		default :
            break;

		
}
    
}

}






void getInput(void)
{
	if (MODE_DEBUG == ON) {
		cout<<"getInput"<<endl;
    }

	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{

			case SDL_QUIT:
				exit(0);
			break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						exit(0);
					break;

					case SDLK_DELETE:
						input.erase = 1;
					break;

					case SDLK_c:
						input.jump = 1;
					break;

				  	case SDLK_v:
						input.attack = 1;
					break;

				  	case SDLK_s:
						input.extra = 1;
					break;

					case SDLK_LEFT:
						input.left = 1;
					break;

					case SDLK_RIGHT:
						input.right = 1;
					break;

					case SDLK_DOWN:
						input.down = 1;
					break;

					case SDLK_UP:
						input.up = 1;
					break;

					case SDLK_RETURN:
						input.enter = 1;
					break;

					case SDLK_SPACE:
						input.espace = 1;
					break;

					default:

					break;
				}
			break;

			case SDL_KEYUP:
				switch (event.key.keysym.sym)
				{
					case SDLK_DELETE:
						input.erase = 0;
					break;

					case SDLK_c:
						input.jump = 0;
					break;
					
				  	case SDLK_v:
						input.attack = 0;
					break;

					case SDLK_LEFT:
						input.left = 0;
					break;

					case SDLK_RIGHT:
						input.right = 0;
					break;

					case SDLK_DOWN:
						input.down = 0;
					break;

					case SDLK_UP:
						input.up = 0;
					break;

				  	case SDLK_s:
						input.extra = 0;
					break;

					case SDLK_RETURN:
						input.enter = 0;
					break;

					case SDLK_SPACE:
						input.espace = 0;
					break;

					default:
					break;
				}
			break;



			case SDL_MOUSEBUTTONDOWN:
				switch(event.button.button)
				{
					/* Le clic gauche de la souris ajoutera la tile en cours */
					case SDL_BUTTON_LEFT:
						input.add = 1;
					break;

					/* Le clic droit de la souris copiera la tile sélectionnée */
					case SDL_BUTTON_RIGHT:
						input.copy = 1;
					break;

					/* La roue de la souris fera défiler les tiles */
					case SDL_BUTTON_WHEELUP:
						input.next = 1;
					break;

					case SDL_BUTTON_WHEELDOWN:
						input.previous = 1;
					break;

					default:
					break;
				}
			break;

			case SDL_MOUSEBUTTONUP:
				switch(event.button.button)
				{
					case SDL_BUTTON_LEFT:
						input.add = 0;
					break;

					case SDL_BUTTON_MIDDLE:
						input.remove = 0;
					break;

					default:
					break;
				}
			break;
		}
	}
	/* Enregistre les coordonnées de la souris */
	//SDL_GetMouseState(&input.mouseX, &input.mouseY);

	// Cette série d'opérations permet d'obtenir les coordonnées exactes de chaque
	//tile, tandis que la souris réagit au pixel près 

	//input.mouseX /= 32;
	//input.mouseY /= 32;

	//input.mouseX *= 32;
	//input.mouseY *= 32;
	



	//	printf("input.mouseX : %d\n", input.mouseX);
//		printf("input.mouseY: %d\n", input.mouseY);
}




