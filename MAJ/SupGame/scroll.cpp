#include "scroll.hpp"

void scroll::load_scroll(JeuComplet *jeucomplet , int largeurscroll , int hauteurscroll)
{
    /*Charge les différentes variables du scroll*/
    jeucomplet->jeu.decor.scroll.hauteurScroll = hauteurscroll * 32;
    jeucomplet->jeu.decor.scroll.largeurScroll = largeurscroll * 32;
    jeucomplet->jeu.decor.scroll.xscroll = jeucomplet->jeu.decor.scroll.yscroll = 0;
}

void scroll::load_valeur_scrool(JeuComplet *jeucomplet)
{
    jeucomplet->jeu.decor.scroll.minx = jeucomplet->jeu.decor.scroll.xscroll / TILE_LG;
    jeucomplet->jeu.decor.scroll.miny = jeucomplet->jeu.decor.scroll.yscroll / TILE_HT;
    jeucomplet->jeu.decor.scroll.maxx = (jeucomplet->jeu.decor.scroll.xscroll + jeucomplet->jeu.decor.scroll.largeurScroll) / TILE_LG;
    jeucomplet->jeu.decor.scroll.maxy = (jeucomplet->jeu.decor.scroll.yscroll + jeucomplet->jeu.decor.scroll.hauteurScroll) / TILE_HT;
}



void scroll::windows_pos(JeuComplet *jeucomplet)
{
    /*Pour que le perso soit centré par rapport au scroll*/
	jeucomplet->jeu.decor.scroll.xscroll = jeucomplet->jeu.perso[_PERSO_LOCAL_].dest.x - (LARGEUR / 2);
    jeucomplet->jeu.decor.scroll.yscroll = jeucomplet->jeu.perso[_PERSO_LOCAL_].dest.y - (HAUTEUR / 2);

   bloquer_fenetre_scroll(jeucomplet);
}

void scroll::bloquer_fenetre_scroll(JeuComplet *jeucomplet)
{
     /*On veille à ce que le scroll ne sort pas*/
    if (jeucomplet->jeu.decor.scroll.xscroll + LARGEUR >= jeucomplet->jeu.decor.scroll.largeurScroll) jeucomplet->jeu.decor.scroll.xscroll = (jeucomplet->jeu.decor.scroll.largeurScroll) - LARGEUR;
	if (jeucomplet->jeu.decor.scroll.xscroll < 0) jeucomplet->jeu.decor.scroll.xscroll = 0;
	if (jeucomplet->jeu.decor.scroll.yscroll + HAUTEUR >= jeucomplet->jeu.decor.scroll.hauteurScroll) jeucomplet->jeu.decor.scroll.yscroll = (jeucomplet->jeu.decor.scroll.hauteurScroll) - HAUTEUR;
	if (jeucomplet->jeu.decor.scroll.yscroll < 0) jeucomplet->jeu.decor.scroll.yscroll = 0;
}
