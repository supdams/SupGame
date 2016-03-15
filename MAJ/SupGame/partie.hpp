// partie.hpp

#ifndef _PARTIE
#define _PARTIE

#include "constantes.hpp"

#include "perso.hpp"
#include "client.hpp"
#include "input.hpp"

#include "collision.hpp"
#include "map.hpp"



/*
G�re le d�roulement d'une partie.
*/
class partie
{
public :
	partie(JeuComplet *jeucomplet);
	~partie();
	void test_clic_monstre(JeuComplet *jeucomplet);
	void traitement_complet_message(JeuComplet *jeucomplet);
	void jouer(SDL_Surface *screen, bool pleinEcran, SDL_Surface *tmps, JeuComplet *jeucomplet);
	void traiter_messages(JeuComplet *jeucomplet);
	void animer(JeuComplet *jeucomplet);
	void gerer_touches(JeuComplet *jeucomplet);

	void choix_perso(JeuComplet *jeucomplet);
	void crer_perso(JeuComplet *jeucomplet);

	void load_anim_perso(JeuComplet *jeucomplet, int p);

	void logger(string SPseudochaine, string SMdpchaine, JeuComplet *jeucomplet);
	void connecte_perso(string guide, JeuComplet *jeucomplet);
	void creation_perso(string SPseudochaine, JeuComplet *jeucomplet);

	int logger_test(JeuComplet *jeucomplet);
	int creation_test(JeuComplet *jeucomplet);
	int getIndicePerso(string id);

	perso *perso_[_MAXPERSO_];
private:
	bool pleinEcran_;
	client * client_;
	int liste_montre_ok ;

};
extern TTF_Font *font[8];


void update_perso(JeuComplet *jeucomplet, int p);
void update_frame_perso(JeuComplet *jeucomplet , Touche *touche , Anim *anim)/*Le h�ros � une fonction d'avancement de frame, c'est plus g�rable*/;
void update_orientation(JeuComplet *jeucomplet , Touche *touche , Anim *anim);
void update_orientation_anim(JeuComplet *jeucomplet , Touche *touche , Anim *anim , int dimensionY);

void update_touche_perso(JeuComplet *jeucomplet);
void update_vector(JeuComplet *jeucomplet , int *vx , int *vy);






#endif