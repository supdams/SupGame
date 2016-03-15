#ifndef SCROLL_HPP
#define SCROLL_HPP

#include "structs.hpp"
#include "perso.hpp"
#include "client.hpp"




class scroll
{
	public :
void load_scroll(JeuComplet *jeucomplet , int largeurscroll , int hauteurscroll);
void load_valeur_scrool(JeuComplet *jeucomplet);
void windows_pos(JeuComplet *jeucomplet);
void bloquer_fenetre_scroll(JeuComplet *jeucomplet);

		
	private:
		friend class perso;
		perso *perso_[_MAXPERSO_];

};
# endif