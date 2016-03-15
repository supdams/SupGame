#ifndef _SOUND_H_
#define _SOUND_H_ 1


/************************************************************************
La classe client permet de g�rer l'interface r�seaux du 'jeu'.
Elle contient les m�thodes pour se connecter au serveur et �changer 
des messages avec lui.
**************************************************************************/
class sound{
public:
sound();
~sound();
void playWAV(const char *wav);
void playMP3(const char *mp3);
};

#endif